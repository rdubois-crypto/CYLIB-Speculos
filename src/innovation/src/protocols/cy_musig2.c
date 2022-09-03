/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois - This file is part of cy_lib project		  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_hybridsig.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: 2 round_multisignature signatures source code
 * https://eprint.iacr.org/2020/1261.pdf             */
/*	*/
/**********************************************************************************/
#include "cy_fp.h"
#include "cy_ec.h"
#include "cy_hash.h"
#include "cy_musig2.h"


/* Initialize Musig with Signatures functions, number of users, hash functions */
cy_error_t cy_musig_SetUp(cy_musig2_ctx_t *ctx,  uint8_t *Ramp, size_t sizeRamp,
		  uint8_t *initializer, size_t init_t,
        const cy_hash_ctx_t *H){

	cy_error_t error=CY_KO;



	end:
			return error;
}
/*************************************************************/
/* Convert signature into Hash to prime field*/
/*************************************************************/
static cy_error_t cy_musig_KeyAggCoeff(cy_musig2_ctx_t *ctx, const cy_ec_point_t *publickeys, size_t n_users, size_t index,
		cy_fp_t *ai)
{
	uint8_t buffer[MAX_MUSIG_EC_T8];
	cy_error_t error=CY_KO;

	/*H_agg is chosen to be H(X1||X2||\ldots||X_i), where || denotes concatenation */
	for(i=0;i<n_users;i++){
		CY_CHECK(cy_ec_export(&ctx->ctx_ec->ctx_fp_q, publickeys+i,ctx->ctx_ec->t8_modular_p,buffer));
		ctx->H->Hash_Update((void *)ctx->H, buffer, ctx->ctx_ec->t8_modular_p);
	}
	CY_CHECK(cy_ec_export(&ctx->ctx_ec->ctx_fp_q, publickeys+index,ctx->ctx_ec->t8_modular_p,buffer));
	ctx->H->Hash_Update((void *)ctx->H, buffer, ctx->ctx_ec->t8_modular_p);

	/*import hash of hash function size to Fq scalar field, assumption is that hash output has same bitsize as field*/
	/* todo: make modular reduction of buffer */
	CY_CHECK(cy_fp_import(&ctx->ctx_ec->ctx_fp_q, buffer,ctx->H->Hash_t8,ai));

	end:
		return error;
}

/*************************************************************/
/* Single user functions*/
/*************************************************************/

/* Each signer generates a random secret key x ←$ Zp and returns the
corresponding public key X= g^x*/
cy_error_t cy_musig_KeyGen(cy_musig2_ctx_t *ctx, const size_t n_users, cy_ec_point_t *X_pub)
{
	/*I.Declarations and allocations*/
	cy_error_t error=CY_KO;
	cy_fp_t x;
	cy_ecpoint_t *G=cy_ec_get_generator(ctx->ctx_ec); /* get generating point of the curve , todo ec: coder un get_generator */
	CY_CHECK(cy_fp_alloc(&x, ctx->ctx_ec->ctx_fp_p->t8_modular));

	/*II. Computations*/
	/*generates a random secret key x ←$ Zp*/
	CY_CHECK(cy_fp_get_random(ctx->gda, &x));
	/* returns the corresponding public key X=g^x*/
	CY_CHECK(cy_ec_scalarmult_fp(G, &x, X_pub));

	/*III. Free pointers*/
	CY_CHECK(cy_fp_free(&x));

	end:
   	  return error;
}

/* generation of user's second round key from aggregated Round1 key L*/
cy_error_t cy_musig_KeyGen_prime(cy_musig2_ctx_t *ctx, const size_t n_users, cy_ec_point_t *X_pub)
{
	cy_error_t error=CY_KO;

	end:
	   	  return error;
}


cy_error_t cy_musig_Sign_Round1_all(const cy_musig2_ctx_t *ctx,const size_t n_users,  const size_t index, cy_ec_point_t *Rijs_Round1)
{
	cy_error_t error=CY_KO;
	size_t i;
	cy_fp_t x;
	cy_ecpoint_t *G=cy_ec_get_generator(ctx->ctx_ec); /* get generating point of the curve , todo ec: coder un get_generator */

	CY_CHECK(cy_fp_alloc(&x, ctx->ctx_ec->ctx_fp_p->t8_modular));
	/* For each j ∈ {1, . . . , ν}*/
	for(i=0;i<n_users; i++)
	{
		/*generates a random nonce key ri,j ←$ Zp*/
		CY_CHECK(cy_fp_get_random(ctx->gda, &x));
		/*computes R1,j =g^r1,j*/
		CY_CHECK(cy_ec_scalarmult_fp(G, &x, Rijs_Round1+i));
	}

	CY_CHECK(cy_fp_free(&x));

	end:
   	  return error;
}


cy_error_t cy_musig_Sign_Round2_all(const cy_musig2_ctx_t *ctx,const size_t n_users,  const size_t index, cy_ec_point_t *Rijs_Round1)
{
	cy_error_t error=CY_KO;

	end:
	   	  return error;
}
/*************************************************************/
/* Aggregator functions*/
/*************************************************************/
/*  aggregate key corresponding to L is X=Prod_1^n X^ai */
cy_error_t cy_musig_KeyAgg(cy_musig2_ctx_t *ctx, const size_t n_users, const cy_ec_point_t *publickeys, cy_ec_point_t *keyagg){
	size_t i;
	cy_ecpoint_t temp;
	cy_fp_t ai;/*H(L,Xi)*/

	cy_fp_alloc(&ctx->ctx_ec->ctx_fp_p, &ai)
	cy_ec_alloc(ctx->ctx_ec, &temp);
	cy_ecpoint_t *G=cy_ec_get_generator(ctx->ctx_ec); /* get generating point of the curve , todo ec: coder un get_generator */


	cy_error_t error=CY_KO;

	/* set accumulator to infinity point*/
	CY_CHECK(cy_ec_setInfty(ctx->ctx_ec, &temp));
	/* compute sum of X^H(L,Xi)*/
	for(i=0;i<n_users;i++)
	{

		cy_musig_HashToScalarField(ctx, publickeys, n_users, i, &ai);
		CY_CHECK(cy_ec_scalarmult_fp(G,  &ai, temp));
		CY_CHECK(cy_ec_add(&temp, publickeys+i, &temp));
	}

	cy_ec_copy(ctx->ctx_ec, &temp, &keyagg);
	cy_ec_free(&temp);
	cy_fp_free(&ai);

	end:
		return error;
}

cy_error_t cy_musig_SigAgg_Round1(const size_t n_users, const cy_ec_point_t *sig, cy_ec_point_t *sigagg){
	size_t i;
	cy_ec_point_t keyagg;
	cy_error_t error=CY_KO;

	/* copy first pubkey in res*/
	CY_CHECK(cy_ec_copy(sigagg, sig));
	/* compute sum of public keys*/
	for(i=0;i<n_users;i++)
	{
		CY_CHECK(cy_ec_add(keyagg, sig+i, keyagg));
	}
	end:
		return error;
}





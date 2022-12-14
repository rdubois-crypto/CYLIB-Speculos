/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois - This file is part of cy_lib project		  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_hybridsig.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: 2 round_multisignature Musig2 signatures source code */
/* https://eprint.iacr.org/2020/1261.pdf             */
/* note that some constant aggregating values could be precomputed	*/
/**********************************************************************************/
#include <stdint.h>
#include <stddef.h>

#include "cy_configuration.h"
#include "cy_bn.h"

#include "cy_fp.h"
#include "cy_ec.h"
#include "cy_hash.h"
#include "cy_musig2.h"


#include <stdio.h>
#include "cy_io_common_tools.h"
#include "cy_io_fp.h"
#include "cy_io_ec.h"


/*************************************************************/
/* Common to user/Agregator functions*/
/*************************************************************/

/*************************************************************/
/* Computation of c=H(X, R,m)*/
/*************************************************************/

/* note that the Hsig is xonly compatible, only X_x and Rx  are aggregated to the sig*/
cy_error_t cy_Hsig(const cy_musig2_ctx_t *musig_ctx, const cy_ecpoint_t *KeyAg, const cy_ecpoint_t *R, const uint8_t *m, const size_t t8_m, uint8_t *tu8_c)
{
	 cy_error_t error=CY_OK;
	 uint8_t zero[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	 uint8_t separation[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,_SEPARATION_SIG};
	 cy_hash_unit_t *H=musig_ctx->H;
     uint8_t buffer[2*MAX_MUSIG_EC_T8]; /* can store a x,y uncompressed point */

	 size_t t8_modular=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

	 CY_CHECK(H->Hash_Init(H->ctx, zero, 1));
	 CY_CHECK(H->Hash_Update(H->ctx, separation, sizeof(separation)));
	 CY_CHECK(cy_ec_export(KeyAg, buffer, t8_modular));
	 CY_CHECK(H->Hash_Update(H->ctx, buffer, t8_modular)); /* only taking half of the buffer on purpose : x coordinate*/

	 CY_CHECK(cy_ec_export(R, buffer, t8_modular));
	 CY_CHECK(H->Hash_Update(H->ctx, buffer, t8_modular)); /* only taking half of the buffer on purpose : x coordinate*/
	 CY_CHECK(H->Hash_Update(H->ctx, m, t8_m));
	 CY_CHECK(H->Hash_Final(H->ctx, tu8_c, t8_modular));

	 end:
	   	return error;
}

/*************************************************************/
/* Computation of b=H(X, R1, ..., Rv, m)*/
/*************************************************************/

cy_error_t cy_Hnon(const cy_musig2_ctx_t *musig_ctx, const cy_ecpoint_t *XAgg, const cy_ecpoint_t *R,  const uint8_t *m, const size_t t8_m, uint8_t *b)
{
	  size_t j;
	  cy_error_t error=CY_OK;
	  uint8_t zero[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	  cy_hash_unit_t *H=musig_ctx->H;

	  uint8_t buffer[2*MAX_MUSIG_EC_T8];
	  uint8_t separation[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,_SEPARATION_NON};

	  size_t t8_buffer=2*musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

	  size_t t8_modular=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

	  CY_CHECK(H->Hash_Init(H->ctx, zero, 1));
	  CY_CHECK(H->Hash_Update(H->ctx, separation, sizeof(separation)));
	  CY_CHECK(cy_ec_export(XAgg, buffer, t8_modular));
	  CY_CHECK(H->Hash_Update(H->ctx, buffer, t8_buffer));

	  for(j=0;j<_MU_;j++)
	  	  {

		  	  CY_CHECK(cy_ec_export(R+j, buffer, t8_modular));
		  	  CY_CHECK(H->Hash_Update(H->ctx, buffer, t8_buffer));

		  }
	  CY_CHECK(H->Hash_Update(H->ctx, m, t8_m));

	  CY_CHECK(H->Hash_Final(H->ctx, b, musig_ctx->ctx_ec->ctx_fp_p->t8_modular));
	  end:
	  	return error;
}
/*************************************************************/
/* Computation of ai=H(L, X_i)*/
/*************************************************************/
cy_error_t cy_Hagg(const cy_musig2_ctx_t *musig_ctx, const cy_ecpoint_t *L, int index_i, uint8_t *ai)
{
	  size_t i;
	  cy_error_t error=CY_OK;
	  uint8_t zero[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	  cy_hash_unit_t *H=musig_ctx->H;

	  uint8_t buffer[2*MAX_MUSIG_EC_T8];
	  uint8_t separation[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,_SEPARATION_AGG};

	  size_t t8_buffer=2*musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
	  size_t t8_modular=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

	  CY_CHECK(H->Hash_Init(H->ctx, zero, 1));
	  CY_CHECK(H->Hash_Update(H->ctx, separation, sizeof(separation)));

	  for(i=0;i<musig_ctx->n_users;i++)
	  {
		  CY_CHECK(cy_ec_export(&L[i], buffer, t8_modular));
		  CY_CHECK(H->Hash_Update(H->ctx, buffer, t8_buffer));
	  }

	  CY_CHECK(cy_ec_export(&L[index_i], buffer, t8_modular));
	  CY_CHECK(H->Hash_Update(H->ctx, buffer, t8_buffer));
	  CY_CHECK(H->Hash_Final(H->ctx, ai, musig_ctx->ctx_ec->ctx_fp_p->t8_modular));

	  end:
	  	return error;
}
/*************************************************************/
/* Initializing the Musig context						 	*/
/*************************************************************/
cy_error_t cy_musig_SetUp( cy_hash_unit_t *H, uint8_t *Elliptic_Ramp, size_t Elliptic_Ramp_t8, int curve,  int n_users,
							   cy_ec_ctx_t *ec_ctx, cy_musig2_ctx_t *musig_ctx){

    cy_error_t error=CY_OK;

    CY_CHECK(cy_ec_init(ec_ctx, Elliptic_Ramp,Elliptic_Ramp_t8, curve, NULL));
   	musig_ctx->ctx_ec=ec_ctx;

    CY_CHECK(cy_ec_get_curveorder(ec_ctx, musig_ctx->order, ec_ctx->ctx_fp_p->t8_modular));

   	musig_ctx->H=H;
   	musig_ctx->n_users=n_users;
    CY_CHECK(H->Hash_Configure( H, (uint8_t *) musig_ctx->ctx_ec, _UNUSED_VALUE_));

   	end:
		return error;
}
/*************************************************************/
/* Freeing the Musig context							 	*/
/*************************************************************/
cy_error_t cy_musig_uninit( cy_musig2_ctx_t *musig_ctx)
{
    cy_error_t error=CY_KO;
    cy_hash_unit_t *H=musig_ctx->H;
    CY_CHECK(H->Hash_Uninit(H->ctx));

    CY_CHECK(cy_ec_uninit(musig_ctx->ctx_ec));

	end:
		return error;
}

/*************************************************************/
/* Single user functions*/
/*************************************************************/
/*
Derivate with B340 constraint (y even) value of pubkey
*/
cy_error_t cy_musig_KeyGenDeriv(const cy_musig2_ctx_t *ctx, const cy_fp_t *random,
								cy_fp_t *xpriv, cy_ecpoint_t *X_pub)
{
	/*I.Declarations and allocations*/
	cy_error_t error=CY_KO;
	cy_fp_t order;
	uint32_t sign=0;
	cy_ec_ctx_t  *ec_ctx= ctx->ctx_ec;
	cy_ecpoint_t G;
	cy_ec_alloc(ctx->ctx_ec, &G);

	/* nope, tis fq*/
	CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &order));
	CY_CHECK(cy_fp_import(ctx->order, ec_ctx->ctx_fp_p->t8_modular, &order));

	/* reduction mod q of random */
	CY_CHECK(cy_bn_mod(random->bn, order.bn, random->bn));
	CY_CHECK(cy_ec_get_generator(ctx->ctx_ec, &G));

	CY_CHECK(cy_ec_scalarmult_fp(random, &G, X_pub));
	/* compatibility BIP340*/
	CY_CHECK(cy_ec_getparityY(X_pub, &sign));

	if(sign==1)
	{

		CY_CHECK(cy_fp_sub( &order, random, xpriv));
		CY_CHECK(cy_ec_scalarmult_fp(xpriv, &G,  X_pub));
	}
	else
	{
		cy_fp_copy(random, xpriv);
	}

	/*III. Free pointers*/
	CY_CHECK(cy_fp_free(&order));

	cy_ec_free(&G);

	end:
   	  return error;
}

cy_error_t cy_musig_KeyGen(const cy_musig2_ctx_t *ctx, cy_fp_t *xpriv, cy_ecpoint_t *X_pub)
{
 uint8_t buffer[_MAX_FP_T8];
 cy_error_t error;

 CY_CHECK(ctx->gda->GDA_Run(ctx->gda, buffer, ctx->ctx_ec->ctx_fp_p->t8_modular));

 CY_CHECK(cy_fp_import(buffer, ctx->ctx_ec->ctx_fp_p->t8_modular, xpriv));

 CY_CHECK(cy_musig_KeyGenDeriv(ctx, xpriv, xpriv, X_pub));
 end:
 	  return error;
}


/* Signature round 1*/
/* beware that sign of random will be modified is derived y is even*/
cy_error_t cy_musig_Sign1_Deriv(const cy_musig2_ctx_t *ctx, cy_fp_t io_random[_MU_],
								 cy_ecpoint_t Ri[_MU_])
{
	/*I.Declarations and allocations*/
	cy_error_t error=CY_KO;
	 size_t j;

	/* returns the corresponding public ephemeral X=g^x*/
	for(j=0;j<_MU_;j++)
	{
			CY_CHECK(cy_musig_KeyGenDeriv(ctx, &io_random[j], &io_random[j], &Ri[j]));
	}

	end:
   	  return error;
}


cy_error_t cy_musig_Sign1(const cy_musig2_ctx_t *ctx, cy_fp_t ri[_MU_],
								 cy_ecpoint_t Ri[_MU_])
{
	/*I.Declarations and allocations*/
	cy_error_t error=CY_KO;
    uint8_t buffer[_MAX_FP_T8];
    size_t j;

    /* generate the private nonces using random generator*/
    for(j=0;j<_MU_;j++)
    {
	 CY_CHECK(ctx->gda->GDA_Run(ctx->gda, buffer, ctx->ctx_ec->ctx_fp_p->t8_modular));
	  /* todo: reduction mod q */
	 CY_CHECK(cy_fp_import(buffer, ctx->ctx_ec->ctx_fp_p->t8_modular, &ri[j]));
    }
    CY_CHECK(cy_musig_Sign1_Deriv(ctx, ri, Ri));

	end:
	   	  return error;
}

/* Sign' function of paper*/
/* ai is computed externally using cy_musig_KeyAggCoeff*/
/* s_i is the final round two ouput for user i*/
/* vec_sigagg is the aggregated output signature of round1 */
/* todo later: to reduce memory consumption, gda state for nonces may be saved*/
cy_error_t cy_musig_Sign_Round2_all(const cy_musig2_ctx_t *musig_ctx,
		const uint8_t *ai,
		const cy_fp_t *privatekey_xi,
		const cy_ecpoint_t *R1,/* aggregated Ri from Round 1*/
		const cy_ecpoint_t *XAgg,
		const uint8_t tu8_ri[_MU_][_MAX_CYLIB_EC_T8],
		const uint8_t *message, const size_t t8_message,
		cy_ecpoint_t *R,
		uint8_t *tu8_s,
		uint8_t *tu8_c
		)
{
	cy_error_t error=CY_KO;
	size_t j;
	cy_ecpoint_t ec_temp ;
	cy_fp_t fp_b, fp_ai, acc_ribj, fp_temp, fp_c, fp_ri[_MU_];
	cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
	size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
	cy_fp_ctx_t *fq_ctx=musig_ctx->ctx_ec->ctx_fp_p; /* note that this is fp ctx, currently cheating using bn internal type */
	uint8_t buffer[_MAX_CYLIB_EC_T8];
	cy_fp_t order;

	/* allocations*/
	cy_ec_alloc(ec_ctx, &ec_temp);
	cy_fp_alloc(fq_ctx, t8_fp, &acc_ribj);
	cy_fp_alloc(fq_ctx, t8_fp, &fp_ai);
	cy_fp_alloc(fq_ctx, t8_fp, &fp_b);
	cy_fp_alloc(fq_ctx, t8_fp, &fp_c);
	cy_fp_alloc(fq_ctx, t8_fp, &fp_temp);
	cy_fp_alloc(fq_ctx, t8_fp, &order);

	for(j=0;j<_MU_;j++)
	{
		cy_fp_alloc(fq_ctx, t8_fp, &fp_ri[j]);
	}

	/* deserialize input */
	CY_CHECK(cy_fp_import(ai, t8_fp, &fp_ai));

	CY_CHECK(cy_fp_import(musig_ctx->order, ec_ctx->ctx_fp_p->t8_modular, &order));
	for(j=0;j<_MU_;j++){
		CY_CHECK(cy_fp_import(tu8_ri[j], t8_fp, &fp_ri[j]));
	}

	/*Hnon(X, (R1. . . , R??	), m)*/
	CY_CHECK(cy_Hnon(musig_ctx, XAgg, R1,  message, t8_message, buffer));/*Hnon(X, (R1. . . , R??	), m)*/
	CY_CHECK(cy_fp_import(buffer, t8_fp, &fp_b));

	/*Accumulate R_j^(b^j-1) in R*/
	CY_CHECK(cy_ec_copy(&R1[0], R)); /*R=RO^bO=R0*/

	CY_CHECK(cy_fp_copy(fp_ri, &acc_ribj)); /*R=RO^bO=R0*/

	for(j=1;j<_MU_;j++)
	{
		CY_CHECK(cy_ec_scalarmult_fp(&fp_b, &R1[j], &ec_temp));
		CY_CHECK(cy_ec_add( R, &ec_temp, R));
		//CY_CHECK(cy_bn_mod_mul(fp_ri[j].bn, fp_b.bn, order.bn, fp_temp.bn));
		CY_CHECK(cy_fp_mul(&fp_ri[j], &fp_b,  &fp_temp));

		//CY_CHECK(cy_bn_mod_add(acc_ribj.bn, fp_temp.bn, order.bn, acc_ribj.bn));
		CY_CHECK(cy_fp_add(&acc_ribj, &fp_temp, & acc_ribj));

		//CY_CHECK(cy_bn_mod_mul(fp_b.bn, fp_b.bn, order.bn, fp_b.bn));
		CY_CHECK(cy_fp_mul(&fp_b, &fp_b, & fp_b));

	}

	/* Hsig(X, R, m)*/
	CY_CHECK(cy_Hsig(musig_ctx, XAgg, R, message, t8_message, tu8_c));
	CY_CHECK(cy_fp_import( tu8_c, t8_fp, &fp_c));
	CY_CHECK(cy_fp_import( ai, t8_fp, &fp_ai));

	//CY_CHECK(cy_bn_mod_mul(fp_c.bn, fp_ai.bn, order.bn, fp_temp.bn));				/* c.ai*/
	CY_CHECK(cy_fp_mul(&fp_c, &fp_ai, &fp_temp));
	//CY_CHECK(cy_bn_mod_mul( fp_temp.bn, privatekey_xi->bn, order.bn, fp_temp.bn));	/* (c*ai*privatekey_xi)*/
	CY_CHECK(cy_fp_mul( &fp_temp, privatekey_xi, &fp_temp));	/* (c*ai*privatekey_xi)*/

	CY_CHECK(cy_fp_add( &fp_temp, &acc_ribj,  &fp_temp));		/*s1= ca1x1 + sum(r1jb^j)*/

	CY_CHECK(cy_fp_export(&fp_temp, tu8_s, t8_fp));

	/* free*/
	CY_CHECK(cy_ec_free( &ec_temp));
	CY_CHECK(cy_fp_free(&  fp_ai));
	CY_CHECK(cy_fp_free(& fp_b));
	CY_CHECK(cy_fp_free(& fp_c));
	CY_CHECK(cy_fp_free(& fp_temp));
	CY_CHECK(cy_fp_free( &order));
	CY_CHECK(cy_fp_free(&acc_ribj));
	for(j=0;j<_MU_;j++)
		{
			cy_fp_free(&fp_ri[j]);
		}
	end:
	   	  return error;
}
/*************************************************************/
/* Aggregator functions*/
/*************************************************************/
/*  aggregate key corresponding to L is X=Prod_1^n X^ai */

// L concatenation of public keys
cy_error_t cy_musig_KeyAgg(cy_musig2_ctx_t *musig_ctx, const cy_ecpoint_t *ec_L, cy_ecpoint_t *KeyAgg, uint8_t  svg_ai[][_MAX_CYLIB_EC_T8])
{
  cy_ecpoint_t ec_temp, accumulator;
  cy_error_t error;
  cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
  size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

 // uint8_t buffer[2*MAX_MUSIG_EC_T8];
  cy_fp_t ai;

  size_t i;
  UNUSED(svg_ai);

  CY_CHECK(cy_ec_alloc(ec_ctx, &ec_temp));
  CY_CHECK(cy_ec_alloc(ec_ctx, &accumulator));

  CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_fp, &ai));

  CY_CHECK(cy_ec_setinfinity(&accumulator));

  for(i=0;i<musig_ctx->n_users;i++){
	  CY_CHECK(cy_Hagg(musig_ctx, ec_L, i, svg_ai[i]));
	  CY_CHECK(cy_fp_import( svg_ai[i], t8_fp, &ai));

	  CY_CHECK(cy_ec_scalarmult_fp(&ai, &ec_L[i], &ec_temp));
	  CY_CHECK(cy_ec_add(&accumulator, &ec_temp, &accumulator));
  }

  CY_CHECK(cy_ec_copy(&accumulator, KeyAgg));

  CY_CHECK(cy_ec_free( &accumulator));
  CY_CHECK(cy_ec_free( &ec_temp));
  CY_CHECK(cy_fp_free( &ai));

  end:
   	return error;
}



/* same but using serialized representation of ephemeral points to spare memory in crypto accelerator*/
cy_error_t cy_musig_Sign1_tu8_Agg(const cy_musig2_ctx_t *musig_ctx,
		  uint8_t  (svg_Rij[][_MU_])[_MAX_CYLIB_EC_T8],
		  cy_ecpoint_t *R)
{
	size_t i,j;
	cy_error_t error=CY_KO;
	cy_ecpoint_t ec_temp;
	size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

	CY_CHECK(cy_ec_alloc(musig_ctx->ctx_ec, &ec_temp));
	for(j=0;j<_MU_;j++)
		{
			CY_CHECK(cy_ec_setinfinity(&R[j]));
			for(i=0;i<musig_ctx->n_users;i++)
			{
				CY_CHECK(cy_ec_import(svg_Rij[i][j], t8_fp, &ec_temp));
				 CY_CHECK(cy_ec_add(&ec_temp, &R[j], &R[j]));
			}
		}
	CY_CHECK(cy_ec_free( &ec_temp));

	end:
	   	  return error;
}

cy_error_t cy_musig_Sign2_Agg_tu8(const cy_musig2_ctx_t *musig_ctx, uint8_t s_i[][MAX_MUSIG_EC_T8], uint8_t *tu8_s)
{
	size_t i;
	cy_error_t error;
	size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

	cy_fp_t fp_temp, fp_s, fp_order;
	CY_CHECK(cy_fp_alloc(musig_ctx->ctx_ec->ctx_fp_p, t8_fp,&fp_temp));
	CY_CHECK(cy_fp_alloc(musig_ctx->ctx_ec->ctx_fp_p, t8_fp,&fp_s));

	CY_CHECK(cy_fp_alloc(musig_ctx->ctx_ec->ctx_fp_p, t8_fp, &fp_order));

	CY_CHECK(cy_fp_import(s_i[0], t8_fp, &fp_s));
	CY_CHECK(cy_fp_import(musig_ctx->order, musig_ctx->ctx_ec->ctx_fp_p->t8_modular, &fp_order));

	for(i=1;i<musig_ctx->n_users;i++)
	{
		CY_CHECK(cy_fp_import(s_i[i], t8_fp, &fp_temp));
		//CY_CHECK(cy_bn_mod_add(fp_temp.bn, fp_s.bn, fp_order.bn, fp_s.bn));
		CY_CHECK(cy_fp_add(&fp_temp, &fp_s, & fp_s));
	}

	CY_CHECK(cy_fp_export(&fp_s, tu8_s, t8_fp));

	CY_CHECK(cy_fp_free(&fp_temp));
	CY_CHECK(cy_fp_free(&fp_s));
	CY_CHECK(cy_fp_free(&fp_order));

	end:
   	  return error;
}

/*************************************************************/
/* Verification functions								 */
/*************************************************************/

/* Ouput : the two part of signatures (R,s) received from previous round, */
/* R and c being computable from aggregation of Rijs_Round1, s being output of Round2 sign'*/


/* core function with external hashing */
cy_error_t cy_musig_Verification_Core(cy_musig2_ctx_t *ctx, cy_ecpoint_t *Key_agg,
		cy_ecpoint_t *R, cy_fp_t *fp_s, cy_fp_t *fp_c,
		boolean_t *flag_verif){

	cy_error_t error=CY_KO;
	*flag_verif=CY_FALSE;
	cy_ecpoint_t ec_temp1, ec_temp2, G;

	/* allocating */
	CY_CHECK(cy_ec_alloc(ctx->ctx_ec, &ec_temp1));
	CY_CHECK(cy_ec_alloc(ctx->ctx_ec, &ec_temp2));
	CY_CHECK(cy_ec_alloc(ctx->ctx_ec, &G));

	/* Accept iff g^s = R.X^c, beware of multiplicative notations*/
	CY_CHECK(cy_ec_get_generator(ctx->ctx_ec, &G));
	CY_CHECK(cy_ec_scalarmult_fp(fp_s, &G, &ec_temp1)); 	/*g^s*/
	CY_CHECK(cy_ec_scalarmult_fp(fp_c, Key_agg,  &ec_temp2)); 	/*X^c*/
	CY_CHECK(cy_ec_add( &ec_temp2, R, &ec_temp2)); 	/*R.X^c*/

	/* final verification */
	CY_CHECK(cy_ec_iseq( &ec_temp1, &ec_temp2, 	flag_verif));

	/* freeing */
	CY_CHECK(cy_ec_free( &ec_temp1));
	CY_CHECK(cy_ec_free( &ec_temp2));
	CY_CHECK(cy_ec_free( &G));

	end:
		return error;
}


/* one shot hashing full verification function with internal hashing */
cy_error_t cy_musig_Verification_All(cy_musig2_ctx_t *ctx, cy_ecpoint_t *Key_agg,
		cy_ecpoint_t *R, cy_fp_t *fp_s, uint8_t *message, size_t t8_message,
		boolean_t *flag_verif){

	cy_error_t error=CY_KO;
	cy_fp_t fp_c;
	uint8_t u8_c[MAX_MUSIG_EC_T8];
	size_t t8_fp=ctx->ctx_ec->ctx_fp_p->t8_modular;

	*flag_verif=CY_FALSE;
	CY_CHECK(cy_fp_alloc(ctx->ctx_ec->ctx_fp_p, t8_fp,&fp_c));

	CY_CHECK(cy_Hsig(ctx, Key_agg, R, message, t8_message, u8_c));
	CY_CHECK(cy_fp_import(u8_c, t8_fp, &fp_c));

	CY_CHECK(cy_musig_Verification_Core(ctx, Key_agg, R, fp_s, &fp_c, flag_verif));

	CY_CHECK(cy_fp_free( &fp_c));
	end:
		return error;
}

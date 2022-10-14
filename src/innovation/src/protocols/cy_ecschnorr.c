
/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois - This file is part of cy_lib project		  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_ecschnorr.c										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Schnorr internal verification fonction with externalized hashing  */
/* https://en.wikipedia.org/wiki/Schnorr_signature    							  */
/**********************************************************************************/
#include <stdint.h>
#include <stddef.h>

#include "cy_configuration.h"
#include "cy_fp.h"
#include "cy_ec.h"
#include "cy_hash.h"
#include "cy_ecschnorr.h"

#define _SCHNORR_SIGNER 0
#define _SCHNORR_VERIFIER 1

struct cy_schnorr_s {
  int state;
  int role;

  cy_ec_ctx_t ctx_ec;
  cy_hash_unit_t *H;
  cy_gda_ctx_t *gda;

  uint8_t pubkey[MAX_BOLOS_EC_T8];
  uint8_t seckey[MAX_BOLOS_EC_T8];
  uint8_t *RAM;
  size_t t8_RAM;
};

typedef struct cy_schnorr_s cy_schnorr_t;

typedef struct cy_FILE_s cy_FILE_t;

cy_error_t cy_Schnorr_Init(cy_schnorr_t *ctx_schnorr, cy_hash_unit_t *H, cy_ec_ctx_t *ctx_ec,  cy_gda_ctx_t *gda, uint8_t *RAM, size_t t8_RAM, int role)
{
	cy_error_t error=CY_KO;

	ctx_schnorr->H=H;
	ctx_schnorr->gda=gda;
	ctx_schnorr->role=role;

	end:
			return error;

}


/*************************************************************/
/* Single user functions*/
/*************************************************************/
/*
Derivate with B340 constraint (y even) value of pubkey
*/
cy_error_t cy_Schnorr_KeyGenDeriv(const cy_schnorr_t *ctx, const cy_fp_t *random,
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
	//CY_CHECK(cy_bn_mod(random->bn, order.bn, random->bn));
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

cy_error_t cy_Schnorr_KeyGen(const cy_schnorr_t *ctx, cy_fp_t *xpriv, cy_ecpoint_t *X_pub)
{
 uint8_t buffer[_MAX_FP_T8];
 cy_error_t error;

 CY_CHECK(ctx->gda->GDA_Run(ctx->gda, buffer, ctx->ctx_ec->ctx_fp_p->t8_modular));

 CY_CHECK(cy_fp_import(buffer, ctx->ctx_ec->ctx_fp_p->t8_modular, xpriv));

 CY_CHECK(cy_musig_KeyGenDeriv(ctx, xpriv, xpriv, X_pub));
 end:
 	  return error;
}

/* one shot signing */
cy_error_t cy_Schnorr_Sign(cy_hash_unit_t *H, cy_ec_ctx_t *ctx_ec, cy_fp_t *xpriv, uint8_t *message, size_t t8_message, uint8_t *R, uint8_t *fp_s)
{
	cy_error_t error=CY_KO;


	end:
		return error;
}

/* core function with external hashing, c=H(R||M) */
cy_error_t cy_Schnorr_Verification_Core(cy_ec_ctx_t *ctx_ec, cy_ecpoint_t *Key_agg,
		cy_ecpoint_t *R, cy_fp_t *fp_s, cy_fp_t *fp_c,
		boolean_t *flag_verif){

	cy_error_t error=CY_KO;
	*flag_verif=CY_FALSE;
	cy_ecpoint_t ec_temp1, ec_temp2, G;

	/* allocating */
	CY_CHECK(cy_ec_alloc(ctx_ec, &ec_temp1));
	CY_CHECK(cy_ec_alloc(ctx_ec, &ec_temp2));
	CY_CHECK(cy_ec_alloc(ctx_ec, &G));

	/* Accept iff g^s = R.X^c, beware of multiplicative notations*/
	CY_CHECK(cy_ec_get_generator(ctx_ec, &G));
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


/* core function with external hashing, c=H(R||M) */
cy_error_t cy_Schnorr_Verification_All(cy_hash_unit_t *H, cy_ec_ctx_t *ctx_ec, cy_ecpoint_t *Key_agg,
		uint8_t *message, size_t t8_message,
		uint8_t *ec_R, uint8_t *fp_s,
		boolean_t *flag_verif)
{
	cy_error_t error=CY_KO;

	end:
			return error;
}

cy_error_t cy_Schnorr_UnInit(cy_schnorr_t *ctx_schnorr)
{



}


/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: test_cy_test_musig2.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: unitary testing of musig2 functions */
/**********************************************************************************/
#include <stdio.h>
#include <malloc.h>
#include <setjmp.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "cy_errors.h"


#include "cy_configuration.h"
#include "cy_errors.h"
#include "cy_fp.h"

#include "cy_ec.h"
#include "cy_hash.h"



#include "cy_io_common_tools.h"
#include "cy_io_ec.h"
#include "cy_ec_const.h"

#include "cy_HashPedersen.h"

#include "cy_musig2.h"

#include "test_vectors/test_vector_musig2.c"

#define _NB_USERS_TEST 4
#define _NB_USER_EXAMPLE 4




int test_keygen(cy_musig2_ctx_t *ctx)
{
  cy_ec_ctx_t *ec_ctx=ctx->ctx_ec;
  cy_error_t error=CY_KO;
  cy_ecpoint_t ExpectedKpub;
  cy_ecpoint_t ComputedKpub;
  cy_fp_t random, xpriv;
  int flag=CY_FALSE;
  size_t t8_p=ec_ctx->ctx_fp_p->t8_modular;

  printf("\n test KeyGenDeriv Musig :");
  /* Allocations*/
  cy_fp_alloc(ec_ctx->ctx_fp_p,t8_p , &random);
  cy_fp_alloc(ec_ctx->ctx_fp_p, t8_p, &xpriv);

  CY_CHECK(cy_ec_alloc(ec_ctx, &ComputedKpub));
  CY_CHECK(cy_ec_alloc(ec_ctx, &ExpectedKpub));

  /* I. for the first part of the test, it is checked that the derivation from random is compliant to test vectors */
  CY_CHECK( cy_ec_import2(PublicKey_X0, sizeof(PublicKey_X0), PublicKey_Y0, sizeof(PublicKey_Y0), &ExpectedKpub));
  CY_CHECK(cy_fp_import(SecretKey_0, t8_p, &random));

  CY_CHECK(cy_musig_KeyGenDeriv(ctx, &random, &xpriv, &ComputedKpub));

  CY_CHECK(cy_ec_iseq(&ComputedKpub, &ExpectedKpub, &flag));



  if(flag!=CY_TRUE)
  {
	  printf("\n failure, flag=%d", flag);
     (cy_io_ec_printMSB(&ComputedKpub, "\n res Keygen"));
     (cy_io_ec_printMSB(&ExpectedKpub, "\n res expected"));

	  error=CY_FALSE;
	  goto end;
  }
  /* II. for the second part of the test, it is checked that full generation is not stuck */
  CY_CHECK(cy_musig_KeyGen(ctx, &xpriv, &ComputedKpub));
  CY_CHECK(cy_musig_KeyGen(ctx, &xpriv, &ExpectedKpub));

  CY_CHECK(cy_ec_iseq(&ComputedKpub, &ExpectedKpub, &flag));

  if(flag==CY_TRUE)
   {
 	  printf("\n gda stuck failure, flag=%d", flag);
      (cy_io_ec_printMSB(&ComputedKpub, "\n res Keygen"));
      (cy_io_ec_printMSB(&ExpectedKpub, "\n res KeyGen2"));

 	  error=CY_FALSE;
 	  goto end;
   }

  /* Release memory */

  CY_CHECK(cy_fp_free( &random));
  CY_CHECK(cy_fp_free( &xpriv));
  CY_CHECK(cy_ec_free( &ExpectedKpub));
  CY_CHECK(cy_ec_free( &ComputedKpub));

  printf(" OK");

  end:
 	  return error;
}

int test_verif_core(cy_musig2_ctx_t *ctx)
{
  cy_ec_ctx_t *ec_ctx=ctx->ctx_ec;

  cy_error_t error=CY_KO;
  cy_ecpoint_t KeyAgg, R;
  cy_fp_t fp_s,fp_c;
  int flag=0;


  size_t t8_p=ec_ctx->ctx_fp_p->t8_modular;

  printf("\n test Verif Musig Core:");
  /* Allocations*/
  CY_CHECK(cy_ec_alloc(ec_ctx, &KeyAgg));
  CY_CHECK(cy_ec_alloc(ec_ctx, &R));
  CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_p, &fp_s));
  CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_p, &fp_c));
  /* Import test vectors */
  CY_CHECK( cy_fp_import(s, sizeof(s), &fp_s));
  CY_CHECK(cy_fp_import(c, sizeof(c), &fp_c));
  CY_CHECK( cy_ec_import2(R_x, sizeof(R_x), R_y, sizeof(R_y), &R));
  CY_CHECK( cy_ec_import2(Key_Agg_X, sizeof(Key_Agg_X), Key_Agg_Y, sizeof(Key_Agg_Y),&KeyAgg ));

  /* Core verification */
  CY_CHECK(cy_musig_Verification_Core(ctx, &KeyAgg,&R, &fp_s, &fp_c,& flag));
  if(flag!=CY_TRUE){
	  error=CY_KO;
	  goto end;
  }

  /* Release memory */
  CY_CHECK(cy_ec_free( &KeyAgg));
  CY_CHECK(cy_ec_free( &R));
  CY_CHECK(cy_fp_free( &fp_s));
  CY_CHECK(cy_fp_free( &fp_c));

  printf(" OK");
  end:
  	  return error;
}


int test_verif_all(cy_musig2_ctx_t *musig_ctx)
{
	cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;

	  cy_error_t error=CY_KO;
	  cy_ecpoint_t KeyAgg, R;
	  cy_fp_t fp_s,fp_c;
	  int flag=CY_FALSE;
	  uint8_t buffer[2*MAX_MUSIG_EC_T8];

	  CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p,  musig_ctx->ctx_ec->ctx_fp_p->t8_modular , &fp_c));


	  size_t t8_p=ec_ctx->ctx_fp_p->t8_modular;

	  printf("\n test Verif Musig All:");
	  /* Allocations*/
	  CY_CHECK(cy_ec_alloc(ec_ctx, &KeyAgg));
	  CY_CHECK(cy_ec_alloc(ec_ctx, &R));
	  CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_p, &fp_s));

	  /* Import test vectors */
	  CY_CHECK( cy_fp_import(s, sizeof(s), &fp_s));
	  CY_CHECK( cy_ec_import2(R_x, sizeof(R_x), R_y, sizeof(R_y), &R));
	  CY_CHECK( cy_ec_import2(Key_Agg_X, sizeof(Key_Agg_X), Key_Agg_Y, sizeof(Key_Agg_Y),&KeyAgg ));

	  CY_CHECK(cy_Hsig(musig_ctx, &KeyAgg, &R, message_0, sizeof(message_0), buffer));
	  CY_CHECK(cy_fp_import(buffer, musig_ctx->ctx_ec->ctx_fp_p->t8_modular , &fp_c));

	  CY_CHECK(cy_musig_Verification_All(musig_ctx, &KeyAgg,&R,  &fp_s,(uint8_t*)( &message_0), sizeof(message_0),& flag));

	  /* Core verification */
	  if(flag!=CY_TRUE){
		  error=CY_KO;
		  goto end;
	  }

	  /* Release memory */
	  CY_CHECK(cy_ec_free( &KeyAgg));
	  CY_CHECK(cy_ec_free( &R));
	  CY_CHECK(cy_fp_free( &fp_s));
	  CY_CHECK(cy_fp_free( &fp_c));

	  printf(" OK");
	  end:
	  	  return error;

}


cy_error_t test_HashnKeyAgg(cy_musig2_ctx_t *musig_ctx)
{
		cy_error_t error=CY_OK;
		size_t i;
		uint8_t buffer[2*MAX_MUSIG_EC_T8];
		cy_ecpoint_t ec_L[_NB_USERS_TEST];
	    cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
		size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
		cy_fp_t ExpectedHAgg, HAgg;
		cy_ecpoint_t KeyAgg, expectedKeyAgg;
		uint8_t  svg_ai[_NB_USERS_TEST][_MAX_CYLIB_EC_T8];

		int flag=0;

		printf("\n test Hash Agg and Key Agg:");

		/* allocations*/
		CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p,t8_fp, &ExpectedHAgg));
		CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p,t8_fp, &HAgg));
		CY_CHECK(cy_ec_alloc(ec_ctx, &KeyAgg));
		CY_CHECK(cy_ec_alloc(ec_ctx, &expectedKeyAgg));

		for(i=0;i<_NB_USERS_TEST;i++)
		{
			CY_CHECK(cy_ec_alloc(ec_ctx, &ec_L[i]));
		}

		/* Import test vectors */
		CY_CHECK(cy_ec_import2(PublicKey_X0,sizeof(PublicKey_X0), PublicKey_Y0,sizeof(PublicKey_Y0),&ec_L[0]));
		CY_CHECK(cy_ec_import2(PublicKey_X1,sizeof(PublicKey_X1), PublicKey_Y1,sizeof(PublicKey_Y1), &ec_L[1]));
		CY_CHECK(cy_ec_import2(PublicKey_X2,sizeof(PublicKey_X2), PublicKey_Y2,sizeof(PublicKey_Y2),&ec_L[2]));
		CY_CHECK(cy_ec_import2(PublicKey_X3,sizeof(PublicKey_X3), PublicKey_Y3,sizeof(PublicKey_Y3), &ec_L[3]));

		CY_CHECK(cy_ec_import2(Key_Agg_X,sizeof(Key_Agg_X), Key_Agg_Y,sizeof(Key_Agg_Y), &expectedKeyAgg));

		CY_CHECK(cy_fp_import(a_1, sizeof(a_1), &ExpectedHAgg));


		/* Compute ai's */
		CY_CHECK(cy_Hagg(musig_ctx, ec_L, 1, buffer));

		CY_CHECK(cy_fp_import(buffer, sizeof(a_1), &HAgg));


		/* Compare computed to expected test vector */
		CY_CHECK(cy_fp_iseq(&ExpectedHAgg, &HAgg, &flag));


		 if(flag!=CY_TRUE) {
					 error=CY_FALSE;
					 printf("\n flag pedersen chain=%d", flag);
					 (cy_io_fp_printMSB(&HAgg, "\n hagg computed"));
					 (cy_io_fp_printMSB(&ExpectedHAgg, "\n hagg expected"));
					 goto end;
				 }
		CY_CHECK(cy_musig_KeyAgg(musig_ctx, ec_L, &KeyAgg, svg_ai));


		CY_CHECK(cy_ec_iseq(&expectedKeyAgg, &KeyAgg, &flag));

		if(flag!=CY_TRUE) {
						 error=CY_FALSE;
						 printf("\n flag pedersen chain=%d", flag);
						 (cy_io_ec_printMSB(&KeyAgg, "\n hagg computed"));
						 (cy_io_ec_printMSB(&expectedKeyAgg, "\n hagg expected"));
						 goto end;
					 }

		/* free memory*/
		for(i=0;i<_NB_USERS_TEST;i++)
		{
				CY_CHECK(cy_ec_free( &ec_L[i]));
		}

		CY_CHECK(cy_ec_free(&KeyAgg));
		CY_CHECK(cy_ec_free( &expectedKeyAgg));

		CY_CHECK(cy_fp_free( &ExpectedHAgg));
		CY_CHECK(cy_fp_free( &HAgg));
		printf(" OK");
		end:

			return error;
}

/* todo*/
cy_error_t test_HashNonce(cy_musig2_ctx_t *musig_ctx)
{
	cy_error_t error=CY_OK;

	UNUSED(musig_ctx);
	return error;
}


/* note that the Hsig is xonly compatible, only X_x and Rx  are aggregated to the sig*/
cy_error_t test_HashSig(cy_musig2_ctx_t *musig_ctx)
{
	cy_error_t error=CY_OK;
	uint8_t buffer[MAX_MUSIG_EC_T8];
	cy_ecpoint_t R, KeyAg;
	cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
	cy_fp_t fp_c, expected;
	int flag;

	printf("\n test Hsig:");

	CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p,  musig_ctx->ctx_ec->ctx_fp_p->t8_modular , &fp_c));
	CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p,  musig_ctx->ctx_ec->ctx_fp_p->t8_modular , &expected));
	CY_CHECK(cy_fp_import(c, sizeof(c), &expected));

	CY_CHECK(cy_ec_alloc(ec_ctx, &R));
	CY_CHECK(cy_ec_alloc(ec_ctx, &KeyAg));

	CY_CHECK(cy_ec_import2(R_x,sizeof(R_x), R_y,sizeof(R_y),&R));
	CY_CHECK(cy_ec_import2(Key_Agg_X,sizeof(Key_Agg_X), Key_Agg_Y,sizeof(Key_Agg_Y),&KeyAg));

	CY_CHECK(cy_Hsig(musig_ctx, &KeyAg, &R, message_0, sizeof(message_0), buffer));
	CY_CHECK(cy_fp_import(buffer, musig_ctx->ctx_ec->ctx_fp_p->t8_modular , &fp_c));


	CY_CHECK(cy_fp_iseq(&fp_c, &expected, &flag));

	 if(flag!=CY_TRUE)
	  {
		  printf("\n failure, flag=%d", flag);
	     (cy_io_fp_printMSB(&fp_c, "\n res Hsig"));
	     (cy_io_fp_printMSB(&expected, "\n res expected"));

		  error=CY_FALSE;
		  goto end;
	  }
	printf(" OK");

	CY_CHECK(cy_ec_free( &R));
	CY_CHECK(cy_ec_free( &KeyAg));
	CY_CHECK(cy_fp_free( &fp_c));
	CY_CHECK(cy_fp_free( &expected));

	end:
	return error;
}

static cy_error_t test_musig_SetUp(cy_hash_unit_t *H, cy_gda_ctx_t *gda, int curve, cy_ec_ctx_t *ec_ctx, uint8_t *Ramp, size_t Ramp_t8, cy_musig2_ctx_t *musig_ctx)
{
	cy_error_t error=CY_OK;

	printf("\n test Musig2 SetUp:");
	printf(" %s:", H->Component_name);fflush(stdout);

	CY_CHECK(cy_musig_SetUp( H, Ramp, Ramp_t8, curve, _NB_USER_EXAMPLE, ec_ctx, musig_ctx));
	musig_ctx->gda=gda;
	//print_MsbString(musig_ctx->order, musig_ctx->ctx_ec->ctx_fp_p->t8_modular, "\n order:");

	CY_CHECK(musig_ctx->gda->GDA_Init(musig_ctx->gda, NULL, musig_ctx->ctx_ec->ctx_fp_p->t8_modular));



	end:
	if(error==CY_OK)  printf(" OK");
	else printf(" KO!!!");
			return error;
}


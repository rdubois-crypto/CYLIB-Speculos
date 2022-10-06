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
/* DESCRIPTION: testing wrappers for elliptic arithmetic */
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

int test_keygen(cy_musig2_ctx_t *ctx)
{
  cy_ec_ctx_t *ec_ctx=ctx->ctx_ec;
  cy_error_t error=CY_KO;
  cy_ecpoint_t ExpectedKpub;
  cy_ecpoint_t ComputedKpub;
  cy_fp_t random, xpriv;
  int flag=CY_FALSE;
  size_t t8_p=ec_ctx->ctx_fp_p->t8_modular;

  printf("\n  KeyGen Musig Core:");
  /* Allocations*/
  cy_fp_alloc(ec_ctx->ctx_fp_p,t8_p , &random);
  cy_fp_alloc(ec_ctx->ctx_fp_p, t8_p, &xpriv);

  CY_CHECK(cy_ec_alloc(ec_ctx, &ComputedKpub));
  CY_CHECK(cy_ec_alloc(ec_ctx, &ExpectedKpub));

  CY_CHECK( cy_ec_import2(PublicKey_X0, sizeof(PublicKey_X0), PublicKey_Y0, sizeof(PublicKey_Y0), &ExpectedKpub));
  CY_CHECK(cy_fp_import(SecretKey_0, t8_p, &random));


  CY_CHECK(cy_musig_KeyGenDeriv(ctx, &random, &xpriv, &ComputedKpub));

  cy_ec_iseq(&ComputedKpub, &ExpectedKpub, &flag);



  if(flag!=CY_TRUE)
  {
	  printf("\n failure, flag=%d", flag);
     (cy_io_ec_printMSB(&ComputedKpub, "\n res Keygen"));
     (cy_io_ec_printMSB(&ExpectedKpub, "\n res expected"));

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
  cy_ecpoint_t Generator, KeyAgg, R;
  cy_fp_t fp_s,fp_c;
  int flag=0;


  size_t t8_p=ec_ctx->ctx_fp_p->t8_modular;

  printf("\n  Verif Musig Core:");
  /* Allocations*/
  CY_CHECK(cy_ec_alloc(ec_ctx, &KeyAgg));
  CY_CHECK(cy_ec_alloc(ec_ctx, &Generator));
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
  CY_CHECK(cy_ec_free( &Generator));
  CY_CHECK(cy_ec_free( &KeyAgg));
  CY_CHECK(cy_ec_free( &R));
  CY_CHECK(cy_fp_free( &fp_s));
  CY_CHECK(cy_fp_free( &fp_c));

  printf(" OK");
  end:
  	  return error;
}

cy_error_t cy_musig_SetUp( cy_hash_unit_t *H, uint8_t *Elliptic_Ramp, size_t Elliptic_Ramp_t8, int curve,  int n_users,
							   cy_ec_ctx_t *ec_ctx, cy_musig2_ctx_t *musig_ctx){

    cy_error_t error=CY_OK;
    uint8_t zero[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    CY_CHECK(cy_ec_init(ec_ctx, Elliptic_Ramp,Elliptic_Ramp_t8, curve, NULL));

   	musig_ctx->ctx_ec=ec_ctx;
   	musig_ctx->H=H;
   	musig_ctx->n_users=n_users;
    CY_CHECK(H->Hash_Configure( H, (uint8_t *) musig_ctx->ctx_ec, _UNUSED_VALUE_));

   	end:
		return error;
}


cy_error_t cy_musig_uninit( cy_musig2_ctx_t *musig_ctx)
{
    cy_error_t error=CY_KO;
    cy_hash_unit_t *H=musig_ctx->H;
    CY_CHECK(H->Hash_Uninit(H->ctx));

    CY_CHECK(cy_ec_uninit(musig_ctx->ctx_ec));

	end:
		return error;
}

/* arbitrary constant for domain separation, beware to be equal with sage simulation */
#define _SEPARATION_AGG 0x3

// L concatenation of public keys
cy_error_t cy_PedersenHashAgg(cy_musig2_ctx_t *musig_ctx,  const cy_ecpoint_t *L, int index_i, uint8_t *ai)
{
  size_t i;
  cy_error_t error=CY_OK;
  uint8_t zero[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  uint8_t buffer[2*MAX_MUSIG_EC_T8];
  uint8_t separation[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,_SEPARATION_AGG};

  size_t t8_buffer=2*musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
  cy_pedersen_ctx_t pedersen_cx;
  size_t t8_modular=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

  CY_CHECK(pedersen_configure( musig_ctx->ctx_ec, &pedersen_cx));



  CY_CHECK(pedersen_hash_init(&pedersen_cx, zero, sizeof(zero)));
  CY_CHECK(pedersen_hash_update(&pedersen_cx, separation, sizeof(separation)));

  for(i=0;i<4;i++)
  {
	  CY_CHECK(cy_ec_export(&L[i], buffer, t8_modular));

	  CY_CHECK(pedersen_hash_update(&pedersen_cx, buffer, t8_buffer));
  }

  CY_CHECK(cy_ec_export(&L[index_i], buffer, t8_modular));
  CY_CHECK(pedersen_hash_update(&pedersen_cx, buffer, t8_buffer));
  CY_CHECK(pedersen_hash_final(&pedersen_cx, ai, musig_ctx->ctx_ec->ctx_fp_p->t8_modular));
  CY_CHECK(pedersen_uninit(  &pedersen_cx));

  end:
  	return error;

}



cy_error_t test_HashnKeyAgg(cy_musig2_ctx_t *musig_ctx)
{
		cy_error_t error=CY_OK;
		cy_ecpoint_t ec_temp;
		size_t i;
		uint8_t buffer[2*MAX_MUSIG_EC_T8];
		cy_ecpoint_t ec_L[_NB_USERS_TEST];
	    cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
		size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
		cy_fp_t ExpectedHAgg, HAgg;
		cy_ecpoint_t KeyAgg;


		int flag=0;

		printf("\n  Test Hash Agg:");

		/* allocations*/
		CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p,t8_fp, &ExpectedHAgg));
		CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p,t8_fp, &HAgg));
		CY_CHECK(cy_ec_alloc(ec_ctx, &KeyAgg));

		for(i=0;i<_NB_USERS_TEST;i++)
		{
			CY_CHECK(cy_ec_alloc(ec_ctx, &ec_L[i]));
		}

		/* Import test vectors */
		CY_CHECK(cy_ec_import2(PublicKey_X0,sizeof(PublicKey_X0), PublicKey_Y0,sizeof(PublicKey_Y0),&ec_L[0]));
		CY_CHECK(cy_ec_import2(PublicKey_X1,sizeof(PublicKey_X1), PublicKey_Y1,sizeof(PublicKey_Y1), &ec_L[1]));
		CY_CHECK(cy_ec_import2(PublicKey_X2,sizeof(PublicKey_X2), PublicKey_Y2,sizeof(PublicKey_Y2),&ec_L[2]));
		CY_CHECK(cy_ec_import2(PublicKey_X3,sizeof(PublicKey_X3), PublicKey_Y3,sizeof(PublicKey_Y3), &ec_L[3]));

		CY_CHECK(cy_fp_import(a_1, sizeof(a_1), &ExpectedHAgg));


		/* Compute ai's */
		CY_CHECK(cy_HashAgg(musig_ctx, ec_L, 1, buffer));

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
		CY_CHECK(cy_KeyAgg(musig_ctx, ec_L, &KeyAgg));

		(cy_io_ec_printMSB(&KeyAgg, "\n KeyAgg computed"));

		/* free memory*/
		for(i=0;i<_NB_USERS_TEST;i++)
		{
				CY_CHECK(cy_ec_free( &ec_L[i]));
		}

		CY_CHECK(cy_ec_free(&KeyAgg));

		CY_CHECK(cy_fp_free( &ExpectedHAgg));
		CY_CHECK(cy_fp_free( &HAgg));
		printf(" OK");
		end:

			return error;
}

/*************************************************************/
/* Computation of ai=H(L, X_i)*/
/*************************************************************/
// L concatenation of public keys
cy_error_t cy_HashAgg(cy_musig2_ctx_t *musig_ctx, const cy_ecpoint_t *L, int index_i, uint8_t *ai)
{
	  size_t i;
	  cy_error_t error=CY_OK;
	  uint8_t zero[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	  cy_hash_unit_t *H=musig_ctx->H;

	  uint8_t buffer[2*MAX_MUSIG_EC_T8];
	  uint8_t separation[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,_SEPARATION_AGG};

	  size_t t8_buffer=2*musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

	  size_t t8_modular=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

	 // CY_CHECK(H->Hash_Configure( H, (uint8_t *) musig_ctx->ctx_ec, _UNUSED_VALUE_));
	  CY_CHECK(H->Hash_Init(H->ctx, zero, 1));
	  CY_CHECK(H->Hash_Update(H->ctx, separation, sizeof(separation)));

	  for(i=0;i<4;i++)
	  {
		  CY_CHECK(cy_ec_export(&L[i], buffer, t8_modular));
		  CY_CHECK(H->Hash_Update(H->ctx, buffer, t8_buffer));
	  }

	  CY_CHECK(cy_ec_export(&L[index_i], buffer, t8_modular));
	  CY_CHECK(H->Hash_Update(H->ctx, buffer, t8_buffer));
	  CY_CHECK(H->Hash_Final(H->ctx, ai, musig_ctx->ctx_ec->ctx_fp_p->t8_modular));
	  //CY_CHECK(H->Hash_Uninit(H->ctx));

	  end:
	  	return error;
}

// L concatenation of public keys
cy_error_t cy_KeyAgg(cy_musig2_ctx_t *musig_ctx, const cy_ecpoint_t *ec_L, cy_ecpoint_t *KeyAgg)
{
  cy_ecpoint_t ec_temp, accumulator;
  cy_error_t error;
  cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
  size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

  uint8_t buffer[2*MAX_MUSIG_EC_T8];
  cy_fp_t ai;

  size_t i;


  CY_CHECK(cy_ec_alloc(ec_ctx, &ec_temp));
  CY_CHECK(cy_ec_alloc(ec_ctx, &accumulator));

  CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_fp, &ai));

  CY_CHECK(cy_ec_setinfinity(&accumulator));

  for(i=0;i<4/*musig_ctx->n_users*/;i++){
	  CY_CHECK(cy_HashAgg(musig_ctx, ec_L, i, buffer));
	  CY_CHECK(cy_fp_import( buffer, t8_fp, &ai));
 	 (pcy_io_fp_printMSB(&ai, "\n ai computed"));

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


cy_error_t test_musig_configure(cy_hash_unit_t *H,int curve, cy_ec_ctx_t *ec_ctx, uint8_t *Ramp, size_t Ramp_t8, cy_musig2_ctx_t *musig_ctx)
{
	cy_error_t error=CY_OK;

	printf("\n\n  Test Musig2 Configure:");
	printf(" %s:", H->Component_name);fflush(stdout);

	CY_CHECK(cy_musig_SetUp( H, Ramp, Ramp_t8, curve, 4, ec_ctx, musig_ctx));

	end:
	if(error==CY_OK)  printf(" OK");
	else printf(" KO!!!");
			return error;
}

int test_musig_unit(uint8_t *Ramp, size_t Ramp_t8)
{
	cy_error_t error=CY_OK;
	cy_ec_ctx_t ec_ctx;

	cy_musig2_ctx_t musig_ctx;
	printf("\n\n /************************ Test Musig2 Protocol:");

	//print_MsbString(Ramp, Ramp_t8, "\nRAMp:\n");

	/* Initiate gda*/
	CY_CHECK(test_musig_configure(&unit_pedersen, CY_CURVE_Stark256, &ec_ctx, Ramp, Ramp_t8, &musig_ctx));

	//return 0;
	//musig_ctx.gda=&bolos_gda_component;

    CY_CHECK(test_verif_core(&musig_ctx));
	CY_CHECK(test_keygen(&musig_ctx));
	CY_CHECK(test_HashnKeyAgg(&musig_ctx));

	//print_MsbString(Ramp, Ramp_t8, "\nRAMp:\n");

	//CY_CHECK(cy_ec_uninit(&ec_ctx));
	CY_CHECK(cy_musig_uninit( &musig_ctx));

	/* configuring Musig2 with hashpedersen and StarkCurve */

	end:
	if(error==CY_OK)  printf("\n All Musig2 tests OK");
	else printf("\n  Musig2 KO with error=%x",(unsigned int) error);
			return error;
}





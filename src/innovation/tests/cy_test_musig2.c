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
		cy_ecpoint_t KeyAgg, expectedKeyAgg;


		int flag=0;

		printf("\n  Test Hash Agg and Key Agg:");

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
		CY_CHECK(cy_musig_KeyAgg(musig_ctx, ec_L, &KeyAgg));


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

/* note that the Hsig is xonly compatible, only X_x and Rx  are aggregated to the sig*/
cy_error_t test_HashSig(cy_musig2_ctx_t *musig_ctx)
{
	cy_error_t error=CY_OK;
	uint8_t buffer[2*MAX_MUSIG_EC_T8];
	cy_ecpoint_t R, KeyAg;
	cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
	cy_fp_t fp_c, expected;
	int flag;

	printf("\n  Test Hsig:");

	CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p,  musig_ctx->ctx_ec->ctx_fp_p->t8_modular , &fp_c));
	CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p,  musig_ctx->ctx_ec->ctx_fp_p->t8_modular , &expected));
	CY_CHECK(cy_fp_import(c, sizeof(c), &expected));

	CY_CHECK(cy_ec_alloc(ec_ctx, &R));
	CY_CHECK(cy_ec_alloc(ec_ctx, &KeyAg));

	CY_CHECK(cy_ec_import2(R_x,sizeof(R_x), R_y,sizeof(R_y),&R));
	CY_CHECK(cy_ec_import2(Key_Agg_X,sizeof(Key_Agg_X), Key_Agg_Y,sizeof(Key_Agg_Y),&KeyAg));

	CY_CHECK(cy_Hsig(musig_ctx, &KeyAg, &R, message_0, sizeof(message_0), &fp_c));


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
/*************************************************************/
/* Computation of b=H(X, R1, ..., Rv, m)*/
/*************************************************************/
cy_error_t cy_Hashnon(cy_musig2_ctx_t *musig_ctx, const cy_ecpoint_t *XAgg, const cy_ecpoint_t *R,  uint8_t *m, size_t t8_m, uint8_t *b)
{
	  size_t i;
	  cy_error_t error=CY_OK;
	  uint8_t zero[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	  cy_hash_unit_t *H=musig_ctx->H;

	  uint8_t buffer[2*MAX_MUSIG_EC_T8];
	  uint8_t separation[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,_SEPARATION_NON};

	  size_t t8_buffer=2*musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

	  size_t t8_modular=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

	 // CY_CHECK(H->Hash_Configure( H, (uint8_t *) musig_ctx->ctx_ec, _UNUSED_VALUE_));
	  CY_CHECK(H->Hash_Init(H->ctx, zero, 1));
	  CY_CHECK(H->Hash_Update(H->ctx, separation, sizeof(separation)));

	  CY_CHECK(cy_ec_export(XAgg, buffer, t8_modular));
	  CY_CHECK(H->Hash_Update(H->ctx, buffer, t8_buffer));
	  for(i=0;i<_MU_;i++)
	  	  {
		  	  CY_CHECK(cy_ec_export(R+i, buffer, t8_modular));
		  	  CY_CHECK(H->Hash_Update(H->ctx, buffer, t8_buffer));
	  	  }
	  CY_CHECK(H->Hash_Update(H->ctx, m, t8_m));
	  CY_CHECK(H->Hash_Final(H->ctx, b, musig_ctx->ctx_ec->ctx_fp_p->t8_modular));
	  //CY_CHECK(H->Hash_Uninit(H->ctx));

	  end:
	  	return error;
}



/* Signature round 1*/
/* beware that sign of random will be modified is derived y is even*/
cy_error_t cy_musig_Sign1_Deriv(const cy_musig2_ctx_t *ctx, const cy_fp_t io_random[_MU_],
								 cy_ecpoint_t Ri[_MU_])
{
	/*I.Declarations and allocations*/
	cy_error_t error=CY_KO;
	cy_fp_t x;
    size_t i;
    uint32_t sign;

	cy_ec_ctx_t  *ec_ctx= ctx->ctx_ec;

	cy_ecpoint_t G;
	cy_ec_alloc(ctx->ctx_ec, &G);

	CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &x));
	CY_CHECK(cy_ec_get_generator(ctx->ctx_ec, &G));

	/*II. Computations*/

	/* returns the corresponding public key X=g^x*/
	for(i=0;i<_MU_;i++)
		{
		  CY_CHECK(cy_ec_scalarmult_fp(io_random, &G, Ri+i));
		  /* compatibility BIP340*/
		  	CY_CHECK(cy_ec_getparityY( Ri+i, &sign));

		  	if(sign==1)
		  	{
		  		CY_CHECK(cy_fp_neg(io_random, io_random));
		  		CY_CHECK(cy_ec_scalarmult_fp(io_random, &G,  Ri+i));
		  	}
		}

	/*III. Free pointers*/
	CY_CHECK(cy_fp_free(&x));
	cy_ec_free(&G);

	end:
   	  return error;
}

cy_error_t cy_musig_Sign1_Agg(const cy_musig2_ctx_t *musig_ctx, cy_ecpoint_t *Rij[_MU_], cy_ecpoint_t *R)
{
	size_t i,j;
	cy_error_t error=CY_KO;
	for(j=0;j<_MU_;j++)
	{
		CY_CHECK(cy_ec_setinfinity(&R[j]));

		for(i=0;i<musig_ctx->n_users;i++)
		{
			 CY_CHECK(cy_ec_add(&(Rij[i][j]), &R[j], &R[j]));
		}
	}
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
	CY_CHECK(test_HashSig(&musig_ctx));
	//print_MsbString(Ramp, Ramp_t8, "\nRAMp:\n");

	//CY_CHECK(cy_ec_uninit(&ec_ctx));
	CY_CHECK(cy_musig_uninit( &musig_ctx));

	/* configuring Musig2 with hashpedersen and StarkCurve */

	end:
	if(error==CY_OK)  printf("\n All Musig2 tests OK");
	else printf("\n  Musig2 KO with error=%x",(unsigned int) error);
			return error;
}





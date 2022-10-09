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

	  CY_CHECK(cy_Hsig(musig_ctx, &KeyAgg, &R, message_0, sizeof(message_0), &fp_c));
	  CY_CHECK(cy_musig_Verification_All(musig_ctx, &KeyAgg,&R, &fp_s, &message_0, sizeof(message_0),& flag));

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
		cy_ecpoint_t ec_temp;
		size_t i;
		uint8_t buffer[2*MAX_MUSIG_EC_T8];
		cy_ecpoint_t ec_L[_NB_USERS_TEST];
	    cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
		size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
		cy_fp_t ExpectedHAgg, HAgg;
		cy_ecpoint_t KeyAgg, expectedKeyAgg;


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

cy_error_t test_HashNonce(cy_musig2_ctx_t *musig_ctx)
{
	cy_error_t error=CY_OK;


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

	printf("\n test Hsig:");

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


#define _NB_USER_EXAMPLE 4

static cy_error_t test_musig_SetUp(cy_hash_unit_t *H, cy_gda_ctx_t *gda, int curve, cy_ec_ctx_t *ec_ctx, uint8_t *Ramp, size_t Ramp_t8, cy_musig2_ctx_t *musig_ctx)
{
	cy_error_t error=CY_OK;

	printf("\n test Musig2 SetUp:");
	printf(" %s:", H->Component_name);fflush(stdout);

	CY_CHECK(cy_musig_SetUp( H, Ramp, Ramp_t8, curve, _NB_USER_EXAMPLE, ec_ctx, musig_ctx));
	musig_ctx->gda=gda;

	CY_CHECK(musig_ctx->gda->GDA_Init(musig_ctx->gda, NULL, musig_ctx->ctx_ec->ctx_fp_p->t8_modular));

	end:
	if(error==CY_OK)  printf(" OK");
	else printf(" KO!!!");
			return error;
}

/* simulating the keygen of all users */
 cy_error_t simu_Keygen_all(cy_musig2_ctx_t *musig_ctx,
						  	 uint8_t svg_Kpub[][_MAX_CYLIB_EC_T8],
							 cy_ecpoint_t *KeyAgg)
{
	size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
	cy_error_t error;
	cy_ecpoint_t Kpub[_NB_USER_EXAMPLE];
	cy_fp_t xpriv[_NB_USER_EXAMPLE];
	size_t i;
	cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
	//uint8_t buffer[_MAX_CYLIB_EC_T8];


	for(i=0;i<musig_ctx->n_users;i++)
	{
				/* I. Allocations*/
				CY_CHECK(cy_ec_alloc(ec_ctx, &Kpub[i]));
				CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_fp, &xpriv[i]));

				/* II. Key Generation :each user generates its keys*/
				CY_CHECK(cy_musig_KeyGen(musig_ctx, &xpriv[i], &Kpub[i]));
				CY_CHECK(cy_ec_export( &Kpub[i],svg_Kpub[i], t8_fp));

	}

	CY_CHECK(cy_musig_KeyAgg(musig_ctx, Kpub, KeyAgg));
	/* IV. Free memory*/
	for(i=0;i<musig_ctx->n_users;i++)
	{
		CY_CHECK(cy_ec_free(&Kpub[i]));
		CY_CHECK(cy_fp_free(&xpriv[i]));
	}
	end:
		return error;
}




/* testing validity of import/export*/
test_keygen_all( cy_musig2_ctx_t *musig_ctx,uint8_t svg_Kpub[][_MAX_CYLIB_EC_T8], cy_ecpoint_t *KeyAgg)
 {
	cy_ecpoint_t Kpub[_NB_USER_EXAMPLE];
	cy_error_t error;
	cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
	size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
	printf("\n    subtest keygen_all:");
	size_t i;


	cy_ecpoint_t KeyAgg_computed;
	int flag=0;

	CY_CHECK(cy_ec_alloc(ec_ctx, &KeyAgg_computed));

	for(i=0;i<musig_ctx->n_users;i++)
	{
				/* I. Allocations*/
				CY_CHECK(cy_ec_alloc(ec_ctx, &Kpub[i]));

				/* II. Key Restauration :each user import its keys*/
				CY_CHECK(cy_ec_import( svg_Kpub[i], t8_fp, &Kpub[i]));
	}

	CY_CHECK(cy_musig_KeyAgg(musig_ctx, Kpub, &KeyAgg_computed));

	CY_CHECK(cy_ec_iseq(&KeyAgg_computed, KeyAgg, &flag));

	if(flag!=CY_TRUE)
	{
		cy_io_ec_printMSB(KeyAgg, "\n expected");
		cy_io_ec_printMSB(&KeyAgg_computed, "\n computed");
		error=CY_FALSE;
		goto end;
	}

	CY_CHECK(cy_ec_free(&KeyAgg_computed));

	for(i=0;i<musig_ctx->n_users;i++)
		CY_CHECK(cy_ec_free( &Kpub[i]));


	end:
			if(error==CY_OK)  printf(" OK");
			else printf(" KO!!!");
				return error;
 }



/* simulating the keygen of all users */
cy_error_t simu_Sign1_all(cy_musig2_ctx_t *musig_ctx,
						  uint8_t svg_rij[][_MU_][_MAX_CYLIB_EC_T8],
						  uint8_t svg_Rij[][_MU_][_MAX_CYLIB_EC_T8],
						  cy_ecpoint_t R[_MU_]
 							 )
{
	cy_error_t error;
	cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
	size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
	size_t i,j;

	cy_fp_t rij[_NB_USER_EXAMPLE][_MU_];
	cy_ecpoint_t Rij[_NB_USER_EXAMPLE][_MU_];

	printf("\n    Sign Round 1");


	for(i=0;i<musig_ctx->n_users;i++)
	{
		for(j=0;j<_MU_;j++)
		{
			CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_fp, &(rij[i][j])));
			//printf("\n i=%d, j=%d",i,j);
			CY_CHECK(cy_ec_alloc(ec_ctx, &(Rij[i][j])));
			//printf("\n allocated cell: %x %x %x", Rij[i][j].ec->x, Rij[i][j].ec->y, Rij[i][j].ec->z);

		}
		CY_CHECK(cy_musig_Sign1(musig_ctx, rij[i], Rij[i]));

		for(j=0;j<_MU_;j++)
		{
			//printf("\n-------------------- i=%d, j=%d", (int) i, (int) j);
			CY_CHECK(cy_ec_export( &Rij[i][j],svg_Rij[i][j], t8_fp));
			//cy_io_ec_printMSB(&Rij[i][j], "\n exporting:");

			CY_CHECK(cy_fp_export(&rij[i][j],  svg_rij[i][j], t8_fp));

			CY_CHECK(cy_fp_free(&(rij[i][j])));
			CY_CHECK(cy_ec_free(&(Rij[i][j])));
		}

	}
	CY_CHECK(cy_musig_Sign1_tu8_Agg(musig_ctx, svg_Rij, R));

	end:
			return error;
}
/* todo: test reimport and compliance with derivated Rij's*/
int test_Sig1Agg(cy_musig2_ctx_t *musig_ctx,  uint8_t svg_rij[][_MU_][_MAX_CYLIB_EC_T8],
		  uint8_t svg_Rij[][_MU_][_MAX_CYLIB_EC_T8])
{
 cy_error_t error;
 cy_fp_t rij[_NB_USER_EXAMPLE][_MU_];
 cy_ecpoint_t Rij[_NB_USER_EXAMPLE][_MU_];
 cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
 int flag;
 cy_ecpoint_t ec_temp;
 size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
 size_t i,j;
 printf("\n    subtest SigAgg1: ");

 CY_CHECK(cy_ec_alloc(ec_ctx, &ec_temp));

 for(i=0;i<musig_ctx->n_users;i++)
 	{
 		for(j=0;j<_MU_;j++)
 		{
 			CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_fp, &(rij[i][j])));
 			CY_CHECK(cy_ec_alloc(ec_ctx, &(Rij[i][j])));

 			CY_CHECK(cy_ec_import( svg_Rij[i][j], t8_fp, &Rij[i][j]));
 			CY_CHECK(cy_fp_import(  svg_rij[i][j], t8_fp, &rij[i][j]));

 			CY_CHECK(cy_musig_KeyGenDeriv(musig_ctx, &rij[i][j], &rij[i][j], &ec_temp));

 			CY_CHECK(cy_ec_iseq(&ec_temp, &Rij[i][j], &flag));

 			CY_CHECK(cy_fp_free(&(rij[i][j])));
 			CY_CHECK(cy_ec_free(&(Rij[i][j])));
 			if(flag!=CY_TRUE)
 			{
 				error=CY_FALSE;
 				goto end;
 			}
 		}

 	}
 printf(" OK");

 end:
 CY_CHECK(cy_ec_free(&ec_temp));

 			return error;
}


/* simulating the keygen of all users */
cy_error_t simu_Sign2_all(cy_musig2_ctx_t *musig_ctx,
						  uint8_t svg_Kpub[][_MAX_CYLIB_EC_T8],
						  uint8_t svg_rij[][_MU_][_MAX_CYLIB_EC_T8],
						  uint8_t svg_Rij[][_MU_][_MAX_CYLIB_EC_T8],
						  cy_fp_t *s
 							 )
{


}

/* full example, assuming an initialized musig_ctx */
cy_error_t test_musig_full_example(cy_musig2_ctx_t *musig_ctx)
{
	size_t i ,j; /*counters,  i for users, j for [0..mu-1]*/
	cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
	size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

	cy_error_t error;

	cy_ecpoint_t ec_R, Kpub[_NB_USER_EXAMPLE];
	cy_fp_t xpriv[_NB_USER_EXAMPLE];

	/* those must be saved if using a single component and swap to memory */
	cy_ecpoint_t KeyAgg;

	uint8_t svg_rij[_NB_USER_EXAMPLE][_MU_][_MAX_CYLIB_EC_T8];
	uint8_t svg_Rij[_NB_USER_EXAMPLE][_MU_][_MAX_CYLIB_EC_T8];
	uint8_t svg_Kpub[_NB_USER_EXAMPLE][_MAX_CYLIB_EC_T8];

	CY_CHECK(cy_ec_alloc(ec_ctx, &KeyAgg));
	CY_CHECK(cy_ec_alloc(ec_ctx, &ec_R));
	printf("\n\n test Musig2 Full example, current bn:%x", KeyAgg.ec->x);

	CY_CHECK(simu_Keygen_all(musig_ctx, svg_Kpub, &KeyAgg));
	CY_CHECK(test_keygen_all(musig_ctx, svg_Kpub, &KeyAgg));
	CY_CHECK(simu_Sign1_all( musig_ctx, svg_rij,svg_Rij, &ec_R));
	CY_CHECK(test_Sig1Agg(musig_ctx,  svg_rij,svg_Rij));

	CY_CHECK(cy_ec_free( &ec_R));
	CY_CHECK(cy_ec_free( &KeyAgg));
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
	CY_CHECK(test_musig_SetUp(&unit_pedersen, &bolos_gda_component, CY_CURVE_Stark256, &ec_ctx, Ramp, Ramp_t8, &musig_ctx));

	//return 0;
	//musig_ctx.gda=&bolos_gda_component;

    CY_CHECK(test_verif_core(&musig_ctx)); 	/* unitary vector test of schnorr verification, external hashing*/
	CY_CHECK(test_keygen(&musig_ctx));	 	/* unitary vector test of key generation */
	CY_CHECK(test_HashnKeyAgg(&musig_ctx)); /* unitary vector test of key aggregation */
	//CY_CHECK(test_HashSig(&musig_ctx));		/* unitary test for Hsig, also validated with full verif_all */
	//print_MsbString(Ramp, Ramp_t8, "\nRAMp:\n");
	CY_CHECK(test_verif_all(&musig_ctx));	/* unitary vector test of schnorr verification, external hashing*/

	CY_CHECK(test_musig_full_example(&musig_ctx));
	CY_CHECK(cy_musig_uninit( &musig_ctx));

	/* configuring Musig2 with hashpedersen and StarkCurve */

	end:
	if(error==CY_OK)  printf("\n All Musig2 tests OK");
	else printf("\n  Musig2 KO with error=%x",(unsigned int) error);
			return error;
}





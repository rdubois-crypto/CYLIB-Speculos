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


#include "cy_musig2.h"

#include "cy_io_common_tools.h"
#include "cy_io_ec.h"
#include "cy_ec_const.h"

#include "cy_HashPedersen.h"
#include "test_vectors/test_vector_musig2.c"

int test_keygen(cy_musig2_ctx_t *ctx)
{
  cy_ec_ctx_t *ec_ctx=ctx->ctx_ec;
  cy_error_t error=CY_KO;
  cy_ecpoint_t Generator;
  cy_ecpoint_t ExpectedKpub;
  cy_ecpoint_t ComputedKpub;
  cy_fp_t random, xpriv;
  int flag=CY_FALSE;
  size_t t8_p=ec_ctx->ctx_fp_p->t8_modular;

  printf("\n  KeyGen Musig Core:");
  /* Allocations*/
  cy_fp_alloc(ec_ctx->ctx_fp_p,t8_p , &random);
  cy_fp_alloc(ec_ctx->ctx_fp_p, t8_p, &xpriv);

  CY_CHECK(cy_ec_alloc(ec_ctx, &Generator));
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

  CY_CHECK(cy_ec_free( &Generator));
  CY_CHECK(cy_ec_free( &ExpectedKpub));

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

cy_error_t cy_musig_configure( cy_hash_unit_t *H, uint8_t *Elliptic_Ramp, size_t Elliptic_Ramp_t8, int curve,  int n_users,
							   cy_ec_ctx_t *ec_ctx, cy_musig2_ctx_t *musig_ctx){

    cy_error_t error=CY_OK;

    CY_CHECK(cy_ec_init(ec_ctx, Elliptic_Ramp,Elliptic_Ramp_t8, curve, NULL));

   	musig_ctx->ctx_ec=ec_ctx;
   	musig_ctx->H=H;
   	musig_ctx->n_users=n_users;
   	//pedersen_unit_configure((void*)H, (uint8_t *) ec_ctx, curve);

   	CY_CHECK(musig_ctx->H->Hash_Configure((void*)H, (uint8_t *) ec_ctx, curve));

   	end:
		return error;
}

/*
cy_error_t cy_Sign_Round1(const cy_fp_t *xpriv, const cy_fp_t *random, cy_ecpoint_t *Ri)
{

 return 0;
}
*/

// L concatenation of public keys
cy_error_t cy_HashAgg(cy_musig2_ctx_t *musig_ctx, const cy_ecpoint_t *L, int index_i, uint8_t *ai)
{
  size_t i;
  cy_error_t error=CY_OK;


  uint8_t buffer[2*MAX_MUSIG_EC_T8];
  size_t t8_buffer=2*musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
 // musig_ctx->H->Hash_Init();

  for(i=0;i<musig_ctx->n_users;i++)
  {
	  CY_CHECK(cy_ec_export(&L[i], buffer, t8_buffer));

	  CY_CHECK(musig_ctx->H->Hash_Update(musig_ctx->H->ctx, buffer, t8_buffer));
  }
  CY_CHECK(cy_ec_export(&L[index_i], buffer, t8_buffer));

  musig_ctx->H->Hash_Update(musig_ctx->H->ctx, buffer, t8_buffer);
  musig_ctx->H->Hash_Final(musig_ctx->H->ctx, ai, t8_buffer);

  end:
  	return error;

}


// L concatenation of public keys
cy_error_t cy_KeyAgg(cy_musig2_ctx_t *musig_ctx, const cy_ecpoint_t *L, cy_ecpoint_t *KeyAgg)
{
  cy_ecpoint_t ec_temp, Generator, accumulator;
  cy_error_t error;
  cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
  size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

  uint8_t buffer[2*MAX_MUSIG_EC_T8];
  cy_fp_t ai;

  size_t i;

  CY_CHECK(cy_ec_alloc(ec_ctx, &ec_temp));
  CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_fp, &ai));

  for(i=0;i<musig_ctx->n_users;i++){
	  CY_CHECK(cy_HashAgg(musig_ctx, L, i, buffer));
	  CY_CHECK(cy_fp_import( buffer, t8_fp, &ai));
	  CY_CHECK(cy_ec_scalarmult_fp(&ai, &Generator, &ec_temp));
	  CY_CHECK(cy_ec_add(&accumulator, &ec_temp, &accumulator));
  }

  cy_ec_copy(&accumulator, KeyAgg);

  CY_CHECK(cy_ec_free( &ec_temp));

  end:
   	return error;
}

cy_error_t test_musig_configure(cy_hash_unit_t *H,int curve, cy_ec_ctx_t *ec_ctx, uint8_t *Ramp, size_t Ramp_t8, cy_musig2_ctx_t *musig_ctx)
{
	cy_error_t error=CY_OK;

	printf("\n\n  Test Musig2 Configure:");
	printf(" %s:", H->Component_name);fflush(stdout);

	CY_CHECK(cy_musig_configure( H, Ramp, Ramp_t8, curve, 4, ec_ctx, musig_ctx));

	end:
	if(error==CY_OK)  printf(" OK");
	else printf(" KO!!!");
			return error;
}

int test_musig_unit(uint8_t *Ramp, size_t Ramp_t8)
{
	cy_error_t error=CY_KO;
	cy_ec_ctx_t ec_ctx;


	cy_musig2_ctx_t musig_ctx;
	printf("\n\n /************************ Test Musig2 Protocol:");


	/* Initiate gda*/
	CY_CHECK(test_musig_configure(&unit_pedersen, CY_CURVE_Stark256, &ec_ctx, Ramp, Ramp_t8, &musig_ctx));
	musig_ctx.gda=&bolos_gda_component;

    CY_CHECK(test_verif_core(&musig_ctx));
	CY_CHECK(test_keygen(&musig_ctx));
	//CY_CHECK(cy_ec_uninit(&ec_ctx));

	/* configuring Musig2 with hashpedersen and StarkCurve */

	end:
	if(error==CY_OK)  printf("\n All Musig2 tests OK");
	else printf("\n  Musig2 KO");
			return error;
}





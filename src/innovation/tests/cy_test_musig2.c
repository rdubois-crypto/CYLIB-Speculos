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
#include "test_vectors/test_vector_musig2.c"

/*************************************************************/
/* Verification function									 */
/*************************************************************/
/* the two part of signatures (R,S) received from previous round, */
/* R and c being computable from aggregation of Rijs_Round1, S being output of Round2 sign'*/

cy_error_t cy_musig_Verification_Core(cy_musig2_ctx_t *ctx, cy_ecpoint_t *Key_agg,
		cy_ecpoint_t *R, cy_fp_t *s, cy_fp_t *c,
		const uint8_t *message, const size_t message_t8,
		boolean_t *flag_verif){

	size_t j ;
	cy_error_t error=CY_KO;
	*flag_verif=CY_FALSE;
	cy_ecpoint_t ec_temp1, ec_temp2, G;
	CY_CHECK(cy_ec_alloc(ctx->ctx_ec, &ec_temp1));
	CY_CHECK(cy_ec_alloc(ctx->ctx_ec, &ec_temp2));

	/* Accept iff g^s = R.X^c, beware of multiplicative notations*/
	CY_CHECK(cy_ec_alloc(ctx->ctx_ec, &G));
	CY_CHECK(cy_ec_get_generator(ctx->ctx_ec, &G));

	CY_CHECK(cy_ec_scalarmult_fp( s, &G, &ec_temp1)); 	/*g^s*/
	/* todo: c should be recomputed here internally */
	/* append R in Hashin*/

	//CY_CHECK(cy_ec_export(&ctx->ctx_ec->ctx_fp_q, R,ctx->ctx_ec->t8_modular_p,buffer));
	//ctx->H->Hash_Update((void *)ctx->H, buffer, ctx->ctx_ec->t8_modular_p);
	/* append message in Hashin*/
	//ctx->H->Hash_Update((void *)ctx->H, message, message_t8);
	//ctx->H->Hash_Final((void *)ctx->H, c); /* final c value */


	CY_CHECK(cy_ec_scalarmult_fp(Key_agg, c, &ec_temp2)); 	/*X^c*/
	CY_CHECK(cy_ec_add( &ec_temp2, R, &ec_temp2)); 	/*R.X^c*/


	/* final verification */
	CY_CHECK(cy_ec_iseq( &ec_temp1, &ec_temp2, flag_verif));

	CY_CHECK(cy_ec_free( &ec_temp1));
	CY_CHECK(cy_ec_free( &ec_temp2));

	end:
		return error;
}


/* Each signer generates a random secret key x ←$ Zp and returns the
corresponding public key X= g^x*/
cy_error_t cy_musig_KeyGen(cy_musig2_ctx_t *ctx,cy_ecpoint_t *X_pub)
{
	/*I.Declarations and allocations*/
	cy_error_t error=CY_KO;
	cy_fp_t x;
	cy_ecpoint_t G;
	cy_ec_alloc(ctx->ctx_ec, &G);
	cy_ec_get_generator(ctx->ctx_ec, &G);
	cy_ec_ctx_t  *ec_ctx= ctx->ctx_ec;

	CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &x));

	/*II. Computations*/
	/*generates a random secret key x ←$ Zp*/
	CY_CHECK(cy_fp_get_random(ctx->gda, &x));
	/* returns the corresponding public key X=g^x*/
	CY_CHECK(cy_ec_scalarmult_fp(&G, &x, X_pub));

	/*III. Free pointers*/
	CY_CHECK(cy_fp_free(&x));
	cy_ec_free(&G);

	end:
   	  return error;
}

int test_verif(cy_ec_ctx_t *ec_ctx)
{
  cy_error_t error=CY_KO;
  cy_ecpoint_t KeyAgg, R;
  cy_fp_t fp_s,fp_c;
  size_t t8_p=ec_ctx->ctx_fp_p->t8_modular;

  printf("\n Entering verif");

  CY_CHECK(cy_ec_alloc(ec_ctx, &KeyAgg));


  CY_CHECK(cy_ec_alloc(ec_ctx, &R));

  CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_p, &fp_s));

  CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_p, &fp_c));

  CY_CHECK( cy_fp_import(s, sizeof(s), &fp_s));
  CY_CHECK(cy_fp_import(c, sizeof(c), &fp_c));

  CY_CHECK( cy_ec_import2(R_x, sizeof(R_x), R_y, sizeof(R_y), &R));
  CY_CHECK( cy_ec_import2(Key_Agg_X, sizeof(Key_Agg_X), Key_Agg_Y, sizeof(Key_Agg_Y),&KeyAgg ));


  CY_CHECK(cy_ec_free( &KeyAgg));
  CY_CHECK(cy_ec_free( &R));
  CY_CHECK(cy_fp_free( &fp_s));
  CY_CHECK(cy_fp_free( &fp_c));

  end:
  	  return error;
}

int test_musig_unit()
{
	cy_error_t error=CY_KO;
	cy_ec_ctx_t ec_ctx;
	uint8_t Ramp[ _EC_ZONE_T8 ];
	size_t nb_users=4;
	cy_musig2_ctx_t musig_ctx;

	/* Initiate elliptic structure*/
	CY_CHECK(cy_ec_init(&ec_ctx, Ramp,_EC_ZONE_T8, CY_CURVE_Stark256, NULL));
	musig_ctx.ctx_ec=&ec_ctx;

	/* Initiate gda*/
	musig_ctx.gda=&bolos_gda_component;

	printf("\n\n /************************ Test Musig2 Protocol:");
	error=test_verif(&ec_ctx);

	printf("\n error=%x", error);

	end:
			return error;
}





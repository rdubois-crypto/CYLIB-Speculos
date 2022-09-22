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
/*R_x*/
 uint8_t  R_x [ 32 ]={  0x85 ,  0xb7 ,  0x68 ,  0x6d ,  0x3b ,  0x63 ,  0x56 ,  0x37 ,  0x42 ,  0xcd ,  0x55 ,  0x63 ,  0x1b ,  0xd2 ,  0x51 ,  0xec ,  0xca ,  0x51 ,  0x58 ,  0xc4 ,  0xae ,  0x1f ,  0x7 ,  0xbc ,  0xd3 ,  0xbb ,  0x7f ,  0x93 ,  0xca ,  0x72 ,  0x8d ,  0x2 };

/*R_y*/
 uint8_t  R_y [ 32 ]={  0xc5 ,  0x5c ,  0x38 ,  0x25 ,  0x86 ,  0x4b ,  0x5e ,  0xc9 ,  0xc4 ,  0xbb ,  0x11 ,  0x98 ,  0xd0 ,  0xa6 ,  0x86 ,  0x5e ,  0xf5 ,  0x73 ,  0x90 ,  0xab ,  0x21 ,  0x91 ,  0x9d ,  0x89 ,  0xbb ,  0x9a ,  0x6b ,  0x80 ,  0xb1 ,  0xd ,  0x12 ,  0x7 };

/*s part */
/*s*/
 uint8_t  s [ 32 ]={  0x73 ,  0x75 ,  0x84 ,  0x2 ,  0x21 ,  0xdd ,  0xd0 ,  0xd8 ,  0x63 ,  0xab ,  0x46 ,  0x8e ,  0x43 ,  0x27 ,  0xa4 ,  0x52 ,  0x4f ,  0xb3 ,  0xcc ,  0xe1 ,  0x71 ,  0x9a ,  0xc7 ,  0xa6 ,  0x81 ,  0x26 ,  0xc3 ,  0xdb ,  0x73 ,  0xbf ,  0x64 ,  0x2 };

/*X (agg) */
/*Key_Agg_X*/
 uint8_t  Key_Agg_X [ 32 ]={  0x21 ,  0xb0 ,  0x71 ,  0x8 ,  0x4e ,  0x32 ,  0x2a ,  0xd1 ,  0x50 ,  0x56 ,  0x52 ,  0xf0 ,  0x92 ,  0x59 ,  0xbf ,  0x16 ,  0xd6 ,  0x51 ,  0x64 ,  0xef ,  0x17 ,  0xac ,  0x4d ,  0x18 ,  0xd0 ,  0x82 ,  0x18 ,  0xb4 ,  0xf4 ,  0x9c ,  0xf3 ,  0x5 };

/*Key_Agg_Y*/
 uint8_t  Key_Agg_Y [ 32 ]={  0xe6 ,  0xd6 ,  0xdc ,  0x28 ,  0xbb ,  0xba ,  0xd1 ,  0xcc ,  0x22 ,  0x97 ,  0xb4 ,  0xdf ,  0xa ,  0x1a ,  0x85 ,  0xf0 ,  0xbe ,  0xab ,  0x56 ,  0x7f ,  0xc5 ,  0xf0 ,  0x99 ,  0x61 ,  0xb2 ,  0x1f ,  0xb7 ,  0x20 ,  0x19 ,  0x20 ,  0x22 ,  0x2 };


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


	end:
			return error;
}

/*
*Secret key user 0 :
 1094657512235374185291588483343030614863730289283862399545675407937049819358
*Public key user 0 :
 (1365852536693796327005890164729843427848958838754291756020573715938681039043 : 2551950694901895242357765230389768956726518433825247241849848339260761978359 : 1)
*Secret key user 1 :
 1576448449201733381605048531123859424271649298253787562369801966884966169831
*Public key user 1 :
 (1588309406506600313646629867090533454437316531563991043707348038842615502251 : 1150697985454088303193462262008678643749412556753225582961575084236833386272 : 1)
*Secret key user 2 :
 340766814953476881753430326832683665209763390437986951091619753285043833992
*Public key user 2 :
 (2279397190584455522887473889768616037256027913300354543529676131244040586322 : 65859339444455916268772367608859166537697428770482424173768347510080450024 : 1)
*Secret key user 3 :
 2761701262821650569617010672442900489532486297426871898323779928886284853397
*Public key user 3 :
 (438449683985570945819736421875300449043557748297596975778291853326013483269 : 3097120940526213345153316322766266977143346028904862421429379075070092085863 : 1)
*/





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



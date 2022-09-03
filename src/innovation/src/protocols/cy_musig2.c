/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois - This file is part of cy_lib project		  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_hybridsig.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: 2 round_multisignature signatures source code
 * https://eprint.iacr.org/2020/1261.pdf             */
/*	*/
/**********************************************************************************/
#include "cy_hash.h"
#include "cy_musig2.h"


/* Initialize Musig with Signatures functions, number of users, hash functions */
cy_error_t cy_musig_SetUp(cy_musig2_ctx_t *ctx,  hash_t *H_agg, hash_t *H_non, hash_t *H_sig){


}


/* sum of public keys */
cy_error_t cy_musig_KeyAgg(const size_t n_users, const cy_ec_point_t *publickeys, cy_ec_point_t *keyagg){
	size_t i;
	cy_ec_point_t keyagg;
	cy_error_t error=CY_KO;

	/* copy first pubkey in res*/
	CY_CHECK(cy_ec_copy(publickeys, keyagg));
	/* compute sum of public keys*/
	for(i=0;i<n_users;i++)
	{
		CY_CHECK(cy_ec_add(keyagg, publickeys+i, keyagg));
	}
	end:
		return error;
}

cy_error_t cy_musig_SigAgg_Round1(const size_t n_users, const cy_ec_point_t *sig, cy_ec_point_t *sigagg){
	size_t i;
	cy_ec_point_t keyagg;
	cy_error_t error=CY_KO;

	/* copy first pubkey in res*/
	CY_CHECK(cy_ec_copy(sigagg, sig));
	/* compute sum of public keys*/
	for(i=0;i<n_users;i++)
	{
		CY_CHECK(cy_ec_add(keyagg, sig+i, keyagg));
	}
	end:
		return error;
}






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

/* core function with external hashing, c=H(R||M) */
extern cy_error_t cy_Schnorr_Verification_Core(cy_ec_ctx_t *ctx_ec, cy_ecpoint_t *Key_agg,
		cy_ecpoint_t *R, cy_fp_t *fp_s, cy_fp_t *fp_c,
		boolean_t *flag_verif);

extern cy_error_t cy_Schnorr_Sign(cy_hash_unit_t *H, cy_ec_ctx_t *ctx_ec, cy_fp_t *xpriv, uint8_t *message, size_t t8_message, uint8_t *R, uint8_t *fp_s);

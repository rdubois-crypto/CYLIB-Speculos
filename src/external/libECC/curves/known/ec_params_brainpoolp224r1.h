/*
 *  Copyright (C) 2017 - This file is part of libecc project
 *
 *  Authors:
 *      Ryad BENADJILA <ryadbenadjila@gmail.com>
 *      Arnaud EBALARD <arnaud.ebalard@ssi.gouv.fr>
 *      Jean-Pierre FLORI <jean-pierre.flori@ssi.gouv.fr>
 *
 *  Contributors:
 *      Nicolas VIVET <nicolas.vivet@ssi.gouv.fr>
 *      Karim KHALFALLAH <karim.khalfallah@ssi.gouv.fr>
 *
 *  This software is licensed under a dual BSD and GPL v2 license.
 *  See LICENSE file at the root folder of the project.
 */
#include "../../lib_ecc_config.h"
#ifdef WITH_CURVE_BRAINPOOLP224R1

#ifndef __EC_PARAMS_BRAINPOOLP224R1_H__
#define __EC_PARAMS_BRAINPOOLP224R1_H__
#include "ec_params_external.h"

static const u8 brainpoolp224r1_p[] = {
	0xD7, 0xC1, 0x34, 0xAA, 0x26, 0x43, 0x66, 0x86,
	0x2A, 0x18, 0x30, 0x25, 0x75, 0xD1, 0xD7, 0x87,
	0xB0, 0x9F, 0x07, 0x57, 0x97, 0xDA, 0x89, 0xF5,
	0x7E, 0xC8, 0xC0, 0xFF
};

TO_EC_STR_PARAM(brainpoolp224r1_p);

#define CURVE_BRAINPOOLP224R1_P_BITLEN 224
static const u8 brainpoolp224r1_p_bitlen[] = { 0xE0 };

TO_EC_STR_PARAM(brainpoolp224r1_p_bitlen);

#if (WORD_BYTES == 8)		/* 64-bit words */
static const u8 brainpoolp224r1_p_normalized[] = {
	0xD7, 0xC1, 0x34, 0xAA, 0x26, 0x43, 0x66, 0x86,
	0x2A, 0x18, 0x30, 0x25, 0x75, 0xD1, 0xD7, 0x87,
	0xB0, 0x9F, 0x07, 0x57, 0x97, 0xDA, 0x89, 0xF5,
	0x7E, 0xC8, 0xC0, 0xFF, 0x00, 0x00, 0x00, 0x00
};

static const u8 brainpoolp224r1_r[] = {
	0x00, 0x00, 0x00, 0x00, 0x79, 0x36, 0x06, 0x11,
	0x1c, 0x3f, 0xb2, 0x44, 0x7b, 0x57, 0x6c, 0xf5,
	0xa8, 0xe8, 0x42, 0x8f, 0x5c, 0xbf, 0x03, 0x89,
	0xd6, 0x82, 0x78, 0x5e, 0x20, 0xb5, 0x62, 0xf7
};

static const u8 brainpoolp224r1_r_square[] = {
	0x00, 0x00, 0x00, 0x00, 0x2b, 0x3d, 0x40, 0xdd,
	0x7e, 0xa5, 0x57, 0x7c, 0x77, 0xe3, 0xd7, 0xe4,
	0x76, 0xf9, 0xee, 0x46, 0xf6, 0x91, 0xd1, 0x05,
	0x3b, 0xfb, 0xc9, 0xbf, 0x6b, 0x3d, 0x58, 0xff
};

static const u8 brainpoolp224r1_p_shift[] = {
	0x20
};

static const u8 brainpoolp224r1_mpinv[] = {
	0xE2, 0x4D, 0x21, 0x6A, 0xE1, 0x49, 0xC1, 0x01
};

static const u8 brainpoolp224r1_p_reciprocal[] = {
	0x2F, 0xC0, 0x99, 0xF7, 0x8F, 0xD2, 0x22, 0x99
};
#elif (WORD_BYTES == 4)		/* 32-bit words */
static const u8 brainpoolp224r1_p_normalized[] = {
	0xd7, 0xc1, 0x34, 0xaa, 0x26, 0x43, 0x66, 0x86,
	0x2a, 0x18, 0x30, 0x25, 0x75, 0xd1, 0xd7, 0x87,
	0xb0, 0x9f, 0x07, 0x57, 0x97, 0xda, 0x89, 0xf5,
	0x7e, 0xc8, 0xc0, 0xff
};

static const u8 brainpoolp224r1_r[] = {
	0x28, 0x3e, 0xcb, 0x55, 0xd9, 0xbc, 0x99, 0x79,
	0xd5, 0xe7, 0xcf, 0xda, 0x8a, 0x2e, 0x28, 0x78,
	0x4f, 0x60, 0xf8, 0xa8, 0x68, 0x25, 0x76, 0x0a,
	0x81, 0x37, 0x3f, 0x01
};

static const u8 brainpoolp224r1_r_square[] = {
	0x05, 0x78, 0xfd, 0x59, 0x2e, 0x6a, 0x6c, 0xe4,
	0x3f, 0xe8, 0xa2, 0xaa, 0x96, 0xaf, 0x77, 0x4c,
	0x43, 0xc2, 0x0e, 0x72, 0x78, 0x67, 0xca, 0x80,
	0x64, 0xdc, 0xd0, 0x4f
};

static const u8 brainpoolp224r1_p_shift[] = {
	0x00
};

static const u8 brainpoolp224r1_mpinv[] = {
	0xE1, 0x49, 0xC1, 0x01
};

static const u8 brainpoolp224r1_p_reciprocal[] = {
	0x2F, 0xC0, 0x99, 0xF7
};
#elif (WORD_BYTES == 2)		/* 16-bit words */
static const u8 brainpoolp224r1_p_normalized[] = {
	0xd7, 0xc1, 0x34, 0xaa, 0x26, 0x43, 0x66, 0x86,
	0x2a, 0x18, 0x30, 0x25, 0x75, 0xd1, 0xd7, 0x87,
	0xb0, 0x9f, 0x07, 0x57, 0x97, 0xda, 0x89, 0xf5,
	0x7e, 0xc8, 0xc0, 0xff
};

static const u8 brainpoolp224r1_r[] = {
	0x28, 0x3e, 0xcb, 0x55, 0xd9, 0xbc, 0x99, 0x79,
	0xd5, 0xe7, 0xcf, 0xda, 0x8a, 0x2e, 0x28, 0x78,
	0x4f, 0x60, 0xf8, 0xa8, 0x68, 0x25, 0x76, 0x0a,
	0x81, 0x37, 0x3f, 0x01
};

static const u8 brainpoolp224r1_r_square[] = {
	0x05, 0x78, 0xfd, 0x59, 0x2e, 0x6a, 0x6c, 0xe4,
	0x3f, 0xe8, 0xa2, 0xaa, 0x96, 0xaf, 0x77, 0x4c,
	0x43, 0xc2, 0x0e, 0x72, 0x78, 0x67, 0xca, 0x80,
	0x64, 0xdc, 0xd0, 0x4f
};

static const u8 brainpoolp224r1_p_shift[] = {
	0x00
};

static const u8 brainpoolp224r1_mpinv[] = {
	0xC1, 0x01
};

static const u8 brainpoolp224r1_p_reciprocal[] = {
	0x2F, 0xC0
};
#else /* unknown word size */
#error "Unsupported word size"
#endif
TO_EC_STR_PARAM(brainpoolp224r1_p_normalized);
TO_EC_STR_PARAM(brainpoolp224r1_p_shift);
TO_EC_STR_PARAM(brainpoolp224r1_r);
TO_EC_STR_PARAM(brainpoolp224r1_r_square);
TO_EC_STR_PARAM(brainpoolp224r1_mpinv);
TO_EC_STR_PARAM(brainpoolp224r1_p_reciprocal);

static const u8 brainpoolp224r1_a[] = {
	0x68, 0xA5, 0xE6, 0x2C, 0xA9, 0xCE, 0x6C, 0x1C,
	0x29, 0x98, 0x03, 0xA6, 0xC1, 0x53, 0x0B, 0x51,
	0x4E, 0x18, 0x2A, 0xD8, 0xB0, 0x04, 0x2A, 0x59,
	0xCA, 0xD2, 0x9F, 0x43
};

TO_EC_STR_PARAM(brainpoolp224r1_a);

static const u8 brainpoolp224r1_b[] = {
	0x25, 0x80, 0xF6, 0x3C, 0xCF, 0xE4, 0x41, 0x38,
	0x87, 0x07, 0x13, 0xB1, 0xA9, 0x23, 0x69, 0xE3,
	0x3E, 0x21, 0x35, 0xD2, 0x66, 0xDB, 0xB3, 0x72,
	0x38, 0x6C, 0x40, 0x0B
};

TO_EC_STR_PARAM(brainpoolp224r1_b);

static const u8 brainpoolp224r1_curve_order[] = {
	0xD7, 0xC1, 0x34, 0xAA, 0x26, 0x43, 0x66, 0x86,
	0x2A, 0x18, 0x30, 0x25, 0x75, 0xD0, 0xFB, 0x98,
	0xD1, 0x16, 0xBC, 0x4B, 0x6D, 0xDE, 0xBC, 0xA3,
	0xA5, 0xA7, 0x93, 0x9F
};

TO_EC_STR_PARAM(brainpoolp224r1_curve_order);

static const u8 brainpoolp224r1_gx[] = {
	0x0D, 0x90, 0x29, 0xAD, 0x2C, 0x7E, 0x5C, 0xF4,
	0x34, 0x08, 0x23, 0xB2, 0xA8, 0x7D, 0xC6, 0x8C,
	0x9E, 0x4C, 0xE3, 0x17, 0x4C, 0x1E, 0x6E, 0xFD,
	0xEE, 0x12, 0xC0, 0x7D
};

TO_EC_STR_PARAM(brainpoolp224r1_gx);

static const u8 brainpoolp224r1_gy[] = {
	0x58, 0xAA, 0x56, 0xF7, 0x72, 0xC0, 0x72, 0x6F,
	0x24, 0xC6, 0xB8, 0x9E, 0x4E, 0xCD, 0xAC, 0x24,
	0x35, 0x4B, 0x9E, 0x99, 0xCA, 0xA3, 0xF6, 0xD3,
	0x76, 0x14, 0x02, 0xCD
};

TO_EC_STR_PARAM(brainpoolp224r1_gy);

static const u8 brainpoolp224r1_gz[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x01
};

TO_EC_STR_PARAM(brainpoolp224r1_gz);

#define CURVE_BRAINPOOLP224R1_CURVE_ORDER_BITLEN 224
static const u8 brainpoolp224r1_gen_order[] = {
	0xD7, 0xC1, 0x34, 0xAA, 0x26, 0x43, 0x66, 0x86,
	0x2A, 0x18, 0x30, 0x25, 0x75, 0xD0, 0xFB, 0x98,
	0xD1, 0x16, 0xBC, 0x4B, 0x6D, 0xDE, 0xBC, 0xA3,
	0xA5, 0xA7, 0x93, 0x9F
};

TO_EC_STR_PARAM(brainpoolp224r1_gen_order);

#define CURVE_BRAINPOOLP224R1_Q_BITLEN 224
static const u8 brainpoolp224r1_gen_order_bitlen[] = { 0xE0 };

TO_EC_STR_PARAM(brainpoolp224r1_gen_order_bitlen);

static const u8 brainpoolp224r1_cofactor[] = { 0x01 };

TO_EC_STR_PARAM(brainpoolp224r1_cofactor);

static const u8 brainpoolp224r1_alpha_montgomery[] = {
	0x00,
};

TO_EC_STR_PARAM_FIXED_SIZE(brainpoolp224r1_alpha_montgomery, 0);

static const u8 brainpoolp224r1_gamma_montgomery[] = {
	0x00,
};

TO_EC_STR_PARAM_FIXED_SIZE(brainpoolp224r1_gamma_montgomery, 0);

static const u8 brainpoolp224r1_alpha_edwards[] = {
	0x00,
};

TO_EC_STR_PARAM_FIXED_SIZE(brainpoolp224r1_alpha_edwards, 0);

static const u8 brainpoolp224r1_oid[] = "1.3.36.3.3.2.8.1.1.5";
TO_EC_STR_PARAM(brainpoolp224r1_oid);

static const u8 brainpoolp224r1_name[] = "BRAINPOOLP224R1";
TO_EC_STR_PARAM(brainpoolp224r1_name);

static const ec_str_params brainpoolp224r1_str_params = {
	.p = &brainpoolp224r1_p_str_param,
	.p_bitlen = &brainpoolp224r1_p_bitlen_str_param,
	.r = &brainpoolp224r1_r_str_param,
	.r_square = &brainpoolp224r1_r_square_str_param,
	.mpinv = &brainpoolp224r1_mpinv_str_param,
	.p_shift = &brainpoolp224r1_p_shift_str_param,
	.p_normalized = &brainpoolp224r1_p_normalized_str_param,
	.p_reciprocal = &brainpoolp224r1_p_reciprocal_str_param,
	.a = &brainpoolp224r1_a_str_param,
	.b = &brainpoolp224r1_b_str_param,
	.curve_order = &brainpoolp224r1_curve_order_str_param,
	.gx = &brainpoolp224r1_gx_str_param,
	.gy = &brainpoolp224r1_gy_str_param,
	.gz = &brainpoolp224r1_gz_str_param,
	.gen_order = &brainpoolp224r1_gen_order_str_param,
	.gen_order_bitlen = &brainpoolp224r1_gen_order_bitlen_str_param,
	.cofactor = &brainpoolp224r1_cofactor_str_param,
	.alpha_montgomery = &brainpoolp224r1_alpha_montgomery_str_param,
	.gamma_montgomery = &brainpoolp224r1_gamma_montgomery_str_param,
	.alpha_edwards = &brainpoolp224r1_alpha_edwards_str_param,
	.oid = &brainpoolp224r1_oid_str_param,
	.name = &brainpoolp224r1_name_str_param,
};

/*
 * Compute max bit length of all curves for p and q
 */
#ifndef CURVES_MAX_P_BIT_LEN
#define CURVES_MAX_P_BIT_LEN    0
#endif
#if (CURVES_MAX_P_BIT_LEN < CURVE_BRAINPOOLP224R1_P_BITLEN)
#undef CURVES_MAX_P_BIT_LEN
#define CURVES_MAX_P_BIT_LEN CURVE_BRAINPOOLP224R1_P_BITLEN
#endif
#ifndef CURVES_MAX_Q_BIT_LEN
#define CURVES_MAX_Q_BIT_LEN    0
#endif
#if (CURVES_MAX_Q_BIT_LEN < CURVE_BRAINPOOLP224R1_Q_BITLEN)
#undef CURVES_MAX_Q_BIT_LEN
#define CURVES_MAX_Q_BIT_LEN CURVE_BRAINPOOLP224R1_Q_BITLEN
#endif
#ifndef CURVES_MAX_CURVE_ORDER_BIT_LEN
#define CURVES_MAX_CURVE_ORDER_BIT_LEN    0
#endif
#if (CURVES_MAX_CURVE_ORDER_BIT_LEN < CURVE_BRAINPOOLP224R1_CURVE_ORDER_BITLEN)
#undef CURVES_MAX_CURVE_ORDER_BIT_LEN
#define CURVES_MAX_CURVE_ORDER_BIT_LEN CURVE_BRAINPOOLP224R1_CURVE_ORDER_BITLEN
#endif

#endif /* __EC_PARAMS_BRAINPOOLP224R1_H__ */

#endif /* WITH_CURVE_BRAINPOOLP224R1 */

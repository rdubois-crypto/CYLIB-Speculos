#include "../../lib_ecc_config.h"
#ifdef WITH_CURVE_GOST_R3410_2012_512_PARAMSETC

#ifndef __EC_PARAMS_GOST_R3410_2012_512_PARAMSETC_H__
#define __EC_PARAMS_GOST_R3410_2012_512_PARAMSETC_H__
#include "../known/ec_params_external.h"
static const u8 gost_R3410_2012_512_paramSetC_p[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xc7,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_p);

#define CURVE_GOST_R3410_2012_512_PARAMSETC_P_BITLEN 512
static const u8 gost_R3410_2012_512_paramSetC_p_bitlen[] = {
	0x02, 0x00,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_p_bitlen);

#if (WORD_BYTES == 8)     /* 64-bit words */
static const u8 gost_R3410_2012_512_paramSetC_r[] = {
	0x02, 0x39,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_r);

static const u8 gost_R3410_2012_512_paramSetC_r_square[] = {
	0x04, 0xf0, 0xb1,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_r_square);

static const u8 gost_R3410_2012_512_paramSetC_mpinv[] = {
	0x58, 0xa1, 0xf7, 0xe6, 0xce, 0x0f, 0x4c, 0x09,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_mpinv);

static const u8 gost_R3410_2012_512_paramSetC_p_shift[] = {
	0x00,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_p_shift);

static const u8 gost_R3410_2012_512_paramSetC_p_normalized[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xc7,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_p_normalized);

static const u8 gost_R3410_2012_512_paramSetC_p_reciprocal[] = {
	0x00,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_p_reciprocal);

#elif (WORD_BYTES == 4)   /* 32-bit words */
static const u8 gost_R3410_2012_512_paramSetC_r[] = {
	0x02, 0x39,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_r);

static const u8 gost_R3410_2012_512_paramSetC_r_square[] = {
	0x04, 0xf0, 0xb1,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_r_square);

static const u8 gost_R3410_2012_512_paramSetC_mpinv[] = {
	0xce, 0x0f, 0x4c, 0x09,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_mpinv);

static const u8 gost_R3410_2012_512_paramSetC_p_shift[] = {
	0x00,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_p_shift);

static const u8 gost_R3410_2012_512_paramSetC_p_normalized[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xc7,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_p_normalized);

static const u8 gost_R3410_2012_512_paramSetC_p_reciprocal[] = {
	0x00,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_p_reciprocal);

#elif (WORD_BYTES == 2)   /* 16-bit words */
static const u8 gost_R3410_2012_512_paramSetC_r[] = {
	0x02, 0x39,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_r);

static const u8 gost_R3410_2012_512_paramSetC_r_square[] = {
	0x04, 0xf0, 0xb1,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_r_square);

static const u8 gost_R3410_2012_512_paramSetC_mpinv[] = {
	0x4c, 0x09,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_mpinv);

static const u8 gost_R3410_2012_512_paramSetC_p_shift[] = {
	0x00,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_p_shift);

static const u8 gost_R3410_2012_512_paramSetC_p_normalized[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xc7,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_p_normalized);

static const u8 gost_R3410_2012_512_paramSetC_p_reciprocal[] = {
	0x00,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_p_reciprocal);

#else                     /* unknown word size */
#error "Unsupported word size"
#endif

static const u8 gost_R3410_2012_512_paramSetC_a[] = {
	0xdc, 0x92, 0x03, 0xe5, 0x14, 0xa7, 0x21, 0x87,
	0x54, 0x85, 0xa5, 0x29, 0xd2, 0xc7, 0x22, 0xfb,
	0x18, 0x7b, 0xc8, 0x98, 0x0e, 0xb8, 0x66, 0x64,
	0x4d, 0xe4, 0x1c, 0x68, 0xe1, 0x43, 0x06, 0x45,
	0x46, 0xe8, 0x61, 0xc0, 0xe2, 0xc9, 0xed, 0xd9,
	0x2a, 0xde, 0x71, 0xf4, 0x6f, 0xcf, 0x50, 0xff,
	0x2a, 0xd9, 0x7f, 0x95, 0x1f, 0xda, 0x9f, 0x2a,
	0x2e, 0xb6, 0x54, 0x6f, 0x39, 0x68, 0x9b, 0xd3,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_a);

static const u8 gost_R3410_2012_512_paramSetC_b[] = {
	0xb4, 0xc4, 0xee, 0x28, 0xce, 0xbc, 0x6c, 0x2c,
	0x8a, 0xc1, 0x29, 0x52, 0xcf, 0x37, 0xf1, 0x6a,
	0xc7, 0xef, 0xb6, 0xa9, 0xf6, 0x9f, 0x4b, 0x57,
	0xff, 0xda, 0x2e, 0x4f, 0x0d, 0xe5, 0xad, 0xe0,
	0x38, 0xcb, 0xc2, 0xff, 0xf7, 0x19, 0xd2, 0xc1,
	0x8d, 0xe0, 0x28, 0x4b, 0x8b, 0xfe, 0xf3, 0xb5,
	0x2b, 0x8c, 0xc7, 0xa5, 0xf5, 0xbf, 0x0a, 0x3c,
	0x8d, 0x23, 0x19, 0xa5, 0x31, 0x25, 0x57, 0xe1,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_b);

#define CURVE_GOST_R3410_2012_512_PARAMSETC_CURVE_ORDER_BITLEN 512
static const u8 gost_R3410_2012_512_paramSetC_curve_order[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x26, 0x33, 0x6e, 0x91, 0x94, 0x1a, 0xac, 0x01,
	0x30, 0xce, 0xa7, 0xfd, 0x45, 0x1d, 0x40, 0xb3,
	0x23, 0xb6, 0xa7, 0x9e, 0x9d, 0xa6, 0x84, 0x9a,
	0x51, 0x88, 0xf3, 0xbd, 0x1f, 0xc0, 0x8f, 0xb4,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_curve_order);

static const u8 gost_R3410_2012_512_paramSetC_gx[] = {
	0xe2, 0xe3, 0x1e, 0xdf, 0xc2, 0x3d, 0xe7, 0xbd,
	0xeb, 0xe2, 0x41, 0xce, 0x59, 0x3e, 0xf5, 0xde,
	0x22, 0x95, 0xb7, 0xa9, 0xcb, 0xae, 0xf0, 0x21,
	0xd3, 0x85, 0xf7, 0x07, 0x4c, 0xea, 0x04, 0x3a,
	0xa2, 0x72, 0x72, 0xa7, 0xae, 0x60, 0x2b, 0xf2,
	0xa7, 0xb9, 0x03, 0x3d, 0xb9, 0xed, 0x36, 0x10,
	0xc6, 0xfb, 0x85, 0x48, 0x7e, 0xae, 0x97, 0xaa,
	0xc5, 0xbc, 0x79, 0x28, 0xc1, 0x95, 0x01, 0x48,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_gx);

static const u8 gost_R3410_2012_512_paramSetC_gy[] = {
	0xf5, 0xce, 0x40, 0xd9, 0x5b, 0x5e, 0xb8, 0x99,
	0xab, 0xbc, 0xcf, 0xf5, 0x91, 0x1c, 0xb8, 0x57,
	0x79, 0x39, 0x80, 0x4d, 0x65, 0x27, 0x37, 0x8b,
	0x8c, 0x10, 0x8c, 0x3d, 0x20, 0x90, 0xff, 0x9b,
	0xe1, 0x8e, 0x2d, 0x33, 0xe3, 0x02, 0x1e, 0xd2,
	0xef, 0x32, 0xd8, 0x58, 0x22, 0x42, 0x3b, 0x63,
	0x04, 0xf7, 0x26, 0xaa, 0x85, 0x4b, 0xae, 0x07,
	0xd0, 0x39, 0x6e, 0x9a, 0x9a, 0xdd, 0xc4, 0x0f,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_gy);

static const u8 gost_R3410_2012_512_paramSetC_gz[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_gz);

static const u8 gost_R3410_2012_512_paramSetC_gen_order[] = {
	0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xc9, 0x8c, 0xdb, 0xa4, 0x65, 0x06, 0xab, 0x00,
	0x4c, 0x33, 0xa9, 0xff, 0x51, 0x47, 0x50, 0x2c,
	0xc8, 0xed, 0xa9, 0xe7, 0xa7, 0x69, 0xa1, 0x26,
	0x94, 0x62, 0x3c, 0xef, 0x47, 0xf0, 0x23, 0xed,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_gen_order);

#define CURVE_GOST_R3410_2012_512_PARAMSETC_Q_BITLEN 510
static const u8 gost_R3410_2012_512_paramSetC_gen_order_bitlen[] = {
	0x01, 0xfe,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_gen_order_bitlen);

static const u8 gost_R3410_2012_512_paramSetC_cofactor[] = {
	0x04,
};

TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_cofactor);

static const u8 gost_R3410_2012_512_paramSetC_alpha_montgomery[] = {
	0x00,
};
TO_EC_STR_PARAM_FIXED_SIZE(gost_R3410_2012_512_paramSetC_alpha_montgomery, 0);

static const u8 gost_R3410_2012_512_paramSetC_gamma_montgomery[] = {
	0x00,
};
TO_EC_STR_PARAM_FIXED_SIZE(gost_R3410_2012_512_paramSetC_gamma_montgomery, 0);

static const u8 gost_R3410_2012_512_paramSetC_alpha_edwards[] = {
	0x00,
};
TO_EC_STR_PARAM_FIXED_SIZE(gost_R3410_2012_512_paramSetC_alpha_edwards, 0);

static const u8 gost_R3410_2012_512_paramSetC_name[] = "GOST_R3410_2012_512_PARAMSETC";
TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_name);

static const u8 gost_R3410_2012_512_paramSetC_oid[] = "1.2.643.7.1.2.1.2.3";
TO_EC_STR_PARAM(gost_R3410_2012_512_paramSetC_oid);

static const ec_str_params gost_R3410_2012_512_paramSetC_str_params = {
	.p = &gost_R3410_2012_512_paramSetC_p_str_param,
	.p_bitlen = &gost_R3410_2012_512_paramSetC_p_bitlen_str_param,
	.r = &gost_R3410_2012_512_paramSetC_r_str_param,
	.r_square = &gost_R3410_2012_512_paramSetC_r_square_str_param,
	.mpinv = &gost_R3410_2012_512_paramSetC_mpinv_str_param,
	.p_shift = &gost_R3410_2012_512_paramSetC_p_shift_str_param,
	.p_normalized = &gost_R3410_2012_512_paramSetC_p_normalized_str_param,
	.p_reciprocal = &gost_R3410_2012_512_paramSetC_p_reciprocal_str_param,
	.a = &gost_R3410_2012_512_paramSetC_a_str_param,
	.b = &gost_R3410_2012_512_paramSetC_b_str_param,
	.curve_order = &gost_R3410_2012_512_paramSetC_curve_order_str_param,
	.gx = &gost_R3410_2012_512_paramSetC_gx_str_param,
	.gy = &gost_R3410_2012_512_paramSetC_gy_str_param,
	.gz = &gost_R3410_2012_512_paramSetC_gz_str_param,
	.gen_order = &gost_R3410_2012_512_paramSetC_gen_order_str_param,
	.gen_order_bitlen = &gost_R3410_2012_512_paramSetC_gen_order_bitlen_str_param,
	.cofactor = &gost_R3410_2012_512_paramSetC_cofactor_str_param,
	.alpha_montgomery = &gost_R3410_2012_512_paramSetC_alpha_montgomery_str_param,
	.gamma_montgomery = &gost_R3410_2012_512_paramSetC_gamma_montgomery_str_param,
	.alpha_edwards = &gost_R3410_2012_512_paramSetC_alpha_edwards_str_param,
	.oid = &gost_R3410_2012_512_paramSetC_oid_str_param,
	.name = &gost_R3410_2012_512_paramSetC_name_str_param,
};

/*
 * Compute max bit length of all curves for p and q
 */
#ifndef CURVES_MAX_P_BIT_LEN
#define CURVES_MAX_P_BIT_LEN    0
#endif
#if (CURVES_MAX_P_BIT_LEN < CURVE_GOST_R3410_2012_512_PARAMSETC_P_BITLEN)
#undef CURVES_MAX_P_BIT_LEN
#define CURVES_MAX_P_BIT_LEN CURVE_GOST_R3410_2012_512_PARAMSETC_P_BITLEN
#endif
#ifndef CURVES_MAX_Q_BIT_LEN
#define CURVES_MAX_Q_BIT_LEN    0
#endif
#if (CURVES_MAX_Q_BIT_LEN < CURVE_GOST_R3410_2012_512_PARAMSETC_Q_BITLEN)
#undef CURVES_MAX_Q_BIT_LEN
#define CURVES_MAX_Q_BIT_LEN CURVE_GOST_R3410_2012_512_PARAMSETC_Q_BITLEN
#endif
#ifndef CURVES_MAX_CURVE_ORDER_BIT_LEN
#define CURVES_MAX_CURVE_ORDER_BIT_LEN    0
#endif
#if (CURVES_MAX_CURVE_ORDER_BIT_LEN < CURVE_GOST_R3410_2012_512_PARAMSETC_CURVE_ORDER_BITLEN)
#undef CURVES_MAX_CURVE_ORDER_BIT_LEN
#define CURVES_MAX_CURVE_ORDER_BIT_LEN CURVE_GOST_R3410_2012_512_PARAMSETC_CURVE_ORDER_BITLEN
#endif

/*
 * Compute and adapt max name and oid length
 */
#ifndef MAX_CURVE_OID_LEN
#define MAX_CURVE_OID_LEN 0
#endif
#ifndef MAX_CURVE_NAME_LEN
#define MAX_CURVE_NAME_LEN 0
#endif
#if (MAX_CURVE_OID_LEN < 20)
#undef MAX_CURVE_OID_LEN
#define MAX_CURVE_OID_LEN 20
#endif
#if (MAX_CURVE_NAME_LEN < 50)
#undef MAX_CURVE_NAME_LEN
#define MAX_CURVE_NAME_LEN 50
#endif

#endif /* __EC_PARAMS_GOST_R3410_2012_512_PARAMSETC_H__ */

#endif /* WITH_CURVE_GOST_R3410_2012_512_PARAMSETC */

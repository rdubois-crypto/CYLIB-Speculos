/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: test_hashpedersen.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: testing hash pedersen functions */
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

#include "cy_io_common_tools.h"

#include "cy_io_fp.h"

#include "cy_HashPedersen.h"

#include "test_vectors/test_vector_hashpedersen.c"

int test_verif_pedersen(cy_ec_ctx_t *ec_ctx)
{
 cy_error_t error=CY_KO;
 cy_pedersen_ctx_t ped_ctx;
 int flag=0;

 cy_fp_t fp_m0, fp_m1, fp_res, fp_expected;
 cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &fp_m0);
 cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &fp_m1);
 cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &fp_res);
 cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &fp_expected);

 cy_fp_import(tv_m0, sizeof(tv_m0), &fp_m0);
 cy_fp_import(tv_m1, sizeof(tv_m1), &fp_m1);
 cy_fp_import(core_pedersen, sizeof(core_pedersen), &fp_expected);

 CY_CHECK(pedersen_init(ec_ctx, &ped_ctx));
 CY_CHECK(pedersen(&ped_ctx, &fp_m0, &fp_m1, &fp_res));
 printf("\n post pedersen, error=%x", error);

 CY_CHECK(cy_fp_iseq(&fp_res, &fp_expected, &flag));
 printf("\n flag cmp=%d", flag);
 cy_io_fp_printMSB(&fp_res, "\n res ped");

 cy_io_fp_printMSB(&fp_expected, "\n res exp");


 cy_fp_free(&fp_m0);
 cy_fp_free(&fp_m1);
 cy_fp_free(&fp_res);
 cy_fp_free(&fp_expected);

end:
  return error;
}

int test_pedersen(uint8_t *Ramp, size_t Ramp_t8)
{

	cy_error_t error=CY_KO;
	cy_ec_ctx_t ec_ctx;

	size_t nb_users=4;

	printf("\n\n /************************ Test Pedersen Hash:");
	/* Initiate elliptic structure*/
	CY_CHECK(cy_ec_init(&ec_ctx, Ramp,_EC_ZONE_T8, CY_CURVE_Stark256, NULL));

	CY_CHECK(test_verif_pedersen(&ec_ctx));

		printf("\n post verif error=%x",(unsigned int) error);

	end:
	  return error;
}

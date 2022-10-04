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

static int test_verif_pedersen_core(cy_ec_ctx_t *ec_ctx)
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

 CY_CHECK(pedersen_configure(ec_ctx, &ped_ctx));
 CY_CHECK(pedersen(&ped_ctx, &fp_m0, &fp_m1, &fp_res));

 CY_CHECK(cy_fp_iseq(&fp_res, &fp_expected, &flag));


 if(flag!=CY_TRUE) {
	 error=CY_FALSE;
	 (cy_io_fp_printMSB(&fp_res, "\n res pedersen"));
	  (cy_io_fp_printMSB(&fp_expected, "\n res expected"));
 }

 cy_fp_free(&fp_m0);
 cy_fp_free(&fp_m1);
 cy_fp_free(&fp_res);
 cy_fp_free(&fp_expected);

 CY_CHECK(pedersen_uninit(&ped_ctx));

end:
  return error;
}


static int test_verif_pedersen_chain(cy_ec_ctx_t *ec_ctx)
{
 cy_error_t error=CY_KO;
 cy_pedersen_ctx_t ped_ctx;
 uint8_t zero[Stark_SIZE_u8]={0};
 uint8_t buffer[Stark_SIZE_u8]={0};


 int flag=0;

 cy_fp_t fp_data[2], fp_res, fp_expected;

 cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &fp_data[0]);
 cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &fp_data[1]);
 cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &fp_res);
 cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &fp_expected);

 cy_fp_import(tv_m0, sizeof(tv_m0), &fp_data[0]);
 cy_fp_import(tv_m1, sizeof(tv_m1), &fp_data[1]);
 cy_fp_import(full_pedersen, sizeof(full_pedersen), &fp_expected);

 CY_CHECK(pedersen_configure(ec_ctx, &ped_ctx));
 CY_CHECK(pedersen_hash(&ped_ctx, (cy_fp_t *) &fp_data,2,  &fp_res));

 CY_CHECK(cy_fp_iseq(&fp_res, &fp_expected, &flag));

 if(flag!=CY_TRUE) {
	 error=CY_FALSE;
	 printf("\n flag pedersen chain=%d", flag);
	 (cy_io_fp_printMSB(&fp_res, "\n res pedersen"));
	  (cy_io_fp_printMSB(&fp_expected, "\n res expected"));
	  goto end;
 }


 CY_CHECK(cy_fp_free(&fp_data[0]));
 CY_CHECK(cy_fp_free(&fp_data[1]));
 CY_CHECK(cy_fp_free(&fp_res));
 CY_CHECK(cy_fp_free(&fp_expected));

 CY_CHECK(pedersen_uninit(&ped_ctx));

 end:
  return error;
}

static int test_verif_pedersen_initupdatefinal(cy_ec_ctx_t *ec_ctx)
{
	cy_pedersen_ctx_t ped_ctx_stream;
	cy_error_t error=CY_KO;
	 cy_pedersen_ctx_t ped_ctx;
	 uint8_t zero[Stark_SIZE_u8]={0};
	 uint8_t buffer[Stark_SIZE_u8]={0};


	 int flag=0;

	 cy_fp_t fp_data[2], fp_res_stream, fp_expected;

	 cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &fp_res_stream);
	 cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &fp_expected);

	 cy_fp_import(full_pedersen, sizeof(full_pedersen), &fp_expected);

	 CY_CHECK(pedersen_configure(ec_ctx, &ped_ctx_stream));

	 CY_CHECK(pedersen_hash_init(&ped_ctx_stream, zero, sizeof(1)));
	 CY_CHECK(pedersen_hash_update(&ped_ctx_stream, tv_m0, sizeof(tv_m0)));
	 CY_CHECK(pedersen_hash_update(&ped_ctx_stream, tv_m1, sizeof(tv_m1)));
	 CY_CHECK(pedersen_hash_final(&ped_ctx_stream,  buffer, Stark_SIZE_u8));

	 CY_CHECK(cy_fp_import(buffer, sizeof(buffer), &fp_res_stream));
	 CY_CHECK(cy_fp_iseq(&fp_res_stream, &fp_expected, &flag));

	 if(flag!=CY_TRUE) {
		 error=CY_FALSE;
		 printf("\n flag pedersen chain=%d", flag);
		 (cy_io_fp_printMSB(&fp_res_stream, "\n res pedersen stream"));
		  (cy_io_fp_printMSB(&fp_expected, "\n res expected"));
	 }

	 CY_CHECK(cy_fp_free(&fp_res_stream));
	 CY_CHECK(cy_fp_free(&fp_expected));

	 CY_CHECK(pedersen_uninit(&ped_ctx_stream));

	 end:
	  return error;
}

cy_error_t test_pedersen_hashunit(cy_ec_ctx_t *ec_ctx, cy_hash_unit_t *H)
{
	cy_error_t error=CY_KO;
	size_t p_unused;
	uint8_t zero[Stark_SIZE_u8]={0};
	uint8_t buffer[Stark_SIZE_u8]={0};
	cy_pedersen_ctx_t ped_ctx_stream;
	cy_fp_t fp_data[2], fp_res_stream, fp_expected;
	int flag;

	H->ctx=(void *) &ped_ctx_stream;
	//H->ctx=(void *) &this_global_pedersen_ctx;
	 //CY_CHECK(pedersen_configure(ec_ctx, &ped_ctx_stream));

	 cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &fp_res_stream);
	 cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &fp_expected);
	 cy_fp_import(full_pedersen, sizeof(full_pedersen), &fp_expected);


	CY_CHECK(H->Hash_Configure(H, (uint8_t *) ec_ctx, p_unused));
	CY_CHECK(H->Hash_Init(H->ctx, zero, 1));
	CY_CHECK(H->Hash_Update(H->ctx, tv_m0, sizeof(tv_m0)));
	CY_CHECK(H->Hash_Update(H->ctx, tv_m1, sizeof(tv_m1)));

	CY_CHECK(H->Hash_Final(H->ctx, buffer, sizeof(buffer)));

	 CY_CHECK(cy_fp_import(buffer, sizeof(buffer), &fp_res_stream));
	 CY_CHECK(cy_fp_iseq(&fp_res_stream, &fp_expected, &flag));

	 if(flag!=CY_TRUE) {
			 error=CY_FALSE;
			 printf("\n flag pedersen chain=%d", flag);
			 (cy_io_fp_printMSB(&fp_res_stream, "\n res pedersen stream"));
			 (cy_io_fp_printMSB(&fp_expected, "\n res expected"));
		 }

	end:
	  return error;
}


int test_pedersen(uint8_t *Ramp, size_t Ramp_t8)
{

	cy_error_t error=CY_KO;
	cy_ec_ctx_t ec_ctx;



	printf("\n\n /************************ Test Pedersen Hash:");
	/* Initiate elliptic structure*/
	CY_CHECK(cy_ec_init(&ec_ctx, Ramp,Ramp_t8, CY_CURVE_Stark256, NULL));

	printf("\n test  pedersen core hash:");
	CY_CHECK(test_verif_pedersen_core(&ec_ctx));
	printf(" OK");

	printf("\n test  pedersen chain:");
	CY_CHECK(test_verif_pedersen_chain(&ec_ctx));
	printf(" OK");

	printf("\n test  pedersen init/update/final:");
	CY_CHECK(test_verif_pedersen_initupdatefinal(&ec_ctx));
	printf(" OK");

	printf("\n test  <CYLIB Pedersen:Hash Unit> :");
	test_pedersen_hashunit(&ec_ctx, &unit_pedersen);
	printf(" OK");

	/* tpdo: investigate cy uninit*/
	(cy_ec_uninit(&ec_ctx));

	end:
	  if(error!=CY_OK){
	  printf("Global error pedersen=%x", (unsigned int) error);
	  }
	  else
	  {
		  printf("\n All Pedersen tests OK");
	  }

	  return error;
}

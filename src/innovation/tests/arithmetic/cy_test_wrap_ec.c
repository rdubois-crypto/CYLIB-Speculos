/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: test_wrap_fp_ec.c
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

#include "cy_io_common_tools.h"


static int test_ec_leak(cy_ec_ctx_t *ctx, uint8_t *Ramp, size_t sizeRam)
{

	 cy_error_t error=CY_KO;
	  size_t i;

	  #define _LEAK_TEST 10
	  cy_ecpoint_t leak[_LEAK_TEST];
	  printf("\n test_ec leakage:");
	  UNUSED(Ramp);
	  UNUSED(sizeRam);

	  for(i=0;i<_LEAK_TEST;i++)
	  {

		// print_MsbString(Ramp, sizeRam, "\nRAMp pre alloc:\n");
		  // printf("\n--- offset prealloc= %d", (int) (ctx->ctx_fp_p->offset ));

		 CY_CHECK( cy_ec_alloc(ctx,  &leak[i]));
		 //printf("\n--- offset post alloc= %d", (int) (ctx->ctx_fp_p->offset ));
		 //print_MsbString(Ramp, sizeRam, "\nRAMp post alloc:\n");

		 //print_MsbString(Ramp, sizeRam, "\nRAMp post alloc:\n");

		// printf("\n @allocated =%x %x %x", leak[i].ec->x, leak[i].ec->y, leak[i].ec->z);
		 CY_CHECK( cy_ec_free(& leak[i]));

		 //print_MsbString(Ramp, sizeRam, "\nRAMp post free:\n");
		 //printf("\n--- offset post free= %d", (int) (ctx->ctx_fp_p->offset ));

	  }

	  printf(" OK");
	  end:
	    return error;

   // end

	   return error;
}

static int test_ec_parameters( char *name, uint8_t *Ramp, size_t sizeRam, cx_testvec_weierstrass_t const *C_cy_allCurves)
{
	 cy_error_t error = CY_OK;
	 cy_ec_ctx_t ctx;

	  /* The shared ram between program and library*/
	// print_MsbString(Ramp, sizeRam, "\nRAMp:\n");

	  debug_printf("\n------------------ @RAMP=%x\n sizeRamp=%x curve=", (unsigned int)Ramp,(int)sizeRam);
	  debug_Print_RAMp(Ramp, sizeRam);
	  cx_curve_t curve=C_cy_allCurves->curve;

	  debug_Print_RAMp(Ramp, sizeRam);
	  UNUSED(name);
	  /* Initializing the fp unit*/
	  CY_CHECK(cy_ec_init(&ctx, Ramp, sizeRam, curve, NULL));
	  printf("\n %s %s : ", ctx.libname, C_cy_allCurves->curve_name);

	  _debug(printf("\n after ec Init:"));
	  CY_CHECK(test_ec_leak(&ctx, Ramp, sizeRam));

	  debug_Print_RAMp(Ramp, sizeRam);

	 // print_MsbString(Ramp, sizeRam, "\nRAMp pre-uninit:\n");

	  CY_CHECK(cy_ec_uninit(&ctx));

	  _debug(printf("\n after ec UnInit:"));
	 // print_MsbString(Ramp, sizeRam, "\nRAMp uninit:\n");

	  debug_Print_RAMp(Ramp, sizeRam);
	  _debug(printf("\n exiting"));

	  if (error == CY_OK)
		    printf(" OK !");
		  else
		    printf(" KO\n");

	 end:
	 // print_MsbString(Ramp, sizeRam, "\nRAMp post uninit:\n");

	   return error;
}

static cy_error_t test_ec_unit(uint8_t *Ramp, size_t Ramp_t8)
{
	cy_error_t error = CY_OK;
	size_t i;
	size_t nb_supported=sizeof(C_cy_allCurves)/sizeof(cx_testvec_weierstrass_t *);

	printf("\n/************************ Test EC Unit:");

	printf("\n nb supported curves=%d", nb_supported);

    const uint8_t *argv_gen[]={NULL, NULL};
    UNUSED(argv_gen); //to make compiler happy
    if(nb_supported>5) nb_supported=5;

	for(i=0;i<nb_supported;i++){

	    argv_gen[0]=C_cy_allCurves[i]->t8_size;
	    argv_gen[1]=C_cy_allCurves[i]->p;

	    if(C_cy_allCurves[i]->curve!=CY_CURVE_BLS12_381_G1) //not supported by speculos emulator
	    	CY_CHECK(test_ec_parameters(C_cy_allCurves[i]->curve_name, Ramp, Ramp_t8, C_cy_allCurves[i]));
	}

	end:
	  if (error == CY_OK)
			    printf("\n All EC tests OK !");
			  else
			    printf(" KO\n");

	  return error;
}


#ifndef _TEST_ALL
int main()
{
	 uint8_t Ramp[_MAX_MEMORY];

     cy_error_t error = CY_OK;
	 CY_CHECK(test_ec_unit(Ramp, _MAX_MEMORY));

	 return error;
}
#endif







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
/* DESCRIPTION: testing Musig 2 FSM (finite state machine) */
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



#include "cy_io_common_tools.h"
#include "cy_io_ec.h"
#include "cy_ec_const.h"
#include "cy_HashPedersen.h"
#include "cy_musig2.h"

#include "test_vectors/test_vector_musig2.c"
#include "cy_musig2_fsm.c"


int test_musig_fsm(uint8_t *Ramp, size_t Ramp_t8)
{
	cy_error_t error=CY_OK;
	cy_ec_ctx_t ec_ctx;

	cy_musig2_ctx_t musig_ctx;
	printf("\n\n /************************ Test Musig2 State Machine:");



	end:
	if(error==CY_OK)  printf("\n All Musig2 tests OK");
	else printf("\n  Musig2 KO with error=%x",(unsigned int) error);
			return error;
}

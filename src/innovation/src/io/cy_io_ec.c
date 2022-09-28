/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_io_fp.c */
/*               										   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: IO for fp */
/**********************************************************************************/

#include <stdint.h>
#include <stddef.h>

#include <stdio.h>
#include "cy_configuration.h"
#include "cy_fp.h"
#include "cy_ec.h"
#include "cy_io_common_tools.h"
#include "cy_io_fp.h"

#define _MAX_EC_T8 1024*3

/* display the value of a fp in msb*/
cy_error_t cy_io_ec_printMSB(const cy_ecpoint_t *in, char *comment)
{
  uint8_t display[_MAX_EC_T8];

  cy_error_t error=CY_KO;

  printf("export of size %d", (int) in->ctx->ctx_fp_p->t8_modular);
  printf("\n values of bn:x=%d, y=%d, z=%d", (int) in->ec->x, (int) in->ec->y, (int) in->ec->z);


  CY_CHECK(cy_ec_export(in,  display, in->ctx->ctx_fp_p->t8_modular));

  printf("\n done");

  print_MsbString(display,  2*in->ctx->ctx_fp_p->t8_modular, comment );



  end:
  	  return error;
}

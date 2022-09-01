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

/* this function must be instantiated for the target*/
#define cy_print(...) printf(_VA_ARGS)

#include <stdint.h>
#include <stddef.h>

#include <stdio.h>
#include "cy_configuration.h"
#include "cy_fp.h"
#include "cy_io_common_tools.h"

/* display the value of a fp in msb*/
cy_error_t cy_io_fp_printMSB(const cy_fp_t *in, char *comment)
{
  uint8_t display[_MAX_FP_T8];

  cy_error_t error=CY_KO;

  CY_CHECK(cy_fp_export(in,  display, in->ctx->t8_modular));

  print_MsbString(display,  in->ctx->t8_modular, comment );



  end:
  	  return error;
}


/* display the raw montgomery value of a fp in msb*/

/* normalized then display a montgomery fp value in msb*/

/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_memory_unit.c */
/*               										   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Memory unit emulation */
/**********************************************************************************/
#include "cy_def.h"
#include "cy_errors.h"
#include "cy_memory_unit.h"
#include "cxlib.h"

enum cy_nano_list{
  NANO_S=1,
  NANO_X=2,
  NANO_SP=3,
  NANO_STAX=4
};

struct ec_ctx_s {
  cy_flag_t is_initialized;
  int nano_id;				//choose value in nano enum
  cy_mem_ctx_t *Nano_mem;

};

typedef struct ec_ctx_s cy_nanoX_ctx_t;

cy_mem_ctx_t NanoX_mem={
		 NULL,     // pointeur to a memory of ST33_RAM bytes
		 ST33_RAM, // total size allocated to the library
		  16, // minimal size of allocation for alignment, can be
		  O, // current offset in bytes from starting stack
		  CY_LIB_UNINITIALIZED,
		  NULL,//pointeur to the nano private key
};

cy_error_t os_export_State(uint8_t *svg, size_t t8_svg){

	if(t8_svg<sizeof(cx_mpi_array))
		return CY_ERR_LENGTH;

	memcpy(svg, cx_mpi_array, sizeof(cx_mpi_array));

};

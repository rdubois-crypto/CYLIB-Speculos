/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois											  */
/* This file is part of cy_lib project						 					  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.				 			  */
/* FILE: cy_wrap_bolos_bn.c								   		                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Multiprecision Integers API                                       */
/**********************************************************************************/

#include <stddef.h>

#include "bolos/cxlib.h"


#include "cy_def.h"
#include "cy_errors.h"

#include "cy_wrap_fp_bolos.h"
#include "cy_fp.h"

#include "cy_wrap_bolos_bn.h"
#include "cy_bn.h"


/* beware to destroy it !*/
cy_error_t cy_bn_import( const uint8_t *in, size_t bn_t8, cy_bn_t *out ){
	cy_error_t error;

			CY_CHECK(sys_cx_bn_init(*out, in,bn_t8));

			end:
				return error;

}


cy_error_t cy_wrap_bolos_bn_or( const cx_bn_t *a, const cx_bn_t *b, cx_bn_t *r){
	cy_error_t error;

		CY_CHECK(sys_cx_bn_or(*r, *a,*b));

		end:
			return error;

}


cy_error_t cy_wrap_bolos_bn_and( const cx_bn_t *a, const cx_bn_t *b, cx_bn_t *r){
	cy_error_t error;

		CY_CHECK(sys_cx_bn_and(*r, *a,*b));

		end:
			return error;

}

cy_error_t cy_wrap_bolos_bn_shift_r(const size_t n, cx_bn_t *r){

	cy_error_t error;

	CY_CHECK(sys_cx_bn_shr(*r, (uint32_t) n));

	end:
		return error;
}


cy_error_t cy_wrap_bolos_bn_sub(const cy_bn_t *a, const cy_bn_t *b, cy_bn_t *out)
{
  cy_error_t error = CY_KO;

  CX_CHECK(sys_cx_bn_sub(*out, *a, *b));

end:
  return error;
}


cy_error_t cy_wrap_bolos_bn_modadd(const cy_bn_t *a, const cy_bn_t *b, const cy_bn_t *modulo, cy_bn_t *out)
{
  cy_error_t error = CY_KO;

  CX_CHECK(sys_cx_bn_mod_add(*out, *a, *b,  *modulo));

end:
  return error;
}


cy_error_t cy_wrap_bolos_bn_modmul(const cy_bn_t *a, const cy_bn_t *b, const cy_bn_t *modulo, cy_bn_t *out)
{
  cy_error_t error = CY_KO;

  CX_CHECK(sys_cx_bn_mod_mul(*out, *a, *b,  *modulo));

end:
  return error;
}

cy_error_t cy_wrap_bolos_bn_mod(const cy_bn_t *a, const cy_bn_t *modulo, cy_bn_t *out )
{
	cy_error_t error = CY_KO;

    CX_CHECK(sys_cx_bn_reduce(*out, *a, *modulo));

	end:
	  return error;

}


/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_wrap_bolos_felt.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: bolos wrappers for cy_lib, header file */
/**********************************************************************************/

#include "bolos/cxlib.h"
#include "cy_errors.h"

#include "cy_speculos.c"

#include "cy_field_unit.h"

#define _STATE_UNINIT 0xfe
#define _STATE_INIT 0xfd

#define _TYPE_BOLOS_BN 0x1
#define _TYPE_BOLOS_MONTGOMERY 0x2
#define _TYPE_BOLOS_QUAD 0x3


#define _FIELD_MODULUS 0
#define _FIELD_NOTSQUARE_FP2 1
#define _FIELD_NOTSQUARE_FP6 1

#define _MEM_RESERVED _MEM_ALLOCATED


/* the montgomery context is the first element of the RAMp*/
#define _MAP_CTX_MONTG 0

#define get_address_bn(ctx_mem) (cx_bn_t *) cy_malloc(ctx_mem, sizeof(cx_bn_t))




cy_error_t wrap_bolos_mp_init(cy_field_ctx_t *self, cy_mem_ctx_t *MemP, uint8_t *characteristic,
        size_t t8_characteristic)
{
 cy_error_t error=CY_OK;
 self->MemP=MemP;
 self->state=_STATE_INIT;

 UNUSED(characteristic);

 CY_CHECK(sys_cx_bn_lock(t8_characteristic, 0) != CX_OK);

 goto end;

  end:
	 return error;
}


cy_error_t wrap_bolos_mp_uninit(cy_field_ctx_t *self)
{
 cy_error_t error=CY_OK;

 self->state=_STATE_UNINIT;

 CX_CHECK(sys_cx_bn_unlock());

  end:
	 return error;
}

cy_error_t wrap_bolos_mp_alloc(cy_felt_t *out, cy_field_ctx_t *self)
{
 cy_error_t error=CY_KO;

  out->inner = get_address_bn(self->MemP);
  CX_CHECK(sys_cx_bn_alloc((cx_bn_t *)out->inner, self->t8_felt));
  self->state=_STATE_INIT;


 end:
 return error;

}


cy_error_t wrap_bolos_mp_free(cy_felt_t *out)
{
 cy_error_t error=CY_KO;

  CX_CHECK(sys_cx_bn_destroy((cx_bn_t *)out->inner));

  CX_CHECK(cy_free(out->ctx->MemP, out->inner, sizeof(cx_bn_t)));

  out->state=_STATE_UNINIT;

 end:
 return error;

}

cy_error_t wrap_bolos_felt_init(cy_field_ctx_t *self, cy_mem_ctx_t *MemP, uint8_t *characteristic,
        size_t t8_characteristic)
{
 cy_error_t error=CY_OK;
 self->MemP=MemP;
 self->state=_STATE_INIT;
 cx_bn_t bn_p;


/* Initialize multiprecision */
 self->SubField->Init(self->SubField, MemP, characteristic, t8_characteristic);
 self->t8_felt=t8_characteristic;
 self->Precalc=MemP->Shared_Memory+ MemP->offset;

 self->state=_STATE_INIT;
 if (sys_cx_bn_alloc_init(&bn_p, t8_characteristic,characteristic,
		 t8_characteristic) !=
       CX_OK) /* allocating and initializing big number a*/
   {
 	  CY_ERROR_RETURN(CY_ERR_ALLOC);
   }

 CY_CHECK(cy_mont_alloc(self->Precalc,
		 t8_characteristic));

 CY_CHECK(cy_mont_init((cy_bn_mont_ctx_t *) self->Precalc, bn_p));
 self->modulus= (void *) (&((cy_bn_mont_ctx_t *) (self->Precalc))->n);


 self->MemP->offset+=sizeof(cy_bn_mont_ctx_t);


  end:
	 return error;
}

cy_error_t wrap_bolos_convert(cy_felt_t *out, const cy_felt_t *in)
{
	cy_error_t error=CY_KO;
	cy_field_ctx_t *ctx;

	if((out->ctx->type==_TYPE_BOLOS_BN)&&(in->ctx->type==_TYPE_BOLOS_MONTGOMERY))
	{
		ctx = in->ctx;
		cy_mont_from_montgomery(*(cx_bn_t *)(out->inner), *(cx_bn_t *)(in->inner),
			                                  ( cx_bn_mont_ctx_t *) ctx->Precalc);
	}

	if((in->ctx->type==_TYPE_BOLOS_BN)&&(out->ctx->type==_TYPE_BOLOS_MONTGOMERY))
	{
		ctx = out->ctx;
		CY_CHECK(cx_mont_to_montgomery(*(cx_bn_t *)(out->inner), *(cx_bn_t *)(in->inner), ctx->Precalc));
	}
	 end:
		 return error;
}


cy_error_t wrap_bolos_felt_import(cy_felt_t *out, const uint8_t *in, const size_t t8_in)
{
	cy_error_t error = CY_KO;

	cy_field_ctx_t *ctx = out->ctx;

	 CY_CHECK(sys_cx_bn_init(*(cx_bn_t *)(out->inner), in, t8_in));

	 CY_CHECK(cx_mont_to_montgomery(*(cx_bn_t *)(out->inner), *(cx_bn_t *)(out->inner), ctx->Precalc));

	end:
	  return error;
}

cy_error_t wrap_bolos_felt_uninit(cy_field_ctx_t *self){
	cy_error_t error=CY_OK;
	size_t i;

	CY_CHECK(cy_mont_destroy((cy_bn_mont_ctx_t *) self->Precalc));

	for(i=0;i<4*sizeof(cx_bn_t );i++)
	{
	 	  ((uint8_t *) self->Precalc)[i]=_MEM_RESERVED;
	}

	CY_CHECK(wrap_bolos_mp_uninit(self->SubField));

	 end:
		 return error;
}


/* big number elements */
cy_field_ctx_t g_field_bolos_bn={
	"< bolos:big number unit>",
	0,
	_TYPE_BOLOS_BN,
	NULL,
	(Fct_Field_Init_t) wrap_bolos_mp_init, //Init;
	(Fct_Field_Alloc_t) wrap_bolos_mp_alloc, //Alloc;
	(Fct_Field_Fe2_t) NULL, //Add;
	(Fct_Field_Fe2_t) NULL, //Mul;
	(Fct_Field_Fe2_t) NULL, //Div;
	(Fct_Field_Fe_t) NULL, //Sqrt;
	(Fct_Field_Fe_t) wrap_bolos_mp_free, //Free;
	(Fct_Field_UnInit_t) NULL, //UnInit;
	(Fct_Field_FeU8_t) NULL,

	(Fct_Field_Fe2_t) wrap_bolos_convert, // Convert
	(cy_field_ctx_t *) NULL, //*SubField;
	(cy_mem_ctx_t *) NULL, //*MemP;
	(void *) NULL, //Precalc;
	_STATE_UNINIT
};

/* Montgomery representation elements */
cy_field_ctx_t g_field_bolos_fp={
	"< bolos:montgomery unit>",
	0,
	_TYPE_BOLOS_MONTGOMERY,
	NULL,
	(Fct_Field_Init_t) wrap_bolos_felt_init, //Init;
	(Fct_Field_Alloc_t) wrap_bolos_mp_alloc, //Alloc;
	(Fct_Field_Fe2_t) NULL, //Add;
	(Fct_Field_Fe2_t) NULL, //Mul;
	(Fct_Field_Fe2_t) NULL, //Div;
	(Fct_Field_Fe_t) NULL, //Sqrt;
	(Fct_Field_Fe_t) NULL, //Free;
	(Fct_Field_UnInit_t) NULL, //UnInit;
	(Fct_Field_FeU8_t) NULL, //getfield

	(Fct_Field_Fe2_t) wrap_bolos_convert,// Convert
	(cy_field_ctx_t *) &g_field_bolos_bn, //*SubField;
	(cy_mem_ctx_t *) NULL, //*MemP;
	(void *) NULL, //Precalc;
	_STATE_UNINIT
};


/* Quadratic representation elements */
cy_field_ctx_t g_field_bolos_quad={
	"< bolos:montgomery unit>",
	0,
	_TYPE_BOLOS_QUAD,
	NULL,
	(Fct_Field_Init_t) NULL, //Init;
	(Fct_Field_Alloc_t) NULL, //Alloc;
	(Fct_Field_Fe2_t) NULL, //Add;
	(Fct_Field_Fe2_t) NULL, //Mul;
	(Fct_Field_Fe2_t) NULL, //Div;
	(Fct_Field_Fe_t) NULL, //Sqrt;
	(Fct_Field_Fe_t) NULL, //Free;
	(Fct_Field_UnInit_t) NULL, //UnInit;
	(Fct_Field_FeU8_t) NULL, //getfield

	(Fct_Field_Fe2_t) NULL,// Convert
	(cy_field_ctx_t *) &g_field_bolos_fp, //*SubField;
	(cy_mem_ctx_t *) NULL, //*MemP;
	(void *) NULL, //Precalc;
	_STATE_UNINIT
};

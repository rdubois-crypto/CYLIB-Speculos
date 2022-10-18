/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_wrap_bolos.h
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: bolos wrappers for cy_lib prime fied arithmetic */
/**********************************************************************************/


#include <stddef.h>
#include <stdio.h>

#include "cy_def.h"
#include "cy_errors.h"
#include "bolos/cxlib.h"

#include "cy_wrap_fp_bolos.h"
#include "cy_fp.h"

#include "cy_speculos.c"


/*****************************************************************************/
/*	II. Wrapping bolos calls to Common APIs  and Memory handling
 */
/*****************************************************************************/

/* The syntax of a bolos initializer is
 *
 * argv[0]:[0][1][2]...[WordByteSize-1]: bytesize of modulus
 * argv[1]:[WordByteSize...] : MSB asn1 encoding of modulus
 *
 * memory mapping of the library
 * 1: n index in SE, modulus, MSB encoding
 * 2: h index in SE, first montgomery constant
 * 3: mpinv index in SE,, second montgomery constant= -p^-1 mod
 * 2^(bitsizeof(word_t))
 */

cy_error_t wrap_bolos_fp_init(cy_fp_ctx_t *ps_ctx, uint8_t *pu8_Mem,
                              const size_t t8_Memory, const int argc,
                              const uint8_t *argv[])
{

  CY_ERROR_INIT();

  size_t i;
  cx_bn_t bn_p;

  if (argc != 2) {
    return CY_KO;
  }

 // printf("\n required mem fp init:%d", (int) t8_Memory);

  /*control memory limit*/
  if (t8_Memory > _MAX_MEMORY) {
    error = CY_KO;
    goto end;
  }
  ps_ctx->t8_allocated_max=t8_Memory;

  for(i=0;i<t8_Memory;i++) pu8_Mem[i]=_MEM_FP_RESERVED;

  ps_ctx->Shared_Memory = pu8_Mem ; /*set Memory*/

  ps_ctx->montgomery_ctx = (void *)(ps_ctx->Shared_Memory);
  ps_ctx->modular = (void *)(ps_ctx->Shared_Memory);

  strcpy(ps_ctx->libname, BOLOS_FP_LIBNAME);
  ps_ctx->is_initialized = CY_LIB_INITIALIZED;

  ps_ctx->t8_modular = word32_from_be(argv[0]);
  // printf("\n modular size=%d", (int) ps_ctx->t8_modular);

  /*
  for(i=0;i<ps_ctx->t8_modular;i++) pu8_Mem[i+offset]=argv[1][i];
  offset+=i;*/

  /* todo: endian independant read of argv0*/
  if (sys_cx_bn_lock(ps_ctx->t8_modular, 0) != CX_OK) {
    return CY_KO;
  }

  if (sys_cx_bn_alloc_init(&bn_p, ps_ctx->t8_modular, argv[1],
                           ps_ctx->t8_modular) !=
      CX_OK) /* allocating and initializing big number a*/
  {
	  CY_ERROR_RETURN(CY_ERR_ALLOC);
  }

  CY_CHECK(cy_mont_alloc((cy_bn_mont_ctx_t *)ps_ctx->montgomery_ctx,
                         ps_ctx->t8_modular));

  CY_CHECK(cy_mont_init((cy_bn_mont_ctx_t *)ps_ctx->montgomery_ctx, bn_p));
  ps_ctx->offset = 4 * sizeof(bn_p);

  /* IV. Free*/
  sys_cx_bn_destroy(&bn_p);

  ps_ctx->is_initialized = CY_LIB_INITIALIZED;
  error = CY_OK;

  CY_ERROR_CLOSE();
}

/* TODO: developp a container to import/export ciphered containers
cy_error_t wrap_bolos_fp_write(cy_fp_ctx_t *ctx, uint8_t *Mem
__attribute__((unused)), int argc, uint8_t **argv)
{


}


cy_error_t wrap_bolos_fp_read(cy_fp_ctx_t *ctx, uint8_t *Mem
__attribute__((unused)), int argc, uint8_t **argv)
{


}*/

cy_error_t wrap_bolos_fp_alloc(cy_fp_ctx_t *ctx, size_t t8_r, cy_fp_t *r)
{
  cy_error_t error = CY_KO;

  if (ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;
    goto end;
  }

  if((ctx->offset+sizeof(cx_bn_t *))> ctx->t8_allocated_max )
  {
	  error=CY_MEM_OVERFLOW;
	  goto end;
  }

  r->ctx = ctx;
  r->index =  ctx->offset;
  r->bn = (cx_bn_t *)(ctx->Shared_Memory + ctx->offset);


  /*printf("\n %s allocating from %x with offset=%x to %x, t8_r=%d", ctx->libname,
         (unsigned int)ctx->Shared_Memory, ctx->offset, (unsigned int)r->index,
         (int)t8_r);*/
  //printf("\n allocating from @%x with value %x", (unsigned int) r->bn, (unsigned int) *(r->bn));

  CX_CHECK(sys_cx_bn_alloc(r->bn, t8_r));
  //UNUSED(t8_r);
  //CX_CHECK(sys_cx_bn_alloc(r->bn, 16));
 // printf("\n allocating bn:%x at @%x", (unsigned int) (*r->bn), (unsigned int) r->bn);

  ctx->offset += sizeof(cx_bn_t *);
  r->is_initialized=CY_LIB_ALLOCATED;
  error = CY_OK;

end:
  return error;
}

cy_error_t wrap_bolos_fp_free(cy_fp_t *r)
{
  size_t i;
  cy_error_t error = CY_KO;
   cy_fp_ctx_t *ctx = r->ctx;
   CY_IS_INIT(ctx);



  // printf("\n destroy bn:%x at @%x", (unsigned int) (*r->bn), (unsigned int) r->bn);

   CX_CHECK(sys_cx_bn_destroy(r->bn));

  for(i=0;i<sizeof(cx_bn_t *);i++)
  {
	  *(ctx->Shared_Memory+r->index+i)=_MEM_FP_RESERVED;
  }


  if (r->index == ( ctx->offset)) {
    ctx->offset -= sizeof(cx_bn_t *);
  }


  r->index = CY_LIB_UNINITIALIZED;

  end:
    return error;
  //return CY_OK;
}

cy_error_t wrap_bolos_fp_import(const uint8_t *in, size_t t8_in, cy_fp_t *out)
{
  cy_error_t error = CY_KO;
  cy_fp_ctx_t *ctx = out->ctx;
  CY_IS_INIT(ctx);

  CX_CHECK(sys_cx_bn_init(*(out->bn), in, t8_in));

  error = CY_OK;

end:
  return error;
}


cy_error_t wrap_bolos_fp_copy(const cy_fp_t *in, cy_fp_t *out)
{
  cy_error_t error = CY_KO;
  cy_fp_ctx_t *ctx = out->ctx;
  CY_IS_INIT(ctx);

  CX_CHECK(sys_cx_bn_copy(*(out->bn), *(in->bn)));

  error = CY_OK;

end:
  return error;
}

cy_error_t wrap_bolos_fp_export(const cy_fp_t *in, uint8_t *out , size_t t8_out)
{
  cy_error_t error = CY_KO;
  cy_fp_ctx_t *ctx = in->ctx;

  CY_IS_INIT(ctx);

  CX_CHECK(sys_cx_bn_export(*(in->bn),   out, t8_out));

end:
  return error;
}

cy_error_t wrap_bolos_fp_add(cy_fp_t *a, cy_fp_t *b, cy_fp_t *out)
{
  cy_error_t error = CY_KO;
  cy_fp_ctx_t *ctx = a->ctx;

  CY_IS_INIT(ctx);

  CX_CHECK(sys_cx_bn_mod_add(*(out->bn), *(a->bn), *(b->bn),
                             *((cx_bn_t *)ctx->modular)));

end:
  return error;
}

cy_error_t wrap_bolos_fp_neg(const cy_fp_t *a,  cy_fp_t *out)
{
	 cy_error_t error = CY_KO;
	  cy_fp_ctx_t *ctx = a->ctx;

	  CY_IS_INIT(ctx);
	  CX_CHECK(cy_mod_neg(*(out->bn), *(a->bn),
              *((cx_bn_t *)ctx->modular)));
	  end:
	    return error;
}


cy_error_t wrap_bolos_fp_sub(const cy_fp_t *a, const cy_fp_t *b, cy_fp_t *out)
{
  cy_error_t error = CY_KO;
  cy_fp_ctx_t *ctx = a->ctx;

  CY_IS_INIT(ctx);

  CX_CHECK(sys_cx_bn_mod_sub(*(out->bn), *(a->bn), *(b->bn),
                             *((cx_bn_t *)ctx->modular)));

end:
  return error;
}


cy_error_t wrap_bolos_fp_cmp(const cy_fp_t *a, cy_fp_t *b, boolean_t *out)
{

	 cy_error_t error = CY_KO;
	  cy_fp_ctx_t *ctx = a->ctx;

	  CY_IS_INIT(ctx);

	  CX_CHECK(sys_cx_bn_cmp(*(b->bn), *(a->bn),out));

	end:
	  return error;
}

cy_error_t wrap_bolos_fp_mul(const cy_fp_t *a, const 	cy_fp_t *b, cy_fp_t *out)
{
  cy_error_t error = CY_KO;
  cy_fp_ctx_t *ctx = a->ctx;

  CY_IS_INIT(ctx);

  CX_CHECK(sys_cx_bn_mod_mul(*(out->bn), *(a->bn), *(b->bn),
                             *((cx_bn_t *)ctx->modular)));

end:
  return error;
}

#define _ZERO_U32 ((uint32_t) 0)

cy_error_t wrap_bolos_fp_set_zero(cy_fp_t *out)
{
	 cy_error_t error = CY_KO;

	  CX_CHECK(sys_cx_bn_set_u32(*(out->bn), _ZERO_U32));

		end:
		  return error;

}

cy_error_t wrap_bolos_fp_sqr(const cy_fp_t *a, cy_fp_t *out)
{
	 cy_error_t error = CY_KO;
	  cy_fp_ctx_t *ctx = a->ctx;

	  CY_IS_INIT(ctx);

	  CX_CHECK(sys_cx_bn_mod_mul(*(out->bn), *(a->bn), *(a->bn),
	                             *((cx_bn_t *)ctx->modular)));
	end:
	  return error;

}

cy_error_t wrap_bolos_fp_inv(cy_fp_t *in, cy_fp_t *out)
{
  cy_error_t error = CY_KO;
  cy_fp_ctx_t *ctx = in->ctx;

  CY_IS_INIT(ctx);

  CX_CHECK(cx_bn_mod_invert_nprime(*(out->bn), *(in->bn), *((cx_bn_t *)ctx->modular)));

end:
  return error;
}

/* untested*/
cy_error_t wrap_bolos_fp_mult_mont(cy_fp_t *a, cy_fp_t *b, cy_fp_t *out)
{
	 cy_error_t error = CY_KO;
	  cy_fp_ctx_t *ctx = a->ctx;


	  CY_IS_INIT(ctx);
	  CX_CHECK(cy_mont_mul(*(out->bn), *(a->bn), *(b->bn), ((cy_bn_mont_ctx_t *)ctx->montgomery_ctx)));

	  UNUSED(b);
	  UNUSED(out);
	end:
	  return error;

}


/* convert normalized in_mont to  out_mont montgomery representation*/
cy_error_t wrap_bolos_fp_to_mont(cy_fp_t *in, cy_fp_t *out_mont)
{
  cy_error_t error = CY_KO;
  cy_fp_ctx_t *ctx = in->ctx;


  CY_IS_INIT(ctx);

  CY_CHECK(
      cx_mont_to_montgomery(*(out_mont->bn), *(in->bn), ctx->montgomery_ctx));


end:
  return error;
}

/* convert a montgomery representation in_mont to normalized out */
cy_error_t wrap_bolos_fp_from_mont(cy_fp_t *in_mont, cy_fp_t *out){
	cy_error_t error = CY_KO;
	cy_fp_ctx_t *ctx = in_mont->ctx;

   CY_IS_INIT(ctx);

	CY_CHECK(
	      cx_mont_from_montgomery(*(out->bn), *(in_mont->bn), ctx->montgomery_ctx));

	end:
	  return error;
}

cy_error_t wrap_bolos_mont_import(const uint8_t *in, const size_t t8_in, cy_fp_t *out)
{
	cy_error_t error = CY_KO;

	cy_fp_ctx_t *ctx = out->ctx;

	   CY_IS_INIT(ctx);


	 CY_CHECK( wrap_bolos_fp_import(in, t8_in, out));
	 CY_CHECK(	      cx_mont_to_montgomery(*(out->bn), *(out->bn), ctx->montgomery_ctx));

	end:
	  return error;
}



cy_error_t wrap_bolos_mont_export(const cy_fp_t *in, uint8_t *out , size_t t8_out)
{
	cy_error_t error = CY_KO;
	cy_fp_ctx_t *ctx = in->ctx;
	cy_fp_t temp;
	cy_fp_alloc(ctx, t8_out, &temp);

    CY_IS_INIT(ctx);

    CY_CHECK(	      cx_mont_from_montgomery(*(in->bn), *(in->bn), ctx->montgomery_ctx));
	CY_CHECK( wrap_bolos_fp_export(in,  out, t8_out));

	cy_fp_free(&temp);

	end:
		  return error;
}

cy_error_t wrap_bolos_fp_uninit(cy_fp_ctx_t *ps_ctx, uint8_t *pu8_Mem,
                                const size_t t8_Memory)
{
  cy_error_t error = CY_KO;
  UNUSED(t8_Memory);

  size_t i;
  cy_bn_mont_ctx_t *ctx= (cy_bn_mont_ctx_t *) ps_ctx->montgomery_ctx;

  /* from here, if correctly used, the only element remaining are the montgomery context*/
  cy_mont_destroy(ctx);

  for(i=0;i<4*sizeof(cx_bn_t *);i++)
    {
 	  pu8_Mem[i]=_MEM_FP_RESERVED;
    }
  CX_CHECK(sys_cx_bn_unlock());


  for(i=0;i<t8_Memory;i++){
	  if(pu8_Mem[i]!=(uint8_t) _MEM_FP_RESERVED){
		  error=CY_MEM_FREE_MISSING;
//	  printf("\n pu8_Mem[i]=%x at @=%x expected=%x", pu8_Mem[i], (unsigned int) i, (unsigned int) _MEM_FP_RESERVED);
	  }
  }

  ps_ctx->is_initialized=CY_LIB_UNINITIALIZED;
  UNUSED(ps_ctx);

end:
  return error;
}

cy_error_t wrap_bolos_fp_from_u32( const uint32_t in, cy_fp_t *out ){
	cy_error_t error = CY_KO;
	 cy_fp_ctx_t *ctx = out->ctx;
	 CY_IS_INIT(ctx);

	 CX_CHECK(sys_cx_bn_set_u32(*(out->bn), in));

	 end:
	 	  return error;

}

cy_error_t wrap_bolos_fp_get_random(cy_gda_ctx_t *gda, cy_fp_t *out)
{
	cy_error_t error = CY_KO;
	 cy_fp_ctx_t *ctx = out->ctx;
	 CY_IS_INIT(ctx);
	 UNUSED(gda);

	 CX_CHECK(sys_cx_bn_rand(*(out->bn)));

	 end:
	 	  return error;
}

cy_error_t wrap_fp_iseq(const cy_fp_t *in1, const cy_fp_t *in2, int *eq){
	cy_error_t error = CY_KO;
	*eq=CY_FALSE;

	cy_fp_ctx_t *ctx = in1->ctx;
	CY_IS_INIT(ctx);

	CY_CHECK(sys_cx_bn_cmp(*in1->bn, *in2->bn, eq));

	if(*eq==0) *eq=CY_TRUE;
	else *eq=CY_FALSE;

	end:
	  return error;
}


cy_error_t wrap_bolos_fp_setone(cy_fp_t *io)
{
	cy_error_t error = CY_KO;

	CX_CHECK(sys_cx_bn_set_u32(*(io->bn), 1));

	end:
	   	return error;
}

/*****************************************************************************/
/*	III. Access to private fields methods
 */
/*****************************************************************************/
cy_error_t wrap_cy_fp_from_bn( const cy_bn_t *in, cy_fp_t *out ){
	cy_error_t error = CY_KO;

	  cy_fp_ctx_t *ctx = out->ctx;
	  CY_IS_INIT(ctx);

	  CX_CHECK(sys_cx_bn_copy(*(out->bn), *in));

	end:
	  return error;
}

cy_bn_t* wrap_bolos_get_fp_montgomery_constant1(const cy_fp_ctx_t *in)
{
	cy_bn_t *res=NULL;

	cy_bn_mont_ctx_t *mont_ctx= (cy_bn_mont_ctx_t *) in->montgomery_ctx;
	res=&mont_ctx->h2;


	  return res;
}

cy_bn_t* wrap_bolos_get_fp_montgomery_constant2(const cy_fp_ctx_t *in)
{
	cy_bn_t *res=NULL;

	cy_bn_mont_ctx_t *mont_ctx= (cy_bn_mont_ctx_t *) in->montgomery_ctx;
	res=&mont_ctx->mpinv;


	  return res;
}

cy_bn_t* wrap_bolos_get_fp_montgomery_one(const cy_fp_ctx_t *in)
{

	cy_bn_t *res=NULL;

	cy_bn_mont_ctx_t *mont_ctx= (cy_bn_mont_ctx_t *) in->montgomery_ctx;
	res=&mont_ctx->h;

  return res;

}





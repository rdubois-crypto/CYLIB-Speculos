/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_wrap_bolos_ec.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: bolos wrappers for cy_lib elliptic curves arithmetic */
/**********************************************************************************/

#include <stddef.h>
#include <stdio.h>

#include "bolos/cxlib.h"
#include "cy_errors.h"


#include "cy_wrap_fp_bolos.h"


#include "cy_fp.h"

#include "cy_wrap_bolos_ec.h"

#include "cy_ec.h"
//#include "cy_ec_const.h"

/*****************************************************************************/
/*	I. completing missing Speculos functions
 */
/*****************************************************************************/

#define  cy_ecdomain_parameters_length(curve, length) sys_cx_ecdomain_parameters_length(curve,length);


cy_error_t cy_ecpoint_add	(	cx_ecpoint_t * 	out,
const cx_ecpoint_t * 	P,
const cx_ecpoint_t * 	Q
)
{
	cx_curve_t curve=P->curve;
    size_t len;
    cy_ecdomain_parameters_length(curve, &len);

    unsigned char bs_P[3*MAX_BOLOS_EC_T8] ;
    unsigned char bs_Q[3*MAX_BOLOS_EC_T8] ;
    unsigned char bs_R[3*MAX_BOLOS_EC_T8] ;

    sys_cx_ecpoint_export(P, bs_P, len,
    		bs_P+len, len);

    sys_cx_ecpoint_export(Q, bs_Q, len,
        		bs_Q+len, len);



	UNUSED(out);
	UNUSED(P);
	UNUSED(Q);
	sys_cx_ecfp_add_point(curve, bs_R, bs_P,bs_Q, len);



    return CY_KO;
}

/*****************************************************************************/
/*	II. Wrapping bolos calls to Common APIs  and Memory handling
 */
/*****************************************************************************/

/* the mapping of memory in bolos for dual FP/EC units is
 * fp_ctx is written at adress 0
 * fp offset counter is used to stack both fp and ec
 * fp_uninit erase all pointers
 * ec_uninit erase the fp ctx
 */
static cy_error_t inc_offset(cy_ec_ctx_t *ec_ctx, size_t t8_inc)
{
	size_t total_offset;

	ec_ctx->ctx_fp_p->offset+=t8_inc;
	total_offset=ec_ctx->ctx_fp_p->offset+ec_ctx->offset;

	if(total_offset > ec_ctx->max_offset) return CY_MEM_OVERFLOW;

	return CY_OK;
}

/* argc input is the value of the curve, argv is  NULL*/
cy_error_t wrap_bolos_ec_init(cy_ec_ctx_t *ec_ctx, uint8_t *pu8_Mem,
                              const size_t t8_Memory, const int argc,
                              const uint8_t *argv[])
{
  UNUSED(argv);

  cy_error_t error = CY_KO;
  //cx_curve_t curve = (cx_curve_t) (argv[0]);
  cx_curve_t curve = (cx_curve_t) argc;



  size_t size_param_t8=0;/* byte size of parameters */
  const uint8_t *argv_gen[]={NULL, NULL};

  unsigned char argv_prime[MAX_BOLOS_EC_T8+4] ;


  if (cx_ecdomain(curve) == NULL)
    return CY_UNHANDLED_CURVE;


  CY_CHECK(cy_ecdomain_parameters_length(curve, & size_param_t8));

  ec_ctx->offset = 0;

  (ec_ctx->Shared_Memory) = pu8_Mem;

  ec_ctx->ctx_fp_p=(cy_fp_ctx_t *) pu8_Mem;

  ec_ctx->offset = sizeof(cy_fp_ctx_t );

//  ec_ctx->offset +=sizeof(cy_fp_ctx_t *);

  be_from_word32(size_param_t8, argv_prime);
  ec_ctx->curve_id = curve;

  sys_cx_ecdomain_parameter(curve, CX_CURVE_PARAM_Field, argv_prime+4, (uint32_t) size_param_t8);


  argv_gen[0]=argv_prime;
  argv_gen[1]=argv_prime+4;


  CY_CHECK(wrap_bolos_fp_init(ec_ctx->ctx_fp_p, pu8_Mem+ec_ctx->offset, t8_Memory-ec_ctx->offset, 2, argv_gen));

  strcpy(ec_ctx->libname, BOLOS_EC_LIBNAME);


  ec_ctx->is_initialized = CY_LIB_INITIALIZED;

 // printf("\n here with code=%x", (unsigned int) error);

 // wrap_bolos_fp_init(ec_ctx->ctx_fp_q, pu8_Mem, t8_Memory, 2, argv_gen);
 // ec_ctx->offset += ec_ctx->ctx_fp_q->offset;


  end:
  	return error;
}

cy_error_t wrap_bolos_ec_uninit(cy_ec_ctx_t *ec_ctx)
{
	size_t i;

    cy_error_t error = CY_KO;
	cy_fp_ctx_t *fp_ctx=ec_ctx->ctx_fp_p;
	for(i=0;i<sizeof(cy_fp_ctx_t *);i++)
		ec_ctx->Shared_Memory[i]=_MEM_FP_RESERVED;

	CY_CHECK(wrap_bolos_fp_uninit(fp_ctx, fp_ctx->Shared_Memory,fp_ctx->offset));

	 end:
		return error;
}


cy_error_t wrap_ecpoint_alloc( cy_ec_ctx_t *ec_ctx, cy_ecpoint_t *P)
{
  cy_error_t error = CY_KO;
  P->ctx = ec_ctx;

  P->index =  ec_ctx->offset;

  if (ec_ctx->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_UNIT;
    goto end;

  }

  P->ec = (cx_ecpoint_t *)(ec_ctx->Shared_Memory + ec_ctx->offset+ec_ctx->ctx_fp_p->offset);

  CY_CHECK(sys_cx_ecpoint_alloc (P->ec, ec_ctx->curve_id));

  inc_offset( ec_ctx, sizeof(cy_inner_ec_t ));

// printf("\n allocating ec %x", (P->ec->x) );

  end:
     return error;
}

cy_error_t wrap_bolos_ec_add(const cy_ecpoint_t * a, const cy_ecpoint_t * b,
                               cy_ecpoint_t * out)
  {
    cy_error_t error = CY_OK;
    cy_ec_ctx_t *ctx = a->ctx;

    if (ctx->is_initialized != CY_LIB_INITIALIZED) {
      error = CY_ERR_UNIT;

      goto end;
    }

    /* Nano S old versions of speculos are not inline with cx_ecpoint_add and return error
     * if one of the input is the infty point
     */
	#ifdef _WITH_SPECULOS
    	int flag;
    	CY_CHECK(wrap_bolos_ec_isinfinity(a, &flag));
    	if(flag==CY_TRUE)
    	{

    		error = CY_OK;
    		wrap_bolos_cy_ec_copy(b, out);
    		goto end;
    	}
    	CY_CHECK(wrap_bolos_ec_isinfinity(b, &flag));
    	if(flag==CY_TRUE)
    	{
    			error = CY_OK;
    	    		wrap_bolos_cy_ec_copy(a, out);
    	    		goto end;
    	 }

	#endif
     CX_CHECK(sys_cx_ecpoint_add(out->ec, a->ec, b->ec));


  end:
    return error;
  }

cy_error_t cy_ec_iseq(const cy_ecpoint_t *a, const cy_ecpoint_t *b, int *flag_verif)
{
	cy_error_t error = CY_KO;

	CY_CHECK(sys_cx_ecpoint_cmp(a->ec,b->ec, (bool*) flag_verif));
	end:
	  return error;
}

cy_error_t wrap_bolos_ec_import(const uint8_t *xy, size_t t8_x, cy_ecpoint_t *G){
	cy_error_t error= CY_KO;

	CY_CHECK(sys_cx_ecpoint_init(G->ec, xy,t8_x, xy+t8_x, t8_x));

	end:
		    return error;
}


cy_error_t wrap_bolos_ec_import2(uint8_t *x, size_t t8_x, uint8_t *y, size_t t8_y, cy_ecpoint_t *G){

	cy_error_t error= CY_KO;

	CY_CHECK(sys_cx_ecpoint_init(G->ec, x,t8_x, y, t8_y));

	end:
		    return error;
}

cy_error_t wrap_bolos_ec_export(const cy_ecpoint_t *G,  uint8_t *xy, size_t t8_x)
{
	 cy_error_t error = CY_KO;
	 int flag=0;
	 CY_CHECK(wrap_bolos_ec_isinfinity(G,&flag));

	 if(flag==CY_TRUE){

		 memset(xy,0,2*t8_x);
		 xy[2*t8_x-1]=1;
	 }

	 CY_CHECK(sys_cx_ecpoint_export(G->ec, xy, t8_x, xy+t8_x, t8_x));

	 end:
	  	return error;
}


cy_error_t wrap_bolos_get_generator(const cy_ec_ctx_t *ec_ctx, cy_ecpoint_t *G){

	cy_error_t error= CY_KO;

	CY_CHECK(sys_cx_ecdomain_generator_bn(ec_ctx->curve_id, G->ec));

	  end:
	    return error;

}

cy_error_t wrap_bolos_ec_isinfinity(const cy_ecpoint_t * P_in, int *flag)
{
	 cy_error_t error = CY_KO;
	 int cmp=0;
	 int cmp_acc=0;


	 CY_CHECK(sys_cx_bn_cmp_u32(P_in->ec->x, 0, &cmp));
	 cmp_acc+=cmp;

	 CY_CHECK(sys_cx_bn_cmp_u32(P_in->ec->y, 1, &cmp));
	 cmp_acc+=cmp;

	 CY_CHECK(sys_cx_bn_cmp_u32(P_in->ec->z, 0, &cmp));
	 cmp_acc+=cmp;

	 *flag=(cmp_acc==0);

	 end:
	 	 return error;
}

cy_error_t wrap_bolos_ec_import_compressed(const uint8_t *xy_compressed,
        const size_t xy_compressed_len, const uint32_t sign, cy_ecpoint_t *P_out)
{

	 cy_error_t error = CY_KO;
	 CY_CHECK(sys_cx_ecpoint_decompress(P_out->ec, xy_compressed, xy_compressed_len, sign));

	 end:
		 	 return error;
}

cy_error_t wrap_bolos_ec_export_compressed(const cy_ecpoint_t *P_in, uint8_t *xy_compressed,
        const size_t xy_compressed_len, uint32_t *sign )
{
    cy_error_t error = CY_KO;
	CY_CHECK(sys_cx_ecpoint_decompress(P_in->ec, xy_compressed, xy_compressed_len,  *sign));

	 end:
		 	 return error;
}



cy_error_t wrap_bolos_cy_ec_copy(const cy_ecpoint_t * P_in, cy_ecpoint_t *P_out)
{
	 cy_error_t error = CY_KO;

	 CY_CHECK(sys_cx_bn_copy(P_out->ec->x, P_in->ec->x));
	 CY_CHECK(sys_cx_bn_copy(P_out->ec->y, P_in->ec->y));
	 CY_CHECK(sys_cx_bn_copy(P_out->ec->z, P_in->ec->z));

	 end:
	 	 return error;
}


cy_error_t wrap_bolos_ec_setinfinity( cy_ecpoint_t *kP)
{
	 cy_error_t error = CY_KO;

	 CX_CHECK(sys_cx_bn_set_u32(kP->ec->x, 0));
	 CX_CHECK(sys_cx_bn_set_u32(kP->ec->y, 1));
	 CX_CHECK(sys_cx_bn_set_u32(kP->ec->z, 0));

	 end:
	    	return error;
}


cy_error_t wrap_bolos_ec_scalarmul_fp(const cy_fp_t * k, const cy_ecpoint_t * P,
                               cy_ecpoint_t *kP)
  {
	int cmp;
    cy_error_t error = CY_KO;

    CY_CHECK(wrap_bolos_cy_ec_copy(P, kP));

    sys_cx_bn_cmp_u32(*k->bn, 0, &cmp);

    /* scalar is null, so output is infty point*/
    if(cmp==0){
    	//printf("\n infinity");
    	CX_CHECK(wrap_bolos_ec_setinfinity( kP));
    	error=CY_OK;
    	goto end;
    }

    CY_CHECK(sys_cx_ecpoint_scalarmul_bn(kP->ec, *k->bn));

    end:
    	return error;
  }

cy_error_t wrap_bolos_export_doublefp(const cy_ecpoint_t *a, cy_fp_t *x, cy_fp_t *y)
{
	 cy_error_t error = CY_KO;

	 CY_CHECK(sys_cx_ecpoint_export_bn(a->ec, x->bn, y->bn));
	 end:
		    	return error;
}



cy_error_t wrap_bolos_exportX(const cy_ecpoint_t *a, uint8_t *out, size_t t8_out)
{
	 cy_error_t error = CY_KO;
     cy_fp_t trash;
     cy_fp_alloc(a->ctx->ctx_fp_p, a->ctx->ctx_fp_p->t8_modular, &trash);


	 CY_CHECK(wrap_bolos_getX(a, &trash));
	 CY_CHECK(cy_fp_export(&trash, out, t8_out));

	 CY_CHECK(cy_fp_free( &trash));

	 end:
		    	return error;
}


cy_error_t wrap_bolos_getX(const cy_ecpoint_t *a, cy_fp_t *out)
{
	 cy_error_t error = CY_KO;
     cy_fp_t trash;
     cy_fp_alloc(a->ctx->ctx_fp_p, a->ctx->ctx_fp_p->t8_modular, &trash);

	 CY_CHECK(sys_cx_ecpoint_export_bn((a->ec), out->bn, trash.bn));

	 cy_fp_free(&trash);

	 end:
		    	return error;
}



cy_error_t wrap_bolos_getY(const cy_ecpoint_t *a, cy_fp_t *out)
{
	cy_error_t error = CY_KO;
	     cy_fp_t trash;
	     int flag;

	     CY_CHECK(cy_fp_alloc(a->ctx->ctx_fp_p, a->ctx->ctx_fp_p->t8_modular, &trash));

	     CY_CHECK(wrap_bolos_ec_isinfinity(a, &flag));;

	     if(flag==CY_TRUE)
	     {
	    	 CY_CHECK(cy_fp_set_one(out));
	     }
	    CY_CHECK(sys_cx_ecpoint_export_bn((a->ec), trash.bn, out->bn));

		 cy_fp_free(&trash);
		 end:
			    	return error;
}

cy_error_t wrap_bolos_ec_getparityY(const cy_ecpoint_t *a, uint32_t *sign){
	cy_error_t error = CY_KO;
	cy_fp_t fp_temp;
	CY_CHECK(cy_fp_alloc(a->ctx->ctx_fp_p, a->ctx->ctx_fp_p->t8_modular, &fp_temp));

	CY_CHECK(wrap_bolos_getY(a, &fp_temp));

	CY_CHECK(sys_cx_bn_is_odd(*(fp_temp.bn), (bool*) sign));

	CY_CHECK(cy_fp_free(&fp_temp));

    end:
				    	return error;
}

cy_error_t wrap_bolos_isoncurve(const cy_ecpoint_t *a, int *flag_verif)
{
	  cy_error_t error = CY_KO;


	    CY_CHECK(sys_cx_ecpoint_is_on_curve(a->ec, (bool*) flag_verif));

	    end:
	    	return error;

}


cy_error_t wrap_ecpoint_free(cy_ecpoint_t *P)
{
  size_t i;
  cy_error_t error = CY_OK;
  cy_ec_ctx_t *ec_ctx= ec_ctx;
  //printf("\n destroying ec %x", (P->ec->x) );

  CY_CHECK(sys_cx_ecpoint_destroy (P->ec));

  for(i=0;i<sizeof(cy_ecpoint_t *);i++)
    {
	  ((uint8_t *) P)[i]=_MEM_EC_RESERVED;
    }


  end:
     return error;
}



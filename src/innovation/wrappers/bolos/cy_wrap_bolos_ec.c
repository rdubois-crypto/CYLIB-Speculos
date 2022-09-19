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
#ifdef emulatedomain
cx_err_t cy_ecdomain_parameter_bn(cx_curve_t cv, cx_curve_dom_param_t id,
                                  cx_bn_t out)
{
  cy_error_t error = CY_UNHANDLED_CURVE;

  /* we only require the prime field p value here to wrap ec init*/
  if (id == CX_CURVE_PARAM_Field)
  {
	  if (cv == CX_CURVE_SECP256K1) {
		  CY_CHECK(sys_cx_bn_init(out, C_cy_secp256k1_p, SEC256K1_SIZE_u8));
	  }

	  if (cv == CX_CURVE_SECP384R1) {
		  CY_CHECK(sys_cx_bn_init(out, C_cy_secp384r1_p, SEC384r1_SIZE_u8));
	  }

	  if (cv == CX_CURVE_Stark256) {
	  		  CY_CHECK(sys_cx_bn_init(out, C_cy_Stark_p, Stark_SIZE_u8));
	  	  }

  }

  if(id==CX_CURVE_PARAM_Order)
  {
	  if (cv == CX_CURVE_SECP256K1) {
			  CY_CHECK(sys_cx_bn_init(out, C_cy_secp256k1_n, SEC256K1_SIZE_u8));
		  }

	 if (cv == CX_CURVE_SECP384R1) {
			  CY_CHECK(sys_cx_bn_init(out, C_cy_secp384r1_n, Stark_SIZE_u8));
		  }
	 if (cv == CX_CURVE_Stark256) {
	 	  		  CY_CHECK(sys_cx_bn_init(out, C_cy_Stark_n, Stark_SIZE_u8));
	 	  	  }


  }
  /*todo: complete with other bolos curves*/
  /*
  CX_CURVE_BLS12_381_G1
  CX_CURVE_SECP384R1
  */
end:
  return error;
}
#endif

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

       /* sys_cx_ecfp_add_point(  ec_ctx->curve_id, uint8_t *R, const uint8_t
*P, const uint8_t *Q, size_t X_len);*/


    return CY_KO;
}

/*****************************************************************************/
/*	II. Wrapping bolos calls to Common APIs  and Memory handling
 */
/*****************************************************************************/

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

  printf("\n here with curve=%d of size:%d", (int) curve, (int) size_param_t8);


  ec_ctx->offset = 0;

  (ec_ctx->Shared_Memory) = pu8_Mem;

  ec_ctx->ctx_fp_p=(cy_fp_ctx_t *) pu8_Mem;

//  ec_ctx->offset +=sizeof(cy_fp_ctx_t *);

  be_from_word32(size_param_t8, argv_prime);
  ec_ctx->curve_id = curve;

  sys_cx_ecdomain_parameter(curve, CX_CURVE_PARAM_Field, argv_prime+4, (uint32_t) size_param_t8);
  ec_ctx->offset = ec_ctx->ctx_fp_p->offset;


  argv_gen[0]=argv_prime;
  argv_gen[1]=argv_prime+4;

 int i;
 for(i=0;i<52;i++) printf(" %02x",argv_prime[i]);
  CY_CHECK(wrap_bolos_fp_init(ec_ctx->ctx_fp_p, pu8_Mem, t8_Memory, 2, argv_gen));



  ec_ctx->offset += ec_ctx->ctx_fp_p->offset;

 // wrap_bolos_fp_init(ec_ctx->ctx_fp_q, pu8_Mem, t8_Memory, 2, argv_gen);
 // ec_ctx->offset += ec_ctx->ctx_fp_q->offset;


end:
  return error;
}

cy_error_t wrap_bolos_ec_init(cy_ec_ctx_t *ec_ctx, uint8_t *pu8_Mem,
                              const size_t t8_Memory)
{
	cy_fp_ctx_t *fp_ctx=ec_ctx->ctx_fp_p;

	wrap_bolos_fp_uninit(fp_ctx, pu8_Mem,t8_Memory);

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

  CY_CHECK(sys_cx_ecpoint_alloc (P->ec, ec_ctx->curve_id));
  ec_ctx->offset += sizeof(cy_inner_ec_t *);



  end:
     return error;
}

cy_error_t wrap_bolos_ec_add(cy_ecpoint_t * a, cy_ecpoint_t * b,
                               cy_ecpoint_t * out)
  {
    cy_error_t error = CY_KO;
    cy_ec_ctx_t *ctx = a->ctx;

    if (ctx->is_initialized != CY_LIB_INITIALIZED) {
      error = CY_ERR_UNIT;

      goto end;
    }

     CX_CHECK(cx_ecpoint_add(out->ec, a->ec, b->ec));

    error = CY_OK;

  end:
    return error;
  }


cy_error_t wrap_bolos_ec_scalarmul_bn(cy_ecpoint_t * a, cy_ecpoint_t * b,
                               cy_ecpoint_t * out)
  {
    cy_error_t error = CY_KO;

    return error;
  }



cy_error_t wrap_ecpoint_free(cy_ecpoint_t *P)
{
  size_t i;
  cy_error_t error = CY_KO;
  cy_ec_ctx_t *ec_ctx= ec_ctx;

  for(i=0;i<sizeof(cy_ecpoint_t *);i++)
    {
	  ((uint8_t *) P)[i]=_MEM_EC_RESERVED;
    }


  CY_CHECK(sys_cx_ecpoint_destroy (P->ec));



  end:
     return error;
}



/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.
 */
/* FILE: cy_HashPedersen.c
 */
/* 																			      */
/* 																			      */
/* DESCRIPTION: Modified  Stark Hash Pedersen*/
/* note: this is a straight translation from the implementation available here:
 * https://github.com/LedgerHQ/app-ethereum/blob/develop/src/stark_utils.c
 * 
 */
/**********************************************************************************/
#include <stddef.h>
#include <stdint.h>


#include "cy_configuration.h"

#include "cy_bn.h"
#include "cy_fp.h"
#include "cy_ec.h"
#include "cy_hash.h"


#include <stdio.h> // todo remove
#include "cy_io_common_tools.h"
#include "cy_io_fp.h"
#include "cy_io_ec.h"


#include "cy_HashPedersen.h"


/* display the value of a fp in msb*/
cy_error_t pcy_io_fp_printMSB(const cy_fp_t *in, char *comment)
{
  uint8_t display[1024];

  cy_error_t error=CY_KO;

  CY_CHECK(cy_fp_export(in,  display, in->ctx->t8_modular));

  print_MsbString(display,  in->ctx->t8_modular, comment );



  end:
  	  return error;
}

static cy_error_t ec_muladd(cy_pedersen_ctx_t *ctx, cy_ecpoint_t *acc, cy_fp_t *a, int index)
{
   cy_error_t error;
   cy_ecpoint_t ec_temp;


   CY_CHECK(cy_ec_alloc(ctx->ec_ctx, &ec_temp));

   CY_CHECK(cy_ec_scalarmult_fp(a, &ctx->P[index], &ec_temp));
   CY_CHECK(cy_ec_add( &ec_temp, acc, acc));
   CY_CHECK(cy_ec_free( &ec_temp));

   end:
   return error;           
}

/* init PShift, P0 to P3*/
cy_error_t pedersen_configure(cy_ec_ctx_t *ec_ctx, cy_pedersen_ctx_t *ctx)
{
   cy_error_t error;
   size_t i;
 //  int flag;
   ctx->ec_ctx=ec_ctx;


   for(i=0;i<_NUM_PEDPOINT;i++){
	   CY_CHECK(cy_ec_alloc(ec_ctx, &ctx->P[i]));
   }
   CY_CHECK(cy_ec_alloc(ec_ctx, &ctx->ShiftPoint));
   CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular,& ctx->mask248_low));
   CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, ec_ctx->ctx_fp_p->t8_modular, &ctx->fp_hash));

   ctx->blocksize=ctx->ec_ctx->ctx_fp_p->t8_modular;

   CY_CHECK(cy_ec_import(Pedersen_P0, Stark_SIZE_u8, &ctx->P[0] ));
   CY_CHECK(cy_ec_import(Pedersen_P1, Stark_SIZE_u8, &ctx->P[1] ));
   CY_CHECK(cy_ec_import(Pedersen_P2, Stark_SIZE_u8, &ctx->P[2] ));
   CY_CHECK(cy_ec_import(Pedersen_P3, Stark_SIZE_u8, &ctx->P[3] ));
   CY_CHECK(cy_ec_import(Pedersen_Shift, Stark_SIZE_u8, &ctx->ShiftPoint ));
   CY_CHECK(cy_fp_import(mask248_low, Stark_SIZE_u8, &ctx->mask248_low) );

   end:
   return error;           
}

cy_error_t pedersen(cy_pedersen_ctx_t *ctx, cy_fp_t *a, cy_fp_t *b,  cy_fp_t *res){
   cy_error_t error;
   cy_ecpoint_t ec_Hash;
   cy_fp_t fp_temp;
  // int flag=0;

  // printf("\n ****************pedersen core");

   CY_CHECK(cy_fp_alloc(ctx->ec_ctx->ctx_fp_p, Stark_SIZE_u8, &fp_temp));
   CY_CHECK(cy_ec_alloc(ctx->ec_ctx, &ec_Hash));

   CY_CHECK(cy_ec_import(Pedersen_Shift, Stark_SIZE_u8, &ec_Hash )); /* R=ShiftPoint*/
   CY_CHECK(cy_bn_and( a->bn, ctx->mask248_low.bn, fp_temp.bn) );		 /*R+= low_a*P0*/


   CY_CHECK(ec_muladd(ctx, &ec_Hash, &fp_temp, 0));

  // printf("\n pedersen 2");
   CY_CHECK(cy_wrap_bolos_bn_shift_r((size_t) 248, a->bn));							/*R+= low_a*P1*/
   CY_CHECK( ec_muladd(ctx, &ec_Hash, a, 1));
   CY_CHECK(cy_bn_and( b->bn, ctx->mask248_low.bn, fp_temp.bn) );		 /*R+= low_b*P2*/

   CY_CHECK(ec_muladd(ctx, &ec_Hash, &fp_temp, 2));


   CY_CHECK(cy_wrap_bolos_bn_shift_r(248, b->bn));							/*R+= low_a*P1*/
   CY_CHECK(ec_muladd(ctx, &ec_Hash, b, 3));

   /* getX*/
   CY_CHECK( cy_ec_getX(&ec_Hash, res));

   CY_CHECK( cy_ec_free(&ec_Hash));
   CY_CHECK( cy_fp_free(&fp_temp));

   end:
   return error;           
   }
 



/* Compute the main loop, without the final padding with length */
cy_error_t pedersen_hash_updatefp(cy_pedersen_ctx_t *ctx,  cy_fp_t *data)
{
	 cy_error_t error;

	// pcy_io_fp_printMSB(data, "\n update with: ");

	 CY_CHECK(pedersen(ctx, &ctx->fp_hash, data , &ctx->fp_hash));

	 ctx->data_fp_length++ ;
	 //printf("\n data length is:%d", (int) ctx->data_fp_length);
	 end:
	    return error;
}

cy_error_t pedersen_hash_init(cy_pedersen_ctx_t *ctx, uint8_t *constant, size_t t8_constant)
{
	 cy_error_t error;

	 ctx->current_t8=0;
	 ctx->data_fp_length=0;

	 //printf("\n size modulus:%d",(int) ctx->fp_hash.ctx->t8_modular);

	 CY_CHECK(cy_fp_import(constant, t8_constant, &ctx->fp_hash));

	 end:
		    return error;
}

/* beware that the update should be one field element per update (no fragmentation of felt is possible)*/
cy_error_t pedersen_hash_update(cy_pedersen_ctx_t *ctx,  uint8_t *data, size_t t8_data)
{
	 cy_error_t error;
	 cy_fp_t fp_temp;
	 size_t i, t8_block=ctx->ec_ctx->ctx_fp_p->t8_modular;

	 CY_CHECK(cy_fp_alloc(ctx->ec_ctx->ctx_fp_p, t8_block, &fp_temp));

	 ctx->current_t8+=t8_data;


	 for(i=0;i<t8_data;i+=ctx->blocksize)
	 {
		 if(i+ctx->blocksize>t8_data)
		 {
			 t8_block=t8_data-i;
		 }
		 //printf("\n tbloc=%d i=%d", (int) t8_block, (int) i);
		 CY_CHECK(cy_fp_import(data+i, t8_block, &fp_temp));
		 //pcy_io_fp_printMSB(&fp_temp, "\n imoported : ");

		 CY_CHECK(pedersen_hash_updatefp(ctx,&fp_temp ));

	 }


	 CY_CHECK(cy_fp_free(&fp_temp));

	 end:
	    return error;
}


/* Compute the main loop, without the final padding with length */
cy_error_t pedersen_hash_unpadded(cy_pedersen_ctx_t *ctx, cy_fp_t *data, size_t data_fp_length, cy_fp_t *res)
{
  cy_error_t error;
  size_t i;
  uint8_t zero[1]={0};


  /*hash=pedersen(0, data[0]); 			#h(0,x1)*/
  CY_CHECK(pedersen_hash_init(ctx, zero, 1));

  for(i=0;i<data_fp_length-1;i++){

	  CY_CHECK(pedersen_hash_updatefp(ctx, &(data[i]) ));

  }

  CY_CHECK(pedersen(ctx, &ctx->fp_hash, &(data[i]) , res));

  ctx->data_fp_length++;

   end:
   return error;
}


/* Compute the main loop, without the final padding with length */
cy_error_t pedersen_hash_final_fp(cy_pedersen_ctx_t *ctx, size_t data_fp_length, cy_fp_t *res)
{
	 cy_error_t error;
	 cy_fp_t fp_temp;
	 CY_CHECK(cy_fp_alloc(ctx->ec_ctx->ctx_fp_p, ctx->ec_ctx->ctx_fp_p->t8_modular, &fp_temp));

	  CY_CHECK(cy_fp_from_u32( (uint32_t) data_fp_length, &fp_temp));
	  CY_CHECK(pedersen(ctx, &ctx->fp_hash, &fp_temp, res));

	  CY_CHECK(cy_fp_free(&fp_temp));

	  end:
	    return error;
}


/* Compute the main loop, without the final padding with length */
cy_error_t pedersen_hash_final(cy_pedersen_ctx_t *ctx, uint8_t *hash_res, size_t t8_res)
{
	 cy_error_t error;
	 cy_fp_t fp_temp;

	 if(t8_res!=ctx->ec_ctx->ctx_fp_p->t8_modular)
	 {
		 error=CY_ERR_LENGTH;
		 goto end;
	 }
	 CY_CHECK(cy_fp_alloc(ctx->ec_ctx->ctx_fp_p, ctx->ec_ctx->ctx_fp_p->t8_modular, &fp_temp));

	  CY_CHECK(cy_fp_from_u32( ctx->data_fp_length, &fp_temp));
	  CY_CHECK(pedersen(ctx, &ctx->fp_hash, &fp_temp, &ctx->fp_hash));

	  /* export resulting felt to uint8_t buffer output*/
	  CY_CHECK(cy_fp_export(&ctx->fp_hash,  hash_res, ctx->ec_ctx->ctx_fp_p->t8_modular));


	  CY_CHECK(cy_fp_free(&fp_temp));

	  end:
	    return error;
}


/* note: for a init/update/final version, pedersen could be considered as a 252 bits block sized hash function*/
/* this implementation is intended to be compatible with Cairo */   
cy_error_t pedersen_hash(cy_pedersen_ctx_t *ctx, cy_fp_t *data, size_t data_fp_length, cy_fp_t *res)
{
  cy_error_t error;


  CY_CHECK(pedersen_hash_unpadded(ctx, data, data_fp_length,& ctx->fp_hash));
  CY_CHECK(pedersen_hash_final_fp(ctx,  data_fp_length,res));  /*hash=pedersen(h(...), n);*/


   end:
   return error;           
}   
   
cy_error_t pedersen_uninit(cy_pedersen_ctx_t *ctx){
  cy_error_t error;
  size_t i;

  for(i=0;i<_NUM_PEDPOINT;i++){
 	   CY_CHECK(cy_ec_free(&(ctx->P[i])));
    }

  CY_CHECK(cy_ec_free(&ctx->ShiftPoint));
  CY_CHECK(cy_fp_free( &ctx->mask248_low) );
  CY_CHECK(cy_fp_free(&ctx->fp_hash));

  end:
     return error;
}
   


/* Ramp is a pointer to the memory for the unit
 * the curve identifier is used to initialize the ellitic context
 */
cy_error_t pedersen_unit_configure(void *pedersen_unit, uint8_t *ec_ctx, size_t t8_ctx)
{
	 cy_error_t error=0;
	 cy_hash_unit_t *unit=(cy_hash_unit_t *) pedersen_unit;



	 UNUSED(t8_ctx);/*t8_ctx is here for hash_unit_t compatibility */
	 UNUSED(ec_ctx);

	 CY_CHECK(pedersen_configure( (cy_ec_ctx_t *) ec_ctx, (cy_pedersen_ctx_t *) (unit->ctx) ));

	 unit->is_initialized=CY_OBJ_INITIALIZED;

	 end:
	 	 return error;
}


cy_hash_unit_t unit_pedersen={
		NULL,
		_PEDERSEN_HASH_UNIT_ID,
		"<CYLIB Pedersen:Hash Unit>",

		(Fct_Hash_Configure_t) pedersen_unit_configure,
		(Fct_Hash_Init_t) pedersen_hash_init,
		(Fct_Hash_Update_t) pedersen_hash_update,
		(Fct_Hash_Final_t) pedersen_hash_final,
		 NULL, /* no hash all*/
		(Fct_Hash_Uninit_t) pedersen_uninit,

		 CY_ERR_INIT,
		 (void*) &global_pedersen_ctx, /* *ctx */
		0,
		  0,
		  NULL,
		  0
};

   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
             



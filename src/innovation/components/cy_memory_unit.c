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


#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "cy_errors.h"
#include "cy_memory_unit.h"

/* Round an allocation to the alignment size multiple, alignment must be a power
 * of 2*/
#define round_memory_alloc(in, alignement)                                     \
  (((in / alignement) + ((in & (alignement - 1)) > 0)) * alignement)

cy_error_t cy_mem_init(cy_mem_ctx_t *io_mem, const size_t allocated_t8,
                       size_t alignment_t8)
{
  /*I. Declarations*/
  cy_error_t error = CY_KO; /* return code*/
  size_t cpt_i;
  /*II. Control parameters*/
  if (allocated_t8 > _MAX_MEMORY) {
    error = CY_MEM_OVERFLOW;
    goto end;
  }

  for (cpt_i = 0; cpt_i < allocated_t8; cpt_i++) {
    io_mem->Shared_Memory[cpt_i] = _MEM_ERASED_READY;
  }
  io_mem->offset = 0;
  io_mem->is_initialized = CY_LIB_INITIALIZED;
  io_mem->alignment_t8 = alignment_t8;
  io_mem->allocated_t8 = allocated_t8;


  error = CY_OK;

end:
  return error;
}

cy_error_t cy_mem_malloc(cy_mem_ctx_t *io_mem, size_t allocated_t8, uint8_t **index_out)
{
  /*I. Declarations*/
  cy_error_t error = CY_KO; /* return code*/
  size_t cpt_i;
  size_t rounded_allocated =
      round_memory_alloc(allocated_t8, io_mem->alignment_t8);

//  printf("\n rounded_allocated=%d",(int)rounded_allocated);
  if (io_mem->is_initialized != CY_LIB_INITIALIZED) {
    error = CY_ERR_INIT;
    goto end;
  }

  /*II. Control parameters*/
  if ((io_mem->offset + rounded_allocated) > io_mem->allocated_t8)
    return CY_MEM_OVERFLOW;

  /*III. Computations*/
  for (cpt_i = 0; cpt_i < rounded_allocated; cpt_i++) {
    io_mem->Shared_Memory[cpt_i + io_mem->offset] = _MEM_ALLOCATED;
  }


  *index_out=(io_mem->Shared_Memory+io_mem->offset);

  io_mem->offset += allocated_t8;

  error = CY_OK;

end:
  return error;
}

/* note that in the current implementation, variables shall be freed in reverse
 * order to save Shared RAM space*/
cy_error_t cy_mem_free(cy_mem_ctx_t *io_mem, uint8_t *index,
                       size_t allocated_t8)
{
  /*I. Declarations*/
  cy_error_t error = CY_KO; /* return code*/
  size_t cpt_i;
  size_t rounded_allocated =
      round_memory_alloc(allocated_t8, io_mem->alignment_t8);

  /*II. Control parameters, zone shall not be after offset*/

  if ((size_t)(index - io_mem->Shared_Memory) > io_mem->offset) {
    error = CY_MEM_FREE_INDEX;
    goto end;
  }

  /*III. Computations*/
  for (cpt_i = 0; cpt_i < rounded_allocated; cpt_i++) {
    index[cpt_i] = _MEM_ERASED_READY;
  }

  if ((io_mem->offset + io_mem->Shared_Memory) == (cpt_i + index)) {
    io_mem->offset -= rounded_allocated;
  }

  index = NULL;

  error = CY_OK;

end:
  return error;
}

static void* cy_mem_spotfree(cy_mem_ctx_t *io_mem, size_t size)
{

 size_t cpt_i, free_acc=0;

 for (cpt_i =0;cpt_i< io_mem->offset;cpt_i++)
 {
	 if(io_mem->Shared_Memory[cpt_i]==_MEM_ALLOCATED)
	 {
		 free_acc++;
	 }
	 else{
		 free_acc=0;
	 }
	 if(free_acc==size)
	 {
		 return (void *) &(io_mem->Shared_Memory[cpt_i-size+1]);
	 }
 }

 return NULL;
}

cy_error_t cy_free(cy_mem_ctx_t *io_mem, void* ptr, size_t size_type)
{
	size_t cpt_i;
	cy_error_t error=CY_OK;

	if((unsigned int) ptr > (unsigned int)(io_mem->Shared_Memory+io_mem->allocated_t8) )
		error=CY_MEM_OVERFLOW;

	for (cpt_i =0;cpt_i< size_type;cpt_i++)
		((uint8_t *) ptr)[cpt_i]=_MEM_ALLOCATED;


	 return error;
}

void *cy_malloc(cy_mem_ctx_t* ctx_mem, size_t size_type){
	void * address=NULL;

	if(ctx_mem->is_initialized!=CY_LIB_INITIALIZED) {
		goto end;
	}

	/* if memory is full, find free spot*/
	if((ctx_mem->offset+size_type)>ctx_mem->allocated_t8 )
	{
		address=(cy_mem_spotfree(ctx_mem, size_type));
	}
	else
	{
		address=(void *)(ctx_mem->Shared_Memory + ctx_mem->offset);
		ctx_mem->offset+=size_type;
	}

	end:
	  return address;
}



/* unallocated memory shall have _MEM_ERASED value*/
cy_error_t cy_mem_check(cy_mem_ctx_t *io_mem)
{
  cy_error_t error = CY_OK; /* return code*/
  size_t cpt_i;

  for (cpt_i = io_mem->offset; cpt_i < io_mem->allocated_t8; cpt_i++) {
    if (io_mem->Shared_Memory[cpt_i + io_mem->offset] != _MEM_ERASED_READY)
      error = CY_KO;
  }

  return error;
}

cy_error_t cy_mem_uninit(cy_mem_ctx_t *io_mem)
{
  cy_error_t error = CY_KO; /* return code*/
  size_t cpt_i;

  CY_CHECK(cy_mem_check(io_mem));

  for (cpt_i = 0; cpt_i < io_mem->allocated_t8; cpt_i++) {
	if(io_mem->Shared_Memory[cpt_i]!=_MEM_ERASED_READY) error=CY_MEM_FREE_MISSING ;
    io_mem->Shared_Memory[cpt_i] = _MEM_ERASED_LOCKED;
  }
  io_mem->is_initialized = CY_LIB_UNINITIALIZED;

end:
  return error;
}

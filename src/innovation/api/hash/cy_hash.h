/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois
 */
/* This file is part of cy_lib project
 */
/* License: This software is licensed under a dual BSD and GPL v2 license.
 */
/* See LICENSE file at the root folder of the project.*/
/* FILE: cy_hash.h*/
/* 																			      */
/* 																			      */
/* DESCRIPTION: hash functions  API */
/**********************************************************************************/
#ifndef _CY_HASH_H
#define _CY_HASH_H


typedef struct hash_unit_s cy_hash_unit_t;

/* for tweakable hash functions (null for most cases, here self_unit is a pointer to the
 * cy_hash_unit_t itself */
typedef cy_error_t (*Fct_Hash_Configure_t)(cy_hash_unit_t *self_unit, uint8_t *initializer,
                                     size_t initializer_t8);

/* reason of the untyped (void*) :ctx is a pointer to the hashing context; which is different for each functions
 * for pedersen, it will adress a structure containing elliptic params
 */
typedef cy_error_t (*Fct_Hash_Init_t)(void *io_ps_ctx, uint8_t *in,
                                     size_t t8_in);
typedef cy_error_t (*Fct_Hash_Update_t)(void *io_ps_ctx, uint8_t *in,
                                    size_t t8_in);
typedef cy_error_t (*Fct_Hash_Final_t)(void *io_ps_ctx, uint8_t *out, size_t sizeout);

/* release memory*/
typedef cy_error_t (*Fct_Hash_Uninit_t)(cy_hash_unit_t *self_unit);


typedef cy_error_t (*Fct_Hash_All_t)(uint8_t *in, size_t in_t8, uint8_t *out);

struct hash_unit_s {
  uint8_t *Sec_RAMp; /* pointer between the Shared RAM and the component*/

  uint64_t id; /**< Component identifier                                   */
  unsigned char Component_name[MAX_SIZENAME];

  /*Hash subfunctions*/
  Fct_Hash_Configure_t Hash_Configure;

  Fct_Hash_Init_t Hash_Init;
  Fct_Hash_Update_t Hash_Update; /**< pointer to run function                   */
  Fct_Hash_Final_t Hash_Final;

  Fct_Hash_All_t Hash_All; /* one shot init, run, final execution */
  Fct_Hash_Configure_t Hash_Uninit; /* release configure*/

  cy_flag_t is_initialized;

  void *ctx;
  size_t state_t8;
  size_t Hash_t8;
  void *opt;
  size_t opt_t8;
};

/* todo: provide ready to use structures */
#define _SHA512_ID 1
#define _KECCAK256_ID 2
#define _PEDDERSEN_ID 3

#define MAX_HASH_T8 64

#endif

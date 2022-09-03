/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois - This file is part of cy_lib project						  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_hybridsig.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: 2 round_multisignature signatures APIS                                    */
/*
/**********************************************************************************/


struct musig2_ctx_s {
  uint8_t *Sec_RAMp; /* pointer between the Shared RAM and the component*/



  void *opt;
  size_t opt_t8;
};

typedef struct musig2_ctx_s musig2_ctx_t;

/* Initialize Musig with Signatures functions, number of users, hash functions */
cy_error_t cy_musig_SetUp(cy_musig2_ctx_t *ctx,  uint8_t *Ramp, size_t sizeRamp,
						  uint8_t *initializer, size_t init_t,
			              const cy_hash_ctx_t *H_agg, const cy_hash_ctx_t *H_non, const cy_hash_ctx_t *H_sig);

cy_error_t cy_musig_KeyAgg(const cy_musig2_ctx_t *ctx,const size_t n_users, const cy_ec_point_t *publickeys, cy_ec_point_t *keyagg);


/*Signature of users Sign in Round 1*/
cy_error_t cy_musig_Sign_Round1_all(const cy_musig2_ctx_t *ctx,const size_t n_users,  const size_t index, cy_ec_point_t *keyagg);


/*Signature of users Sign' in Round 2*/
cy_error_t cy_musig_Sign_Round2_all(const cy_musig2_ctx_t *ctx,const size_t n_users, const cy_ec_point_t *publickeys, const size_t index, cy_ec_point_t *keyagg);

cy_error_t cy_musig_Sign_Round2_Init(const cy_musig2_ctx_t *ctx,const size_t n_users, const cy_ec_point_t *publickeys, const size_t index, cy_ec_point_t *keyagg);
cy_error_t cy_musig_Sign_Round2_Update(const cy_musig2_ctx_t *ctx,const size_t n_users, const cy_ec_point_t *publickeys, const size_t index, cy_ec_point_t *keyagg);
cy_error_t cy_musig_Sign_Round2_Final(const cy_musig2_ctx_t *ctx,const size_t n_users, const cy_ec_point_t *publickeys, const size_t index, cy_ec_point_t *keyagg);


/*Aggregator */
cy_error_t cy_musig_SignAgg_Round1(const cy_musig2_ctx_t *ctx,const size_t n_users, const cy_ec_point_t *publickeys, const size_t index, cy_ec_point_t *keyagg);
cy_error_t cy_musig_SignAgg_Round2(const cy_musig2_ctx_t *ctx,const size_t n_users, const cy_ec_point_t *publickeys, const size_t index, cy_ec_point_t *keyagg);

/* Initialize Musig with Signatures functions, number of users, hash functions */
cy_error_t cy_musig_Uninit(cy_musig2_ctx_t *ctx);


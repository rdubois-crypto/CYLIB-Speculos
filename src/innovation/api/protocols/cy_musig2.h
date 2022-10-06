/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois - This file is part of cy_lib project						  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_musig2.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: 2 round_multisignature signatures APIS                                    */
/* https://eprint.iacr.org/2020/1261.pdf    */
/**********************************************************************************/


#ifndef API_CY_MUSIG2_H_
#define API_CY_MUSIG2_H_


struct musig2_ctx_s {
  uint8_t *Sec_RAMp; /* pointer between the Shared RAM and the component*/

  cy_hash_unit_t *H;/* hash underlying function, used to build hash to scalar function */

  cy_ec_ctx_t *ctx_ec; /* elliptic context for elliptic curves operations 	*/
  cy_fp_ctx_t *ctx_order;	         /* modular context to perform operation over the scalar field */
  cy_gda_ctx_t *gda;   /* random generator */
  unsigned int n_users;

  void *opt;		   /* reserved for future use*/
  size_t opt_t8;	   /* size of optional previous field( future use)*/
};

typedef struct musig2_ctx_s cy_musig2_ctx_t;

/* the mu constant is the size of the vector of nonces by user*/
#define _MU_ 2

/* arbitrary constant for domain separation, beware to be equal with sage simulation */
#define _SEPARATION_SIG 0x1
#define _SEPARATION_NON 0x2
#define _SEPARATION_AGG 0x3

/* Initialize Musig with Signatures functions, number of users, hash functions (H is used for agg,nonce and sig)*/
extern cy_error_t cy_musig_SetUp( cy_hash_unit_t *H, uint8_t *Elliptic_Ramp, size_t Elliptic_Ramp_t8, int curve,  int n_users,
							   cy_ec_ctx_t *ec_ctx, cy_musig2_ctx_t *musig_ctx);


extern cy_error_t cy_Hagg(cy_musig2_ctx_t *musig_ctx, const cy_ecpoint_t *L, int index_i, uint8_t *ai);
extern cy_error_t cy_Hnon(cy_musig2_ctx_t *musig_ctx, const cy_ecpoint_t *XAgg, const cy_ecpoint_t *R,  uint8_t *m, size_t t8_m, uint8_t *b);
extern cy_error_t cy_Hsig(cy_musig2_ctx_t *musig_ctx, const cy_ecpoint_t *KeyAg, const cy_ecpoint_t *R, uint8_t *m, size_t t8_m, cy_fp_t *c);


/* compute the aggregation (sum) of public keys*/
extern cy_error_t cy_musig_KeyAgg(cy_musig2_ctx_t *musig_ctx, const cy_ecpoint_t *ec_L, cy_ecpoint_t *KeyAgg);

/*************************************************************/
/* Single user functions*/
/*************************************************************/


extern cy_error_t cy_musig_KeyGenDeriv(const cy_musig2_ctx_t *ctx, const cy_fp_t *random,
								cy_fp_t *xpriv, cy_ecpoint_t *X_pub);

/*Signature of users Sign in Round 1*/
cy_error_t cy_musig_Sign_Round1(const cy_musig2_ctx_t *ctx, cy_fp_t *ri, cy_ecpoint_t *Ri);

/*Signature of users Sign' in Round 2*/
cy_error_t cy_musig_Sign_Round2_all(const cy_musig2_ctx_t *ctx,const size_t n_users,  const cy_fp_t *ai, const cy_ecpoint_t **vec_sigagg,
									const uint8_t *message, const size_t message_t8,
									cy_fp_t *s_i);


/* Limited stack compatible versions  (to be done)*/
cy_error_t cy_musig_Sign_Round2_Init(const cy_musig2_ctx_t *ctx,const size_t n_users, const cy_ecpoint_t *publickeys, const size_t index, cy_ecpoint_t *keyagg);
cy_error_t cy_musig_Sign_Round2_Update(const cy_musig2_ctx_t *ctx,const size_t n_users, const cy_ecpoint_t *publickeys, const size_t index, cy_ecpoint_t *keyagg);
cy_error_t cy_musig_Sign_Round2_Final(const cy_musig2_ctx_t *ctx,const size_t n_users, const cy_ecpoint_t *publickeys, const size_t index, cy_ecpoint_t *keyagg);


/* core function with external hashing */
extern cy_error_t cy_musig_Verification_Core(cy_musig2_ctx_t *ctx, cy_ecpoint_t *Key_agg,
		cy_ecpoint_t *R, cy_fp_t *fp_s, cy_fp_t *fp_c,
		boolean_t *flag_verif);

/* one shot verification with internal hashing*/
extern cy_error_t cy_musig_Verification_All(cy_musig2_ctx_t *ctx, cy_ecpoint_t *Key_agg,
		cy_ecpoint_t *R, cy_fp_t *fp_s, uint8_t *message, size_t t8_message,
		boolean_t *flag_verif);

/* limited stack implementation: (to be done)*/
cy_error_t cy_musig_Verification_Init(cy_musig2_ctx_t *ctx, cy_ecpoint_t *Key_agg,
		cy_ecpoint_t *R, cy_fp_t *s, cy_fp_t *c);

cy_error_t cy_musig_Verification_Update(cy_musig2_ctx_t *ctx, cy_ecpoint_t *Key_agg,
		cy_ecpoint_t *R, cy_fp_t *s, cy_fp_t *c,
		const uint8_t *message, const size_t message_t8);

cy_error_t cy_musig_Verification_Final(cy_musig2_ctx_t *ctx, cy_ecpoint_t *Key_agg,
		cy_ecpoint_t *R, cy_fp_t *s, cy_fp_t *c);

/*************************************************************/
/* Aggregator functions*/
/*************************************************************/

cy_error_t cy_musig_SigAgg_Round1(cy_musig2_ctx_t *ctx, const size_t n_users, const cy_ecpoint_t **vec_sig, cy_ecpoint_t **vec_sigagg);
cy_error_t cy_musig_SigAgg_Round2(const size_t n_users, const cy_fp_t **vec_sig2, cy_fp_t *sig);

/* Initialize Musig with Signatures functions, number of users, hash functions */
extern cy_error_t cy_musig_Uninit(cy_musig2_ctx_t *ctx);

/* Maximal size of ec element supported by this musig2 implementation */
#define MAX_MUSIG_EC_T8 128
#define MAX_MUSIG_HASH_T8 128

#endif //API_CY_MUSIG2_H_

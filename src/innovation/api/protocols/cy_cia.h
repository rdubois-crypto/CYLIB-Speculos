/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois - This file is part of cy_lib project		  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.							  */
/* FILE: cy_musig2.h										   		              */
/* 																			      */
/* 																			      */
/* DESCRIPTION: A module to cipher and sign a memory unit                         */
/* https://eprint.iacr.org/2020/1261.pdf    									  */
/**********************************************************************************/

#ifndef CY_MEM_CIA


#define _T8_BLOC 64

#define T8_FILE_BUFF 1024
#define T8_SECRET_APP 64 /* to be confirmed, oversize won't cause crash */
#define T8_IV 64
#define T8_MAXNAME 64
#define T8_KEYHMAC 32

#define _CIA_ENCRYPT 0
#define _CIA_DECRYPT 1

#define _FILE_UNINIT 0
#define _FILE_W 1
#define _FILE_R 2
#define _FILE_A 3
#define _FILE_CLOSED 4

#define _CIA_HASH CX_SHA256 	/* used to authenticate */
#define _CIA_CIPHER CX_SHA512	/* used in CTR mode to cipher */
#define SIZE_PUBKEY_ENDORSEMENT 66

struct cy_FILE_s {
  int state;
  size_t t8_file; /* bytelength of output file: contains header and sign overhead */
  uint8_t sym_sk[T8_SECRET_APP];
  char pathname[T8_MAXNAME];
  uint8_t iv[T8_IV];
  cx_hmac_ctx ctx_hmac;
  uint8_t pubkey[SIZE_PUBKEY_ENDORSEMENT];

  uint8_t buffer[T8_IV+T8_FILE_BUFF];

};

typedef struct cy_FILE_s cy_FILE_t;


extern cy_FILE_t* cy_fopen(const char *pathname, const char *mode);
extern size_t     cy_fwrite(const void *ptr, size_t size, size_t nmemb,
		cy_FILE_t *stream);

extern int cy_fclose(cy_FILE_t *stream);
extern size_t cy_fread(void *restrict ptr, size_t size, size_t nmemb,
		cy_FILE_t *restrict stream);

/* A ciphered and sign mode of operation:*/
extern cy_error_t cy_CIA_Init(uint8_t *iv, char *path, cx_hmac_ctx *ctx_hmac, uint8_t *key, uint8_t *buffer, int mode);
extern cy_error_t cy_CIA_Update(uint8_t *iv, cx_hmac_ctx *ctx_hmac, const uint8_t *key, const  uint8_t *data, const size_t t8_data, uint8_t *ciphered);
extern cy_error_t cy_CIA_Final(uint8_t *iv, cx_hmac_ctx *ctx_hmac,uint8_t *sig);



#endif

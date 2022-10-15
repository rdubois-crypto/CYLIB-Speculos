#include <stdint.h>
#include <stddef.h>

#include "cy_def.h"
#include "cy_errors.h"



#include "bolos/cx.h"
#include "bolos/cx_ec.h"
#include "emulate.h"
#include "bolos/cx_hash.h"
#include "bolos/cx_hmac.h"
#include "bolos/cxlib.h"
#include "bolos/endorsement.h"

#include "cy_cia.h"


#define _SPECULOS
#ifdef _SPECULOS

#define cx_ecfp_init_public_key      sys_cx_ecfp_init_public_key
#define cx_hash                      sys_cx_hash
#define cx_ecdsa_verify              sys_cx_ecdsa_verify
#define os_endorsement_get_code_hash sys_os_endorsement_get_code_hash
#define os_endorsement_get_public_key_certificate                              \
  sys_os_endorsement_get_public_key_certificate

static int os_endorsement_key1_get_app_secret(uint8_t *buffer)
{
 size_t i;
 for(i=0;i<T8_SECRET_APP;i++)
	 buffer[i]=i;
 return CY_OK;
}


#define _T8_PUBKEY 65
#define _T8_SIG 64

#define BLS_ENDORSEMENT_KEY1 1

#endif


cy_FILE_t global_file_ptr={
		_FILE_UNINIT,


};


int sys_os_endorsement_key1_verif_data(uint8_t *buffer, size_t t8_buffer,  uint8_t *sig)
{
 cx_ecfp_public_key_t endorsement_pubkey;
 cx_sha256_t ctx_sha256;
 int res;

 sys_os_endorsement_get_public_key(BLS_ENDORSEMENT_KEY1, global_file_ptr.pubkey);
 sys_cx_ecfp_init_public_key(CX_CURVE_SECP256K1, global_file_ptr.pubkey, _T8_PUBKEY, &endorsement_pubkey);
 //uint8_t certif_prefix = 0xFE;
 uint8_t hash[32];

 (cx_sha256_init(&ctx_sha256));
 //(cx_hash(&ctx_sha256.header, 0, &certif_prefix, 1, NULL, 0));
 (cx_hash(&ctx_sha256.header, CX_LAST, buffer, t8_buffer, hash, 32));

 res=cx_ecdsa_verify(&endorsement_pubkey, CX_LAST, CX_SHA256,
                                    hash, sizeof(hash), sig,
									_T8_SIG);

 return res;
}

static void inc_iv(uint8_t *stream)
{
	size_t i=0;
	int carry=1;

	while(carry && (i<_T8_BLOC))
	{
		stream[i]++;
		carry=(stream[i]==0);
	}
}


cy_FILE_t* cy_fopen(const char *pathname, const char *mode){
	UNUSED(pathname);
	UNUSED(mode);

	/* derive asym and symkey of app*/
	os_endorsement_key1_get_app_secret(global_file_ptr.sym_sk);


	/*sys_os_endorsement_get_public_key(uint8_t index, uint8_t *buffer);*/
	if(strcmp(pathname, "w")==0){
		if(global_file_ptr.state!=_FILE_UNINIT) return NULL;
		cy_CIA_Init(global_file_ptr.iv, global_file_ptr.pathname, &global_file_ptr.ctx_hmac, global_file_ptr.sym_sk, global_file_ptr.buffer, _CIA_ENCRYPT);
		global_file_ptr.state=_FILE_W;
	}

	if(strcmp(pathname, "a")==0){
		if(global_file_ptr.state!=_FILE_CLOSED) return NULL;
		global_file_ptr.state=_FILE_A;
	}

	if(strcmp(pathname, "r")==0){
		if(global_file_ptr.state!=_FILE_CLOSED) return NULL;
		global_file_ptr.state=_FILE_R;
	}

	/* initialize context*/

	return &global_file_ptr;
}

/*
size_t cy_fread(void *restrict ptr, size_t size, size_t nmemb,
		cy_FILE_t *restrict stream){
	if(global_file_ptr.state!=_FILE_R) return 0;


}*/

size_t cy_fwrite(const void *ptr, size_t size, size_t nmemb,
		cy_FILE_t *stream){

	cy_error_t error;

	if(global_file_ptr.state!=_FILE_W) return 0;

    CY_CHECK(cy_CIA_Update(stream->iv, &stream->ctx_hmac, stream->sym_sk, (const uint8_t*) ptr, nmemb*size, stream->buffer));

    end:

	return (size_t) error;
}

int cy_fclose(cy_FILE_t *stream){

	UNUSED(stream);
	return 0;
}


/* the CIA mode uses:
 * HMAC-512 in CTR_mode for ciphering to enable HW acceleration
 * ECDSA for authentication of data to host (no injection possible)
 * The computed signature over data is Sign(Hmac(IV||Ciphered_data))

 * The integrity is implicitely reached through signature verification
 * in cipher mode, the iv is an output, in decipher mode it is an input
 */
cy_error_t cy_CIA_Init(uint8_t *iv, const char *path, cx_hmac_ctx *ctx_hmac, const uint8_t *key, uint8_t *buffer, const unsigned int mode)
{
	cy_error_t error;
	uint8_t otp[T8_IV];
	size_t i;

	if(mode==_CIA_ENCRYPT){
		sys_cx_get_random_bytes(otp, sizeof(otp));
		CY_CHECK(spec_cx_hash_sha512((const uint8_t *)path, strlen(path), iv, T8_IV));
		for(i=0;i<T8_IV;i++)
		{
			iv[i]^=otp[i];
		}
	}


	CY_CHECK(spec_cx_hmac_init(ctx_hmac, _CIA_HASH,key , T8_KEYHMAC));
	memcpy(buffer, iv, T8_IV);
	CY_CHECK(spec_cx_hmac_update(ctx_hmac, iv, T8_IV));

	end:
	return error;

}

cy_error_t cy_CIA_Update(uint8_t *iv, cx_hmac_ctx *ctx_hmac, const uint8_t *key, const  uint8_t *data, const size_t t8_data, uint8_t *out){
	cx_hmac_ctx ctx_fork;
	cy_error_t error;
	size_t i,j;
	uint8_t stream[_T8_BLOC];
	size_t trash;

	memcpy(&ctx_fork, ctx_hmac, sizeof(cx_hmac_ctx));
	for(i=0;i<t8_data-_T8_BLOC;i+=_T8_BLOC)
	{
		CY_CHECK(spec_cx_hmac_init(&ctx_fork, _CIA_CIPHER, key , T8_KEYHMAC));
		inc_iv(iv);
		CY_CHECK(spec_cx_hmac_update(&ctx_fork, iv, T8_IV));
		CY_CHECK(spec_cx_hmac_final(&ctx_fork, stream, &trash));

		for(j=0;j<64;j++)
		{
			out[i+j+64]=data[i+j]^stream[i];
		}
		CY_CHECK(spec_cx_hmac_update(ctx_hmac, out, _T8_BLOC));

	}
	/*todo: handling incomplete blocs */

	end:
		return error;
}


cy_error_t cy_CIA_Final(uint8_t *iv, cx_hmac_ctx *ctx_hmac,uint8_t *sig){
	cy_error_t error;
	size_t trash;
	uint8_t stream[_T8_BLOC];
	size_t endorsement_sig_len=0;

	UNUSED(endorsement_sig_len);/* mak ecompiler happy*/
	UNUSED(iv);

	CY_CHECK(spec_cx_hmac_final(ctx_hmac, stream, &trash));
	/*todo: handling incomplete blocs */
	endorsement_sig_len=(sys_os_endorsement_key1_sign_data(stream, _T8_BLOC, sig));

	end:
		return error;
}

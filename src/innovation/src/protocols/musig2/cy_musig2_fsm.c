#include <stdint.h>
#include <stddef.h>

#include "cy_configuration.h"
#include "cy_bn.h"

#include "cy_fp.h"
#include "cy_ec.h"
#include "cy_hash.h"
#include "cy_musig2.h"


#include "cy_ec_const.h"
#include <stdio.h>
#include "cy_io_common_tools.h"
#include "cy_io_fp.h"
#include "cy_io_ec.h"

#define MAX_MUSIG2_USERS 10

#define MUSIG2_USER_INIT 1
#define MUSIG2_USER_KEYED 2
#define MUSIG2_USER_KEYED_WAIT_KEYAGG 3

#define MUSIG2_USER_NONCED_WAIT_R1 4

/* liste of message:
 * change keyagg
 *
 *
 */
struct musig2_fsm_user_s {
  int state;

  cy_fp_t xpriv;
  cy_fp_t ai;
  cy_ecpoint_t Kpub;
  cy_musig2_ctx_t musig;
  cy_ec_ctx_t ctx_ec;
  int role; /* initiator, contributor, aggregator */


  cy_fp_t *rij[MAX_MUSIG2_USERS]; /* nonces for round 1*/

};



typedef struct musig2_fsm_user_s musig2_fsm_user_t;
#ifdef fsm


/*&unit_pedersen, &bolos_gda_component, CY_CURVE_Stark256*/
cy_error_t cy_musig2_starknet_init(musig2_fsm_user_t *ctx, size_t n_users, int role, uint8_t *Ramp, size_t Ramp_t8){

 cy_error_t error;
 ctx->musig.gda=&bolos_gda_component;
 ctx->musig.H=&unit_pedersen;

 CY_CHECK(cy_musig_SetUp( &unit_pedersen, Ramp, Ramp_t8, CY_CURVE_Stark256, n_users, &ctx->ctx_ec, &ctx->musig));
 CY_CHECK(ctx->musig.gda->GDA_Init(ctx->musig.H, NULL, ctx->musig.ctx_ec->ctx_fp_p->t8_modular));

 end:
 return CY_KO;
}

cy_error_t cy_musig2_starknet_uninit(musig2_fsm_user_t *ctx){

 UNUSED(ctx);

 return CY_KO;
}

/***************************************************************************/
/* User FSM	*/
/***************************************************************************/




/* reusing an existing private key */
cy_error_t cy_musig2_starknet_user_init_from_sk(musig2_fsm_user_t *ctx, uint8_t *sk){
	 UNUSED(ctx);

	 return CY_KO;


}

cy_error_t cy_musig2_starknet_user_send_pubkey(musig2_fsm_user_t *ctx, uint8_t *pubkey[Stark_SIZE_u8]){

	 UNUSED(ctx);
	 UNUSED(pubkey);
	 return CY_KO;

}

/* if flip=1, each user flip its public key*/
cy_error_t cy_musig2_starknet_user_receive_keyagg(musig2_fsm_user_t *ctx, uint8_t *pubkey[Stark_SIZE_u8], uint8_t *keyagg, int sign){

	 UNUSED(ctx);
	 UNUSED(pubkey);
	 return CY_KO;

}

cy_error_t cy_musig2_starknet_user_send_round1(musig2_fsm_user_t *ctx, uint8_t *nonces[Stark_SIZE_u8]){

	 UNUSED(ctx);
	 UNUSED(nonces);
	 return CY_KO;

}

cy_error_t cy_musig2_starknet_user_receive_round1agg(musig2_fsm_user_t *ctx, uint8_t *nonces[Stark_SIZE_u8]){

	 UNUSED(ctx);
	 UNUSED(nonces);
	 return CY_KO;

}


cy_error_t cy_musig2_starknet_user_send_round2(musig2_fsm_user_t *ctx, uint8_t *sig2){

	 UNUSED(ctx);
	 UNUSED(nonces);
	 return CY_KO;

}

/***************************************************************************/
/* Aggregator FSM	*/
/***************************************************************************/
cy_error_t cy_musig2_starknet_agg_receive_pubkey(musig2_fsm_user_t *ctx, uint8_t *pubkey[Stark_SIZE_u8], int role){

	 UNUSED(ctx);
	 UNUSED(pubkey);
	 return CY_KO;

}

#endif



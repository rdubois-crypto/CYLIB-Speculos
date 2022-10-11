#include <stdint.h>
#include <stddef.h>

#include "cy_configuration.h"
#include "cy_bn.h"

#include "cy_fp.h"
#include "cy_ec.h"
#include "cy_hash.h"
#include "cy_musig2.h"


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

  cy_fp_t *xpriv;
  cy_fp_t *ai;
  cy_ecpoint_t *Kpub;
  cy_musig2_ctx_t *musig;

  cy_fp_t *rij[MAX_MUSIG2_USERS]; /* nonces for round 1*/

};



typedef struct musig2_fsm_user_s musig2_fsm_user_t;

cy_error_t cy_musig2_starknet_user_init(musig2_fsm_user_t *ctx){



}

/* reusing an existing private key */
cy_error_t cy_musig2_starknet_user_init_from_sk(musig2_fsm_user_t *ctx){



}


cy_error_t cy_musig2_starknet_user_send_pubkey(musig2_fsm_user_t *ctx, uint8_t *pubkey[_MAX_CYLIB_EC_T8]){



}

cy_error_t cy_musig2_starknet_user_send_round1(musig2_fsm_user_t *ctx, uint8_t *nonces[_MAX_CYLIB_EC_T8]){



}






#include <stdint.h>   // uint*_t
#include <string.h>   // memset, explicit_bzero
#include <stdbool.h>  // bool
#include <stdio.h>
//#include "transaction/types.h"

#define cx_ecpoint_init(a,b,c,d,e) sys_cx_ecpoint_init(a,b,c,d,e)
#define cx_ecpoint_alloc(a,b) sys_cx_ecpoint_alloc(a,b)
#define cx_ecpoint_destroy(a) sys_cx_ecpoint_destroy(a)
#define cx_ecfp_scalar_mult(r,k,a,b,c) sys_cx_ecfp_scalar_mult(r,k,a,b,c)
#define cx_ecfp_add_point(a,b,c,d,e) sys_cx_ecfp_add_point(a,b,c,d,e)

#define PRINTF(a) printf(a)

#include "bolos/cxlib.h"
//#include "bolos/os.h"
//#include "bolos/cx.h"

//#include "bolos/os_io_seproxyhal.h"

//#include "bolos/hash.h"

/* EC points */
#define FIELD_ELEMENT_SIZE (32)
#define EC_POINT_SIZE      (2 * FIELD_ELEMENT_SIZE + 1)
typedef unsigned char FieldElement[FIELD_ELEMENT_SIZE];
typedef unsigned char ECPoint[EC_POINT_SIZE];

#define SIGNATURE_MAX_LEN (72)

static const ECPoint PEDERSEN_SHIFT[] = {{
    0x04,

    0x04, 0x9e, 0xe3, 0xeb, 0xa8, 0xc1, 0x60, 0x07, 0x00, 0xee, 0x1b, 0x87, 0xeb, 0x59, 0x9f, 0x16,
    0x71, 0x6b, 0x0b, 0x10, 0x22, 0x94, 0x77, 0x33, 0x55, 0x1f, 0xde, 0x40, 0x50, 0xca, 0x68, 0x04,

    0x03, 0xca, 0x0c, 0xfe, 0x4b, 0x3b, 0xc6, 0xdd, 0xf3, 0x46, 0xd4, 0x9d, 0x06, 0xea, 0x0e, 0xd3,
    0x4e, 0x62, 0x10, 0x62, 0xc0, 0xe0, 0x56, 0xc1, 0xd0, 0x40, 0x5d, 0x26, 0x6e, 0x10, 0x26, 0x8a,
}};

static const ECPoint PEDERSEN_POINTS[4] = {
    {
        0x04,

        0x02, 0x34, 0x28, 0x7d, 0xcb, 0xaf, 0xfe, 0x7f, 0x96, 0x9c, 0x74,
        0x86, 0x55, 0xfc, 0xa9, 0xe5, 0x8f, 0xa8, 0x12, 0x0b, 0x6d, 0x56,
        0xeb, 0x0c, 0x10, 0x80, 0xd1, 0x79, 0x57, 0xeb, 0xe4, 0x7b,

        0x03, 0xb0, 0x56, 0xf1, 0x00, 0xf9, 0x6f, 0xb2, 0x1e, 0x88, 0x95,
        0x27, 0xd4, 0x1f, 0x4e, 0x39, 0x94, 0x01, 0x35, 0xdd, 0x7a, 0x6c,
        0x94, 0xcc, 0x6e, 0xd0, 0x26, 0x8e, 0xe8, 0x9e, 0x56, 0x15,
    },
    {
        0x04,

        0x04, 0xfa, 0x56, 0xf3, 0x76, 0xc8, 0x3d, 0xb3, 0x3f, 0x9d, 0xab,
        0x26, 0x56, 0x55, 0x8f, 0x33, 0x99, 0x09, 0x9e, 0xc1, 0xde, 0x5e,
        0x30, 0x18, 0xb7, 0xa6, 0x93, 0x2d, 0xba, 0x8a, 0xa3, 0x78,

        0x03, 0xfa, 0x09, 0x84, 0xc9, 0x31, 0xc9, 0xe3, 0x81, 0x13, 0xe0,
        0xc0, 0xe4, 0x7e, 0x44, 0x01, 0x56, 0x27, 0x61, 0xf9, 0x2a, 0x7a,
        0x23, 0xb4, 0x51, 0x68, 0xf4, 0xe8, 0x0f, 0xf5, 0xb5, 0x4d,
    },
    {
        0x04,

        0x04, 0xba, 0x4c, 0xc1, 0x66, 0xbe, 0x8d, 0xec, 0x76, 0x49, 0x10,
        0xf7, 0x5b, 0x45, 0xf7, 0x4b, 0x40, 0xc6, 0x90, 0xc7, 0x47, 0x09,
        0xe9, 0x0f, 0x3a, 0xa3, 0x72, 0xf0, 0xbd, 0x2d, 0x69, 0x97,

        0x00, 0x40, 0x30, 0x1c, 0xf5, 0xc1, 0x75, 0x1f, 0x4b, 0x97, 0x1e,
        0x46, 0xc4, 0xed, 0xe8, 0x5f, 0xca, 0xc5, 0xc5, 0x9a, 0x5c, 0xe5,
        0xae, 0x7c, 0x48, 0x15, 0x1f, 0x27, 0xb2, 0x4b, 0x21, 0x9c,
    },
    {
        0x04,

        0x05, 0x43, 0x02, 0xdc, 0xb0, 0xe6, 0xcc, 0x1c, 0x6e, 0x44, 0xcc,
        0xa8, 0xf6, 0x1a, 0x63, 0xbb, 0x2c, 0xa6, 0x50, 0x48, 0xd5, 0x3f,
        0xb3, 0x25, 0xd3, 0x6f, 0xf1, 0x2c, 0x49, 0xa5, 0x82, 0x02,

        0x01, 0xb7, 0x7b, 0x3e, 0x37, 0xd1, 0x35, 0x04, 0xb3, 0x48, 0x04,
        0x62, 0x68, 0xd8, 0xae, 0x25, 0xce, 0x98, 0xad, 0x78, 0x3c, 0x25,
        0x56, 0x1a, 0x87, 0x9d, 0xcc, 0x77, 0xe9, 0x9c, 0x24, 0x26,
    }};

const uint8_t INVOKE[6] = {
    0x69, 0x6e, 0x76, 0x6f, 0x6b, 0x65
};

const uint8_t DECLARE[7] = {
    0x64, 0x65, 0x63, 0x6c, 0x61, 0x72, 0x65
};

const uint8_t DEPLOY[6] = {
    0x64, 0x65, 0x70, 0x6c, 0x6f, 0x79
};

const uint8_t L1_HANDLER[10] = {
    0x6c, 0x31, 0x5f, 0X68, 0x61, 0x6e, 0x64, 0x6c, 0x65, 0x72
};

static __attribute__((no_instrument_function)) inline int allzeroes(void *buf, size_t n) {
    uint8_t *p = (uint8_t *) buf;
    for (size_t i = 0; i < n; ++i) {
        if (p[i]) {
            return 0;
        }
    }
    return 1;
}

cx_err_t double_accum_ec_mul(cx_ecpoint_t *hash, uint8_t *buf1, int len1, uint8_t *buf2, int len2, int pedersen_idx)
{
	 cx_err_t error;

	 cx_ecpoint_t tmp1, tmp2;

	 CX_CHECK(cx_ecpoint_alloc(&tmp1, CX_CURVE_Stark256));
	 CX_CHECK(cx_ecpoint_alloc(&tmp2, CX_CURVE_Stark256));


	 if (!allzeroes(buf1, len1)&&!allzeroes(buf2, len2))
	 {
		  uint8_t pad1[FIELD_ELEMENT_SIZE];
		  uint8_t pad2[FIELD_ELEMENT_SIZE];
		  const uint8_t *px= ((const uint8_t *)PEDERSEN_POINTS[pedersen_idx])+1;
		  const uint8_t *py= ((const uint8_t *)PEDERSEN_POINTS[pedersen_idx])+1+FIELD_ELEMENT_SIZE;


		 //memcpy(tmp1, PEDERSEN_POINTS[pedersen_idx], sizeof(ECPoint));
			CX_CHECK(cx_ecpoint_init (&tmp1, px, FIELD_ELEMENT_SIZE, py, FIELD_ELEMENT_SIZE)  );

		        //io_seproxyhal_io_heartbeat();
		        memset(pad1, 0, FIELD_ELEMENT_SIZE);
		        memmove(pad1 + 32 - len1, buf1, len1);

		       px= ((const uint8_t *)PEDERSEN_POINTS[pedersen_idx+2])+1;
		       py= ((const uint8_t *)PEDERSEN_POINTS[pedersen_idx+2])+1+FIELD_ELEMENT_SIZE;
		       CX_CHECK(cx_ecpoint_init (&tmp2, px, FIELD_ELEMENT_SIZE, py, FIELD_ELEMENT_SIZE)  );

		       memset(pad2, 0, sizeof(pad2));
		       memmove(pad2 + 32 - len2, buf2, len2);
		       CX_CHECK(sys_cx_ecpoint_double_scalarmul(&tmp1, &tmp1, &tmp2, pad1, sizeof(pad1), pad2,  sizeof(pad2)));
		       CX_CHECK(sys_cx_ecpoint_add(hash, hash, &tmp1));
	 }

	 CX_CHECK( cx_ecpoint_destroy(&tmp1));
	 CX_CHECK( cx_ecpoint_destroy(&tmp2));

	 end:
	 	 return error;
}


cx_err_t pedersen_x2(
    FieldElement res,
    FieldElement a,
    FieldElement b) {

	cx_err_t error;
    cx_ecpoint_t ec_hash;
    CX_CHECK(cx_ecpoint_alloc(&ec_hash, CX_CURVE_Stark256));

    PRINTF("Pedersen: IN\n");

   // memcpy(hash, PEDERSEN_SHIFT, sizeof(hash));
    uint8_t *px= ((uint8_t *)PEDERSEN_SHIFT)+1;
    uint8_t *py= ((uint8_t *)PEDERSEN_SHIFT)+1+FIELD_ELEMENT_SIZE;

    CX_CHECK(cx_ecpoint_init (&ec_hash, px, FIELD_ELEMENT_SIZE, py, FIELD_ELEMENT_SIZE)  );

    CX_CHECK(double_accum_ec_mul(&ec_hash,a,1,b,1,1));
    CX_CHECK(double_accum_ec_mul(&ec_hash,a+1,FIELD_ELEMENT_SIZE,b+1,FIELD_ELEMENT_SIZE,3));

    CX_CHECK(sys_cx_ecpoint_export(&ec_hash,res, FIELD_ELEMENT_SIZE, NULL, 0 ));

    CX_CHECK( cx_ecpoint_destroy(&ec_hash));

    PRINTF("Pedersen: OUT\n");
	 end:
	 	 return error;
}



static void accum_ec_mul(ECPoint *hash, uint8_t *buf, int len, int pedersen_idx) {
    ECPoint tmp;
    if (!allzeroes(buf, len)) {
        uint8_t pad[32];
        memcpy(tmp, PEDERSEN_POINTS[pedersen_idx], sizeof(ECPoint));
        //io_seproxyhal_io_heartbeat();
        memset(pad, 0, sizeof(pad));
        memmove(pad + 32 - len, buf, len);
        cx_ecfp_scalar_mult(CX_CURVE_Stark256, tmp, sizeof(ECPoint), pad, sizeof(pad));
        //io_seproxyhal_io_heartbeat();
        cx_ecfp_add_point(CX_CURVE_Stark256, *hash, *hash, tmp, sizeof(ECPoint));
    }
}

static void pedersen(
    FieldElement res,
    FieldElement a,
    FieldElement b) {

    ECPoint hash;

    PRINTF("Pedersen: IN\n");

    memcpy(hash, PEDERSEN_SHIFT, sizeof(hash));

    accum_ec_mul(&hash, a, 1, 1);
    accum_ec_mul(&hash, a + 1, FIELD_ELEMENT_SIZE - 1, 0);
    accum_ec_mul(&hash, b, 1, 3);
    accum_ec_mul(&hash, b + 1, FIELD_ELEMENT_SIZE - 1, 2);

    memcpy(res, hash + 1, FIELD_ELEMENT_SIZE);

    PRINTF("Pedersen: OUT\n");
}

cx_err_t test_pedersenx2()
{
	size_t i;
	int flag=true;
	flag=flag&flag;/* make compiler happy*/

	cx_err_t error;
	FieldElement a={  0x02, 0x6b, 0x0b, 0x10, 0x22, 0x94, 0x77, 0x33, 0x55, 0x1f, 0xde, 0x40, 0x50, 0xca, 0x68, 0x04,
		    0x03, 0xca, 0x0c, 0xfe, 0x4b, 0x3b, 0xc6, 0xdd, 0xf3, 0x46, 0xd4, 0x9d, 0x06, 0xea, 0x0e, 0xd3};

	FieldElement b={ 0x03, 0xca, 0x0c, 0xfe, 0x4b, 0x3b, 0xc6, 0xdd, 0xf3, 0x46, 0xd4, 0x9d, 0x06, 0xea, 0x0e, 0xd3,
		    0x4e, 0x62, 0x10, 0x62, 0xc0, 0xe0, 0x56, 0xc1, 0xd0, 0x40, 0x5d, 0x26, 0x6e, 0x10, 0x26, 0x8a};

	FieldElement res;
	FieldElement res2;

	pedersen(res,a,b);
	CX_CHECK(pedersen_x2(res,a,b));

	PRINTF("\n Test Pedersen x2:");
	for(i=0;i<sizeof(res);i++)
	{
		if(res[i]!=res2[i]){
			flag=false;
		}
		PRINTF(" KO");
	}

	PRINTF(" OK");
	 end:
		 	 return error;
}










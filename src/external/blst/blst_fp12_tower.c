/*
 * Copyright Supranational LLC
 * Licensed under the Apache License, Version 2.0, see LICENSE for details.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "blst_fields.h"
#define __BLST_NO_ASM__
/*
 * Fp2  = Fp[u]  / (u^2 + 1)
 * Fp6  = Fp2[v] / (v^3 - u - 1)
 * Fp12 = Fp6[w] / (w^2 - v)
 */

static inline void mul_by_u_plus_1_fp2(vec384x ret, const vec384x a)
{
  mul_by_1_plus_i_mod_384x(ret, a, BLS12_381_P);
}

/*
 * Fp6 extension
 */
statik void mul_fp6(vec384fp6 ret, const vec384fp6 a, const vec384fp6 b)
{
  vec384x t0, t1, t2, t3, t4, t5;

  mul_fp2(t0, a[0], b[0]);
  mul_fp2(t1, a[1], b[1]);
  mul_fp2(t2, a[2], b[2]);

  /* ret[0] = ((a1 + a2)*(b1 + b2) - a1*b1 - a2*b2)*(u+1) + a0*b0
            = (a1*b2 + a2*b1)*(u+1) + a0*b0 */
  add_fp2(t4, a[1], a[2]);
  add_fp2(t5, b[1], b[2]);
  mul_fp2(t3, t4, t5);
  sub_fp2(t3, t3, t1);
  sub_fp2(t3, t3, t2);
  mul_by_u_plus_1_fp2(t3, t3);
  /* add_fp2(ret[0], t3, t0); considering possible aliasing... */

  /* ret[1] = (a0 + a1)*(b0 + b1) - a0*b0 - a1*b1 + a2*b2*(u+1)
            = a0*b1 + a1*b0 + a2*b2*(u+1) */
  add_fp2(t4, a[0], a[1]);
  add_fp2(t5, b[0], b[1]);
  mul_fp2(ret[1], t4, t5);
  sub_fp2(ret[1], ret[1], t0);
  sub_fp2(ret[1], ret[1], t1);
  mul_by_u_plus_1_fp2(t4, t2);
  add_fp2(ret[1], ret[1], t4);

  /* ret[2] = (a0 + a2)*(b0 + b2) - a0*b0 - a2*b2 + a1*b1
            = a0*b2 + a2*b0 + a1*b1 */
  add_fp2(t4, a[0], a[2]);
  add_fp2(t5, b[0], b[2]);
  mul_fp2(ret[2], t4, t5);
  sub_fp2(ret[2], ret[2], t0);
  sub_fp2(ret[2], ret[2], t2);
  add_fp2(ret[2], ret[2], t1);

  add_fp2(ret[0], t3, t0); /* ... moved from above */
}

statik void sqr_fp6(vec384fp6 ret, const vec384fp6 a)
{
  vec384x s0, m01, m12, s2;

  sqr_fp2(s0, a[0]);

  mul_fp2(m01, a[0], a[1]);
  add_fp2(m01, m01, m01);

  mul_fp2(m12, a[1], a[2]);
  add_fp2(m12, m12, m12);

  sqr_fp2(s2, a[2]);

  /* ret[2] = (a0 + a1 + a2)^2 - a0^2 - a2^2 - 2*(a0*a1) - 2*(a1*a2)
            = a1^2 + 2*(a0*a2) */
  add_fp2(ret[2], a[2], a[1]);
  add_fp2(ret[2], ret[2], a[0]);
  sqr_fp2(ret[2], ret[2]);
  sub_fp2(ret[2], ret[2], s0);
  sub_fp2(ret[2], ret[2], s2);
  sub_fp2(ret[2], ret[2], m01);
  sub_fp2(ret[2], ret[2], m12);

  /* ret[0] = a0^2 + 2*(a1*a2)*(u+1) */
  mul_by_u_plus_1_fp2(ret[0], m12);
  add_fp2(ret[0], ret[0], s0);

  /* ret[1] = a2^2*(u+1) + 2*(a0*a1) */
  mul_by_u_plus_1_fp2(ret[1], s2);
  add_fp2(ret[1], ret[1], m01);
}

statik void add_fp6(vec384fp6 ret, const vec384fp6 a, const vec384fp6 b)
{
  add_fp2(ret[0], a[0], b[0]);
  add_fp2(ret[1], a[1], b[1]);
  add_fp2(ret[2], a[2], b[2]);
}

statik void sub_fp6(vec384fp6 ret, const vec384fp6 a, const vec384fp6 b)
{
  sub_fp2(ret[0], a[0], b[0]);
  sub_fp2(ret[1], a[1], b[1]);
  sub_fp2(ret[2], a[2], b[2]);
}

statik void neg_fp6(vec384fp6 ret, const vec384fp6 a)
{
  neg_fp2(ret[0], a[0]);
  neg_fp2(ret[1], a[1]);
  neg_fp2(ret[2], a[2]);
}

/*
 * Fp12 extension
 */
statik void mul_fp12(vec384fp12 ret, const vec384fp12 a, const vec384fp12 b)
{
  vec384fp6 t0, t1, t2;

  mul_fp6(t0, a[0], b[0]);
  mul_fp6(t1, a[1], b[1]);

  /* ret[1] = (a0 + a1)*(b0 + b1) - a0*b0 - a1*b1
            = a0*b1 + a1*b0 */
  add_fp6(t2, a[0], a[1]);
  add_fp6(ret[1], b[0], b[1]);
  mul_fp6(ret[1], ret[1], t2);
  sub_fp6(ret[1], ret[1], t0);
  sub_fp6(ret[1], ret[1], t1);

  /* ret[0] = a0*b0 + a1*b1*v */
#ifdef mul_by_v_fp6
  mul_by_v_fp6(t1, t1);
  add_fp6(ret[0], t0, t1);
#else
  mul_by_u_plus_1_fp2(t1[2], t1[2]);
  add_fp2(ret[0][0], t0[0], t1[2]);
  add_fp2(ret[0][1], t0[1], t1[0]);
  add_fp2(ret[0][2], t0[2], t1[1]);
#endif
}

static inline void mul_by_0y0_fp6(vec384fp6 ret, const vec384fp6 a,
                                  const vec384fp2 b)
{
  vec384x t;

  mul_fp2(t, a[2], b);
  mul_fp2(ret[2], a[1], b);
  mul_fp2(ret[1], a[0], b);
  mul_by_u_plus_1_fp2(ret[0], t);
}

statik void mul_by_xy0_fp6(vec384fp6 ret, const vec384fp6 a, const vec384fp6 b)
{
  vec384x t0, t1, /*t2,*/ t3, t4, t5;

  mul_fp2(t0, a[0], b[0]);
  mul_fp2(t1, a[1], b[1]);

  /* ret[0] = ((a1 + a2)*(b1 + 0) - a1*b1 - a2*0)*(u+1) + a0*b0
            = (a1*0 + a2*b1)*(u+1) + a0*b0 */
  mul_fp2(t3, a[2], b[1]);
  mul_by_u_plus_1_fp2(t3, t3);
  /* add_fp2(ret[0], t3, t0); considering possible aliasing... */

  /* ret[1] = (a0 + a1)*(b0 + b1) - a0*b0 - a1*b1 + a2*0*(u+1)
            = a0*b1 + a1*b0 + a2*0*(u+1) */
  add_fp2(t4, a[0], a[1]);
  add_fp2(t5, b[0], b[1]);
  mul_fp2(ret[1], t4, t5);
  sub_fp2(ret[1], ret[1], t0);
  sub_fp2(ret[1], ret[1], t1);

  /* ret[2] = (a0 + a2)*(b0 + 0) - a0*b0 - a2*0 + a1*b1
            = a0*0 + a2*b0 + a1*b1 */
  mul_fp2(ret[2], a[2], b[0]);
  add_fp2(ret[2], ret[2], t1);

  add_fp2(ret[0], t3, t0); /* ... moved from above */
}

statik void mul_by_xy00z0_fp12(vec384fp12 ret, const vec384fp12 a,
                               const vec384fp6 xy00z0)
{
  vec384fp6 t0, t1, t2;

  mul_by_xy0_fp6(t0, a[0], xy00z0);
  mul_by_0y0_fp6(t1, a[1], xy00z0[2]);

  /* ret[1] = (a0 + a1)*(b0 + b1) - a0*b0 - a1*b1
            = a0*b1 + a1*b0 */
  vec_copy(t2[0], xy00z0[0], sizeof(t2[0]));
  add_fp2(t2[1], xy00z0[1], xy00z0[2]);
  add_fp6(ret[1], a[0], a[1]);
  mul_by_xy0_fp6(ret[1], ret[1], t2);
  sub_fp6(ret[1], ret[1], t0);
  sub_fp6(ret[1], ret[1], t1);

  /* ret[0] = a0*b0 + a1*b1*v */
  mul_by_u_plus_1_fp2(t1[2], t1[2]);
  add_fp2(ret[0][0], t0[0], t1[2]);
  add_fp2(ret[0][1], t0[1], t1[0]);
  add_fp2(ret[0][2], t0[2], t1[1]);
}

statik void sqr_fp12(vec384fp12 ret, const vec384fp12 a)
{
  vec384fp6 t0, t1;

  add_fp6(t0, a[0], a[1]);
  mul_by_u_plus_1_fp2(t1[2], a[1][2]);
  add_fp2(t1[0], a[0][0], t1[2]);
  add_fp2(t1[1], a[0][1], a[1][0]);
  add_fp2(t1[2], a[0][2], a[1][1]);
  mul_fp6(t0, t0, t1);
  mul_fp6(t1, a[0], a[1]);

  /* ret[1] = 2*(a0*a1) */
  add_fp6(ret[1], t1, t1);

  /* ret[0] = (a0 + a1)*(a0 + a1*v) - a0*a1 - a0*a1*v
            = a0^2 + a1^2*v */
  sub_fp6(ret[0], t0, t1);
  mul_by_u_plus_1_fp2(t1[2], t1[2]);
  sub_fp2(ret[0][0], ret[0][0], t1[2]);
  sub_fp2(ret[0][1], ret[0][1], t1[0]);
  sub_fp2(ret[0][2], ret[0][2], t1[1]);
}

statik void conjugate_fp12(vec384fp12 a)
{
  neg_fp6(a[1], a[1]);
}

statik void inverse_fp6(vec384fp6 ret, const vec384fp6 a)
{
  vec384x c0, c1, c2, t0, t1;

  /* c0 = a0^2 - (a1*a2)*(u+1) */
  sqr_fp2(c0, a[0]);
  mul_fp2(t0, a[1], a[2]);
  mul_by_u_plus_1_fp2(t0, t0);
  sub_fp2(c0, c0, t0);

  /* c1 = a2^2*(u+1) - (a0*a1) */
  sqr_fp2(c1, a[2]);
  mul_by_u_plus_1_fp2(c1, c1);
  mul_fp2(t0, a[0], a[1]);
  sub_fp2(c1, c1, t0);

  /* c2 = a1^2 - a0*a2 */
  sqr_fp2(c2, a[1]);
  mul_fp2(t0, a[0], a[2]);
  sub_fp2(c2, c2, t0);

  /* (a2*c1 + a1*c2)*(u+1) + a0*c0 */
  mul_fp2(t0, c1, a[2]);
  mul_fp2(t1, c2, a[1]);
  add_fp2(t0, t0, t1);
  mul_by_u_plus_1_fp2(t0, t0);
  mul_fp2(t1, c0, a[0]);
  add_fp2(t0, t0, t1);

  reciprocal_fp2(t1, t0);

  mul_fp2(ret[0], c0, t1);
  mul_fp2(ret[1], c1, t1);
  mul_fp2(ret[2], c2, t1);
}

statik void inverse_fp12(vec384fp12 ret, const vec384fp12 a)
{
  vec384fp6 t0, t1;

  sqr_fp6(t0, a[0]);
  sqr_fp6(t1, a[1]);
  mul_by_u_plus_1_fp2(t1[2], t1[2]);
  sub_fp2(t0[0], t0[0], t1[2]);
  sub_fp2(t0[1], t0[1], t1[0]);
  sub_fp2(t0[2], t0[2], t1[1]);

  inverse_fp6(t1, t0);

  mul_fp6(ret[0], a[0], t1);
  mul_fp6(ret[1], a[1], t1);
  neg_fp6(ret[1], ret[1]);
}

typedef vec384x vec384fp4[2];

statik void sqr_fp4(vec384fp4 ret, const vec384x a0, const vec384x a1)
{
  vec384x t0, t1;

  sqr_fp2(t0, a0);
  sqr_fp2(t1, a1);
  add_fp2(ret[1], a0, a1);

  mul_by_u_plus_1_fp2(ret[0], t1);
  add_fp2(ret[0], ret[0], t0);

  sqr_fp2(ret[1], ret[1]);
  sub_fp2(ret[1], ret[1], t0);
  sub_fp2(ret[1], ret[1], t1);
}

statik void cyclotomic_sqr_fp12(vec384fp12 ret, const vec384fp12 a)
{
  vec384fp4 t0, t1, t2;

  sqr_fp4(t0, a[0][0], a[1][1]);
  sqr_fp4(t1, a[1][0], a[0][2]);
  sqr_fp4(t2, a[0][1], a[1][2]);

  sub_fp2(ret[0][0], t0[0], a[0][0]);
  add_fp2(ret[0][0], ret[0][0], ret[0][0]);
  add_fp2(ret[0][0], ret[0][0], t0[0]);

  sub_fp2(ret[0][1], t1[0], a[0][1]);
  add_fp2(ret[0][1], ret[0][1], ret[0][1]);
  add_fp2(ret[0][1], ret[0][1], t1[0]);

  sub_fp2(ret[0][2], t2[0], a[0][2]);
  add_fp2(ret[0][2], ret[0][2], ret[0][2]);
  add_fp2(ret[0][2], ret[0][2], t2[0]);

  mul_by_u_plus_1_fp2(t2[1], t2[1]);
  add_fp2(ret[1][0], t2[1], a[1][0]);
  add_fp2(ret[1][0], ret[1][0], ret[1][0]);
  add_fp2(ret[1][0], ret[1][0], t2[1]);

  add_fp2(ret[1][1], t0[1], a[1][1]);
  add_fp2(ret[1][1], ret[1][1], ret[1][1]);
  add_fp2(ret[1][1], ret[1][1], t0[1]);

  add_fp2(ret[1][2], t1[1], a[1][2]);
  add_fp2(ret[1][2], ret[1][2], ret[1][2]);
  add_fp2(ret[1][2], ret[1][2], t1[1]);
}

/*
 * caveat lector! |n| has to be non-zero and not more than 3!
 */
static inline void frobenius_map_fp2(vec384x ret, const vec384x a, size_t n)
{
  vec_copy(ret[0], a[0], sizeof(ret[0]));
  cneg_fp(ret[1], a[1], n & 1);
}

statik void frobenius_map_fp6(vec384fp6 ret, const vec384fp6 a, size_t n)
{
  statik const vec384x coeffs1[] = {
    /* (u + 1)^((P^n - 1) / 3) */
    { { 0 },
      { TO_LIMB_T(0xcd03c9e48671f071), TO_LIMB_T(0x5dab22461fcda5d2),
        TO_LIMB_T(0x587042afd3851b95), TO_LIMB_T(0x8eb60ebe01bacb9e),
        TO_LIMB_T(0x03f97d6e83d050d2), TO_LIMB_T(0x18f0206554638741) } },
    { { TO_LIMB_T(0x30f1361b798a64e8), TO_LIMB_T(0xf3b8ddab7ece5a2a),
        TO_LIMB_T(0x16a8ca3ac61577f7), TO_LIMB_T(0xc26a2ff874fd029b),
        TO_LIMB_T(0x3636b76660701c6e), TO_LIMB_T(0x051ba4ab241b6160) } },
    { { 0 }, { ONE_MONT_P } }
  };
  statik const vec384 coeffs2[] = {
    /* (u + 1)^((2P^n - 2) / 3) */
    { TO_LIMB_T(0x890dc9e4867545c3), TO_LIMB_T(0x2af322533285a5d5),
      TO_LIMB_T(0x50880866309b7e2c), TO_LIMB_T(0xa20d1b8c7e881024),
      TO_LIMB_T(0x14e4f04fe2db9068), TO_LIMB_T(0x14e56d3f1564853a) },
    { TO_LIMB_T(0xcd03c9e48671f071), TO_LIMB_T(0x5dab22461fcda5d2),
      TO_LIMB_T(0x587042afd3851b95), TO_LIMB_T(0x8eb60ebe01bacb9e),
      TO_LIMB_T(0x03f97d6e83d050d2), TO_LIMB_T(0x18f0206554638741) },
    { TO_LIMB_T(0x43f5fffffffcaaae), TO_LIMB_T(0x32b7fff2ed47fffd),
      TO_LIMB_T(0x07e83a49a2e99d69), TO_LIMB_T(0xeca8f3318332bb7a),
      TO_LIMB_T(0xef148d1ea0f4c069), TO_LIMB_T(0x040ab3263eff0206) }
  };

  frobenius_map_fp2(ret[0], a[0], n);
  frobenius_map_fp2(ret[1], a[1], n);
  frobenius_map_fp2(ret[2], a[2], n);
  --n; /* implied ONE_MONT_P at index 0 */
  mul_fp2(ret[1], ret[1], coeffs1[n]);
  mul_fp(ret[2][0], ret[2][0], coeffs2[n]);
  mul_fp(ret[2][1], ret[2][1], coeffs2[n]);
}

statik void frobenius_map_fp12(vec384fp12 ret, const vec384fp12 a, size_t n)
{
  statik const vec384x coeffs[] = {
    /* (u + 1)^((P^n - 1) / 6) */
    { { TO_LIMB_T(0x07089552b319d465), TO_LIMB_T(0xc6695f92b50a8313),
        TO_LIMB_T(0x97e83cccd117228f), TO_LIMB_T(0xa35baecab2dc29ee),
        TO_LIMB_T(0x1ce393ea5daace4d), TO_LIMB_T(0x08f2220fb0fb66eb) },
      { TO_LIMB_T(0xb2f66aad4ce5d646), TO_LIMB_T(0x5842a06bfc497cec),
        TO_LIMB_T(0xcf4895d42599d394), TO_LIMB_T(0xc11b9cba40a8e8d0),
        TO_LIMB_T(0x2e3813cbe5a0de89), TO_LIMB_T(0x110eefda88847faf) } },
    { { TO_LIMB_T(0xecfb361b798dba3a), TO_LIMB_T(0xc100ddb891865a2c),
        TO_LIMB_T(0x0ec08ff1232bda8e), TO_LIMB_T(0xd5c13cc6f1ca4721),
        TO_LIMB_T(0x47222a47bf7b5c04), TO_LIMB_T(0x0110f184e51c5f59) } },
    { { TO_LIMB_T(0x3e2f585da55c9ad1), TO_LIMB_T(0x4294213d86c18183),
        TO_LIMB_T(0x382844c88b623732), TO_LIMB_T(0x92ad2afd19103e18),
        TO_LIMB_T(0x1d794e4fac7cf0b9), TO_LIMB_T(0x0bd592fc7d825ec8) },
      { TO_LIMB_T(0x7bcfa7a25aa30fda), TO_LIMB_T(0xdc17dec12a927e7c),
        TO_LIMB_T(0x2f088dd86b4ebef1), TO_LIMB_T(0xd1ca2087da74d4a7),
        TO_LIMB_T(0x2da2596696cebc1d), TO_LIMB_T(0x0e2b7eedbbfd87d2) } },
  };

  frobenius_map_fp6(ret[0], a[0], n);
  frobenius_map_fp6(ret[1], a[1], n);
  --n; /* implied ONE_MONT_P at index 0 */
  mul_fp2(ret[1][0], ret[1][0], coeffs[n]);
  mul_fp2(ret[1][1], ret[1][1], coeffs[n]);
  mul_fp2(ret[1][2], ret[1][2], coeffs[n]);
}

/*
 * BLS12-381-specifc Fp12 shortcuts.
 */
void blst_fp12_sqr(vec384fp12 ret, const vec384fp12 a)
{
  sqr_fp12(ret, a);
}

void blst_fp12_cyclotomic_sqr(vec384fp12 ret, const vec384fp12 a)
{
  cyclotomic_sqr_fp12(ret, a);
}

void blst_fp12_mul(vec384fp12 ret, const vec384fp12 a, const vec384fp12 b)
{
  mul_fp12(ret, a, b);
}

void blst_fp12_mul_by_xy00z0(vec384fp12 ret, const vec384fp12 a,
                             const vec384fp6 xy00z0)
{
  mul_by_xy00z0_fp12(ret, a, xy00z0);
}

void blst_fp12_conjugate(vec384fp12 a)
{
  conjugate_fp12(a);
}

void blst_fp12_inverse(vec384fp12 ret, const vec384fp12 a)
{
  inverse_fp12(ret, a);
}

/* caveat lector! |n| has to be non-zero and not more than 3! */
void blst_fp12_frobenius_map(vec384fp12 ret, const vec384fp12 a, size_t n)
{
  frobenius_map_fp12(ret, a, n);
}

int blst_fp12_is_equal(const vec384fp12 a, const vec384fp12 b)
{
  return (int)vec_is_equal(a, b, sizeof(vec384fp12));
}

int blst_fp12_is_one(const vec384fp12 a)
{
  return (int)(vec_is_equal(a[0][0], BLS12_381_Rx.p2, sizeof(a[0][0])) &
               vec_is_zero(a[0][1], sizeof(vec384fp12) - sizeof(a[0][0])));
}

const vec384fp12 *blst_fp12_one(void)
{
  return (const vec384fp12 *)BLS12_381_Rx.p12;
}

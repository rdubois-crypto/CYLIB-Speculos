

/* Speculos is not emulating Montgomery functions, added here*/
#define _WITH_SPECULOS
#ifdef _WITH_SPECULOS
#define cx_mont_mul(r, a, b, c)           (cy_mont_mul(r, a, b, c))
#define cx_mont_init(ctx_out, n_in)       (cy_mont_init(ctx_out, n_in))
#define cx_mont_to_montgomery(out, in, c) (cy_mont_to_montgomery(out, in, c))
#define cx_mont_from_montgomery(out, in, c) (cy_mont_from_montgomery(out, in, c))
#define cx_bn_mod_invert_nprime(r,a,n) (sys_cx_bn_mod_invert_nprime(r,a,n))
#define cx_bn_mont_ctx_t                  cy_bn_mont_ctx_t
#define cx_mont_alloc(ctx, t8_length)     cy_mont_alloc(ctx, t8_length)


#else
#endif



#define _2POWB                                                                 \
  {                                                                            \
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,    \
        0x00, 0x00, 0x00, 0x00, 0x00                                           \
  } /*2^128*/
#define _2POWBm1                                                               \
  {                                                                            \
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,    \
        0xff, 0xff, 0xff, 0xff                                                 \
  } /* 2^128-1*/

// When the BN_CTX structure is initialized (not NULL), it also means
// 'bn_is_locked'.

/*****************************************************************************/
/*	I. completing missing Speculos functions
 * Those function are for Nano emulation only, use another library for a
 * full software implementation (libECC for instance)
 */
/*****************************************************************************/

/* while not specified, one can check that the montgomery representation used by
 * bolos is a 128 bits multiplier.
 */
struct cy_bn_mont_ctx_s {
  cx_bn_t n;
  //cy_bn_t h2; /* 2^2*bitsizeof(p) mod p */
  /* -p^-1 mod 2^(bitsizeof(word_t)) */
  cx_bn_t mpinv;
  cx_bn_t h; /* 2^bitsizeof(p) mod p, it is also 1 in montgo rep */
  cx_bn_t h2; /* 2^bitsizeof(p) mod p, it is also 1 in montgo rep */

};


typedef struct cy_bn_mont_ctx_s cy_bn_mont_ctx_t;

#define cx_bn_mont_ctx_t                  cy_bn_mont_ctx_t


/* computation of -x mod n=n-x*/
static cx_err_t cy_mod_neg(cx_bn_t o_r, cx_bn_t i_x, cx_bn_t i_n)
{
  cx_err_t error;

  CX_CHECK(sys_cx_bn_mod_sub(o_r, i_n, i_x, i_n)); /*  n-x mod n*/

end:
  return error;
}

/* a fork of cx version with possible even value of n*/
static cx_err_t cy_mpi_mod_pow(cx_mpi_t *r, const cx_mpi_t *a, const cx_mpi_t *e,
                        const cx_mpi_t *n)
{
  // This Function will compute r = pow(a, e) % n
  cx_err_t error;

  BN_CTX *local_bn_ctx = cx_get_bn_ctx();

  if (cx_mpi_is_zero(e)) {
    error = CX_INVALID_PARAMETER;
  } else if (BN_mod_exp_recp(r, a, e, n, local_bn_ctx) == 0) {
    error = CX_INTERNAL_ERROR;
  } else {
    error = CX_OK;
  }
  return error;
}

static cx_err_t sys_cy_bn_mod_pow(cx_bn_t bn_r, const cx_bn_t bn_a,
                           const uint8_t *bytes_e, uint32_t len_e,
                           const cx_bn_t bn_n)
{
  cx_err_t error;
  cx_mpi_t *r, *a, *e, *n;
  cx_bn_t bn_e;

  CX_CHECK(sys_cx_bn_alloc_init(&bn_e, cx_get_bn_size(), bytes_e, len_e));
  CX_CHECK(cx_bn_rabm_to_mpi(bn_r, &r, bn_a, &a, bn_e, &e, bn_n, &n));

  CX_CHECK(cy_mpi_mod_pow(r, a, e, n));
end:
  return error;
}

/* an emulation of montgomery conversion, not present in speculos, compute a*R
 * mod p*/
/* conversion to montgomery is equivalent to modular multiplication by 2^128*/
static cy_error_t cy_mont_to_montgomery(cx_bn_t x, const cx_bn_t z,
                                 const cy_bn_mont_ctx_t *ctx)
{
  cx_err_t error = CY_KO;

  CY_CHECK(sys_cx_bn_mod_mul(x, ctx->h, z, ctx->n));
  goto end;

end:
  return error;
}

/* an horrible emulation of cx_mont_mul, not present in speculos, compute aR*bR=abR mod p*/
/* todo: integrate some decent Montgomery implementation */
static cy_error_t cy_mont_mul(cx_bn_t r, const cx_bn_t a, const cx_bn_t b,
                       const cy_bn_mont_ctx_t *ctx)
{
  cy_error_t error = CY_KO;
  size_t field_t8;

  cx_bn_t temp;


  CY_CHECK(sys_cx_bn_nbytes(ctx->n, &field_t8));

  CY_CHECK(sys_cx_bn_alloc(&temp, field_t8));


  CY_CHECK(sys_cx_bn_mod_invert_nprime( temp, ctx->h, ctx->n));/* R^-1 (yes an inversion to emulate a mul, god forgive me)*/

  CY_CHECK(sys_cx_bn_mod_mul(temp, a, temp, ctx->n));
  CY_CHECK(sys_cx_bn_mod_mul(r, b, temp, ctx->n));

  /* destroy*/
  sys_cx_bn_destroy(&temp);

end:
  return error;
}

/* conversion from montgomery is equivalent to montgomery multiplication by 1*/
static cy_error_t cy_mont_from_montgomery(cx_bn_t z, const cx_bn_t x,
                                   const cx_bn_mont_ctx_t *ctx)
{
  cx_err_t error = CY_KO;
  cx_bn_t temp;
  size_t field_t8;

  CY_CHECK(sys_cx_bn_nbytes(ctx->h, &field_t8));
  CY_CHECK(sys_cx_bn_alloc(&temp, field_t8));

  CY_CHECK(sys_cx_bn_set_u32(temp, 1));

  CY_CHECK(cy_mont_mul(z, temp, x, ctx));

  CY_CHECK(sys_cx_bn_destroy(&temp));

  /* destroy*/
  CY_CHECK(sys_cx_bn_destroy(&temp));

end:
  return error;
}

static cy_error_t cy_mont_alloc(cx_bn_mont_ctx_t *ctx, size_t t8_length)
{
  cx_err_t error;

  CY_CHECK(sys_cx_bn_alloc(&ctx->n, t8_length));
  CY_CHECK(sys_cx_bn_alloc(&ctx->h, t8_length));
  CY_CHECK(sys_cx_bn_alloc(&ctx->h2, t8_length));

  CY_CHECK(sys_cx_bn_alloc(&ctx->mpinv, t8_length));

  /*printf("\n mont alloc:%x %x %x with size %d", ctx->n, ctx->h, ctx->mpinv,
         (int)t8_length);*/

/* V. Return*/
end:
  return error;
}


/* Mysteriously missing from bolos API*/
static cy_error_t cy_mont_destroy(cy_bn_mont_ctx_t *ctx)
{
  cx_err_t error;

  CY_CHECK(sys_cx_bn_destroy(&ctx->n));
  CY_CHECK(sys_cx_bn_destroy(&ctx->h));
  CY_CHECK(sys_cx_bn_destroy(&ctx->mpinv));
  CY_CHECK(sys_cx_bn_destroy(&ctx->h2));

  /*printf("\n mont alloc:%x %x %x with size %d", ctx->n, ctx->h, ctx->mpinv,
         (int)t8_length);*/

/* V. Return*/
end:
  return error;
}

/* computation of Montgomery constants, it is a temporary chicken or egg
 * implementation as bn_mod_pow should use montgomery rep.*/
static cy_error_t cy_mont_init(cy_bn_mont_ctx_t *ctx, const cx_bn_t n)
{
  /* I.Declarations*/

  cx_err_t error;
  size_t sizen;
  uint8_t tu8_basis[] = _2POWB;
  uint8_t tu8_basism1[] = _2POWBm1;
  cx_bn_t basis, temp;

  CY_CHECK(sys_cx_bn_nbytes(n, &sizen));
  /* II. Allocations*/
  CY_CHECK(sys_cx_bn_alloc_init(&basis, sizen, tu8_basis,
                                (size_t)sizeof(tu8_basis)));
  CY_CHECK(sys_cx_bn_alloc(&temp, sizen));

  /* III. Computations*/
  /* copy modulus*/
  CY_CHECK(sys_cx_bn_copy((ctx->n), n));
  /* -p^-1 mod 2^sizeword*/
  CY_CHECK(sys_cx_bn_reduce(temp, n, basis));

  //	 printf("\n mont alloc pour pow:%x %x %x %x  %x %x",ctx->n, ctx->h,
  //ctx->mpinv, temp, basis,n); 	 printf("sizeof n bm bm1 = %d %d %d", (int)
  //sizen, (int) sizeof(tu8_basis), (int) sizeof(tu8_basism1));

  CY_CHECK(sys_cy_bn_mod_pow(ctx->h, temp, tu8_basism1, sizeof(tu8_basism1),
                             basis)); /*1/P mod 2^n*/

  CY_CHECK(cy_mod_neg(ctx->mpinv, ctx->h, basis)); /*-1/P*/

  /* 2^bitsizeof(n) mod n */
  CX_CHECK(sys_cx_bn_xor(basis, basis, basis)); /* zero*/
  CX_CHECK(sys_cx_bn_set_bit(
      basis,
      (sizen << 3) - 1)); /*2^(sizeofp)-1 to fit in memory before reduction*/
  CX_CHECK(sys_cx_bn_mod_add(ctx->h, basis, basis, n)); /* 2^(bitsize(p))*/
  CX_CHECK(sys_cx_bn_mod_mul(ctx->h2, ctx->h, ctx->h, n)); /* 2^(bitsize(p))^2*/


  /* IV. Free*/
  sys_cx_bn_destroy(&temp);
  sys_cx_bn_destroy(&basis);

/* V. Return*/
end:
  return error;
}

#ifdef init2
static cy_error_t cy_mont_init2(cy_bn_mont_ctx_t *ctx, const cx_bn_t n,
                         const cx_bn_t h)
{
  /* I.Declarations*/
  cx_err_t error;
  /* III. Computations*/
  CY_CHECK(sys_cx_bn_copy((ctx->n), n));
  CY_CHECK(sys_cx_bn_copy((ctx->h), h));
  CY_CHECK(sys_cx_bn_copy((ctx->h2), h));

  //   CY_CHECK(cy_mod_neg(ctx->mpinv, ctx->h, basis)); /*-1/P*/
  error=CY_KO;
/* V. Return*/
end:
  return error;
}
#endif

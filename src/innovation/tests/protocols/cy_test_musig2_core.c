/* spliting tests for clarity */
#include "cy_test_musig2_unitary.c"

/* simulating the keygen of all users */
 cy_error_t simu_Keygen_all(cy_musig2_ctx_t *musig_ctx,
		 	 	 	 	 	 uint8_t  svg_xpriv[][_MAX_CYLIB_EC_T8],
							 uint8_t svg_ai[_NB_USER_EXAMPLE][_MAX_CYLIB_EC_T8],
						  	 uint8_t  svg_Kpub[][_MAX_CYLIB_EC_T8],
							 cy_ecpoint_t *KeyAgg)
{
	size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
	cy_error_t error;
	cy_ecpoint_t Kpub[_NB_USER_EXAMPLE];
	cy_fp_t xpriv[_NB_USER_EXAMPLE];
	size_t i;
	cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
	//uint8_t buffer[_MAX_CYLIB_EC_T8];
	printf("\n   *Keygen Phase:");


	for(i=0;i<musig_ctx->n_users;i++)
	{
				/* I. Allocations*/
				CY_CHECK(cy_ec_alloc(ec_ctx, &Kpub[i]));
				CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_fp, &xpriv[i]));

				/* II. Key Generation :each user generates its keys*/
				CY_CHECK(cy_musig_KeyGen(musig_ctx, &xpriv[i], &Kpub[i]));
				CY_CHECK(cy_ec_export( &Kpub[i],svg_Kpub[i], t8_fp));
				CY_CHECK(cy_fp_export( &xpriv[i],svg_xpriv[i], t8_fp));

	}

	CY_CHECK(cy_musig_KeyAgg(musig_ctx, Kpub, KeyAgg, svg_ai));
	/* IV. Free memory*/
	for(i=0;i<musig_ctx->n_users;i++)
	{
		CY_CHECK(cy_ec_free(&Kpub[i]));
		CY_CHECK(cy_fp_free(&xpriv[i]));
	}
	end:
		return error;
}




/* testing validity of import/export*/
cy_error_t test_keygen_all( cy_musig2_ctx_t *musig_ctx , uint8_t svg_Kpub[][_MAX_CYLIB_EC_T8], cy_ecpoint_t *KeyAgg)
 {
	cy_ecpoint_t Kpub[_NB_USER_EXAMPLE];
	cy_error_t error;
	cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
	size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
	printf("\n    subtest keygen_all:");
	size_t i;
	uint8_t  svg_ai[_NB_USERS_TEST][_MAX_CYLIB_EC_T8];


	cy_ecpoint_t KeyAgg_computed;
	int flag=0;

	CY_CHECK(cy_ec_alloc(ec_ctx, &KeyAgg_computed));

	for(i=0;i<musig_ctx->n_users;i++)
	{
				/* I. Allocations*/
				CY_CHECK(cy_ec_alloc(ec_ctx, &Kpub[i]));

				/* II. Key Restauration :each user import its keys*/
				CY_CHECK(cy_ec_import( svg_Kpub[i], t8_fp, &Kpub[i]));
	}

	CY_CHECK(cy_musig_KeyAgg(musig_ctx, Kpub, &KeyAgg_computed, svg_ai));

	CY_CHECK(cy_ec_iseq(&KeyAgg_computed, KeyAgg, &flag));

	if(flag!=CY_TRUE)
	{
		cy_io_ec_printMSB(KeyAgg, "\n expected");
		cy_io_ec_printMSB(&KeyAgg_computed, "\n computed");
		error=CY_FALSE;
		goto end;
	}

	CY_CHECK(cy_ec_free(&KeyAgg_computed));

	for(i=0;i<musig_ctx->n_users;i++)
		CY_CHECK(cy_ec_free( &Kpub[i]));


	end:
			if(error==CY_OK)  printf(" OK");
			else printf(" KO!!!");
				return error;
 }



/* simulating the keygen of all users */
cy_error_t simu_Sign1_all(cy_musig2_ctx_t *musig_ctx,
						  uint8_t svg_rij[][_MU_][_MAX_CYLIB_EC_T8],
						  uint8_t svg_Rij[][_MU_][_MAX_CYLIB_EC_T8],
						  cy_ecpoint_t R[_MU_]
 							 )
{
	cy_error_t error;
	cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
	size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
	size_t i,j;

	cy_fp_t rij[_NB_USER_EXAMPLE][_MU_];
	cy_ecpoint_t Rij[_NB_USER_EXAMPLE][_MU_];

	printf("\n   *Sign Round 1");


	for(i=0;i<musig_ctx->n_users;i++)
	{
		for(j=0;j<_MU_;j++)
		{
			CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_fp, &(rij[i][j])));
			//printf("\n i=%d, j=%d",i,j);
			CY_CHECK(cy_ec_alloc(ec_ctx, &(Rij[i][j])));
			//printf("\n allocated cell: %x %x %x", Rij[i][j].ec->x, Rij[i][j].ec->y, Rij[i][j].ec->z);

		}
		CY_CHECK(cy_musig_Sign1(musig_ctx, rij[i], Rij[i]));

		for(j=0;j<_MU_;j++)
		{
			//printf("\n-------------------- i=%d, j=%d", (int) i, (int) j);
			CY_CHECK(cy_ec_export( &Rij[i][j],svg_Rij[i][j], t8_fp));
			//cy_io_ec_printMSB(&Rij[i][j], "\n exporting:");

			CY_CHECK(cy_fp_export(&rij[i][j],  svg_rij[i][j], t8_fp));

			//cy_io_fp_printMSB(&rij[i][j], "rij");
			//cy_io_ec_printMSB(&Rij[i][j], "Rij");

			CY_CHECK(cy_fp_free(&(rij[i][j])));
			CY_CHECK(cy_ec_free(&(Rij[i][j])));
		}

	}
	CY_CHECK(cy_musig_Sign1_tu8_Agg(musig_ctx, svg_Rij, R));

	end:
			return error;
}
/* todo: test reimport and compliance with derivated Rij's*/
cy_error_t test_Sig1Agg(cy_musig2_ctx_t *musig_ctx,  uint8_t svg_rij[][_MU_][_MAX_CYLIB_EC_T8],
		  uint8_t svg_Rij[][_MU_][_MAX_CYLIB_EC_T8])
{
 cy_error_t error;
 cy_fp_t rij[_NB_USER_EXAMPLE][_MU_];
 cy_ecpoint_t Rij[_NB_USER_EXAMPLE][_MU_];
 cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
 int flag=0;

 cy_ecpoint_t ec_temp;
 size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
 size_t i,j;
 printf("\n    subtest SigAgg1: ");

 CY_CHECK(cy_ec_alloc(ec_ctx, &ec_temp));

 for(i=0;i<musig_ctx->n_users;i++)
 	{
 		for(j=0;j<_MU_;j++)
 		{
 			//printf("\n i=%d, j=%d", (int) i, (int) j);

 			CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_fp, &(rij[i][j])));
 			CY_CHECK(cy_ec_alloc(ec_ctx, &(Rij[i][j])));

 			CY_CHECK(cy_ec_import(  svg_Rij[i][j], t8_fp, &Rij[i][j]));
 			CY_CHECK(cy_fp_import(  svg_rij[i][j], t8_fp, &rij[i][j]));

 			//cy_io_fp_printMSB(&rij[i][j], "rij");
 			//cy_io_ec_printMSB(&Rij[i][j], "Rij");

 			CY_CHECK(cy_musig_KeyGenDeriv(musig_ctx, &rij[i][j], &rij[i][j], &ec_temp));

 			CY_CHECK(cy_ec_iseq(&ec_temp, &Rij[i][j], &flag));

 			CY_CHECK(cy_fp_free(&(rij[i][j])));
 			CY_CHECK(cy_ec_free(&(Rij[i][j])));

 			if(flag!=CY_TRUE)
 			{
 				error=CY_FALSE;
 				cy_io_ec_printMSB(&Rij[i][j], "Expected");
 				cy_io_ec_printMSB(&ec_temp, "computed");


 				goto end;
 			}
 		}

 	}
 printf(" OK");
 CY_CHECK(cy_ec_free(&ec_temp));

 end:
 	return error;
}


/* simulating the keygen of all users */
cy_error_t simu_Sign2_all(cy_musig2_ctx_t *musig_ctx,
						  uint8_t svg_xpriv[][_MAX_CYLIB_EC_T8],			/* result of private keygens */
						  uint8_t svg_ai[_NB_USERS_TEST][_MAX_CYLIB_EC_T8],	/* result of Haggs*/
						  uint8_t svg_Kpub[][_MAX_CYLIB_EC_T8],				/* result of public keygens 	*/
						  uint8_t svg_rij[][_MU_][_MAX_CYLIB_EC_T8],		/* first round nonces */
						  uint8_t svg_Ri[][_MU_][_MAX_CYLIB_EC_T8],			/* result of sig1Agg*/
						  cy_ecpoint_t *KeyAgg,
						  uint8_t *R,
						  uint8_t *tu8_s,
						  uint8_t *tu8_c
 							 )
{
	cy_error_t error;
	 cy_fp_t ri[_MU_];
	 //uint8_t buffer[MAX_MUSIG_EC_T8];
	 cy_ecpoint_t Ri[_MU_];
	 cy_ecpoint_t R2;
	 cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
	 int flag=CY_TRUE;
	 cy_ecpoint_t ec_temp;
	 size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;
	 size_t i,j;
	 cy_fp_t privatekey_xi;
	 cy_fp_t fp_ai;

	 printf("\n   *Sign Round2: ");

	 CY_CHECK(cy_ec_alloc(ec_ctx, &ec_temp));
	 CY_CHECK(cy_ec_alloc(ec_ctx, &R2));
	 CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_fp, &privatekey_xi));
	 CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_fp, &fp_ai));

	 for(j=0;j<_MU_;j++)
		 {
		 	 CY_CHECK(cy_ec_alloc(ec_ctx, &(Ri[j])));

		 	 CY_CHECK(cy_fp_alloc(ec_ctx->ctx_fp_p, t8_fp, &(ri[j])));
		 	 CY_CHECK(cy_ec_import( svg_Ri[j], t8_fp, &Ri[j]));

		 }

	 for(i=0;i<musig_ctx->n_users;i++)
	 	{
		    printf("\n USer %d", (int) i);

			CY_CHECK(cy_fp_import(  svg_xpriv[i], t8_fp, &privatekey_xi));
			CY_CHECK(cy_fp_import(  svg_ai[i], t8_fp, &fp_ai));
			//cy_io_fp_printMSB(&fp_ai, "ai=");

	 		for(j=0;j<_MU_;j++)
	 		{
	 			CY_CHECK(cy_fp_import(  svg_rij[i][j], t8_fp, &ri[j]));
	 		}

	 		CY_CHECK(cy_musig_Sign_Round2_all(musig_ctx,
	 			 					svg_ai[i],
	 								&privatekey_xi,
	 								Ri,KeyAgg,
	 								svg_rij,
	 			 					( uint8_t *) message_0, sizeof(message_0),
	 			 					&R2,
	 			 					tu8_s,
	 			 					tu8_c
	 			 					));
	 	}
	 CY_CHECK(cy_ec_free(&ec_temp));
		 CY_CHECK(cy_ec_free(&R2));

		 CY_CHECK(cy_fp_free( &privatekey_xi));
		 CY_CHECK(cy_fp_free(&fp_ai));

	 printf(" OK");
	 end:


	 for(j=0;j<_MU_;j++)
	 {
		 CY_CHECK(cy_fp_free(&(ri[j])));
		 CY_CHECK(cy_ec_free(&(Ri[j])));
	 }
	 			return error;
}

/* full example, assuming an initialized musig_ctx */
cy_error_t test_musig_full_example(cy_musig2_ctx_t *musig_ctx)
{
	cy_ec_ctx_t *ec_ctx=musig_ctx->ctx_ec;
	size_t t8_fp=musig_ctx->ctx_ec->ctx_fp_p->t8_modular;

	cy_error_t error;

	cy_ecpoint_t ec_R, Kpub[_NB_USER_EXAMPLE];
	cy_fp_t xpriv[_NB_USER_EXAMPLE];
	cy_ecpoint_t ec_Rpart;

	uint8_t R_part[_MAX_CYLIB_EC_T8];
	uint8_t s_part[_MAX_FP_T8];
	uint8_t c_part[_MAX_FP_T8];

	/* those must be saved if using a single component and swap to memory */
	cy_ecpoint_t KeyAgg;

	/* SERIALIZATION OF ALL RESULTS: to avoid memory overflow of cryptographic accelerator */
	uint8_t svg_rij[_NB_USER_EXAMPLE][_MU_][_MAX_CYLIB_EC_T8]; /* all nonces of all users */
	uint8_t svg_Rij[_NB_USER_EXAMPLE][_MU_][_MAX_CYLIB_EC_T8]; /* all ephemeral of all users */
	uint8_t svg_Kpub[_NB_USER_EXAMPLE][_MAX_CYLIB_EC_T8];	   /* all public keys of all users */
	uint8_t svg_xpriv[_NB_USER_EXAMPLE][_MAX_CYLIB_EC_T8];	   /* all private keys of all users */
	uint8_t svg_ai[_NB_USER_EXAMPLE][_MAX_CYLIB_EC_T8];	   /* all private keys of all users */

	CY_CHECK(cy_ec_alloc(ec_ctx, &KeyAgg));
	CY_CHECK(cy_ec_alloc(ec_ctx, &ec_R));
	CY_CHECK(cy_ec_alloc(ec_ctx, &ec_Rpart));

	printf("\n\n test Musig2 Full example, current bn:%x", KeyAgg.ec->x);

	CY_CHECK(simu_Keygen_all(musig_ctx, svg_xpriv, svg_ai, svg_Kpub, &KeyAgg));
	CY_CHECK(test_keygen_all(musig_ctx,  svg_Kpub, &KeyAgg));
	CY_CHECK(simu_Sign1_all( musig_ctx, svg_rij,svg_Rij, &ec_R));
	CY_CHECK(test_Sig1Agg(musig_ctx,  svg_rij,svg_Rij));

#ifdef sig2
	CY_CHECK(simu_Sign2_all(musig_ctx,
							svg_xpriv, svg_ai,svg_Kpub, svg_rij, svg_Rij, /*serialization of previous results */
							&KeyAgg,
							R_part, s_part,c_part ));
#endif

	CY_CHECK(cy_ec_free( &ec_R));
	CY_CHECK(cy_ec_free( &ec_Rpart));
	CY_CHECK(cy_ec_free( &KeyAgg));

	end:
		if(error==CY_OK)  printf("\n   *End example OK");
		else printf("\n  *End example KO!!!");
				return error;
}

int test_musig_unit(uint8_t *Ramp, size_t Ramp_t8)
{
	cy_error_t error=CY_OK;
	cy_ec_ctx_t ec_ctx;

	cy_musig2_ctx_t musig_ctx;
	printf("\n\n /************************ Test Musig2 Protocol core:");


	CY_CHECK(test_musig_SetUp(&unit_pedersen, &bolos_gda_component, CY_CURVE_Stark256, &ec_ctx, Ramp, Ramp_t8, &musig_ctx));

	//CY_CHECK(test_verif_core(&musig_ctx)); 	/* unitary vector test of schnorr verification, external hashing*/
	//CY_CHECK(test_keygen(&musig_ctx));	 	/* unitary vector test of key generation */
	//CY_CHECK(test_HashnKeyAgg(&musig_ctx)); /* unitary vector test of key aggregation */
	//CY_CHECK(test_HashSig(&musig_ctx));		/* unitary test for Hsig, also validated with full verif_all */

	CY_CHECK(test_verif_all(&musig_ctx));	/* unitary vector test of schnorr verification, external hashing*/

	CY_CHECK(test_musig_full_example(&musig_ctx));
	CY_CHECK(cy_musig_uninit( &musig_ctx));


	end:
	if(error==CY_OK)  printf("\n All Musig2_core tests OK");
	else printf("\n  Musig2 KO with error=%x",(unsigned int) error);
			return error;
}





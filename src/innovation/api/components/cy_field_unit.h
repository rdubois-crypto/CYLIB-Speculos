#include <stddef.h>
#include <stdint.h>
#include "cy_errors.h"
#include "cy_memory_unit.h"


struct cy_felt_s{
	void *inner;
	unsigned int magic;
};

typedef struct cy_felt_s cy_felt_t;

typedef struct field_ctx_s cy_field_ctx_t;

typedef cy_error_t (*Fct_Field_Init_t)(cy_field_ctx_t *self, uint8_t *characteristic,
                                     size_t t8_characteristic);


typedef cy_error_t (*Fct_Field_Alloc_t)( cy_felt_t *out, cy_field_ctx_t *self);




typedef cy_error_t (*Fct_Field_Fe_t)(cy_felt_t *out);

typedef cy_error_t (*Fct_Field_Fe2_t)(cy_felt_t *out, cy_felt_t *a);

typedef cy_error_t (*Fct_Field_Fe3t)(cy_felt_t *out, cy_felt_t *b,cy_felt_t *a,);

typedef cy_error_t (*Fct_Field_FeU8_t)(cy_felt_t *out, cy_felt_t *b,uint8_t *a, size_t t8_a);

typedef cy_error_t (*Fct_Field_UnInit_t)(cy_field_ctx_t *self);

struct field_ctx_s {
	Fct_Field_Init_t Init;
	Fct_Field_Alloc_t Alloc;
	Fct_Field_Fe2_t Add;
	Fct_Field_Fe2_t Mul;
	Fct_Field_Fe2_t Div;
	Fct_Field_Fe_t Sqrt;

	Fct_Field_Fe_t Free;
	Fct_Field_UnInit_t UnInit;

};


#ifndef ALCHEMY_FIELD_FIELD_EFFECT_H
#define ALCHEMY_FIELD_FIELD_EFFECT_H

/* The tints a field Psynergy washes the screen in, and the motion updates of
   its effect actors. */

#include "types.h"

union FIELD_OBJECT;

/* The eight tints a field Psynergy washes the screen in, by their index in
   FieldPsynergyCastColors and FLAG_PSYNERGY_TINT. */
typedef enum PSYNERGY_TINT {
	PSYNERGY_TINT_BLUE = 0,
	PSYNERGY_TINT_GREEN = 1,
	PSYNERGY_TINT_RED = 2,
	PSYNERGY_TINT_CYAN = 3,
	PSYNERGY_TINT_MAGENTA = 4,
	PSYNERGY_TINT_YELLOW = 5,
	PSYNERGY_TINT_DARK_GREY = 6,
	PSYNERGY_TINT_GREY = 7,
	PSYNERGY_TINT_COUNT = 8
} PSYNERGY_TINT;

/* Each tint's colour (field_effect_data.c). */
extern const Uint32 FieldPsynergyCastColors[PSYNERGY_TINT_COUNT];

void fdEffectAdvanceMotion(union FIELD_OBJECT *object);

void fdEffectAdvanceMotionAtLeaderPriority(union FIELD_OBJECT *object);

#endif

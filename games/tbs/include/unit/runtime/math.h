#ifndef ALCHEMY_UNIT_RUNTIME_MATH_H
#define ALCHEMY_UNIT_RUNTIME_MATH_H

#include "types.h"
#include "callback_scheduler.h"
#include "irq.h"

u32 Random16(void);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
s32 ArcTan2(s32 x, s32 y);
s32 Math_IntegerSqrt(s32 value);
s32 FixedSqrt(s32 value);

#endif

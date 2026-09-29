#ifndef ALCHEMY_RAMAKAN_H
#define ALCHEMY_RAMAKAN_H

/* The Lamakan desert (resource_3a5): what its files share. */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "FIXED_POINT_POSITION.H"

#define NULL ((void *)0)

extern u8 *gWork;

void BattleFx_RunPageEffectForSlot();
void RamakanSabaku_UpdateTravelDust(void);
void SceneState_SetHalfwordB030(u16 value);
void Vector_AddPolarOffset();
/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}


#endif

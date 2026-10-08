#ifndef ALCHEMY_RAMAKAN_H
#define ALCHEMY_RAMAKAN_H

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "FIXED_POINT_POSITION.H"

#define NULL ((void *)0)


void BattleFx_RunPageEffectForSlot();
void RamakanSabaku_UpdateTravelDust(void);
void RamakanSabaku_SetSandPhase(u16 value);
void Vector_AddPolarOffset();
#endif

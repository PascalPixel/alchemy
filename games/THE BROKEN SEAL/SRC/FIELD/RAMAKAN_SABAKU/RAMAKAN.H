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
#endif

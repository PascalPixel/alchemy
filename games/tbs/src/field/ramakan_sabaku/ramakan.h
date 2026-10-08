#ifndef ALCHEMY_RAMAKAN_H
#define ALCHEMY_RAMAKAN_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

#include "fixed_point_position.h"

#define NULL ((void *)0)


void BattleFx_RunPageEffectForSlot();
void RamakanSabaku_UpdateTravelDust(void);
void RamakanSabaku_SetSandPhase(u16 value);
void Vector_AddPolarOffset();
#endif

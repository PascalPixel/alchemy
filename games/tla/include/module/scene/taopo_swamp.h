#ifndef ALCHEMY_MODULE_SCENE_TAOPO_SWAMP_H
#define ALCHEMY_MODULE_SCENE_TAOPO_SWAMP_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

s32 Vent_IsActive(s32 hole);

void TaopoSwamp_FocusMapMiddle();
void TaopoSwamp_JumpForward(void);

void TaopoSwamp_GrowVine(void);

void TaopoSwamp_DigHoleUnderfoot(void);

void TaopoSwamp_StartMadPlantBattle(void);

void TaopoSwamp_TremorPillar(void);

void TaopoSwamp_SinkInMud(void);

void TaopoSwamp_RiseOnBubble(void);

void TaopoSwamp_StepOnFirmGround(void);

#endif

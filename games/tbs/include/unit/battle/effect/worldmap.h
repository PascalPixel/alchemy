#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_WORLDMAP_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_WORLDMAP_H

#include "types.h"
#include "effect_slot.h"

void BattleFx_SetupResourcesAndWindow(void);
void BattleFx_CleanupResourcesAndWindow(void);
void Map_UpdateWorldMapMarkers(void);
void Map_ShowWorldMap(s32 picture);
void BattleFx_UpdateObjectVisibilityBounds(void);
void BattleFx_RunVisibilityTransition(void);

#endif

#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT3_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_OBJECT3_H

#include "types.h"
#include "effect_slot.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object2.h"

void BattleFx_ResetCounters(void);
s32 BattleFx_SumCounters(void);
s32 GetFocusedObjectCollision(void);
void UpdateMapRegionAtPosition(s32 position_x, s32 position_y, s32 position_z);
void Field_ProcessStep(s32 layer, s32 x, s32 y, s32 z);
void BattleParty_ApplyDrain(s32 amount, s32 unused);
void BattleParty_ApplyHealthDelta(s32 amount, s32 scaled);
s32 BattleParty_ApplyStatusDamage(void);
void Battle_SetObjectFlag5bWhenMode3(void);
void Battle_ClearObjectFlag5bWhenMode3(void);
s32 Field_RunScene(s32 entrance);
s32 Object_GetTriggerTileAheadOfCurrent(void);
void BattleMap_ApplyEntranceView(void);
void Debug_RunPaletteEditor(void);

#endif

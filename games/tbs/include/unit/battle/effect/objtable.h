#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_OBJTABLE_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_OBJTABLE_H

#include "types.h"
#include "effect_slot.h"
#include "unit/battle/effect/object.h"

struct ScenePlacement;
struct SceneRegionEntry;

s32 Party_RemapCharacterIdByFlags(s32 arg0);
s32 Party_RemapCharacterIdIfEnabled(s32 arg0, s32 arg1);
void Event_SpawnObjectTable(struct ScenePlacement *entry, s32 slot);
void ObjectTable_DestroyAtIndex(s32 index);
void ObjectTable_ResetForObject(struct ScenePlacement *table);
s32 ObjectTable_FindLastActiveId(void);
void *ObjectTable_GetSlotAddress(u32 index);
void Scene_AssignViewFlags(struct SceneRegionEntry *entry);
void BattleEffect_ClearOutOfBoundsObjects(void);
void BattleEffect_ClearAllObjects(void);
void ObjectTable_ClearBattleSlots(void);
void *ObjectTable_Get(u32 index);
void ObjectTable_Snapshot(void);
void ObjectTable_Restore(void);

#endif

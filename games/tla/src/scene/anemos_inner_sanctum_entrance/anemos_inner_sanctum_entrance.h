#ifndef TLA_ANEMOS_INNER_SANCTUM_ENTRANCE_H
#define TLA_ANEMOS_INNER_SANCTUM_ENTRANCE_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "scene_ids.h"

struct AnemosInnerSanctumEntranceParty {
  u8 unk000[0x24b];
  u8 battleRule;
  u8 unk24c[8];
  u16 battleGroup;
};

void Func_02000494(void);
void Func_02000be0(u16 value);
void Actor_MoveToObject(s32 id, s32 a);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
s32 Field_FindNearbyTalkTarget(s32 id);
void Field_TryJumpForward(void);
s32 Func_02000d4c(void);
void Func_02000d54(void);

#endif

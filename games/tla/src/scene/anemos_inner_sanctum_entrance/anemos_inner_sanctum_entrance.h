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

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
void Event_Wait(s32 frames);
struct Actor *Actor_GetById(u32 objectId);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);
void Actor_StartRepeatedMotion(u32 objectId, s32 variant);
void Actor_SetMovementTypeById(s32 id, const void *script);
void Actor_SetMovementTypeAndWaitById(s32 id, const void *script);
void Func_02000494(void);
void Func_02000be0(u16 value);
void Func_0803ce64(s32 message, s32 a, s32 b);
void Event_Start(void);
void Event_End(void);
void Actor_MoveToObject(s32 id, s32 a);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
s32 Func_02000d0c(s32 id);
void Func_02000d14(void);
void BattleEffect_ApplyColorToTargetBuffer(s32 a, s32 b);
void BattleEffect_ApplyColorToSourceBuffer(s32 a, s32 b);
void BattleEffect_StartBufferInterpolation(s32 a);
void Event_ResolvePendingActions(s32 a);
s32 Func_02000d4c(void);
void Func_02000d54(void);
void Sound_Play(s32 cue);

#endif

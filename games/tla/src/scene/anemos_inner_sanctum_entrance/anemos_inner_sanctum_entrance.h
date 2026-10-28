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
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_StartRepeatedMotion(s32 id, s32 variant);
void Actor_SetMovementTypeById(s32 id, const void *script);
void Actor_SetMovementTypeAndWaitById(s32 id, const void *script);
void Func_02000494(void);
void Func_02000be0(u16 value);
void Func_02000ca4(s32 message, s32 a, s32 b);
void Func_02000cb4(void);
void Func_02000cbc(void);
void Func_02000cdc(s32 id, s32 a);
void Func_02000d04(s32 a, s32 b);
s32 Func_02000d0c(s32 id);
void Func_02000d14(void);
void Func_02000d1c(s32 a, s32 b);
void Func_02000d24(s32 a, s32 b);
void Func_02000d2c(s32 a);
void Func_02000d44(s32 a);
s32 Func_02000d4c(void);
void Func_02000d54(void);
void Func_02000d64(s32 cue);

#endif

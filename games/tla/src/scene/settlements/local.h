#ifndef SETTLEMENTS_LOCAL_H
#define SETTLEMENTS_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "scene_ids.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Actor_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void PartyInventory_GiveItem(s32 item, s32 mode);
void Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));
s32 Math_Cosine(s32 angle);

void FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);

extern u8 MsgSettlements01;

void Func_02002018(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002030(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020020e0(s32 message);
void Func_020020f0(s32 id, s32 a);
void Func_02002110(s32 a);
s32 Func_020021a8(void);
void Func_020021b0(s32 a);
#include "frame.h"

extern const u8 gData_0200226c[];
extern u16 gData_02002ce8;

void Func_02000758(s32 x, s32 y, s32 z);
void Func_02000d14(void);
void Func_02000ed0(void);
void Func_02000f2c(void);
void Func_02000fd0(void);
void Func_0200122c(void);
void Func_0200138c(void);
void Func_02001770(void);
void Func_02002118(void);
void Func_02002148(void);
void Func_02002168(s32 a);
void Func_02002178(s32 a, s32 b, s32 c);
void Func_02002190(const void *data);
void Func_02002198(void);
void Func_0200052c(void);
void Func_02000714(struct Actor *object);
struct Actor *Func_02001ff8(s32 kind, s32 x, s32 y, s32 z);
void Func_02001fe0(struct Actor *object, s32 a);
void Func_02002000(struct Actor *object);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
extern s32 gData_02002ce0;
extern u16 gData_02002ce4;
extern u16 gData_02002ce6;
void Func_02000e30(void);
void Func_02000e90(void);
void Func_02001fa8(s32 a, s32 b, void (*callback)(void));
void Func_02002068(void);
void Func_02002070(void);
void Func_020020c0(s32 id, s32 a, s32 b);
void Func_02002130(s32 a, s32 b);
void Func_02002150(s32 a);
void Func_02002158(s32 a, s32 b, s32 c);
void Func_020021a0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

extern const u8 gData_02002380[];
extern const u8 gData_020023a0[];

static __inline__ void CommitPos(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_CommitPositionAndActivate(id, x, z);
}

static __inline__ void Window(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020021a0(a, b, c, d, e, f, g);
}

static __inline__ void CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002018(a, b, c, d, e, f);
}

static __inline__ void CopyCells2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002030(a, b, c, d, e, f);
}

static __inline__ void SetPosCommit(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndCommit(id, x, z);
}

static __inline__ void Mark(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02002178(a, b, c);
}

static __inline__ s32 Flag_Test(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static __inline__ void Flag_Set(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static __inline__ void Flag_Clear(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_ClearBit(flag);
}

#endif

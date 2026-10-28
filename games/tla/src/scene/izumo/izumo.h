#ifndef TLA_IZUMO_H
#define TLA_IZUMO_H

#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "adventure.h"
#include "call.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern const u8 gData_02000e10[];
extern const u8 gData_02001644[];
extern const u8 gData_02001494[];
extern const u8 gData_02001b00[];
extern const u8 gData_020018b4[];
extern u8 MsgCanYouImagine[];
extern u8 MsgSeeThisRing[];
extern u8 MsgButItS[];
extern u8 MsgSeeThisRingOf[];
extern u8 MsgLadyUzumeAnd[];
extern u8 MsgWouldYouBe[];
extern u8 MsgTheLegendSays[];
extern u8 MsgWait2[];
extern u8 MsgThisTokenIsn[];
extern u8 MsgTheMisalignedStone[];

struct FieldActor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_Wait(s32 frames);
void FieldMessage_ShowAtActor(s32 id, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Camera_FollowActor(s32 id, s32 mode);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(s32 id);
void Actor_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void Actor_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Actor_SetAngleToward(s32 id, s32 target, s32 frames);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_StartRepeatedMotion(s32 id, s32 variant);
void Actor_SetSingleChildFlags(struct FieldActor *object, s32 flags);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000ce0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000ce8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000d00(void);
void Func_02000d08(void);
void Func_02000d40(s32 id, s32 x, s32 z);
void Func_02000d60(const u8 *message);
void Func_02000d70(s32 id, s32 flags);
void Func_02000d88(s32 id, s32 a, s32 b);
void Func_02000d90(s32 id, s32 a);
void Func_02000da0(void);
void Func_02000da8(s32 id, s32 value);
void Func_02000db0(s32 value);
void Func_02000dd0(s32 value);
void Func_02000de0(const u8 *data);
void Func_02000de8(const u8 *data);
void Func_02000df0(void);
void Func_02000df8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
s32 Func_02000e00(void);
void Func_02000e08(s32 cue);
s32 Task_Add(void (*func)(void), s32 order);
void Func_0200060c(void);
void Func_020006a4(void);
void Func_02000814(void);

#endif

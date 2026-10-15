#ifndef TLA_OVERLAY_67E_OV67E_H
#define TLA_OVERLAY_67E_OV67E_H

#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "party_state.h"
#include "call.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern const u8 Data_02000e10[];
extern const u8 Data_02001644[];
extern const u8 Data_02001494[];
extern const u8 Data_02001b00[];
extern const u8 Data_020018b4[];
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

struct FieldActor *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Battle_WaitMode0(s32 frames);
void UiText_OpenMessageAtObject(s32 id, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 id, s32 flags);
void Object_AttachWorkTargetToObject(s32 id, s32 mode);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_CommitPositionAndActivate(s32 id, s32 x, s32 z);
void ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 frames);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetVariantCallback(s32 id, s32 variant);
void ObjectDispatch_SetSingleChildFlags(struct FieldActor *object, s32 flags);
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
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Func_0200060c(void);
void Func_020006a4(void);
void Func_02000814(void);

#endif

#ifndef TLA_JUPITER_LIGHTHOUSE_ROOM_7_H
#define TLA_JUPITER_LIGHTHOUSE_ROOM_7_H

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "input.h"
#include "call.h"
#include "text/msg_ids.h"

struct JupiterLighthouseRoom7Cell {
  u8 unk00[6];
  s16 x;
  s16 z;
};

extern u32 gFrameCount;
extern s16 gData_02002c64[];

void wait(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_OffsetPositionAndResetMotion(s32 actor, s32 x, s32 z);

void Func_02000060(s32 mode);
void Func_020005e8(s32 mode);
void Func_020009f8(s32 id, s32 x, s32 z, s32 dx, s32 dz);
void Func_020024ac(void);
void Func_02002b1c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002b24(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002b2c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002ba4(s32 id, s32 x, s32 z);
void Func_02002c5c(s32 cue);

#endif

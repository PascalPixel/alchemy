#ifndef TLA_OVERLAY_674_H
#define TLA_OVERLAY_674_H

#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "map.h"
#include "input.h"
#include "call.h"
#include "text/msg_ids.h"

struct Ov674EventWork {
    u8 unknown_000[0x154];
    s16 exit;
    u8 unknown_156[4];
    s16 field_15a;
    u8 unknown_15c[0x50];
    s32 field_1ac;
};

extern u32 gFrameCount;

void WaitFrames(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

struct ObjectRuntime *Func_02003df4(s32 kind, s32 x, s32 y, s32 z);
void Func_02003de4(struct ObjectRuntime *object, s32 value);
void Func_02003dec(struct ObjectRuntime *object, const void *script);
void Func_02003e0c(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02003e24(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003e64(void);
void Func_02003e6c(void);
void Func_02003f24(s32 message);
void Func_02003f2c(s32 actor, s32 flags);
void Func_02003f3c(s32 actor, s32 angle);
void Func_02003f7c(s32 exit);
void Func_02003fa4(s32 value);
void Func_02003ffc(s32 cue);
void Func_020034ec(void);

void Func_02001cac(void);
void Func_02001cfc(void);

extern const u8 Data_020044f0[];
extern const u8 Data_02004b14[];
extern const u8 Data_02004b20[];
extern const u8 Data_02004bb0[];
extern const u8 Data_02004ca0[];
extern const u8 Data_02004d84[];
extern const u8 Data_02004e5c[];
extern u16 gSceneState[];
extern char SceneId_Ov674Room1;
extern char SceneId_Ov674Room2;
extern char SceneId_Ov674Room3;
extern char SceneId_Ov674Room4;
extern char SceneId_Ov674Room5;

void Func_02002c38(void);
void Func_02002f1c(void);
void Func_020030b4(void);
void Func_02003164(void);
void Func_0200329c(void);
void Func_02003e3c(struct ObjectRuntime *object, s32 value);

#endif

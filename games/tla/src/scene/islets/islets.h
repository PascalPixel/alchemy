#ifndef TLA_ISLETS_H
#define TLA_ISLETS_H

#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "map.h"
#include "input.h"
#include "call.h"
#include "text/msg_ids.h"

struct IsletsEventWork {
    u8 unk000[0x154];
    s16 exit;
    u8 unk156[4];
    s16 field15a;
    u8 unk15c[0x50];
    s32 field1ac;
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

extern const u8 gData_020044f0[];
extern const u8 gData_02004b14[];
extern const u8 gData_02004b20[];
extern const u8 gData_02004bb0[];
extern const u8 gData_02004ca0[];
extern const u8 gData_02004d84[];
extern const u8 gData_02004e5c[];
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

struct IsletsMapFlags {
    u8 unk00[0x17];
    u8 unused : 2;
    u8 mode : 2;
};

struct IsletsExit {
    s32 value;
    struct {
        u16 first;
        u16 second;
    } pair;
};

struct IsletsVector {
    s32 x;
    s32 y;
    s32 z;
};

struct IsletsPair {
    s32 first;
    s32 second;
};

struct IsletsTarget {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    struct IsletsPair pair;
};

extern u8 gData_02004754[];
extern u8 gData_0200476c[];
extern u8 gData_020047e4[];
extern u8 gData_0200494c[];
extern u8 gData_020049c4[];
extern u8 gData_02004a84[];
extern s32 gData_02005190;
extern s32 gData_02005194;
extern s32 gData_02005198;
extern s32 gData_0200519c;
extern const u8 gData_02004028[];
extern const struct IsletsExit gData_020046bc[];
extern const u16 gData_02004070[];

void Func_020020ec(s32 x, s32 z, s32 layers, s32 value);
void Func_020021d0(void);
void Func_02002bd8(void);
void Func_02003fe4(void);
void Func_02003edc(s32 actor, s32 x, s32 z);
void Func_02003e14(struct ObjectRuntime *object);
void Func_02003e1c(s32 value, s32 first, s32 second);
s32 Func_02003e2c(struct ObjectRuntime *object, struct IsletsVector *position);
struct ObjectRuntime *Func_02003fdc(struct IsletsVector *position, s32 mode);
s32 Func_020037d8(struct IsletsTarget *target);
void Func_02003a5c(struct IsletsTarget target);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, struct IsletsVector *position);
void Motion_CamBounds(s32 a, s32 b, s32 c, s32 d);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *script);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 x, s32 z);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);

void Func_02001f58(void);
void Func_02001278(struct ObjectRuntime *object);
void Func_02000df8(struct ObjectRuntime *object);
void Func_020034d8(void);
void Func_02003eec(s32 actor, s32 target);
void Func_02003fac(s32 actor, s32 target);
void Func_02003fb4(void);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void ObjectMotion_EnableActionAndResetMotion(s32 actor);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void ObjectMotion_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void Event_StartScreenTransition(void);

#endif

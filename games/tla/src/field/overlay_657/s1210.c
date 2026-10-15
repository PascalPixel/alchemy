#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "party_state.h"
#include "callback_scheduler.h"
#include "call.h"
#include "flag_call.h"

struct ObjectRuntime *Func_02001f0c(s32 type, s32 x, s32 y, s32 z);
void Func_02001f4c(s32 a);
u8 *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 VramBlock_LoadCached(s32 block, s32 count, u8 *source);

struct ObjectRuntime *Func_02001210(void)
{
    struct ObjectRuntime *object = Func_02001f0c(0x1e9, 0x1680000, 0x400000, 0x1a80000);
    u8 *buffer = 0;
    struct FieldSprite *sprite;

    if (object == 0)
        return 0;
    sprite = object->animation;
    sprite->full_color = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    object->motion_flags = 0;
    object->unknown_5c[0] = 1;
    sprite->next_frame = 0;
    sprite->unknown_1b[0] = 0;
    buffer = Runtime_AllocateHeapBlockFar(68, 0x608);
    Func_02001f4c(242);
    VramBlock_LoadCached(sprite->vram_block, 128, buffer + 0x400);
    Runtime_ReleaseHeapBlock(68);
    return object;
}

extern u8 MsgAtLastI[];
extern const u8 Data_02002120[];

void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void ObjectMotion_ArmCallback(s32 id, s32 a, s32 b);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_Launch(s32 id, s32 speed, s32 frames);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 frames);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void Func_02001188(void);
void Func_02001f14(struct ObjectRuntime *object);
void Func_02001f2c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001f5c(void);
void Func_02001f64(void);
void Func_02001fc4(s32 id, s32 a, s32 b);
void Func_02001fd4(s32 id, s32 target);
void Func_0200201c(const void *message);
void Func_02002034(s32 id, s32 a);
void Func_02002044(s32 id, s32 angle);
void Func_02002084(s32 a, s32 b);
void Func_0200208c(s32 frames);
void Func_020020ac(s32 a);
void Func_020020d4(s32 cue);

// Once flag 7 is set, and only until flag 0x985 is: a scene of actor 7 and the
// leader, which ends by setting flags 0x201 and 0x985.
void Func_0200128c(void)
{
    struct ObjectRuntime *handle;
    void (*callback)(void);
    struct ObjectRuntime *object;

    if (Flag_IsSet(0x985) != 0)
        return;
    if (Flag_IsSet(7) == 0)
        return;
    Func_02001f5c();
    Call1((void (*)())Func_020020ac, 0);
    Call2((void (*)())Func_02001fd4, 7, gPartyState.current_owner);
    Call1((void (*)())WaitFrames, 1);
    Call3((void (*)())ObjectMotion_ArmCallback, gPartyState.current_owner, 0, 0);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 7, 0x19999, 0xcccc);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 7, 412, 424);
    Call2((void (*)())Func_02002044, 7, 0xa000);
    Call1((void (*)())Func_0200201c, (s32)MsgAtLastI);
    Call3((void (*)())ObjectMotion_Launch, 7, 4, 10);
    Call2((void (*)())Func_02002034, 7, 0);
    Call2((void (*)())Func_02002044, 7, 0xe000);
    Call2((void (*)())Func_02002034, 7, 0);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 7, 386, 424);
    Call3((void (*)())ObjectMotion_SetAngleToward, 7, gPartyState.current_owner, 0);
    Call2((void (*)())Object_SetModeById, gPartyState.current_owner, 3);
    Call2((void (*)())Func_02002034, 7, 0);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, gPartyState.current_owner, 0xcccc, 0x6666);
    Call2((void (*)())ObjectMotion_EnableActionAndSetCallback, gPartyState.current_owner, (s32)Data_02002120);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 7, 0xcccc, 0x6666);
    Call3((void (*)())ObjectMotion_SetPositionAndReset, 7, 360, 424);
    Call2((void (*)())Func_02002044, 7, 0xc000);
    Call1((void (*)())Func_020020d4, 182);
    handle = Func_02001210();
    Call1((void (*)())Battle_WaitMode0, 20);
    Call1((void (*)())Func_020020d4, 220);
    callback = Func_02001188;
    Scheduler_AddOrUpdateCallback(callback, 0x480);
    Call2((void (*)())Func_02002084, 0x10003, 1);
    Call1((void (*)())Func_0200208c, 20);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Func_02002084, 0x10006, 1);
    Call1((void (*)())Func_0200208c, 20);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Func_02002084, 0x7fff, 0);
    Call1((void (*)())Func_0200208c, 20);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call6((void (*)())Func_02001f2c, 51, 38, 70, 22, 1, 2);
    Flag_Set(0x201);
    Call1((void (*)())Scheduler_RemoveCallbackFar, (s32)callback);
    if (handle != 0)
        Call1((void (*)())Func_02001f14, handle);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Func_02002084, 0x10000, 0);
    Call1((void (*)())Func_0200208c, 20);
    Call1((void (*)())Battle_WaitMode0, 20);
    Call2((void (*)())Func_02002044, 7, 0x4000);
    Call2((void (*)())Func_02002034, 7, 0);
    Call2((void (*)())Object_SetModeById, 7, 3);
    Call2((void (*)())Func_02002034, 7, 0);
    Call2((void (*)())Object_SetModeById, 7, 2);
    object = Object_GetById(gPartyState.current_owner);
    if (object != 0)
        Call3((void (*)())ObjectMotion_ResetAndSetPosition, 7, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
    Call1((void (*)())ObjectMotion_CommitCurrentPositionAndActivate, 7);
    Call3((void (*)())Func_02001fc4, 7, 0, 0);
    Flag_Set(0x985);
    Func_02001f64();
}

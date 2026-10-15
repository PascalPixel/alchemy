#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"
#include "call.h"

extern const u8 MsgGuardsAnIntruder[];
extern const u8 MsgGrrrrrRrrruffRrufff[];

struct ObjectRuntime *Object_GetById(s32 id);
s32                   GameFlag_Test(s32 flag);
void                  Battle_WaitMode0(s32 frames);
void                  ObjectMotion_ResetTargetsAndVelocity(s32 id);
void                  ObjectMotion_EnableActionAndResetMotion(s32 id);
void                  ObjectMotion_SetAngleToward(s32 id, s32 target, s32 frames);
void                  Func_02001084(void);
void                  Func_0200108c(void);
void                  Func_02001104(const u8 *message);
void                  Func_0200110c(s32 id, s32 flags);
void                  Func_0200111c(s32 id, s32 a, s32 b);
void                  Func_02001124(s32 id, s32 a);
void                  Func_0200113c(s32 scene, s32 entrance);
void                  Func_02001164(s32 a);

void Func_02000d80(void)
{
    s32 moved = GameFlag_Test(0x300);
    s32 scene = (s32)&SceneId_Ov66fRoom2;

    if (moved)
        Func_0200113c(scene, 9);
    else
        Func_0200113c(scene, 8);
}

void Func_02000da8(s32 id)
{
    s32 owner = gPartyState.currentOwner;

    Func_02001084();
    Func_02001164(0);
    ObjectMotion_ResetTargetsAndVelocity(id);
    Call3((void (*)())Func_0200111c, id, 0x100, 30);
    ObjectMotion_SetAngleToward(id, owner, 20);
    Func_02001104(MsgGuardsAnIntruder);
    Func_0200110c(id, 0);
    Call2((void (*)())Func_02001124, owner, 0x101);
    Object_SetModeById(id, 4);
    Battle_WaitMode0(30);
    Func_0200110c(id, 0);
    Call2((void (*)())Func_02001124, owner, 0x100);
    Func_02000d80();
    Func_0200108c();
}

void Func_02000e28(s32 id)
{
    s32 owner = gPartyState.currentOwner;

    Func_02001084();
    Func_02001164(0);
    ObjectMotion_EnableActionAndResetMotion(id);
    Call3((void (*)())Func_0200111c, id, 0x100, 30);
    ObjectMotion_SetAngleToward(id, owner, 20);
    Func_02001104(MsgGrrrrrRrrruffRrufff);
    Func_0200110c(id, 0);
    Call2((void (*)())Func_02001124, owner, 0x101);
    Object_SetModeById(id, 4);
    Battle_WaitMode0(30);
    Func_0200110c(8, 0);
    Call2((void (*)())Func_02001124, owner, 0x100);
    Func_02000d80();
    Func_0200108c();
}

void Func_02000ea8(void)
{
    struct ObjectRuntime *owner = Object_GetById(gPartyState.currentOwner);
    struct ObjectRuntime *guard = Object_GetById(14);

    if ((guard->z - owner->z >= 0 ? guard->z - owner->z : owner->z - guard->z) < 0x100000
        && guard->x > owner->x)
        return;
    Func_02000da8(10);
}

void Func_02000ef4(void)
{
    struct ObjectRuntime *owner = Object_GetById(gPartyState.currentOwner);
    struct ObjectRuntime *guard = Object_GetById(15);

    if ((guard->z - owner->z >= 0 ? guard->z - owner->z : owner->z - guard->z) < 0x100000
        && guard->x < owner->x)
        return;
    Func_02000da8(10);
}

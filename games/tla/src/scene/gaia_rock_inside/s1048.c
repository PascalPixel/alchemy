#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "text/msg_ids.h"
#include "call.h"

extern s32 Data_0200559c[];
extern s32 Data_02005610[];
extern s32 Data_020056c4[];
extern s32 Data_02005728[];
extern s32 Data_0200646c[];
extern s32 Data_020064ec;
extern s32 Data_020064f0;

void                  GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void                  Object_AttachWorkTargetToObject(s32 id, s32 value);
void                  Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void                  Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void                  ObjectMotion_SetAngleToward(s32 id, s32 target, s32 frames);
void                  ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void                  ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void                  ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void                  Battle_WaitMode0(s32 frames);
void                  Func_02000f00(void);
void                  Func_02000fac(void);
void                  Func_02004e5c(void);
void                  Func_02004e64(void);
void                  Func_02004f0c(s32 message);
void                  Func_02004f14(s32 id, s32 value);
s32                   Func_02004f2c(s32 id, s32 emote, s32 frames);
void                  Func_02004f54(void);
void                  Func_02004fcc(s32 value);

// Object 10 turns to the leader, talks and walks to its place while the camera
// follows; flag 0x9de is set, and object 9's script decides whether two
// words of Data_0200646c are copied into place.
void Func_02001048(void)
{
    struct ObjectRuntime *object = Object_GetById(9);

    Func_02004e5c();
    Func_02004fcc(0);
    Func_02004f0c((s32)MsgGaiaRockInside02);
    Object_AttachWorkTargetToObject(10, 1);
    if (object->script == Data_02005610 || object->script == Data_0200559c)
        Object_RefreshSelectorById(9);
    Func_02000f00();
    Object_SetModeById(10, 1);
    ObjectMotion_SetAngleToward(10, gPartyState.current_owner, 0);
    Value3(Func_02004f2c, 10, 256, 0);
    Call1(Battle_WaitMode0, 30);
    Call2(Func_02004f14, 10, 0);
    Call3(ObjectMotion_ResetAndSetPositionInMode2, 10, 496, 456);
    Call1(ObjectMotion_CommitCurrentPositionAndActivate, 10);
    Call3(ObjectMotion_ArmCallback, 10, 0x1000, 0);
    Call1(Battle_WaitMode0, 10);
    Call2(Func_02004f14, 10, 0);
    Value3(Func_02004f2c, 10, 261, 0);
    Call1(Battle_WaitMode0, 45);
    Call2(Func_02004f14, 10, 0);
    Call3(ObjectMotion_ArmCallback, 10, 0xd000, 0);
    Call1(Battle_WaitMode0, 30);
    Call4(Motion_CamBounds, 0x1e80000, -1, 0x1880000, 1);
    Func_02004f54();
    Call2(Func_02004f14, 10, 0);
    Call2(Object_AttachWorkTargetToObject, 10, 1);
    Func_02004f54();
    Call3(ObjectMotion_ArmCallback, 10, 0x1000, 0);
    Call1(Battle_WaitMode0, 10);
    Call2(Func_02004f14, 10, 0);
    Call2(Motion_SetModeAndWaitAnimation, 10, 3);
    Call4(Motion_CamBounds, -1, -1, -1, 0);
    Call1(GameFlag_SetBit, 0x9de);
    if (object->script == Data_020056c4 || object->script == Data_02005728)
    {
        Data_0200646c[2] = Data_020064ec = Data_0200646c[14];
        Data_0200646c[4] = Data_020064f0 = Data_0200646c[16];
    }
    Func_02000fac();
    Func_02004e64();
}

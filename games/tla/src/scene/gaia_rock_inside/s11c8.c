#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "text/msg_ids.h"

extern s32 Data_02005610[];

s32                   GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void                  Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void                  ObjectMotion_SetAngleToward(s32 id, s32 target, s32 frames);
void                  Battle_WaitMode0(s32 frames);
void                  Func_02000f00(void);
void                  Func_02000fac(void);
void                  Func_02004e5c(void);
void                  Func_02004e64(void);
void                  Func_02004f0c(s32 message);
void                  Func_02004f14(s32 id, s32 value);
void                  Func_02004fcc(s32 value);

// Talking to object 10 while its byte at 0x60 is at most 2: once flag 0x9ac is
// set it says one of two lines, by whether any of flags 0x9bc to 0x9bf is set;
// before that it turns to the leader and speaks.
void Func_020011c8(void)
{
    if (*((u8 *)Object_GetById(10) + 96) > 2)
        return;
    Func_02004e5c();
    Func_02004fcc(0);
    if (GameFlag_Test(0x9ac))
    {
        if (GameFlag_Test(0x9bc) || GameFlag_Test(0x9bd) || GameFlag_Test(0x9be)
            || GameFlag_Test(0x9bf))
            Func_02004f0c((s32)MsgGaiaRockInside10);
        else
            Func_02004f0c((s32)MsgGaiaRockInside06);
        Func_02004f14(10, 0);
    }
    else
    {
        if (Object_GetById(9)->script == Data_02005610)
            Object_RefreshSelectorById(9);
        Func_02000f00();
        Func_02004f0c((s32)MsgGaiaRockInside03);
        ObjectMotion_SetAngleToward(10, gPartyState.currentOwner, 0);
        Battle_WaitMode0(10);
        Func_02004f14(10, 0);
        Motion_SetModeAndWaitAnimation(10, 3);
        Battle_WaitMode0(10);
        Func_02000fac();
    }
    Func_02004e64();
}

#include "ov696.h"

struct Ov696Object *Func_02001fec(void);
void DriftScene_SpawnObject(s32 x, s32 y, s32 z, u16 value);
void Object_LinkObjectAndSetCallback(s32 actor, s32 target);

extern const u8 Data_02004cfc[];
extern const u8 Data_02004d40[];
extern const u8 Data_02004d84[];
extern const u8 Data_02004dc8[];
extern const u8 Data_02004e0c[];
extern const u8 Data_02004e5c[];
extern const u8 Data_02004eac[];
extern const u8 Data_02004f08[];

void Func_02002068(void)
{
    struct Ov696Object *rod;
    struct Ov696Object *object;
    s32 angle;

    Func_020048c4();
    Func_02004a24(0);
    Engine_AudioPlayCue(78);
    angle = 0xc000;
    Object_GetById(gPartyState.current_owner)->facing = angle;
    WaitFrames(1);
    Ov696_CamSpeed(0xcccc, 0x1999);
    Func_020049fc()->motion_flags = 0;
    Ov696_CamMove(0x780000, 0x280000, 0xf60000, 1);
    Ov696_ObjSpeed(gPartyState.current_owner, 0x13333, 0x9999);
    Ov696_ObjWalk(gPartyState.current_owner, 120, 264);
    Object_SetModeById(gPartyState.current_owner, 28);
    rod = Func_02001fec();
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(15, 256, 20)
    Ov696_ObjSpeed(15, 0x26666, 0x13333);
    Ov696_ObjWalk(15, 120, 242);
    Engine_AudioPlayCue(29);
    Func_0200498c((s32)MsgShamanRecognizeShamanRod);
    Ov696_MesPut(15, 0);
    Battle_WaitMode0(20);
    if (rod != NULL)
        Func_0200481c(rod);
    Object_SetModeById(gPartyState.current_owner, 1);
    Ov696_ObjTurn(gPartyState.current_owner, angle);
    Func_02004944(18, gPartyState.current_owner);
    Func_02004944(7, gPartyState.current_owner);
    Func_02004944(5, gPartyState.current_owner);
    Func_02004944(6, gPartyState.current_owner);
    WaitFrames(1);
    Ov696_ObjSpeed(18, 0x19999, 0xcccc);
    Ov696_ObjSpeed(7, 0x19999, 0xcccc);
    Ov696_ObjSpeed(5, 0x19999, 0xcccc);
    Ov696_ObjSpeed(6, 0x19999, 0xcccc);
    ObjectMotion_EnableActionAndSetCallback(18, Data_02004cfc);
    ObjectMotion_EnableActionAndSetCallback(7, Data_02004d40);
    ObjectMotion_EnableActionAndSetCallback(5, Data_02004d84);
    ObjectMotion_EnableActionAndSetCallback(6, Data_02004dc8);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(17, 256, 0)
    Ov696_ObjSpeed(17, 0x13333, 0x9999);
    Ov696_ObjWalk(17, 136, 232);
    Ov696_MesPut(17, 0);
    Ov696_ObjSpeed(16, 0x13333, 0x9999);
    Ov696_ObjWalk(16, 104, 232);
    Ov696_MesPut(16, 0);
    Motion_SetModeAndWaitAnimation(15, 3);
    Ov696_MesPut(15, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(15, 257, 20)
    Ov696_MesPut(15, 0);
    Ov696_ObjEmoteWait(18, 258);
    Battle_WaitMode0(20);
    Ov696_MesPut(18, 0);
    Ov696_ObjSpeed(15, 0xcccc, 0x6666);
    Ov696_ObjWalk(15, 120, 252);
    Battle_WaitMode0(40);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(15, 257, 20)
    Ov696_ObjWalk(15, 136, 252);
    Ov696_ObjArm(15, 0x4000, 20);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(15, 257, 40)
    Ov696_ObjWalk(15, 104, 252);
    Ov696_ObjArm(15, 0x4000, 20);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(15, 257, 20)
    Ov696_ObjWalk(15, 152, 252);
    Ov696_ObjArm(15, 0x4000, 20);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(15, 257, 40)
    Ov696_ObjWalk(15, 88, 252);
    Ov696_ObjArm(15, 0x4000, 40);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(15, 257, 80)
    Ov696_ObjWalk(15, 120, 252);
    Ov696_ObjWalk(15, 120, 242);
    Ov696_ObjTurn(15, 0x4000);
    Object_SetModeById(15, 4);
    Ov696_MesPut(15, 0);
    Ov696_ObjArm(6, 0, 0);
    Ov696_ObjArm(5, 0x8000, 0);
    Ov696_ObjArm(gPartyState.current_owner, 0, 0);
    Ov696_ObjArm(7, 0x8000, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(18, 256, 20)
    ObjectMotion_Launch(18, 4, 0);
    Ov696_MesPut(18, 0);
    Ov696_ObjArm(15, 0x2000, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(15, 265, 20)
    Ov696_MesPut(15, 0);
    Ov696_ObjArm(6, 0xc000, 0);
    Ov696_ObjArm(5, 0xc000, 0);
    Ov696_ObjArm(gPartyState.current_owner, 0xc000, 0);
    Ov696_ObjArm(7, 0xc000, 0);
    Ov696_ObjTurn(15, 0x4000);
    Motion_SetModeAndWaitAnimation(15, 4);
    Ov696_MesPut(15, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(6, 259, 0)
    Ov696_MesPut(6, 0);
    Ov696_ObjArm(15, 0x6000, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(5, 257, 0)
    Ov696_MesPut(5, 0);
    Motion_SetModeAndWaitAnimation(15, 3);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(7, 265, 20)
    Ov696_MesPut(7, 0);
    Ov696_ObjArm(15, 0x2000, 0);
    Ov696_ObjEmoteWait(18, 258);
    Ov696_MesPut(18, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(15, 265, 40)
    Ov696_MesPut(15, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(17, 262, 20)
    Ov696_ObjTurn(17, 0x6000);
    Ov696_MesPut(17, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(15, 257, 0)
    Ov696_ObjArm(15, 0xe000, 20);
    Object_SetModeById(17, 3);
    Ov696_MesPut(17, 0);
    {
        s32 y = 0x500000;
        s32 z = 0xf20000;

        DriftScene_SpawnObject(0x740000, y, z, 0xe000);
        Battle_WaitMode0(10);
        DriftScene_SpawnObject(0x7c0000, y, z, 0x2000);
        Battle_WaitMode0(30);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(16, 262, 20)
    Ov696_ObjTurn(16, 0);
    Ov696_MesPut(16, 0);
    Ov696_ObjTurn(17, 0x8000);
    Motion_SetModeAndWaitAnimation(17, 3);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(15, 264, 40)
    Motion_SetModeAndWaitAnimation(15, 3);
    Ov696_ObjArm(17, 0x4000, 0);
    Ov696_ObjArm(16, 0x4000, 0);
    Ov696_ObjTurn(15, 0x4000);
    Ov696_MesPut(15, 0);
    Object_SetModeById(15, 3);
    Ov696_MesPut(15, 0);
    Ov696_CamSpeed(0x6666, 0xccc);
        Ov696_CamMove(0xb40000, 0x200000, z, 1);
    }
    object = Object_GetById(16);
    object->unknown_63 = 0;
    Object_LinkObjectAndSetCallback(gPartyState.current_owner, 15);
    Object_LinkObjectAndSetCallback(5, 15);
    Object_LinkObjectAndSetCallback(6, 15);
    Object_LinkObjectAndSetCallback(7, 15);
    Object_LinkObjectAndSetCallback(18, 15);
    Ov696_ObjSpeed(15, 0x19999, 0xcccc);
    Ov696_ObjSpeed(16, 0x19999, 0xcccc);
    Ov696_ObjSpeed(17, 0x19999, 0xcccc);
    ObjectMotion_EnableActionAndSetCallback(15, Data_02004e0c);
    Battle_WaitMode0(20);
    ObjectMotion_EnableActionAndSetCallback(17, Data_02004e5c);
    ObjectMotion_EnableActionAndSetCallback(16, Data_02004eac);
    Engine_AudioPlayCue(78);
    do {
        WaitFrames(1);
    } while (object->unknown_63 == 0);
    ObjectMotion_EnableActionAndResetMotion(gPartyState.current_owner);
    ObjectMotion_EnableActionAndResetMotion(18);
    ObjectMotion_EnableActionAndResetMotion(7);
    ObjectMotion_EnableActionAndResetMotion(5);
    ObjectMotion_EnableActionAndResetMotion(6);
    WaitFrames(1);
    Func_02004a2c();
    Ov696_CamSpeed(0x33333, 0x6666);
    Ov696_CamMove(0x780000, -1, 0x1040000, 1);
    Func_020049f4();
    Func_02004934(15, 0, 0);
    Func_02004934(17, 0, 0);
    Func_02004934(16, 0, 0);
    Ov696_ObjTurn(6, 0);
    Ov696_MesPut(6, 0);
    Ov696_ObjTurn(5, 0x8000);
    Ov696_MesPut(0x2005, 0);
    Ov696_MesPut(18, 0);
    Ov696_ObjTurn(18, 0x8000);
    Ov696_MesPut(18, 0);
    Ov696_ObjArm(7, 0, 0);
    Ov696_ObjArm(5, 0, 0);
    Ov696_ObjTurn(gPartyState.current_owner, 0);
    Motion_SetModeAndWaitAnimation(gPartyState.current_owner, 3);
    Ov696_ObjSpeed(6, 0x10000, 0x8000);
    Ov696_ObjSpeed(5, 0x10000, 0x8000);
    Ov696_ObjSpeed(7, 0x10000, 0x8000);
    Ov696_ObjSpeed(18, 0x10000, 0x8000);
    ObjectMotion_EnableActionAndSetCallback(6, Data_02004f08);
    ObjectMotion_EnableActionAndSetCallback(5, Data_02004f08);
    ObjectMotion_EnableActionAndSetCallback(7, Data_02004f08);
    Object_SetActionCallbackAndRefreshById(18, Data_02004f08);
    Battle_WaitMode0(20);
    GameFlag_SetBit(514);
    GameFlag_SetBit(0x9e1);
    Func_020048cc();
}

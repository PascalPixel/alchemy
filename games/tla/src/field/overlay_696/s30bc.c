#include "ov696.h"

#define EVENT_WORK ((struct Ov696EventWork *)Ram_HeapSlots->event_work)

void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 x, s32 z);
void ObjectMotion_WaitForAnimationChange(s32 actor);
void Object_LinkObjectAndSetCallback(s32 actor, s32 target);
void Func_02003b24(void);

extern const u8 Data_02004f3c[];
extern const u8 Data_02004f80[];
extern const u8 Data_02004fc4[];
extern const u8 Data_02005008[];
extern const u8 Data_0200504c[];
extern const u8 Data_020050bc[];

void Func_020030bc(void)
{
    u8 *work = Ram_HeapSlots->effect_work;
    struct Ov696Object *object = Object_GetById(16);

    s32 result;
    s32 owner;
    s32 angle;
    u8 *done;

    work[34] = 0;
    work[35] = 0;
    Func_020048c4();
    Func_02004a24(0);
    Engine_AudioPlayCue(0);
    Ov696_CamMove(-1, -1, -1, 0);
    Ov696_CamSpeed(0x6666, 0xccc);
    Ov696_CamMove(0xa80000, -1, 0x2100000, 1);
    Ov696_ObjSpeed(gPartyState.current_owner, 0xcccc, 0x6666);
    Ov696_ObjWalk(gPartyState.current_owner, 168, 536);
    Func_02004944(18, gPartyState.current_owner);
    Func_02004944(7, gPartyState.current_owner);
    Func_02004944(5, gPartyState.current_owner);
    Func_02004944(7, gPartyState.current_owner);
    Func_02004944(6, gPartyState.current_owner);
    WaitFrames(1);
    Ov696_ObjArm(6, 0xc000, 0);
    Ov696_ObjSpeed(18, 0x10000, 0x8000);
    Ov696_ObjSpeed(7, 0x10000, 0x8000);
    Ov696_ObjSpeed(5, 0x10000, 0x8000);
    Ov696_ObjSpeed(gPartyState.current_owner, 0x10000, 0x8000);
    ObjectMotion_EnableActionAndSetCallback(gPartyState.current_owner, Data_02004f80);
    ObjectMotion_EnableActionAndSetCallback(5, Data_02004fc4);
    ObjectMotion_EnableActionAndSetCallback(18, Data_02004f3c);
    Object_SetActionCallbackAndRefreshById(7, Data_02005008);
    Ov696_ObjTurn(5, 0);
    Func_0200498c((s32)MsgShamanCheerForCompanion);
    Ov696_MesPut(5, 0);
    Ov696_ObjTurn(6, 0x8000);
    Object_SetModeById(6, 3);
    Ov696_MesPut(0x2006, 0);
    Ov696_ObjTurn(6, 0xc000);
    Ov696_ObjTurn(6, 0);
    Motion_SetModeAndWaitAnimation(6, 3);
    Ov696_ObjSpeed(6, 0x6666, 0x3333);
    Ov696_ObjWalk(6, 168, 524);
    Ov696_ObjArm(15, 0xc000, 0);
    Ov696_ObjArm(17, 0xc000, 0);
    Ov696_ObjArm(16, 0xc000, 0);
    Ov696_CamSpeed(0x19999, 0x3333);
    Func_02003b24();
    Engine_AudioPlayCue(29);
    Ov696_CamSpeed(0x4cccc, 0x9999);
    Ov696_CamMove(0xa80000, -1, 0x21c0000, 1);
    Func_020049f4();
    Battle_WaitMode0(20);
    Ov696_ObjEmoteWait(15, 258);
    Battle_WaitMode0(20);
    Ov696_MesPut(15, 0);
    Ov696_ObjEmoteWait(17, 258);
    Ov696_MesPut(0x8011, 0);
    Ov696_ObjEmoteWait(16, 258);
    Ov696_MesWait(0x8010, 0, 20);
    Motion_SetVarCbAndRefresh(15, 1);
    Ov696_ObjTurn(15, 0xa000);
    Ov696_MesPut(0x200f, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(15, 256, 0)
    Ov696_ObjTurn(15, 0xc000);
    Ov696_MesPut(15, 0);
    ObjectMotion_Launch(6, 4, 0);
    Ov696_ObjTurn(6, 0x4000);
    Object_SetModeById(6, 3);
    Ov696_MesPut(6, 0);
    Ov696_ObjTurn(7, 0x6000);
    Ov696_MesPut(0x4007, 0);
    Ov696_ObjTurn(15, 0xe000);
    Object_SetModeById(15, 4);
    Ov696_MesPut(15, 0);
    Ov696_ObjArm(gPartyState.current_owner, 0x4000, 0);
    Ov696_ObjArm(18, 0, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(5, 256, 20)
    Ov696_ObjTurn(5, 0x2000);
    Ov696_MesPut(5, 0);
    Ov696_ObjTurn(15, 0xa000);
    Ov696_MesPut(0x200f, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(18, 257, 40)
    Ov696_MesPut(0x4012, 0);
    Motion_SetModeAndWaitAnimation(15, 3);
    Ov696_ObjTurn(15, 0xc000);
    Ov696_MesPut(15, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(17, 256, 0)
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(16, 256, 0)
    Ov696_ObjArm(16, 0x2000, 0);
    Ov696_ObjTurn(17, 0x6000);
    ObjectMotion_SetVariantCallback(17, 1);
    Ov696_MesPut(17, 0);
    ObjectMotion_SetVariantCallback(16, 1);
    Ov696_MesPut(16, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(6, 259, 0)
    Ov696_ObjArm(6, 0x6000, 20);
    Ov696_ObjTurn(6, 0x4000);
    Ov696_MesPut(6, 0);
    Ov696_ObjEmoteWait(15, 258);
    Ov696_MesPut(15, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(6, 259, 0)
    ObjectMotion_Launch(6, 2, 10);
    ObjectMotion_Launch(6, 4, 10);
    Ov696_MesPut(6, 0);
    Ov696_ObjTurn(17, 0xa000);
    Object_SetModeById(17, 4);
    Ov696_MesPut(0x8011, 0);
    Ov696_ObjTurn(16, 0xe000);
    Object_SetModeById(16, 4);
    Ov696_MesPut(0x8010, 0);
    Ov696_ObjArm(16, 0, 0);
    Ov696_ObjTurn(17, 0x8000);
    Battle_WaitMode0(10);
    Object_SetModeById(16, 3);
    Motion_SetModeAndWaitAnimation(17, 3);
    Ov696_CamSpeed(0x6666, 0xccc);
    Ov696_CamMove(0xa80000, -1, 0x2300000, 1);
    Ov696_ObjArm(17, 0x4000, 0);
    Ov696_ObjTurn(16, 0x4000);
    Ov696_ObjSpeed(17, 0x10000, 0x8000);
    Ov696_ObjSpeed(16, 0x10000, 0x8000);
    Ov696_ObjWalk2(16, 120, 560);
    Ov696_ObjWalk(17, 216, 560);
    Object_SetModeById(17, 1);
    Ov696_ObjArm(17, 0x8000, 0);
    Ov696_ObjTurn(16, 0);
    Ov696_ObjWalk2(16, 152, 560);
    Ov696_ObjWalk(17, 184, 560);
    Object_SetModeById(16, 1);
    Ov696_ObjArm(17, 0x4000, 0);
    Ov696_ObjArm(16, 0x4000, 0);
    Ov696_ObjTurn(15, 0x4000);
    Ov696_ObjSpeed(15, 0x10000, 0x8000);
    Object_SetModeById(15, 2);
    Object_SetModeById(17, 2);
    Object_SetModeById(16, 2);
    ObjectMotion_OffsetPositionAndResetMotion(15, 0, 16);
    ObjectMotion_OffsetPositionAndResetMotion(17, 0, 16);
    ObjectMotion_OffsetPositionAndResetMotion(16, 0, 16);
    ObjectMotion_CommitCurrentPositionAndActivate(15);
    Object_SetModeById(15, 1);
    Object_SetModeById(17, 1);
    Object_SetModeById(16, 1);
    Ov696_ObjTurn(18, 0x3000);
    Ov696_MesPut(18, 0);
    Ov696_ObjArm(15, 0xa000, 0);
    Ov696_ObjArm(17, 0xa000, 0);
    Ov696_ObjArm(16, 0xa000, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(18, 265, 20)
    Ov696_MesPut(18, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(17, 257, 0)
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(16, 257, 20)
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(5, 264, 40)
    Ov696_MesPut(5, 0);
    Ov696_ObjArm(16, 0, 0);
    Ov696_ObjTurn(17, 0x8000);
    Motion_SetModeAndWaitAnimation(7, 3);
    Ov696_MesPut(7, 0);
    Ov696_ObjArm(15, 0xe000, 0);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(15, 259, 20)
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(6, 259, 20)
    Ov696_MesPut(6, 0);
    Ov696_ObjArm(15, 0xc000, 20);
    Motion_SetModeAndWaitAnimation(15, 4);
    Battle_WaitMode0(20);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(17, 262, 0)
    Ov696_ObjTurn(17, 0x6000);
    Ov696_MesPut(17, 0);
    Ov696_ObjTurn(15, 0xe000);
    Ov696_ObjTurn(16, 0x2000);
    Ov696_MesPut(16, 0);
    Ov696_ObjEmoteWait(15, 258);
    Ov696_ObjArm(15, 0xa000, 20);
    Ov696_ObjTurn(17, 0x8000);
    ObjectMotion_Launch(17, 4, 10);
    Ov696_MesPut(17, 0);
    Ov696_ObjTurn(16, 0);
    Motion_SetModeAndWaitAnimation(16, 3);
    Battle_WaitMode0(10);
    Ov696_ObjTurn(16, 0x2000);
    Ov696_MesPut(16, 0);
    Ov696_ObjArm(6, 0x6000, 0);
    Ov696_ObjArm(5, 0xe000, 40);
    Ov696_ObjArm(6, 0x4000, 0);
    Ov696_ObjTurn(5, 0x4000);
    Ov696_MesPut(5, 0);
    Object_SetModeById(6, 3);
    Ov696_MesPut(6, 0);
    Ov696_ObjEmoteWait(15, 258);
    Ov696_ObjArm(15, 0xc000, 40);
    Ov696_ObjTurn(15, 0xe000);
    Battle_WaitMode0(20);
    Motion_SetModeAndWaitAnimation(17, 3);
    Battle_WaitMode0(20);
    Ov696_ObjTurn(15, 0xa000);
    Battle_WaitMode0(20);
    Motion_SetModeAndWaitAnimation(16, 3);
    Battle_WaitMode0(20);
    Ov696_ObjTurn(15, 0xc000);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see OV696_EMOTE. */
    OV696_EMOTE(15, 265, 40)
    Ov696_MesPut(15, 0);
    Object_SetModeById(15, 4);
    Ov696_MesOpen(15, 0);
    Ov696_ObjArm(6, 0x2000, 0);
    Ov696_ObjArm(5, 0, 0);
    Ov696_ObjArm(18, 0, 0);
    Ov696_ObjArm(7, 0xa000, 0);
    result = Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0);
    done = &object->unknown_63;
    owner = gPartyState.current_owner;
    if (result == 0) {
        ObjectMotion_WaitForAnimationChange(owner);
        Motion_SetModeAndWaitAnimation(15, 4);
        Ov696_MesPut(15, 0);
    } else {
        ObjectMotion_WaitForAnimationChange(owner);
        Motion_SetModeAndWaitAnimation(15, 4);
        EVENT_WORK->field_1c4++;
        Ov696_MesPut(15, 0);
        Object_SetModeById(15, 3);
        Ov696_MesPut(15, 0);
    }
    Ov696_ObjWalk(15, 212, 582);
    Ov696_CamMove(0xa80000, -1, 0x1f40000, 1);
    Object_LinkObjectAndSetCallback(gPartyState.current_owner, 15);
    Object_LinkObjectAndSetCallback(18, 15);
    Object_LinkObjectAndSetCallback(5, 15);
    Object_LinkObjectAndSetCallback(7, 15);
    Object_LinkObjectAndSetCallback(6, 15);
    *done = 0;
    ObjectMotion_EnableActionAndSetCallback(15, Data_0200504c);
    Battle_WaitMode0(20);
    ObjectMotion_EnableActionAndSetCallback(17, Data_0200504c);
    ObjectMotion_EnableActionAndSetCallback(16, Data_0200504c);
    Engine_AudioPlayCue(78);
    do {
        WaitFrames(1);
    } while (*done == 0);
    ObjectMotion_EnableActionAndResetMotion(gPartyState.current_owner);
    ObjectMotion_EnableActionAndResetMotion(18);
    ObjectMotion_EnableActionAndResetMotion(5);
    ObjectMotion_EnableActionAndResetMotion(7);
    ObjectMotion_EnableActionAndResetMotion(6);
    Func_02004a2c();
    Ov696_CamSpeed(0x4cccc, 0x9999);
    Ov696_CamMove(0xa80000, -1, 0x21c0000, 1);
    Battle_WaitMode0(20);
    angle = 0x4000;
    Ov696_ObjPut(15, 0xe00000, 0x1180000, angle);
    Ov696_ObjPut(16, 0xd00000, 0x1080000, angle);
    Ov696_ObjPut(17, 0xf00000, 0x1080000, angle);
    Ov696_ObjArm(gPartyState.current_owner, 0x8000, 0);
    Ov696_ObjArm(18, 0, 0);
    Ov696_ObjArm(5, 0, 0);
    Ov696_ObjArm(7, 0xa000, 0);
    Ov696_ObjTurn(6, 0x2000);
    Battle_WaitMode0(10);
    Object_SetModeById(18, 3);
    Object_SetModeById(5, 3);
    Object_SetModeById(7, 3);
    Motion_SetModeAndWaitAnimation(6, 3);
    ObjectMotion_EnableActionAndResetMotion(gPartyState.current_owner);
    ObjectMotion_EnableActionAndResetMotion(18);
    ObjectMotion_EnableActionAndResetMotion(5);
    ObjectMotion_EnableActionAndResetMotion(7);
    ObjectMotion_EnableActionAndResetMotion(6);
    Object_GetById(gPartyState.current_owner)->update = NULL;
    Object_GetById(18)->update = NULL;
    Object_GetById(5)->update = NULL;
    Object_GetById(7)->update = NULL;
    Object_GetById(6)->update = NULL;
    WaitFrames(1);
    Ov696_ObjSpeed(6, 0x10000, 0x8000);
    ObjectMotion_EnableActionAndSetCallback(7, Data_020050bc);
    ObjectMotion_EnableActionAndSetCallback(6, Data_020050bc);
    ObjectMotion_EnableActionAndSetCallback(5, Data_020050bc);
    Object_SetActionCallbackAndRefreshById(18, Data_020050bc);
    GameFlag_SetBit(0x94c);
    Func_020048cc();
}

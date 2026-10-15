#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "text/msg_ids.h"

struct VinasuActor {
    u8 unknown_00[0xa];
    s16 x_pixel;
    u8 unknown_0c[6];
    s16 z_pixel;
};

void Func_02001c1c(s32 message);
void Object_AttachWorkTargetToObject(s32 actor, s32 target);
void Func_02001c4c(void);
void Engine_EventWait(s32 frames);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Func_02001c34(s32 actor, s32 height, s32 frames);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void Motion_SetVarCbAndRefresh(s32 actor, s32 value);
void Func_02001c24(s32 actor, s32 value);
void Engine_AudioPlayCue(s32 cue);
void Func_02001be4(s32 actor, s32 x, s32 z);
void Func_02001cd4(s32 actor);
void ObjectMotion_SetVariantCallback(s32 actor, s32 value);
void Object_LinkPair(s32 first, s32 second, s32 distance);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Object_LinkObjectAndSetCallback(s32 actor, s32 target);
void ObjectMotion_CommitPositionAndActivate(s32 actor, s32 x, s32 z);
void Engine_ActorSetAnimation(s32 actor, s32 animation);
struct VinasuActor *Engine_ActorGet(s32 id);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void Func_02001ccc(s32 actor, s32 value);
void Engine_MapCopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);
void GameFlag_ClearBit(s32 flag);
void Party_RemoveActiveOwner(s32 owner);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001c54(s32 value);
void Event_StartScreenTransition(void);
void VinasuChojo_RunSetupStep(void);
void SceneState_RunActor13AtColumn42Setup(void);

static inline void Vinasu_MesSet(s32 message)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001c1c(message);
}

static inline void Vinasu_ObjAttach(s32 actor, s32 target)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Object_AttachWorkTargetToObject(actor, target);
}

static inline void Vinasu_Wait(s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Engine_EventWait(frames);
}

static inline void Vinasu_ObjWalk(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndReset(actor, x, z);
}

static inline void Vinasu_ObjTurn(s32 actor, s32 angle, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void Vinasu_ObjModeWait(s32 actor, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_SetModeAndWaitAnimation(actor, mode);
}

static inline void Vinasu_ObjWalk2(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ResetAndSetPositionInMode2(actor, x, z);
}

static inline void Vinasu_ObjCommitHere(s32 actor)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_CommitCurrentPositionAndActivate(actor);
}

static inline void Vinasu_Jump(s32 actor, s32 height, s32 frames)
{
    /* FAKEMATCH: the native loads the actor after the height and frames. */
    register s32 h __asm__("r1") = height;
    /* FAKEMATCH: the frames stay fixed in r2 for the same reason. */
    register s32 f __asm__("r2") = frames;

    Func_02001c34(actor, h, f);
}

static inline void Vinasu_CamMove(s32 x, s32 y, s32 z, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_CamBounds(x, y, z, mode);
}

static inline void Vinasu_ObjVar(s32 actor, s32 value)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_SetVarCbAndRefresh(actor, value);
}

static inline void Vinasu_MesPut(s32 actor, s32 value)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001c24(actor, value);
}

static inline void Vinasu_Se(s32 cue)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Engine_AudioPlayCue(cue);
}

static inline void Vinasu_ObjSetPos(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001be4(actor, x, z);
}

static inline void Vinasu_ObjEffect(s32 actor)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001cd4(actor);
}

static inline void Vinasu_ObjVariant(s32 actor, s32 value)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetVariantCallback(actor, value);
}

static inline void Vinasu_ObjLink(s32 first, s32 second, s32 distance)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Object_LinkPair(first, second, distance);
}

static inline void Vinasu_ObjSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static inline void Vinasu_ObjFollow(s32 actor, s32 target)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Object_LinkObjectAndSetCallback(actor, target);
}

static inline void Vinasu_ObjCommit(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_CommitPositionAndActivate(actor, x, z);
}

static inline void Vinasu_ObjAnim(s32 actor, s32 animation)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Engine_ActorSetAnimation(actor, animation);
}

static inline void Vinasu_ObjPlace(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ResetAndSetPosition(actor, x, z);
}

static inline void Vinasu_ObjEffectSet(s32 actor, s32 value)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001ccc(actor, value);
}

static inline void Vinasu_CopyCells(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Engine_MapCopyCellsTo(src_x, src_y, dest_x, dest_y, width, height);
}

static inline void Vinasu_FlagClr(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_ClearBit(flag);
}

static inline void Vinasu_PartyRemove(s32 owner)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Party_RemoveActiveOwner(owner);
}

static inline void Vinasu_Countdown(s32 value)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001c54(value);
}

void Func_02000b2c(void)
{
    struct EventWork *work;

    Vinasu_ObjSetPos(4, 0x2c80000, 0x3300000);
    Vinasu_ObjSetPos(5, 0x2b80000, 0x3480000);
    Vinasu_ObjSetPos(8, 0x2d80000, 0x3480000);
    Vinasu_ObjTurn(4, 0x4000, 0);
    Vinasu_ObjTurn(5, 0xc000, 0);
    Vinasu_ObjTurn(8, 0xc000, 0);
    Vinasu_ObjSpeed(4, 0x10000, 0x8000);
    Vinasu_ObjSpeed(5, 0x10000, 0x8000);
    Vinasu_ObjSpeed(8, 0x10000, 0x8000);
    Vinasu_ObjSpeed(9, 0x10000, 0x8000);
    Vinasu_CamMove(0x2980000, -1, 0x3380000, 0);
    work = Ram_HeapSlots->event_work;
    work->screen_effect = 256;
    work->delay = 60;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Vinasu_Wait(20);
    Vinasu_ObjModeWait(4, 3);
    Vinasu_Wait(20);
    Vinasu_ObjAnim(5, 3);
    Vinasu_ObjModeWait(8, 3);
    Vinasu_Wait(10);
    Vinasu_ObjWalk(4, 664, 824);
    Vinasu_ObjTurn(4, 0xc000, 0);
    Vinasu_Wait(10);
    Vinasu_ObjWalk(8, 712, 824);
    Vinasu_ObjTurn(5, 0xa000, 0);
    Vinasu_Wait(20);
    VinasuChojo_RunSetupStep();
    SceneState_RunActor13AtColumn42Setup();
    Vinasu_Wait(20);
    Vinasu_ObjWalk(8, 728, 840);
    Vinasu_ObjTurn(8, 0xc000, 0);
    Vinasu_ObjTurn(5, 0xc000, 0);
    Vinasu_ObjWalk(4, 712, 816);
    Vinasu_ObjTurn(4, 0x4000, 0);
    Vinasu_Wait(30);
    Vinasu_ObjModeWait(4, 3);
    Vinasu_Wait(20);
    Vinasu_ObjAnim(5, 3);
    Vinasu_ObjModeWait(8, 3);
    Vinasu_Wait(10);
    Vinasu_ObjFollow(5, 4);
    Vinasu_ObjFollow(8, 4);
    Vinasu_ObjWalk(4, 744, 816);
    Vinasu_ObjWalk(4, 744, 780);
    Vinasu_Se(129);
    Vinasu_ObjEffectSet(4, -1);
    Vinasu_Wait(50);
    Vinasu_ObjTurn(5, 0xc000, 0);
    Vinasu_ObjTurn(8, 0xc000, 0);
    Vinasu_ObjSetPos(4, 0, 0);
}

void Func_02000d30(void)
{
    struct VinasuActor *actor;
    struct EventWork *work;

    Vinasu_MesSet((s32)MsgVinasuChojoAreYouSureWeShouldBeLeaving);
    Vinasu_ObjAttach(8, 1);
    Func_02001c4c();
    Vinasu_Wait(30);
    Vinasu_ObjWalk(5, 664, 840);
    Vinasu_ObjTurn(5, 0, 0);
    Vinasu_ObjWalk(8, 696, 840);
    Vinasu_Wait(20);
    Vinasu_ObjModeWait(8, 3);
    Vinasu_Wait(10);
    Vinasu_ObjModeWait(5, 3);
    Vinasu_Wait(10);
    Vinasu_ObjWalk2(8, 696, 920);
    Vinasu_Wait(10);
    Vinasu_ObjWalk(5, 664, 888);
    Vinasu_ObjCommitHere(8);
    Vinasu_Jump(8, 256, 30);
    Vinasu_ObjTurn(8, 0xa000, 0);
    Vinasu_Wait(20);
    Vinasu_ObjWalk(8, 696, 888);
    Vinasu_ObjTurn(5, 0, 0);
    Vinasu_ObjTurn(8, 0x8000, 0);
    Vinasu_Wait(20);
    Vinasu_CamMove(0x2b80000, -1, 0x3980000, 1);
    Func_02001c4c();
    Vinasu_Wait(20);
    Vinasu_ObjVar(5, 2);
    Vinasu_Wait(10);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(8, 257, 50);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_ObjModeWait(8, 3);
    Vinasu_Wait(20);
    Vinasu_ObjVar(5, 2);
    Vinasu_Wait(10);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(8, 257, 30);
    Vinasu_MesPut(0x2008, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(5, 0xe000, 0);
    Vinasu_Wait(30);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(8, 258, 40);
    Vinasu_MesPut(0x2008, 0);
    Vinasu_Wait(10);
    Vinasu_ObjModeWait(5, 3);
    Vinasu_Wait(10);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(30);
    Vinasu_Se(198);
    Vinasu_ObjSetPos(9, 0x2a80000, 0x3980000);
    Vinasu_ObjTurn(9, 0xb000, 0);
    Vinasu_ObjEffect(9);
    Vinasu_ObjTurn(9, 0xb000, 0);
    Vinasu_Wait(20);
    Vinasu_ObjTurn(5, 0x4000, 0);
    Vinasu_ObjTurn(8, 0x4000, 0);
    Vinasu_Wait(10);
    Vinasu_Wait(20);
    Vinasu_ObjModeWait(9, 4);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjVariant(5, 2);
    Vinasu_ObjVar(8, 2);
    Vinasu_Wait(20);
    Vinasu_ObjLink(5, 8, 40);
    Vinasu_Jump(9, 256, 30);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(5, 0x4000, 0);
    Vinasu_ObjTurn(8, 0x4000, 0);
    Vinasu_Wait(20);
    Vinasu_Jump(8, 258, 40);
    Vinasu_MesPut(0x2008, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(9, 257, 30);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(9, 0, 0);
    Vinasu_Wait(30);
    Vinasu_ObjTurn(9, 0x8000, 0);
    Vinasu_Wait(30);
    Vinasu_ObjTurn(9, 0xc000, 0);
    Vinasu_Wait(30);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjVar(5, 2);
    Vinasu_Wait(20);
    Vinasu_ObjSpeed(5, 0x8000, 0x4000);
    Vinasu_ObjFollow(8, 5);
    Vinasu_ObjCommit(5, 0, -32);
    Vinasu_Wait(10);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(9, 261, 30);
    Vinasu_ObjCommit(9, -8, -8);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjVar(8, 2);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(8, 0x5000, 0);
    Vinasu_Wait(20);
    Vinasu_MesPut(0x2008, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(9, 256, 30);
    Vinasu_ObjTurn(9, 0xe000, 0);
    Vinasu_Wait(30);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(9, 257, 30);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(8, 258, 0);
    Vinasu_Jump(5, 258, 40);
    Vinasu_MesPut(0x2008, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(9, 264, 30);
    Vinasu_ObjTurn(9, 0xf000, 0);
    Vinasu_Wait(30);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(20);
    Vinasu_Jump(5, 262, 40);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(9, 0xb000, 0);
    Vinasu_Wait(20);
    Vinasu_ObjTurn(5, 0x3000, 0);
    Vinasu_Wait(30);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(9, 265, 30);
    Vinasu_Wait(20);
    Vinasu_ObjVar(9, 2);
    Vinasu_Wait(10);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjModeWait(9, 4);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjModeWait(5, 4);
    Vinasu_Wait(10);
    Vinasu_ObjSpeed(5, 0x10000, 0x8000);
    Vinasu_ObjCommit(5, 0, 16);
    Vinasu_ObjTurn(5, 0x3000, 0);
    Vinasu_Wait(10);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(5, 263, 30);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_ObjVar(9, 2);
    Vinasu_Wait(10);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjModeWait(9, 7);
    Vinasu_Wait(30);
    Vinasu_ObjTurn(8, 0x9000, 0);
    Vinasu_Wait(20);
    Vinasu_Jump(8, 258, 40);
    Vinasu_MesPut(0x2008, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(5, 0x1000, 0);
    Vinasu_Wait(20);
    Vinasu_ObjModeWait(5, 4);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(5, 0x4000, 0);
    Vinasu_Wait(20);
    Vinasu_ObjCommit(5, 0, 16);
    Vinasu_ObjTurn(5, 0x3000, 0);
    Vinasu_Wait(10);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_ObjModeWait(9, 4);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(5, 261, 50);
    Vinasu_ObjVar(9, 2);
    Vinasu_Wait(10);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjModeWait(5, 3);
    Vinasu_Wait(10);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_ObjVar(8, 2);
    Vinasu_Wait(10);
    Vinasu_MesPut(0x2008, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(5, 0, 0);
    Vinasu_Wait(20);
    Vinasu_ObjModeWait(5, 3);
    Vinasu_Wait(20);
    Vinasu_ObjVar(9, 2);
    Vinasu_Wait(10);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(5, 0x3000, 0);
    Vinasu_Wait(20);
    Vinasu_ObjModeWait(5, 3);
    Vinasu_Wait(30);
    Vinasu_Jump(8, 258, 40);
    Vinasu_MesPut(0x2008, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(5, 0, 0);
    Vinasu_Wait(20);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(8, 265, 30);
    Vinasu_MesPut(0x2008, 0);
    Vinasu_Wait(10);
    Vinasu_ObjVar(5, 2);
    Vinasu_Wait(10);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(5, 258, 40);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_ObjModeWait(9, 4);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(8, 261, 30);
    Vinasu_MesPut(0x2008, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(5, 263, 30);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(8, 0xe000, 0);
    Vinasu_Wait(30);
    Vinasu_ObjModeWait(8, 3);
    Vinasu_Wait(10);
    Vinasu_MesPut(0x2008, 0);
    Vinasu_Wait(10);
    Vinasu_ObjVar(5, 2);
    Vinasu_Wait(10);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(8, 0x8000, 0);
    Vinasu_Wait(30);
    Vinasu_ObjCommit(5, 0, -16);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(5, 0xe000, 0);
    Vinasu_Wait(30);
    Vinasu_ObjModeWait(5, 3);
    Vinasu_Wait(20);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(9, 0xe000, 0);
    Vinasu_ObjTurn(8, 0x5000, 0);
    Vinasu_Wait(50);
    Vinasu_ObjTurn(8, 0x9000, 0);
    Vinasu_ObjTurn(9, 0xd000, 0);
    Vinasu_Wait(20);
    Vinasu_ObjTurn(5, 0x1000, 0);
    Vinasu_Wait(30);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(9, 0xb000, 0);
    Vinasu_Wait(20);
    Vinasu_ObjModeWait(9, 4);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(5, 256, 30);
    Vinasu_ObjTurn(5, 0x3000, 0);
    Vinasu_Wait(20);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_ObjModeWait(9, 4);
    Vinasu_MesPut(9, 0);
    Vinasu_ObjVar(5, 2);
    Vinasu_Wait(20);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(9, 0xf000, 0);
    Vinasu_Wait(25);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(8, 0x4000, 0);
    Vinasu_Wait(20);
    Vinasu_ObjModeWait(8, 3);
    Vinasu_Wait(10);
    Vinasu_MesPut(0x2008, 0);
    Vinasu_Wait(10);
    Vinasu_ObjVar(9, 2);
    Vinasu_Wait(10);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(5, 258, 40);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(9, 0xa000, 0);
    Vinasu_Wait(20);
    Vinasu_ObjModeWait(9, 3);
    Vinasu_Wait(10);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjLink(5, 8, 50);
    Vinasu_ObjTurn(9, 0x4000, 0);
    Vinasu_Wait(30);
    Vinasu_Jump(9, 264, 30);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjModeWait(9, 6);
    Vinasu_Wait(30);
    Vinasu_Jump(8, 257, 30);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(8, 0x5000, 0);
    Vinasu_Wait(20);
    Vinasu_MesPut(0x2008, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(9, 256, 30);
    Vinasu_ObjTurn(9, 0xf000, 0);
    Vinasu_Wait(20);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(5, 257, 30);
    Vinasu_ObjTurn(5, 0x4000, 0);
    Vinasu_Wait(20);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_Jump(9, 258, 40);
    Vinasu_ObjTurn(9, 0xb000, 0);
    Vinasu_Wait(30);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjVar(9, 2);
    Vinasu_Wait(10);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(9, 0xf000, 0);
    Vinasu_Wait(10);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjModeWait(8, 3);
    Vinasu_Wait(10);
    Vinasu_MesPut(0x2008, 0);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(9, 0xb000, 0);
    Vinasu_Wait(20);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjModeWait(5, 3);
    Vinasu_Wait(10);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(20);
    Vinasu_ObjModeWait(9, 4);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjVar(9, 2);
    Vinasu_Wait(10);
    Vinasu_MesPut(9, 0);
    Vinasu_Wait(10);
    Vinasu_ObjModeWait(8, 3);
    Vinasu_Wait(20);
    Vinasu_ObjTurn(8, 0x8000, 0);
    Vinasu_Wait(20);
    Vinasu_MesPut(0x2008, 0);
    Vinasu_Wait(10);
    Vinasu_ObjVar(5, 2);
    Vinasu_Wait(10);
    Vinasu_ObjTurn(5, 0x1000, 0);
    Vinasu_Wait(30);
    Vinasu_Jump(5, 261, 30);
    Vinasu_MesPut(5, 0);
    Vinasu_Wait(10);
    Vinasu_ObjModeWait(5, 3);
    Vinasu_Wait(20);
    Vinasu_ObjAnim(9, 3);
    Vinasu_ObjModeWait(8, 3);
    Vinasu_Wait(20);
    Vinasu_ObjSpeed(8, 0x13333, 0x9999);
    Vinasu_ObjAnim(8, 2);
    actor = Engine_ActorGet(5);
    if (actor != NULL)
        ObjectMotion_ResetAndSetPosition(8, actor->x_pixel, actor->z_pixel);
    Vinasu_ObjCommitHere(8);
    Vinasu_ObjSetPos(8, 0, 0);
    Vinasu_ObjSpeed(9, 0x13333, 0x9999);
    Vinasu_ObjAnim(9, 2);
    actor = Engine_ActorGet(5);
    if (actor != NULL)
        ObjectMotion_ResetAndSetPosition(9, actor->x_pixel, actor->z_pixel);
    Vinasu_ObjCommitHere(9);
    Vinasu_ObjSetPos(9, 0, 0);
    Vinasu_Wait(10);
    Vinasu_ObjCommit(5, 16, 0);
    Vinasu_ObjWalk(5, 680, 944);
    Vinasu_Wait(15);
    Vinasu_ObjTurn(5, 0xd000, 0);
    Vinasu_Wait(50);
    Vinasu_ObjTurn(5, 0, 0);
    Vinasu_Wait(15);
    Vinasu_ObjWalk(5, 728, 944);
    Vinasu_ObjWalk(5, 728, 928);
    Vinasu_ObjSetPos(5, 0x2d80000, 0x3900000);
    Vinasu_Se(129);
    Vinasu_ObjEffectSet(5, -1);
    Vinasu_Wait(40);
    Vinasu_CopyCells(53, 50, 42, 49, 1, 1);
    Vinasu_CopyCells(55, 117, 41, 117, 3, 5);
    Vinasu_FlagClr(513);
    Vinasu_ObjSetPos(10, 0x2980000, 0x3180000);
    Vinasu_CamMove(-1, -1, -1, 0);
    gPartyState.current_owner = 5;
    Vinasu_PartyRemove(4);
    Vinasu_Wait(50);
    work = Ram_HeapSlots->event_work;
    work->delay = 60;
    work->screen_effect = 256;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Vinasu_Countdown(22);

}

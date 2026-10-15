#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"

struct VenusActor {
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
struct VenusActor *Engine_ActorGet(s32 id);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void Func_02001ccc(s32 actor, s32 value);
void Engine_MapCopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);
void GameFlag_ClearBit(s32 flag);
void Party_RemoveActiveOwner(s32 owner);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001c54(s32 value);
void Event_StartScreenTransition(void);
void VenusLighthouse_RunSetupStep(void);
void SceneState_RunActor13AtColumn42Setup(void);

static inline void Venus_MessageSet(s32 message)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001c1c(message);
}

static inline void Venus_ObjAttach(s32 actor, s32 target)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Object_AttachWorkTargetToObject(actor, target);
}

static inline void Venus_Wait(s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Engine_EventWait(frames);
}

static inline void Venus_ObjWalk(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndReset(actor, x, z);
}

static inline void Venus_ObjTurn(s32 actor, s32 angle, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void Venus_ObjModeWait(s32 actor, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_SetModeAndWaitAnimation(actor, mode);
}

static inline void Venus_ObjWalk2(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ResetAndSetPositionInMode2(actor, x, z);
}

static inline void Venus_ObjCommitHere(s32 actor)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_CommitCurrentPositionAndActivate(actor);
}

static inline void Venus_Jump(s32 actor, s32 height, s32 frames)
{
    /* FAKEMATCH: the native loads the actor after the height and frames. */
    register s32 h __asm__("r1") = height;
    /* FAKEMATCH: the frames stay fixed in r2 for the same reason. */
    register s32 f __asm__("r2") = frames;

    Func_02001c34(actor, h, f);
}

static inline void Venus_CamMove(s32 x, s32 y, s32 z, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_CamBounds(x, y, z, mode);
}

static inline void Venus_ObjVar(s32 actor, s32 value)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_SetVarCbAndRefresh(actor, value);
}

static inline void Venus_MessagePut(s32 actor, s32 value)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001c24(actor, value);
}

static inline void Venus_Se(s32 cue)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Engine_AudioPlayCue(cue);
}

static inline void Venus_ObjSetPos(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001be4(actor, x, z);
}

static inline void Venus_ObjEffect(s32 actor)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001cd4(actor);
}

static inline void Venus_ObjVariant(s32 actor, s32 value)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetVariantCallback(actor, value);
}

static inline void Venus_ObjLink(s32 first, s32 second, s32 distance)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Object_LinkPair(first, second, distance);
}

static inline void Venus_ObjSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static inline void Venus_ObjFollow(s32 actor, s32 target)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Object_LinkObjectAndSetCallback(actor, target);
}

static inline void Venus_ObjCommit(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_CommitPositionAndActivate(actor, x, z);
}

static inline void Venus_ObjAnim(s32 actor, s32 animation)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Engine_ActorSetAnimation(actor, animation);
}

static inline void Venus_ObjPlace(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ResetAndSetPosition(actor, x, z);
}

static inline void Venus_ObjEffectSet(s32 actor, s32 value)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001ccc(actor, value);
}

static inline void Venus_CopyCells(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Engine_MapCopyCellsTo(src_x, src_y, dest_x, dest_y, width, height);
}

static inline void Venus_FlagClr(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_ClearBit(flag);
}

static inline void Venus_PartyRemove(s32 owner)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Party_RemoveActiveOwner(owner);
}

static inline void Venus_Countdown(s32 value)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001c54(value);
}

void Func_02000b2c(void)
{
    struct EventWork *work;

    Venus_ObjSetPos(4, 0x2c80000, 0x3300000);
    Venus_ObjSetPos(5, 0x2b80000, 0x3480000);
    Venus_ObjSetPos(8, 0x2d80000, 0x3480000);
    Venus_ObjTurn(4, 0x4000, 0);
    Venus_ObjTurn(5, 0xc000, 0);
    Venus_ObjTurn(8, 0xc000, 0);
    Venus_ObjSpeed(4, 0x10000, 0x8000);
    Venus_ObjSpeed(5, 0x10000, 0x8000);
    Venus_ObjSpeed(8, 0x10000, 0x8000);
    Venus_ObjSpeed(9, 0x10000, 0x8000);
    Venus_CamMove(0x2980000, -1, 0x3380000, 0);
    work = Ram_HeapSlots->event_work;
    work->screen_effect = 256;
    work->delay = 60;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Venus_Wait(20);
    Venus_ObjModeWait(4, 3);
    Venus_Wait(20);
    Venus_ObjAnim(5, 3);
    Venus_ObjModeWait(8, 3);
    Venus_Wait(10);
    Venus_ObjWalk(4, 664, 824);
    Venus_ObjTurn(4, 0xc000, 0);
    Venus_Wait(10);
    Venus_ObjWalk(8, 712, 824);
    Venus_ObjTurn(5, 0xa000, 0);
    Venus_Wait(20);
    VenusLighthouse_RunSetupStep();
    SceneState_RunActor13AtColumn42Setup();
    Venus_Wait(20);
    Venus_ObjWalk(8, 728, 840);
    Venus_ObjTurn(8, 0xc000, 0);
    Venus_ObjTurn(5, 0xc000, 0);
    Venus_ObjWalk(4, 712, 816);
    Venus_ObjTurn(4, 0x4000, 0);
    Venus_Wait(30);
    Venus_ObjModeWait(4, 3);
    Venus_Wait(20);
    Venus_ObjAnim(5, 3);
    Venus_ObjModeWait(8, 3);
    Venus_Wait(10);
    Venus_ObjFollow(5, 4);
    Venus_ObjFollow(8, 4);
    Venus_ObjWalk(4, 744, 816);
    Venus_ObjWalk(4, 744, 780);
    Venus_Se(129);
    Venus_ObjEffectSet(4, -1);
    Venus_Wait(50);
    Venus_ObjTurn(5, 0xc000, 0);
    Venus_ObjTurn(8, 0xc000, 0);
    Venus_ObjSetPos(4, 0, 0);
}

void Func_02000d30(void)
{
    struct VenusActor *actor;
    struct EventWork *work;

    Venus_MessageSet((s32)MsgVenusLighthouseAreYouSureWeShouldBeLeaving);
    Venus_ObjAttach(8, 1);
    Func_02001c4c();
    Venus_Wait(30);
    Venus_ObjWalk(5, 664, 840);
    Venus_ObjTurn(5, 0, 0);
    Venus_ObjWalk(8, 696, 840);
    Venus_Wait(20);
    Venus_ObjModeWait(8, 3);
    Venus_Wait(10);
    Venus_ObjModeWait(5, 3);
    Venus_Wait(10);
    Venus_ObjWalk2(8, 696, 920);
    Venus_Wait(10);
    Venus_ObjWalk(5, 664, 888);
    Venus_ObjCommitHere(8);
    Venus_Jump(8, 256, 30);
    Venus_ObjTurn(8, 0xa000, 0);
    Venus_Wait(20);
    Venus_ObjWalk(8, 696, 888);
    Venus_ObjTurn(5, 0, 0);
    Venus_ObjTurn(8, 0x8000, 0);
    Venus_Wait(20);
    Venus_CamMove(0x2b80000, -1, 0x3980000, 1);
    Func_02001c4c();
    Venus_Wait(20);
    Venus_ObjVar(5, 2);
    Venus_Wait(10);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_Jump(8, 257, 50);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_ObjModeWait(8, 3);
    Venus_Wait(20);
    Venus_ObjVar(5, 2);
    Venus_Wait(10);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_Jump(8, 257, 30);
    Venus_MessagePut(0x2008, 0);
    Venus_Wait(10);
    Venus_ObjTurn(5, 0xe000, 0);
    Venus_Wait(30);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_Jump(8, 258, 40);
    Venus_MessagePut(0x2008, 0);
    Venus_Wait(10);
    Venus_ObjModeWait(5, 3);
    Venus_Wait(10);
    Venus_MessagePut(5, 0);
    Venus_Wait(30);
    Venus_Se(198);
    Venus_ObjSetPos(9, 0x2a80000, 0x3980000);
    Venus_ObjTurn(9, 0xb000, 0);
    Venus_ObjEffect(9);
    Venus_ObjTurn(9, 0xb000, 0);
    Venus_Wait(20);
    Venus_ObjTurn(5, 0x4000, 0);
    Venus_ObjTurn(8, 0x4000, 0);
    Venus_Wait(10);
    Venus_Wait(20);
    Venus_ObjModeWait(9, 4);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjVariant(5, 2);
    Venus_ObjVar(8, 2);
    Venus_Wait(20);
    Venus_ObjLink(5, 8, 40);
    Venus_Jump(9, 256, 30);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjTurn(5, 0x4000, 0);
    Venus_ObjTurn(8, 0x4000, 0);
    Venus_Wait(20);
    Venus_Jump(8, 258, 40);
    Venus_MessagePut(0x2008, 0);
    Venus_Wait(10);
    Venus_Jump(9, 257, 30);
    Venus_Wait(10);
    Venus_ObjTurn(9, 0, 0);
    Venus_Wait(30);
    Venus_ObjTurn(9, 0x8000, 0);
    Venus_Wait(30);
    Venus_ObjTurn(9, 0xc000, 0);
    Venus_Wait(30);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjVar(5, 2);
    Venus_Wait(20);
    Venus_ObjSpeed(5, 0x8000, 0x4000);
    Venus_ObjFollow(8, 5);
    Venus_ObjCommit(5, 0, -32);
    Venus_Wait(10);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_Jump(9, 261, 30);
    Venus_ObjCommit(9, -8, -8);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjVar(8, 2);
    Venus_Wait(10);
    Venus_ObjTurn(8, 0x5000, 0);
    Venus_Wait(20);
    Venus_MessagePut(0x2008, 0);
    Venus_Wait(10);
    Venus_Jump(9, 256, 30);
    Venus_ObjTurn(9, 0xe000, 0);
    Venus_Wait(30);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_Jump(9, 257, 30);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_Jump(8, 258, 0);
    Venus_Jump(5, 258, 40);
    Venus_MessagePut(0x2008, 0);
    Venus_Wait(10);
    Venus_Jump(9, 264, 30);
    Venus_ObjTurn(9, 0xf000, 0);
    Venus_Wait(30);
    Venus_MessagePut(9, 0);
    Venus_Wait(20);
    Venus_Jump(5, 262, 40);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_ObjTurn(9, 0xb000, 0);
    Venus_Wait(20);
    Venus_ObjTurn(5, 0x3000, 0);
    Venus_Wait(30);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_Jump(9, 265, 30);
    Venus_Wait(20);
    Venus_ObjVar(9, 2);
    Venus_Wait(10);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjModeWait(9, 4);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjModeWait(5, 4);
    Venus_Wait(10);
    Venus_ObjSpeed(5, 0x10000, 0x8000);
    Venus_ObjCommit(5, 0, 16);
    Venus_ObjTurn(5, 0x3000, 0);
    Venus_Wait(10);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_Jump(5, 263, 30);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_ObjVar(9, 2);
    Venus_Wait(10);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjModeWait(9, 7);
    Venus_Wait(30);
    Venus_ObjTurn(8, 0x9000, 0);
    Venus_Wait(20);
    Venus_Jump(8, 258, 40);
    Venus_MessagePut(0x2008, 0);
    Venus_Wait(10);
    Venus_ObjTurn(5, 0x1000, 0);
    Venus_Wait(20);
    Venus_ObjModeWait(5, 4);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_ObjTurn(5, 0x4000, 0);
    Venus_Wait(20);
    Venus_ObjCommit(5, 0, 16);
    Venus_ObjTurn(5, 0x3000, 0);
    Venus_Wait(10);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_ObjModeWait(9, 4);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_Jump(5, 261, 50);
    Venus_ObjVar(9, 2);
    Venus_Wait(10);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjModeWait(5, 3);
    Venus_Wait(10);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_ObjVar(8, 2);
    Venus_Wait(10);
    Venus_MessagePut(0x2008, 0);
    Venus_Wait(10);
    Venus_ObjTurn(5, 0, 0);
    Venus_Wait(20);
    Venus_ObjModeWait(5, 3);
    Venus_Wait(20);
    Venus_ObjVar(9, 2);
    Venus_Wait(10);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjTurn(5, 0x3000, 0);
    Venus_Wait(20);
    Venus_ObjModeWait(5, 3);
    Venus_Wait(30);
    Venus_Jump(8, 258, 40);
    Venus_MessagePut(0x2008, 0);
    Venus_Wait(10);
    Venus_ObjTurn(5, 0, 0);
    Venus_Wait(20);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_Jump(8, 265, 30);
    Venus_MessagePut(0x2008, 0);
    Venus_Wait(10);
    Venus_ObjVar(5, 2);
    Venus_Wait(10);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_Jump(5, 258, 40);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_ObjModeWait(9, 4);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_Jump(8, 261, 30);
    Venus_MessagePut(0x2008, 0);
    Venus_Wait(10);
    Venus_Jump(5, 263, 30);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_ObjTurn(8, 0xe000, 0);
    Venus_Wait(30);
    Venus_ObjModeWait(8, 3);
    Venus_Wait(10);
    Venus_MessagePut(0x2008, 0);
    Venus_Wait(10);
    Venus_ObjVar(5, 2);
    Venus_Wait(10);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_ObjTurn(8, 0x8000, 0);
    Venus_Wait(30);
    Venus_ObjCommit(5, 0, -16);
    Venus_Wait(10);
    Venus_ObjTurn(5, 0xe000, 0);
    Venus_Wait(30);
    Venus_ObjModeWait(5, 3);
    Venus_Wait(20);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_ObjTurn(9, 0xe000, 0);
    Venus_ObjTurn(8, 0x5000, 0);
    Venus_Wait(50);
    Venus_ObjTurn(8, 0x9000, 0);
    Venus_ObjTurn(9, 0xd000, 0);
    Venus_Wait(20);
    Venus_ObjTurn(5, 0x1000, 0);
    Venus_Wait(30);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_ObjTurn(9, 0xb000, 0);
    Venus_Wait(20);
    Venus_ObjModeWait(9, 4);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_Jump(5, 256, 30);
    Venus_ObjTurn(5, 0x3000, 0);
    Venus_Wait(20);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_ObjModeWait(9, 4);
    Venus_MessagePut(9, 0);
    Venus_ObjVar(5, 2);
    Venus_Wait(20);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_ObjTurn(9, 0xf000, 0);
    Venus_Wait(25);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjTurn(8, 0x4000, 0);
    Venus_Wait(20);
    Venus_ObjModeWait(8, 3);
    Venus_Wait(10);
    Venus_MessagePut(0x2008, 0);
    Venus_Wait(10);
    Venus_ObjVar(9, 2);
    Venus_Wait(10);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_Jump(5, 258, 40);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_ObjTurn(9, 0xa000, 0);
    Venus_Wait(20);
    Venus_ObjModeWait(9, 3);
    Venus_Wait(10);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjLink(5, 8, 50);
    Venus_ObjTurn(9, 0x4000, 0);
    Venus_Wait(30);
    Venus_Jump(9, 264, 30);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjModeWait(9, 6);
    Venus_Wait(30);
    Venus_Jump(8, 257, 30);
    Venus_Wait(10);
    Venus_ObjTurn(8, 0x5000, 0);
    Venus_Wait(20);
    Venus_MessagePut(0x2008, 0);
    Venus_Wait(10);
    Venus_Jump(9, 256, 30);
    Venus_ObjTurn(9, 0xf000, 0);
    Venus_Wait(20);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_Jump(5, 257, 30);
    Venus_ObjTurn(5, 0x4000, 0);
    Venus_Wait(20);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_Jump(9, 258, 40);
    Venus_ObjTurn(9, 0xb000, 0);
    Venus_Wait(30);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjVar(9, 2);
    Venus_Wait(10);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjTurn(9, 0xf000, 0);
    Venus_Wait(10);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjModeWait(8, 3);
    Venus_Wait(10);
    Venus_MessagePut(0x2008, 0);
    Venus_Wait(10);
    Venus_ObjTurn(9, 0xb000, 0);
    Venus_Wait(20);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjModeWait(5, 3);
    Venus_Wait(10);
    Venus_MessagePut(5, 0);
    Venus_Wait(20);
    Venus_ObjModeWait(9, 4);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjVar(9, 2);
    Venus_Wait(10);
    Venus_MessagePut(9, 0);
    Venus_Wait(10);
    Venus_ObjModeWait(8, 3);
    Venus_Wait(20);
    Venus_ObjTurn(8, 0x8000, 0);
    Venus_Wait(20);
    Venus_MessagePut(0x2008, 0);
    Venus_Wait(10);
    Venus_ObjVar(5, 2);
    Venus_Wait(10);
    Venus_ObjTurn(5, 0x1000, 0);
    Venus_Wait(30);
    Venus_Jump(5, 261, 30);
    Venus_MessagePut(5, 0);
    Venus_Wait(10);
    Venus_ObjModeWait(5, 3);
    Venus_Wait(20);
    Venus_ObjAnim(9, 3);
    Venus_ObjModeWait(8, 3);
    Venus_Wait(20);
    Venus_ObjSpeed(8, 0x13333, 0x9999);
    Venus_ObjAnim(8, 2);
    actor = Engine_ActorGet(5);
    if (actor != NULL)
        ObjectMotion_ResetAndSetPosition(8, actor->x_pixel, actor->z_pixel);
    Venus_ObjCommitHere(8);
    Venus_ObjSetPos(8, 0, 0);
    Venus_ObjSpeed(9, 0x13333, 0x9999);
    Venus_ObjAnim(9, 2);
    actor = Engine_ActorGet(5);
    if (actor != NULL)
        ObjectMotion_ResetAndSetPosition(9, actor->x_pixel, actor->z_pixel);
    Venus_ObjCommitHere(9);
    Venus_ObjSetPos(9, 0, 0);
    Venus_Wait(10);
    Venus_ObjCommit(5, 16, 0);
    Venus_ObjWalk(5, 680, 944);
    Venus_Wait(15);
    Venus_ObjTurn(5, 0xd000, 0);
    Venus_Wait(50);
    Venus_ObjTurn(5, 0, 0);
    Venus_Wait(15);
    Venus_ObjWalk(5, 728, 944);
    Venus_ObjWalk(5, 728, 928);
    Venus_ObjSetPos(5, 0x2d80000, 0x3900000);
    Venus_Se(129);
    Venus_ObjEffectSet(5, -1);
    Venus_Wait(40);
    Venus_CopyCells(53, 50, 42, 49, 1, 1);
    Venus_CopyCells(55, 117, 41, 117, 3, 5);
    Venus_FlagClr(513);
    Venus_ObjSetPos(10, 0x2980000, 0x3180000);
    Venus_CamMove(-1, -1, -1, 0);
    gPartyState.current_owner = 5;
    Venus_PartyRemove(4);
    Venus_Wait(50);
    work = Ram_HeapSlots->event_work;
    work->delay = 60;
    work->screen_effect = 256;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Venus_Countdown(22);

}

/* Pin-free Ov694_Jump trial in the return scene: 877 native and built,
 * 847 common, differ 60. Actor loads cross height/frame construction.
 * Height-only removal differs 60; frame-only removal 38; actor-only removal
 * fails the complete TLA comparisons. A one-pass call block closes the
 * small event1 caller but changes this return scene (differ 190). A whole
 * helper block differs 150; one-pass height initialization differs 164;
 * one-pass actor initialization with frame unbound differs 64. All linked
 * helper bindings are retained until every caller and edition matches. */
#ifndef TLA_OVERLAY_694_OV694_H
#define TLA_OVERLAY_694_OV694_H

#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "text/msg_ids.h"

struct Ov694Object;

void Object_LinkObjectAndSetCallback(s32 actor, s32 target);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_020040e4(s32 actor, s32 target);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *script);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *script);
void ObjectMotion_EnableActionAndResetMotion(s32 actor);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Func_0200412c(s32 message);
void Func_02004144(s32 speaker, s32 flags);
void Object_SetModeById(s32 actor, s32 mode);
void Func_0200417c(s32 actor, s32 emote);
void Func_02004174(s32 actor, s32 height, s32 frames);
void Func_0200415c(s32 actor, s32 angle);
void Func_0200418c(s32 speed, s32 acceleration);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void Func_0200413c(s32 actor, s32 a, s32 b);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
void Func_020042b4(s32 cue);
void Func_020040dc(s32 actor, s32 x, s32 z, s32 angle);
void Func_020040d4(s32 actor, s32 x, s32 z);
void Object_SetPartAttribute(struct Ov694Object *object, s32 value);
struct Ov694Object *Object_GetById(s32 id);
void Func_0200406c(void);
void Func_02004074(void);
void Func_02004204(s32 skip);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void WaitFrames(s32 frames);
void ObjectMotion_WaitForAnimationChange(s32 actor);
void Func_0200419c(void);
void Func_02003e54(struct Ov694Object *object, const void *effect);
void Func_02003f54(struct Ov694Object *object);
void Func_0200420c(void);
void Func_02004044(void);
void GameFlag_SetBit(s32 flag);
void Func_02004164(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k);
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 mode);
void Motion_SetVarCbAndRefresh(s32 actor, s32 value);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02001938(s32 mode);
void Func_02001c5c(s32 a, s32 b, s32 c, s32 d);
void Func_02004004(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);
void Func_0200400c(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Func_020041a4(s32 a, s32 x, s32 y, s32 z);
void Func_020041b4(s32 countdown);
void Func_02004214(s32 a, s32 b, s32 c);
void Func_0200423c(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y, s32 mode);

static inline void Ov694_ObjFollow(s32 actor, s32 target)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Object_LinkObjectAndSetCallback(actor, target);
}

static inline void Ov694_Wait(s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Battle_WaitMode0(frames);
}

static inline void Ov694_ObjSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static inline void Ov694_ObjWalk(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndReset(actor, x, z);
}

static inline void Ov694_ObjCommit(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndCommit(actor, x, z);
}

static inline void Ov694_ObjTurn(s32 actor, s32 angle, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void Ov694_ObjMoveTo(s32 actor, s32 target)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_020040e4(actor, target);
}

static inline void Ov694_ObjAction(s32 actor, const void *script)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_EnableActionAndSetCallback(actor, script);
}

static inline void Ov694_ObjActionRefresh(s32 actor, const void *script)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Object_SetActionCallbackAndRefreshById(actor, script);
}

static inline void Ov694_ObjReset(s32 actor)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_EnableActionAndResetMotion(actor);
}

static inline void Ov694_CamMove(s32 x, s32 y, s32 z, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_CamBounds(x, y, z, mode);
}

static inline void Ov694_ObjModeWait(s32 actor, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_SetModeAndWaitAnimation(actor, mode);
}

static inline void Ov694_MesSet(s32 message)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_0200412c(message);
}

static inline void Ov694_MesPut(s32 speaker, s32 flags)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02004144(speaker, flags);
}

static inline void Ov694_ObjMode(s32 actor, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Object_SetModeById(actor, mode);
}

static inline void Ov694_ObjEmote(s32 actor, s32 emote)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_0200417c(actor, emote);
}

static inline void Ov694_Jump(s32 actor, s32 height, s32 frames)
{
    s32 h = height;
    s32 f = frames;
    s32 a = actor;

    Func_02004174(a, h, f);
}

static inline void Ov694_ObjFace(s32 actor, s32 angle)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_0200415c(actor, angle);
}

static inline void Ov694_CamSpeed(s32 speed, s32 acceleration)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_0200418c(speed, acceleration);
}

static inline void Ov694_ObjLaunch(s32 actor, s32 speed, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_Launch(actor, speed, frames);
}

static inline void Ov694_ObjShake(s32 actor, s32 a, s32 b)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_0200413c(actor, a, b);
}

static inline s32 Ov694_MesOpen(s32 speaker, s32 flags)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return UiText_OpenMessageAtObject(speaker, flags);
}

static inline void Ov694_Se(s32 cue)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_020042b4(cue);
}

static inline void Ov694_ObjPlaceTurn(s32 actor, s32 x, s32 z, s32 angle)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_020040dc(actor, x, z, angle);
}

static inline void Ov694_ObjPlace(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_020040d4(actor, x, z);
}

static inline void Ov694_PairShow(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02004164(a, b, c, d, e, f, g, h, i, j, k);
}

static inline void Ov694_ObjVar(s32 actor, s32 value)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_SetVarCbAndRefresh(actor, value);
}

static inline void Ov694_CopyCells(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02004004(src_x, src_y, dest_x, dest_y, width, height);
}

static inline void Ov694_CopyAttrs(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_0200400c(src_x, src_y, width, height, dest_x, dest_y);
}

static inline void Ov694_CamSet(s32 a, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_020041a4(a, x, y, z);
}

static inline void Ov694_Tiles(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_0200423c(src_x, src_y, width, height, dest_x, dest_y, mode);
}

#endif

extern const u8 Data_02004ac0[];
extern const u8 Data_02004b18[];

void Func_02002e18(void)
{
    if (GameFlag_Test(0x9b2)) {
        Func_0200406c();
        Func_02004204(0);
        Ov694_CamSpeed(0xcccc, 0x1999);
        Ov694_CamMove(0xa00000, -1, 0x1700000, 1);
        Ov694_ObjSpeed(gPartyState.current_owner, 0xcccc, 0x6666);
        Ov694_ObjWalk(gPartyState.current_owner, 158, 394);
        Ov694_ObjTurn(8, 0x5000, 0);
        Ov694_ObjMoveTo(2, gPartyState.current_owner);
        WaitFrames(1);
        Ov694_ObjSpeed(2, 0xcccc, 0x6666);
        Ov694_ObjWalk(2, 144, 374);
        Ov694_ObjFace(2, 0xe000);
        Ov694_Jump(8, 265, 20);
        Ov694_MesSet((s32)MsgReturnedToMyNewHome);
        Ov694_MesPut(8, 0);
        Ov694_ObjModeWait(2, 3);
        Ov694_MesPut(0x1002, 0);
        Ov694_ObjMode(2, 4);
        Ov694_MesPut(0x1002, 0);
        Ov694_ObjMode(8, 4);
        Ov694_MesPut(8, 0);
        Ov694_ObjTurn(2, 0xa000, 40);
        Ov694_ObjWalk(2, 180, 364);
        Ov694_Wait(20);
        Ov694_ObjWalk(2, 186, 388);
        Ov694_Wait(40);
        Ov694_ObjWalk(2, 146, 380);
        Ov694_Jump(2, 265, 20);
        Ov694_MesPut(2, 0);
        Ov694_Jump(8, 259, 20);
        Ov694_MesPut(8, 0);
        Ov694_Jump(2, 257, 0);
        Ov694_ObjTurn(2, 0xe000, 40);
        Ov694_MesPut(8, 0);
        Ov694_Jump(2, 256, 0);
        Ov694_MesPut(0x1002, 0);
        Ov694_ObjMode(8, 4);
        Ov694_MesPut(8, 0);
        Ov694_ObjEmote(2, 258);
        Ov694_MesPut(0x1002, 0);
        Ov694_Jump(8, 265, 20);
        Ov694_MesPut(8, 0);
        Ov694_ObjMoveTo(0, gPartyState.current_owner);
        WaitFrames(1);
        Ov694_Jump(0, 256, 0);
        Ov694_ObjSpeed(0, 0x13333, 0x9999);
        Ov694_ObjWalk(0, 168, 380);
        Ov694_ObjFace(0, 0xa000);
        Ov694_MesPut(0, 0);
        Ov694_Jump(8, 256, 0);
        Ov694_ObjTurn(8, 0x3000, 0);
        Ov694_ObjMoveTo(9, gPartyState.current_owner);
        WaitFrames(1);
        Ov694_ObjSpeed(gPartyState.current_owner, 0xcccc, 0x6666);
        Ov694_ObjAction(gPartyState.current_owner, Data_02004ac0);
        Ov694_ObjSpeed(9, 0xcccc, 0x6666);
        Ov694_ObjWalk(9, 182, 378);
        Ov694_ObjWalk(9, 182, 360);
        Ov694_ObjFace(9, 0x8000);
        Ov694_Jump(9, 265, 20);
        Ov694_MesPut(0x8009, 0);
        Ov694_ObjTurn(8, 0, 0);
        Ov694_ObjEmote(9, 258);
        Ov694_MesPut(0x8009, 0);
        Ov694_ObjModeWait(8, 3);
        Ov694_MesPut(0x1008, 0);
        Ov694_Jump(9, 257, 0);
        Ov694_Jump(gPartyState.current_owner, 257, 0);
        Ov694_Jump(0, 257, 0);
        Ov694_Jump(2, 257, 40);
        Ov694_ObjFace(8, 0x5000);
        Ov694_MesPut(8, 0);
        Ov694_MesPut(0x8009, 0);
        Ov694_ObjFace(8, 0);
        Ov694_ObjModeWait(8, 3);
        Ov694_ObjTurn(2, 0xa000, 0);
        Ov694_ObjTurn(gPartyState.current_owner, 0x2000, 40);
        Ov694_ObjTurn(gPartyState.current_owner, 0, 0);
        Ov694_ObjFace(2, 0xe000);
        Ov694_MesPut(0x1002, 0);
        Ov694_Jump(8, 259, 0);
        Ov694_ObjTurn(8, 0x5000, 20);
        Ov694_MesPut(8, 0);
        Ov694_ObjTurn(0, 0xe000, 0);
        Ov694_ObjTurn(9, 0x5000, 40);
        Ov694_ObjTurn(9, 0x8000, 0);
        Ov694_ObjFace(0, 0xa000);
        Ov694_MesPut(0, 0);
        Ov694_ObjEmote(8, 258);
        Ov694_ObjFace(8, 0x3000);
        Ov694_MesPut(0x1008, 0);
        Ov694_Jump(gPartyState.current_owner, 256, 0);
        Ov694_Jump(0, 256, 0);
        Ov694_Jump(2, 256, 0);
        Ov694_Jump(9, 256, 40);
        Ov694_ObjFace(8, 0x5000);
        Ov694_ObjMode(8, 4);
        Ov694_MesPut(8, 0);
        Ov694_ObjTurn(0, 0xe000, 0);
        Ov694_ObjTurn(9, 0x5000, 0);
        Ov694_ObjTurn(2, 0xa000, 0);
        Ov694_ObjTurn(gPartyState.current_owner, 0x2000, 40);
        Ov694_ObjEmote(0, 258);
        Ov694_Wait(20);
        Ov694_ObjFace(0, 0xa000);
        Ov694_MesPut(0, 0);
        Ov694_ObjTurn(gPartyState.current_owner, 0, 0);
        Ov694_ObjTurn(9, 0x8000, 0);
        Ov694_ObjTurn(2, 0xe000, 0);
        Ov694_Jump(8, 265, 0);
        Ov694_ObjTurn(8, 0x3000, 0);
        Ov694_MesPut(0x1008, 0);
        Ov694_ObjVar(2, 1);
        Ov694_MesPut(0x1002, 0);
        Ov694_ObjFace(8, 0x5000);
        Ov694_MesPut(8, 0);
        Ov694_Jump(9, 261, 20);
        Ov694_ObjTurn(0, 0xe000, 0);
        Ov694_ObjTurn(8, 0, 80);
        Ov694_Jump(9, 261, 40);
        Ov694_Jump(0, 257, 0);
        Ov694_Jump(gPartyState.current_owner, 257, 0);
        Ov694_Jump(2, 257, 40);
        Ov694_Jump(9, 262, 0);
        Ov694_MesPut(0x8009, 0);
        Ov694_MesPut(0x1000, 0);
        Ov694_Jump(9, 264, 0);
        Ov694_ObjTurn(9, 0x5000, 20);
        Ov694_MesPut(9, 0);
        Ov694_ObjLaunch(2, 2, 10);
        Ov694_MesPut(0x1002, 0);
        Ov694_ObjFace(9, 0x5000);
        Ov694_ObjModeWait(9, 3);
        Ov694_MesOpen(9, 0);
        Ov694_Jump(2, 257, 0);
        Ov694_ObjTurn(2, 0xa000, 0);
        if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
            Ov694_ObjFace(2, 0xe000);
            Ov694_ObjMode(2, 3);
            Ov694_MesPut(0x1002, 0);
            ((struct EventWork *)Ram_HeapSlots->event_work)->message_id++;
        } else {
            Ov694_ObjFace(2, 0xe000);
            ((struct EventWork *)Ram_HeapSlots->event_work)->message_id++;
            Ov694_ObjMode(2, 4);
            Ov694_MesPut(0x1002, 0);
        }
        Ov694_ObjFace(9, 0x5000);
        Ov694_ObjMode(9, 4);
        Ov694_MesOpen(9, 0);
        Ov694_Jump(0, 257, 0);
        Ov694_ObjTurn(0, 0xa000, 0);
        if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
            Ov694_ObjFace(0, 0xe000);
            Ov694_ObjMode(0, 3);
            Ov694_MesPut(0x1000, 0);
            ((struct EventWork *)Ram_HeapSlots->event_work)->message_id++;
        } else {
            Ov694_ObjFace(0, 0xe000);
            ((struct EventWork *)Ram_HeapSlots->event_work)->message_id++;
            Ov694_ObjMode(0, 4);
            Ov694_MesPut(0x1000, 0);
        }
        Ov694_ObjModeWait(9, 3);
        Ov694_MesPut(9, 0);
        Ov694_Jump(9, 265, 0);
        Ov694_ObjFace(9, 0x8000);
        Ov694_MesPut(9, 0);
        Ov694_ObjEmote(2, 258);
        Ov694_MesPut(0x1002, 0);
        Ov694_ObjFace(9, 0x5000);
        Ov694_ObjMode(9, 3);
        Ov694_MesPut(9, 0);
        Ov694_ObjMode(0, 3);
        Ov694_MesPut(0x1000, 0);
        Ov694_Jump(8, 257, 20);
        Ov694_MesPut(0x1008, 0);
        Ov694_ObjFace(9, 0x8000);
        Ov694_ObjMode(9, 3);
        Ov694_MesPut(0x8009, 0);
        Ov694_ObjFace(9, 0x5000);
        Ov694_Jump(9, 265, 0);
        Ov694_MesPut(9, 0);
        Ov694_ObjFace(0, 0xa000);
        Ov694_MesPut(0, 0);
        Ov694_ObjEmote(2, 258);
        Ov694_ObjFace(2, 0xa000);
        Ov694_MesPut(2, 0);
        Ov694_ObjFace(8, 0x5000);
        Ov694_ObjModeWait(8, 3);
        Ov694_MesPut(8, 0);
        Ov694_ObjFace(9, 0x8000);
        Ov694_ObjMode(9, 3);
        Ov694_MesPut(9, 0);
        Ov694_ObjModeWait(gPartyState.current_owner, 3);
        Ov694_ObjSpeed(2, 0x13333, 0x9999);
        Ov694_ObjSpeed(0, 0x13333, 0x9999);
        Ov694_ObjSpeed(9, 0x13333, 0x9999);
        Ov694_ObjAction(2, Data_02004b18);
        Ov694_ObjAction(0, Data_02004b18);
        Ov694_ObjActionRefresh(9, Data_02004b18);
        GameFlag_SetBit(2513);
        Func_02004074();
    }
}

/* Pin-free trial (tla-en): listing 1701 instructions, draft 1699, common 1645, differ 110.
 * Earlier register-allocation notes describe the prior pinned form.
 */
/* Near miss (28 of 1,701 instructions differ, all from two yes/no
 * prompts): after the prompt the native reloads the leader into r5 and
 * copies it to r0 in each branch, keeping the answer in r0; the draft moves
 * the answer to r3 and loads the leader straight into r0. The rest are
 * literal pool offsets that follow from it. */
#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "text/msg_ids.h"

struct Ov694Object;

extern const u8 Data_020048a0[];
extern const u8 Data_020048f8[];
extern const u8 Data_0200493c[];
extern const u8 Data_02004980[];
extern const u8 Data_020049c4[];
extern const u8 Data_02006030[];

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
    /* FAKEMATCH: the native loads the actor after the height and frames. */
    s32 h = height;
    /* FAKEMATCH: the frames stay fixed in r2 for the same reason. */
    s32 f = frames;
    /* FAKEMATCH: and the actor last, in r0. */
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

void Func_02001d50(void)
{
    struct Ov694Object *object;
    s32 answer;
    s32 owner;
    s32 angle;

    object = Object_GetById(8);
    Func_0200406c();
    Func_02004204(0);
    Ov694_Se(78);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Ov694_ObjFollow(0, gPartyState.current_owner);
    Ov694_ObjFollow(1, gPartyState.current_owner);
    Ov694_ObjFollow(3, gPartyState.current_owner);
    Ov694_ObjFollow(2, gPartyState.current_owner);
    Ov694_ObjFollow(8, gPartyState.current_owner);
    Ov694_Wait(10);
    Ov694_ObjSpeed(gPartyState.current_owner, 0xcccc, 0x6666);
    Ov694_ObjWalk(gPartyState.current_owner, 160, 382);
    Ov694_ObjTurn(gPartyState.current_owner, 0x8000, 0);
    Ov694_ObjMoveTo(5, gPartyState.current_owner);
    Ov694_ObjMoveTo(7, gPartyState.current_owner);
    Ov694_ObjMoveTo(6, gPartyState.current_owner);
    Ov694_ObjMoveTo(9, gPartyState.current_owner);
    WaitFrames(1);
    Ov694_ObjSpeed(5, 0xcccc, 0x6666);
    Ov694_ObjSpeed(7, 0xcccc, 0x6666);
    Ov694_ObjSpeed(9, 0xcccc, 0x6666);
    Ov694_ObjSpeed(6, 0xcccc, 0x6666);
    Ov694_ObjAction(5, Data_020048a0);
    Ov694_ObjAction(9, Data_02004980);
    Ov694_ObjAction(6, Data_0200493c);
    Ov694_ObjActionRefresh(7, Data_020048f8);
    Ov694_ObjReset(gPartyState.current_owner);
    Ov694_ObjReset(1);
    Ov694_ObjReset(3);
    Ov694_ObjReset(2);
    Ov694_ObjReset(8);
    Ov694_CamMove(-1, -1, -1, 0);
    Ov694_Se(43);
    Ov694_Wait(20);
    Ov694_ObjModeWait(0, 3);
    Ov694_MesSet((s32)MsgGladYouKeptYourPromise);
    Ov694_MesPut(0, 0);
    Ov694_ObjMode(1, 3);
    Ov694_MesPut(1, 0);
    Ov694_ObjEmote(5, 258);
    Ov694_Wait(20);
    Ov694_MesPut(5, 0);
    Ov694_ObjMode(6, 3);
    Ov694_MesPut(6, 0);
    Ov694_ObjModeWait(3, 3);
    Ov694_MesPut(3, 0);
    Ov694_ObjMode(2, 4);
    Ov694_MesPut(2, 0);
    Ov694_ObjEmote(7, 258);
    Ov694_MesPut(7, 0);
    Ov694_Jump(0, 259, 0);
    Ov694_MesPut(0, 0);
    Ov694_ObjModeWait(9, 3);
    Ov694_MesPut(9, 0);
    Ov694_ObjTurn(0, 0xc000, 0);
    Ov694_ObjFace(1, 0x4000);
    Ov694_CamSpeed(0xcccc, 0x1999);
    Ov694_CamMove(0x940000, -1, 0x1700000, 1);
    Ov694_ObjWalk(9, 172, 342);
    Ov694_ObjTurn(0, 0xe000, 0);
    Ov694_ObjTurn(1, 0xe000, 0);
    Ov694_ObjTurn(3, 0xe000, 0);
    Ov694_ObjTurn(2, 0xe000, 0);
    Ov694_ObjTurn(5, 0xa000, 0);
    Ov694_ObjTurn(6, 0xa000, 0);
    Ov694_ObjWalk(9, 146, 342);
    Ov694_ObjFace(9, 0x5000);
    Ov694_ObjMode(9, 4);
    Ov694_MesPut(9, 0);
    Ov694_ObjLaunch(1, 2, 10);
    Ov694_MesPut(0x8001, 0);
    Ov694_MesPut(9, 0);
    Ov694_ObjMode(3, 4);
    Ov694_MesPut(0x8003, 0);
    Ov694_MesPut(2, 0);
    Ov694_Jump(5, 261, 20);
    Ov694_ObjFace(5, 0x6000);
    Ov694_MesPut(5, 0);
    Ov694_ObjTurn(1, 0, 0);
    Ov694_ObjTurn(0, 0, 0);
    Ov694_Jump(0, 256, 0);
    Ov694_Jump(1, 256, 40);
    Ov694_MesPut(0x4001, 0);
    Ov694_ObjModeWait(9, 4);
    Ov694_MesPut(9, 0);
    Ov694_ObjTurn(0, 0xc000, 0);
    Ov694_ObjFace(1, 0x4000);
    Ov694_ObjFace(5, 0x8000);
    Ov694_ObjMode(5, 4);
    Ov694_MesPut(5, 0);
    Ov694_ObjTurn(1, 0, 0);
    Ov694_ObjTurn(0, 0, 0);
    Ov694_Jump(1, 256, 0);
    Ov694_Jump(0, 256, 40);
    Ov694_MesPut(9, 0);
    Ov694_ObjModeWait(5, 3);
    Ov694_MesPut(5, 0);
    Ov694_ObjMode(1, 3);
    Ov694_ObjModeWait(0, 3);
    Ov694_ObjMode(9, 4);
    Ov694_MesPut(9, 0);
    Ov694_Jump(0, 265, 20);
    Ov694_ObjTurn(0, 0xe000, 0);
    Ov694_ObjShake(0, 0, 40);
    Ov694_Jump(2, 257, 20);
    Ov694_MesPut(2, 0);
    Ov694_Jump(3, 256, 0);
    Ov694_ObjFace(3, 0x4000);
    Ov694_MesPut(3, 0);
    Ov694_ObjFace(2, 0xc000);
    Ov694_ObjModeWait(2, 3);
    Ov694_ObjFace(2, 0xe000);
    Ov694_ObjTurn(0, 0, 0);
    Ov694_Jump(0, 256, 20);
    Ov694_MesPut(0, 0);
    Ov694_ObjTurn(3, 0, 0);
    Ov694_Jump(1, 257, 0);
    Ov694_ObjFace(1, 0xe000);
    Ov694_ObjModeWait(9, 3);
    Ov694_MesPut(9, 0);
    Ov694_Jump(3, 257, 20);
    Ov694_ObjFace(3, 0x2000);
    Ov694_MesOpen(3, 0);
    Ov694_ObjTurn(1, 0x2000, 0);
    Ov694_ObjTurn(5, 0x4000, 0);
    Ov694_ObjTurn(7, 0x6000, 0);
    Ov694_ObjTurn(9, 0x3000, 0);
    answer = Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0);
    owner = gPartyState.current_owner;
    if (answer == 0) {
        ObjectMotion_WaitForAnimationChange(owner);
        Ov694_ObjMode(2, 3);
        Ov694_MesPut(2, 0);
        ((struct EventWork *)Ram_HeapSlots->event_work)->message_id++;
    } else {
        ObjectMotion_WaitForAnimationChange(owner);
        ((struct EventWork *)Ram_HeapSlots->event_work)->message_id++;
        Ov694_ObjMode(2, 4);
        Ov694_MesPut(2, 0);
    }
    Ov694_ObjFace(9, 0x5000);
    Ov694_ObjModeWait(9, 4);
    Ov694_MesPut(9, 0);
    Ov694_ObjTurn(gPartyState.current_owner, 0x8000, 0);
    Ov694_ObjTurn(5, 0xa000, 0);
    Ov694_ObjTurn(7, 0x8000, 0);
    Ov694_ObjTurn(3, 0, 0);
    Ov694_ObjTurn(0, 0xc000, 0);
    Ov694_ObjFace(1, 0x4000);
    Ov694_Jump(9, 265, 20);
    Ov694_MesPut(9, 0);
    Ov694_Jump(0, 256, 0);
    Ov694_ObjTurn(1, 0xe000, 0);
    Ov694_ObjTurn(0, 0xe000, 20);
    Ov694_MesPut(0, 0);
    Ov694_ObjFace(7, 0x6000);
    Ov694_ObjMode(7, 4);
    Ov694_MesPut(7, 0);
    Ov694_ObjTurn(0, 0, 0);
    Ov694_ObjTurn(1, 0, 0);
    Ov694_ObjTurn(2, 0, 0);
    Ov694_Jump(0, 256, 0);
    Ov694_Jump(1, 256, 0);
    Ov694_Jump(3, 256, 0);
    Ov694_Jump(2, 256, 20);
    Ov694_ObjFace(6, 0x8000);
    Ov694_ObjModeWait(6, 3);
    Ov694_MesPut(6, 0);
    Ov694_ObjTurn(1, 0x4000, 0);
    Ov694_ObjTurn(0, 0xc000, 20);
    Ov694_Jump(3, 257, 20);
    Ov694_MesPut(3, 0);
    Ov694_ObjTurn(1, 0xe000, 0);
    Ov694_ObjTurn(0, 0xe000, 0);
    Ov694_Jump(9, 265, 40);
    Ov694_MesPut(9, 0);
    Ov694_ObjEmote(1, 258);
    Ov694_Wait(20);
    Ov694_MesPut(1, 0);
    Ov694_ObjModeWait(9, 3);
    Ov694_ObjShake(9, 0, 80);
    Ov694_MesPut(0, 0);
    Ov694_ObjMode(9, 4);
    Ov694_MesPut(9, 0);
    Ov694_MesPut(9, 0);
    Ov694_ObjFace(1, 0x2000);
    Ov694_MesOpen(0x4001, 0);
    Ov694_ObjTurn(gPartyState.current_owner, 0xa000, 0);
    Ov694_ObjTurn(5, 0x4000, 0);
    Ov694_ObjTurn(9, 0x3000, 0);
    answer = Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0);
    owner = gPartyState.current_owner;
    if (answer == 0) {
        ObjectMotion_WaitForAnimationChange(owner);
        Ov694_ObjEmote(1, 258);
        Ov694_MesPut(0x4001, 0);
        ((struct EventWork *)Ram_HeapSlots->event_work)->message_id++;
    } else {
        ObjectMotion_WaitForAnimationChange(owner);
        Ov694_Jump(1, 264, 20);
        ((struct EventWork *)Ram_HeapSlots->event_work)->message_id++;
        Ov694_MesPut(0x4001, 0);
    }
    Ov694_ObjTurn(5, 0x8000, 0);
    Ov694_ObjTurn(0, 0, 0);
    Ov694_Jump(0, 265, 20);
    Ov694_MesPut(0, 0);
    Ov694_ObjFace(9, 0x5000);
    Ov694_ObjMode(9, 4);
    Ov694_MesPut(9, 0);
    Ov694_ObjTurn(0, 0xe000, 0);
    Ov694_ObjTurn(1, 0xe000, 0);
    Ov694_Jump(0, 261, 0);
    Ov694_Jump(1, 261, 20);
    Ov694_Se(0);
    Object_SetPartAttribute(Object_GetById(8), 15);
    angle = 0xb000;
    Func_020040dc(8, 0xa00000, 0x1d20000, angle);
    WaitFrames(1);
    Ov694_MesPut(8, 0);
    Ov694_ObjFollow(gPartyState.current_owner, 8);
    Ov694_ObjFollow(5, 8);
    Ov694_ObjFollow(7, 8);
    Ov694_ObjFollow(6, 8);
    Ov694_ObjFollow(0, 8);
    Ov694_ObjFollow(1, 8);
    Ov694_ObjFollow(3, 8);
    Ov694_ObjFollow(2, 8);
    Object_SetPartAttribute(Object_GetById(8), 0);
    Ov694_CamSpeed(0x26666, 0x4ccc);
    Ov694_CamMove(0xb40000, -1, 0x19a0000, 1);
    Func_0200419c();
    Ov694_CamSpeed(0xcccc, 0x1999);
    Ov694_CamMove(0x940000, -1, 0x1700000, 1);
    Ov694_ObjSpeed(8, 0xcccc, 0x6666);
    Ov694_ObjWalk(8, 146, 406);
    Ov694_Jump(3, 264, 20);
    Ov694_MesPut(3, 0);
    Ov694_ObjReset(gPartyState.current_owner);
    Ov694_ObjReset(5);
    Ov694_ObjReset(7);
    Ov694_ObjReset(6);
    Ov694_ObjReset(0);
    Ov694_ObjReset(1);
    Ov694_ObjReset(3);
    Ov694_ObjReset(2);
    Ov694_Se(705);
    Ov694_ObjModeWait(8, 3);
    Ov694_MesPut(8, 0);
    Ov694_ObjMode(gPartyState.current_owner, 3);
    Ov694_ObjMode(5, 3);
    Ov694_ObjMode(7, 3);
    Ov694_ObjMode(6, 3);
    Ov694_ObjMode(0, 3);
    Ov694_ObjMode(1, 3);
    Ov694_ObjMode(3, 3);
    Ov694_ObjModeWait(2, 3);
    Ov694_Wait(20);
    Ov694_Jump(9, 257, 20);
    Ov694_MesPut(9, 0);
    Ov694_ObjFace(0, 0xe000);
    Ov694_ObjMode(0, 3);
    Ov694_Wait(10);
    Ov694_PairShow(0, 5, 5, 8, 10, 8, 9, 20, 18, 10, 10);
    Ov694_ObjTurn(0, 0x2000, 0);
    Ov694_Jump(gPartyState.current_owner, 256, 0);
    Ov694_Jump(5, 256, 0);
    Ov694_Jump(7, 256, 0);
    Ov694_Jump(6, 256, 0);
    Ov694_Jump(0, 256, 0);
    Ov694_Jump(1, 256, 0);
    Ov694_Jump(3, 256, 0);
    Ov694_Jump(2, 256, 0);
    Ov694_Jump(9, 256, 40);
    Ov694_ObjMode(8, 3);
    Ov694_MesPut(8, 0);
    Ov694_ObjEmote(1, 258);
    Ov694_Wait(20);
    Ov694_MesPut(0x4001, 0);
    Ov694_MesPut(3, 0);
    Ov694_ObjFace(6, 0x8000);
    Ov694_MesPut(6, 0);
    Ov694_Jump(8, 265, 0);
    Ov694_ObjFace(8, 0);
    Ov694_MesPut(0x1008, 0);
    Ov694_ObjSpeed(2, 0xcccc, 0x6666);
    Ov694_ObjWalk(2, 130, 400);
    Ov694_ObjTurn(2, 0, 0);
    Ov694_ObjFace(8, 0x8000);
    Ov694_MesPut(0x4008, 0);
    Ov694_Jump(2, 256, 20);
    Ov694_ObjMode(8, 4);
    Ov694_MesPut(0x4008, 0);
    Ov694_ObjTurn(0, 0xe000, 0);
    Ov694_ObjTurn(1, 0xa000, 0);
    Ov694_ObjTurn(3, 0, 0);
    Ov694_ObjTurn(gPartyState.current_owner, 0xc000, 0);
    Ov694_ObjTurn(5, 0x4000, 0);
    Ov694_ObjTurn(7, 0x4000, 0);
    Ov694_ObjTurn(6, 0xc000, 40);
    Ov694_ObjFace(8, 0xd000);
    Ov694_MesPut(8, 0);
    Ov694_ObjTurn(0, 0x2000, 0);
    Ov694_ObjTurn(1, 0x2000, 0);
    Ov694_ObjTurn(3, 0x2000, 0);
    Ov694_ObjTurn(gPartyState.current_owner, 0x6000, 0);
    Ov694_ObjTurn(5, 0x4000, 0);
    Ov694_ObjTurn(7, 0x6000, 0);
    Ov694_ObjTurn(6, 0x6000, 0);
    Ov694_ObjMode(8, 4);
    Ov694_MesPut(8, 0);
    Ov694_ObjShake(5, 0, 10);
    Func_02003e54(object, Data_02006030);
    Ov694_Wait(80);
    Func_02003f54(object);
    Ov694_ObjModeWait(8, 4);
    Ov694_MesPut(8, 0);
    Ov694_ObjEmote(7, 258);
    Ov694_ObjShake(7, 0, 10);
    Func_02003e54(object, Data_02006030);
    Ov694_Wait(80);
    Func_02003f54(object);
    Ov694_ObjFace(8, 0xd000);
    Ov694_ObjMode(8, 4);
    Ov694_MesPut(8, 0);
    Ov694_ObjEmote(0, 258);
    Ov694_ObjEmote(1, 258);
    Ov694_ObjEmote(3, 258);
    Ov694_ObjEmote(gPartyState.current_owner, 258);
    Ov694_ObjEmote(5, 258);
    Ov694_ObjEmote(7, 258);
    Ov694_ObjEmote(6, 258);
    Ov694_ObjEmote(2, 258);
    Ov694_Wait(20);
    Func_0200415c(8, angle);
    Ov694_ObjMode(8, 4);
    Ov694_MesPut(8, 0);
    Ov694_ObjTurn(0, 0, 0);
    Ov694_ObjTurn(1, 0, 0);
    Ov694_ObjTurn(3, 0, 0);
    Ov694_ObjTurn(2, 0xe000, 0);
    Ov694_ObjTurn(gPartyState.current_owner, 0x8000, 0);
    Ov694_ObjTurn(5, 0x8000, 0);
    Ov694_ObjTurn(6, 0x8000, 0);
    Ov694_ObjTurn(7, 0x8000, 0);
    Ov694_ObjLaunch(9, 4, 10);
    Ov694_MesPut(9, 0);
    Ov694_ObjMode(0, 3);
    Ov694_ObjMode(1, 3);
    Ov694_ObjMode(3, 3);
    Ov694_ObjMode(2, 3);
    Ov694_ObjMode(gPartyState.current_owner, 3);
    Ov694_ObjMode(5, 3);
    Ov694_ObjMode(6, 3);
    Ov694_ObjMode(7, 3);
    Ov694_Jump(8, 264, 20);
    Ov694_MesPut(8, 0);
    Ov694_MesPut(8, 0);
    Ov694_ObjMode(8, 3);
    Ov694_MesPut(8, 0);
    Ov694_Se(78);
    Ov694_CamMove(0xb40000, -1, 0x19a0000, 1);
    Ov694_ObjWalk(8, 160, 466);
    Ov694_ObjPlace(8, 0, 0);
    Ov694_CamSpeed(0x26666, 0x4ccc);
    Ov694_CamMove(0x940000, -1, 0x1700000, 1);
    Func_0200419c();
    Func_0200420c();
    Ov694_Jump(5, 265, 20);
    Ov694_MesPut(5, 0);
    Ov694_ObjTurn(1, 0x4000, 0);
    Ov694_ObjTurn(0, 0xc000, 20);
    Ov694_ObjTurn(1, 0, 0);
    Ov694_ObjFace(0, 0);
    Ov694_ObjMode(1, 3);
    Ov694_ObjModeWait(0, 3);
    Ov694_ObjLaunch(5, 2, 0);
    Ov694_ObjFace(5, 0x4000);
    Ov694_ObjMode(5, 3);
    Ov694_MesPut(5, 0);
    Ov694_ObjFace(6, 0xa000);
    Ov694_ObjEmote(6, 258);
    Ov694_MesPut(6, 0);
    Ov694_ObjTurn(5, 0x2000, 0);
    Ov694_Jump(3, 264, 20);
    Ov694_MesPut(3, 0);
    Ov694_ObjTurn(5, 0x8000, 0);
    Ov694_ObjMode(7, 3);
    Ov694_MesPut(7, 0);
    Ov694_Jump(1, 265, 20);
    Ov694_MesPut(0x4001, 0);
    Ov694_ObjFace(0, 0xc000);
    Ov694_ObjMode(0, 3);
    Ov694_MesPut(0, 0);
    Ov694_Jump(5, 264, 0);
    Ov694_ObjTurn(5, 0x4000, 20);
    Ov694_MesOpen(5, 0);
    Ov694_ObjTurn(gPartyState.current_owner, 0xc000, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
        Ov694_ObjFace(9, 0x3000);
        Ov694_ObjMode(9, 3);
        Ov694_MesPut(0x1009, 0);
        ((struct EventWork *)Ram_HeapSlots->event_work)->message_id++;
    } else {
        Ov694_ObjFace(9, 0x3000);
        ((struct EventWork *)Ram_HeapSlots->event_work)->message_id++;
        Ov694_ObjMode(9, 4);
        Ov694_MesPut(0x1009, 0);
    }
    Ov694_ObjWalk(9, 146, 368);
    Ov694_ObjTurn(9, 0x8000, 40);
    Ov694_ObjTurn(9, 0, 40);
    Ov694_ObjFace(9, 0x5000);
    Ov694_MesPut(0x6009, 0);
    Ov694_ObjMode(0, 3);
    Ov694_ObjMode(1, 3);
    Ov694_ObjMode(3, 3);
    Ov694_ObjMode(2, 3);
    Ov694_ObjMode(gPartyState.current_owner, 3);
    Ov694_ObjMode(5, 3);
    Ov694_ObjMode(6, 3);
    Ov694_ObjModeWait(7, 3);
    Ov694_ObjAction(0, Data_020049c4);
    Ov694_ObjAction(1, Data_020049c4);
    Ov694_ObjAction(3, Data_020049c4);
    Ov694_ObjAction(2, Data_020049c4);
    Ov694_ObjAction(9, Data_020049c4);
    Ov694_ObjAction(5, Data_020049c4);
    Ov694_ObjAction(6, Data_020049c4);
    Ov694_ObjActionRefresh(7, Data_020049c4);
    Func_02004044();
    GameFlag_SetBit(2512);
    Func_02004074();
}

// Near miss, differ 65 of 714 (tla-en, 2026-11-06; the grow and spin loops as while loops in place of labels, the same code): cse1 puts the stores of 0
// to velocityY and terrainHeight in the flag test result (reg 62, known 0 on
// the then-arm through the skipped gInput block), so it lives in r8 and shifts
// r8, r9, fp and the frame; with those two stores probed as nonzero it is
// differ 28. 6 November, plain tries: GameFlag_Test direct 99; i = Flag_IsSet,
// if (i == 0) 96; the same through owner 66; count = 0 at entry 133; a do-while
// grow loop and a chained store both 65. 5 November: differ 65 of 714 with x
// read first as the native does (46 with the false order). What turns the rest
// is the zero: after the test of 0x16e the then-arm knows the result is 0 and
// cse keeps it in r8 for the later stores of 0 (reg 62), where the native has
// movs r3, #0. A <= 0 test gives 28 but is not the native's beq. An empty asm
// or one-pass after the test does not stop it. Draft, differ 46 of 714 (tla-en,
// 2026-11-05). Older notes below may predate this figure. Under -Os, 5
// November: differ 46 of 714 (tla-en); older figures below predate it. Draft,
// differ 57 of 714, skeleton gap 10 (tla-en, 2026-11-01) The Venus Djinni
// (actor 9) joins on the world map and offers its tutorial, as The Broken
// Seal's WorldMap_MeetFlint; the second visit offers it again. owner is set
// only on the first path: the native reads r7 uninitialized on the second, so
// owner is live from entry and conflicts with everything. Plain shape, all
// measured here: no count = 0 at entry; u16 stores go through a pointer cast (a
// member store's bit-field path leaves a zero pseudo that CSE reuses across
// calls); the first motionFlags store takes an s32 local before its address,
// the later two stay member stores (their zero is the native's r2); wait(1)
// comes before i--; ShowEmote and SetMoveTarget are The Broken Seal's
// forwarding inlines (Actor_ShowEmote, FieldActor_SetMoveTarget); the +1 and +2
// arms each end with Func_02004cf0, so both jumps cross-jump; Djinn_AddToOwner,
// DjinnRecovery_AddStandby and FieldMessage_ShowAtActor return values.
// Func_02004d08's s32 return is what ordering asks for, its main-image type
// unchecked. The long store is an alias spelling: the native has no store
// dependence between the motionFlags strb and velocityY. Left, one cause: the
// inline's return pseudo is known 0 after the flag test, and cse2 reuses it for
// velocityY = 0, costing r5 and the frame. Plain tries failed: goto, switch,
// u32 compares, a one-pass block (cse2 ignores its loop end), a zero local, an
// inline for the zero stores. Two devices were taken back out because neither
// reaches differ 0; with them this draft is differ 10: `flag = 0x16e; flag =
// GameFlag_Test(flag); if (flag == 0) {` with `flag = Flag_IsSet(0x175);`
// before the MsgHey2 message: 20, temporary. `flags = &motionFlags; *flags =
// two;`, an unread `flags = (u8 *)djinni;` beside the zero stores, `flags =
// &djinni->motionFlags;` after them and
// `*flags = 0;` at the third store: 10, dead; this puts native's late mov sl.
// The last 10 of that form: strb [r2, r6] where native keeps adds r2 and mov
// sl, r2 (reload_combine fires; a second use of r2 would stop it), and at
// MsgSoWeGo and MsgHmmmmDoYou the pool load before muls: sched2 ranks muls 45
// over the load 44 (4 + 41 against 2 + 1 + 41). An asm or one-pass fixes these
// but flips `adds r1, r7` at Func_02004c98. That ranking is K2's
// -mthumb-slow-loads model, a compiler question for Pascal, out of scope here.
#include "types.h"
#include "call.h"
#include "main_routine_work.h"
#include "actor.h"
#include "flag_call.h"

extern u32 gInput;
extern char MsgDjinnGiveTutorialA[];
extern char MsgDjinnGiveTutorialB[];
extern char MsgHey2[];
extern char MsgWellYouWeren[];
extern char MsgSeeYouKnow[];
extern char MsgEvenIKnow[];
extern char MsgOhAllRight[];
extern char MsgOhComeOn[];
extern char MsgIMSo[];
extern char MsgTheVenusDjinni[];
extern char MsgSeeWeDjinn[];
extern char MsgSoWeGo[];
extern char MsgHmmmmDoYou[];
extern char MsgWellThatS[];

void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Party_RemoveActiveOwner(s32 owner);
void Party_AddActiveOwner(s32 owner);
s32 Djinn_AddToOwner(s32 owner, s32 element, s32 djinni);
u32 *DjinnRecovery_AddStandby(u32 owner, u32 element, u32 djinni);
void Actor_SetAngleToward(s32 id, s32 target, s32 mode);
void Actor_Jump(s32 id, s32 height, s32 frames);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Motion_SetVarCbAndRefresh(s32 id, s32 value);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
s32 FieldMessage_ShowAtActor(s32 id, s32 mode);
void FieldMessage_Show(s32 message, s32 mode);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 mode);
void Menu_AnimateSelectionToEntry(s32 a, s32 b);
void Func_020011fc(void);
void Func_02004b50(struct Actor *object, s32 x, s32 y, s32 z);
static inline void SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z) {
  Func_02004b50(object, x, y, z);
}
void Func_02004ba8(s32 a, s32 b);
void Func_02004bf8(s32 value);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004c98(s32 id, s32 target);
void Func_02004cd8(s32 message);
void Func_02004cf0(s32 id, s32 mode);
s32 Func_02004d08(s32 id, s32 animation, s32 frames);

static inline void ShowEmote(s32 id, s32 animation, s32 frames) {
  Func_02004d08(id, animation, frames);
}
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004db8(s32 value);
void Func_02004dc0(void);
void Func_02004dd0(s32 id, s32 element, s32 djinni);
void Func_02004df8(s32 a, s32 b);
void Func_02004e00(void);
void Func_02004e28(void);
void Func_02004e30(s32 cue);

/// A tutorial message: the second set (from MsgDjinnGiveTutorialB) while flag
/// 0x175 is set.
#define TUTORIAL_MESSAGE(base)                                                 \
  (Flag_IsSet(0x175) *                                                         \
       ((s32)MsgDjinnGiveTutorialB - (s32)MsgDjinnGiveTutorialA) +             \
   (s32)(base))

void Func_02001e34(void) {
  struct Actor *djinni;
  struct Actor *leader;
  s32 x;
  s32 z;
  s32 owner;
  s32 count;
  s32 i;

  djinni = Actor_GetById(9);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  x = (leader->x - 0x27880000) / 2 + 0x27880000;
  z = (leader->z - 0x20a40000) / 2 + 0x20a40000;
  if (Flag_IsSet(0x16e) == 0) {
    if (gInput & 64)
      Flag_Set(0x175);
    Func_02004c20();
    Func_02004db8(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Party_RemoveActiveOwner(0);
    Party_RemoveActiveOwner(1);
    Party_RemoveActiveOwner(2);
    Party_RemoveActiveOwner(3);
    Party_RemoveActiveOwner(4);
    Party_RemoveActiveOwner(5);
    Party_RemoveActiveOwner(6);
    Party_RemoveActiveOwner(7);
    Party_AddActiveOwner(4);
    Party_AddActiveOwner(5);
    Party_AddActiveOwner(6);
    owner = gMainRoutineWork.currentOwner;
    Func_02004bf8(1);
    Flag_Set(0x16e);
    Func_02004c20();
    Func_02004db8(0);
    Djinn_AddToOwner(owner, 0, 7);
    DjinnRecovery_AddStandby(owner, 0, 7);
    Func_02004da8();
    Actor_SetAngleToward(owner, 9, 0);
    FieldEvent_Wait(10);
    ShowEmote(owner, 0x101, 60);
    {
      s32 one = 1;
      *(u16 *)&djinni->unk66 = one;
    }
    Actor_SetAngleToward(9, owner, 0);
    wait(16);
    Func_02004cd8(TUTORIAL_MESSAGE(MsgHey2));
    i = 15;
    Func_02004cf0(9, 0);
    Func_02004db0();
    Func_02004df8(0x13333, 6);
    Func_02004e00();
    Func_02004da8();
    {
      s32 two = 2;
      *(u8 *)&djinni->motionFlags = two;
    }
    djinni->gravity = 0x4000;
    djinni->speedLimit = 0x10000;
    djinni->acceleration = 0x10000;
    {
      s32 zero = 0;
      djinni->velocityY = zero;
      djinni->terrainHeight = zero;
    }
    SetMoveTarget(djinni, 0x27880000, 0, 0x20a40000);
    while (i >= 0) {
      djinni->scaleX += 0x800;
      djinni->scaleY += 0x800;
      wait(1);
      i--;
    }
    Actor_SetAngleToward(9, owner, 0);
    Actor_SetAngleToward(owner, 9, 0);
    wait(16);
    djinni->update = 0;
    Animation_ApplyChildValues(djinni, 0);
    djinni->gravity = 0x10000;
    FieldMessage_ShowAtActor(9, 0);
    if (Event_AskYesNoWithActorAnimation(owner, 0) == 1) {
      Actor_Jump(9, 2, 20);
      Actor_Jump(9, 2, 20);
      Func_02004cd8(TUTORIAL_MESSAGE(MsgWellYouWeren));
      FieldMessage_ShowAtActor(9, 0);
      if (Event_AskYesNoWithActorAnimation(owner, 0) == 1) {
        Func_02004cd8(TUTORIAL_MESSAGE(MsgWellYouWeren + 1));
        Func_02004cf0(9, 0);
      } else {
        Func_02004cd8(TUTORIAL_MESSAGE(MsgWellYouWeren + 2));
        Func_02004cf0(9, 0);
      }
    } else {
      Func_02004cd8(TUTORIAL_MESSAGE(MsgSeeYouKnow));
      Func_02004cf0(9, 0);
    }
    ShowEmote(owner, 0x100, 30);
    Actor_Jump(9, 2, 20);
    Actor_Jump(9, 2, 20);
    Func_02004cd8(TUTORIAL_MESSAGE(MsgEvenIKnow));
    i = 15;
    Func_02004cf0(9, 0);
    ShowEmote(owner, 0x102, 30);
    Func_02004cf0(9, 0);
    ShowEmote(owner, 0x100, 30);
    Func_02004cf0(9, 0);
    Motion_SetVarCbAndRefresh(owner, 2);
    Func_02004cf0(9, 0);
    Actor_Jump(9, 2, 30);
    Func_02004cf0(9, 0);
    SetMoveTarget(djinni, x, 0x100000, z);
    djinni->motionFlags = 0;
    while (i >= 0) {
      *(u16 *)&djinni->angle += 0x1000;
      wait(1);
      i--;
    }
    Actor_SetModeById(owner, 1);
    Func_02004cf0(9, 0);
    djinni->motionFlags = 2;
    *(long *)&djinni->velocityY = 0;
    djinni->terrainHeight = 0;
    i = 7;
    while (i >= 0) {
      *(u16 *)&djinni->angle += 0x1000;
      wait(1);
      i--;
    }
    Actor_SetModeById(owner, 22);
    Func_02004cf0(9, 0);
    Func_02004d08(9, 0x102, 30);
    Actor_SetAngleToward(9, owner, 0);
    Motion_SetVarCbAndRefresh(9, 2);
    Func_02004cf0(9, 0);
    Actor_Jump(9, 2, 30);
    FieldMessage_ShowAtActor(9, 0);
    count = 0;
  plead:
    {
      i = Event_AskYesNoWithActorAnimation(owner, 0);

      if (count == 7) {
        if (i == 0) {
          Actor_Jump(9, 2, 20);
          Actor_Jump(9, 2, 20);
          Func_02004cd8(TUTORIAL_MESSAGE(MsgOhAllRight));
          Func_02004cf0(9, 0);
          goto joined;
        }
      } else if (i == 1) {
        Actor_Jump(9, 2, 20);
        Actor_Jump(9, 2, 20);
        Func_02004cd8(count + TUTORIAL_MESSAGE(MsgOhComeOn));
        FieldMessage_ShowAtActor(9, 0);
        count++;
        goto plead;
      }
    }
    Actor_SetModeById(owner, 22);
    Actor_Jump(9, 2, 20);
    Actor_Jump(9, 4, 20);
    Func_02004cd8(TUTORIAL_MESSAGE(MsgIMSo));
    Func_02004cf0(9, 0);
  joined:
    Func_02004ba8(0x133, 4);
    Func_02004e30(81);
    FieldMessage_Show(TUTORIAL_MESSAGE(MsgTheVenusDjinni), 3);
    Func_02004cd8(TUTORIAL_MESSAGE(MsgTheVenusDjinni + 1));
    Actor_Jump(9, 2, 20);
    FieldMessage_ShowAtActor(9, 0);
    if (Event_AskYesNoWithActorAnimation(owner, 0) == 1)
      goto refused;
    if (Flag_IsSet(0x175))
      Func_02004e30(17);
    else
      Func_02004e30(35);
    goto tutorial;
  }
  Func_02004c20();
  Func_02004db8(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004c98(9, owner);
  djinni->velocityY = 0xa0000;
  SetMoveTarget(djinni, x, 0, z);
  FieldEvent_Wait(30);
  Func_02004da8();
  Actor_SetAngleToward(9, owner, 0);
  Actor_SetAngleToward(owner, 9, 0);
  Actor_SetModeById(owner, 22);
  Func_02004cd8(TUTORIAL_MESSAGE(MsgSeeWeDjinn));
  Actor_Jump(9, 2, 20);
  Actor_Jump(9, 2, 20);
  Func_02004cf0(9, 0);
  Motion_SetVarCbAndRefresh(9, 2);
  Func_02004cf0(9, 0);
  Func_02004e30(111);
  Menu_AnimateSelectionToEntry(0, 2);
  Flag_Set(0x16f);
  Flag_Clear(0x171);
  Func_02004e28();
  Func_02004cd8(TUTORIAL_MESSAGE(MsgSoWeGo));
  SetMoveTarget(djinni, 0x27880000, 0, 0x20a40000);
  FieldEvent_Wait(30);
  Func_02004cf0(9, 0);
  Actor_SetAngleToward(9, owner, 0);
  Func_02004cf0(9, 0);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(owner, 0) != 1)
    goto refused;
  Actor_SetModeById(owner, 22);
  Motion_SetVarCbAndRefresh(9, 2);
  Func_02004cd8(TUTORIAL_MESSAGE(MsgHmmmmDoYou));
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(owner, 0) == 1)
    goto refused;
  Func_02004cf0(9, 0);
  Actor_SetPositionAndReset(9, x >> 16, z >> 16);
tutorial:
  Func_020011fc();
  Func_02004db0();
  return;
refused:
  Actor_SetModeById(owner, 22);
  Func_02004cd8(TUTORIAL_MESSAGE(MsgWellThatS));
  Actor_Jump(9, 2, 20);
  Actor_Jump(9, 2, 20);
  Func_02004cf0(9, 0);
  Func_02004d08(9, 0x100, 30);
  Func_02004cf0(9, 0);
  Flag_Set(0x16f);
  Flag_Set(0x171);
  Func_02004e28();
  Actor_Jump(9, 2, 20);
  Func_02004cf0(9, 0);
  Func_02004db0();
  Func_02004dd0(9, 0, 7);
  Func_02004dc0();
  Func_02004c28();
  Flag_Clear(0x16e);
  Flag_Clear(0x16f);
  Flag_Clear(0x171);
}

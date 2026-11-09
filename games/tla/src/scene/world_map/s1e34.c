#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "flag_call.h"
#include "event_inline.h"

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

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Party_RemoveActiveOwner(s32 owner);
void Party_AddActiveOwner(s32 owner);
s32 Djinn_AddToOwner(s32 owner, s32 element, s32 djinni);
u32 *DjinnRecovery_AddStandby(u32 owner, u32 element, u32 djinni);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Menu_AnimateSelectionToEntry(s32 a, s32 b);
void Func_020011fc(void);
static inline void SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z) {
  Actor_SetMoveTarget(object, x, y, z);
}
void Func_02004ba8(s32 a, s32 b);
void Func_02004bf8(s32 value);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004c98(s32 id, s32 target);

static inline void ShowEmote(s32 id, s32 animation, s32 frames) {
  Func_080d47b4(id, animation, frames);
}
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004dc0(void);
void Func_02004e00(void);
void Func_02004e28(void);

/// A tutorial message: the second set (from MsgDjinnGiveTutorialB) when
/// secondSet is 1.
#define TUTORIAL_SET_MESSAGE(secondSet, base)                                  \
  ((secondSet) * ((s32)MsgDjinnGiveTutorialB - (s32)MsgDjinnGiveTutorialA) +   \
   (s32)(base))

/// A tutorial message: the second set while flag 0x175 is set.
#define TUTORIAL_MESSAGE(base) TUTORIAL_SET_MESSAGE(Flag_IsSet(0x175), base)

void Func_02001e34(void) {
  struct Actor *djinni;
  struct Actor *leader;
  s32 x;
  s32 z;
  s32 owner;
  s32 count;
  s32 i;
  s32 flag;
  // Holds 0 for its whole life; see its one use.
  s32 zero = 0;

  djinni = Actor_GetById(9);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  x = (leader->x - 0x27880000) / 2 + 0x27880000;
  z = (leader->z - 0x20a40000) / 2 + 0x20a40000;
  /* FAKEMATCH(temporary): flag holds the id 0x16e, then the test's result,
   * then flag 0x175's result for MsgHey2, so the test's known zero dies before
   * the stores of 0 to velocityY and terrainHeight; as
   * `if (Flag_IsSet(0x16e) == 0)` with TUTORIAL_MESSAGE(MsgHey2), 53 of 714
   * instructions differ in tla-en. */
  flag = 0x16e;
  flag = GameFlag_Test(flag);
  if (flag == 0) {
    if (gInput & 64)
      Flag_Set(0x175);
    Func_02004c20();
    Event_ResolvePendingActions(0);
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
    Event_ResolvePendingActions(0);
    Djinn_AddToOwner(owner, 0, 7);
    DjinnRecovery_AddStandby(owner, 0, 7);
    Func_02004da8();
    Actor_SetAngleToward(owner, 9, 0);
    Event_Wait(10);
    ShowEmote(owner, 0x101, 60);
    {
      s32 one = 1;
      *(u16 *)&djinni->unk66 = one;
    }
    Actor_SetAngleToward(9, owner, 0);
    wait(16);
    flag = Flag_IsSet(0x175);
    Event_SetMessageId(TUTORIAL_SET_MESSAGE(flag, MsgHey2));
    i = 15;
    Event_ShowMessageAndWait(9, 0);
    Func_02004db0();
    Func_080d46a4(0x13333, 6);
    Func_02004e00();
    Func_02004da8();
    djinni->motionFlags = 2;
    djinni->gravity = 0x4000;
    djinni->speedLimit = 0x10000;
    djinni->acceleration = 0x10000;
    djinni->velocityY = 0;
    djinni->terrainHeight = 0;
    /* FAKEMATCH(barrier): an empty asm that reads and writes all of djinni,
     * so sched2 takes the two stores of 0 before the copy of the motionFlags
     * address into sl; as no asm, 2 of 714 instructions differ in tla-en. */
    asm("" : "+m"(*djinni));
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
      Event_SetMessageId(TUTORIAL_MESSAGE(MsgWellYouWeren));
      FieldMessage_ShowAtActor(9, 0);
      if (Event_AskYesNoWithActorAnimation(owner, 0) == 1) {
        Event_SetMessageId(TUTORIAL_MESSAGE(MsgWellYouWeren + 1));
        Event_ShowMessageAndWait(9, 0);
      } else {
        Event_SetMessageId(TUTORIAL_MESSAGE(MsgWellYouWeren + 2));
        Event_ShowMessageAndWait(9, 0);
      }
    } else {
      Event_SetMessageId(TUTORIAL_MESSAGE(MsgSeeYouKnow));
      Event_ShowMessageAndWait(9, 0);
    }
    ShowEmote(owner, 0x100, 30);
    Actor_Jump(9, 2, 20);
    Actor_Jump(9, 2, 20);
    Event_SetMessageId(TUTORIAL_MESSAGE(MsgEvenIKnow));
    i = 15;
    Event_ShowMessageAndWait(9, 0);
    ShowEmote(owner, 0x102, 30);
    Event_ShowMessageAndWait(9, 0);
    ShowEmote(owner, 0x100, 30);
    Event_ShowMessageAndWait(9, 0);
    Motion_SetVarCbAndRefresh(owner, 2);
    Event_ShowMessageAndWait(9, 0);
    Actor_Jump(9, 2, 30);
    Event_ShowMessageAndWait(9, 0);
    djinni->motionFlags = 0;
    SetMoveTarget(djinni, x, 0x100000, z);
    while (i >= 0) {
      *(u16 *)&djinni->angle += 0x1000;
      wait(1);
      i--;
    }
    Actor_SetModeById(owner, 1);
    Event_ShowMessageAndWait(9, 0);
    djinni->motionFlags = 2;
    djinni->velocityY = 0;
    djinni->terrainHeight = 0;
    i = 7;
    while (i >= 0) {
      *(u16 *)&djinni->angle += 0x1000;
      wait(1);
      i--;
    }
    Actor_SetModeById(owner, 22);
    Event_ShowMessageAndWait(9, 0);
    Func_080d47b4(9, 0x102, 30);
    Actor_SetAngleToward(9, owner, 0);
    Motion_SetVarCbAndRefresh(9, 2);
    Event_ShowMessageAndWait(9, 0);
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
          Event_SetMessageId(TUTORIAL_MESSAGE(MsgOhAllRight));
          Event_ShowMessageAndWait(9, 0);
          goto joined;
        }
      } else if (i == 1) {
        Actor_Jump(9, 2, 20);
        Actor_Jump(9, 2, 20);
        Event_SetMessageId(count + TUTORIAL_MESSAGE(MsgOhComeOn));
        FieldMessage_ShowAtActor(9, 0);
        count++;
        goto plead;
      }
    }
    Actor_SetModeById(owner, 22);
    Actor_Jump(9, 2, 20);
    Actor_Jump(9, 4, 20);
    Event_SetMessageId(TUTORIAL_MESSAGE(MsgIMSo));
    Event_ShowMessageAndWait(9, 0);
  joined:
    Func_02004ba8(0x133, 4);
    Sound_Play(81);
    FieldMessage_Show(TUTORIAL_MESSAGE(MsgTheVenusDjinni), 3);
    Event_SetMessageId(TUTORIAL_MESSAGE(MsgTheVenusDjinni + 1));
    Actor_Jump(9, 2, 20);
    FieldMessage_ShowAtActor(9, 0);
    if (Event_AskYesNoWithActorAnimation(owner, 0) == 1)
      goto refused;
    if (Flag_IsSet(0x175))
      Sound_Play(17);
    else
      Sound_Play(35);
    goto tutorial;
  }
  Func_02004c20();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004c98(9, owner);
  djinni->velocityY = 0xa0000;
  SetMoveTarget(djinni, x, 0, z);
  Event_Wait(30);
  Func_02004da8();
  Actor_SetAngleToward(9, owner, 0);
  Actor_SetAngleToward(owner, 9, 0);
  Actor_SetModeById(owner, 22);
  Event_SetMessageId(TUTORIAL_MESSAGE(MsgSeeWeDjinn));
  Actor_Jump(9, 2, 20);
  Actor_Jump(9, 2, 20);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_ShowMessageAndWait(9, 0);
  Sound_Play(111);
  Menu_AnimateSelectionToEntry(0, 2);
  Flag_Set(0x16f);
  Flag_Clear(0x171);
  Func_02004e28();
  Event_SetMessageId(TUTORIAL_MESSAGE(MsgSoWeGo));
  SetMoveTarget(djinni, 0x27880000, 0, 0x20a40000);
  Event_Wait(30);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetAngleToward(9, owner, 0);
  Event_ShowMessageAndWait(9, 0);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(owner, 0) != 1)
    goto refused;
  Actor_SetModeById(owner, 22);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_SetMessageId(TUTORIAL_MESSAGE(MsgHmmmmDoYou));
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(owner, 0) == 1)
    goto refused;
  Event_ShowMessageAndWait(9, 0);
  Actor_WalkToAndWait(9, x >> 16, z >> 16);
tutorial:
  Func_020011fc();
  Func_02004db0();
  return;
refused:
  Actor_SetModeById(owner, 22);
  Event_SetMessageId(TUTORIAL_MESSAGE(MsgWellThatS));
  Actor_Jump(9, 2, 20);
  Actor_Jump(9, 2, 20);
  Event_ShowMessageAndWait(9, 0);
  Func_080d47b4(9, 0x100, 30);
  Event_ShowMessageAndWait(9, 0);
  Flag_Set(0x16f);
  Flag_Set(0x171);
  Func_02004e28();
  Actor_Jump(9, 2, 20);
  Event_ShowMessageAndWait(9, 0);
  Func_02004db0();
  // The constant local: with a plain 0 here, the zero that motionFlags = 2
  // leaves is shared by the first visit's stores of 0 and its reloads take
  // other registers (differ 26).
  Djinn_JoinParty(9, zero, 7);
  Func_02004dc0();
  Func_02004c28();
  Flag_Clear(0x16e);
  Flag_Clear(0x16f);
  Flag_Clear(0x171);
}

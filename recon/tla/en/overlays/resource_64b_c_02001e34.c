// Draft, differ 309 of 714: the Venus Djinni (actor 9) joins on the world
// map and offers its tutorial, as The Broken Seal's EventScript_MeetFlint;
// the second visit offers it again. owner is read only on the first path, as
// the native reads r7 uninitialized on the second. Left: register allocation
// (the native keeps the Djinni in r6, the owner in r7 and the two home
// coordinates in r9 and r8) and the order of the coordinate setup.
#include "types.h"
#include "sequence_parent_work.h"
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
void Djinn_AddToOwner(s32 owner, s32 element, s32 djinni);
void DjinnRecovery_AddStandby(s32 owner, s32 element, s32 djinni);
void Actor_SetAngleToward(s32 id, s32 target, s32 mode);
void Actor_Jump(s32 id, s32 height, s32 frames);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Motion_SetVarCbAndRefresh(s32 id, s32 value);
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void FieldMessage_ShowAtActor(s32 id, s32 mode);
void FieldMessage_Show(s32 message, s32 mode);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 mode);
void Menu_AnimateSelectionToEntry(s32 a, s32 b);
void Func_020011fc(void);
void Func_02004b50(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02004ba8(s32 a, s32 b);
void Func_02004bf8(s32 value);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004c98(s32 id, s32 target);
void Func_02004cd8(s32 message);
void Func_02004cf0(s32 id, s32 mode);
void Func_02004d08(s32 id, s32 animation, s32 frames);
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
  struct Actor *djinni = Actor_GetById(9);
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);
  s32 x = (leader->x - 0x27880000) / 2 + 0x27880000;
  s32 z = (leader->z - 0x20a40000) / 2 + 0x20a40000;
  s32 owner;
  s32 i;

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
    owner = gSequenceParentWork.currentOwner;
    Func_02004bf8(1);
    Flag_Set(0x16e);
    Func_02004c20();
    Func_02004db8(0);
    Djinn_AddToOwner(owner, 0, 7);
    DjinnRecovery_AddStandby(owner, 0, 7);
    Func_02004da8();
    Actor_SetAngleToward(owner, 9, 0);
    FieldEvent_Wait(10);
    Func_02004d08(owner, 0x101, 60);
    djinni->unk66 = 1;
    Actor_SetAngleToward(9, owner, 0);
    wait(16);
    Func_02004cd8(TUTORIAL_MESSAGE(MsgHey2));
    Func_02004cf0(9, 0);
    Func_02004db0();
    Func_02004df8(0x13333, 6);
    Func_02004e00();
    Func_02004da8();
    djinni->motionFlags = 2;
    djinni->gravity = 0x4000;
    djinni->speedLimit = 0x10000;
    djinni->acceleration = 0x10000;
    djinni->velocityY = 0;
    djinni->terrainHeight = 0;
    Func_02004b50(djinni, 0x27880000, 0, 0x20a40000);
    for (i = 15; i >= 0; i--) {
      djinni->scaleX += 0x800;
      djinni->scaleY += 0x800;
      wait(1);
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
      if (Event_AskYesNoWithActorAnimation(owner, 0) == 1)
        Func_02004cd8(TUTORIAL_MESSAGE(MsgWellYouWeren + 1));
      else
        Func_02004cd8(TUTORIAL_MESSAGE(MsgWellYouWeren + 2));
      Func_02004cf0(9, 0);
    } else {
      Func_02004cd8(TUTORIAL_MESSAGE(MsgSeeYouKnow));
      Func_02004cf0(9, 0);
    }
    Func_02004d08(owner, 0x100, 30);
    Actor_Jump(9, 2, 20);
    Actor_Jump(9, 2, 20);
    Func_02004cd8(TUTORIAL_MESSAGE(MsgEvenIKnow));
    Func_02004cf0(9, 0);
    Func_02004d08(owner, 0x102, 30);
    Func_02004cf0(9, 0);
    Func_02004d08(owner, 0x100, 30);
    Func_02004cf0(9, 0);
    Motion_SetVarCbAndRefresh(owner, 2);
    Func_02004cf0(9, 0);
    Actor_Jump(9, 2, 30);
    Func_02004cf0(9, 0);
    djinni->motionFlags = 0;
    Func_02004b50(djinni, x, 0x100000, z);
    for (i = 15; i >= 0; i--) {
      djinni->angle += 0x1000;
      wait(1);
    }
    Actor_SetModeById(owner, 1);
    Func_02004cf0(9, 0);
    djinni->motionFlags = 2;
    djinni->velocityY = 0;
    djinni->terrainHeight = 0;
    for (i = 7; i >= 0; i--) {
      djinni->angle += 0x1000;
      wait(1);
    }
    Actor_SetModeById(owner, 22);
    Func_02004cf0(9, 0);
    Func_02004d08(9, 0x102, 30);
    Actor_SetAngleToward(9, owner, 0);
    Motion_SetVarCbAndRefresh(9, 2);
    Func_02004cf0(9, 0);
    Actor_Jump(9, 2, 30);
    FieldMessage_ShowAtActor(9, 0);
    i = 0;
  plead:
    {
      s32 answer = Event_AskYesNoWithActorAnimation(owner, 0);

      if (i == 7) {
        if (answer == 0) {
          Actor_Jump(9, 2, 20);
          Actor_Jump(9, 2, 20);
          Func_02004cd8(TUTORIAL_MESSAGE(MsgOhAllRight));
          Func_02004cf0(9, 0);
          goto joined;
        }
      } else if (answer == 1) {
        Actor_Jump(9, 2, 20);
        Actor_Jump(9, 2, 20);
        Func_02004cd8(i + TUTORIAL_MESSAGE(MsgOhComeOn));
        FieldMessage_ShowAtActor(9, 0);
        i++;
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
  Func_02004b50(djinni, x, 0, z);
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
  Func_02004b50(djinni, 0x27880000, 0, 0x20a40000);
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

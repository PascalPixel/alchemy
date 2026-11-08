#include "mars_lighthouse_aerie.h"
#include "scene_tables.h"

extern const u8 MsgWillYouCast[];

void Func_02003658(void);
void Func_0200567c(s32 id, s32 a1, s32 a2);
void GameFlag_SetBit(s32 flag);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
void wait(s32 frames);

void Func_02004bc4(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 14 of 39 instructions
   * differ in tla-en. */
  Func_0200560c();
  Func_02005784(0);
  Func_02005704(19, FACING_SOUTH + FACING_STEP);
  Func_020056dc(MsgDoNotLet);
  Func_020056f4(SPLIT_CONSTANT(0x8000, 19), 0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner,
                           SPLIT_CONSTANT(0xcc00, 204),
                           SPLIT_CONSTANT(0x6600, 102));
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 480, 348);
  MarsLighthouseAerie_ObjTurn(19, FACING_NORTHWEST + FACING_STEP);
  Func_02005614();
}

void Func_02004c2c(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 14 of 39 instructions
   * differ in tla-en. */
  Func_0200560c();
  Func_02005784(0);
  Func_02005704(19, FACING_SOUTHEAST + FACING_STEP);
  Func_020056dc(MsgDoNotLet);
  Func_020056f4(SPLIT_CONSTANT(0x8000, 19), 0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner,
                           SPLIT_CONSTANT(0xcc00, 204),
                           SPLIT_CONSTANT(0x6600, 102));
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 544, 348);
  MarsLighthouseAerie_ObjTurn(19, FACING_NORTHWEST + FACING_STEP);
  Func_02005614();
}

void Func_02004c94(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 45 instructions
   * differ in tla-en. */
  Func_0200560c();
  Func_02005784(0);
  Func_0200567c(18, PIXELS(512), PIXELS(232));
  wait(1);
  Func_020056dc(MsgWillYouCast);
  FieldMessage_ShowAtActor(18, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Func_0200567c(18, 0, 0);
    wait(1);
    Func_02003658();
  }
  Func_0200567c(18, 0, 0);
  wait(1);
  GameFlag_SetBit(SPLIT_CONSTANT(0x200, 2));
  Func_02005614();
}

#include "local.h"

struct MapCell {
  u8 unk0[2];
  u8 unk2;
  u8 unk3;
};

struct MapWork170 {
  u8 unk000[0x170];
  struct MapCell *cells;
};

void Func_02001c8c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 priority = ((struct FieldSprite *)leader->animation)->priority;

  Actor_SetSpritePriority(13, priority);
  Actor_SetSpritePriority(14, priority);
  Actor_SetSpritePriority(15, priority);
}

void Func_02001cc4(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct MapWork170 *map = (struct MapWork170 *)Ram_HeapSlots->mapWork;

  struct MapCell *cell = &map->cells[Func_020053b0()];

  if (cell->unk2 != 0) {
    leader->y = 0;
    leader->terrainHeight = 0;
  }
}

void Func_02001d00(void) {
  Func_020051c0();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)&MsgKandoreanTemple10, 1);
  Func_020051c8();
}

void Func_02001d20(void) {
  Func_020051c0();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)&MsgKandoreanTemple01, 1);
  Func_020051c8();
}

void Func_02001d40(void) {
  Func_020051c0();
  Event_ResolvePendingActions(0);
  Actor_Stop(9);
  wait(1);
  Actor_SetAngleToward(9, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)&MsgKandoreanTemple02);
  Event_AskYesNoAndShowResponse(9, 0);
  Actor_GetById(9)->angle = FACING_SOUTHEAST + FACING_STEP;
  wait(1);
  Actor_SetMovementTypeById(9, gData_02005514);
  wait(1);
  Func_020051c8();
  GameFlag_SetBit(0x845);
}

void Func_02001db4(void) {
  Func_020051c0();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(10, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)&MsgKandoreanTemple03);
  Event_ShowMessageAndWait(10, 0);
  Actor_GetById(10)->angle = FACING_SOUTH + FACING_STEP;
  wait(1);
  Func_020051c8();
  GameFlag_SetBit(0x846);
}

void Func_02001e0c(void) {
  Func_020051c0();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgKandoreanTemple09);
  Event_AskYesNoAndShowResponse(13, 0);
  Func_020051c8();
}

void Func_02001e30(void) {
  Func_080d47b4(9, 0x100, 0);
  Func_02001d40();
}

void Func_02001e44(void) {
  Func_080d47b4(10, 0x100, 0);
  Func_02001db4();
}

void Func_02001e58(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;
  struct MainRoutineWork *party;
  u8 *flags;
  s32 zero;

  if (z == 5) {
    if (x == 9 && (gInput.held & 16) == 0)
      return;
    if (x == 11 && (gInput.held & 32) == 0)
      return;
  } else if (x == 10 && z == 6 && (gInput.held & 64) == 0) {
    return;
  }
  Func_020051c0();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(temporary): the flags address and the zero held in locals, with
   * gMainRoutineWork's address taken in the block below, so the byte store
   * follows the address load; as leader->motionFlags = 0, which stores first,
   * 2 of 92 instructions differ in tla-en. */
  flags = &leader->motionFlags;
  zero = 0;
  /* FAKEMATCH(dead): gMainRoutineWork's address taken in a once-only block; as
   * a plain statement, tla-en does not match. */
  do {
    party = &gMainRoutineWork;
  } while (0);
  *flags = zero;
  Actor_SetSpeedParameters(party->currentOwner, 0x10000, 0x8000);
  Actor_SetPositionAndReset(party->currentOwner, 168, 80);
  Actor_ArmCallbackAndRefresh(party->currentOwner, FACING_NORTH);
  Actor_SetSingleChildFlags(Actor_GetById(party->currentOwner), 0);
  Actor_SetModeById(party->currentOwner, 13);
  Actor_SetMoveTarget(leader, PIXELS(168), PIXELS(-12), PIXELS(80));
  Script_WaitForEventTimeout(leader);
  Actor_SetModeById(party->currentOwner, 10);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(8);
}

#include "../../../../games/tla/src/scene/lemurian_ship/lemurian_ship.h"
#include "event_inline.h"


void Func_02000870(void) {
  /* FAKEMATCH(wrapper): Map_SetScreenShake through LemurianShip_SetWork; as a direct
   * call, 54 of 260 instructions differ in tla-en. */
  struct LemurianShipObject *leader;
  struct LemurianShipObject *helper;
  s32 opened;
  u8 *state;

  if (!GameFlag_Test(0x983)) {
    leader = ((struct LemurianShipObject *)Actor_GetById(gMainRoutineWork.currentOwner));
    Func_02002c68();
    Event_ResolvePendingActions(0);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 840, 72);
    opened = GameFlag_Test(0x932);
    state = &leader->field22;
    if (opened) {
      Sound_Play(243);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      MapGrid_SetShake(0, 0x20000, 0x10000);
      Event_Wait(20);
      MapGrid_SetShake(0, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      *state = 2;
      Event_Wait(20);
      Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
      Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 840, 88);
      Event_Wait(20);
      Sound_Play(232);
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      Event_Wait(20);
      *state = 1;
    } else {
      helper = ((struct LemurianShipObject *)Actor_GetById(9));
      Actor_SetSpritePriority(9, 2);
      Actor_SetSpeed(9, 0x26666, 0x13333);
      ((struct LemurianShipObject *)Actor_GetById(9))->unk5a[0] &= ~1;
      Actor_WalkToAndWait(9, 824, 88);
      Event_Wait(1);
      {
        /* FAKEMATCH(temporary): the bit in a local the byte is or'ed into; as
         * unk5a[0] |= 1, 47 of 260 instructions differ in tla-en. */
        u8 *bits = &((struct LemurianShipObject *)Actor_GetById(9))->unk5a[0];
        s32 value = 1;

        value |= *bits;
        *bits = value;
      }
      Sound_Play(243);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      MapGrid_SetShake(0, 0x20000, 0x10000);
      Event_Wait(20);
      MapGrid_SetShake(0, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      { s32 two = 2;
      *state = two;
      Event_Wait(20);
      Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x6000, 20);
      Actor_Jump(gMainRoutineWork.currentOwner, 4, 0);
      Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
      Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 840, 88);
      Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x8000);
      Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 9);
      helper->field48 = 0x10000;
      helper->field22 = two; }
      Actor_Jump(9, 4, 0);
      Actor_MoveToAndWait(9, 832, 74);
      Actor_Jump(9, 4, 0);
      Actor_MoveToAndWait(9, 856, 88);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
      Event_Wait(20);
      Sound_Play(232);
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      Event_Wait(20);
      Actor_Stop(gMainRoutineWork.currentOwner);
      *state = 1;
      helper->field22 = 0;
      wait(1);
      GameFlag_ClearBit(0x201);
    }
    Func_02002c70();
  }
}

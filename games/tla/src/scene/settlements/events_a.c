#include "local.h"

void Func_02000404(void) {
  s32 mode;

  Func_02002068();
  Func_02002150(0);
  Func_020020c0(11, 0, 0);
  GameFlag_SetBit(0xf5d);
  Func_02002130(193, 3);
  mode = 0;
  /* FAKEMATCH: the native loads the mode before the item; direct calls and
   * forwarding inlines load the item first. */
  __asm__ volatile("" : "+r"(mode));
  PartyInventory_GiveItem(193, mode);
  Func_02002070();
}

void Func_0200043c(s32 actor) {
  s32 msg = (s32)&MsgSettlements01;

  Func_020020e0(msg);
  UiText_OpenMessageAtObject(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Func_020021a8(), 0) == 0) {
    FieldEvent_WaitTicks(10);
    Func_020020e0(msg + 1);
  } else {
    FieldEvent_WaitTicks(20);
    Func_020020e0(msg + 2);
  }
  Func_020020f0(actor, 0);
}

void Func_02000488(void) { Window(5, 70, 9, 7, 5, 43, 1); }

void Func_020004a8(void) {
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (x == 5 && z == 17) {
    ObjectMotion_Launch(gPartyState.currentOwner, 2, 0);
    SetPosCommit(gPartyState.currentOwner, 88, 296);
  }
  Func_020021a0(5, 70, 9, 7, 5, 43, 1);
  Func_02002018(4, 25, 5, 17, 1, 1);
  Func_02002030(4, 25, 1, 1, 5, 17);
  GameFlag_SetBit(0xa60);
}

void Func_0200052c(void) {
  s32 next = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154);

  ObjectMotion_ArmCallback(4, 0xc000, 0);
  FieldEvent_WaitTicks(10);
  Object_GetById(4)->actionFlags &= 0xfe;
  CopyCells(96, 14, 72, 14, 2, 1);
  ObjectMotion_OffsetPositionAndResetMotion(4, 0, 4);
  ObjectMotion_CommitCurrentPositionAndActivate(4);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(4), 0);
  Object_SetModeById(4, 13);
  ObjectMotion_OffsetPositionAndResetMotion(4, 0, 16);
  ObjectMotion_CommitCurrentPositionAndActivate(4);
  Object_SetModeById(4, 10);
  FieldEvent_WaitTicks(10);
  Func_020021b0(123);
  Func_02002110(next);
  Func_02002070();
}

void Func_020005cc(void) {
  s32 id;

  Func_02002068();
  Func_02002150(0);
  CommitPos(4, 0, -14);
  id = 4;
  ObjectMotion_CommitPositionAndActivate(id, 16, 0);
  Func_0200052c();
}

void Func_020005f4(void) {
  s32 id, x, z;

  Func_02002068();
  Func_02002150(0);
  CommitPos(4, -2, -6);
  x = 14;
  z = 8;
  id = 4;
  /* FAKEMATCH: the actor id comes between the distances and their negation. */
  __asm__ volatile("" : "+r"(id));
  ObjectMotion_CommitPositionAndActivate(id, -x, -z);
  Func_0200052c();
}

void Func_02000624(void) {
  s32 id, a;

  Func_02002068();
  Func_02002150(0);
  a = 192;
  id = 4;
  /* FAKEMATCH: the actor id comes between the angle byte and its shift. */
  __asm__ volatile("" : "+r"(id));
  ObjectMotion_ArmCallback(id, a << 8, 0);
  Func_0200052c();
}

void Func_02000644(void) {
  CopyCells2(0, 3, 1, 1, 3, 15);
  GameFlag_SetBit(0x230);
}

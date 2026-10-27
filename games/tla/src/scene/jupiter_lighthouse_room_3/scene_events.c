#include "local.h"

/// Sets flag 0xa04 or 0xa05 when the object stands on cell (20, 36) or
/// (32, 36), then waits up to 30 frames for it to land and lifts it there.
void Func_02000134(s32 unused, s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);
  s32 placed = 0;
  s32 i;
  /* FAKEMATCH: the mode pinned to r2 before an empty asm is built after the
   * motion flag store, before the position loads; as a plain constant, 2
   * instructions differ in tla-en. */
  register s32 mode asm("r2");

  if (object->x >> 20 == 20 && object->z >> 20 == 36) {
    GameFlag_SetBit_(0xa04);
    placed = 1;
  }
  /* FAKEMATCH: the volatile x read stops PRE keeping x in r2 across the first
   * test and reloading it after the call (plain: differ 22). */
  if (*(volatile s32 *)&object->x >> 20 == 32 && object->z >> 20 == 36) {
    GameFlag_SetBit_(0xa05);
    placed = 1;
  }
  if (placed) {
    WaitTicks_(10);
    Func_02002470(134);
    for (i = 0; i < 30; i++) {
      if (object->y <= object->terrainHeight && object->velocityY == 0)
        break;
      WaitTicks(1);
    }
    object->motionFlags = 0;
    mode = 0;
    __asm__("");
    Func_02002368(object->x, object->z, mode, 0);
  }
}

void Func_020001c0(void) { Func_020023f8(12); }

const struct SceneEvent *Scene_GetEvents(void) { return gSceneEvents; }

s32 Func_020001d4(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  Func_02002440(12, 1);
  Func_02002440(13, 1);
  if (GameFlag_Test(0x109) == 0 && gPartyState.entrance == 12)
    Func_02002468(gPartyState.currentOwner);
  if (GameFlag_Test(0x109) == 0 && gPartyState.entrance == 13)
    Func_02002468(gPartyState.currentOwner);
  return 0;
}

s32 Func_02000250(void) {
  struct ObjectRuntime *object;
  s32 i;
  s32 x;

  Scene_SetArrivalFlags(0);
  if (GameFlag_Test(0xa04)) {
    object = Object_GetById(14);
    object->motionFlags = 0;
    object->terrainHeight = -0x200000;
    object->y = -0x200000;
    Func_020023b0_(14, 0x1480000, 0x2480000);
    Func_02002368(object->x, object->z, 0, 0);
  }
  if (GameFlag_Test(0xa05)) {
    object = Object_GetById(15);
    object->motionFlags = 0;
    object->terrainHeight = -0x200000;
    object->y = -0x200000;
    Func_020023b0_(15, 0x2080000, 0x2480000);
    Func_02002368(object->x, object->z, 0, 0);
  }
  Object_GetById(8)->priorityFlags |= 32;
  Object_GetById(9)->priorityFlags |= 32;
  Object_GetById(10)->priorityFlags |= 32;
  Object_GetById(11)->priorityFlags |= 32;
  ObjectMotion_SetActionVariant(8, 3);
  ObjectMotion_SetActionVariant(10, 3);
  ObjectMotion_SetActionVariant(9, 3);
  ObjectMotion_SetActionVariant(11, 3);
  Object_GetById(10)->priorityFlags |= 2;
  Func_020023d0(8);
  Func_020023d0(9);
  Func_020023d0(10);
  Func_020023d0(11);
  Func_02000c78(8);
  Func_02000c78(9);
  Func_02000c78(10);
  Func_02000c78(11);
  if (GameFlag_Test(0x200)) {
    for (i = 0, x = 0xb00000; i <= 3; i++, x += 0x100000) {
      Lift_(0x1000000, x, 0, 0);
      Func_02002458_(0x1000000, x, 0, 0);
      if (i <= 1)
        Func_02002460_(0x1000000, x, 0, 128);
    }
  }
  return 0;
}

void Func_020003c4(void) { Func_02002400(); }

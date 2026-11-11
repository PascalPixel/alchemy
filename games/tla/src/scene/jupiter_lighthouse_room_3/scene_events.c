#include "local.h"

/// Sets flag 0xa04 or 0xa05 when the object stands on cell (20, 36) or
/// (32, 36), then waits up to 30 frames for it to land and lifts it there.
void Func_02000134(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 placed = 0;
  s32 i;
  /* FAKEMATCH(pin): the mode pinned to r2, which with the empty once-only block
   * after it is built after the motion flag store, before the position loads;
   * as a plain constant, 2 instructions differ in tla-en. */
  register s32 mode asm("r2");

  if (object->x >> 20 == 20 && object->z >> 20 == 36) {
    Event_SetFlag(0xa04);
    placed = 1;
  }
  /* FAKEMATCH(volatile): x read through a volatile pointer, which stops PRE
   * keeping x in r2 across the first test and reloads it after the call; as
   * object->x, 22 instructions differ. */
  if (*(volatile s32 *)&object->x >> 20 == 32 && object->z >> 20 == 36) {
    Event_SetFlag(0xa05);
    placed = 1;
  }
  if (placed) {
    System_Wait(10);
    Event_PlaySound(134);
    for (i = 0; i < 30; i++) {
      if (object->y <= object->terrainHeight && object->velocityY == 0)
        break;
      System_Wait(1);
    }
    object->motionFlags = 0;
    mode = 0;
    do {
    } while (0);
    Func_02002368(object->x, object->z, mode, 0);
  }
}

void Func_020001c0(void) { Import_EventSetExitRequest(12); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Func_020001d4(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  Func_02002440(12, 1);
  Func_02002440(13, 1);
  if (Event_GetFlag(0x109) == 0 && gMainRoutineWork.entrance == 12)
    Func_02002468(gMainRoutineWork.currentOwner);
  if (Event_GetFlag(0x109) == 0 && gMainRoutineWork.entrance == 13)
    Func_02002468(gMainRoutineWork.currentOwner);
  return 0;
}

s32 Func_02000250(void) {
  struct Actor *object;
  s32 i;
  s32 x;

  Scene_SetArrivalFlags(0);
  if (Event_GetFlag(0xa04)) {
    object = Actor_Get(14);
    object->motionFlags = 0;
    object->terrainHeight = PIXELS(-32);
    object->y = PIXELS(-32);
    Func_020023b0_(14, PIXELS(328), PIXELS(584));
    Func_02002368(object->x, object->z, 0, 0);
  }
  if (Event_GetFlag(0xa05)) {
    object = Actor_Get(15);
    object->motionFlags = 0;
    object->terrainHeight = PIXELS(-32);
    object->y = PIXELS(-32);
    Func_020023b0_(15, PIXELS(520), PIXELS(584));
    Func_02002368(object->x, object->z, 0, 0);
  }
  Actor_Get(8)->priorityFlags |= 32;
  Actor_Get(9)->priorityFlags |= 32;
  Actor_Get(10)->priorityFlags |= 32;
  Actor_Get(11)->priorityFlags |= 32;
  FieldActor_SetSpritePriority(8, 3);
  FieldActor_SetSpritePriority(10, 3);
  FieldActor_SetSpritePriority(9, 3);
  FieldActor_SetSpritePriority(11, 3);
  Actor_Get(10)->priorityFlags |= 2;
  Func_020023d0(8);
  Func_020023d0(9);
  Func_020023d0(10);
  Func_020023d0(11);
  Func_02000c78(8);
  Func_02000c78(9);
  Func_02000c78(10);
  Func_02000c78(11);
  if (Event_GetFlag(0x200)) {
    for (i = 0, x = 0xb00000; i <= 3; i++, x += 0x100000) {
      Lift_(0x1000000, x, 0, 0);
      MapGrid_SetCellBehavior(0x1000000, x, 0, 0);
      if (i <= 1)
        Func_02002460(0x1000000, x, 0, 128);
    }
  }
  return 0;
}

void Func_020003c4(void) { Func_02002400(); }

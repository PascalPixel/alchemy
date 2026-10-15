#include "local.h"

u8 *Func_0200004c(void) {
  s32 scene = gPartyState.scene;

  if (scene == (s32)&SceneId_Ov66aRoom1)
    return gData_02001778;
  if (scene == (s32)&SceneId_Ov66aRoom2)
    return gData_02001868;
  return gData_02001778;
}

void Func_02000084(s32 mode, s32 frame) {
  Func_0200129c(gData_020016e4, mode, frame);
}

void Func_02000098(void) {}

void Func_0200009c(void) {}

void Func_020000a0(s32 id) {
  Func_020014b8((s32)&MsgOhStoppedBy);
  UiText_OpenMessageAtObject(id, 0);
  if (Inventory_PromptAndSetObjectMode(Func_02001590(), 0) == 0) {
    Battle_WaitMode0(10);
    Func_020014b8((s32)(&MsgOhStoppedBy + 1));
  } else {
    Battle_WaitMode0(20);
    Func_020014b8((s32)(&MsgOhStoppedBy + 2));
  }
  Func_020014d0(id, 0);
}

void Func_020000ec(void) { Func_02001580(gData_02001608); }

void Func_020000fc(void) {
  GameFlag_SetBit(0x204);
  Func_02001420_(0x1900000, 0x1300000, 0, 4);
  Func_02001420_(0x1a00000, 0x1300000, 0, 4);
  Func_02001420_(0x1b00000, 0x1300000, 0, 4);
  Func_020015d0_(0x1800000, 0x1200000, 0, 255);
  Func_02001420_(0x1200000, 0x1800000, 0, 4);
  Func_02001420_(0x1500000, 0x1800000, 0, 4);
}

void Func_02000168(void) {
  GameFlag_ClearBit(0x204);
  Func_02001420_(0x1900000, 0x1300000, 0, 0);
  Func_02001420_(0x1a00000, 0x1300000, 0, 0);
  Func_02001420_(0x1b00000, 0x1300000, 0, 0);
  Func_020015d0_(0x1800000, 0x1200000, 0, 0);
  Func_02001420_(0x1200000, 0x1800000, 0, 0);
  Func_02001420_(0x1500000, 0x1800000, 0, 0);
}

void Func_020001d4(void) {
  struct AlhafranCaveEventWork *work = Ram_HeapSlots->eventWork;

  /* FAKEMATCH: a one-pass block loads the event work before its offset is built
   * (one reordered row). */
  do {
  } while (0);
  work->valueD4a = 2;
}

u8 *Func_020001e8(void) {
  s32 scene = gPartyState.scene;

  if (scene == (s32)&SceneId_Ov66aRoom1)
    return gData_02001a18;
  if (scene == (s32)&SceneId_Ov66aRoom2)
    return gData_02001a90;
  return gData_02001a18;
}

void Func_02000220(void) {
  s32 y = Object_GetById(gPartyState.currentOwner)->y;

  /* FAKEMATCH: a one-pass block loads the height before the threshold is built
   * (one reordered row). */
  do {
  } while (0);
  if (y < 0x80000)
    ((u8 *)&gPartyState)[0x2c3] = 0;
  else
    ((u8 *)&gPartyState)[0x2c3] = 1;
}

s32 Func_02000258(void) {
  struct ObjectRuntime *object = Object_GetById(8);

  Func_020015f0(gData_02001be0, object->x, object->y + 0x1c0000, object->z);
  return 0;
}

s32 Func_0200027c(void) {
#if defined(TLA_EDITION_JA) /* 日本版は入口を見ない */
  ((struct AlhafranCaveEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x201;
  if (gPartyState.scene == (s32)&SceneId_Ov66aRoom1)
    Func_020002f8();
  else if (gPartyState.scene == (s32)&SceneId_Ov66aRoom2)
    Func_0200051c();
  return 0;
#else
  s32 scene = gPartyState.scene;

  if (scene == (s32)&SceneId_Ov66aRoom1) {
    ((struct AlhafranCaveEventWork *)Ram_HeapSlots->eventWork)->value1ac =
        0x201;
    Func_020002f8();
  } else if (scene == (s32)&SceneId_Ov66aRoom2) {
    if (gPartyState.entrance == 8 || gPartyState.entrance == 9)
      ((struct AlhafranCaveEventWork *)Ram_HeapSlots->eventWork)->value1ac =
          0x209;
    else
      ((struct AlhafranCaveEventWork *)Ram_HeapSlots->eventWork)->value1ac =
          0x201;
    Func_0200051c();
  }
  return 0;
#endif
}

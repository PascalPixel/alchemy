/// @file
/// The scene code of overlay 0x67c that its scene tables belong to, in ROM
/// order; the rest of the overlay is in aqua_rock_inside/.

#include "aqua_rock_inside/local.h"

void Func_0200033c(void) { Func_080d2b4c(10, 32); }

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "scene_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_02004688[];
extern const u8 gData_020046a0[];
extern const u8 gData_02004748[];
extern const u8 gData_020047a8[];
extern const u8 gData_02004808[];
extern const u8 gData_020048c8[];
extern const u8 gData_02004910[];
extern const u8 gData_020049e8[];
extern const u8 gData_02004a00[];
extern const u8 gData_02004a48[];
extern const u8 gData_02004b98[];

const u8 *Func_0200035c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov67cRoom1)
    return gData_020046a0;
  if (scene == (s32)&SceneId_Ov67cRoom2)
    return gData_02004748;
  if (scene == (s32)&SceneId_Ov67cRoom3)
    return gData_020047a8;
  if (scene == (s32)&SceneId_Ov67cRoom4)
    return gData_02004808;
  if (scene == (s32)&SceneId_Ov67cRoom5)
    return gData_020048c8;
  if (scene == (s32)&SceneId_Ov67cRoom6)
    return gData_02004910;
  if (scene == (s32)&SceneId_Ov67cRoom7)
    return gData_020049e8;
  if (scene == (s32)&SceneId_Ov67cRoom8)
    return gData_02004a00;
  if (scene == (s32)&SceneId_Ov67cRoom9)
    return gData_02004a48;
  if (scene == (s32)&SceneId_Ov67cRoom10)
    return gData_02004b98;
  return gData_02004688;
}

#include "version.h"

/// When object 10 reaches row 12, or object 11 column 51, stops its motion,
/// lifts it one pixel and sets its flag; object 10 also redraws two blocks of
/// row 12.
void Func_0200042c(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == 10) {
    s32 row = object->z >> 20;

    if (row == 12) {
      object->motionFlags = 0;
      {
        s32 step = -0x10000;

        object->y += step;
      }
      Event_SetFlag(0x95a);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 9, row);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 13, row);
    }
  } else if (id == 11) {
    if (object->x >> 20 == 51) {
      object->motionFlags = 0;
      {
        s32 step = -0x10000;

        object->y += step;
      }
      Event_SetFlag(0x95b);
    }
  }
}

void Func_020004b4(void) { Func_02003e6c(); }

void Func_0200349c(u32 index, s32 height);

void Func_020004bc(s32 unused, s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *layer;
  s32 i;
  s32 column;
  s32 objectId = id;

  layer = (s32 *)(map + 0x140);
  Actor_SetSpriteColor(Actor_Get(objectId), 0);
  Func_02001e78(objectId);
  Actor_SetAnimation(objectId, 3);
  layer[3] -= 0x200000;
  Func_02003d54();
  Func_02003cdc();
  column = (Actor_Get(12)->x >> 20) - 42;
  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(0x22c);
  for (i = 0; i <= 31; i++) {
    Func_02003460(i << 16);
    {
      /* FAKEMATCH(pin): the zero pinned to r1, set before the column is copied
       * to r0; as a plain local, 2 of 72 instructions differ in tla-en (2 as a
       * literal 0). */
      register s32 height asm("r1") = 0;

      Func_0200349c(column, height);
    }
    layer[3] += 0x10000;
    System_Wait(4);
  }
  if (Event_GetFlag(0x250))
    Func_02003d1c_(41, 42, 1, 1, 43, 44);
  Func_02003d84();
  Event_SetFlag(0x230);
}

void Func_02000574(s32 unused, s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *layer;
  s32 i;

  layer = (s32 *)(map + 0x140);
  Actor_SetSpriteColor(Actor_Get(id), 0);
  Func_02001e78(id);
  Actor_SetAnimation(id, 3);
  layer[3] -= 0x200000;
  Func_02003d54();
  Func_02003cdc();
  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(0x22c);
  for (i = 0; i <= 31; i++) {
    Func_02003460(i << 16);
    layer[3] += 0x10000;
    System_Wait(4);
  }
  Func_02003d84();
  Event_SetFlag(0x231);
}

void Data_020005f4(void) {}

void Func_020005f8(void) {
  Func_02003d1c_(41, 42, 1, 1, 43, 44);
  Event_SetFlag(0x250);
}

void Func_0200061c(struct Actor *object);

#if TLA_PATCH_1
void Func_0200061c(struct Actor *object) {
  object->motionFlags = 3;
  do {
    System_Wait(1);
  } while (object->velocityY != 0);
  Event_PlaySound(188);
  System_Wait(10);
  object->motionFlags = 0;
}
#endif

void Func_02000644(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 x;

  if (id == 9) {
    x = object->x >> 20;
    if (x == 39) {
#if !TLA_PATCH_1
      object->motionFlags = 3;
      System_Wait(15);
      object->motionFlags = 0;
      Event_SetFlag(0x95c);
      MapGrid_CopyCellAttributes(41, 9, 1, 1, x, id);
#else
      Func_02003d7c();
      FieldEvent_ResolvePendingActions(0);
      Func_0200061c(object);
      Event_SetFlag(0x95c);
      MapGrid_CopyCellAttributes(41, 9, 1, 1, x, id);
      Func_02003d84();
#endif
    }
  }
}

void Func_02000690(void) {
  Actor_SetAnimation(8, 4);
  Actor_SetSpriteColor(Actor_Get(8), 6);
  Func_02003d14_(123, 15, 5, 3, 110, 15);
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->value16a = 88;
}

void Func_020006d0(void) {
  s32 count = gData_02004d84 - 1;
  s32 row;

  gData_02004d84 = count;
  row = count / 8;
  MapGrid_CopyCells((count & 1) * 5 + 118, 47 - row, 5, 2, 110, 47 - row);
  if (gData_02004d84 == 0)
    FieldTask_Remove(Func_020006d0);
}

void Func_0200071c(void) {
  s32 priority;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Actor_ArmCallback_(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Event_PlaySound(0x22d);
  priority = 0x480;
  FieldTask_AddFrame(Func_020006d0, priority);
  FieldCamera_SetSpeed(0x28000, 0x4000);
  FieldCamera_MoveTo(PIXELS(776), -1, PIXELS(648), 1);
  Func_02003e54();
  FieldEvent_Wait(30);
  Func_02003d84();
  Event_SetFlag(0x240);
  Func_02003d1c_(59, 16, 5, 32, 46, 16);
}

void Func_020007a8(void) {
  struct Actor *object;
  struct FieldSpriteFlags *sprite;

  Actor_ShowItemIconById(10, 0x1c8);
  object = Actor_Get(20);
  sprite = object->sprite;
  sprite->flag0 = 1;
  object->motionFlags = 0;
  object->y = 0x180000;
}

extern const u8 gData_02003fb8[];

void Sound_WaitForBgmRestore(void);
void Func_02003ebc(void);

static __inline__ void Func_02003d44_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH(wrapper): the call forwarded through an inline, which keeps each
   * call's own constants, as The Broken Seal's scene layer does; as direct
   * calls to Map_SetScreenShake, 36 of 154 instructions of Func_020007d4 differ
   * in tla-en. */
  Map_SetScreenShake(a, b, c);
}

void Func_020007d4(void) {
  s32 priority;
  s32 i;
  s32 j;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 0);
  Func_020007a8();
  Event_PlaySound(78);
  FieldEvent_Wait(60);
  Event_PlaySound(162);
  Func_02003d44_(0x10000, 0x10000, 0x10000);
  FieldEvent_Wait(20);
  Func_02003d44_(0x10000, 0x20000, 0x10000);
  FieldEvent_Wait(20);
  Func_02003d44_(0x10000, 0x30000, 0x10000);
  FieldEvent_Wait(40);
  priority = 0x480;
  FieldTask_AddFrame(Func_020006d0, priority);
  FieldCamera_SetSpeed(0x28000, 0x4000);
  FieldCamera_MoveTo(0x1380000, -1, 0x1f80000, 1);
  Event_PlaySound(164);
  Func_02003d24_(75, 96, 7, 13, 80, 27);
  MapGrid_CopyCellsLayered(91, 96, 7, 10, 16, 31);
  MapGrid_CopyCellsLayered(99, 96, 7, 14, 16, 91);
  MapGrid_CopyCellAttributes(43, 0, 5, 9, 17, 31);
  // Both loops are goto loops: the ROM builds the two stack arguments and the
  // table address on every pass, which for loops hoist (differ 38).
  i = 0;
row:
  j = 0;
cell:
  {
    s32 index = (i + (1 & ~j)) * 2;

    Func_02003d24_(gData_02003fb8[index], gData_02003fb8[index + 1], 7, 13, 80,
                   27);
    System_Wait(1);
  }
  j++;
  if (j <= 6)
    goto cell;
  i++;
  if (i <= 14)
    goto row;
  Func_02003e54();
  Func_02003d44_(-1, -1, 0xe666);
  Event_PlaySound(390);
  FieldEvent_Wait(30);
  Event_PlaySound(80);
  Sound_WaitForBgmRestore();
  Func_02003d84();
  Func_02003ebc();
  Event_SetFlag(0x95d);
  Event_SetFlag(0x143);
}

void Func_02000958(s32 value) {
  s32 width = 1;
  s32 height = 2;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(188);
  MapGrid_CopyCellsTo(107, 96, 19, 29, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(108, 96, 19, 29, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(107, 98, 19, 29, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(108, 98, 19, 29, width, height);
  FieldEvent_Wait(15);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_Get(gMainRoutineWork.currentOwner)->speedLimit =
      Actor_Get(gMainRoutineWork.currentOwner)->acceleration = 0x8000;
  Event_PlaySound(123);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 312, 496);
  FieldEvent_Wait(5);
  FieldEvent_SetExitRequest(value);
}

void Func_02000a24(void) {
  s32 seen;
  const u8 *message;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  seen = Event_GetFlag(0x95d);
  message = MsgCheckedTheColumn;
  if (!seen) {
    Message_ShowCentered(message, 1);
    Message_ShowCentered(message + 1, 1);
    if (FieldPartyInventory_FindOwner(0x1c8) != -1)
      ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->value15a = 1;
  } else {
    Message_ShowCentered(message, 1);
    Func_02003d64(0x1c8, 2);
    Message_ShowCentered(message + 2, 1);
  }
  Func_02003d84();
}

void Func_02000a98(void) {
  s32 i;

  Func_02003628();
  Func_02003688(16, 4, 16, 2);
  Func_02003688(22, 4, 16, 0);
  for (i = 5; i >= 0; i--) {
    Func_020036b8(33, 55);
    System_Wait(15);
  }
  Event_SetFlag(0xa66);
}

void Func_02000ad8(void) {
  MapGrid_CopyCellsLayered(106, 14, 14, 13, 81, 14);
  MapGrid_CopyCellsLayered(44, 78, 14, 12, 17, 78);
  MapGrid_CopyCellAttributes(81, 14, 14, 13, 17, 14);
  Event_SetFlag(0xa6a);
}

s32 Func_02000b24(s32 id, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_Get(id);

  if (object != 0) {
    struct MapCell *cells = map->layers[0].cells;
    struct MapCell *cell = &cells[(object->x >> 20) + (object->z >> 20) * 128];

    cell->collisionCode = value;
    cell->attributeB |= 0x80;
  }
}

void Func_02000b5c(void) {
  struct Actor *object = Actor_Get(12);

  Func_02003d1c_(64, 43, 1, 1, object->x >> 20, object->z >> 20);
}

void Func_02000b84(void) {
  struct Actor *object = Actor_Get(12);

  Func_02003d1c_(64, 42, 1, 1, object->x >> 20, object->z >> 20);
}

void Func_02000bac(void) {
  struct Actor *object = Actor_Get(12);

  Func_02003d1c_(64, 41, 1, 1, object->x >> 20, object->z >> 20);
}

void Func_02000bd4(void) {
  s32 i;
  s32 result;

  if (Event_GetFlag(0x249)) {
    Func_02000b24(12, 38);
    Func_02003628();
    Func_02003688(12, 2, 45, 1);
    for (i = 3; i >= 0; i--)
      result = Func_020036b8(33, 55);
    Event_SetFlag(0x248);
    if (result == 1)
      Event_SetFlag(0xa64);
  }
}

void Func_02000c28(s32 unused, s32 id) {
  s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x140);
  s32 i;

  Actor_SetSpriteColor(Actor_Get(9), 5);
  Func_02001e78(id);
  Actor_SetAnimation(id, 3);
  layer[3] -= 0x200000;
  Func_02003d54();
  Func_02003cdc();
  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(0x22d);
  for (i = 31; i >= 0; i--) {
    layer[3] += 0x10000;
    System_Wait(4);
  }
  Func_02003d84();
  Func_02000b5c();
  Event_SetFlag(0x249);
}

void Func_02000ca8(void) {
  Func_02003df4(gMainRoutineWork.currentOwner, 6, 0);
  Func_02003d84();
}

/// @file
/// Scene code of overlay 0x67c, in ROM order, from s0cc8's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "system/vector3.h"
#include "event_inline.h"
#include "local.h"
#include "scene_tables.h"
#include "io_write_queue.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_02000ca8();
s32 Func_02003d2c(struct Actor *object, struct Vector3 *target);

void Func_02000cc8(s32 trigger) {
  s32 clear = 1;
  struct Actor *object;
  struct Vector3 target;
  s32 result;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67cRoom5) {
    s32 column = Actor_Get(8)->x >> 20;

    if (trigger == 77 && column == 17)
      clear = 0;
    if (trigger == 78 && column == 19)
      clear = 0;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67cRoom6) {
    if (trigger == 77 && Actor_Get(10)->x >> 20 == 9)
      clear = 0;
    if (trigger == 78 && Actor_Get(11)->x >> 20 == 13)
      clear = 0;
  }
  if (clear == 0)
    return;
  object = Actor_Get(gMainRoutineWork.currentOwner);
  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  goto test;
step:
  if (result < 0) {
    object->angle = 0x4000;
    Func_02000ca8(trigger);
    return;
  }
  {
    s32 offset, x;

    object->z += 0x20000;
    x = object->x;
    offset = (x & 0xfffff) - 0x80000;
    {
      /* FAKEMATCH(temporary): each bound in one local just past the limit; as
       * literal bounds, 4 of 120 instructions differ in tla-en. */
      s32 limit = 0x10001;

      if (offset >= limit)
        offset = 0x10000;
      limit = -0x10001;
      if (offset <= limit)
        offset = -0x10000;
    }
    object->x = x - offset;
  }
  object->angle += 0x1000;
  System_Wait(1);
test:
  target.x = object->x;
  target.y = object->y;
  target.z = object->z + 0x80000;
  result = Func_02003d2c(object, &target);
  if (result <= 0)
    goto step;
}

void Func_02000de4(void) {
  s32 i;

  Func_02003628();
  Func_02003688(44, 0, 34, 2);
  for (i = 4; i >= 0; i--) {
    Func_020036b8(33, 55);
    System_Wait(15);
  }
  Event_SetFlag(0xa67);
}

void Func_02000e18(s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 speed;
  s32 i;
  u8 *flags;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  flags = &object->motionFlags;
  speed = 0;
  *flags = 0;
  object->priorityFlags |= 0x20;
  Func_02003cc4(object, 18);
  Event_PlaySound(215);
  System_Wait(13);
  Animation_ApplyChildValues(object, 9);
  Func_02003cc4(object, 28);
  Event_PlaySound(0x236);
  for (i = 0; i <= 59; i++) {
    if (i == 30)
      Event_FinishScreenTransition();
    speed += 0x4000;
    object->y += speed;
    if ((i & 7) == 0)
      Func_02002bd4(Actor_Get(15));
    System_Wait(1);
  }
  Func_02003d84();
  FieldEvent_SetExitRequest(value);
}

void Func_02003cfc(void);
void Func_02003d04(void);

void Func_02000ecc(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object->x >> 20 == 49) {
    s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x140);
    s32 i;

    Func_02003d7c();
    FieldEvent_ResolvePendingActions(0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Event_PlaySound(0x22d);
    Actor_SetAnimation(9, 2);
    Func_02003d24_(114, 39, 1, 2, 113, 39);
    for (i = 31; i >= 0; i--) {
      layer[3] -= 0x10000;
      System_Wait(4);
    }
    // A goto loop: the ROM rebuilds the zero and the depth on every pass, which
    // a for loop hoists.
    i = 13;
  place:
    object = Actor_GetById(i);
    object->x = object->action << 16;
    object->z = *(s16 *)((u8 *)object + 102) << 16;
    object->motionFlags = 0;
    object->terrainHeight = -0x200000;
    object->y = -0x200000;
    FieldActor_SetSpritePriority(i, 3);
    i++;
    if (i <= 15)
      goto place;
    Func_02003d04();
    // A goto loop too: here a for loop hoists the queue's and the register's
    // addresses (differ 24).
    i = 12;
  blend:
    IO_WRITE_QUEUE_PUSH(0x04000052, 0x1000 | i, 0x20000);
    System_Wait(4);
    i--;
    if (i >= 0)
      goto blend;
    MapGrid_CopyCellAttributes(105, 41, 12, 13, 41, 41);
    MapGrid_CopyCellsLayered(105, 105, 12, 13, 105, 41);
    Func_02003cfc();
    Func_02003d84();
    Event_SetFlag(0x238);
  }
}

void Func_0200101c(s32 unused, s32 id) {
  if ((u32)(id - 13) <= 2) {
    struct Actor *object = Actor_Get(id);

    Func_02003d1c_(47, 45, 1, 1, object->x >> 20, object->z >> 20);
    Event_SetFlag(id + 0x244);
    object->priorityFlags = 0;
  }
}

void Func_02001060(void) { Func_02003d1c_(8, 63, 7, 1, 8, 34); }

/// When object 10 or 11 arrives, stops both and redraws row 33 at each one's
/// column.
void Func_0200107c(s32 unused, s32 id) {
  if ((u32)(id - 10) <= 1) {
    struct Actor *object = Actor_Get(10);

    object->motionFlags = 0;
    {
      s32 row = 33;

      Func_02003d1c_(8, row, 1, 1, object->x >> 20, 34);
    }
    object = Actor_Get(11);
    object->motionFlags = 0;
    {
      s32 row = 33;

      MapGrid_CopyCellAttributes(8, row, 1, 1, object->x >> 20, 34);
    }
  }
}

void Func_020010d0(struct Actor *object) {
  s32 frames = 60;

  while (frames != 0) {
    System_Wait(1);
    frames--;
    if (object->velocityY == 0)
      break;
  }
}

void Func_020010ec(void) {
  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgCheckedThePsynergy, 1);
  Event_PlaySound(126);
  Func_02003eb4(999, 0);
  FieldEvent_Wait(10);
  Event_ClearFlag(0x142);
  Message_ShowCentered(MsgCheckedThePsynergy + 1, 1);
  Func_02003d84();
}

void Func_02001138(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  MapGrid_CopyCellAttributes(17, 39, 2, 1, 20, 39);
  MapGrid_CopyCellAttributes(23, 39, 1, 1, object->x >> 20, 39);
}

void Func_02001170(void) {
  s32 i;
  s32 result;

  if (Event_GetFlag(0x24b)) {
    /* FAKEMATCH(signature): these two calls go through a void function type, so
     * r0 is set before r1, as the ROM does; as declared (value-returning, as
     * the third), r1 loads first, 4 of 41 instructions differ in tla-en. */
    ((void (*)(s32, s32))Func_02000b24)(12, 38);
    ((void (*)(s32, s32))Func_02000b24)(13, 39);
    Func_02000b24(15, 37);
    Func_02003628();
    Func_02003688(38, 2, 19, 1);
    for (i = 8; i >= 0; i--)
      result = Func_020036b8(33, 55);
    Event_SetFlag(0x24a);
    if (result == 1)
      Event_SetFlag(0xa65);
  }
}

void Func_020011d4(s32 unused, s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *layer;
  s32 i;

  layer = (s32 *)(map + 0x140);
  Actor_SetSpriteColor(Actor_Get(id), 5);
  Func_02001e78(id);
  Actor_SetAnimation(id, 3);
  layer[3] -= 0x200000;
  Func_02003d54();
  Func_02003cdc();
  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(0x22c);
  for (i = 31; i >= 0; i--) {
    layer[3] += 0x10000;
    System_Wait(4);
  }
  Func_02003d84();
  Event_SetFlag(0x24b);
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02003d44_(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

void Func_0200124c(void) {
  s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x140);
  s32 i;

  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  layer[3] -= 0x200000;
  Func_02003cdc();
  Func_02003d44_(0x10000, 0x10000, 0x10000);
  // A goto loop: the ROM sets the column and the row on every pass, which a for
  // loop hoists (differ 44).
  i = 14;
shake:
  {
    s32 column = 77;
    s32 row = 8;

    MapGrid_CopyCellsLayered(70, 8, 7, 6, column, row);
    System_Wait(1);
    MapGrid_CopyCellsLayered(50, 8, 7, 6, column, row);
    System_Wait(1);
  }
  i--;
  if (i >= 0)
    goto shake;
  System_Wait(10);
  for (i = 127; i >= 0; i--) {
    layer[3] += 0x4000;
    System_Wait(1);
  }
  Func_02003d44_(-1, -1, 0xe666);
  Func_02003d84();
  Func_02003d1c_(38, 9, 7, 6, 13, 9);
  Event_SetFlag(0x349);
}

void Func_02001328(void) {
  Func_02003d24_(70, 8, 7, 6, 77, 8);
  Func_02003d1c_(38, 17, 7, 6, 13, 9);
  Event_ClearFlag(0x349);
}

void Func_02001364(struct Actor *object) {
  struct Actor *child =
      Import_ActorCreate_(0x2a0, object->x, object->y, object->z);

  child->scaleY = 0x6666;
  child->scaleX = 0x6666;
  child->motionFlags = 0;
  Actor_SetSpriteColor(child, 2);
  Scene_SetActorSpritePriority(child, 1);
  Func_02003cc4(child, 1);
  Actor_SetSingleChildFlags(child, 0);
  Animation_SetStateFlags(child, 1);
  Func_02003ccc(child, gData_02003fd8);
}

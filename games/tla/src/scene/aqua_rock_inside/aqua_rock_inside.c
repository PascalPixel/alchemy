/// @file
/// Scene code of overlay 0x67c, in ROM order.

// ---- games/tla/src/scene/aqua_rock_inside.c ----

#include "../shared/effects.h"
#include "../aqua_rock_inside.h"
#include "field.h"
#include "module/field/map.h"
#include "module/scene/aqua_rock_inside.h"
#include "module/scene/scene.h"
#include "module/sound/sound.h"
#include "module/system/system.h"
#include "module/tla.h"
#include "text/msg_ids.h"

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

const u8 *Scene_GetPlacements(void) {
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
  Map_CenterOnFollowedPoint();
  column = (Actor_Get(12)->x >> 20) - 42;
  Event_Start();
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
  Event_End();
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
  Map_CenterOnFollowedPoint();
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(0x22c);
  for (i = 0; i <= 31; i++) {
    Func_02003460(i << 16);
    layer[3] += 0x10000;
    System_Wait(4);
  }
  Event_End();
  Event_SetFlag(0x231);
}

void Data_020005f4(void) {}

void Func_020005f8(void) {
  Func_02003d1c_(41, 42, 1, 1, 43, 44);
  Event_SetFlag(0x250);
}

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
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Func_0200061c(object);
      Event_SetFlag(0x95c);
      MapGrid_CopyCellAttributes(41, 9, 1, 1, x, id);
      Event_End();
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

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_ArmCallback_(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Event_PlaySound(0x22d);
  priority = 0x480;
  FieldTask_AddFrame(Func_020006d0, priority);
  FieldCamera_SetSpeed(0x28000, 0x4000);
  FieldCamera_MoveTo(PIXELS(776), -1, PIXELS(648), 1);
  Func_02003e54();
  FieldEvent_Wait(30);
  Event_End();
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

void Func_020007d4(void) {
  s32 priority;
  s32 i;
  s32 j;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 0);
  Func_020007a8();
  Event_PlaySound(78);
  FieldEvent_Wait(60);
  Event_PlaySound(162);
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  FieldEvent_Wait(20);
  MapGrid_SetShake(0x10000, 0x20000, 0x10000);
  FieldEvent_Wait(20);
  MapGrid_SetShake(0x10000, 0x30000, 0x10000);
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
  MapGrid_SetShake(-1, -1, 0xe666);
  Event_PlaySound(390);
  FieldEvent_Wait(30);
  Event_PlaySound(80);
  Sound_WaitForBgmRestore();
  Event_End();
  Party_PlaySavedMusic();
  Event_SetFlag(0x95d);
  Event_SetFlag(0x143);
}

void Func_02000958(s32 value) {
  s32 width = 1;
  s32 height = 2;

  Event_Start();
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

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  seen = Event_GetFlag(0x95d);
  message = MsgCheckedTheColumn;
  if (!seen) {
    Message_ShowCentered(message, 1);
    Message_ShowCentered(message + 1, 1);
    if (FieldPartyInventory_FindMember(0x1c8) != -1)
      ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->value15a = 1;
  } else {
    Message_ShowCentered(message, 1);
    MessageArgument_Push(0x1c8, 2);
    Message_ShowCentered(message + 2, 1);
  }
  Event_End();
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
  Map_CenterOnFollowedPoint();
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(0x22d);
  for (i = 31; i >= 0; i--) {
    layer[3] += 0x10000;
    System_Wait(4);
  }
  Event_End();
  Func_02000b5c();
  Event_SetFlag(0x249);
}

void Func_02000ca8(void) {
  Actor_JumpDownLedge(gMainRoutineWork.currentOwner, 6, 0);
  Event_End();
}

// ---- games/tla/src/scene/aqua_rock_inside/s0cc8.c ----
// Scene code of overlay 0x67c, in ROM order, from s0cc8's offset up to the next object the overlay links.

#include "actor.h"
#include "system/vector3.h"
#include "event_inline.h"
#include "io_write_queue.h"

/* VIEW(s0cc8): routine Func_02000ca8 as void Func_02000ca8(); the parts before it has another declaration; tried as the definition's void Func_02000ca8(void), which the object already declares, it does not compile in tla-ja: games/tla/src/scene/aqua_rock_inside.c:503: too many arguments to function `Func_02000ca8'. */
extern void Func_02000ca8__s0cc8() __asm__("Func_02000ca8");

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
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  goto test;
step:
  if (result < 0) {
    object->facing = 0x4000;
    Func_02000ca8__s0cc8(trigger);
    return;
  }
  {
    s32 offset, x;

    object->z += 0x20000;
    x = object->x;
    offset = (x & 0xfffff) - 0x80000;
    {
      s32 limit = 0x10001;

      if (offset >= limit)
        offset = 0x10000;
      limit = -0x10001;
      if (offset <= limit)
        offset = -0x10000;
    }
    object->x = x - offset;
  }
  object->facing += 0x1000;
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

  Event_Start();
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
  Event_End();
  FieldEvent_SetExitRequest(value);
}

void Func_02000ecc(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object->x >> 20 == 49) {
    s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x140);
    s32 i;

    Event_Start();
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
    object->x = object->timer << 16;
    object->z = *(s16 *)((u8 *)object + 102) << 16;
    object->motionFlags = 0;
    object->terrainHeight = -0x200000;
    object->y = -0x200000;
    FieldActor_SetSpritePriority(i, 3);
    i++;
    if (i <= 15)
      goto place;
    DisplayBlend_EnableRunScript();
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
    DisplayBlend_DisableRunScript();
    Event_End();
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
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgCheckedThePsynergy, 1);
  Event_PlaySound(126);
  BattleParty_ApplyDrain(999, 0);
  FieldEvent_Wait(10);
  Event_ClearFlag(0x142);
  Message_ShowCentered(MsgCheckedThePsynergy + 1, 1);
  Event_End();
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
  Map_CenterOnFollowedPoint();
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(0x22c);
  for (i = 31; i >= 0; i--) {
    layer[3] += 0x10000;
    System_Wait(4);
  }
  Event_End();
  Event_SetFlag(0x24b);
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

void Func_0200124c(void) {
  s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x140);
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  layer[3] -= 0x200000;
  Map_CenterOnFollowedPoint();
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
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
  MapGrid_SetShake(-1, -1, 0xe666);
  Event_End();
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
  Actor_StartScript(child, gData_02003fd8);
}

// ---- games/tla/src/scene/aqua_rock_inside/s13bc.c ----
// Scene code of overlay 0x67c, in ROM order, from s13bc's offset up to the next object the overlay links.

#include "frame.h"

// Steps an orbiting object around its anchor: turns it by half its speed, puts
// it on the anchor (raised 8 pixels) and pushes it out by its radius; when its
// countdown runs out returns 0, else, unless its radius is down to 2 pixels,
// takes a second, shrinking step. Returns 1 while it runs. The same code as
// overlay 681's Func_0200136c. The first push goes through Call3 (directly, its
// offset pointer is set after the angle's load).

DEFINE_ORBITER_SPIRAL(Orbiter_Spiral, Func_02001364)

// Moves around in a circle.
DEFINE_ORBIT_UPDATE(Orbit_Update)

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

DEFINE_ACTOR_AIM_AT_WATCHED(Actor_AimAtWatched)

DEFINE_ACTOR_STEP_TO_WATCHED(Actor_StepToWatched)

#include "system/field_effect.h"

void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

DEFINE_ACTOR_EMIT_BUBBLES(Actor_EmitBubbles, Func_0200015c)
#undef BYTE_AT

// ---- games/tla/src/scene/aqua_rock_inside/s1620.c ----
// Scene code of overlay 0x67c, in ROM order, from s1620's offset up to the
// next object the overlay links.

#include "system/io_reg.h"
#include "ram_buffer.h"

union ActorView {
  struct Actor actor;
  struct {
    u8 unk00[0x62];
    u8 unk62;
    u8 unk63;
  } bytes;
};
extern const u8 gData_02004034[], gData_02003fe8[], gData_02004004[],
    gData_02004058[], gData_02004014[], gData_0200407c[], gData_020040ac[];

void Func_02001620(s32 firstId, s32 secondId) {
  struct Actor *first = Actor_GetById(firstId);
  struct Actor *second = Actor_GetById(secondId);
  struct Actor *object;
  struct Actor *spark;
  struct Actor *riser;
  s32 i;
  u16 noMotion;
  s32 probe;
  const u8 *script;
  // Unused: gives the stack frame its 12 bytes.
  s32 unused[3];

  script = gData_02004058;
  Sound_Play(78);
  Event_Wait(30);
  FieldCamera_SetSpeed(0x8000, 0x1000);
  Camera_MoveWithinMapBounds((first->x >> 16) << 16, (first->y >> 16) << 16,
                             (first->z >> 16) << 16, 1);
  Sound_Play(141);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  BattleEffect_StartBufferInterpolation(60);
  first->motionFlags = 0;
  first->acceleration = 0x3333;
  first->speedLimit = 0x3333;
  Actor_SetMovementTypeById(firstId, gData_02004034);
  Sound_Play(194);
  Event_Wait(45);
  Actor_ConfigureSpriteColor(firstId, 256);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  i = 0;
burst:
  object = ((void *)Import_ActorCreate(672, first->x, first->y, first->z));
  Sound_Play(246);
  object->x += ((Random16Far() & 31) - 16) << 16;
  object->y += ((Random16Far() & 15) - 8) << 16;
  object->speedLimit = 0x80000;
  object->acceleration = 0x3333;
  object->motionFlags = 0;
  Actor_SetPartAttribute(object, 2);
  Scene_SetActorSpritePriority(object, 1);
  Func_02003cc4(object, 7);
  Actor_SetSingleChildFlags(object, 0);
  Animation_SetStateFlags(object, 1);
  Actor_StartScript(object, gData_02003fe8);
  REG_BLDALPHA = 0x1004;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  REG_BLDALPHA = 0x1010;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  i++;
  if (i != 16)
    goto burst;
  REG_BLDALPHA = 0x1008;
  wait(30);
  i = 0;
  /* FAKEMATCH(dead): the never-taken jump into the spiral keeps its refs at
   * weight 1; as the spiral alone, 174 of 402 instructions differ in tla-en. */
  probe = second->unk57;
  if (probe & 0x100) {
    wait(1);
    goto tail;
  }
spiral:
  spark = FieldActor_Create(672, second->x, second->y, second->z);
  Sound_Play(195);
  spark->timer = (Random16Far() & 0x7fff) + 0x8000;
  spark->unk66 = (((i & 1) << 1) - 1) << ((Random16Far() & 3) + 9);
  noMotion = 0;
  spark->motionFlags = noMotion;
  spark->unk4c = 0x7a0000;
  spark->watchedObject = second;
  ((union ActorView *)spark)->bytes.unk62 = ((16 - i) << 3) + 15;
  Actor_SetPartAttribute(spark, 2);
  Scene_SetActorSpritePriority(spark, 1);
  Func_02003cc4(spark, 7);
  Actor_SetSingleChildFlags(spark, 0);
  Animation_SetStateFlags(spark, 1);
  FieldVector_AddPolarOffset(spark->unk4c, spark->timer, &spark->x);
  Actor_StartScript(spark, gData_02004004);
  if (i == 3) {
    Actor_SetSpriteColorById(secondId, 256);
    second->motionFlags = noMotion;
    second->acceleration = 0x3333;
    second->speedLimit = 0x3333;
    Actor_SetMovementTypeById(secondId, script);
  }
tail:
  wait(8);
  i++;
  if (i != 16)
    goto spiral;
  Sound_Play(220);
  wait(16);
  i = 2;
column:
  riser = FieldActor_Create(672, second->x, second->y + i * 0x10000 - 0x80000,
                            second->z);
  riser->timer = Random16Far();
  riser->unk66 = 0x800;
  ((union ActorView *)riser)->bytes.unk62 = i;
  ((union ActorView *)riser)->bytes.unk63 = 1;
  riser->bounce = riser->x;
  riser->unk4c = 0;
  riser->gravity = riser->z;
  riser->watchedObject = second;
  riser->motionFlags = (u16)0;
  riser->priorityFlags = 1;
  Actor_SetPartAttribute(riser, 2);
  Func_02003cc4(riser, 7);
  Actor_SetSingleChildFlags(riser, 0);
  Animation_SetStateFlags(riser, 1);
  Actor_StartScript(riser, gData_02004014);
  i += 2;
  if (i != 32)
    goto column;
  Sound_Play(220);
  wait(50);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_ConfigureSpriteColor(firstId, 0);
  Actor_ConfigureSpriteColor(secondId, 0);
  Sound_Play(574);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(80);
  Actor_SetMovementTypeById(firstId, gData_0200407c);
  Actor_SetMovementTypeById(secondId, gData_020040ac);
  second->update = Actor_EmitBubbles;
  wait(120);
  second->update = 0;
  Sound_Play(390);
  Party_PlaySavedMusic();
  wait(1);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_0200124c();
void Event_Start();
void Event_End();
void Actor_MoveToObject();

void Func_02001a1c(void) {
  s32 t4, t59;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)&MsgCheckedTheTabletIt, 1);
  t4 = Event_GetFlag(0x9ae);
  if (t4 == 0)
    goto L_02009a46;
  Event_End();
  goto L_02009c56;
L_02009a46:
  FieldEvent_SetMessageId((s32)&MsgCheckedTheTabletIt);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), s16, 0) =
      (FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1c4), u16, 0) + 1);
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  FieldEvent_Wait(1);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 256, 184);
  Actor_WalkTo(7, 274, 184);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  FieldActor_WaitForMoveById(7);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  FieldEvent_Wait(5);
  Event_ShowMessage(7, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(30);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 184);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(7, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(7, 3);
  FieldEvent_Wait(30);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(7, 0x10000, 0x8000);
  Actor_WalkToAndWait(7, 264, 168);
  Actor_FaceDirection(7, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Message_ShowCentered(((s32)&MsgCheckedTheTabletIt + 2), 1);
  Func_02001620(14, 7);
  Party_LearnAbility(7, 138);
  Event_SetFlag(0x9ae);
  Event_PlaySound(107);
  FieldEvent_Wait(30);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  FieldEvent_Wait(10);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  Actor_ShowEmote(7, 0x100, 0);
  Func_0200124c();
  Event_PlaySound(0x186);
  FieldEvent_Wait(30);
  FieldEvent_SetMessageId((s32)&MsgWhatWentWrong);
  Actor_ShowEmote(7, 0x105, 20);
  Actor_FaceEachOther(gMainRoutineWork.currentOwner, 7, 0);
  FieldEvent_Wait(10);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(7, 0x100, 20);
  Event_ShowMessage(7, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetAnimation(7, 2);
  t59 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if (t59 == 0)
    goto L_02009c42;
  Actor_SetDestination(7, FIELD_AT(t59, s16, 10), FIELD_AT(t59, s16, 18));
L_02009c42:
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
  Event_End();
L_02009c56:
}

#include "call.h"

void Func_02001c68(void) {
  if (Event_GetFlag(0x9ae)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgCheckedTheTabletIt, 1);
    Event_End();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 152);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 184);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 184);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02001a1c();
  }
}

/// 2 copies: aqua_rock_inside/aqua_rock_inside,
/// aqua_rock_inside/aqua_rock_inside.
#define DEFINE_FUNC_02001CD8(name, n1)                                         \
  void name(void) {                                                            \
    if (Event_GetFlag(0x9ae)) {                                                \
      Event_Start();                                                           \
      FieldEvent_ResolvePendingActions(0);                                     \
      Message_ShowCentered(MsgCheckedTheTabletIt, 1);                          \
      Event_End();                                                             \
    } else {                                                                   \
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, n1, 184);             \
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 184);            \
      Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);     \
      FieldEvent_Wait(10);                                                     \
      Func_02001a1c();                                                         \
    }                                                                          \
  }

DEFINE_FUNC_02001CD8(Func_02001cd8, 280)

DEFINE_FUNC_02001CD8(Func_02001d40, 248)

extern const u8 gData_02004c70[];
extern const u8 gData_02004c7c[];
extern const u8 gData_02004d88[];
extern const u8 gData_02004e48[];
extern const u8 gData_02004ed8[];
extern const u8 gData_02004fb0[];
extern const u8 gData_0200507c[];
extern const u8 gData_020051b4[];
extern const u8 gData_020051fc[];
extern const u8 gData_02005268[];
extern const u8 gData_020052f8[];

const u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov67cRoom1)
    return gData_02004c7c;
  if (scene == (s32)&SceneId_Ov67cRoom2)
    return gData_02004d88;
  if (scene == (s32)&SceneId_Ov67cRoom3)
    return gData_02004e48;
  if (scene == (s32)&SceneId_Ov67cRoom4)
    return gData_02004ed8;
  if (scene == (s32)&SceneId_Ov67cRoom5)
    return gData_02004fb0;
  if (scene == (s32)&SceneId_Ov67cRoom6)
    return gData_0200507c;
  if (scene == (s32)&SceneId_Ov67cRoom7)
    return gData_020051b4;
  if (scene == (s32)&SceneId_Ov67cRoom8)
    return gData_020051fc;
  if (scene == (s32)&SceneId_Ov67cRoom9)
    return gData_02005268;
  if (scene == (s32)&SceneId_Ov67cRoom10)
    return gData_020052f8;
  return gData_02004c70;
}

void Func_02001e78(s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object != 0) {
    u8 *meta = ResourceMetadata_Register(object->sprite, 0x17d);

    meta[5] = 0;
    meta[6] = 0;
    Func_02003cc4(object, 2);
  }
}

void Func_02001ea0(struct Actor *object) {
  s32 phase = gFrameCount & 15;

  if (phase == 0) {
    struct Actor *child =
        Import_ActorCreate_(0x10d, object->x, object->y, object->z);

    if (child != 0) {
      struct FieldSprite *sprite = child->sprite;

      Actor_StartScript(child, gData_020040dc);
      child->motionFlags = 0;
      child->terrainId = 1;
      if (sprite != 0) {
        Animation_ApplyChildArgument(sprite, 2);
        sprite->nextFrame = 0;
        sprite->rotation = 0x4000;
        sprite->priority = 2;
      }
    }
  }
}

void Func_02001f04(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u8 *flags = &object->motionFlags;
  s32 saved = *flags;
  s32 speed;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Animation_ApplyChildValues(object, 9);
  *flags = 0;
  object->y = 0x500000;
  speed = 0x30000;
  // A goto loop: the ROM rebuilds the turn step on every pass, which a for loop
  // hoists (differ 43).
  i = 31;
fall:
  object->y -= speed;
  object->facing += 0x2000;
  speed += -0xccc;
  Actor_SetSingleChildFlags(object, 0);
  System_Wait(1);
  i--;
  if (i >= 0)
    goto fall;
  Func_02002bd4(object);
  Animation_ApplyChildValues(object, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
  *flags = saved;
  object->velocityY = 0x80000;
  Event_PlaySound(215);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 168, 200);
  Event_End();
}

void Func_02001fcc(void) {
  s32 counter = Func_02003bcc();

  switch (counter) {
  case 0:
    Func_02003bb8(0, 1);
    Func_02003bb8(1, 1);
    Func_02003ecc(170);
    break;
  case 0x34bbf:
    Func_02003bb8(0, 0);
    Func_02003bb8(1, 0);
    Func_02003ecc(-1);
    break;
  }
  Vents_AdvanceCounter(0x34bc0);
}

extern const u8 gData_020040f4[];
extern const u8 gData_02004102[];
extern const u8 gData_02004106[];
extern const u8 gData_02004114[];

// Returns what its closing Task_Add call leaves in r0.
s32 Func_02003ab0(const u8 *a, const u8 *b, void (*handler)(void));

/// Aqua Rock's entry setup: in each room it shows the statues already filled by
/// their flags (or dims those still empty), opens what the story has opened and
/// arms the room's water and cell handlers by entrance.
s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *object;

  FIELD_AT(Ram_HeapSlots->eventWork, s32, 428) = 0x204;
  if (scene == (s32)&SceneId_Ov67cRoom1) {
    if (GameFlag_Test(0x230)) {
      Call1((void (*)())Func_02001e78, 8);
      Actor_SetAnimation(8, 3);
    } else {
      Actor_SetSpriteColor(Actor_GetById(8), 6);
    }
    if (GameFlag_Test(0x231)) {
      Call1((void (*)())Func_02001e78, 9);
      Actor_SetAnimation(9, 3);
    } else {
      Actor_SetSpriteColor(Actor_GetById(9), 6);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom2) {
    if (GameFlag_Test(0x240)) {
      MapGrid_CopyCellsLayered(118, 15, 5, 32, 110, 15);
      MapGrid_CopyCellAttributes(59, 16, 5, 32, 46, 16);
      Actor_SetSpriteColor(Actor_GetById(8), 6);
    } else {
      Call1((void (*)())Func_02001e78, 8);
      Actor_SetAnimation(8, 3);
    }
    object = Actor_GetById(9);
    object->motionFlags = 0;
    if (GameFlag_Test(0x95c)) {
      Actor_SetPosition(9, PIXELS(632), PIXELS(152));
      MapGrid_CopyCellAttributes(41, 9, 1, 1, 39, 9);
      object->y = 0;
      object->terrainHeight = 0;
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom3) {
    if (entrance == 7)
      Leader_RiseFromWater();
  } else if (scene == (s32)&SceneId_Ov67cRoom4) {
    Call1((void (*)())Func_02001e78, 10);
    Actor_SetAnimation(10, 3);
    Call1((void (*)())Func_02001e78, 11);
    Actor_SetAnimation(11, 3);
    if ((u32)(entrance - 6) <= 1) {
      if (GameFlag_Test(0x249)) {
        Actor_SetSpriteColor(Actor_GetById(9), 5);
        Call1((void (*)())Func_02001e78, 9);
        Actor_SetAnimation(9, 3);
        Func_02000b5c();
      } else {
        Actor_SetSpriteColor(Actor_GetById(9), 6);
      }
    }
    FIELD_AT(Actor_GetById(13), u8, 98) = 1;
    FIELD_AT(Actor_GetById(14), u8, 98) = 1;
    object = Actor_GetById(64);
    if (object != 0) {
      object->motionFlags = 0;
      object->terrainHeight = -0x200000;
      object->y = -0x200000;
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom5) {
    if ((u32)(entrance - 5) <= 1) {
      Func_02003ab0(gData_020040f4, gData_02004102, Func_02001fcc);
      Func_02003ecc(170);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom6) {
    if (entrance == 5 || entrance == 7) {
      Func_02003ab0(gData_02004106, gData_02004114, Func_02001fcc);
      Func_02003ecc(170);
      Func_0200107c(0, 10);
    }
    if (!GameFlag_Test(0x238)) {
      Func_02001e78(9);
      Actor_SetAnimation(9, 3);
    }
    object = Actor_GetById(12);
    Actor_SetSingleChildFlags(object, 0);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    object->update = (void (*)(void))Func_02001ea0;
    if (!GameFlag_Test(0x109) && entrance == 13)
      Func_02001f04();
  } else if (scene == (s32)&SceneId_Ov67cRoom9) {
    if (GameFlag_Test(0x24b)) {
      Actor_SetSpriteColor(Actor_GetById(9), 5);
      Func_02001e78(9);
      Actor_SetAnimation(9, 3);
    } else {
      Actor_SetSpriteColor(Actor_GetById(9), 6);
    }
    FIELD_AT(Actor_GetById(10), u8, 98) = 1;
    FIELD_AT(Actor_GetById(11), u8, 98) = 1;
    FIELD_AT(Actor_GetById(14), u8, 98) = 1;
    FIELD_AT(Actor_GetById(16), u8, 98) = 1;
    FIELD_AT(Actor_GetById(17), u8, 98) = 1;
    FIELD_AT(Actor_GetById(18), u8, 98) = 1;
  }
  return 0;
}

#include "iwram_call.h"
#include "text/msg_ids.h"

// The original keeps the masked BG1CNT in a stack slot it never reads again:
// control is volatile, and the register is written in a once-only block.
#define BG1_SET_PRIORITY_1()                                                   \
  {                                                                            \
    u16 value;                                                                 \
                                                                               \
    value = (control = REG_BG1CNT & 0xfffc);                                   \
    do {                                                                       \
      REG_BG1CNT = value | 1;                                                  \
    } while (0);                                                               \
  }

void Func_02000b84(void);
void Func_02003edc();

/// Aqua Rock's map preparation: for each room and entrance it sets the water
/// level, places the statues and boulders the flags have moved, opens or
/// closes the cells around them, and in the rooms whose water drains it shows
/// the drained state once its flag is set. Room 6 clears its actors' x and z
/// where the other rooms clear terrainHeight and y.
s32 Scene_PrepareMap(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  volatile u16 control;
  // i1, i3, i4, i6, i7, i12, i14, i17 and b each hold one constant for their
  // whole life, i2 holds a flag test and the others each count one loop.
  s32 i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15, i16,
      i17;
  struct Actor *actor;
  s32 a, b;
  u8 *map;

  ((struct MapBits *)Ram_HeapSlots->mapWork)->mode = 1;
  Func_02003278();
  if (scene == (s32)&SceneId_Ov67cRoom1) {
    if ((u32)(entrance - 3) <= 1 || entrance == 6 || entrance == 8) {
      Func_020032d4(33, 0, -2);
    } else if (entrance == 5 || entrance == 7) {
      actor = Actor_GetById(12);
      i1 = 0;
      actor->motionFlags = i1;
      actor->terrainHeight = i1;
      actor->y = i1;
      Call2((void (*)())Func_02003cc4, (s32)actor, 0);
      actor = Actor_GetById(13);
      actor->motionFlags = i1;
      actor->terrainHeight = i1;
      actor->y = i1;
      i2 = GameFlag_Test(0x230);
      if (i2 == 0) {
        MapGrid_CopyCellsLayered(104, 80, 16, 10, 104, 40);
        MapGrid_CopyCellsLayered(110, 116, 18, 12, 39, 105);
        // i2 is the flag test's 0.
        MapGrid_CopyCellAttributes(64, 32, 64, 32, i2, 32);
        Func_020032d4(33, 0, 0);
      } else {
        Call3((void (*)())Func_020032d4, 33, 4, 0);
        actor = Actor_GetById(12);
        Call2((void (*)())Func_0200349c, (actor->x >> 20) - 42, 0);
      }
      if (GameFlag_Test(0x250)) {
        MapGrid_CopyCellAttributes(41, 42, 1, 1, 43, 44);
      }
    } else if ((u32)(entrance - 9) <= 1) {
      a = GameFlag_Test(0x231);
      if (a == 0) {
        i3 = 32;
        MapGrid_CopyCellsLayered(77, 80, 23, 20, 77, i3);
        MapGrid_CopyCellsLayered(112, 96, 16, 20, 12, 98);
        // a is the flag test's 0.
        MapGrid_CopyCellAttributes(64, 32, 64, 32, a, i3);
        Func_020032d4(33, 0, 0);
      } else {
        Func_020032d4(33, 4, 0);
      }
    }
    if (GameFlag_Test(0x95a)) {
      Actor_SetPosition(10, PIXELS(184), PIXELS(200));
      i4 = 12;
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 9, i4);
      MapGrid_CopyCellAttributes(0, 12, 1, 1, 13, i4);
    }
    if (GameFlag_Test(0x95b)) {
      Actor_SetPosition(11, PIXELS(824), PIXELS(216));
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom2) {
    if ((u32)(entrance - 1) <= 1) {
      Func_020032d4(33, 0, -2);
    }
    actor = Actor_GetById(8);
    actor->motionFlags = 0;
    actor->terrainHeight = 0;
    actor->y = 0;
  } else if (scene == (s32)&SceneId_Ov67cRoom3) {
    Call1((void (*)())Func_02003ea4, 0);
    Call2((void (*)())Actor_SetSingleChildFlags, (s32)Actor_GetById(9), 0);
    Call2((void (*)())Actor_SetSingleChildFlags, (s32)Actor_GetById(10), 0);
    Func_020032d4(33, 8, -2);
    Call3((void (*)())Func_020032d4, 40, 8, 8);
    if (GameFlag_Test(0xa66)) {
      Func_02003628();
      Value4((s32 (*)())Func_02003688, 16, 4, 16, 2);
      Value4((s32 (*)())Func_02003688, 22, 4, 16, 0);
      for (i5 = 0; i5 < 6; i5++)
        Func_020036b8(33, 55);
    }
    if (GameFlag_Test(0x95d)) {
      MapGrid_CopyCellsLayered(75, 96, 7, 13, 80, 27);
      i6 = 16;
      b = 31;
      MapGrid_CopyCellsLayered(91, 96, 7, 10, i6, b);
      MapGrid_CopyCellsLayered(99, 96, 7, 14, i6, 91);
      MapGrid_CopyCellAttributes(43, 0, 5, 9, 17, b);
      Func_020007a8();
    } else {
      MapGrid_CopyCellAttributes(17, 32, 5, 1, 17, 31);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom4) {
    Call1((void (*)())Func_02003ea4, 0);
    if ((u32)(entrance - 6) <= 1)
      goto statues;
    goto no_statues;
  statues:
    {
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
      Value3((s32 (*)())Func_02003420, 12, 5, 4);
      Value3((s32 (*)())Func_02003420, 13, 3, 4);
      Value3((s32 (*)())Func_02003420, 14, 4, 4);
      Func_020032d4(40, 4, 4);
      if (!GameFlag_Test(0x249) && !Event_GetFlag(0xa64)) {
        map = (u8 *)Ram_HeapSlots->mapWork;
        i7 = 39;
        MapGrid_CopyCellsLayered(64, 64, 16, 13, 71, i7);
        MapGrid_CopyCellAttributes(71, 39, 16, 15, 7, i7);
        Func_02000b84();
        map[262] = 1;
        BG1_SET_PRIORITY_1();
      } else if (Event_GetFlag(0xa64) || GameFlag_Test(0x248)) {
        if (Event_GetFlag(0xa64)) {
          Actor_SetPosition(12, PIXELS(280), PIXELS(712));
        }
        Func_02000b24(12, 38);
        Func_02003628();
        Value4((s32 (*)())Func_02003688, 12, 2, 45, 1);
        for (i8 = 0; i8 < 8; i8++)
          Func_020036b8(33, 55);
        Event_SetFlag(0x249);
      }
      i9 = 12;
      do {
        actor = Actor_GetById(i9);
        actor->motionFlags = 0;
        actor->terrainHeight = 0;
        actor->y = 0;
        i9++;
      } while (i9 <= 14);
      goto statues_cleared;
    }
  no_statues:
    Func_020032d4(33, 0, -4);
  statues_cleared:
    if (GameFlag_Test(0xa6a)) {
      Func_02000ad8();
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom5) {
    Func_020032d4(33, 0, -2);
    Call3((void (*)())Func_020032d4, 40, 0, 0);
    Call1((void (*)())Func_02003ea4, 0);
    if (((u32)(entrance - 1) <= 3 || entrance == 7) && GameFlag_Test(0xa67)) {
      Func_02003628();
      Value4((s32 (*)())Func_02003688, 44, 0, 34, 2);
      for (i10 = 0; i10 <= 4; i10++)
        Func_020036b8(33, 55);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom6) {
    Call1((void (*)())Func_02003ea4, 0);
    if (entrance == 1 || entrance == 13) {
      Func_020032d4(33, 0, -2);
    } else if (entrance == 9 || entrance == 11 || entrance == 12) {
      Func_020032d4(33, 0, -2);
    }
    if (!GameFlag_Test(0x238)) {
      i11 = 13;
      do {
        actor = Actor_GetById(i11);
        actor->motionFlags = 0;
        actor->x = 0;
        actor->z = 0;
        i11++;
      } while (i11 <= 15);
      goto done;
    } else {
      MapGrid_CopyCellsLayered(114, 39, 1, 2, 113, 39);
      i12 = 41;
      MapGrid_CopyCellAttributes(105, 41, 12, 13, i12, i12);
      MapGrid_CopyCellsLayered(105, 105, 12, 13, 105, i12);
      for (i13 = 13; i13 <= 15; i13++) {
        actor = Actor_GetById(i13);
        if (GameFlag_Test(i13 + 580)) {
          MapGrid_CopyCellAttributes(47, 45, 1, 1, actor->x >> 20,
                                     actor->z >> 20);
        }
      }
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom7) {
    Func_020032d4(33, 0, -2);
  } else if (scene == (s32)&SceneId_Ov67cRoom8) {
    Call1((void (*)())Func_02003ea4, 0);
    Func_020032d4(33, 0, -2);
    Func_02003edc();
    Call3((void (*)())Func_02003ee4, 0, 8, 9);
  } else if (scene == (s32)&SceneId_Ov67cRoom9) {
    if ((u32)(entrance - 3) <= 1) {
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
      Value3((s32 (*)())Func_02003420, 10, 6, 4);
      Value3((s32 (*)())Func_02003420, 11, 3, 4);
      Value3((s32 (*)())Func_02003420, 12, 5, 4);
      Value3((s32 (*)())Func_02003420, 13, 6, 4);
      Value3((s32 (*)())Func_02003420, 14, 5, 4);
      Value3((s32 (*)())Func_02003420, 15, 4, 4);
      Value3((s32 (*)())Func_02003420, 16, 4, 4);
      Value3((s32 (*)())Func_02003420, 17, 3, 4);
      Value3((s32 (*)())Func_02003420, 18, 6, 4);
      Func_020032d4(40, 4, 4);
      if (!GameFlag_Test(0x24b) && !Event_GetFlag(0xa65)) {
        map = (u8 *)Ram_HeapSlots->mapWork;
        i14 = 13;
        MapGrid_CopyCellsLayered(64, 64, 19, 17, 100, i14);
        MapGrid_CopyCellsLayered(35, 105, 23, 24, 35, 73);
        MapGrid_CopyCellAttributes(100, 13, 19, 19, 36, i14);
        map[262] = 1;
        BG1_SET_PRIORITY_1();
      } else if (Event_GetFlag(0xa65) || GameFlag_Test(0x24a)) {
        if (Event_GetFlag(0xa65)) {
          Actor_SetPosition(12, PIXELS(808), PIXELS(296));
          Actor_SetPosition(13, PIXELS(680), PIXELS(360));
          Actor_SetPosition(15, PIXELS(744), PIXELS(424));
        }
        Call2((void (*)())Func_02000b24, 12, 38);
        Call2((void (*)())Func_02000b24, 13, 39);
        Func_02000b24(15, 37);
        Func_02003628();
        Value4((s32 (*)())Func_02003688, 38, 2, 19, 1);
        for (i15 = 0; i15 < 8; i15++)
          Func_020036b8(33, 55);
        Event_SetFlag(0x24b);
      }
    }
    i16 = 10;
  clear_pillars:
    actor = Actor_GetById(i16);
    actor->motionFlags = 0;
    actor->terrainHeight = 0;
    actor->y = 0;
    i16++;
    if (i16 <= 20)
      goto clear_pillars;
    actor = Actor_GetById(19);
    MapGrid_CopyCellAttributes(23, 39, 1, 1, actor->x >> 20, 39);
    if ((u32)(entrance - 1) <= 1) {
      Call1((void (*)())Func_02003ea4, 0);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom10) {
    Call1((void (*)())Func_02003ea4, 0);
    Func_020032d4(33, 0, -2);
    Event_SetFlag(0x144);
    if (GameFlag_Test(0x349)) {
      MapGrid_CopyCellAttributes(38, 9, 7, 6, 13, 9);
    } else {
      i17 = 8;
      MapGrid_CopyCellsLayered(77, 8, 7, 6, 50, i17);
      MapGrid_CopyCellsLayered(70, 8, 7, 6, 77, i17);
      MapGrid_CopyCellAttributes(38, 17, 7, 6, 13, 9);
    }
  }
done:
  return 0;
}

#include "system/frame.h"

void Func_02002bd4(struct Actor *object) {
  struct Actor *child = FieldActor_Create(14, object->x, object->y + 0x40000,
                                          object->z + 0x40000);
  struct FieldSprite *source = object->sprite;

  if (child != 0) {
    struct FieldSprite *sprite = child->sprite;

    child->terrainHeight = object->terrainHeight;
    Actor_StartScript(child, gData_0200411c);
    child->motionFlags = 0;
    if (sprite != 0) {
      Animation_ApplyChildArgument(sprite, 1);
      sprite->nextFrame = 0;
      sprite->priority = source->priority;
    }
  }
}

DEFINE_ACTOR_EMIT_WATER_DROP(Actor_EmitWaterDrop, gWaterDropScript)

void Func_02002ca4(void) {
  struct Actor *center;
  s32 id;
  struct Actor *actor;
  s32 position[3];
  s32 hit;

  {
    struct SlideWork *work = Ram_HeapSlots->eventWork;

    id = gMainRoutineWork.currentOwner;
    center = work->camera;
  }
  actor = Actor_Get(id);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67bRoom1 &&
      actor->terrainHeight == 0)
    return;
  position[0] = actor->x;
  position[1] = actor->y;
  position[2] = actor->z + 0x20000;
  hit = Func_02003d2c(actor, position);
  if ((gFrameCount & 4) == 0)
    Func_02002bd4(actor);
  if (hit < 0) {
    Actor_SetAttachedEffect(id, 258);
    FieldActor_SetMoveTarget(actor, actor->x, actor->y, actor->z + 0x80000);
    Func_02003cc4(actor, 49);
    Actor_WaitForMove(actor);
    do {
      System_Wait(1);
    } while (actor->y != actor->terrainHeight);
    Func_02002bd4(actor);
    Func_02003cc4(actor, 49);
    System_Wait(3);
    return;
  }
  position[0] = actor->x;
  position[1] = actor->y;
  position[2] = actor->z + 0x80000;
  hit = Func_02003d2c(actor, position);
  if (hit > 0)
    return;
  position[0] = actor->x + 0x5b333;
  position[1] = actor->y;
  position[2] = actor->z + 0x5b333;
  hit = Func_02003d2c(actor, position);
  if (hit > 0)
    return;
  position[0] = actor->x - 0x5b333;
  position[1] = actor->y;
  position[2] = actor->z + 0x5b333;
  hit = Func_02003d2c(actor, position);
  if (hit > 0)
    return;
  center->z += 0x20000;
  actor->z += 0x20000;
  actor->facing = 0x4000;
}

DEFINE_ACTOR_SHOW_PARTS(Actor_ShowParts, Func_02003cc4)

DEFINE_LEADER_SPIN_AWAY(Leader_SpinAway, Func_02003cc4, Func_02002bd4)

/// On odd frames, spawns a drifting effect above the object, swaying with the
/// frame count; Func_02002030 of Aqua Rock is the same function.
DEFINE_ACTOR_EMIT_MIST(Actor_EmitMist)

DEFINE_LEADER_SINK_AWAY(Leader_SinkAway, Actor_EmitMist)

DEFINE_LEADER_RISE_FROM_WATER(Leader_RiseFromWater, Actor_EmitMist)

#include "map.h"
#include "vram_tab.h"

/// Draws the scene's pillars: a top sprite for each active one and, when it
/// stands above its base, a column sprite as tall as its height.
void Func_020030f4(void) {
  s32 *view = &Ram_HeapSlots->mapWork->viewX;
  struct PillarWork *work = &gData_0200536c;
  s32 viewX = view[0];
  struct PillarEntry *entry = work->entries;
  s32 viewZ;
  s32 originY;
  s32 depth;
  s32 i;
  u32 tile;

  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = Ram_HeapSlots->mapWork->origin[1];
  tile = gResourceTableEntries[work->vramEntry].offset >> 5;

  for (i = 0; i < 32; i++) {
    if (entry->state != 0) {
      s32 *position = entry->position;
      s32 x;
      s32 y;
      s32 z;
      s32 screenY;
      s32 attributes;

      if (position != 0) {
        x = position[0] - viewX;
        y = position[1] + entry->height - originY;
        z = position[2] - viewZ - originY;
        screenY = z - y;
      } else {
        x = entry->x - viewX;
        y = entry->height - originY;
        z = entry->z - viewZ - originY;
        screenY = z - y;
      }
      if (entry->height != entry->base || entry->state == 2)
        attributes = 0x800;
      else
        attributes = 0xc00;
      x = (x >> 16) - 8;
      screenY = (screenY >> 16) - 8;
      depth = ((y + z) >> 16) + 50;
      if ((u32)(x + 15) <= 270) {
        s32 rise;

        if ((u32)(screenY + 47) <= 238) {
          x &= 511;
          screenY &= 255;
          rise = entry->kind * 8;
          entry->top[0] = 0;
          screenY |= x << 16;
          screenY |= 0x40002000;
          entry->top[1] = screenY;
          entry->top[2] = (tile + rise) | attributes;
          Import_OamAddSpriteAtDepth(entry->top, depth);
        }
        screenY = z - y;
        screenY = (screenY >> 16) + 8;
        if ((u32)(screenY + 47) <= 238) {
          rise = (entry->height - entry->base) >> 16;
          if (rise != 0) {
            u32 *column;
            rise--;
            screenY &= 255;
            column = entry->column;
            if ((u32)rise > 7)
              rise = 7;
            rise = rise * 4 + 64;
            column[0] = 0;
            screenY |= x << 16;
            screenY |= 0x6000;
            entry->column[1] = screenY;
            entry->column[2] = attributes | (tile + rise);
            Import_OamAddSpriteAtDepth(column, depth);
          }
        }
      }
    }
    entry++;
  }
}

void Func_02003278(void) {
  void *buffer;
  s32 priority;

  Iwram_ClearWordsEntry(&gData_0200536c, 0x604);
  buffer = MemAlloc_(0xc80);
  Resource_DecodeType01(gData_0200414c, buffer);
  gData_0200536c.vramEntry = Resource_FindFreeEntry();
  Vram_Load(gData_0200536c.vramEntry, 0xc80, buffer);
  Heap_Release(buffer);
  priority = 0x41c;
  FieldTask_AddFrame(Func_020030f4, priority);
}

#define LAYER_S32(layer, off) (*(s32 *)((layer)->unk08 + (off) - 8))

// Adds a water spout on every map cell of the given layer whose spout kind lies
// in [first, first + 8), at the given heights, and marks those cells as spout
// cells (120). The loops are built from labels: both 0x80000 loads stay in the
// body and neither counter is strength-reduced.
void Func_020032d4(u32 first, s32 level, s32 baseLevel) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayer *layer = map->layers;
  struct MapCell *cells = layer->cells;
  s32 originX = LAYER_S32(layer, 8) + map->minX;
  s32 originZ = LAYER_S32(layer, 12) + map->minZ;
  struct PillarWork *work = &gData_0200536c;
  s32 width = (map->maxX - map->minX) >> 20;
  s32 depth = (map->maxZ - map->minZ) >> 20;
  struct PillarEntry *spout = &work->entries[work->count];
  s32 corner = ((map->minZ >> 20) << 7) + (map->minX >> 20);
  s16 row;

  *(s32 *)(Data_0202c001 + 0x1df) = level << 8;
  cells += corner;
  row = 0;
  if (row < depth) {
    struct MapCell *cell;
    s16 column;
  rowTop:
    cell = cells + ((u16)row << 7);
    column = 0;
    if (column < width) {
    columnTop:
      {
        u32 kind = cell->collisionCode;

        if (kind != 0 && kind >= first && kind < first + 8) {
          spout->x = ((u16)column << 20) + originX + 0x80000;
          spout->z = ((u16)row << 20) + originZ + 0x80000;
          spout->base = baseLevel << 19;
          spout->height = level << 19;
          spout->kind = kind - first;
          spout->position = 0;
          spout->state = 1;
          spout++;
          work->count++;
          {
            u8 *terrain = map->layers[0].terrainIds + corner;

            terrain[(u16)column + ((u16)row << 7)] = 120;
          }
        }
      }
      {
        s32 next = (column << 16) + 0x10000;
        column = next >> 16;
        cell++;
        if ((s32)((u32)next >> 16) < width)
          goto columnTop;
      }
    }
    {
      s32 next = (row << 16) + 0x10000;
      row = next >> 16;
      if ((s32)((u32)next >> 16) < depth)
        goto rowTop;
    }
  }
}

void Func_02003420(s32 id, s32 kind, s32 height) {
  struct Actor *object = Actor_Get(id);
  struct PillarWork *work = &gData_0200536c;
  struct PillarEntry *entry = &work->entries[work->count];

  if (object != 0) {
    entry->x = 0;
    entry->z = 0;
    entry->base = height << 19;
    entry->height = height << 19;
    entry->state = 2;
    entry->kind = kind;
    entry->position = &object->x;
    work->count++;
  }
}

void Func_02003460(s32 height) {
  struct PillarWork *work = &gData_0200536c;
  struct PillarEntry *entry;
  s32 i;

  *(s32 *)(Ram_MapCollision + 0x1e0) = ((height / 0x80000) & 0xff) << 8;
  entry = work->entries;
  for (i = 31; i >= 0; i--) {
    if (entry->x != 0)
      entry->height = height;
    entry++;
  }
}

void Func_0200349c(u32 index, s32 height) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct PillarWork *work = &gData_0200536c;

  if (index <= 31) {
    struct PillarEntry *entry = &work->entries[index];

    if (entry->x != 0) {
      *(s32 *)(Ram_MapCollision + 0x1e4) = ((height / 0x80000) & 0xff) << 8;
      {
        u8 *ids = map->layers[0].terrainIds;
        s32 at = (entry->x >> 20) + (entry->z >> 20) * 128;
        s32 v = 121;
        ids[at] = v;
      }
      entry->height = height;
    }
  }
}

/// Draws thirty-two sprites that each shake by a random few pixels a frame.
void Func_020034fc(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  struct BeamWork *splashes = &gData_02005970;
  s32 viewX = view[0];
  s32 viewZ;
  s32 originY;
  struct Splash *splash;
  s32 i;
  u32 tile;
  // The sprite word's priority bits (priority 2): a local that holds this one
  // value.
  s32 priority = 0x800;

  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = map->origin[1];
  tile = gResourceTableEntries[splashes->vramEntry].offset >> 5;
  splash = splashes->splashes;
  i = 31;

  do {
    if (splash->active != 0) {
      s32 x = splash->x - viewX;
      s32 y = splash->y - originY;
      s32 z = splash->z;
      s32 screenY, rawX, rawY;
      z -= viewZ;
      z -= originY;
      screenY = z - y;
      rawX = (x >> 16) + ((Random16Far() * 10) >> 16);

      x = rawX - 10;
      rawY = (screenY >> 16) + ((Random16Far() * 10) >> 16);
      screenY = rawY - 10;
      z = ((y + z) >> 16) + 50;

      if ((u32)(x + 15) <= 270 && (u32)(screenY + 47) <= 238) {
        u32 *frame = &splash->sprite[2];
        s32 column = x & 511;

        screenY &= 255;
        splash->sprite[0] = 0;
        splash->sprite[1] = screenY | (column << 16) | 0x40002000;
        *frame = (tile + (((Random16Far() * 3) >> 16) << 3)) | priority;
        Import_OamAddSpriteAtDepth(splash->sprite, z);
      }
    }
    i--;
    splash++;
  } while (i >= 0);
}

#include "macros.h"

void Func_02003628(void) {
  void *buffer;
  s32 priority;

  Iwram_ClearWordsEntry(&gData_02005970, 0x3a4);
  buffer = MemAlloc_(0x400);
  Resource_DecodeType01(gData_02004351, buffer);
  gData_02005970.vramEntry = Resource_FindFreeEntry();
  Vram_Load(gData_02005970.vramEntry, 0x400, buffer);
  Heap_Release(buffer);
  priority = 0x41c;
  FieldTask_AddFrame(Func_020034fc, priority);
  Event_PlaySound(220);
}

s32 Func_02003688(s32 x, s32 y, s32 z, s32 kind) {
  struct BeamWork *work = &gData_02005970;
  struct BeamEntry *entry = work->entries;

  if (entry->x != 0)
    entry++;
  SET_POSITION(entry, (x << 20) + 0x80000, y << 20, (z << 20) + 0x80000);
  entry->direction = kind;
}

extern const s8 gData_0200534c[];

// Advances Aqua Rock's two water streams one cell along the map: each stream
// follows the turn its cell's kind gives, leaves a splash on every open cell
// (255) it crosses, stops on a wall kind and counts the cells of the given kind
// it reaches; then it clears the splashes past the new count and opens the
// cells under those left.
s32 Func_020036b8(s32 first, s32 target) {
  s32 reached = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct BeamWork *state = &gData_02005970;
  s32 count = state->count;
  s32 turn;
  s32 again = 0;
  s32 i;
  struct BeamEntry *stream = state->entries;
  struct Splash *splash;

  for (i = 0; i < 2; i++) {
    if (stream->x != 0) {
      splash = &state->splashes[state->count];
      do {
        struct MapCell *cells = map->layers[0].cells;
        struct MapCell *cell =
            &cells[(stream->x >> 20) + (stream->z >> 20) * 128];
        s32 kind = cell->collisionCode;

        if (kind == 255) {
          splash->x = stream->x;
          splash->z = stream->z;
          splash->y = stream->y;
          splash->active = 1;
          splash++;
          state->count++;
          turn = 0;
          again = 1;
        } else if (kind >= first && kind <= first + 7) {
          turn = gData_0200534c[(stream->direction << 3) + kind - first];
          if (turn != 8)
            count = state->count;
          again = 0;
        } else if (kind == target) {
          turn = 8;
          count = state->count;
          reached++;
          again = 0;
        } else if (kind == 0) {
          turn = 8;
          again = 0;
        }
        if (turn != 8) {
          // The unsigned % 4: as & 3 the mask narrows to a halfword and pools.
          stream->direction = (u32)(stream->direction + turn) % 4;
          FieldVector_AddPolarOffset(0x100000, stream->direction << 14,
                                     &stream->x);
        }
      } while (again != 0);
      state->count = count;
    }
    stream++;
  }
  splash = &state->splashes[count];
  if (count < 32) {
    i = 32 - count;
  clear:
    if (splash->active != 0)
      splash->active = 0;
    splash++;
    if (--i != 0)
      goto clear;
  }
  splash = state->splashes;
  if (count != 0) {
    i = count;
  open:
    if (splash->active != 0) {
      struct MapCell *cells = map->layers[0].cells;
      struct MapCell *cell =
          &cells[(splash->x >> 20) + (splash->z >> 20) * 128];

      cell->collisionCode = 0;
      cell->attributeB |= 0x80;
    }
    splash++;
    if (--i != 0)
      goto open;
  }
  return reached;
}
#undef FIELD_AT
#undef BG1_SET_PRIORITY_1
#undef LAYER_S32

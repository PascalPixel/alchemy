/// @file
/// Scene code of overlay 0x661, in ROM order, from placements' offset up to the next object the overlay links.

#include "scene.h"

extern const u8 gData_02004810[];
extern const u8 gData_02004840[];
extern const u8 gData_020048b8[];
extern const u8 gData_020048d8[];

void Func_02000280(void) { Func_02004148(19, 68); }

const u8 *Func_0200028c(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room11)
    return gData_02004840;
  return gData_02004810;
}

const u8 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov661Room10)
    return gData_020048b8;
  if (scene == (s32)&SceneId_Ov661Room11)
    return gData_020048d8;
  return 0;
}

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

const u32 *Scene_GetExits(void) { return gSceneExits; }


#include "text/msg_ids.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"

extern const u8 gData_020049c4[];
extern const u8 gData_020049dc[];
extern const u8 gData_02004a84[];
extern const u8 gData_02004b44[];
extern const u8 gData_02004c04[];
extern const u8 gData_02004d3c[];
extern const u8 gData_02004e44[];
extern const u8 gData_02004fdc[];
extern const u8 gData_0200506c[];
extern const u8 gData_020050b4[];

const u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov661Room1)
    return gData_020049dc;
  if (scene == (s32)&SceneId_Ov661Room2) {
    if (GameFlag_Test(0x8f0))
      return gData_02004b44;
    return gData_02004a84;
  }
  if (scene == (s32)&SceneId_Ov661Room3)
    return gData_02004c04;
  if (scene == (s32)&SceneId_Ov661Room4)
    return gData_02004d3c;
  if (scene == (s32)&SceneId_Ov661Room5)
    return gData_02004e44;
  if (scene == (s32)&SceneId_Ov661Room8)
    return gData_02004fdc;
  if (scene == (s32)&SceneId_Ov661Room10)
    return gData_0200506c;
  if (scene == (s32)&SceneId_Ov661Room12)
    return gData_020050b4;
  return gData_020049c4;
}

void Func_020003b8(s32 a) { Scene_Call4178(a, 64, 0x302); }

extern const u8 gData_0200518c[];
extern const u8 gData_02005198[];
extern const u8 gData_0200521c[];
extern const u8 gData_020053fc[];
extern const u8 gData_020054e0[];
extern const u8 gData_02005594[];
extern const u8 gData_02005630[];
extern const u8 gData_02005678[];
extern const u8 gData_020056b4[];
extern const u8 gData_02005750[];
extern const u8 gData_0200578c[];
extern const u8 gData_020057bc[];
extern const u8 gData_020057e0[];

/// Returns the current room's table.
// As ☀️'s Scene_GetEntrances picks one.
const u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov661Room1)
    return gData_02005198;
  if (scene == (s32)&SceneId_Ov661Room2)
    return gData_0200521c;
  if (scene == (s32)&SceneId_Ov661Room3)
    return gData_020053fc;
  if (scene == (s32)&SceneId_Ov661Room4)
    return gData_020054e0;
  if (scene == (s32)&SceneId_Ov661Room5)
    return gData_02005594;
  if (scene == (s32)&SceneId_Ov661Room6)
    return gData_02005630;
  if (scene == (s32)&SceneId_Ov661Room7)
    return gData_02005678;
  if (scene == (s32)&SceneId_Ov661Room8)
    return gData_020056b4;
  if (scene == (s32)&SceneId_Ov661Room9)
    return gData_02005750;
  if (scene == (s32)&SceneId_Ov661Room10)
    return gData_0200578c;
  if (scene == (s32)&SceneId_Ov661Room11)
    return gData_020057bc;
  if (scene == (s32)&SceneId_Ov661Room12)
    return gData_020057e0;
  return gData_0200518c;
}

void Func_020004bc(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgWeFollowedThe);
  Func_020040d8(8, 0);
  Event_End();
}

void Func_020004e0(void) {
  Event_SetMessageId((s32)MsgCanTWe);
  Actor_SetModeById(12, 4);
  Event_ShowMessageAndWait(12, 0);
  Scene_ArmCallback(13, FACING_WEST, 20);
  Actor_SetModeById(13, 4);
  Event_ShowMessageAndWait(13, 0);
  Actor_ArmCallback(13, 0, 20);
  Scene_ArmCallback(13, 0x3000, 20);
  Actor_ArmCallback(13, 0, 20);
  Scene_ArmCallback(13, 0x3000, 0);
  Event_ShowMessageAndWait(13, 0);
}

void Func_02000550(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (!Scene_FlagTest(0x88f))
    Func_020004e0();
  Actor_SetAngleToward(12, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgAsYouCan);
  Event_ShowMessageAndWait(12, 0);
  GameFlag_SetBit(0x88f);
  Event_End();
}

void Func_020005a8(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (!Scene_FlagTest(0x88f))
    Func_020004e0();
  Actor_SetAngleToward(13, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgThisIsWhere);
  Event_ShowMessageAndWait(13, 0);
  GameFlag_SetBit(0x88f);
  Event_End();
}

void Func_02000600(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgNorthAlhafraSouth, 1);
  Event_End();
}

void Func_02000620(s32 a, s32 b) { Func_0200388c(gData_02004768, a, b); }

void Func_02000634(s32 a, s32 b) { Func_0200388c(gData_0200478e, a, b); }

void Func_02000648(s32 a, s32 b) {
  Func_0200388c(gData_020047d2, a, b);
  if (a == 1) {
    if (gData_02004808 == 0)
      gData_02004808 = 210;
  }
}

void Func_02000670(s32 a, s32 b) { Func_0200388c(gData_02004800, a, b); }

void Func_02000684(s32 a, s32 b) {
  Func_0200388c(gData_020047ec, a, b);
  if (a == 1) {
    if (gData_02004808 == 0)
      gData_02004808 = 210;
  }
}

extern u8 gMapShapeGrid[];

s32 Func_02004020(s32 layer, s32 x, s32 z);

s32 Func_020041e8(s32 layer, s32 shape, s32 height);

void Func_020006ac(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u16 *list = NULL;
  u8 *grid = gMapShapeGrid;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room3)
    list = gData_0200478e;
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room4)
    list = gData_020047d2;
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room5)
    list = gData_020047ec;
  if (list == NULL)
    return;
  while (*list != 0xffff) {
    struct Actor *object = Actor_GetById(*list);

    list += 2;
    if (!GameFlag_Test(*list)) {
      s32 layer = object->terrainId;
      struct MapCell *cells = map->layers[layer].cells;
      u8 *shapes =
          (u8 *)((((s32)cells - (s32)Ram_MapCellBuffer) >> 2) + (s32)grid);
      s32 index = Func_020041e0(object);
      s32 shape = Func_02004020(layer, object->x, object->z);
      s32 height = Map_GetTerrainHeight(layer, object->x, object->z);

      cells += index;
      shapes += index;
      object->motionFlags = 0;
      cells->collisionCode = 0;
      cells->attributeB |= 128;
      *shapes = Func_020041e8(layer, shape, (height >> 19) + 4);
    }
    list++;
  }
  GameFlag_SetBit(0x20b);
}

void Func_020007e0(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u16 *list = NULL;
  u8 *grid = gMapShapeGrid;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room3)
    list = gData_0200478e;
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room4)
    list = gData_020047d2;
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room5)
    list = gData_020047ec;
  if (list == NULL)
    return;
  if (*list == 0xffff)
    goto done;
next:
  {
    struct Actor *object = Actor_GetById(*list);
    s32 set;

    list += 2;
    set = GameFlag_Test(*list);
    if (!set) {
      s32 layer = object->terrainId;
      struct MapCell *cells = map->layers[layer].cells;
      u8 *shapes =
          (u8 *)((((s32)cells - (s32)Ram_MapCellBuffer) >> 2) + (s32)grid);
      /* FAKEMATCH(pin): the index in r10; as a plain local, 32 of 130 instructions
       * differ in tla-en. */
      register s32 index asm("r10") = Func_020041e0(object);
      s32 shape = Func_02004020(layer, object->x, object->z);
      s32 height = Map_GetTerrainHeight(layer, object->x, object->z);
      struct MapCell *cell = cells + index;

      object->motionFlags = set;
      cell->collisionCode = 255;
      cell->attributeB &= 127;
      shapes[index] = Func_020041e8(layer, shape, (height >> 19) - 4);
    }
  }
  list++;
  if (*list != 0xffff)
    goto next;
done:
  GameFlag_ClearBit(0x20b);
}

/// For each six-byte record (actor, unused, flag) up to an actor of 0xffff:
/// when the record's flag is set, the cell under the actor in map layer 2 takes
/// collision code 255.
void Func_02000918(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  /* FAKEMATCH(pin): the limit in r2, so the next id takes r3; as a plain
   * local, 10 of 31 instructions differ in tla-en. */
  register s32 limit asm("r2");

  goto next;
  do {
    /* FAKEMATCH(barrier): the record pointer passed through an empty asm, so
     * the id is read again and the pointer outranks the actor for r5; without
     * it, 19 of 31 instructions differ in tla-en. */
    asm("" : "+l"(list));
    object = Actor_GetById(list[0]);
    if (GameFlag_Test(list[2]))
      *(u8 *)((u32)map->layers[2].cells + (Func_020041e0(object) << 2) + 2) =
          255;
    list += 3;
  next:
    limit = 0xff;
  } while (list[0] != (limit << 8) + 0xff);
}

/// For each six-byte record up to an actor of 0xffff, the cell under the actor
/// in map layer 2 takes collision code 232.
void Func_0200095c(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cell;
  s32 collisionCode;

  if (*list != 0xffff) {
  loop:
    cell = &map->layers[2].cells[Func_020041e0(Actor_GetById(*list))];
    collisionCode = 232;
    /* FAKEMATCH(dead): the advance in a once-only block, so the store waits for
     * it; as list += 3, 2 of 25 instructions differ in tla-en. */
    do {
      list += 3;
    } while (0);
    cell->collisionCode = collisionCode;
    if (*list != 0xffff)
      goto loop;
  }
}

void Func_02000994(void) {
  s16 scene;

  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 2;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov661Room1)
    Func_02000918((u16 *)gData_02004768);
  else if (scene == (s32)&SceneId_Ov661Room3)
    Func_02000918((u16 *)gData_0200478e);
  else if (scene == (s32)&SceneId_Ov661Room4)
    Func_02000918((u16 *)gData_020047d2);
  else if (scene == (s32)&SceneId_Ov661Room5)
    Func_02000918((u16 *)gData_020047ec);
}

void Func_02000a10(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s16 scene;

  leader->terrainId = 0;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov661Room1)
    Func_0200095c((u16 *)gData_02004768);
  else if (scene == (s32)&SceneId_Ov661Room3)
    Func_0200095c((u16 *)gData_0200478e);
  else if (scene == (s32)&SceneId_Ov661Room4)
    Func_0200095c((u16 *)gData_020047d2);
  else if (scene == (s32)&SceneId_Ov661Room5)
    Func_0200095c((u16 *)gData_020047ec);
}

void Func_02000a8c(void) {
  s16 scene;

  Event_Start();
  Event_ResolvePendingActions(0);
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov661Room4) {
    Camera_FollowActor(16, 1);
    Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 16);
  } else if (scene == (s32)&SceneId_Ov661Room5) {
    Camera_FollowActor(21, 1);
    Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 21);
  }
  // Entered at its test, which keeps the test single and the address load
  // inside the loop: as a while loop the test is copied above it and the
  // address kept in r5.
  goto test;
  do {
    wait(1);
  test:;
  } while (gData_02004808 == 210);
  Event_End();
}

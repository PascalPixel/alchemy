/// @file
/// The scene code of overlay 0x669 that its scene tables belong to, in ROM order; the rest of the overlay is in eastern_alhafra_ship/.

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
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"

extern u8 gData_0200345c[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

s32 Func_0200004c(void) { return (s32)gData_0200345c; }

void Func_02000054(void) {}

#include "event_inline.h"

void Func_02000058(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (x == 24 && z == 40)
    GameFlag_SetBit(0x900);
  if (x == 23 && z == 40)
    GameFlag_ClearBit(0x900);
}

void Func_02003148();

void Func_0200008c(void) {
  s32 v5;
  struct Actor *object0;
  s32 v6;
  struct Actor *object1;

  GameFlag_SetBit(0x901);
  v5 = &gMainRoutineWork;
  v5 = (v5 + 0x214);
  object0 = Actor_GetById(*(s32 *)((u8 *)v5 + 0));
  v6 = 192;
  v6 = (v6 << 11);
  *(s32 *)((u8 *)object0 + 40) = v6;
  Actor_SetSpeedParameters(*(s32 *)((u8 *)v5 + 0), 0x20000, 0x10000);
  Actor_WalkTo(*(s32 *)((u8 *)v5 + 0), 136, 488);
  Event_Wait(10);
  Actor_SetDestination(15, 120, 472);
  Actor_SetPositionAndCommit(14, 136, 475);
  object1 = Actor_GetById(15);
  *(s32 *)((u8 *)object1 + 40) = v6;
  Sound_Play(0x237);
  Actor_WaitForMoveById(*(s32 *)((u8 *)v5 + 0));
  Actor_SetModeById(*(s32 *)((u8 *)v5 + 0), 1);
  Actor_ArmCallback(*(s32 *)((u8 *)v5 + 0), FACING_NORTH, 0);
  Event_Wait(20);
  Actor_WaitForMoveById(15);
  Sound_Play(159);
  return;
}

void Func_02000128(s32 arg0) {
  Func_02003148(arg0, 0x1c9, 0x902);
  return;
}

void Func_02003130();
void Func_02003138();
void Func_02003268();
void Func_02003278();
void Func_02003280();
void Func_02003288();
void Func_02003290();

void Func_0200013c(s32 id) {
  struct Actor *object = Actor_GetById(12);
  u8 *flags;
  s32 i;

  Func_02003130();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgGooGooGa);
  Event_ShowMessageAndWait(11, 0);
  Func_02003268(8, 0x1210);
  Func_02003278();
  Func_02003290((s32)Actor_GetById(id), 2);
  Sound_Play(201);
  Event_Wait(30);
  flags = &object->motionFlags;
  *flags = 4;
  for (i = 39; i >= 0; i--) {
    object->y += 0x3333;
    Event_Wait(1);
  }
  Event_Wait(70);
  Func_02003290((s32)Actor_GetById(id), 0);
  Func_02003288();
  Func_02003280();
  Field_StartPaletteTransition(8);
  *flags = 3;
  object->gravity = 0x8000;
  Event_Wait(5);
  Sound_Play(132);
  if (Event_GetFlag(0x8ae) == 0) {
    GameFlag_SetBit(0x8ae);
    Event_Wait(30);
    Actor_ArmCallback(10, 0x3000, 0);
    Event_Wait(30);
    Motion_SetVarCbAndRefresh(10, 2);
    Event_Wait(10);
    Event_ShowMessageThenWait(10, 0, 10);
    Actor_FaceDirection(10, 0xafff, 0);
  }
  Func_02003138();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

void Func_02000c5c(void);

s32 Func_02000250(void) {
  struct Actor *object;

  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  if (gMainRoutineWork.entrance == 99) {
    GameFlag_SetBit(0x8aa);
    GameFlag_SetBit(0x8ab);
    gMainRoutineWork.secondary = 3;
    gMainRoutineWork.entrance = 3;
    Func_02000c5c();
  }
  if (GameFlag_Test(0x8a7)) {
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  } else if (GameFlag_Test(0x8ab)) {
    Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    if (!GameFlag_Test(0x8bd)) {
      Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
      Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    }
  } else {
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  }
  if (GameFlag_Test(0x900))
    Actor_SetPosition(13, 0x1880000, 0x2880000);
  Actor_GetById(13)->priorityFlags |= 32;
  Func_080d3b28(15, 457);
  if (!GameFlag_Test(0x901)) {
    if (!GameFlag_Test(0x109)) {
      Actor_GetById(14)->priorityFlags |= 128;
      object = Actor_GetById(15);
      object->priorityFlags |= 64;
      object->y += 0x100000;
      object->terrainHeight += 0x100000;
    }
  } else {
    Actor_SetHorizontalPositionWithTerrain(14, 0x880000, 0x1db0000);
    if (!GameFlag_Test(0x902))
      Actor_SetPosition(15, 0x780000, 0x1d80000);
    else
      Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

struct Sprite020003e8 {
  u8 unk00[18];
  s16 rotation;
  u8 unk14[6];
  u8 nextFrame;
};

struct Actor020003e8 {
  u8 unk00[0x50];
  struct Sprite020003e8 *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[8];
  s16 unk5e;
};

extern const u8 gData_02003720[];

void Func_02003100(struct Actor020003e8 *object, s32 mode);
void Func_02003108(struct Actor020003e8 *object, const void *script);

void Func_020003e8(s32 x, s32 y, s32 z, s16 value, s32 fifth) {
  struct Actor020003e8 *object;
  struct Sprite020003e8 *sprite;

  object = ((struct Actor020003e8 *)Import_ActorCreate(0x1e8, x, y, z));
  if (object != 0) {
    sprite = object->sprite;
    Func_02003100(object, 1);
    Func_02003108(object, gData_02003720);
    object->motionFlags = 0;
    /* FAKEMATCH(barrier): an empty asm, so the fifth argument is read from the
     * stack before the zero for nextFrame, as the native does; as no asm, 2 of
     * 32 instructions differ in tla-en. */
    __asm__("");
    object->unk5e = fifth;
    sprite->nextFrame = 0;
    sprite->rotation = value;
  }
}

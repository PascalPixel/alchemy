/// @file
/// The scene code of overlay 0x6a1 that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_room_8/.

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
#include "event_inline.h"
#include "actor.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "field_event.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

struct Probe {
  s32 unk00[6];
};

s32 Func_02001f60(void);
s32 Func_02000c44(struct Probe *probe);
void Func_02000ec8(struct Probe probe);

void Func_02000054(void) {
  if (Func_02001f60())
    FieldEvent_SetExitRequest(13);
}

void Func_02000068(void) {
  struct Probe probe;

  if (Func_02000c44(&probe))
    Func_02000ec8(probe);
}

void Func_02000090(void) {}

void Func_020021e0(s32 x, s32 z, s32 a, s32 b);
void Func_020022c0(s32 x, s32 z, s32 a, s32 b);

/// On cell (6, 14), sets flag 0xa18, waits until the object finishes falling
/// and stops (at most 30 frames), then releases it.
void Func_02000094(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 i;
  s32 y;
  s32 floor;

  if (object->x >> 20 == 6 && object->z >> 20 == 14) {
    Event_SetFlag(0xa18);
    System_Wait(10);
    Event_PlaySound(134);
    y = object->y;
    floor = object->terrainHeight;
    /* FAKEMATCH(dead): the height and the floor are read before the counter
     * is cleared and again at the end of each pass; as plain C, 8 of 50
     * instructions differ in tla-en. */
    do {
    } while (0);
    i = 0;
    goto test;
    do {
      System_Wait(1);
      if (++i > 29)
        break;
      y = object->y;
      floor = object->terrainHeight;
    test:;
    } while (y > floor || object->velocityY != 0);
    {
      /* FAKEMATCH(temporary): the motion flags cleared from a zero of their
       * own; as a plain 0 store, 2 of 50 instructions differ in tla-en. */
      s32 cleared = 0;

      object->motionFlags = cleared;
      Func_020021e0(object->x, object->z, 0, 0);
    }
    Func_020022c0(object->x, object->z, 0, 128);
  }
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

struct JupiterLighthouseRoom8EventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

void Func_02002248(s32 id);
void Func_02000af0(s32 id);
void Func_020018f4(s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_02002010(s32 x, s32 z, s32 a);

s32 Func_02000110(void) {
  ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x204;
  Actor_Get(8)->scaleX = 0xc000;
  Actor_Get(8)->scaleY = 0xc000;
  FieldActor_SetSpritePriority(12, 3);
  FieldActor_SetSpritePriority(14, 3);
  Func_02002248(11);
  Func_02002248(12);
  Func_02002248(13);
  Func_02002248(14);
  Func_02002248(15);
  Func_02000af0(11);
  Func_02000af0(12);
  Func_02000af0(13);
  Func_02000af0(14);
  Func_02000af0(15);
  Func_020018f4(33, 0x280, 0x380, 0, 1);
  if (gMainRoutineWork.entrance == 13)
    Func_02002010(19, 0, 0x204);
  else if (gMainRoutineWork.entrance == 14)
    Func_02002010(-20, -22, 0x204);
  return 0;
}

s32 Func_020001d4(void) {
  Scene_SetArrivalFlags(0);
  if (Event_GetFlag(0x109) == 0)
    Event_ClearFlag(0xa18);
  if (Event_GetFlag(0xa18)) {
    struct FieldActor *object = ((struct FieldActor *)Actor_Get(10));

    object->motionFlags = 0;
    object->terrainHeight = -0x200000;
    object->y = -0x200000;
    Actor_SetPosition(10, PIXELS(104), PIXELS(232));
    Func_020021e0(object->x, object->z, 0, 0);
    Func_020022c0(object->x, object->z, 0, 128);
  }
  return 0;
}

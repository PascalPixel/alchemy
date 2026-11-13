/// @file
/// The scene code of overlay 0x6a1 that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_room_8/.

#include "shared/routines.h"
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
#include "event_work.h"
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_room_8.h"
#include "module/scene/scene.h"
#include "module/tla.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

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
      s32 cleared = 0;

      object->motionFlags = cleared;
      Func_020021e0(object->x, object->z, 0, 0);
    }
    Func_020022c0(object->x, object->z, 0, 128);
  }
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }


void Func_02000af0(s32 id);
void Func_020018f4(s32 a, s32 b, s32 c, s32 d, s32 e);

s32 Scene_Initialize(void) {
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

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  if (Event_GetFlag(0x109) == 0)
    Event_ClearFlag(0xa18);
  if (Event_GetFlag(0xa18)) {
    struct Actor *object = Actor_Get(10);

    object->motionFlags = 0;
    object->terrainHeight = -0x200000;
    object->y = -0x200000;
    Actor_SetPosition(10, PIXELS(104), PIXELS(232));
    Func_020021e0(object->x, object->z, 0, 0);
    Func_020022c0(object->x, object->z, 0, 128);
  }
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

#include "jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_02002a88;

void Func_02000270(s32 value) { gData_02002a88 = value; }

extern s32 gData_02002a84;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gData_02002a84 = value; }

DEFINE_FUNC_0200027C(Func_0200027c, gData_02002a84, gData_02002a88,
    gData_02002ab0)

extern u16 gData_02002ad0[];

DEFINE_FUNC_020002F4(Func_02000338, gData_02002a88, gData_02002ab0,
    gData_02002ad0, Func_0200027c, gData_02002ad4)

DEFINE_FUNC_02000378(Func_020003bc, Func_020021e0, Func_020022c0)

#include "frame.h"
#include "field_sprite.h"
#include "event_work.h"

DEFINE_FUNC_02000410(Func_02000410)

DEFINE_FUNC_020003D0(Func_020005fc)

extern const s32 gData_02002a8c[];

s32 Func_020022b8(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_FUNC_02000628(Func_02000628, Func_020022b8, Func_020005fc,
    gData_02002a8c, Data_02002aac)

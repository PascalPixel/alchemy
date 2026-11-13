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
#include "macros.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_inline.h"
#include "frame.h"
#include "field_sprite.h"
#include "debug.h"
#include "cell_target.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_basement.h"
#include "module/scene/mars_lighthouse_entrance.h"
#include "module/scene/scene.h"
#include "module/tla.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

struct MainRoutineWork254 {
  u8 unk000[0x254];
  u16 value;
};

extern const u8 gData_020022e8[];
extern const u8 gData_0200236c[];

void Func_02001d44(s32 a, s32 b, s32 c);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02001d1c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  Func_02001d1c(a, b, c, d, e, f, g);
}

static __inline__ void Func_02001d44_(s32 a, s32 b, s32 c) {
  Func_02001d44(a, b, c);
}

void Func_02000054(void) { Func_02001d1c_(69, 0, 4, 4, 67, 24, 1); }

void Func_02000074(void) { Func_02001d1c_(64, 0, 4, 3, 89, 28, 1); }

void Func_02000094(void) {
  Func_02001d1c_(64, 0, 4, 3, 89, 28, 1);
  if (!Event_GetFlag(0x9b8)) {
    Func_02001d44_(249, 424, 488);
    ((struct MainRoutineWork254 *)&gMainRoutineWork)->value = 0x19b8;
    gMainRoutineWork.battleRule24b = 2;
    Field_RequestBattleFromEncounterTable(106, 3);
  }
}

void Func_020000f8(void) { Func_02001d1c_(69, 1, 4, 3, 108, 31, 1); }

void Func_02000118(void) { Import_EventSetExitRequest(4); }

void Func_02000124(void) { Import_EventSetExitRequest(6); }

const u8 *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov699Room2)
    return gData_0200236c;
  return gData_020022e8;
}

void Func_0200015c(struct Actor *object) {
  if (Random16Far() & 0xff) {
    s32 position[3];
    s32 radius;
    s32 angle;

    SET_VECTOR(position, 200 << 16, -32 << 16, 164 << 16);
    radius = Random16Far();
    angle = Random16Far();
    FieldVector_AddPolarOffset((radius << 5) + 0x80000, angle, position);
    SET_POSITION(object, position[0], position[1], position[2]);
  }
  object->unk66 = object->timer;
  if (object->timer == 212)
    Func_02001bec(object, 9);
  else
    Func_02001bec(object, 10);
}

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
void Func_020004dc(s32 mode);

#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

void Func_020001d8(struct Actor *object) {
  u8 *work = Ram_HeapSlots->eventWork;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    Actor_SetAnimationSpeed(object, 0);
    Map_EnableUpdateCallback();
    return;
  }
  Actor_SetAnimationSpeed(object, 16);
  Map_DisableUpdateCallback();
  if (OBJECT_BYTE(object, 99) != 0) {
    if ((gFrameCount & 7) == 0)
      OBJECT_BYTE(object, 99)--;
    return;
  }
  if (*(s16 *)((u8 *)object + 102) == 0)
    Func_0200015c(object);
  (*(s16 *)((u8 *)object + 102))--;
  /* FAKEMATCH(dead): an empty once-only block, whose loop note ends the
   * scheduling block, so the timer is stored before z loads as in the native;
   * without it, 2 of 87 instructions differ in tla-en. */
  do {
  } while (0);
  object->x += Math_Cosine(((object->z >> 14) + (object->y >> 15)) << 8) / 2;
  object->y += object->speedLimit;
}

void Func_02000298(void) {
  s32 i;
  s32 next;

  struct Actor *object;
  s16 *action;
  s16 *timer;

  i = 0;
  do {
    object = (FieldActor_Create(0x2a0, 0, 0, 0));

    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->speedLimit = (i << 12) + 0x8000;
    object->acceleration = (i << 12) + 0x8000;
    object->update = (void (*)(void))Func_020001d8;
    OBJECT_BYTE(object, 99) = i << 3;
    Actor_SetSingleChildFlags(object, 0);
    Actor_SetSpriteColor(object, 2);
    ((struct FieldSprite *)object->sprite)->priority = 1;
    Animation_SetStateFlags(object, 1);
    action = &object->timer;
    timer = (s16 *)&object->unk66;
    next = i + 1;
    if (i & 1) {
      s32 parameter = 212;
      *action = parameter;
      *timer = 0;
    } else {
      s32 parameter = 116;
      *action = parameter;
      *timer = 0;
    }
    i = next;
  } while (i != 16);
}

s32 Scene_Initialize(void) {
  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 0x204;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov699Room1) {
    Func_020004dc(1);
    if (!Event_GetFlag(0x109) && gMainRoutineWork.entrance == 4)
      Func_02001d3c(gMainRoutineWork.currentOwner);
    if (!Event_GetFlag(0x109) && gMainRoutineWork.entrance == 6)
      Func_02001d3c(gMainRoutineWork.currentOwner);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov699Room2) {
    *(volatile u16 *)0x04000052 = 0x1008;
    *(volatile u16 *)0x04000050 = 0x3f10;
    Func_02000298();
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

#include "jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "field_event.h"

extern s32 gData_02002388;

void Func_02000414(s32 value) { gData_02002388 = value; }

extern s32 gData_02002384;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gData_02002384 = value; }

DEFINE_FUNC_02000238(Func_02000420, gData_02002384, gData_02002388,
    gData_020023b0, 0x05000184)

DEFINE_FUNC_020002F4(Func_020004dc, gData_02002388, gData_020023b0,
    gData_020023d0, Func_02000420, gData_020023d4)

DEFINE_FUNC_02000378(Func_02000560, Func_02001c74, Func_02001d34)

#include "event_work.h"

DEFINE_FUNC_020003CC(Func_020005b4)

#include "irq.h"

DEFINE_FUNC_020003D0(Func_020007a0)

extern const s32 gData_0200238c[];

void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_FUNC_020005E4(Func_020007cc, Func_02001d2c, Func_020007a0,
    gData_0200238c, Data_020023ac)

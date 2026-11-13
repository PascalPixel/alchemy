/// @file
/// Overlay 0x6b9's scene: its code, then its tables, in ROM order.

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
#include "event_inline.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "call.h"
#include "item_icon.h"

struct EventFlags {
  u8 unk000[0xd36];
  u8 unkd36;
  u8 unkd37;
};

#include "module/scene/kibombo.h"
#include "module/tla.h"
#include "particle.h"
#include "resource.h"
#include "gamepak/far_call/system.h"

struct Particle;

void Func_020001e0(s32 x, s32 y, s32 z, s32 count);

void Func_0200029c(void);
void Func_02000358(void);

void Func_020005e8(s32 a, s32 b, s32 c);

void Func_02000600(struct Particle *particle, s32 unk1, s32 step);

void Func_02000560(void *entry, void *destination);

void Func_020005d8(s32 a, s32 b);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Test_ReceiveFlint(void) {
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
}

void Func_02000064(s32 mode) {
  if (mode == 1)
    Actor_SetPosition(8, 0, 0);
}

void Func_02000078(void) {}

void Test_ReceiveForge(void) { Actor_SetModeById(9, 3); }

void Func_02000088(s32 mode) {
  if (mode == 1) {
    MapGrid_CopyCellsLayered(49, 1, 1, 2, 34, mode);
    MapGrid_CopyCellAttributes(49, 1, 1, 2, 34, 3);
  }
}

void Func_020000b8(void) {
  if (!Event_GetFlag(0x220)) {
    Event_SetFlag(0x220);
    Func_020005d8(14, 0);
  }
}

void Func_020000d8(s32 id) {
  struct Actor *object = Actor_Get(id);

  Func_0200029c();
  Func_020001e0(object->x, object->y, object->z, 20);
  System_Wait(20);
  Func_02000358();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Func_02000104(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 256;
  ((struct EventFlags *)work)->unkd37 = 0;
  ((struct EventFlags *)work)->unkd36 = 0;
  gMainRoutineWork.eventMode = 2;
  Func_020005e0();
  Value3((s32 (*)())Func_020005e8, 0, 10, 11);
  Actor_SetAnimation(9, 4);
  return 0;
}

// XXX: what this room tests that the first Psynergy test room does not; its
// scene tables and a render of it would settle it.

s32 Scene_PrepareMap(void) { return 0; }

void Func_020005f8(struct ParticleSprite *sprite, struct Particle *particle);

void Func_0200015c(void) {
  struct ParticleWork *work = Ram_HeapSlots->menuRuntime;
  struct Particle *particle = work->particles;
  struct ParticleSprite *sprite = work->sprites;
  s32 i;
  s32 step = -0x8000;

  i = 63;
next:
  if ((u32)particle->age <= 19) {
    u16 *base = &work->tile;
    s32 mask = 7;
    s32 tile = (u16)((*base + ((particle->age >> 2) & mask) * 8) & 0x3ff);
    s32 attr = sprite->attr;

    sprite->attr = (attr & ~0x3ff) | tile;
    Func_020005f8(sprite, particle);
    Func_02000600(particle, 63, step);
    particle->age++;
  }
  i--;
  sprite++;
  particle++;
  if (i >= 0)
    goto next;
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 408, 0, 88, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
    {1, CONDITION_ALWAYS, 408, 0, 408, FACING_NORTH, 0, 280, 0, 780, 464, 0},
    {2, CONDITION_ALWAYS, 504, 0, 232, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(322),
    SCENE_EXIT(20, 322, 2),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {306, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(104), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(248), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(104), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(136), FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {0, 9, CONDITION_ALWAYS, EVENT(Func_020000d8)},
    {2, 30, CONDITION_ALWAYS, EVENT(Func_020000b8)},
    {0x2115, 8, CONDITION_ALWAYS, EVENT(Test_ReceiveFlint)},
    {0x50008615, 8, CONDITION_ALWAYS, EVENT(Func_02000064)},
    {0x8715, 9, CONDITION_ALWAYS, (u32)(Func_02000078)},
    {0x9815, 9, CONDITION_ALWAYS, EVENT(Test_ReceiveForge)},
    {0x8515, 10, CONDITION_ALWAYS, 0},
    {0x50009705, 50, CONDITION_ALWAYS, EVENT(Func_02000088)},
    {TRIGGER_TABLE_END},
};

struct Spark {
  s32 position[3];
  s32 velocity[3];
  s32 timer;
};

struct SparkWork {
  u8 unk0000[0x500];
  struct Spark sparks[64];
  struct SparkSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

void Func_020001e0(s32 x, s32 y, s32 z, s32 count) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;
  struct Spark *spark;
  s32 angle;
  s32 i;

  if (count > 0) {
    i = count;
  loop:
    angle = Random16();
    spark = &work->sparks[work->next];
    spark->timer = 0;
    spark->position[0] = x;
    spark->position[1] = y;
    spark->position[2] = z;
    FieldVector_AddPolarOffset((Random16() << 3) + 0x80000, angle,
                               spark->position);
    spark->velocity[0] = 0;
    spark->velocity[1] = 0x50000;
    spark->velocity[2] = 0;
    FieldVector_AddPolarOffset((Random16() << 1) + 0x20000, angle,
                               spark->velocity);
    work->next++;
    work->next &= 63;
    i--;
    if (i != 0)
      goto loop;
  }
}

#include "system/task.h"

struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  u16 resource;
};

void Func_020005f0(struct RockSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);

static __inline__ void StartTask(void (*func)(void), s32 priority) {
  /* FAKEMATCH(wrapper): Task_Add forwarded inline; as a direct call, 6 of 83
   * instructions differ in tla-en. */
  Task_Add(func, priority);
}

void Func_0200029c(void) {
  struct RockWork *work = HeapSlot_Alloc(220, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct RockSprite *sprite;

  Func_02000560(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--, sprite++, rock++) {
    Func_020005f0(sprite, 8, 8, 0x40000000, tile);
    sprite->unk05_5 = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    rock->timer = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  StartTask(Func_0200015c, 0x480);
}

#include "fixed_math.h"

void Func_02000358(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(Func_0200015c);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(220);
}

void Func_02000384(s32 mode, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  if (mode == 1)
    MapGrid_CopyCells(a, b, c, d, e, f);
}

void Func_020003a8(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;
  s32 scale;

  object = Actor_Get(id);
  if (mode == 1) {
    Actor_SetPosition(id, x << 16, z << 16);
    object->scaleX = 0;
    object->scaleY = 0;
  }
  if (mode == 2) {
    while (object->scaleX < Q16_ONE) {
      System_Wait(1);
      scale = object->scaleX;
      scale += 0x51e;
      object->scaleX = scale;
      object->scaleY = scale;
    }
    object->scaleX = Q16_ONE;
    object->scaleY = Q16_ONE;
  }
}

void Func_02000418(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;

  object = Actor_Get(id);
  if (mode == 2) {
    Actor_SetPosition(id, x << 16, z << 16);
    object->y = 0x200000;
    object->motionFlags = 3;
    object->gravity = 0x170a;
    System_Wait(50);
  }
}

/// Mode 1 places object kind 354 at (x, z) in pixels; mode 2 grows object
/// kind 252 there from 0x6666 to full size while it turns, then turns it
/// opposite the leader's facing and passes scene and entrance on to
/// Func_020005d8.
void Func_02000468(s32 mode, s32 x, s32 z, s32 scene, s32 entrance) {
  struct Actor *object;

  if (mode == 1) {
    object = FieldActor_Create(354, x << 16, 0, z << 16);
    if (object != NULL) {
      Actor_SetSpriteFlags(object, 0);
      object->priorityFlags = 2;
    }
  }
  if (mode == 2) {
    object = (FieldActor_Create(252, x << 16, 0, z << 16));
    if (object != NULL) {
      s32 scale;

      Actor_SetSpriteFlags(object, 0);
      object->velocityY = 0x60000;
      object->scaleX = 0x6666;
      object->scaleY = 0x6666;
      while (object->scaleX < Q16_ONE) {
        System_Wait(1);
        scale = object->scaleX + 0x51e;
        {
          /* FAKEMATCH(temporary): the turn is added as a word, so its step
           * is built inside the loop as in the native; as
           * object->angle += 0x1000, 22 of 86 instructions differ in
           * tla-en. */
          s32 angle = object->facing + 0x1000;

          object->facing = angle;
        }
        object->scaleX = scale;
        object->scaleY = scale;
      }
      object->scaleX = Q16_ONE;
      object->scaleY = Q16_ONE;
      object->facing =
          Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x8000;
      Func_020005d8(scene, entrance);
    }
  }
}

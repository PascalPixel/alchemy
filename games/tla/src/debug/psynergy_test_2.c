/// @file
/// Overlay 0x6b9's scene: its code up to psynergy_test_2/s01e0.c, then its tables, in ROM order.

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

#include "system/ram_buffer.h"

void Func_020001e0(s32 x, s32 y, s32 z, s32 count);
void Func_0200029c(void);
void Func_02000358(void);
void Func_020005d8(s32 a, s32 b);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000054(void) { Actor_SetHorizontalPositionWithTerrain(8, 0, 0); }

void Func_02000064(s32 mode) {
  if (mode == 1)
    Actor_SetPosition(8, 0, 0);
}

void Func_02000078(void) {}

void Func_0200007c(void) { Actor_SetModeById(9, 3); }

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

struct EventFlags {
  u8 unk000[0xd36];
  u8 unkd36;
  u8 unkd37;
};

void Func_020005e0(void);
void Func_020005e8(s32 a, s32 b, s32 c);

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

struct Particle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 age;
};

struct ParticleSprite {
  u8 unk0[8];
  u16 attr;
  u8 unkA[30];
};

struct ParticleWork {
  u8 tiles[0x500];
  struct Particle particles[64];
  struct ParticleSprite sprites[64];
  s16 next;
  u16 tile;
};

void Func_020005f8(struct ParticleSprite *sprite, struct Particle *particle);
void Func_02000600(struct Particle *particle, s32 unk1, s32 step);

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

void Func_02000054(void);
void Func_0200007c(void);
void Func_020000b8(void);

void Func_02000078(void);

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
    {0x2115, 8, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {0x50008615, 8, CONDITION_ALWAYS, EVENT(Func_02000064)},
    {0x8715, 9, CONDITION_ALWAYS, (u32)(Func_02000078)},
    {0x9815, 9, CONDITION_ALWAYS, EVENT(Func_0200007c)},
    {0x8515, 10, CONDITION_ALWAYS, 0},
    {0x50009705, 50, CONDITION_ALWAYS, EVENT(Func_02000088)},
    {TRIGGERS_END},
};

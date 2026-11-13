#include "../shared/effects.h"
#include "../shared/map_cells.h"
#include "../shared/tables.h"
#include "types.h"
#include "system.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "event_inline.h"
#include "gamepak/far_call/system.h"
#include "module/field/field_effect.h"
#include "module/scene/scene.h"
#include "module/scene/tundaria_tower.h"
#include "module/tla.h"
#include "particle.h"

s32 Func_02003180(s32 layer, s32 shape, s32 height);
u32 Import_SpriteShareVramCacheEntry(struct FieldSprite *sprite, u32 frame);

void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);

void Func_020029d0(s32 spread) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local, so x is computed before it; as
       * one expression, 14 of 122 instructions differ in tla-en. */
      s32 y = leader->y - (((Random16Far() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)Import_ActorCreate(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Import_SpriteShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSingleChildFlags(effect, 0);
      Import_ActorSelectAnimation(effect, 2);
      Func_02002f90(effect, gData_020032d4);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Math_Cosine(i << 13), 0x13333);
        effect->velocity.z = Math_Sine(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16Far() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16Far() << 9) >> 16) - 0x100;
      effect->scaleRateX = -0x800;
      effect->scaleRateY = -0x600;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}

void Func_02002b04(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 state;
  u32 i;
  // Default: without it the set in the loop is hoisted.
  s32 zero = 0;

  if (leader != NULL) {
    state = 0;
    for (i = 0; i < 17; i++) {
      struct FieldEffect *effect = ((struct FieldEffect *)Import_ActorCreate(
          285, leader->x, leader->y, leader->z));

      if (effect != NULL) {
        s32 angle;

        state = Import_SpriteShareVramCacheEntry(effect->sprite, state);
        zero = 0;
        effect->motionFlags = zero;
        effect->collisionFlags = zero;
        Actor_SetSingleChildFlags(effect, 0);
        Import_ActorSelectAnimation(effect, 2);
        Func_02002f90(effect, gData_02003304);
        angle = i << 12;
        effect->velocity.x = Math_Cosine(angle);
        effect->velocity.y = zero;
        effect->velocity.z = Math_Sine(angle);
        effect->velocity.x += effect->velocity.x >> 1;
        effect->velocity.x += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
        effect->velocity.z += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
        effect->spin = ((Random16Far() << 12) >> 16) - 0x800;
        effect->scaleRateX = zero;
        effect->scaleRateY = zero;
        effect->update = Effect_AdvanceMotionAtLeaderPriority;
        effect->sprite->priority =
            ((struct FieldSprite *)leader->sprite)->priority;
      }
    }
  }
}

#include "tundaria_tower.h"
#include "map.h"
#include "system/ram_buffer.h"

DEFINE_ACTOR_SET_CELL_SHAPE(Func_02002c1c, MapGrid_GetTerrainKind, Func_02003180)

void Func_02002ca8(s32 a, s32 b) {
  Func_02002c1c(a, b, gMainRoutineWork.currentOwner);
}

void Func_02002cc0(void) {
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
    EffectSprite_DrawAtWorldPosition(sprite, particle);
    Import_BattleEffectStepVector3(particle, 63, step);
    particle->age++;
  }
  i--;
  sprite++;
  particle++;
  if (i >= 0)
    goto next;
}

/// A falling rock's sprite.
/* VIEW(s29d0): tag struct RockSprite as struct RockSprite__s29d0; module/tla.h
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/scene/tundaria_tower/s29d0.c:232: structure has no member named
 * `blend'. */
struct RockSprite__s29d0 {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 blend : 1;
  u8 unk05_6 : 2;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x14];
  u16 unk1e;
  u8 unk20[8];
};

/// The falling rocks' work: their tiles, 64 rocks and their sprites, in a
/// ring that next walks round.
struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite__s29d0 sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

/// Spawns count rocks around (x, y, z), each pushed out at a random angle and
/// thrown up and out along it.
DEFINE_ROCKS_SPAWN(Func_02002d44)

// The falling rocks' setup, and Func_02002ebc, which stops their task and
// frees their picture and heap slot.

void Func_02003190(struct RockSprite__s29d0 *sprite, s32 width, s32 height,
                   u32 attr, s32 tile);

/// The falling rocks' setup: loads their picture, sets up 64 small sprites,
/// marks each rock idle and starts the task that spawns them.
void Func_02002e00(void) {
  struct RockWork *work = HeapSlot_AllocFar(220, sizeof(struct RockWork));
  s32 resource;
  s32 tile;
  struct Rock *rock;
  struct RockSprite__s29d0 *sprite;
  s32 i;

  Import_DecompressTaggedLz(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  resource = VramCache_FindFreeEntry();
  tile = Vram_Load(resource, sizeof(work->tiles), work);
  work->tileBase = tile;
  work->resource = resource;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--) {
    Func_02003190(sprite, 8, 8, 0x40000000, tile);
    sprite->blend = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    rock->timer = -1;
    sprite++;
    rock++;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  Task_AddFrame(Func_02002cc0, 0x480);
}

void Func_02002ebc(void) {
  struct TundariaTowerSparkWork *work = Ram_HeapSlots->menuRuntime;

  Task_RemoveFar(Func_02002cc0);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(220);
}

#include "scene_tables.h"

void Func_02000280(void);
void Func_020005d0(void);
void Func_020008a8(void);
void Func_02000b3c(void);
void Func_02000bfc(void);
void Func_02000c64(void);

// clang-format off
u32 gData_020031b0[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020031ec[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003228[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s16 gData_02003264[] = {
    11, 12, 13, 14, -1,
};

s16 gData_0200326e[] = {
    8, -1,
};

DEFINE_G_DATA_02002276(gData_02003272)

s16 gData_020032b2[] = {
    -1, 0, 0x8000, -1, 0xc000, 0, 0x8000, 0xc000,
    0x4000, 0, 0x8000, 0x4000, -1, 0, 0x8000, -1,
    0,
};

u32 gData_020032d4[] = {
    0, 33, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

u32 gData_02003304[] = {
    0, 44, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

s32 gData_02003334[] = {
    (s32)gData_020031b0,
    (s32)gData_020031ec,
    (s32)gData_02003228,
};

s16 gData_02003340[] = {
    8, 7, 512, 9, 7, 513, 10, 7,
    514, 11, 7, 515, -1,
};

s16 gData_0200335a[] = {
    8, 7, 512, 9, 7, 513, 10, 7,
    514, -1, 0,
};

u32 gData_02003370[] = {
    22, 3, 0, 0x80010000, 0, 1,
    23, 3, 0x1000, 13, 0xffff, 0x80010000,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_020033d0[] = {
    0x500330, 0x3400210, 0x2200060, 0x6ffff, 0xffff, 0,
    0, 0,
};

u32 gData_020033f0[] = {
    0xf00b0, 0xc000c0, 0xd00021, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(206),
    SCENE_EXIT_IF(1, 2, 46, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 207, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(207),
    SCENE_EXIT_IF(1, 206, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 207, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 207, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 207, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 211, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 207, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(7, 207, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(8, 207, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(9, 207, 18, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(10, 208, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(11, 207, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(12, 211, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(13, 207, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(14, 207, 17, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(15, 207, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(16, 207, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(17, 207, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(18, 207, 9, EXIT_FLAG_NONE),
    SCENE_EXITS(208),
    SCENE_EXIT_IF(1, 208, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 208, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 208, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 208, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 209, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 208, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(7, 208, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(8, 208, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(9, 207, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(10, 208, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(11, 208, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(12, 208, 8, EXIT_FLAG_NONE),
    SCENE_EXITS(209),
    SCENE_EXIT_IF(1, 210, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 208, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 210, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 210, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 209, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 209, 5, EXIT_FLAG_NONE),
    SCENE_EXITS(210),
    SCENE_EXIT_IF(1, 210, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 209, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 210, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 210, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 209, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 209, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(7, 211, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(8, 210, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(9, 211, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(10, 211, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(11, 209, 7, EXIT_FLAG_NONE),
    SCENE_EXITS(211),
    SCENE_EXIT_IF(1, 210, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 210, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 212, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 207, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 207, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 212, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(7, 211, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(8, 211, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(9, 210, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(10, 211, 10, EXIT_FLAG_NONE),
    SCENE_EXITS(212),
    SCENE_EXIT_IF(1, 211, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 211, 3, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02003618[] = {
    {489, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_EAST, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003648[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003660[] = {
    {245, 0x64, 7, PIXELS(424), 0, PIXELS(536), FACING_WEST, 0, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_DATA_02003690(gData_02003690, 406, 1, 2, 406, 406, 3, 406, 4)

DEFINE_G_DATA_020013D8(gData_02003708, 406, 1, 2, 3, 272, 4, 5, 6, 7)

struct ActorPlacement gData_020037c8[] = {
    {272, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},
    {SCENE_TABLE_END},
};

struct Trigger gData_020037f8[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gData_0200381c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_020005d0)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_020005d0)},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {33, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXAMINE, 52, CONDITION_ALWAYS, EVENT(Func_02000920)},
    {0x50008a05, 50, 0x942, EVENT(Func_02000548)},
    {0x50008a05, 51, 0x943, EVENT(Func_0200058c)},
    {0x50009705, 52, 0x944, EVENT(Func_020007dc)},
    {0x9705, 52, 0x944, EVENT(Func_020008a8)},
    {TRIGGERS_END},
};

struct Trigger gData_0200393c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {33, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {49, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(Func_0200095c)},
    {TRIGGERS_END},
};

struct Trigger gData_020039e4[] = {
    {33, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {33, 3, CONDITION_ALWAYS, 3},
    {33, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(Func_0200095c)},
    {TRIGGER_STEP, 51, 0x200, EVENT(Func_02000964)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Func_02000280)},
    {TRIGGERS_END},
};

struct Trigger gData_02003a5c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {33, 4, CONDITION_ALWAYS, 4},
    {49, 5, CONDITION_ALWAYS, 5},
    {49, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {49, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {33, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(Func_02000b3c)},
    {TRIGGER_STEP, 50, CONDITION_ALWAYS, EVENT(Func_0200095c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200,
     EVENT(Func_02000b34)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201,
     EVENT(Func_02000b34)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 10, 0x202,
     EVENT(Func_02000b34)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 11, 0x203,
     EVENT(Func_02000b34)},
    {TRIGGERS_END},
};

struct Trigger gData_02003b28[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {49, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(Func_02000f00)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200,
     EVENT(Func_02000b34)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201,
     EVENT(Func_02000b34)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 10, 0x202,
     EVENT(Func_02000b34)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_02000bfc)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000d2c)},
    {0x80008c15, 11, CONDITION_ALWAYS, EVENT(Func_02000fac)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 11,
     CONDITION_ALWAYS, EVENT(Func_02000bfc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS), 11,
     CONDITION_ALWAYS, EVENT(Func_02000c64)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12,
     0x946, EVENT(Func_02000bfc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x946, EVENT(Func_02000d2c)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 13,
     0x947, EVENT(Func_02000bfc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, 0x947, EVENT(Func_02000d2c)},
    {0x80008c15, 14, CONDITION_ALWAYS, EVENT(Func_02000fac)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 14,
     CONDITION_ALWAYS, EVENT(Func_02000bfc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_ACTS), 14,
     CONDITION_ALWAYS, EVENT(Func_02000c64)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Func_02000e78)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Func_02000de4)},
    {0x80009705, 52, CONDITION_ALWAYS, EVENT(Func_02000fac)},
    {0x50009705, 52, 0x94b, EVENT(Func_02000f0c)},
    {TRIGGERS_END},
};

struct Trigger gData_02003c90[] = {
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(Func_02000bfc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS,
     EVENT(Func_02000d2c)},
    {0x50009705, 50, 0x948, EVENT(Func_02000fb8)},
    {TRIGGERS_END},
};

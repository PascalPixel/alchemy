/// @file
/// Magma Rock's rubble and falling rocks, and its tables.

#include "../shared/map_cells.h"
#include "../shared/effects.h"
#include "../shared/tables.h"
#include "types.h"
#include "system.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "event_inline.h"
#include "map.h"
#include "system/ram_buffer.h"
#include "resource.h"
#include "heap_release.h"
#include "field.h"
#include "module/field/field_effect.h"
#include "module/scene/magma_rock.h"
#include "particle.h"

void Actor_StartScript(struct Actor *object, const s32 *script);
s32 Trig_Cos(s32 angle);

void MagmaRock_SpawnRubble(s32 spread) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local, so x is computed before it; as
       * one expression, 14 of 122 instructions differ in tla-en. */
      s32 y = leader->y - (((Random16() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)FieldActor_Create(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript(effect, gMagmaRock_RubbleScript);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Trig_Cos(i << 13), 0x13333);
        effect->velocity.z = Math_Sin(i << 13);
      } else {
        effect->velocity.x = spread;
        effect->velocity.z = spread;
      }
      effect->velocity.y = -(((Random16() << 14) >> 16) + 0x6000);
      effect->spin = ((Random16() << 9) >> 16) - 0x100;
      effect->scaleRateX = -0x800;
      effect->scaleRateY = -0x600;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
    }
    i++;
  } while (i <= 7);
}

void MagmaRock_SpawnDustRing(void) {
  struct Actor *leader;
  struct FieldEffect *effect;
  s32 state;
  u32 i;
  s32 angle;
  u8 zero;

  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader == 0)
    return;
  // Default: without it loop.c lifts the set of zero out of the loop.
  zero = 0;
  state = 0;
  for (i = 0; i <= 16; i++) {
    effect = (struct FieldEffect *)FieldActor_Create(285, leader->x, leader->y,
                                                     leader->z);
    if (effect != 0) {
      state = Sprite_ShareVramCacheEntry(effect->sprite, state);
      zero = 0;
      effect->motionFlags = zero;
      effect->collisionFlags = zero;
      Actor_SetSpriteFlags((struct Actor *)effect, 0);
      Actor_SelectAnimation(effect, 2);
      Actor_StartScript(effect, gMagmaRock_DustScript);
      angle = i << 12;
      effect->velocity.x = Trig_Cos(angle);
      effect->velocity.y = zero;
      effect->velocity.z = Math_Sin(angle);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += (Random16() * 0xc000 >> 16) - 0x6000;
      effect->velocity.z += (Random16() * 0x6000 >> 16) - 0x3000;
      effect->spin = (Random16() * 0x1000 >> 16) - 0x800;
      effect->scaleRateX = zero;
      effect->scaleRateY = zero;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
  }
}

extern u8 gMapShapeGrid[];

s32 MapGrid_GetTerrainKind(s32 terrainId, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 MapCollision_FindOrAddTag(s32 terrainId, s32 value, s32 height);

DEFINE_ACTOR_SET_CELL_SHAPE_AT(MagmaRock_SetActorCellShape,
                               MapGrid_GetTerrainKind,
                               MapCollision_FindOrAddTag)

void MagmaRock_StepOnCrumblingPillar(const u8 *data);

void MagmaRock_SetLeaderCellShape(s32 a, s32 b) {
  MagmaRock_SetActorCellShape(a, b, gMainRoutineWork.currentOwner);
}

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

void BattleEffect_StepVector3(struct Particle *particle, s32 unk1, s32 step);

DEFINE_PARTICLES_UPDATE_FALLING(Particles_UpdateFalling,
                                EffectSprite_DrawAtWorldPosition)

void MagmaRock_DropRocks(s32 x, s32 y, s32 z, s32 count) {
  struct EntryWork *work;
  s32 i;

  work = Ram_HeapSlots->menuRuntime;
  if (count > 0) {
    i = count;
  next:
    {
      struct Entry *entry;
      s32 angle = Random16();

      entry = &work->entries[work->unk1600];
      entry->unk18 = 0;
      entry->x = x;
      entry->y = y;
      entry->z = z;
      FieldVector_AddPolarOffset(Random16() * 8 + 0x80000, angle, entry);
      entry->velocityX = 0;
      entry->velocityY = 0x50000;
      entry->velocityZ = 0;
      FieldVector_AddPolarOffset(Random16() * 2 + 0x20000, angle,
                                 &entry->velocityX);
      {
        s32 mask = 63;

        work->unk1600 = (work->unk1600 + 1) & mask;
      }
      i--;
    }
    if (i != 0)
      goto next;
  }
}

// The falling rocks' setup: loads their picture, sets up 64 small sprites,
// marks each rock idle and starts the task that spawns them.
#include "system/task.h"

extern char ResourceId_FallingRock;

struct Rock {
  u8 unk00[24];
  s32 timer;
};

struct RockSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 unk05_5 : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  u16 resource;
};

void Import_DecompressTaggedLz(void *entry, void *destination);
void EffectSprite_Init(struct RockSprite *sprite, s32 width, s32 height,
                       u32 attributes, s32 tile);

void MagmaRock_StartFallingRocks(void) {
  struct RockWork *work = HeapSlot_Alloc(220, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct RockSprite *sprite;

  Import_DecompressTaggedLz(
      Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--, sprite++, rock++) {
    EffectSprite_Init(sprite, 8, 8, 0x40000000, tile);
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
  Task_AddFrame(Particles_UpdateFalling, 0x480);
}

void MagmaRock_EndFallingRocks(void) {
  struct MagmaRockSparkWork *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(Particles_UpdateFalling);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(220);
}

#include "scene_tables.h"
#include "objcmd.h"

void MagmaRock_UnblockMovedPillar(void);
void MagmaRock_BlockMovedPillar(void);
void MagmaRock_TargetStatue(void);
void MagmaRock_BurstColumn1(void);
void MagmaRock_BurstColumn2(void);
void MagmaRock_BurstColumn3(void);
void MagmaRock_BurstColumn4(void);
void MagmaRock_BurstColumn5(void);
void MagmaRock_BlastOpenWall(void);

// clang-format off
DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

DEFINE_PUSH_LEADER_STEPS(gPushLeaderSteps)

u32 gMagmaRock_EmberScript[] = {
    39, 2, 0, 18, 3, 0,
    1, 38, 17,
};

u32 gMagmaRock_FlameEmberScript[] = {
    39, 3, 0, 23, 3, 0,
    1, 38, 17,
};

s16 gMagmaRock_Room1Pillars[] = {
    8, 10, -1,
};

s16 gMagmaRock_Room1Statues[] = {
    9, 50, 20, 12, 30, -1, -1,
};

s16 gMagmaRock_Room2LowerPillars[] = {
    8, 15, -1,
};

s16 gMagmaRock_Room2LowerStatues[] = {
    13, 30, -1, -1,
};

s16 gMagmaRock_CrumblingPillars[] = {
    9, 512, 10, 514, -1,
};

s16 gMagmaRock_Room2MiddleStatues[] = {
    14, 50, 15, 17, 51, 18, -1,
};

s16 gMagmaRock_Room2MiddlePillars[] = {
    16, -1,
};

s16 gMagmaRock_Room2UpperStatues[] = {
    9, 50, 10, 11, 51, 12, -1,
};

s16 gMagmaRock_Room2UpperPillars[] = {
    8, -1,
};

s16 gMagmaRock_Room2TopPillars[] = {
    8, 9, -1,
};

s16 gMagmaRock_Room2TopStatues[] = {
    11, 30, -1, 12, 50, 13, 15, 31,
    -1, -1,
};

s16 gMagmaRock_Room3Statues[] = {
    9, 50, -1, -1, 0,
};

DEFINE_G_DATA_02001540(gMagmaRock_RubbleScript, 33)

DEFINE_G_DATA_02001540(gMagmaRock_DustScript, 44)

s32 gFieldEffect_DurationScripts[] = {
    (s32)gFieldEffect_ShortScript,
    (s32)gFieldEffect_MediumScript,
    (s32)gFieldEffect_LongScript,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

s16 gMagmaRock_Regions[] = {
    240, 328, 352, 256, 344, 368, -1, 3,
    448, 328, 352, 464, 344, 368, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gMagmaRock_Room2Regions[] = {
    272, 56, 576, 288, 72, 592, -1, 1,
    352, 40, 560, 368, 56, 576, -1, 2,
    752, 512, 536, 768, 528, 552, -1, 5,
    784, 80, 592, 800, 96, 608, -1, 7,
    896, 512, 528, 912, 528, 544, -1, 8,
    272, 48, 576, 288, 64, 592, -1, 9,
    352, 32, 560, 368, 48, 576, -1, 10,
    784, 56, 592, 800, 72, 608, -1, 14,
    752, 0xfea8, 672, 768, 0xfeb8, 688, -1, 16,
    976, 0xfea8, 672, 992, 0xfeb8, 688, -1, 17,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gMagmaRock_Room3Regions[] = {
    240, -56, 448, 256, -40, 464, -1, 1,
    384, -56, 448, 400, -40, 464, -1, 2,
    416, -128, 448, 432, -112, 464, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(223),
    SCENE_EXIT(1, 2, 51),
    SCENE_EXIT(2, 232, 11),
    SCENE_EXIT(3, 224, 16),
    SCENE_EXIT(4, 224, 17),
    SCENE_EXITS(224),
    SCENE_EXIT(1, 224, 9),
    SCENE_EXIT(2, 224, 10),
    SCENE_EXIT(3, 224, 4),
    SCENE_EXIT(4, 224, 3),
    SCENE_EXIT(5, 225, 1),
    SCENE_EXIT(6, 225, 2),
    SCENE_EXIT(7, 224, 14),
    SCENE_EXIT(8, 225, 2),
    SCENE_EXIT(9, 224, 1),
    SCENE_EXIT(10, 224, 2),
    SCENE_EXIT(11, 224, 13),
    SCENE_EXIT(12, 224, 15),
    SCENE_EXIT(13, 224, 11),
    SCENE_EXIT(14, 224, 7),
    SCENE_EXIT(15, 224, 12),
    SCENE_EXIT(16, 223, 3),
    SCENE_EXIT(17, 223, 4),
    SCENE_EXITS(225),
    SCENE_EXIT(1, 224, 5),
    SCENE_EXIT(2, 224, 8),
    SCENE_EXIT(3, 227, 8),
    SCENE_EXIT(4, 224, 6),
    SCENE_EXITS_END,
};

struct ActorPlacement gMagmaRock_Placements[] = {
    {272, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 6, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 7, 0, 0, FACING_WEST, 2, 2},
    {404, CONDITION_ALWAYS, 1, 8, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 9, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 10, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 11, 0, 0, FACING_NORTH, 2, 2},
    {329, CONDITION_ALWAYS, 1, 12, 0, 0, FACING_NORTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_NORTH, 2, 2},
    {403, CONDITION_ALWAYS, 7, PIXELS(8), 0, PIXELS(8), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_Room2LowerPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_NORTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_SOUTH, 2, 3},
    {377, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_EAST, 2, 2},
    {404, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 8, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_Room2MiddlePlacements[] = {
    {377, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 10, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 10, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 10, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 11, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 11, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 12, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 13, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 13, 0, 0, FACING_NORTH, 2, 2},
    {403, CONDITION_ALWAYS, 7, PIXELS(8), 0, PIXELS(8), FACING_WEST, 2, 0},
    {403, CONDITION_ALWAYS, 7, PIXELS(8), 0, PIXELS(8), FACING_WEST, 2, 0},
    {403, CONDITION_ALWAYS, 7, PIXELS(8), 0, PIXELS(8), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_Room2UpperPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 14, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 15, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 15, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 1, 16, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 16, 0, 0, FACING_NORTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 17, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 18, 0, 0, FACING_SOUTH, 2, 3},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_Room2TopPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 19, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 20, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 21, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 7, 22, 0, 0, FACING_WEST, 2, 2},
    {377, CONDITION_ALWAYS, 1, 23, 0, 0, FACING_SOUTH, 2, 2},
    {379, CONDITION_ALWAYS, 1, 23, 0, 0, FACING_NORTH, 2, 2},
    {404, CONDITION_ALWAYS, 1, 24, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 7, 25, 0, 0, FACING_WEST, 2, 2},
    {299, CONDITION_ALWAYS, 7, 26, 0, 0, FACING_NORTH, 2, 2},
    {299, CONDITION_ALWAYS, 7, 27, 0, 0, FACING_NORTH, 2, 2},
    {299, CONDITION_ALWAYS, 7, 28, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRock_Room3Placements[] = {
    {384, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_WEST, 2, 2},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gMagmaRock_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xca02, 2, 0x2f8, EVENT(MagmaRock_SlideDown)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(MagmaRock_ClearSlideFlag)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 19, CONDITION_ALWAYS, 0},
    {8, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     0x8d5, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x8d5,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     0x8d6, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x8d6,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31, 0x200,
     EVENT(MagmaRock_BurstColumn1)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 32, 0x201,
     EVENT(MagmaRock_BurstColumn2)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 33, 0x202,
     EVENT(MagmaRock_BurstColumn3)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 34, 0x203,
     EVENT(MagmaRock_BurstColumn4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 35, 0x204,
     EVENT(MagmaRock_BurstColumn5)},
    {0x80009705, 30, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {0x80009705, 50, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_Room2LowerTriggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0x202, 20, CONDITION_ALWAYS, EVENT(MagmaRock_CheckCrumblingPillars)},
    {8, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     0x8d7, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, 0x8d7,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31, 0x200,
     EVENT(MagmaRock_BurstColumn1)},
    {TRIGGER_STEP, 50, 0x212, EVENT(MagmaRock_SetLedgeBehavior)},
    {TRIGGER_STEP, 51, CONDITION_ALWAYS, EVENT(MagmaRock_ClearLedgeBehavior)},
    {0x80009705, 30, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_Room2MiddleTriggers[] = {
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {8, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16,
     0x8d8, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, 0x8d8,
     EVENT(MagmaRock_BlockMovedPillar)},
    {0x80009705, 50, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {0x80009705, 51, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(MagmaRock_AmplifyBurst)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_Room2UpperTriggers[] = {
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {0x602, 40, 0x8, EVENT(MagmaRock_PushPillarEast)},
    {8, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     0x8d9, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x8d9,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 14, 0x250, 0},
    {0x80009705, 50, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {0x80009705, 51, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 51,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_Room2TopTriggers[] = {
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {0x8602, 40, CONDITION_ALWAYS, EVENT(MagmaRock_PushPillarWest)},
    {8, 0, CONDITION_ALWAYS, EVENT(MagmaRock_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8,
     0x8da, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x8da,
     EVENT(MagmaRock_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9,
     0x8db, EVENT(MagmaRock_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x8db,
     EVENT(MagmaRock_BlockMovedPillar)},
    {0x80009705, 30, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 30,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {0x80009705, 31, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 31,
     CONDITION_ALWAYS, EVENT(MagmaRock_BurstStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 31,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 32, 0x201,
     EVENT(MagmaRock_BurstColumn2)},
    {0x80009705, 50, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_IgniteStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 16, 0x250, EVENT(MagmaRock_GrowVine1)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 17, 0x251, EVENT(MagmaRock_GrowVine2)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 18, 0x252, EVENT(MagmaRock_GrowVine3)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRock_Room3Triggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0x80009705, 50, CONDITION_ALWAYS, EVENT(MagmaRock_ResolveBeforeBurst)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50, 0x2f0,
     EVENT(MagmaRock_TargetStatue)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_BlastOpenWall)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_AFTER_CAST), 50,
     CONDITION_ALWAYS, EVENT(MagmaRock_FireStatue)},
    {TRIGGER_TABLE_END},
};

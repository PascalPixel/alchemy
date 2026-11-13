/// @file
/// Magma Rock's inside: its falling rocks and rubble, and its tables.

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
#include "heap_release.h"
#include "item_icon.h"
#include "module/field/field_effect.h"
#include "module/scene/magma_rock_inside.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "particle.h"
#include "resource.h"

void Actor_SetSpriteFlags(struct FieldEffect *effect, s32 flags);
/* VIEW(s46e8): routine Actor_StartScript as void Actor_StartScript(struct
 * FieldEffect *effect, const s32 *script); local.h has void
 * Actor_StartScript(struct Actor *object, const void *data); not tried:
 * Actor_StartScript's bound declaration was not found. */
extern void
Func_02004ca0__s46e8(struct FieldEffect *effect,
                     const s32 *script) __asm__("Actor_StartScript");
void Actor_StartScript(struct Actor *object, const void *data);

void MagmaRockInside_SpawnRubble(s32 spread) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame = 0;
  u32 i = 0;

  do {
    {
      /* FAKEMATCH(temporary): y held in a local, so x is computed before it; as
       * one expression, 14 of 122 instructions differ in tla-en. */
      s32 y = leader->y - (((Random16Far() * 14) >> 16) << 16);

      effect = (struct FieldEffect *)FieldActor_Create(
          285, (i << 17) + leader->x - 0x80000, y - 0x20000, leader->z);
    }
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Func_02004c98(effect, 2);
      Func_02004ca0__s46e8(effect, gMagmaRockInside_RubbleScript);
      effect->scaleY = effect->scaleX = 0xb333;
      effect->sprite->priority = 1;
      if (spread != 0) {
        effect->velocity.x = Iwram_MulQ16(Trig_Cos(i << 13), 0x13333);
        effect->velocity.z = Math_Sin(i << 13);
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

void MagmaRockInside_SpawnDustRing(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  u32 frame;
  u32 i;

  if (leader == 0)
    return;
  frame = 0;
  i = 0;
  do {
    effect = (struct FieldEffect *)FieldActor_Create(285, leader->x, leader->y,
                                                     leader->z);
    if (effect != 0) {
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      effect->motionFlags = 0;
      effect->collisionFlags = 0;
      Actor_SetSpriteFlags(effect, 0);
      Func_02004c98(effect, 2);
      Func_02004ca0__s46e8(effect, gMagmaRockInside_DustScript);
      effect->velocity.x = Trig_Cos(i << 12);
      effect->velocity.y = 0;
      effect->velocity.z = Math_Sin(i << 12);
      effect->velocity.x += effect->velocity.x >> 1;
      effect->velocity.x += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
      effect->velocity.z += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
      effect->spin = ((Random16Far() << 12) >> 16) - 0x800;
      effect->scaleRateX = 0;
      effect->scaleRateY = 0;
      effect->update = Effect_AdvanceMotionAtLeaderPriority;
      effect->sprite->priority =
          ((struct FieldSprite *)leader->sprite)->priority;
    }
    i++;
  } while (i <= 16);
}

#include "../../../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"
#include "map.h"

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 Func_02004ef0(s32 layer, s32 shape, s32 height);

DEFINE_ACTOR_SET_CELL_SHAPE(MagmaRockInside_SetActorCellShape,
                            MapGrid_GetTerrainKind, Func_02004ef0)

#include "../magma_rock_inside.h"

void MagmaRockInside_SetLeaderCellShape(s32 a, s32 b) {
  MagmaRockInside_SetActorCellShape(a, b, gMainRoutineWork.currentOwner);
}

#include "system/ram_buffer.h"

void BattleEffect_StepVector3(struct Particle *particle, s32 unk1, s32 step);

DEFINE_PARTICLES_UPDATE_FALLING(MagmaRockInside_UpdateFallingRocks,
                                EffectSprite_DrawAtWorldPosition)

/// The falling rocks' work: their tiles, 64 rocks and their sprites, in a
/// ring that next walks round.
struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

/// Spawns count rocks around (x, y, z), each pushed out at a random angle and
/// thrown up and out along it.
DEFINE_ROCKS_SPAWN(MagmaRockInside_DropRocks)

#include "system/task.h"

void MagmaRockInside_StartFallingRocks(void) {
  struct RockWork *work = HeapSlot_AllocFar(220, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct RockSprite *sprite;

  Func_02004c58(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--, sprite++, rock++) {
    Func_02004f10(sprite, 8, 8, 0x40000000, tile);
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
  FieldTask_AddFrame(MagmaRockInside_UpdateFallingRocks, 0x480);
}

void MagmaRockInside_EndFallingRocks(void) {
  struct MagmaRockInsideSparkWork *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(MagmaRockInside_UpdateFallingRocks);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(220);
}

#include "scene_tables.h"
#include "objcmd.h"

void MagmaRockInside_MeetDjinni(void);
void MagmaRockInside_ResolveBeforeBurst(void);
void MagmaRockInside_TargetStatue(void);
void MagmaRockInside_FireStatue(void);
void MagmaRockInside_BurstCrackedPillar1(void);
void MagmaRockInside_BurstCrackedPillar2(void);
void MagmaRockInside_LightTorch(void);
void MagmaRockInside_OpenStairs(void);
void MagmaRockInside_OrbitParent(void);
void Orbit_Update(void);
void Actor_AimAtWatched(void);

extern u32 gFieldEffect_LongScript[];

// clang-format off
DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_LongScript, 126)

DEFINE_PUSH_LEADER_STEPS(gPushLeaderSteps)

u32 gMagmaRockInside_EmberScript[] = {
    39, 2, 0, 18, 3, 0,
    1, 38, 17,
};

u32 gMagmaRockInside_FlameEmberScript[] = {
    39, 3, 0, 23, 3, 0,
    1, 38, 17,
};

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath1, 776, 680, 744, 680, 744, 632, 776, 632)

DEFINE_G_DATA_020050D4(gMagmaRockInside_PlatformPath2, 216, 568, 216, 584, 136, 584, 136, 568)

u16 gMagmaRockInside_PlatformPath3[] = {
    4, 0, 0, 600, 0, 16, 0, 664,
    1, 0, 4, 0, 0, 728, 0, 16,
    0, 664, 1, 0, 4, 0, 0, 728,
    0, 16, 0, 632, 1, 0, 4, 0,
    0, 760, 0, 16, 0, 632, 1, 0,
    4, 0, 0, 760, 0, 16, 0, 664,
    1, 0, 17, 0,
};

DEFINE_G_DATA_020050D4(gMagmaRockInside_PlatformPath4, 184, 232, 280, 232, 280, 264, 184, 264)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath5, 472, 296, 424, 296, 424, 264, 472, 264)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath6, 216, 600, 280, 600, 280, 664, 216, 664)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath7, 312, 664, 376, 664, 376, 600, 312, 600)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath8, 440, 600, 504, 600, 504, 664, 440, 664)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath9, 536, 664, 600, 664, 600, 600, 536, 600)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath10, 376, 264, 376, 232, 408, 232, 408, 264)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath11, 216, 680, 280, 680, 280, 712, 216, 712)

DEFINE_G_DATA_02005074(gMagmaRockInside_PlatformPath12, 376, 680, 312, 680, 312, 712, 376, 712)

s16 gMagmaRockInside_Room2Pillars[] = {
    15, -1,
};

s32 gMagmaRockInside_Statues[] = {
    0x320008, -1,
};

s16 gMagmaRockInside_Room3Pillars[] = {
    11, -1,
};

s16 gMagmaRockInside_Room4Statues[] = {
    8, 50, -1, 13, 51, -1, 14, 52,
    -1, -1,
};

s16 gMagmaRockInside_Room4Pillars[] = {
    23, 24, 25, -1,
};

s16 gMagmaRockInside_Room6Pillars[] = {
    9, 12, -1,
};

s16 gMagmaRockInside_Room7Pillars[] = {
    23, -1, 0,
};

u32 gMagmaRockInside_FlameScript[] = {
    0, 6, 38, 17,
};

DEFINE_G_DATA_02005218(gMagmaRockInside_TorchScript)

s32 gMagmaRockInside_SparkScript[] = {
    SC_CALL(MagmaRockInside_OrbitParent),
    SC_FREE,
    SC_END,
};

s32 MagmaRockInside_DriftForSteps(struct Actor *object);

DEFINE_G_DATA_02005244(gMagmaRockInside_OrbitScript, Orbit_Update, Actor_AimAtWatched,
    MagmaRockInside_DriftForSteps)

DEFINE_G_DATA_02005264(gMagmaRockInside_BlazeCasterScript, 24)

DEFINE_G_DATA_02005264(gMagmaRockInside_BlazeWatcherScript, 16)

DEFINE_G_DATA_020052AC(gMagmaRockInside_BlazeCasterReturnScript, 24, 0)

DEFINE_G_DATA_020052AC(gMagmaRockInside_BlazeWatcherReturnScript, 16, 3)

DEFINE_G_DATA_02001540(gMagmaRockInside_RubbleScript, 33)

DEFINE_G_DATA_02001540(gMagmaRockInside_DustScript, 44)

s32 gFieldEffect_DurationScripts[] = {
    (s32)gFieldEffect_ShortScript,
    (s32)gFieldEffect_MediumScript,
    (s32)gFieldEffect_LongScript,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(226),
    SCENE_EXIT(1, 226, 4),
    SCENE_EXIT(2, 227, 1),
    SCENE_EXIT(3, 227, 5),
    SCENE_EXIT(4, 226, 1),
    SCENE_EXIT(5, 227, 2),
    SCENE_EXIT(6, 227, 3),
    SCENE_EXIT(7, 227, 4),
    SCENE_EXIT(8, 228, 1),
    SCENE_EXITS(227),
    SCENE_EXIT(1, 226, 2),
    SCENE_EXIT(2, 226, 5),
    SCENE_EXIT(3, 226, 6),
    SCENE_EXIT(4, 226, 7),
    SCENE_EXIT(5, 226, 3),
    SCENE_EXIT(6, 227, 7),
    SCENE_EXIT(7, 227, 6),
    SCENE_EXIT(8, 225, 3),
    SCENE_EXITS(228),
    SCENE_EXIT(1, 226, 8),
    SCENE_EXIT(2, 228, 6),
    SCENE_EXIT(3, 229, 1),
    SCENE_EXIT(4, 229, 2),
    SCENE_EXIT(5, 229, 3),
    SCENE_EXIT(6, 228, 2),
    SCENE_EXIT(7, 229, 4),
    SCENE_EXIT(8, 228, 9),
    SCENE_EXIT(9, 228, 8),
    SCENE_EXIT(10, 229, 5),
    SCENE_EXIT(11, 229, 6),
    SCENE_EXITS(229),
    SCENE_EXIT(1, 228, 3),
    SCENE_EXIT(2, 228, 4),
    SCENE_EXIT(3, 228, 5),
    SCENE_EXIT(4, 228, 7),
    SCENE_EXIT(5, 228, 10),
    SCENE_EXIT(6, 228, 11),
    SCENE_EXIT(7, 231, 7),
    SCENE_EXIT(8, 230, 7),
    SCENE_EXIT(9, 230, 8),
    SCENE_EXIT(10, 230, 1),
    SCENE_EXIT(11, 231, 1),
    SCENE_EXIT(12, 231, 2),
    SCENE_EXIT(13, 231, 5),
    SCENE_EXIT(14, 231, 6),
    SCENE_EXITS(230),
    SCENE_EXIT(1, 229, 10),
    SCENE_EXIT(2, 231, 3),
    SCENE_EXIT(3, 230, 9),
    SCENE_EXIT(4, 230, 10),
    SCENE_EXIT(5, 231, 11),
    SCENE_EXIT(6, 231, 4),
    SCENE_EXIT(7, 229, 8),
    SCENE_EXIT(8, 229, 9),
    SCENE_EXIT(9, 230, 3),
    SCENE_EXIT(10, 230, 4),
    SCENE_EXITS(231),
    SCENE_EXIT(1, 229, 11),
    SCENE_EXIT(2, 229, 12),
    SCENE_EXIT(3, 230, 2),
    SCENE_EXIT(4, 230, 6),
    SCENE_EXIT(5, 229, 13),
    SCENE_EXIT(6, 229, 14),
    SCENE_EXIT(7, 229, 7),
    SCENE_EXIT(8, 231, 14),
    SCENE_EXIT(9, 231, 16),
    SCENE_EXIT(10, 231, 17),
    SCENE_EXIT(11, 230, 5),
    SCENE_EXIT(12, 231, 18),
    SCENE_EXIT(13, 231, 19),
    SCENE_EXIT(14, 231, 8),
    SCENE_EXIT(15, 232, 10),
    SCENE_EXIT(16, 231, 9),
    SCENE_EXIT(17, 231, 10),
    SCENE_EXIT(18, 231, 12),
    SCENE_EXIT(19, 231, 13),
    SCENE_EXITS(232),
    SCENE_EXIT(1, 233, 1),
    SCENE_EXIT(2, 232, 5),
    SCENE_EXIT(3, 232, 6),
    SCENE_EXIT(4, 232, 7),
    SCENE_EXIT(5, 232, 2),
    SCENE_EXIT(6, 232, 3),
    SCENE_EXIT(7, 232, 4),
    SCENE_EXIT(8, 232, 9),
    SCENE_EXIT(9, 232, 8),
    SCENE_EXIT(10, 231, 15),
    SCENE_EXIT(11, 223, 2),
    SCENE_EXITS(233),
    SCENE_EXIT(1, 232, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gMagmaRockInside_Placements[] = {
    {319, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRockInside_Room2Placements[] = {
    {377, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_WEST, 2, 2},
    {334, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 2},
    {334, CONDITION_ALWAYS, 7, 3, 0, 0, FACING_NORTH, 2, 3},
    {334, CONDITION_ALWAYS, 7, 4, 0, 0, FACING_NORTH, 2, 3},
    {334, CONDITION_ALWAYS, 7, 5, 0, 0, FACING_NORTH, 2, 3},
    {334, CONDITION_ALWAYS, 7, 6, 0, 0, FACING_NORTH, 2, 3},
    {334, CONDITION_ALWAYS, 7, 7, 0, 0, FACING_NORTH, 2, 3},
    {312, CONDITION_ALWAYS, 7, 8, 0, 0, FACING_NORTH, 2, 2},
    {489, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_NORTH, 2, 2},
    {245, 0x6b, 1, PIXELS(296), 0, PIXELS(504), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRockInside_Room3Placements[] = {
    {377, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_WEST, 2, 2},
    {489, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 3, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 4, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 5, 0, 0, FACING_NORTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_SOUTH, 2, 3},
    {245, 0x68, 1, PIXELS(456), 0, PIXELS(600), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRockInside_Room4Placements[] = {
    {377, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_EAST, 2, 2},
    {404, 0x350, 1, 2, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 3, 0, 0, FACING_NORTH, 2, 2},
    {404, 0x351, 1, 4, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 5, 0, 0, FACING_NORTH, 2, 2},
    {377, CONDITION_ALWAYS, 7, 6, 0, 0, FACING_EAST, 2, 2},
    {377, CONDITION_ALWAYS, 7, 7, 0, 0, FACING_WEST, 2, 2},
    {319, CONDITION_ALWAYS, 7, 8, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 9, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 10, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 11, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 12, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 13, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 14, 0, 0, FACING_NORTH, 2, 3},
    {319, CONDITION_ALWAYS, 7, 15, 0, 0, FACING_NORTH, 2, 3},
    {312, CONDITION_ALWAYS, 7, 16, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 17, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 18, 0, 0, FACING_NORTH, 2, 2},
    {334, CONDITION_ALWAYS, 7, 19, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRockInside_Room5Placements[] = {
    {489, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {489, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 2},
    {373, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(392), FACING_SOUTH, 2, 0},
    {373, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(392), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRockInside_Room6Placements[] = {
    {319, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 3, 0, 0, FACING_NORTH, 2, 2},
    {319, CONDITION_ALWAYS, 7, 4, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 5, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRockInside_Room7Placements[] = {
    {312, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 3},
    {312, CONDITION_ALWAYS, 7, 3, 0, 0, FACING_NORTH, 2, 3},
    {312, CONDITION_ALWAYS, 7, 4, 0, 0, FACING_NORTH, 2, 2},
    {292, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_NORTH, 2, 2},
    {325, CONDITION_ALWAYS, 1, 6, 0, 0, FACING_NORTH, 2, 2},
    {325, CONDITION_ALWAYS, 1, 7, 0, 0, FACING_NORTH, 2, 3},
    {325, CONDITION_ALWAYS, 1, 8, 0, 0, FACING_NORTH, 2, 3},
    {325, CONDITION_ALWAYS, 1, 9, 0, 0, FACING_NORTH, 2, 3},
    {364, CONDITION_ALWAYS, 1, 10, 0, 0, FACING_NORTH, 2, 2},
    {364, CONDITION_ALWAYS, 1, 11, 0, 0, FACING_NORTH, 2, 3},
    {364, CONDITION_ALWAYS, 1, 12, 0, 0, FACING_NORTH, 2, 3},
    {364, CONDITION_ALWAYS, 1, 13, 0, 0, FACING_NORTH, 2, 3},
    {365, CONDITION_ALWAYS, 1, 14, 0, 0, FACING_NORTH, 2, 2},
    {365, CONDITION_ALWAYS, 1, 15, 0, 0, FACING_NORTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 16, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMagmaRockInside_Room8Placements[] = {
    {323, CONDITION_ALWAYS, 7, 1, 0, 0, FACING_SOUTH, 2, 2},
    {312, CONDITION_ALWAYS, 7, 2, 0, 0, FACING_NORTH, 2, 2},
    {365, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_NORTH, 2, 2},
    {365, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_NORTH, 2, 2},
    {5, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gMagmaRockInside_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {0x31, 8, CONDITION_ALWAYS, 8},
    {0x202, 70, CONDITION_ALWAYS, EVENT(MagmaRockInside_PushPillarIntoLava)},
    {0x202, 78, CONDITION_ALWAYS, EVENT(MagmaRockInside_PushPillarIntoLava)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRockInside_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(MagmaRockInside_FloodWithLava)},
    {0xc400, 17, CONDITION_ALWAYS, EVENT(MagmaRockInside_MeetDjinni)},
    {8, 0, CONDITION_ALWAYS, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15,
     0x9f0, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, 0x9f0,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {0x80009705, 50, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ResolveBeforeBurst)},
    {0x50009705, 50, CONDITION_ALWAYS, EVENT(MagmaRockInside_TargetStatue)},
    {0x9705, 50, CONDITION_ALWAYS, EVENT(MagmaRockInside_FireStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRockInside_Room3Triggers[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(MagmaRockInside_FloodWithLava)},
    {0x202, 70, CONDITION_ALWAYS, EVENT(MagmaRockInside_PushPillarIntoLava)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(MagmaRockInside_FightFury)},
    {8, 0, CONDITION_ALWAYS, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 11,
     0x9f1, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x9f1,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {0x80009705, 50, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ResolveBeforeBurst)},
    {0x50009705, 50, CONDITION_ALWAYS, EVENT(MagmaRockInside_TargetStatue)},
    {0x9705, 50, CONDITION_ALWAYS, EVENT(MagmaRockInside_FireStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRockInside_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {0x202, 70, CONDITION_ALWAYS, EVENT(MagmaRockInside_PushPillarIntoLava)},
    {0x202, 71, 0x1301, EVENT(MagmaRockInside_PushPillarIntoLava)},
    {0x202, 72, CONDITION_ALWAYS, EVENT(MagmaRockInside_PushPillarIntoLava)},
    {0xc602, 73, CONDITION_ALWAYS, EVENT(MagmaRockInside_PushPillarIntoLava)},
    {0x8602, 75, CONDITION_ALWAYS, EVENT(MagmaRockInside_PushPillarIntoLava)},
    {8, 0, CONDITION_ALWAYS, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 23,
     0x9f2, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 23, 0x9f2,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 24,
     0x9f3, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 24, 0x9f3,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 25,
     0x9f4, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 25, 0x9f4,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {0x50009705, 71, 0x350, EVENT(MagmaRockInside_BurstCrackedPillar1)},
    {0x50009705, 72, 0x351, EVENT(MagmaRockInside_BurstCrackedPillar2)},
    {0x80009705, 50, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ResolveBeforeBurst)},
    {0x50009705, 50, 0x2f0, EVENT(MagmaRockInside_TargetStatue)},
    {0x50009705, 50, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_BurstCrackedPillar1)},
    {0x9705, 50, CONDITION_ALWAYS, EVENT(MagmaRockInside_FireStatue)},
    {0x80009705, 51, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ResolveBeforeBurst)},
    {0x50009705, 51, CONDITION_ALWAYS, EVENT(MagmaRockInside_TargetStatue)},
    {0x9705, 51, CONDITION_ALWAYS, EVENT(MagmaRockInside_FireStatue)},
    {0x80009705, 52, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ResolveBeforeBurst)},
    {0x50009705, 52, CONDITION_ALWAYS, EVENT(MagmaRockInside_TargetStatue)},
    {0x9705, 52, CONDITION_ALWAYS, EVENT(MagmaRockInside_FireStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRockInside_Room5Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(MagmaRockInside_FloodWithLava)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(MagmaRockInside_FloodWithLava)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, 0},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRockInside_Room6Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {0x31, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXIT, 19, CONDITION_ALWAYS, 19},
    {0x202, 70, 0x1302, EVENT(MagmaRockInside_PushPillarIntoLava)},
    {8, 0, CONDITION_ALWAYS, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9,
     0x9f5, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x9f5,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 12,
     0x9f6, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x9f6,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMagmaRockInside_Room7Triggers[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(MagmaRockInside_OpenStairs)},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0x21, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, 30, 0x9f8, EVENT(MagmaRockInside_ReleaseRocks)},
    {TRIGGER_EXAMINE, 31, 0x9f9, EVENT(MagmaRockInside_ExamineEmberStone)},
    {8, 0, CONDITION_ALWAYS, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {9, 0, CONDITION_ALWAYS, EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 23,
     0x9f7, EVENT(MagmaRockInside_UnblockMovedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 23, 0x9f7,
     EVENT(MagmaRockInside_BlockMovedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 17,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_TakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 18,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_TakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 19,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_TakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 20,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_TakeFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 21,
     CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 22,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_LightTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 22,
     0x304, EVENT(MagmaRockInside_GrowFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 21,
     0x303, EVENT(MagmaRockInside_GrowFlame)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 17, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 18, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 19, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 20, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 22, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutFlames)},
    {TRIGGER_TABLE_END},
};

s32 gMagmaRockInside_LastStepCell = -1;

struct Trigger gMagmaRockInside_Room8Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 80, CONDITION_ALWAYS, EVENT(MagmaRockInside_StampStepCell)},
    {TRIGGER_STEP, 81, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StampOtherStepCell)},
    {0xc403, 30, CONDITION_ALWAYS, EVENT(MagmaRockInside_ReadBlazeTablet)},
    {0x4403, 30, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ReadBlazeTabletFromWest)},
    {0x8403, 30, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ReadBlazeTabletFromEast)},
    {0x403, 30, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_ReadBlazeTabletFromSouth)},
    {TRIGGER_RAISED, 202, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StepOnMarker)},
    {TRIGGER_RAISED, 203, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StepOnMarker)},
    {TRIGGER_RAISED, 204, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StepOnMarker)},
    {TRIGGER_RAISED, 205, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StepOnMarker)},
    {TRIGGER_RAISED, 206, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_StepOnMarker)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     CONDITION_ALWAYS, EVENT(MagmaRockInside_LightTabletTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 11,
     CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 11,
     0x305, EVENT(MagmaRockInside_GrowTabletFlame)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 10, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutTabletFlame)},
    {PSYNERGY_ON_ACTOR(ABILITY_BLAZE), 11, CONDITION_ALWAYS,
     EVENT(MagmaRockInside_PutOutTabletFlame)},
    {TRIGGER_TABLE_END},
};

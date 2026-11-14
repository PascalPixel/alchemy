/// @file
/// The second debug room of Psynergy tests: Douse and Pound take an actor away,
/// Grind and Tremor are tried on another, which scatters rocks when talked to,
/// Burst breaks a wall, a rope is coiled, and one step starts a battle.

#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "field/field_sequence_event.h"
#include "call.h"
#include "item_icon.h"
#include "effect_sprite.h"
#include "flags.h"
#include "field/particle.h"

#include "module/scene/kibombo.h"
#include "module/tla.h"
#include "field/particle.h"
#include "graphics/resource.h"
#include "gamepak/far_call/system.h"
#include "system/task.h"
#include "fixed_math.h"

// The battle the step on cell 30 starts, once.
#define FLAG_BATTLE_FOUGHT FLAG_SCENE_TEMP_20

/// The rocks a talk scatters, in heap slot 220: their tiles, 64 rocks and their
/// sprites, the next rock to use, and where the tiles are in VRAM.
struct RockWork {
  u8 tiles[0x500];
  struct Particle rocks[64];
  struct EffectSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

void PsynergyTest2_SpawnRocks(s32 x, s32 y, s32 z, s32 count);

void PsynergyTest2_StartRocks(void);
void PsynergyTest2_EndRocks(void);

void Rope_Coil(s32 a, s32 b, s32 c);

void BattleEffect_StepVector3(struct Particle *particle, s32 damping,
                              s32 gravity);

void Field_RequestBattleFromEncounterTable(s32 a, s32 b);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Douse on actor 8 takes it off the map.
void PsynergyTest2_DouseTarget(void) {
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
}

/// So does Pound, as it acts.
void PsynergyTest2_PoundTarget(s32 mode) {
  if (mode == 1)
    Actor_SetPosition(8, 0, 0);
}

/// The event for Tremor on actor 9, which does nothing.
void PsynergyTest2_IgnoreTremor(void) {}

/// Grind on actor 9 puts it in mode 3.
void PsynergyTest2_GrindActor(void) { Actor_SetModeById(9, 3); }

/// Burst on cell 50 copies in the broken wall, a 1 by 2 block from (49, 1),
/// cell behaviors and all.
void PsynergyTest2_BurstWall(s32 mode) {
  if (mode == 1) {
    MapGrid_CopyCellsLayered(49, 1, 1, 2, 34, mode);
    MapGrid_CopyCellAttributes(49, 1, 1, 2, 34, 3);
  }
}

/// Stepping onto cell 30 starts a battle, the first time only.
void PsynergyTest2_StepIntoBattle(void) {
  if (!Event_GetFlag(FLAG_BATTLE_FOUGHT)) {
    Event_SetFlag(FLAG_BATTLE_FOUGHT);
    Field_RequestBattleFromEncounterTable(14, 0);
  }
}

/// Talking to actor 9: twenty rocks burst out around it and fall for 20
/// frames.
void PsynergyTest2_ScatterRocks(s32 id) {
  struct Actor *object = Actor_Get(id);

  PsynergyTest2_StartRocks();
  PsynergyTest2_SpawnRocks(object->x.fixed, object->y.fixed, object->z.fixed,
                           20);
  System_Wait(20);
  PsynergyTest2_EndRocks();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets screen transition 256 and event mode 2, and coils the rope.
s32 Scene_Initialize(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition = 256;
  work->sceneAttributeHigh = 0;
  work->sceneAttributeLow = 0;
  gMainRoutineWork.messageSpeed = 2;
  Rope_Initialize();
  // Coils rope 0 between actors 10 and 11.
  Value3((s32 (*)())Rope_Coil, 0, 10, 11);
  Actor_SetAnimation(9, 4);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct Particle *particle);

/// Each frame of the rocks: each of the 64 younger than 20 frames is drawn,
/// its picture one of eight as it ages, and falls.
void PsynergyTest2_UpdateRocks(void) {
  struct RockWork *work = Ram_HeapSlots->menuWork;
  struct Particle *particle = work->rocks;
  struct EffectSprite *sprite = work->sprites;
  s32 i;
  s32 step = -0x8000;

  i = 63;
  do {
    if ((u32)particle->variant <= 19) {
      sprite->oam.index = work->tileBase + ((particle->variant >> 2) & 7) * 8;
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      BattleEffect_StepVector3(particle, 63, step);
      particle->variant++;
    }
    i--;
    sprite++;
    particle++;
  } while (i >= 0);
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 408, 0, 88, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
    {1, CONDITION_ALWAYS, 408, 0, 408, FACING_NORTH, 0, 280, 0, 780, 464, 0},
    {2, CONDITION_ALWAYS, 504, 0, 232, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(PsynergyTest),
    SCENE_EXIT(20, PsynergyTest, 2),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(306, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 104,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 248,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 104,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 136,
                    FACING_SOUTH, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(PsynergyTest2_ScatterRocks)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(PsynergyTest2_StepIntoBattle)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 8, CONDITION_ALWAYS,
     EVENT(PsynergyTest2_DouseTarget)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8,
     CONDITION_ALWAYS, EVENT(PsynergyTest2_PoundTarget)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), 9, CONDITION_ALWAYS,
     EVENT(PsynergyTest2_IgnoreTremor)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 9, CONDITION_ALWAYS,
     EVENT(PsynergyTest2_GrindActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 10, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 50,
     CONDITION_ALWAYS, EVENT(PsynergyTest2_BurstWall)},
    {TRIGGER_TABLE_END},
};

/// Throws `count` rocks out of (x, y, z): each starts 8 pixels or more away
/// at a random angle and flies up and outward.
void PsynergyTest2_SpawnRocks(s32 x, s32 y, s32 z, s32 count) {
  struct RockWork *work = Ram_HeapSlots->menuWork;
  struct Particle *rock;
  s32 angle;
  s32 i;

  if (count > 0) {
    i = count;
  loop:
    angle = Random16();
    rock = &work->rocks[work->next];
    rock->variant = 0;
    rock->x = x;
    rock->y = y;
    rock->z = z;
    FieldVector_AddPolarOffset((Random16() << 3) + px(8), angle, &rock->x);
    rock->velocityX = 0;
    rock->velocityY = px(5);
    rock->velocityZ = 0;
    FieldVector_AddPolarOffset((Random16() << 1) + px(2), angle,
                               &rock->velocityX);
    work->next++;
    work->next &= 63;
    i--;
    if (i != 0)
      goto loop;
  }
}

/// Loads the rocks' tiles, hides all 64 and starts PsynergyTest2_UpdateRocks.
void PsynergyTest2_StartRocks(void) {
  struct RockWork *work =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Particle *rock;
  struct EffectSprite *sprite;

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_FallingRock),
                      work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--, sprite++, rock++) {
    EffectSprite_Init(sprite, 8, 8, 0x40000000, tile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    rock->variant = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  FieldTask_AddFrame(PsynergyTest2_UpdateRocks, 0x480);
}

/// Stops the rocks and frees their tiles and work.
void PsynergyTest2_EndRocks(void) {
  struct RockWork *work = Ram_HeapSlots->menuWork;

  FieldTask_Remove(PsynergyTest2_UpdateRocks);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

UNUSED void PsynergyTest2_DigCells(s32 mode, s32 a, s32 b, s32 c, s32 d, s32 e,
                                   s32 f) {
  if (mode == 1)
    MapGrid_CopyCells(a, b, c, d, e, f);
}

UNUSED void PsynergyTest2_GrowActor(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;
  s32 scale;

  object = Actor_Get(id);
  if (mode == 1) {
    Actor_SetPositionFixed(id, x << 16, z << 16);
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

UNUSED void PsynergyTest2_DropActor(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;

  object = Actor_Get(id);
  if (mode == 2) {
    Actor_SetPositionFixed(id, px(x), px(z));
    object->y.fixed = px(32);
    object->motionFlags = 3;
    object->gravity = 0x170a;
    System_Wait(50);
  }
}

// Unused: the Psynergy test's Scoop helpers (PsynergyTest_DigCells and the
// rest), copied here with nothing calling them.
UNUSED void PsynergyTest2_RaiseMonster(s32 mode, s32 x, s32 z, s32 row,
                                       s32 column) {
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
      object->velocityY = px(6);
      object->scaleX = 0x6666;
      object->scaleY = 0x6666;
      while (object->scaleX < Q16_ONE) {
        System_Wait(1);
        scale = object->scaleX + 0x51e;
        {
          /* FAKEMATCH(temporary): the turn is added as a word. */
          s32 angle = object->facing + TRIG_TURN / 16;

          object->facing = angle;
        }
        object->scaleX = scale;
        object->scaleY = scale;
      }
      object->scaleX = Q16_ONE;
      object->scaleY = Q16_ONE;
      object->facing =
          Actor_Get(gMainRoutineWork.leaderActorId)->facing + TRIG_TURN / 2;
      Field_RequestBattleFromEncounterTable(row, column);
    }
  }
}

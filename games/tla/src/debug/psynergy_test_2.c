/// @file
/// The second debug room of Psynergy tests, on the east side of the Psynergy
/// test's map, where a fire, a stake sunk flat and two posts stand: Douse and
/// Pound take the fire away, Grind stands the stake up and Tremor on it does
/// nothing, talking to it scatters rocks, and a rope is coiled between the
/// posts. Its Burst row answers the Psynergy test's first Scoop spot, west of
/// this room's view, and its battle row a cell behavior no cell holds.

#include "battle/battle.h"
#include "field/field_sequence.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/io_reg.h"
#include "types.h"
#include "sprite_ids.h"
#include "field/scene_tables.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "field/field_sequence_event.h"
#include "field/call.h"
#include "window/item_icon.h"
#include "window/message_speed.h"
#include "field/effect_sprite.h"
#include "flags.h"
#include "field/particle.h"

#include "scene/kibombo.h"
#include "scene/falling_rocks.h"
#include "graphics/resource.h"
#include "gamepak/far_call/system.h"
#include "system/task.h"
#include "fixed_math.h"

/// Set once the step onto CELL_BEHAVIOR_BATTLE_STEP has started its battle.
#define FLAG_BATTLE_FOUGHT FLAG_SCENE_TEMP_20

/// The cell behaviors the room's triggers answer.
enum PsynergyTest2CellBehavior {
  /// A step here starts a battle; no cell of the map holds it.
  CELL_BEHAVIOR_BATTLE_STEP = 30,
  /// The Psynergy test's first Scoop spot, cell (10, 10), west of this room.
  CELL_BEHAVIOR_HOLE_SPOT = 50
};

/// The stage Pound passes on the frame its fist lands.
enum { POUND_STAGE_LANDS = 1 };

/// The stage Scoop passes its spot's event each frame of the dig.
enum { SCOOP_STAGE_DUG = 1, SCOOP_STAGE_AFTER_DIG };

/// The room's actors, numbered in the order of gActorPlacements.
enum PsynergyTest2ActorId {
  ACTOR_FIRE = ACTOR_FIRST_PLACED, ///< Douse and Pound take it off the map
  ACTOR_STAKE,       ///< Grind and Tremor are tried on it; talked to, rocks fly
  ACTOR_FIRST_POST,  ///< the rope's first end; Lash at it does nothing
  ACTOR_SECOND_POST, ///< the rope's other end
};

#define ROCK_FRAMES 20 ///< how long a rock flies

/// The rocks a talk scatters, in the menu's heap slot: the scenes' struct
/// RockWork with each rock read as a particle.
struct RockParticleWork {
  u8 tiles[ROCK_SHEET_BYTES];
  struct Particle rocks[ROCK_COUNT];
  struct EffectSprite sprites[ROCK_COUNT];
  s16 next;
  u16 tileBase;
  s16 resource;
};

void PsynergyTest2_SpawnRocks(s32 x, s32 y, s32 z, s32 count);

void PsynergyTest2_StartRocks(void);
void PsynergyTest2_EndRocks(void);

void Rope_Coil(s32 rope, s32 firstActor, s32 secondActor);

void BattleEffect_StepVector3(struct Particle *particle, s32 damping,
                              s32 gravity);

void Field_RequestBattleFromEncounterTable(s32 zone, s32 slot);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Douse on ACTOR_FIRE takes it off the map.
void PsynergyTest2_DouseFire(void) { Actor_SetPosition(ACTOR_FIRE, 0, 0); }

/// So does Pound, as its fist lands.
void PsynergyTest2_PoundFire(s32 stage) {
  if (stage == POUND_STAGE_LANDS)
    Actor_SetPosition(ACTOR_FIRE, 0, 0);
}

/// The event for Tremor on ACTOR_STAKE, which does nothing.
void PsynergyTest2_IgnoreTremor(void) {}

/// Grind on ACTOR_STAKE stands the stone stake up again.
void PsynergyTest2_GrindStake(void) { Actor_SetAnimationById(ACTOR_STAKE, 3); }

/// Burst at CELL_BEHAVIOR_HOLE_SPOT, as its great blast dies down, copies the
/// 1 by 2 block of cells at (49, 1) over (34, 1), and its cell behaviors over
/// (34, 3).
void PsynergyTest2_BurstCells(s32 stage) {
  if (stage == BURST_STAGE_DYING_DOWN) {
    MapGrid_CopyCellsLayered(49, 1, 1, 2, 34, 1);
    MapGrid_CopyCellAttributes(49, 1, 1, 2, 34, 3);
  }
}

/// Stepping onto CELL_BEHAVIOR_BATTLE_STEP starts a battle, the first time
/// only; no cell holds it, so it never runs.
void PsynergyTest2_StepIntoBattle(void) {
  if (!Event_GetFlag(FLAG_BATTLE_FOUGHT)) {
    Event_SetFlag(FLAG_BATTLE_FOUGHT);
    Field_RequestBattleFromEncounterTable(
        ENCOUNTER_ZONE_WORLD_MAP_BEACH_AND_GRASSLAND, 0);
  }
}

/// Talking to ACTOR_STAKE: twenty rocks burst out around it and fall for 20
/// frames.
void PsynergyTest2_ScatterRocks(s32 id) {
  struct Actor *actor = Actor_Get(id);

  PsynergyTest2_StartRocks();
  PsynergyTest2_SpawnRocks(actor->x.fixed, actor->y.fixed, actor->z.fixed, 20);
  System_Wait(20);
  PsynergyTest2_EndRocks();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Fades the room in over the backdrop, sets the fast message speed, coils the
/// rope and shows the stake sunk flat.
s32 Scene_Initialize(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->sceneAttributeHigh = 0;
  work->sceneAttributeLow = 0;
  gMainRoutineWork.messageSpeed = MESSAGE_SPEED_FAST;
  Rope_Initialize();
  // Coils rope 0 between the two posts.
  Value3((s32 (*)())Rope_Coil, 0, ACTOR_FIRST_POST, ACTOR_SECOND_POST);
  Actor_SetAnimation(ACTOR_STAKE, 4);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct Particle *particle);

/// Each frame of the rocks: each rock younger than ROCK_FRAMES is drawn, its
/// picture one of eight as it ages, and falls.
void PsynergyTest2_UpdateRocks(void) {
  struct RockParticleWork *work = Ram_HeapSlots->menuWork;
  struct Particle *particle = work->rocks;
  struct EffectSprite *sprite = work->sprites;
  s32 i;
  s32 step = -toFIXED(1) / 2;

  i = ROCK_COUNT - 1;
  do {
    if ((u32)particle->variant < ROCK_FRAMES) {
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

#include "../scene/shared/psynergy_test_exits.c"

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0,
                    104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0,
                    248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 360, 0, 104, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 488, 0, 136, FACING_SOUTH,
                    TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_TALK, ACTOR_STAKE, CONDITION_ALWAYS,
     EVENT(PsynergyTest2_ScatterRocks)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BATTLE_STEP, CONDITION_ALWAYS,
     EVENT(PsynergyTest2_StepIntoBattle)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRE, CONDITION_ALWAYS,
     EVENT(PsynergyTest2_DouseFire)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_FIRE,
     CONDITION_ALWAYS, EVENT(PsynergyTest2_PoundFire)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_STAKE, CONDITION_ALWAYS,
     EVENT(PsynergyTest2_IgnoreTremor)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), ACTOR_STAKE, CONDITION_ALWAYS,
     EVENT(PsynergyTest2_GrindStake)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_FIRST_POST, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_HOLE_SPOT, CONDITION_ALWAYS,
     EVENT(PsynergyTest2_BurstCells)},
    {TRIGGER_TABLE_END},
};

/// Throws `count` rocks out of (x, y, z): each starts 8 pixels or more away
/// at a random angle and flies up and outward.
void PsynergyTest2_SpawnRocks(s32 x, s32 y, s32 z, s32 count) {
  struct RockParticleWork *work = Ram_HeapSlots->menuWork;
  struct Particle *rock;
  s32 angle;
  s32 i;

  if (count > 0) {
    i = count;
    do {
      angle = Random16();
      rock = &work->rocks[work->next];
      rock->variant = 0;
      rock->x = x;
      rock->y = y;
      rock->z = z;
      FieldVector_AddPolarOffsetFX((Random16() << 3) + toFIXED(8), angle,
                                   &rock->x);
      rock->velocityX = 0;
      rock->velocityY = toFIXED(5);
      rock->velocityZ = 0;
      FieldVector_AddPolarOffsetFX((Random16() << 1) + toFIXED(2), angle,
                                   &rock->velocityX);
      work->next++;
      work->next &= ROCK_COUNT - 1;
      i--;
    } while (i != 0);
  }
}

/// Loads the rocks' tiles, hides every rock and starts
/// PsynergyTest2_UpdateRocks.
void PsynergyTest2_StartRocks(void) {
  struct RockParticleWork *work =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct RockParticleWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Particle *rock;
  struct EffectSprite *sprite;

  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_FALLING_ROCK), work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, ROCK_SHEET_BYTES, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  for (i = 0; i < ROCK_COUNT; i++) {
    sprite = &work->sprites[i];
    EffectSprite_Init(sprite, 8, 8, OBJ_SIZE_16X16, tile);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = OAM_DEPTH_WINDOW;
    rock->variant = -1;
    rock++;
  }
  work->next = 0;
  FieldTask_AddFrame(PsynergyTest2_UpdateRocks, TASK_PASS_FRAME);
}

/// Stops the rocks and frees their tiles and work.
void PsynergyTest2_EndRocks(void) {
  struct RockParticleWork *work = Ram_HeapSlots->menuWork;

  FieldTask_Remove(PsynergyTest2_UpdateRocks);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

/// Unused: the Psynergy test's PsynergyTest_DigCells again.
UNUSED void PsynergyTest2_DigCells(s32 stage, s32 sourceX, s32 sourceY,
                                   s32 width, s32 height, s32 destinationX,
                                   s32 destinationY) {
  if (stage == SCOOP_STAGE_DUG)
    MapGrid_CopyCells(sourceX, sourceY, width, height, destinationX,
                      destinationY);
}

/// Unused: the Psynergy test's PsynergyTest_GrowActor again.
UNUSED void PsynergyTest2_GrowActor(s32 stage, s32 id, s32 x, s32 z) {
  struct Actor *actor;
  s32 scale;

  actor = Actor_Get(id);
  if (stage == SCOOP_STAGE_DUG) {
    Actor_SetPosition(id, x, z);
    actor->scaleX = 0;
    actor->scaleY = 0;
  }
  if (stage == SCOOP_STAGE_AFTER_DIG) {
    while (actor->scaleX < FIXED_ONE) {
      System_Wait(1);
      scale = actor->scaleX;
      scale += toFIXED(0.02);
      actor->scaleX = scale;
      actor->scaleY = scale;
    }
    actor->scaleX = FIXED_ONE;
    actor->scaleY = FIXED_ONE;
  }
}

/// Unused: the Psynergy test's PsynergyTest_DropActor again.
UNUSED void PsynergyTest2_DropActor(s32 stage, s32 id, s32 x, s32 z) {
  struct Actor *actor;

  actor = Actor_Get(id);
  if (stage == SCOOP_STAGE_AFTER_DIG) {
    Actor_SetPosition(id, x, z);
    actor->y.fixed = toFIXED(32);
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    actor->gravity = toFIXED(0.09);
    System_Wait(50);
  }
}

/// Unused: the Psynergy test's PsynergyTest_RaiseStatue again.
UNUSED void PsynergyTest2_RaiseStatue(s32 stage, s32 x, s32 z, s32 row,
                                      s32 column) {
  struct Actor *actor;

  if (stage == SCOOP_STAGE_DUG) {
    actor = FieldActor_Create(SPRITE_BROWN_BOULDER, toFIXED(x), 0, toFIXED(z));
    if (actor != NULL) {
      Actor_SetSpriteFlags(actor, 0);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    }
  }
  if (stage == SCOOP_STAGE_AFTER_DIG) {
    actor =
        (FieldActor_Create(SPRITE_WINGED_STATUE, toFIXED(x), 0, toFIXED(z)));
    if (actor != NULL) {
      s32 scale;

      Actor_SetSpriteFlags(actor, 0);
      actor->velocityY = toFIXED(6);
      actor->scaleX = FIXED_ONE * 2 / 5;
      actor->scaleY = FIXED_ONE * 2 / 5;
      while (actor->scaleX < FIXED_ONE) {
        System_Wait(1);
        scale = actor->scaleX + toFIXED(0.02);
        actor->facing += FACING_STEP;
        actor->scaleX = scale;
        actor->scaleY = scale;
      }
      actor->scaleX = FIXED_ONE;
      actor->scaleY = FIXED_ONE;
      actor->facing =
          Actor_Get(gMainRoutineWork.leaderActorId)->facing + DEGtoANG(180);
      Field_RequestBattleFromEncounterTable(row, column);
    }
  }
}

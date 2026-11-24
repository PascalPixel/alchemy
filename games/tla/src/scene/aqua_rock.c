/// @file
/// Aqua Rock's outside, the island south of the Apojii Islands, in two rooms
/// of this overlay. On the slopes, red moai pour streams that stop now and
/// then, a pillar Moved before one stops it for good, Douse fills a dry moai
/// and turns a Water Stone's streams back uphill, and the guardian moai step
/// aside once the Apojii's Aqua Stone is Doused. In the Mist Sabre chest's
/// room, Frost freezes puddles into steps, Move drops two rocks into the
/// stream, and Douse on its Water Stone opens the way inside
/// (aqua_rock_inside.c).

#include "../field/actor_set_sprite_priority.c"
#include "field/field_sequence.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "scene/dust.h"
#include "field/event_commands.h"
#include "field/objdisp.h"

#include "../field/actor_prepare.c"

#include "types.h"
#include "graphics/output.h"
#include "system/system.h"
#include "system/task.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
#include "field/scene_tables.h"
#include "system/iwram_call.h"
#include "system/io_reg.h"
#include "field/field_sprite.h"
#include "system/frame.h"
#include "graphics/palette_buffers.h"

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);
void Sprite_StartAnimation(void *animation, s32 value);
u8 *Sprite_AddAnimation(void *animation, s32 id);

#include "scene_ids.h"

/// The bubbles a stream carries along at once.
#define STREAM_BUBBLE_COUNT 64

/// A bubble a stream carries: its place, its age in frames and its sprite's
/// attributes.
struct Bubble {
  s32 x;
  s32 y;
  s32 z;
  volatile s32 age;
  u32 oam[3];
};

/// The bubbles' state: the actor they come from, their VRAM entry, the next
/// free bubble, which way they go (up the stream when positive) and the
/// bubbles.
struct AquaRockBubbles {
  struct Actor *anchor;
  u16 vramEntry;
  u16 next;
  s16 direction;
  u8 unusedAfterDirection[2]; ///< no code names it
  struct Bubble bubbles[STREAM_BUBBLE_COUNT];
};

void Actor_StartScript(struct Actor *actor, const void *data);
s32 Actor_EmitMist(struct Actor *actor);

void AquaRock_TurnFish(struct Actor *actor);
void AquaRock_FlowSparkleUpStream(struct Actor *actor);
void AquaRock_DrawRisingBubbles(void);
void AquaRock_DrawFallingBubbles(void);
void AquaRock_DrawBubbles(void);

extern u32 gWadingSplash_Script[];
extern u32 gWaterDropScript[];
/// The bubbles' four frames, compressed (graphics/aqua_rock/tiles.s).
extern const u8 gAquaRock_MarkerTiles[];
extern struct SceneRegion gAquaRock_SlopesRegions[];
extern struct SceneRegion gAquaRock_MistSabreChestRegions[];
extern struct ActorPlacement gAquaRock_Placements[];
extern struct ActorPlacement gAquaRock_SlopesPlacements[];
extern struct ActorPlacement gAquaRock_MistSabreChestPlacements[];

/// Aqua Rock's lasting flags outside: the slopes' second pillar Moved into
/// the hole in its ledge, the two rocks Moved into the Mist Sabre chest's
/// room's stream, the guardian moai stepped aside, the streams the slopes'
/// Water Stone has turned back (each Douse turns them again), the Mist Sabre
/// chest's room's Water Stone Doused, and the dry moai filled.
#define FLAG_AQUA_ROCK_PILLAR_IN_HOLE 0x950
#define FLAG_AQUA_ROCK_FIRST_ROCK_DROPPED 0x951
#define FLAG_AQUA_ROCK_SECOND_ROCK_DROPPED 0x952
#define FLAG_AQUA_ROCK_MOAI_STEPPED_ASIDE 0x955
#define FLAG_AQUA_ROCK_STREAMS_TURNED_BACK 0xa68
#define FLAG_AQUA_ROCK_MIST_SABRE_CHEST_WATER_STONE_DOUSED 0xa69
#define FLAG_AQUA_ROCK_DRY_MOAI_FILLED 0xa6b
/// While the party is on the island: the cell of the pillar Move sets before
/// the top's red moai, kept as two bytes, and whether it stands before the
/// west or the east one, which stops that moai's stream.
#define FLAG_PILLAR_COLUMN FLAG_AREA_TEMP_10
#define FLAG_PILLAR_ROW FLAG_AREA_TEMP_18
#define FLAG_PILLAR_BEFORE_WEST_MOAI FLAG_AREA_TEMP_20
#define FLAG_PILLAR_BEFORE_EAST_MOAI FLAG_AREA_TEMP_21
/// This visit's flags. Each of the four red moai's streams, stopped
/// (AquaRock_TimeStreams).
#define FLAG_WEST_MOAI_STREAM_STOPPED FLAG_SCENE_TEMP_00
#define FLAG_EAST_MOAI_STREAM_STOPPED FLAG_SCENE_TEMP_01
#define FLAG_LOWER_WEST_MOAI_STREAM_STOPPED FLAG_SCENE_TEMP_02
#define FLAG_LOWER_EAST_MOAI_STREAM_STOPPED FLAG_SCENE_TEMP_03
/// Frost on each of the Mist Sabre chest's room's three puddles.
#define FLAG_PUDDLE_A_FROZEN FLAG_SCENE_TEMP_21
#define FLAG_PUDDLE_B_FROZEN FLAG_SCENE_TEMP_22
#define FLAG_PUDDLE_C_FROZEN FLAG_SCENE_TEMP_23

/// The cells before the top's west and east red moai, where the pillar stops a
/// stream; the column of the hole in the ledge; and the column of the Mist
/// Sabre chest's room's stream, where a rock Moved drops in.
#define WEST_MOAI_COLUMN 7
#define EAST_MOAI_COLUMN 15
#define MOAI_FRONT_ROW 35
#define PILLAR_HOLE_COLUMN 32
#define ROCK_STREAM_COLUMN 53

/// The trigger StepIntoCurrent raises for a leader the current carries over
/// the edge of the falls (AquaRock_DropDownFalls).
#define TRIGGER_DROP_DOWN_FALLS 44

/// The slopes' map animations of the streams their Water Stone turns, two
/// each: running downhill, and turned back to run uphill.
enum { STREAMS_DOWNHILL_ANIMATION = 5, STREAMS_UPHILL_ANIMATION = 7 };

/// The cell behaviours the rooms' trigger tables answer: the streams the top's
/// west and east red moai pour, the heads of the two lower red moai's streams,
/// the streams that only ever run downhill, the streams the slopes' Water
/// Stone turns back, and the mouth of the sparkling passage.
enum {
  CELL_BEHAVIOR_WEST_MOAI_STREAM = 30,
  CELL_BEHAVIOR_EAST_MOAI_STREAM = 31,
  CELL_BEHAVIOR_LOWER_WEST_MOAI_STREAM = 32,
  CELL_BEHAVIOR_LOWER_EAST_MOAI_STREAM = 33,
  CELL_BEHAVIOR_DOWNHILL_STREAM = 40,
  CELL_BEHAVIOR_TURNING_STREAM = 41,
  CELL_BEHAVIOR_SPARKLING_PASSAGE = 4
};

/// The slopes (gAquaRock_SlopesPlacements).
enum AquaRockSlopesActorId {
  /// The slopes' sixteen moai: four red ones, the first two at the top, then
  /// the dry one, two green ones and nine green ones lower down that share
  /// one sprite.
  ACTOR_FIRST_MOAI = ACTOR_FIRST_PLACED,
  /// The grey moai Douse fills, so that it pours a stream
  /// (AquaRock_DouseDryMoai).
  ACTOR_DRY_MOAI = ACTOR_FIRST_MOAI + 4,
  ACTOR_FIRST_BOTTOM_MOAI = ACTOR_FIRST_MOAI + 7,
  /// The five guardian moai that stand in the way in, until they step aside
  /// together (AquaRock_MoaiStepAside).
  ACTOR_FIRST_GUARDIAN_MOAI = ACTOR_FIRST_MOAI + 9,
  ACTOR_LAST_MOAI = ACTOR_FIRST_MOAI + 15,
  /// The pillar Move sets before the top's red moai, and the pillar on the
  /// ledge Move pushes into its hole.
  ACTOR_MOAI_PILLAR,
  ACTOR_LEDGE_PILLAR,
  /// The Water Stone whose Douse turns the streams back uphill.
  ACTOR_WATER_STONE,
  /// The three fish of the pools (gAquaRock_FishScript and
  /// gAquaRock_SlowFishScript).
  ACTOR_FIRST_FISH
};

/// The Mist Sabre chest's room (gAquaRock_MistSabreChestPlacements).
enum AquaRockMistSabreChestActorId {
  /// The Water Stone whose Douse opens the way inside.
  ACTOR_MIST_SABRE_WATER_STONE = ACTOR_FIRST_PLACED,
  /// The three puddles Frost freezes into steps.
  ///@{
  ACTOR_PUDDLE_A,
  ACTOR_PUDDLE_B,
  ACTOR_PUDDLE_C,
  ///@}
  /// The two rocks Move pushes into the stream.
  ///@{
  ACTOR_FIRST_ROCK,
  ACTOR_SECOND_ROCK,
  ///@}
  /// An unseen marker the bubbles and sparkles rise from once the streams are
  /// turned back.
  ACTOR_BUBBLE_MARKER,
  /// An unseen marker in the pool where, once its Water Stone is Doused,
  /// rings of water rise and the leader sinks inside (AquaRock_SinkInside).
  ACTOR_WAY_INSIDE
};
extern struct Trigger gAquaRock_Triggers[];
extern struct Trigger gAquaRock_SlopesTriggers[];
extern struct Trigger gAquaRock_MistSabreChestTriggers[];

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y,
                                                    s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

/// Takes `size` bytes from the heap.
static __inline__ void *MemAlloc_(s32 size) { return Heap_Alloc(size); }
#include "flags.h"
#include "field/event.h"
#include "field/field_sequence_event.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/event.h"
#include "field/ground_mask.h"
#include "scene/scene.h"
#include "scene/ground_effects.h"
#include "system/task.h"
#include "field/actor.h"
#include "system/iwram_call.h"
#include "system/vector3.h"
#include "field/event_inline.h"
#include "system/main_routine_state.h"
#include "system/ram_buffer.h"
#include "system/frame.h"
#include "field/map_grid.h"
#include "system/input.h"
#include "field/call.h"
#include "scene_ids.h"
#include "graphics/vram_tab.h"
#include "system/heap_release.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "graphics/resource.h"
#include "macros.h"
#include "shared/tables.h"

/// Unused: creates an unseen, still actor of `kind` at a point, untargeted and
/// drawn underfoot.
UNUSED struct Actor *AquaRock_CreateActor(s32 x, s32 y, s32 z, s32 kind) {
  struct Actor *actor = Import_ActorCreate(kind, x, y, z);

  if (actor != 0) {
    Scene_SetActorSpritePriority(actor, 1);
    actor->motionFlags = 0;
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
    actor->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT | ACTOR_PRIORITY_IGNORES_RISES;
    return actor;
  }
  return 0;
}

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

struct Actor;

void GroundParticles_SetAnchor(s32 id);
void AquaRock_SetBubbleAnchor(s32 id);
void Leader_FlowWithStream(void);
void Actor_SpawnWadingSplash(struct Actor *actor);
void AquaRock_AddMoaiWater(s32 id);
void Actor_EmitWaterDrop(struct Actor *actor);

void AquaRock_KeepLeaderPriority(void);
void AquaRock_StartBubbles(void);
void GroundParticles_Start(void);
void AquaRock_MoaiStepAside(void);
void AquaRock_SetUpStreams(void);
void AquaRock_ArriveFromPassage(void);
void AquaRock_ArriveFromInside(void);
void AquaRock_ArriveUpStream(void);

extern s16 gGroundParticleFrames[];

s32 AquaRock_StartFish(struct Actor *actor);
void AquaRock_RideStreamUp(void);
void AquaRock_StepIntoCurrent(void);

void AquaRock_DouseDryMoai(void);

void AquaRock_DropDownFalls(void);

/// One of the streams the moai pour, which stops and starts with its flag: each
/// change runs down it as a wave, copying three cells a row from the flowing
/// or stopped pictures beside it, a row every four frames.
struct AquaRockStream {
  s16 pictureSourceX, pictureSourceY; ///< its pictures, flowing then stopped
  s16 cellSourceX, cellSourceY;       ///< its cells, flowing then stopped
  s16 pictureX, pictureY;             ///< where its pictures are copied
  s16 cellX, cellY;                   ///< where its cells are copied
  s16 height;                         ///< its rows
  /// The flag that stops it, or IF_FLAG_SET of one that sets it flowing.
  s16 flag;
  s16 shown;        ///< the flag's value it was last told
  s16 waveRow[2];   ///< each wave's row times four, or STREAM_NO_WAVE
  s16 waveStops[2]; ///< whether each wave stops the stream or starts it
  s16 nextWave;
};

/// AquaRockStream.waveRow for no wave.
#define STREAM_NO_WAVE 9999

/// The five moai streams AquaRock_UpdateStreams stops and starts: the four red
/// moai's and the dry moai's.
struct AquaRockStream gAquaRock_Streams[5] BSS_DATA ALIGNED(16);
struct AquaRockBubbles gAquaRock_Bubbles BSS_DATA;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_AQUA_ROCK_MOAI)
    return gAquaRock_SlopesRegions;
  if (scene == SCENE_AQUA_ROCK_MIST_SABRE_CHEST)
    return gAquaRock_MistSabreChestRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// A fish's script call: picks a random spot ahead within 16 pixels of its
/// home and swims there; after three misses it turns around.
s32 AquaRock_PickRandomSpot(struct Actor *actor) {
  struct Vector3 spot;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  struct Vector3 unused;
  s32 x;
  FIXED coordinate;
  s32 tries = 0;
  s32 maxDistanceSquared = 16 * 16;
  FIXED distance;
  s32 dx, dz;
  u32 a, b;

again:
  tries++;
  if (tries <= 3) {
    spot.x = actor->x.fixed;
    spot.y = actor->y.fixed;
    spot.z = actor->z.fixed;
    distance = Iwram_MulFX(Random16(), toFIXED(8)) + toFIXED(4);
    a = Random16();
    b = Random16();
    Vector_AddPolarOffset(distance, actor->facing + (a >> 2) - (b >> 2), &spot);
    // The spot's distance from home in whole pixels, rounded toward zero.
    x = spot.x;
    if (x < 0)
      x += FIXED_ONE - 1;
    dx = FIXEDtoINT(x) - actor->timer;
    coordinate = spot.z;
    if (coordinate < 0)
      coordinate += FIXED_ONE - 1;
    dz = FIXEDtoINT(coordinate) - actor->userValue;
    if (dx * dx + dz * dz > maxDistanceSquared)
      goto again;
    goto found;
  }
  actor->facing += DEGtoANG(180);
  actor->wait = 1;
  goto end;
found:
  Actor_SetMoveTarget(actor, spot.x, spot.y, spot.z);
end:
  /* FAKEMATCH(dead): x set to 0, a store nothing reads. */
  x = 0;
  /* FAKEMATCH(dead): coordinate set to 0, a store nothing reads. */
  coordinate = 0;
  return 0;
}

/// A fish's update: its sprite turns with its facing.
void AquaRock_TurnFish(struct Actor *actor) {
  struct FieldSprite *sprite;
  s32 angle;

  angle = actor->facing;
  sprite = actor->sprite;
  sprite->rotation = angle + DEGtoANG(90);
}

/// A fish's script call: takes away its shadow and gives it its speed and its
/// turning update.
s32 AquaRock_StartFish(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  actor->speedLimit = toFIXED(1);
  actor->acceleration = toFIXED(1) / 20;
  actor->collisionFlags = 0;
  actor->update = (ObjectUpdate)AquaRock_TurnFish;
  return 0;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_AQUA_ROCK_MOAI)
    return gAquaRock_SlopesPlacements;
  if (scene == SCENE_AQUA_ROCK_MIST_SABRE_CHEST)
    return gAquaRock_MistSabreChestPlacements;
  return gAquaRock_Placements;
}

/// Unused: a sparkle's update. It rises to the water's surface and then flows
/// north up the stream, until it reaches the top or the stream's end.
void AquaRock_FlowSparkleUpStream(struct Actor *actor) {
  if (Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed) > actor->y.fixed)
    actor->y.fixed += toFIXED(2);
  else
    actor->z.fixed -= toFIXED(2);
  if (FIELD_CELL(actor->y.fixed) == 23)
    Actor_Release(actor);
  if (FIELD_CELL(actor->z.fixed) == 48)
    Actor_Release(actor);
}

/// Unused: now and then a sparkle starts up the stream from somewhere along the
/// actor's 48-pixel width.
UNUSED void AquaRock_SpawnSparkleAtRandom(struct Actor *actor) {
  if ((u32)Random16() < 4096) {
    struct Actor *child;
    s32 spread = Random16();
    FIXED x = actor->x.fixed;

    x += spread * 48;
    child = Import_ActorCreate(SPRITE_GLINT, x - toFIXED(24), actor->y.fixed,
                               actor->z.fixed);
    if (child != 0) {
      Actor_SetSpriteFlags(child, 0);
      child->update = (ObjectUpdate)AquaRock_FlowSparkleUpStream;
      child->motionFlags = 0;
    }
  }
}

/// A moai's stream stops or starts as a wave running down it from the moai's
/// mouth, a row every four frames.
/// It runs every frame, sending a wave down each stream whose flag has changed.
void AquaRock_UpdateStreams(void) {
  struct AquaRockStream *entry;
  s32 n, i, flag;

  entry = gAquaRock_Streams;
  for (n = 0; n <= 4; n++, entry++) {
    flag = GameFlag_Get(entry->flag & (CONDITION_FLAG_SET - 1));
    if (entry->flag & CONDITION_FLAG_SET)
      flag ^= 1;
    if (entry->shown != flag) {
      entry->waveRow[entry->nextWave] = 0;
      entry->waveStops[entry->nextWave] = flag;
      if (++entry->nextWave > 1)
        entry->nextWave = 0;
      entry->shown = flag;
    }
    for (i = 0; i < 2; i++) {
      s32 row = entry->waveRow[i];
      if (row < entry->height * 4) {
        s32 inverse;
        row /= 4;
        flag = entry->waveStops[i];
        inverse = flag ^ 1;
        if (entry->waveRow[i] & 1) {
          MapGrid_CopyCellsLayered(flag * 3 + entry->pictureSourceX,
                                   entry->pictureSourceY + row, 3, 1,
                                   entry->pictureX, entry->pictureY + row);
          MapGrid_CopyCells(flag * 3 + entry->cellSourceX,
                            entry->cellSourceY + row, 3, 1, entry->cellX,
                            entry->cellY + row);
        } else {
          MapGrid_CopyCellsLayered(inverse * 3 + entry->pictureSourceX,
                                   entry->pictureSourceY + row, 3, 1,
                                   entry->pictureX, entry->pictureY + row);
          MapGrid_CopyCells(inverse * 3 + entry->cellSourceX,
                            entry->cellSourceY + row, 3, 1, entry->cellX,
                            entry->cellY + row);
        }
        entry->waveRow[i]++;
      }
    }
  }
}

extern s32 gAquaRock_FirstStreamTimer;
extern s32 gAquaRock_SecondStreamTimer;
extern s32 gAquaRock_ThirdStreamTimer;
extern s32 gAquaRock_FourthStreamTimer;

/// The frames of a red moai's stream's cycle, and the last of them it stops
/// for.
#define STREAM_CYCLE_FRAMES 240
#define STREAM_STOP_FRAMES 90

/// The four red moai's streams flow for two and a half seconds and stop for
/// one and a half, over and over, and the top two stay stopped while the
/// pillar stands before them.
/// It runs every frame while no event runs, setting each stream's stopped
/// flag, which AquaRock_UpdateStreams turns into the wave down the stream.
void AquaRock_TimeStreams(void) {
  if (GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS))
    return;
  if (gAquaRock_FirstStreamTimer < STREAM_CYCLE_FRAMES - STREAM_STOP_FRAMES) {
    if (GameFlag_Get(FLAG_PILLAR_BEFORE_WEST_MOAI))
      GameFlag_Set(FLAG_WEST_MOAI_STREAM_STOPPED);
    else
      GameFlag_Clear(FLAG_WEST_MOAI_STREAM_STOPPED);
  } else {
    GameFlag_Set(FLAG_WEST_MOAI_STREAM_STOPPED);
  }
  if (++gAquaRock_FirstStreamTimer >= STREAM_CYCLE_FRAMES)
    gAquaRock_FirstStreamTimer = 0;
  if (gAquaRock_SecondStreamTimer < STREAM_CYCLE_FRAMES - STREAM_STOP_FRAMES) {
    if (GameFlag_Get(FLAG_PILLAR_BEFORE_EAST_MOAI))
      GameFlag_Set(FLAG_EAST_MOAI_STREAM_STOPPED);
    else
      GameFlag_Clear(FLAG_EAST_MOAI_STREAM_STOPPED);
  } else {
    GameFlag_Set(FLAG_EAST_MOAI_STREAM_STOPPED);
  }
  if (++gAquaRock_SecondStreamTimer >= STREAM_CYCLE_FRAMES)
    gAquaRock_SecondStreamTimer = 0;
  if (gAquaRock_ThirdStreamTimer < STREAM_CYCLE_FRAMES - STREAM_STOP_FRAMES)
    GameFlag_Clear(FLAG_LOWER_WEST_MOAI_STREAM_STOPPED);
  else
    GameFlag_Set(FLAG_LOWER_WEST_MOAI_STREAM_STOPPED);
  if (++gAquaRock_ThirdStreamTimer >= STREAM_CYCLE_FRAMES)
    gAquaRock_ThirdStreamTimer = 0;
  if (gAquaRock_FourthStreamTimer < STREAM_CYCLE_FRAMES - STREAM_STOP_FRAMES)
    GameFlag_Clear(FLAG_LOWER_EAST_MOAI_STREAM_STOPPED);
  else
    GameFlag_Set(FLAG_LOWER_EAST_MOAI_STREAM_STOPPED);
  if (++gAquaRock_FourthStreamTimer >= STREAM_CYCLE_FRAMES)
    gAquaRock_FourthStreamTimer = 0;
}

/// Sets up a stream (struct AquaRockStream) to show its flag's opposite, so
/// that its first update runs a wave to the flag's state.
#define STREAM_SET(stream, pictureSourceX_, pictureSourceY_, cellSourceX_,     \
                   cellSourceY_, pictureX_, pictureY_, cellX_, cellY_,         \
                   height_, flag_)                                             \
  (stream)->pictureSourceX = (pictureSourceX_);                                \
  (stream)->pictureSourceY = (pictureSourceY_);                                \
  (stream)->cellSourceX = (cellSourceX_);                                      \
  (stream)->cellSourceY = (cellSourceY_);                                      \
  (stream)->pictureX = (pictureX_);                                            \
  (stream)->pictureY = (pictureY_);                                            \
  (stream)->cellX = (cellX_);                                                  \
  (stream)->cellY = (cellY_);                                                  \
  (stream)->height = (height_);                                                \
  (stream)->flag = (flag_);                                                    \
  (stream)->shown = GameFlag_Get(flag_) ^ 1;                                   \
  (stream)->nextWave = 0;                                                      \
  (stream)->waveRow[1] = STREAM_NO_WAVE;                                       \
  (stream)->waveRow[0] = STREAM_NO_WAVE

/// Sets the five streams with their pictures, cells and flags, stops at once
/// the top two the pillar stands before, and starts the streams' tasks.
void AquaRock_SetUpStreams(void) {
  struct AquaRockStream *entry = gAquaRock_Streams;

  STREAM_SET(entry, 64, 74, 76, 74, 70, 10, 6, 74, 36,
             FLAG_WEST_MOAI_STREAM_STOPPED);
  entry++;
  STREAM_SET(entry, 70, 74, 82, 74, 78, 10, 14, 74, 36,
             FLAG_EAST_MOAI_STREAM_STOPPED);
  entry++;
  STREAM_SET(entry, 90, 93, 102, 93, 103, 26, 39, 90, 15,
             FLAG_LOWER_WEST_MOAI_STREAM_STOPPED);
  entry++;
  STREAM_SET(entry, 96, 93, 108, 93, 107, 26, 43, 90, 15,
             FLAG_LOWER_EAST_MOAI_STREAM_STOPPED);
  entry++;
  STREAM_SET(entry, 116, 95, 122, 95, 96, 32, 32, 96, 13,
             IF_FLAG_SET(FLAG_AQUA_ROCK_DRY_MOAI_FILLED));
  if (GameFlag_Get(FLAG_PILLAR_BEFORE_WEST_MOAI))
    GameFlag_Set(FLAG_WEST_MOAI_STREAM_STOPPED);
  else
    GameFlag_Clear(FLAG_WEST_MOAI_STREAM_STOPPED);
  if (GameFlag_Get(FLAG_PILLAR_BEFORE_EAST_MOAI))
    GameFlag_Set(FLAG_EAST_MOAI_STREAM_STOPPED);
  else
    GameFlag_Clear(FLAG_EAST_MOAI_STREAM_STOPPED);
  if (GameFlag_Get(FLAG_WEST_MOAI_STREAM_STOPPED)) {
    struct AquaRockStream *stream = &gAquaRock_Streams[0];
    MapGrid_CopyCellsLayered(stream->pictureSourceX + 3, stream->pictureSourceY,
                             3, stream->height, stream->pictureX,
                             stream->pictureY);
    MapGrid_CopyCells(stream->cellSourceX + 3, stream->cellSourceY, 3,
                      stream->height, stream->cellX, stream->cellY);
    stream->waveRow[1] = STREAM_NO_WAVE;
    stream->waveRow[0] = STREAM_NO_WAVE;
    stream->shown = 1;
  }
  if (GameFlag_Get(FLAG_EAST_MOAI_STREAM_STOPPED)) {
    struct AquaRockStream *stream = &gAquaRock_Streams[1];
    MapGrid_CopyCellsLayered(stream->pictureSourceX + 3, stream->pictureSourceY,
                             3, stream->height, stream->pictureX,
                             stream->pictureY);
    MapGrid_CopyCells(stream->cellSourceX + 3, stream->cellSourceY, 3,
                      stream->height, stream->cellX, stream->cellY);
    stream->waveRow[1] = STREAM_NO_WAVE;
    stream->waveRow[0] = STREAM_NO_WAVE;
    stream->shown = 1;
  }
  Task_AddFrame(AquaRock_TimeStreams, TASK_PASS_FRAME);
  Task_Add(AquaRock_UpdateStreams, TASK_PASS_FRAME);
}

#include "shared/actor_flicker_child.h"

/// Unused: a moai's update while it flashes, grey every other two frames.
UNUSED void AquaRock_FlashMoai(struct Actor *actor) {
  if (gFrameCount & 2)
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_GREY);
  else
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
}

/// A flashing actor's last update: its sprite's colour back to normal.
void AquaRock_StopFlashing(struct Actor *actor) {
  Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
  actor->update = 0;
}

void TilesetAnim_Pause(s32 channel);

/// Douse on the slopes' Water Stone: it flickers, the screen flashes four
/// colours and turns white, and the streams turn back to run uphill, bubbles
/// rising from the stone; Doused again, they run downhill once more.
void AquaRock_DouseWaterStone(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  actor->update = (ObjectUpdate)Actor_FlickerChild;
  wait(30);
  if (GameFlag_Get(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK) == 0)
    GroundParticles_SetAnchor(ACTOR_WATER_STONE);
  else
    GroundParticles_SetAnchor(0);
  for (i = 0; i < 4; i++) {
    PaletteFade_SetTarget(Random16() & 0x7fff, 0);
    PaletteFade_StartInterpolation(15);
    wait(10);
    PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_StartInterpolation(10);
    wait(10);
  }
  PaletteFade_SetTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_StartInterpolation(20);
  Event_Wait(20);
  if (Event_GetFlag(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK) == 0) {
    GameFlag_Set(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK);
    MapGrid_CopyCells(64, 64, 1, 2, 51, 82);
    MapGrid_CopyCells(66, 64, 1, 2, 115, 18);
    TilesetAnim_Pause(STREAMS_DOWNHILL_ANIMATION);
    TilesetAnim_Pause(STREAMS_DOWNHILL_ANIMATION + 1);
    TilesetAnim_Resume(STREAMS_UPHILL_ANIMATION);
    TilesetAnim_Resume(STREAMS_UPHILL_ANIMATION + 1);
    actor->update = (ObjectUpdate)AquaRock_StopFlashing;
    AquaRock_SetBubbleAnchor(ACTOR_WATER_STONE);
  } else {
    GameFlag_Clear(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK);
    MapGrid_CopyCells(65, 64, 1, 2, 51, 82);
    MapGrid_CopyCells(67, 64, 1, 2, 115, 18);
    TilesetAnim_Pause(STREAMS_UPHILL_ANIMATION);
    TilesetAnim_Pause(STREAMS_UPHILL_ANIMATION + 1);
    TilesetAnim_Resume(STREAMS_DOWNHILL_ANIMATION);
    TilesetAnim_Resume(STREAMS_DOWNHILL_ANIMATION + 1);
    actor->update = (ObjectUpdate)AquaRock_StopFlashing;
    AquaRock_SetBubbleAnchor(0);
  }
  Event_Wait(30);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(30);
  Event_Wait(15);
  Event_End();
}

void GameFlag_SetByte(s32 flag, s32 value);

/// Move on a pillar. The pillar by the top's red moai keeps its cell in the
/// area's flags, and stopped before one of them stops that moai's stream; the
/// ledge's pillar, pushed onto its hole, drops into it.
void AquaRock_MovePillar(s32 unused, s32 id) {

  if (id == ACTOR_MOAI_PILLAR) {
    s32 column;
    s32 row;

    struct Actor *pillar = Actor_GetById(ACTOR_MOAI_PILLAR);

    column = FIELD_CELL(pillar->x.fixed);
    row = FIELD_CELL(pillar->z.fixed);
    GameFlag_SetByte(FLAG_PILLAR_COLUMN, column);
    GameFlag_SetByte(FLAG_PILLAR_ROW, row);
    Event_ClearFlag(FLAG_PILLAR_BEFORE_WEST_MOAI);
    GameFlag_Clear(FLAG_PILLAR_BEFORE_EAST_MOAI);
    if (column == WEST_MOAI_COLUMN && row == MOAI_FRONT_ROW)
      GameFlag_Set(FLAG_PILLAR_BEFORE_WEST_MOAI);
    if (column == EAST_MOAI_COLUMN && row == MOAI_FRONT_ROW)
      GameFlag_Set(FLAG_PILLAR_BEFORE_EAST_MOAI);
  } else if (id == ACTOR_LEDGE_PILLAR) {
    struct Actor *actor = Actor_GetById(ACTOR_LEDGE_PILLAR);

    if (FIELD_CELL(actor->x.fixed) == PILLAR_HOLE_COLUMN) {
      wait(30);
      actor->z.fixed += toFIXED(1);
      GameFlag_Set(FLAG_AQUA_ROCK_PILLAR_IN_HOLE);
    }
  }
}

/// Unused: waits until the actor has come down to `height`, a second at most.
UNUSED void Actor_WaitBelowHeight(struct Actor *actor, FIXED height) {
  s32 frames = 60;

  while (frames != 0) {
    wait(1);
    frames--;
    if (actor->y.fixed <= height)
      break;
  }
}

/// Felix, walking into one of the streams the slopes' Water Stone turns back,
/// is carried up it, splashing, once the stone has been Doused; at the head of
/// the slopes' stream he spins up out of the water and away into the Mist
/// Sabre chest's room. Until then, or once Doused again, the stream carries
/// him down instead (Leader_FlowWithStream).
/// It runs every frame he stands on the stream's cells, moving him and the
/// camera two pixels north, and leaping him up each ledge the stream climbs.
void AquaRock_RideStreamUp(void) {
  struct Actor *leader, *carried;
  struct Vector3 ahead;
  s32 owner;
  struct FieldState *state;
  s32 result, i;

  if (GameFlag_Get(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK)) {
    state = Ram_HeapSlots->fieldState;
    owner = gMainRoutineState.leaderActorId;
    carried = state->camera;
    leader = Actor_GetById(owner);
    if (gMainRoutineState.next == SCENE_AQUA_ROCK_MOAI &&
        leader->groundHeight == 0)
      return;
    ahead.x = leader->x.fixed;
    ahead.y = leader->y.fixed;
    ahead.z = leader->z.fixed + toFIXED(2);
    result = MapGrid_CheckCollision(leader, &ahead);
    if ((gFrameCount & 4) == 0)
      Actor_SpawnWadingSplash(leader);
    if (result < 0) {
      Actor_SetAttachedEffect(owner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetMoveTarget(leader, leader->x.fixed, leader->y.fixed,
                          leader->z.fixed - toFIXED(8));
      Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
      Actor_WaitForMove(leader);
      do
        wait(1);
      while (leader->y.fixed != leader->groundHeight);
      Actor_SpawnWadingSplash(leader);
      Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
      wait(3);
      return;
    }
    ahead.x = leader->x.fixed;
    ahead.y = leader->y.fixed;
    ahead.z = leader->z.fixed - toFIXED(8);
    result = MapGrid_CheckCollision(leader, &ahead);
    if (result > 0) {
      if (gMainRoutineState.next != SCENE_AQUA_ROCK_MOAI)
        return;
      Event_Start();
      Event_ResolvePendingActions(0);
      leader->motionFlags = 0;
      Actor_SetSpriteFlags(leader, 0);
      gMainRoutineState.movementMode = LEADER_MOVEMENT_KEYS;
      Actor_SpawnWadingSplash(leader);
      Event_Wait(5);
      for (i = 0; i < 60; i++) {
        leader->y.fixed += toFIXED(2);
        leader->facing += TRIG_TURN / 16;
        if (Random16() <= 4095)
          Actor_SpawnWadingSplash(leader);
        wait(1);
      }
      gMainRoutineState.movementMode = LEADER_MOVEMENT_WALK;
      Event_End();
      Import_EventSetExitRequest(
          AQUA_ROCK_MOAI_TO_AQUA_ROCK_MIST_SABRE_CHEST_C);
      return;
    }
    ahead.x = leader->x.fixed + (toFIXED(57) / 10);
    ahead.y = leader->y.fixed;
    ahead.z = leader->z.fixed - (toFIXED(57) / 10);
    result = MapGrid_CheckCollision(leader, &ahead);
    if (result > 0)
      return;
    ahead.x = leader->x.fixed - (toFIXED(57) / 10);
    ahead.y = leader->y.fixed;
    ahead.z = leader->z.fixed - (toFIXED(57) / 10);
    result = MapGrid_CheckCollision(leader, &ahead);
    if (result > 0)
      return;
    carried->z.fixed -= toFIXED(2);
    leader->z.fixed -= toFIXED(2);
  } else
    Leader_FlowWithStream();
}

/// The attribute byte of a cell of a layer.
static inline u8 CellAttribute(struct MapCell *cells, s32 row, s32 col) {
  cells += row * MAPGRID_SIZE + col;
  return cells->attributeB;
}

/// Felix, stepping into the stream one of the red moai pours, is swept away
/// down the slopes, tumbling over the ledges, and where it carries him over
/// the edge of the falls he drops down them, flailing, to the pool below
/// (AquaRock_DropDownFalls).
/// It runs every frame he stands on the stream's cells while it flows; the
/// top two moai's streams take him only while the pillar does not stop them.
void AquaRock_StepIntoCurrent(void) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *actor = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 col;
  s32 row;

  if (gMainRoutineState.movementMode == LEADER_MOVEMENT_KEYS) {
    col = FIELD_CELL(actor->x.fixed);
    row = FIELD_CELL(actor->z.fixed - actor->y.fixed - toFIXED(12));
    if (CellAttribute(map->layers[1].cells, row, col) != 0)
      state->raisedTrigger = TRIGGER_DROP_DOWN_FALLS;
  } else {
    col = FIELD_CELL(actor->x.fixed);
    row = FIELD_CELL(actor->z.fixed - actor->y.fixed);
    if (CellAttribute(map->layers[1].cells, row, col) != 0)
      Leader_FlowWithStream();
  }
}

/// Douse on the dry moai: it fills, turns blue and pours, and its stream down
/// the slopes starts to flow.
void AquaRock_DouseDryMoai(void) {
  MapGrid_CopyCellAttributes(62, 41, 1, 2, 33, 41);
  MapGrid_CopyCellTilesCellsRect(125, 32, 3, 13, 96, 32);
  MapGrid_CopyCellTilesCellsRect(61, 96, 3, 13, 32, 96);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_DRY_MOAI), SPRITE_COLOR_BRG);
  AquaRock_AddMoaiWater(ACTOR_DRY_MOAI);
  GameFlag_Set(FLAG_AQUA_ROCK_DRY_MOAI_FILLED);
}

/// The leader drops down the falls, flailing, and lands, for at most three
/// seconds.
void AquaRock_DropDownFalls(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 saved = actor->gravity;
  s32 i;
  struct MainRoutineState *state;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldEffect_SetAttached(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SelectAnimation(actor, ANIM_LEADER_FLAIL);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  for (i = 0; i < 180; i++) {
    wait(1);
    if (actor->y.fixed <= actor->groundHeight)
      break;
  }
  state = &gMainRoutineState;
  actor->gravity = saved;
  FieldEffect_SetAttached(state->leaderActorId, ATTACHED_EFFECT_REMOVE);
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  Actor_SelectAnimation(actor, ANIM_STAND);
  state->movementMode = LEADER_MOVEMENT_WALK;
  Event_End();
}

/// Defines Frost on a puddle: it freezes into a step, its cell taking the
/// attributes of the frozen cell at (sourceX, row), and its flag is set.
#define DEFINE_AQUA_ROCK_FREEZE_PUDDLE(name, sourceX, row, column, flag)       \
  void name(s32 unused, s32 id) {                                              \
    struct Actor *actor = Actor_GetById(id);                                   \
                                                                               \
    actor->priorityFlags = 0;                                                  \
    actor->motionFlags = 0;                                                    \
    MapGrid_CopyCellAttributes(sourceX, row, 1, 1, column, row);               \
    GameFlag_Set(flag);                                                        \
  }

/// Frost on the Mist Sabre chest's room's first puddle: it freezes into a step.
DEFINE_AQUA_ROCK_FREEZE_PUDDLE(AquaRock_FreezeFirstPuddle, 42, 32, 44,
                               FLAG_PUDDLE_A_FROZEN)

/// Frost on the second puddle: it freezes into a step.
DEFINE_AQUA_ROCK_FREEZE_PUDDLE(AquaRock_FreezeSecondPuddle, 48, 32, 46,
                               FLAG_PUDDLE_B_FROZEN)

/// Frost on the third puddle: it freezes into a step.
DEFINE_AQUA_ROCK_FREEZE_PUDDLE(AquaRock_FreezeThirdPuddle, 44, 44, 46,
                               FLAG_PUDDLE_C_FROZEN)

/// Defines Move on a rock: pushed to the stream's column it drops in, its flag
/// is set and its cell in `row` takes the attributes of the stream's
/// rock-filled cell.
#define DEFINE_AQUA_ROCK_DROP_AT_COLUMN(name, flag, row)                       \
  void name(s32 unused, s32 id) {                                              \
    struct Actor *actor = Actor_GetById(id);                                   \
    s32 column = FIELD_CELL(actor->x.fixed);                                   \
                                                                               \
    if (column == ROCK_STREAM_COLUMN) {                                        \
      wait(30);                                                                \
      actor->z.fixed += toFIXED(1);                                            \
      GameFlag_Set(flag);                                                      \
      Map_CopyCellAttributeRect(54, 58, 1, 1, column, row);                    \
    }                                                                          \
  }

/// Move on the first rock to the stream: it drops in.
DEFINE_AQUA_ROCK_DROP_AT_COLUMN(AquaRock_DropFirstRock,
                                FLAG_AQUA_ROCK_FIRST_ROCK_DROPPED, 52)

/// Move on the second rock to the stream: it drops in.
DEFINE_AQUA_ROCK_DROP_AT_COLUMN(AquaRock_DropSecondRock,
                                FLAG_AQUA_ROCK_SECOND_ROCK_DROPPED, 58)

/// Douse on the Mist Sabre chest's room's Water Stone: the screen flashes four
/// colours and turns white, the stone's bubbles start, the pool's cells change
/// and the camera pans down to the pool, where rings of water start to rise
/// over the way inside.
void AquaRock_DouseMistSabreChestWaterStone(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  actor->update = (ObjectUpdate)Actor_FlickerChild;
  wait(30);
  GroundParticles_SetAnchor(ACTOR_MIST_SABRE_WATER_STONE);
  Sound_Play(SE_SHIMMER);
  for (i = 0; i < 4; i++) {
    PaletteFade_SetTarget(Random16() & 0x7fff, 0);
    PaletteFade_StartInterpolation(15);
    wait(10);
    PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_StartInterpolation(10);
    wait(10);
  }
  PaletteFade_SetTarget(RGB_WHITE, PALETTES_ALL);
  PaletteFade_StartInterpolation(20);
  Event_Wait(20);
  GameFlag_Set(FLAG_AQUA_ROCK_MIST_SABRE_CHEST_WATER_STONE_DOUSED);
  Map_CopyCellAttributeRect(15, 48, 1, 1, 16, 48);
  Map_CopyCellAttributeRect(17, 50, 1, 1, 16, 50);
  actor->update = (ObjectUpdate)AquaRock_StopFlashing;
  AquaRock_SetBubbleAnchor(ACTOR_MIST_SABRE_WATER_STONE);
  MapGrid_CopyCellTilesCellsRect(64, 68, 5, 13, 14, 94);
  Map_CopyCellAttributeRect(64, 68, 5, 8, 14, 34);
  Event_Wait(30);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(30);
  Event_Wait(15);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(248, -32, 760, 1);
  FieldCamera_WaitForArrival();
  Actor_SetPosition(ACTOR_WAY_INSIDE, 264, 792);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_WAY_INSIDE), 0);
  Actor_GetById(ACTOR_WAY_INSIDE)->collisionFlags = 0;
  Actor_GetById(ACTOR_WAY_INSIDE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_GetById(ACTOR_WAY_INSIDE)->update = (ObjectUpdate)Actor_EmitWaterDrop;
  Event_Wait(30);
  for (i = 0; i < 6; i++) {
    Sound_Play(SE_WADE);
    wait(10);
  }
  Event_Wait(30);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_AQUA_ROCK_MOAI)
    return gAquaRock_SlopesTriggers;
  if (scene == SCENE_AQUA_ROCK_MIST_SABRE_CHEST)
    return gAquaRock_MistSabreChestTriggers;
  return gAquaRock_Triggers;
}

/// On the slopes Felix is drawn behind the falls while a current carries him,
/// and in front of them otherwise.
/// It runs every frame, giving his sprite priority 2 while he is carried and 1
/// otherwise.
void AquaRock_KeepLeaderPriority(void) {
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());

  if (actor != 0 &&
      (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_ONE) {
    struct FieldSprite *sprite;
    s32 priority;

    sprite = actor->sprite;
    priority = gMainRoutineState.movementMode == LEADER_MOVEMENT_KEYS ? 2 : 1;
    sprite->priority = priority;
    sprite->partPriority = priority;
    actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  }
}

/// Arriving from inside, the Vial chest's room below: the leader spins up out
/// of the pool and lands, going on north or south by the key held.
void AquaRock_ArriveFromInside(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 saved = actor->motionFlags;
  s32 i;
  s32 angle;
  s32 up;
  struct MainRoutineState *state;

  Sound_Play(SE_SPIN_AWAY);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  actor->y.fixed = -toFIXED(16);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 2);
  for (i = 0; i < 16; i++) {
    actor->y.fixed += toFIXED(1);
    angle = actor->facing;
    angle += DEGtoANG(45);
    actor->facing = angle;
    wait(1);
  }
  Actor_SpawnWadingSplash(actor);
  actor->motionFlags = saved;
  actor->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  up = gInput.held & KEY_UP;
  state = &gMainRoutineState;
  if (up)
    Actor_MoveToAndWaitById(state->leaderActorId, 264, 760);
  else
    Actor_MoveToAndWaitById(state->leaderActorId, 264, 824);
  Event_End();
}

/// Arriving by the slopes' stream turned back: it throws the leader spinning
/// up out of the water, and he walks out onto the bank.
void AquaRock_ArriveUpStream(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 saved = actor->motionFlags;
  s32 i;
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  actor->y.fixed = -toFIXED(64);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 2);
  for (i = 0; i < 24; i++) {
    actor->y.fixed += toFIXED(2);
    angle = actor->facing;
    angle += DEGtoANG(45);
    actor->facing = angle;
    wait(1);
  }
  Actor_SpawnWadingSplash(actor);
  actor->motionFlags = saved;
  actor->velocityY = toFIXED(12);
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  Actor_MoveToAndWaitById(gMainRoutineState.leaderActorId, 680, 912);
  Event_End();
}

/// The farthest a guardian moai rocks either way as it shakes.
#define MOAI_SHAKE (TRIG_TURN / 128)

/// A guardian moai's update as it moves aside: it shakes, the swing growing
/// over 16 frames to MOAI_SHAKE.
void AquaRock_ShakeMoai(struct Actor *actor) {
  struct FieldSprite *sprite = actor->sprite;
  s16 *amplitude;

  actor->timer += DEGtoANG(45);
  amplitude = (s16 *)&actor->userValue;
  *amplitude += MOAI_SHAKE / 16;
  if (*amplitude > MOAI_SHAKE)
    *amplitude = MOAI_SHAKE;
  sprite->rotation = Iwram_MulFX(Trig_Sin(actor->timer), *amplitude);
}

/// A guardian moai's update once it has moved: the swing dies away over 32
/// frames.
void AquaRock_StopShakingMoai(struct Actor *actor) {
  struct FieldSprite *sprite = actor->sprite;
  s16 *amplitude;

  actor->timer += DEGtoANG(45);
  amplitude = (s16 *)&actor->userValue;
  *amplitude -= MOAI_SHAKE / 32;
  if (*amplitude < 0) {
    *amplitude = 0;
  }
  sprite->rotation = Iwram_MulFX(Trig_Sin(actor->timer), *amplitude);
}

/// The last guardian moai rocks a sixty-fourth of a turn either way: its sine,
/// whose one is a whole turn, divided by this.
#define MOAI_ROCK_DIVISOR 64
/// The last guardian moai's lean, about which it rocks.
#define MOAI_LEAN (TRIG_TURN * 3 / 128)

/// The last guardian moai's update: it keeps rocking gently, leaning a little.
void AquaRock_RockMoai(struct Actor *actor) {
  struct FieldSprite *sprite = actor->sprite;

  actor->timer += TRIG_TURN / 256;
  sprite->rotation = Trig_Sin(actor->timer) / MOAI_ROCK_DIVISOR + MOAI_LEAN;
}

/// Arriving by the north way, as Douse wakes the Apojii Islands' Aqua Stone:
/// the five guardian moai shake and move aside into a row clear of the way
/// in, the last left rocking, and the party goes back to the Apojii Islands
/// where it stood.
void AquaRock_MoaiStepAside(void) {
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetPosition(gMainRoutineState.leaderActorId, 0, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_POP_UP);
  for (i = ACTOR_FIRST_GUARDIAN_MOAI; i <= ACTOR_FIRST_GUARDIAN_MOAI + 4; i++) {
    {
      struct Actor *actor = Actor_GetById(i);

      actor->speedLimit = toFIXED(1) / 4;
      actor->acceleration = toFIXED(1) / 64;
      actor->update = (ObjectUpdate)AquaRock_ShakeMoai;
      actor->timer = Random16();
      actor->userValue = 0;
    }
  }
  Event_Wait(60);
  Map_CopyCellsRect(64, 69, 11, 4, 87, 53);
  Actor_SetDestination(ACTOR_FIRST_GUARDIAN_MOAI, 392, 824);
  Actor_SetDestination(ACTOR_FIRST_GUARDIAN_MOAI + 1, 424, 824);
  Actor_SetDestination(ACTOR_FIRST_GUARDIAN_MOAI + 2, 456, 824);
  Actor_SetDestination(ACTOR_FIRST_GUARDIAN_MOAI + 3, 488, 824);
  Actor_SetDestinationById(ACTOR_FIRST_GUARDIAN_MOAI + 4, 520, 824);
  Event_Wait(240);
  Map_CopyCellsRect(64, 66, 11, 7, 87, 50);
  for (i = ACTOR_FIRST_GUARDIAN_MOAI; i <= ACTOR_FIRST_GUARDIAN_MOAI + 4; i++) {
    Actor_GetById(i)->update = (ObjectUpdate)AquaRock_StopShakingMoai;
  }
  Sound_Play(SE_STOP_EXIT_SE);
  Event_Wait(60);
  {
    struct Actor *actor = Actor_GetById(ACTOR_FIRST_GUARDIAN_MOAI + 4);

    actor->update = (ObjectUpdate)AquaRock_RockMoai;
    actor->timer = 0;
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  Import_EventSetExitRequest(AQUA_ROCK_MOAI_TO_APOJII_ISLANDS);
  Event_End();
  GameFlag_Set(FLAG_AQUA_ROCK_MOAI_STEPPED_ASIDE);
}

/// Sets up the room the party comes into, Retreat leading back to the slopes'
/// south shore. On the slopes: the moai's colours and water, the pillars where
/// they were left, the guardian moai aside once they have moved, the streams,
/// and the bubbles and sparkles of a Doused Water Stone. In the Mist Sabre
/// chest's room: its frozen puddles and dropped rocks, its Water Stone's
/// bubbles and the way inside it opens, and the party's arrival up the stream
/// or from inside.
s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineState.next;
  struct MapStateGroundEffects *map =
      (struct MapStateGroundEffects *)Ram_HeapSlots->mapGrid;
  s32 id, x, z;
  s16 *entrance;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  gMainRoutineState.retreatScene = SCENE_AQUA_ROCK_MOAI;
  gMainRoutineState.retreatEntrance = AQUA_ROCK_MOAI_FROM_WORLD_MAP_SOUTH;
  map->wading = MAP_WADING_UPPER_LEVEL;
  Scene_QueueMusic(SE_AMBIENT_WATER);
  if (scene == SCENE_AQUA_ROCK_MOAI) {
    Actor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_MOAI), SPRITE_COLOR_GBR);
    Actor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_MOAI + 1), SPRITE_COLOR_GBR);
    Actor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_MOAI + 2), SPRITE_COLOR_GBR);
    Actor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_MOAI + 3), SPRITE_COLOR_GBR);
    if (GameFlag_Get(FLAG_AQUA_ROCK_DRY_MOAI_FILLED))
      Actor_SetSpriteColor(Actor_GetById(ACTOR_DRY_MOAI), SPRITE_COLOR_BRG);
    else
      Actor_SetSpriteColor(Actor_GetById(ACTOR_DRY_MOAI), SPRITE_COLOR_GREY);
    AquaRock_AddMoaiWater(ACTOR_FIRST_MOAI);
    AquaRock_AddMoaiWater(ACTOR_FIRST_MOAI + 1);
    AquaRock_AddMoaiWater(ACTOR_FIRST_MOAI + 2);
    AquaRock_AddMoaiWater(ACTOR_FIRST_MOAI + 3);
    AquaRock_AddMoaiWater(ACTOR_FIRST_MOAI + 5);
    AquaRock_AddMoaiWater(ACTOR_FIRST_MOAI + 6);
    for (id = ACTOR_FIRST_BOTTOM_MOAI; id <= ACTOR_LAST_MOAI; id++)
      AquaRock_AddMoaiWater(id);
    for (id = ACTOR_FIRST_MOAI; id < ACTOR_FIRST_BOTTOM_MOAI; id++)
      Actor_SetSpritePriority(id, 3);
    for (id = ACTOR_FIRST_BOTTOM_MOAI; id <= ACTOR_LAST_MOAI; id++)
      Actor_SetSpritePriority(id, 1);
    Actor_GetById(ACTOR_FIRST_BOTTOM_MOAI)->retainSpriteCache = 2;
    Actor_SetAnimationById(ACTOR_FIRST_BOTTOM_MOAI, 3);
    for (id = ACTOR_FIRST_BOTTOM_MOAI + 1; id <= ACTOR_LAST_MOAI; id++) {
      Actor_SetAnimationById(id, 1);
      Actor_SetAnimationById(id, 0);
    }
    if (GameFlag_Get(FLAG_AQUA_ROCK_PILLAR_IN_HOLE))
      Actor_SetPosition(ACTOR_LEDGE_PILLAR, 520, 584);
    x = GameFlag_GetByte(FLAG_PILLAR_COLUMN);
    z = GameFlag_GetByte(FLAG_PILLAR_ROW);
    if (z != 0)
      Actor_SetHorizontalPositionWithTerrain(
          ACTOR_MOAI_PILLAR, FIELD_CELL_CENTER(x), FIELD_CELL_CENTER(z));
    Actor_GetById(Party_GetLeaderActor())->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
    Task_Add(AquaRock_KeepLeaderPriority, TASK_PASS_FRAME);
    Actor_SetSpritePriority(ACTOR_FIRST_FISH + 1, 1);
    Value2((s32 (*)())Actor_SetSpritePriority, ACTOR_MOAI_PILLAR, 1);
    AquaRock_StartBubbles();
    GroundParticles_Start();
    if (GameFlag_Get(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK)) {
      AquaRock_SetBubbleAnchor(ACTOR_WATER_STONE);
      GroundParticles_SetAnchor(ACTOR_WATER_STONE);
      TilesetAnim_Pause(STREAMS_DOWNHILL_ANIMATION);
      TilesetAnim_Pause(STREAMS_DOWNHILL_ANIMATION + 1);
      TilesetAnim_Resume(STREAMS_UPHILL_ANIMATION);
      TilesetAnim_Resume(STREAMS_UPHILL_ANIMATION + 1);
      Actor_GetById(ACTOR_WATER_STONE)->update = AquaRock_StopFlashing;
    }
    Actor_SetSpritePriority(ACTOR_WATER_STONE, 1);
    if (gMainRoutineState.entrance == AQUA_ROCK_MOAI_FROM_WORLD_MAP_NORTH)
      AquaRock_MoaiStepAside();
    if (GameFlag_Get(FLAG_AQUA_ROCK_MOAI_STEPPED_ASIDE)) {
      Actor_SetPosition(ACTOR_FIRST_GUARDIAN_MOAI, 392, 824);
      Actor_SetPosition(ACTOR_FIRST_GUARDIAN_MOAI + 1, 424, 824);
      Actor_SetPosition(ACTOR_FIRST_GUARDIAN_MOAI + 2, 456, 824);
      Actor_SetPosition(ACTOR_FIRST_GUARDIAN_MOAI + 3, 488, 824);
      Actor_SetPosition(ACTOR_FIRST_GUARDIAN_MOAI + 4, 520, 824);
      MapGrid_CopyCellAttributes(64, 66, 9, 3, 24, 53);
      MapGrid_CopyCells(64, 66, 11, 7, 87, 50);
      Actor_GetById(ACTOR_FIRST_GUARDIAN_MOAI + 4)->update = AquaRock_RockMoai;
    }
    if (GameFlag_Get(FLAG_AQUA_ROCK_DRY_MOAI_FILLED)) {
      MapGrid_CopyCellAttributes(62, 41, 1, 2, 33, 41);
      MapGrid_CopyCellTilesCellsRect(116, 95, 3, 13, 96, 32);
      MapGrid_CopyCellsLayered(122, 95, 3, 13, 32, 96);
      AquaRock_AddMoaiWater(ACTOR_DRY_MOAI);
    }
    AquaRock_SetUpStreams();
    if (gMainRoutineState.entrance ==
        AQUA_ROCK_MOAI_FROM_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST)
      AquaRock_ArriveFromPassage();
  } else if (scene == SCENE_AQUA_ROCK_MIST_SABRE_CHEST) {
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_BUBBLE_MARKER), 0);
    if (GameFlag_Get(FLAG_PUDDLE_A_FROZEN))
      MapGrid_CopyCellAttributes(42, 32, 1, 1, 44, 32);
    if (GameFlag_Get(FLAG_PUDDLE_B_FROZEN))
      MapGrid_CopyCellAttributes(48, 32, 1, 1, 46, 32);
    if (GameFlag_Get(FLAG_PUDDLE_C_FROZEN))
      MapGrid_CopyCellAttributes(44, 44, 1, 1, 46, 44);
    Actor_SetAnimationById(ACTOR_FIRST_ROCK, 2);
    if (GameFlag_Get(FLAG_AQUA_ROCK_FIRST_ROCK_DROPPED)) {
      Actor_SetPosition(ACTOR_FIRST_ROCK, 856, 840);
      MapGrid_CopyCellAttributes(54, 58, 1, 1, 53, 52);
    }
    Actor_SetAnimationById(ACTOR_SECOND_ROCK, 2);
    if (GameFlag_Get(FLAG_AQUA_ROCK_SECOND_ROCK_DROPPED)) {
      Actor_SetPosition(ACTOR_SECOND_ROCK, 856, 936);
      MapGrid_CopyCellAttributes(54, 58, 1, 1, 53, 58);
    }
    if (gMainRoutineState.entrance == 5 || gMainRoutineState.entrance == 7) {
      AquaRock_StartBubbles();
      GroundParticles_Start();
      if (GameFlag_Get(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK)) {
        AquaRock_SetBubbleAnchor(ACTOR_BUBBLE_MARKER);
        GroundParticles_SetAnchor(ACTOR_BUBBLE_MARKER);
      } else {
        TilesetAnim_Pause(STREAMS_UPHILL_ANIMATION);
        TilesetAnim_Pause(STREAMS_UPHILL_ANIMATION + 1);
        TilesetAnim_Resume(STREAMS_DOWNHILL_ANIMATION);
        TilesetAnim_Resume(STREAMS_DOWNHILL_ANIMATION + 1);
      }
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BUBBLE_MARKER), 0);
    } else {
      AquaRock_StartBubbles();
      GroundParticles_Start();
      Actor_GetById(ACTOR_MIST_SABRE_WATER_STONE)->update =
          AquaRock_StopFlashing;
      if (GameFlag_Get(FLAG_AQUA_ROCK_MIST_SABRE_CHEST_WATER_STONE_DOUSED)) {
        AquaRock_SetBubbleAnchor(ACTOR_MIST_SABRE_WATER_STONE);
        GroundParticles_SetAnchor(ACTOR_MIST_SABRE_WATER_STONE);
        Actor_SetPosition(ACTOR_WAY_INSIDE, 264, 792);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_WAY_INSIDE), 0);
        Actor_GetById(ACTOR_WAY_INSIDE)->collisionFlags = 0;
        Actor_GetById(ACTOR_WAY_INSIDE)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Actor_GetById(ACTOR_WAY_INSIDE)->update = Actor_EmitWaterDrop;
        Map_CopyCellAttributeRect(15, 48, 1, 1, 16, 48);
        Map_CopyCellAttributeRect(17, 50, 1, 1, 16, 50);
        MapGrid_CopyCellTilesCellsRect(64, 68, 5, 13, 14, 94);
        Map_CopyCellAttributeRect(64, 68, 5, 8, 14, 34);
      }
    }
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      if (gMainRoutineState.entrance ==
          AQUA_ROCK_MIST_SABRE_CHEST_FROM_AQUA_ROCK_INSIDE_VIAL_CHEST)
        AquaRock_ArriveFromInside();
      entrance = &gMainRoutineState.entrance;
      if (*entrance == AQUA_ROCK_MIST_SABRE_CHEST_FROM_AQUA_ROCK_MOAI_C)
        AquaRock_ArriveUpStream();
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "shared/actor_spawn_wading_splash.h"

#include "shared/actor_emit_water_drop.h"

#include "shared/leader_flow_with_stream.h"

/// Gives a moai its water: the pool in the top of its head, pouring from its
/// mouth.
void AquaRock_AddMoaiWater(s32 id) {
  struct Actor *actor = Actor_GetById(id);

  if (actor != 0) {
    struct SpriteAnimation *water =
        (struct SpriteAnimation *)Sprite_AddAnimation(actor->sprite,
                                                      SPRITE_MOAI_WATER);

    water->colorMode = SPRITE_COLOR_NORMAL;
    water->priority = 0;
    Actor_SelectAnimation(actor, 0);
    Actor_SelectAnimation(actor, 2);
  }
}

/// Once the Water Stone is Doused, stepping on the way inside: the leader
/// tumbles and spins down into the pool and leaves by `exit`, to the Vial
/// chest's room inside.
void AquaRock_SinkInside(s32 exit) {
  struct Actor *actor = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 i;
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpritePriority(gMainRoutineState.leaderActorId, 1);
  actor->motionFlags = 0;
  actor->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Sound_Play(SE_SPLASH_SMALL);
  Actor_SelectAnimation(actor, ANIM_LEADER_TUMBLE_IN_WATER);
  Sound_Play(SE_SPIN_AWAY);
  for (i = 0; i < 60; i++) {
    if (i == 30)
      Event_FinishScreenTransition();
    actor->y.fixed -= toFIXED(1) / 4;
    angle = actor->facing;
    angle += TRIG_TURN / 16;
    actor->facing = angle;
    if ((i & 7) == 0)
      Actor_SpawnWadingSplash(Actor_GetById(ACTOR_WAY_INSIDE));
    wait(1);
  }
  Event_End();
  Import_EventSetExitRequest(exit);
}

#include "shared/actor_emit_mist.h"

/// The leader walks into the sparkling passage in a mist and leaves by
/// `entrance`.
void AquaRock_EnterSparklingPassage(s32 entrance) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 *owner = &gMainRoutineState.leaderActorId;

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_SPARKLING_PASSAGE);
  leader->update = (ObjectUpdate)Actor_EmitMist;
  leader->motionFlags = 0;
  leader->speedLimit = toFIXED(1) / 5;
  Actor_SetAnimationById(*owner, 2);
  Actor_SetDestinationOffsetById(*owner, 0, -8);
  Actor_WaitForMoveById(*owner);
  Actor_SetSpriteColor(Actor_GetById(*owner), SPRITE_COLOR_BLUE_TINT);
  Actor_SetSpriteFlags(Actor_GetById(*owner), 0);
  leader->update = 0;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(entrance);
  Event_End();
}

/// Arriving from the sparkling passage, unless the party's place is kept: the
/// leader walks out of the mist.
void AquaRock_ArriveFromPassage(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 flag = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
  s32 *owner = &gMainRoutineState.leaderActorId;

  if (flag == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    leader->motionFlags = flag;
    Actor_SetHorizontalPositionWithTerrain(
        *owner, toFIXED(leader->x.part[0].integer),
        toFIXED(leader->z.part[0].integer) - toFIXED(16));
    Actor_SetSpriteColor(Actor_GetById(*owner), SPRITE_COLOR_BLUE_TINT);
    Actor_SetSpriteFlags(Actor_GetById(*owner), 0);
    Event_StartScreenTransition();
    Sound_Play(SE_SPARKLING_PASSAGE);
    leader->update = (ObjectUpdate)Actor_EmitMist;
    Actor_SetSpeed(*owner, 0.4);
    Actor_WalkByAndWaitById(*owner, 0, 8);
    Actor_SetSpriteColor(Actor_GetById(*owner), SPRITE_COLOR_NORMAL);
    Actor_SetSpriteFlags(Actor_GetById(*owner), SPRITE_FLAG_SHADOW);
    ((struct FieldSprite *)leader->sprite)->priority = 1;
    Actor_WalkByAndWaitById(*owner, 0, 10);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    leader->update = (ObjectUpdate)flag;
    Event_NoOp();
    Event_WaitTransitionFrames();
    Event_End();
  }
}

/// Each frame, for bubbles going up: now and then one starts from the actor
/// they come from, and each rises up the stream, north, until it reaches the
/// top; those in view are drawn.
void AquaRock_DrawRisingBubbles(void) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
#ifndef UBFIX
  s32 *view = &map->viewX;
#endif
  // UB: viewZ is read as view[1], though it is a separate member, not a second
  // element of viewX.
#ifdef UBFIX
  s32 viewX = map->viewX & FIELD_PIXEL_MASK;
  s32 viewZ = map->viewZ & FIELD_PIXEL_MASK;
#else
  s32 viewX = view[0] & FIELD_PIXEL_MASK;
  s32 viewZ = view[1] & FIELD_PIXEL_MASK;
#endif
  s32 lift = map->origin[1];
  struct AquaRockBubbles *state = &gAquaRock_Bubbles;
  u32 tile = gVramCacheEntries[state->vramEntry].offset >> 5;
  struct Actor *source;
  struct Bubble *entry;
  s32 i, x, y, z, screenX, screenY;

  if (Random16() < 4096 && (source = state->anchor) != 0) {
    entry = &state->bubbles[state->next];
    entry->x = source->x.fixed + Random16() * 3 * 16 - toFIXED(24);
    entry->y = source->y.fixed;
    entry->z = source->z.fixed;
    entry->age = 0;
    if (++state->next > STREAM_BUBBLE_COUNT - 1)
      state->next = 0;
  }
  entry = state->bubbles;
  for (i = 0; i < STREAM_BUBBLE_COUNT; i++, entry++) {
    if (entry->x != 0) {
      // screenX holds the ground's height here, before its own value.
      screenX = Map_GetTerrainHeight(0, entry->x, entry->z);
      if (screenX > entry->y)
        entry->y += toFIXED(2);
      else
        entry->z -= toFIXED(2);
      if (FIELD_CELL(entry->y) == 23)
        entry->x = 0;
      if (FIELD_CELL(entry->z) == 48)
        entry->x = 0;
      entry->age++;
      x = entry->x - viewX;
      y = entry->y;
      z = entry->z;
      y -= lift;
      z -= viewZ;
      z -= lift;
      screenY = z - y;
      y += z;
      y = FIXEDtoINT(y);
      screenX = FIXEDtoINT(x);
      x = screenX - 4;
      screenY = FIXEDtoINT(screenY);
      screenY -= 4;
      z = y + 58;
      // screenX now holds its offset into the range test (on screen from
      // -11 to 323).
      screenX += 11;
      if ((u32)screenX <= 334 && screenY > -16 && screenY <= 239) {
        x &= OBJ_X_MASK;
        screenY &= OBJ_Y_MASK;
        entry->oam[0] = 0;
        entry->oam[1] = screenY | OBJ_X(x) | OBJ_256_COLORS;
        entry->oam[2] = (tile + (entry->age & 3) * 2) | OBJ_PRIORITY(1);
        Oam_AddSpriteAtDepth(entry->oam, z);
      }
    }
  }
}

/// Each frame, for bubbles going down: now and then one starts from the actor
/// they come from, and each falls down the stream, south, until it reaches the
/// bottom; those in view are drawn.
void AquaRock_DrawFallingBubbles(void) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & FIELD_PIXEL_MASK;
  s32 viewZ = view[1] & FIELD_PIXEL_MASK;
  s32 lift = map->origin[1];
  struct AquaRockBubbles *state = &gAquaRock_Bubbles;
  u32 tile = gVramCacheEntries[state->vramEntry].offset >> 5;
  struct Actor *source;
  struct Bubble *bubble;
  s32 i, x, y, depth, screenX, screenY, z, row;

  if (Random16() < 4096 && (source = state->anchor) != 0) {
    bubble = &state->bubbles[state->next];
    bubble->x = source->x.fixed + Random16() * 3 * 8 - toFIXED(12);
    bubble->y = source->y.fixed;
    bubble->z = source->z.fixed;
    bubble->age = 0;
    if (++state->next > STREAM_BUBBLE_COUNT - 1)
      state->next = 0;
  }
  bubble = state->bubbles;
  for (i = 0; i < STREAM_BUBBLE_COUNT; i++, bubble++) {
    if (bubble->x != 0) {
      if (Map_GetTerrainHeight(0, bubble->x, bubble->z) < bubble->y)
        bubble->y -= toFIXED(2);
      else
        bubble->z += toFIXED(2);
      if (FIELD_CELL(bubble->y) == -4)
        bubble->x = 0;
      if (FIELD_CELL(bubble->z) == 50)
        bubble->x = 0;
      bubble->age++;
      z = bubble->z;
      x = bubble->x - viewX;
      y = bubble->y - lift;
      z -= viewZ;
      z -= lift;
      row = z - y;
      screenX = FIXEDtoINT(x);
      y += z;
      x = screenX - 4;
      screenY = FIXEDtoINT(row) - 4;
      y = FIXEDtoINT(y);
      depth = y + 58;
      if ((u32)(screenX + 11) <= 334 && screenY > -16 && screenY <= 239) {
        x &= OBJ_X_MASK;
        screenY &= OBJ_Y_MASK;
        bubble->oam[0] = 0;
        bubble->oam[1] = screenY | OBJ_X(x) | OBJ_256_COLORS;
        bubble->oam[2] = (tile + (bubble->age & 3) * 2) | OBJ_PRIORITY(1);
        Oam_AddSpriteAtDepth(bubble->oam, depth);
      }
    }
  }
}

/// Each frame: the bubbles go up or down the stream, as their direction says.
void AquaRock_DrawBubbles(void) {
  if (gAquaRock_Bubbles.direction > 0)
    AquaRock_DrawRisingBubbles();
  else
    AquaRock_DrawFallingBubbles();
}

/// Clears the bubbles, loads their tiles into VRAM and starts their drawing.
void AquaRock_StartBubbles(void) {
  struct AquaRockBubbles *state = &gAquaRock_Bubbles;
  void *buffer;

  Iwram_ClearWordsEntry(state, sizeof(struct AquaRockBubbles));
  buffer = MemAlloc_(256);
  Decompress_TaggedLz(gAquaRock_MarkerTiles, buffer);
  state->vramEntry = VramCache_FindFreeEntry();
  VramCache_Load(state->vramEntry, 256, buffer);
  Heap_Release(buffer);
  ((s32 (*)(void *, s32))Task_Add)(AquaRock_DrawBubbles, TASK_PASS_FRAME);
}

/// Sets the actor the bubbles come from, or with 0 none: they fall from the
/// Mist Sabre chest's room's Water Stone and rise from any other.
void AquaRock_SetBubbleAnchor(s32 id) {
  struct AquaRockBubbles *state = &gAquaRock_Bubbles;

  if (id == 0) {
    state->anchor = 0;
  } else {
    state->anchor = Actor_GetById(id);
    if (id == ACTOR_MIST_SABRE_WATER_STONE)
      state->direction = -1;
    else
      state->direction = 1;
  }
}

#include "shared/ground_particles_draw.h"

#include "shared/ground_particles_start.h"

#include "shared/ground_particles_set_anchor.h"

// clang-format off
#include "shared/field_effect_duration_scripts.c"

/// A fish's script: it swims to random spots, resting between.
s32 gAquaRock_FishScript[] = {
    SC_CALL((s32)AquaRock_StartFish),
    SC_LABEL(1),
    SC_ANIMATION(2),
    SC_CALL((s32)AquaRock_PickRandomSpot),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(120),
    SC_ANIMATION(2),
    SC_CALL((s32)AquaRock_PickRandomSpot),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_CALL((s32)AquaRock_PickRandomSpot),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(180),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

/// A fish's script: it swims to random spots, resting longer between.
s32 gAquaRock_SlowFishScript[] = {
    SC_CALL((s32)AquaRock_StartFish),
    SC_LABEL(1),
    SC_WAIT(60),
    SC_ANIMATION(2),
    SC_CALL((s32)AquaRock_PickRandomSpot),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(120),
    SC_ANIMATION(2),
    SC_CALL((s32)AquaRock_PickRandomSpot),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(15),
    SC_ANIMATION(2),
    SC_CALL((s32)AquaRock_PickRandomSpot),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(180),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

/// A splash's script: it shows for 20 frames, then is released.
u32 gWadingSplash_Script[] = {
    SC_WAIT(20),
    SC_RELEASE,
};

#include "shared/water_drop_script.c"
// clang-format on

#undef DEFINE_AQUA_ROCK_FREEZE_PUDDLE
#undef DEFINE_AQUA_ROCK_DROP_AT_COLUMN
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY

// clang-format off
#define GROUND_PARTICLE_FRAMES_SECTION LATE_DATA
#include "shared/ground_particle_frames.c"

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

/// The slopes' exit regions: the two ways up into the Mist Sabre chest's room.
LATE_DATA struct SceneRegion gAquaRock_SlopesRegions[] = {
    {176, 424, 544, 192, 440, 560, CONDITION_ALWAYS,
     AQUA_ROCK_MOAI_TO_AQUA_ROCK_MIST_SABRE_CHEST_A},
    {272, 424, 544, 288, 440, 560, CONDITION_ALWAYS,
     AQUA_ROCK_MOAI_TO_AQUA_ROCK_MIST_SABRE_CHEST_B},
    {SCENE_TABLE_END},
};

/// The Mist Sabre chest's room's exit regions: the ways back down to the
/// slopes.
LATE_DATA struct SceneRegion gAquaRock_MistSabreChestRegions[] = {
    {48, -56, 960, 64, -40, 976, CONDITION_ALWAYS,
     AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_A},
    {144, -56, 960, 160, -40, 976, CONDITION_ALWAYS,
     AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_B},
    {236, -108, 956, 260, -84, 980, CONDITION_ALWAYS,
     AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_D},
    {268, -108, 956, 292, -84, 980, CONDITION_ALWAYS,
     AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_D},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(AquaRockMoai),
    SCENE_EXIT(AQUA_ROCK_MOAI_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_AQUA_ROCK_MOAI_SOUTH),
    SCENE_EXIT(AQUA_ROCK_MOAI_TO_AQUA_ROCK_MIST_SABRE_CHEST_A, AquaRockMistSabreChest, AQUA_ROCK_MIST_SABRE_CHEST_FROM_AQUA_ROCK_MOAI_A),
    SCENE_EXIT(AQUA_ROCK_MOAI_TO_AQUA_ROCK_MIST_SABRE_CHEST_B, AquaRockMistSabreChest, AQUA_ROCK_MIST_SABRE_CHEST_FROM_AQUA_ROCK_MOAI_B),
    SCENE_EXIT(AQUA_ROCK_MOAI_TO_AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST, AquaRockInsideRustySwordChest, AQUA_ROCK_INSIDE_RUSTY_SWORD_CHEST_FROM_AQUA_ROCK_MOAI),
    SCENE_EXIT(AQUA_ROCK_MOAI_TO_AQUA_ROCK_MIST_SABRE_CHEST_C, AquaRockMistSabreChest, AQUA_ROCK_MIST_SABRE_CHEST_FROM_AQUA_ROCK_MOAI_C),
    SCENE_EXIT(AQUA_ROCK_MOAI_TO_APOJII_ISLANDS, ApojiiIslands, APOJII_ISLANDS_FROM_WORLD_MAP_A),
    SCENE_EXITS(AquaRockMistSabreChest),
    SCENE_EXIT(AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST, AquaRockInsideVialChest, AQUA_ROCK_INSIDE_VIAL_CHEST_FROM_AQUA_ROCK_MIST_SABRE_CHEST),
    SCENE_EXIT(AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_A, AquaRockMoai, AQUA_ROCK_MOAI_FROM_AQUA_ROCK_MIST_SABRE_CHEST_A),
    SCENE_EXIT(AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_B, AquaRockMoai, AQUA_ROCK_MOAI_FROM_AQUA_ROCK_MIST_SABRE_CHEST_B),
    SCENE_EXIT(4, AquaRockMistSabreChest, 5),
    SCENE_EXIT(5, AquaRockMistSabreChest, 4),
    SCENE_EXIT(AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_D, AquaRockMoai, AQUA_ROCK_MOAI_FROM_AQUA_ROCK_MIST_SABRE_CHEST_D),
    SCENE_EXITS_END,
};

/// The scene's other rooms' actors.
LATE_DATA struct ActorPlacement gAquaRock_Placements[] = {
    {SCENE_TABLE_END},
};

/// The slopes' actors, in AquaRockSlopesActorId's order.
LATE_DATA struct ActorPlacement gAquaRock_SlopesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 551, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 551, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 615, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 615, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 647, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 647, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 679, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 823, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 927, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 879, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 863, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 847, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 863, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 879, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 847, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MOAI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 959, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WATER_DROP_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 600, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FISH, CONDITION_ALWAYS, (s32)gAquaRock_FishScript, 376, 0, 960, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FISH, CONDITION_ALWAYS, (s32)gAquaRock_FishScript, 136, 0, 904, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FISH, CONDITION_ALWAYS, (s32)gAquaRock_SlowFishScript, 144, 0, 912, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The Mist Sabre chest's room's actors, in AquaRockMistSabreChestActorId's
/// order.
LATE_DATA struct ActorPlacement gAquaRock_MistSabreChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WATER_DROP_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EMBER_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 840, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EMBER_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 936, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 936, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The frame counts of the four red moai's streams' cycles
/// (AquaRock_TimeStreams).
LATE_DATA s32 gAquaRock_FirstStreamTimer = 0;
LATE_DATA s32 gAquaRock_SecondStreamTimer = 0;
LATE_DATA s32 gAquaRock_ThirdStreamTimer = 0;
LATE_DATA s32 gAquaRock_FourthStreamTimer = 0;

/// The scene's other rooms' triggers.
LATE_DATA struct Trigger gAquaRock_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// The slopes' exits, currents and streams, Move on the pillars and Douse on
/// the dry moai and the Water Stone.
LATE_DATA struct Trigger gAquaRock_SlopesTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, AQUA_ROCK_MOAI_TO_WORLD_MAP_SOUTH},
    {STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_SPARKLING_PASSAGE, CONDITION_ALWAYS, EVENT(AquaRock_EnterSparklingPassage)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WEST_MOAI_STREAM, FLAG_PILLAR_BEFORE_WEST_MOAI, EVENT(AquaRock_StepIntoCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_EAST_MOAI_STREAM, FLAG_PILLAR_BEFORE_EAST_MOAI, EVENT(AquaRock_StepIntoCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LOWER_WEST_MOAI_STREAM, CONDITION_ALWAYS, EVENT(AquaRock_StepIntoCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LOWER_EAST_MOAI_STREAM, CONDITION_ALWAYS, EVENT(AquaRock_StepIntoCurrent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_DOWNHILL_STREAM, CONDITION_ALWAYS, EVENT(Leader_FlowWithStream)},
    {TRIGGER_STEP, CELL_BEHAVIOR_TURNING_STREAM, CONDITION_ALWAYS, EVENT(AquaRock_RideStreamUp)},
    {TRIGGER_RAISED, TRIGGER_DROP_DOWN_FALLS, CONDITION_ALWAYS, EVENT(AquaRock_DropDownFalls)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_DRY_MOAI, FLAG_AQUA_ROCK_DRY_MOAI_FILLED, EVENT(AquaRock_DouseDryMoai)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_WATER_STONE, FLAG_AQUA_ROCK_STREAMS_TURNED_BACK, EVENT(AquaRock_DouseWaterStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MOAI_PILLAR, CONDITION_ALWAYS, EVENT(AquaRock_MovePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEDGE_PILLAR, FLAG_AQUA_ROCK_PILLAR_IN_HOLE, EVENT(AquaRock_MovePillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AquaRock_MovePillar)},
    {TRIGGER_TABLE_END},
};

/// The Mist Sabre chest's room's way inside, exits, streams, puddles to Frost,
/// rocks to drop and Water Stone to Douse.
LATE_DATA struct Trigger gAquaRock_MistSabreChestTriggers[] = {
    {TRIGGER_STEP, AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_INSIDE_VIAL_CHEST, CONDITION_ALWAYS, EVENT(AquaRock_SinkInside)},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_A},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, AQUA_ROCK_MIST_SABRE_CHEST_TO_AQUA_ROCK_MOAI_B},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, CELL_BEHAVIOR_DOWNHILL_STREAM, CONDITION_ALWAYS, EVENT(Leader_FlowWithStream)},
    {TRIGGER_STEP, CELL_BEHAVIOR_TURNING_STREAM, CONDITION_ALWAYS, EVENT(AquaRock_RideStreamUp)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_A, FLAG_PUDDLE_A_FROZEN, EVENT(AquaRock_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_B, FLAG_PUDDLE_B_FROZEN, EVENT(AquaRock_FreezeSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_C, FLAG_PUDDLE_C_FROZEN, EVENT(AquaRock_FreezeThirdPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_ROCK, FLAG_AQUA_ROCK_FIRST_ROCK_DROPPED, EVENT(AquaRock_DropFirstRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_ROCK, FLAG_AQUA_ROCK_SECOND_ROCK_DROPPED, EVENT(AquaRock_DropSecondRock)},
    {TRIGGER_PUSHED, 0, FLAG_AQUA_ROCK_SECOND_ROCK_DROPPED, EVENT(AquaRock_DropSecondRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_MIST_SABRE_WATER_STONE, FLAG_AQUA_ROCK_MIST_SABRE_CHEST_WATER_STONE_DOUSED, EVENT(AquaRock_DouseMistSabreChestWaterStone)},
    {TRIGGER_TABLE_END},
};

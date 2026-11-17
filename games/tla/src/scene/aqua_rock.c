/// @file
/// Aqua Rock's outside, the island south of the Apojii Islands, in the rooms
/// of this overlay: currents the party runs against, a pillar Moved before
/// the red Moai, dry Moai that Douse turns into new streams, puddles to
/// Frost, and the tear-shaped rock whose Douse opens the way inside
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
#include "system/main_routine_work.h"
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

/// A bubble or drop of the cell markers: its place, its age in frames and its
/// sprite's attributes.
struct Bubble {
  s32 x;
  s32 y;
  s32 z;
  volatile s32 age;
  u32 oam[3];
};

/// The cell markers' work: the actor they come from, their VRAM entry, the next
/// free bubble, which way they go (rising when positive) and 64 bubbles.
struct AquaRockWork {
  struct Actor *object;
  u16 vramEntry;
  u16 next;
  s16 direction;
  u8 unusedAfterDirection[2]; ///< no code names it
  struct Bubble bubbles[64];
};

void Actor_StartScript(struct Actor *actor, const void *data);
s32 Actor_EmitMist(struct Actor *actor);

void AquaRock_TurnSprite(struct Actor *actor);
void AquaRock_SinkOrFlow(struct Actor *actor);
void AquaRock_DrawRisingCellMarkers(void);
void AquaRock_DrawFallingCellMarkers(void);
void AquaRock_DrawCellMarkers(void);

extern u32 gAquaRock_SplashScript[];
extern u32 gWaterDropScript[];
/// The cell markers' tiles, compressed (graphics/aqua_rock/tiles.s).
extern const u8 gAquaRock_MarkerTiles[];
extern s16 gAquaRock_SlopesRegions[];
extern s16 gAquaRock_MistSabreChestRegions[];
extern struct ActorPlacement gAquaRock_Placements[];
extern struct ActorPlacement gAquaRock_SlopesPlacements[];
extern struct ActorPlacement gAquaRock_MistSabreChestPlacements[];

/// The slopes by the entrance (gAquaRock_SlopesPlacements).
/// Aqua Rock's lasting flags outside: the pillar Moved before the Moai, the
/// two rocks dropped into the current, the Moai stepped aside from the way in,
/// the streams the Water Stone has turned back (Douse turns them each time),
/// and the Moai and the tear-shaped rock Douse has filled.
#define FLAG_AQUA_ROCK_PILLAR_BEFORE_MOAI 0x950
#define FLAG_AQUA_ROCK_FIRST_ROCK_DROPPED 0x951
#define FLAG_AQUA_ROCK_SECOND_ROCK_DROPPED 0x952
#define FLAG_AQUA_ROCK_MOAI_STEPPED_ASIDE 0x955
#define FLAG_AQUA_ROCK_STREAMS_TURNED_BACK 0xa68
#define FLAG_AQUA_ROCK_DRY_MOAI_DOUSED 0xa69
#define FLAG_AQUA_ROCK_TEAR_ROCK_DOUSED 0xa6b

enum AquaRockSlopesActorId {
  /// The rock's sixteen Moai statues.
  ACTOR_FIRST_MOAI = ACTOR_FIRST_PLACED,
  /// The dried-up Moai Douse sets flowing again (AquaRock_DouseDryMoai).
  ACTOR_DRY_MOAI = ACTOR_FIRST_MOAI + 7,
  /// The five Moai that step aside together (AquaRock_MoaiStepAside).
  ACTOR_FIRST_STEPPING_MOAI = ACTOR_FIRST_MOAI + 9,
  /// The last of the sixteen Moai.
  ACTOR_LAST_MOAI = ACTOR_FIRST_MOAI + 15,
  /// The pillars Move pushes before the Moai.
  ///@{
  ACTOR_FIRST_PILLAR = ACTOR_FIRST_MOAI + 16,
  ACTOR_SECOND_PILLAR,
  ///@}
  /// The Water Stone that turns the stream when Doused.
  ACTOR_WATER_STONE,
  /// The three drifters in the water (sprite 401), on gAquaRock_DriftScript and
  /// gAquaRock_SlowDriftScript.
  ACTOR_FIRST_DRIFTER
};

/// The Mist Sabre chest's room (gAquaRock_MistSabreChestPlacements).
enum AquaRockMistSabreChestActorId {
  ACTOR_MIST_SABRE_WATER_STONE = ACTOR_FIRST_PLACED,
  /// The three puddles Frost freezes.
  ///@{
  ACTOR_PUDDLE_A,
  ACTOR_PUDDLE_B,
  ACTOR_PUDDLE_C,
  ///@}
  /// The two ember stones Move pushes into the current.
  ///@{
  ACTOR_FIRST_EMBER_STONE,
  ACTOR_SECOND_EMBER_STONE,
  ///@}
  /// An unseen marker the bubbles and sparkles rise from once the streams are
  /// turned back.
  ACTOR_BUBBLE_MARKER,
  /// Where the stream the dry Moai pours out starts, dropping water.
  ACTOR_STREAM_SOURCE
};
extern struct Trigger gAquaRock_Triggers[];
extern struct Trigger gAquaRock_SlopesTriggers[];
extern struct Trigger gAquaRock_MistSabreChestTriggers[];

/// Whether the game flag is set.
static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Get(flag); }

/// Clears the game flag.
static __inline__ void GameFlag_ClearBit_(s32 flag) { GameFlag_Clear(flag); }

static __inline__ struct Actor *Import_ActorCreate_(s32 kind, s32 x, s32 y,
                                                    s32 z) {
  return Import_ActorCreate(kind, x, y, z);
}

/// Takes `size` bytes from the heap.
static __inline__ void *MemAlloc_(s32 size) { return Heap_Alloc(size); }
#include "flags.h"
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
#include "system/main_routine_work.h"
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

/// Creates a hidden, still actor of `kind` at a point, untargeted, in colour 15
/// and drawn underfoot.
struct Actor *AquaRock_CreateActor(s32 x, s32 y, s32 z, s32 kind) {
  struct Actor *actor = Import_ActorCreate(kind, x, y, z);

  if (actor != 0) {
    Scene_SetActorSpritePriority(actor, 1);
    actor->motionFlags = 0;
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpriteColor(actor, 15);
    actor->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT | ACTOR_PRIORITY_IGNORES_RISES;
    return actor;
  }
  return 0;
}

#include "../field/field_effect_move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

#undef EFFECT_SPAWN

struct Actor;

void AquaRock_SetSparkleAnchor(s32 id);
void AquaRock_SetCellMarkerAnchor(s32 id);
void AquaRock_FlowWithStream(void);
void AquaRock_SpawnSplash(struct Actor *actor);
void AquaRock_SetMoaiFace(s32 id);
void Actor_EmitWaterDrop(struct Actor *actor);

void AquaRock_KeepLeaderPriority(void);
void AquaRock_StartCellMarkers(void);
void AquaRock_StartSparkles(void);
void AquaRock_MoaiStepAside(void);
void AquaRock_SetUpStreams(void);
void AquaRock_ArriveFromPassage(void);
void AquaRock_SpinOut(void);
void AquaRock_DropIn(void);

extern s16 gGroundParticleFrames[];

s32 AquaRock_StartDrifter(struct Actor *actor);
void AquaRock_RideStreamUp(void);
void AquaRock_StepIntoCurrent(void);

void AquaRock_DouseTearRock(void);

void AquaRock_DropDownFalls(void);

/// One of the rock's water streams, which turns on and off with its flag:
/// each change runs down it as a wave, copying three cells a row from the
/// open or closed pictures beside it, a row every four frames.
struct AquaRockStream {
  s16 pictureSourceX, pictureSourceY; ///< its pictures, closed then open
  s16 cellSourceX, cellSourceY;       ///< its cells, closed then open
  s16 pictureX, pictureY;             ///< where its pictures are copied
  s16 cellX, cellY;                   ///< where its cells are copied
  s16 height;                         ///< its rows
  /// The flag that opens it; with 0x1000 the stream is open while it is clear.
  s16 flag;
  s16 shown;       ///< the flag's value it was last told
  s16 waveRow[2];  ///< each wave's row times four; 9999 for none
  s16 waveOpen[2]; ///< whether each wave opens the stream or closes it
  s16 nextWave;
};

/// The five streams AquaRock_UpdateStreams turns on and off.
struct AquaRockStream gAquaRock_Streams[5] BSS_DATA ALIGNED(16);
struct AquaRockWork gAquaRock_CellMarkers BSS_DATA;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const u8 *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_AQUA_ROCK_MOAI)
    return gAquaRock_SlopesRegions;
  if (scene == SCENE_AQUA_ROCK_MIST_SABRE_CHEST)
    return gAquaRock_MistSabreChestRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// A drifter's script call: picks a random spot ahead within 16 pixels of its
/// home and moves there; after three misses it turns around.
s32 AquaRock_PickRandomSpot(struct Actor *actor) {
  struct Vector3 spot;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  struct Vector3 unused;
  s32 x;
  FIXED coordinate;
  s32 tries, maxDistanceSquared;
  FIXED distance;
  s32 dx, dz;
  u32 a, b;

  tries = 0;
  maxDistanceSquared = 256;
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
    x = spot.x;
    if (x < 0)
      x += FIXED_ONE - 1;
    dx = (x >> 16) - actor->timer;
    coordinate = spot.z;
    if (coordinate < 0)
      coordinate += FIXED_ONE - 1;
    dz = (coordinate >> 16) - (s16)actor->userValue;
    if (dx * dx + dz * dz > maxDistanceSquared)
      goto again;
    goto found;
  }
  actor->facing += DEGtoANG(180);
  actor->wait = 1;
  goto end;
found:
  coordinate = spot.y;
  Actor_SetMoveTarget(actor, spot.x, coordinate, spot.z);
end:
  /* FAKEMATCH(temporary): the form kept. */
  x = 0;
  return x;
}

/// A drifter's update: its sprite turns with its facing.
void AquaRock_TurnSprite(struct Actor *actor) {
  struct FieldSprite *sprite;
  s32 angle;

  angle = actor->facing;
  sprite = actor->sprite;
  sprite->rotation = angle + DEGtoANG(90);
}

/// A drifter's script call: hides its sprite and gives it its speed and its
/// turning update.
s32 AquaRock_StartDrifter(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  actor->speedLimit = toFIXED(1);
  actor->acceleration = toFIXED(1) / 20;
  actor->collisionFlags = 0;
  actor->update = (void (*)(void))AquaRock_TurnSprite;
  return 0;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_AQUA_ROCK_MOAI)
    return gAquaRock_SlopesPlacements;
  if (scene == SCENE_AQUA_ROCK_MIST_SABRE_CHEST)
    return gAquaRock_MistSabreChestPlacements;
  return gAquaRock_Placements;
}

/// A drop's update: it falls until it reaches the water, then flows north, and
/// is released at the bottom or the edge.
void AquaRock_SinkOrFlow(struct Actor *actor) {
  if (Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed) > actor->y.fixed)
    actor->y.fixed += toFIXED(2);
  else
    actor->z.fixed -= toFIXED(2);
  if (FIELD_CELL(actor->y.fixed) == 23)
    Actor_Release(actor);
  if (FIELD_CELL(actor->z.fixed) == 48)
    Actor_Release(actor);
}

/// Now and then a drop falls from somewhere along the actor's 48-pixel width.
void AquaRock_DripAtRandom(struct Actor *actor) {
  if ((u32)Random16() < 4096) {
    struct Actor *child;
    s32 spread = Random16();
    FIXED x = actor->x.fixed;

    x += spread * 48;
    child = Import_ActorCreate(SPRITE_GLINT, x - toFIXED(24), actor->y.fixed,
                               actor->z.fixed);
    if (child != 0) {
      Actor_SetSpriteFlags(child, 0);
      child->update = (void (*)(void))AquaRock_SinkOrFlow;
      child->motionFlags = 0;
    }
  }
}

/// Each frame: a stream whose flag has changed sends a wave down it, opening or
/// closing it a row every four frames.
void AquaRock_UpdateStreams(void) {
  struct AquaRockStream *entry;
  s32 n, i, flag;

  entry = gAquaRock_Streams;
  for (n = 0; n <= 4; n++, entry++) {
    flag = GameFlag_Get(entry->flag & 0xfff);
    if (entry->flag & 0x1000)
      flag ^= 1;
    if (entry->shown != flag) {
      entry->waveRow[entry->nextWave] = 0;
      entry->waveOpen[entry->nextWave] = flag;
      if (++entry->nextWave > 1)
        entry->nextWave = 0;
      entry->shown = flag;
    }
    for (i = 0; i < 2; i++) {
      s32 row = entry->waveRow[i];
      if (row < entry->height * 4) {
        s32 inverse;
        row /= 4;
        flag = entry->waveOpen[i];
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

/// Unless flag 0x104 is set, runs four 240-frame timers
/// (gAquaRock_FirstStreamTimer to gAquaRock_FourthStreamTimer) that hold flags
/// 0x200 to 0x203 set for their last 90 frames; in their first 150 the first
/// two follow flags 0x320 and 0x321 and the last two are clear.
void AquaRock_TimeStreams(void) {
  if (GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS))
    return;
  if (gAquaRock_FirstStreamTimer <= 149) {
    if (GameFlag_Get(FLAG_AREA_TEMP_20))
      GameFlag_Set(FLAG_SCENE_TEMP_00);
    else
      GameFlag_Clear(FLAG_SCENE_TEMP_00);
  } else {
    GameFlag_Set(FLAG_SCENE_TEMP_00);
  }
  if (++gAquaRock_FirstStreamTimer > 239)
    gAquaRock_FirstStreamTimer = 0;
  if (gAquaRock_SecondStreamTimer <= 149) {
    if (GameFlag_Get(FLAG_AREA_TEMP_21))
      GameFlag_Set(FLAG_SCENE_TEMP_01);
    else
      GameFlag_Clear(FLAG_SCENE_TEMP_01);
  } else {
    GameFlag_Set(FLAG_SCENE_TEMP_01);
  }
  if (++gAquaRock_SecondStreamTimer > 239)
    gAquaRock_SecondStreamTimer = 0;
  if (gAquaRock_ThirdStreamTimer <= 149)
    GameFlag_Clear(FLAG_SCENE_TEMP_02);
  else
    GameFlag_Set(FLAG_SCENE_TEMP_02);
  if (++gAquaRock_ThirdStreamTimer > 239)
    gAquaRock_ThirdStreamTimer = 0;
  if (gAquaRock_FourthStreamTimer <= 149)
    GameFlag_Clear(FLAG_SCENE_TEMP_03);
  else
    GameFlag_Set(FLAG_SCENE_TEMP_03);
  if (++gAquaRock_FourthStreamTimer > 239)
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
  (stream)->waveRow[1] = 9999;                                                 \
  (stream)->waveRow[0] = 9999

/// Sets the five streams with their pictures, cells and flags, opens the first
/// two where the pillar left them open, and starts the streams' tasks.
void AquaRock_SetUpStreams(void) {
  struct AquaRockStream *entry = gAquaRock_Streams;

  STREAM_SET(entry, 64, 74, 76, 74, 70, 10, 6, 74, 36, FLAG_SCENE_TEMP_00);
  entry++;
  STREAM_SET(entry, 70, 74, 82, 74, 78, 10, 14, 74, 36, FLAG_SCENE_TEMP_01);
  entry++;
  STREAM_SET(entry, 90, 93, 102, 93, 103, 26, 39, 90, 15, FLAG_SCENE_TEMP_02);
  entry++;
  STREAM_SET(entry, 96, 93, 108, 93, 107, 26, 43, 90, 15, FLAG_SCENE_TEMP_03);
  entry++;
  STREAM_SET(entry, 116, 95, 122, 95, 96, 32, 32, 96, 13,
             IF_FLAG_SET(FLAG_AQUA_ROCK_TEAR_ROCK_DOUSED));
  if (GameFlag_Get(FLAG_AREA_TEMP_20))
    GameFlag_Set(FLAG_SCENE_TEMP_00);
  else
    GameFlag_Clear(FLAG_SCENE_TEMP_00);
  if (GameFlag_Get(FLAG_AREA_TEMP_21))
    GameFlag_Set(FLAG_SCENE_TEMP_01);
  else
    GameFlag_Clear(FLAG_SCENE_TEMP_01);
  if (GameFlag_Get(FLAG_SCENE_TEMP_00)) {
    struct AquaRockStream *opened = &gAquaRock_Streams[0];
    MapGrid_CopyCellsLayered(opened->pictureSourceX + 3, opened->pictureSourceY,
                             3, opened->height, opened->pictureX,
                             opened->pictureY);
    MapGrid_CopyCells(opened->cellSourceX + 3, opened->cellSourceY, 3,
                      opened->height, opened->cellX, opened->cellY);
    opened->waveRow[1] = 9999;
    opened->waveRow[0] = 9999;
    opened->shown = 1;
  }
  if (GameFlag_Get(FLAG_SCENE_TEMP_01)) {
    struct AquaRockStream *opened = &gAquaRock_Streams[1];
    MapGrid_CopyCellsLayered(opened->pictureSourceX + 3, opened->pictureSourceY,
                             3, opened->height, opened->pictureX,
                             opened->pictureY);
    MapGrid_CopyCells(opened->cellSourceX + 3, opened->cellSourceY, 3,
                      opened->height, opened->cellX, opened->cellY);
    opened->waveRow[1] = 9999;
    opened->waveRow[0] = 9999;
    opened->shown = 1;
  }
  Task_AddFrame(AquaRock_TimeStreams, TASK_PASS_FRAME);
  Task_Add(AquaRock_UpdateStreams, TASK_PASS_FRAME);
}

#include "shared/actor_flicker_child.h"

/// A Moai's update while it flashes: its colour every other two frames.
void AquaRock_FlashMoai(struct Actor *actor) {
  if (gFrameCount & 2)
    Actor_SetSpriteColorMode(actor, 6);
  else
    Actor_SetSpriteColorMode(actor, 0);
}

/// A flashing actor's last update: its sprite's colour back to normal.
void AquaRock_StopFlashing(struct Actor *actor) {
  Actor_SetSpriteColorMode(actor, 0);
  actor->update = 0;
}

void TilesetAnim_Pause(s32 layer);

/// Toggles FLAG_AQUA_ROCK_STREAMS_TURNED_BACK and with it which of two layer
/// pairs is shown: four rounds of a random colour value, then one pair's blend
/// scripts and layer fields set and the other's cleared, while the object
/// blinks.
void AquaRock_DouseWaterStone(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  actor->update = (void (*)(void))Actor_FlickerChild;
  wait(30);
  if (GameFlag_Get(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK) == 0)
    AquaRock_SetSparkleAnchor(ACTOR_WATER_STONE);
  else
    AquaRock_SetSparkleAnchor(0);
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
    TilesetAnim_Pause(5);
    TilesetAnim_Pause(6);
    TilesetAnim_Resume(7);
    TilesetAnim_Resume(8);
    actor->update = (void (*)(void))AquaRock_StopFlashing;
    AquaRock_SetCellMarkerAnchor(ACTOR_WATER_STONE);
  } else {
    GameFlag_Clear(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK);
    MapGrid_CopyCells(65, 64, 1, 2, 51, 82);
    MapGrid_CopyCells(67, 64, 1, 2, 115, 18);
    TilesetAnim_Pause(7);
    TilesetAnim_Pause(8);
    TilesetAnim_Resume(5);
    TilesetAnim_Resume(6);
    actor->update = (void (*)(void))AquaRock_StopFlashing;
    AquaRock_SetCellMarkerAnchor(0);
  }
  Event_Wait(30);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(30);
  Event_Wait(15);
  Event_End();
}

void GameFlag_SetByte(s32 flag, s32 value);

/// Move on a pillar: the first remembers its cell and opens a stream when it
/// stands in one; the second, pushed to column 32, drops before the Moai.
void AquaRock_MovePillarBeforeMoai(s32 unused, s32 id) {

  if (id == ACTOR_FIRST_PILLAR) {
    s32 column;
    s32 row;

    struct Actor *first = Actor_GetById(ACTOR_FIRST_PILLAR);

    column = FIELD_CELL(first->x.fixed);
    row = FIELD_CELL(first->z.fixed);
    GameFlag_SetByte(FLAG_AREA_TEMP_10, column);
    GameFlag_SetByte(FLAG_AREA_TEMP_18, row);
    GameFlag_ClearBit_(FLAG_AREA_TEMP_20);
    GameFlag_Clear(FLAG_AREA_TEMP_21);
    if (column == 7 && row == 35)
      GameFlag_Set(FLAG_AREA_TEMP_20);
    if (column == 15 && row == 35)
      GameFlag_Set(FLAG_AREA_TEMP_21);
  } else if (id == ACTOR_SECOND_PILLAR) {
    struct Actor *actor = Actor_GetById(ACTOR_SECOND_PILLAR);

    if (FIELD_CELL(actor->x.fixed) == 32) {
      wait(30);
      actor->z.fixed += toFIXED(1);
      GameFlag_Set(FLAG_AQUA_ROCK_PILLAR_BEFORE_MOAI);
    }
  }
}

/// Waits, for 60 frames at most, until the actor has risen to `limit`.
void AquaRock_WaitToRiseTo(struct Actor *actor, s32 limit) {
  s32 frames = 60;

  while (frames != 0) {
    wait(1);
    frames--;
    if (actor->y.fixed <= limit)
      break;
  }
}

/// Each frame in a stream turned back: the current carries the leader north
/// and, at its head on the slopes, spins him up and out by exit 6; otherwise
/// the stream flows south.
void AquaRock_RideStreamUp(void) {
  struct Actor *leader, *carried;
  struct Vector3 ahead;
  s32 owner;
  struct FieldWork *work;
  s32 result, i;

  if (GameFlag_Get(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK)) {
    work = Ram_HeapSlots->fieldWork;
    owner = gMainRoutineWork.leaderActorId;
    carried = work->camera;
    leader = Actor_GetById(owner);
    if (gMainRoutineWork.next == SCENE_AQUA_ROCK_MOAI &&
        leader->groundHeight == 0)
      return;
    ahead.x = leader->x.fixed;
    ahead.y = leader->y.fixed;
    ahead.z = leader->z.fixed + toFIXED(2);
    result = MapGrid_CheckCollision(leader, &ahead);
    if ((gFrameCount & 4) == 0)
      AquaRock_SpawnSplash(leader);
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
      AquaRock_SpawnSplash(leader);
      Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
      wait(3);
      return;
    }
    ahead.x = leader->x.fixed;
    ahead.y = leader->y.fixed;
    ahead.z = leader->z.fixed - toFIXED(8);
    result = MapGrid_CheckCollision(leader, &ahead);
    if (result > 0) {
      if (gMainRoutineWork.next != SCENE_AQUA_ROCK_MOAI)
        return;
      Event_Start();
      Event_ResolvePendingActions(0);
      leader->motionFlags = 0;
      Actor_SetSpriteFlags(leader, 0);
      gMainRoutineWork.movementMode = LEADER_MOVEMENT_KEYS;
      AquaRock_SpawnSplash(leader);
      Event_Wait(5);
      for (i = 0; i < 60; i++) {
        leader->y.fixed += toFIXED(2);
        leader->facing += TRIG_TURN / 16;
        if (Random16() <= 4095)
          AquaRock_SpawnSplash(leader);
        wait(1);
      }
      gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
      Event_End();
      Import_EventSetExitRequest(6);
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
    AquaRock_FlowWithStream();
}

/// The attribute byte of a cell of a layer.
static inline u8 CellAttribute(struct MapCell *cells, s32 row, s32 col) {
  cells += row * 128 + col;
  return cells->attributeB;
}

/// Each frame: a leader stepping into a current's cell is carried, or, while
/// moved by others, sent on by trigger 44.
void AquaRock_StepIntoCurrent(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 col;
  s32 row;

  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_KEYS) {
    col = FIELD_CELL(actor->x.fixed);
    row = FIELD_CELL(actor->z.fixed - actor->y.fixed - toFIXED(12));
    if (CellAttribute(map->layers[1].cells, row, col) != 0)
      work->raisedTrigger = 44;
  } else {
    col = FIELD_CELL(actor->x.fixed);
    row = FIELD_CELL(actor->z.fixed - actor->y.fixed);
    if (CellAttribute(map->layers[1].cells, row, col) != 0)
      AquaRock_FlowWithStream();
  }
}

/// Douse on the tear-shaped rock: it fills, the Moai beside it changes colour
/// and face, and the way inside opens.
void AquaRock_DouseTearRock(void) {
  MapGrid_CopyCellAttributes(62, 41, 1, 2, 33, 41);
  MapGrid_CopyCellPictureCellsRect(125, 32, 3, 13, 96, 32);
  MapGrid_CopyCellPictureCellsRect(61, 96, 3, 13, 32, 96);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_MOAI + 4), 5);
  AquaRock_SetMoaiFace(ACTOR_FIRST_MOAI + 4);
  GameFlag_Set(FLAG_AQUA_ROCK_TEAR_ROCK_DOUSED);
}

/// The leader drops down the falls and lands, for at most three seconds.
void AquaRock_DropDownFalls(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 saved = actor->gravity;
  s32 i;
  struct MainRoutineWork *state;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SelectAnimation(actor, 40);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  for (i = 0; i < 180; i++) {
    wait(1);
    if (actor->y.fixed <= actor->groundHeight)
      break;
  }
  state = &gMainRoutineWork;
  actor->gravity = saved;
  FieldEffect_SetAttached(state->leaderActorId, ATTACHED_EFFECT_REMOVE);
  Actor_SetSpriteFlags(actor, 1);
  Actor_SelectAnimation(actor, 1);
  state->movementMode = LEADER_MOVEMENT_WALK;
  Event_End();
}

/// Defines a puddle's freezing: the actor stills, its cell takes (n1, n2)'s
/// attributes at column n3, and flag n4 is set.
#define DEFINE_AQUA_ROCK_SET_MOVED(name, n1, n2, n3, n4)                       \
  void name(s32 unused, s32 id) {                                              \
    struct Actor *actor = Actor_GetById(id);                                   \
                                                                               \
    actor->priorityFlags = 0;                                                  \
    actor->motionFlags = 0;                                                    \
    MapGrid_CopyCellAttributes(n1, n2, 1, 1, n3, n2);                          \
    GameFlag_Set(n4);                                                          \
  }

/// Frost on the Mist Sabre chest's room's first puddle: it freezes into a step.
DEFINE_AQUA_ROCK_SET_MOVED(AquaRock_FreezeFirstPuddle, 42, 32, 44,
                           FLAG_SCENE_TEMP_21)

/// Frost on the second puddle: it freezes into a step.
DEFINE_AQUA_ROCK_SET_MOVED(AquaRock_FreezeSecondPuddle, 48, 32, 46,
                           FLAG_SCENE_TEMP_22)

/// Frost on the third puddle: it freezes into a step.
DEFINE_AQUA_ROCK_SET_MOVED(AquaRock_FreezeThirdPuddle, 44, 44, 46,
                           FLAG_SCENE_TEMP_23)

/// Defines a rock's drop: pushed to column 53 it drops a pixel, flag n1 is set
/// and its cell in row n2 opens.
#define DEFINE_AQUA_ROCK_DROP_AT_COLUMN(name, n1, n2)                          \
  void name(s32 unused, s32 id) {                                              \
    struct Actor *actor = Actor_GetById(id);                                   \
    s32 column = FIELD_CELL(actor->x.fixed);                                   \
                                                                               \
    if (column == 53) {                                                        \
      wait(30);                                                                \
      actor->z.fixed += toFIXED(1);                                            \
      GameFlag_Set(n1);                                                        \
      Map_CopyCellAttributeRect(54, 58, 1, 1, column, n2);                     \
    }                                                                          \
  }

/// Move on the first ember stone to column 53: it drops into the current.
DEFINE_AQUA_ROCK_DROP_AT_COLUMN(AquaRock_DropFirstRock,
                                FLAG_AQUA_ROCK_FIRST_ROCK_DROPPED, 52)

/// Move on the second ember stone to column 53: it drops into the current.
DEFINE_AQUA_ROCK_DROP_AT_COLUMN(AquaRock_DropSecondRock,
                                FLAG_AQUA_ROCK_SECOND_ROCK_DROPPED, 58)

/// Sets FLAG_AQUA_ROCK_DRY_MOAI_DOUSED after four rounds of a random colour
/// value, runs the blend scripts, then moves the camera to actor 15, starts
/// its update and plays cue 623 six times.
void AquaRock_DouseDryMoai(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  actor->update = (void (*)(void))Actor_FlickerChild;
  wait(30);
  AquaRock_SetSparkleAnchor(ACTOR_MIST_SABRE_WATER_STONE);
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
  GameFlag_Set(FLAG_AQUA_ROCK_DRY_MOAI_DOUSED);
  Map_CopyCellAttributeRect(15, 48, 1, 1, 16, 48);
  Map_CopyCellAttributeRect(17, 50, 1, 1, 16, 50);
  actor->update = (void (*)(void))AquaRock_StopFlashing;
  AquaRock_SetCellMarkerAnchor(ACTOR_MIST_SABRE_WATER_STONE);
  MapGrid_CopyCellPictureCellsRect(64, 68, 5, 13, 14, 94);
  Map_CopyCellAttributeRect(64, 68, 5, 8, 14, 34);
  Event_Wait(30);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(30);
  Event_Wait(15);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(248, -32, 760, 1);
  FieldCamera_WaitForArrival();
  Actor_SetPosition(ACTOR_STREAM_SOURCE, 264, 792);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_STREAM_SOURCE), 0);
  Actor_GetById(ACTOR_STREAM_SOURCE)->collisionFlags = 0;
  Actor_GetById(ACTOR_STREAM_SOURCE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_GetById(ACTOR_STREAM_SOURCE)->update =
      (void (*)(void))Actor_EmitWaterDrop;
  Event_Wait(30);
  for (i = 0; i < 6; i++) {
    Sound_Play(SE_WADE);
    wait(10);
  }
  Event_Wait(30);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_AQUA_ROCK_MOAI)
    return gAquaRock_SlopesTriggers;
  if (scene == SCENE_AQUA_ROCK_MIST_SABRE_CHEST)
    return gAquaRock_MistSabreChestTriggers;
  return gAquaRock_Triggers;
}

/// Each frame: the leader's sprite priority, 2 while moved by others
/// (LEADER_MOVEMENT_KEYS) and 1 otherwise.
void AquaRock_KeepLeaderPriority(void) {
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());

  if (actor != 0 && (actor->spriteStorageKind & 15) == 1) {
    struct FieldSprite *sprite;
    s32 priority;

    sprite = actor->sprite;
    priority = gMainRoutineWork.movementMode == LEADER_MOVEMENT_KEYS ? 2 : 1;
    sprite->priority = priority;
    sprite->partPriority = priority;
    actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  }
}

/// Arriving by the stream from below: the leader spins up out of the water and
/// lands, going on north or south by the key held.
void AquaRock_SpinOut(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 saved = actor->motionFlags;
  s32 i;
  s32 angle;
  s32 up;
  struct MainRoutineWork *state;

  Sound_Play(SE_SPIN_AWAY);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  actor->y.fixed = -toFIXED(16);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 2);
  for (i = 0; i < 16; i++) {
    actor->y.fixed += toFIXED(1);
    angle = actor->facing;
    angle += DEGtoANG(45);
    actor->facing = angle;
    wait(1);
  }
  AquaRock_SpawnSplash(actor);
  actor->motionFlags = saved;
  actor->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(actor, 1);
  up = gInput.held & KEY_UP;
  state = &gMainRoutineWork;
  if (up)
    Actor_MoveToAndWaitById(state->leaderActorId, 264, 760);
  else
    Actor_MoveToAndWaitById(state->leaderActorId, 264, 824);
  Event_End();
}

/// Arriving from above: the leader spins down into the water and walks out.
void AquaRock_DropIn(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 saved = actor->motionFlags;
  s32 i;
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  actor->y.fixed = -toFIXED(64);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 2);
  for (i = 0; i < 24; i++) {
    actor->y.fixed += toFIXED(2);
    angle = actor->facing;
    angle += DEGtoANG(45);
    actor->facing = angle;
    wait(1);
  }
  AquaRock_SpawnSplash(actor);
  actor->motionFlags = saved;
  actor->velocityY = toFIXED(12);
  Actor_SetSpriteFlags(actor, 1);
  Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, 680, 912);
  Event_End();
}

/// A stepping Moai's update: it sways, the swing growing to 1/128 turn.
void AquaRock_SwayMoai(struct Actor *actor) {
  struct FieldSprite *sprite = actor->sprite;
  s16 *amplitude;

  actor->timer += DEGtoANG(45);
  amplitude = (s16 *)&actor->userValue;
  *amplitude += 32;
  if (*amplitude > TRIG_TURN / 128)
    *amplitude = TRIG_TURN / 128;
  sprite->rotation = Iwram_MulFX(Trig_Sin(actor->timer), *amplitude);
}

/// A stepping Moai's update as it settles: the swing dies away.
void AquaRock_SwayMoaiBack(struct Actor *actor) {
  struct FieldSprite *sprite = actor->sprite;
  s16 *amplitude;

  actor->timer += DEGtoANG(45);
  amplitude = (s16 *)&actor->userValue;
  *amplitude -= 16;
  if (*amplitude < 0) {
    *amplitude = 0;
  }
  sprite->rotation = Iwram_MulFX(Trig_Sin(actor->timer), *amplitude);
}

/// The last stepping Moai's update: it rocks gently, leaning.
void AquaRock_RockMoai(struct Actor *actor) {
  struct FieldSprite *sprite = actor->sprite;

  actor->timer += TRIG_TURN / 256;
  sprite->rotation = Trig_Sin(actor->timer) / 64 + TRIG_TURN * 3 / 128;
}

/// Scatters objects 17 to 21 under cue 184, lines them up 32 pixels apart
/// at z 824, then settles them under cue 298 and leaves object 21 rocking;
/// sets FLAG_KEEP_PARTY_POSITION and FLAG_AQUA_ROCK_MOAI_STEPPED_ASIDE and
/// clears FLAG_SHOW_AREA_NAME.
void AquaRock_MoaiStepAside(void) {
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 0, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_POP_UP);
  i = ACTOR_FIRST_STEPPING_MOAI;
  do {
    {
      struct Actor *actor = Actor_GetById(i);

      actor->speedLimit = toFIXED(1) / 4;
      actor->acceleration = toFIXED(1) / 64;
      actor->update = (void (*)(void))AquaRock_SwayMoai;
      actor->timer = Random16();
      actor->userValue = 0;
    }
  } while (++i <= ACTOR_FIRST_STEPPING_MOAI + 4);
  Event_Wait(60);
  Map_CopyCellsRect(64, 69, 11, 4, 87, 53);
  Actor_SetDestination(ACTOR_FIRST_STEPPING_MOAI, 392, 824);
  Actor_SetDestination(ACTOR_FIRST_STEPPING_MOAI + 1, 424, 824);
  Actor_SetDestination(ACTOR_FIRST_STEPPING_MOAI + 2, 456, 824);
  Actor_SetDestination(ACTOR_FIRST_STEPPING_MOAI + 3, 488, 824);
  Actor_SetDestinationById(ACTOR_FIRST_STEPPING_MOAI + 4, 520, 824);
  Event_Wait(240);
  Map_CopyCellsRect(64, 66, 11, 7, 87, 50);
  i = ACTOR_FIRST_STEPPING_MOAI;
  do {
    Actor_GetById(i)->update = (void (*)(void))AquaRock_SwayMoaiBack;
  } while (++i <= ACTOR_FIRST_STEPPING_MOAI + 4);
  Sound_Play(SE_STOP_EXIT_SE);
  Event_Wait(60);
  {
    struct Actor *actor = Actor_GetById(ACTOR_FIRST_STEPPING_MOAI + 4);

    actor->update = (void (*)(void))AquaRock_RockMoai;
    actor->timer = 0;
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  Import_EventSetExitRequest(77);
  Event_End();
  GameFlag_Set(FLAG_AQUA_ROCK_MOAI_STEPPED_ASIDE);
}

s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.next;
  struct MapWorkGroundEffects *map =
      (struct MapWorkGroundEffects *)Ram_HeapSlots->mapGrid;
  s32 id, x, z;
  s16 *entrance;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  gMainRoutineWork.retreatScene = SCENE_AQUA_ROCK_MOAI;
  gMainRoutineWork.retreatEntrance = 1;
  map->wading = MAP_WADING_UPPER_LEVEL;
  Scene_QueueMusic(SE_AMBIENT_WATER);
  if (scene == SCENE_AQUA_ROCK_MOAI) {
    Actor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_MOAI), 4);
    Actor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_MOAI + 1), 4);
    Actor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_MOAI + 2), 4);
    Actor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_MOAI + 3), 4);
    if (GameFlag_Get(FLAG_AQUA_ROCK_TEAR_ROCK_DOUSED))
      Actor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_MOAI + 4), 5);
    else
      Actor_SetSpriteColor(Actor_GetById(ACTOR_FIRST_MOAI + 4), 6);
    AquaRock_SetMoaiFace(ACTOR_FIRST_MOAI);
    AquaRock_SetMoaiFace(ACTOR_FIRST_MOAI + 1);
    AquaRock_SetMoaiFace(ACTOR_FIRST_MOAI + 2);
    AquaRock_SetMoaiFace(ACTOR_FIRST_MOAI + 3);
    AquaRock_SetMoaiFace(ACTOR_FIRST_MOAI + 5);
    AquaRock_SetMoaiFace(ACTOR_FIRST_MOAI + 6);
    for (id = ACTOR_DRY_MOAI; id <= ACTOR_LAST_MOAI; id++)
      AquaRock_SetMoaiFace(id);
    for (id = ACTOR_FIRST_MOAI; id < ACTOR_DRY_MOAI; id++)
      Actor_SetSpritePriority(id, 3);
    for (id = ACTOR_DRY_MOAI; id <= ACTOR_LAST_MOAI; id++)
      Actor_SetSpritePriority(id, 1);
    Actor_GetById(ACTOR_DRY_MOAI)->retainSpriteCache = 2;
    Actor_SetAnimationById(ACTOR_DRY_MOAI, 3);
    for (id = ACTOR_DRY_MOAI + 1; id <= ACTOR_LAST_MOAI; id++) {
      Actor_SetAnimationById(id, 1);
      Actor_SetAnimationById(id, 0);
    }
    if (GameFlag_Get(FLAG_AQUA_ROCK_PILLAR_BEFORE_MOAI))
      Actor_SetPosition(ACTOR_SECOND_PILLAR, 520, 584);
    x = GameFlag_GetByte(FLAG_AREA_TEMP_10);
    z = GameFlag_GetByte(FLAG_AREA_TEMP_18);
    if (z != 0)
      Actor_SetHorizontalPositionWithTerrain(
          ACTOR_FIRST_PILLAR, (x << 20) + toFIXED(8), (z << 20) + toFIXED(8));
    Actor_GetById(Party_GetLeaderActor())->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
    Task_Add(AquaRock_KeepLeaderPriority, TASK_PASS_FRAME);
    Actor_SetSpritePriority(ACTOR_FIRST_DRIFTER + 1, 1);
    Value2((s32 (*)())Actor_SetSpritePriority, ACTOR_FIRST_PILLAR, 1);
    AquaRock_StartCellMarkers();
    AquaRock_StartSparkles();
    if (GameFlag_Get(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK)) {
      AquaRock_SetCellMarkerAnchor(ACTOR_WATER_STONE);
      AquaRock_SetSparkleAnchor(ACTOR_WATER_STONE);
      TilesetAnim_Pause(5);
      TilesetAnim_Pause(6);
      TilesetAnim_Resume(7);
      TilesetAnim_Resume(8);
      Actor_GetById(ACTOR_WATER_STONE)->update = AquaRock_StopFlashing;
    }
    Actor_SetSpritePriority(ACTOR_WATER_STONE, 1);
    if (gMainRoutineWork.entrance == 5)
      AquaRock_MoaiStepAside();
    if (GameFlag_Get(FLAG_AQUA_ROCK_MOAI_STEPPED_ASIDE)) {
      Actor_SetPosition(ACTOR_FIRST_STEPPING_MOAI, 392, 824);
      Actor_SetPosition(ACTOR_FIRST_STEPPING_MOAI + 1, 424, 824);
      Actor_SetPosition(ACTOR_FIRST_STEPPING_MOAI + 2, 456, 824);
      Actor_SetPosition(ACTOR_FIRST_STEPPING_MOAI + 3, 488, 824);
      Actor_SetPosition(ACTOR_FIRST_STEPPING_MOAI + 4, 520, 824);
      MapGrid_CopyCellAttributes(64, 66, 9, 3, 24, 53);
      MapGrid_CopyCells(64, 66, 11, 7, 87, 50);
      Actor_GetById(ACTOR_FIRST_STEPPING_MOAI + 4)->update = AquaRock_RockMoai;
    }
    if (GameFlag_Get(FLAG_AQUA_ROCK_TEAR_ROCK_DOUSED)) {
      MapGrid_CopyCellAttributes(62, 41, 1, 2, 33, 41);
      MapGrid_CopyCellPictureCellsRect(116, 95, 3, 13, 96, 32);
      MapGrid_CopyCellsLayered(122, 95, 3, 13, 32, 96);
      AquaRock_SetMoaiFace(ACTOR_FIRST_MOAI + 4);
    }
    AquaRock_SetUpStreams();
    if (gMainRoutineWork.entrance == 4)
      AquaRock_ArriveFromPassage();
  } else if (scene == SCENE_AQUA_ROCK_MIST_SABRE_CHEST) {
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_BUBBLE_MARKER), 0);
    if (GameFlag_Get(FLAG_SCENE_TEMP_21))
      MapGrid_CopyCellAttributes(42, 32, 1, 1, 44, 32);
    if (GameFlag_Get(FLAG_SCENE_TEMP_22))
      MapGrid_CopyCellAttributes(48, 32, 1, 1, 46, 32);
    if (GameFlag_Get(FLAG_SCENE_TEMP_23))
      MapGrid_CopyCellAttributes(44, 44, 1, 1, 46, 44);
    Actor_SetAnimationById(ACTOR_FIRST_EMBER_STONE, 2);
    if (GameFlag_Get(FLAG_AQUA_ROCK_FIRST_ROCK_DROPPED)) {
      Actor_SetPosition(ACTOR_FIRST_EMBER_STONE, 856, 840);
      MapGrid_CopyCellAttributes(54, 58, 1, 1, 53, 52);
    }
    Actor_SetAnimationById(ACTOR_SECOND_EMBER_STONE, 2);
    if (GameFlag_Get(FLAG_AQUA_ROCK_SECOND_ROCK_DROPPED)) {
      Actor_SetPosition(ACTOR_SECOND_EMBER_STONE, 856, 936);
      MapGrid_CopyCellAttributes(54, 58, 1, 1, 53, 58);
    }
    if (gMainRoutineWork.entrance == 5 || gMainRoutineWork.entrance == 7) {
      AquaRock_StartCellMarkers();
      AquaRock_StartSparkles();
      if (GameFlag_Get(FLAG_AQUA_ROCK_STREAMS_TURNED_BACK)) {
        AquaRock_SetCellMarkerAnchor(ACTOR_BUBBLE_MARKER);
        AquaRock_SetSparkleAnchor(ACTOR_BUBBLE_MARKER);
      } else {
        TilesetAnim_Pause(7);
        TilesetAnim_Pause(8);
        TilesetAnim_Resume(5);
        TilesetAnim_Resume(6);
      }
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_BUBBLE_MARKER), 0);
    } else {
      AquaRock_StartCellMarkers();
      AquaRock_StartSparkles();
      Actor_GetById(ACTOR_MIST_SABRE_WATER_STONE)->update =
          AquaRock_StopFlashing;
      if (GameFlag_Get(FLAG_AQUA_ROCK_DRY_MOAI_DOUSED)) {
        AquaRock_SetCellMarkerAnchor(ACTOR_MIST_SABRE_WATER_STONE);
        AquaRock_SetSparkleAnchor(ACTOR_MIST_SABRE_WATER_STONE);
        Actor_SetPosition(ACTOR_STREAM_SOURCE, 264, 792);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_STREAM_SOURCE), 0);
        Actor_GetById(ACTOR_STREAM_SOURCE)->collisionFlags = 0;
        Actor_GetById(ACTOR_STREAM_SOURCE)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Actor_GetById(ACTOR_STREAM_SOURCE)->update = Actor_EmitWaterDrop;
        Map_CopyCellAttributeRect(15, 48, 1, 1, 16, 48);
        Map_CopyCellAttributeRect(17, 50, 1, 1, 16, 50);
        MapGrid_CopyCellPictureCellsRect(64, 68, 5, 13, 14, 94);
        Map_CopyCellAttributeRect(64, 68, 5, 8, 14, 34);
      }
    }
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      if (gMainRoutineWork.entrance == 1)
        AquaRock_SpinOut();
      entrance = &gMainRoutineWork.entrance;
      if (*entrance == 6)
        AquaRock_DropIn();
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// A splash at the actor's feet, on its sprite's priority.
void AquaRock_SpawnSplash(struct Actor *actor) {
  struct Actor *child =
      Import_ActorCreate(14, actor->x.fixed, actor->y.fixed + toFIXED(4),
                         actor->z.fixed + toFIXED(4));
  struct FieldSprite *parentSprite = actor->sprite;

  if (child != 0) {
    struct FieldSprite *sprite = child->sprite;

    child->groundHeight = actor->groundHeight;
    Actor_StartScript(child, gAquaRock_SplashScript);
    child->motionFlags = 0;
    if (sprite != 0) {
      Sprite_StartAnimation(sprite, 1);
      sprite->flags = 0;
      sprite->priority = parentSprite->priority;
    }
  }
}

#include "shared/actor_emit_water_drop.h"

/// Each frame in a stream: the current carries the leader south, splashing, and
/// off a ledge he drops with the water.
void AquaRock_FlowWithStream(void) {
  struct Actor *anchor;
  s32 owner;
  struct Actor *leader;
  FIXED position[3];
  s32 result;

  {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;

    owner = gMainRoutineWork.leaderActorId;
    anchor = work->camera;
  }
  leader = Actor_GetById(owner);
  if (gMainRoutineWork.next == SCENE_AQUA_ROCK_MOAI &&
      leader->groundHeight == 0)
    return;
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed + toFIXED(2);
  result = MapGrid_CheckCollision(leader, position);
  if ((gFrameCount & 4) == 0)
    AquaRock_SpawnSplash(leader);
  if (result < 0) {
    Actor_SetAttachedEffect(owner,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetMoveTarget(leader, leader->x.fixed, leader->y.fixed,
                        leader->z.fixed + toFIXED(8));
    Actor_SelectAnimation(leader, 49);
    Actor_WaitForMove(leader);
    do {
      wait(1);
    } while (leader->y.fixed != leader->groundHeight);
    AquaRock_SpawnSplash(leader);
    Actor_SelectAnimation(leader, 49);
    wait(3);
    return;
  }
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed + toFIXED(8);
  result = MapGrid_CheckCollision(leader, position);
  if (result > 0)
    return;
  position[0] = leader->x.fixed + (toFIXED(57) / 10);
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed + (toFIXED(57) / 10);
  result = MapGrid_CheckCollision(leader, position);
  if (result > 0)
    return;
  position[0] = leader->x.fixed - (toFIXED(57) / 10);
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed + (toFIXED(57) / 10);
  result = MapGrid_CheckCollision(leader, position);
  if (result > 0)
    return;
  anchor->z.fixed += toFIXED(2);
  leader->z.fixed += toFIXED(2);
  leader->facing = FACING_SOUTH;
}

/// Gives a Moai its face: the face animation added to its sprite, its first
/// frame shown, then animation 2 started.
void AquaRock_SetMoaiFace(s32 id) {
  struct Actor *actor = Actor_GetById(id);

  if (actor != 0) {
    u8 *meta = Sprite_AddAnimation(actor->sprite, 381);

    meta[5] = 0;
    meta[6] = 0;
    Actor_SelectAnimation(actor, 0);
    Actor_SelectAnimation(actor, 2);
  }
}

/// The leader spins up out of the water and away, to `exit`.
void AquaRock_SpinUpAndAway(s32 exit) {
  struct Actor *actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 i;
  s32 angle;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  actor->motionFlags = 0;
  actor->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Sound_Play(SE_SPLASH_SMALL);
  Actor_SelectAnimation(actor, 18);
  Sound_Play(SE_SPIN_AWAY);
  for (i = 0; i < 60; i++) {
    if (i == 30)
      Event_FinishScreenTransition();
    actor->y.fixed -= toFIXED(1) / 4;
    angle = actor->facing;
    angle += TRIG_TURN / 16;
    actor->facing = angle;
    if ((i & 7) == 0)
      AquaRock_SpawnSplash(Actor_GetById(ACTOR_STREAM_SOURCE));
    wait(1);
  }
  Event_End();
  Import_EventSetExitRequest(exit);
}

// Every other frame, emits particle 7 (5 on every other such frame) from the
// object, 26 pixels up, swaying with the frame count and drifting by a random
// amount.
#include "shared/actor_emit_mist.h"

/// The leader walks into the sparkling passage in a mist and leaves by
/// `entrance`.
void AquaRock_EnterSparklingPassage(s32 entrance) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 *owner = &gMainRoutineWork.leaderActorId;

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_SPARKLING_PASSAGE);
  leader->update = (void (*)(void))Actor_EmitMist;
  leader->motionFlags = 0;
  leader->speedLimit = toFIXED(1) / 5;
  Actor_SetAnimationById(*owner, 2);
  Actor_SetDestinationOffsetById(*owner, 0, -8);
  Actor_WaitForMoveById(*owner);
  Actor_SetSpriteColor(Actor_GetById(*owner), 9);
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
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 flag = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
  s32 *owner = &gMainRoutineWork.leaderActorId;
  s32 offset;

  if (flag == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    leader->motionFlags = flag;
    Actor_SetHorizontalPositionWithTerrain(
        *owner, toFIXED(leader->x.part[0].integer),
        toFIXED(leader->z.part[0].integer) - toFIXED(16));
    Actor_SetSpriteColor(Actor_GetById(*owner), 9);
    Actor_SetSpriteFlags(Actor_GetById(*owner), 0);
    Event_StartScreenTransition();
    Sound_Play(SE_SPARKLING_PASSAGE);
    leader->update = (void (*)(void))Actor_EmitMist;
    Actor_SetSpeed(*owner, 0.4);
    Actor_WalkByAndWaitById(*owner, 0, 8);
    Actor_SetSpriteColor(Actor_GetById(*owner), 0);
    Actor_SetSpriteFlags(Actor_GetById(*owner), 1);
    ((struct FieldSprite *)leader->sprite)->priority = 1;
    offset = 10;
    Actor_WalkByAndWaitById(*owner, 0, offset);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    leader->update = (void (*)(void))flag;
    Event_NoOp();
    Event_WaitTransitionFrames();
    Event_End();
  }
}

/// Each frame: bubbles rise from the anchor actor now and then, and those in
/// view are drawn.
void AquaRock_DrawRisingCellMarkers(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
#ifndef UBFIX
  s32 *view = &map->viewX;
#endif
  // UB: viewZ is a separate member, not a second element of viewX (as the
  // members, 69 of 169 instructions differ in tla-en).
#ifdef UBFIX
  s32 viewX = map->viewX & FIELD_PIXEL_MASK;
  s32 viewZ = map->viewZ & FIELD_PIXEL_MASK;
#else
  s32 viewX = view[0] & FIELD_PIXEL_MASK;
  s32 viewZ = view[1] & FIELD_PIXEL_MASK;
#endif
  s32 lift = map->origin[1];
  struct AquaRockWork *work = &gAquaRock_CellMarkers;
  u32 tile = gVramCacheEntries[work->vramEntry].offset >> 5;
  struct Actor *source;
  struct Bubble *entry;
  s32 i, x, y, z, screenX, screenY;

  if (Random16() < 4096 && (source = work->object) != 0) {
    entry = &work->bubbles[work->next];
    entry->x = source->x.fixed + Random16() * 3 * 16 - toFIXED(24);
    entry->y = source->y.fixed;
    entry->z = source->z.fixed;
    entry->age = 0;
    if (++work->next > 63)
      work->next = 0;
  }
  entry = work->bubbles;
  for (i = 63; i >= 0; i--, entry++) {
    if (entry->x != 0) {
      // screenX holds the ground's height here, before its own value.
      screenX = Map_GetTerrainHeight(0, entry->x, entry->z);
      if (screenX > entry->y)
        entry->y += toFIXED(2);
      else
        entry->z -= 2 << 16;
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
      y >>= 16;
      screenX = x >> 16;
      x = screenX - 4;
      screenY >>= 16;
      screenY -= 4;
      z = y + 58;
      // screenX now holds its offset into the range test (on screen from
      // -11 to 323).
      screenX += 11;
      if ((u32)screenX <= 334 && screenY > -16 && screenY <= 239) {
        x &= 0x1ff;
        screenY &= 255;
        entry->oam[0] = 0;
        entry->oam[1] = screenY | (x << 16) | 0x2000;
        entry->oam[2] = (tile + (entry->age & 3) * 2) | OBJ_PRIORITY(1);
        Oam_AddSpriteAtDepth(entry->oam, z);
      }
    }
  }
}

/// Each frame: drops fall from the anchor actor now and then, and those in view
/// are drawn.
void AquaRock_DrawFallingCellMarkers(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & FIELD_PIXEL_MASK;
  s32 viewZ = view[1] & FIELD_PIXEL_MASK;
  s32 lift = map->origin[1];
  struct AquaRockWork *work = &gAquaRock_CellMarkers;
  u32 tile = gVramCacheEntries[work->vramEntry].offset >> 5;
  struct Actor *source;
  struct Bubble *bubble;
  s32 i, x, y, depth, screenX, screenY, z, row;

  if (Random16() < 4096 && (source = work->object) != 0) {
    bubble = &work->bubbles[work->next];
    bubble->x = source->x.fixed + Random16() * 3 * 8 - toFIXED(12);
    bubble->y = source->y.fixed;
    bubble->z = source->z.fixed;
    bubble->age = 0;
    if (++work->next > 63)
      work->next = 0;
  }
  bubble = work->bubbles;
  for (i = 63; i >= 0; i--, bubble++) {
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
      screenX = x >> 16;
      y += z;
      x = screenX - 4;
      screenY = (row >> 16) - 4;
      y >>= 16;
      depth = y + 58;
      if ((u32)(screenX + 11) <= 334 && screenY > -16 && screenY <= 239) {
        x &= 0x1ff;
        screenY &= 255;
        bubble->oam[0] = 0;
        bubble->oam[1] = screenY | (x << 16) | 0x2000;
        bubble->oam[2] = (tile + (bubble->age & 3) * 2) | OBJ_PRIORITY(1);
        Oam_AddSpriteAtDepth(bubble->oam, depth);
      }
    }
  }
}

/// Each frame: the bubbles rise or the drops fall, by the markers' direction.
void AquaRock_DrawCellMarkers(void) {
  if (gAquaRock_CellMarkers.direction > 0)
    AquaRock_DrawRisingCellMarkers();
  else
    AquaRock_DrawFallingCellMarkers();
}

/// Clears the cell markers, loads their tiles into VRAM and starts their
/// drawing.
void AquaRock_StartCellMarkers(void) {
  struct AquaRockWork *work = &gAquaRock_CellMarkers;
  void *buffer;

  Iwram_ClearWordsEntry(work, sizeof(struct AquaRockWork));
  buffer = MemAlloc_(256);
  Decompress_TaggedLz(gAquaRock_MarkerTiles, buffer);
  work->vramEntry = VramCache_FindFreeEntry();
  VramCache_Load(work->vramEntry, 256, buffer);
  Heap_Release(buffer);
  ((s32 (*)(void *, s32))Task_Add)(AquaRock_DrawCellMarkers, TASK_PASS_FRAME);
}

/// Sets the actor the cell markers come from, or with 0 none: drops fall from
/// actor 8, and bubbles rise from any other.
void AquaRock_SetCellMarkerAnchor(s32 id) {
  struct AquaRockWork *work = &gAquaRock_CellMarkers;

  if (id == 0) {
    work->object = 0;
  } else {
    work->object = Actor_GetById(id);
    if (id == 8)
      work->direction = -1;
    else
      work->direction = 1;
  }
}

#include "shared/ground_particles_draw.h"

/// Starts the sparkles: 64 sprites on the ground at the origin, their tiles
/// decoded into VRAM, and GroundParticles_Draw scheduled in TASK_PASS_FRAME to
/// move them; the overlay's copy of FieldEffect_InitializeSparkles.
void AquaRock_StartSparkles(void) {
  struct GroundParticleWork *work =
      HeapSlot_AllocEwram(HEAP_SLOT_WEATHER, sizeof(struct GroundParticleWork));
  struct GroundParticle *p = work->particles;
  u8 *buf;
  u32 i;
  u32 size;
  volatile u32 zero;

  size = 0;
  zero = size;
  Dma_Set((const void *)&zero, work,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
              sizeof(struct GroundParticleWork) / 4,
          REG_DMA3);
  buf = HeapSlot_AllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
  Decompress_ByteLz(gField_GroundTiles, buf);
  work->entry = VramCache_FindFreeEntry();
  work->tileBase = VramCache_Load(work->entry, 768, buf);
  HeapSlot_Release(HEAP_SLOT_TILE_SCRATCH);
  work->leader = (s32 *)size;
  i = 0;
  do {
    size = (u32)p;
    *(u32 *)size = 0;
    size += 4;
    *(u32 *)size = OBJ_SIZE_16X16 | OBJ_MODE(1);
    size += 4;
    *(u32 *)size = OBJ_PRIORITY(1) | OBJ_PALETTE(11);
    p->posX = 0;
    p->posZ = 0;
    p->posY = toFIXED(Map_GetTerrainHeight(0, 0, 0));
    p->timer = (i & 15) + 1;
    i++;
    p++;
  } while (i < 64);
  Task_Add(GroundParticles_Draw, TASK_PASS_FRAME);
}

/// Ties the sparkles to the actor `id`, or to none.
void AquaRock_SetSparkleAnchor(s32 id) {
  struct GroundParticleWork *work = Ram_HeapSlots->weather;
  struct Actor *actor = Actor_GetById(id);

  work->leader = NULL;
  if (id != 0 && actor != 0)
    work->leader = &actor->x.fixed;
}

// clang-format off
#include "shared/field_effect_duration_scripts.c"

/// A drifter's script: it moves to random spots, resting between.
s32 gAquaRock_DriftScript[] = {
    SC_CALL((s32)AquaRock_StartDrifter),
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

/// A drifter's script: it moves to random spots, resting longer between.
s32 gAquaRock_SlowDriftScript[] = {
    SC_CALL((s32)AquaRock_StartDrifter),
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
u32 gAquaRock_SplashScript[] = {
    SC_WAIT(20),
    SC_RELEASE,
};

/// A water drop's script: it rises a quarter of a pixel a frame for 31 frames,
/// then is released.
u32 gWaterDropScript[] = {
    SC_ADD(SW_Y, toFIXED(1) / 4),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_RELEASE,
};
// clang-format on

#undef DEFINE_AQUA_ROCK_SET_MOVED
#undef DEFINE_AQUA_ROCK_DROP_AT_COLUMN
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY

// clang-format off
LATE_DATA s16 gGroundParticleFrames[] = {                                                               
      -8,  -12,  20,  0,   1,    -8,  -12,  20,  0,    1,   -8,   -12,         
      20,  0,    1,   -8,  -12,  16,  0,    1,   -8,   -12, 16,   0,           
      1,   -8,   -12, 12,  0,    1,   -8,   -12, 8,    0,   1,    0,           
      -32, 0,    2,   2,   8,    -48, 0,    2,   2,    16,  -64,  0,           
      2,   2,    24,  -80, 0,    2,   2,    32,  -96,  0,   2,    2,           
      40,  -112, 0,   2,   2,    48,  -128, 0,   2,    2,   56,   -144,        
      0,   2,    2,   64,  -160, 0,   2,    2,   -2,   12,  -4,   12,          
      -6,  12,   -8,  8,   -10,  8,   -12,  8,   -14,  8,   -16,  8,           
      -19, 4,    -21, 4,   -24,  4,   -27,  4,   -30,  4,   -33,  4,           
      -36, 4,    -40, 4,   -44,  4,   -48,  0,   -52,  0,   -56,  0,           
      -60, 0,    -64, 0,   -68,  0,   -72,  0,   -76,  0,   -80,  0,           
      -85, 0,    -90, 0,   -95,  0,   -100, 0,   -110, 0,   -120, 0,           
  };

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

/// The slopes' exit regions.
LATE_DATA s16 gAquaRock_SlopesRegions[] = {
    176, 424, 544, 192, 440, 560, -1, 2,
    272, 424, 544, 288, 440, 560, -1, 3,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

/// The Mist Sabre chest's room's exit regions.
LATE_DATA s16 gAquaRock_MistSabreChestRegions[] = {
    48, -56, 960, 64, -40, 976, -1, 2,
    144, -56, 960, 160, -40, 976, -1, 3,
    236, -108, 956, 260, -84, 980, -1, 7,
    268, -108, 956, 292, -84, 980, -1, 7,
    -1, 0, 0, 0, 0, 0, 0, 0,
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
    ACTOR_PLACEMENT(401, CONDITION_ALWAYS, (s32)gAquaRock_DriftScript, 376, 0, 960, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(401, CONDITION_ALWAYS, (s32)gAquaRock_DriftScript, 136, 0, 904, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(401, CONDITION_ALWAYS, (s32)gAquaRock_SlowDriftScript, 144, 0, 912, FACING_SOUTH, TALK_FACING_KEEP, 0),
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

/// The first timed stream's frame count, which AquaRock_TimeStreams turns on and
/// off.
LATE_DATA s32 gAquaRock_FirstStreamTimer = 0;

/// The second timed stream's frame count.
LATE_DATA s32 gAquaRock_SecondStreamTimer = 0;

/// The third timed stream's frame count.
LATE_DATA s32 gAquaRock_ThirdStreamTimer = 0;

/// The fourth timed stream's frame count.
LATE_DATA s32 gAquaRock_FourthStreamTimer = 0;

/// The scene's other rooms' triggers.
LATE_DATA struct Trigger gAquaRock_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// The slopes' exits, streams, Move on the pillars and Douse on the Water Stone,
/// the dry Moai and the tear-shaped rock.
LATE_DATA struct Trigger gAquaRock_SlopesTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_FACING(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(AquaRock_EnterSparklingPassage)},
    {TRIGGER_STEP, 30, FLAG_AREA_TEMP_20, EVENT(AquaRock_StepIntoCurrent)},
    {TRIGGER_STEP, 31, FLAG_AREA_TEMP_21, EVENT(AquaRock_StepIntoCurrent)},
    {TRIGGER_STEP, 32, CONDITION_ALWAYS, EVENT(AquaRock_StepIntoCurrent)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(AquaRock_StepIntoCurrent)},
    {TRIGGER_STEP, 40, CONDITION_ALWAYS, EVENT(AquaRock_FlowWithStream)},
    {TRIGGER_STEP, 41, CONDITION_ALWAYS, EVENT(AquaRock_RideStreamUp)},
    {TRIGGER_RAISED, 44, CONDITION_ALWAYS, EVENT(AquaRock_DropDownFalls)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 12, FLAG_AQUA_ROCK_TEAR_ROCK_DOUSED, EVENT(AquaRock_DouseTearRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_WATER_STONE, FLAG_AQUA_ROCK_STREAMS_TURNED_BACK, EVENT(AquaRock_DouseWaterStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, CONDITION_ALWAYS, EVENT(AquaRock_MovePillarBeforeMoai)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR, FLAG_AQUA_ROCK_PILLAR_BEFORE_MOAI, EVENT(AquaRock_MovePillarBeforeMoai)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AquaRock_MovePillarBeforeMoai)},
    {TRIGGER_TABLE_END},
};

/// The Mist Sabre chest's room's exits, streams, puddles to Frost and rocks to
/// drop.
LATE_DATA struct Trigger gAquaRock_MistSabreChestTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(AquaRock_SpinUpAndAway)},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 40, CONDITION_ALWAYS, EVENT(AquaRock_FlowWithStream)},
    {TRIGGER_STEP, 41, CONDITION_ALWAYS, EVENT(AquaRock_RideStreamUp)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_A, FLAG_SCENE_TEMP_21, EVENT(AquaRock_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_B, FLAG_SCENE_TEMP_22, EVENT(AquaRock_FreezeSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_C, FLAG_SCENE_TEMP_23, EVENT(AquaRock_FreezeThirdPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_EMBER_STONE, FLAG_AQUA_ROCK_FIRST_ROCK_DROPPED, EVENT(AquaRock_DropFirstRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_EMBER_STONE, FLAG_AQUA_ROCK_SECOND_ROCK_DROPPED, EVENT(AquaRock_DropSecondRock)},
    {TRIGGER_PUSHED, 0, FLAG_AQUA_ROCK_SECOND_ROCK_DROPPED, EVENT(AquaRock_DropSecondRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_MIST_SABRE_WATER_STONE, FLAG_AQUA_ROCK_DRY_MOAI_DOUSED, EVENT(AquaRock_DouseDryMoai)},
    {TRIGGER_TABLE_END},
};

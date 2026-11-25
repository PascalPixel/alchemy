/// @file
/// The Northern Reaches, the icy sea where a great wall of ice blocks the
/// ship's way north to Prox.
///
/// Sailing up to the wall before the ship carries the cannon from Loho, the
/// crew calls Kraden to see it. With the cannon aboard, Kraden says to use it
/// and to take the ship between the two blocks of ice before the wall for
/// shelter. There, Felix fires the Magma Ball, the wall bursts in fire
/// and is gone, and the ship sails off north.

#include "shared/tables.h"
#include "field/field_sprite.h"
#include "graphics/palette_buffers.h"
#include "sprite_ids.h"
#include "flags.h"
#include "system/io_reg.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/actor.h"
#include "field/actor_view.h"
#include "system/main_routine_state.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "field/event_inline.h"
#include "system/dma.h"
#include "gamepak/far_call/system.h"
#include "window/item_icon.h"
#include "scene/scene.h"
#include "graphics/render_input.h"
#include "field/effect_sprite.h"
#include "graphics/resource.h"
#include "text/msg_ids.h"
#include "field/actor_script.h"

/// The Magma Ball has shattered the ice wall.
#define FLAG_ICE_WALL_SHATTERED 0x90e

/// Piers has said the ship is at a safe distance.
#define FLAG_SAFE_DISTANCE_TOLD 0x90d

/// The crew has called Kraden to see the ice wall, for this visit.
#define FLAG_ICE_WALL_SIGHTED FLAG_SCENE_TEMP_00

/// The crew has talked the ice wall over.
#define FLAG_ICE_WALL_DISCUSSED 0x90c

/// The cells of the sea before the ice wall that start its events: a row
/// close under the wall, (90, 11) to (96, 11); the gap between the two blocks
/// of ice, (92, 13) and (93, 13); and a row further south, (89, 15) to
/// (91, 15).
enum {
  CELL_UNDER_ICE_WALL = 22,
  CELL_BETWEEN_ICE_BLOCKS = 21,
  CELL_SOUTH_OF_ICE_WALL = 20
};

/// Ways in that no exit reaches.
enum {
  /// Unused: the party below the ice wall, which still stands.
  ENTRANCE_BELOW_ICE_WALL = 2,
  /// Unused: the party below the ice wall, drawn already gone.
  ENTRANCE_ICE_WALL_GONE = 3,
  /// Unused: loads the cannon aboard, hands the ship to the player and enters
  /// again by the north way in.
  ENTRANCE_ABOARD_SHIP_WITH_CANNON = 90
};

/// The Northern Reaches, by the rows of `gActorPlacements`; the rows for
/// Jenna, Sheba and Piers go to their party slots.
enum NorthernReachesActorId {
  /// The party's ship, placed blank; it sails off north once the wall is gone.
  ACTOR_SHIP = ACTOR_FIRST_PLACED,
  ACTOR_MAGMA_BALL_SHOT, ///< the Magma Ball fired at the ice wall
  ACTOR_CANNON_SMOKE,    ///< a puff of smoke at the bow as the cannon fires
  ACTOR_KRADEN
};

void NorthernReaches_ShatterIceWall(FIXED *blast, FIXED *position,
                                    void (*callback)(void));

void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      s32 *position);

extern s32 gNorthernReaches_ShipAtRest[];

void RisingParticles_Start(void);

/// One of the ice wall's shards: its place and speed in 16.16 pixels, as a
/// struct Particle has them, and how many frames it has flown, counted
/// unsigned.
struct IceShard {
  FIXED x;
  FIXED y;
  FIXED z;
  FIXED velocityX;
  FIXED velocityY;
  FIXED velocityZ;
  u32 age;
};

/// One of the sparks the Magma Ball's explosion throws over the wall: the link
/// the sprite list threads it by, its sprite, and its age in frames, negative
/// while it waits to appear.
struct ExplosionSpark {
  struct ExplosionSpark *next;
  union SparkOam oam;
  s32 age;
};

/// The wall breaks into 54 shards, nine columns of six, and the explosion's
/// sparks (`EXPLOSION_SPARK_COUNT`) fly about it.
#define ICE_SHARD_COUNT 54
/// How long a shard flies, in frames.
#define ICE_SHARD_FRAMES 60
/// The buffer a sprite sheet is unpacked into.
#define SHEET_BUFFER_SIZE 4096

/// The shattering's state: the shards' sprite sheet, and each shard's sprite
/// and motion.
struct IceWallState {
  u8 sheet[SHEET_BUFFER_SIZE];
  struct EffectSprite sprites[ICE_SHARD_COUNT];
  struct IceShard shards[ICE_SHARD_COUNT];
};

#include "shared/ship_drop_shadow.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Draws the ice wall gone, as the Magma Ball leaves it and on every later
/// visit.
void NorthernReaches_ClearIceWall(void) {
  MapGrid_CopyCellsLayered(47, 9, 9, 4, 25, 9);
  MapGrid_CopyCellsLayered(47, 69, 9, 10, 25, 69);
  MapGrid_CopyCellAttributes(111, 8, 9, 5, 89, 8);
}

/// The ship sails up to the ice wall, cells (90, 11) to (96, 11), before it
/// carries the cannon: Jenna calls "Kraden, come look at this... There's a huge
/// ice wall blocking our path," and Kraden says they will go no farther unless
/// they do something about it. Once a visit.
void NorthernReaches_SightIceWall(void) {
  if (Event_GetFlag(FLAG_ICE_WALL_SIGHTED) != 0)
    return;
  Event_SetFlag(FLAG_ICE_WALL_SIGHTED);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgShipIceWallFirstApproach);
  FieldActor_WaitForMove(Actor_Get(ACTOR_SHIP));
  Actor_FaceDirectionAndWait(ACTOR_SHIP, FACING_NORTH);
  FieldEvent_Wait(5);
  // "Kraden, come look at this... There's a huge ice wall blocking our..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 5);
  // "Hm... If we can't do something about that wall, we're not going any..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Event_End();
}

/// The ship, carrying the cannon, sails north toward the ice wall, cells
/// (89, 15) to (91, 15): the view turns to the wall, and Kraden says to use the
/// cannon they got in Loho and to take the ship between the two blocks of ice
/// before it for shelter. Only once.
void NorthernReaches_DiscussIceWall(void) {
  if (Event_GetFlag(FLAG_ICE_WALL_DISCUSSED) != 0)
    return;
  Event_SetFlag(FLAG_ICE_WALL_DISCUSSED);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgShipIceWallCrewReposition);
  FieldActor_WaitForMove(Actor_Get(ACTOR_SHIP));
  FieldEvent_Wait(5);
  // "Kraden, come look at this... There's a huge ice wall blocking our..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 5);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 176, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(5);
  Actor_SetPosition(ACTOR_KRADEN, 408, 288);
  Actor_SetPosition(ACTOR_JENNA, 408, 288);
  Actor_SetPosition(ACTOR_SHEBA, 408, 288);
  // "Nothing to worry about, Jenna. We'll just have to use that cannon we..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  // "That's right! If the cannon can blast away a stone wall, think what..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 5);
  // "Felix, take us between those two blocks of ice for shelter when you..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Event_End();
}

/// Shrinks an actor evenly by step, as the ship sails off into the distance.
#define Actor_ShrinkBy(actor, step)                                            \
  {                                                                            \
    s32 scale = (actor)->scaleY;                                               \
                                                                               \
    (actor)->scaleY = scale - (step);                                          \
    (actor)->scaleX = scale - (step);                                          \
  }

/// The Magma Ball grows from half its size to full, a thirty-second a frame,
/// over its first MAGMA_BALL_GROW_FRAMES frames of flight.
#define MAGMA_BALL_GROW_FRAMES 16
/// The ship's last shrink as it sails off: SHIP_SHRINK_STEPS steps of seven
/// frames.
#define SHIP_SHRINK_STEPS 20

/// The ship, carrying the cannon, comes between the two blocks of ice before
/// the ice wall, cell (92, 13) or (93, 13). Piers says they should be safe
/// firing from here, and Kraden asks Felix to aim at the wall and fire when
/// ready. With a Yes the ship jolts back as the cannon fires, the Magma Ball
/// flies into the wall, which bursts in flames and white light and is gone,
/// and after the crew's cheers the ship sails off north. With a No it pulls
/// back to try again.
void NorthernReaches_FireMagmaBall(void) {
  FIXED from[3];
  struct Actor *ship;
  struct Actor *ball;
  struct Actor *smoke;
  FIXED to[3];
  s32 count, message;
  s32 told;

  ball = Actor_Get(ACTOR_MAGMA_BALL_SHOT);
  smoke = Actor_Get(ACTOR_CANNON_SMOKE);
  ship = Actor_Get(ACTOR_SHIP);
  count = Event_GetFlag(FLAG_ICE_WALL_SHATTERED);
  if (count != 0)
    return;
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgShipSafeFiringDistance);
  Actor_MoveToAndWait(ACTOR_SHIP, 464, 216);
  Actor_FaceDirectionAndWait(ACTOR_SHIP, FACING_NORTH);
  FieldEvent_Wait(5);
  Actor_Get(ACTOR_KRADEN)->scaleX = 0;
  Actor_Get(ACTOR_PIERS)->scaleX = 0;
  Actor_Get(ACTOR_JENNA)->scaleX = 0;
  Actor_Get(ACTOR_SHEBA)->scaleX = 0;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_KRADEN), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PIERS), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNA), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SHEBA), 0);
  Actor_SetPosition(ACTOR_KRADEN, 464, 216);
  Actor_SetPosition(ACTOR_PIERS, 464, 216);
  Actor_SetPosition(ACTOR_JENNA, 464, 216);
  Actor_SetPosition(ACTOR_SHEBA, 464, 216);
  told = Event_GetFlag(FLAG_SAFE_DISTANCE_TOLD);
  message = (s32)MsgShipSafeFiringDistance;
  if (told == 0) {
    Event_SetFlag(FLAG_SAFE_DISTANCE_TOLD);
    // "We should be safe if we fire from this distance."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0, 5);
  }
  FieldEvent_SetMessageId((message + 1));
  // "OK, here we go! Felix, aim right at the ice wall and fire when ready!"
  // Yes: fire; No: wait.
  Event_OpenMessage(ACTOR_KRADEN, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
    // "What? You're not ready yet? Take your time and get into position..."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
    Actor_SetDestinationOffset(ACTOR_SHIP, 0, 16);
    FieldActor_WaitForMove(ship);
    FieldEvent_Wait(5);
  } else {
    gFieldState->message++;
    // "Fire the Magma Ball!"
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
    Actor_SetSpeed(ACTOR_SHIP, 3);
    Actor_Get(ACTOR_SHIP)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkBy(ACTOR_SHIP, 0, 16);
    Event_PlaySound(SE_CLATTER);
    FieldEvent_Wait(4);
    smoke->scaleX = FIXED_ONE * 3 / 2;
    smoke->scaleY = FIXED_ONE * 3 / 2;
    Actor_SetSpriteColorMode(Actor_Get(ACTOR_CANNON_SMOKE), SPRITE_COLOR_GREY);
    Actor_SetPosition(ACTOR_CANNON_SMOKE, 464, 200);
    Actor_SetAnimation(ACTOR_CANNON_SMOKE, 1);
    ball->scaleX = FIXED_ONE / 2;
    ball->scaleY = FIXED_ONE / 2;
    Actor_SetSpeed(ACTOR_MAGMA_BALL_SHOT, 2);
    Actor_SetPosition(ACTOR_MAGMA_BALL_SHOT, 464, 216);
    Actor_WalkTo(ACTOR_MAGMA_BALL_SHOT, 464, 144);
    for (count = MAGMA_BALL_GROW_FRAMES - 1; count >= 0; count--) {
      ball->scaleX += FIXED_ONE / 32;
      ball->scaleY += FIXED_ONE / 32;
      System_Wait(1);
    }
    FieldCamera_MoveTo(472, CAMERA_KEEP, 176, 1);
    FieldCamera_WaitForArrival();
    FieldActor_WaitForMoveById(ACTOR_MAGMA_BALL_SHOT);
    Event_PlaySound(SE_CRASH);
    from[0] = ((ball->x.fixed) - toFIXED(2));
    from[1] = (ball->y.fixed);
    from[2] = ((ball->z.fixed) - toFIXED(8));
    to[0] = toFIXED(400);
    to[1] = 0;
    to[2] = toFIXED(80);
    NorthernReaches_ShatterIceWall(from, to, NorthernReaches_ClearIceWall);
    FieldEvent_Wait(20);
    Actor_SetPosition(ACTOR_CANNON_SMOKE, 0, 0);
    Actor_SetPosition(ACTOR_MAGMA_BALL_SHOT, 0, 0);
    gFieldState->transitionFrames = 100;
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    gFieldState->transitionFrames = 8;
    // "Great! We did it! We can keep going now!"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 5);
    // "All right, let's head for the Northern Seas."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
    // "Let's go!"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 5);
    Actor_SetSpeed(ACTOR_SHIP, 0.8);
    Actor_MoveToAndWait(ACTOR_SHIP, 464, 176);
    EventCamera_FollowActor(ACTOR_SHIP, 1);
    Actor_MoveToAndWait(ACTOR_SHIP, 464, 152);
    Actor_SetSpeed(ACTOR_SHIP, 0.6);
    // Off toward the horizon: five steps of 16 pixels north, written out as
    // the event's other steps are, the ship a sixteenth smaller at each.
    Actor_ShrinkBy(ship, FIXED_ONE / 16);
    Actor_SetDestinationOffset(ACTOR_SHIP, 0, -16);
    FieldActor_WaitForMoveById(ACTOR_SHIP);
    Actor_ShrinkBy(ship, FIXED_ONE / 16);
    Actor_SetDestinationOffset(ACTOR_SHIP, 0, -16);
    FieldActor_WaitForMoveById(ACTOR_SHIP);
    Actor_ShrinkBy(ship, FIXED_ONE / 16);
    Actor_SetDestinationOffset(ACTOR_SHIP, 0, -16);
    FieldActor_WaitForMoveById(ACTOR_SHIP);
    Actor_ShrinkBy(ship, FIXED_ONE / 16);
    Actor_SetDestinationOffset(ACTOR_SHIP, 0, -16);
    FieldActor_WaitForMoveById(ACTOR_SHIP);
    Actor_ShrinkBy(ship, FIXED_ONE / 16);
    Actor_SetDestinationOffset(ACTOR_SHIP, 0, -16);
    FieldActor_WaitForMoveById(ACTOR_SHIP);
    Actor_SetSpeed(ACTOR_SHIP, 0.2);
    Actor_SetDestinationOffset(ACTOR_SHIP, 0, -12);
    for (count = 0; count < SHIP_SHRINK_STEPS; count++) {
      Actor_ShrinkBy(ship, FIXED_ONE * 9 / 256);
      System_Wait(7);
    }
    ship->scaleY = 0;
    ship->scaleX = 0;
    FieldEvent_Wait(10);
    Event_SetFlag(FLAG_ICE_WALL_SHATTERED);
    FieldEvent_SetExitRequest(NORTHERN_REACHES_TO_WORLD_MAP_NORTH);
  }
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  struct MainRoutineState *state;

  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  state = &gMainRoutineState;
  Actor_Get(state->leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (state->entrance == ENTRANCE_ABOARD_SHIP_WITH_CANNON) {
    Event_SetFlag(FLAG_SHIP_CARRIES_CANNON);
    // The player steers the ship, actor 8, in place of the leader.
    state->ownerOverride = 1;
    Event_SetDestination(SCENE_NORTHERN_REACHES,
                         NORTHERN_REACHES_FROM_WORLD_MAP_NORTH);
  }
  RisingParticles_Start();
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineState.entrance == ENTRANCE_ICE_WALL_GONE)
    NorthernReaches_ClearIceWall();
  if (Event_GetFlag(FLAG_ICE_WALL_SHATTERED))
    NorthernReaches_ClearIceWall();
  return 0;
}

void BattleEffect_StepVector3(s32 *vector, s32 damping, s32 gravity);

/// The Magma Ball's explosion over the ice wall: sparks fly, the screen fades
/// to white, `callback` draws the wall gone and its shards fly apart from
/// `position`. The caller passes where the ball struck as `blast`, but nothing
/// reads it: the sparks burst at random over a fixed stretch of the screen, 52
/// to 179 pixels across and 16 to 79 down, wherever the ball landed.
void NorthernReaches_ShatterIceWall(FIXED *blast, FIXED *position,
                                    void (*callback)(void)) {
  struct IceWallState *state;
  struct IceShard *shard;
  struct EffectSprite *sprite;
  struct EffectSprite *flySprite;
  struct ExplosionSpark *sparks;
  struct ExplosionSpark *spark;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  u8 frameSlot[4];
  void *sheet;
  s32 i, frame, column, entry, sparkEntry, tiles, flying, sparkTiles;
  s32 size;

  state = HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct IceWallState));
  flying = 0;
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_FIELD_EFFECT_ICE_SHARD),
                      state);
  entry = VramCache_FindFreeEntry();
  tiles = VramCache_Load(entry, 1280, state);
  i = 0;
  shard = state->shards;
  sprite = state->sprites;
  for (; i < ICE_SHARD_COUNT; i++) {
    EffectSprite_Init(sprite, 0, 0, OBJ_SIZE_16X16, (i & 3) * 8 + tiles);
    sprite->depth = EFFECT_SPRITE_FRONT_DEPTH;
    sprite->oam.priority = 0;
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    column = i / 6;
    shard->x = position[0] + FIELD_CELL_ORIGIN(column);
    shard->y = position[1] - FIELD_CELL_ORIGIN(i % 6);
    column -= 4;
    sprite++;
    shard->z = position[2];
    shard->velocityX = (Random16() * 3 >> 1) * column;
    shard->velocityY = (Random16() << 2) + toFIXED(2);
    shard->velocityZ = 0;
    shard->age = 0;
    shard++;
  }
  /* FAKEMATCH(temporary): size, the sprite sheet's size held in a local before
   * the sparks are allocated; as SHEET_BUFFER_SIZE at the allocation. */
  size = SHEET_BUFFER_SIZE;
  sparks =
      Heap_AllocEwram(EXPLOSION_SPARK_COUNT * sizeof(struct ExplosionSpark));
  sheet = Heap_Alloc(size);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_FIELD_EFFECT_EXPLOSION),
                      sheet);
  sparkEntry = VramCache_FindFreeEntry();
  sparkTiles = VramCache_Load(sparkEntry, 1024, sheet);
  Heap_Release(sheet);
  for (i = 0; i < EXPLOSION_SPARK_COUNT; i++)
    sparks[i].age = -(Random16() << 5 >> 16);
  Dma_Set(Field_ExplosionPalette,
          (void *)&OBJ_PLTT_COLOR(EXPLOSION_SPARK_PALETTE, 0),
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
  for (frame = 0; frame <= 149; frame++) {
    if ((frame & 31) == 0) {
      Event_PlaySound(SE_CRASH);
    }
    if (frame == 60) {
      PaletteFade_SetTarget(RGB_WHITE, PALETTES_ALL);
      PaletteFade_StartInterpolation(90);
      Event_PlaySound(SE_RUMBLE);
    }
    if (frame == 105) {
      callback();
      flying = 1;
      MapGrid_SetShake(6, 6, 1);
      Event_PlaySound(SE_SPARKLING_PASSAGE);
    }
    if (frame == 130) {
      flying = 1;
    }
    if (flying != 0) {
      shard = state->shards;
      flySprite = state->sprites;
      for (i = ICE_SHARD_COUNT - 1; i >= 0; i--) {
        if (shard->age < ICE_SHARD_FRAMES) {
          EffectSprite_DrawAtWorldPosition(flySprite, &shard->x);
          BattleEffect_StepVector3(&shard->x, 63, -toFIXED(3) / 16);
          shard->age++;
        }
        flySprite++;
        shard++;
      }
    }
    spark = sparks;
    do {
      if (spark->age == 0) {
        spark->oam.words.first = OBJ_SIZE_16X16;
        spark->oam.words.second = 0;
        spark->oam.oam.x = (Random16() << 7 >> 16) + 52;
        spark->oam.oam.y = (Random16() << 6 >> 16) + 16;
        spark->oam.oam.palette = EXPLOSION_SPARK_PALETTE;
        spark->oam.oam.priority = 0;
      }
      if (spark->age >= 0) {
        spark->oam.oam.index = sparkTiles + (spark->age >> 2) * 4;
        Oam_AddSpriteAtDepthClamped(spark, EXPLOSION_SPARK_DEPTH);
      }
      spark->age++;
      if (spark->age >= EXPLOSION_SPARK_FRAMES) {
        spark->age = -(Random_Below(7));
      }
      spark++;
    } while ((s32)spark <= (s32)&sparks[EXPLOSION_SPARK_COUNT - 1]);
    wait(1);
  }
  VramCache_FreeEntry(sparkEntry);
  Heap_Release(sparks);
  FieldEvent_Wait(40);
  MapGrid_SetShake(0, 0, 0);
  VramCache_FreeEntry(entry);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

/// The ship at rest on the sea, without its shadow, on the middle layer.
s32 gNorthernReaches_ShipAtRest[] = {
    SC_CALL((s32)Ship_DropShadow),
    SC_ANIMATION(7),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_BELOW_ICE_WALL, CONDITION_ALWAYS, 470, 0, 192, FACING_NORTH, 0,
     -1, -1, -1, -1, 0},
    {ENTRANCE_ICE_WALL_GONE, CONDITION_ALWAYS, 470, 0, 192, FACING_NORTH, 0, -1,
     -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(NorthernReaches),
    SCENE_EXIT_UNLESS(NORTHERN_REACHES_TO_WORLD_MAP_SOUTH, WorldMap,
                      WORLD_MAP_FROM_NORTHERN_REACHES_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(NORTHERN_REACHES_TO_WORLD_MAP_NORTH, WorldMap,
                      WORLD_MAP_FROM_NORTHERN_REACHES_NORTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS,
                    (s32)gNorthernReaches_ShipAtRest, 120, 0, 280, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXPLOSION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DUST_CLOUD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56,
                    0, -56, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0,
                    -56, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0,
                    -56, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0,
                    -56, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, NORTHERN_REACHES_TO_WORLD_MAP_SOUTH, CONDITION_ALWAYS,
     NORTHERN_REACHES_TO_WORLD_MAP_SOUTH},
    {TRIGGER_EXIT, NORTHERN_REACHES_TO_WORLD_MAP_NORTH, CONDITION_ALWAYS,
     NORTHERN_REACHES_TO_WORLD_MAP_NORTH},
    {TRIGGER_STEP, CELL_SOUTH_OF_ICE_WALL,
     IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
     EVENT(NorthernReaches_DiscussIceWall)},
    {TRIGGER_STEP, CELL_BETWEEN_ICE_BLOCKS,
     IF_FLAG_SET(FLAG_SHIP_CARRIES_CANNON),
     EVENT(NorthernReaches_FireMagmaBall)},
    {TRIGGER_STEP, CELL_UNDER_ICE_WALL, FLAG_SHIP_CARRIES_CANNON,
     EVENT(NorthernReaches_SightIceWall)},
    {TRIGGER_TABLE_END},
};

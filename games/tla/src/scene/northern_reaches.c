/// @file
/// The Northern Reaches, where an ice wall blocks the ship's way to Prox until
/// Felix fires the Magma Ball at it from a safe distance.

#include "shared/tables.h"
#include "field/field_sprite.h"
#include "graphics/palette_buffers.h"
#include "sprite_ids.h"
#include "flags.h"
#include "system/io_reg.h"
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
#include "field/actor.h"
#include "system/main_routine_work.h"
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

/// The crew has talked the ice wall over.
#define FLAG_ICE_WALL_DISCUSSED 0x90c

/// The Northern Reaches, by the rows of gActorPlacements; the rows for
/// Jenna, Sheba and Piers go to their party slots.
enum NorthernReachesActorId {
  ACTOR_SHIP = ACTOR_FIRST_PLACED, ///< the ship at rest, the leader at sea
  ACTOR_MAGMA_BALL_SHOT,           ///< the Magma Ball fired at the ice wall
  ACTOR_BLAST, ///< a dust cloud at the bow as the cannon fires
  ACTOR_KRADEN
};

struct IceShard;
struct IceShardSpark;
struct IceShardSprite;

void NorthernReaches_ShatterIceWall(s32 unused, s32 *position,
                                    void (*callback)(void));

void EffectSprite_DrawAtWorldPosition(struct IceShardSprite *sprite,
                                      struct IceShard *particle);

extern s32 gNorthernReaches_ShipAtRest[];

/// The Magma Ball's ice shards and the explosion's sparks, as the ice wall
/// shatters (NorthernReaches_ShatterIceWall).
struct IceShard {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  u32 age;
};

struct IceShardSpark {
  struct IceShardSpark *next; ///< The sprite list's link.
  union SparkOam oam;
  s32 timer;
};

/// The wall breaks into 54 shards, and 96 sparks of the explosion fly about it.
#define ICE_SHARD_COUNT 54
#define SPARK_COUNT 96

struct IceWallWork {
  u8 sheet[0x1000];
  struct EffectSprite sprites[ICE_SHARD_COUNT];
  struct IceShard particles[ICE_SHARD_COUNT];
};

/// Starts an actor's animation from its first frame, on map layer 1.
s32 NorthernReaches_ResetAnimationFrame(struct Actor *object) {
  ((struct FieldSprite *)object->sprite)->nextFrame = 0;
  object->layer = 1;
  return 1;
}

void Event_Start();
void Event_End();
void FieldCamera_WaitForArrival();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Draws the ice wall gone: its pictures and its cells, as the Magma Ball
/// leaves it and on every later visit (FLAG_ICE_WALL_SHATTERED).
void NorthernReaches_ClearIceWall(void) {
  MapGrid_CopyCellsLayered(47, 9, 9, 4, 25, 9);
  MapGrid_CopyCellsLayered(47, 69, 9, 10, 25, 69);
  MapGrid_CopyCellAttributes(111, 8, 9, 5, 89, 8);
  return;
}

/// The first time the ship nears the ice wall, once a visit, the crew calls
/// Kraden to see it.
/// "Kraden, come look at this... There's a huge ice wall blocking our path..."
void NorthernReaches_SightIceWall(void) {

  if (Event_GetFlag(FLAG_SCENE_TEMP_00) != 0)
    goto done;
  Event_SetFlag(FLAG_SCENE_TEMP_00);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgShipIceWallFirstApproach);
  FieldActor_WaitForMove((s32)Actor_Get(ACTOR_SHIP));
  Actor_FaceDirectionAndWait(ACTOR_SHIP, FACING_NORTH);
  FieldEvent_Wait(5);
  // "Kraden, come look at this... There's a huge ice wall blocking our..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 5);
  // "Hm... If we can't do something about that wall, we're not going any..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Event_End();
done:
  return;
}

/// The crew gathers at the bow to talk the ice wall over, once
/// (FLAG_ICE_WALL_DISCUSSED).
void NorthernReaches_DiscussIceWall(void) {

  if (Event_GetFlag(FLAG_ICE_WALL_DISCUSSED) != 0)
    goto done;
  Event_SetFlag(FLAG_ICE_WALL_DISCUSSED);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgShipIceWallCrewReposition);
  FieldActor_WaitForMove((s32)Actor_Get(ACTOR_SHIP));
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
done:
  return;
}

s32 Actor_SetSpriteColorMode();

/// At a safe distance the crew asks Felix to fire the Magma Ball at the ice
/// wall; if he is ready the ship backs off, fires, and the wall shatters (flag
/// FLAG_ICE_WALL_SHATTERED).
/// "We should be safe if we fire from this distance."
/// Shrinks an actor evenly by step, as the ship sails off into the distance.
#define Actor_ShrinkBy(actor, step)                                            \
  {                                                                            \
    s32 scale = (actor)->scaleY;                                               \
                                                                               \
    (actor)->scaleY = scale - (step);                                          \
    (actor)->scaleX = scale - (step);                                          \
  }

void NorthernReaches_FireMagmaBall(void) {
  FIXED from[3];
  struct Actor *ship;
  struct Actor *ball;
  struct Actor *blast;
  FIXED to[3];
  s32 count, message;
  s32 told;

  ball = Actor_Get(ACTOR_MAGMA_BALL_SHOT);
  blast = Actor_Get(ACTOR_BLAST);
  ship = Actor_Get(ACTOR_SHIP);
  count = Event_GetFlag(FLAG_ICE_WALL_SHATTERED);
  if (count != 0)
    goto done;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
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
  if (told != 0)
    goto ask;
  Event_SetFlag(FLAG_SAFE_DISTANCE_TOLD);
  // "We should be safe if we fire from this distance."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_PIERS, 0, 5);
ask:
  FieldEvent_SetMessageId((message + 1));
  // "OK, here we go! Felix, aim right at the ice wall and fire when ready!"
  // Yes: fire; No: wait.
  Event_OpenMessage(ACTOR_KRADEN, 0);
  if (!FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0))
    goto ready;
  // "What? You're not ready yet? Take your time and get into position..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_SetDestinationOffset(ACTOR_SHIP, 0, 16);
  FieldActor_WaitForMove(ship);
  FieldEvent_Wait(5);
  goto putAway;
ready:
  gFieldWork->message++;
  // "Fire the Magma Ball!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
  Actor_SetSpeed(ACTOR_SHIP, 3);
  Actor_Get(ACTOR_SHIP)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkBy(ACTOR_SHIP, 0, 16);
  Event_PlaySound(SE_CLATTER);
  FieldEvent_Wait(4);
  blast->scaleX = FIXED_ONE * 3 / 2;
  blast->scaleY = FIXED_ONE * 3 / 2;
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_BLAST), 6);
  Actor_SetPosition(ACTOR_BLAST, 464, 200);
  Actor_SetAnimation(ACTOR_BLAST, 1);
  ball->scaleX = FIXED_ONE / 2;
  ball->scaleY = FIXED_ONE / 2;
  Actor_SetSpeed(ACTOR_MAGMA_BALL_SHOT, 2);
  Actor_SetPosition(ACTOR_MAGMA_BALL_SHOT, 464, 216);
  Actor_WalkTo(ACTOR_MAGMA_BALL_SHOT, 464, 144);
  count = 15;
growBall:
  ball->scaleX += FIXED_ONE / 32;
  ball->scaleY += FIXED_ONE / 32;
  System_Wait(1);
  count--;
  if (count >= 0)
    goto growBall;
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
  NorthernReaches_ShatterIceWall((s32)from, to, NorthernReaches_ClearIceWall);
  FieldEvent_Wait(20);
  Actor_SetPosition(ACTOR_BLAST, 0, 0);
  Actor_SetPosition(ACTOR_MAGMA_BALL_SHOT, 0, 0);
  gFieldWork->transitionFrames = 100;
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  gFieldWork->transitionFrames = 8;
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
  count = 19;
settle:
  Actor_ShrinkBy(ship, FIXED_ONE * 9 / 256);
  System_Wait(7);
  count--;
  if (count >= 0)
    goto settle;
  ship->scaleY = 0;
  ship->scaleX = 0;
  FieldEvent_Wait(10);
  Event_SetFlag(FLAG_ICE_WALL_SHATTERED);
  FieldEvent_SetExitRequest(4);
putAway:
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Event_End();
done:
  return;
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  struct MainRoutineWork *state;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  state = &gMainRoutineWork;
  Actor_Get(state->leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (state->entrance == 90) {
    Event_SetFlag(FLAG_SHIP_CARRIES_CANNON);
    state->ownerOverride = 1;
    Event_SetDestination(SCENE_NORTHERN_REACHES, 4);
  }
  RisingParticles_Start();
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.entrance == 3)
    NorthernReaches_ClearIceWall();
  if (Event_GetFlag(FLAG_ICE_WALL_SHATTERED))
    NorthernReaches_ClearIceWall();
  return 0;
}

void BattleEffect_StepVector3(struct IceShard *particle, s32 a, s32 b);

/// The Magma Ball's flight and the wall's ice shards
/// (ResourceId_FieldEffectIceShard); `callback` draws the wall gone.
void NorthernReaches_ShatterIceWall(s32 unused, s32 *position,
                                    void (*callback)(void)) {
  struct IceWallWork *work;
  struct IceShard *particle;
  struct EffectSprite *sprite;
  struct EffectSprite *flySprite;
  struct IceShardSpark *sparks;
  struct IceShardSpark *spark;
  s32 *timer;
  // Unused: gives the stack frame its 4 bytes.
  u8 frameSlot[4];
  void *sheet;
  s32 i, frame, column, zero, entry, sparkEntry, tiles, flying, sparkTiles;

  work = HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct IceWallWork));
  flying = 0;
  Decompress_TaggedLz(
      Resource_GetTableEntry((s32)&ResourceId_FieldEffectIceShard), work);
  entry = VramCache_FindFreeEntry();
  tiles = VramCache_Load(entry, 0x500, work);
  i = 0;
  particle = work->particles;
  sprite = work->sprites;
  for (; i < ICE_SHARD_COUNT; i++) {
    EffectSprite_Init(sprite, 0, 0, 0x40000000, (i & 3) * 8 + tiles);
    zero = 0;
    sprite->depth = 250;
    sprite->oam.priority = zero;
    sprite->oam.fullColor = 1;
    sprite->oam.palette = zero;
    column = i / 6;
    particle->x = position[0] + (column << 20);
    particle->y = position[1] - ((i % 6) << 20);
    column -= 4;
    sprite++;
    particle->z = position[2];
    particle->velocityX = (Random16() * 3 >> 1) * column;
    particle->velocityY = (Random16() << 2) + toFIXED(2);
    particle->velocityZ = zero;
    particle->age = zero;
    particle++;
  }
  sparks = Heap_AllocEwram(SPARK_COUNT * sizeof(struct IceShardSpark));
  /* FAKEMATCH(temporary): the size held in the local that then takes the
   * buffer. */
  sheet = (void *)0x1000;
  sheet = Heap_Alloc((s32)sheet);
  Decompress_TaggedLz(
      Resource_GetTableEntry((s32)&ResourceId_FieldEffectExplosion), sheet);
  sparkEntry = VramCache_FindFreeEntry();
  sparkTiles = VramCache_Load(sparkEntry, 0x400, sheet);
  Heap_Release(sheet);
  timer = &sparks->timer;
  for (i = SPARK_COUNT - 1; i >= 0; i--) {
    *timer = -(Random16() << 5 >> 16);
    timer += 4;
  }
  Dma_Set(Field_ExplosionPalette, (void *)&OBJ_PLTT_COLOR(15, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  for (frame = 0; frame <= 149; frame++) {
    if ((frame & 31) == 0) {
      Event_PlaySound(SE_CRASH);
    }
    if (frame == 60) {
      PaletteFade_SetTarget(RGB_WHITE, 0);
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
      particle = work->particles;
      flySprite = work->sprites;
      for (i = ICE_SHARD_COUNT - 1; i >= 0; i--) {
        if (particle->age < 60) {
          EffectSprite_DrawAtWorldPosition(flySprite, particle);
          BattleEffect_StepVector3(particle, 63, -12288);
          particle->age++;
        }
        flySprite++;
        particle++;
      }
    }
    spark = sparks;
    do {
      if (spark->timer == 0) {
        spark->oam.words.first = 0x40000000;
        spark->oam.words.second = 0;
        spark->oam.oam.x = (Random16() << 7 >> 16) + 52;
        spark->oam.oam.y = (Random16() << 6 >> 16) + 16;
        spark->oam.oam.palette = 15;
        spark->oam.oam.priority = 0;
      }
      if (spark->timer >= 0) {
        spark->oam.oam.index = sparkTiles + (spark->timer >> 2) * 4;
        Oam_AddSpriteAtDepthClamped(spark, 100);
      }
      spark->timer++;
      if (spark->timer > 31) {
        spark->timer = -(Random_Below(7));
      }
      spark++;
    } while ((s32)spark <= (s32)&sparks[SPARK_COUNT - 1]);
    wait(1);
  }
  VramCache_FreeEntry(sparkEntry);
  Heap_Release(sparks);
  FieldEvent_Wait(40);
  MapGrid_SetShake(0, 0, 0);
  VramCache_FreeEntry(entry);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

s32 gNorthernReaches_ShipAtRest[] = {
    SC_CALL((s32)NorthernReaches_ResetAnimationFrame),
    SC_ANIMATION(7),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 470, 0, 192, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 470, 0, 192, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(NorthernReaches),
    SCENE_EXIT_UNLESS(1, WorldMap, 60, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, WorldMap, 61, EXIT_FLAG_NONE),
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
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 20, 0x190a, EVENT(NorthernReaches_DiscussIceWall)},
    {TRIGGER_STEP, 21, 0x190a, EVENT(NorthernReaches_FireMagmaBall)},
    {TRIGGER_STEP, 22, FLAG_SHIP_CARRIES_CANNON,
     EVENT(NorthernReaches_SightIceWall)},
    {TRIGGER_TABLE_END},
};

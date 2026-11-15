/// @file
#include "djinn_ids.h"
#include "fixed_math.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "scene_ids.h"
#include "sprite_ids.h"
#include "types.h"
#include "version.h"
#include "battle/battle.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_coordinate.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "field/map_grid.h"
#include "field/scene_tables.h"
#include "field/sprite.h"
#include "game/game.h"
#include "game/owner_state.h"
#include "graphics/graphics.h"
#include "graphics/resource.h"
#include "graphics/vram_tab.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "scene/staff_credit.h"
#include "sound/sound.h"
#include "system/bg_scroll.h"
#include "system/frame.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/io_write_queue.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/task.h"
#include "window/item_icon.h"
#include "window/window.h"
#include "text/msg_ids.h"

void Actor_StartScript(struct Actor *actor, const s32 *script);
void WorldMap_LoadTidalWave(void);

#include "shared/tables.h"

struct Actor;

/// FieldSprite's byte 0x11 read as its bits.
struct SpriteCacheBits {
  u8 retainCacheEntry : 1; ///< keeps the VRAM cache entry when it is released
};

struct WorldMapOam {
  u8 unk00[6];
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
};

/// The World Map's own actors, numbered in the order of
/// gWorldMapActorPlacements from ACTOR_FIRST_PLACED; a placement wearing a
/// party member's sprite is that member, and takes no number.
enum WorldMapActorId {
  /// Piers's ship, drawn from the map's tiles (gWorldMap_TileSpriteScript).
  ACTOR_SHIP = ACTOR_FIRST_PLACED,
  /// The Venus Djinni Echo, circling south of Daila until the party meets him.
  ACTOR_ECHO,
  ACTOR_IRON,
  ACTOR_SOUR,
  ACTOR_CHILL,
  ACTOR_WHEEZE,
  ACTOR_PETRA,
  ACTOR_CORE,
  /// The men from Loho who bring the cannon to the ship; the first speaks.
  ACTOR_LOHO_WORKER_A,
  ACTOR_LOHO_WORKER_B,
  ACTOR_LOHO_WORKER_C,
  /// Where the Golden Boots lie once they are shown (WorldMap_ShowGoldenBoots).
  ACTOR_GOLDEN_BOOTS,
  /// The bandit and the two thieves from Vault, after a linked game.
  ACTOR_BANDIT,
  ACTOR_THIEF_A,
  ACTOR_THIEF_B,
  ACTOR_KRADEN
};

/// gWorldMap_CollapsePlacements' and gWorldMap_DriftPlacements' actors: Kraden
/// and Alex on Idejima, and the piece of Idejima that splits away, then the
/// island itself.
enum { ACTOR_IDEJIMA_KRADEN = ACTOR_FIRST_PLACED, ACTOR_ALEX, ACTOR_IDEJIMA };

/// gWorldMap_FirstVoyagePlacements' actors.
enum { ACTOR_VOYAGE_SHIP = ACTOR_FIRST_PLACED, ACTOR_VOYAGE_KRADEN };

/// gWorldMap_DriftingIdejimaPlacements' one actor.
enum { ACTOR_DRIFTING_IDEJIMA = ACTOR_FIRST_PLACED };

/// The World Map's own cell behaviors, which the map's cells carry: the ground
/// around each Djinni met on the map.
enum WorldMapCellBehavior {
  CELL_BEHAVIOR_WORLD_MAP_IRON = 92,
  CELL_BEHAVIOR_WORLD_MAP_SOUR,
  CELL_BEHAVIOR_WORLD_MAP_CHILL,
  CELL_BEHAVIOR_WORLD_MAP_WHEEZE,
  CELL_BEHAVIOR_WORLD_MAP_PETRA,
  CELL_BEHAVIOR_WORLD_MAP_CORE
};

/// The base of the cell behaviors an area's placed actor raises when touched
/// (gWorldMap_PlacedActors), as The Broken Seal's; the areas with two ways in
/// choose one by the leader's approach.
enum { STEP_TRIGGER_AREA_BASE = 100 };
#define STEP_TRIGGER_AREA(area) (STEP_TRIGGER_AREA_BASE + (area))
enum { AREA_DAILA = 0, AREA_YALLAM = 25, AREA_PROX = 50 };

void Gauge_Update(void);

void WorldMap_SelectExitByZ(s32 a, s32 b, s32 c);

void Actor_SetFlightMode(struct Actor *object, s32 mode);

void Gauge_Init(void);
void WorldMap_EndFlight(void);
void Gauge_End(void);

void Import_ActorSelectAnimation(struct Actor *object, s32 mode);

void WorldMap_FightBanditAndThieves(s32 entrance);

void WorldMap_DeliverLohoCannon(void);

void WorldMap_MeetEcho(void);

void WorldMap_ShowIdejimaDrifting(void);

void WorldMap_DriftNorthOfContinent(void);

void WorldMap_PanToAlhafraManor(void);

void WorldMap_TakeTheTiller(void);
void WorldMap_SailToShipDeck(void);
void WorldMap_CarryPartyToAquaRock(void);

void WorldMap_PlayLighthouseTransition(s32 mode);

void WorldMap_MeetSour(void);

void WorldMap_MeetChill(void);

void WorldMap_MeetWheeze(void);

void WorldMap_MeetPetra(void);

void WorldMap_MeetCore(void);

void WorldMap_SetUpFlyingShip(void);

void WorldMap_SetUpShip(void);
void WorldMap_GoAboard(void);

void WorldMap_RefuseToRunAway(void);

void WorldMap_GazeIntoPond(void);

void WorldMap_PickUpGoldenBoots(void);

void WorldMap_TakeOff(void);

extern s32 gWorldMap_TileSpriteScript[];
extern s32 gWorldMap_IslandScript[];
extern s32 gWorldMap_TidalWaveScript[];

extern u32 gWorldMap_GlowColorModes[];

s32 WorldMap_GetBlockPlace(s32 *position);

extern u16 gWorldMapBlend;

extern u8 gWorldMapPackedTilesB[];
extern u8 gWorldMapPackedPicturesB[];
extern u8 gWorldMapPalettesB[];

void WorldMap_SpawnTwinSparks(struct Actor *object);

void Actor_Release(struct Actor *object);

extern u8 gGauge_Tiles[][32];
extern s16 gGauge_Countdown;
extern u32 gGauge_Sprite[3];

void Party_DrainPp(s32 value);
s32 Party_GetPpRatio(void);

void WorldMap_WatchVenusLighthouseCollapse(void);

s32 Event_PickUpItem(s32 a, s32 b, s32 c);
void Actor_StartRenderModeScript(void);
s32 Field_FindNearbyTalkTarget(s32 owner);
s32 WorldMap_IsWalkable(struct Actor *object, s32 *position);
void Party_GivePiersDjinn(void);
void Event_WaitForZoomTransition(void);
void DjinnMenu_Run(void);
void Party_RestoreHpPp(s32 value);
extern s32 gWorldMap_HiddenMembers[8];

// Once the party has walked nine tenths of the encounter threshold, a coin
// toss either requests the event's battle now or makes the party walk the
// whole threshold first. The Japanese release has no render mode test.
#if !TLA_PATCH_1
#define WORLD_MAP_TRY_ENCOUNTER(a, b)                                          \
  {                                                                            \
    struct FieldWork *work;                                                    \
    struct MainRoutineWork *party;                                             \
                                                                               \
    work = Ram_HeapSlots->fieldWork;                                           \
    party = &gMainRoutineWork;                                                 \
    if (party->encounterSteps >= work->encounterThreshold * 9 / 10) {          \
      if (Random16() < 0x8000) {                                               \
        Event_RequestDjinnBattle(a, b);                                        \
        work->encounterBias = 0;                                               \
      } else {                                                                 \
        party->encounterSteps = work->encounterThreshold;                      \
      }                                                                        \
    }                                                                          \
  }
#else
#define WORLD_MAP_TRY_ENCOUNTER(a, b)                                          \
  {                                                                            \
    struct MainRoutineWork *party;                                             \
    struct FieldWork *work;                                                    \
                                                                               \
    party = &gMainRoutineWork;                                                 \
    work = Ram_HeapSlots->fieldWork;                                           \
    if (party->movementMode == 1) {                                            \
      if (party->encounterSteps >= work->encounterThreshold * 9 / 10) {        \
        if (Random16() < 0x8000) {                                             \
          Event_RequestDjinnBattle(a, b);                                      \
          work->encounterBias = 0;                                             \
        } else {                                                               \
          party->encounterSteps = work->encounterThreshold;                    \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }
#endif

void WorldMap_MeetIron(void) {
  WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_IRON, DJINNI_IRON)
}

void WorldMap_MeetSour(void) {
  WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_SOUR, DJINNI_SOUR)
}

void WorldMap_MeetChill(void) {
  WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_CHILL, DJINNI_CHILL)
}

void WorldMap_MeetWheeze(void) {
  WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_WHEEZE, DJINNI_WHEEZE)
}

void WorldMap_MeetPetra(void) {
  WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_PETRA, DJINNI_PETRA)
}

void WorldMap_MeetCore(void) {
  WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_CORE, DJINNI_CORE)
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

/* VIEW(world_map): routine Actor_SetSpriteColorMode as void
 * Actor_SetSpriteColorMode(struct Actor *object, s32 value); field/actor_view.h
 * has void Actor_SetSpriteColorMode(struct Actor *, u32); tried as the
 * definition's void Actor_SetSpriteColorMode(struct Actor *object, u32 value),
 * it compiles and moves 1 of 714 instructions in tla-ja (WorldMap_MeetEcho); as
 * The Broken Seal's inline Actor_SetColorMode(struct Actor *, s32), 2 lines in
 * every edition. */
void Actor_SetSpriteColorMode__world_map(
    struct Actor *object, s32 value) __asm__("Actor_SetSpriteColorMode");
#define Actor_SetSpriteColorMode Actor_SetSpriteColorMode__world_map
u32 __umodsi3(u32 a, u32 b);

#include "shared/tile_sprite_reset.h"

/// The marker blinks and bobs up and down on a circle.
// As The Broken Seal's WorldMap_CircleEcho.
void WorldMap_CircleEcho(struct BobbingMarker *marker) {
  s16 *angle;
  s32 y;
  s32 z;
  s32 radius;
  s32 rise;

  if (gFrameCount & 2)
    Actor_SetSpriteColorMode(marker, 10);
  else
    Actor_SetSpriteColorMode(marker, 7);
  if (marker->stopped == 0) {
    angle = &marker->angle;
    marker->x.fixed = px(10120);
    y = Math_MulQ16(Math_Sin(*angle << 3), px(4));
    z = px(8356);

    rise = px(16);
    marker->y.fixed = y + rise;
    marker->z.fixed = z;
    radius = rise;
    FieldVector_AddPolarOffsetFixed(radius, *angle, &marker->x);
    marker->facing = *angle + TRIG_TURN / 4;
    *angle += 0x400;
  }
}

s32 WorldMap_SetUpEcho(struct Actor *actor) {
  if (Event_GetFlag(FLAG_DJINNI(DJINNI_ECHO)) != 0) {
    return 0;
  }
  if (Event_GetFlag(FLAG_DJINN_LESSON) != 0) {
    return 0;
  }
  actor->update = (void (*)(void))WorldMap_CircleEcho;
  actor->motionFlags = 0;
  actor->timer = 0;
  actor->userValue = 0;
  actor->scaleX = Q16_ONE / 2;
  actor->scaleY = Q16_ONE / 2;
  return 0;
}

s32 WorldMap_SetIslandTile(struct Actor *object) {
  u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapGrid + 286);
  struct TileSprite *sprite = object->sprite;

  sprite->tile = *tile;
  sprite->frame = 0;
  return 1;
}

s32 WorldMap_SetUpWave(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

/// With flag 0x200 set the object is released, takes acceleration and speed
/// limit 0x1000 and is sent to an offset of its position.
s32 WorldMap_SendWaveIn(struct Actor *object) {
  s32 x, z;

  if (!Event_GetFlag(FLAG_SCENE_TEMP_00))
    return 1;
  Actor_SetSpriteFlags(object, 0);
  x = object->x.fixed;
  object->acceleration = px(1) / 16;
  object->speedLimit = px(1) / 16;
  z = object->z.fixed;
  FieldActor_SetMoveTargetFixed(object, x - px(30), object->y.fixed, z + px(8));
  return 0;
}

extern struct ActorPlacement gWorldMapActorPlacements[];
extern struct ActorPlacement gWorldMap_CollapsePlacements[];
extern struct ActorPlacement gWorldMap_DriftPlacements[];
extern struct ActorPlacement gWorldMap_FirstVoyagePlacements[];
extern struct ActorPlacement gWorldMap_DriftingIdejimaPlacements[];

/// The world map's entrances its code tests: arrivals at a place by its exit
/// (WORLD_MAP_AT_), and the story's scenes the map plays on arrival.
enum WorldMapEntrance {
  WORLD_MAP_AT_DAILA = 2,
  WORLD_MAP_AT_MADRA = 9,
  WORLD_MAP_AT_GAIA_ROCK = 39,
  WORLD_MAP_AT_LOHO = 59,
  WORLD_MAP_AT_NORTHERN_REACHES = 61,
  WORLD_MAP_VENUS_COLLAPSE = 64, ///< Venus Lighthouse falls, Idejima breaks
  WORLD_MAP_DRIFT_NORTH = 65,    ///< the tidal wave carries Idejima north
  WORLD_MAP_ALHAFRA_MANOR_VIEW = 67,
  WORLD_MAP_FIRST_VOYAGE = 68, ///< the party first takes the ship's tiller
  WORLD_MAP_SAIL_TO_DECK = 69,
  WORLD_MAP_PIERS_DJINN = 70,
  WORLD_MAP_JUPITER_BEACON = 72, ///< the beacon lit, seen from the sea
  WORLD_MAP_BANDIT_BEATEN = 73,  ///< after the battle with the bandit
  WORLD_MAP_BANDIT_LOST = 74,
  WORLD_MAP_IDEJIMA_DRIFTING = 77,
  WORLD_MAP_TO_AQUA_ROCK = 78,
  WORLD_MAP_DEBUG_FLIGHT = 98, ///< the debug menu's: the ship flying
  WORLD_MAP_DEBUG_SAILING = 99
};

/// The world map's exits its story scenes leave by (gSceneExits).
enum WorldMapExit {
  WORLD_MAP_EXIT_IDEJIMA_DRIFTING = 64,
  WORLD_MAP_EXIT_IDEJIMA = 65,
  WORLD_MAP_EXIT_SHIP_TILLER = 66,
  WORLD_MAP_EXIT_IDEJIMA_AFTER_DRIFT = 77,
  WORLD_MAP_EXIT_AQUA_ROCK = 78
};

void Leader_Search(void);

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  switch (gMainRoutineWork.entrance) {
  case WORLD_MAP_VENUS_COLLAPSE:
    return gWorldMap_CollapsePlacements;
  case WORLD_MAP_DRIFT_NORTH:
    return gWorldMap_DriftPlacements;
  case WORLD_MAP_FIRST_VOYAGE:
    if (!Event_GetFlag(FLAG_SHIP_SAILABLE))
      return gWorldMap_FirstVoyagePlacements;
  case WORLD_MAP_IDEJIMA_DRIFTING:
    return gWorldMap_DriftingIdejimaPlacements;
  default:
    return gWorldMapActorPlacements;
  }
}

/// The world map's render mode 8 start: with flag 0x147 a blank message is
/// shown first, the event work's counters are cleared.
void WorldMap_SetUpFlyingShip(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  gMainRoutineWork.movementMode = 8;
  if (Event_GetFlag(0x147)) {
    WorldMap_Freeze();
    MessageArgument_Push(ABILITY_HOVER, MESSAGE_ARGUMENT_ABILITY);
    // "Hover"
    Message_ShowCentered((s32)MsgCastAbilityName, 4);
  }
  if (Actor_Get(FieldParty_GetLeaderActor()) != NULL)
    Actor_StartRenderModeScript();
  Leader_Search();
  if (Event_GetFlag(0x147))
    work->climbing = 0;
  else
    work->climbing = 1;
  if (Event_GetFlag(0x147)) {
    Message_CloseFinishedWindows();
    WorldMap_ReloadPendingAreaGraphics();
  }
  Gauge_Init();
  work->fieldMenu = 0;
  work->pauseMenu = 0;
  work->aButton = 0;
  work->encounter = 0;
  work->psynergy = 0;
}

/// Sets render mode 7, clears object 8's child flags, then adds up the PP of
/// the active party; when it is 0, the current owner gets 1 PP back.
void WorldMap_SetUpShip(void) {
  s32 count;
  s32 pp;
  s32 i;

  gMainRoutineWork.movementMode = 7;
  WorldMap_EndFlight();
  Gauge_End();
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SHIP), 0);
  count = Party_CountMembers();
  pp = 0;
  i = 0;
  if (pp < count) {
  loop:
    // One member's PP.
    pp += Unit_Get(gMainRoutineWork.party[i])->pp;
    i++;
    if (i < count)
      goto loop;
  }
  if (pp == 0)
    Unit_AddPp(gMainRoutineWork.leaderActorId, 1);
}

void Actor_StartIdle(struct Actor *object);

/// The leader jumps aboard the ship, actor 8, when close enough.
void WorldMap_BoardShip(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader;
  struct Actor *ship;
  s32 kind, dx, dz, distance;

  if (GameFlag_Get(FLAG_SHIP_SAILABLE) == 0)
    return;
  kind = Field_FindNearbyTalkTarget(gMainRoutineWork.leaderActorId);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  ship = Actor_Get(ACTOR_SHIP);
  dx = FIELD_PIXEL(leader->x.fixed - ship->x.fixed);
  dz = FIELD_PIXEL(leader->z.fixed - ship->z.fixed);
  distance = dx * dx + dz * dz;
  if (kind != 8 && distance > 255)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  gMainRoutineWork.movementMode = 7;
  gMainRoutineWork.ownerOverride = 1;
  Event_SetFlag(FLAG_SAILING);
  gMainRoutineWork.shipX = ship->x.fixed;
  gMainRoutineWork.shipZ = ship->z.fixed;
  gMainRoutineWork.shipFacing = ship->facing;
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Import_ActorSelectAnimation(leader, 6);
  System_Wait(6);
  Event_PlaySound(SE_JUMP);
  Actor_SetSpriteFlags(leader, 0);
  Import_ActorSelectAnimation(leader, 7);
  leader->speedLimit = px(3);
  leader->acceleration = px(2);
  leader->velocityY = px(4);
  leader->groundHeight = px(4);
  Actor_StartIdle(leader);
  FieldActor_SetMoveTargetFixed(leader, ship->x.fixed, 0, ship->z.fixed);
  FieldActor_WaitForMove(leader);
  Import_ActorSelectAnimation(leader, 6);
  System_Wait(6);
  leader->groundHeight = 0;
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 0, 0);
  Scene_ChooseMusic();
  Scene_PlayMusic();
  Event_End();
  work->aButton = 0;
#if TLA_PATCH_1
  work->stepTrigger = 0;
#endif
  FieldEffect_ScheduleRatioTransition(Q16_ONE, 30);
}

/// The leader jumps ashore from the ship, where the shore ahead lets them land.
void WorldMap_LeaveShip(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *ship = Actor_GetById(ACTOR_SHIP);
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 cell = WorldMap_GetCollision(&ship->x.fixed);
  s32 position[3];
  s32 shore = 0;
  s32 blocked;
  u8 *block;
  u8 *blocks = gCellPictures;
  u8 flags;

  position[0] = ship->x.fixed;
  position[1] = ship->y.fixed;
  position[2] = ship->z.fixed;
  Vector_AddPolarOffset(0x180000, ship->facing, position);
  blocked = 0;
  block = blocks + ((((position[0] / 0x200000) & 31) +
                     ((position[2] / 0x200000) & 31) * 32) *
                    4);
  if (cell > 12) {
    blocked = 1;
  } else {
    if ((u32)(cell - 1) <= 2)
      shore = 1;
    if ((block[3] & 64) == 0 && shore != 0) {
      blocked = 1;
    } else {
      position[0] += px(6);
      blocked |= WorldMap_IsWalkable(ship, position);
      position[0] += -px(12);
      blocked |= WorldMap_IsWalkable(ship, position);
      position[0] += px(6);
      position[1] += px(6);
      blocked |= WorldMap_IsWalkable(ship, position);
      position[1] += -px(12);
      blocked |= WorldMap_IsWalkable(ship, position);
      position[1] += px(6);
      blocked |= WorldMap_IsWalkable(ship, position);
    }
  }
  if (blocked == 0) {
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEffect_ScheduleRatioTransition(Q16_ONE, 10);
    gMainRoutineWork.movementMode = 1;
    gMainRoutineWork.ownerOverride = 0;
    Event_ClearFlag(FLAG_SAILING);
    gMainRoutineWork.shipX = ship->x.fixed;
    gMainRoutineWork.shipZ = ship->z.fixed;
    gMainRoutineWork.shipFacing = ship->facing;
    flags = leader->motionFlags;
    leader->x.fixed = ship->x.fixed;
    leader->y.fixed = px(4);
    leader->z.fixed = ship->z.fixed;
    leader->motionFlags = 0;
    Actor_SetSpriteFlags(leader, 0);
    Import_ActorSelectAnimation(leader, 6);
    System_Wait(6);
    Event_PlaySound(SE_JUMP);
    Import_ActorSelectAnimation(leader, 7);
    leader->speedLimit = px(3);
    leader->acceleration = px(2);
    leader->velocityY = px(4);
    leader->motionFlags = flags;
    Actor_StartIdle(leader);
    FieldActor_SetMoveTargetFixed(leader, position[0], 0, position[2]);
    FieldActor_WaitForMove(leader);
    Import_ActorSelectAnimation(leader, 6);
    System_Wait(6);
    Event_End();
    Scene_ChooseMusic();
    Scene_PlayMusic();
    work->aButton = 0;
  }
}

void Actor_MoveToObject(s32 actor, s32 target);

/// Going aboard from the map: the ship's position is kept in gMainRoutineWork
/// for the way back.
void WorldMap_GoAboard(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_Get(ACTOR_SHIP);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (gMainRoutineWork.movementMode == 8)
    WorldMap_SetUpShip();
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  gMainRoutineWork.shipX = leader->x.fixed;
  gMainRoutineWork.shipZ = leader->z.fixed;
  gMainRoutineWork.shipFacing = 0;
  gMainRoutineWork.ownerOverride = 0;
  Event_ClearFlag(FLAG_SAILING);
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_SetExitRequest(work->stepTrigger);
}

/// Brings the actor to the player character's position.
static __inline__ void Actor_JoinLeader(s32 actor) {
  struct Actor *leader;

  Actor_SetAnimation(actor, 2);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(actor, leader->x.part[0].pixel,
                         leader->z.part[0].pixel);
  Actor_WaitForMoveById(actor);
  Actor_SetPosition(actor, 0, 0);
}

void WorldMap_RefuseToRunAway(void) {
  struct Actor *object;
  s32 x;
  s32 z;
  s32 southwest = FACING_SOUTHWEST;
  s32 west = FACING_WEST;

  if (Event_GetFlag(FLAG_JUPITER_BEACON_LIT) &&
      !Event_GetFlag(FLAG_REUNITED_AT_CONTIGO)) {
    object = Actor_Get(gMainRoutineWork.leaderActorId);
    x = object->x.fixed;
    z = object->z.fixed;
    x >>= 20;
    z >>= 20;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (Event_GetFlag(0x9b3)) {
      FieldEffect_ScheduleRatioTransition(Q16_ONE * 7 / 5, 6);
      Actor_MoveToObject(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
      System_Wait(1);
      Actor_WalkToAndWait(ACTOR_JENNA, (x << 4) + 24, (z << 4) - 12);
      Actor_FaceDirection(ACTOR_JENNA, southwest, 0);
      Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId,
                                 FACING_NORTHEAST);
      WorldMap_Freeze();
      FieldEvent_SetMessageId((s32)MsgWeShouldnT);
      // "We shouldn't run from Isaac anymore, Felix."
      Event_ShowMessage(ACTOR_JENNA, 0);
      WorldMap_ReloadPendingAreaGraphics();
      Actor_JoinLeader(ACTOR_JENNA);
      System_Wait(1);
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 7312, 8192);
      FieldEffect_ScheduleRatioTransition(Q16_ONE, 6);
    } else {
      s32 besideZ;

      FieldEffect_ScheduleRatioTransition(Q16_ONE * 7 / 5, 6);
      Actor_MoveToObject(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
      System_Wait(1);
      x <<= 4;
      besideZ = (z << 4) - 12;
      Actor_WalkToAndWait(ACTOR_JENNA, x + 24, besideZ);
      Actor_FaceDirection(ACTOR_JENNA, southwest, 0);
      Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId,
                                 FACING_NORTHEAST);
      WorldMap_Freeze();
      FieldEvent_SetMessageId((s32)MsgYouReNot);
      // "You're not planning on running away from Isaac again, are you?"
      Event_ShowMessage(ACTOR_JENNA, 0);
      WorldMap_ReloadPendingAreaGraphics();
      Actor_FaceDirection(ACTOR_JENNA, west, 0);
      Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, x + 12, besideZ);
      Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_EAST);
      Actor_SetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
      WorldMap_Freeze();
      // "We can't run anymore, Felix. I promised him that we wouldn't."
      Event_ShowMessage(ACTOR_JENNA, 0);
      Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
      // "Besides, they want to talk, not fight..."
      Event_ShowMessage(ACTOR_JENNA, 0);
      Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
      // "They're waiting for us. Let's go to Contigo."
      Event_ShowMessage(ACTOR_JENNA, 0);
      WorldMap_ReloadPendingAreaGraphics();
      Actor_JoinLeader(ACTOR_JENNA);
      System_Wait(1);
      Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 7312, 8192);
      Event_SetFlag(0x9b3);
      FieldEffect_ScheduleRatioTransition(Q16_ONE, 6);
    }
    System_Wait(8);
    Event_End();
  } else {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;

    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(SE_MAP_EXIT);
    FieldEvent_SetExitRequest(work->stepTrigger);
  }
}

/// The actor slot the pond's coins take when they show (WorldMap_GazeIntoPond).
enum { ACTOR_POND_COINS = 79 };

void WorldMap_GazeIntoPond(void) {
  u32 roll = (Random16() << 4) >> 16;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  // "Felix gazed into the pond..."
  Message_ShowCentered((s32)MsgGazedIntoThe, 1);
  if (roll == 0) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;
    s32 slot = ACTOR_POND_COINS;
    struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
    struct Unit *unit = Unit_Get(gMainRoutineWork.leaderActorId);
    s32 amount = (((unit->level * Random16()) << 1) >> 16) + 0x8001;
    s32 position[3];
    s32 x;

    // "There are some coins in the shallows!"
    Message_ShowCentered((s32)MsgThereAreSome, 1);
    position[0] = actor->x.fixed;
    position[1] = actor->y.fixed;
    position[2] = actor->z.fixed;
    FieldVector_AddPolarOffset(16, actor->facing, position);
    actor = Actor_Get(ACTOR_POND_COINS);
    x = position[0];
    if (actor == NULL) {
      actor = FieldActor_Create(0x1e9, x, position[1], position[2]);
      if (actor == NULL)
        return;
      work->actors[slot] = actor;
    } else {
      actor->x.fixed = x;
      actor->y.fixed = position[1];
      actor->z.fixed = position[2];
    }
    FieldActor_ShowItemIconById(ACTOR_POND_COINS, amount);
    /* FAKEMATCH(dead): the store and the call in a once-only block. */
    do {
      work->scoopedItem = amount;
      Scene_TalkToActor(0x1000 | slot);
    } while (0);
  } else if (roll == 1) {
    MapGrid_SetShake(3, 5, 1);
    MapGrid_SetShakeFixed((s32)(-1), (s32)(-1), (s32)(0xe666));
    FieldEvent_Wait(30);
    // "The boat's rocking! This thing's dangerous! I almost fell out!"
    Message_ShowCentered((s32)MsgTheBoatS, 1);
  } else {
    Message_ShowCentered((s32)MsgTheBoatS + (roll >> 1), 1);
  }
  Event_End();
}

void WorldMap_PickUpGoldenBoots(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Value3((s32 (*)())Event_PickUpItem, ACTOR_GOLDEN_BOOTS, ITEM_GOLDEN_BOOTS,
             -1) == 0)
    Event_ClearFlag(0xa77);
  Event_End();
}

void WorldMap_ShowGoldenBoots(void) {
  struct Actor *object;
  u8 *animation;
  s32 a, id;

  FieldActor_ShowItemIconById(ACTOR_GOLDEN_BOOTS, ITEM_GOLDEN_BOOTS);
  object = Actor_Get(ACTOR_GOLDEN_BOOTS);
  animation = object->sprite;
  animation[17] |= 1;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GOLDEN_BOOTS), 0);
  a = FACING_SOUTH;
  id = ACTOR_GOLDEN_BOOTS;
  Actor_SetPositionAndFacing(id, px(10168), px(9648), a);
  Event_SetFlag(0xa77);
}

void WorldMap_EnterProx(s32 id) { WorldMap_SelectExitByZ(id, 63, 62); }

void WorldMap_EnterYallam(s32 id) { WorldMap_SelectExitByZ(id, 34, 33); }

void WorldMap_EnterDaila(s32 id) { WorldMap_SelectExitByZ(id, 3, 2); }

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

extern s32 gWorldMap_EchoScript[];

s32 Scene_Initialize(void) {
  struct FieldWork *work;

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0 &&
      GameFlag_Get(FLAG_SHIP_SAILABLE) != 0 &&
      gMainRoutineWork.ownerOverride == 0 && gMainRoutineWork.shipX != 0) {
    struct Actor *ship = Actor_GetById(ACTOR_SHIP);

    ship->x.fixed = gMainRoutineWork.shipX;
    ship->z.fixed = gMainRoutineWork.shipZ;
    ship->facing = gMainRoutineWork.shipFacing;
    ship->timer = 0;
    ship->userValue = 0;
  }
  if (gMainRoutineWork.ownerOverride != 0)
    FieldEffect_ScheduleRatioTransition(Q16_ONE, 1);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  work = Ram_HeapSlots->fieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_CENTRE_LINE, 0);
  work->transitionFrames = 16;
  wait(1);
  FieldCamera_SetSpeed(8);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  WorldMap_StartPlacedActors();
  if (GameFlag_Get(FLAG_ALHAFRA_SHIP_GONE))
    Field_SetWorldMapWindowCellTile(0, 240, 336, 288);
  if (GameFlag_Get(FLAG_JUPITER_BEACON_LIT)) {
    Field_SetWorldMapWindowCellTile(0, 224, 208, 256);
    Field_SetWorldMapWindowCellTile(16, 224, 224, 256);
  }
  if (GameFlag_Get(0xa77))
    WorldMap_ShowGoldenBoots();
  switch (gMainRoutineWork.entrance) {
  case WORLD_MAP_AT_GAIA_ROCK:
    if (GameFlag_Get(FLAG_SERPENT_SLAIN))
      GameFlag_Set(0x9ab);
    break;
  case WORLD_MAP_AT_MADRA:
  case WORLD_MAP_BANDIT_BEATEN:
  case WORLD_MAP_BANDIT_LOST:
    if (GameFlag_Get(0xa76))
      WorldMap_FightBanditAndThieves(gMainRoutineWork.entrance);
    break;
  case WORLD_MAP_IDEJIMA_DRIFTING:
    WorldMap_ShowIdejimaDrifting();
    break;
  case WORLD_MAP_TO_AQUA_ROCK:
    WorldMap_CarryPartyToAquaRock();
    break;
  case WORLD_MAP_AT_LOHO:
    if (!Event_GetFlag(FLAG_SHIP_CARRIES_CANNON) && GameFlag_Get(0xa5f) != 0) {
      GameFlag_Set(FLAG_SHIP_CARRIES_CANNON);
      WorldMap_DeliverLohoCannon();
    }
    break;
  case WORLD_MAP_VENUS_COLLAPSE:
    WorldMap_WatchVenusLighthouseCollapse();
    break;
  case WORLD_MAP_DRIFT_NORTH:
    WorldMap_DriftNorthOfContinent();
    break;
  case WORLD_MAP_PIERS_DJINN:
    Party_GivePiersDjinn();
    break;
  case WORLD_MAP_AT_DAILA:
    if (GameFlag_Get(FLAG_DJINNI(DJINNI_ECHO)) == 0) {
      Actor_SetScript(ACTOR_ECHO, gWorldMap_EchoScript);
      WorldMap_MeetEcho();
    }
    break;
  case WORLD_MAP_ALHAFRA_MANOR_VIEW:
    WorldMap_PanToAlhafraManor();
    break;
  case WORLD_MAP_FIRST_VOYAGE:
    WorldMap_TakeTheTiller();
    break;
  case WORLD_MAP_SAIL_TO_DECK:
    WorldMap_SailToShipDeck();
    break;
  case WORLD_MAP_JUPITER_BEACON:
    WorldMap_PlayLighthouseTransition(2);
    FieldActor_SetSpriteColor(Actor_GetById(ACTOR_PIERS), 15);
    Actor_SetPosition(ACTOR_PIERS, 7277, 7300);
    wait(1);
    FieldEvent_SetMessageId((s32)MsgIVeHeard);
    // "I've heard tales of this..."
    Event_ShowMessage(ACTOR_PIERS, 0);
    FieldEvent_Wait(20);
    // "But I had no idea it would be so spectacular."
    Event_ShowMessage(ACTOR_PIERS, 0);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Event_SetDestination(SCENE_JUPITER_LIGHTHOUSE_AERIE_BEACON, 9);
    break;
  case WORLD_MAP_DEBUG_FLIGHT:
    gMainRoutineWork.ownerOverride = 1;
    Event_SetFlag(FLAG_SHIP_SAILABLE);
    Event_SetFlag(FLAG_SHIP_FLIES);
    Event_SetFlag(FLAG_DEBUG_STEPS_OFF);
    Event_SetFlag(0x11d);
    Event_SetDestination(SCENE_WORLD_MAP, WORLD_MAP_AT_NORTHERN_REACHES);
    break;
  case WORLD_MAP_DEBUG_SAILING:
    GameFlag_Set(FLAG_DEBUG_STEPS_OFF);
    GameFlag_Set(0x11d);
    break;
  }
  return 0;
}

void Menu_AnimateSelectionToEntry(s32 a, s32 b);

s32 Scene_PrepareMap(void) { return 0; }

/// The djinn tutorial. With flag 0x175 set each message is offset by
/// MsgDjinnGiveTutorialB less A.
// As The Broken Seal's WorldMap_ShowDjinnTutorial.
void WorldMap_ExplainGivingDjinn(void) {
  WorldMap_ReloadPendingAreaGraphics();
  FieldEffect_ScheduleRatioTransition(Q16_ONE, 6);
  Event_WaitForZoomTransition();
  WorldMap_Freeze();
  Actor_RunRepeatedMotion(ACTOR_ECHO, 2);
  FieldEvent_SetMessageId(
      Event_GetFlag(FLAG_ALREADY_KNOWS_DJINN) *
          ((s32)MsgDjinnGiveTutorialB - (s32)MsgDjinnGiveTutorialA) +
      (s32)MsgAfterBringingA);
  Event_ShowMessage(ACTOR_ECHO, 0);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_MENU_CURSOR_MOVE);
  Menu_AnimateSelectionToEntry(0, 2);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_A);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  FieldActor_Jump(ACTOR_ECHO, 4, 30);
  {
    s32 flag = Event_GetFlag(FLAG_ALREADY_KNOWS_DJINN);
    s32 base = (s32)MsgNextILl;

    FieldEvent_SetMessageId(
        flag * ((s32)MsgDjinnGiveTutorialB - (s32)MsgDjinnGiveTutorialA) +
        base);
  }
  Event_ShowMessage(ACTOR_ECHO, 0);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_A);
  Event_SetFlag(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  FieldEvent_Wait(30);
  Field_RequestBattleFromEncounterTable(9, 5);
}

/// The bandit and the thieves from Vault, after a linked game, want Isaac.
void WorldMap_FightBanditAndThieves(s32 entrance) {
  s32 south, north, facingSouth, southB, northB;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (entrance == WORLD_MAP_AT_MADRA) {
    north = FACING_NORTH;
    Actor_SetPositionAndFacing(ACTOR_BANDIT, px(10168), px(9000), north);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    WorldMap_Freeze();
    Event_SetMessageId((s32)MsgHeyWaitA);
    // "Hey! Wait a minute!"
    FieldEvent_ShowMessageThenWait(ACTOR_BANDIT, 0, 5);
    WorldMap_ReloadPendingAreaGraphics();
    FieldEffect_ScheduleRatioTransition(Q16_ONE * 31 / 20, 16);
    Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    Actor_WalkByAndWait(ACTOR_FELIX, 0, 32);
    Actor_FaceDirection(ACTOR_FELIX, north, 0);
    Event_WaitForZoomTransition();
    Actor_WaitForMoveById(ACTOR_FELIX);
    WorldMap_Freeze();
    Actor_LaunchFromLeader(ACTOR_KRADEN, 16, 0, north);
    Actor_WaitForScriptEndById(ACTOR_KRADEN);
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, north);
    south = FACING_SOUTH;
    Event_Wait(10);
    Actor_SetPositionAndFacing(ACTOR_BANDIT, px(10168), px(9600), south);
    Actor_SetPositionAndFacing(ACTOR_THIEF_A, px(10168), px(9600), south);
    Actor_SetPositionAndFacing(ACTOR_THIEF_B, px(10168), px(9600), south);
    Actor_WalkBy(ACTOR_BANDIT, 0, 16);
    Actor_WalkBy(ACTOR_THIEF_A, -16, 16);
    Actor_WalkByAndWait(ACTOR_THIEF_B, 16, 16);
    Actor_FaceDirection(ACTOR_BANDIT, FACING_SOUTH_SOUTHEAST, 0);
    Actor_FaceDirection(ACTOR_THIEF_A, FACING_SOUTH_SOUTHEAST, 0);
    Actor_FaceDirection(ACTOR_THIEF_B, FACING_SOUTH_SOUTHEAST, 0);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    Actor_WaitForMoveById(ACTOR_BANDIT);
    Actor_WaitForMoveById(ACTOR_THIEF_A);
    Actor_WaitForMoveById(ACTOR_THIEF_B);
    WorldMap_Freeze();
    // "Who are you?"
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
    Actor_RunRepeatedMotion(ACTOR_BANDIT, 2);
    Event_Wait(5);
    // "You were talking about Isaac, weren't you?"
    FieldEvent_ShowMessageThenWait(ACTOR_BANDIT, 0, 5);
    Actor_TurnToFaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_THIEF_A, 2);
    Actor_FaceDirection(ACTOR_FELIX, north, 0);
    Actor_FaceDirection(ACTOR_KRADEN, north, 0);
    Event_Wait(5);
    // "Don't play dumb... We all heard you."
    FieldEvent_ShowMessageThenWait(ACTOR_THIEF_A, 0, 5);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    // "Yes, we know Isaac. What about him?"
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
    Actor_ShowEmote(ACTOR_THIEF_B, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    // "Well, why don't you tell us where we can find him?"
    FieldEvent_ShowMessageThenWait(ACTOR_THIEF_B, 0, 5);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "Why should we tell you anything?"
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
    Actor_ShowEmote(ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    // "Isaac took good care of us when we were in Vault, and we'd like to..."
    FieldEvent_ShowMessageThenWait(ACTOR_BANDIT, 0, 5);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "Ah, yes... Well... I'm afraid we don't know where you can find Isaac..."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
    // "Oh, is that so? Well, I guess we'll just have to give you the reward..."
    FieldEvent_ShowMessageThenWait(ACTOR_THIEF_A, 0, 5);
    Actor_TurnToFaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
    Event_Wait(30);
    // "You've got a grudge against Isaac, and you're going to take it out..."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
    Actor_ShowEmote(ACTOR_THIEF_B, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    {
      s32 facing = FACING_NORTH;

      Actor_FaceDirection(ACTOR_FELIX, facing, 0);
      Actor_FaceDirection(ACTOR_KRADEN, facing, 0);
    }
    // "Oh yeah, you could call it a grudge... A serious grudge."
    FieldEvent_ShowMessageThenWait(ACTOR_THIEF_B, 0, 5);
    Actor_WalkBy(ACTOR_FELIX, 8, -8);
    Actor_WalkBy(ACTOR_KRADEN, -8, 8);
    Actor_WaitForMoveById(ACTOR_FELIX);
    Actor_WaitForMoveById(ACTOR_KRADEN);
    {
      s32 facing = FACING_NORTH;

      Actor_FaceDirection(ACTOR_FELIX, facing, 0);
      Actor_FaceDirection(ACTOR_KRADEN, facing, 0);
    }
    {
      s32 emote = EMOTE_IN_FRONT | EMOTE_WINCE;

      Actor_ShowEmote(ACTOR_FELIX, emote, 30);
    }
    Actor_SetAnimationAndWait(ACTOR_BANDIT, 3);
    Event_Wait(10);
    // "But don't worry... We're just going to give you a little sample of..."
    FieldEvent_ShowMessageThenWait(ACTOR_BANDIT, 0, 5);
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    FieldEvent_SetAfterBattleDestination(SCENE_WORLD_MAP,
                                         WORLD_MAP_BANDIT_BEATEN);
    Event_SetDefeatDestination(SCENE_WORLD_MAP, WORLD_MAP_BANDIT_LOST);
    WorldMap_ReloadPendingAreaGraphics();
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Field_RequestBattleFromEncounterTable(102, 3);
  } else if (entrance == WORLD_MAP_BANDIT_LOST) {
    Event_ClearFlag(0xa76);
    facingSouth = FACING_SOUTH;
    Actor_SetPositionAndFacing(ACTOR_BANDIT, px(10168), px(9616), facingSouth);
    Actor_SetPositionAndFacing(ACTOR_THIEF_A, px(10152), px(9616), facingSouth);
    Actor_SetPositionAndFacing(ACTOR_THIEF_B, px(10184), px(9616), facingSouth);
    {
      s32 a3 = 0xe000;

      Actor_SetPositionAndFacing(ACTOR_KRADEN, px(10152), px(9664), a3);
    }
    Actor_SetPositionAndFacing(ACTOR_FELIX, px(10168), px(9648),
                               FACING_NORTHWEST);
    EventCamera_FollowActor(ACTOR_FELIX, 0);
    FieldEffect_ScheduleRatioTransition(Q16_ONE * 31 / 20, 1);
    Actor_SetAnimation(ACTOR_FELIX, 19);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FELIX), 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    WorldMap_Freeze();
    Event_SetMessageId((s32)MsgItLooksLike);
    Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
    Event_Wait(5);
    Actor_ShowEmote(ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "It looks like Isaac runs with a pretty feeble crowd..."
    FieldEvent_ShowMessageThenWait(ACTOR_BANDIT, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_THIEF_A, facingSouth);
    Event_Wait(10);
    // "Well, we have been working out. Maybe Isaac won't be so tough this..."
    FieldEvent_ShowMessageThenWait(ACTOR_THIEF_A, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_THIEF_B, facingSouth);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_THIEF_B, 2);
    Event_Wait(5);
    // "So you think we just got a lot stronger?"
    FieldEvent_ShowMessageThenWait(ACTOR_THIEF_B, 0, 5);
    Actor_SetAnimationAndWait(ACTOR_THIEF_A, 3);
    Event_Wait(10);
    Actor_FaceDirectionAndWait(ACTOR_BANDIT, facingSouth);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_BANDIT, 2);
    Event_Wait(5);
    // "You let Isaac know we're looking for him!"
    FieldEvent_ShowMessageThenWait(ACTOR_BANDIT, 0, 5);
    Actor_SetAnimationAndWait(ACTOR_BANDIT, 3);
    Event_Wait(10);
    // "The band of thieves he busted in Vault are coming to repay the favor!"
    FieldEvent_ShowMessageThenWait(ACTOR_BANDIT, 0, 5);
    Actor_SetAnimationAndWait(ACTOR_THIEF_A, 3);
    Event_Wait(10);
    // "And we're not messing around!"
    FieldEvent_ShowMessageThenWait(ACTOR_THIEF_A, 0, 5);
    Actor_ShowEmote(ACTOR_THIEF_B, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "Be sure you tell him! And send him this way!"
    FieldEvent_ShowMessageThenWait(ACTOR_THIEF_B, 0, 5);
    Actor_SetAnimationAndWait(ACTOR_BANDIT, 3);
    Event_Wait(10);
    Event_Wait(40);
    Event_SetDestination(
        ((struct SavedDestination *)&gMainRoutineWork)->scene,
        ((struct SavedDestination *)&gMainRoutineWork)->entrance);
    gFieldWork->transition = 0;
  } else {
    Event_ClearFlag(0xa76);
    southB = FACING_SOUTH;
    Actor_SetPositionAndFacing(ACTOR_BANDIT, px(10168), px(9632), southB);
    northB = FACING_NORTH;
    Actor_SetPositionAndFacing(ACTOR_THIEF_A, px(10152), px(9632), southB);
    Actor_SetPositionAndFacing(ACTOR_THIEF_B, px(10184), px(9632), southB);
    Actor_SetPositionAndFacing(ACTOR_KRADEN, px(10160), px(9680), northB);
    Actor_SetPositionAndFacing(ACTOR_FELIX, px(10176), px(9680), northB);
    EventCamera_FollowActor(ACTOR_FELIX, 0);
    FieldEffect_ScheduleRatioTransition(Q16_ONE * 3 / 2, 1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    WorldMap_Freeze();
    Event_SetMessageId((s32)MsgWeLostAgain);
    Actor_RunRepeatedMotion(ACTOR_BANDIT, 2);
    Event_Wait(5);
    Actor_ShowEmote(ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "We lost again..."
    FieldEvent_ShowMessageThenWait(ACTOR_BANDIT, 0, 5);
    Actor_ShowEmote(ACTOR_THIEF_A, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "So, boss... You, uh... You think maybe it was a mistake going after..."
    FieldEvent_ShowMessageThenWait(ACTOR_THIEF_A, 0, 5);
    Actor_ShowEmote(ACTOR_THIEF_B, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "This means we're going to jail again, doesn't it?"
    FieldEvent_ShowMessageThenWait(ACTOR_THIEF_B, 0, 5);
    Actor_RunRepeatedMotion(ACTOR_BANDIT, 2);
    Event_Wait(5);
    // "Aw... Why do we have to go back to jail. I hate it there..."
    FieldEvent_ShowMessageThenWait(ACTOR_BANDIT, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_THIEF_A, FACING_EAST);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_THIEF_A, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "It's not like we really hurt anyone... After all, we lost!"
    FieldEvent_ShowMessageThenWait(ACTOR_THIEF_A, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_BANDIT, FACING_WEST);
    Actor_SetAnimationAndWait(ACTOR_BANDIT, 3);
    Event_Wait(10);
    // "Yeah... And it's not like we stole anything...this time..."
    FieldEvent_ShowMessageThenWait(ACTOR_BANDIT, 0, 5);
    Actor_FaceDirectionAndWait(ACTOR_THIEF_B, FACING_WEST);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_THIEF_B, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "We're not gonna have to go to jail, are we?"
    FieldEvent_ShowMessageThenWait(ACTOR_THIEF_B, 0, 5);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    Actor_FaceDirection(ACTOR_BANDIT, southB, 0);
    Actor_FaceDirection(ACTOR_THIEF_A, southB, 0);
    Actor_FaceDirection(ACTOR_THIEF_B, southB, 0);
    // "You're thieves!"
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
    Actor_SetAnimation(ACTOR_BANDIT, 3);
    Actor_SetAnimation(ACTOR_THIEF_A, 3);
    Actor_SetAnimationAndWait(ACTOR_THIEF_B, 3);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    // "If we asked around, we could probably find a dozen reasons to have..."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
    Actor_ShowEmote(ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_THIEF_A, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_THIEF_B, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    Event_Wait(40);
    Actor_TurnToFaceEachOther(ACTOR_BANDIT, ACTOR_THIEF_A, 0);
    Actor_FaceDirection(ACTOR_THIEF_B, FACING_WEST, 0);
    Actor_ShowEmote(ACTOR_THIEF_B, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
    Actor_ShowEmote(ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Actor_FaceDirectionAndWait(ACTOR_BANDIT, southB);
    Event_Wait(20);
    // "Are you guys, like, wandering sheriffs or something?"
    Event_OpenMessage(20, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
      Actor_ShowEmote(ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
      // "Because we could be really useful to you if you are. We know all..."
      FieldEvent_ShowMessageThenWait(ACTOR_BANDIT, 0, 5);
      (gFieldWork->message)++;
    } else {
      Actor_ShowEmote(ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
      (gFieldWork->message)++;
      // "And even if you're not, we could be really useful to you......"
      FieldEvent_ShowMessageThenWait(ACTOR_BANDIT, 0, 5);
    }
    Actor_TurnToFaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    // "You can't honestly believe we would let you join us!"
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
    Actor_RunRepeatedMotion(ACTOR_THIEF_A, 2);
    Actor_FaceDirection(ACTOR_THIEF_A, FACING_SOUTH_SOUTHEAST, 0);
    Actor_FaceDirection(ACTOR_THIEF_B, FACING_SOUTH_SOUTHWEST, 0);
    Event_Wait(5);
    // "Hey, come on! We haven't done anything wrong, really! Not yet..."
    FieldEvent_ShowMessageThenWait(ACTOR_THIEF_A, 0, 5);
    Actor_SetAnimationAndWait(ACTOR_THIEF_B, 4);
    // "Yeah! We didn't have enough time to do anything bad before we ran..."
    FieldEvent_ShowMessageThenWait(ACTOR_THIEF_B, 0, 5);
    Actor_RunRepeatedMotion(ACTOR_BANDIT, 2);
    WorldMap_ShowGoldenBoots();
    Event_Wait(5);
    Actor_ShowEmote(ACTOR_BANDIT, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    // "Yeah, so give us a break!"
    FieldEvent_ShowMessageThenWait(ACTOR_BANDIT, 0, 5);
    Actor_TurnToFaceEachOther(ACTOR_BANDIT, ACTOR_THIEF_A, 0);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_THIEF_A, 3);
    Actor_SetAnimationAndWait(ACTOR_BANDIT, 3);
    Event_Wait(10);
    Actor_TurnToFaceEachOther(ACTOR_BANDIT, ACTOR_THIEF_B, 0);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_THIEF_B, 3);
    Actor_SetAnimationAndWait(ACTOR_BANDIT, 3);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_BANDIT, 2);
    Actor_FaceDirection(ACTOR_THIEF_A, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_THIEF_B, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(ACTOR_BANDIT, FACING_SOUTH);
    Event_Wait(5);
    // "So, uh... Bye! See you later!"
    FieldEvent_ShowMessageThenWait(ACTOR_BANDIT, 0, 5);
    Actor_WalkBy(ACTOR_BANDIT, -32, 0);
    Actor_WalkBy(ACTOR_THIEF_A, -32, 0);
    Actor_WalkByAndWait(ACTOR_THIEF_B, -32, 0);
    Actor_WalkBy(ACTOR_BANDIT, -64, -64);
    Actor_WalkBy(ACTOR_THIEF_A, -64, -64);
    Actor_WalkByAndWait(ACTOR_THIEF_B, -64, -64);
    Actor_SetPosition(ACTOR_BANDIT, 0, 0);
    Actor_SetPosition(ACTOR_THIEF_A, 0, 0);
    Actor_SetPosition(ACTOR_THIEF_B, 0, 0);
    Event_Wait(10);
    Actor_FaceDirectionAndWait(ACTOR_BANDIT, FACING_SOUTH);
    Event_Wait(10);
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
    Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_WEST);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
    Event_Wait(10);
    // "Will you promise to stop following Isaac and trying to get revenge..."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 5);
    Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_KRADEN, 1.2);
    Actor_SetAnimation(ACTOR_KRADEN, 2);
    {
      struct Actor *object = Actor_Get(ACTOR_FELIX);

      if (object != NULL)
        Actor_SetDestination(ACTOR_KRADEN, *(s16 *)((u8 *)object + 10),
                             *(s16 *)((u8 *)object + 18));
    }
    Actor_WaitForMoveById(ACTOR_KRADEN);
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    WorldMap_ReloadPendingAreaGraphics();
    FieldEffect_ScheduleRatioTransition(Q16_ONE, 15);
    Event_End();
  }
}

extern u32 gWorldMap_LohoWorkerLeaveScriptA[];
extern u32 gWorldMap_LohoWorkerLeaveScriptB[];
extern u32 gWorldMap_LohoWorkerLeaveScriptC[];

void WorldMap_DeliverLohoCannon(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_Get(ACTOR_LOHO_WORKER_A)->motionFlags = ACTOR_FALLS;
  Actor_Get(ACTOR_LOHO_WORKER_B)->motionFlags = ACTOR_FALLS;
  Actor_Get(ACTOR_LOHO_WORKER_C)->motionFlags = ACTOR_FALLS;
  {
    s32 facing = FACING_NORTHEAST;

    Actor_SetPositionAndFacing(ACTOR_LOHO_WORKER_A, px(8913), px(5853), facing);
    Actor_SetPositionAndFacing(ACTOR_LOHO_WORKER_B, px(8929), px(5869), facing);
    Actor_SetPositionAndFacing(ACTOR_LOHO_WORKER_C, px(8929), px(5885), facing);
  }
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_LOHO_WORKER_A, 0.2);
  Actor_SetSpeed(ACTOR_LOHO_WORKER_B, 0.2);
  Actor_SetSpeed(ACTOR_LOHO_WORKER_C, 0.2);
  Actor_WalkBy(ACTOR_LOHO_WORKER_A, 10, 0);
  Actor_WalkBy(ACTOR_LOHO_WORKER_B, 10, 0);
  Actor_WalkBy(ACTOR_LOHO_WORKER_C, 10, 0);
  Event_WaitTransitionFrames();
  FieldEffect_ScheduleRatioTransition(Q16_ONE * 12 / 10, 6);
  Event_WaitForZoomTransition();
  WorldMap_Freeze();
  FieldActor_WaitForMoveById(ACTOR_LOHO_WORKER_A);
  FieldEvent_Wait(40);
  Actor_SetAnimation(ACTOR_LOHO_WORKER_A, 3);
  FieldEvent_SetMessageId((s32)MsgYouDonT);
  Actor_FaceDirection(ACTOR_LOHO_WORKER_B, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_LOHO_WORKER_C, FACING_NORTH_NORTHEAST, 0);
  // "You don't have anything to worry about. We got the cannon onto your..."
  Event_ShowMessage(ACTOR_LOHO_WORKER_A, 0);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(30);
  // "Now, we're going to start excavating the ground north of the wall..."
  Event_ShowMessage(ACTOR_LOHO_WORKER_A, 0);
  Actor_SetAnimation(ACTOR_LOHO_WORKER_A, 3);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(60);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_LOHO_WORKER_C);
  Actor_SetSpeed(ACTOR_LOHO_WORKER_A, 0.4);
  Actor_SetSpeed(ACTOR_LOHO_WORKER_B, 0.4);
  Actor_SetSpeed(ACTOR_LOHO_WORKER_C, 0.4);
  Actor_SetScript(ACTOR_LOHO_WORKER_A, gWorldMap_LohoWorkerLeaveScriptA);
  FieldEvent_Wait(40);
  Actor_SetScript(ACTOR_LOHO_WORKER_B, gWorldMap_LohoWorkerLeaveScriptB);
  Actor_SetScript(ACTOR_LOHO_WORKER_C, gWorldMap_LohoWorkerLeaveScriptC);
  FieldActor_WaitForScriptEndById(ACTOR_LOHO_WORKER_A);
  FieldActor_WaitForScriptEndById(ACTOR_LOHO_WORKER_B);
  FieldActor_WaitForScriptEndById(ACTOR_LOHO_WORKER_C);
  WorldMap_ReloadPendingAreaGraphics();
  FieldEvent_Wait(30);
  FieldEffect_ScheduleRatioTransition(Q16_ONE, 6);
  Event_End();
}

/// A tutorial message: the second set (from MsgDjinnGiveTutorialB) when
/// secondSet is 1.
#define TUTORIAL_SET_MESSAGE(secondSet, base)                                  \
  ((secondSet) * ((s32)MsgDjinnGiveTutorialB - (s32)MsgDjinnGiveTutorialA) +   \
   (s32)(base))

/// A tutorial message: the second set while flag 0x175 is set.
#define TUTORIAL_MESSAGE(base)                                                 \
  TUTORIAL_SET_MESSAGE(Event_GetFlag(FLAG_ALREADY_KNOWS_DJINN), base)

/// The first Djinni, Echo, joins south of Daila, however often he is refused.
void WorldMap_MeetEcho(void) {
  struct Actor *djinni;
  struct Actor *leader;
  s32 x;
  s32 z;
  s32 owner;
  s32 count;
  s32 i;
  s32 flag;
  // Holds 0 for its whole life; see its one use.
  s32 zero = 0;

  djinni = Actor_GetById(ACTOR_ECHO);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  x = (leader->x.fixed - px(10120)) / 2 + px(10120);
  z = (leader->z.fixed - px(8356)) / 2 + px(8356);
  flag = Event_GetFlag(FLAG_DJINN_LESSON);
  if (flag == 0) {
    if (gInput.held & KEY_UP)
      Event_SetFlag(FLAG_ALREADY_KNOWS_DJINN);
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Party_RemoveMember(0);
    Party_RemoveMember(1);
    Party_RemoveMember(2);
    Party_RemoveMember(3);
    Party_RemoveMember(4);
    Party_RemoveMember(5);
    Party_RemoveMember(6);
    Party_RemoveMember(7);
    Party_AddMember(4);
    Party_AddMember(5);
    Party_AddMember(6);
    owner = gMainRoutineWork.leaderActorId;
    Party_RestoreHpPp(1);
    Event_SetFlag(FLAG_DJINN_LESSON);
    Event_Start();
    Event_ResolvePendingActions(0);
    Djinn_Give(owner, 0, 7);
    DjinnRecovery_AddStandby(owner, 0, 7);
    WorldMap_Freeze();
    Actor_FaceActorById(owner, ACTOR_ECHO, 0);
    Event_Wait(10);
    FieldActor_ShowEmote(owner, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    {
      s32 one = 1;
      *(u16 *)&djinni->userValue = one;
    }
    Actor_FaceActorById(ACTOR_ECHO, owner, 0);
    wait(16);
    flag = Event_GetFlag(FLAG_ALREADY_KNOWS_DJINN);
    Event_SetMessageId(TUTORIAL_SET_MESSAGE(flag, MsgHey));
    i = 15;
    Event_ShowMessageAndWait(ACTOR_ECHO, 0);
    WorldMap_ReloadPendingAreaGraphics();
    FieldEffect_ScheduleRatioTransition(Q16_ONE * 12 / 10, 6);
    Event_WaitForZoomTransition();
    WorldMap_Freeze();
    djinni->motionFlags = ACTOR_FALLS;
    djinni->gravity = px(1) / 4;
    djinni->speedLimit = px(1);
    djinni->acceleration = px(1);
    djinni->velocityY = 0;
    djinni->groundHeight = 0;
    /* FAKEMATCH(barrier): an empty asm that reads and writes all of djinni. */
    asm("" : "+m"(*djinni));
    FieldActor_SetMoveTarget(djinni, 10120, 0, 8356);
    while (i >= 0) {
      djinni->scaleX += Q16_ONE / 32;
      djinni->scaleY += Q16_ONE / 32;
      wait(1);
      i--;
    }
    Actor_FaceActorById(ACTOR_ECHO, owner, 0);
    Actor_FaceActorById(owner, ACTOR_ECHO, 0);
    wait(16);
    djinni->update = 0;
    Actor_SetSpriteColorMode(djinni, 0);
    djinni->gravity = px(1);
    FieldMessage_ShowAtActor(ACTOR_ECHO, 0);
    if (Event_AskYesNoWithActorAnimation(owner, 0) == 1) {
      Actor_Jump(ACTOR_ECHO, 2, 20);
      Actor_Jump(ACTOR_ECHO, 2, 20);
      Event_SetMessageId(TUTORIAL_MESSAGE(MsgWellYouWeren));
      FieldMessage_ShowAtActor(ACTOR_ECHO, 0);
      if (Event_AskYesNoWithActorAnimation(owner, 0) == 1) {
        Event_SetMessageId(TUTORIAL_MESSAGE(MsgWellYouWeren + 1));
        Event_ShowMessageAndWait(ACTOR_ECHO, 0);
      } else {
        Event_SetMessageId(TUTORIAL_MESSAGE(MsgWellYouWeren + 2));
        Event_ShowMessageAndWait(ACTOR_ECHO, 0);
      }
    } else {
      Event_SetMessageId(TUTORIAL_MESSAGE(MsgSeeYouKnow));
      Event_ShowMessageAndWait(ACTOR_ECHO, 0);
    }
    FieldActor_ShowEmote(owner, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    Actor_Jump(ACTOR_ECHO, 2, 20);
    Actor_Jump(ACTOR_ECHO, 2, 20);
    Event_SetMessageId(TUTORIAL_MESSAGE(MsgEvenIKnow));
    i = 15;
    Event_ShowMessageAndWait(ACTOR_ECHO, 0);
    FieldActor_ShowEmote(owner, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    Event_ShowMessageAndWait(ACTOR_ECHO, 0);
    FieldActor_ShowEmote(owner, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    Event_ShowMessageAndWait(ACTOR_ECHO, 0);
    Actor_RunRepeatedMotion(owner, 2);
    Event_ShowMessageAndWait(ACTOR_ECHO, 0);
    Actor_Jump(ACTOR_ECHO, 2, 30);
    Event_ShowMessageAndWait(ACTOR_ECHO, 0);
    djinni->motionFlags = 0;
    FieldActor_SetMoveTargetFixed(djinni, x, px(16), z);
    while (i >= 0) {
      *(u16 *)&djinni->facing += 0x1000;
      wait(1);
      i--;
    }
    Actor_SetAnimationById(owner, 1);
    Event_ShowMessageAndWait(ACTOR_ECHO, 0);
    djinni->motionFlags = ACTOR_FALLS;
    djinni->velocityY = 0;
    djinni->groundHeight = 0;
    i = 7;
    while (i >= 0) {
      *(u16 *)&djinni->facing += 0x1000;
      wait(1);
      i--;
    }
    Actor_SetAnimationById(owner, 22);
    Event_ShowMessageAndWait(ACTOR_ECHO, 0);
    Actor_ShowEmoteById(ACTOR_ECHO, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    Actor_FaceActorById(ACTOR_ECHO, owner, 0);
    Actor_RunRepeatedMotion(ACTOR_ECHO, 2);
    Event_ShowMessageAndWait(ACTOR_ECHO, 0);
    Actor_Jump(ACTOR_ECHO, 2, 30);
    FieldMessage_ShowAtActor(ACTOR_ECHO, 0);
    count = 0;
  plead:
    {
      i = Event_AskYesNoWithActorAnimation(owner, 0);

      if (count == 7) {
        if (i == 0) {
          Actor_Jump(ACTOR_ECHO, 2, 20);
          Actor_Jump(ACTOR_ECHO, 2, 20);
          Event_SetMessageId(TUTORIAL_MESSAGE(MsgOhAllRight));
          Event_ShowMessageAndWait(ACTOR_ECHO, 0);
          goto joined;
        }
      } else if (i == 1) {
        Actor_Jump(ACTOR_ECHO, 2, 20);
        Actor_Jump(ACTOR_ECHO, 2, 20);
        Event_SetMessageId(count + TUTORIAL_MESSAGE(MsgOhComeOn));
        FieldMessage_ShowAtActor(ACTOR_ECHO, 0);
        count++;
        goto plead;
      }
    }
    Actor_SetAnimationById(owner, 22);
    Actor_Jump(ACTOR_ECHO, 2, 20);
    Actor_Jump(ACTOR_ECHO, 4, 20);
    Event_SetMessageId(TUTORIAL_MESSAGE(MsgIMSo));
    Event_ShowMessageAndWait(ACTOR_ECHO, 0);
  joined:
    MessageArgument_Push(ABILITY_ECHO, MESSAGE_ARGUMENT_ABILITY);
    Sound_Play(MUS_JOINED);
    FieldMessage_Show(TUTORIAL_MESSAGE(MsgTheVenusDjinni), 3);
    Event_SetMessageId(TUTORIAL_MESSAGE(MsgTheVenusDjinni + 1));
    Actor_Jump(ACTOR_ECHO, 2, 20);
    FieldMessage_ShowAtActor(ACTOR_ECHO, 0);
    if (Event_AskYesNoWithActorAnimation(owner, 0) == 1)
      goto refused;
    if (Event_GetFlag(FLAG_ALREADY_KNOWS_DJINN))
      Sound_Play(MUS_SONG_GAME);
    else
      Sound_Play(MUS_BRIGGS_STEALS_SHIP);
    goto tutorial;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_MoveToObject(ACTOR_ECHO, owner);
  djinni->velocityY = px(10);
  FieldActor_SetMoveTargetFixed(djinni, x, 0, z);
  Event_Wait(30);
  WorldMap_Freeze();
  Actor_FaceActorById(ACTOR_ECHO, owner, 0);
  Actor_FaceActorById(owner, ACTOR_ECHO, 0);
  Actor_SetAnimationById(owner, 22);
  Event_SetMessageId(TUTORIAL_MESSAGE(MsgSeeWeDjinn));
  Actor_Jump(ACTOR_ECHO, 2, 20);
  Actor_Jump(ACTOR_ECHO, 2, 20);
  Event_ShowMessageAndWait(ACTOR_ECHO, 0);
  Actor_RunRepeatedMotion(ACTOR_ECHO, 2);
  Event_ShowMessageAndWait(ACTOR_ECHO, 0);
  Sound_Play(SE_MENU_CURSOR_MOVE);
  Menu_AnimateSelectionToEntry(0, 2);
  Event_SetFlag(FLAG_DJINN_LESSON_PART_A);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  Event_SetMessageId(TUTORIAL_MESSAGE(MsgSoWeGo));
  FieldActor_SetMoveTarget(djinni, 10120, 0, 8356);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_ECHO, 0);
  Actor_FaceActorById(ACTOR_ECHO, owner, 0);
  Event_ShowMessageAndWait(ACTOR_ECHO, 0);
  FieldMessage_ShowAtActor(ACTOR_ECHO, 0);
  if (Event_AskYesNoWithActorAnimation(owner, 0) != 1)
    goto refused;
  Actor_SetAnimationById(owner, 22);
  Actor_RunRepeatedMotion(ACTOR_ECHO, 2);
  Event_SetMessageId(TUTORIAL_MESSAGE(MsgHmmmmDoYou));
  FieldMessage_ShowAtActor(ACTOR_ECHO, 0);
  if (Event_AskYesNoWithActorAnimation(owner, 0) == 1)
    goto refused;
  Event_ShowMessageAndWait(ACTOR_ECHO, 0);
  Actor_WalkToAndWait(ACTOR_ECHO, FIELD_PIXEL(x), FIELD_PIXEL(z));
tutorial:
  WorldMap_ExplainGivingDjinn();
  WorldMap_ReloadPendingAreaGraphics();
  return;
refused:
  Actor_SetAnimationById(owner, 22);
  Event_SetMessageId(TUTORIAL_MESSAGE(MsgWellThatS));
  Actor_Jump(ACTOR_ECHO, 2, 20);
  Actor_Jump(ACTOR_ECHO, 2, 20);
  Event_ShowMessageAndWait(ACTOR_ECHO, 0);
  Actor_ShowEmoteById(ACTOR_ECHO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessageAndWait(ACTOR_ECHO, 0);
  Event_SetFlag(FLAG_DJINN_LESSON_PART_A);
  Event_SetFlag(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  Actor_Jump(ACTOR_ECHO, 2, 20);
  Event_ShowMessageAndWait(ACTOR_ECHO, 0);
  WorldMap_ReloadPendingAreaGraphics();
  FieldDjinn_JoinParty(9, zero, 7);
  Scene_PlayMusic();
  Event_End();
  Event_ClearFlag(FLAG_DJINN_LESSON);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_A);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_B);
}

extern u32 gWorldMap_CollapseDustScript[];

/// Spawns object 285 at a random spot with a random scale and starts its
/// script.
void WorldMap_SpawnCollapseDust(void) {
  struct Actor *obj;
  struct FieldSprite *sprite;
  u8 *motionFlags;
  s32 x;
  s32 z;
  s32 scale;

  x = (Random_Below(40)) << 16;
  z = (Random_Below(30)) << 16;
  obj = (FieldActor_Create(285, x + px(1976), 0, z + px(7260)));
  if (obj != NULL) {
    sprite = obj->sprite;
    scale = ((Random16() << 15) >> 16) + 0x13333;
    sprite->nextFrame = 0;
    sprite->priority = 2;
    motionFlags = &obj->motionFlags;
    *motionFlags = 0;
    obj->scaleX = scale;
    obj->scaleY = scale;
    Import_ActorSelectAnimation(obj, 1);
    Actor_StartScript(obj, gWorldMap_CollapseDustScript);
  }
}

static __inline__ void PosSet(s32 id, s32 x, s32 z) {
  Actor_SetPosition(id, x, z);
}

void WorldMap_ShiftActor(s32 id, s32 dx, s32 dz) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL)
    PosSet(id, *(s16 *)((u8 *)object + 10) + dx,
           *(s16 *)((u8 *)object + 18) + dz);
}

void WorldMap_SplitIdejima(s32 a, s32 b, s32 dx, s32 dz) {
  FieldWorldMap_SetWindowCellTileAndDraw(a, b, 48, 224);
  WorldMap_ShiftActor(ACTOR_JENNA, dx, dz);
  WorldMap_ShiftActor(ACTOR_IDEJIMA_KRADEN, dx, dz);
  WorldMap_ShiftActor(ACTOR_ALEX, dx, dz);
  WorldMap_ShiftActor(ACTOR_IDEJIMA, dx, dz);
  FieldWorldMap_SetWindowCellTileAndDraw(a + 16, b, 64, 224);
}

void WorldMap_WatchVenusLighthouseCollapse(void) {
  WorldMap_PlayLighthouseTransition(1);
  FieldEvent_SetMessageId((s32)MsgTheyVeDone);
  // "They've done it, Jenna! They've lit the beacon!"
  Event_ShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
  MapGrid_SetShake(4, 4, 1);
  FieldEvent_Wait(30);
  // "What's happening, Kraden!? Why won't the ground stop shaking?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  WorldMap_ReloadPendingAreaGraphics();
  Event_PlaySound(SE_CRASH);
  PaletteFade_ApplySource(RGB_WHITE, 0);
  PaletteFade_ApplyTarget(Q16_ONE, 0);
  PaletteFade_Interpolate(60);
  Task_AddFrame(WorldMap_SpawnCollapseDust, TASK_PASS_VBLANK);
  MapGrid_SetShake(5, 5, 1);
  FieldEvent_Wait(30);
  MapGrid_SetShake(7, 6, 1);
  FieldEvent_Wait(30);
  WorldMap_Freeze();
  MapGrid_SetShake(3, 3, 1);
  // "Strange... I think it might actually be getting worse!"
  Event_ShowMessage(ACTOR_ALEX, 0);
  WorldMap_ReloadPendingAreaGraphics();
  MapGrid_SetShake(7, 7, 1);
  FieldCamera_MoveTo(1988, CAMERA_KEEP, 7553, 1);
  FieldEffect_ScheduleRatioTransition(Q16_ONE * 3 / 5, 60);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(30);
  WorldMap_Freeze();
  // "I think the lighthouse is collapsing! Look! The ground around it is..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  // "Mercury Lighthouse didn't react this way... What makes Venus different?"
  Event_ShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
  WorldMap_ReloadPendingAreaGraphics();
  Event_PlaySound(SE_CRASH);
  WorldMap_SplitIdejima(0, 128, 0, 0);
  Actor_SetAnimation(ACTOR_JENNA, 19);
  Actor_SetAnimation(ACTOR_IDEJIMA_KRADEN, 5);
  Actor_SetAnimation(ACTOR_ALEX, 7);
  System_Wait(4);
  Event_PlaySound(SE_CRASH);
  WorldMap_SplitIdejima(32, 128, 32, 0);
  System_Wait(4);
  WorldMap_SplitIdejima(64, 128, 32, 0);
  System_Wait(4);
  FieldCamera_MoveTo(2052, CAMERA_KEEP, 7553, 1);
  FieldCamera_WaitForArrival();
  WorldMap_Freeze();
  MapGrid_SetShake(3, 3, 1);
  // "That last tremor tore a rift in the Suhalla Range!"
  Event_ShowMessage(ACTOR_ALEX, 0);
  // "How are we going to get back to Gondowan now!?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  WorldMap_ReloadPendingAreaGraphics();
  MapGrid_SetShake(3, 3, 1);
  WorldMap_SplitIdejima(96, 128, 32, 0);
  System_Wait(20);
  WorldMap_SplitIdejima(0, 160, 32, 0);
  System_Wait(20);
  WorldMap_SplitIdejima(32, 160, 32, 0);
  FieldCamera_MoveTo(2148, CAMERA_KEEP, 7553, 1);
  FieldCamera_WaitForArrival();
  WorldMap_Freeze();
  // "Idejima... It's drifting away from the mainland!"
  Event_ShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
  // "But...what will happen to us?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(WORLD_MAP_EXIT_IDEJIMA_DRIFTING);
  FieldEvent_Wait(120);
}

extern s32 gWorldMap_IdejimaWakeScript[];

void WorldMap_SpawnIdejimaWake(void) {
  struct Actor *leader;
  struct Actor *obj;
  struct FieldSprite *spr;
  u32 value;

  if ((gFrameCount & 15) != 0)
    return;
  leader = Actor_Get(ACTOR_DRIFTING_IDEJIMA);
  obj = FieldActor_Create(285, leader->x.fixed - px(32), leader->y.fixed,
                          leader->z.fixed - px(16));
  if (obj == NULL)
    return;
  obj->scaleX = Q16_ONE / 2;
  obj->scaleY = Q16_ONE / 2;
  spr = obj->sprite;
  if (Random_Below(2)) {
    s32 back = (Random_Below(48)) << 16;

    obj->x.fixed -= back >> 1;
    obj->z.fixed -= back;
  } else {
    value = ((Random16() << 5) >> 16) << 16;
    obj->x.fixed += value;
    value = (s32)value >> 1;
    obj->z.fixed += value;
  }
  spr->nextFrame = 0;
  spr->priority = ((struct FieldSprite *)leader->sprite)->priority;
  obj->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  obj->motionFlags = leader->motionFlags;
  FieldActor_SetSpriteColor(obj, 9);
  Import_ActorSelectAnimation(obj, 2);
  Actor_StartScript(obj, gWorldMap_IdejimaWakeScript);
}

extern u32 gWorldMap_IdejimaDriftScript[];

void WorldMap_ShowIdejimaDrifting(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_DRIFTING_IDEJIMA);
  FieldEvent_Wait(60);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEffect_ScheduleRatioTransition(Q16_ONE * 3 / 5, 1);
  actor->scaleX = Q16_ONE * 12 / 10;
  actor->scaleY = Q16_ONE * 12 / 10;
  EventCamera_FollowActor(ACTOR_DRIFTING_IDEJIMA, 1);
  System_Wait(1);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId), 15);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_DRIFTING_IDEJIMA), 0);
  Actor_SetSpeed(ACTOR_DRIFTING_IDEJIMA, 0.4);
  actor->timer = 0;
  Actor_SetScript(ACTOR_DRIFTING_IDEJIMA, gWorldMap_IdejimaDriftScript);
  Task_AddFrame(WorldMap_SpawnIdejimaWake, TASK_PASS_VBLANK);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  PaletteFade_ApplyTarget(0x10003, 1);
  gFieldWork->transitionFrames = 32;
  Event_StartScreenTransition();
  FieldEvent_Wait(120);
  FieldEffect_ScheduleRatioTransition(Q16_ONE * 7 / 5, 300);
  FieldEvent_Wait(270);
  gFieldWork->transitionFrames = 16;
  *(u16 *)BG_PLTT = RGB_WHITE;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(WORLD_MAP_EXIT_IDEJIMA_AFTER_DRIFT);
}

void WorldMap_DriftNorthOfContinent(void) {
  struct Actor *boat = Actor_Get(ACTOR_IDEJIMA);
  struct RescueMap *map = (struct RescueMap *)Ram_HeapSlots->mapGrid;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldWorldMap_SetWindowCellTileAndDraw(64, 176, 304, 240);
  FieldWorldMap_SetWindowCellTileAndDraw(80, 176, 320, 240);
  boat->scaleX = Q16_ONE * 12 / 10;
  boat->scaleY = Q16_ONE * 12 / 10;
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  System_Wait(1);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId), 15);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IDEJIMA), 0);
  Actor_SetSpeed(ACTOR_IDEJIMA, 0.4);
  boat->timer = 0;
  boat->acceleration = px(1) / 8;
  boat->speedLimit = px(1) / 8;
  FieldActor_SetMoveTargetFixed(boat, px(10239), boat->y.fixed, px(7884));
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  WorldMap_Freeze();
  FieldEvent_SetMessageId((s32)MsgWeReSaved);
  // "We're saved!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  // "So it would seem..."
  Event_ShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
  FieldActor_SetMoveTargetFixed(boat, px(10255), boat->y.fixed, px(7868));
  // "Wait..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "What is it?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  // "We're going to pass north of the continent..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "She's right!"
  Event_ShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
  // "I don't think we're going to make it!"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "Oh, Kraden... Felix... What are we going to do?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Event_PlaySound(SE_WAVE);
  // "???"
  Event_ShowMessage(gMainRoutineWork.leaderActorId, 0);
  // "Oh... This can't be good."
  Event_ShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
  Event_PlaySound(MUS_SHIP_DRIFTS);
  WorldMap_ReloadPendingAreaGraphics();
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(10303, CAMERA_KEEP, 7820, 1);
  for (i = 31; i >= 0; i--) {
    map->unk11e += 0xff80;
    map->unk11c += 128;
    Actor_Get(ACTOR_IDEJIMA)->scaleY += -(Q16_ONE * 3 / 256);
    System_Wait(1);
  }
  WorldMap_Freeze();
  Event_SetFlag(FLAG_SCENE_TEMP_00);
  // "What...is it?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  // "It's...a tidal wave."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "A tidal wave?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  // "The earthquake must have caused it!"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "Oh... Oh my... It's coming right at us!"
  Event_ShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
  MapGrid_SetShake(1, 2, 1);
  WorldMap_ReloadPendingAreaGraphics();
  FieldEffect_ScheduleRatioTransition(Q16_ONE * 3 / 5, 1);
  map->unk11e = 0;
  map->unk11c = 0xe000;
  System_Wait(1);
  FieldCamera_MoveTo(3655, CAMERA_KEEP, 7632, 0);
  WorldMap_UpdateCurrentTileBlock();
  Actor_SetPosition(ACTOR_IDEJIMA_KRADEN, 3690, 7616);
  Actor_SetPosition(ACTOR_ALEX, 3646, 7679);
  Actor_SetPosition(ACTOR_FELIX, 3669, 7664);
  Actor_SetPosition(ACTOR_JENNA, 3657, 7621);
  Actor_SetPosition(ACTOR_SHEBA, 3703, 7651);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 1);
  System_Wait(1);
  WorldMap_Freeze();
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "Alex... How can you stay so calm at a time like this?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  // "At times like this, where would be the good in panicking?"
  Event_ShowMessage(ACTOR_ALEX, 0);
  FieldActor_Jump(ACTOR_SHEBA, 6, 15);
  FieldActor_Jump(ACTOR_SHEBA, 6, 23);
  Actor_FaceActor(ACTOR_SHEBA, ACTOR_FELIX, 0);
  // "We'll be washed away!"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAttachedEffect(ACTOR_IDEJIMA_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  // "Help!!!"
  Event_ShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
  Actor_SetSpeed(ACTOR_IDEJIMA_KRADEN, 1);
  Actor_WalkTo(ACTOR_IDEJIMA_KRADEN, 3608, 7695);
  Event_PlaySound(SE_WAVE_HIGH);
#ifdef UBFIX
  WorldMap_LoadTidalWave();
#else
  // UB: WorldMap_LoadTidalWave takes no argument, but this call passes 0.
  ((void (*)(s32))WorldMap_LoadTidalWave)(0);
#endif
  FieldEvent_SetExitRequest(WORLD_MAP_EXIT_IDEJIMA);
}

/// The leader is hidden and released, the screen fades and the camera moves;
/// once a key is held the scene changes to SceneId_AlhafraManor.
void WorldMap_PanToAlhafraManor(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->scaleX = 0;
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(11890, CAMERA_KEEP, 10090, 1);
  while (gInput.held == 0)
    wait(1);
  Event_SetDestination(SCENE_ALHAFRA_MANOR, 80);
}

void WorldMap_TakeTheTiller(void) {
  s32 scale;
  s32 seen = Event_GetFlag(FLAG_SHIP_SAILABLE);

  if (seen == 0) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    scale = Actor_GetById(gMainRoutineWork.leaderActorId)->scaleX;
    Actor_GetById(gMainRoutineWork.leaderActorId)->scaleX = 0;
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    WorldMap_Freeze();
    FieldEvent_SetMessageId((s32)MsgTakeTheTiller);
    Event_Wait(40);
    // "Take the tiller, Felix!"
    FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
    Actor_ShowEmote(ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    // "?!?"
    FieldEvent_ShowMessageThenWait(ACTOR_FELIX, 0, 10);
    // "If you hope to reach Lemuria, you'll have to learn a few things..."
    FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
    Actor_ShowEmote(ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "Kraden, shouldn't we be heading straight for Lemuria?"
    FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
    // "Well, I suppose if we have to learn to sail, it's better to do it..."
    FieldEvent_ShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
    Actor_ShowEmote(ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "Then I want to go to Lalivero! And Tolbi! Oh! And Vale! Can we go to..."
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    // "I'd love to see your hometown, Jenna! Shall we? Let's do it! To Vale..."
    FieldMessage_ShowAtActor(ACTOR_PIERS, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
      Actor_ShowEmote(ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
      // "I don't think that's a good idea."
      FieldEvent_ShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
      gFieldWork->message++;
    } else {
      Actor_ShowEmote(ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
      gFieldWork->message++;
      // "I'm with Felix. I don't think that would be a wise course of action."
      FieldEvent_ShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
    }
    FieldActor_ShowEmote(ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                         30);
    // "Come, now! Why not?"
    FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
    // "Think about the roads we have traveled to get where we are..."
    FieldEvent_ShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
    FieldActor_ShowEmote(ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_QUESTION,
                         30);
    // "I can't... I was never any good at geography."
    FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
    // "Then allow me to explain... The coastline of Angara, the continent..."
    FieldEvent_ShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
    Actor_ShowEmote(ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    // "Oh... You don't think Piers's boat could land there?"
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    // "And as for Gondowan, I would guess the people from Lalivero..."
    FieldEvent_ShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
    // "He's right. Saturos and Menardi went a little wild in Lalivero, and..."
    FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
    FieldActor_ShowEmote(ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "Oh... But Sheba can explain the situation for us. Faran and the..."
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    Actor_ShowEmote(ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "And then they'd make me stay in Lalivero! Would you just leave me..."
    FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
    // "Oh yeah... Maybe that wouldn't be such a good idea after all..."
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    // "Then it's decided!"
    FieldEvent_ShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
    Actor_ShowEmote(ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "What's decided?"
    FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
    Actor_ShowEmote(ACTOR_VOYAGE_SHIP, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "The sea is vast... We can go wherever we please! So, let's explore a..."
    FieldEvent_ShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
    // "That's a great idea! We've already met werewolves and discovered new..."
    FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
    // "Wow! This is going to be great!"
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    // "Hmm... The seas can be quite dangerous. It won't be my fault if..."
    FieldEvent_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
    // "To the open sea, Felix!"
    FieldEvent_ShowMessageThenWait(ACTOR_VOYAGE_KRADEN, 0, 10);
    Actor_SetPosition(gMainRoutineWork.leaderActorId, 0, 0);
    gMainRoutineWork.movementMode = 7;
    gMainRoutineWork.ownerOverride = 1;
    WorldMap_ReloadPendingAreaGraphics();
    Event_SetFlag(FLAG_SHIP_SAILABLE);
    Actor_SetPosition(ACTOR_PIERS, 0, 0);
    Actor_SetPosition(ACTOR_SHEBA, 0, 0);
    Actor_SetPosition(ACTOR_VOYAGE_KRADEN, 0, 0);
    Actor_SetPosition(ACTOR_JENNA, 0, 0);
    Actor_GetById(gMainRoutineWork.leaderActorId)->scaleX = scale;
    Event_End();
  }
}

void WorldMap_SailToShipDeck(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_MoveToAndWait(ACTOR_SHIP, 10720, 9075);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(WORLD_MAP_EXIT_SHIP_TILLER);
}

/// The ship's first flight once it has wings (flag 0xa78).
void WorldMap_TakeOff(void) {
  s32 x, z, i;

  if (GameFlag_Get(FLAG_SHIP_FLIES) == 0)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_SetFlag(0xa78);
  Actor_MoveToAndWait(ACTOR_SHIP, 7158, 8532);
  WorldMap_SetUpShip();
  x = Actor_Get(ACTOR_SHIP)->x.fixed / px(1);
  z = Actor_Get(ACTOR_SHIP)->z.fixed / px(1);
  i = 0;
  {
    do {
      if (Actor_Get(i) != NULL) {
        gWorldMap_HiddenMembers[i] = i;
        Actor_Get(i)->scaleX = 0;
        Actor_SetSpriteFlags(Actor_Get(i), 0);
        Actor_SetPosition(i, x, z);
      } else {
        gWorldMap_HiddenMembers[i] = -1;
      }
      i++;
    } while (i <= 7);
  }
  Actor_Get(ACTOR_KRADEN)->scaleX = 0;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_KRADEN), 0);
  Actor_SetPosition(ACTOR_KRADEN, x, z);
  WorldMap_Freeze();
  FieldEvent_SetMessageId((s32)MsgWeDidIt);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  // "We did it! The ship took off without a hitch!"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_ShowEmote(ACTOR_SHIP, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Yeah, but...shouldn't it be flying a little higher than this?"
  Event_ShowMessage(ACTOR_GARET, 0);
  // "Hamma told us that we wouldn't be able to fly very high until we..."
  Event_ShowMessage(ACTOR_MIA, 0);
  // "Let's see how far we can take this thing!"
  Event_ShowMessage(ACTOR_ISAAC, 0);
  FieldEvent_Wait(30);
  // "Sheba, what's wrong? You don't look happy..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  // "Do you remember on Idejima, when I told you I had my own reasons for..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "You were so secretive! Are you going to tell us now?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldEvent_Wait(50);
  // "... ..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "You've gone quiet again... It's OK... You don't have to tell us if..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldEvent_Wait(30);
  Event_PlaySound(MUS_SHEBAS_PAST);
  // "I thought that if I went to Jupiter Lighthouse, I would learn who I..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  FieldEvent_Wait(20);
  // "I remember the people of Lalivero talking about how Sheba fell from..."
  Event_ShowMessage(ACTOR_ISAAC, 0);
  FieldEvent_Wait(20);
  // "All my life, I've been looking for the answer... Where was I born..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "You knew Felix was going to Jupiter Lighthouse eventually, and so..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  FieldEvent_Wait(20);
  // "But you didn't find any answers, did you, Sheba?"
  Event_ShowMessage(ACTOR_MIA, 0);
  FieldEvent_Wait(30);
  // "I'm sorry, Sheba... I was so excited to find Contigo that I didn't..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  FieldEvent_Wait(20);
  // "What? Why is everyone so down all of a sudden?"
  Event_ShowMessage(ACTOR_GARET, 0);
  // "... ..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "Faran raised Sheba like his own daughter. He's cared for her ever..."
  Event_ShowMessage(ACTOR_GARET, 0);
  // "Yes, that's true, but..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "But you can never be his real daughter, is that it?"
  Event_ShowMessage(ACTOR_GARET, 0);
  // "Garet! You don't have to be so blunt about it!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  // "No, he's right... I'd want to know my real parents, no matter how..."
  Event_ShowMessage(ACTOR_MIA, 0);
  // "You think so? If it were me, I doubt it'd bother me at all..."
  Event_ShowMessage(ACTOR_GARET, 0);
  FieldEvent_Wait(40);
  // "Sheba, you may not know this, but you and I are very similar..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  // "What do you mean?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "Well, it's not exactly the same situation, but..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldEvent_Wait(20);
  // "I was born in a poor village. My memories are hazy, but I still..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldEvent_Wait(20);
  // "I was only four when Babi took me under his wing."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldEvent_Wait(20);
  // "You see, even though I was young, I was quite intelligent. Babi had..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  // "And Babi took you to Tolbi to further your education, to raise you..."
  Event_ShowMessage(ACTOR_ISAAC, 0);
  FieldEvent_Wait(20);
  // "I was separated from my parents so early in life... I've never known..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  // "Faran has always been like a father to me... I must have been very..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "I should say so."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldEvent_Wait(60);
  // "Thank you, Kraden... I feel better now..."
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  // "I'm glad I could help."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldEvent_Wait(40);
  // "Well, we should be going... Let's hurry on, Felix!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Scene_PlayMusic();
  WorldMap_ReloadPendingAreaGraphics();
  i = 0;
  {
  restore:
    if (gWorldMap_HiddenMembers[i] >= 0) {
      Actor_SetSpriteFlags(Actor_Get(gWorldMap_HiddenMembers[i]), 1);
      Actor_Get(gWorldMap_HiddenMembers[i])->scaleX = Q16_ONE;
      Actor_SetPosition(gWorldMap_HiddenMembers[i], 0, 0);
    }
    i++;
    if (i <= 7)
      goto restore;
  }
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Event_End();
}

s32 WorldMap_TwinkleTrail(struct Actor *object) {
  if (Random16() < 0x4000)
    Actor_SetSpriteColorMode(object, Random_Below(8));
  return 0;
}

extern s32 gWorldMap_AquaRockTrailScript[];

void WorldMap_SpawnAquaRockTrail(struct Actor *parent) {
  if ((gFrameCount & 7) == 0) {
    s32 x = parent->x.fixed;
    s32 y = parent->y.fixed;
    s32 z = parent->z.fixed;
    struct Actor *object = (FieldActor_Create(673, x, y, z));

    if (object != NULL) {
      Actor_StartScript(object, gWorldMap_AquaRockTrailScript);
      Import_ActorSelectAnimation(object, 1);
      Actor_SetSpriteFlags(object, 0);
      object->motionFlags = 0;
      object->scaleX = Q16_ONE * 2;
      object->scaleY = Q16_ONE * 2;
    }
  }
}

void WorldMap_CarryPartyToAquaRock(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId), 15);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  Actor_Get(gMainRoutineWork.leaderActorId)->update =
      WorldMap_SpawnAquaRockTrail;
  Actor_Get(gMainRoutineWork.leaderActorId)->acceleration = px(1) / 8;
  Actor_Get(gMainRoutineWork.leaderActorId)->speedLimit = px(2);
  System_Wait(5);
  FieldEffect_ScheduleRatioTransition(Q16_ONE * 3 / 4, 60);
  Event_StartScreenTransition();
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 14537, 8577);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 14465, 8662);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 14452, 8766);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 14387, 8828);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 14331, 8888);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 14284, 8904);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 14255, 8870);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 14255, 8831);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 14255, 8772);
  FieldEffect_ScheduleRatioTransition(Q16_ONE * 5 / 4, 120);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 14275, 8736);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 14257, 8720);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
  FieldEvent_SetExitRequest(WORLD_MAP_EXIT_AQUA_ROCK);
}

s32 WorldMap_SetPlacedActorTile(struct Actor *object) {
  u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapGrid + 286);
  struct TileSprite *sprite = object->sprite;

  sprite->tile = *tile;
  sprite->frame = 0;
  return 1;
}

#undef Actor_SetSpriteColorMode
#include "shared/world_map_objects.c"

/// The ship's flicker and its sparks while it powers up (flight mode 1).
void WorldMap_FlashPoweringUp(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Actor_SetSpriteColorMode(object, 7);
  } else {
    Actor_SetSpriteColorMode(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    WorldMap_SpawnSparkPair(object);
  }
}

/// The step through gWorldMap_GlowColorModes, every other frame.
extern s32 gWorldMap_GlowColorStep;

/// The ship's glow and its trail in flight (flight mode 2): the colour cycles
/// every other frame, and a particle every fourth.
void WorldMap_GlowInFlight(struct Actor *object) {
  if (gFrameCount & 1) {
    Actor_SetSpriteColorMode(object,
                             gWorldMap_GlowColorModes[gWorldMap_GlowColorStep]);
    gWorldMap_GlowColorStep = (gWorldMap_GlowColorStep + 1) & 3;
  }
  if (!(gFrameCount & 3)) {
    WorldMap_SpawnParticle(object);
  }
}

/// Sets the ship's look by flight mode: 0 plain, 1 powering up, 2 in flight.
void Actor_SetFlightMode(struct Actor *object, s32 mode) {
  if (mode == 0) {
    object->update = 0;
    Actor_SetSpriteColorMode(object, 0);
    Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  } else if (mode == 1) {
    object->update = (void (*)(void))WorldMap_FlashPoweringUp;
  } else {
    object->update = (void (*)(void))WorldMap_GlowInFlight;
    Event_PlaySound(SE_TAKE_FLIGHT);
  }
}

#include "shared/leader_search.h"

void WorldMap_EndFlight(void) {
  Actor_SetFlightMode(Actor_GetById(Party_GetLeaderActor()), 0);
}

/// Run each frame of the lighthouse transition: every other two frames the
/// blend alternates between two strengths.
void WorldMap_UpdateTransitionBlend(void) {

  Scene_QueueIoWrite16((u32)&REG_BLDCNT,
                       BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  if (gFrameCount & 2)
    Scene_QueueIoWrite16((u32)&REG_BLDALPHA,
                         gWorldMapBlend | (BLEND_WEIGHT_MAX - 4));
  else
    Scene_QueueIoWrite16((u32)&REG_BLDALPHA, gWorldMapBlend | BLEND_WEIGHT_MAX);
}

static inline void CameraMove(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveToFixed(x, y, z, mode);
}

/// Shakes the camera diagonally by one pixel every three frames.
#include "shared/camera_jitter.h"

#define PUSH_PICTURE(frame)                                                    \
  IO_WRITE_QUEUE_PUSH(                                                         \
      PICTURE_MAP,                                                             \
      (u32)(buffer + PICTURES_FIRST_FRAME + (frame) * PICTURES_FRAME_STRIDE),  \
      DMA_COPY_WORDS(PICTURE_FRAME_SIZE / 4))

void WorldMap_PlayLighthouseTransition(s32 mode) {
  u8 *buffer;

  System_Wait(1);
  Event_ClearFlag(FLAG_KEEP_PARTY_POSITION);
  gFieldWork->transition = 0;
  Actor_Get(gMainRoutineWork.leaderActorId)->spriteStorageKind = 0;
  gFieldWork->transitionFrames = 16;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (mode == 2) {
    MapGrid_SetShake(2, 3, 1);
    Event_PlaySound(SE_RUMBLE);
  }
  WorldMap_Freeze();
  buffer = Heap_AllocEwram(0x4000);
  Decompress_TaggedLz(gWorldMapPackedTiles, buffer);
  Decompress_TaggedLz(gWorldMapPackedPictures,
                      buffer + PICTURE_FIRST_TILE * TILE_SIZE_4BPP);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
                      (u32)(gWorldMapPalettes + mode * PALETTE_COLORS * 2),
                      DMA_COPY_HALFWORDS(PALETTE_COLORS));
  IO_WRITE_QUEUE_PUSH(PICTURE_TILES, (u32)buffer,
                      DMA_COPY_WORDS(PICTURE_FIRST_TILE * TILE_SIZE_4BPP / 4));
  Task_AddFrame(WorldMap_UpdateTransitionBlend, TASK_PASS_VBLANK);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_GLITTER);
  gWorldMapBlend = BLEND_ALPHA(0, 14);
  PUSH_PICTURE(6);
  FieldEvent_Wait(2);
  gWorldMapBlend = BLEND_ALPHA(0, 13);
  PUSH_PICTURE(5);
  FieldEvent_Wait(2);
  gWorldMapBlend = BLEND_ALPHA(0, 12);
  PUSH_PICTURE(4);
  FieldEvent_Wait(2);
  gWorldMapBlend = BLEND_ALPHA(0, 11);
  PUSH_PICTURE(3);
  FieldEvent_Wait(2);
  gWorldMapBlend = BLEND_ALPHA(0, 10);
  PUSH_PICTURE(2);
  FieldEvent_Wait(2);
  gWorldMapBlend = BLEND_ALPHA(0, 9);
  PUSH_PICTURE(1);
  FieldEvent_Wait(2);
  gWorldMapBlend = BLEND_ALPHA(0, 8);
  PUSH_PICTURE(0);
  FieldEvent_Wait(140);
  PUSH_PICTURE(1);
  FieldEvent_Wait(4);
  PUSH_PICTURE(2);
  FieldEvent_Wait(4);
  PUSH_PICTURE(3);
  FieldEvent_Wait(4);
  gWorldMapBlend = BLEND_ALPHA(0, 9);
  PUSH_PICTURE(4);
  FieldEvent_Wait(4);
  gWorldMapBlend = BLEND_ALPHA(0, 10);
  PUSH_PICTURE(5);
  FieldEvent_Wait(4);
  gWorldMapBlend = BLEND_ALPHA(0, 11);
  PUSH_PICTURE(6);
  FieldEvent_Wait(4);
  gWorldMapBlend = BLEND_ALPHA(0, 12);
  PUSH_PICTURE(7);
  if (mode == 1) {
    MapGrid_SetShake(2, 2, 1);
    Event_PlaySound(SE_RUMBLE);
    gWorldMapBlend = BLEND_ALPHA(0, 13);
    FieldEvent_Wait(4);
    gWorldMapBlend = BLEND_ALPHA(0, 14);
    FieldEvent_Wait(4);
    gWorldMapBlend = BLEND_ALPHA(0, 15);
    FieldEvent_Wait(4);
    gWorldMapBlend = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
    FieldEvent_Wait(45);
    FieldTask_Remove(WorldMap_UpdateTransitionBlend);
    DebugText_Clear();
    Scene_QueueIoWrite16((u32)&REG_BLDALPHA, gWorldMapBlend);
    Scene_QueueIoWrite16((u32)&REG_BLDCNT, 0);
  } else {
    gWorldMapBlend = BLEND_ALPHA(0, 13);
    FieldEvent_Wait(4);
    gWorldMapBlend = BLEND_ALPHA(0, 14);
    FieldEvent_Wait(4);
    gWorldMapBlend = BLEND_ALPHA(0, 15);
    FieldEvent_Wait(4);
    gWorldMapBlend = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
    FieldEvent_Wait(45);
    FieldTask_Remove(WorldMap_UpdateTransitionBlend);
    DebugText_Clear();
    Scene_QueueIoWrite16((u32)&REG_BLDALPHA, gWorldMapBlend);
    Scene_QueueIoWrite16((u32)&REG_BLDCNT, 0);
  }
  Heap_Release(buffer);
  Event_SetFlag(FLAG_SKIP_SCENE_CHANGE_SOUND);
}

void WorldMap_LoadTidalWave(void) {
  u8 *buffer = Heap_AllocEwram(0x4000);
  u8 *frames;
  u32 x;
  s32 y, i, scrollX;
  u16 scrollY;
  s32 mode;

  Decompress_TaggedLz(gWorldMapPackedTilesB, buffer);
  frames = buffer + 0x2000;
  Decompress_TaggedLz(gWorldMapPackedPicturesB, frames);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
                      (u32)gWorldMapPalettesB,
                      DMA_COPY_HALFWORDS(PALETTE_COLORS));
  IO_WRITE_QUEUE_PUSH(VRAM, (u32)buffer,
                      DMA_COPY_WORDS(PICTURE_FIRST_TILE * TILE_SIZE_4BPP / 4));
  IO_WRITE_QUEUE_PUSH(PICTURE_TILES, (u32)frames,
                      DMA_COPY_WORDS(256 * TILE_SIZE_4BPP / 4));
  Scene_QueueIoWrite16((u32)&REG_BLDCNT, 0x3f41);
  Scene_QueueIoWrite16((u32)&REG_BLDALPHA,
                       BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX));
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 352;
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG0CNT, 0xc200, 0x20000);
  y = 0x2d60;
  x = 561;
  for (i = 95; i >= 0; i--) {
    scrollX = (s32)(x * 16) / 128;
    scrollY = -y / 128 + 352;
    gBgScroll[0].x = scrollX;
    gBgScroll[0].y = scrollY;
    System_Wait(1);
    y += 352;
    x += 17;
  }
  Event_PlaySound(SE_SCATTER);
  System_Wait(120);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  // mode holds one 0 for the rest of the function; the closing scroll reset
  // stores it again.
  mode = 0;
  PaletteFade_ApplyTarget(RGB_WHITE, mode);
  PaletteFade_Interpolate(1);
  Scene_QueueIoWrite16((u32)&REG_BLDALPHA, BLEND_ALPHA(0, BLEND_WEIGHT_MAX));
  System_Wait(1);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(60);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  System_Wait(60);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  System_Wait(120);
  Ram_HeapSlots->actorSystem->suspended = 1;
  System_Wait(1);
  Window_LoadGraphics();
  DebugText_Clear();
  FieldWindowColor_Apply(gMainRoutineWork.windowHue,
                         gMainRoutineWork.windowBrightness);
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
}

void WorldMap_FlashWithSparks(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Actor_SetSpriteColorMode(object, 7);
  } else {
    Actor_SetSpriteColorMode(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    WorldMap_SpawnTwinSparks(object);
  }
}

/// Animates an actor, running WorldMap_SpawnTwinSparks every sixteenth frame.
void Actor_AnimateAndEmit(struct Actor *object) {
  u32 frame = gFrameCount;

  if (frame & 1)
    Actor_SetSpriteColorMode(object, (frame >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    WorldMap_SpawnTwinSparks(object);
}

#include "shared/actor_animate.h"

#include "shared/actor_grow_up.h"

#include "shared/actor_grow_down.h"

void WorldMap_SpawnTwinSparks(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct FieldSprite *sprite;
  struct LinkedEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  s32 i;

  for (i = 0; i < 2; ++i) {
    child = (FieldActor_Create(SPRITE_GOLDEN_RINGS, parent->x.fixed,
                               parent->y.fixed, parent->z.fixed));
    pair[i] = child;
    if (child != NULL) {
      child->groundHeight = parent->groundHeight;
      sprite = child->sprite;
      child->motionFlags = 0;
      child->timer = 0;
      child->watchedActor = parent;
      if (sprite != NULL) {
        Sprite_StartAnimation(sprite, 0);
        sprite->nextFrame = 0;
        VramCache_FreeEntry(sprite->vramBlock);
        sprite->vramBlock = work->vramBlock;
        ((struct SpriteCacheBits *)&sprite->unk11)->retainCacheEntry = 1;
        sprite->tile =
            (gVramCacheEntries[sprite->vramBlock].offset >> 5) & 0x3ff;
        sprite->fullColor = 0;
        sprite->shape = 1;
        ((struct WorldMapOam *)sprite)->size = 2;
        ((struct LinkedDetail *)sprite->record)->field16 = 0;
      }
    }
  }
  {
    struct Actor *p = pair[0];
    struct FieldSprite *sp = p->sprite;

    p->update = (void *)Actor_GrowDown;
    sp->priority = 1;
  }
  {
    struct Actor *p = pair[1];
    struct FieldSprite *sp = p->sprite;

    sp->priority = 1;
    p->update = (void *)Actor_GrowUp;
    p->priorityFlags = 2;
  }
}

extern s16 gGauge_Shown;
extern s16 gGauge_Vram;
extern u8 gGauge_SourceTiles[][32];

/// Redraws the gauge for `progress`.
void Gauge_Draw(s32 progress) {
  s32 level = ((progress * GAUGE_LEVELS + Q16_ONE / 2) >> 16) + 1;
  s32 kind = GAUGE_EMPTY;
  s32 count, base, i;

  if (progress != 0)
    kind = GAUGE_SHOWN;
  if (level <= 15 && (gFrameCount & 15) <= 1)
    kind = GAUGE_LOW;
  if (level <= 7 && (gFrameCount & 7) <= 1)
    kind = GAUGE_LOW;
  if (kind + level == gGauge_Shown)
    return;
  gGauge_Shown = kind + level;
  Iwram_ClearWords((s32)(gGauge_Tiles), (s32)(GAUGE_SIZE));
  {
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    if (kind == GAUGE_EMPTY) {
      copy(gGauge_Tiles, gGauge_SourceTiles[8], 4 * TILE_SIZE_4BPP);
      goto done;
    }
    if (kind == GAUGE_SHOWN)
      copy(gGauge_Tiles, gGauge_SourceTiles[4], 4 * TILE_SIZE_4BPP);
    else
      copy(gGauge_Tiles, gGauge_SourceTiles[8], 4 * TILE_SIZE_4BPP);
  }
  base = 0;
  count = level;
  if (count > 7) {
    Iwram_CopyWordsEntry(gGauge_Tiles, gGauge_SourceTiles, TILE_SIZE_4BPP);
    base = 8;
    count -= 8;
  }
  if (level > 15) {
    Iwram_CopyWordsEntry(gGauge_Tiles[1], gGauge_SourceTiles[1],
                         TILE_SIZE_4BPP);
    count = level;
    base = 16;
    count -= 16;
  }
  if (level > 23) {
    Iwram_CopyWordsEntry(gGauge_Tiles[2], gGauge_SourceTiles[2],
                         TILE_SIZE_4BPP);
    count = level;
    base = 24;
    count -= 24;
  }
  if (level > 30) {
    Iwram_CopyWordsEntry(gGauge_Tiles[3], gGauge_SourceTiles[3],
                         TILE_SIZE_4BPP);
    base = 0;
    count = 0;
  }
  if (count != 0) {
    for (i = 0; i < count; i++) {
      u8 *source = &gGauge_SourceTiles[base / 8][4] + i / 2;
      u8 *dest = &gGauge_Tiles[base / 8][4] + i / 2;
      s32 j;

      if ((i & 1) == 0) {
        for (j = 1; j <= 3; j++) {
          *dest = (*dest & 240) | (*source & 15);
          source += 4;
          dest += 4;
        }
      } else {
        for (j = 2; j >= 0; j--) {
          *dest = (*dest & 15) | (*source & 240);
          source += 4;
          dest += 4;
        }
      }
    }
  }
done:
  Vram_Load(gGauge_Vram, GAUGE_SIZE, gGauge_Tiles);
}

/// The Psynergy gauge over the leader as the ship flies: while A or B is held
/// aloft it drains a PP every ten frames, then redraws the gauge and queues its
/// sprite.
void Gauge_Update(void) {
  u32 *oam = gGauge_Sprite;
  s32 tile = gVramCacheEntries[gGauge_Vram].offset >> 5;
  struct Actor *leader = Actor_Get(FieldParty_GetLeaderActor());
  s32 screen[3];
  u32 attributes;

  if ((gInput.held & (KEY_A | KEY_B)) != 0 &&
      Event_GetFlag(FLAG_MENU_OPEN) == 0 && leader->y.fixed > px(4)) {
    if (--gGauge_Countdown <= 0) {
      Party_DrainPp(1);
      gGauge_Countdown = 10;
    }
  }
  Gauge_Draw(Party_GetPpRatio());
  attributes = OBJ_SIZE_32X16;
  Call2((void (*)())Render_ProjectPoint, (s32)(&leader->x.fixed),
        (s32)(screen));
  {
    s32 x = screen[0] - 16;
    s32 y = screen[1] - 32;

    *oam++ = 0;
    *oam++ = (y | OBJ_X(x)) | attributes;
  }
  *oam = tile | OBJ_PRIORITY(1);
  Oam_AddSpriteAtDepthClamped(gGauge_Sprite, OAM_DEPTH_FRONT);
}

/// Sets up the gauge.
void Gauge_Init(void) {
  Decompress_TaggedLz(gWorldMapPackedTilesC, gGauge_SourceTiles);
  gGauge_Vram = VramCache_FindFreeEntry();
  Vram_Load(gGauge_Vram, GAUGE_SIZE, 0);
  gGauge_Shown = -1;
  gGauge_Countdown = 10;
  Task_AddFrame(Gauge_Update, TASK_PASS_VBLANK - 10);
}

/// Takes the gauge down.
void Gauge_End(void) {
  Decompress_TaggedLz(gWorldMapPackedTilesC, gGauge_SourceTiles);
  VramCache_FreeEntry(gGauge_Vram);
  gGauge_Shown = -1;
  FieldTask_Remove(Gauge_Update);
}

// clang-format off
u32 gWorldMap_LohoWorkerLeaveScriptA[] = {
    39, 2, 48, 0x23010000, 0x16cd0000, 1,
    3, 17,
};

u32 gWorldMap_LohoWorkerLeaveScriptB[] = {
    39, 2, 48, 0x22f10000, 0x16dd0000, 1,
    48, 0x23010000, 0x16cd0000, 1, 3, 17,
};

u32 gWorldMap_LohoWorkerLeaveScriptC[] = {
    39, 2, 48, 0x22f10000, 0x16dd0000, 1,
    48, 0x23010000, 0x16cd0000, 1, 3, 17,
};

u32 gWorldMap_IdejimaDriftScript[] = {
    5, 0x180000, 0, 0, 1, 13,
    10, 0, 22, 35, 1, 5,
    0x180000, 0, 0, 1, 5, 0x180000,
    0, 0, 1, 17,
};

s32 gWorldMap_IdejimaWakeScript[] = {
    23, 5, (s32)0xffffe667, 23, 7, 0x1999,
    23, 9, 0xe00, 23, 10, -384,
    0, 1, 13, 48, 0, 2,
    0, 0, 0, 0, 1, 38,
};

s32 gWorldMap_AquaRockTrailScript[] = {
    46,
    (s32)WorldMap_TwinkleTrail,
    0,
    1,
    13,
    120,
    0,
    38,
};

s32 gWorldMap_SparkScript[] = {
    SC_CALL((s32)WorldMap_RiseFromAnchor),
    SC_RELEASE,
};

u32 gWorldMap_ParticleScript[] = {
    SC_WAIT(39),
    SC_SET(SW_X, 0),
    SC_SET(SW_Z, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

u32 gWorldMap_GlowColorModes[] = {
    0, 9, 0, 6,
};
// clang-format on

#undef PUSH_PICTURE
#undef WORLD_MAP_TRY_ENCOUNTER
#undef TUTORIAL_SET_MESSAGE
#undef TUTORIAL_MESSAGE
#undef Actor_SetSpriteColorMode

void WorldMap_MeetIron(void);
void WorldMap_BoardShip(void);
void WorldMap_LeaveShip(void);

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 9850, 0, 9748, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 10240, 0, 7974, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 10144, 0, 8330, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 10144, 0, 8240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 10137, 0, 8602, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 10423, 0, 8354, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 10190, 0, 8883, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 10578, 0, 8952, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 10635, 0, 9040, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 10168, 0, 9630, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 10270, 0, 9522, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 10271, 0, 9425, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 10380, 0, 9370, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {13, CONDITION_ALWAYS, 10440, 0, 9399, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {14, CONDITION_ALWAYS, 11125, 0, 10695, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {15, CONDITION_ALWAYS, 12070, 0, 10470, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {16, CONDITION_ALWAYS, 11880, 0, 10090, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {17, CONDITION_ALWAYS, 10976, 0, 9960, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {18, CONDITION_ALWAYS, 11736, 0, 9530, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {19, CONDITION_ALWAYS, 11910, 0, 9729, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {20, CONDITION_ALWAYS, 11633, 0, 9406, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {21, CONDITION_ALWAYS, 9244, 0, 9528, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {22, CONDITION_ALWAYS, 9328, 0, 9528, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {23, CONDITION_ALWAYS, 9148, 0, 9048, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {24, CONDITION_ALWAYS, 9148, 0, 8988, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {25, CONDITION_ALWAYS, 9199, 0, 8410, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {26, CONDITION_ALWAYS, 9060, 0, 8290, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {27, CONDITION_ALWAYS, 9256, 0, 7950, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {28, CONDITION_ALWAYS, 9546, 0, 8316, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {29, CONDITION_ALWAYS, 12770, 0, 11570, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {30, CONDITION_ALWAYS, 11724, 0, 8064, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {31, CONDITION_ALWAYS, 11680, 0, 6678, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {32, CONDITION_ALWAYS, 13486, 0, 7482, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {33, CONDITION_ALWAYS, 12923, 0, 9681, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {34, CONDITION_ALWAYS, 12921, 0, 9752, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {35, CONDITION_ALWAYS, 12680, 0, 10297, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {36, CONDITION_ALWAYS, 14478, 0, 8410, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {37, CONDITION_ALWAYS, 14250, 0, 8728, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {38, CONDITION_ALWAYS, 13376, 0, 6068, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {39, CONDITION_ALWAYS, 13452, 0, 6204, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {40, CONDITION_ALWAYS, 10903, 0, 6285, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {41, CONDITION_ALWAYS, 11201, 0, 6105, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {42, CONDITION_ALWAYS, 12461, 0, 7566, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {43, CONDITION_ALWAYS, 12090, 0, 7165, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {44, CONDITION_ALWAYS, 13010, 0, 1240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {45, CONDITION_ALWAYS, 13088, 0, 1090, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {46, CONDITION_ALWAYS, 9490, 0, 10960, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {47, CONDITION_ALWAYS, 5836, 0, 10120, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {48, CONDITION_ALWAYS, 8582, 0, 7420, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {49, CONDITION_ALWAYS, 6314, 0, 6223, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {50, CONDITION_ALWAYS, 8847, 0, 4629, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {51, CONDITION_ALWAYS, 8970, 0, 7500, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {52, CONDITION_ALWAYS, 7256, 0, 8325, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {53, CONDITION_ALWAYS, 7280, 0, 8180, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {54, CONDITION_ALWAYS, 7714, 0, 7760, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {55, CONDITION_ALWAYS, 7277, 0, 7330, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {56, CONDITION_ALWAYS, 7383, 0, 5753, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {57, CONDITION_ALWAYS, 7504, 0, 5650, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {58, CONDITION_ALWAYS, 7663, 0, 5535, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {59, CONDITION_ALWAYS, 8960, 0, 5856, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {60, CONDITION_ALWAYS, 8933, 0, 3704, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {61, CONDITION_ALWAYS, 8916, 0, 1640, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {62, CONDITION_ALWAYS, 9056, 0, 1250, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {63, CONDITION_ALWAYS, 9056, 0, 1175, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {64, CONDITION_ALWAYS, 1992, 0, 7272, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {65, CONDITION_ALWAYS, 10239, 0, 7884, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {67, CONDITION_ALWAYS, 11633, 0, 9406, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {68, CONDITION_ALWAYS, 7256, 0, 8325, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {69, CONDITION_ALWAYS, 10640, 0, 9075, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {71, CONDITION_ALWAYS, 10720, 0, 9075, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {72, CONDITION_ALWAYS, 7279, 0, 7307, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {73, CONDITION_ALWAYS, 10168, 0, 9630, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {74, CONDITION_ALWAYS, 10168, 0, 9630, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {77, CONDITION_ALWAYS, 12096, 0, 8325, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {78, CONDITION_ALWAYS, 14558, 0, 8457, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {80, CONDITION_ALWAYS, 11754, 0, 8030, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {81, CONDITION_ALWAYS, 11700, 0, 8034, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {82, CONDITION_ALWAYS, 11596, 0, 10204, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {83, CONDITION_ALWAYS, 10240, 0, 9142, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {84, CONDITION_ALWAYS, 9020, 0, 882, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {85, CONDITION_ALWAYS, 9288, 0, 9434, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {86, CONDITION_ALWAYS, 9288, 0, 9640, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {87, CONDITION_ALWAYS, 12640, 0, 4390, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {88, CONDITION_ALWAYS, 7868, 0, 9120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {89, CONDITION_ALWAYS, 9479, 0, 5060, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {98, CONDITION_ALWAYS, 8916, 0, 1640, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 10271, 0, 9540, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(WorldMap),
    SCENE_EXIT(WORLD_MAP_EXIT_IDEJIMA_DRIFTING, WorldMap, 77),
    SCENE_EXIT(WORLD_MAP_EXIT_IDEJIMA, Idejima, 2),
    SCENE_EXIT(WORLD_MAP_EXIT_SHIP_TILLER, LemurianShipTiller, 10),
    SCENE_EXIT(WORLD_MAP_EXIT_IDEJIMA_AFTER_DRIFT, Idejima, 12),
    SCENE_EXIT(WORLD_MAP_EXIT_AQUA_ROCK, AquaRockMoai, 5),
    SCENE_EXIT(1, Idejima, 4),
    SCENE_EXIT_UNLESS(2, DailaFlooded, 8, 0x873),
    SCENE_EXIT_UNLESS(3, DailaFlooded, 9, 0x873),
    SCENE_EXIT(2, Daila, 8),
    SCENE_EXIT(3, Daila, 9),
    SCENE_EXIT(4, KandoreanTempleGate, 1),
    SCENE_EXIT(5, ShrineOfTheSeaGodRope, 3),
    SCENE_EXIT(6, DehkanPlateauFullMetalVestChest, 1),
    SCENE_EXIT(7, DehkanPlateauTightropes, 6),
    SCENE_EXIT(8, LemurianShipMainDeck, 2),
    SCENE_EXIT(9, Madra, 11),
    SCENE_EXIT(10, MadraDrawbridge, 1),
    SCENE_EXIT(11, MadraDrawbridge, 2),
    SCENE_EXIT(12, OseniaCliffs, 1),
    SCENE_EXIT(13, OseniaCliffs, 2),
    SCENE_EXIT(14, Mikasalla, 6),
    SCENE_EXIT(15, GarohBlocks, 1),
    SCENE_EXIT(16, AirsRockStormBrandChest, 1),
    SCENE_EXIT(17, YampiDesertGuardianRingChest, 1),
    SCENE_EXIT(18, YampiDesertTrainersWhipChest, 2),
    SCENE_EXIT(19, YampiDesertMedalSpot, 2),
    SCENE_EXIT(20, Alhafra, 9),
    SCENE_EXIT(21, GondowanCliffs, 1),
    SCENE_EXIT(22, GondowanCliffs, 2),
    SCENE_EXIT(23, Naribwe, 6),
    SCENE_EXIT(25, KibomboMountainsDiskAxeChest, 1),
    SCENE_EXIT(26, KibomboMountainsWaft, 1),
    SCENE_EXIT_UNLESS(27, KibomboCeremony, 1, 0x8ff),
    SCENE_EXIT_UNLESS(27, Kibombo, 1, EXIT_FLAG_NONE),
    SCENE_EXIT(28, IsletsDog, 1),
    SCENE_EXIT(29, IsletsPrettyStone, 1),
    SCENE_EXIT(30, IsletsCow, 1),
    SCENE_EXIT(31, IsletsPier, 10),
    SCENE_EXIT(32, IsletsTurtle, 1),
    SCENE_EXIT(33, Yallam, 10),
    SCENE_EXIT(34, Yallam, 11),
    SCENE_EXIT(35, TaopoSwampEntrance, 1),
    SCENE_EXIT(36, ApojiiIslands, 1),
    SCENE_EXIT(37, AquaRockMoai, 1),
    SCENE_EXIT(38, Izumo, 1),
    SCENE_EXIT(39, GaiaRockSusa, 5),
    SCENE_EXIT(40, Champa, 9),
    SCENE_EXIT(41, AnkohlRuinsEntrance, 1),
    SCENE_EXIT(42, SeaOfTimeWhirlpool, 1),
    SCENE_EXIT(43, SeaOfTimeToLemuria, 5),
    SCENE_EXIT(44, SeaOfTimeToLemuria, 3),
    SCENE_EXIT(45, LemuriaEntrance, 1),
    SCENE_EXIT(46, TundariaTowerEntrance, 1),
    SCENE_EXIT(47, SwAttekaIslet, 1),
    SCENE_EXIT(48, GondowanSettlement, 1),
    SCENE_EXIT(49, HesperiaSettlement, 1),
    SCENE_EXIT(50, KaltIsland, 1),
    SCENE_EXIT(51, MagmaRockSlide, 1),
    SCENE_EXIT(52, AttekaInlet, 1),
    SCENE_EXIT(53, AttekaInletGeode, 2),
    SCENE_EXIT(54, Contigo, 1),
    SCENE_EXIT(55, JupiterLighthouseEntrance, 1),
    SCENE_EXIT(56, ShamanVillageCave, 1),
    SCENE_EXIT(57, ShamanVillageCave, 10),
    SCENE_EXIT(58, ShamanVillage, 1),
    SCENE_EXIT(59, Loho, 7),
    SCENE_EXIT(60, NorthernReaches, 1),
    SCENE_EXIT(61, NorthernReaches, 4),
    SCENE_EXIT(62, ProxSouthGate, 1),
    SCENE_EXIT(63, ProxMold, 3),
    SCENE_EXIT(82, CavernsOsenia, 1),
    SCENE_EXIT(83, CavernsIndra, 1),
    SCENE_EXIT(84, MarsLighthouseOutside, 1),
    SCENE_EXIT(85, GondowanCliffsPass, 1),
    SCENE_EXIT(86, GondowanCliffsPass, 2),
    SCENE_EXIT(87, TreasureIsleEntrance, 1),
    SCENE_EXIT(88, CavernsAtteka, 5),
    SCENE_EXIT(89, CavernsAngara, 1),
    SCENE_EXITS_END,
};

LATE_DATA s32 gWorldMap_TileSpriteScript[] = {
    46,
    (s32)TileSprite_Reset,
    17,
};

LATE_DATA s32 gWorldMap_EchoScript[] = {
    46,
    (s32)WorldMap_SetUpEcho,
    17,
};

LATE_DATA struct ActorPlacement gWorldMapActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS,
                    (s32)gWorldMap_TileSpriteScript, 10640, 0, 9075,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_ECHO),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_IRON),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_SOUR),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_CHILL),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_WHEEZE),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_PETRA),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_CORE),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    0, 0, 0, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(154, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_LONG_DARK_HAIR, IF_FLAG_SET(0xa76),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE, IF_FLAG_SET(0xa76),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE, IF_FLAG_SET(0xa76),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gWorldMap_CollapsePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1968,
                    0, 7488, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1936,
                    0, 7472, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(57, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1904, 0, 7488,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS,
                    (s32)gWorldMap_TileSpriteScript, 1946, 0, 7557, FACING_EAST,
                    TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA s32 gWorldMap_IslandScript[] = {
    46,
    (s32)WorldMap_SetIslandTile,
    17,
};

LATE_DATA s32 gWorldMap_TidalWaveScript[] = {
    46, (s32)WorldMap_SetUpWave, 46, (s32)WorldMap_SendWaveIn, 17,
};

LATE_DATA struct ActorPlacement gWorldMap_DriftPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11000,
                    0, 7500, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10000,
                    0, 7500, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(57, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10000, 0, 8000,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11000,
                    0, 8000, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_ISLAND, CONDITION_ALWAYS,
                    (s32)gWorldMap_IslandScript, 10207, 0, 7900, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gWorldMap_TidalWaveScript,
                    10524, 0, 7450, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gWorldMap_TidalWaveScript,
                    10532, 0, 7480, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gWorldMap_TidalWaveScript,
                    10540, 0, 7510, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gWorldMap_TidalWaveScript,
                    10548, 0, 7540, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gWorldMap_TidalWaveScript,
                    10556, 0, 7570, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gWorldMap_TidalWaveScript,
                    10564, 0, 7600, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gWorldMap_TidalWaveScript,
                    10572, 0, 7630, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(277, CONDITION_ALWAYS, (s32)gWorldMap_TidalWaveScript,
                    10580, 0, 7660, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS,
                    (s32)gWorldMap_TileSpriteScript, 3515, 0, 7621, FACING_EAST,
                    TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gWorldMap_FirstVoyagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WINGED_SHIP_SMALL, CONDITION_ALWAYS,
                    (s32)gWorldMap_TileSpriteScript, 10640, 0, 9075,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10550,
                    0, 8682, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10550,
                    0, 9282, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10850,
                    0, 8682, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10850,
                    0, 9282, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gWorldMap_DriftingIdejimaPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_ISLAND, CONDITION_ALWAYS,
                    (s32)gWorldMap_IslandScript, 12096, 0, 8325, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gSceneEvents[] = {
    {TRIGGER_RAISED, 251, CONDITION_ALWAYS, EVENT(WorldMap_BoardShip)},
    {TRIGGER_RAISED, 252, CONDITION_ALWAYS, EVENT(WorldMap_LeaveShip)},
    {TRIGGER_RAISED, 253, CONDITION_ALWAYS, EVENT(WorldMap_SetUpFlyingShip)},
    {TRIGGER_RAISED, 254, CONDITION_ALWAYS, EVENT(WorldMap_SetUpShip)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WORLD_MAP_IRON, FLAG_DJINNI(DJINNI_IRON),
     EVENT(WorldMap_MeetIron)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WORLD_MAP_SOUR, FLAG_DJINNI(DJINNI_SOUR),
     EVENT(WorldMap_MeetSour)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WORLD_MAP_CHILL, FLAG_DJINNI(DJINNI_CHILL),
     EVENT(WorldMap_MeetChill)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WORLD_MAP_WHEEZE, FLAG_DJINNI(DJINNI_WHEEZE),
     EVENT(WorldMap_MeetWheeze)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WORLD_MAP_PETRA, FLAG_DJINNI(DJINNI_PETRA),
     EVENT(WorldMap_MeetPetra)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WORLD_MAP_CORE, FLAG_DJINNI(DJINNI_CORE),
     EVENT(WorldMap_MeetCore)},
    {TRIGGER_EXAMINE, 1123, CONDITION_ALWAYS, EVENT(WorldMap_GazeIntoPond)},
    {TRIGGER_STEP, STEP_TRIGGER_AREA(AREA_DAILA), CONDITION_ALWAYS,
     EVENT(WorldMap_EnterDaila)},
    {TRIGGER_EXIT, 101, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 102, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 103, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 104, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 105, 0x8de, 8},
    {TRIGGER_EXIT, 106, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 107, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 108, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 109, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 110, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 111, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 112, CONDITION_ALWAYS, 19},
    {TRIGGER_EXIT, 113, CONDITION_ALWAYS, 20},
    {TRIGGER_EXIT, 114, CONDITION_ALWAYS, 21},
    {TRIGGER_EXIT, 115, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT, 116, CONDITION_ALWAYS, 23},
    {TRIGGER_EXIT, 117, CONDITION_ALWAYS, 25},
    {TRIGGER_EXIT, 118, CONDITION_ALWAYS, 26},
    {TRIGGER_EXIT, 119, CONDITION_ALWAYS, 27},
    {TRIGGER_EXIT, 120, CONDITION_ALWAYS, 28},
    {TRIGGER_EXIT, 121, CONDITION_ALWAYS, 29},
    {TRIGGER_EXIT, 122, CONDITION_ALWAYS, 30},
    {TRIGGER_EXIT, 123, CONDITION_ALWAYS, 31},
    {TRIGGER_EXIT, 124, CONDITION_ALWAYS, 32},
    {TRIGGER_STEP, STEP_TRIGGER_AREA(AREA_YALLAM), CONDITION_ALWAYS,
     EVENT(WorldMap_EnterYallam)},
    {TRIGGER_EXIT, 126, CONDITION_ALWAYS, 35},
    {TRIGGER_EXIT, 127, CONDITION_ALWAYS, 36},
    {TRIGGER_EXIT, 128, CONDITION_ALWAYS, 37},
    {TRIGGER_EXIT, 129, CONDITION_ALWAYS, 38},
    {TRIGGER_EXIT, 130, CONDITION_ALWAYS, 39},
    {TRIGGER_EXIT, 131, CONDITION_ALWAYS, 40},
    {TRIGGER_EXIT, 132, CONDITION_ALWAYS, 41},
    {TRIGGER_EXIT, 133, CONDITION_ALWAYS, 42},
    {TRIGGER_EXIT, 134, CONDITION_ALWAYS, 43},
    {TRIGGER_EXIT, 136, CONDITION_ALWAYS, 45},
    {TRIGGER_EXIT, 137, CONDITION_ALWAYS, 46},
    {TRIGGER_EXIT, 138, CONDITION_ALWAYS, 47},
    {TRIGGER_EXIT, 139, CONDITION_ALWAYS, 48},
    {TRIGGER_EXIT, 140, CONDITION_ALWAYS, 49},
    {TRIGGER_EXIT, 141, CONDITION_ALWAYS, 50},
    {TRIGGER_EXIT, 142, CONDITION_ALWAYS, 51},
    {TRIGGER_EXIT, 143, CONDITION_ALWAYS, 54},
    {TRIGGER_EXIT, 144, CONDITION_ALWAYS, 55},
    {TRIGGER_EXIT, 145, CONDITION_ALWAYS, 56},
    {TRIGGER_EXIT, 146, CONDITION_ALWAYS, 57},
    {TRIGGER_EXIT, 147, CONDITION_ALWAYS, 58},
    {TRIGGER_EXIT, 148, CONDITION_ALWAYS, 59},
    {TRIGGER_STEP, STEP_TRIGGER_AREA(AREA_PROX), CONDITION_ALWAYS,
     EVENT(WorldMap_EnterProx)},
    {TRIGGER_EXIT, 151, CONDITION_ALWAYS, 82},
    {TRIGGER_EXIT, 152, CONDITION_ALWAYS, 83},
    {TRIGGER_EXIT, 153, CONDITION_ALWAYS, 84},
    {TRIGGER_EXIT, 154, CONDITION_ALWAYS, 84},
    {TRIGGER_EXIT, 155, CONDITION_ALWAYS, 87},
    {TRIGGER_EXIT, 156, CONDITION_ALWAYS, 88},
    {TRIGGER_EXIT, 157, CONDITION_ALWAYS, 89},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXIT, 42, CONDITION_ALWAYS, 42},
    {TRIGGER_EXIT, 43, CONDITION_ALWAYS, 43},
    {TRIGGER_EXIT, 44, CONDITION_ALWAYS, 44},
    {TRIGGER_EXIT, 45, CONDITION_ALWAYS, 45},
    {TRIGGER_STEP, 52, CONDITION_ALWAYS, EVENT(WorldMap_GoAboard)},
    {TRIGGER_STEP, 53, CONDITION_ALWAYS, EVENT(WorldMap_RefuseToRunAway)},
    {TRIGGER_STEP, 40, 0xa78, EVENT(WorldMap_TakeOff)},
    {TRIGGER_EXIT, 60, CONDITION_ALWAYS, 60},
    {TRIGGER_EXIT, 61, CONDITION_ALWAYS, 61},
    {TRIGGER_EXIT, 85, CONDITION_ALWAYS, 85},
    {TRIGGER_EXIT, 86, CONDITION_ALWAYS, 86},
    {TRIGGER_TALK, ACTOR_GOLDEN_BOOTS, CONDITION_ALWAYS,
     EVENT(WorldMap_PickUpGoldenBoots)},
    {TRIGGER_TABLE_END},
};

LATE_DATA u32 gWorldMap_CollapseDustScript[] = {
    0,
    16,
    38,
};

LATE_DATA s32 gWorldMap_PlacedActorScript[] = {
    46,
    (s32)WorldMap_SetPlacedActorTile,
    17,
    0,
};

LATE_DATA s32 gWorldMap_GlowColorStep = 0;

/// @file
/// The World Map (SceneId_WorldMap), the perspective map that joins the areas,
/// and the only scene of this overlay.
///
/// It lets the party into the areas as The Broken Seal's does. On it the party
/// meets Echo and six more Djinn, sails and later flies the Lemurian Ship,
/// gazes into a pond and finds the Golden Boots, and the story plays several
/// of its scenes here: Venus Lighthouse lit and falling as Idejima breaks away
/// and drifts north on a tidal wave, the first voyage at the tiller, Loho's
/// men bringing the cannon aboard, the bandit and thieves from Vault after a
/// linked game, the party carried in sparkles to Aqua Rock, and Jupiter's
/// beacon seen lit from the sea.

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
#include "field/field_sequence.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/actor_view.h"
#include "field/call.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_coordinate.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "field/map.h"
#include "field/map_grid.h"
#include "field/perspective.h"
#include "field/scene_tables.h"
#include "field/sprite.h"
#include "field/world_map.h"
#include "game/game.h"
#include "game/owner_state.h"
#include "graphics/palette_buffers.h"
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

/// A FieldSprite read as its sprite attribute 1, whose top two bits are the
/// sprite size where FieldSprite names them its flips.
struct WorldMapOam {
  u8 filler[6]; ///< FieldSprite's unused bytes and attribute 0, not read here
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
  CELL_BEHAVIOR_WORLD_MAP_CORE,
  /// The landing where the ship moors at Atteka Inlet
  /// (WorldMap_MoorAtAttekaInlet).
  CELL_BEHAVIOR_WORLD_MAP_ATTEKA_INLET_LANDING = 52,
  /// The ground round the ways out of Gondowan that Jenna will not take
  /// before the parties meet at Contigo (WorldMap_RefuseToRunAway).
  CELL_BEHAVIOR_WORLD_MAP_AWAY_FROM_CONTIGO = 53
};

/// The World Map's blocks (struct WorldMapBlocks): 32 pixels square, 32 to a
/// row and a column.
#define WORLD_MAP_BLOCK_SIZE toFIXED(32)
#define WORLD_MAP_BLOCKS_ACROSS 32
/// A block's terrain byte: the party may step ashore from the open sea here.
#define WORLD_MAP_BLOCK_LANDING (1 << 6)

/// The step trigger an area's placed actor raises when touched
/// (gWorldMap_PlacedActors, from field/world_map.h's STEP_TRIGGER_AREA_BASE),
/// as The Broken Seal's; the areas with two ways in choose one by the leader's
/// approach.
#define STEP_TRIGGER_AREA(area) (STEP_TRIGGER_AREA_BASE + (area))
/// Daila's, Yallam's and Prox's slots among gWorldMap_PlacedActors.
enum { PLACED_DAILA = 0, PLACED_YALLAM = 25, PLACED_PROX = 50 };

void Gauge_Update(void);

void WorldMap_SelectExitByZ(s32 slot, s32 exitBefore, s32 exitAfter);

void Actor_SetFlightMode(struct Actor *actor, s32 mode);

void Gauge_Init(void);
void WorldMap_EndFlight(void);
void Gauge_End(void);

void WorldMap_FightBanditAndThieves(s32 entrance);

void WorldMap_DeliverLohoCannon(void);

void WorldMap_MeetEcho(void);

void WorldMap_ShowIdejimaDrifting(void);

void WorldMap_DriftNorthOfContinent(void);

void WorldMap_ShowAirsRockFromAlhafra(void);

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
void WorldMap_MoorAtAttekaInlet(void);

void WorldMap_RefuseToRunAway(void);

void WorldMap_GazeIntoPond(void);

void WorldMap_PickUpGoldenBoots(void);

void WorldMap_TakeOff(void);

extern s32 gWorldMap_TileSpriteScript[];
extern s32 gWorldMap_IslandScript[];
extern s32 gWorldMap_TidalWaveScript[];

extern u32 gWorldMap_GlowColorModes[];

s32 WorldMap_GetBlockCellBehavior(s32 *position);

/// The party members hidden as the ship first takes off, -1 for none.
s32 gWorldMap_HiddenMembers[8] BSS_DATA;

struct Actor *gWorldMap_PlacedActors[128] BSS_DATA;
u16 gWorldMapBlend BSS_DATA;
s16 gGauge_Shown BSS_DATA ALIGNED(4);
s16 gGauge_Vram BSS_DATA;
s16 gGauge_Countdown BSS_DATA;
u32 gGauge_Sprite[3] BSS_DATA;
u8 gGauge_Tiles[8][32] BSS_DATA;
u8 gGauge_SourceTiles[12][32] BSS_DATA;

/// The tidal wave's tiles, pictures and palettes, tagged LZ.
extern u8 gWorldMapPackedTilesB[];
extern u8 gWorldMapPackedPicturesB[];
extern u8 gWorldMapPalettesB[];

void WorldMap_SpawnTwinSparks(struct Actor *actor);

void Actor_Release(struct Actor *actor);

void Party_DrainPp(s32 value);
FIXED Party_GetPpRatio(void);

void WorldMap_WatchVenusLighthouseCollapse(void);

s32 Event_PickUpItem(s32 actor, s32 item, s32 flag);
void Actor_StartRenderModeScript(void);
s32 Field_FindNearbyTalkTarget(s32 owner);
s32 WorldMap_IsWalkable(struct Actor *actor, s32 *position);
void Party_GivePiersDjinn(void);
void Event_WaitForZoomTransition(void);
void DjinnMenu_Run(void);
void Party_RestoreHpPp(s32 value);

/// Once the party has walked nine tenths of the encounter threshold, a coin
/// toss either requests the Djinni's battle now or makes the party walk the
/// whole threshold first. From the international editions on, only while the
/// party walks the World Map, not aboard the ship or as sand.
#if !TLA_PATCH_1
#define WORLD_MAP_TRY_ENCOUNTER(actorId, djinni)                               \
  {                                                                            \
    struct FieldWork *work;                                                    \
    struct MainRoutineWork *party;                                             \
                                                                               \
    work = Ram_HeapSlots->fieldWork;                                           \
    party = &gMainRoutineWork;                                                 \
    if (party->encounterSteps >= work->encounterThreshold * 9 / 10) {          \
      if (Random16() < 32768) {                                                \
        Event_RequestDjinnBattle(actorId, djinni);                             \
        work->encounterBias = 0;                                               \
      } else {                                                                 \
        party->encounterSteps = work->encounterThreshold;                      \
      }                                                                        \
    }                                                                          \
  }
#else
#define WORLD_MAP_TRY_ENCOUNTER(actorId, djinni)                               \
  {                                                                            \
    struct MainRoutineWork *party;                                             \
    struct FieldWork *work;                                                    \
                                                                               \
    party = &gMainRoutineWork;                                                 \
    work = Ram_HeapSlots->fieldWork;                                           \
    if (party->movementMode == LEADER_MOVEMENT_WORLD_MAP) {                    \
      if (party->encounterSteps >= work->encounterThreshold * 9 / 10) {        \
        if (Random16() < 32768) {                                              \
          Event_RequestDjinnBattle(actorId, djinni);                           \
          work->encounterBias = 0;                                             \
        } else {                                                               \
          party->encounterSteps = work->encounterThreshold;                    \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }
#endif

/// Walking about the patch of ground east of Madra, over the small bridge and
/// down by the water, may meet the Venus Djinni Iron and start his battle, as
/// a random battle would (WORLD_MAP_TRY_ENCOUNTER); if he escapes, walking on
/// there meets him again.
void WorldMap_MeetIron(void) {
  WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_IRON, DJINNI_IRON)
}

/// Walking about the field between Mikasalla and Osenia Cavern may meet the
/// Mercury Djinni Sour and start his battle.
void WorldMap_MeetSour(void) {
  WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_SOUR, DJINNI_SOUR)
}

/// Walking about the field near the beach far south of Naribwe may meet the
/// Mercury Djinni Chill and start his battle.
void WorldMap_MeetChill(void) {
  WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_CHILL, DJINNI_CHILL)
}

/// Walking about the dead-end inlet south-west of Tundaria Tower may meet the
/// Jupiter Djinni Wheeze and start his battle.
void WorldMap_MeetWheeze(void) {
  WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_WHEEZE, DJINNI_WHEEZE)
}

/// Walking about the patch of ground north of Hesperia may meet the Venus
/// Djinni Petra and start her battle.
void WorldMap_MeetPetra(void) {
  WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_PETRA, DJINNI_PETRA)
}

/// Walking about the clearings in the east of Atteka, the lower continent
/// beyond its western beach, may meet the Mars Djinni Core and start his
/// battle.
void WorldMap_MeetCore(void) {
  WORLD_MAP_TRY_ENCOUNTER(DJINN_NO_HOP | ACTOR_CORE, DJINNI_CORE)
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "shared/tile_sprite_reset.h"

/// The marker blinks and bobs up and down on a circle.
/// As The Broken Seal's WorldMap_CircleEcho.
void WorldMap_CircleEcho(struct BobbingMarker *marker) {
  s16 *angle;
  FIXED y;
  FIXED z;
  FIXED radius;
  FIXED rise;

  if (gFrameCount & 2)
    Actor_SetSpriteColorMode(marker, SPRITE_COLOR_RED_TINT);
  else
    Actor_SetSpriteColorMode(marker, SPRITE_COLOR_WHITE);
  if (marker->stopped == 0) {
    angle = &marker->angle;
    marker->x.fixed = toFIXED(10120);
    y = Math_MulFX(Math_Sin(*angle << 3), toFIXED(4));
    z = toFIXED(8356);

    rise = toFIXED(16);
    marker->y.fixed = y + rise;
    marker->z.fixed = z;
    radius = rise;
    FieldVector_AddPolarOffsetFX(radius, *angle, &marker->x);
    marker->facing = *angle + DEGtoANG(90);
    *angle += TRIG_TURN / 64;
  }
}

/// Until the party has Echo or has learnt about Djinn, Echo circles south of
/// Daila (WorldMap_CircleEcho), at half size.
s32 WorldMap_SetUpEcho(struct Actor *actor) {
  if (Event_GetFlag(FLAG_DJINNI(DJINNI_ECHO)) != 0) {
    return 0;
  }
  if (Event_GetFlag(FLAG_DJINN_LESSON) != 0) {
    return 0;
  }
  actor->update = (ObjectUpdate)WorldMap_CircleEcho;
  actor->motionFlags = 0;
  actor->timer = 0;
  actor->userValue = 0;
  actor->scaleX = FIXED_ONE / 2;
  actor->scaleY = FIXED_ONE / 2;
  return 0;
}

/// Turns the island's sprite with the World Map's view and shows it.
s32 WorldMap_SetIslandTile(struct Actor *actor) {
  u16 *yaw = &((struct WorldMapView *)Ram_HeapSlots->mapGrid)->yaw;
  struct FieldSprite *sprite = actor->sprite;

  sprite->rotation = *yaw;
  sprite->flags = 0;
  return 1;
}

/// Takes the tidal wave's shadow away (its sprite flags cleared).
s32 WorldMap_SetUpWave(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

/// Once the scene's first temporary flag is set, the tidal wave loses its
/// shadow and rolls slowly 30 pixels west and 8 south.
s32 WorldMap_SendWaveIn(struct Actor *actor) {
  FIXED x, z;

  if (!Event_GetFlag(FLAG_SCENE_TEMP_00))
    return 1;
  Actor_SetSpriteFlags(actor, 0);
  x = actor->x.fixed;
  actor->acceleration = toFIXED(1) / 16;
  actor->speedLimit = toFIXED(1) / 16;
  z = actor->z.fixed;
  FieldActor_SetMoveTargetFX(actor, x - toFIXED(30), actor->y.fixed,
                             z + toFIXED(8));
  return 0;
}

extern struct ActorPlacement gWorldMapActorPlacements[];
extern struct ActorPlacement gWorldMap_CollapsePlacements[];
extern struct ActorPlacement gWorldMap_DriftPlacements[];
extern struct ActorPlacement gWorldMap_FirstVoyagePlacements[];
extern struct ActorPlacement gWorldMap_DriftingIdejimaPlacements[];

/// The story's scenes the world map plays on arrival that no other scene's exit
/// leads to; the others it tests by scene_exits.h's names.
enum WorldMapEntrance {
  WORLD_MAP_PIERS_DJINN = 70,
  WORLD_MAP_BANDIT_BEATEN = 73, ///< after the battle with the bandit
  WORLD_MAP_BANDIT_LOST = 74,
  WORLD_MAP_DEBUG_FLIGHT = 98, ///< the debug menu's: the ship flying
  WORLD_MAP_DEBUG_SAILING = 99
};

/// The exit the Venus Lighthouse's collapse leaves by, to the world map's own
/// entrance where Idejima drifts; the story's other exits are scene_exits.h's.
enum WorldMapExit { WORLD_MAP_TO_IDEJIMA_DRIFTING = 64 };

void Leader_Search(void);

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  switch (gMainRoutineWork.entrance) {
  case WORLD_MAP_VENUS_COLLAPSE:
    return gWorldMap_CollapsePlacements;
  case WORLD_MAP_DRIFT_NORTH:
    return gWorldMap_DriftPlacements;
  case WORLD_MAP_FIRST_VOYAGE:
    if (!Event_GetFlag(FLAG_SHIP_CAN_SAIL))
      return gWorldMap_FirstVoyagePlacements;
  case WORLD_MAP_IDEJIMA_DRIFTING:
    return gWorldMap_DriftingIdejimaPlacements;
  default:
    return gWorldMapActorPlacements;
  }
}

/// When the ship takes to the air over the World Map, "Hover" is shown
/// while it rises, and the PP gauge appears, since flying uses the party's PP.
void WorldMap_SetUpFlyingShip(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  gMainRoutineWork.movementMode = LEADER_MOVEMENT_FLYING_SHIP;
  if (Event_GetFlag(FLAG_SHIP_HOVERING)) {
    WorldMap_Freeze();
    MessageArgument_Push(ABILITY_HOVER, MESSAGE_ARGUMENT_ABILITY);
    // "Hover"
    Message_ShowCentered((s32)MsgCastAbilityName, 4);
  }
  if (Actor_Get(FieldParty_GetLeaderActor()) != NULL)
    Actor_StartRenderModeScript();
  Leader_Search();
  if (Event_GetFlag(FLAG_SHIP_HOVERING))
    work->climbing = 0;
  else
    work->climbing = 1;
  if (Event_GetFlag(FLAG_SHIP_HOVERING)) {
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

/// The ship sails the map again, out of flight: the PP gauge goes, and a party
/// with no PP left at all gives the leader 1 back.
void WorldMap_SetUpShip(void) {
  s32 count;
  s32 pp;
  s32 i;

  gMainRoutineWork.movementMode = LEADER_MOVEMENT_SHIP;
  WorldMap_EndFlight();
  Gauge_End();
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SHIP), 0);
  count = Party_CountMembers();
  pp = 0;
  for (i = 0; i < count; i++) {
    // One member's PP.
    pp += Unit_Get(gMainRoutineWork.party[i])->pp;
  }
  if (pp == 0)
    Unit_AddPp(gMainRoutineWork.leaderActorId, 1);
}

void Actor_StartIdle(struct Actor *actor);

/// The leader jumps aboard the ship when close enough.
void WorldMap_BoardShip(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader;
  struct Actor *ship;
  s32 kind, dx, dz, distance;

  if (GameFlag_Get(FLAG_SHIP_CAN_SAIL) == 0)
    return;
  kind = Field_FindNearbyTalkTarget(gMainRoutineWork.leaderActorId);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  ship = Actor_Get(ACTOR_SHIP);
  dx = FIXEDtoINT(leader->x.fixed - ship->x.fixed);
  dz = FIXEDtoINT(leader->z.fixed - ship->z.fixed);
  distance = dx * dx + dz * dz;
  if (kind != 8 && distance > 255)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  gMainRoutineWork.movementMode = LEADER_MOVEMENT_SHIP;
  gMainRoutineWork.ownerOverride = 1;
  Event_SetFlag(FLAG_SAILING);
  gMainRoutineWork.shipX = ship->x.fixed;
  gMainRoutineWork.shipZ = ship->z.fixed;
  gMainRoutineWork.shipFacing = ship->facing;
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Actor_SelectAnimation(leader, 6);
  System_Wait(6);
  Event_PlaySound(SE_JUMP);
  Actor_SetSpriteFlags(leader, 0);
  Actor_SelectAnimation(leader, 7);
  leader->speedLimit = toFIXED(3);
  leader->acceleration = toFIXED(2);
  leader->velocityY = toFIXED(4);
  leader->groundHeight = toFIXED(4);
  Actor_StartIdle(leader);
  FieldActor_SetMoveTargetFX(leader, ship->x.fixed, 0, ship->z.fixed);
  FieldActor_WaitForMove(leader);
  Actor_SelectAnimation(leader, 6);
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
  FieldEffect_ScheduleRatioTransition(FIXED_ONE, 30);
}

/// The leader jumps ashore from the ship, where the shore ahead lets them land.
void WorldMap_LeaveShip(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *ship = Actor_GetById(ACTOR_SHIP);
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 cell = WorldMap_GetCollision(&ship->x.fixed);
  FIXED position[3];
  s32 shore = 0;
  s32 blocked;
  struct WorldMapBlock *block;
  struct WorldMapBlocks *blocks = (struct WorldMapBlocks *)gCellPictures;
  u8 flags;

  position[0] = ship->x.fixed;
  position[1] = ship->y.fixed;
  position[2] = ship->z.fixed;
  Vector_AddPolarOffset(toFIXED(24), ship->facing, position);
  blocked = 0;
  block = &blocks->blocks[(position[0] / WORLD_MAP_BLOCK_SIZE &
                           WORLD_MAP_BLOCKS_ACROSS - 1) +
                          (position[2] / WORLD_MAP_BLOCK_SIZE &
                           WORLD_MAP_BLOCKS_ACROSS - 1) *
                              WORLD_MAP_BLOCKS_ACROSS];
  if (cell > WORLD_MAP_COLLISION_WALKABLE_LAST) {
    blocked = 1;
  } else {
    // Collisions 1 to 3, the open sea (BATTLE_ENVIRONMENT_SEA's), short of
    // WORLD_MAP_COLLISION_SEA_LAST.
    if ((u32)(cell - WORLD_MAP_COLLISION_SEA_FIRST) <= 2)
      shore = 1;
    if ((block->unusedAfterPlace & WORLD_MAP_BLOCK_LANDING) == 0 &&
        shore != 0) {
      blocked = 1;
    } else {
      position[0] += toFIXED(6);
      blocked |= WorldMap_IsWalkable(ship, position);
      position[0] += -toFIXED(12);
      blocked |= WorldMap_IsWalkable(ship, position);
      position[0] += toFIXED(6);
      position[1] += toFIXED(6);
      blocked |= WorldMap_IsWalkable(ship, position);
      position[1] += -toFIXED(12);
      blocked |= WorldMap_IsWalkable(ship, position);
      position[1] += toFIXED(6);
      blocked |= WorldMap_IsWalkable(ship, position);
    }
  }
  if (blocked == 0) {
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEffect_ScheduleRatioTransition(FIXED_ONE, 10);
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_WORLD_MAP;
    gMainRoutineWork.ownerOverride = 0;
    Event_ClearFlag(FLAG_SAILING);
    gMainRoutineWork.shipX = ship->x.fixed;
    gMainRoutineWork.shipZ = ship->z.fixed;
    gMainRoutineWork.shipFacing = ship->facing;
    flags = leader->motionFlags;
    leader->x.fixed = ship->x.fixed;
    leader->y.fixed = toFIXED(4);
    leader->z.fixed = ship->z.fixed;
    leader->motionFlags = 0;
    Actor_SetSpriteFlags(leader, 0);
    Actor_SelectAnimation(leader, 6);
    System_Wait(6);
    Event_PlaySound(SE_JUMP);
    Actor_SelectAnimation(leader, 7);
    leader->speedLimit = toFIXED(3);
    leader->acceleration = toFIXED(2);
    leader->velocityY = toFIXED(4);
    leader->motionFlags = flags;
    Actor_StartIdle(leader);
    FieldActor_SetMoveTargetFX(leader, position[0], 0, position[2]);
    FieldActor_WaitForMove(leader);
    Actor_SelectAnimation(leader, 6);
    System_Wait(6);
    Event_End();
    Scene_ChooseMusic();
    Scene_PlayMusic();
    work->aButton = 0;
  }
}

void Actor_PlaceOnActor(s32 actor, s32 target);

/// Sailing up to the landing at Atteka Inlet moors the ship there: the
/// screen fades and the party goes ashore into the inlet, by the ship's
/// cabins. Where the ship lay is kept, to find it there again; a flying ship
/// comes down to the water first.
void WorldMap_MoorAtAttekaInlet(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_Get(ACTOR_SHIP);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_FLYING_SHIP)
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
    Actor_SetDestination(actor, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  Actor_WaitForMoveById(actor);
  Actor_SetPosition(actor, 0, 0);
}

/// Once Jupiter Lighthouse is lit and before the parties meet at Contigo,
/// Jenna stops Felix leaving by any other exit: they must go to Contigo.
void WorldMap_RefuseToRunAway(void) {
  struct Actor *actor;
  s32 x;
  s32 z;
  s32 southwest = FACING_SOUTHWEST;
  s32 west = FACING_WEST;

  if (Event_GetFlag(FLAG_JUPITER_BEACON_LIT) &&
      !Event_GetFlag(FLAG_REUNION_AT_CONTIGO)) {
    actor = Actor_Get(gMainRoutineWork.leaderActorId);
    x = actor->x.fixed;
    z = actor->z.fixed;
    x >>= 20;
    z >>= 20;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (Event_GetFlag(FLAG_JENNA_POINTED_TO_CONTIGO)) {
      FieldEffect_ScheduleRatioTransition(FIXED_ONE * 7 / 5, 6);
      Actor_PlaceOnActor(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
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
      FieldEffect_ScheduleRatioTransition(FIXED_ONE, 6);
    } else {
      s32 besideZ;

      FieldEffect_ScheduleRatioTransition(FIXED_ONE * 7 / 5, 6);
      Actor_PlaceOnActor(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
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
      Event_SetFlag(FLAG_JENNA_POINTED_TO_CONTIGO);
      FieldEffect_ScheduleRatioTransition(FIXED_ONE, 6);
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

/// Searching the pond: one time in sixteen there are coins in the shallows,
/// one in sixteen the boat rocks, and otherwise one of the pond's lines.
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
    s32 amount = (((unit->level * Random16()) << 1) >> 16) + SCOOPED_COINS + 1;
    FIXED position[3];

    // "There are some coins in the shallows!"
    Message_ShowCentered((s32)MsgThereAreSome, 1);
    position[0] = actor->x.fixed;
    position[1] = actor->y.fixed;
    position[2] = actor->z.fixed;
    FieldVector_AddPolarOffset(16, actor->facing, position);
    actor = Actor_Get(ACTOR_POND_COINS);
    if (actor == NULL) {
      actor = FieldActor_Create(SPRITE_UNSEEN_MARKER, position[0], position[1],
                                position[2]);
      if (actor == NULL)
        return;
      work->actors[slot] = actor;
    } else {
      actor->x.fixed = position[0];
      actor->y.fixed = position[1];
      actor->z.fixed = position[2];
    }
    FieldActor_ShowItemIconById(ACTOR_POND_COINS, amount);
    /* FAKEMATCH(dead): the store and the call in a once-only block. */
    do {
      work->scoopedItem = amount;
      Scene_TalkToActor(FIELD_TALK_SILENT | slot);
    } while (0);
  } else if (roll == 1) {
    MapGrid_SetShake(3, 5, 1);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    FieldEvent_Wait(30);
    // "The boat's rocking! This thing's dangerous! I almost fell out!"
    Message_ShowCentered((s32)MsgTheBoatS, 1);
  } else {
    Message_ShowCentered((s32)MsgTheBoatS + (roll >> 1), 1);
  }
  Event_End();
}

/// Searching the Golden Boots picks them up.
void WorldMap_PickUpGoldenBoots(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (Value3((s32 (*)())Event_PickUpItem, ACTOR_GOLDEN_BOOTS, ITEM_GOLDEN_BOOTS,
             -1) == 0)
    Event_ClearFlag(FLAG_GOLDEN_BOOTS_ON_MAP);
  Event_End();
}

/// Places the Golden Boots on the map as an item icon
/// (FLAG_GOLDEN_BOOTS_ON_MAP).
void WorldMap_ShowGoldenBoots(void) {
  struct Actor *actor;
  struct FieldSprite *sprite;
  s32 a, id;

  FieldActor_ShowItemIconById(ACTOR_GOLDEN_BOOTS, ITEM_GOLDEN_BOOTS);
  actor = Actor_Get(ACTOR_GOLDEN_BOOTS);
  sprite = actor->sprite;
  ((struct SpriteCacheBits *)&sprite->cacheBits)->retainCacheEntry = 1;
  Actor_SetSpriteFlags(Actor_Get(ACTOR_GOLDEN_BOOTS), 0);
  a = FACING_SOUTH;
  id = ACTOR_GOLDEN_BOOTS;
  Actor_SetPositionAndFacing(id, toFIXED(10168), toFIXED(9648), a);
  Event_SetFlag(FLAG_GOLDEN_BOOTS_ON_MAP);
}

/// Walking onto Prox from the World Map: coming from the north, the party
/// enters by its northern way in (SceneId_ProxMold), and from the south by
/// its south gate. Yallam and Daila choose their way in the same way.
void WorldMap_EnterProx(s32 id) {
  WorldMap_SelectExitByZ(id, WORLD_MAP_TO_PROX_MOLD,
                         WORLD_MAP_TO_PROX_SOUTH_GATE);
}

/// Walking onto Yallam from the World Map: coming from the north, the party
/// enters by its north side, and from the south by its south side.
void WorldMap_EnterYallam(s32 id) {
  WorldMap_SelectExitByZ(id, WORLD_MAP_TO_YALLAM_SOUTH_B,
                         WORLD_MAP_TO_YALLAM_SOUTH_A);
}

/// Walking onto Daila from the World Map: coming from the north, the party
/// enters by its north side, and from the south by its south side.
void WorldMap_EnterDaila(s32 id) {
  WorldMap_SelectExitByZ(id, WORLD_MAP_TO_DAILA_NORTH,
                         WORLD_MAP_TO_DAILA_SOUTH);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

extern s32 gWorldMap_EchoScript[];

s32 Scene_Initialize(void) {
  struct FieldWork *work;

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0 &&
      GameFlag_Get(FLAG_SHIP_CAN_SAIL) != 0 &&
      gMainRoutineWork.ownerOverride == 0 && gMainRoutineWork.shipX != 0) {
    struct Actor *ship = Actor_GetById(ACTOR_SHIP);

    ship->x.fixed = gMainRoutineWork.shipX;
    ship->z.fixed = gMainRoutineWork.shipZ;
    ship->facing = gMainRoutineWork.shipFacing;
    ship->timer = 0;
    ship->userValue = 0;
  }
  if (gMainRoutineWork.ownerOverride != 0)
    FieldEffect_ScheduleRatioTransition(FIXED_ONE, 1);
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
  if (GameFlag_Get(FLAG_GOLDEN_BOOTS_ON_MAP))
    WorldMap_ShowGoldenBoots();
  switch (gMainRoutineWork.entrance) {
  case WORLD_MAP_FROM_GAIA_ROCK_SUSA:
    if (GameFlag_Get(FLAG_SERPENT_SLAIN))
      GameFlag_Set(FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE);
    break;
  case WORLD_MAP_FROM_MADRA:
  case WORLD_MAP_BANDIT_BEATEN:
  case WORLD_MAP_BANDIT_LOST:
    if (GameFlag_Get(FLAG_BANDIT_ON_WORLD_MAP))
      WorldMap_FightBanditAndThieves(gMainRoutineWork.entrance);
    break;
  case WORLD_MAP_IDEJIMA_DRIFTING:
    WorldMap_ShowIdejimaDrifting();
    break;
  case WORLD_MAP_CARRIED_TO_AQUA_ROCK:
    WorldMap_CarryPartyToAquaRock();
    break;
  case WORLD_MAP_FROM_LOHO:
    if (!Event_GetFlag(FLAG_SHIP_CARRIES_CANNON) &&
        GameFlag_Get(FLAG_LOHO_WALL_BREACHED) != 0) {
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
  case WORLD_MAP_FROM_DAILA_SOUTH:
    if (GameFlag_Get(FLAG_DJINNI(DJINNI_ECHO)) == 0) {
      Actor_SetScript(ACTOR_ECHO, gWorldMap_EchoScript);
      WorldMap_MeetEcho();
    }
    break;
  case WORLD_MAP_ALHAFRA_MANOR_VIEW:
    WorldMap_ShowAirsRockFromAlhafra();
    break;
  case WORLD_MAP_FIRST_VOYAGE:
    WorldMap_TakeTheTiller();
    break;
  case WORLD_MAP_SAIL_TO_DECK:
    WorldMap_SailToShipDeck();
    break;
  case WORLD_MAP_JUPITER_BEACON:
    WorldMap_PlayLighthouseTransition(2);
    FieldActor_SetSpriteColor(Actor_GetById(ACTOR_PIERS), SPRITE_COLOR_HIDDEN);
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
    Event_SetFlag(FLAG_SHIP_CAN_SAIL);
    Event_SetFlag(FLAG_SHIP_FLIES);
    Event_SetFlag(FLAG_DEBUG_STEPS_OFF);
    // Flag 0x11d changes nothing: no code of this game reads it, and only
    // these two debug arrivals set it.
    Event_SetFlag(0x11d);
    Event_SetDestination(SCENE_WORLD_MAP,
                         WORLD_MAP_FROM_NORTHERN_REACHES_NORTH);
    break;
  case WORLD_MAP_DEBUG_SAILING:
    GameFlag_Set(FLAG_DEBUG_STEPS_OFF);
    GameFlag_Set(0x11d);
    break;
  }
  return 0;
}

s32 Menu_AnimateSelectionToEntry(s32 start, s32 command);

s32 Scene_PrepareMap(void) { return 0; }

/// Echo's lesson on Djinn: "After bringing a Djinni into your party, you must
/// set it to somebody." and the Djinn menu, then "Next, I'll show you how you
/// can use us in battle." and the menu again, and a battle. With
/// FLAG_ALREADY_KNOWS_DJINN set he says the lesson's second copy of its lines.
void WorldMap_ExplainGivingDjinn(void) {
  WorldMap_ReloadPendingAreaGraphics();
  FieldEffect_ScheduleRatioTransition(FIXED_ONE, 6);
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
  if (entrance == WORLD_MAP_FROM_MADRA) {
    north = FACING_NORTH;
    Actor_SetPositionAndFacing(ACTOR_BANDIT, toFIXED(10168), toFIXED(9000),
                               north);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    WorldMap_Freeze();
    Event_SetMessageId((s32)MsgHeyWaitA);
    // "Hey! Wait a minute!"
    FieldEvent_ShowMessageThenWait(ACTOR_BANDIT, 0, 5);
    WorldMap_ReloadPendingAreaGraphics();
    FieldEffect_ScheduleRatioTransition(FIXED_ONE * 31 / 20, 16);
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
    Actor_SetPositionAndFacing(ACTOR_BANDIT, toFIXED(10168), toFIXED(9600),
                               south);
    Actor_SetPositionAndFacing(ACTOR_THIEF_A, toFIXED(10168), toFIXED(9600),
                               south);
    Actor_SetPositionAndFacing(ACTOR_THIEF_B, toFIXED(10168), toFIXED(9600),
                               south);
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
    Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 3);
  } else if (entrance == WORLD_MAP_BANDIT_LOST) {
    Event_ClearFlag(FLAG_BANDIT_ON_WORLD_MAP);
    facingSouth = FACING_SOUTH;
    Actor_SetPositionAndFacing(ACTOR_BANDIT, toFIXED(10168), toFIXED(9616),
                               facingSouth);
    Actor_SetPositionAndFacing(ACTOR_THIEF_A, toFIXED(10152), toFIXED(9616),
                               facingSouth);
    Actor_SetPositionAndFacing(ACTOR_THIEF_B, toFIXED(10184), toFIXED(9616),
                               facingSouth);
    {
      Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(10152), toFIXED(9664),
                                 FACING_NORTHEAST);
    }
    Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(10168), toFIXED(9648),
                               FACING_NORTHWEST);
    EventCamera_FollowActor(ACTOR_FELIX, 0);
    FieldEffect_ScheduleRatioTransition(FIXED_ONE * 31 / 20, 1);
    Actor_SetAnimation(ACTOR_FELIX, ANIM_COLLAPSE);
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
    Event_SetDestination((s16)gMainRoutineWork.restartScene,
                         (s16)gMainRoutineWork.restartEntrance);
    gFieldWork->transition = 0;
  } else {
    Event_ClearFlag(FLAG_BANDIT_ON_WORLD_MAP);
    southB = FACING_SOUTH;
    Actor_SetPositionAndFacing(ACTOR_BANDIT, toFIXED(10168), toFIXED(9632),
                               southB);
    northB = FACING_NORTH;
    Actor_SetPositionAndFacing(ACTOR_THIEF_A, toFIXED(10152), toFIXED(9632),
                               southB);
    Actor_SetPositionAndFacing(ACTOR_THIEF_B, toFIXED(10184), toFIXED(9632),
                               southB);
    Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(10160), toFIXED(9680),
                               northB);
    Actor_SetPositionAndFacing(ACTOR_FELIX, toFIXED(10176), toFIXED(9680),
                               northB);
    EventCamera_FollowActor(ACTOR_FELIX, 0);
    FieldEffect_ScheduleRatioTransition(FIXED_ONE * 3 / 2, 1);
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
    Event_OpenMessage(ACTOR_BANDIT, 0);
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
      struct Actor *actor = Actor_Get(ACTOR_FELIX);

      if (actor != NULL)
        Actor_SetDestination(ACTOR_KRADEN, actor->x.part[0].integer,
                             actor->z.part[0].integer);
    }
    Actor_WaitForMoveById(ACTOR_KRADEN);
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    WorldMap_ReloadPendingAreaGraphics();
    FieldEffect_ScheduleRatioTransition(FIXED_ONE, 15);
    Event_End();
  }
}

extern u32 gWorldMap_LohoWorkerLeaveScriptA[];
extern u32 gWorldMap_LohoWorkerLeaveScriptB[];
extern u32 gWorldMap_LohoWorkerLeaveScriptC[];

/// Loho's men carry the cannon aboard the ship and go home.
void WorldMap_DeliverLohoCannon(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_Get(ACTOR_LOHO_WORKER_A)->motionFlags = ACTOR_FALLS;
  Actor_Get(ACTOR_LOHO_WORKER_B)->motionFlags = ACTOR_FALLS;
  Actor_Get(ACTOR_LOHO_WORKER_C)->motionFlags = ACTOR_FALLS;
  {
    Actor_SetPositionAndFacing(ACTOR_LOHO_WORKER_A, toFIXED(8913),
                               toFIXED(5853), FACING_NORTHEAST);
    Actor_SetPositionAndFacing(ACTOR_LOHO_WORKER_B, toFIXED(8929),
                               toFIXED(5869), FACING_NORTHEAST);
    Actor_SetPositionAndFacing(ACTOR_LOHO_WORKER_C, toFIXED(8929),
                               toFIXED(5885), FACING_NORTHEAST);
  }
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_LOHO_WORKER_A, 0.2);
  Actor_SetSpeed(ACTOR_LOHO_WORKER_B, 0.2);
  Actor_SetSpeed(ACTOR_LOHO_WORKER_C, 0.2);
  Actor_WalkBy(ACTOR_LOHO_WORKER_A, 10, 0);
  Actor_WalkBy(ACTOR_LOHO_WORKER_B, 10, 0);
  Actor_WalkBy(ACTOR_LOHO_WORKER_C, 10, 0);
  Event_WaitTransitionFrames();
  FieldEffect_ScheduleRatioTransition(FIXED_ONE * 12 / 10, 6);
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
  FieldEffect_ScheduleRatioTransition(FIXED_ONE, 6);
  Event_End();
}

/// A message of Echo's Djinn lesson: the second set (from
/// MsgDjinnGiveTutorialB), for a player who knows Djinn already, when
/// secondSet is 1.
#define TUTORIAL_SET_MESSAGE(secondSet, base)                                  \
  ((secondSet) * ((s32)MsgDjinnGiveTutorialB - (s32)MsgDjinnGiveTutorialA) +   \
   (s32)(base))

/// A message of Echo's Djinn lesson, from its second set when the player held
/// Up as the lesson began and Echo says "You look like you already know a
/// little about us, huh?".
#define TUTORIAL_MESSAGE(base)                                                 \
  TUTORIAL_SET_MESSAGE(Event_GetFlag(FLAG_ALREADY_KNOWS_DJINN), base)

/// The first Djinni, Echo, joins south of Daila, however often he is refused.
void WorldMap_MeetEcho(void) {
  struct Actor *djinni;
  struct Actor *leader;
  FIXED x;
  FIXED z;
  s32 owner;
  s32 count;
  s32 i;
  s32 flag;
  // Echo is a Venus Djinni.
  s32 element = ELEMENT_VENUS;

  djinni = Actor_GetById(ACTOR_ECHO);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  x = (leader->x.fixed - toFIXED(10120)) / 2 + toFIXED(10120);
  z = (leader->z.fixed - toFIXED(8356)) / 2 + toFIXED(8356);
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
    Djinn_Give(owner, element, 7);
    DjinnRecovery_AddStandby(owner, element, 7);
    WorldMap_Freeze();
    Actor_FaceActorById(owner, ACTOR_ECHO, 0);
    Event_Wait(10);
    FieldActor_ShowEmote(owner, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    djinni->userValue = 1;
    Actor_FaceActorById(ACTOR_ECHO, owner, 0);
    wait(16);
    flag = Event_GetFlag(FLAG_ALREADY_KNOWS_DJINN);
    Event_SetMessageId(TUTORIAL_SET_MESSAGE(flag, MsgHey));
    Event_ShowMessageAndWait(ACTOR_ECHO, 0);
    WorldMap_ReloadPendingAreaGraphics();
    FieldEffect_ScheduleRatioTransition(FIXED_ONE * 12 / 10, 6);
    Event_WaitForZoomTransition();
    WorldMap_Freeze();
    djinni->motionFlags = ACTOR_FALLS;
    djinni->gravity = toFIXED(1) / 4;
    djinni->speedLimit = toFIXED(1);
    djinni->acceleration = toFIXED(1);
    djinni->velocityY = 0;
    djinni->groundHeight = 0;
    FieldActor_SetMoveTarget(djinni, 10120, 0, 8356);
    for (i = 0; i < 16; i++) {
      djinni->scaleX += FIXED_ONE / 32;
      djinni->scaleY += FIXED_ONE / 32;
      wait(1);
    }
    Actor_FaceActorById(ACTOR_ECHO, owner, 0);
    Actor_FaceActorById(owner, ACTOR_ECHO, 0);
    wait(16);
    djinni->update = 0;
    Actor_SetSpriteColorMode(djinni, 0);
    djinni->gravity = toFIXED(1);
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
    i = TRIG_TURN / FACING_STEP - 1;
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
    FieldActor_SetMoveTargetFX(djinni, x, toFIXED(16), z);
    // A whole turn as he rises, then half a turn as he drops.
    while (i >= 0) {
      djinni->facing += FACING_STEP;
      wait(1);
      i--;
    }
    Actor_SetAnimationById(owner, 1);
    Event_ShowMessageAndWait(ACTOR_ECHO, 0);
    djinni->motionFlags = ACTOR_FALLS;
    djinni->velocityY = 0;
    djinni->groundHeight = 0;
    i = TRIG_TURN / 2 / FACING_STEP - 1;
    while (i >= 0) {
      djinni->facing += FACING_STEP;
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
    Sound_Play(MUS_A_NEW_ALLY);
    FieldMessage_Show(TUTORIAL_MESSAGE(MsgTheVenusDjinni), 3);
    Event_SetMessageId(TUTORIAL_MESSAGE(MsgTheVenusDjinni + 1));
    Actor_Jump(ACTOR_ECHO, 2, 20);
    FieldMessage_ShowAtActor(ACTOR_ECHO, 0);
    if (Event_AskYesNoWithActorAnimation(owner, 0) == 1)
      goto refused;
    if (Event_GetFlag(FLAG_ALREADY_KNOWS_DJINN))
      Sound_Play(MUS_CHILDRENS_SONG);
    else
      Sound_Play(MUS_BRIGGS_STEALS_SHIP);
    goto tutorial;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_PlaceOnActor(ACTOR_ECHO, owner);
  djinni->velocityY = toFIXED(10);
  FieldActor_SetMoveTargetFX(djinni, x, 0, z);
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
  Actor_WalkToAndWait(ACTOR_ECHO, FIXEDtoINT(x), FIXEDtoINT(z));
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
  FieldDjinn_JoinParty(9, element, 7);
  Scene_PlayMusic();
  Event_End();
  Event_ClearFlag(FLAG_DJINN_LESSON);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_A);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_B);
}

extern u32 gWorldMap_CollapseDustScript[];

/// A cloud of dust thrown up at a random spot as the earthquake that fells
/// Venus Lighthouse shakes the land, at a random scale.
void WorldMap_SpawnCollapseDust(void) {
  struct Actor *obj;
  struct FieldSprite *sprite;
  u8 *motionFlags;
  FIXED x;
  FIXED z;
  s32 scale;

  x = toFIXED(Random_Below(40));
  z = toFIXED(Random_Below(30));
  obj = (FieldActor_Create(SPRITE_DUST_CLOUD, x + toFIXED(1976), 0,
                           z + toFIXED(7260)));
  if (obj != NULL) {
    sprite = obj->sprite;
    scale = ((Random16() << 15) >> 16) + FIXED_ONE * 6 / 5;
    sprite->flags = 0;
    sprite->priority = 2;
    motionFlags = &obj->motionFlags;
    *motionFlags = 0;
    obj->scaleX = scale;
    obj->scaleY = scale;
    Actor_SelectAnimation(obj, 1);
    Actor_StartScript(obj, gWorldMap_CollapseDustScript);
  }
}

/// Moves the actor by whole pixels.
void WorldMap_ShiftActor(s32 id, s32 dx, s32 dz) {
  struct Actor *actor = Actor_Get(id);

  if (actor != NULL)
    Actor_SetPosition(id, actor->x.part[0].integer + dx,
                      actor->z.part[0].integer + dz);
}

/// Breaks a piece of Idejima's tiles away and moves the party on it with
/// the piece.
void WorldMap_SplitIdejima(s32 x, s32 z, s32 dx, s32 dz) {
  FieldWorldMap_SetWindowCellTileAndDraw(x, z, 48, 224);
  WorldMap_ShiftActor(ACTOR_JENNA, dx, dz);
  WorldMap_ShiftActor(ACTOR_IDEJIMA_KRADEN, dx, dz);
  WorldMap_ShiftActor(ACTOR_ALEX, dx, dz);
  WorldMap_ShiftActor(ACTOR_IDEJIMA, dx, dz);
  FieldWorldMap_SetWindowCellTileAndDraw(x + 16, z, 64, 224);
}

/// Venus Lighthouse is lit and falls; the earthquake breaks Idejima away with
/// Jenna, Kraden and Alex on it.
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
  PaletteFade_ApplyTarget(FIXED_ONE, 0);
  PaletteFade_Interpolate(60);
  Task_AddFrame(WorldMap_SpawnCollapseDust, TASK_PASS_FRAME);
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
  FieldEffect_ScheduleRatioTransition(FIXED_ONE * 3 / 5, 60);
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
  FieldEvent_SetExitRequest(WORLD_MAP_TO_IDEJIMA_DRIFTING);
  FieldEvent_Wait(120);
}

extern s32 gWorldMap_IdejimaWakeScript[];

/// Every sixteenth frame, a puff of spray behind the drifting Idejima.
void WorldMap_SpawnIdejimaWake(void) {
  struct Actor *leader;
  struct Actor *obj;
  struct FieldSprite *sprite;
  u32 value;

  if ((gFrameCount & 15) != 0)
    return;
  leader = Actor_Get(ACTOR_DRIFTING_IDEJIMA);
  obj = FieldActor_Create(SPRITE_DUST_CLOUD, leader->x.fixed - toFIXED(32),
                          leader->y.fixed, leader->z.fixed - toFIXED(16));
  if (obj == NULL)
    return;
  obj->scaleX = FIXED_ONE / 2;
  obj->scaleY = FIXED_ONE / 2;
  sprite = obj->sprite;
  if (Random_Below(2)) {
    FIXED back = (Random_Below(48)) << 16;

    obj->x.fixed -= back >> 1;
    obj->z.fixed -= back;
  } else {
    value = ((Random16() << 5) >> 16) << 16;
    obj->x.fixed += value;
    value = (s32)value >> 1;
    obj->z.fixed += value;
  }
  sprite->flags = 0;
  sprite->priority = ((struct FieldSprite *)leader->sprite)->priority;
  obj->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  obj->motionFlags = leader->motionFlags;
  FieldActor_SetSpriteColor(obj, 9);
  Actor_SelectAnimation(obj, 2);
  Actor_StartScript(obj, gWorldMap_IdejimaWakeScript);
}

extern u32 gWorldMap_IdejimaDriftScript[];

/// The camera follows Idejima as it drifts away north.
void WorldMap_ShowIdejimaDrifting(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_DRIFTING_IDEJIMA);
  FieldEvent_Wait(60);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEffect_ScheduleRatioTransition(FIXED_ONE * 3 / 5, 1);
  actor->scaleX = FIXED_ONE * 12 / 10;
  actor->scaleY = FIXED_ONE * 12 / 10;
  EventCamera_FollowActor(ACTOR_DRIFTING_IDEJIMA, 1);
  System_Wait(1);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId),
                            SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_DRIFTING_IDEJIMA), 0);
  Actor_SetSpeed(ACTOR_DRIFTING_IDEJIMA, 0.4);
  actor->timer = 0;
  Actor_SetScript(ACTOR_DRIFTING_IDEJIMA, gWorldMap_IdejimaDriftScript);
  Task_AddFrame(WorldMap_SpawnIdejimaWake, TASK_PASS_FRAME);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 1);
  gFieldWork->transitionFrames = 32;
  Event_StartScreenTransition();
  FieldEvent_Wait(120);
  FieldEffect_ScheduleRatioTransition(FIXED_ONE * 7 / 5, 300);
  FieldEvent_Wait(270);
  gFieldWork->transitionFrames = 16;
  BG_PLTT_COLOR(0, 0) = RGB_WHITE;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(WORLD_MAP_TO_IDEJIMA_NORTH_B);
}

/// Idejima drifts north; a tidal wave rises behind it and carries it away.
void WorldMap_DriftNorthOfContinent(void) {
  struct Actor *boat = Actor_Get(ACTOR_IDEJIMA);
  struct WorldMapView *view = (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldWorldMap_SetWindowCellTileAndDraw(64, 176, 304, 240);
  FieldWorldMap_SetWindowCellTileAndDraw(80, 176, 320, 240);
  boat->scaleX = FIXED_ONE * 12 / 10;
  boat->scaleY = FIXED_ONE * 12 / 10;
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  System_Wait(1);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId),
                            SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_IDEJIMA), 0);
  Actor_SetSpeed(ACTOR_IDEJIMA, 0.4);
  boat->timer = 0;
  boat->acceleration = toFIXED(1) / 8;
  boat->speedLimit = toFIXED(1) / 8;
  FieldActor_SetMoveTargetFX(boat, toFIXED(10239), boat->y.fixed,
                             toFIXED(7884));
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  WorldMap_Freeze();
  FieldEvent_SetMessageId((s32)MsgWeReSaved);
  // "We're saved!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  // "So it would seem..."
  Event_ShowMessage(ACTOR_IDEJIMA_KRADEN, 0);
  FieldActor_SetMoveTargetFX(boat, toFIXED(10255), boat->y.fixed,
                             toFIXED(7868));
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
  Event_PlaySound(MUS_FATED_CONNECTION_WITH_SWORN_ENEMIES);
  WorldMap_ReloadPendingAreaGraphics();
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(10303, CAMERA_KEEP, 7820, 1);
  for (i = 0; i < 32; i++) {
    view->yaw -= TRIG_TURN / 512;
    view->pitch += TRIG_TURN / 512;
    Actor_Get(ACTOR_IDEJIMA)->scaleY += -(FIXED_ONE * 3 / 256);
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
  FieldEffect_ScheduleRatioTransition(FIXED_ONE * 3 / 5, 1);
  view->yaw = 0;
  view->pitch = -DEGtoANG(45);
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
  FieldEvent_SetExitRequest(WORLD_MAP_TO_IDEJIMA_NORTHEAST);
}

/// When the man gazing out from Alhafra's manor shows Felix Air's Rock, the
/// World Map's view glides over to the rock, Felix hidden; pressing any
/// button returns him to the manor.
void WorldMap_ShowAirsRockFromAlhafra(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->scaleX = 0;
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(11890, CAMERA_KEEP, 10090, 1);
  while (gInput.held == 0)
    wait(1);
  Event_SetDestination(SCENE_ALHAFRA_MANOR, ALHAFRA_MANOR_FROM_WORLD_MAP_VIEW);
}

/// The first voyage: the party takes the ship's tiller and learns to sail.
void WorldMap_TakeTheTiller(void) {
  s32 scale;
  s32 seen = Event_GetFlag(FLAG_SHIP_CAN_SAIL);

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
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_SHIP;
    gMainRoutineWork.ownerOverride = 1;
    WorldMap_ReloadPendingAreaGraphics();
    Event_SetFlag(FLAG_SHIP_CAN_SAIL);
    Actor_SetPosition(ACTOR_PIERS, 0, 0);
    Actor_SetPosition(ACTOR_SHEBA, 0, 0);
    Actor_SetPosition(ACTOR_VOYAGE_KRADEN, 0, 0);
    Actor_SetPosition(ACTOR_JENNA, 0, 0);
    Actor_GetById(gMainRoutineWork.leaderActorId)->scaleX = scale;
    Event_End();
  }
}

/// The ship sails to its berth and the party goes down to the deck.
void WorldMap_SailToShipDeck(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_MoveToAndWait(ACTOR_SHIP, 10720, 9075);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(WORLD_MAP_TO_LEMURIAN_SHIP_TILLER_A);
}

/// The ship's first flight once it has wings (FLAG_SHIP_TOOK_OFF).
void WorldMap_TakeOff(void) {
  s32 x, z, i;

  if (GameFlag_Get(FLAG_SHIP_FLIES) == 0)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_SetFlag(FLAG_SHIP_TOOK_OFF);
  Actor_MoveToAndWait(ACTOR_SHIP, 7158, 8532);
  WorldMap_SetUpShip();
  x = Actor_Get(ACTOR_SHIP)->x.fixed / toFIXED(1);
  z = Actor_Get(ACTOR_SHIP)->z.fixed / toFIXED(1);
  {
    for (i = 0; i <= 7; i++) {
      if (Actor_Get(i) != NULL) {
        gWorldMap_HiddenMembers[i] = i;
        Actor_Get(i)->scaleX = 0;
        Actor_SetSpriteFlags(Actor_Get(i), 0);
        Actor_SetPosition(i, x, z);
      } else {
        gWorldMap_HiddenMembers[i] = -1;
      }
    }
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
  for (i = 0; i <= 7; i++) {
    if (gWorldMap_HiddenMembers[i] >= 0) {
      Actor_SetSpriteFlags(Actor_Get(gWorldMap_HiddenMembers[i]), 1);
      Actor_Get(gWorldMap_HiddenMembers[i])->scaleX = FIXED_ONE;
      Actor_SetPosition(gWorldMap_HiddenMembers[i], 0, 0);
    }
  }
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Event_End();
}

/// A sparkle of the party's path to Aqua Rock: each frame, one time in four,
/// it takes one of the eight colour modes at random.
s32 WorldMap_ChangeSparkleColor(struct Actor *actor) {
  if (Random16() < 16384)
    Actor_SetSpriteColorMode(actor, Random_Below(8));
  return 0;
}

extern s32 gWorldMap_AquaRockSparkleScript[];

/// The hidden leader's update as the party is carried to Aqua Rock: every
/// eighth frame it leaves a sparkle where it is, which lasts about two seconds.
void WorldMap_SpawnAquaRockSparkle(struct Actor *parent) {
  if ((gFrameCount & 7) == 0) {
    FIXED x = parent->x.fixed;
    FIXED y = parent->y.fixed;
    FIXED z = parent->z.fixed;
    struct Actor *actor = (FieldActor_Create(SPRITE_SPARKLE, x, y, z));

    if (actor != NULL) {
      Actor_StartScript(actor, gWorldMap_AquaRockSparkleScript);
      Actor_SelectAnimation(actor, 1);
      Actor_SetSpriteFlags(actor, 0);
      actor->motionFlags = 0;
      actor->scaleX = FIXED_ONE * 2;
      actor->scaleY = FIXED_ONE * 2;
    }
  }
}

/// The party, hidden in a trail of sparkles, is carried across the sea to
/// Aqua Rock.
void WorldMap_CarryPartyToAquaRock(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineWork.leaderActorId),
                            SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.leaderActorId), 0);
  Actor_Get(gMainRoutineWork.leaderActorId)->update =
      WorldMap_SpawnAquaRockSparkle;
  Actor_Get(gMainRoutineWork.leaderActorId)->acceleration = toFIXED(1) / 8;
  Actor_Get(gMainRoutineWork.leaderActorId)->speedLimit = toFIXED(2);
  System_Wait(5);
  FieldEffect_ScheduleRatioTransition(FIXED_ONE * 3 / 4, 60);
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
  FieldEffect_ScheduleRatioTransition(FIXED_ONE * 5 / 4, 120);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 14275, 8736);
  Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 14257, 8720);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
  FieldEvent_SetExitRequest(WORLD_MAP_TO_AQUA_ROCK_MOAI_NORTH);
}

// The same routine as WorldMap_SetIslandTile, under another name.
/// Turns a placed actor's sprite with the World Map's view and shows it.
s32 WorldMap_SetPlacedActorTile(struct Actor *actor) {
  u16 *yaw = &((struct WorldMapView *)Ram_HeapSlots->mapGrid)->yaw;
  struct FieldSprite *sprite = actor->sprite;

  sprite->rotation = *yaw;
  sprite->flags = 0;
  return 1;
}

#include "shared/world_map_objects.c"

/// The ship's flicker and its sparks while it powers up (flight mode 1).
void WorldMap_FlashPoweringUp(struct Actor *actor) {
  if ((gFrameCount & 2) != 0) {
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_WHITE);
  } else {
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
  }
  if ((gFrameCount & 15) == 0) {
    WorldMap_SpawnSparkPair(actor);
  }
}

/// The step through gWorldMap_GlowColorModes, every other frame.
extern s32 gWorldMap_GlowColorStep;

/// The ship's glow and its trail in flight (flight mode 2): the colour cycles
/// every other frame, and a particle every fourth.
void WorldMap_GlowInFlight(struct Actor *actor) {
  if (gFrameCount & 1) {
    Actor_SetSpriteColorMode(actor,
                             gWorldMap_GlowColorModes[gWorldMap_GlowColorStep]);
    gWorldMap_GlowColorStep = (gWorldMap_GlowColorStep + 1) & 3;
  }
  if (!(gFrameCount & 3)) {
    WorldMap_SpawnParticle(actor);
  }
}

/// Sets the ship's look by flight mode: 0 plain, 1 powering up, 2 in flight.
void Actor_SetFlightMode(struct Actor *actor, s32 mode) {
  if (mode == 0) {
    actor->update = 0;
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
    Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  } else if (mode == 1) {
    actor->update = (ObjectUpdate)WorldMap_FlashPoweringUp;
  } else {
    actor->update = (ObjectUpdate)WorldMap_GlowInFlight;
    Event_PlaySound(SE_TAKE_FLIGHT);
  }
}

#include "shared/leader_search.h"

/// Brings the leader's ship down out of flight.
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

static inline void Camera_Move(FIXED x, FIXED y, FIXED z, s32 mode) {
  FieldCamera_MoveToFX(x, y, z, mode);
}

/// Shakes the camera diagonally by one pixel every three frames.
#include "shared/camera_jitter.h"

/// The view of a lighthouse lit, seen from the World Map: mode 1 for Venus
/// Lighthouse, 2 for Jupiter's beacon, which also rumbles and shakes the
/// ground.
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
  buffer = Heap_AllocEwram(16384);
  Decompress_TaggedLz(gWorldMapPackedTiles, buffer);
  Decompress_TaggedLz(gWorldMapPackedPictures,
                      buffer + PICTURE_FIRST_TILE * TILE_SIZE_4BPP);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
                      (u32)(gWorldMapPalettes + mode * PALETTE_COLORS * 2),
                      DMA_COPY_HALFWORDS(PALETTE_COLORS));
  IO_WRITE_QUEUE_PUSH(PICTURE_TILES, (u32)buffer,
                      DMA_COPY_WORDS(PICTURE_FIRST_TILE * TILE_SIZE_4BPP / 4));
  Task_AddFrame(WorldMap_UpdateTransitionBlend, TASK_PASS_FRAME);
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

/// The tidal wave picture's tiles, loaded after the picture layer's own.
#define WAVE_TILES 256

/// Loads the tidal wave's tiles, pictures and palette and sets its
/// background up.
void WorldMap_LoadTidalWave(void) {
  u8 *buffer = Heap_AllocEwram(16384);
  u8 *frames;
  u32 x;
  s32 y, i, scrollX;
  u16 scrollY;
  s32 mode;

  Decompress_TaggedLz(gWorldMapPackedTilesB, buffer);
  frames = buffer + 8192;
  Decompress_TaggedLz(gWorldMapPackedPicturesB, frames);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT_COLOR(PICTURE_PALETTE, 0),
                      (u32)gWorldMapPalettesB,
                      DMA_COPY_HALFWORDS(PALETTE_COLORS));
  IO_WRITE_QUEUE_PUSH(VRAM, (u32)buffer,
                      DMA_COPY_WORDS(PICTURE_FIRST_TILE * TILE_SIZE_4BPP / 4));
  IO_WRITE_QUEUE_PUSH(PICTURE_TILES, (u32)frames,
                      DMA_COPY_WORDS(WAVE_TILES * TILE_SIZE_4BPP / 4));
  Scene_QueueIoWrite16((u32)&REG_BLDCNT,
                       BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  Scene_QueueIoWrite16((u32)&REG_BLDALPHA,
                       BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX));
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 352;
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG0CNT, BGCNT_SIZE(3) | BGCNT_SCREEN_BASE(2),
                      IO_WRITE_HALFWORD);
  y = 11616;
  x = 561;
  for (i = 0; i < 96; i++) {
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

/// The actor flashes white, with a pair of sparks every sixteenth frame.
void WorldMap_FlashWithSparks(struct Actor *actor) {
  if ((gFrameCount & 2) != 0) {
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_WHITE);
  } else {
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
  }
  if ((gFrameCount & 15) == 0) {
    WorldMap_SpawnTwinSparks(actor);
  }
}

/// Animates an actor, running WorldMap_SpawnTwinSparks every sixteenth frame.
void Actor_AnimateAndEmit(struct Actor *actor) {
  u32 frame = gFrameCount;

  if (frame & 1)
    Actor_SetSpriteColorMode(actor, (frame >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    WorldMap_SpawnTwinSparks(actor);
}

#include "shared/actor_animate.h"

#include "shared/actor_grow_up.h"

#include "shared/actor_grow_down.h"

/// Two golden rings rise from the actor and grow away.
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
        sprite->flags = 0;
        VramCache_FreeEntry(sprite->slot);
        sprite->slot = work->vramBlock;
        ((struct SpriteCacheBits *)&sprite->cacheBits)->retainCacheEntry = 1;
        sprite->tile =
            (gVramCacheEntries[sprite->slot].offset >> 5) & OBJ_TILE_MASK;
        sprite->fullColor = 0;
        sprite->shape = 1;
        ((struct WorldMapOam *)sprite)->size = 2;
        ((struct SpriteAnimation *)sprite->record)->picture = 0;
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

#include "shared/world_map_gauge.c"

// clang-format off
u32 gWorldMap_LohoWorkerLeaveScriptA[] = {
    SC_ANIMATION(2),
    SC_MOVE_XZ(8961, 5837),
    SC_SYNC,
    SC_POSITION_ORIGIN,
    SC_END,
};

u32 gWorldMap_LohoWorkerLeaveScriptB[] = {
    SC_ANIMATION(2),
    SC_MOVE_XZ(8945, 5853),
    SC_SYNC,
    SC_MOVE_XZ(8961, 5837),
    SC_SYNC,
    SC_POSITION_ORIGIN,
    SC_END,
};

// The same script as gWorldMap_LohoWorkerLeaveScriptB, under another name.
u32 gWorldMap_LohoWorkerLeaveScriptC[] = {
    SC_ANIMATION(2),
    SC_MOVE_XZ(8945, 5853),
    SC_SYNC,
    SC_MOVE_XZ(8961, 5837),
    SC_SYNC,
    SC_POSITION_ORIGIN,
    SC_END,
};

u32 gWorldMap_IdejimaDriftScript[] = {
    SC_MOVE_BY(24, 0, 0),
    SC_SYNC,
    SC_LOOP(10, SC_TOP),
    SC_SET(SW_TIMER, 1),
    SC_MOVE_BY(24, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(24, 0, 0),
    SC_SYNC,
    SC_END,
};

s32 gWorldMap_IdejimaWakeScript[] = {
    SC_ADD(SW_X, -toFIXED(0.1)),
    SC_ADD(SW_Z, toFIXED(0.1)),
    SC_ADD(SW_SCALE_X, FIXED_ONE * 7 / 128),
    SC_ADD(SW_SCALE_Y, -384),
    SC_WAIT(1),
    SC_LOOP(48, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

s32 gWorldMap_AquaRockSparkleScript[] = {
    SC_CALL(WorldMap_ChangeSparkleColor),
    SC_WAIT(1),
    SC_LOOP(120, SC_TOP),
    SC_RELEASE,
};

s32 gWorldMap_SparkScript[] = {
    SC_CALL((s32)WorldMap_RiseFromAnchor),
    SC_RELEASE,
};

#include "shared/world_map_particle_script.c"

/// The colour modes the ship's glow steps through, one every other frame.
u32 gWorldMap_GlowColorModes[] = {
    0, 9, 0, 6,
};
// clang-format on

#undef WORLD_MAP_TRY_ENCOUNTER
#undef TUTORIAL_SET_MESSAGE
#undef TUTORIAL_MESSAGE

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
    SCENE_EXIT(WORLD_MAP_TO_IDEJIMA_DRIFTING, WorldMap,
               WORLD_MAP_IDEJIMA_DRIFTING),
    SCENE_EXIT(WORLD_MAP_TO_IDEJIMA_NORTHEAST, Idejima,
               IDEJIMA_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_LEMURIAN_SHIP_TILLER_A, LemurianShipTiller,
               LEMURIAN_SHIP_TILLER_FROM_WORLD_MAP_C),
    SCENE_EXIT(WORLD_MAP_TO_IDEJIMA_NORTH_B, Idejima, 12),
    SCENE_EXIT(WORLD_MAP_TO_AQUA_ROCK_MOAI_NORTH, AquaRockMoai,
               AQUA_ROCK_MOAI_FROM_WORLD_MAP_NORTH),
    SCENE_EXIT(WORLD_MAP_TO_IDEJIMA_SOUTHWEST, Idejima,
               IDEJIMA_FROM_WORLD_MAP_SOUTHWEST),
    SCENE_EXIT_UNLESS(WORLD_MAP_TO_DAILA_SOUTH, DailaFlooded,
                      DAILA_FLOODED_FROM_WORLD_MAP_SOUTH, 0x873),
    SCENE_EXIT_UNLESS(WORLD_MAP_TO_DAILA_NORTH, DailaFlooded,
                      DAILA_FLOODED_FROM_WORLD_MAP_NORTH, 0x873),
    SCENE_EXIT(WORLD_MAP_TO_DAILA_SOUTH, Daila, DAILA_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_DAILA_NORTH, Daila, DAILA_FROM_WORLD_MAP_NORTH),
    SCENE_EXIT(WORLD_MAP_TO_KANDOREAN_TEMPLE_GATE, KandoreanTempleGate,
               KANDOREAN_TEMPLE_GATE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_SHRINE_OF_THE_SEA_GOD_ROPE, ShrineOfTheSeaGodRope,
               SHRINE_OF_THE_SEA_GOD_ROPE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST,
               DehkanPlateauFullMetalVestChest,
               DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_DEHKAN_PLATEAU_TIGHTROPES, DehkanPlateauTightropes,
               DEHKAN_PLATEAU_TIGHTROPES_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_LEMURIAN_SHIP_MAIN_DECK_NORTH, LemurianShipMainDeck,
               LEMURIAN_SHIP_MAIN_DECK_FROM_WORLD_MAP_NORTH),
    SCENE_EXIT(WORLD_MAP_TO_MADRA, Madra, MADRA_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_MADRA_DRAWBRIDGE_SOUTH, MadraDrawbridge,
               MADRA_DRAWBRIDGE_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_MADRA_DRAWBRIDGE_NORTH, MadraDrawbridge,
               MADRA_DRAWBRIDGE_FROM_WORLD_MAP_NORTH),
    SCENE_EXIT(WORLD_MAP_TO_OSENIA_CLIFFS_WEST, OseniaCliffs,
               OSENIA_CLIFFS_FROM_WORLD_MAP_WEST),
    SCENE_EXIT(WORLD_MAP_TO_OSENIA_CLIFFS_EAST, OseniaCliffs,
               OSENIA_CLIFFS_FROM_WORLD_MAP_EAST),
    SCENE_EXIT(WORLD_MAP_TO_MIKASALLA, Mikasalla, MIKASALLA_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_GAROH_WAY_UP, GarohWayUp,
               GAROH_WAY_UP_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_AIRS_ROCK_STORM_BRAND_CHEST,
               AirsRockStormBrandChest,
               AIRS_ROCK_STORM_BRAND_CHEST_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_YAMPI_DESERT_GUARDIAN_RING_CHEST,
               YampiDesertGuardianRingChest,
               YAMPI_DESERT_GUARDIAN_RING_CHEST_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST,
               YampiDesertTrainersWhipChest,
               YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE,
               YampiDesertHiddenPsynergyStone,
               YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_ALHAFRA, Alhafra, ALHAFRA_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_CLIFFS_A, GondowanCliffs,
               GONDOWAN_CLIFFS_FROM_WORLD_MAP_A),
    SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_CLIFFS_B, GondowanCliffs,
               GONDOWAN_CLIFFS_FROM_WORLD_MAP_B),
    SCENE_EXIT(WORLD_MAP_TO_NARIBWE_SOUTH, Naribwe,
               NARIBWE_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST,
               KibomboMountainsDiskAxeChest,
               KIBOMBO_MOUNTAINS_DISK_AXE_CHEST_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_KIBOMBO_MOUNTAINS_WAFT, KibomboMountainsWaft,
               KIBOMBO_MOUNTAINS_WAFT_FROM_WORLD_MAP),
    SCENE_EXIT_UNLESS(WORLD_MAP_TO_KIBOMBO, KibomboCeremony,
                      KIBOMBO_CEREMONY_FROM_WORLD_MAP, 0x8ff),
    SCENE_EXIT_UNLESS(WORLD_MAP_TO_KIBOMBO, Kibombo, KIBOMBO_FROM_WORLD_MAP,
                      EXIT_FLAG_NONE),
    SCENE_EXIT(WORLD_MAP_TO_WEST_INDRA_ISLET, WestIndraIslet,
               WEST_INDRA_ISLET_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_EAST_TUNDARIA_ISLET, EastTundariaIslet,
               EAST_TUNDARIA_ISLET_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_NORTH_OSENIA_ISLET_A, NorthOseniaIslet,
               NORTH_OSENIA_ISLET_FROM_WORLD_MAP_A),
    SCENE_EXIT(WORLD_MAP_TO_SOUTHEAST_ANGARA_ISLET, SoutheastAngaraIslet,
               SOUTHEAST_ANGARA_ISLET_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_SEA_OF_TIME_ISLET, SeaOfTimeIslet,
               SEA_OF_TIME_ISLET_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_YALLAM_SOUTH_A, Yallam,
               YALLAM_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_YALLAM_SOUTH_B, Yallam,
               YALLAM_FROM_WORLD_MAP_NORTH),
    SCENE_EXIT(WORLD_MAP_TO_TAOPO_SWAMP_ENTRANCE, TaopoSwampEntrance,
               TAOPO_SWAMP_ENTRANCE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_APOJII_ISLANDS, ApojiiIslands,
               APOJII_ISLANDS_FROM_WORLD_MAP_A),
    SCENE_EXIT(WORLD_MAP_TO_AQUA_ROCK_MOAI_SOUTH, AquaRockMoai,
               AQUA_ROCK_MOAI_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_IZUMO, Izumo, IZUMO_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_GAIA_ROCK_SUSA, GaiaRockSusa,
               GAIA_ROCK_SUSA_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_CHAMPA, Champa, CHAMPA_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_ANKOHL_RUINS_ENTRANCE, AnkohlRuinsEntrance,
               ANKOHL_RUINS_ENTRANCE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_SEA_OF_TIME_SWIRLS, SeaOfTimeSwirls,
               SEA_OF_TIME_SWIRLS_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_SEA_OF_TIME_TO_LEMURIA_NORTHEAST,
               SeaOfTimeToLemuria, SEA_OF_TIME_TO_LEMURIA_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_SEA_OF_TIME_TO_LEMURIA_NORTH, SeaOfTimeToLemuria,
               SEA_OF_TIME_TO_LEMURIA_FROM_WORLD_MAP_NORTH),
    SCENE_EXIT(WORLD_MAP_TO_LEMURIA_ENTRANCE, LemuriaEntrance,
               LEMURIA_ENTRANCE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_TUNDARIA_TOWER_ENTRANCE, TundariaTowerEntrance,
               TUNDARIA_TOWER_ENTRANCE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_SW_ATTEKA_ISLET, SwAttekaIslet,
               SW_ATTEKA_ISLET_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_SETTLEMENT, GondowanSettlement,
               GONDOWAN_SETTLEMENT_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_HESPERIA_SETTLEMENT, HesperiaSettlement,
               HESPERIA_SETTLEMENT_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_KALT_ISLAND, KaltIsland,
               KALT_ISLAND_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_MAGMA_ROCK_SLIDE, MagmaRockSlide,
               MAGMA_ROCK_SLIDE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_ATTEKA_INLET_EAST, AttekaInlet,
               ATTEKA_INLET_FROM_LEMURIAN_SHIP_CABINS),
    SCENE_EXIT(WORLD_MAP_TO_ATTEKA_INLET_GEODE_NORTH, AttekaInletGeode,
               ATTEKA_INLET_GEODE_FROM_WORLD_MAP_NORTH),
    SCENE_EXIT(WORLD_MAP_TO_CONTIGO, Contigo, CONTIGO_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_JUPITER_LIGHTHOUSE_ENTRANCE,
               JupiterLighthouseEntrance,
               JUPITER_LIGHTHOUSE_ENTRANCE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_SHAMAN_VILLAGE_CAVE_WEST, ShamanVillageCave,
               SHAMAN_VILLAGE_CAVE_FROM_WORLD_MAP_WEST),
    SCENE_EXIT(WORLD_MAP_TO_SHAMAN_VILLAGE_CAVE_EAST, ShamanVillageCave,
               SHAMAN_VILLAGE_CAVE_FROM_WORLD_MAP_EAST),
    SCENE_EXIT(WORLD_MAP_TO_SHAMAN_VILLAGE, ShamanVillage,
               SHAMAN_VILLAGE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_LOHO, Loho, LOHO_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_NORTHERN_REACHES_SOUTH, NorthernReaches,
               NORTHERN_REACHES_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_NORTHERN_REACHES_NORTH, NorthernReaches,
               NORTHERN_REACHES_FROM_WORLD_MAP_NORTH),
    SCENE_EXIT(WORLD_MAP_TO_PROX_SOUTH_GATE, ProxSouthGate,
               PROX_SOUTH_GATE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_PROX_MOLD, ProxMold, PROX_MOLD_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_CAVERNS_OSENIA, CavernsOsenia,
               CAVERNS_OSENIA_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_CAVERNS_INDRA, CavernsIndra,
               CAVERNS_INDRA_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_MARS_LIGHTHOUSE_OUTSIDE, MarsLighthouseOutside,
               MARS_LIGHTHOUSE_OUTSIDE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_CLIFFS_PASS_NORTH, GondowanCliffsPass,
               GONDOWAN_CLIFFS_PASS_FROM_WORLD_MAP_NORTH),
    SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_CLIFFS_PASS_SOUTH, GondowanCliffsPass,
               GONDOWAN_CLIFFS_PASS_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_TREASURE_ISLE_ENTRANCE, TreasureIsleEntrance,
               TREASURE_ISLE_ENTRANCE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_CAVERNS_ATTEKA, CavernsAtteka,
               CAVERNS_ATTEKA_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_CAVERNS_ANGARA, CavernsAngara,
               CAVERNS_ANGARA_FROM_WORLD_MAP),
    SCENE_EXITS_END,
};

LATE_DATA s32 gWorldMap_TileSpriteScript[] = {
    SC_CALL(TileSprite_Reset),
    SC_END,
};

LATE_DATA s32 gWorldMap_EchoScript[] = {
    SC_CALL(WorldMap_SetUpEcho),
    SC_END,
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
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_PURPLE_ROBE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_LONG_DARK_HAIR,
                    IF_FLAG_SET(FLAG_BANDIT_ON_WORLD_MAP), MOVEMENT_TYPE_STAND,
                    0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE,
                    IF_FLAG_SET(FLAG_BANDIT_ON_WORLD_MAP), MOVEMENT_TYPE_STAND,
                    0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE,
                    IF_FLAG_SET(FLAG_BANDIT_ON_WORLD_MAP), MOVEMENT_TYPE_STAND,
                    0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
                    0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gWorldMap_CollapsePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1968,
                    0, 7488, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1936,
                    0, 7472, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1904, 0,
                    7488, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS,
                    (s32)gWorldMap_TileSpriteScript, 1946, 0, 7557, FACING_EAST,
                    TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA s32 gWorldMap_IslandScript[] = {
    SC_CALL(WorldMap_SetIslandTile),
    SC_END,
};

LATE_DATA s32 gWorldMap_TidalWaveScript[] = {
    SC_CALL(WorldMap_SetUpWave),
    SC_CALL(WorldMap_SendWaveIn),
    SC_END,
};

LATE_DATA struct ActorPlacement gWorldMap_DriftPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11000,
                    0, 7500, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10000,
                    0, 7500, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10000,
                    0, 8000, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11000,
                    0, 8000, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_ISLAND, CONDITION_ALWAYS,
                    (s32)gWorldMap_IslandScript, 10207, 0, 7900, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
                    (s32)gWorldMap_TidalWaveScript, 10524, 0, 7450,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
                    (s32)gWorldMap_TidalWaveScript, 10532, 0, 7480,
                    FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
                    (s32)gWorldMap_TidalWaveScript, 10540, 0, 7510,
                    FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
                    (s32)gWorldMap_TidalWaveScript, 10548, 0, 7540,
                    FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
                    (s32)gWorldMap_TidalWaveScript, 10556, 0, 7570,
                    FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
                    (s32)gWorldMap_TidalWaveScript, 10564, 0, 7600,
                    FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
                    (s32)gWorldMap_TidalWaveScript, 10572, 0, 7630,
                    FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_WATER_SPRAY, CONDITION_ALWAYS,
                    (s32)gWorldMap_TidalWaveScript, 10580, 0, 7660,
                    FACING_SOUTH, TALK_FACING_KEEP, 1),
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
    {TRIGGER_RAISED, WORLD_MAP_RAISED_BOARD_SHIP, CONDITION_ALWAYS,
     EVENT(WorldMap_BoardShip)},
    {TRIGGER_RAISED, WORLD_MAP_RAISED_LEAVE_SHIP, CONDITION_ALWAYS,
     EVENT(WorldMap_LeaveShip)},
    {TRIGGER_RAISED, WORLD_MAP_RAISED_TAKE_OFF, CONDITION_ALWAYS,
     EVENT(WorldMap_SetUpFlyingShip)},
    {TRIGGER_RAISED, WORLD_MAP_RAISED_LAND, CONDITION_ALWAYS,
     EVENT(WorldMap_SetUpShip)},
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
    {TRIGGER_STEP, STEP_TRIGGER_AREA(PLACED_DAILA), CONDITION_ALWAYS,
     EVENT(WorldMap_EnterDaila)},
    // Walking into a place's marker (its slot among gWorldMap_PlacedActors)
    // leaves by the exit to that place.
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(1), CONDITION_ALWAYS,
     WORLD_MAP_TO_KANDOREAN_TEMPLE_GATE},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(2), CONDITION_ALWAYS,
     WORLD_MAP_TO_SHRINE_OF_THE_SEA_GOD_ROPE},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(3), CONDITION_ALWAYS,
     WORLD_MAP_TO_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(4), CONDITION_ALWAYS,
     WORLD_MAP_TO_DEHKAN_PLATEAU_TIGHTROPES},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(5), FLAG_SHIP_CAN_SAIL,
     WORLD_MAP_TO_LEMURIAN_SHIP_MAIN_DECK_NORTH},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(6), CONDITION_ALWAYS, WORLD_MAP_TO_MADRA},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(7), CONDITION_ALWAYS,
     WORLD_MAP_TO_OSENIA_CLIFFS_WEST},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(8), CONDITION_ALWAYS,
     WORLD_MAP_TO_OSENIA_CLIFFS_EAST},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(9), CONDITION_ALWAYS,
     WORLD_MAP_TO_MIKASALLA},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(10), CONDITION_ALWAYS,
     WORLD_MAP_TO_GAROH_WAY_UP},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(11), CONDITION_ALWAYS,
     WORLD_MAP_TO_AIRS_ROCK_STORM_BRAND_CHEST},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(12), CONDITION_ALWAYS,
     WORLD_MAP_TO_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(13), CONDITION_ALWAYS,
     WORLD_MAP_TO_ALHAFRA},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(14), CONDITION_ALWAYS,
     WORLD_MAP_TO_GONDOWAN_CLIFFS_A},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(15), CONDITION_ALWAYS,
     WORLD_MAP_TO_GONDOWAN_CLIFFS_B},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(16), CONDITION_ALWAYS,
     WORLD_MAP_TO_NARIBWE_SOUTH},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(17), CONDITION_ALWAYS,
     WORLD_MAP_TO_KIBOMBO_MOUNTAINS_DISK_AXE_CHEST},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(18), CONDITION_ALWAYS,
     WORLD_MAP_TO_KIBOMBO_MOUNTAINS_WAFT},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(19), CONDITION_ALWAYS,
     WORLD_MAP_TO_KIBOMBO},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(20), CONDITION_ALWAYS,
     WORLD_MAP_TO_WEST_INDRA_ISLET},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(21), CONDITION_ALWAYS,
     WORLD_MAP_TO_EAST_TUNDARIA_ISLET},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(22), CONDITION_ALWAYS,
     WORLD_MAP_TO_NORTH_OSENIA_ISLET_A},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(23), CONDITION_ALWAYS,
     WORLD_MAP_TO_SOUTHEAST_ANGARA_ISLET},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(24), CONDITION_ALWAYS,
     WORLD_MAP_TO_SEA_OF_TIME_ISLET},
    {TRIGGER_STEP, STEP_TRIGGER_AREA(PLACED_YALLAM), CONDITION_ALWAYS,
     EVENT(WorldMap_EnterYallam)},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(26), CONDITION_ALWAYS,
     WORLD_MAP_TO_TAOPO_SWAMP_ENTRANCE},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(27), CONDITION_ALWAYS,
     WORLD_MAP_TO_APOJII_ISLANDS},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(28), CONDITION_ALWAYS,
     WORLD_MAP_TO_AQUA_ROCK_MOAI_SOUTH},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(29), CONDITION_ALWAYS, WORLD_MAP_TO_IZUMO},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(30), CONDITION_ALWAYS,
     WORLD_MAP_TO_GAIA_ROCK_SUSA},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(31), CONDITION_ALWAYS,
     WORLD_MAP_TO_CHAMPA},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(32), CONDITION_ALWAYS,
     WORLD_MAP_TO_ANKOHL_RUINS_ENTRANCE},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(33), CONDITION_ALWAYS,
     WORLD_MAP_TO_SEA_OF_TIME_SWIRLS},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(34), CONDITION_ALWAYS,
     WORLD_MAP_TO_SEA_OF_TIME_TO_LEMURIA_NORTHEAST},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(36), CONDITION_ALWAYS,
     WORLD_MAP_TO_LEMURIA_ENTRANCE},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(37), CONDITION_ALWAYS,
     WORLD_MAP_TO_TUNDARIA_TOWER_ENTRANCE},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(38), CONDITION_ALWAYS,
     WORLD_MAP_TO_SW_ATTEKA_ISLET},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(39), CONDITION_ALWAYS,
     WORLD_MAP_TO_GONDOWAN_SETTLEMENT},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(40), CONDITION_ALWAYS,
     WORLD_MAP_TO_HESPERIA_SETTLEMENT},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(41), CONDITION_ALWAYS,
     WORLD_MAP_TO_KALT_ISLAND},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(42), CONDITION_ALWAYS,
     WORLD_MAP_TO_MAGMA_ROCK_SLIDE},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(43), CONDITION_ALWAYS,
     WORLD_MAP_TO_CONTIGO},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(44), CONDITION_ALWAYS,
     WORLD_MAP_TO_JUPITER_LIGHTHOUSE_ENTRANCE},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(45), CONDITION_ALWAYS,
     WORLD_MAP_TO_SHAMAN_VILLAGE_CAVE_WEST},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(46), CONDITION_ALWAYS,
     WORLD_MAP_TO_SHAMAN_VILLAGE_CAVE_EAST},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(47), CONDITION_ALWAYS,
     WORLD_MAP_TO_SHAMAN_VILLAGE},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(48), CONDITION_ALWAYS, WORLD_MAP_TO_LOHO},
    {TRIGGER_STEP, STEP_TRIGGER_AREA(PLACED_PROX), CONDITION_ALWAYS,
     EVENT(WorldMap_EnterProx)},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(51), CONDITION_ALWAYS,
     WORLD_MAP_TO_CAVERNS_OSENIA},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(52), CONDITION_ALWAYS,
     WORLD_MAP_TO_CAVERNS_INDRA},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(53), CONDITION_ALWAYS,
     WORLD_MAP_TO_MARS_LIGHTHOUSE_OUTSIDE},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(54), CONDITION_ALWAYS,
     WORLD_MAP_TO_MARS_LIGHTHOUSE_OUTSIDE},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(55), CONDITION_ALWAYS,
     WORLD_MAP_TO_TREASURE_ISLE_ENTRANCE},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(56), CONDITION_ALWAYS,
     WORLD_MAP_TO_CAVERNS_ATTEKA},
    {TRIGGER_EXIT, STEP_TRIGGER_AREA(57), CONDITION_ALWAYS,
     WORLD_MAP_TO_CAVERNS_ANGARA},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, WORLD_MAP_TO_DAILA_NORTH},
    {TRIGGER_EXIT, WORLD_MAP_TO_DEHKAN_PLATEAU_TIGHTROPES, CONDITION_ALWAYS,
     WORLD_MAP_TO_DEHKAN_PLATEAU_TIGHTROPES},
    {TRIGGER_EXIT, WORLD_MAP_TO_MADRA_DRAWBRIDGE_SOUTH, CONDITION_ALWAYS,
     WORLD_MAP_TO_MADRA_DRAWBRIDGE_SOUTH},
    {TRIGGER_EXIT, WORLD_MAP_TO_MADRA_DRAWBRIDGE_NORTH, CONDITION_ALWAYS,
     WORLD_MAP_TO_MADRA_DRAWBRIDGE_NORTH},
    {TRIGGER_EXIT, WORLD_MAP_TO_YAMPI_DESERT_GUARDIAN_RING_CHEST,
     CONDITION_ALWAYS, WORLD_MAP_TO_YAMPI_DESERT_GUARDIAN_RING_CHEST},
    {TRIGGER_EXIT, WORLD_MAP_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST,
     CONDITION_ALWAYS, WORLD_MAP_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST},
    {TRIGGER_EXIT, WORLD_MAP_TO_SEA_OF_TIME_SWIRLS, CONDITION_ALWAYS,
     WORLD_MAP_TO_SEA_OF_TIME_SWIRLS},
    {TRIGGER_EXIT, WORLD_MAP_TO_SEA_OF_TIME_TO_LEMURIA_NORTHEAST,
     CONDITION_ALWAYS, WORLD_MAP_TO_SEA_OF_TIME_TO_LEMURIA_NORTHEAST},
    {TRIGGER_EXIT, WORLD_MAP_TO_SEA_OF_TIME_TO_LEMURIA_NORTH, CONDITION_ALWAYS,
     WORLD_MAP_TO_SEA_OF_TIME_TO_LEMURIA_NORTH},
    {TRIGGER_EXIT, WORLD_MAP_TO_LEMURIA_ENTRANCE, CONDITION_ALWAYS,
     WORLD_MAP_TO_LEMURIA_ENTRANCE},
    {TRIGGER_STEP, CELL_BEHAVIOR_WORLD_MAP_ATTEKA_INLET_LANDING,
     CONDITION_ALWAYS, EVENT(WorldMap_MoorAtAttekaInlet)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WORLD_MAP_AWAY_FROM_CONTIGO, CONDITION_ALWAYS,
     EVENT(WorldMap_RefuseToRunAway)},
    {TRIGGER_STEP, 40, FLAG_SHIP_TOOK_OFF, EVENT(WorldMap_TakeOff)},
    {TRIGGER_EXIT, WORLD_MAP_TO_NORTHERN_REACHES_SOUTH, CONDITION_ALWAYS,
     WORLD_MAP_TO_NORTHERN_REACHES_SOUTH},
    {TRIGGER_EXIT, WORLD_MAP_TO_NORTHERN_REACHES_NORTH, CONDITION_ALWAYS,
     WORLD_MAP_TO_NORTHERN_REACHES_NORTH},
    {TRIGGER_EXIT, WORLD_MAP_TO_GONDOWAN_CLIFFS_PASS_NORTH, CONDITION_ALWAYS,
     WORLD_MAP_TO_GONDOWAN_CLIFFS_PASS_NORTH},
    {TRIGGER_EXIT, WORLD_MAP_TO_GONDOWAN_CLIFFS_PASS_SOUTH, CONDITION_ALWAYS,
     WORLD_MAP_TO_GONDOWAN_CLIFFS_PASS_SOUTH},
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
    SC_CALL(WorldMap_SetPlacedActorTile),
    SC_END,
    0,
};

LATE_DATA s32 gWorldMap_GlowColorStep = 0;

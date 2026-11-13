/// @file
/// The World Map (SceneId_WorldMap), the one perspective map (map/world)
/// that joins the areas, and the only scene of this overlay.
///
/// Its code does three kinds of work.
///
/// It lets the party into the areas. Each area stands on the map as an actor
/// drawn as a picture of its kind (SPRITE_WORLD_MAP_VILLAGE, _CAVE, _DOCK and
/// the rest), turned with the map (WorldMap_AlignToMap).
///
/// WorldMap_UpdateAreaContacts runs every frame. It hides the actors outside
/// a box around the view and raises the STEP_TRIGGER_AREA of the one the
/// leader touches. The trigger table turns that into the area's exit. A few
/// ways in are cells instead, each carrying the number of its exit.
///
/// Five areas have two ways in, and their events choose the exit by the side
/// the leader comes from. For the Bilibin Barricade that is the west or the
/// east. For Kolima Bridge, Kalay Docks, Gondowan Passage and Lalivero it is
/// the north or the south.
///
/// Kolima Bridge's actor shows the drawbridge let down once
/// FLAG_KOLIMA_BRIDGE_LOWERED is set.
///
/// It holds what the party meets on the map itself. Vine, Corona and Hail, a
/// Venus, a Mars and a Mercury Djinni, stand on the map. Talked to or walked
/// near, they fight once a random battle is nearly due.
///
/// A whirlwind spins near Crossbone Isle (ACTOR_STORM). It is put away while
/// the Tolbi-bound Ship lies at anchor off the island. Walked into, it lifts
/// the party for 180 frames, as the Suhalla Desert's storms do. Douse on it
/// brings the party down into a battle. When the time runs out it drops the
/// party outside Suhalla (WORLD_MAP_FROM_SUHALLA, FLAG_ARRIVE_FALLING).
///
/// The Turtle Boots are buried in the ground
/// (CELL_BEHAVIOR_WORLD_MAP_TURTLE_BOOTS).
///
/// And it plays the story's moments on the map, each run by WorldMap_Initialize
/// for the entrance another scene sends the party to. In story order:
/// - Leaving Vale (WORLD_MAP_FROM_VALE_PLAZA) sets FLAG_VALE_PARTY_LEFT the
///   first time.
/// - Flint, the first Djinni, circles his spot until the party walks up to him
///   (WorldMap_MeetFlint). He joins and teaches how Djinn are set and put on
///   standby, and a tutorial battle follows.
/// - Hammet's caravan. It plays the first time the party touches Vault
///   (WORLD_MAP_HAMMETS_CARAVAN_LEAVES). The caravan, cut off from Kalay by the
///   destroyed bridge, turns north for Lunpa as the ground shakes and a rock
///   falls beside it. Then the party goes on into Vault. It sets
///   FLAG_LUNPA_GUARDS_SUSPECT_KALAY.
/// - Mercury Lighthouse lit (WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT). The
///   lighthouse transition plays (WorldMap_PlayLighthouseTransition), then
///   Mercury Lighthouse Aerie follows
///   (MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT).
/// - Bunza's wagon. Out of West Lunpa Cave (WORLD_MAP_FROM_WEST_LUNPA_CAVE)
///   Bunza, Hammet and the leader board it and it rolls off
///   (FLAG_BUNZAS_WAGON_LEFT). At the Kalay Tunnel Entrance
///   (WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH) it stops and the leader steps
///   down.
/// - The Tolbi-bound Ship's crossing of the Karagol Sea. Between the ship's
///   scenes (WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_FIRST_ATTACK to
///   _SAIL_TO_LAND) the map shows the ship sailing west and sends the party
///   back aboard (FLAG_TOLBI_SHIP_ROWING). A rowing off course takes it north
///   to anchor off the island (WORLD_MAP_FROM_SHIP_ROOMS,
///   FLAG_TOLBI_SHIP_AT_ISLAND). There
///   CELL_BEHAVIOR_WORLD_MAP_CROSSBONE_ISLE leads ashore and
///   CELL_BEHAVIOR_WORLD_MAP_TOLBI_SHIP back aboard.
///   WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI ends the crossing at Tolbi
///   (FLAG_TOLBI_SHIP_REACHED_TOLBI).
/// - The Black Orb. Out of Gateway Cave (WORLD_MAP_FROM_GATEWAY_CAVE) Iodem
///   sees Babi's ship scuttled off the coast and gives Isaac the orb to use on
///   it (FLAG_WORLD_MAP_BLACK_ORB_HANDED_OVER, FLAG_SAW_SHIP_AT_IDEJIMA). He
///   calls him back if he walks into the cave again. Used on the wreck
///   (CELL_BEHAVIOR_WORLD_MAP_SHIPWRECK) the orb does nothing. Iodem takes it
///   back and the party must go on to Babi Lighthouse
///   (FLAG_BLACK_ORB_RETURNED). A defeat meanwhile returns to
///   WORLD_MAP_AFTER_BLACK_ORB_DEFEAT, which discards the orb and leads on by
///   WORLD_MAP_TO_ROCK_ROOMS.
/// - Venus Lighthouse lit (WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT). The
///   lighthouse transition plays, then the party is back at the Lighthouse
///   Aerie (VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT). After the look back
///   (WORLD_MAP_BEFORE_SHEBAS_FALL) the lighthouse shakes in clouds of dust
///   before Sheba's fall (WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE).
/// - The ending. Once the cleared game is saved (scene/file_screen.c,
///   WORLD_MAP_FROM_FILE_SCREEN), the island Jenna and Kraden are adrift on
///   drifts east. WORLD_MAP_JENNA_AND_KRADEN_ADRIFT then shows Alex leading
///   them to Felix and Sheba before the Title returns.
///
/// WORLD_MAP_FROM_DEBUG, the debug menus' entrance, turns encounters
/// off.

#include "script_operands.h"
#include "main_routine_work.h"
#include "heap.h"
#include "vram_cache.h"
#include "module/import.h"
#include "module/scene/world_map.h"
#include "module/scene/shared/world_map_attached_effects.h"
#include "module/field/event_commands.h"
#include "module/game/game.h"
#include "module/window/window.h"
#include "module/window/screens.h"
#include "module/system/system.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "heap_slots.h"
#include "event.h"
#include "djinn_ids.h"
#include "world_map_render_work.h"
#include "event_inline.h"
#include "iwram_call.h"
#include "text/msg_ids.h"
#include "inventory.h"
#include "field_effect.h"
#include "io_write_queue.h"
#include "ability_ids.h"
#include "field_psynergy_work.h"
#include "flags.h"
#include "sound_driver_symbols.h"
#include "item_ids.h"
#include "shared/walk_into_leader.h"

/// The falling rock of Hammet's caravan event has landed and shattered.
#define FLAG_WORLD_MAP_FALLING_ROCK_LANDED FLAG_SCENE_TEMP_00
/// Iodem has handed Isaac the Black Orb and waits for it to be used on the
/// wreck off Gateway Cave.
#define FLAG_WORLD_MAP_BLACK_ORB_HANDED_OVER FLAG_SCENE_TEMP_34
/// The scene's ordinary actor placements (gWorldMapActorPlacements)
/// are in use rather than an event's.
#define FLAG_WORLD_MAP_ORDINARY_ACTORS FLAG_SCENE_TEMP_35
/// The storm holds the party up in the air, where Douse can bring it down.
#define FLAG_WORLD_MAP_CAUGHT_IN_STORM FLAG_SCENE_TEMP_F0
/// The storm is done with, or put away while the Tolbi-bound Ship lies off the
/// island, and catches no one.
#define FLAG_WORLD_MAP_STORM_GONE FLAG_SCENE_TEMP_F1
/// The first flag of a byte counting the frames the storm keeps the party in
/// the air. The byte is read and written with FieldGameFlag_GetByte and
/// FieldGameFlag_SetByte.
#define FLAG_WORLD_MAP_STORM_FRAMES_LEFT FLAG_SCENE_TEMP_F8

static s32 WorldMap_AlignToMap(struct Actor *actor);
static void WorldMap_BoardBunzasWagon(void);
static void WorldMap_ShowHammetsCaravan(void);
static void WorldMap_ShakeCameraOverCaravan(void);
static void WorldMap_ShakeCameraWhereRockFalls(void);
static void WorldMap_UpdateFallingRock(void);
static void WorldMap_SailToFirstMonsterAttack(void);
static void WorldMap_SailToSecondMonsterAttack(void);
static void WorldMap_SailToThirdMonsterAttack(void);
static void WorldMap_SailToKraken(void);
static void WorldMap_SailUntilLandSighted(void);
static void WorldMap_SailIntoTolbi(void);
static void WorldMap_StepDownFromBunzasWagon(void);
static void WorldMap_RunBlackOrbEvent(void);
static void WorldMap_BlockGatewayCaveExit(s32 actorId);
static void WorldMap_IodemCallsIsaacBack(void);
static void WorldMap_ShowEndingEvent(void);
static void WorldMap_ShakeVenusLighthouse(void);
static void WorldMap_UpdateLighthouseQuake(void);
static void WorldMap_SailTolbiShipToIsland(void);
static s32 WorldMap_GetRowingBalance(void);
static s32 WorldMap_GetSeatRowValue(u32 seat);
static void WorldMap_ShowIslandDrifting(void);
static void WorldMap_UpdateStormTimer(void);
static void
WorldMap_PlayLighthouseTransition(enum WorldMapLighthouse lighthouse);

/// The actors WorldMap_UpdateAreaContacts looks at, the placed ones
/// to the last slot, and the base of the cell behaviors a touch raises
/// (STEP_TRIGGER_AREA).
enum { CONTACT_LAST_ACTOR = 65, STEP_TRIGGER_AREA_BASE = 100 };

/// The World Map's own actors, numbered in the order of
/// gWorldMapActorPlacements from ACTOR_FIRST_PLACED.
///
/// First come the Djinn met on the map, then the areas, then the Tolbi-bound
/// Ship, the storm and Iodem.
///
/// Each area is named by the scene its exit leads into (gWorldMapExits).
/// Touching it raises its STEP_TRIGGER_AREA (WorldMap_UpdateAreaContacts).
enum WorldMapActorId {
  /// The Venus Djinni: Flint, circling his spot until the party meets him,
  /// and later Vine.
  ACTOR_VENUS_DJINNI = ACTOR_FIRST_PLACED,
  /// The Mars Djinni Corona.
  ACTOR_CORONA,
  /// The Mercury Djinni Hail.
  ACTOR_HAIL,
  /// Vale, whose exit leads to the town gate (SceneId_ValePlaza).
  ACTOR_TO_VALE_PLAZA,
  ACTOR_TO_VAULT,
  ACTOR_TO_ALTIN,
  ACTOR_TO_SUHALLA,
  ACTOR_TO_LUNPA,
  ACTOR_TO_KALAY_DOCKS,
  ACTOR_TO_TOLBI_DOCKS,
  ACTOR_TO_IMIL,
  ACTOR_TO_KOLIMA,
  ACTOR_TO_XIAN,
  ACTOR_TO_LAMA_TEMPLE,
  ACTOR_TO_FUCHIN_TEMPLE,
  ACTOR_TO_BILIBIN,
  ACTOR_TO_KALAY,
  ACTOR_TO_LALIVERO,
  ACTOR_TO_TOLBI,
  ACTOR_TO_MERCURY_LIGHTHOUSE,
  /// After two pictures no event answers: a mountain north of Vale
  /// (SPRITE_WORLD_MAP_MOUNTAIN) and Venus Lighthouse, which is entered by
  /// the cells around it.
  ACTOR_TO_BILIBIN_BARRICADE = ACTOR_TO_MERCURY_LIGHTHOUSE + 3,
  ACTOR_TO_ROAD_TO_BABI_LIGHTHOUSE,
  ACTOR_TO_GOMA_CAVE_ENTRANCE,
  /// Goma Cave's way out, where a wooden bridge crosses the river
  /// (SceneId_GomaCaveBridge).
  ACTOR_TO_GOMA_CAVE_BRIDGE,
  ACTOR_TO_ALTMILLER_CAVE,
  ACTOR_TO_GONDOWAN_CAVE_NORTH,
  ACTOR_TO_GONDOWAN_CAVE_SOUTH,
  /// The way into Mogall Forest where the Killer Ape waits
  /// (SceneId_MogallForestKillerApe).
  ACTOR_TO_MOGALL_FOREST_KILLER_APE,
  ACTOR_TO_GATEWAY_CAVE,
  ACTOR_TO_BILIBIN_CAVE_ENTRANCE,
  ACTOR_TO_BILIBIN_CAVE,
  ACTOR_TO_WEST_LUNPA_CAVE,
  ACTOR_TO_ALTIN_PEAK,
  ACTOR_TO_LAMAKAN_DESERT_CAVE,
  ACTOR_TO_KOLIMA_FOREST,
  /// The way into Mogall Forest by its broken sign (SceneId_MogallForestSign).
  ACTOR_TO_MOGALL_FOREST_SIGN,
  ACTOR_TO_KALAY_TUNNEL_ENTRANCE,
  /// After three signposts (SPRITE_SIGNPOST). Unused: they stand at
  /// the map's origin, out of the player's reach, and no code moves them.
  ACTOR_TO_KOLIMA_BRIDGE = ACTOR_TO_KALAY_TUNNEL_ENTRANCE + 4,
  ACTOR_TO_GONDOWAN_PASSAGE,
  /// The Tolbi-bound Ship at anchor off Crossbone Isle, after the wreck of
  /// Babi's ship (SPRITE_SHIPWRECK), which stands off Gateway Cave.
  ACTOR_TOLBI_SHIP = ACTOR_TO_GONDOWAN_PASSAGE + 2,
  /// The storm that lifts the party, as the Suhalla Desert's do.
  ACTOR_STORM,
  /// Placed by the coast once he has handed over the Black Orb.
  ACTOR_IODEM
};

/// The cell behavior raised by touching an area's actor.
#define STEP_TRIGGER_AREA(actorId) (STEP_TRIGGER_AREA_BASE + (actorId))

/// The World Map's own cell behaviors, which the map's cells carry.
///
/// Each exit cell carries the number of the exit it leads to, so the exit rows
/// of gWorldTriggers name their cell behavior by that exit.
enum WorldMapCellBehavior {
  CELL_BEHAVIOR_WORLD_MAP_TOLBI_SHIP =
      74, ///< the shore beside the anchored ship
  CELL_BEHAVIOR_WORLD_MAP_CROSSBONE_ISLE =
      75, ///< the island's shore, walked onto northward
  CELL_BEHAVIOR_WORLD_MAP_FLINT = 90,  ///< the ground around Flint
  CELL_BEHAVIOR_WORLD_MAP_CORONA = 91, ///< the ground around Corona
  CELL_BEHAVIOR_WORLD_MAP_VINE = 92,   ///< the ground around Vine
  CELL_BEHAVIOR_WORLD_MAP_HAIL = 93,   ///< the ground around Hail
  CELL_BEHAVIOR_WORLD_MAP_STORM = 95,  ///< the ground under the storm
  CELL_BEHAVIOR_WORLD_MAP_SHIPWRECK =
      96, ///< the scuttled ship off Gateway Cave
  CELL_BEHAVIOR_WORLD_MAP_TURTLE_BOOTS =
      100 ///< the ground where the Turtle Boots are buried
};

/// Raised by WorldMap_UpdateStormTimer when the party's time in the air runs
/// out.
enum WorldMapRaisedTrigger { RAISED_STORM_TIME_UP = 99 };

/// The rowers' balance (WorldMap_GetRowingBalance) that takes the Tolbi-bound
/// Ship off course to the island.
enum { ROWING_BALANCE_OFF_COURSE = 11 };

/// SPRITE_WORLD_MAP_BRIDGE's animation once Kolima Bridge's drawbridge is down.
enum { WORLD_MAP_BRIDGE_ANIMATION_LOWERED = 2 };

/// The falling rock's animations (blue_crystal.png). It tumbles as it falls,
/// then shatters where it lands.
enum { FALLING_ROCK_ANIMATION_TUMBLE = 2, FALLING_ROCK_ANIMATION_SHATTER = 3 };

/// The actors of Hammet's caravan leaving Vault
/// (gWorldMapActorPlacementsHammetsCaravan,
/// WORLD_MAP_HAMMETS_CARAVAN_LEAVES), numbered from ACTOR_FIRST_PLACED.
/// Garet's row takes his own actor id.
enum WorldMapCaravanActorId {
  /// Hammet, in his sprite, as ACTOR_HAMMET is out of West Lunpa Cave.
  ACTOR_CARAVAN_HAMMET = ACTOR_FIRST_PLACED,
  /// One of Hammet's men, bald and bare-chested. "Master Hammet! This is
  /// terrible! The bridge has been destroyed..."
  ACTOR_CARAVAN_MAN,
  /// The caravan's four wagons, in the sprite of Bunza's wagon.
  ACTOR_CARAVAN_FIRST_WAGON,
  /// A blank actor (SPRITE_BLANK) that the falling rock is held above as
  /// it drops (WorldMap_UpdateFallingRock). It grows from a tenth of its size
  /// as it moves (gCaravanRockGuideScript).
  ACTOR_INVISIBLE_ROCK_GUIDE = ACTOR_CARAVAN_FIRST_WAGON + 4,
  /// Drawn as the rocks in sol_sanctum_eruption.c. It lands by the caravan
  /// and shatters.
  ACTOR_FALLING_ROCK
};

/// The Tolbi-bound Ship, shown on its way across the Karagol Sea.
///
/// It is the first actor of gWorldMapActorPlacementsCrossingEnds and
/// gWorldMapActorPlacementsCrossing
/// (WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_FIRST_ATTACK to
/// WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI and
/// WORLD_MAP_FROM_SHIP_ROOMS).
enum { ACTOR_CROSSING_SHIP = ACTOR_FIRST_PLACED };

/// Bunza's wagon at Kalay
/// (gWorldMapActorPlacementsFromKalayTunnelEntranceSouth), after Kalay's
/// and the Kalay Tunnel Entrance's area actors.
enum { ACTOR_KALAY_WAGON = ACTOR_FIRST_PLACED + 2 };

/// The one actor of gWorldMapActorPlacementsFromClear, before the ending.
///
/// It is the island Jenna and Kraden are adrift on ("How long will this
/// island continue to drift?"). It drifts east trailing a wake
/// (WorldMap_SpawnIslandWake) until the map leads into their event.
enum { ACTOR_DRIFTING_ISLAND = ACTOR_FIRST_PLACED };

/// The ending's actors
/// (gWorldMapActorPlacementsJennaAndKradenAdrift), numbered from
/// ACTOR_FIRST_PLACED after the rows of Jenna and Sheba, which take their own
/// actor ids.
enum WorldMapEndingActorId {
  /// Kraden, adrift with Jenna.
  ACTOR_KRADEN = ACTOR_FIRST_PLACED,
  /// Alex, who comes to show them that Felix and Sheba are alive. "There is
  /// something I wish to show you."
  ACTOR_ALEX,
  /// Felix, in his sprite as at Mercury Lighthouse (SPRITE_FELIX,
  /// mercury_lighthouse_aerie.c), shown with Sheba as Jenna and Kraden call
  /// their names.
  ACTOR_FELIX
};

/// Animations the map plays by number beyond enum ActorAnimation, from
/// each sprite's animation table (graphics/sprite/<name>.tsv).
enum {
  ANIM_JENNA_LIE = 19, ///< lying on her back (jenna.tsv's animation list)
  ANIM_KRADEN_LIE = 5  ///< lying on his back (kraden.tsv)
};

// The World Map's scene flags: FLAG_WORLD_MAP_FALLING_ROCK_LANDED: the falling
// rock has landed, in Hammet's caravan event.
// FLAG_WORLD_MAP_BLACK_ORB_HANDED_OVER: Iodem has handed over the Black Orb.
// FLAG_WORLD_MAP_ORDINARY_ACTORS: the scene's ordinary actor
// placements are in use. FLAG_WORLD_MAP_CAUGHT_IN_STORM: the storm holds the
// party up in the air. FLAG_WORLD_MAP_STORM_GONE: the storm is done with, or
// put away. FLAG_WORLD_MAP_STORM_FRAMES_LEFT: a byte: frames left in the air.

/// Walking near Vine, or talking to him, can start a fight with him.
static void WorldMap_MeetVine(void) {
  struct FieldWork *work = gFieldWork;

  if (gMainRoutineWork.encounterSteps >=
      Math_Divide(work->encounterThreshold * 9, 10)) {
    if ((u32)Random_Next() < 32768) {
      Field_RequestDjinnBattle(DJINN_NO_HOP | ACTOR_VENUS_DJINNI, DJINNI_VINE);
      work->encounterBias = 0;
    } else {
      gMainRoutineWork.encounterSteps = work->encounterThreshold;
    }
  }
}

/// Walking near Corona, or talking to him, makes Corona fight once a battle
/// is nearly due.
static void WorldMap_MeetCorona(void) {
  struct MainRoutineWork *routine = &gMainRoutineWork;
  s32 *steps = &routine->encounterSteps;
  struct FieldWork *work = gFieldWork;

  if (*steps >= Math_Divide(work->encounterThreshold * 9, 10)) {
    Field_RequestDjinnBattle(DJINN_NO_HOP | ACTOR_CORONA, DJINNI_CORONA);
    work->encounterBias = 0;
  }
}

/// Walking near Hail, or talking to him, makes Hail fight once a battle is
/// nearly due.
static void WorldMap_MeetHail(void) {
  struct MainRoutineWork *routine = &gMainRoutineWork;
  s32 *steps = &routine->encounterSteps;
  struct FieldWork *work = gFieldWork;

  if (*steps >= Math_Divide(work->encounterThreshold * 9, 10)) {
    Field_RequestDjinnBattle(DJINN_NO_HOP | ACTOR_HAIL, DJINNI_HAIL);
    work->encounterBias = 0;
  }
}

/// The update routine of a sparkle around the Black Orb. It releases the
/// sparkle on its second frame.
static void WorldMap_ReleaseSparkAfterDelay(struct Actor *actor) {
  if (actor->timer <= 0) {
    actor->timer++;
  } else {
    Actor_Release(actor);
  }
}

/// The update routine of the Black Orb held up over the wreck. It swells and
/// shrinks every four frames and gives off a sparkle every other frame.
static void WorldMap_PulseOrbAndSpawnSparks(struct Actor *actor) {
  struct Actor *spark;
  s32 x;
  s32 y;
  s32 z;

  if (gFrameCount & 4) {
    actor->scaleX = 13 * Q16_ONE / 10;
    actor->scaleY = 13 * Q16_ONE / 10;
  } else {
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  if (!(gFrameCount & 2))
    return;
  x = actor->x.fixed;
  y = actor->y.fixed;
  z = actor->z.fixed;
  spark = FieldActor_Create(SPRITE_SPARKLE, x, y, z);
  Sound_Play(SE_GLITTER);
  if (spark == 0)
    return;
  spark->motionFlags = 0;
  spark->sprite->image.priority = 1;
  Actor_SetSpriteFlags(spark, 0);
  Actor_SelectAnimation(spark, 1);
  spark->timer = 0;
  spark->update = WorldMap_ReleaseSparkAfterDelay;
}

/// Flint's update routine until the party meets him (WorldMap_SetUpFlint).
static void WorldMap_CircleFlint(struct Actor *actor) {
  if (gFrameCount & 2)
    Actor_SetColorMode(actor, SPRITE_COLOR_RED_TINT);
  else
    Actor_SetColorMode(actor, SPRITE_COLOR_WHITE);
  if ((s16)actor->userValue == 0) {
    actor->x.fixed = px(5584);
    actor->y.fixed = Iwram_MulQ16(Trig_Sin(actor->timer << 3), px(4)) + px(16);
    actor->z.fixed = px(1328);
    Vector_AddPolarOffset(px(16), actor->timer, &actor->x);
    actor->facing = actor->timer + FACING_SOUTH;
    actor->timer += TRIG_TURN / 64;
  }
}

/// Flint's actor script (gWorldMapFlintScript). Until he is caught or his
/// lesson has begun (FLAG_DJINN_LESSON), he circles his spot at half size.
static s32 WorldMap_SetUpFlint(struct Actor *actor) {
  if (GameFlag_Get(FLAG_DJINNI(DJINNI_FLINT)) != 0) {
    return 0;
  }
  if (GameFlag_Get(FLAG_DJINN_LESSON) != 0) {
    return 0;
  }
  actor->update = WorldMap_CircleFlint;
  actor->motionFlags = 0;
  actor->timer = 0;
  actor->userValue = 0;
  actor->scaleX = Q16_ONE / 2;
  actor->scaleY = Q16_ONE / 2;
  return 0;
}

const struct SceneEntrance *WorldMap_GetEntrances(void) {
  return gWorldSceneEntrances;
}

const struct SceneExitRegion *WorldMap_GetRegions(void) { return NULL; }

const u32 *WorldMap_GetExits(void) { return gWorldMapExits; }

/// Kolima Bridge's actor script (gWorldMapKolimaBridgeScript). It turns with
/// the map and shows the drawbridge let down once FLAG_KOLIMA_BRIDGE_LOWERED is
/// set.
static s32 WorldMap_SetUpKolimaBridge(struct Actor *actor) {
  WorldMap_AlignToMap(actor);
  if (GameFlag_Get(FLAG_KOLIMA_BRIDGE_LOWERED) != 0) {
    Actor_SelectAnimation(actor, WORLD_MAP_BRIDGE_ANIMATION_LOWERED);
  }
  return 1;
}

/// The actor script of every picture on the map (gWorldMapAlignToMapScript).
/// It turns the sprite with the map's rotation.
static s32 WorldMap_AlignToMap(struct Actor *actor) {
  struct WorldMapRenderWork *work = gMapGridWork[0];
  struct Sprite *sprite = actor->sprite;

  sprite->rotation = work->rotation;
  sprite->flags = 0;
  return 1;
}

/// Iodem's actor script (gWorldMapIodemScript). He turns with the map and
/// blocks the way, so the leader cannot walk through him.
static s32 WorldMap_SetUpIodem(struct Actor *actor) {
  struct WorldMapRenderWork *work = gMapGridWork[0];
  struct Sprite *sprite = actor->sprite;

  actor->collisionFlags |= ACTOR_SOLID;
  sprite->rotation = work->rotation;
  return 1;
}

/// Sets up the storm (ACTOR_STORM) as it is placed.
static s32 WorldMap_SetUpStorm(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  Actor_SetColorMode(actor, SPRITE_COLOR_RED_TINT);
  actor->collisionFlags = 0;
  if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    GameFlag_Set(FLAG_WORLD_MAP_STORM_GONE);
    actor->x.fixed = 0;
    actor->y.fixed = 0;
  }
  return 0;
}

/// Called each frame by the storm's actor script (gWorldMapStormScript). It
/// hides the storm on odd frames, and WorldMap_UpdateAreaContacts shows it
/// again on the next, so it flickers.
static s32 WorldMap_BlinkActorSprite(struct Actor *actor) {
  if ((actor->spriteStorageKind & 1) && (gFrameCount & 1))
    actor->spriteStorageKind ^= 1;
  return 1;
}

/// Chooses the actors for the entrance.
const struct ActorPlacement *WorldMap_GetActorPlacements(void) {
  switch (gMainRoutineWork.entrance) {
  case WORLD_MAP_FROM_WEST_LUNPA_CAVE:
    if (GameFlag_Get(FLAG_BUNZAS_WAGON_LEFT) == 0 &&
        GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
      return gWorldMapActorPlacementsFromWestLunpaCave;
    }
    break;
  case WORLD_MAP_HAMMETS_CARAVAN_LEAVES:
    if (GameFlag_Get(FLAG_LUNPA_GUARDS_SUSPECT_KALAY) == 0) {
      return gWorldMapActorPlacementsHammetsCaravan;
    }
    break;
  case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_FIRST_ATTACK:
  case WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI:
    return gWorldMapActorPlacementsCrossingEnds;
  case WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH:
    return gWorldMapActorPlacementsFromKalayTunnelEntranceSouth;
  case WORLD_MAP_FROM_FILE_SCREEN:
    return gWorldMapActorPlacementsFromClear;
  case WORLD_MAP_BEFORE_SHEBAS_FALL:
    return gWorldMapActorPlacementsBeforeShebasFall;
  case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_SECOND_ATTACK:
  case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_THIRD_ATTACK:
  case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_KRAKEN:
  case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_LAND:
  case WORLD_MAP_FROM_SHIP_ROOMS:
    return gWorldMapActorPlacementsCrossing;
  case WORLD_MAP_JENNA_AND_KRADEN_ADRIFT:
    return gWorldMapActorPlacementsJennaAndKradenAdrift;
  default:
    break;
  }

  GameFlag_Set(FLAG_WORLD_MAP_ORDINARY_ACTORS);
  return gWorldMapActorPlacements;
}

/// Chooses a passage exit from the side of the actor the leader
/// approaches.
static void WorldMap_SelectExitByX(s32 trigger, s32 exitBefore, s32 exitAfter) {
  struct FieldWork *work;
  struct Actor *area;
  struct Actor *leader;

  area = Actor_Get(trigger - STEP_TRIGGER_AREA_BASE);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  work = gFieldWork;
  if (leader->x.fixed < area->x.fixed) {
    work->exit = exitBefore;
  } else {
    work->exit = exitAfter;
  }
  Sound_Play(SE_MAP_EXIT);
}

/// Chooses a passage exit from the leader's north-south approach.
static void WorldMap_SelectExitByZ(s32 trigger, s32 exitBefore, s32 exitAfter) {
  struct FieldWork *work;
  struct Actor *area;
  struct Actor *leader;

  area = Actor_Get(trigger - STEP_TRIGGER_AREA_BASE);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  work = gFieldWork;
  if (leader->z.fixed < area->z.fixed) {
    work->exit = exitBefore;
  } else {
    work->exit = exitAfter;
  }
  Sound_Play(SE_MAP_EXIT);
}

/// Touching the Bilibin Barricade's actor takes the party in by the side it
/// comes from.
static void WorldMap_EnterBilibinBarricade(void) {
  WorldMap_SelectExitByX(STEP_TRIGGER_AREA(ACTOR_TO_BILIBIN_BARRICADE),
                         WORLD_MAP_TO_BILIBIN_BARRICADE_WEST,
                         WORLD_MAP_TO_BILIBIN_BARRICADE_EAST);
}

/// The party enters Kolima Bridge from the World Map.
static void WorldMap_EnterKolimaBridge(void) {
  WorldMap_SelectExitByZ(STEP_TRIGGER_AREA(ACTOR_TO_KOLIMA_BRIDGE),
                         WORLD_MAP_TO_KOLIMA_BRIDGE_WEST,
                         WORLD_MAP_TO_KOLIMA_BRIDGE_EAST);
}

/// As WorldMap_EnterKolimaBridge, for Kalay Docks. From the north the exit is
/// WORLD_MAP_TO_KALAY_DOCKS_NORTH, and from the south
/// WORLD_MAP_TO_KALAY_DOCKS_EAST.
static void WorldMap_EnterKalayDocks(void) {
  WorldMap_SelectExitByZ(STEP_TRIGGER_AREA(ACTOR_TO_KALAY_DOCKS),
                         WORLD_MAP_TO_KALAY_DOCKS_NORTH,
                         WORLD_MAP_TO_KALAY_DOCKS_EAST);
}

/// As WorldMap_EnterKolimaBridge, for Gondowan Passage. From the north the
/// exit is WORLD_MAP_TO_GONDOWAN_PASSAGE_EAST, and from the south
/// WORLD_MAP_TO_GONDOWAN_PASSAGE_WEST.
static void WorldMap_EnterGondowanPassage(void) {
  WorldMap_SelectExitByZ(STEP_TRIGGER_AREA(ACTOR_TO_GONDOWAN_PASSAGE),
                         WORLD_MAP_TO_GONDOWAN_PASSAGE_EAST,
                         WORLD_MAP_TO_GONDOWAN_PASSAGE_WEST);
}

/// As WorldMap_EnterKolimaBridge, for Lalivero. From the north the exit is
/// WORLD_MAP_TO_LALIVERO_WEST, and from the south WORLD_MAP_TO_LALIVERO_EAST.
static void WorldMap_EnterLalivero(void) {
  WorldMap_SelectExitByZ(STEP_TRIGGER_AREA(ACTOR_TO_LALIVERO),
                         WORLD_MAP_TO_LALIVERO_WEST,
                         WORLD_MAP_TO_LALIVERO_EAST);
}

const struct Trigger *WorldMap_GetTriggers(void) { return gWorldTriggers; }

/// Run each frame on the map, where WorldMap_Initialize adds it.
static void WorldMap_UpdateAreaContacts(void) {
  struct Actor *leader;
  struct FieldWork *work;
  struct Actor *actor;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;
  s32 leaderReach;
  u32 i;

  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  leaderReach = leader->sprite->scale * leader->radius;
  work = gFieldWork;
  actor = work->camera;
  left = actor->x.fixed - px(160);
  right = actor->x.fixed + px(160);
  top = actor->z.fixed - px(300);
  bottom = actor->z.fixed + px(200);

  for (i = ACTOR_FIRST_PLACED; i <= CONTACT_LAST_ACTOR; i++) {
    s32 x;
    s32 z;
    s32 dx;
    s32 reach;
    s32 scale;

    actor = Actor_Lookup(i);
    if (actor == NULL) {
      continue;
    }
    x = actor->x.fixed;
    z = actor->z.fixed;
    if (x < left || x > right || z < top || z > bottom) {
      actor->spriteStorageKind = 0;
      continue;
    }
    actor->spriteStorageKind = 1;
    if (gDebugMode != 0 && GameFlag_Get(FLAG_DEBUG_STEPS_OFF) != 0) {
      continue;
    }
    scale = actor->sprite->scale;
    dx = actor->x.fixed - leader->x.fixed;
    if (dx < 0) {
      dx = leader->x.fixed - actor->x.fixed;
    }
    reach = leaderReach + actor->radius * scale;
    if (dx + (actor->z.fixed - leader->z.fixed < 0
                  ? leader->z.fixed - actor->z.fixed
                  : actor->z.fixed - leader->z.fixed) <
            reach &&
        GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
      work->stepTrigger = i + STEP_TRIGGER_AREA_BASE;
    }
  }
}

/// Touching Vault's actor takes the party into Vault.
static void WorldMap_EnterVault(void) {
  if (GameFlag_Get(FLAG_LUNPA_GUARDS_SUSPECT_KALAY) == 0) {
    Import_EventSetExitRequest(WORLD_MAP_TO_HAMMETS_CARAVAN_LEAVES);
  } else {
    Sound_Play(SE_MAP_EXIT);
    Import_EventSetExitRequest(WORLD_MAP_TO_VAULT_SOUTH);
  }
}

/// Walking east from Crossbone Isle's shore back aboard the Tolbi-bound Ship
/// (WORLD_MAP_TO_TOLBI_SHIP_DECK_CROSSBONE_ISLE, to
/// TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS).
static void WorldMap_ReturnToTolbiShip(void) {
  Event_Start();
  Import_EventSetExitRequest(WORLD_MAP_TO_TOLBI_SHIP_DECK_CROSSBONE_ISLE);
}

/// The scene's initialization, which runs the moment the entrance selects.
s32 WorldMap_Initialize(void) {
  struct MainRoutineWork *routine = &gMainRoutineWork;

  if (routine->entrance == WORLD_MAP_FROM_DEBUG) {
    GameFlag_Set(FLAG_NO_ENCOUNTERS);
    GameFlag_Set(FLAG_NO_ENCOUNTERS_IN_AREA);
    GameFlag_Set(FLAG_DEBUG_STEPS_OFF);
  }
  if (routine->entrance == WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT) {
    WorldMap_PlayLighthouseTransition(WORLD_MAP_LIGHTHOUSE_MERCURY);
    Event_SetDestination((s32)&SceneId_MercuryLighthouseAerie,
                         MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT);
  } else if (routine->entrance == WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT) {
    WorldMap_PlayLighthouseTransition(WORLD_MAP_LIGHTHOUSE_VENUS);
    Event_SetDestination((s32)&SceneId_VenusLighthouseAerie,
                         VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT);
  } else if (routine->entrance == WORLD_MAP_AFTER_BLACK_ORB_DEFEAT) {
    Event_Start();
    PartyInventory_Discard(ITEM_BLACK_ORB);
    Import_EventSetExitRequest(WORLD_MAP_TO_ROCK_ROOMS);
  } else {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_CENTRE_LINE, 0);
    gFieldWork->transitionFrames = 16;
    wait(1);
    FieldCamera_SetSpeed(8);
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    Task_AddFrame(WorldMap_UpdateAreaContacts, TASK_PASS_FRAME);
    if (GameFlag_Get(FLAG_LAMAKAN_MANTICORE_BEATEN) == 0) {
      Field_SetWorldMapWindowCellTile(128, 256, 176, 56);
    }
    // Each special entrance resumes its journey or story event.
    switch (routine->entrance) {
    case WORLD_MAP_FROM_VALE_PLAZA:
      if (Event_GetFlag(FLAG_VALE_PARTY_LEFT) == 0) {
        GameFlag_Set(FLAG_VALE_PARTY_LEFT);
        // XXX (no evidence): what this flag records; no code in either game
        // reads it.
        GameFlag_Set(0x85c);
      }
      break;
    case WORLD_MAP_FROM_GATEWAY_CAVE:
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
        if (GameFlag_Get(FLAG_BLACK_ORB_RETURNED) == 0 &&
            GameFlag_Get(FLAG_WORLD_MAP_BLACK_ORB_HANDED_OVER) != 0) {
          gWorldMapIodem = ACTOR_IODEM;
          Actor_SetPosition(ACTOR_IODEM, 6036, 3400);
          Actor_Get(gWorldMapIodem)->facing = FACING_SOUTH_SOUTHEAST;
          WorldMap_BlockGatewayCaveExit(gWorldMapIodem);
        }
      } else if (GameFlag_Get(FLAG_BLACK_ORB_RETURNED) == 0 &&
                 GameFlag_Get(FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN) == 0) {
        WorldMap_RunBlackOrbEvent();
      }
      break;
    case WORLD_MAP_FROM_WEST_LUNPA_CAVE:
      if (GameFlag_Get(FLAG_BUNZAS_WAGON_LEFT) == 0 &&
          GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
        WorldMap_BoardBunzasWagon();
      }
      break;
    case WORLD_MAP_HAMMETS_CARAVAN_LEAVES:
      if (GameFlag_Get(FLAG_LUNPA_GUARDS_SUSPECT_KALAY) == 0) {
        WorldMap_ShowHammetsCaravan();
      }
      break;
    case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_FIRST_ATTACK:
      WorldMap_SailToFirstMonsterAttack();
      break;
    case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_SECOND_ATTACK:
      WorldMap_SailToSecondMonsterAttack();
      break;
    case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_THIRD_ATTACK:
      WorldMap_SailToThirdMonsterAttack();
      break;
    case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_KRAKEN:
      WorldMap_SailToKraken();
      break;
    case WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_LAND:
      WorldMap_SailUntilLandSighted();
      break;
    case WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI:
      WorldMap_SailIntoTolbi();
      break;
    case WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH:
      WorldMap_StepDownFromBunzasWagon();
      break;
    case WORLD_MAP_FROM_FILE_SCREEN:
      WorldMap_ShowIslandDrifting();
      break;
    case WORLD_MAP_BEFORE_SHEBAS_FALL:
      WorldMap_ShakeVenusLighthouse();
      break;
    case WORLD_MAP_FROM_TOLBI_SHIP_DECK_CROSSBONE_ISLE:
    case WORLD_MAP_FROM_CROSSBONE_ISLE:
    case WORLD_MAP_FROM_SUHALLA_DESERT_SANDSTORM:
      GameFlag_Set(FLAG_WORLD_MAP_LEFT);
      if (FieldGameFlag_GetByte(FLAG_WORLD_MAP_STORM_FRAMES_LEFT) != 0) {
        gMainRoutineWork.movementMode = LEADER_MOVEMENT_AIRBORNE;
        Task_AddFrame(WorldMap_UpdateStormTimer, TASK_PASS_FRAME);
      }
      break;
    case WORLD_MAP_FROM_SHIP_ROOMS:
      WorldMap_SailTolbiShipToIsland();
      break;
    case WORLD_MAP_JENNA_AND_KRADEN_ADRIFT:
      WorldMap_ShowEndingEvent();
      break;
    default:
      Actor_Get(ACTOR_TOLBI_SHIP)->scaleX = 5 * Q16_ONE / 4;
      Actor_Get(ACTOR_TOLBI_SHIP)->scaleY = 5 * Q16_ONE / 4;
      break;
    }
  }
  return 0;
}

/// The actors placed at WORLD_MAP_FROM_WEST_LUNPA_CAVE
/// (gWorldMapActorPlacementsFromWestLunpaCave), numbered from
/// ACTOR_FIRST_PLACED.
///
/// Bunza's wagon comes first. Bunza and Hammet come after Lunpa's and West
/// Lunpa Cave's area actors.
enum {
  /// Bunza's wagon.
  ACTOR_WAGON = ACTOR_FIRST_PLACED,
  ACTOR_BUNZA = ACTOR_FIRST_PLACED + 3,
  ACTOR_HAMMET
};

/// Out of West Lunpa Cave by wagon, Bunza and Hammet nod to each other and
/// climb aboard with the leader, and the wagon rolls off.
static void WorldMap_BoardBunzasWagon(void) {
  struct Actor *actor;

  Event_Start();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  WorldMap_Freeze();
  GameFlag_Set(FLAG_BUNZAS_WAGON_LEFT);
  Actor_SetPosition(ACTOR_BUNZA, 5856, 1180);
  Actor_SetDestinationOffset(ACTOR_BUNZA, 24, 8);
  Actor_WaitForMoveById(ACTOR_BUNZA);
  Event_Wait(60);
  Actor_SetPosition(ACTOR_HAMMET, 5856, 1180);
  Actor_SetDestinationOffset(ACTOR_HAMMET, 12, 24);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(60);
  Actor_SetAnimation(ACTOR_BUNZA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_HAMMET, ANIM_NOD);
  Event_Wait(120);
  Actor_SetPosition(ACTOR_WAGON, 5880, 1208);
  Event_Wait(60);
  Actor_SetAnimation(ACTOR_HAMMET, ANIM_WALK);
  actor = Actor_Get(ACTOR_WAGON);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_HAMMET, actor->x.part.pixel,
                         actor->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_HAMMET);
  Actor_SetPosition(ACTOR_HAMMET, 0, 0);
  Event_Wait(60);
  Actor_SetAnimation(ACTOR_BUNZA, ANIM_WALK);
  actor = Actor_Get(ACTOR_WAGON);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_BUNZA, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_BUNZA);
  Actor_SetPosition(ACTOR_BUNZA, 0, 0);
  Event_Wait(60);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  actor = Actor_Get(ACTOR_WAGON);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_PARTY_LEADER, actor->x.part.pixel,
                         actor->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Event_Wait(60);
  Actor_SetSpeed(ACTOR_WAGON, 0.5);
  Actor_SetDestinationOffset(ACTOR_WAGON, 56, 8);
  Actor_WaitForMoveById(ACTOR_WAGON);
  Actor_SetDestinationOffset(ACTOR_WAGON, 40, 40);
  Actor_WaitForMoveById(ACTOR_WAGON);
  Actor_SetDestinationOffset(ACTOR_WAGON, 8, 88);
  Actor_WaitForMoveById(ACTOR_WAGON);
  Event_FinishScreenTransition();
  Import_EventSetExitRequest(WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE_SOUTH);
  Event_End();
}

/// Outside Vault, the first time the party comes to it, Hammet's caravan
/// leaves.
static void WorldMap_ShowHammetsCaravan(void) {
  struct Actor *actor;
  struct Actor *spriteActor;
  const s32 *action;

  actor = Actor_Get(ACTOR_FALLING_ROCK);
  Event_Start();
  WorldMap_StartZoom(5 * Q16_ONE / 4, 1);
  wait(4);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  WorldMap_Freeze();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.6);
  Actor_SetSpeed(ACTOR_GARET, 1.6);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 5884, 1576);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_SetPosition(ACTOR_CARAVAN_HAMMET, 5848, 1576);
  wait(1);
  Actor_SetSpriteColorById(ACTOR_CARAVAN_HAMMET, SPRITE_COLOR_HIDDEN);
  spriteActor = Actor_Get(ACTOR_CARAVAN_HAMMET);
  Actor_SetSpriteFlags(spriteActor, 0);
  Actor_SetSpeedAndAcceleration(ACTOR_CARAVAN_FIRST_WAGON, 1.6, 0.4);
  Actor_SetSpeedAndAcceleration(ACTOR_CARAVAN_FIRST_WAGON + 1, 1.6, 0.4);
  Actor_SetSpeedAndAcceleration(ACTOR_CARAVAN_FIRST_WAGON + 2, 1.6, 0.4);
  Actor_SetSpeedAndAcceleration(ACTOR_CARAVAN_FIRST_WAGON + 3, 1.6, 0.4);
  Sound_Play(SE_RUMBLE);
  Actor_SetScript(ACTOR_CARAVAN_FIRST_WAGON, gCaravanFirstWagonArrives);
  Event_Wait(20);
  Actor_SetScript(ACTOR_CARAVAN_FIRST_WAGON + 1, gCaravanSecondWagonArrives);
  Event_Wait(20);
  Actor_SetScript(ACTOR_CARAVAN_FIRST_WAGON + 2, gCaravanThirdWagonArrives);
  Event_Wait(20);
  Actor_RunScriptAndWait(ACTOR_CARAVAN_FIRST_WAGON + 3,
                         gCaravanFourthWagonArrives);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  spriteActor = Actor_Get(ACTOR_PARTY_LEADER);
  if (spriteActor != 0) {
    Actor_SetPositionFixed(ACTOR_GARET, spriteActor->x.fixed,
                           spriteActor->z.fixed);
  }
  Actor_WalkToAndWait(ACTOR_GARET, 5892, 1600);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetPosition(ACTOR_CARAVAN_HAMMET, 5848, 1592);
  wait(1);
  // Hammet will not stay, and the wagons head south for the bridge.
  Event_SetMessageId(MsgWorldMapWeCantStayAnotherMinute);
  // "We can't stay another minute in a dangerous place like this...."
  Event_ShowMessageThenWait(ACTOR_CARAVAN_HAMMET, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 10);
  Sound_Play(SE_RUMBLE);
  Actor_SetScript(ACTOR_CARAVAN_FIRST_WAGON, gCaravanFirstWagonDeparts);
  Event_Wait(20);
  Actor_SetScript(ACTOR_CARAVAN_FIRST_WAGON + 1, gCaravanSecondWagonDeparts);
  Event_Wait(20);
  Actor_SetScript(ACTOR_CARAVAN_FIRST_WAGON + 2, gCaravanThirdWagonDeparts);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 10);
  Actor_RunScriptAndWait(ACTOR_CARAVAN_FIRST_WAGON + 3,
                         gCaravanFourthWagonDeparts);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Event_Wait(20);
  WorldMap_Unfreeze();
  Actor_SetAnimation(ACTOR_CARAVAN_FIRST_WAGON, ANIM_STAND);
  Actor_SetAnimation(ACTOR_CARAVAN_FIRST_WAGON + 1, ANIM_STAND);
  Actor_SetAnimation(ACTOR_CARAVAN_FIRST_WAGON + 2, ANIM_STAND);
  Actor_SetAnimation(ACTOR_CARAVAN_FIRST_WAGON + 3, ANIM_STAND);
  FieldCamera_MoveTo(5640, CAMERA_KEEP, 1784, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  WorldMap_Freeze();
  Actor_SetPosition(ACTOR_CARAVAN_MAN, 5640, 1752);
  wait(1);
  Actor_SetSpeed(ACTOR_CARAVAN_MAN, 1.2);
  Actor_WalkToAndWait(ACTOR_CARAVAN_MAN, 5640, 1736);
  Actor_WalkToAndWait(ACTOR_CARAVAN_MAN, 5624, 1736);
  Actor_WalkToAndWait(ACTOR_CARAVAN_MAN, 5624, 1784);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_CARAVAN_MAN, 2);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_CARAVAN_MAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_CARAVAN_MAN, FACING_EAST, 20);
  Actor_StartRepeatedMotion(ACTOR_CARAVAN_MAN, 3);
  // "Master Hammet! This is terrible! The bridge has been..."
  Event_ShowMessageThenWait(ACTOR_CARAVAN_MAN, 0, 20);
  Actor_SetPosition(ACTOR_CARAVAN_HAMMET, 5656, 1784);
  wait(1);
  Actor_SetSpriteColorById(ACTOR_CARAVAN_HAMMET, SPRITE_COLOR_NORMAL);
  spriteActor = Actor_Get(ACTOR_CARAVAN_HAMMET);
  Actor_SetSpriteFlags(spriteActor, SPRITE_FLAG_SHADOW);
  Actor_SetSpeed(ACTOR_CARAVAN_HAMMET, 0.8);
  Actor_WalkToAndWait(ACTOR_CARAVAN_HAMMET, 5640, 1784);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_CARAVAN_HAMMET, 2);
  // "What...? The bridge?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET,
                                 0, 10);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_SOUTH_SOUTHEAST, 60);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_WEST, 10);
  Actor_SetAttachedEffect(ACTOR_CARAVAN_HAMMET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_CARAVAN_MAN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_SOUTH_SOUTHEAST, 40);
  Actor_SetAttachedEffect(ACTOR_CARAVAN_HAMMET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_StartRepeatedMotion(ACTOR_CARAVAN_HAMMET, 2);
  // "This is awful... How are we going to get to Kalay now?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET,
                                 0, 40);
  Actor_RunRepeatedMotion(ACTOR_CARAVAN_MAN, 1);
  Actor_FaceDirection(ACTOR_CARAVAN_MAN, FACING_EAST, 10);
  // "...Master Hammet, I'm worried about leaving Ivan behind..."
  Event_ShowMessageThenWait(ACTOR_CARAVAN_MAN, 0, 10);
  Actor_ShowEmote(ACTOR_CARAVAN_HAMMET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  // "As fond as I am of Ivan, it is his fault my rod was stolen..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET,
                                 0, 10);
  Actor_RunRepeatedMotion(ACTOR_CARAVAN_HAMMET, 1);
  Actor_SetAnimationAndWait(ACTOR_CARAVAN_HAMMET, ANIM_NOD);
  // "And his strange powers will aid him in search of the rod."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0,
                            10);
  Actor_ShowEmote(ACTOR_CARAVAN_MAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "Ivan has strange powers?"
  Event_ShowMessageThenWait(ACTOR_CARAVAN_MAN, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_CARAVAN_HAMMET, ANIM_NOD);
  WorldMap_Unfreeze();
  Sound_Play(SE_TREMOR);
  FieldCamera_SetSpeedAndAcceleration(4, 4);
  // The ground shakes, and a rock falls beside Hammet and his man.
  WorldMap_ShakeCameraOverCaravan();
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Actor_ShowEmote(ACTOR_CARAVAN_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_CARAVAN_MAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_CARAVAN_MAN, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_CARAVAN_MAN, FACING_NORTH_NORTHWEST, 0);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(5608, CAMERA_KEEP, 1736, 1);
  FieldCamera_WaitForMove();
  Actor_SetPosition(ACTOR_INVISIBLE_ROCK_GUIDE, 5544, 1704);
  wait(1);
  Actor_SetSpeed(ACTOR_INVISIBLE_ROCK_GUIDE, 0.3);
  Actor_SetScript(ACTOR_INVISIBLE_ROCK_GUIDE, gCaravanRockGuideScript);
  Event_Wait(160);
  // The falling rock drops from 64 pixels up, and bounces as it lands.
  actor->gravity = px(1) / 10;
  actor->bounce = Q16_ONE / 10;
  actor->scaleX = 3 * Q16_ONE / 2;
  actor->scaleY = 3 * Q16_ONE / 2;
  actor->timer = 0;
  actor->y.fixed = px(64);
  actor->sprite->rotation = TRIG_TURN * 15 / 16;
  Actor_SetSpriteFlags(actor, 0);
  Actor_SelectAnimation(actor, FALLING_ROCK_ANIMATION_TUMBLE);
  wait(1);
  spriteActor = Actor_Get(ACTOR_FALLING_ROCK);
  Actor_SetSpriteFlags(spriteActor, 0);
  Task_AddFrame(WorldMap_UpdateFallingRock, TASK_PASS_FRAME);
  do {
    wait(1);
  } while (actor->timer == 0);
  spriteActor = Actor_Get(ACTOR_FALLING_ROCK);
  Actor_SetSpriteFlags(spriteActor, 0);
  spriteActor = Actor_Get(ACTOR_INVISIBLE_ROCK_GUIDE);
  Actor_SetSpriteFlags(spriteActor, 0);
  Event_Wait(10);
  spriteActor = Actor_Get(ACTOR_CARAVAN_MAN);
  spriteActor->velocityY = px(6);
  spriteActor = Actor_Get(ACTOR_CARAVAN_HAMMET);
  spriteActor->velocityY = px(6);
  Sound_Play(SE_CRASH);
  FieldCamera_SetSpeedAndAcceleration(4, 4);
  WorldMap_ShakeCameraWhereRockFalls();
  WorldMap_ShakeCameraWhereRockFalls();
  Event_Wait(60);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(5640, CAMERA_KEEP, 1784, 1);
  FieldCamera_WaitForMove();
  WorldMap_Freeze();
  Actor_SetAttachedEffect(ACTOR_CARAVAN_MAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_CARAVAN_HAMMET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Task_Remove(WorldMap_UpdateFallingRock);
  wait(1);
  Actor_SetPosition(ACTOR_INVISIBLE_ROCK_GUIDE, 0, 0);
  Actor_SetPosition(ACTOR_FALLING_ROCK, 0, 0);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_WEST, 10);
  Actor_Jump(ACTOR_CARAVAN_HAMMET, 4, 40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET,
                                 0, 10);
  Actor_FaceDirection(ACTOR_CARAVAN_MAN, FACING_EAST, 10);
  Event_ShowMessageThenWait(ACTOR_CARAVAN_MAN, 0, 20);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_NORTH, 40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET,
                                 0, 20);
  Actor_Jump(ACTOR_CARAVAN_MAN, 4, 20);
  Event_ShowMessageThenWait(ACTOR_CARAVAN_MAN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_CARAVAN_HAMMET, 1);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_WEST, 10);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0,
                            10);
  Actor_SetAttachedEffect(ACTOR_CARAVAN_MAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  Actor_SetAnimationAndWait(ACTOR_CARAVAN_HAMMET, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_CARAVAN_MAN, 1);
  Actor_SetAnimationAndWait(ACTOR_CARAVAN_MAN, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_CARAVAN_HAMMET, 5656, 1784);
  Actor_SetPosition(ACTOR_CARAVAN_HAMMET, 0, 0);
  Actor_WalkToAndWait(ACTOR_CARAVAN_MAN, 5624, 1736);
  Actor_WalkToAndWait(ACTOR_CARAVAN_MAN, 5640, 1736);
  Actor_WalkToAndWait(ACTOR_CARAVAN_MAN, 5640, 1752);
  Actor_SetPosition(ACTOR_CARAVAN_MAN, 0, 0);
  Sound_Play(SE_RUMBLE);
  Actor_SetScript(ACTOR_CARAVAN_FIRST_WAGON, gCaravanFirstWagonReturns);
  Actor_SetScript(ACTOR_CARAVAN_FIRST_WAGON + 1, gCaravanSecondWagonReturns);
  Event_Wait(40);
  Actor_SetScript(ACTOR_CARAVAN_FIRST_WAGON + 2, gCaravanThirdWagonReturns);
  Event_Wait(40);
  Actor_RunScriptAndWait(ACTOR_CARAVAN_FIRST_WAGON + 3,
                         gCaravanFourthWagonReturns);
  WorldMap_Unfreeze();
  Actor_SetPosition(ACTOR_PARTY_LEADER, 5900, 1576);
  Actor_SetPosition(ACTOR_GARET, 5908, 1600);
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(5848, CAMERA_KEEP, 1608, 1);
  FieldCamera_WaitForMove();
  action = gCaravanWagonHeadsNorth;
  Actor_SetScript(ACTOR_CARAVAN_FIRST_WAGON, action);
  Event_Wait(20);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(5848, CAMERA_KEEP, 1544, 1);
  Actor_SetScript(ACTOR_CARAVAN_FIRST_WAGON + 1, action);
  Event_Wait(20);
  Actor_SetScript(ACTOR_CARAVAN_FIRST_WAGON + 2, action);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_SetScript(ACTOR_CARAVAN_FIRST_WAGON + 3, action);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_WaitForScriptEndById(ACTOR_CARAVAN_FIRST_WAGON + 3);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  FieldCamera_SetSpeed(4);
  FieldCamera_MoveTo(5880, CAMERA_KEEP, 1608, 1);
  FieldCamera_WaitForMove();
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 80);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Scene_WalkIntoLeader(ACTOR_GARET);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(5848, CAMERA_KEEP, 1608, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 5848, 1576);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_LUNPA_GUARDS_SUSPECT_KALAY);
  Import_EventSetExitRequest(WORLD_MAP_TO_VAULT_SOUTH);
  Event_End();
}

/// Moves to the next camera-shake point, in whole pixels, and holds it for
/// four frames.
static inline void WorldMap_MoveShakeCamera(s32 x, s32 z) {
  FieldCamera_MoveToFixed(px(x), CAMERA_KEEP, px(z), 1);
  wait(4);
}

/// Shakes the camera around Hammet and his man as the ground trembles.
static void WorldMap_ShakeCameraOverCaravan(void) {
  WorldMap_MoveShakeCamera(5644, 1784);
  WorldMap_MoveShakeCamera(5636, 1788);
  WorldMap_MoveShakeCamera(5644, 1780);
  WorldMap_MoveShakeCamera(5644, 1788);
  WorldMap_MoveShakeCamera(5636, 1780);
  WorldMap_MoveShakeCamera(5640, 1784);
  WorldMap_MoveShakeCamera(5642, 1784);
  WorldMap_MoveShakeCamera(5638, 1786);
  WorldMap_MoveShakeCamera(5642, 1782);
  WorldMap_MoveShakeCamera(5642, 1786);
  WorldMap_MoveShakeCamera(5638, 1782);
  WorldMap_MoveShakeCamera(5640, 1784);
}

/// Shakes the camera around the spot where the falling rock lands.
static void WorldMap_ShakeCameraWhereRockFalls(void) {
  WorldMap_MoveShakeCamera(5612, 1736);
  WorldMap_MoveShakeCamera(5604, 1740);
  WorldMap_MoveShakeCamera(5612, 1732);
  WorldMap_MoveShakeCamera(5612, 1740);
  WorldMap_MoveShakeCamera(5604, 1732);
  WorldMap_MoveShakeCamera(5608, 1736);
  WorldMap_MoveShakeCamera(5610, 1736);
  WorldMap_MoveShakeCamera(5606, 1738);
  WorldMap_MoveShakeCamera(5610, 1734);
  WorldMap_MoveShakeCamera(5610, 1738);
  WorldMap_MoveShakeCamera(5606, 1734);
  WorldMap_MoveShakeCamera(5608, 1736);
}

/// Keeps the falling rock above its guide as it drops, and lands it once.
static void WorldMap_UpdateFallingRock(void) {
  struct Actor *actor;
  struct Actor *guide;

  actor = Actor_Get(ACTOR_FALLING_ROCK);
  guide = Actor_Get(ACTOR_INVISIBLE_ROCK_GUIDE);
  actor->x.fixed = guide->x.fixed;
  actor->z.fixed = guide->z.fixed;
  if (actor->y.fixed < px(10)) {
    actor->y.fixed = px(10);
    if (Event_GetFlag(FLAG_WORLD_MAP_FALLING_ROCK_LANDED) == 0) {
      Sound_Play(SE_CRASH);
      Actor_SelectAnimation(actor, FALLING_ROCK_ANIMATION_SHATTER);
      GameFlag_Set(FLAG_WORLD_MAP_FALLING_ROCK_LANDED);
      actor->timer = 1;
    }
  }
}

/// A leg of the Tolbi-bound Ship's crossing, in which the ship sails out of
/// Kalay Docks.
static void WorldMap_SailToFirstMonsterAttack(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_CROSSING_SHIP);
  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  actor->scaleY = 5 * Q16_ONE / 4;
  actor->scaleX = 5 * Q16_ONE / 4;
  FieldCamera_FollowActor(ACTOR_CROSSING_SHIP, 1);
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, 0.4);
  Actor_WalkToAndWait(ACTOR_CROSSING_SHIP, 5288, 2328);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_TOLBI_SHIP_ROWING);
  Import_EventSetExitRequest(WORLD_MAP_TO_TOLBI_SHIP_MAST_FIRST_ATTACK);
  Event_End();
}

/// The middle legs of the crossing, with the ship seen from above.
static inline void WorldMap_SailCrossingLeg(s32 exit) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_CROSSING_SHIP);
  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_CROSSING_SHIP, 7944, 200);
  actor->scaleX = 5 * Q16_ONE / 4;
  actor->scaleY = 5 * Q16_ONE / 4;
  wait(1);
  FieldCamera_FollowActor(ACTOR_CROSSING_SHIP, 1);
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, 0.6);
  actor->timer = 0;
  Actor_SetScript(ACTOR_CROSSING_SHIP, gCrossingShipSailsWest);
  do {
    wait(1);
  } while (actor->timer == 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_TOLBI_SHIP_ROWING);
  Import_EventSetExitRequest(exit);
  Event_End();
}

/// A leg of the Tolbi-bound Ship's crossing. WORLD_MAP_TO_TOLBI_SHIP_MAST_
/// SECOND_ATTACK leads up to the lookout on the mast for the second monster
/// attack (TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK).
static void WorldMap_SailToSecondMonsterAttack(void) {
  WorldMap_SailCrossingLeg(WORLD_MAP_TO_TOLBI_SHIP_MAST_SECOND_ATTACK);
}

/// A leg of the Tolbi-bound Ship's crossing. WORLD_MAP_TO_TOLBI_SHIP_MAST_
/// THIRD_ATTACK leads up to the lookout on the mast for the third monster
/// attack (TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK).
static void WorldMap_SailToThirdMonsterAttack(void) {
  WorldMap_SailCrossingLeg(WORLD_MAP_TO_TOLBI_SHIP_MAST_THIRD_ATTACK);
}

/// A leg of the Tolbi-bound Ship's crossing. WORLD_MAP_TO_SHIP_ROOMS
/// leads back down to the rowing deck as the Kraken comes aboard
/// (SHIP_ROOMS_FROM_WORLD_MAP).
static void WorldMap_SailToKraken(void) {
  WorldMap_SailCrossingLeg(WORLD_MAP_TO_SHIP_ROOMS);
}

/// The last leg of the Tolbi-bound Ship's crossing, in which the ship sails
/// west until land is sighted.
static void WorldMap_SailUntilLandSighted(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_CROSSING_SHIP);
  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_CROSSING_SHIP, 7944, 200);
  actor->scaleX = 5 * Q16_ONE / 4;
  actor->scaleY = 5 * Q16_ONE / 4;
  wait(1);
  FieldCamera_FollowActor(ACTOR_CROSSING_SHIP, 1);
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, 0.6);
  actor->timer = 0;
  if (WorldMap_GetRowingBalance() == ROWING_BALANCE_OFF_COURSE) {
    Actor_SetScript(ACTOR_CROSSING_SHIP, gCrossingShipVeersNorth);
  } else {
    Actor_SetScript(ACTOR_CROSSING_SHIP, gCrossingShipSailsWest);
  }
  do {
    wait(1);
  } while (actor->timer == 0);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_TOLBI_SHIP_ROWING);
  Import_EventSetExitRequest(WORLD_MAP_TO_TOLBI_SHIP_MAST_LAND_HO);
  Event_End();
}

/// The crossing's last leg, in which the ship sails into Tolbi's harbour.
static void WorldMap_SailIntoTolbi(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_CROSSING_SHIP);
  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_CROSSING_SHIP, 5096, 2328);
  actor->scaleY = 5 * Q16_ONE / 4;
  actor->scaleX = 5 * Q16_ONE / 4;
  wait(1);
  FieldCamera_FollowActor(ACTOR_CROSSING_SHIP, 1);
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, 0.4);
  Actor_WalkToAndWait(ACTOR_CROSSING_SHIP, 5064, 2328);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_TOLBI_SHIP_REACHED_TOLBI);
  GameFlag_Clear(FLAG_TOLBI_SHIP_ROWING);
  Import_EventSetExitRequest(WORLD_MAP_TO_TOLBI_SHIP_DECK);
  Event_End();
}

/// Demonstrates setting a Djinni and placing it on standby.
static void WorldMap_ShowDjinnTutorial(void) {
  WorldMap_Unfreeze();
  WorldMap_StartZoom(Q16_ONE, 6);
  Event_WaitForZoomTransition();
  WorldMap_Freeze();
  Actor_RunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
  Event_SetMessageId(MsgWorldMapAfterBringingDjinniIntoYour);
  // "After bringing a Djinni into your party, you must set it to..."
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Event_Wait(30);
  Sound_Play(SE_MENU_CURSOR_MOVE);
  Menu_AnimateSelectionToEntry(0, 2);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_A);
  Event_ClearFlag(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  Actor_Jump(ACTOR_VENUS_DJINNI, 4, 30);
  Event_SetMessageId(MsgWorldMapNextIllShowHowCan);
  // "Next, I'll show you how you can use us in battle."
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  GameFlag_Clear(FLAG_DJINN_LESSON_PART_A);
  GameFlag_Set(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  Event_Wait(30);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 6);
}

/// Walking up to Flint makes him join the party and show how Djinn are set
/// and put on standby. Then comes the tutorial battle.
static void WorldMap_MeetFlint(void) {
  struct Actor *djinni;
  struct Actor *leader;
  s32 x;
  s32 z;
  s32 count;
  s32 i;

  count = 0;
  djinni = Actor_Get(ACTOR_VENUS_DJINNI);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  x = (leader->x.fixed - px(5584)) / 2 + px(5584);
  z = (leader->z.fixed - px(1328)) / 2 + px(1328);
  if (!Event_GetFlag(FLAG_DJINN_LESSON)) {
    Party_RestoreHpPp(1);
    GameFlag_Set(FLAG_DJINN_LESSON);
    Event_Start();
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0)
      Actor_SetPositionFixed(ACTOR_VENUS_DJINNI, leader->x.fixed,
                             leader->z.fixed);
    Djinn_Give(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT));
    DjinnRecovery_AddStandby(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT));
    WorldMap_Freeze();
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_VENUS_DJINNI, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    djinni->userValue = 1;
    Actor_FaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
    wait(16);
    Event_SetMessageId(MsgWorldMapOh);
    // "Oh!"
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    WorldMap_Unfreeze();
    WorldMap_StartZoom(6 * Q16_ONE / 5, 6);
    Event_WaitForZoomTransition();
    WorldMap_Freeze();
    djinni->motionFlags = ACTOR_FALLS;
    djinni->gravity = px(1) / 4;
    djinni->speedLimit = px(1);
    djinni->acceleration = px(1);
    djinni->velocityY = 0;
    djinni->groundHeight = 0;
    FieldActor_SetMoveTarget(djinni, 5584, 0, 1328);
    for (i = 0; i < 16; i++) {
      djinni->scaleX += Q16_ONE / 32;
      djinni->scaleY += Q16_ONE / 32;
      wait(1);
    }
    Actor_FaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_VENUS_DJINNI, 0);
    wait(16);
    djinni->update = 0;
    Actor_SetColorMode(djinni, SPRITE_COLOR_NORMAL);
    djinni->gravity = px(1);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Sound_Play(SE_PSYNERGY_SPARKLE);
    FieldPsynergy_LoadResources(ABILITY_MOVE, 0);
    for (i = 0; i < 60; i++) {
      if (gFrameCount & 2)
        Actor_SetColorMode(djinni, SPRITE_COLOR_WHITE);
      else
        Actor_SetColorMode(djinni, SPRITE_COLOR_NORMAL);
      if ((gFrameCount & 15) == 0)
        WorldMap_CreateAttachedEffects(djinni);
      wait(1);
    }
    FieldPsynergy_End();
    Actor_SetColorMode(djinni, SPRITE_COLOR_NORMAL);
    Actor_RunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    Actor_WalkToAndWait(ACTOR_VENUS_DJINNI, x >> 16, z >> 16);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Actor_Jump(ACTOR_VENUS_DJINNI, 4, 30);
    MessageArgument_Push(ABILITY_DJINNI_FIRST, MESSAGE_ARGUMENT_ABILITY);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_Jump(ACTOR_VENUS_DJINNI, 2, 30);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    i = 0;
    djinni->motionFlags = 0;
    FieldActor_SetMoveTargetFixed(djinni, x, px(16), z);
    for (; i < 16; i++) {
      djinni->facing += FACING_STEP;
      wait(1);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    djinni->motionFlags = ACTOR_FALLS;
    djinni->velocityY = 0;
    djinni->groundHeight = 0;
    for (i = 0; i < 8; i++) {
      djinni->facing += FACING_STEP;
      wait(1);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_ShowEmote(ACTOR_VENUS_DJINNI, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    Actor_FaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
    Actor_RunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_Jump(ACTOR_VENUS_DJINNI, 2, 30);
    Event_OpenMessage(ACTOR_VENUS_DJINNI, 0);
    // Each "No" makes Flint plead again; the seventh time he gives up asking.
    count = 0;
  plead:
    if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
      Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
      Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
      if (count == 6) {
        Event_SetMessageId(MsgWorldMapMeanieDontCare);
        // "Meanie! I don't care what you say! I'm coming along with you!"
        Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
        goto listened;
      }
      Event_SetMessageId(count + MsgWorldMapComePromiseWont);
      Event_OpenMessage(ACTOR_VENUS_DJINNI, 0);
      count++;
      goto plead;
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
    Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
    Actor_Jump(ACTOR_VENUS_DJINNI, 4, 20);
    Event_SetMessageId(MsgWorldMapSeeWontRegret);
    // "Oh, you'll see! You won't regret it!"
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  listened:
    MessageArgument_Push(ABILITY_DJINNI_FIRST, MESSAGE_ARGUMENT_ABILITY);
    Sound_Play(MUS_JOINED);
    count = MsgWorldMapAbilityVenusDjinni;
    Message_ShowCentered(count++, 3);
    Event_SetMessageId(count);
    Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
    // "..., the Venus Djinni, is now Isaac's ally!"
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Sound_Play(MUS_DJINN_LESSON);
    WorldMap_ShowDjinnTutorial();
    WorldMap_Unfreeze();
    return;
  }
  Event_Start();
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0)
    Actor_SetPositionFixed(ACTOR_VENUS_DJINNI, leader->x.fixed,
                           leader->z.fixed);
  djinni->velocityY = px(10);
  FieldActor_SetMoveTargetFixed(djinni, x, 0, z);
  Event_Wait(30);
  WorldMap_Freeze();
  Actor_FaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_VENUS_DJINNI, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  // Offer the tutorial again, then demonstrate setting and standby.
  Event_SetMessageId(MsgWorldMapSeeDjinnUseful);
  Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
  Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
  // "See! We Djinn can be very useful, can't we?"
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Actor_RunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
  // "And one more thing..."
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Sound_Play(SE_MENU_CURSOR_MOVE);
  Menu_AnimateSelectionToEntry(0, 2);
  GameFlag_Set(FLAG_DJINN_LESSON_PART_A);
  GameFlag_Clear(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  Event_SetMessageId(MsgWorldMapGoSetStandby);
  FieldActor_SetMoveTarget(djinni, 5584, 0, 1328);
  Event_Wait(30);
  // "So, we go from set to standby to recovery, then back to set."
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Actor_FaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
  // "This is the simplest way to use us."
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  // "Did you get all that?"
  Event_OpenMessage(ACTOR_VENUS_DJINNI, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
    Actor_RunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
    Event_SetMessageId(MsgWorldMapHmmmmExplainAgain);
    // "Hmmmm, shall I explain it all to you again?"
    Event_OpenMessage(ACTOR_VENUS_DJINNI, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) != 1) {
      // "Well, then, I guess I'll start from the beginning, OK?"
      Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
      Actor_WalkToAndWait(ACTOR_VENUS_DJINNI, x >> 16, z >> 16);
      WorldMap_ShowDjinnTutorial();
      WorldMap_Unfreeze();
      return;
    }
  }
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  Event_SetMessageId(MsgWorldMapYeahWantLearn);
  Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
  Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_ShowEmote(ACTOR_VENUS_DJINNI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Oh, yeah! If you want to learn more about Djinn, refer to..."
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Event_SetFlag(FLAG_DJINN_LESSON_PART_A);
  Event_SetFlag(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
  // "Okay, I'll join you now. Thank you very much! Please, use us..."
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  WorldMap_Unfreeze();
  Djinn_JoinParty(ACTOR_VENUS_DJINNI, ELEMENT_VENUS,
                  DJINNI_INDEX(DJINNI_FLINT));
  Sound_Play(MUS_WORLD_MAP);
  Event_End();
  GameFlag_Clear(FLAG_DJINN_LESSON);
  GameFlag_Clear(FLAG_DJINN_LESSON_PART_A);
  GameFlag_Clear(FLAG_DJINN_LESSON_PART_B);
}

s32 gWorldMapIodem;

/// Brings Bunza's wagon to a stop and lets Isaac step down, after Hammet's
/// return home (kalay.c's Kalay_HammetReturnsToPalace).
static void WorldMap_StepDownFromBunzasWagon(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_KALAY_WAGON);
  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  wait(1);
  actor->scaleX = 3 * Q16_ONE / 2;
  actor->scaleY = 3 * Q16_ONE / 2;
  actor->facing = FACING_SOUTH;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_SetPosition(ACTOR_KALAY_WAGON, 5480, 2104);
  wait(1);
  Sound_Play(SE_RUMBLE);
  Actor_SetSpeedAndAcceleration(ACTOR_KALAY_WAGON, 1.6, 0.4);
  Actor_SetAnimation(ACTOR_KALAY_WAGON, ANIM_WALK);
  Actor_MoveToAndWait(ACTOR_KALAY_WAGON, 5485, 2136);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(5560, CAMERA_KEEP, 2136, 1);
  Actor_MoveToAndWait(ACTOR_KALAY_WAGON, 5534, 2136);
  Actor_MoveToAndWait(ACTOR_KALAY_WAGON, 5544, 2158);
  Actor_MoveToAndWait(ACTOR_KALAY_WAGON, 5608, 2168);
  Actor_SetAnimation(ACTOR_KALAY_WAGON, ANIM_STAND);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Event_Wait(20);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 5592, 2168);
  wait(1);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 5576, 2168);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
  Sound_Play(SE_RUMBLE);
  Actor_SetAnimation(ACTOR_KALAY_WAGON, ANIM_WALK);
  Actor_MoveToAndWait(ACTOR_KALAY_WAGON, 5624, 2168);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_MoveToAndWait(ACTOR_KALAY_WAGON, 5624, 2104);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_KALAY_WAGON, 5565, 2104);
  Actor_MoveToAndWait(ACTOR_KALAY_WAGON, 5560, 2131);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_MoveToAndWait(ACTOR_KALAY_WAGON, 5490, 2136);
  Actor_MoveToAndWait(ACTOR_KALAY_WAGON, 5480, 2104);
  Actor_SetPosition(ACTOR_KALAY_WAGON, 0, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Event_Wait(40);
  FieldCamera_MoveTo(5592, CAMERA_KEEP, 2136, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 5592, 2136);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(WORLD_MAP_TO_KALAY_EAST);
  Event_End();
}

/// Out of Gateway Cave, Iodem sees Babi's ship scuttled off the coast, gives
/// Isaac the Black Orb and prompts him to use it on the ship.
static void WorldMap_RunBlackOrbEvent(void) {
  struct Actor *leader;

  Event_Start();
  gWorldMapIodem = ACTOR_IODEM;
  WorldMap_BlockGatewayCaveExit(ACTOR_IODEM);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  WorldMap_Freeze();
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFixed(gWorldMapIodem, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(gWorldMapIodem, 1.6);
  Actor_WalkToAndWait(gWorldMapIodem, 5992, 3448);
  Actor_FaceDirection(gWorldMapIodem, FACING_EAST, 20);
  Actor_ShowEmote(gWorldMapIodem, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_StartRepeatedMotion(gWorldMapIodem, 2);
  Event_SetMessageId(MsgWorldMapLook);
  // "Look at this!"
  Event_ShowMessageThenWait(gWorldMapIodem | SPEAKER_PORTRAIT_LEFT, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  WorldMap_Unfreeze();
  WorldMap_StartZoom(7 * Q16_ONE / 5, 10);
  FieldCamera_SetSpeed(8);
  FieldCamera_MoveTo(6024, CAMERA_KEEP, 3432, 1);
  FieldCamera_WaitForMove();
  Event_Wait(40);
  WorldMap_Freeze();
  Actor_SetAttachedEffect(gWorldMapIodem,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_RunRepeatedMotion(gWorldMapIodem, 1);
  // "What happened?"
  Event_ShowMessageThenWait(
      gWorldMapIodem | SPEAKER_PORTRAIT_LEFT | SPEAKER_PORTRAIT_RIGHT, 0, 40);
  Actor_WalkToAndWait(gWorldMapIodem, 5992, 3400);
  Actor_WalkToAndWait(gWorldMapIodem, 6036, 3400);
  Actor_FaceDirection(gWorldMapIodem, FACING_SOUTH_SOUTHEAST, 20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_SetScript(ACTOR_PARTY_LEADER, gBlackOrbLeaderScript);
  Event_Wait(20);
  Actor_RunRepeatedMotion(gWorldMapIodem, 1);
  // "Babi's ship..."
  Event_ShowMessageThenWait(gWorldMapIodem, 0, 10);
  while (Actor_Get(ACTOR_PARTY_LEADER)->timer == 0) {
    wait(1);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
  Actor_ShowEmote(gWorldMapIodem, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Actor_RunRepeatedMotion(gWorldMapIodem, 2);
  Event_ShowMessageThenWait(gWorldMapIodem, 0, 10);
  Actor_FaceDirection(gWorldMapIodem, FACING_WEST, 10);
  Event_ShowMessage(gWorldMapIodem, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
  Actor_Get(gWorldMapIodem)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(gWorldMapIodem, 6028, 3400);
  Event_Wait(1);
  Actor_Get(gWorldMapIodem)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(20);
  Actor_Get(gWorldMapIodem)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(gWorldMapIodem, 6036, 3400);
  Event_Wait(1);
  Actor_Get(gWorldMapIodem)->steeringFlags |= ACTOR_FACES_MOTION;
  Item_ShowFound(ITEM_BLACK_ORB, 3);
  Party_GiveItem(ITEM_BLACK_ORB, 0);
  Actor_RunRepeatedMotion(gWorldMapIodem, 1);
  Event_SetMessageId(MsgWorldMapNowUseOnShip);
  // "Now use it... on the ship."
  Event_ShowMessageThenWait(gWorldMapIodem, 0, 10);
  Actor_FaceDirection(gWorldMapIodem, FACING_SOUTH_SOUTHEAST, 0);
  WorldMap_Unfreeze();
  WorldMap_StartZoom(Q16_ONE, 10);
  Event_Wait(20);
  GameFlag_Set(FLAG_WORLD_MAP_BLACK_ORB_HANDED_OVER);
  GameFlag_Set(FLAG_SAW_SHIP_AT_IDEJIMA);
  gMainRoutineWork.restartScene = (s32)&SceneId_WorldMap;
  gMainRoutineWork.restartEntrance = WORLD_MAP_AFTER_BLACK_ORB_DEFEAT;
  Event_End();
}

/// Turns Gateway Cave's exit into a step trigger that runs
/// WorldMap_IodemCallsIsaacBack.
static void WorldMap_BlockGatewayCaveExit(s32 actorId) {
  s32 i;

  i = 0;
  while (1) {
    if (gWorldTriggers[i].kind == TRIGGER_EXIT &&
        gWorldTriggers[i].subject == STEP_TRIGGER_AREA(ACTOR_TO_GATEWAY_CAVE)) {
      gWorldTriggers[i].kind = TRIGGER_STEP;
      gWorldTriggers[i].value = (u32)WorldMap_IodemCallsIsaacBack;
    }
    if (gWorldTriggers[i].kind == TRIGGER_TABLE_END)
      break;
    i++;
  }
  for (i = 0;; i++) {
    if (gWorldMapActorPlacements[i].sprite == SPRITE_IODEM) {
      gWorldMapActorPlacements[i].x = px(6036);
      gWorldMapActorPlacements[i].z = px(3400);
      gWorldMapActorPlacements[i].facing = FACING_SOUTH_SOUTHEAST;
      return;
    }
  }
}

/// Makes touching Gateway Cave an exit again, to WORLD_MAP_TO_GATEWAY_CAVE.
static void WorldMap_RestoreGatewayCaveExit(void) {
  s32 i;

  i = 0;
  while (1) {
    if (gWorldTriggers[i].kind == TRIGGER_STEP &&
        gWorldTriggers[i].subject == STEP_TRIGGER_AREA(ACTOR_TO_GATEWAY_CAVE)) {
      gWorldTriggers[i].kind = TRIGGER_EXIT;
      gWorldTriggers[i].value = WORLD_MAP_TO_GATEWAY_CAVE;
      break;
    }
    if (gWorldTriggers[i].kind == TRIGGER_TABLE_END)
      break;
    i++;
  }
}

/// Talking to Iodem, who gave the Black Orb, or reading his mind, makes him
/// urge its use. "Now use it... on the ship."
static void WorldMap_TalkAboutBlackOrb(void) {
  Event_Start();
  Actor_FaceActor(ACTOR_IODEM, ACTOR_PARTY_LEADER, 0);
  Event_SetMessageId(MsgWorldMapNowUseOnShip);
  // "Now use it... on the ship."
  Event_ShowMessage(gWorldMapIodem, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH_SOUTHEAST, 0);
  Event_End();
}

/// Walking back into Gateway Cave with the Black Orb unused makes Iodem call
/// the leader back to the coast. "Isaac, where you going? I said to use the
/// Orb on Babi's ship!"
static void WorldMap_IodemCallsIsaacBack(void) {
  Event_Start();
  WorldMap_Freeze();
  Event_SetMessageId(MsgWorldMapIsaacWhereGoingSaidUse);
  // "Isaac, where you going? I said to use the Orb on Babi's ship!"
  Event_ShowMessage(gWorldMapIodem, 0);
  WorldMap_Unfreeze();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 6008, 3400);
  Event_End();
}

/// Searching the wreck off Gateway Cave shows "The wreckage of a ship
/// scuttled off the coast."
static void WorldMap_SearchShipwreck(void) {
  Event_Start();
#if !TBS_PATCH_3
  WorldMap_Freeze();
#endif
  FieldMessage_Show(MsgWorldMapWreckageShipScuttledOffCoast, 1);
  if (GameFlag_Get(FLAG_WORLD_MAP_BLACK_ORB_HANDED_OVER) != 0) {
    gFieldWork->fieldMenu = 1;
  }
#if !TBS_PATCH_3
  WorldMap_Unfreeze();
#endif
  Event_End();
}

/// Using the Black Orb on the shipwreck does nothing. The orb is taken back
/// and the party is to press on to Babi Lighthouse.
static void WorldMap_UseBlackOrb(void) {
  struct Actor *iodem;
  struct Actor *orb;
  struct Actor *leader;
  struct Actor *actor;
  s32 message;
  struct Sprite *sprite;
  u8 *buffer;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Start();
  WorldMap_Unfreeze();
  WorldMap_StartZoom(7 * Q16_ONE / 5, 6);
  FieldCamera_SetSpeed(3);
  FieldCamera_MoveTo(6024, CAMERA_KEEP, 3432, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  iodem = NULL;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  leader->paused = 0;
  Actor_StartIdle(leader);
  if (leader->z.fixed > px(3432)) {
    if (leader->x.fixed > px(5998)) {
      FieldActor_SetMoveTargetFixed(leader, px(5998), leader->y.fixed,
                                    px(3453));
      Actor_WaitForMove(leader);
    }
  } else if (leader->x.fixed > px(6010)) {
    // BUG: iodem is NULL here; this branch reads its height through null.
    FieldActor_SetMoveTargetFixed(leader, px(6010), iodem->y.fixed, px(3400));
    Actor_WaitForMove(leader);
  }
  FieldActor_SetMoveTarget(leader, 5993, 0, 3432);
  Actor_WaitForMove(leader);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
  WorldMap_Freeze();
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
  orb = FieldActor_Create(SPRITE_BLANK, leader->x.fixed + px(2), px(38),
                          leader->z.fixed);
  if (orb != NULL) {
    orb->motionFlags = 0;
    sprite = orb->sprite;
    sprite->flags = 0;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    /* FAKEMATCH(dead): clearing the motion flags again through a pointer;
     * without it, 11 of 363 instructions differ in tbs-en. */
    *(u8 *)&orb->motionFlags = 0;
    buffer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
    Item_LoadIcon(ITEM_BLACK_ORB);
    Vram_Load(sprite->slot, 128, buffer + 1024);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
    Event_Wait(20);
    orb->update = WorldMap_PulseOrbAndSpawnSparks;
    Event_Wait(80);
  }
  Actor_Get(gWorldMapIodem)->facing = FACING_SOUTH_SOUTHEAST;
  Actor_ShowEmote(gWorldMapIodem, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(gWorldMapIodem, 2);
  message = MsgWorldMapMatter;
  Event_SetMessageId(message);
  // "What's the matter?"
  Event_ShowMessageThenWait(gWorldMapIodem, 0, 80);
  if (orb != NULL) {
    Actor_Release(orb);
  }
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(40);
  Actor_Jump(gWorldMapIodem, 6, 40);
  // "Strange... Nothing happened. What does it mean?"
  Event_ShowMessageThenWait(gWorldMapIodem, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(gWorldMapIodem, FACING_NORTH_NORTHEAST, 20);
  // "Could Babi have been wrong?"
  Event_ShowMessageThenWait(
      gWorldMapIodem | SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT, 0, 40);
  Actor_SetAnimationAndWait(gWorldMapIodem, ANIM_SHAKE_HEAD);
  // "No. Not Babi..."
  Event_ShowMessageThenWait(
      gWorldMapIodem | SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT, 0, 20);
  Actor_FaceDirection(gWorldMapIodem, FACING_SOUTH_SOUTHEAST, 20);
  // "I was wrong-it is not yet time. We must press on to Babi..."
  Event_ShowMessageThenWait(gWorldMapIodem, 0, 10);
  Actor_SetSpeed(gWorldMapIodem, 0.8);
  Actor_SetAnimation(gWorldMapIodem, ANIM_WALK);
  iodem = Actor_Get(ACTOR_IODEM);
  FieldActor_SetMoveTargetFixed(iodem, px(6010), iodem->y.fixed, px(3400));
  Actor_WaitForMove(iodem);
  FieldActor_SetMoveTarget(iodem, 6001, 0, 3416);
  Actor_WaitForMove(iodem);
  Actor_SetAnimation(ACTOR_IODEM, ANIM_STAND);
  Actor_FaceDirection(gWorldMapIodem, FACING_SOUTH_SOUTHWEST, 10);
  Actor_RunRepeatedMotion(gWorldMapIodem, 1);
  // "I will take that back, for now."
  Event_ShowMessageThenWait(gWorldMapIodem | SPEAKER_PORTRAIT_LEFT, 0, 20);
  Actor_SetSpeed(gWorldMapIodem, 1);
  Actor_Get(ACTOR_IODEM)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetAnimation(ACTOR_IODEM, ANIM_WALK);
  FieldActor_SetMoveTarget(iodem, 5997, 0, 3424);
  Actor_WaitForMove(iodem);
  Actor_SetAnimation(ACTOR_IODEM, ANIM_STAND);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_IODEM, ANIM_WALK);
  FieldActor_SetMoveTarget(iodem, 6001, 0, 3416);
  Actor_WaitForMove(iodem);
  Actor_SetAnimation(ACTOR_IODEM, ANIM_STAND);
  Message_ShowCentered(message + 6, 1);
  gFieldWork->message++;
  PartyInventory_Discard(ITEM_BLACK_ORB);
  Event_Wait(20);
  Actor_SetAnimation(gWorldMapIodem, ANIM_SHAKE_HEAD);
  // "We will return to the ship later."
  Event_ShowMessageThenWait(gWorldMapIodem, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(gWorldMapIodem, ANIM_NOD);
  // Iodem walks into Isaac and is taken off the map.
  Actor_SetAnimation(gWorldMapIodem, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(gWorldMapIodem, actor->x.part.pixel,
                         actor->z.part.pixel);
  }
  Actor_WaitForMoveById(gWorldMapIodem);
  Actor_SetPosition(gWorldMapIodem, 0, 0);
  WorldMap_Unfreeze();
  WorldMap_StartZoom(Q16_ONE, 6);
  Event_Wait(20);
  WorldMap_RestoreGatewayCaveExit();
  Actor_ReleaseById(gWorldMapIodem);
  GameFlag_Clear(FLAG_WORLD_MAP_BLACK_ORB_HANDED_OVER);
  GameFlag_Set(FLAG_BLACK_ORB_RETURNED);
  Event_End();
}

/// The box around Venus Lighthouse, in pixels, where dust puffs up as it
/// shakes (WorldMap_UpdateLighthouseQuake).
enum {
  QUAKE_DUST_LEFT = 6064,
  QUAKE_DUST_TOP = 3148,
  QUAKE_DUST_WIDTH = 40,
  QUAKE_DUST_DEPTH = 30
};

u16 gWorldMapBlend;

/// Queues one picture of the lighthouse transition, a whole tile map, into
/// BG0's screen block.
#define WORLD_MAP_QUEUE_TRANSITION_PICTURE(buffer, offset)                     \
  IO_WRITE_QUEUE_PUSH((u32)BG_SCREEN_BLOCK(4), (u32)((buffer) + (offset)),     \
                      DMA_COPY_WORDS(320))

/// The ending, on the island Jenna and Kraden are adrift on.
static void WorldMap_ShowEndingEvent(void) {
  WindowColor_Apply(gMainRoutineWork.windowHue,
                    gMainRoutineWork.windowBrightness);
  Event_Start();
  WorldMap_StartZoom(Q16_ONE, 300);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_JENNA_LIE);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_KRADEN_LIE);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  wait(1);
  WorldMap_StartZoom(3 * Q16_ONE / 2, 16);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  gFieldWork->transitionFrames = 16;
  Event_StartScreenTransition();
  Event_WaitForZoomTransition();
  WorldMap_Freeze();
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  Event_Wait(20);
  // Jenna and Kraden talk on the drifting island before Alex arrives.
  Event_SetMessageId(MsgWorldMapKradenHowLongWillIsland);
  // "Oh, Kraden... How long will this island continue to drift?"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(20);
  // "I don't know... I wish you could tell me."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  // "Sheesh... You're no help in times like this."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  // "Hey, I'm hungry..."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  // "How can you think about food?"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 100);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "I wonder what happened to my brother..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  // "I can't be certain, but it looks like they lit the Venus beacon."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  // "You don't have to tell me that. That's how we wound up..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(80);
  // "You are so cranky today, Jenna!"
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  // "Well, I suppose you must be worried about Felix."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 120);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  // "Isaac and his friends must have been nearby, too."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 40);
  // "They probably were..."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "I miss them... Isaac... Felix..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 120);
  // Alex comes to show them something and leads them on.
  Actor_SetSpeed(ACTOR_ALEX, 0.4);
  Actor_SetPosition(ACTOR_ALEX, 7644, 3460);
  Actor_WalkToAndWait(ACTOR_ALEX, 7572, 3468);
  Actor_WalkToAndWait(ACTOR_ALEX, 7560, 3488);
  Event_Wait(20);
  // "There is something I wish to show you."
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  // "Walk this way."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX,
                    0);
  WorldMap_LoadDefaultCellsAndUpdateBlock();
  Event_Wait(20);
  Actor_SetScript(ACTOR_ALEX, gEndingAlexWalk);
  Event_Wait(80);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Actor_Jump(ACTOR_KRADEN, 4, 40);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_STAND);
  Actor_Jump(ACTOR_JENNA, 4, 60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 40);
  Actor_SetSpeed(ACTOR_KRADEN, 0.6);
  Actor_SetSpeed(ACTOR_JENNA, 0.6);
  Actor_SetScript(ACTOR_KRADEN, gEndingKradenWalk);
  Event_Wait(20);
  FieldCamera_SetSpeed(0.7);
  FieldCamera_MoveTo(7736, CAMERA_KEEP, 3528, 1);
  Actor_SetScript(ACTOR_JENNA, gEndingJennaWalk);
  do {
    Actor_SetAnimation(ACTOR_FELIX, FELIX_ANIMATION_LIE);
    Actor_SetAnimation(ACTOR_SHEBA, SHEBA_ANIMATION_LIE);
    wait(1);
  } while (Actor_Get(ACTOR_JENNA)->timer == 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_WEST, 20);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  WorldMap_Freeze();
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_JENNA, 7, 13, 2, 12, ACTOR_KRADEN, 9,
                                    4, 4, 3, 0);
  Event_Wait(20);
  WorldMap_LoadDefaultCellsAndUpdateBlock();
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(7768, CAMERA_KEEP, 3528, 1);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.6);
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_WalkTo(ACTOR_KRADEN, 7804, 3512);
  Actor_WalkToAndWait(ACTOR_JENNA, 7788, 3544);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  WorldMap_Freeze();
  Event_Wait(80);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 2);
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 60);
  WorldMap_LoadDefaultCellsAndUpdateBlock();
  Sound_Play(SOUND_FADE_OUT_BGM);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(120);
  wait(120);
  Event_SetDestination((s32)&SceneId_Title, TITLE_FROM_WORLD_MAP);
}

/// The look back before Sheba's fall, with the view settling on Venus
/// Lighthouse.
static void WorldMap_ShakeVenusLighthouse(void) {
  Event_Start();
  Sound_Play(SE_RUMBLE);
  PaletteFade_ApplySource(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  wait(2);
  gFieldWork->transitionFrames = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  wait(1);
  FieldCamera_SetSpeed(4);
  Task_AddFrame(WorldMap_UpdateLighthouseQuake, TASK_PASS_FRAME);
  PaletteFade_ApplySource(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_MUTED, PALETTES_BG);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(40);
  Event_Wait(240);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(80);
  wait(90);
  Import_EventSetExitRequest(WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE);
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
  Event_End();
}

/// Run each frame while Venus Lighthouse shakes. It puffs up dust at a random
/// spot in the QUAKE_DUST box, and every third frame moves the camera a pixel
/// to one of four points around the lighthouse.
static void WorldMap_UpdateLighthouseQuake(void) {
  u32 x = Random_Below(QUAKE_DUST_WIDTH);
  u32 z = Random_Below(QUAKE_DUST_DEPTH);
  struct Actor *actor;

  actor = FieldActor_Create(SPRITE_DUST_PUFF, px(x) + px(QUAKE_DUST_LEFT), 0,
                            px(z) + px(QUAKE_DUST_TOP));
  if (actor != NULL) {
    struct Sprite *sprite = actor->sprite;
    s32 scale = (Random_Below(32768)) + 6 * Q16_ONE / 5;

    sprite->flags = 0;
    sprite->image.priority = 2;
    actor->motionFlags = 0;
    actor->scaleX = scale;
    actor->scaleY = scale;
    Actor_SelectAnimation(actor, 1);
    Actor_StartScript(actor, gQuakeDustScript);
  }
  if (gFrameCount % 3 == 0) {
    switch (Random_Below(4)) {
    case 0:
      FieldCamera_MoveTo(6087, CAMERA_KEEP, 3177, 1);
      break;
    case 1:
      FieldCamera_MoveTo(6089, CAMERA_KEEP, 3175, 1);
      break;
    case 2:
      FieldCamera_MoveTo(6089, CAMERA_KEEP, 3177, 1);
      break;
    case 3:
      FieldCamera_MoveTo(6087, CAMERA_KEEP, 3175, 1);
      break;
    }
  }
}

/// A leg of the Tolbi-bound Ship's crossing, in which the ship sails north to
/// its anchorage off the island.
static void WorldMap_SailTolbiShipToIsland(void) {
  struct Actor *actor;

  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  Actor_SetAnimation(ACTOR_CROSSING_SHIP, ANIM_WALK);
  Actor_SetPosition(ACTOR_CROSSING_SHIP, 4872, 808);
  actor = Actor_Get(ACTOR_CROSSING_SHIP);
  actor->facing = FACING_NORTHWEST;
  wait(1);
  WorldMap_StartZoom(6 * Q16_ONE / 5, 1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  FieldCamera_FollowActor(ACTOR_CROSSING_SHIP, 1);
  wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, 0.4);
  Actor_MoveToAndWait(ACTOR_CROSSING_SHIP, 4824, 712);
  Actor_MoveToAndWait(ACTOR_CROSSING_SHIP, 4776, 616);
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, 0.3);
  Actor_MoveToAndWait(ACTOR_CROSSING_SHIP, 4776, 472);
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, 0.2);
  Actor_MoveToAndWait(ACTOR_CROSSING_SHIP, 4760, 456);
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, 0.1);
  Actor_MoveToAndWait(ACTOR_CROSSING_SHIP, 4760, 440);
  Actor_SetAnimation(ACTOR_CROSSING_SHIP, ANIM_STAND);
  Event_Wait(40);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(WORLD_MAP_TO_SHIP_ROOMS_ROWING_DECK_ISLAND);
}

/// The rowers' balance, as TolbiShipMast_GetRowingBalance (tolbi_ship_mast.c)
/// reckons it.
static s32 WorldMap_GetRowingBalance(void) {
  s32 positiveTotal = WorldMap_GetSeatRowValue(0);
  s32 negativeTotal;

  positiveTotal += WorldMap_GetSeatRowValue(2);
  negativeTotal = WorldMap_GetSeatRowValue(1);
  negativeTotal += WorldMap_GetSeatRowValue(3);
  return positiveTotal - negativeTotal;
}

/// The rowing value of the passenger chosen for a seat, or 0 while it is empty.
static s32 WorldMap_GetSeatRowValue(u32 seat) {
  s32 flagBase = 0;
  u32 offset;

  switch (seat) {
  case 0:
    flagBase = FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS;
    break;
  case 1:
    flagBase = FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS;
    break;
  case 2:
    flagBase = FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS;
    break;
  case 3:
    flagBase = FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS;
    break;
  }
  for (offset = 0; offset < 9; offset++) {
    if (GameFlag_Get(flagBase + offset) != 0)
      return gWorldMapPassengerRowValues[offset];
  }
  return 0;
}

/// Spawns a puff of the wake behind ACTOR_DRIFTING_ISLAND every 16 frames.
static void WorldMap_SpawnIslandWake(void) {
  struct Actor *island;
  struct Actor *puff;
  struct Sprite *sprite;
  s32 distance;

  if ((gFrameCount & 15) != 0)
    return;
  island = Actor_Get(ACTOR_DRIFTING_ISLAND);
  puff = FieldActor_Create(SPRITE_DUST_PUFF, island->x.fixed - px(32),
                           island->y.fixed, island->z.fixed - px(16));
  if (puff == 0)
    return;
  puff->scaleX = Q16_ONE / 2;
  puff->scaleY = Q16_ONE / 2;
  sprite = puff->sprite;
  // Up to 48 pixels behind the island, or up to 32 to the side.
  if ((Random_Below(2)) != 0) {
    s32 behind = px(Random_Below(48));

    puff->x.fixed -= behind / 2;
    puff->z.fixed -= behind;
  } else {
    distance = px(Random_Below(32));
    puff->x.fixed += distance;
    distance /= 2;
    puff->z.fixed += distance;
  }
  sprite->flags = 0;
  sprite->image.priority = island->sprite->image.priority;
  puff->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  puff->motionFlags = island->motionFlags;
  Actor_SetSpriteColor(puff, SPRITE_COLOR_BLUE_TINT);
  Actor_SelectAnimation(puff, 2);
  Actor_StartScript(puff, gIslandWakeScript);
}

/// Before the ending, the island Jenna and Kraden are adrift on drifts east
/// with its wake under warm colours. Then the map fades to white and leads
/// into their event (WORLD_MAP_TO_JENNA_AND_KRADEN_ADRIFT).
static void WorldMap_ShowIslandDrifting(void) {
  struct Actor *actor;
  struct Actor *spriteActor;

  actor = Actor_Get(ACTOR_DRIFTING_ISLAND);
  Event_Wait(60);
  Event_Start();
  WorldMap_StartZoom(3 * Q16_ONE / 5, 1);
  actor->scaleX = 6 * Q16_ONE / 5;
  actor->scaleY = 6 * Q16_ONE / 5;
  FieldCamera_FollowActor(ACTOR_DRIFTING_ISLAND, 1);
  wait(1);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  spriteActor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(spriteActor, 0);
  spriteActor = Actor_Get(ACTOR_DRIFTING_ISLAND);
  Actor_SetSpriteFlags(spriteActor, 0);
  Actor_SetSpeed(ACTOR_DRIFTING_ISLAND, 0.4);
  actor->timer = 0;
  Actor_SetScript(ACTOR_DRIFTING_ISLAND, gIslandDriftsEast);
  Task_AddFrame(WorldMap_SpawnIslandWake, TASK_PASS_FRAME);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  gFieldWork->transitionFrames = 32;
  Event_StartScreenTransition();
  Event_Wait(120);
  WorldMap_StartZoom(7 * Q16_ONE / 5, 300);
  Event_Wait(270);
  gFieldWork->transitionFrames = 16;
  PLTT_BACKDROP = RGB(31, 31, 31);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(WORLD_MAP_TO_JENNA_AND_KRADEN_ADRIFT);
}

/// Run each frame while a storm holds the party in the air.
static void WorldMap_UpdateStormTimer(void) {
  struct Actor *actor;
  struct FieldWork *work;
  s32 progress;

  actor = Actor_Get(gMainRoutineWork.leaderActorId);
  work = gFieldWork;
  actor->facing = (gFrameCount << 12);

  progress = FieldGameFlag_GetByte(FLAG_WORLD_MAP_STORM_FRAMES_LEFT);
  if (progress != 0) {
    if (progress == 1) {
      work->raisedTrigger = RAISED_STORM_TIME_UP;
    } else if (GameFlag_Get(FLAG_MENU_OPEN) == 0) {
      progress -= 1;
    }
  }
  FieldGameFlag_SetByte(FLAG_WORLD_MAP_STORM_FRAMES_LEFT, progress);
}

/// Draws the leader into the storm, lifts him and starts its timer.
static void WorldMap_CatchInStorm(s32 targetActorId) {
  s32 alreadyTraveling;
  struct Actor *actor;
  struct Actor *storm;
  s32 id;

  id = gMainRoutineWork.leaderActorId;
  actor = Actor_Get(id);
  alreadyTraveling = Event_GetFlag(FLAG_WORLD_MAP_CAUGHT_IN_STORM);
  if (alreadyTraveling == 0) {
    Event_Start();
    Actor_SetAttachedEffect(id,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    Actor_SetAnimation(id, ANIM_LEADER_LOOK_AROUND);
    storm = Actor_Get(targetActorId);
    if (storm != 0) {
      Actor_SetDestination(id, storm->x.part.pixel, storm->z.part.pixel);
    }
    Actor_WaitForMoveById(id);
    Sound_Play(SE_SANDSTORM);
    Task_AddFrame(WorldMap_UpdateStormTimer, TASK_PASS_FRAME);
    actor->motionFlags = alreadyTraveling;
    FieldActor_SetMoveTargetFixed(actor, actor->x.fixed,
                                  actor->y.fixed + px(32), actor->z.fixed);
    Actor_WaitForMoveById(id);
    actor->velocityY = alreadyTraveling;
    actor->motionFlags = ACTOR_FLOATS;
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_AIRBORNE;
    GameFlag_Set(FLAG_WORLD_MAP_CAUGHT_IN_STORM);
    FieldGameFlag_SetByte(FLAG_WORLD_MAP_STORM_FRAMES_LEFT, 180);
    Event_End();
    gFieldWork->encounter = 0; // drop any battle this step rolled
  }
}

/// Walking into the storm lifts the party up into the air.
static void WorldMap_WalkIntoStorm(void) { WorldMap_CatchInStorm(ACTOR_STORM); }

/// Starting to cast Douse on the storm gathers Douse's rain cloud 80 pixels
/// up, where the party floats (douse.c reads childOption).
static void WorldMap_StartDousingStorm(void) {
  ((struct FieldEffectWork *)gFieldEffectWork)->childOption = 1;
}

/// Douse on the storm brings the party down, and the battle with what is in
/// it begins (ENCOUNTER_ZONE_SCRIPTED, entries 5 and 7), as with the Suhalla
/// Desert's storms.
static void WorldMap_DouseMapStorm(void) {
  struct Actor *leader;

  if (gFieldWork->raisedTrigger == RAISED_STORM_TIME_UP) {
    gFieldWork->raisedTrigger = 0;
  }
  GameFlag_Clear(FLAG_WORLD_MAP_CAUGHT_IN_STORM);
  GameFlag_Set(FLAG_WORLD_MAP_STORM_GONE);
  FieldGameFlag_SetByte(FLAG_WORLD_MAP_STORM_FRAMES_LEFT, 0);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 5);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 7);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  leader->motionFlags = ACTOR_FALLS;
}

/// The storm drops the party outside Suhalla when its time in the air runs out.
static void WorldMap_StormDropsParty(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *storm = Actor_Get(ACTOR_STORM);
  s32 frames;

  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Sound_Play(SE_GUSH);
  Actor_SetSpriteFlags(leader, 0);
  storm->motionFlags = 0;
  leader->motionFlags = 0;
  leader->velocityY = 0;
  leader->frozen = 1;
  storm->frozen = 1;
  for (frames = 0; frames < 60; frames++) {
    leader->velocityY += px(1) / 5;
    storm->velocityY += px(1) / 5;
    wait(1);
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
  GameFlag_Set(FLAG_ARRIVE_FALLING);
  Event_SetDestination((s32)&SceneId_WorldMap, WORLD_MAP_FROM_SUHALLA);
}

/// Run each frame of the lighthouse transition. It blends BG0, where the
/// transition is drawn, over everything. BG0's own weight flickers between 12
/// and 16 every two frames, and the rest's comes from gWorldMapBlend.
static void WorldMap_UpdateTransitionBlend(void) {
  IoWriteQueue_Store16((u32)&REG_BLDCNT,
                       BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
  if ((gFrameCount & 2) != 0) {
    IoWriteQueue_Store16((u32)&REG_BLDALPHA,
                         gWorldMapBlend | BLEND_ALPHA(12, 0));
  } else {
    IoWriteQueue_Store16((u32)&REG_BLDALPHA,
                         gWorldMapBlend | BLEND_ALPHA(16, 0));
  }
}

/// Run each frame at the end of the lighthouse transition. Every third frame
/// it moves the camera a pixel diagonally from the leader.
static void WorldMap_JitterTransitionCamera(void) {
  struct Actor *actor;
  s32 sx;
  s32 sy;
  u32 mode;

  actor = Actor_Get(gMainRoutineWork.leaderActorId);
  sx = actor->x.part.pixel;
  sy = actor->z.part.pixel;
  if (gFrameCount % 3 == 0) {
    mode = Random_Below(4);
    switch (mode) {
    case 0:
      FieldCamera_MoveToFixed(px(sx) - px(1), CAMERA_KEEP, px(sy) + px(1), 1);
      break;
    case 1:
      FieldCamera_MoveWithinMapBounds(px(sx) + px(1), CAMERA_KEEP,
                                      px(sy) - px(1), 1);
      break;
    case 2:
      FieldCamera_MoveToFixed(px(sx) + px(1), CAMERA_KEEP, px(sy) + px(1), 1);
      break;
    case 3:
      FieldCamera_MoveToFixed(px(sx) - px(1), CAMERA_KEEP, px(sy) - px(1), 1);
      break;
    }
  }
}

/// Queues one picture of the lighthouse transition and waits frames.
static inline void WorldMap_ShowTransitionPicture(u8 *buffer, s32 offset,
                                                  s32 frames) {
  WORLD_MAP_QUEUE_TRANSITION_PICTURE(buffer, offset);
  Event_Wait(frames);
}

/// The lighthouse transition, after a beacon is lit.
///
/// A ball of light swells over the lighthouse's top, frame by frame, as the
/// map darkens behind it. It holds for 140 frames and shrinks again. Then the
/// camera shakes and the screen closes for the aerie
/// (https://alchemy.pascalpixel.com/story/venus-lighthouse-lit.gif).
///
/// The ball is seven frames, tile maps drawn on BG0 in the lighthouse's
/// colours (gWorldMapPalettes, by enum WorldMapLighthouse).
///
/// It clears FLAG_KEEP_PARTY_POSITION and sets FLAG_SKIP_SCENE_CHANGE_SOUND.
static void
WorldMap_PlayLighthouseTransition(enum WorldMapLighthouse lighthouse) {
  u8 *buffer = Heap_AllocEwram(16384);

  wait(1);
  GameFlag_Clear(FLAG_KEEP_PARTY_POSITION);
  WorldMap_StartStillPicture();
  FieldDecompress_TaggedLz(gWorldMapPackedTiles, buffer);
  FieldDecompress_TaggedLz(gWorldMapPackedPictures, buffer + 0x1000);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT[14 * 16],
                      (u32)(gWorldMapPalettes + lighthouse * 32),
                      DMA_ENABLE << 16 | 16);
  IO_WRITE_QUEUE_PUSH((u32)BG_CHAR_BLOCK(0) + 0x1000, (u32)buffer,
                      DMA_COPY_WORDS(1024));
  Task_AddFrame(WorldMap_UpdateTransitionBlend, TASK_PASS_FRAME);
  Event_Start();
  WORLD_MAP_QUEUE_TRANSITION_PICTURE(buffer, 0x3a80);
  Actor_Get(gMainRoutineWork.leaderActorId)->spriteStorageKind = 0;
  gFieldWork->transitionFrames = 16;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_GLITTER);

  gWorldMapBlend = BLEND_ALPHA(0, 14);
  WorldMap_ShowTransitionPicture(buffer, 0x3480, 2);
  gWorldMapBlend = BLEND_ALPHA(0, 13);
  WorldMap_ShowTransitionPicture(buffer, 0x2e80, 2);
  gWorldMapBlend = BLEND_ALPHA(0, 12);
  WorldMap_ShowTransitionPicture(buffer, 0x2880, 2);
  gWorldMapBlend = BLEND_ALPHA(0, 11);
  WorldMap_ShowTransitionPicture(buffer, 0x2280, 2);
  gWorldMapBlend = BLEND_ALPHA(0, 10);
  WorldMap_ShowTransitionPicture(buffer, 0x1c80, 2);
  gWorldMapBlend = BLEND_ALPHA(0, 9);
  WorldMap_ShowTransitionPicture(buffer, 0x1680, 2);
  gWorldMapBlend = BLEND_ALPHA(0, 8);
  WorldMap_ShowTransitionPicture(buffer, 0x1080, 140);

  WorldMap_ShowTransitionPicture(buffer, 0x1680, 4);
  WorldMap_ShowTransitionPicture(buffer, 0x1c80, 4);
  WorldMap_ShowTransitionPicture(buffer, 0x2280, 4);
  gWorldMapBlend = BLEND_ALPHA(0, 9);
  WorldMap_ShowTransitionPicture(buffer, 0x2880, 4);
  gWorldMapBlend = BLEND_ALPHA(0, 10);
  WorldMap_ShowTransitionPicture(buffer, 0x2e80, 4);
  gWorldMapBlend = BLEND_ALPHA(0, 11);
  WorldMap_ShowTransitionPicture(buffer, 0x3480, 4);
  gWorldMapBlend = BLEND_ALPHA(0, 12);
  WORLD_MAP_QUEUE_TRANSITION_PICTURE(buffer, 0x3a80);
  WorldMap_LoadAreaGraphics();
  Task_AddFrame(WorldMap_JitterTransitionCamera, TASK_PASS_FRAME);
  Sound_Play(SE_RUMBLE);
  gWorldMapBlend = BLEND_ALPHA(0, 13);
  Event_Wait(4);
  gWorldMapBlend = BLEND_ALPHA(0, 14);
  Event_Wait(4);
  gWorldMapBlend = BLEND_ALPHA(0, 15);
  Event_Wait(4);
  gWorldMapBlend = BLEND_ALPHA(0, 16);
  Event_Wait(45);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Heap_Release(buffer);
  GameFlag_Set(FLAG_SKIP_SCENE_CHANGE_SOUND);
}

/// An actor's update routine that blinks it and now and then creates its
/// attached effects.
UNUSED static void WorldMap_UpdateBlinkingLinkedEffects(struct Actor *actor) {
  if ((gFrameCount & 2) != 0) {
    Actor_SetColorMode(actor, SPRITE_COLOR_WHITE);
  } else {
    Actor_SetColorMode(actor, SPRITE_COLOR_NORMAL);
  }
  if ((gFrameCount & 15) == 0) {
    WorldMap_CreateAttachedEffects(actor);
  }
}

/// An actor's update routine that cycles its palettes and now and then
/// creates its attached effects.
UNUSED static void WorldMap_UpdateCyclingLinkedEffects(struct Actor *actor) {
  if ((gFrameCount & 1) != 0) {
    Actor_SetColorMode(actor, ((u32)((gFrameCount >> 1)) % (u32)6));
  }
  if ((gFrameCount & 15) == 0) {
    WorldMap_CreateAttachedEffects(actor);
  }
}

#include "shared/color_mode_cycle.h"

#include "shared/field_effect_vertical.h"
#include "shared/world_map_attached_effects.h"

/// Hammet's caravan outside Vault (WorldMap_ShowHammetsCaravan): each of
/// the four wagons, at one and a half times its size, rolls up to the party
/// and stops in its own place.
s32 gCaravanFirstWagonArrives[] = {
    SC_POSITION(5848, 0, 1576),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(5848, 0, 1544),
    SC_SYNC,
    SC_MOVE(5795, 0, 1544),
    SC_SYNC,
    SC_MOVE(5800, 0, 1614),
    SC_SYNC,
    SC_MOVE(5864, 0, 1624),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/// Each wagon rolls south for the bridge and, out of sight, is put down where
/// it waits at the broken bridge.
s32 gCaravanFirstWagonDeparts[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(5868, 0, 1624),
    SC_SYNC,
    SC_MOVE(5868, 0, 1736),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(5656, 0, 1800),
    SC_END,
};

/// Each wagon rolls back from the broken bridge and leaves the stage.
s32 gCaravanFirstWagonReturns[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(5800, 0, 1800),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

/// Every wagon in turn: from the road south of Vault it rolls north for Lunpa
/// and leaves the stage.
s32 gCaravanWagonHeadsNorth[] = {
    SC_POSITION(5868, 0, 1704),
    SC_WAIT(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(5872, 0, 1528),
    SC_SYNC,
    SC_MOVE(5880, 0, 1352),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gCaravanSecondWagonArrives[] = {
    SC_POSITION(5848, 0, 1576),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(5848, 0, 1544),
    SC_SYNC,
    SC_MOVE(5795, 0, 1544),
    SC_SYNC,
    SC_MOVE(5800, 0, 1630),
    SC_SYNC,
    SC_MOVE(5832, 0, 1640),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gCaravanSecondWagonDeparts[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(5863, 0, 1640),
    SC_SYNC,
    SC_MOVE(5868, 0, 1736),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(5640, 0, 1768),
    SC_END,
};

s32 gCaravanSecondWagonReturns[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(5640, 0, 1795),
    SC_SYNC,
    SC_MOVE(5800, 0, 1800),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gCaravanThirdWagonArrives[] = {
    SC_POSITION(5848, 0, 1576),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(5848, 0, 1544),
    SC_SYNC,
    SC_MOVE(5795, 0, 1544),
    SC_SYNC,
    SC_MOVE(5800, 0, 1614),
    SC_SYNC,
    SC_MOVE(5832, 0, 1624),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gCaravanThirdWagonDeparts[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(5863, 0, 1624),
    SC_SYNC,
    SC_MOVE(5868, 0, 1736),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(5656, 0, 1768),
    SC_END,
};

s32 gCaravanThirdWagonReturns[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(5656, 0, 1795),
    SC_SYNC,
    SC_MOVE(5800, 0, 1800),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gCaravanFourthWagonArrives[] = {
    SC_POSITION(5848, 0, 1576),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(5848, 0, 1544),
    SC_SYNC,
    SC_MOVE(5795, 0, 1544),
    SC_SYNC,
    SC_MOVE(5800, 0, 1598),
    SC_SYNC,
    SC_MOVE(5832, 0, 1608),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gCaravanFourthWagonDeparts[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(5863, 0, 1608),
    SC_SYNC,
    SC_MOVE(5868, 0, 1736),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(5672, 0, 1768),
    SC_END,
};

s32 gCaravanFourthWagonReturns[] = {
    SC_ANIMATION(ANIM_NOD),
    SC_MOVE(5672, 0, 1795),
    SC_SYNC,
    SC_MOVE(5800, 0, 1800),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

/// The invisible guide of the rock that falls beside the caravan: it grows
/// from a tenth of its size over 160 frames as it moves to where the rock
/// lands.
s32 gCaravanRockGuideScript[] = {
    SC_SET(SW_SCALE_X, Q16_ONE / 10),
    SC_SET(SW_SCALE_Y, Q16_ONE / 10),
    SC_WAIT(1),
    SC_MOVE(5608, 0, 1720),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 655),
    SC_ADD(SW_SCALE_Y, 655),
    SC_WAIT(1),
    SC_LOOP(160, SC_TO(1)),
    SC_END,
};

/// The leader's walk in WorldMap_RunBlackOrbEvent: SW_TIMER turns 1 when he
/// has stopped.
s32 gBlackOrbLeaderScript[] = {
    SC_SET(SW_TIMER, 0),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(6008, 0, 3400),
    SC_SYNC,
    SC_MOVE(6020, 0, 3400),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_TIMER, 1),
    SC_WAIT(1),
    SC_END,
};

/// The walks of the ending (WorldMap_ShowEndingEvent): Alex, Kraden and Jenna
/// follow one path and stop side by side; Jenna's turns SW_TIMER to 1 when
/// she has stopped.
s32 gEndingAlexWalk[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(7564, 0, 3476),
    SC_SYNC,
    SC_MOVE(7576, 0, 3464),
    SC_SYNC,
    SC_MOVE(7640, 0, 3464),
    SC_SYNC,
    SC_MOVE(7656, 0, 3480),
    SC_SYNC,
    SC_MOVE(7720, 0, 3480),
    SC_SYNC,
    SC_MOVE(7752, 0, 3496),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(1),
    SC_END,
};

s32 gEndingKradenWalk[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(7564, 0, 3476),
    SC_SYNC,
    SC_MOVE(7576, 0, 3464),
    SC_SYNC,
    SC_MOVE(7640, 0, 3464),
    SC_SYNC,
    SC_MOVE(7656, 0, 3480),
    SC_SYNC,
    SC_MOVE(7704, 0, 3512),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

s32 gEndingJennaWalk[] = {
    SC_SET(SW_TIMER, 0),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(7564, 0, 3476),
    SC_SYNC,
    SC_MOVE(7576, 0, 3464),
    SC_SYNC,
    SC_MOVE(7640, 0, 3464),
    SC_SYNC,
    SC_MOVE(7656, 0, 3480),
    SC_SYNC,
    SC_MOVE(7688, 0, 3528),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_EAST),
    SC_SET(SW_TIMER, 1),
    SC_WAIT(1),
    SC_END,
};

/// A puff of dust in the lighthouse quake (WorldMap_UpdateLighthouseQuake):
/// it lasts 16 frames.
s32 gQuakeDustScript[] = {
    SC_WAIT(16),
    SC_RELEASE,
};

/// The ship crossing the sea (WorldMap_SailToFirstMonsterAttack,
/// WorldMap_SailUntilLandSighted): 8 pixels west a step for 40 steps, then
/// SW_TIMER turns 1 while it sails on two more.
s32 gCrossingShipSailsWest[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_LOOP(40, SC_TOP),
    SC_SET(SW_TIMER, 1),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_END,
};

/// As gCrossingShipSailsWest, drifting 2 pixels north with each step.
s32 gCrossingShipVeersNorth[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(-8, 0, -2),
    SC_SYNC,
    SC_LOOP(40, SC_TOP),
    SC_SET(SW_TIMER, 1),
    SC_MOVE_BY(-8, 0, -2),
    SC_SYNC,
    SC_MOVE_BY(-8, 0, -2),
    SC_SYNC,
    SC_END,
};

/// The island drifting east (WorldMap_ShowIslandDrifting): 24 pixels a step
/// for 10 steps, then SW_TIMER turns 1 while it drifts on two more.
s32 gIslandDriftsEast[] = {
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

/// Flint's actor script on the map: it sets him up
/// (WorldMap_SetUpFlint).
s32 gWorldMapFlintScript[] = {
    SC_CALL(WorldMap_SetUpFlint),
    SC_END,
};

struct SceneEntrance gWorldSceneEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 5656, 0, 1256, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {WORLD_MAP_FROM_VALE_PLAZA, CONDITION_ALWAYS, 5680, 0, 1226, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_TITLE, CONDITION_ALWAYS, 5688, 0, 1224, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {WORLD_MAP_FROM_VAULT_SOUTH, CONDITION_ALWAYS, 5848, 0, 1608, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_GOMA_CAVE_ENTRANCE, CONDITION_ALWAYS, 6424, 0, 1320,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_BILIBIN, CONDITION_ALWAYS, 6584, 0, 1224, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_BILIBIN_BARRICADE_EAST, CONDITION_ALWAYS, 6928, 0, 1368,
     FACING_WEST, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_KOLIMA_WEST, CONDITION_ALWAYS, 7328, 0, 1240, FACING_WEST,
     0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_KOLIMA_FOREST_ENTRANCE, CONDITION_ALWAYS, 7136, 0, 1054,
     FACING_WEST, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_IMIL_SOUTH, CONDITION_ALWAYS, 5928, 0, 296, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST, CONDITION_ALWAYS, 6024, 0,
     200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_KOLIMA_BRIDGE_EAST, CONDITION_ALWAYS, 7568, 0, 1416,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_FUCHIN_TEMPLE_GROUNDS, CONDITION_ALWAYS, 7880, 0, 1592,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_MOGALL_FOREST_SIGN, CONDITION_ALWAYS, 7852, 0, 1688,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_XIAN, CONDITION_ALWAYS, 7544, 0, 2008, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {WORLD_MAP_FROM_ALPINE_CROSSING_EAST, CONDITION_ALWAYS, 7144, 0, 1736,
     FACING_EAST, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_ALTIN, CONDITION_ALWAYS, 7008, 0, 1624, FACING_EAST, 0, -1,
     -1, -1, -1, 0},
    {WORLD_MAP_FROM_LAMA_TEMPLE, CONDITION_ALWAYS, 6888, 0, 1736, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_LAMAKAN_DESERT_EAST, CONDITION_ALWAYS, 6888, 0, 1828,
     FACING_EAST, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_KALAY_WEST, CONDITION_ALWAYS, 5560, 0, 2136, FACING_WEST, 0,
     -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_KALAY_EAST, CONDITION_ALWAYS, 5592, 0, 2168, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_KALAY_DOCKS_EAST, CONDITION_ALWAYS, 5424, 0, 2264,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_TOLBI_DOCKS, CONDITION_ALWAYS, 5004, 0, 2328, FACING_WEST,
     0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_TOLBI, CONDITION_ALWAYS, 4904, 0, 2312, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {WORLD_MAP_FROM_ALTMILLER_CAVE_ENTRANCE, CONDITION_ALWAYS, 4840, 0, 1992,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_GONDOWAN_PASSAGE_EAST, CONDITION_ALWAYS, 4940, 0, 2648,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_GONDOWAN_CAVE_NORTH, CONDITION_ALWAYS, 5656, 0, 2424,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_SUHALLA, CONDITION_ALWAYS, 5388, 0, 2840, FACING_WEST, 0,
     -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_SUHALLA_DESERT_NORTH, CONDITION_ALWAYS, 5448, 0, 2920,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_SUHALLA_GATE_CHECKPOINT_SOUTH, CONDITION_ALWAYS, 5864, 0,
     3400, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_LALIVERO_EAST, CONDITION_ALWAYS, 5996, 0, 2930,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_ROAD_TO_BABI_LIGHTHOUSE, CONDITION_ALWAYS, 5996, 0, 2840,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_VENUS_LIGHTHOUSE_ENTRY_EAST, CONDITION_ALWAYS, 6032, 0,
     3192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_GATEWAY_CAVE, CONDITION_ALWAYS, 5976, 0, 3432, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_LUNPA, CONDITION_ALWAYS, 5896, 0, 1160, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {WORLD_MAP_FROM_BILIBIN_CAVE_ENTRANCE, CONDITION_ALWAYS, 5990, 0, 804,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_KOLIMA_EAST, CONDITION_ALWAYS, 7376, 0, 1240, FACING_EAST,
     0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_IMIL_NORTH, CONDITION_ALWAYS, 5956, 0, 266, FACING_EAST, 0,
     -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_GOMA_CAVE_BRIDGE, CONDITION_ALWAYS, 6484, 0, 1212,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_GONDOWAN_CAVE_SOUTH, CONDITION_ALWAYS, 5480, 0, 2632,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_SUHALLA_GATE_CHECKPOINT_EAST, CONDITION_ALWAYS, 5928, 0,
     3272, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_MOGALL_FOREST_KILLER_APE, CONDITION_ALWAYS, 7792, 0, 2104,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_VAULT_NORTH, CONDITION_ALWAYS, 5848, 0, 1544, FACING_NORTH,
     0, -1, -1, -1, -1, 0},
    // Unused: no exit leads to entrance 45, 97 or 98.
    {45, CONDITION_ALWAYS, 6248, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_KOLIMA_BRIDGE_WEST, CONDITION_ALWAYS, 7568, 0, 1352,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_BILIBIN_BARRICADE_WEST, CONDITION_ALWAYS, 6976, 0, 1360,
     FACING_EAST, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_BILIBIN_CAVE_ICE, CONDITION_ALWAYS, 6228, 0, 552,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_WEST_LUNPA_CAVE, CONDITION_ALWAYS, 5860, 0, 1212,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_ALPINE_CROSSING_NORTH, CONDITION_ALWAYS, 7056, 0, 1656,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_ALPINE_CROSSING_WEST, CONDITION_ALWAYS, 7000, 0, 1736,
     FACING_WEST, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_ALTIN_PEAK_WEST_EXIT, CONDITION_ALWAYS, 6920, 0, 1696,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_LAMAKAN_DESERT_CAVE, CONDITION_ALWAYS, 6090, 0, 2232,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_GONDOWAN_PASSAGE_WEST, CONDITION_ALWAYS, 4940, 0, 2728,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_SUHALLA_DESERT_SOUTH, CONDITION_ALWAYS, 5416, 0, 3272,
     FACING_EAST, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_SUHALLA_NORTH, CONDITION_ALWAYS, 5416, 0, 2808,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_SUHALLA_EAST, CONDITION_ALWAYS, 5444, 0, 2840, FACING_EAST,
     0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_KALAY_DOCKS_WEST, CONDITION_ALWAYS, 5424, 0, 2200,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_LALIVERO_WEST, CONDITION_ALWAYS, 5996, 0, 2856,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_NORTH, CONDITION_ALWAYS, 5480, 0,
     2120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_VENUS_LIGHTHOUSE_ENTRY_WEST, CONDITION_ALWAYS, 6152, 0,
     3212, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_HAMMETS_CARAVAN_LEAVES, CONDITION_ALWAYS, 5848, 0, 1608,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_FIRST_ATTACK, CONDITION_ALWAYS, 5384, 0,
     2248, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_SECOND_ATTACK, CONDITION_ALWAYS, 7944, 0,
     200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_THIRD_ATTACK, CONDITION_ALWAYS, 7944, 0,
     200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_KRAKEN, CONDITION_ALWAYS, 7944, 0, 200,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_SHIP_ROOMS_SAIL_TO_LAND, CONDITION_ALWAYS, 7944, 0, 200,
     FACING_SOUTH, 0, -1, 128, -1, -1, 0},
    {WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI, CONDITION_ALWAYS, 5096, 0,
     2328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH, CONDITION_ALWAYS, 5528, 0,
     2120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_FILE_SCREEN, CONDITION_ALWAYS, 7464, 0, 200, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {WORLD_MAP_BEFORE_SHEBAS_FALL, CONDITION_ALWAYS, 6088, 0, 3176, FACING_WEST,
     0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_TOLBI_SHIP_DECK_CROSSBONE_ISLE, CONDITION_ALWAYS, 4728, 0,
     424, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_SHIP_ROOMS, CONDITION_ALWAYS, 4872, 0, 808, FACING_WEST, 0,
     -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_CROSSBONE_ISLE, CONDITION_ALWAYS, 4576, 0, 508,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_SUHALLA_DESERT_SANDSTORM, CONDITION_ALWAYS, 4392, 0, 328,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_AFTER_BLACK_ORB_DEFEAT, CONDITION_ALWAYS, 8, 0, 8, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {WORLD_MAP_JENNA_AND_KRADEN_ADRIFT, CONDITION_ALWAYS, 7552, 0, 3504,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT, CONDITION_ALWAYS, 6024, 0, 160,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT, CONDITION_ALWAYS, 6088, 0, 3168,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    // Unused.
    {97, CONDITION_ALWAYS, 6248, 0, 872, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    // Unused.
    {98, CONDITION_ALWAYS, 4248, 0, 2424, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_DEBUG, CONDITION_ALWAYS, 5576, 0, 2088, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gWorldMapExits[] = {
    SCENE_EXITS(WorldMap),
    SCENE_EXIT(WORLD_MAP_TO_VALE_PLAZA, ValePlaza, VALE_PLAZA_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_WORLD_MAP_FROM_TITLE, WorldMap,
               WORLD_MAP_FROM_TITLE),
    SCENE_EXIT(WORLD_MAP_TO_VAULT_SOUTH, Vault, VAULT_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_GOMA_CAVE_ENTRANCE, GomaCaveEntrance,
               GOMA_CAVE_ENTRANCE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_BILIBIN, Bilibin, BILIBIN_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_BILIBIN_BARRICADE_WEST, BilibinBarricade,
               BILIBIN_BARRICADE_FROM_WORLD_MAP_WEST),
    SCENE_EXIT(WORLD_MAP_TO_KOLIMA_WEST, Kolima, KOLIMA_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_KOLIMA_FOREST_ENTRANCE, KolimaForestEntrance,
               KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_IMIL_SOUTH, Imil, IMIL_FROM_HOUSE_SOUTHWEST),
    SCENE_EXIT(WORLD_MAP_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST,
               MercuryLighthouseExterior,
               MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_KOLIMA_BRIDGE_EAST, KolimaBridge,
               KOLIMA_BRIDGE_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_FUCHIN_TEMPLE_GROUNDS, FuchinTempleGrounds,
               FUCHIN_TEMPLE_GROUNDS_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_MOGALL_FOREST_SIGN, MogallForestSign,
               MOGALL_FOREST_SIGN_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_XIAN, Xian, XIAN_FROM_HOUSE),
    SCENE_EXIT(WORLD_MAP_TO_ALPINE_CROSSING_EAST, AlpineCrossing,
               ALPINE_CROSSING_FROM_WORLD_MAP_EAST),
    SCENE_EXIT(WORLD_MAP_TO_ALTIN, Altin, ALTIN_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_LAMA_TEMPLE, LamaTemple,
               LAMA_TEMPLE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_LAMAKAN_DESERT_EAST, LamakanDesertEast,
               LAMAKAN_DESERT_EAST_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_WORLD_MAP_FROM_KALAY_WEST, WorldMap,
               WORLD_MAP_FROM_KALAY_WEST),
    SCENE_EXIT(WORLD_MAP_TO_KALAY_EAST, Kalay, KALAY_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_KALAY_DOCKS_EAST, KalayDocks,
               KALAY_DOCKS_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_TOLBI_DOCKS, TolbiDocks,
               TOLBI_DOCKS_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_TOLBI, Tolbi, TOLBI_FROM_HOUSE_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_ALTMILLER_CAVE_ENTRANCE, AltmillerCaveEntrance,
               ALTMILLER_CAVE_ENTRANCE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_PASSAGE_EAST, GondowanPassage,
               GONDOWAN_PASSAGE_FROM_WORLD_MAP_NORTH),
    SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_CAVE_NORTH, GondowanCave,
               GONDOWAN_CAVE_FROM_WORLD_MAP_EAST),
    SCENE_EXIT(WORLD_MAP_TO_SUHALLA, Suhalla, SUHALLA_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_SUHALLA_DESERT_NORTH, SuhallaDesertNorth,
               SUHALLA_DESERT_NORTH_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_SOUTH,
               SuhallaGateCheckpoint,
               SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_LALIVERO_EAST, Lalivero,
               LALIVERO_FROM_WORLD_MAP_EAST),
    SCENE_EXIT(WORLD_MAP_TO_ROAD_TO_BABI_LIGHTHOUSE, RoadToBabiLighthouse,
               ROAD_TO_BABI_LIGHTHOUSE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_WEST, VenusLighthouseEntry,
               VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_WEST),
    SCENE_EXIT(WORLD_MAP_TO_GATEWAY_CAVE, GatewayCave,
               GATEWAY_CAVE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_LUNPA, Lunpa, LUNPA_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_BILIBIN_CAVE_ENTRANCE, BilibinCaveEntrance,
               BILIBIN_CAVE_ENTRANCE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_GOMA_CAVE_BRIDGE, GomaCaveBridge,
               GOMA_CAVE_BRIDGE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_CAVE_SOUTH, GondowanCave,
               GONDOWAN_CAVE_FROM_WORLD_MAP_WEST),
    SCENE_EXIT(WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_EAST, SuhallaGateCheckpoint,
               SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_EAST),
    SCENE_EXIT(WORLD_MAP_TO_MOGALL_FOREST_KILLER_APE, MogallForestKillerApe,
               MOGALL_FOREST_KILLER_APE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_VAULT_NORTH, Vault, VAULT_FROM_WORLD_MAP_NORTH),
    SCENE_EXIT(WORLD_MAP_TO_BILIBIN_CAVE_ENTRANCE_NORTHEAST,
               BilibinCaveEntrance, BILIBIN_CAVE_ENTRANCE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_KOLIMA_BRIDGE_WEST, KolimaBridge,
               KOLIMA_BRIDGE_FROM_WORLD_MAP_NORTH),
    SCENE_EXIT(WORLD_MAP_TO_BILIBIN_BARRICADE_EAST, BilibinBarricade,
               BILIBIN_BARRICADE_FROM_WORLD_MAP_EAST),
    SCENE_EXIT(WORLD_MAP_TO_BILIBIN_CAVE_ICE, BilibinCaveIce,
               BILIBIN_CAVE_ICE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_WEST_LUNPA_CAVE, WestLunpaCave,
               WEST_LUNPA_CAVE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_ALPINE_CROSSING_NORTH, AlpineCrossing,
               ALPINE_CROSSING_FROM_WORLD_MAP_NORTH),
    SCENE_EXIT(WORLD_MAP_TO_ALPINE_CROSSING_WEST, AlpineCrossing,
               ALPINE_CROSSING_FROM_WORLD_MAP_WEST),
    SCENE_EXIT(WORLD_MAP_TO_ALTIN_PEAK_WEST_EXIT, AltinPeakWestExit,
               ALTIN_PEAK_WEST_EXIT_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_LAMAKAN_DESERT_CAVE, LamakanDesertCave,
               LAMAKAN_DESERT_CAVE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_GONDOWAN_PASSAGE_WEST, GondowanPassage,
               GONDOWAN_PASSAGE_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_SUHALLA_DESERT_SOUTH, SuhallaDesertSouth,
               SUHALLA_DESERT_SOUTH_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_KALAY_DOCKS_NORTH, KalayDocks,
               KALAY_DOCKS_FROM_WORLD_MAP_NORTH),
    SCENE_EXIT(WORLD_MAP_TO_LALIVERO_WEST, Lalivero,
               LALIVERO_FROM_WORLD_MAP_WEST),
    SCENE_EXIT(WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE, KalayTunnelEntrance,
               KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_NORTH),
    SCENE_EXIT(WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_EAST, VenusLighthouseEntry,
               VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_EAST),
    SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_DECK_CROSSBONE_ISLE, TolbiShipDeck,
               TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS),
    SCENE_EXIT(WORLD_MAP_TO_CROSSBONE_ISLE, CrossboneIsle,
               CROSSBONE_ISLE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_WORLD_MAP_FROM_VALE_PLAZA, WorldMap,
               WORLD_MAP_FROM_VALE_PLAZA),
    SCENE_EXIT(WORLD_MAP_TO_HAMMETS_CARAVAN_LEAVES, WorldMap,
               WORLD_MAP_HAMMETS_CARAVAN_LEAVES),
    SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_MAST_FIRST_ATTACK, TolbiShipMast,
               TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK),
    SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_MAST_SECOND_ATTACK, TolbiShipMast,
               TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK),
    SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_MAST_THIRD_ATTACK, TolbiShipMast,
               TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK),
    SCENE_EXIT(WORLD_MAP_TO_SHIP_ROOMS, ShipRooms, SHIP_ROOMS_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_MAST_LAND_HO, TolbiShipMast,
               TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO),
    SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
               TOLBI_SHIP_DECK_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE_SOUTH, KalayTunnelEntrance,
               KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE, VenusLighthouseAerie,
               VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_SHIP_ROOMS_ROWING_DECK_ISLAND, ShipRooms,
               SHIP_ROOMS_ROWING_DECK_FROM_WORLD_MAP_ISLAND),
    SCENE_EXIT(WORLD_MAP_TO_JENNA_AND_KRADEN_ADRIFT, WorldMap,
               WORLD_MAP_JENNA_AND_KRADEN_ADRIFT),
    SCENE_EXIT(WORLD_MAP_TO_ROCK_ROOMS, RockRooms,
               ROCK_ROOMS_FROM_SUHALLA_GATE_CHECKPOINT),
    SCENE_EXITS_END,
};

s32 gWorldMapKolimaBridgeScript[] = {
    SC_CALL(WorldMap_SetUpKolimaBridge),
    SC_END,
};

s32 gWorldMapAlignToMapScript[] = {
    SC_CALL(WorldMap_AlignToMap),
    SC_END,
};

s32 gWorldMapIodemScript[] = {
    SC_CALL(WorldMap_SetUpIodem),
    SC_END,
};

s32 gWorldMapStormScript[] = {
    SC_CALL(WorldMap_SetUpStorm),
    SC_CALL(WorldMap_BlinkActorSprite),
    SC_END,
};

// clang-format off
struct ActorPlacement gWorldMapActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_VINE), (s32)gWorldMapFlintScript,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_CORONA), MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_HAIL), MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5678, 0, 1200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5848, 0, 1576, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        6976, 0, 1624, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5416, 0, 2840, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5892, 0, 1128, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5422, 0, 2232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5036, 0, 2328, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_SNOWY_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5928, 0, 270, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_STUMP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        7352, 0, 1240, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_BLUE_ROOFED_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        7544, 0, 1976, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TEMPLE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        6888, 0, 1712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TEMPLE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        7880, 0, 1560, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        6584, 0, 1192, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5592, 0, 2136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5996, 0, 2894, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        4904, 0, 2280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_MERCURY_LIGHTHOUSE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        6024, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_MOUNTAIN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5656, 0, 1112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        6088, 0, 3176, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_BARRICADE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        6952, 0, 1362, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_BABI_LIGHTHOUSE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5996, 0, 2808, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        6424, 0, 1302, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        6484, 0, 1184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        4840, 0, 1960, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5656, 0, 2392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5480, 0, 2600, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_FOREST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        7790, 0, 2080, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5976, 0, 3400, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5988, 0, 774, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        6224, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5856, 0, 1174, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        6920, 0, 1664, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        6092, 0, 2200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_FOREST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        7166, 0, 1054, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_FOREST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        7856, 0, 1736, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5480, 0, 2088, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_BRIDGE, CONDITION_ALWAYS, (s32)gWorldMapKolimaBridgeScript,
        7568, 0, 1384, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_GATEHOUSE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        4942, 0, 2690, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHIPWRECK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        6036, 0, 3458, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHIP, IF_FLAG_SET(FLAG_TOLBI_SHIP_AT_ISLAND), (s32)gWorldMapAlignToMapScript,
        4760, 0, 440, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_WORLD_MAP_STORM_GONE, (s32)gWorldMapStormScript,
        4392, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, (s32)gWorldMapIodemScript,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gWorldMapActorPlacementsHammetsCaravan[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5848, 0, 1576, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gWorldMapActorPlacementsFromWestLunpaCave[] = {
    ACTOR_PLACEMENT(SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5892, 0, 1128, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5856, 0, 1174, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BUNZA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gWorldMapActorPlacementsCrossingEnds[] = {
    ACTOR_PLACEMENT(SPRITE_SHIP, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5384, 0, 2248, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5592, 0, 2136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5422, 0, 2232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5028, 0, 2328, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

struct ActorPlacement gWorldMapActorPlacementsCrossing[] = {
    ACTOR_PLACEMENT(SPRITE_SHIP, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5432, 0, 2312, FACING_WEST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gWorldMapActorPlacementsFromKalayTunnelEntranceSouth[] = {
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5592, 0, 2136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        5480, 0, 2088, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gWorldMapActorPlacementsJennaAndKradenAdrift[] = {
    ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        7560, 0, 3512, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(ACTOR_SHEBA, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        7816, 0, 3528, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        7544, 0, 3496, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        7708, 0, 3456, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_FELIX, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        7800, 0, 3544, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SHIPWRECK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        7604, 0, 3522, FACING_WEST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gWorldMapActorPlacementsFromClear[] = {
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_ISLAND, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        7464, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gWorldMapActorPlacementsBeforeShebasFall[] = {
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        6080, 0, 3168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct Trigger gWorldTriggers[] = {
    { TRIGGER_STEP, CELL_BEHAVIOR_WORLD_MAP_FLINT, FLAG_DJINNI(DJINNI_FLINT), EVENT(WorldMap_MeetFlint) },
    { TRIGGER_STEP, CELL_BEHAVIOR_WORLD_MAP_STORM, FLAG_WORLD_MAP_STORM_GONE, EVENT(WorldMap_WalkIntoStorm) },
    { TRIGGER_TALK, ACTOR_VENUS_DJINNI, CONDITION_ALWAYS, EVENT(WorldMap_MeetVine) },
    { TRIGGER_TALK, ACTOR_CORONA, CONDITION_ALWAYS, EVENT(WorldMap_MeetCorona) },
    { TRIGGER_TALK, ACTOR_HAIL, CONDITION_ALWAYS, EVENT(WorldMap_MeetHail) },
    { TRIGGER_STEP, CELL_BEHAVIOR_WORLD_MAP_VINE, FLAG_DJINNI(DJINNI_VINE), EVENT(WorldMap_MeetVine) },
    { TRIGGER_STEP, CELL_BEHAVIOR_WORLD_MAP_CORONA, FLAG_DJINNI(DJINNI_CORONA), EVENT(WorldMap_MeetCorona) },
    { TRIGGER_STEP, CELL_BEHAVIOR_WORLD_MAP_HAIL, FLAG_DJINNI(DJINNI_HAIL), EVENT(WorldMap_MeetHail) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_WORLD_MAP_SHIPWRECK, CONDITION_ALWAYS, EVENT(WorldMap_SearchShipwreck) },
    { USE_ITEM(ITEM_BLACK_ORB), CELL_BEHAVIOR_WORLD_MAP_SHIPWRECK, FLAG_BLACK_ORB_RETURNED, EVENT(WorldMap_UseBlackOrb) },
    { EXAMINE_TRIGGER(EXAMINE_GROUND), CELL_BEHAVIOR_WORLD_MAP_TURTLE_BOOTS, FLAG_WORLD_MAP_TURTLE_BOOTS, FIND_ITEM(ITEM_TURTLE_BOOTS) },
    { TRIGGER_TALK, ACTOR_IODEM, CONDITION_ALWAYS, EVENT(WorldMap_TalkAboutBlackOrb) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_IODEM, CONDITION_ALWAYS, EVENT(WorldMap_TalkAboutBlackOrb) },
    { TRIGGER_EXIT, WORLD_MAP_TO_MOGALL_FOREST_SIGN, CONDITION_ALWAYS, WORLD_MAP_TO_MOGALL_FOREST_SIGN },
    { TRIGGER_EXIT, WORLD_MAP_TO_ALPINE_CROSSING_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_ALPINE_CROSSING_EAST },
    { TRIGGER_EXIT, WORLD_MAP_TO_ALPINE_CROSSING_NORTH, CONDITION_ALWAYS, WORLD_MAP_TO_ALPINE_CROSSING_NORTH },
    { TRIGGER_EXIT, WORLD_MAP_TO_ALPINE_CROSSING_WEST, CONDITION_ALWAYS, WORLD_MAP_TO_ALPINE_CROSSING_WEST },
    { TRIGGER_EXIT, WORLD_MAP_TO_LAMAKAN_DESERT_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_LAMAKAN_DESERT_EAST },
    { TRIGGER_EXIT, WORLD_MAP_TO_SUHALLA_DESERT_NORTH, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_DESERT_NORTH },
    { TRIGGER_EXIT, WORLD_MAP_TO_SUHALLA_DESERT_SOUTH, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_DESERT_SOUTH },
    { TRIGGER_EXIT, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_SOUTH, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_SOUTH },
    { TRIGGER_EXIT, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_EAST },
    { TRIGGER_EXIT, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_WEST, CONDITION_ALWAYS, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_WEST },
    { TRIGGER_EXIT, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_EAST },
    { TRIGGER_EXIT, WORLD_MAP_TO_BILIBIN_BARRICADE_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN_BARRICADE_EAST },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_VALE_PLAZA), CONDITION_ALWAYS, WORLD_MAP_TO_VALE_PLAZA },
    { TRIGGER_STEP, STEP_TRIGGER_AREA(ACTOR_TO_VAULT), CONDITION_ALWAYS, EVENT(WorldMap_EnterVault) },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_GOMA_CAVE_ENTRANCE), CONDITION_ALWAYS, WORLD_MAP_TO_GOMA_CAVE_ENTRANCE },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_GOMA_CAVE_BRIDGE), CONDITION_ALWAYS, WORLD_MAP_TO_GOMA_CAVE_BRIDGE },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_BILIBIN), CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN },
    { TRIGGER_STEP, STEP_TRIGGER_AREA(ACTOR_TO_BILIBIN_BARRICADE), CONDITION_ALWAYS, EVENT(WorldMap_EnterBilibinBarricade) },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_KOLIMA), CONDITION_ALWAYS, WORLD_MAP_TO_KOLIMA_WEST },
    { TRIGGER_STEP, STEP_TRIGGER_AREA(ACTOR_TO_KOLIMA_BRIDGE), CONDITION_ALWAYS, EVENT(WorldMap_EnterKolimaBridge) },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_KOLIMA_FOREST), CONDITION_ALWAYS, WORLD_MAP_TO_KOLIMA_FOREST_ENTRANCE },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_BILIBIN_CAVE_ENTRANCE), CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN_CAVE_ENTRANCE },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_BILIBIN_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN_CAVE_ICE },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_IMIL), CONDITION_ALWAYS, WORLD_MAP_TO_IMIL_SOUTH },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_MERCURY_LIGHTHOUSE), CONDITION_ALWAYS, WORLD_MAP_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_FUCHIN_TEMPLE), CONDITION_ALWAYS, WORLD_MAP_TO_FUCHIN_TEMPLE_GROUNDS },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_MOGALL_FOREST_SIGN), CONDITION_ALWAYS, WORLD_MAP_TO_MOGALL_FOREST_SIGN },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_MOGALL_FOREST_KILLER_APE), CONDITION_ALWAYS, WORLD_MAP_TO_MOGALL_FOREST_KILLER_APE },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_XIAN), CONDITION_ALWAYS, WORLD_MAP_TO_XIAN },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_ALTIN), CONDITION_ALWAYS, WORLD_MAP_TO_ALTIN },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_ALTIN_PEAK), CONDITION_ALWAYS, WORLD_MAP_TO_ALTIN_PEAK_WEST_EXIT },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_LAMA_TEMPLE), CONDITION_ALWAYS, WORLD_MAP_TO_LAMA_TEMPLE },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_LAMAKAN_DESERT_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_LAMAKAN_DESERT_CAVE },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_KALAY), CONDITION_ALWAYS, WORLD_MAP_TO_KALAY_EAST },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_LUNPA), CONDITION_ALWAYS, WORLD_MAP_TO_LUNPA },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_WEST_LUNPA_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_WEST_LUNPA_CAVE },
    { TRIGGER_STEP, STEP_TRIGGER_AREA(ACTOR_TO_KALAY_DOCKS), CONDITION_ALWAYS, EVENT(WorldMap_EnterKalayDocks) },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_TOLBI_DOCKS), CONDITION_ALWAYS, WORLD_MAP_TO_TOLBI_DOCKS },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_TOLBI), CONDITION_ALWAYS, WORLD_MAP_TO_TOLBI },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_ALTMILLER_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_ALTMILLER_CAVE_ENTRANCE },
    { TRIGGER_STEP, STEP_TRIGGER_AREA(ACTOR_TO_GONDOWAN_PASSAGE), CONDITION_ALWAYS, EVENT(WorldMap_EnterGondowanPassage) },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_KALAY_TUNNEL_ENTRANCE), CONDITION_ALWAYS, WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_GONDOWAN_CAVE_NORTH), CONDITION_ALWAYS, WORLD_MAP_TO_GONDOWAN_CAVE_NORTH },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_GONDOWAN_CAVE_SOUTH), CONDITION_ALWAYS, WORLD_MAP_TO_GONDOWAN_CAVE_SOUTH },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_SUHALLA), CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_GATEWAY_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_GATEWAY_CAVE },
    { TRIGGER_EXIT, STEP_TRIGGER_AREA(ACTOR_TO_ROAD_TO_BABI_LIGHTHOUSE), CONDITION_ALWAYS, WORLD_MAP_TO_ROAD_TO_BABI_LIGHTHOUSE },
    { TRIGGER_STEP, STEP_TRIGGER_AREA(ACTOR_TO_LALIVERO), CONDITION_ALWAYS, EVENT(WorldMap_EnterLalivero) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_WORLD_MAP_TOLBI_SHIP, IF_FLAG_SET(FLAG_TOLBI_SHIP_AT_ISLAND), EVENT(WorldMap_ReturnToTolbiShip) },
    { TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_WORLD_MAP_CROSSBONE_ISLE, CONDITION_ALWAYS, WORLD_MAP_TO_CROSSBONE_ISLE },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_STORM, IF_FLAG_SET(FLAG_WORLD_MAP_CAUGHT_IN_STORM), EVENT(WorldMap_StartDousingStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_STORM, IF_FLAG_SET(FLAG_WORLD_MAP_CAUGHT_IN_STORM), EVENT(WorldMap_DouseMapStorm) },
    { TRIGGER_RAISED, RAISED_STORM_TIME_UP, CONDITION_ALWAYS, EVENT(WorldMap_StormDropsParty) },
    { TRIGGER_TABLE_END },
};

/// Each passenger's rowing value, the World Map's copy of
/// gTolbiShipMast_PassengerRowValues (tolbi_ship_mast.c).
u32 gWorldMapPassengerRowValues[] = {
    7, 5, 8, 4, 9, 12, 6, 3,
    0,
};
// clang-format on

s32 gIslandWakeScript[] = {
    SC_ADD(SW_X, -px(1) / 10),
    SC_ADD(SW_Z, px(1) / 10),
    SC_ADD(SW_SCALE_X, 7 * Q16_ONE / 128),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 512),
    SC_WAIT(1),
    SC_LOOP(48, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

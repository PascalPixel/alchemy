/// @file
/// The World Map (MapId_WorldMap), the one perspective map (layout/world)
/// that joins the map sections, and the only map of this overlay. Its code does
/// three kinds of work.
///
/// It lets the party into the map sections. Each map section stands on the map
/// as an object event drawn as a picture of its kind (SPRITE_WORLD_MAP_VILLAGE,
/// _CAVE, _DOCK and the rest), turned with the map (WorldMap_AlignToMap).
/// WorldMap_UpdateLocationContacts, run every frame, hides the object events
/// outside a box around the view and raises the COORD_EVENT_MAPSEC of the one
/// the leader touches, which the map event table turns into that map section's
/// warp; a few ways in are cells instead, each carrying the number of its warp.
/// The world map laid flat, with each map section's object event boxed and
/// labelled by its COORD_EVENT_MAPSEC and the box kept visible around a view at
/// Vale, is
/// https://alchemy.pascalpixel.com/places/world-map/place-triggers.png. Five
/// map sections have two ways in, and their scripts choose the warp by the side
/// the leader comes from: the Bilibin Barricade from the west or the east, and
/// Kolima Bridge, Kalay Docks, Gondowan Passage and Lalivero from the north or
/// the south. Kolima Bridge's object event shows the drawbridge let down once
/// FLAG_KOLIMA_BRIDGE_LOWERED is set.
///
/// It holds what the party meets on the map itself. Vine, Corona and Hail, a
/// Venus, a Mars and a Mercury Djinni, stand on the map and, talked to or
/// walked near, fight once a random battle is nearly due. A whirlwind near
/// Crossbone Isle (LOCALID_STORM, put away while the Tolbi-bound Ship lies at
/// anchor off the island) lifts the party for 180 frames when walked into,
/// as the Suhalla Desert's storms do: Douse on it brings the party down into
/// a battle, and when the time runs out it drops the party outside Suhalla
/// (WORLD_MAP_FROM_SUHALLA, FLAG_ARRIVE_FALLING). The Turtle Boots are buried
/// in the ground (MB_WORLD_MAP_TURTLE_BOOTS).
///
/// And it plays the story's moments on the map, each run by
/// WorldMap_Initialize for the warp destination another map sends the party to.
/// In story order:
/// - Leaving Vale (WORLD_MAP_FROM_VALE_GATE) sets FLAG_VALE_PARTY_LEFT the
///   first time.
/// - Flint, the first Djinni, circles his spot until the party walks up to
///   him (EventScript_MeetFlint); he joins, teaches how Djinn are set and put
///   on standby, and a tutorial battle follows.
/// - Hammet's caravan: the first time the party touches Vault
///   (WORLD_MAP_HAMMETS_CARAVAN_LEAVES), the caravan, cut off from Kalay by
///   the destroyed bridge, turns north for Lunpa as the ground shakes and a
///   rock falls beside it; then the party goes on into Vault. It sets
///   FLAG_LUNPA_GUARDS_SUSPECT_KALAY.
/// - Mercury Lighthouse lit (WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT): the
///   lighthouse transition (WorldMap_PlayLighthouseTransition), then Mercury
///   Lighthouse Aerie (MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT).
/// - Bunza's wagon: out of West Lunpa Cave (WORLD_MAP_FROM_WEST_LUNPA_CAVE)
///   Bunza, Hammet and the leader board it and it rolls off
///   (FLAG_BUNZAS_WAGON_LEFT); at the Kalay Tunnel Entrance
///   (WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH) it stops and the leader
///   steps down.
/// - The Tolbi-bound Ship's crossing of the Karagol Sea: between the ship's
///   maps (WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_FIRST_ATTACK to
///   _SAIL_TO_LAND) the map shows the ship sailing west and sends the party
///   back aboard (FLAG_TOLBI_SHIP_ROWING). A rowing off course takes it north
///   to anchor off the island (WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS,
///   FLAG_TOLBI_SHIP_AT_ISLAND), where MB_WORLD_MAP_CROSSBONE_ISLE leads ashore
///   and MB_WORLD_MAP_TOLBI_SHIP back aboard;
///   WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI ends the crossing at Tolbi
///   (FLAG_TOLBI_SHIP_REACHED_TOLBI).
/// - The Black Orb: out of Gateway Cave (WORLD_MAP_FROM_GATEWAY_CAVE) Iodem
///   sees Babi's ship scuttled off the coast and gives Isaac the orb to use
///   on it (FLAG_MAP_TEMP_34, FLAG_SAW_SHIP_AT_IDEJIMA), and calls him back
///   if he walks into the cave again. Used on the wreck
///   (MB_WORLD_MAP_SHIPWRECK) the orb does nothing; Iodem takes it back and the
///   party must go on to Babi Lighthouse (FLAG_BLACK_ORB_RETURNED). A defeat
///   meanwhile returns to WORLD_MAP_AFTER_BLACK_ORB_DEFEAT, which discards the
///   orb and leads on by WORLD_MAP_TO_LUNPA_INTERIORS.
/// - Venus Lighthouse lit (WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT): the
///   lighthouse transition, then back to the Lighthouse Aerie
///   (VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT); after the look back
///   (WORLD_MAP_BEFORE_SHEBAS_FALL) the lighthouse shakes in clouds of dust
///   before Sheba's fall (WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE).
/// - The ending: once the cleared game is saved (menu/main_menu.c,
///   WORLD_MAP_FROM_MAIN_MENU), the island Jenna and Kraden are adrift on
///   drifts east, and WORLD_MAP_JENNA_AND_KRADEN_ADRIFT shows Alex leading them
///   to Felix and Sheba before the title screen returns.
///
/// WORLD_MAP_FROM_DEBUG, the debug menus' warp destination, turns encounters
/// off.
///
/// Open questions:
/// - XXX (no evidence): what flag 0x85c records, set the first time the party
///   leaves Vale; no code in either game reads it.

#include "action_script.h"
#include "version.h"
#include "save_block.h"
#include "heap.h"
#include "vram_cache.h"
#include "unit/import.h"
#include "unit/map/world_map.h"
#include "unit/map/common/world_map_linked_effects.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "heap_slots.h"
#include "types.h"
#include "animspr.h"
#include "script_layer.h"
#include "map_tables.h"
#include "djinn_ids.h"
#include "world_map_render_work.h"
#include "field_service.h"
#include "iwram_call.h"
#include "map_ids.h"
#include "call.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "tbs_edition.h"
#include "inventory.h"
#include "resource.h"
#include "mem.h"
#include "field_effect.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "object_event.h"
#include "frame.h"
#include "fx_work.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "item_ids.h"

static s32 WorldMap_AlignToMap(struct Object *interpreter);
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
static void WorldMap_RunBlackOrbScene(void);
static void WorldMap_BlockGatewayCaveWarp(s32 localId);
static void EventScript_IodemCallsLeaderBack(void);
static void WorldMap_ShowEndingScene(void);
static void WorldMap_ShakeVenusLighthouse(void);
static void WorldMap_UpdateLighthouseQuake(void);
static void WorldMap_SailTolbiShipToIsland(void);
static s32 WorldMap_GetRowingBalance(void);
static s32 WorldMap_GetSeatRowValue(u32 seat);
static void WorldMap_ShowIslandDrifting(void);
static void WorldMap_UpdateStormTimer(void);
static void WorldMap_PlayLighthouseTransition(s32 palette);

/// The object events WorldMap_UpdateLocationContacts looks at, the placed ones
/// to the last slot, and the base of the metatile behaviors a touch raises
/// (COORD_EVENT_MAPSEC).
enum { CONTACT_LAST_OBJ_EVENT = 65, COORD_EVENT_MAPSEC_BASE = 100 };

/// The world map's own object events, numbered in the order of
/// gWorldMapObjectEventTemplates from LOCALID_FIRST_PLACED: the Djinn met on
/// the map, the map sections, then the Tolbi-bound Ship, the storm and Iodem.
/// Each map section is named by the map its warp leads into (gWorldMapWarps),
/// and touching it raises its COORD_EVENT_MAPSEC
/// (WorldMap_UpdateLocationContacts).
enum WorldMapLocalId {
  /// The Venus Djinni: Flint, circling his spot until the party meets him,
  /// and later Vine.
  /// ★ Named from the game: "The Venus Djinni" (MsgVenusDjinnJoined)
  LOCALID_VENUS_DJINNI = LOCALID_FIRST_PLACED,
  /// The Mars Djinni Corona.
  /// ★ Named from the game: "Corona" (MsgDjinnName)
  LOCALID_CORONA,
  /// The Mercury Djinni Hail.
  /// ★ Named from the game: "Hail" (MsgDjinnName)
  LOCALID_HAIL,
  /// Vale, whose warp leads to the town gate (MapId_ValeGate).
  /// ★ Named from the game: "Why's everyone hanging around the town gate?"
  /// (MsgValeWhysEveryoneHanging)
  LOCALID_MAPSEC_VALE_GATE,
  /// ★ Named from its area name: Vault
  LOCALID_MAPSEC_VAULT,
  /// ★ Named from its area name: Altin
  LOCALID_MAPSEC_ALTIN,
  /// ★ Named from its area name: Suhalla
  LOCALID_MAPSEC_SUHALLA,
  /// ★ Named from its area name: Lunpa
  LOCALID_MAPSEC_LUNPA,
  /// ★ Named from its area name: Kalay Docks
  LOCALID_MAPSEC_KALAY_DOCKS,
  /// ★ Named from its area name: Tolbi Docks
  LOCALID_MAPSEC_TOLBI_DOCKS,
  /// ★ Named from its area name: Imil
  LOCALID_MAPSEC_IMIL,
  /// ★ Named from its area name: Kolima
  LOCALID_MAPSEC_KOLIMA,
  /// ★ Named from its area name: Xian
  LOCALID_MAPSEC_XIAN,
  /// ★ Named from its area name: Lama Temple
  LOCALID_MAPSEC_LAMA_TEMPLE,
  /// ★ Named from its area name: Fuchin Temple
  LOCALID_MAPSEC_FUCHIN_TEMPLE,
  /// ★ Named from its area name: Bilibin
  LOCALID_MAPSEC_BILIBIN,
  /// ★ Named from its area name: Kalay
  LOCALID_MAPSEC_KALAY,
  /// ★ Named from its area name: Lalivero
  LOCALID_MAPSEC_LALIVERO,
  /// ★ Named from its area name: Tolbi
  LOCALID_MAPSEC_TOLBI,
  /// ★ Named from its area name: Mercury Lighthouse
  LOCALID_MAPSEC_MERCURY_LIGHTHOUSE,
  /// After two pictures no event answers: a mountain north of Vale
  /// (SPRITE_WORLD_MAP_MOUNTAIN) and Venus Lighthouse, which is entered by
  /// the cells around it.
  /// ★ Named from its area name: Bilibin Barricade
  LOCALID_MAPSEC_BILIBIN_BARRICADE = LOCALID_MAPSEC_MERCURY_LIGHTHOUSE + 3,
  /// ★ Named from its area name: Road to Babi Lighthouse
  LOCALID_MAPSEC_ROAD_TO_BABI_LIGHTHOUSE,
  /// ★ Named from its area name: Goma Cave Entrance
  LOCALID_MAPSEC_GOMA_CAVE_ENTRANCE,
  /// Goma Cave's way out, where a wooden bridge crosses the river
  /// (MapId_GomaCaveBridge).
  /// ★ Named from its area name: Goma Cave
  LOCALID_MAPSEC_GOMA_CAVE_BRIDGE,
  /// ★ Named from its area name: Altmiller Cave
  LOCALID_MAPSEC_ALTMILLER_CAVE,
  /// ★ Named from its area name: Gondowan Cave
  LOCALID_MAPSEC_GONDOWAN_CAVE_NORTH,
  /// ★ Named from its area name: Gondowan Cave
  LOCALID_MAPSEC_GONDOWAN_CAVE_SOUTH,
  /// The way into Mogall Forest where the Killer Ape waits
  /// (MapId_MogallForestKillerApe).
  /// ★ Named from the game: "Killer Ape" (MsgEnemyName)
  LOCALID_MAPSEC_MOGALL_FOREST_KILLER_APE,
  /// ★ Named from its area name: Gateway Cave
  LOCALID_MAPSEC_GATEWAY_CAVE,
  /// ★ Named from its area name: Bilibin Cave Entrance
  LOCALID_MAPSEC_BILIBIN_CAVE_ENTRANCE,
  /// ★ Named from its area name: Bilibin Cave
  LOCALID_MAPSEC_BILIBIN_CAVE,
  /// ★ Named from its area name: West Lunpa Cave
  LOCALID_MAPSEC_WEST_LUNPA_CAVE,
  /// ★ Named from its area name: Altin Peak
  LOCALID_MAPSEC_ALTIN_PEAK,
  /// ★ Named from its area name: Lamakan Desert
  LOCALID_MAPSEC_LAMAKAN_DESERT_CAVE,
  /// ★ Named from its area name: Kolima Forest
  LOCALID_MAPSEC_KOLIMA_FOREST,
  /// The way into Mogall Forest by its broken sign (MapId_MogallForestSign).
  /// ★ Named from the game: "The broken sign reads:"
  /// (MsgMogallForestBrokenSign)
  LOCALID_MAPSEC_MOGALL_FOREST_SIGN,
  /// ★ Named from its area name: Kalay Tunnel Entrance
  LOCALID_MAPSEC_KALAY_TUNNEL_ENTRANCE,
  /// After three signposts (SPRITE_SIGNPOST). Unused: they stand at
  /// the map's origin, out of the player's reach, and no code moves them.
  /// ★ Named from its area name: Kolima Bridge
  LOCALID_MAPSEC_KOLIMA_BRIDGE = LOCALID_MAPSEC_KALAY_TUNNEL_ENTRANCE + 4,
  /// ★ Named from its area name: Gondowan Passage
  LOCALID_MAPSEC_GONDOWAN_PASSAGE,
  /// The Tolbi-bound Ship at anchor off Crossbone Isle, after the wreck of
  /// Babi's ship (SPRITE_SHIPWRECK), which stands off Gateway Cave.
  /// ★ Named from its area name: Tolbi-bound Ship
  LOCALID_TOLBI_SHIP = LOCALID_MAPSEC_GONDOWAN_PASSAGE + 2,
  /// The storm that lifts the party, as the Suhalla Desert's do.
  /// ★ Named from what the player sees: a white whirlwind spinning on the map,
  /// drawn as the Suhalla Desert's sandstorms, spr_289.png (its render)
  LOCALID_STORM,
  /// Placed by the coast once he has handed over the Black Orb.
  /// ★ Named from the game: "Iodem, Iodem!" (MsgTolbiIodemIodem)
  LOCALID_IODEM
};

/// The metatile behavior raised by touching a map section's object event.
#define COORD_EVENT_MAPSEC(localId) (COORD_EVENT_MAPSEC_BASE + (localId))

/// The world map's own metatile behaviors, which the map's cells carry. Each
/// warp cell carries the number of the warp it leads to, so the warp rows of
/// gWorldMapEvents name their metatile behavior by that warp.
enum WorldMapMetatileBehavior {
  MB_WORLD_MAP_TOLBI_SHIP = 74, ///< the shore beside the anchored ship
  MB_WORLD_MAP_CROSSBONE_ISLE =
      75,                      ///< the island's shore, walked onto northward
  MB_WORLD_MAP_FLINT = 90,     ///< the ground around Flint
  MB_WORLD_MAP_CORONA = 91,    ///< the ground around Corona
  MB_WORLD_MAP_VINE = 92,      ///< the ground around Vine
  MB_WORLD_MAP_HAIL = 93,      ///< the ground around Hail
  MB_WORLD_MAP_STORM = 95,     ///< the ground under the storm
  MB_WORLD_MAP_SHIPWRECK = 96, ///< the scuttled ship off Gateway Cave
  MB_WORLD_MAP_TURTLE_BOOTS =
      100 ///< the ground where the Turtle Boots are buried
};

/// Raised by WorldMap_UpdateStormTimer when the party's time in the air runs
/// out.
enum WorldMapRaisedEvent { RAISED_STORM_TIME_UP = 99 };

/// The rowers' balance (WorldMap_GetRowingBalance) that takes the Tolbi-bound
/// Ship off course to the island.
enum { ROWING_BALANCE_OFF_COURSE = 11 };

/// SPRITE_WORLD_MAP_BRIDGE's animation once Kolima Bridge's drawbridge is down.
enum { WORLD_MAP_BRIDGE_ANIMATION_LOWERED = 2 };

/// The falling rock's animations (spr_216.png): it tumbles as it falls, then
/// shatters where it lands.
enum { FALLING_ROCK_ANIMATION_TUMBLE = 2, FALLING_ROCK_ANIMATION_SHATTER = 3 };

/// The object events of Hammet's caravan leaving Vault
/// (gWorldMapObjectEventTemplatesHammetsCaravan,
/// WORLD_MAP_HAMMETS_CARAVAN_LEAVES), numbered from LOCALID_FIRST_PLACED;
/// Garet's row takes his own local id.
enum WorldMapCaravanLocalId {
  /// Hammet, in his sprite, as LOCALID_HAMMET is out of West Lunpa Cave.
  /// ★ Named from the game: "Master Hammet!"
  /// (MsgWorldMapWeCantStayAnotherMinute)
  LOCALID_CARAVAN_HAMMET = LOCALID_FIRST_PLACED,
  /// One of Hammet's men, bald and bare-chested. "Master Hammet! This is
  /// terrible! The bridge has been destroyed..."
  /// ★ Named from the game: "Master Hammet! This is terrible!"
  /// (MsgWorldMapWeCantStayAnotherMinute)
  LOCALID_CARAVAN_MAN,
  /// The caravan's four wagons, in the sprite of Bunza's wagon.
  /// ★ Named from what the player sees: four covered wagons, each drawn by a
  /// horse (its render)
  LOCALID_CARAVAN_FIRST_WAGON,
  /// A blank object event (SPRITE_BLANK) that the falling rock is held above as
  /// it drops (WorldMap_UpdateFallingRock); it grows from a tenth of its size
  /// as it moves (gCaravanRockGuideScript).
  LOCALID_BLANK_ROCK_GUIDE = LOCALID_CARAVAN_FIRST_WAGON + 4,
  /// Drawn as the rocks in sol_sanctum_eruption.c; it lands by the caravan
  /// and shatters.
  /// ★ Named from the game: "being smashed by falling rocks here"
  /// (MsgWorldMapWeCantStayAnotherMinute)
  LOCALID_FALLING_ROCK
};

/// The Tolbi-bound Ship, the first object event of
/// gWorldMapObjectEventTemplatesCrossingEnds and
/// gWorldMapObjectEventTemplatesCrossing
/// (WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_FIRST_ATTACK to
/// WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI and
/// WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS), shown on its way across the Karagol
/// Sea.
enum {
  /// ★ Named from its area name: Tolbi-bound Ship
  LOCALID_CROSSING_SHIP = LOCALID_FIRST_PLACED
};

/// Bunza's wagon at Kalay
/// (gWorldMapObjectEventTemplatesFromKalayTunnelEntranceSouth), after Kalay's
/// and the Kalay Tunnel Entrance's map section object events.
enum {
  /// ★ Named from the game: "go back to Kalay in my wagon." (MsgLunpaBunza)
  LOCALID_KALAY_WAGON = LOCALID_FIRST_PLACED + 2
};

/// The one object event of gWorldMapObjectEventTemplatesFromClear, before the
/// ending: the island Jenna and Kraden are adrift on ("How long will this
/// island continue to drift?"). It drifts east trailing a wake
/// (WorldMap_SpawnIslandWake) until the map leads into their scene.
enum {
  /// ★ Named from what the player sees: an island of green hills around a
  /// mountain, drifting on the sea, spr_504.png (its render)
  LOCALID_DRIFTING_ISLAND = LOCALID_FIRST_PLACED
};

/// The ending's object events
/// (gWorldMapObjectEventTemplatesJennaAndKradenAdrift), numbered from
/// LOCALID_FIRST_PLACED after the rows of Jenna and Sheba, which take their own
/// local ids.
enum WorldMapEndingLocalId {
  /// Kraden, adrift with Jenna.
  /// ★ Named from the game: "Oh, Kraden... How long will this island continue
  /// to drift?" (MsgWorldMapKradenHowLongWillIsland)
  LOCALID_KRADEN = LOCALID_FIRST_PLACED,
  /// Alex, who comes to show them that Felix and Sheba are alive: "There is
  /// something I wish to show you."
  /// ★ Named from the game: "Alex!" (MsgMercuryGot)
  LOCALID_ALEX,
  /// Felix, in his sprite as at Mercury Lighthouse (SPRITE_FELIX,
  /// mercury_lighthouse_aerie.c), shown with Sheba as Jenna and Kraden call
  /// their names.
  /// ★ Named from the game: "Felix" (MsgCharacterName)
  LOCALID_FELIX
};

/// Animations the map plays by number beyond enum ObjectEventAnimation, from
/// each sprite's animation table (graphics/sprite/spr_<number>.tsv).
enum {
  ANIM_JENNA_LIE = 19, ///< lying on her back (spr_005.tsv's animation list)
  ANIM_KRADEN_LIE = 5, ///< lying on his back (spr_030.tsv)
  ANIM_FELIX_LIE = 6,  ///< lying face down, one arm out (spr_035.tsv)
  ANIM_SHEBA_LIE = 8   ///< lying on her back (spr_006.tsv)
};

// The world map's map flags:
// FLAG_MAP_TEMP_00: the falling rock has landed, in Hammet's caravan scene.
// FLAG_MAP_TEMP_34: Iodem has handed over the Black Orb.
// FLAG_MAP_TEMP_35: the map's ordinary object event templates are in use.
// FLAG_MAP_TEMP_F0: the storm holds the party up in the air.
// FLAG_MAP_TEMP_F1: the storm is done with, or put away.
// FLAG_MAP_TEMP_F8: a byte: frames left in the air.

/// Walking near Vine, or talking to him: once a battle is nearly due, half the
/// time Vine fights; otherwise the next step brings an ordinary battle.
/// Reached by talking to LOCALID_VENUS_DJINNI, or by walking onto
/// MB_WORLD_MAP_VINE until Vine is caught.
static void EventScript_MeetVine(void) {
  struct FieldWork *work = gFieldWork;

  if (gSaveBlock.encounterSteps >=
      Math_Divide(work->encounterThreshold * 9, 10)) {
    if ((u32)Engine_RandomNext() < 0x8000) {
      Field_RequestDjinnBattle(DJINN_NO_HOP | LOCALID_VENUS_DJINNI,
                               DJINNI_VINE);
      work->encounterBias = 0;
    } else {
      gSaveBlock.encounterSteps = work->encounterThreshold;
    }
  }
}

/// Walking near Corona, or talking to him: once a battle is nearly due, Corona
/// fights.
/// Reached by talking to LOCALID_CORONA, or by walking onto MB_WORLD_MAP_CORONA
/// until Corona is caught.
static void EventScript_MeetCorona(void) {
  struct SaveBlock *state = &gSaveBlock;
  s32 *steps = &state->encounterSteps;
  struct FieldWork *work = gFieldWork;

  if (*steps >= Math_Divide(work->encounterThreshold * 9, 10)) {
    Field_RequestDjinnBattle(DJINN_NO_HOP | LOCALID_CORONA, DJINNI_CORONA);
    work->encounterBias = 0;
  }
}

/// Walking near Hail, or talking to him: once a battle is nearly due, Hail
/// fights.
/// Reached by talking to LOCALID_HAIL, or by walking onto
/// MB_WORLD_MAP_HAIL until Hail is caught.
static void EventScript_MeetHail(void) {
  struct SaveBlock *state = &gSaveBlock;
  s32 *steps = &state->encounterSteps;
  struct FieldWork *work = gFieldWork;

  if (*steps >= Math_Divide(work->encounterThreshold * 9, 10)) {
    Field_RequestDjinnBattle(DJINN_NO_HOP | LOCALID_HAIL, DJINNI_HAIL);
    work->encounterBias = 0;
  }
}

/// The update hook of a sparkle around the Black Orb: it frees the sparkle on
/// its second frame.
static void WorldMap_ReleaseSparkAfterDelay(union FieldObject *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  if (objectEvent->timer <= 0) {
    objectEvent->timer++;
  } else {
    ObjectEvent_Release(objectEvent);
  }
}

/// The update hook of the Black Orb held up over the wreck: it swells and
/// shrinks every four frames and gives off a sparkle every other frame.
static void WorldMap_PulseOrbAndSpawnSparks(union FieldObject *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *spark;
  s32 x;
  s32 y;
  s32 z;

  if (gFrameCount & 4) {
    objectEvent->scaleX = 13 * Q16_ONE / 10;
    objectEvent->scaleY = 13 * Q16_ONE / 10;
  } else {
    objectEvent->scaleX = Q16_ONE;
    objectEvent->scaleY = Q16_ONE;
  }
  if (!(gFrameCount & 2))
    return;
  x = objectEvent->x.fixed;
  y = objectEvent->y.fixed;
  z = objectEvent->z.fixed;
  spark = ObjectEvent_Create(SPRITE_SPARKLE, x, y, z);
  Sound_PlaySong(SE_246);
  if (spark == 0)
    return;
  spark->motionFlags = 0;
  spark->sprite->image.priority = 1;
  ObjectEvent_SetSpriteFlags(spark, 0);
  ObjectDispatch_SelectAnimation(spark, 1);
  spark->timer = 0;
  spark->update = WorldMap_ReleaseSparkAfterDelay;
}

/// Flint's update hook until the party meets him (WorldMap_SetUpFlint): he
/// flickers white and red and circles 16 pixels around his spot, bobbing 4
/// pixels up and down, facing the way he goes. EventScript_MeetFlint stops him
/// by setting his work to 1.
static void WorldMap_CircleFlint(union FieldObject *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  s16 *angle;

  if (gFrameCount & 2)
    ObjectEvent_SetColorMode(objectEvent, SPRITE_COLOR_RED_TINT);
  else
    ObjectEvent_SetColorMode(objectEvent, SPRITE_COLOR_WHITE);
  if ((s16)objectEvent->work == 0) {
    angle = (s16 *)&objectEvent->timer;
    objectEvent->x.fixed = PIXELS(5584);
    objectEvent->y.fixed =
        Iwram_MulQ16(Trig_Sin(*angle << 3), PIXELS(4)) + PIXELS(16);
    objectEvent->z.fixed = PIXELS(1328);
    Vector_AddPolarOffset(PIXELS(16), *angle, &objectEvent->x);
    objectEvent->facing = *angle + FACING_SOUTH;
    *angle += 0x400;
  }
}

/// Flint's object script (gWorldMapFlintScript): until he is caught or his
/// lesson has begun (FLAG_DJINN_LESSON), he circles his spot at half size.
static s32 WorldMap_SetUpFlint(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  if (GameFlag_Get(FLAG_DJINNI(DJINNI_FLINT)) != 0) {
    return 0;
  }
  if (GameFlag_Get(FLAG_DJINN_LESSON) != 0) {
    return 0;
  }
  objectEvent->update = WorldMap_CircleFlint;
  objectEvent->motionFlags = 0;
  objectEvent->timer = 0;
  objectEvent->work = 0;
  objectEvent->scaleX = Q16_ONE / 2;
  objectEvent->scaleY = Q16_ONE / 2;
  return 0;
}

const struct MapWarpDest *WorldMap_GetWarpDests(void) {
  return gWorldMapWarpDests;
}

const struct MapWarpRegion *WorldMap_GetRegions(void) { return NULL; }

const u32 *WorldMap_GetWarps(void) { return gWorldMapWarps; }

/// Kolima Bridge's object script (gWorldMapKolimaBridgeScript): it turns with
/// the map and shows the drawbridge let down once FLAG_KOLIMA_BRIDGE_LOWERED is
/// set.
static s32 WorldMap_SetUpKolimaBridge(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  WorldMap_AlignToMap(interpreter);
  if (GameFlag_Get(FLAG_KOLIMA_BRIDGE_LOWERED) != 0) {
    ObjectDispatch_SelectAnimation(objectEvent,
                                   WORLD_MAP_BRIDGE_ANIMATION_LOWERED);
  }
  return 1;
}

/// The object script of every picture on the map (gWorldMapAlignToMapScript):
/// it turns the sprite with the map's rotation.
static s32 WorldMap_AlignToMap(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  struct WorldMapRenderWork *work = gMapGridWork[0];
  struct Sprite *sprite = objectEvent->sprite;

  sprite->rotation = work->rotation;
  sprite->flags = 0;
  return 1;
}

/// Iodem's object script (gWorldMapIodemScript): he turns with the map and
/// blocks the way, so the leader cannot walk through him.
static s32 WorldMap_SetUpIodem(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  struct WorldMapRenderWork *work = gMapGridWork[0];
  struct Sprite *sprite = objectEvent->sprite;

  objectEvent->collisionFlags |= 1;
  sprite->rotation = work->rotation;
  return 1;
}

/// Sets up the storm (LOCALID_STORM) as it is placed: it does not collide, and
/// while the Tolbi-bound Ship lies at anchor off the island
/// (FLAG_TOLBI_SHIP_AT_ISLAND) it is put away at the map's origin and
/// FLAG_MAP_TEMP_F1 keeps it from catching the party.
/// Called by its object script.
static s32 WorldMap_SetUpStorm(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  ObjectEvent_SetColorMode(objectEvent, SPRITE_COLOR_RED_TINT);
  objectEvent->collisionFlags = 0;
  if (GameFlag_Get(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    GameFlag_Set(FLAG_MAP_TEMP_F1);
    objectEvent->x.fixed = 0;
    objectEvent->y.fixed = 0;
  }
  return 0;
}

/// Called each frame by the storm's object script (gWorldMapStormScript): it
/// hides the storm on odd frames, and WorldMap_UpdateLocationContacts shows it
/// again on the next, so it flickers.
static s32 WorldMap_BlinkObjectEventSprite(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  if ((objectEvent->spriteStorageKind & 1) && (gFrameCount & 1))
    objectEvent->spriteStorageKind ^= 1;
  return 1;
}

/// Chooses the object events for the warp destination: each scripted moment has
/// a table of its own while it is still to play, and every other arrival takes
/// the map sections, the Djinn, the storm and Iodem
/// (gWorldMapObjectEventTemplates) and sets FLAG_MAP_TEMP_35.
const struct ObjectEventTemplate *WorldMap_GetObjectEventTemplates(void) {
  switch (gSaveBlock.warpDest) {
  case WORLD_MAP_FROM_WEST_LUNPA_CAVE:
    if (GameFlag_Get(FLAG_BUNZAS_WAGON_LEFT) == 0 &&
        GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
      return gWorldMapObjectEventTemplatesFromWestLunpaCave;
    }
    break;
  case WORLD_MAP_HAMMETS_CARAVAN_LEAVES:
    if (GameFlag_Get(FLAG_LUNPA_GUARDS_SUSPECT_KALAY) == 0) {
      return gWorldMapObjectEventTemplatesHammetsCaravan;
    }
    break;
  case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_FIRST_ATTACK:
  case WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI:
    return gWorldMapObjectEventTemplatesCrossingEnds;
  case WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH:
    return gWorldMapObjectEventTemplatesFromKalayTunnelEntranceSouth;
  case WORLD_MAP_FROM_MAIN_MENU:
    return gWorldMapObjectEventTemplatesFromClear;
  case WORLD_MAP_BEFORE_SHEBAS_FALL:
    return gWorldMapObjectEventTemplatesBeforeShebasFall;
  case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_SECOND_ATTACK:
  case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_THIRD_ATTACK:
  case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_KRAKEN:
  case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_LAND:
  case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS:
    return gWorldMapObjectEventTemplatesCrossing;
  case WORLD_MAP_JENNA_AND_KRADEN_ADRIFT:
    return gWorldMapObjectEventTemplatesJennaAndKradenAdrift;
  default:
    break;
  }

  GameFlag_Set(FLAG_MAP_TEMP_35);
  return gWorldMapObjectEventTemplates;
}

/// Chooses a passage warp from the side of the object event the leader
/// approaches.
static void WorldMap_SelectWarpByX(s32 trigger, s32 warpBefore, s32 warpAfter) {
  struct FieldWork *work;
  struct ObjectEvent *subject;
  struct ObjectEvent *other;

  subject = ObjectEvent_GetById(trigger - COORD_EVENT_MAPSEC_BASE);
  other = ObjectEvent_GetById(gSaveBlock.leaderLocalId);
  work = gFieldWork;
  if (other->x.fixed < subject->x.fixed) {
    work->warp = warpBefore;
  } else {
    work->warp = warpAfter;
  }
  Sound_PlaySong(SE_MAP_EXIT);
}

/// Chooses a passage warp from the leader's north-south approach.
static void WorldMap_SelectWarpByZ(s32 trigger, s32 warpBefore, s32 warpAfter) {
  struct FieldWork *work;
  struct ObjectEvent *subject;
  struct ObjectEvent *other;

  subject = ObjectEvent_GetById(trigger - COORD_EVENT_MAPSEC_BASE);
  other = ObjectEvent_GetById(gSaveBlock.leaderLocalId);
  work = gFieldWork;
  if (other->z.fixed < subject->z.fixed) {
    work->warp = warpBefore;
  } else {
    work->warp = warpAfter;
  }
  Sound_PlaySong(SE_MAP_EXIT);
}

/// Touching the Bilibin Barricade's object event: the side the party comes from
/// decides the warp, from the west WORLD_MAP_TO_BILIBIN_BARRICADE_WEST, from
/// the east WORLD_MAP_TO_BILIBIN_BARRICADE_EAST.
static void EventScript_EnterBilibinBarricade(void) {
  WorldMap_SelectWarpByX(COORD_EVENT_MAPSEC(LOCALID_MAPSEC_BILIBIN_BARRICADE),
                         WORLD_MAP_TO_BILIBIN_BARRICADE_WEST,
                         WORLD_MAP_TO_BILIBIN_BARRICADE_EAST);
}

/// The party enters Kolima Bridge from the world map. It works as
/// EventScript_EnterBilibinBarricade does, entering from the north by
/// WORLD_MAP_TO_KOLIMA_BRIDGE_WEST and from the south by
/// WORLD_MAP_TO_KOLIMA_BRIDGE_EAST.
static void EventScript_EnterKolimaBridge(void) {
  WorldMap_SelectWarpByZ(COORD_EVENT_MAPSEC(LOCALID_MAPSEC_KOLIMA_BRIDGE),
                         WORLD_MAP_TO_KOLIMA_BRIDGE_WEST,
                         WORLD_MAP_TO_KOLIMA_BRIDGE_EAST);
}

/// As EventScript_EnterKolimaBridge, for Kalay Docks: from the north
/// WORLD_MAP_TO_KALAY_DOCKS_NORTH, from the south
/// WORLD_MAP_TO_KALAY_DOCKS_EAST.
static void EventScript_EnterKalayDocks(void) {
  WorldMap_SelectWarpByZ(COORD_EVENT_MAPSEC(LOCALID_MAPSEC_KALAY_DOCKS),
                         WORLD_MAP_TO_KALAY_DOCKS_NORTH,
                         WORLD_MAP_TO_KALAY_DOCKS_EAST);
}

/// As EventScript_EnterKolimaBridge, for Gondowan Passage: from the north
/// WORLD_MAP_TO_GONDOWAN_PASSAGE_EAST, from the south
/// WORLD_MAP_TO_GONDOWAN_PASSAGE_WEST.
static void EventScript_EnterGondowanPassage(void) {
  WorldMap_SelectWarpByZ(COORD_EVENT_MAPSEC(LOCALID_MAPSEC_GONDOWAN_PASSAGE),
                         WORLD_MAP_TO_GONDOWAN_PASSAGE_EAST,
                         WORLD_MAP_TO_GONDOWAN_PASSAGE_WEST);
}

/// As EventScript_EnterKolimaBridge, for Lalivero: from the north
/// WORLD_MAP_TO_LALIVERO_WEST, from the south WORLD_MAP_TO_LALIVERO_EAST.
static void EventScript_EnterLalivero(void) {

  WorldMap_SelectWarpByZ(COORD_EVENT_MAPSEC(LOCALID_MAPSEC_LALIVERO),
                         WORLD_MAP_TO_LALIVERO_WEST,
                         WORLD_MAP_TO_LALIVERO_EAST);
}

const struct MapEvent *WorldMap_GetEvents(void) { return gWorldMapEvents; }

/// Run each frame on the map (WorldMap_Initialize adds it): it hides every
/// placed object event outside a box around the view, 160 pixels either side,
/// 300 north and 200 south, and raises the COORD_EVENT_MAPSEC of an object
/// event within reach of the leader, their radii scaled by their sprites,
/// measured as the sum of the distances east-west and north-south. With the
/// debug flag FLAG_DEBUG_STEPS_OFF it raises none.
static void WorldMap_UpdateLocationContacts(void) {
  struct ObjectEvent *leader;
  struct FieldWork *work;
  struct ObjectEvent *objectEvent;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;
  s32 leaderReach;
  u32 i;

  leader = ObjectEvent_GetById(gSaveBlock.leaderLocalId);
  leaderReach = leader->sprite->scale * leader->radius;
  work = gFieldWork;
  objectEvent = (struct ObjectEvent *)work->camera;
  left = objectEvent->x.fixed - PIXELS(160);
  right = objectEvent->x.fixed + PIXELS(160);
  top = objectEvent->z.fixed - PIXELS(300);
  bottom = objectEvent->z.fixed + PIXELS(200);

  for (i = LOCALID_FIRST_PLACED; i <= CONTACT_LAST_OBJ_EVENT; i++) {
    s32 x;
    s32 z;
    s32 dx;
    s32 reach;
    s32 scale;

    objectEvent = ObjectEvent_GetByLocalId(i);
    if (objectEvent == NULL) {
      continue;
    }
    x = objectEvent->x.fixed;
    z = objectEvent->z.fixed;
    if (x < left || x > right || z < top || z > bottom) {
      objectEvent->spriteStorageKind = 0;
      continue;
    }
    objectEvent->spriteStorageKind = 1;
    if (gDebugMode != 0 && GameFlag_Get(FLAG_DEBUG_STEPS_OFF) != 0) {
      continue;
    }
    scale = objectEvent->sprite->scale;
    dx = objectEvent->x.fixed - leader->x.fixed;
    if (dx < 0) {
      dx = leader->x.fixed - objectEvent->x.fixed;
    }
    reach = leaderReach + objectEvent->radius * scale;
    if (dx + (objectEvent->z.fixed - leader->z.fixed < 0
                  ? leader->z.fixed - objectEvent->z.fixed
                  : objectEvent->z.fixed - leader->z.fixed) <
            reach &&
        GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
      work->coordEvent = i + COORD_EVENT_MAPSEC_BASE;
    }
  }
}

/// Touching Vault's object event: the first time, until
/// FLAG_LUNPA_GUARDS_SUSPECT_KALAY is set, WORLD_MAP_HAMMETS_CARAVAN_LEAVES
/// plays Hammet's caravan leaving (WORLD_MAP_TO_HAMMETS_CARAVAN_LEAVES); later
/// it is Vault's door (WORLD_MAP_TO_VAULT_SOUTH).
static void EventScript_EnterVault(void) {
  if (GameFlag_Get(FLAG_LUNPA_GUARDS_SUSPECT_KALAY) == 0) {
    Engine_ScriptRequestWarp(WORLD_MAP_TO_HAMMETS_CARAVAN_LEAVES);
  } else {
    Sound_PlaySong(SE_MAP_EXIT);
    Engine_ScriptRequestWarp(WORLD_MAP_TO_VAULT_SOUTH);
  }
}

/// Walking east from Crossbone Isle's shore back aboard the Tolbi-bound Ship
/// (WORLD_MAP_TO_TOLBI_SHIP_DECK_CROSSBONE_ISLE, to
/// TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS).
/// Reached by walking east into MB_WORLD_MAP_TOLBI_SHIP while the ship lies at
/// anchor off the island (FLAG_TOLBI_SHIP_AT_ISLAND).
static void EventScript_ReturnToTolbiShip(void) {

  FieldScript_Start();
  Engine_ScriptRequestWarp(WORLD_MAP_TO_TOLBI_SHIP_DECK_CROSSBONE_ISLE);
}

/// The map's initialization: it runs the moment the warp destination
/// selects, the story's moments listed at the top of this file. The two
/// lighthouse warp destinations and WORLD_MAP_AFTER_BLACK_ORB_DEFEAT leave at
/// once; every other arrival sets FLAG_RETREAT_BLOCKED, starts
/// WorldMap_UpdateLocationContacts, opens the screen from a centre line, and
/// until the Manticore of the Lamakan Desert is beaten
/// (FLAG_LAMAKAN_MANTICORE_BEATEN) swaps in the 512-pixel tile south of Vault
/// whose bridge over the river is broken in the middle (the world map laid
/// flat with and without the swap). The arrivals by Crossbone
/// Isle and from the Suhalla Desert's storm resume the time in the air a storm
/// left (FLAG_MAP_TEMP_F8). WORLD_MAP_FROM_DEBUG turns encounters off, and
/// leaving Vale the first time sets FLAG_VALE_PARTY_LEFT and flag 0x85c,
/// which nothing reads.
s32 WorldMap_Initialize(void) {
  struct SaveBlock *state = &gSaveBlock;
  s16 *warpDest = &state->warpDest;

  if (*warpDest == WORLD_MAP_FROM_DEBUG) {
    GameFlag_Set(FLAG_NO_ENCOUNTERS);
    GameFlag_Set(FLAG_NO_ENCOUNTERS_IN_MAPSEC);
    GameFlag_Set(FLAG_DEBUG_STEPS_OFF);
  }
  if (*warpDest == WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT) {
    WorldMap_PlayLighthouseTransition(0);
    FieldScript_SetDestination((s32)&MapId_MercuryLighthouseAerie,
                               MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT);
  } else if (*warpDest == WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT) {
    WorldMap_PlayLighthouseTransition(1);
    FieldScript_SetDestination((s32)&MapId_VenusLighthouseAerie,
                               VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT);
  } else if (*warpDest == WORLD_MAP_AFTER_BLACK_ORB_DEFEAT) {
    FieldScript_Start();
    PartyInventory_Discard(ITEM_BLACK_ORB);
    Engine_ScriptRequestWarp(WORLD_MAP_TO_LUNPA_INTERIORS);
  } else {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    gFieldWork->transition = MAP_TRANSITION(TRANSITION_CENTRE_LINE, 0);
    gFieldWork->transitionFrames = 16;
    WaitFrames(1);
    FieldCamera_SetSpeedParameters(PIXELS(8), PIXELS(1));
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
    FieldScheduler_AddCallback(WorldMap_UpdateLocationContacts,
                               TASK_PRIORITY_MAP);
    if (GameFlag_Get(FLAG_LAMAKAN_MANTICORE_BEATEN) == 0) {
      Field_SetWorldMapWindowCellTile(128, 256, 176, 56);
    }
    // Each special warp destination resumes its travel or story sequence.
    switch (*warpDest) {
    case WORLD_MAP_FROM_VALE_GATE:
      if (Script_GetFlag(FLAG_VALE_PARTY_LEFT) == 0) {
        GameFlag_Set(FLAG_VALE_PARTY_LEFT);
        GameFlag_Set(0x85c);
      }
      break;
    case WORLD_MAP_FROM_GATEWAY_CAVE:
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
        if (GameFlag_Get(FLAG_BLACK_ORB_RETURNED) == 0 &&
            GameFlag_Get(FLAG_MAP_TEMP_34) != 0) {
          gWorldMapIodem = LOCALID_IODEM;
          ObjectEvent_SetPosition(LOCALID_IODEM, PIXELS(6036), PIXELS(3400));
          ObjectEvent_GetById(gWorldMapIodem)->facing =
              FACING_SOUTHEAST + FACING_STEP;
          WorldMap_BlockGatewayCaveWarp(gWorldMapIodem);
        }
      } else if (GameFlag_Get(FLAG_BLACK_ORB_RETURNED) == 0 &&
                 GameFlag_Get(FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN) == 0) {
        WorldMap_RunBlackOrbScene();
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
    case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_FIRST_ATTACK:
      WorldMap_SailToFirstMonsterAttack();
      break;
    case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_SECOND_ATTACK:
      WorldMap_SailToSecondMonsterAttack();
      break;
    case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_THIRD_ATTACK:
      WorldMap_SailToThirdMonsterAttack();
      break;
    case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_KRAKEN:
      WorldMap_SailToKraken();
      break;
    case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_LAND:
      WorldMap_SailUntilLandSighted();
      break;
    case WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI:
      WorldMap_SailIntoTolbi();
      break;
    case WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH:
      WorldMap_StepDownFromBunzasWagon();
      break;
    case WORLD_MAP_FROM_MAIN_MENU:
      WorldMap_ShowIslandDrifting();
      break;
    case WORLD_MAP_BEFORE_SHEBAS_FALL:
      WorldMap_ShakeVenusLighthouse();
      break;
    case WORLD_MAP_FROM_TOLBI_SHIP_DECK_CROSSBONE_ISLE:
    case WORLD_MAP_FROM_CROSSBONE_ISLE:
    case WORLD_MAP_FROM_SUHALLA_DESERT_STORM:
      GameFlag_Set(FLAG_WORLD_MAP_LEFT);
      if (FieldGameFlag_GetByte(FLAG_MAP_TEMP_F8) != 0) {
        gSaveBlock.movementMode = LEADER_MOVEMENT_AIRBORNE;
        FieldScheduler_AddCallback(WorldMap_UpdateStormTimer,
                                   TASK_PRIORITY_MAP);
      }
      break;
    case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS:
      WorldMap_SailTolbiShipToIsland();
      break;
    case WORLD_MAP_JENNA_AND_KRADEN_ADRIFT:
      WorldMap_ShowEndingScene();
      break;
    default:
      ObjectEvent_GetById(LOCALID_TOLBI_SHIP)->scaleX = 5 * Q16_ONE / 4;
      ObjectEvent_GetById(LOCALID_TOLBI_SHIP)->scaleY = 5 * Q16_ONE / 4;
      break;
    }
  }
  return 0;
}

/// The object events placed at WORLD_MAP_FROM_WEST_LUNPA_CAVE
/// (gWorldMapObjectEventTemplatesFromWestLunpaCave), numbered from
/// LOCALID_FIRST_PLACED: Bunza's wagon, then, after Lunpa's and West Lunpa
/// Cave's map section object events, Bunza and Hammet.
enum {
  /// Bunza's wagon.
  /// ★ Named from the game: "go back to Kalay in my wagon." (MsgLunpaBunza)
  LOCALID_WAGON = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "Then what should we do, Bunza?" (MsgLunpaBunza)
  LOCALID_BUNZA = LOCALID_FIRST_PLACED + 3,
  /// ★ Named from the game: "Master Hammet!"
  /// (MsgWorldMapWeCantStayAnotherMinute)
  LOCALID_HAMMET
};

/// Out of West Lunpa Cave by wagon: Bunza and Hammet nod to each other and
/// climb aboard with the leader, and the wagon rolls off.
/// Called by WorldMap_Initialize at WORLD_MAP_FROM_WEST_LUNPA_CAVE, after the
/// trade reopens (FLAG_DODONPA_LOCKED_UP) and until the wagon has left;
/// sets FLAG_BUNZAS_WAGON_LEFT.
static void WorldMap_BoardBunzasWagon(void) {
  struct ObjectEvent *record;

  FieldScript_Start();
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  WorldMap_StartScript();
  GameFlag_Set(FLAG_BUNZAS_WAGON_LEFT);
  ObjectEvent_SetPosition(LOCALID_BUNZA, PIXELS(5856), PIXELS(1180));
  ObjectEvent_SetDestinationOffset(LOCALID_BUNZA, 24, 8);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_BUNZA);
  FieldScript_WaitFrames(60);
  ObjectEvent_SetPosition(LOCALID_HAMMET, PIXELS(5856), PIXELS(1180));
  ObjectEvent_SetDestinationOffset(LOCALID_HAMMET, 12, 24);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_NORTH + FACING_STEP, 0);
  FieldScript_WaitFrames(60);
  Engine_ObjectEventSetAnimation(LOCALID_BUNZA, ANIM_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_HAMMET, ANIM_NOD);
  FieldScript_WaitFrames(120);
  ObjectEvent_SetPosition(LOCALID_WAGON, PIXELS(5880), PIXELS(1208));
  FieldScript_WaitFrames(60);
  Engine_ObjectEventSetAnimation(LOCALID_HAMMET, ANIM_WALK);
  record = ObjectEvent_GetById(LOCALID_WAGON);
  if (record != 0) {
    ObjectEvent_SetDestination(LOCALID_HAMMET, record->x.part.pixel,
                               record->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_HAMMET);
  ObjectEvent_SetPosition(LOCALID_HAMMET, 0, 0);
  FieldScript_WaitFrames(60);
  Engine_ObjectEventSetAnimation(LOCALID_BUNZA, ANIM_WALK);
  record = ObjectEvent_GetById(LOCALID_WAGON);
  if (record != 0) {
    ObjectEvent_SetDestination(LOCALID_BUNZA, record->x.part.pixel,
                               record->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_BUNZA);
  ObjectEvent_SetPosition(LOCALID_BUNZA, 0, 0);
  FieldScript_WaitFrames(60);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  record = ObjectEvent_GetById(LOCALID_WAGON);
  if (record != 0) {
    ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, record->x.part.pixel,
                               record->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_SetSpeed(LOCALID_WAGON, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_SetDestinationOffset(LOCALID_WAGON, 56, 8);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_WAGON);
  ObjectEvent_SetDestinationOffset(LOCALID_WAGON, 40, 40);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_WAGON);
  ObjectEvent_SetDestinationOffset(LOCALID_WAGON, 8, 88);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_WAGON);
  FieldScript_FinishScreenTransition();
  Engine_ScriptRequestWarp(WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE_SOUTH);
  FieldScript_End();
}

/// Outside Vault, the first time the party comes to it: Hammet's caravan
/// rolls out to leave for Kalay, finds the bridge south destroyed, and turns
/// north for Lunpa, leaving Ivan behind, as the ground shakes and a rock falls
/// beside it. Then the party goes on into Vault (WORLD_MAP_TO_VAULT_SOUTH).
/// Called by WorldMap_Initialize at WORLD_MAP_HAMMETS_CARAVAN_LEAVES until
/// FLAG_LUNPA_GUARDS_SUSPECT_KALAY is set; sets it.
static void WorldMap_ShowHammetsCaravan(void) {
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *spriteObjectEvent;
  const s32 *action;

  objectEvent = ObjectEvent_GetById(LOCALID_FALLING_ROCK);
  FieldScript_Start();
  FieldEffect_ScheduleRatioTransition(5 * Q16_ONE / 4, 1);
  WaitFrames(4);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  WorldMap_StartScript();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 5884, 1576);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_SetPosition(LOCALID_CARAVAN_HAMMET, PIXELS(5848), PIXELS(1576));
  WaitFrames(1);
  ObjectEvent_SetChildValue(LOCALID_CARAVAN_HAMMET, SPRITE_COLOR_HIDDEN);
  spriteObjectEvent = ObjectEvent_Get(LOCALID_CARAVAN_HAMMET);
  ObjectEvent_SetSpriteFlags(spriteObjectEvent, 0);
  ObjectEvent_SetSpeed(LOCALID_CARAVAN_FIRST_WAGON, PIXELS(8) / 5,
                       PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_CARAVAN_FIRST_WAGON + 1, PIXELS(8) / 5,
                       PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_CARAVAN_FIRST_WAGON + 2, PIXELS(8) / 5,
                       PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_CARAVAN_FIRST_WAGON + 3, PIXELS(8) / 5,
                       PIXELS(2) / 5);
  Script_PlaySong(SE_RUMBLE);
  ObjectEvent_SetActionScript(LOCALID_CARAVAN_FIRST_WAGON,
                              gCaravanFirstWagonArrives);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetActionScript(LOCALID_CARAVAN_FIRST_WAGON + 1,
                              gCaravanSecondWagonArrives);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetActionScript(LOCALID_CARAVAN_FIRST_WAGON + 2,
                              gCaravanThirdWagonArrives);
  FieldScript_WaitFrames(20);
  ObjectEvent_RunActionScriptAndWait(LOCALID_CARAVAN_FIRST_WAGON + 3,
                                     gCaravanFourthWagonArrives);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  spriteObjectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (spriteObjectEvent != 0) {
    ObjectEvent_SetPosition(LOCALID_GARET, spriteObjectEvent->x.fixed,
                            spriteObjectEvent->z.fixed);
  }
  ObjectEvent_WalkToAndWait(LOCALID_GARET, 5892, 1600);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectEvent_SetPosition(LOCALID_CARAVAN_HAMMET, PIXELS(5848), PIXELS(1592));
  WaitFrames(1);
  // Hammet will not stay, and the wagons head south for the bridge.
  FieldScript_SetMessageId(MsgWorldMapWeCantStayAnotherMinute);
  Script_ShowMessageAndWait(LOCALID_CARAVAN_HAMMET, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 60);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 10);
  Script_PlaySong(SE_RUMBLE);
  ObjectEvent_SetActionScript(LOCALID_CARAVAN_FIRST_WAGON,
                              gCaravanFirstWagonDeparts);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetActionScript(LOCALID_CARAVAN_FIRST_WAGON + 1,
                              gCaravanSecondWagonDeparts);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetActionScript(LOCALID_CARAVAN_FIRST_WAGON + 2,
                              gCaravanThirdWagonDeparts);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 10);
  ObjectEvent_RunActionScriptAndWait(LOCALID_CARAVAN_FIRST_WAGON + 3,
                                     gCaravanFourthWagonDeparts);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  FieldScript_WaitFrames(20);
  WorldMap_EndScript();
  Engine_ObjectEventSetAnimation(LOCALID_CARAVAN_FIRST_WAGON, ANIM_STAND);
  Engine_ObjectEventSetAnimation(LOCALID_CARAVAN_FIRST_WAGON + 1, ANIM_STAND);
  Engine_ObjectEventSetAnimation(LOCALID_CARAVAN_FIRST_WAGON + 2, ANIM_STAND);
  Engine_ObjectEventSetAnimation(LOCALID_CARAVAN_FIRST_WAGON + 3, ANIM_STAND);
  FieldCamera_MoveTo(PIXELS(5640), -1, PIXELS(1784), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  WorldMap_StartScript();
  ObjectEvent_SetPosition(LOCALID_CARAVAN_MAN, PIXELS(5640), PIXELS(1752));
  WaitFrames(1);
  ObjectEvent_SetSpeed(LOCALID_CARAVAN_MAN, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_CARAVAN_MAN, 5640, 1736);
  ObjectEvent_WalkToAndWait(LOCALID_CARAVAN_MAN, 5624, 1736);
  ObjectEvent_WalkToAndWait(LOCALID_CARAVAN_MAN, 5624, 1784);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_CARAVAN_MAN, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetAttachedEffect(LOCALID_CARAVAN_MAN, ATTACHED_EFFECT_IN_FRONT |
                                                         ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(LOCALID_CARAVAN_MAN, 0, 20);
  ObjectMotion_SetVariantCallback(LOCALID_CARAVAN_MAN, 3);
  Script_ShowMessageAndWait(LOCALID_CARAVAN_MAN, 0, 20);
  ObjectEvent_SetPosition(LOCALID_CARAVAN_HAMMET, PIXELS(5656), PIXELS(1784));
  WaitFrames(1);
  ObjectEvent_SetChildValue(LOCALID_CARAVAN_HAMMET, SPRITE_COLOR_NORMAL);
  spriteObjectEvent = ObjectEvent_Get(LOCALID_CARAVAN_HAMMET);
  ObjectEvent_SetSpriteFlags(spriteObjectEvent, SPRITE_FLAG_SHADOW);
  ObjectEvent_SetSpeed(LOCALID_CARAVAN_HAMMET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_CARAVAN_HAMMET, 5640, 1784);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(LOCALID_CARAVAN_HAMMET, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_CARAVAN_HAMMET, 0,
                            10);
  ObjectEvent_FaceDirection(LOCALID_CARAVAN_HAMMET,
                            FACING_SOUTHEAST + FACING_STEP, 60);
  ObjectEvent_FaceDirection(LOCALID_CARAVAN_HAMMET, FACING_WEST, 10);
  ObjectEvent_SetAttachedEffect(
      LOCALID_CARAVAN_HAMMET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(LOCALID_CARAVAN_MAN, FACING_SOUTHEAST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_CARAVAN_HAMMET,
                            FACING_SOUTHEAST + FACING_STEP, 40);
  ObjectEvent_SetAttachedEffect(
      LOCALID_CARAVAN_HAMMET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVariantCallback(LOCALID_CARAVAN_HAMMET, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_CARAVAN_HAMMET, 0,
                            40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_CARAVAN_MAN, 1);
  ObjectEvent_FaceDirection(LOCALID_CARAVAN_MAN, 0, 10);
  Script_ShowMessageAndWait(LOCALID_CARAVAN_MAN, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_CARAVAN_HAMMET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        60);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_CARAVAN_HAMMET, 0,
                            10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_CARAVAN_HAMMET, 1);
  ObjectMotion_SetAnimationAndWait(LOCALID_CARAVAN_HAMMET, ANIM_NOD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_CARAVAN_HAMMET, 0,
                            10);
  ObjectEvent_ShowEmote(LOCALID_CARAVAN_MAN, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        60);
  Script_ShowMessageAndWait(LOCALID_CARAVAN_MAN, 0, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_CARAVAN_HAMMET, ANIM_NOD);
  WorldMap_EndScript();
  Script_PlaySong(SE_107);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(4));
  // The ground shakes, and a rock falls beside Hammet and his man.
  WorldMap_ShakeCameraOverCaravan();
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  ObjectEvent_ShowEmote(LOCALID_CARAVAN_HAMMET,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_CARAVAN_MAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        0);
  ObjectEvent_FaceDirection(LOCALID_CARAVAN_HAMMET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_CARAVAN_MAN, 0, 40);
  ObjectEvent_FaceDirection(LOCALID_CARAVAN_HAMMET,
                            FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_CARAVAN_MAN, FACING_NORTHWEST + FACING_STEP,
                            0);
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(5608), -1, PIXELS(1736), 1);
  FieldCamera_WaitForArrival();
  ObjectEvent_SetPosition(LOCALID_BLANK_ROCK_GUIDE, PIXELS(5544), PIXELS(1704));
  WaitFrames(1);
  ObjectEvent_SetSpeed(LOCALID_BLANK_ROCK_GUIDE, PIXELS(3) / 10,
                       PIXELS(3) / 20);
  ObjectEvent_SetActionScript(LOCALID_BLANK_ROCK_GUIDE,
                              gCaravanRockGuideScript);
  FieldScript_WaitFrames(160);
  // The falling rock drops from 64 pixels up, and bounces as it lands.
  objectEvent->gravity = PIXELS(1) / 10;
  objectEvent->bounce = PIXELS(1) / 10;
  objectEvent->scaleX = 3 * Q16_ONE / 2;
  objectEvent->scaleY = 3 * Q16_ONE / 2;
  objectEvent->timer = 0;
  objectEvent->y.fixed = PIXELS(64);
  objectEvent->sprite->rotation = 0xf000;
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  ObjectDispatch_SelectAnimation(objectEvent, FALLING_ROCK_ANIMATION_TUMBLE);
  WaitFrames(1);
  spriteObjectEvent = ObjectEvent_Get(LOCALID_FALLING_ROCK);
  ObjectEvent_SetSpriteFlags(spriteObjectEvent, 0);
  FieldScheduler_AddCallback(WorldMap_UpdateFallingRock, TASK_PRIORITY_MAP);
  do {
    WaitFrames(1);
  } while (objectEvent->timer == 0);
  spriteObjectEvent = ObjectEvent_Get(LOCALID_FALLING_ROCK);
  ObjectEvent_SetSpriteFlags(spriteObjectEvent, 0);
  spriteObjectEvent = ObjectEvent_Get(LOCALID_BLANK_ROCK_GUIDE);
  ObjectEvent_SetSpriteFlags(spriteObjectEvent, 0);
  FieldScript_WaitFrames(10);
  spriteObjectEvent = ObjectEvent_Get(LOCALID_CARAVAN_MAN);
  spriteObjectEvent->velocityY = PIXELS(6);
  spriteObjectEvent = ObjectEvent_GetById(LOCALID_CARAVAN_HAMMET);
  spriteObjectEvent->velocityY = PIXELS(6);
  Script_PlaySong(SE_145);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(4));
  WorldMap_ShakeCameraWhereRockFalls();
  WorldMap_ShakeCameraWhereRockFalls();
  FieldScript_WaitFrames(60);
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(5640), -1, PIXELS(1784), 1);
  FieldCamera_WaitForArrival();
  WorldMap_StartScript();
  ObjectEvent_SetAttachedEffect(LOCALID_CARAVAN_MAN, ATTACHED_EFFECT_IN_FRONT |
                                                         ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(
      LOCALID_CARAVAN_HAMMET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  Task_Remove(WorldMap_UpdateFallingRock);
  WaitFrames(1);
  ObjectEvent_SetPosition(LOCALID_BLANK_ROCK_GUIDE, 0, 0);
  ObjectEvent_SetPosition(LOCALID_FALLING_ROCK, 0, 0);
  ObjectEvent_FaceDirection(LOCALID_CARAVAN_HAMMET, FACING_WEST, 10);
  ObjectMotion_Launch(LOCALID_CARAVAN_HAMMET, 4, 40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_CARAVAN_HAMMET, 0,
                            10);
  ObjectEvent_FaceDirection(LOCALID_CARAVAN_MAN, 0, 10);
  Script_ShowMessageAndWait(LOCALID_CARAVAN_MAN, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_CARAVAN_HAMMET, FACING_NORTH, 40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_CARAVAN_HAMMET, 0,
                            20);
  ObjectMotion_Launch(LOCALID_CARAVAN_MAN, 4, 20);
  Script_ShowMessageAndWait(LOCALID_CARAVAN_MAN, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_CARAVAN_HAMMET, 1);
  ObjectEvent_FaceDirection(LOCALID_CARAVAN_HAMMET, FACING_WEST, 10);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_CARAVAN_HAMMET, 0,
                            10);
  ObjectEvent_SetAttachedEffect(LOCALID_CARAVAN_MAN, ATTACHED_EFFECT_IN_FRONT |
                                                         ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(80);
  ObjectMotion_SetAnimationAndWait(LOCALID_CARAVAN_HAMMET, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_CARAVAN_MAN, 1);
  ObjectMotion_SetAnimationAndWait(LOCALID_CARAVAN_MAN, ANIM_NOD);
  ObjectEvent_WalkToAndWait(LOCALID_CARAVAN_HAMMET, 5656, 1784);
  ObjectEvent_SetPosition(LOCALID_CARAVAN_HAMMET, 0, 0);
  ObjectEvent_WalkToAndWait(LOCALID_CARAVAN_MAN, 5624, 1736);
  ObjectEvent_WalkToAndWait(LOCALID_CARAVAN_MAN, 5640, 1736);
  ObjectEvent_WalkToAndWait(LOCALID_CARAVAN_MAN, 5640, 1752);
  ObjectEvent_SetPosition(LOCALID_CARAVAN_MAN, 0, 0);
  Script_PlaySong(SE_RUMBLE);
  ObjectEvent_SetActionScript(LOCALID_CARAVAN_FIRST_WAGON,
                              gCaravanFirstWagonReturns);
  ObjectEvent_SetActionScript(LOCALID_CARAVAN_FIRST_WAGON + 1,
                              gCaravanSecondWagonReturns);
  FieldScript_WaitFrames(40);
  ObjectEvent_SetActionScript(LOCALID_CARAVAN_FIRST_WAGON + 2,
                              gCaravanThirdWagonReturns);
  FieldScript_WaitFrames(40);
  ObjectEvent_RunActionScriptAndWait(LOCALID_CARAVAN_FIRST_WAGON + 3,
                                     gCaravanFourthWagonReturns);
  WorldMap_EndScript();
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(5900), PIXELS(1576));
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(5908), PIXELS(1600));
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  FieldCamera_MoveTo(PIXELS(5848), -1, PIXELS(1608), 1);
  FieldCamera_WaitForArrival();
  action = gCaravanWagonHeadsNorth;
  ObjectEvent_SetActionScript(LOCALID_CARAVAN_FIRST_WAGON, action);
  FieldScript_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(5848), -1, PIXELS(1544), 1);
  ObjectEvent_SetActionScript(LOCALID_CARAVAN_FIRST_WAGON + 1, action);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetActionScript(LOCALID_CARAVAN_FIRST_WAGON + 2, action);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
  ObjectEvent_SetActionScript(LOCALID_CARAVAN_FIRST_WAGON + 3, action);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_RefreshSelectorById(LOCALID_CARAVAN_FIRST_WAGON + 3);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  FieldCamera_MoveTo(PIXELS(5880), -1, PIXELS(1608), 1);
  FieldCamera_WaitForArrival();
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 80);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Engine_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  spriteObjectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (spriteObjectEvent != 0) {
    ObjectEvent_SetDestination(LOCALID_GARET, spriteObjectEvent->x.part.pixel,
                               spriteObjectEvent->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(5848), -1, PIXELS(1608), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 5848, 1576);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  GameFlag_Set(FLAG_LUNPA_GUARDS_SUSPECT_KALAY);
  Engine_ScriptRequestWarp(WORLD_MAP_TO_VAULT_SOUTH);
  FieldScript_End();
}

/// Moves to the next camera-shake point and holds it for four frames.
static inline void WorldMap_MoveShakeCamera(s32 x, s32 z) {
  FieldCamera_MoveTo(x, -1, z, 1);
  WaitFrames(4);
}

/// Shakes the camera around Hammet and his man as the ground trembles.
static void WorldMap_ShakeCameraOverCaravan(void) {
  WorldMap_MoveShakeCamera(PIXELS(5644), PIXELS(1784));
  WorldMap_MoveShakeCamera(PIXELS(5636), PIXELS(1788));
  WorldMap_MoveShakeCamera(PIXELS(5644), PIXELS(1780));
  WorldMap_MoveShakeCamera(PIXELS(5644), PIXELS(1788));
  WorldMap_MoveShakeCamera(PIXELS(5636), PIXELS(1780));
  WorldMap_MoveShakeCamera(PIXELS(5640), PIXELS(1784));
  WorldMap_MoveShakeCamera(PIXELS(5642), PIXELS(1784));
  WorldMap_MoveShakeCamera(PIXELS(5638), PIXELS(1786));
  WorldMap_MoveShakeCamera(PIXELS(5642), PIXELS(1782));
  WorldMap_MoveShakeCamera(PIXELS(5642), PIXELS(1786));
  WorldMap_MoveShakeCamera(PIXELS(5638), PIXELS(1782));
  WorldMap_MoveShakeCamera(PIXELS(5640), PIXELS(1784));
}

/// Shakes the camera around the spot where the falling rock lands.
static void WorldMap_ShakeCameraWhereRockFalls(void) {
  WorldMap_MoveShakeCamera(PIXELS(5612), PIXELS(1736));
  WorldMap_MoveShakeCamera(PIXELS(5604), PIXELS(1740));
  WorldMap_MoveShakeCamera(PIXELS(5612), PIXELS(1732));
  WorldMap_MoveShakeCamera(PIXELS(5612), PIXELS(1740));
  WorldMap_MoveShakeCamera(PIXELS(5604), PIXELS(1732));
  WorldMap_MoveShakeCamera(PIXELS(5608), PIXELS(1736));
  WorldMap_MoveShakeCamera(PIXELS(5610), PIXELS(1736));
  WorldMap_MoveShakeCamera(PIXELS(5606), PIXELS(1738));
  WorldMap_MoveShakeCamera(PIXELS(5610), PIXELS(1734));
  WorldMap_MoveShakeCamera(PIXELS(5610), PIXELS(1738));
  WorldMap_MoveShakeCamera(PIXELS(5606), PIXELS(1734));
  WorldMap_MoveShakeCamera(PIXELS(5608), PIXELS(1736));
}

/// Keeps the falling rock above its guide as it drops, and lands it once.
/// Run each frame while the rock falls in WorldMap_ShowHammetsCaravan; sets
/// FLAG_MAP_TEMP_00 as it lands.
static void WorldMap_UpdateFallingRock(void) {
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *guide;

  objectEvent = ObjectEvent_GetById(LOCALID_FALLING_ROCK);
  guide = ObjectEvent_GetById(LOCALID_BLANK_ROCK_GUIDE);
  objectEvent->x.fixed = guide->x.fixed;
  objectEvent->z.fixed = guide->z.fixed;
  if (objectEvent->y.fixed < PIXELS(10)) {
    objectEvent->y.fixed = PIXELS(10);
    if (Script_GetFlag(FLAG_MAP_TEMP_00) == 0) {
      Script_PlaySong(SE_145);
      ObjectDispatch_SelectAnimation(objectEvent,
                                     FALLING_ROCK_ANIMATION_SHATTER);
      GameFlag_Set(FLAG_MAP_TEMP_00);
      objectEvent->timer = 1;
    }
  }
}

/// A leg of the Tolbi-bound Ship's crossing: the ship sails out of Kalay
/// Docks, and WORLD_MAP_TO_TOLBI_SHIP_MAST_FIRST_ATTACK leads up to the lookout
/// on the mast for the first monster attack
/// (TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK, tolbi_ship_mast.c). Called by
/// WorldMap_Initialize at
/// WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_FIRST_ATTACK; sets
/// FLAG_TOLBI_SHIP_ROWING.
static void WorldMap_SailToFirstMonsterAttack(void) {
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(LOCALID_CROSSING_SHIP);
  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  objectEvent->scaleY = 5 * Q16_ONE / 4;
  objectEvent->scaleX = 5 * Q16_ONE / 4;
  FieldCamera_FollowObjectEvent(LOCALID_CROSSING_SHIP, 1);
  FieldScript_StartScreenTransition();
  ObjectEvent_SetSpeed(LOCALID_CROSSING_SHIP, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_CROSSING_SHIP, 5288, 2328);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  GameFlag_Set(FLAG_TOLBI_SHIP_ROWING);
  Engine_ScriptRequestWarp(WORLD_MAP_TO_TOLBI_SHIP_MAST_FIRST_ATTACK);
  FieldScript_End();
}

/// The middle legs of the crossing: the ship, seen from above, sails west
/// across the Karagol Sea from the far east of the map until
/// gCrossingShipSailsWest says it has gone far enough, and the party leaves
/// by warp, back aboard. Sets FLAG_TOLBI_SHIP_ROWING.
static inline void WorldMap_SailCrossingLeg(s32 warp) {
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(LOCALID_CROSSING_SHIP);
  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_CROSSING_SHIP, PIXELS(7944), PIXELS(200));
  objectEvent->scaleX = 5 * Q16_ONE / 4;
  objectEvent->scaleY = 5 * Q16_ONE / 4;
  WaitFrames(1);
  FieldCamera_FollowObjectEvent(LOCALID_CROSSING_SHIP, 1);
  FieldScript_StartScreenTransition();
  ObjectEvent_SetSpeed(LOCALID_CROSSING_SHIP, PIXELS(3) / 5, PIXELS(3) / 10);
  objectEvent->timer = 0;
  ObjectEvent_SetActionScript(LOCALID_CROSSING_SHIP, gCrossingShipSailsWest);
  do {
    WaitFrames(1);
  } while (objectEvent->timer == 0);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  GameFlag_Set(FLAG_TOLBI_SHIP_ROWING);
  Engine_ScriptRequestWarp(warp);
  FieldScript_End();
}

/// A leg of the Tolbi-bound Ship's crossing: WORLD_MAP_TO_TOLBI_SHIP_MAST_
/// SECOND_ATTACK leads up to the lookout on the mast for the second monster
/// attack (TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK).
/// Called by WorldMap_Initialize at
/// WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_SECOND_ATTACK.
static void WorldMap_SailToSecondMonsterAttack(void) {
  WorldMap_SailCrossingLeg(WORLD_MAP_TO_TOLBI_SHIP_MAST_SECOND_ATTACK);
}

/// A leg of the Tolbi-bound Ship's crossing: WORLD_MAP_TO_TOLBI_SHIP_MAST_
/// THIRD_ATTACK leads up to the lookout on the mast for the third monster
/// attack (TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK).
/// Called by WorldMap_Initialize at
/// WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_THIRD_ATTACK.
static void WorldMap_SailToThirdMonsterAttack(void) {
  WorldMap_SailCrossingLeg(WORLD_MAP_TO_TOLBI_SHIP_MAST_THIRD_ATTACK);
}

/// A leg of the Tolbi-bound Ship's crossing: WORLD_MAP_TO_TOLBI_SHIP_INTERIORS
/// leads back down to the rowing deck as the Kraken comes aboard
/// (TOLBI_SHIP_INTERIORS_FROM_WORLD_MAP).
/// Called by WorldMap_Initialize at
/// WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_KRAKEN.
static void WorldMap_SailToKraken(void) {
  WorldMap_SailCrossingLeg(WORLD_MAP_TO_TOLBI_SHIP_INTERIORS);
}

/// The last leg of the Tolbi-bound Ship's crossing: the ship sails west,
/// veering north when the rowers' balance (WorldMap_GetRowingBalance) is the
/// one that takes it off course to the island, and
/// WORLD_MAP_TO_TOLBI_SHIP_MAST_LAND_HO leads up to the lookout on the mast as
/// he sights land (TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO). Called by
/// WorldMap_Initialize at WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_LAND;
/// sets FLAG_TOLBI_SHIP_ROWING.
static void WorldMap_SailUntilLandSighted(void) {
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(LOCALID_CROSSING_SHIP);
  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_CROSSING_SHIP, PIXELS(7944), PIXELS(200));
  objectEvent->scaleX = 5 * Q16_ONE / 4;
  objectEvent->scaleY = 5 * Q16_ONE / 4;
  WaitFrames(1);
  FieldCamera_FollowObjectEvent(LOCALID_CROSSING_SHIP, 1);
  FieldScript_StartScreenTransition();
  ObjectEvent_SetSpeed(LOCALID_CROSSING_SHIP, PIXELS(3) / 5, PIXELS(3) / 10);
  objectEvent->timer = 0;
  if (WorldMap_GetRowingBalance() == ROWING_BALANCE_OFF_COURSE) {
    ObjectEvent_SetActionScript(LOCALID_CROSSING_SHIP, gCrossingShipVeersNorth);
  } else {
    ObjectEvent_SetActionScript(LOCALID_CROSSING_SHIP, gCrossingShipSailsWest);
  }
  do {
    WaitFrames(1);
  } while (objectEvent->timer == 0);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  GameFlag_Set(FLAG_TOLBI_SHIP_ROWING);
  Engine_ScriptRequestWarp(WORLD_MAP_TO_TOLBI_SHIP_MAST_LAND_HO);
  FieldScript_End();
}

/// The crossing's last leg: the ship sails into Tolbi's harbour, and
/// WORLD_MAP_TO_TOLBI_SHIP_DECK leads back aboard as it docks
/// (TOLBI_SHIP_DECK_FROM_WORLD_MAP).
/// Called by WorldMap_Initialize at WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI;
/// sets FLAG_TOLBI_SHIP_REACHED_TOLBI and clears FLAG_TOLBI_SHIP_ROWING.
static void WorldMap_SailIntoTolbi(void) {
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(LOCALID_CROSSING_SHIP);
  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_CROSSING_SHIP, PIXELS(5096), PIXELS(2328));
  objectEvent->scaleY = 5 * Q16_ONE / 4;
  objectEvent->scaleX = 5 * Q16_ONE / 4;
  WaitFrames(1);
  FieldCamera_FollowObjectEvent(LOCALID_CROSSING_SHIP, 1);
  FieldScript_StartScreenTransition();
  ObjectEvent_SetSpeed(LOCALID_CROSSING_SHIP, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_CROSSING_SHIP, 5064, 2328);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  GameFlag_Set(FLAG_TOLBI_SHIP_REACHED_TOLBI);
  GameFlag_Clear(FLAG_TOLBI_SHIP_ROWING);
  Engine_ScriptRequestWarp(WORLD_MAP_TO_TOLBI_SHIP_DECK);
  FieldScript_End();
}

/// Demonstrates setting a Djinni and placing it on standby.
/// Called by EventScript_MeetFlint; sets and clears FLAG_DJINN_LESSON_PART_A
/// and _PART_B to choose the menu's two lessons.
static void WorldMap_ShowDjinnTutorial(void) {
  WorldMap_EndScript();
  FieldEffect_ScheduleRatioTransition(Q16_ONE, 6);
  FieldScript_WaitForZoomTransition();
  WorldMap_StartScript();
  ObjectMotion_SetVarCbAndRefresh(LOCALID_VENUS_DJINNI, 2);
  FieldScript_SetMessageId(MsgWorldMapAfterBringingDjinniIntoYour);
  Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
  FieldScript_WaitFrames(30);
  Script_PlaySong(SE_MENU_CURSOR_MOVE);
  Menu_AnimateSelectionToEntry(0, 2);
  Script_ClearFlag(FLAG_DJINN_LESSON_PART_A);
  Script_ClearFlag(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  ObjectMotion_Launch(LOCALID_VENUS_DJINNI, 4, 30);
  FieldScript_SetMessageId(MsgWorldMapNextIllShowHowCan);
  Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
  GameFlag_Clear(FLAG_DJINN_LESSON_PART_A);
  GameFlag_Set(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  FieldScript_WaitFrames(30);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 6);
}

/// Walking up to Flint: he joins the party and shows how Djinn are set and put
/// on standby; then comes the tutorial battle.
/// Reached by walking onto MB_WORLD_MAP_FLINT until Flint is caught
/// (FLAG_DJINNI(DJINNI_FLINT)). Sets FLAG_DJINN_LESSON and its two part
/// flags for each lesson, and clears them all after the battle.
static void EventScript_MeetFlint(void) {
  struct ObjectEvent *djinni;
  struct ObjectEvent *leader;
  s32 x;
  s32 z;
  s32 count;
  s32 i;

  count = 0;
  djinni = ObjectEvent_GetById(LOCALID_VENUS_DJINNI);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  x = (leader->x.fixed - PIXELS(5584)) / 2 + PIXELS(5584);
  z = (leader->z.fixed - PIXELS(1328)) / 2 + PIXELS(1328);
  // Introduce the Djinni and demonstrate how it joins the party.
  if (!Script_GetFlag(FLAG_DJINN_LESSON)) {
    Party_RestoreHpPp(1);
    GameFlag_Set(FLAG_DJINN_LESSON);
    Script_Start();
    leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    if (leader != 0)
      ObjectEvent_SetPosition(LOCALID_VENUS_DJINNI, leader->x.fixed,
                              leader->z.fixed);
    Djinn_Give(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT));
    DjinnRecovery_AddStandby(0, 0, 0);
    WorldMap_StartScript();
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_VENUS_DJINNI, 0);
    Script_Wait(10);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          60);
    djinni->work = 1;
    ObjectEvent_FaceObjectEvent(LOCALID_VENUS_DJINNI, LOCALID_PARTY_LEADER, 0);
    Task_Wait(16);
    Script_SetMessage(MsgWorldMapOh);
    Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
    WorldMap_EndScript();
    FieldEffect_ScheduleRatioTransition(6 * Q16_ONE / 5, 6);
    FieldScript_WaitForZoomTransition();
    WorldMap_StartScript();
    djinni->motionFlags = OBJ_EVENT_FALLS;
    djinni->gravity = PIXELS(1) / 4;
    djinni->speed = PIXELS(1);
    djinni->acceleration = PIXELS(1);
    djinni->velocityY = 0;
    djinni->groundHeight = 0;
    Object_SetPosition(djinni, PIXELS(5584), 0, PIXELS(1328));
    for (i = 0; i < 16; i++) {
      djinni->scaleX += Q16_ONE / 32;
      djinni->scaleY += Q16_ONE / 32;
      Task_Wait(1);
    }
    ObjectEvent_FaceObjectEvent(LOCALID_VENUS_DJINNI, LOCALID_PARTY_LEADER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_VENUS_DJINNI, 0);
    Task_Wait(16);
    djinni->update = 0;
    ObjectEvent_SetColorMode(djinni, SPRITE_COLOR_NORMAL);
    djinni->gravity = PIXELS(1);
    Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
    Script_PlaySong(SE_131);
    FieldEffect_LoadActionEffectResources(140, 0);
    for (i = 59; i >= 0; i--) {
      if (gFrameCount & 2)
        ObjectEvent_SetColorMode(djinni, SPRITE_COLOR_WHITE);
      else
        ObjectEvent_SetColorMode(djinni, SPRITE_COLOR_NORMAL);
      if ((gFrameCount & 15) == 0)
        WorldMap_CreateLinkedEffects(djinni);
      Task_Wait(1);
    }
    FieldEffect_End();
    ObjectEvent_SetColorMode(djinni, SPRITE_COLOR_NORMAL);
    ObjectEvent_RunRepeatedMotion(LOCALID_VENUS_DJINNI, 2);
    Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          30);
    Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          30);
    ObjectEvent_WalkToAndWait(LOCALID_VENUS_DJINNI, x >> 16, z >> 16);
    ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
    Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          40);
    ObjectEvent_Jump(LOCALID_VENUS_DJINNI, 4, 30);
    MessageArgument_Push(300, 4);
    Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
    ObjectEvent_RunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
    Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
    ObjectEvent_Jump(LOCALID_VENUS_DJINNI, 2, 30);
    Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
    i = 0;
    djinni->motionFlags = 0;
    Object_SetPosition(djinni, x, PIXELS(16), z);
    for (; i < 16; i++) {
      djinni->facing += FACING_STEP;
      Task_Wait(1);
    }
    ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
    Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
    djinni->motionFlags = OBJ_EVENT_FALLS;
    djinni->velocityY = 0;
    djinni->groundHeight = 0;
    for (i = 7; i >= 0; i--) {
      djinni->facing += FACING_STEP;
      Task_Wait(1);
    }
    ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
    Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
    ObjectEvent_ShowEmote(LOCALID_VENUS_DJINNI, EMOTE_IN_FRONT | EMOTE_WINCE,
                          30);
    ObjectEvent_FaceObjectEvent(LOCALID_VENUS_DJINNI, LOCALID_PARTY_LEADER, 0);
    ObjectEvent_RunRepeatedMotion(LOCALID_VENUS_DJINNI, 2);
    Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
    ObjectEvent_Jump(LOCALID_VENUS_DJINNI, 2, 30);
    Script_OpenMessage(LOCALID_VENUS_DJINNI, 0);
    // Each "No" makes Flint plead again; the seventh time he gives up asking.
    count = 0;
  plead:
    if (Script_ChooseYesNo(0, 0) == 1) {
      ObjectEvent_Jump(LOCALID_VENUS_DJINNI, 2, 20);
      ObjectEvent_Jump(LOCALID_VENUS_DJINNI, 2, 20);
      if (count == 6) {
        Script_SetMessage(MsgWorldMapMeanieDontCare);
        Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
        goto listened;
      }
      Script_SetMessage(count + MsgWorldMapComePromiseWont);
      Script_OpenMessage(LOCALID_VENUS_DJINNI, 0);
      count++;
      goto plead;
    }
    ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
    ObjectEvent_Jump(LOCALID_VENUS_DJINNI, 2, 20);
    ObjectEvent_Jump(LOCALID_VENUS_DJINNI, 4, 20);
    Script_SetMessage(MsgWorldMapSeeWontRegret);
    Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
  listened:
    MessageArgument_Push(300, 4);
    Script_PlaySong(MUS_JOINED);
    count = MsgWorldMapAbilityVenusDjinni;
    Message_ShowCentered(count++, 3);
    Script_SetMessage(count);
    ObjectEvent_Jump(LOCALID_VENUS_DJINNI, 2, 20);
    Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
    Script_PlaySong(MUS_009);
    WorldMap_ShowDjinnTutorial();
    WorldMap_EndScript();
    return;
  }
  Script_Start();
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0)
    ObjectEvent_SetPosition(LOCALID_VENUS_DJINNI, leader->x.fixed,
                            leader->z.fixed);
  djinni->velocityY = PIXELS(10);
  Object_SetPosition(djinni, x, 0, z);
  Script_Wait(30);
  WorldMap_StartScript();
  ObjectEvent_FaceObjectEvent(LOCALID_VENUS_DJINNI, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_VENUS_DJINNI, 0);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  // Offer the tutorial again, then demonstrate setting and standby.
  Script_SetMessage(MsgWorldMapSeeDjinnUseful);
  ObjectEvent_Jump(LOCALID_VENUS_DJINNI, 2, 20);
  ObjectEvent_Jump(LOCALID_VENUS_DJINNI, 2, 20);
  Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
  ObjectEvent_RunRepeatedMotion(LOCALID_VENUS_DJINNI, 2);
  Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
  Script_PlaySong(SE_MENU_CURSOR_MOVE);
  Menu_AnimateSelectionToEntry(0, 2);
  GameFlag_Set(FLAG_DJINN_LESSON_PART_A);
  GameFlag_Clear(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  Script_SetMessage(MsgWorldMapGoSetStandby);
  Object_SetPosition(djinni, PIXELS(5584), 0, PIXELS(1328));
  Script_Wait(30);
  Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_VENUS_DJINNI, LOCALID_PARTY_LEADER, 0);
  Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
  Script_OpenMessage(LOCALID_VENUS_DJINNI, 0);
  if (Script_ChooseYesNo(0, 0) == 1) {
    ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
    ObjectEvent_RunRepeatedMotion(LOCALID_VENUS_DJINNI, 2);
    Script_SetMessage(MsgWorldMapHmmmmExplainAgain);
    Script_OpenMessage(LOCALID_VENUS_DJINNI, 0);
    if (Script_ChooseYesNo(0, 0) != 1) {
      Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
      ObjectEvent_WalkToAndWait(LOCALID_VENUS_DJINNI, x >> 16, z >> 16);
      WorldMap_ShowDjinnTutorial();
      WorldMap_EndScript();
      return;
    }
  }
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  Script_SetMessage(MsgWorldMapYeahWantLearn);
  ObjectEvent_Jump(LOCALID_VENUS_DJINNI, 2, 20);
  ObjectEvent_Jump(LOCALID_VENUS_DJINNI, 2, 20);
  ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectEvent_ShowEmote(LOCALID_VENUS_DJINNI,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
  Script_SetFlag(FLAG_DJINN_LESSON_PART_A);
  Script_SetFlag(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Run();
  ObjectEvent_Jump(LOCALID_VENUS_DJINNI, 2, 20);
  Script_ShowMessage(LOCALID_VENUS_DJINNI, 0);
  WorldMap_EndScript();
  Djinn_JoinParty(LOCALID_VENUS_DJINNI, ELEMENT_VENUS,
                  DJINNI_INDEX(DJINNI_FLINT));
  Script_PlaySong(MUS_WORLD_MAP);
  Script_End();
  GameFlag_Clear(FLAG_DJINN_LESSON);
  GameFlag_Clear(FLAG_DJINN_LESSON_PART_A);
  GameFlag_Clear(FLAG_DJINN_LESSON_PART_B);
}

s32 gWorldMapIodem;

/// Brings Bunza's wagon to a stop and lets Isaac step down, after Hammet's
/// return home (kalay.c's Kalay_HammetReturnsToPalace).
/// Called by WorldMap_Initialize at WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH.
static void WorldMap_StepDownFromBunzasWagon(void) {
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(LOCALID_KALAY_WAGON);
  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  WaitFrames(1);
  objectEvent->scaleX = 3 * Q16_ONE / 2;
  objectEvent->scaleY = 3 * Q16_ONE / 2;
  objectEvent->facing = FACING_SOUTH;
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  ObjectEvent_SetPosition(LOCALID_KALAY_WAGON, PIXELS(5480), PIXELS(2104));
  WaitFrames(1);
  Script_PlaySong(SE_RUMBLE);
  ObjectEvent_SetSpeed(LOCALID_KALAY_WAGON, PIXELS(8) / 5, PIXELS(2) / 5);
  Engine_ObjectEventSetAnimation(LOCALID_KALAY_WAGON, ANIM_WALK);
  ObjectEvent_MoveToAndWait(LOCALID_KALAY_WAGON, 5485, 2136);
  FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(5560), -1, PIXELS(2136), 1);
  ObjectEvent_MoveToAndWait(LOCALID_KALAY_WAGON, 5534, 2136);
  ObjectEvent_MoveToAndWait(LOCALID_KALAY_WAGON, 5544, 2158);
  ObjectEvent_MoveToAndWait(LOCALID_KALAY_WAGON, 5608, 2168);
  Engine_ObjectEventSetAnimation(LOCALID_KALAY_WAGON, ANIM_STAND);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(5592), PIXELS(2168));
  WaitFrames(1);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 6, 0);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 5576, 2168);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 40);
  Script_PlaySong(SE_RUMBLE);
  Engine_ObjectEventSetAnimation(LOCALID_KALAY_WAGON, ANIM_WALK);
  ObjectEvent_MoveToAndWait(LOCALID_KALAY_WAGON, 5624, 2168);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  ObjectEvent_MoveToAndWait(LOCALID_KALAY_WAGON, 5624, 2104);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_MoveToAndWait(LOCALID_KALAY_WAGON, 5565, 2104);
  ObjectEvent_MoveToAndWait(LOCALID_KALAY_WAGON, 5560, 2131);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 0);
  ObjectEvent_MoveToAndWait(LOCALID_KALAY_WAGON, 5490, 2136);
  ObjectEvent_MoveToAndWait(LOCALID_KALAY_WAGON, 5480, 2104);
  ObjectEvent_SetPosition(LOCALID_KALAY_WAGON, 0, 0);
  Script_PlaySong(SONG_FADE_OUT_ACTION_SE);
  FieldScript_WaitFrames(40);
  FieldCamera_MoveTo(PIXELS(5592), -1, PIXELS(2136), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 5592, 2136);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Engine_ScriptRequestWarp(WORLD_MAP_TO_KALAY_EAST);
  FieldScript_End();
}

/// Out of Gateway Cave: Iodem sees Babi's ship scuttled off the coast, gives
/// Isaac the Black Orb and prompts him to use it on the ship.
/// Called by WorldMap_Initialize at WORLD_MAP_FROM_GATEWAY_CAVE until the Black
/// Orb is handed back and before the wounded soldiers are seen in Lalivero
/// (FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN). Sets FLAG_MAP_TEMP_34 and
/// FLAG_SAW_SHIP_AT_IDEJIMA, and turns Gateway Cave's warp into
/// EventScript_IodemCallsLeaderBack.
static void WorldMap_RunBlackOrbScene(void) {
  struct ObjectEvent *leader;

  FieldScript_Start();
  gWorldMapIodem = LOCALID_IODEM;
  WorldMap_BlockGatewayCaveWarp(LOCALID_IODEM);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  WorldMap_StartScript();
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ObjectEventSetPosition(gWorldMapIodem, leader->x.fixed,
                                  leader->z.fixed);
  }
  ObjectEvent_SetSpeed(gWorldMapIodem, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkToAndWait(gWorldMapIodem, 5992, 3448);
  ObjectMotion_ArmCallback(gWorldMapIodem, FACING_EAST, 20);
  FieldEffect_SpawnLinked(gWorldMapIodem, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          60);
  ObjectMotion_SetVariantCallback(gWorldMapIodem, 2);
  FieldScript_SetMessageId(MsgWorldMapLook);
  FieldScript_ShowMessageThenWait(gWorldMapIodem | SPEAKER_PORTRAIT_LEFT, 0,
                                  10);
  ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  WorldMap_EndScript();
  FieldEffect_ScheduleRatioTransition(7 * Q16_ONE / 5, 10);
  FieldCamera_SetSpeedParameters(PIXELS(8), PIXELS(1));
  FieldCamera_MoveWithinMapBounds(PIXELS(6024), -1, PIXELS(3432), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  WorldMap_StartScript();
  FieldEffect_ConfigureLinkedObject(gWorldMapIodem, ATTACHED_EFFECT_IN_FRONT |
                                                        ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(gWorldMapIodem, 1);
  FieldScript_ShowMessageThenWait(
      gWorldMapIodem | SPEAKER_PORTRAIT_LEFT | SPEAKER_PORTRAIT_RIGHT, 0, 40);
  ObjectEvent_WalkToAndWait(gWorldMapIodem, 5992, 3400);
  ObjectMotion_SetPositionAndReset(gWorldMapIodem, 6036, 3400);
  ObjectMotion_ArmCallback(gWorldMapIodem, FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, gBlackOrbLeaderScript);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(gWorldMapIodem, 1);
  FieldScript_ShowMessageThenWait(gWorldMapIodem, 0, 10);
  while (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->timer == 0) {
    WaitFrames(1);
  }
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 20);
  FieldEffect_SpawnLinked(gWorldMapIodem, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  ObjectMotion_SetVarCbAndRefresh(gWorldMapIodem, 2);
  FieldScript_ShowMessageThenWait(gWorldMapIodem, 0, 10);
  ObjectMotion_ArmCallback(gWorldMapIodem, FACING_WEST, 10);
  FieldScript_ShowMessageAndWait(gWorldMapIodem, 0);
  ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_EAST, 20);
  ObjectEvent_GetById(gWorldMapIodem)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(gWorldMapIodem, 6028, 3400);
  FieldScript_WaitFrames(1);
  ObjectEvent_GetById(gWorldMapIodem)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  FieldScript_WaitFrames(20);
  ObjectEvent_GetById(gWorldMapIodem)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectMotion_SetPositionAndReset(gWorldMapIodem, 6036, 3400);
  FieldScript_WaitFrames(1);
  ObjectEvent_GetById(gWorldMapIodem)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  FieldEffect_StartEffectObject22(ITEM_BLACK_ORB, 3);
  PartyInventory_GiveItem(ITEM_BLACK_ORB, 0);
  ObjectMotion_SetVarCbAndRefresh(gWorldMapIodem, 1);
  FieldScript_SetMessageId(MsgWorldMapNowUseOnShip);
  FieldScript_ShowMessageThenWait(gWorldMapIodem, 0, 10);
  ObjectMotion_ArmCallback(gWorldMapIodem, FACING_SOUTHEAST + FACING_STEP, 0);
  WorldMap_EndScript();
  FieldEffect_ScheduleRatioTransition(Q16_ONE, 10);
  FieldScript_WaitFrames(20);
  GameFlag_Set(FLAG_MAP_TEMP_34);
  GameFlag_Set(FLAG_SAW_SHIP_AT_IDEJIMA);
  gSaveBlock.savedMap = (s32)&MapId_WorldMap;
  gSaveBlock.savedWarpDest = WORLD_MAP_AFTER_BLACK_ORB_DEFEAT;
  FieldScript_End();
}

/// Turns Gateway Cave's warp (the map event of LOCALID_MAPSEC_GATEWAY_CAVE's
/// COORD_EVENT_MAPSEC) into a coord event that runs
/// EventScript_IodemCallsLeaderBack, and stands Iodem (LOCALID_IODEM, whose
/// object event template row it finds by his sprite) at pixel (6036, 3400); the
/// object event given is not used. Called by WorldMap_RunBlackOrbScene, and by
/// WorldMap_Initialize when the party returns to WORLD_MAP_FROM_GATEWAY_CAVE
/// with the orb still to use.
static void WorldMap_BlockGatewayCaveWarp(s32 localId) {
  s32 i;

  i = 0;
  while (1) {
    if (gWorldMapEvents[i].kind == MAP_EVENT_WARP &&
        gWorldMapEvents[i].trigger ==
            COORD_EVENT_MAPSEC(LOCALID_MAPSEC_GATEWAY_CAVE)) {
      gWorldMapEvents[i].kind = MAP_EVENT_COORD;
      gWorldMapEvents[i].value = (u32)EventScript_IodemCallsLeaderBack;
    }
    if (gWorldMapEvents[i].kind == MAP_EVENTS_END)
      break;
    i++;
  }
  for (i = 0;; i++) {
    if (gWorldMapObjectEventTemplates[i].sprite == SPRITE_IODEM) {
      gWorldMapObjectEventTemplates[i].x = PIXELS(6036);
      gWorldMapObjectEventTemplates[i].z = PIXELS(3400);
      gWorldMapObjectEventTemplates[i].facing = FACING_SOUTHEAST + FACING_STEP;
      return;
    }
  }
}

/// Makes touching Gateway Cave a warp again, to WORLD_MAP_TO_GATEWAY_CAVE.
/// Called by EventScript_UseBlackOrb once the orb is handed back.
static void WorldMap_RestoreGatewayCaveWarp(void) {
  s32 i;

  i = 0;
  while (1) {
    if (gWorldMapEvents[i].kind == MAP_EVENT_COORD &&
        gWorldMapEvents[i].trigger ==
            COORD_EVENT_MAPSEC(LOCALID_MAPSEC_GATEWAY_CAVE)) {
      gWorldMapEvents[i].kind = MAP_EVENT_WARP;
      gWorldMapEvents[i].value = WORLD_MAP_TO_GATEWAY_CAVE;
      break;
    }
    if (gWorldMapEvents[i].kind == MAP_EVENTS_END)
      break;
    i++;
  }
}

/// Talking to Iodem, who gave the Black Orb (or reading his mind): he urges
/// its use. "Now use it... on the ship."
/// Reached by talking to LOCALID_IODEM, or by Mind Read on him.
static void EventScript_TalkAboutBlackOrb(void) {
  FieldScript_Start();
  ObjectEvent_FaceObjectEvent(LOCALID_IODEM, LOCALID_PARTY_LEADER, 0);
  FieldScript_SetMessageId(MsgWorldMapNowUseOnShip);
  FieldScript_ShowMessageAndWait(gWorldMapIodem, 0);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldScript_End();
}

/// Walking back into Gateway Cave with the Black Orb unused: Iodem calls the
/// leader back to the coast. "Isaac, where you going? I said to use the Orb on
/// Babi's ship!"
/// Reached by touching Gateway Cave while WorldMap_BlockGatewayCaveWarp has
/// made it a coord event.
static void EventScript_IodemCallsLeaderBack(void) {
  FieldScript_Start();
  WorldMap_StartScript();
  FieldScript_SetMessageId(MsgWorldMapIsaacWhereGoingSaidUse);
  FieldScript_ShowMessageAndWait(gWorldMapIodem, 0);
  WorldMap_EndScript();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 6008, 3400);
  FieldScript_End();
}

/// Searching the wreck off Gateway Cave: "The wreckage of a ship scuttled off
/// the coast." While Iodem waits for the Black Orb to be used
/// (FLAG_MAP_TEMP_34) the main menu opens as the search ends, so that the orb
/// can be used.
/// Reached by searching MB_WORLD_MAP_SHIPWRECK.
static void EventScript_SearchShipwreck(void) {

  FieldScript_Start();
#if !TBS_PATCH_3
  WorldMap_StartScript();
#endif
  FieldMessage_Show(MsgWorldMapWreckageShipScuttledOffCoast, 1);
  if (GameFlag_Get(FLAG_MAP_TEMP_34) != 0) {
    gFieldWork->menu = 1;
  }
#if !TBS_PATCH_3
  WorldMap_EndScript();
#endif
  FieldScript_End();
}

/// Using the Black Orb on the shipwreck: nothing happens, the orb is taken back
/// and the party is to press on to Babi Lighthouse.
/// Reached by using the Black Orb on MB_WORLD_MAP_SHIPWRECK until it is handed
/// back. Clears FLAG_MAP_TEMP_34 and sets FLAG_BLACK_ORB_RETURNED.
static void EventScript_UseBlackOrb(void) {
  struct ObjectEvent *iodem;
  struct ObjectEvent *orb;
  struct ObjectEvent *leader;
  struct ObjectEvent *objectEvent;
  s32 message;
  struct Sprite *sprite;
  u8 *buffer;

  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  Script_Start();
  WorldMap_EndScript();
  FieldEffect_ScheduleRatioTransition(7 * Q16_ONE / 5, 6);
  FieldCamera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  FieldCamera_MoveTo(PIXELS(6024), -1, PIXELS(3432), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  iodem = NULL;
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  leader->paused = 0;
  ObjectDispatch_StartIdle((struct Object *)leader);
  if (leader->z.fixed > PIXELS(3432)) {
    if (leader->x.fixed > PIXELS(5998)) {
      Object_SetPosition(leader, PIXELS(5998), leader->y.fixed, PIXELS(3453));
      Object_WaitForMove(leader);
    }
  } else if (leader->x.fixed > PIXELS(6010)) {
    // BUG: iodem is NULL here; this branch reads its height through null.
    Object_SetPosition(leader, PIXELS(6010), iodem->y.fixed, PIXELS(3400));
    Object_WaitForMove(leader);
  }
  Object_SetPosition(leader, PIXELS(5993), 0, PIXELS(3432));
  Object_WaitForMove(leader);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 40);
  WorldMap_StartScript();
  ObjectEvent_RunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Script_Wait(20);
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
  // Present the orb icon before its sparks and the ship's response.
  orb = ObjectEvent_Create(SPRITE_BLANK, leader->x.fixed + PIXELS(2),
                           PIXELS(38), leader->z.fixed);
  if (orb != NULL) {
    u8 *motion;

    orb->motionFlags = 0;
    sprite = orb->sprite;
    sprite->flags = 0;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    /* FAKEMATCH(dead): a second clear through a byte pointer, removed after
     * reload, keeps the zero in r1 and the address in r0; without it, 11 of
     * 363 instructions differ in tbs-en. */
    motion = &orb->motionFlags;
    *motion = 0;
    buffer = Heap_Allocate(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    Item_LoadIcon(ITEM_BLACK_ORB);
    Vram_Load(sprite->slot, 128, buffer + 0x400);
    Heap_Release(HEAP_SLOT_GLYPH_TRANSFER);
    Script_Wait(20);
    orb->update = WorldMap_PulseOrbAndSpawnSparks;
    Script_Wait(80);
  }
  ObjectEvent_GetById(gWorldMapIodem)->facing = FACING_SOUTHEAST + FACING_STEP;
  ObjectEvent_ShowEmote(gWorldMapIodem, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_RunRepeatedMotion(gWorldMapIodem, 2);
  message = MsgWorldMapMatter;
  Script_SetMessage(message);
  Script_ShowMessageAndWait(gWorldMapIodem, 0, 80);
  if (orb != NULL) {
    ObjectEvent_Release(orb);
  }
  ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  Script_Wait(40);
  ObjectEvent_Jump(gWorldMapIodem, 6, 40);
  Script_ShowMessageAndWait(gWorldMapIodem, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(gWorldMapIodem, FACING_NORTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(
      gWorldMapIodem | SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT, 0, 40);
  ObjectEvent_SetAnimationAndWait(gWorldMapIodem, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(
      gWorldMapIodem | SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT, 0, 20);
  ObjectEvent_FaceDirection(gWorldMapIodem, FACING_SOUTHEAST + FACING_STEP, 20);
  Script_ShowMessageAndWait(gWorldMapIodem, 0, 10);
  ObjectEvent_SetSpeed(gWorldMapIodem, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetAnimation(gWorldMapIodem, ANIM_WALK);
  iodem = ObjectEvent_Get(LOCALID_IODEM);
  Object_SetPosition(iodem, PIXELS(6010), iodem->y.fixed, PIXELS(3400));
  Object_WaitForMove(iodem);
  Object_SetPosition(iodem, PIXELS(6001), 0, PIXELS(3416));
  Object_WaitForMove(iodem);
  ObjectEvent_SetAnimation(LOCALID_IODEM, ANIM_STAND);
  ObjectEvent_FaceDirection(gWorldMapIodem, FACING_SOUTH + FACING_STEP, 10);
  ObjectEvent_RunRepeatedMotion(gWorldMapIodem, 1);
  Script_ShowMessageAndWait(gWorldMapIodem | SPEAKER_PORTRAIT_LEFT, 0, 20);
  ObjectEvent_SetSpeed(gWorldMapIodem, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_GetById(LOCALID_IODEM)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetAnimation(LOCALID_IODEM, ANIM_WALK);
  Object_SetPosition(iodem, PIXELS(5997), 0, PIXELS(3424));
  Object_WaitForMove(iodem);
  ObjectEvent_SetAnimation(LOCALID_IODEM, ANIM_STAND);
  Script_Wait(10);
  ObjectEvent_SetAnimation(LOCALID_IODEM, ANIM_WALK);
  Object_SetPosition(iodem, PIXELS(6001), 0, PIXELS(3416));
  Object_WaitForMove(iodem);
  ObjectEvent_SetAnimation(LOCALID_IODEM, ANIM_STAND);
  Message_ShowCentered(message + 6, 1);
  gFieldWork->message++;
  PartyInventory_Discard(ITEM_BLACK_ORB);
  Script_Wait(20);
  ObjectEvent_SetAnimation(gWorldMapIodem, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(gWorldMapIodem, 0, 10);
  ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectEvent_SetAnimationAndWait(gWorldMapIodem, ANIM_NOD);
  ObjectEvent_SetAnimation(gWorldMapIodem, ANIM_WALK);
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (objectEvent != NULL) {
    ObjectEvent_SetDestination(gWorldMapIodem, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  ObjectEvent_WaitForMove(gWorldMapIodem);
  ObjectEvent_SetPosition(gWorldMapIodem, 0, 0);
  WorldMap_EndScript();
  FieldEffect_ScheduleRatioTransition(Q16_ONE, 6);
  Script_Wait(20);
  WorldMap_RestoreGatewayCaveWarp();
  ObjectEvent_DestroyById(gWorldMapIodem);
  GameFlag_Clear(FLAG_MAP_TEMP_34);
  GameFlag_Set(FLAG_BLACK_ORB_RETURNED);
  Script_End();
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

/// Queues one frame of the lighthouse transition, a whole tile map, into BG0's
/// screen block.
#define WORLD_MAP_QUEUE_TRANSITION_FRAME(buffer, offset)                       \
  IO_WRITE_QUEUE_PUSH((u32)BG_SCREEN_BLOCK(4), (u32)((buffer) + (offset)),     \
                      (DMA_ENABLE | DMA_32BIT) << 16 | 0x140)

/// The ending, on the island Jenna and Kraden are adrift on: they talk of the
/// lit Venus beacon and of Felix and Isaac until Alex comes and leads them to
/// Felix and Sheba, and the title screen returns (TITLE_FROM_WORLD_MAP).
/// Called by WorldMap_Initialize at WORLD_MAP_JENNA_AND_KRADEN_ADRIFT.
static void WorldMap_ShowEndingScene(void) {
  WindowColor_Apply(gSaveBlock.windowColor[0], gSaveBlock.windowColor[1]);
  FieldScript_Start();
  FieldEffect_ScheduleRatioTransition(Q16_ONE, 300);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Engine_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_JENNA_LIE);
  Engine_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_KRADEN_LIE);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  WaitFrames(1);
  FieldEffect_ScheduleRatioTransition(3 * Q16_ONE / 2, 16);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 1);
  gFieldWork->transitionFrames = 16;
  FieldScript_StartScreenTransition();
  FieldScript_WaitForZoomTransition();
  WorldMap_StartScript();
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 1);
  FieldScript_WaitFrames(20);
  // Jenna and Kraden talk on the drifting island before Alex arrives.
  FieldScript_SetMessageId(MsgWorldMapKradenHowLongWillIsland);
  Script_ShowMessageAndWait(LOCALID_JENNA, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  Script_ShowMessageAndWait(LOCALID_JENNA, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
  Script_ShowMessageAndWait(LOCALID_JENNA, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 100);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Script_ShowMessageAndWait(LOCALID_JENNA, 0, 20);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  Script_ShowMessageAndWait(LOCALID_JENNA, 0, 10);
  ObjectEvent_SetAttachedEffect(LOCALID_KRADEN, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(80);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 120);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 1);
  Script_ShowMessageAndWait(LOCALID_JENNA, 0, 40);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Script_ShowMessageAndWait(LOCALID_JENNA, 0, 120);
  // Alex comes to show them something and leads them on.
  ObjectEvent_SetSpeed(LOCALID_ALEX, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_SetPosition(LOCALID_ALEX, PIXELS(7644), PIXELS(3460));
  ObjectEvent_WalkToAndWait(LOCALID_ALEX, 7572, 3468);
  ObjectEvent_WalkToAndWait(LOCALID_ALEX, 7560, 3488);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | LOCALID_ALEX, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectMotion_SetAnimationAndWait(LOCALID_ALEX, ANIM_NOD);
  Script_ShowMessage(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | LOCALID_ALEX, 0);
  WorldMap_LoadDefaultCellsAndUpdateBlock();
  FieldScript_WaitFrames(20);
  ObjectEvent_SetActionScript(LOCALID_ALEX, gEndingAlexWalk);
  FieldScript_WaitFrames(80);
  Engine_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_STAND);
  ObjectMotion_Launch(LOCALID_KRADEN, 4, 40);
  Engine_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_STAND);
  ObjectMotion_Launch(LOCALID_JENNA, 4, 60);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST + FACING_STEP, 40);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetActionScript(LOCALID_KRADEN, gEndingKradenWalk);
  FieldScript_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(7) / 10, PIXELS(7) / 80);
  FieldCamera_MoveTo(PIXELS(7736), -1, PIXELS(3528), 1);
  ObjectEvent_SetActionScript(LOCALID_JENNA, gEndingJennaWalk);
  do {
    Engine_ObjectEventSetAnimation(LOCALID_FELIX, ANIM_FELIX_LIE);
    Engine_ObjectEventSetAnimation(LOCALID_SHEBA, ANIM_SHEBA_LIE);
    WaitFrames(1);
  } while (ObjectEvent_GetById(LOCALID_JENNA)->timer == 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_WEST, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_KRADEN, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_JENNA, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  WorldMap_StartScript();
  Script_ShowTwoMessagesAndWait(LOCALID_JENNA, 7, 13, 2, 12, LOCALID_KRADEN, 9,
                                4, 4, 3, 0);
  FieldScript_WaitFrames(20);
  WorldMap_LoadDefaultCellsAndUpdateBlock();
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(7768), -1, PIXELS(3528), 1);
  ObjectEvent_FaceDirection(LOCALID_ALEX, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(8) / 5, PIXELS(4) / 5);
  ObjectEvent_WalkTo(LOCALID_KRADEN, 7804, 3512);
  ObjectEvent_WalkToAndWait(LOCALID_JENNA, 7788, 3544);
  Engine_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_STAND);
  WorldMap_StartScript();
  FieldScript_WaitFrames(80);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 1);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | LOCALID_JENNA, 0, 40);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST, 20);
  ObjectMotion_SetVariantCallback(LOCALID_KRADEN, 2);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 60);
  WorldMap_LoadDefaultCellsAndUpdateBlock();
  Script_PlaySong(SONG_FADE_OUT_BGM);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_StartInterpolation(120);
  WaitFrames(120);
  FieldScript_SetDestination((s32)&MapId_Title, TITLE_FROM_WORLD_MAP);
}

/// The look back before Sheba's fall: the view settles on Venus Lighthouse,
/// which shakes in clouds of dust (WorldMap_UpdateLighthouseQuake) as the
/// colours fade, and WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE leads back up to the
/// aerie, its music kept.
/// Called by WorldMap_Initialize at WORLD_MAP_BEFORE_SHEBAS_FALL; sets
/// FLAG_KEEP_MUSIC.
static void WorldMap_ShakeVenusLighthouse(void) {
  FieldScript_Start();
  Sound_PlaySong(SE_RUMBLE);
  PaletteFade_SetSource(0, 0);
  PaletteFade_SetTarget(0, 0);
  PaletteFade_StartInterpolation(1);
  WaitFrames(2);
  gFieldWork->transitionFrames = 1;
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  ObjectGroup_ConfigureChildValue(LOCALID_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 0);
  WaitFrames(1);
  FieldCamera_SetSpeedParameters(PIXELS(4), PIXELS(1) / 2);
  {
    s32 priority = TASK_PRIORITY_MAP;
    TaskFunc callback = WorldMap_UpdateLighthouseQuake;
    FieldScheduler_AddCallback(callback, priority);
  }
  PaletteFade_SetSource(0, 0);
  PaletteFade_SetTarget(PALETTE_TINT_MUTED, 1);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 2);
  PaletteFade_StartInterpolation(40);
  FieldScript_WaitFrames(240);
  PaletteFade_SetTarget(0, 0);
  PaletteFade_StartInterpolation(80);
  WaitFrames(90);
  Engine_ScriptRequestWarp(WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE);
  GameFlag_Set(FLAG_KEEP_MUSIC);
  FieldScript_End();
}

/// Run each frame while Venus Lighthouse shakes: it puffs up dust at a random
/// spot in the QUAKE_DUST box, and every third frame moves the camera a pixel
/// to one of four points around the lighthouse.
static void WorldMap_UpdateLighthouseQuake(void) {
  u32 x = (u32)Engine_RandomNext() * QUAKE_DUST_WIDTH >> 16;
  u32 z = (u32)Engine_RandomNext() * QUAKE_DUST_DEPTH >> 16;
  struct ObjectEvent *objectEvent;

  objectEvent =
      ObjectEvent_Create(SPRITE_DUST_PUFF, PIXELS(x) + PIXELS(QUAKE_DUST_LEFT),
                         0, PIXELS(z) + PIXELS(QUAKE_DUST_TOP));
  if (objectEvent != NULL) {
    struct Sprite *sprite = objectEvent->sprite;
    s32 scale = (((u32)Engine_RandomNext() << 15) >> 16) + 6 * Q16_ONE / 5;

    sprite->flags = 0;
    sprite->image.priority = 2;
    objectEvent->motionFlags = 0;
    objectEvent->scaleX = scale;
    objectEvent->scaleY = scale;
    ObjectDispatch_SelectAnimation(objectEvent, 1);
    ObjectEvent_SetScript(objectEvent, gQuakeDustScript);
  }
  if (gFrameCount % 3 == 0) {
    switch (((u32)Engine_RandomNext() << 2) >> 16) {
    case 0:
      FieldCamera_MoveWithinMapBounds(PIXELS(6087), -1, PIXELS(3177), 1);
      break;
    case 1:
      FieldCamera_MoveWithinMapBounds(PIXELS(6089), -1, PIXELS(3175), 1);
      break;
    case 2:
      FieldCamera_MoveWithinMapBounds(PIXELS(6089), -1, PIXELS(3177), 1);
      break;
    case 3:
      FieldCamera_MoveWithinMapBounds(PIXELS(6087), -1, PIXELS(3175), 1);
      break;
    }
  }
}

/// A leg of the Tolbi-bound Ship's crossing: out of the ship's interior
/// (TOLBI_SHIP_INTERIORS_TO_WORLD_MAP) the ship sails north to its anchorage
/// off the island, where LOCALID_TOLBI_SHIP stands, and the party goes back
/// into it (WORLD_MAP_TO_TOLBI_SHIP_INTERIORS_ROWING_DECK_ISLAND). Called by
/// WorldMap_Initialize at WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS.
static void WorldMap_SailTolbiShipToIsland(void) {

  struct ObjectEvent *objectEvent;

  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  Engine_ObjectEventSetAnimation(LOCALID_CROSSING_SHIP, ANIM_WALK);
  ObjectEvent_SetPosition(LOCALID_CROSSING_SHIP, PIXELS(4872), PIXELS(808));
  objectEvent = ObjectEvent_Get(LOCALID_CROSSING_SHIP);
  {
    s32 value = FACING_NORTHWEST;

    objectEvent->facing = value;
  }
  WaitFrames(1);
  FieldEffect_ScheduleRatioTransition(6 * Q16_ONE / 5, 1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  FieldCamera_FollowObjectEvent(LOCALID_CROSSING_SHIP, 1);
  WaitFrames(1);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  FieldScript_StartScreenTransition();
  ObjectEvent_SetSpeed(LOCALID_CROSSING_SHIP, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_MoveToAndWait(LOCALID_CROSSING_SHIP, 4824, 712);
  ObjectEvent_MoveToAndWait(LOCALID_CROSSING_SHIP, 4776, 616);
  ObjectEvent_SetSpeed(LOCALID_CROSSING_SHIP, PIXELS(3) / 10, PIXELS(3) / 20);
  ObjectEvent_MoveToAndWait(LOCALID_CROSSING_SHIP, 4776, 472);
  ObjectEvent_SetSpeed(LOCALID_CROSSING_SHIP, PIXELS(1) / 5, PIXELS(1) / 10);
  ObjectEvent_MoveToAndWait(LOCALID_CROSSING_SHIP, 4760, 456);
  ObjectEvent_SetSpeed(LOCALID_CROSSING_SHIP, PIXELS(1) / 10, PIXELS(1) / 20);
  ObjectEvent_MoveToAndWait(LOCALID_CROSSING_SHIP, 4760, 440);
  Engine_ObjectEventSetAnimation(LOCALID_CROSSING_SHIP, ANIM_STAND);
  FieldScript_WaitFrames(40);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Engine_ScriptRequestWarp(
      WORLD_MAP_TO_TOLBI_SHIP_INTERIORS_ROWING_DECK_ISLAND);
}

/// The rowers' balance, as TolbiShipMast_GetRowingBalance (tolbi_ship_mast.c)
/// reckons it: the rowing values at the first and third seats less those at
/// the second and fourth. ROWING_BALANCE_OFF_COURSE takes the Tolbi-bound Ship
/// off course to the island.
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

/// Spawns a puff of the wake behind LOCALID_DRIFTING_ISLAND every 16 frames.
static void WorldMap_SpawnIslandWake(void) {
  struct ObjectEvent *leader;
  struct ObjectEvent *objectEvent;
  struct Sprite *sprite;
  u32 value;

  if ((*(s32 *)&gFrameCount & 15) != 0)
    return;
  leader = ObjectEvent_GetById(LOCALID_DRIFTING_ISLAND);
  objectEvent =
      ObjectEvent_Create(SPRITE_DUST_PUFF, leader->x.fixed - PIXELS(32),
                         leader->y.fixed, leader->z.fixed - PIXELS(16));
  if (objectEvent == 0)
    return;
  objectEvent->scaleX = Q16_ONE / 2;
  objectEvent->scaleY = Q16_ONE / 2;
  sprite = objectEvent->sprite;
  if (((u32)Engine_RandomNext() * 2 >> 16)) {
    s32 back = (((u32)Engine_RandomNext() * 48) >> 16) << 16;

    objectEvent->x.fixed -= back >> 1;
    objectEvent->z.fixed -= back;
  } else {
    value = (((u32)Engine_RandomNext() << 5) >> 16) << 16;
    objectEvent->x.fixed += value;
    value = (s32)value >> 1;
    objectEvent->z.fixed += value;
  }
  sprite->flags = 0;
  sprite->image.priority = leader->sprite->image.priority;
  objectEvent->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
  objectEvent->motionFlags = leader->motionFlags;
  ObjectGroup_SetChildValue(objectEvent, SPRITE_COLOR_BLUE_TINT);
  ObjectDispatch_SelectAnimation(objectEvent, 2);
  ObjectEvent_SetScript(objectEvent, gIslandWakeScript);
}

/// Before the ending, the island Jenna and Kraden are adrift on drifts east
/// with its wake under warm colours, and the map fades to white and leads into
/// their scene (WORLD_MAP_TO_JENNA_AND_KRADEN_ADRIFT).
/// Called by WorldMap_Initialize at WORLD_MAP_FROM_MAIN_MENU, once the cleared
/// game is saved (menu/main_menu.c).
static void WorldMap_ShowIslandDrifting(void) {
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *spriteObjectEvent;

  objectEvent = ObjectEvent_GetById(LOCALID_DRIFTING_ISLAND);
  FieldScript_WaitFrames(60);
  FieldScript_Start();
  FieldEffect_ScheduleRatioTransition(3 * Q16_ONE / 5, 1);
  objectEvent->scaleX = 6 * Q16_ONE / 5;
  objectEvent->scaleY = 6 * Q16_ONE / 5;
  FieldCamera_FollowObjectEvent(LOCALID_DRIFTING_ISLAND, 1);
  WaitFrames(1);
  ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  spriteObjectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  ObjectEvent_SetSpriteFlags(spriteObjectEvent, 0);
  spriteObjectEvent = ObjectEvent_Get(LOCALID_DRIFTING_ISLAND);
  ObjectEvent_SetSpriteFlags(spriteObjectEvent, 0);
  ObjectEvent_SetSpeed(LOCALID_DRIFTING_ISLAND, PIXELS(2) / 5, PIXELS(1) / 5);
  objectEvent->timer = 0;
  ObjectEvent_SetActionScript(LOCALID_DRIFTING_ISLAND, gIslandDriftsEast);
  FieldScheduler_AddCallback(WorldMap_SpawnIslandWake, TASK_PRIORITY_MAP);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 1);
  gFieldWork->transitionFrames = 32;
  FieldScript_StartScreenTransition();
  FieldScript_WaitFrames(120);
  FieldEffect_ScheduleRatioTransition(7 * Q16_ONE / 5, 300);
  FieldScript_WaitFrames(270);
  gFieldWork->transitionFrames = 16;
  PLTT_BACKDROP = 0x7fff;
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Engine_ScriptRequestWarp(WORLD_MAP_TO_JENNA_AND_KRADEN_ADRIFT);
}

/// Run each frame while a storm holds the party in the air: it spins the
/// leader and counts the frames left in the byte at FLAG_MAP_TEMP_F8, paused
/// while a menu is open, raising RAISED_STORM_TIME_UP at the last.
static void WorldMap_UpdateStormTimer(void) {

  struct ObjectEvent *objectEvent;
  struct FieldWork *work;
  s32 progress;

  objectEvent = ObjectEvent_GetById(gSaveBlock.leaderLocalId);
  work = gFieldWork;
  objectEvent->facing = (gFrameCount << 12);

  progress = FieldGameFlag_GetByte(FLAG_MAP_TEMP_F8);
  if (progress != 0) {
    if (progress == 1) {
      work->raisedEvent = RAISED_STORM_TIME_UP;
    } else if (GameFlag_Get(FLAG_MENU_OPEN) == 0) {
      progress -= 1;
    }
  }
  FieldGameFlag_SetByte(FLAG_MAP_TEMP_F8, progress);
}

/// Draws the leader into the storm, lifts him and starts its timer.
/// Called by EventScript_WalkIntoStorm. Sets FLAG_MAP_TEMP_F0 and gives the
/// party 180 frames in the air (FLAG_MAP_TEMP_F8).
static void WorldMap_CatchInStorm(s32 targetLocalId) {
  s32 alreadyTraveling;
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *record;
  s32 id;

  id = gSaveBlock.leaderLocalId;
  objectEvent = ObjectEvent_GetById(id);
  alreadyTraveling = Script_GetFlag(FLAG_MAP_TEMP_F0);
  if (alreadyTraveling == 0) {
    FieldScript_Start();
    ObjectEvent_SetAttachedEffect(id, ATTACHED_EFFECT_IN_FRONT |
                                          ATTACHED_EFFECT_ATTACH);
    Engine_ObjectEventSetAnimation(id, ANIM_LEADER_LOOK_AROUND);
    record = ObjectEvent_GetById(targetLocalId);
    if (record != 0) {
      ObjectEvent_SetDestination(id, record->x.part.pixel,
                                 record->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(id);
    Script_PlaySong(SE_SANDSTORM);
    FieldScheduler_AddCallback(WorldMap_UpdateStormTimer, TASK_PRIORITY_MAP);
    objectEvent->motionFlags = alreadyTraveling;
    Object_SetPosition(objectEvent, objectEvent->x.fixed,
                       objectEvent->y.fixed + PIXELS(32), objectEvent->z.fixed);
    ObjectMotion_CommitCurrentPositionAndActivate(id);
    objectEvent->velocityY = alreadyTraveling;
    objectEvent->motionFlags = OBJ_EVENT_FLOATS;
    gSaveBlock.movementMode = LEADER_MOVEMENT_AIRBORNE;
    GameFlag_Set(FLAG_MAP_TEMP_F0);
    FieldGameFlag_SetByte(FLAG_MAP_TEMP_F8, 180);
    FieldScript_End();
    gFieldWork->battle = 0; // drop any battle this step rolled
  }
}

/// Walking into the storm: the party is lifted up into the air.
/// Reached by walking onto MB_WORLD_MAP_STORM until the storm is done with
/// (FLAG_MAP_TEMP_F1).
static void EventScript_WalkIntoStorm(void) {
  WorldMap_CatchInStorm(LOCALID_STORM);
}

/// Starting to cast Douse on the storm: Douse's rain cloud gathers 80 pixels
/// up, where the party floats (douse.c reads childOption).
/// Reached by the first phase of Douse on the storm (LOCALID_STORM) while it
/// holds the party in the air (FLAG_MAP_TEMP_F0).
static void EventScript_StartDousingStorm(void) {
  ((struct FieldEffectWork *)gFieldEffectWork)->childOption = 1;
}

/// Douse on the storm: the party comes down and the battle with what is in it
/// begins (ENCOUNTER_ZONE_SCRIPTED, entries 5 and 7), as with the Suhalla
/// Desert's storms. Reached by Douse on the storm (LOCALID_STORM) while it
/// holds the party in the air. Clears FLAG_MAP_TEMP_F0, sets FLAG_MAP_TEMP_F1
/// and empties the timer.
static void EventScript_DouseMapStorm(void) {
  struct ObjectEvent *selectedObjectEvent;

  if (gFieldWork->raisedEvent == RAISED_STORM_TIME_UP) {
    gFieldWork->raisedEvent = 0;
  }
  GameFlag_Clear(FLAG_MAP_TEMP_F0);
  GameFlag_Set(FLAG_MAP_TEMP_F1);
  FieldGameFlag_SetByte(FLAG_MAP_TEMP_F8, 0);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 5);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 7);
  selectedObjectEvent = ObjectEvent_GetById(gSaveBlock.leaderLocalId);
  selectedObjectEvent->motionFlags = OBJ_EVENT_FALLS;
}

/// The storm drops the party outside Suhalla when its time in the air runs out.
/// The party and the storm fly off, and the map reopens outside Suhalla, where
/// the desert's storms also drop the party. Reached by RAISED_STORM_TIME_UP,
/// raised as the timer runs out. Sets FLAG_ARRIVE_FALLING and sends the party
/// to WORLD_MAP_FROM_SUHALLA.
static void EventScript_StormDropsParty(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_Get(gSaveBlock.leaderLocalId);
  struct ObjectEvent *other = ObjectEvent_Get(LOCALID_STORM);
  s32 frames;

  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Script_PlaySong(SE_219);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  other->motionFlags = 0;
  objectEvent->motionFlags = 0;
  objectEvent->velocityY = 0;
  objectEvent->frozen = 1;
  other->frozen = 1;
  for (frames = 59; frames >= 0; --frames) {
    objectEvent->velocityY += PIXELS(1) / 5;
    other->velocityY += PIXELS(1) / 5;
    WaitFrames(1);
  }
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_End();
  GameFlag_Set(FLAG_ARRIVE_FALLING);
  FieldScript_SetDestination((s32)&MapId_WorldMap, WORLD_MAP_FROM_SUHALLA);
}

/// Run each frame of the lighthouse transition: it blends BG0, where the
/// transition is drawn, over everything, its own weight flickering between 12
/// and 16 every two frames and the rest's from gWorldMapBlend.
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

/// Run each frame at the end of the lighthouse transition: every third frame
/// it moves the camera a pixel diagonally from the leader.
static void WorldMap_JitterTransitionCamera(void) {

  struct ObjectEvent *record;
  s32 sx;
  s32 sy;
  u32 mode;

  record = ObjectEvent_Get(gSaveBlock.leaderLocalId);
  sx = record->x.part.pixel;
  sy = record->z.part.pixel;
  if (gFrameCount % 3 == 0) {
    mode = (u32)(Random_Next() << 2) >> 16;
    switch (mode) {
    case 0:
      FieldCamera_MoveTo((sx << 16) - PIXELS(1), -1, (sy << 16) + PIXELS(1), 1);
      break;
    case 1:
      FieldCamera_MoveWithinMapBounds((sx << 16) + PIXELS(1), -1,
                                      (sy << 16) - PIXELS(1), 1);
      break;
    case 2:
      FieldCamera_MoveTo((sx << 16) + PIXELS(1), -1, (sy << 16) + PIXELS(1), 1);
      break;
    case 3:
      FieldCamera_MoveTo((sx << 16) - PIXELS(1), -1, (sy << 16) - PIXELS(1), 1);
      break;
    }
  }
}

/// Queues one frame of the lighthouse transition and waits frames.
static inline void WorldMap_ShowTransitionFrame(u8 *buffer, s32 offset,
                                                s32 frames) {
  WORLD_MAP_QUEUE_TRANSITION_FRAME(buffer, offset);
  FieldScript_WaitFrames(frames);
}

/// The lighthouse transition, after a beacon is lit: seven frames, tile maps
/// drawn on BG0 in the lighthouse's colours (gWorldMapPalettes, palette 0 for
/// Mercury and 1 for Venus), are a ball of light over the lighthouse's top
/// that swells frame by frame as the map darkens behind it, holds for 140
/// frames and shrinks again; then the camera shakes and the screen closes for
/// the aerie
/// (https://alchemy.pascalpixel.com/story/venus-lighthouse-lit.gif).
/// Called by WorldMap_Initialize at WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT and
/// WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT. Clears FLAG_KEEP_PARTY_POSITION and
/// sets FLAG_SKIP_MAP_CHANGE_SOUND.
static void WorldMap_PlayLighthouseTransition(s32 palette) {
  u8 *buffer = AllocEwram(0x4000);

  WaitFrames(1);
  GameFlag_Clear(FLAG_KEEP_PARTY_POSITION);
  WorldMap_StartStillView();
  FieldResource_DecodeType01(gWorldMapPackedTiles, buffer);
  FieldResource_DecodeType01(gWorldMapPackedFrames, buffer + 0x1000);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT[14 * 16],
                      (u32)(gWorldMapPalettes + palette * 32),
                      DMA_ENABLE << 16 | 16);
  IO_WRITE_QUEUE_PUSH((u32)BG_CHAR_BLOCK(0) + 0x1000, (u32)buffer,
                      (DMA_ENABLE | DMA_32BIT) << 16 | 0x400);
  FieldScheduler_AddCallback(WorldMap_UpdateTransitionBlend, TASK_PRIORITY_MAP);
  FieldScript_Start();
  WORLD_MAP_QUEUE_TRANSITION_FRAME(buffer, 0x3a80);
  ObjectEvent_GetById(gSaveBlock.leaderLocalId)->spriteStorageKind = 0;
  gFieldWork->transitionFrames = 16;
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  Sound_PlaySong(SE_246);

  // Fade through the inward frames, hold, then reverse the animation.
  gWorldMapBlend = BLEND_ALPHA(0, 14);
  WorldMap_ShowTransitionFrame(buffer, 0x3480, 2);
  gWorldMapBlend = BLEND_ALPHA(0, 13);
  WorldMap_ShowTransitionFrame(buffer, 0x2e80, 2);
  gWorldMapBlend = BLEND_ALPHA(0, 12);
  WorldMap_ShowTransitionFrame(buffer, 0x2880, 2);
  gWorldMapBlend = BLEND_ALPHA(0, 11);
  WorldMap_ShowTransitionFrame(buffer, 0x2280, 2);
  gWorldMapBlend = BLEND_ALPHA(0, 10);
  WorldMap_ShowTransitionFrame(buffer, 0x1c80, 2);
  gWorldMapBlend = BLEND_ALPHA(0, 9);
  WorldMap_ShowTransitionFrame(buffer, 0x1680, 2);
  gWorldMapBlend = BLEND_ALPHA(0, 8);
  WorldMap_ShowTransitionFrame(buffer, 0x1080, 140);

  WorldMap_ShowTransitionFrame(buffer, 0x1680, 4);
  WorldMap_ShowTransitionFrame(buffer, 0x1c80, 4);
  WorldMap_ShowTransitionFrame(buffer, 0x2280, 4);
  gWorldMapBlend = BLEND_ALPHA(0, 9);
  WorldMap_ShowTransitionFrame(buffer, 0x2880, 4);
  gWorldMapBlend = BLEND_ALPHA(0, 10);
  WorldMap_ShowTransitionFrame(buffer, 0x2e80, 4);
  gWorldMapBlend = BLEND_ALPHA(0, 11);
  WorldMap_ShowTransitionFrame(buffer, 0x3480, 4);
  gWorldMapBlend = BLEND_ALPHA(0, 12);
  WORLD_MAP_QUEUE_TRANSITION_FRAME(buffer, 0x3a80);
  WorldMap_LoadAreaGraphics();
  FieldScheduler_AddCallback(WorldMap_JitterTransitionCamera,
                             TASK_PRIORITY_MAP);
  Sound_PlaySong(SE_RUMBLE);
  gWorldMapBlend = BLEND_ALPHA(0, 13);
  FieldScript_WaitFrames(4);
  gWorldMapBlend = BLEND_ALPHA(0, 14);
  FieldScript_WaitFrames(4);
  gWorldMapBlend = BLEND_ALPHA(0, 15);
  FieldScript_WaitFrames(4);
  gWorldMapBlend = BLEND_ALPHA(0, 16);
  FieldScript_WaitFrames(45);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Free(buffer);
  GameFlag_Set(FLAG_SKIP_MAP_CHANGE_SOUND);
}

/// Unused: an update hook that blinks an object event and now and then creates
/// its linked effects; no object event is given it.
UNUSED static void
WorldMap_UpdateBlinkingLinkedEffects(struct ObjectEvent *objectEvent) {
  if ((gFrameCount & 2) != 0) {
    ObjectEvent_SetColorMode(objectEvent, SPRITE_COLOR_WHITE);
  } else {
    ObjectEvent_SetColorMode(objectEvent, SPRITE_COLOR_NORMAL);
  }
  if ((gFrameCount & 15) == 0) {
    WorldMap_CreateLinkedEffects(objectEvent);
  }
}

/// Unused: an update hook that cycles an object event's palettes and now and
/// then creates its linked effects; no object event is given it.
UNUSED static void
WorldMap_UpdateCyclingLinkedEffects(struct ObjectEvent *objectEvent) {
  if ((gFrameCount & 1) != 0) {
    ObjectEvent_SetColorMode(objectEvent, ((u32)((gFrameCount >> 1)) % (u32)6));
  }
  if ((gFrameCount & 15) == 0) {
    WorldMap_CreateLinkedEffects(objectEvent);
  }
}

/// Unused: an update hook that advances an object event's palette on odd
/// frames; no object event is given it.
UNUSED static void
WorldMap_CycleObjectEventPalettes(struct ObjectEvent *objectEvent) {
  if ((gFrameCount & 1) != 0) {
    s32 slot = (gFrameCount >> 1) % 6;

    ObjectEvent_SetColorMode(objectEvent, slot);
  }
}

#include "common/effect_vertical.h"
#include "common/world_map_linked_effects.h"

s32 gCaravanFirstWagonArrives[] = {
    SC_POSITION(PIXELS(5848), 0, PIXELS(1576)),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1614)),
    SC_SYNC,
    SC_MOVE(PIXELS(5864), 0, PIXELS(1624)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gCaravanFirstWagonDeparts[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(5868), 0, PIXELS(1624)),
    SC_SYNC,
    SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(PIXELS(5656), 0, PIXELS(1800)),
    SC_END,
};

s32 gCaravanFirstWagonReturns[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(5800), 0, PIXELS(1800)),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gCaravanWagonHeadsNorth[] = {
    SC_POSITION(PIXELS(5868), 0, PIXELS(1704)),
    SC_WAIT(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(5872), 0, PIXELS(1528)),
    SC_SYNC,
    SC_MOVE(PIXELS(5880), 0, PIXELS(1352)),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gCaravanSecondWagonArrives[] = {
    SC_POSITION(PIXELS(5848), 0, PIXELS(1576)),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1630)),
    SC_SYNC,
    SC_MOVE(PIXELS(5832), 0, PIXELS(1640)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gCaravanSecondWagonDeparts[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(5863), 0, PIXELS(1640)),
    SC_SYNC,
    SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(PIXELS(5640), 0, PIXELS(1768)),
    SC_END,
};

s32 gCaravanSecondWagonReturns[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(5640), 0, PIXELS(1795)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1800)),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gCaravanThirdWagonArrives[] = {
    SC_POSITION(PIXELS(5848), 0, PIXELS(1576)),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1614)),
    SC_SYNC,
    SC_MOVE(PIXELS(5832), 0, PIXELS(1624)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gCaravanThirdWagonDeparts[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(5863), 0, PIXELS(1624)),
    SC_SYNC,
    SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(PIXELS(5656), 0, PIXELS(1768)),
    SC_END,
};

s32 gCaravanThirdWagonReturns[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(5656), 0, PIXELS(1795)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1800)),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gCaravanFourthWagonArrives[] = {
    SC_POSITION(PIXELS(5848), 0, PIXELS(1576)),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1598)),
    SC_SYNC,
    SC_MOVE(PIXELS(5832), 0, PIXELS(1608)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gCaravanFourthWagonDeparts[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(5863), 0, PIXELS(1608)),
    SC_SYNC,
    SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(PIXELS(5672), 0, PIXELS(1768)),
    SC_END,
};

s32 gCaravanFourthWagonReturns[] = {
    SC_ANIMATION(3), SC_MOVE(PIXELS(5672), 0, PIXELS(1795)),
    SC_SYNC,         SC_MOVE(PIXELS(5800), 0, PIXELS(1800)),
    SC_SYNC,         SC_POSITION(0, 0, 0),
    SC_END,
};

s32 gCaravanRockGuideScript[] = {
    SC_SET(SW_SCALE_X, Q16_ONE / 10),
    SC_SET(SW_SCALE_Y, Q16_ONE / 10),
    SC_WAIT(1),
    SC_MOVE(PIXELS(5608), 0, PIXELS(1720)),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 655),
    SC_ADD(SW_SCALE_Y, 655),
    SC_WAIT(1),
    SC_LOOP(160, SC_TO(1)),
    SC_END,
};

s32 gBlackOrbLeaderScript[] = {
    SC_SET(SW_TIMER, 0),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(6008), 0, PIXELS(3400)),
    SC_SYNC,
    SC_MOVE(PIXELS(6020), 0, PIXELS(3400)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_TIMER, 1),
    SC_WAIT(1),
    SC_END,
};

s32 gEndingAlexWalk[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(7564), 0, PIXELS(3476)),
    SC_SYNC,
    SC_MOVE(PIXELS(7576), 0, PIXELS(3464)),
    SC_SYNC,
    SC_MOVE(PIXELS(7640), 0, PIXELS(3464)),
    SC_SYNC,
    SC_MOVE(PIXELS(7656), 0, PIXELS(3480)),
    SC_SYNC,
    SC_MOVE(PIXELS(7720), 0, PIXELS(3480)),
    SC_SYNC,
    SC_MOVE(PIXELS(7752), 0, PIXELS(3496)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(1),
    SC_END,
};

s32 gEndingKradenWalk[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(7564), 0, PIXELS(3476)),
    SC_SYNC,
    SC_MOVE(PIXELS(7576), 0, PIXELS(3464)),
    SC_SYNC,
    SC_MOVE(PIXELS(7640), 0, PIXELS(3464)),
    SC_SYNC,
    SC_MOVE(PIXELS(7656), 0, PIXELS(3480)),
    SC_SYNC,
    SC_MOVE(PIXELS(7704), 0, PIXELS(3512)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

s32 gEndingJennaWalk[] = {
    SC_SET(SW_TIMER, 0),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(7564), 0, PIXELS(3476)),
    SC_SYNC,
    SC_MOVE(PIXELS(7576), 0, PIXELS(3464)),
    SC_SYNC,
    SC_MOVE(PIXELS(7640), 0, PIXELS(3464)),
    SC_SYNC,
    SC_MOVE(PIXELS(7656), 0, PIXELS(3480)),
    SC_SYNC,
    SC_MOVE(PIXELS(7688), 0, PIXELS(3528)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_EAST),
    SC_SET(SW_TIMER, 1),
    SC_WAIT(1),
    SC_END,
};

s32 gQuakeDustScript[] = {
    SC_WAIT(16),
    SC_FREE,
};

s32 gCrossingShipSailsWest[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(PIXELS(-8), 0, 0),
    SC_SYNC,
    SC_LOOP(40, SC_TOP),
    SC_SET(SW_TIMER, 1),
    SC_MOVE_BY(PIXELS(-8), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(PIXELS(-8), 0, 0),
    SC_SYNC,
    SC_END,
};

s32 gCrossingShipVeersNorth[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(PIXELS(-8), 0, PIXELS(-2)),
    SC_SYNC,
    SC_LOOP(40, SC_TOP),
    SC_SET(SW_TIMER, 1),
    SC_MOVE_BY(PIXELS(-8), 0, PIXELS(-2)),
    SC_SYNC,
    SC_MOVE_BY(PIXELS(-8), 0, PIXELS(-2)),
    SC_SYNC,
    SC_END,
};

s32 gIslandDriftsEast[] = {
    SC_MOVE_BY(PIXELS(24), 0, 0),
    SC_SYNC,
    SC_LOOP(10, SC_TOP),
    SC_SET(SW_TIMER, 1),
    SC_MOVE_BY(PIXELS(24), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(PIXELS(24), 0, 0),
    SC_SYNC,
    SC_END,
};

s32 gWorldMapFlintScript[] = {
    SC_CALL(WorldMap_SetUpFlint),
    SC_END,
};

struct MapWarpDest gWorldMapWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 5656, 0, 1256, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {WORLD_MAP_FROM_VALE_GATE, CONDITION_ALWAYS, 5680, 0, 1226, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
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
    // Unused: no warp leads to warp destination 45, 97 or 98.
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
    {WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_FIRST_ATTACK, CONDITION_ALWAYS,
     5384, 0, 2248, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_SECOND_ATTACK,
     CONDITION_ALWAYS, 7944, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_THIRD_ATTACK, CONDITION_ALWAYS,
     7944, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_KRAKEN, CONDITION_ALWAYS, 7944,
     0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_LAND, CONDITION_ALWAYS, 7944,
     0, 200, FACING_SOUTH, 0, -1, 128, -1, -1, 0},
    {WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI, CONDITION_ALWAYS, 5096, 0,
     2328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH, CONDITION_ALWAYS, 5528, 0,
     2120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_MAIN_MENU, CONDITION_ALWAYS, 7464, 0, 200, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {WORLD_MAP_BEFORE_SHEBAS_FALL, CONDITION_ALWAYS, 6088, 0, 3176, FACING_WEST,
     0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_TOLBI_SHIP_DECK_CROSSBONE_ISLE, CONDITION_ALWAYS, 4728, 0,
     424, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS, CONDITION_ALWAYS, 4872, 0, 808,
     FACING_WEST, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_CROSSBONE_ISLE, CONDITION_ALWAYS, 4576, 0, 508,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {WORLD_MAP_FROM_SUHALLA_DESERT_STORM, CONDITION_ALWAYS, 4392, 0, 328,
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
    {MAP_TABLE_END},
};

u32 gWorldMapWarps[] = {
    MAP_WARPS(WorldMap),
    MAP_WARP(WORLD_MAP_TO_VALE_GATE, ValeGate, VALE_GATE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_WORLD_MAP_FROM_TITLE, WorldMap, WORLD_MAP_FROM_TITLE),
    MAP_WARP(WORLD_MAP_TO_VAULT_SOUTH, Vault, VAULT_FROM_WORLD_MAP_SOUTH),
    MAP_WARP(WORLD_MAP_TO_GOMA_CAVE_ENTRANCE, GomaCaveEntrance,
             GOMA_CAVE_ENTRANCE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_BILIBIN, Bilibin, BILIBIN_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_BILIBIN_BARRICADE_WEST, BilibinBarricade,
             BILIBIN_BARRICADE_FROM_WORLD_MAP_WEST),
    MAP_WARP(WORLD_MAP_TO_KOLIMA_WEST, Kolima, KOLIMA_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_KOLIMA_FOREST_ENTRANCE, KolimaForestEntrance,
             KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_IMIL_SOUTH, Imil, IMIL_FROM_HOUSE_SOUTHWEST),
    MAP_WARP(WORLD_MAP_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST,
             MercuryLighthouseExterior,
             MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_KOLIMA_BRIDGE_EAST, KolimaBridge,
             KOLIMA_BRIDGE_FROM_WORLD_MAP_SOUTH),
    MAP_WARP(WORLD_MAP_TO_FUCHIN_TEMPLE_GROUNDS, FuchinTempleGrounds,
             FUCHIN_TEMPLE_GROUNDS_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_MOGALL_FOREST_SIGN, MogallForestSign,
             MOGALL_FOREST_SIGN_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_XIAN, Xian, XIAN_FROM_HOUSE),
    MAP_WARP(WORLD_MAP_TO_ALPINE_CROSSING_EAST, AlpineCrossing,
             ALPINE_CROSSING_FROM_WORLD_MAP_EAST),
    MAP_WARP(WORLD_MAP_TO_ALTIN, Altin, ALTIN_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_LAMA_TEMPLE, LamaTemple, LAMA_TEMPLE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_LAMAKAN_DESERT_EAST, LamakanDesertEast,
             LAMAKAN_DESERT_EAST_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_WORLD_MAP_FROM_KALAY_WEST, WorldMap,
             WORLD_MAP_FROM_KALAY_WEST),
    MAP_WARP(WORLD_MAP_TO_KALAY_EAST, Kalay, KALAY_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_KALAY_DOCKS_EAST, KalayDocks,
             KALAY_DOCKS_FROM_WORLD_MAP_SOUTH),
    MAP_WARP(WORLD_MAP_TO_TOLBI_DOCKS, TolbiDocks, TOLBI_DOCKS_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_TOLBI, Tolbi, TOLBI_FROM_HOUSE_SOUTH),
    MAP_WARP(WORLD_MAP_TO_ALTMILLER_CAVE_ENTRANCE, AltmillerCaveEntrance,
             ALTMILLER_CAVE_ENTRANCE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_GONDOWAN_PASSAGE_EAST, GondowanPassage,
             GONDOWAN_PASSAGE_FROM_WORLD_MAP_NORTH),
    MAP_WARP(WORLD_MAP_TO_GONDOWAN_CAVE_NORTH, GondowanCave,
             GONDOWAN_CAVE_FROM_WORLD_MAP_EAST),
    MAP_WARP(WORLD_MAP_TO_SUHALLA, Suhalla, SUHALLA_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_SUHALLA_DESERT_NORTH, SuhallaDesertNorth,
             SUHALLA_DESERT_NORTH_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_SOUTH, SuhallaGateCheckpoint,
             SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_SOUTH),
    MAP_WARP(WORLD_MAP_TO_LALIVERO_EAST, Lalivero,
             LALIVERO_FROM_WORLD_MAP_EAST),
    MAP_WARP(WORLD_MAP_TO_ROAD_TO_BABI_LIGHTHOUSE, RoadToBabiLighthouse,
             ROAD_TO_BABI_LIGHTHOUSE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_WEST, VenusLighthouseEntry,
             VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_WEST),
    MAP_WARP(WORLD_MAP_TO_GATEWAY_CAVE, GatewayCave,
             GATEWAY_CAVE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_LUNPA, Lunpa, LUNPA_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_BILIBIN_CAVE_ENTRANCE, BilibinCaveEntrance,
             BILIBIN_CAVE_ENTRANCE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_GOMA_CAVE_BRIDGE, GomaCaveBridge,
             GOMA_CAVE_BRIDGE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_GONDOWAN_CAVE_SOUTH, GondowanCave,
             GONDOWAN_CAVE_FROM_WORLD_MAP_WEST),
    MAP_WARP(WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_EAST, SuhallaGateCheckpoint,
             SUHALLA_GATE_CHECKPOINT_FROM_WORLD_MAP_EAST),
    MAP_WARP(WORLD_MAP_TO_MOGALL_FOREST_KILLER_APE, MogallForestKillerApe,
             MOGALL_FOREST_KILLER_APE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_VAULT_NORTH, Vault, VAULT_FROM_WORLD_MAP_NORTH),
    MAP_WARP(WORLD_MAP_TO_BILIBIN_CAVE_ENTRANCE_NORTHEAST, BilibinCaveEntrance,
             BILIBIN_CAVE_ENTRANCE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_KOLIMA_BRIDGE_WEST, KolimaBridge,
             KOLIMA_BRIDGE_FROM_WORLD_MAP_NORTH),
    MAP_WARP(WORLD_MAP_TO_BILIBIN_BARRICADE_EAST, BilibinBarricade,
             BILIBIN_BARRICADE_FROM_WORLD_MAP_EAST),
    MAP_WARP(WORLD_MAP_TO_BILIBIN_CAVE_ICE, BilibinCaveIce,
             BILIBIN_CAVE_ICE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_WEST_LUNPA_CAVE, WestLunpaCave,
             WEST_LUNPA_CAVE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_ALPINE_CROSSING_NORTH, AlpineCrossing,
             ALPINE_CROSSING_FROM_WORLD_MAP_NORTH),
    MAP_WARP(WORLD_MAP_TO_ALPINE_CROSSING_WEST, AlpineCrossing,
             ALPINE_CROSSING_FROM_WORLD_MAP_WEST),
    MAP_WARP(WORLD_MAP_TO_ALTIN_PEAK_WEST_EXIT, AltinPeakWestExit,
             ALTIN_PEAK_WEST_EXIT_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_LAMAKAN_DESERT_CAVE, LamakanDesertCave,
             LAMAKAN_DESERT_CAVE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_GONDOWAN_PASSAGE_WEST, GondowanPassage,
             GONDOWAN_PASSAGE_FROM_WORLD_MAP_SOUTH),
    MAP_WARP(WORLD_MAP_TO_SUHALLA_DESERT_SOUTH, SuhallaDesertSouth,
             SUHALLA_DESERT_SOUTH_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_KALAY_DOCKS_NORTH, KalayDocks,
             KALAY_DOCKS_FROM_WORLD_MAP_NORTH),
    MAP_WARP(WORLD_MAP_TO_LALIVERO_WEST, Lalivero,
             LALIVERO_FROM_WORLD_MAP_WEST),
    MAP_WARP(WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE, KalayTunnelEntrance,
             KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_NORTH),
    MAP_WARP(WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_EAST, VenusLighthouseEntry,
             VENUS_LIGHTHOUSE_ENTRY_FROM_WORLD_MAP_EAST),
    MAP_WARP(WORLD_MAP_TO_TOLBI_SHIP_DECK_CROSSBONE_ISLE, TolbiShipDeck,
             TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS),
    MAP_WARP(WORLD_MAP_TO_CROSSBONE_ISLE, CrossboneIsle,
             CROSSBONE_ISLE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_WORLD_MAP_FROM_VALE_GATE, WorldMap,
             WORLD_MAP_FROM_VALE_GATE),
    MAP_WARP(WORLD_MAP_TO_HAMMETS_CARAVAN_LEAVES, WorldMap,
             WORLD_MAP_HAMMETS_CARAVAN_LEAVES),
    MAP_WARP(WORLD_MAP_TO_TOLBI_SHIP_MAST_FIRST_ATTACK, TolbiShipMast,
             TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK),
    MAP_WARP(WORLD_MAP_TO_TOLBI_SHIP_MAST_SECOND_ATTACK, TolbiShipMast,
             TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK),
    MAP_WARP(WORLD_MAP_TO_TOLBI_SHIP_MAST_THIRD_ATTACK, TolbiShipMast,
             TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK),
    MAP_WARP(WORLD_MAP_TO_TOLBI_SHIP_INTERIORS, TolbiShipInteriors,
             TOLBI_SHIP_INTERIORS_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_TOLBI_SHIP_MAST_LAND_HO, TolbiShipMast,
             TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO),
    MAP_WARP(WORLD_MAP_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
             TOLBI_SHIP_DECK_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE_SOUTH, KalayTunnelEntrance,
             KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH),
    MAP_WARP(WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE, VenusLighthouseAerie,
             VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP),
    MAP_WARP(WORLD_MAP_TO_TOLBI_SHIP_INTERIORS_ROWING_DECK_ISLAND,
             TolbiShipInteriors,
             TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_WORLD_MAP_ISLAND),
    MAP_WARP(WORLD_MAP_TO_JENNA_AND_KRADEN_ADRIFT, WorldMap,
             WORLD_MAP_JENNA_AND_KRADEN_ADRIFT),
    MAP_WARP(WORLD_MAP_TO_LUNPA_INTERIORS, LunpaInteriors,
             LUNPA_INTERIORS_FROM_SUHALLA_GATE_CHECKPOINT),
    MAP_WARPS_END,
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
    SC_CALL(WorldMap_BlinkObjectEventSprite),
    SC_END,
};

// clang-format off
struct ObjectEventTemplate gWorldMapObjectEventTemplates[] = {
    { VENUS_DJINNI, FLAG_DJINNI(DJINNI_VINE), (s32)gWorldMapFlintScript,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MARS_DJINNI, FLAG_DJINNI(DJINNI_CORONA), MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MERCURY_DJINNI, FLAG_DJINNI(DJINNI_HAIL), MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5678), 0, PIXELS(1200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5848), 0, PIXELS(1576), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6976), 0, PIXELS(1624), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5416), 0, PIXELS(2840), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5892), 0, PIXELS(1128), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5422), 0, PIXELS(2232), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5036), 0, PIXELS(2328), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_SNOWY_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5928), 0, PIXELS(270), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_STUMP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7352), 0, PIXELS(1240), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_BLUE_ROOFED_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7544), 0, PIXELS(1976), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_TEMPLE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6888), 0, PIXELS(1712), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_TEMPLE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7880), 0, PIXELS(1560), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6584), 0, PIXELS(1192), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5592), 0, PIXELS(2136), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5996), 0, PIXELS(2894), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(4904), 0, PIXELS(2280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_MERCURY_LIGHTHOUSE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6024), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_MOUNTAIN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5656), 0, PIXELS(1112), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6088), 0, PIXELS(3176), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_BARRICADE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6952), 0, PIXELS(1362), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_BABI_LIGHTHOUSE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5996), 0, PIXELS(2808), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6424), 0, PIXELS(1302), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6484), 0, PIXELS(1184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(4840), 0, PIXELS(1960), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5656), 0, PIXELS(2392), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5480), 0, PIXELS(2600), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_FOREST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7790), 0, PIXELS(2080), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5976), 0, PIXELS(3400), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5988), 0, PIXELS(774), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6224), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5856), 0, PIXELS(1174), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6920), 0, PIXELS(1664), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6092), 0, PIXELS(2200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_FOREST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7166), 0, PIXELS(1054), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_FOREST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7856), 0, PIXELS(1736), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5480), 0, PIXELS(2088), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SIGNPOST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SIGNPOST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SIGNPOST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WORLD_MAP_BRIDGE, CONDITION_ALWAYS, (s32)gWorldMapKolimaBridgeScript,
        PIXELS(7568), 0, PIXELS(1384), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_GATEHOUSE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(4942), 0, PIXELS(2690), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SHIPWRECK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6036), 0, PIXELS(3458), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_SHIP, IF_FLAG_SET(FLAG_TOLBI_SHIP_AT_ISLAND), (s32)gWorldMapAlignToMapScript,
        PIXELS(4760), 0, PIXELS(440), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SANDSTORM, FLAG_MAP_TEMP_F1, (s32)gWorldMapStormScript,
        PIXELS(4392), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_IODEM, CONDITION_ALWAYS, (s32)gWorldMapIodemScript,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
    { LOCALID_GARET, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gWorldMapObjectEventTemplatesHammetsCaravan[] = {
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0xd8, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5848), 0, PIXELS(1576), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gWorldMapObjectEventTemplatesFromWestLunpaCave[] = {
    { SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5892), 0, PIXELS(1128), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5856), 0, PIXELS(1174), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BUNZA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gWorldMapObjectEventTemplatesCrossingEnds[] = {
    { SPRITE_SHIP, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5384), 0, PIXELS(2248), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5592), 0, PIXELS(2136), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5422), 0, PIXELS(2232), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5028), 0, PIXELS(2328), FACING_WEST, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gWorldMapObjectEventTemplatesCrossing[] = {
    { SPRITE_SHIP, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5432), 0, PIXELS(2312), FACING_WEST, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gWorldMapObjectEventTemplatesFromKalayTunnelEntranceSouth[] = {
    { SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5592), 0, PIXELS(2136), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5480), 0, PIXELS(2088), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gWorldMapObjectEventTemplatesJennaAndKradenAdrift[] = {
    { LOCALID_JENNA, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7560), 0, PIXELS(3512), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { LOCALID_SHEBA, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7816), 0, PIXELS(3528), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7544), 0, PIXELS(3496), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_ALEX, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7708), 0, PIXELS(3456), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_FELIX, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7800), 0, PIXELS(3544), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SHIPWRECK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7604), 0, PIXELS(3522), FACING_WEST, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gWorldMapObjectEventTemplatesFromClear[] = {
    { SPRITE_WORLD_MAP_ISLAND, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7464), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gWorldMapObjectEventTemplatesBeforeShebasFall[] = {
    { SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6080), 0, PIXELS(3168), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gWorldMapEvents[] = {
    { MAP_EVENT_COORD, MB_WORLD_MAP_FLINT, FLAG_DJINNI(DJINNI_FLINT), EVENT_SCRIPT(EventScript_MeetFlint) },
    { MAP_EVENT_COORD, MB_WORLD_MAP_STORM, FLAG_MAP_TEMP_F1, EVENT_SCRIPT(EventScript_WalkIntoStorm) },
    { MAP_EVENT_TALK, LOCALID_VENUS_DJINNI, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MeetVine) },
    { MAP_EVENT_TALK, LOCALID_CORONA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MeetCorona) },
    { MAP_EVENT_TALK, LOCALID_HAIL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MeetHail) },
    { MAP_EVENT_COORD, MB_WORLD_MAP_VINE, FLAG_DJINNI(DJINNI_VINE), EVENT_SCRIPT(EventScript_MeetVine) },
    { MAP_EVENT_COORD, MB_WORLD_MAP_CORONA, FLAG_DJINNI(DJINNI_CORONA), EVENT_SCRIPT(EventScript_MeetCorona) },
    { MAP_EVENT_COORD, MB_WORLD_MAP_HAIL, FLAG_DJINNI(DJINNI_HAIL), EVENT_SCRIPT(EventScript_MeetHail) },
    { BG_EVENT(BG_EVENT_SILENT), MB_WORLD_MAP_SHIPWRECK, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchShipwreck) },
    { USE_ITEM(ITEM_BLACK_ORB), MB_WORLD_MAP_SHIPWRECK, FLAG_BLACK_ORB_RETURNED, EVENT_SCRIPT(EventScript_UseBlackOrb) },
    { BG_EVENT(BG_EVENT_GROUND), MB_WORLD_MAP_TURTLE_BOOTS, FLAG_WORLD_MAP_TURTLE_BOOTS, FIND_ITEM(ITEM_TURTLE_BOOTS) },
    { MAP_EVENT_TALK, LOCALID_IODEM, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutBlackOrb) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_IODEM, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutBlackOrb) },
    { MAP_EVENT_WARP, WORLD_MAP_TO_MOGALL_FOREST_SIGN, CONDITION_ALWAYS, WORLD_MAP_TO_MOGALL_FOREST_SIGN },
    { MAP_EVENT_WARP, WORLD_MAP_TO_ALPINE_CROSSING_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_ALPINE_CROSSING_EAST },
    { MAP_EVENT_WARP, WORLD_MAP_TO_ALPINE_CROSSING_NORTH, CONDITION_ALWAYS, WORLD_MAP_TO_ALPINE_CROSSING_NORTH },
    { MAP_EVENT_WARP, WORLD_MAP_TO_ALPINE_CROSSING_WEST, CONDITION_ALWAYS, WORLD_MAP_TO_ALPINE_CROSSING_WEST },
    { MAP_EVENT_WARP, WORLD_MAP_TO_LAMAKAN_DESERT_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_LAMAKAN_DESERT_EAST },
    { MAP_EVENT_WARP, WORLD_MAP_TO_SUHALLA_DESERT_NORTH, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_DESERT_NORTH },
    { MAP_EVENT_WARP, WORLD_MAP_TO_SUHALLA_DESERT_SOUTH, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_DESERT_SOUTH },
    { MAP_EVENT_WARP, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_SOUTH, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_SOUTH },
    { MAP_EVENT_WARP, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_EAST },
    { MAP_EVENT_WARP, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_WEST, CONDITION_ALWAYS, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_WEST },
    { MAP_EVENT_WARP, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_EAST },
    { MAP_EVENT_WARP, WORLD_MAP_TO_BILIBIN_BARRICADE_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN_BARRICADE_EAST },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_VALE_GATE), CONDITION_ALWAYS, WORLD_MAP_TO_VALE_GATE },
    { MAP_EVENT_COORD, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_VAULT), CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterVault) },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_GOMA_CAVE_ENTRANCE), CONDITION_ALWAYS, WORLD_MAP_TO_GOMA_CAVE_ENTRANCE },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_GOMA_CAVE_BRIDGE), CONDITION_ALWAYS, WORLD_MAP_TO_GOMA_CAVE_BRIDGE },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_BILIBIN), CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN },
    { MAP_EVENT_COORD, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_BILIBIN_BARRICADE), CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterBilibinBarricade) },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_KOLIMA), CONDITION_ALWAYS, WORLD_MAP_TO_KOLIMA_WEST },
    { MAP_EVENT_COORD, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_KOLIMA_BRIDGE), CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterKolimaBridge) },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_KOLIMA_FOREST), CONDITION_ALWAYS, WORLD_MAP_TO_KOLIMA_FOREST_ENTRANCE },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_BILIBIN_CAVE_ENTRANCE), CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN_CAVE_ENTRANCE },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_BILIBIN_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN_CAVE_ICE },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_IMIL), CONDITION_ALWAYS, WORLD_MAP_TO_IMIL_SOUTH },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_MERCURY_LIGHTHOUSE), CONDITION_ALWAYS, WORLD_MAP_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_FUCHIN_TEMPLE), CONDITION_ALWAYS, WORLD_MAP_TO_FUCHIN_TEMPLE_GROUNDS },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_MOGALL_FOREST_SIGN), CONDITION_ALWAYS, WORLD_MAP_TO_MOGALL_FOREST_SIGN },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_MOGALL_FOREST_KILLER_APE), CONDITION_ALWAYS, WORLD_MAP_TO_MOGALL_FOREST_KILLER_APE },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_XIAN), CONDITION_ALWAYS, WORLD_MAP_TO_XIAN },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_ALTIN), CONDITION_ALWAYS, WORLD_MAP_TO_ALTIN },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_ALTIN_PEAK), CONDITION_ALWAYS, WORLD_MAP_TO_ALTIN_PEAK_WEST_EXIT },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_LAMA_TEMPLE), CONDITION_ALWAYS, WORLD_MAP_TO_LAMA_TEMPLE },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_LAMAKAN_DESERT_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_LAMAKAN_DESERT_CAVE },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_KALAY), CONDITION_ALWAYS, WORLD_MAP_TO_KALAY_EAST },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_LUNPA), CONDITION_ALWAYS, WORLD_MAP_TO_LUNPA },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_WEST_LUNPA_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_WEST_LUNPA_CAVE },
    { MAP_EVENT_COORD, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_KALAY_DOCKS), CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterKalayDocks) },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_TOLBI_DOCKS), CONDITION_ALWAYS, WORLD_MAP_TO_TOLBI_DOCKS },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_TOLBI), CONDITION_ALWAYS, WORLD_MAP_TO_TOLBI },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_ALTMILLER_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_ALTMILLER_CAVE_ENTRANCE },
    { MAP_EVENT_COORD, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_GONDOWAN_PASSAGE), CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterGondowanPassage) },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_KALAY_TUNNEL_ENTRANCE), CONDITION_ALWAYS, WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_GONDOWAN_CAVE_NORTH), CONDITION_ALWAYS, WORLD_MAP_TO_GONDOWAN_CAVE_NORTH },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_GONDOWAN_CAVE_SOUTH), CONDITION_ALWAYS, WORLD_MAP_TO_GONDOWAN_CAVE_SOUTH },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_SUHALLA), CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_GATEWAY_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_GATEWAY_CAVE },
    { MAP_EVENT_WARP, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_ROAD_TO_BABI_LIGHTHOUSE), CONDITION_ALWAYS, WORLD_MAP_TO_ROAD_TO_BABI_LIGHTHOUSE },
    { MAP_EVENT_COORD, COORD_EVENT_MAPSEC(LOCALID_MAPSEC_LALIVERO), CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterLalivero) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), MB_WORLD_MAP_TOLBI_SHIP, IF_FLAG_SET(FLAG_TOLBI_SHIP_AT_ISLAND), EVENT_SCRIPT(EventScript_ReturnToTolbiShip) },
    { MAP_EVENT_WARP | MAP_EVENT_FLAG_FACING | FACING_NORTH, MB_WORLD_MAP_CROSSBONE_ISLE, CONDITION_ALWAYS, WORLD_MAP_TO_CROSSBONE_ISLE },
    { PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), LOCALID_STORM, IF_FLAG_SET(FLAG_MAP_TEMP_F0), EVENT_SCRIPT(EventScript_StartDousingStorm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_DOUSE), LOCALID_STORM, IF_FLAG_SET(FLAG_MAP_TEMP_F0), EVENT_SCRIPT(EventScript_DouseMapStorm) },
    { MAP_EVENT_RAISED, RAISED_STORM_TIME_UP, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StormDropsParty) },
    { MAP_EVENTS_END },
};

/// Each passenger's rowing value, the world map's copy of
/// gTolbiShipMast_PassengerRowValues (tolbi_ship_mast.c).
u32 gWorldMapPassengerRowValues[] = {
    7, 5, 8, 4, 9, 12, 6, 3,
    0,
};
// clang-format on

s32 gIslandWakeScript[] = {
    SC_ADD(SW_X, -PIXELS(1) / 10),
    SC_ADD(SW_Z, PIXELS(1) / 10),
    SC_ADD(SW_SCALE_X, 7 * Q16_ONE / 128),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 512),
    SC_WAIT(1),
    SC_LOOP(48, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

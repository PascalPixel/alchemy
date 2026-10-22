/// @file
/// The World Map (SceneId_WorldMap), the one perspective map (map/world) that
/// joins the places, and the only scene of this overlay. Its code does three
/// kinds of work.
///
/// It lets the party into the places. Each place stands on the map as an
/// actor drawn as a picture of its kind (SPRITE_WORLD_MAP_VILLAGE, _CAVE,
/// _DOCK and the rest), turned with the map (WorldMap_AlignToMap).
/// WorldMap_UpdateLocationContacts, run every frame, hides the actors outside
/// a box around the view and raises the PLACE_TRIGGER of the one the leader
/// touches, which the event table turns into that place's exit; a few ways in
/// are cells instead, each carrying the number of its exit. The world map laid
/// flat, with each place actor boxed and labelled by its PLACE_TRIGGER and the
/// box kept visible around a view at Vale, is
/// https://alchemy.pascalpixel.com/places/world-map/place-triggers.png. Five
/// places have
/// two ways in, and their events choose the exit by the side the leader comes
/// from: the Bilibin Barricade from the west or the east, and Kolima Bridge,
/// Kalay Docks, Gondowan Passage and Lalivero from the north or the south.
/// Kolima Bridge's actor shows the drawbridge let down once
/// FLAG_KOLIMA_BRIDGE_LOWERED is set.
///
/// It holds what the party meets on the map itself. Vine, Corona and Hail, a
/// Venus, a Mars and a Mercury Djinni, stand on the map and, talked to or
/// walked near, fight once a random battle is nearly due. A whirlwind near
/// Crossbone Isle (ACTOR_STORM, put away while the Tolbi-bound Ship lies at
/// anchor off the island) lifts the party for 180 frames when walked into,
/// as the Suhalla Desert's storms do: Douse on it brings the party down into
/// a battle, and when the time runs out it drops the party outside Suhalla
/// (WORLD_MAP_FROM_SUHALLA, FLAG_ARRIVE_FALLING). The Turtle Boots are buried
/// in the ground (TRIGGER_TURTLE_BOOTS).
///
/// And it plays the story's moments on the map, each run by
/// WorldMap_Initialize for the entrance another scene sends the party to. In
/// story order:
/// - Leaving Vale (WORLD_MAP_FROM_VALE_GATE) sets FLAG_PARTY_LEFT_VALE the
///   first time.
/// - Flint, the first Djinni, circles his spot until the party walks up to
///   him (Event_MeetFlint); he joins, teaches how Djinn are set and put on
///   standby, and a tutorial battle follows.
/// - Hammet's caravan: the first time the party touches Vault
///   (WORLD_MAP_HAMMETS_CARAVAN_LEAVES), the caravan, cut off from Kalay by
///   the destroyed bridge, turns north for Lunpa as the ground shakes and a
///   rock falls beside it; then the party goes on into Vault. It sets
///   FLAG_GUARDS_SUSPECT_KALAY.
/// - Mercury Lighthouse lit (WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT): the
///   lighthouse transition (WorldMap_PlayLighthouseTransition), then Mercury
///   Lighthouse Aerie (MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT).
/// - Bunza's wagon: out of West Lunpa Cave (WORLD_MAP_FROM_WEST_LUNPA_CAVE)
///   Bunza, Hammet and the leader board it and it rolls off
///   (FLAG_BUNZAS_WAGON_LEFT); at the Kalay Tunnel Entrance
///   (WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH) it stops and the leader
///   steps down.
/// - The Tolbi-bound Ship's crossing of the Karagol Sea: between the ship's
///   scenes (WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_FIRST_ATTACK to
///   _SAIL_TO_LAND) the map shows the ship sailing west and sends the party
///   back aboard (FLAG_TOLBI_SHIP_ROWING). A rowing off course takes it north
///   to anchor off the island (WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS,
///   FLAG_TOLBI_SHIP_AT_ISLAND), where TRIGGER_CROSSBONE_ISLE leads ashore
///   and TRIGGER_TOLBI_SHIP back aboard;
///   WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI ends the crossing at Tolbi
///   (FLAG_TOLBI_SHIP_REACHED_TOLBI).
/// - The Black Orb: out of Gateway Cave (WORLD_MAP_FROM_GATEWAY_CAVE) Iodem
///   sees Babi's ship scuttled off the coast and gives Isaac the orb to use
///   on it (FLAG_SCENE_TEMP_34, FLAG_SAW_SHIP_AT_IDEJIMA), and calls him back
///   if he walks into the cave again. Used on the wreck (TRIGGER_SHIPWRECK)
///   the orb does nothing; Iodem takes it back and the party must go on to
///   Babi Lighthouse (FLAG_BLACK_ORB_RETURNED). A defeat meanwhile returns to
///   WORLD_MAP_AFTER_BLACK_ORB_DEFEAT, which discards the orb and leads on by
///   WORLD_MAP_TO_LUNPA_SUHALLA.
/// - Venus Lighthouse lit (WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT): the
///   lighthouse transition, then back to the Lighthouse Aerie
///   (VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT); after the look back
///   (WORLD_MAP_BEFORE_SHEBAS_FALL) the lighthouse shakes in clouds of dust
///   before Sheba's fall (WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE).
/// - The ending: once the cleared game is saved (menu/clear.c,
///   WORLD_MAP_FROM_CLEAR), the island Jenna and Kraden are adrift on drifts
///   east, and WORLD_MAP_JENNA_AND_KRADEN_ADRIFT shows Alex leading them to
///   Felix and Sheba before the title screen returns.
///
/// WORLD_MAP_FROM_DEBUG, the debug menus' entrance, turns encounters off.
///
/// Open questions:
/// - XXX (no evidence): what flag 0x85c records, set the first time the party
///   leaves Vale; no code in either game reads it.

#include "script.h"
#include "version.h"
#include "game_state.h"
#include "heap_state.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/scene/world_map.h"
#include "unit/scene/common/world_map_linked_effects.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
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
#include "global_cells.h"
#include "types.h"
#include "animspr.h"
#include "field_event.h"
#include "field_scene.h"
#include "djinn_ids.h"
#include "map_render_work.h"
#include "field_service.h"
#include "iwram_call.h"
#include "scene_ids.h"
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
#include "field_obj.h"
#include "frame.h"
#include "fx_scene.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "item_ids.h"

static s32 WorldMap_AlignToMap(struct ScriptInterpreter *interpreter);
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
static void WorldMap_BlockGatewayCaveExit(s32 actor);
static void Event_IodemCallsLeaderBack(void);
static void WorldMap_ShowEndingScene(void);
static void WorldMap_ShakeVenusLighthouse(void);
static void WorldMap_UpdateLighthouseQuake(void);
static void WorldMap_SailTolbiShipToIsland(void);
static s32 WorldMap_GetRowingBalance(void);
static s32 WorldMap_GetSeatRowValue(u32 seat);
static void WorldMap_ShowIslandDrifting(void);
static void WorldMap_UpdateStormTimer(void);
static void WorldMap_PlayLighthouseTransition(s32 palette);

/// The actors WorldMap_UpdateLocationContacts looks at, the placed ones to the
/// last slot, and the base of the triggers a touch raises (PLACE_TRIGGER).
enum { CONTACT_LAST_ACTOR = 65, CONTACT_TRIGGER_BASE = 100 };

/// The world map's own actors, numbered in the order of gWorldMapPlacements
/// from ACTOR_FIRST_PLACED: the Djinn met on the map, the places, then the
/// Tolbi-bound Ship, the storm and Iodem. Each place is named by the scene its
/// exit leads into (gWorldMapExits), and touching it raises its PLACE_TRIGGER
/// (WorldMap_UpdateLocationContacts).
enum WorldMapActor {
  /// The Venus Djinni: Flint, circling his spot until the party meets him,
  /// and later Vine.
  /// ★ Named from the game: "The Venus Djinni" (MsgVenusDjinnJoined)
  ACTOR_VENUS_DJINNI = ACTOR_FIRST_PLACED,
  /// Corona.
  /// ★ Named from the game: "The Mars Djinni" (MsgVenusDjinnJoined)
  ACTOR_MARS_DJINNI,
  /// Hail.
  /// ★ Named from the game: "The Mercury Djinni" (MsgVenusDjinnJoined)
  ACTOR_MERCURY_DJINNI,
  /// Vale, whose exit leads to the town gate (SceneId_ValeGate).
  /// ★ Named from the game: "Why's everyone hanging around the town gate?"
  /// (MsgValeWhysEveryoneHanging)
  ACTOR_PLACE_VALE_GATE,
  /// ★ Named from its area name: Vault
  ACTOR_PLACE_VAULT,
  /// ★ Named from its area name: Altin
  ACTOR_PLACE_ALTIN,
  /// ★ Named from its area name: Suhalla
  ACTOR_PLACE_SUHALLA,
  /// ★ Named from its area name: Lunpa
  ACTOR_PLACE_LUNPA,
  /// ★ Named from its area name: Kalay Docks
  ACTOR_PLACE_KALAY_DOCKS,
  /// ★ Named from its area name: Tolbi Docks
  ACTOR_PLACE_TOLBI_DOCKS,
  /// ★ Named from its area name: Imil
  ACTOR_PLACE_IMIL,
  /// ★ Named from its area name: Kolima
  ACTOR_PLACE_KOLIMA,
  /// ★ Named from its area name: Xian
  ACTOR_PLACE_XIAN,
  /// ★ Named from its area name: Lama Temple
  ACTOR_PLACE_LAMA_TEMPLE,
  /// ★ Named from its area name: Fuchin Temple
  ACTOR_PLACE_FUCHIN_TEMPLE,
  /// ★ Named from its area name: Bilibin
  ACTOR_PLACE_BILIBIN,
  /// ★ Named from its area name: Kalay
  ACTOR_PLACE_KALAY,
  /// ★ Named from its area name: Lalivero
  ACTOR_PLACE_LALIVERO,
  /// ★ Named from its area name: Tolbi
  ACTOR_PLACE_TOLBI,
  /// ★ Named from its area name: Mercury Lighthouse
  ACTOR_PLACE_MERCURY_LIGHTHOUSE,
  /// After two pictures no event answers: a mountain north of Vale
  /// (SPRITE_WORLD_MAP_MOUNTAIN) and Venus Lighthouse, which is entered by
  /// the cells around it.
  /// ★ Named from its area name: Bilibin Barricade
  ACTOR_PLACE_BILIBIN_BARRICADE = ACTOR_PLACE_MERCURY_LIGHTHOUSE + 3,
  /// ★ Named from its area name: Road to Babi Lighthouse
  ACTOR_PLACE_ROAD_TO_BABI_LIGHTHOUSE,
  /// ★ Named from its area name: Goma Cave Entrance
  ACTOR_PLACE_GOMA_CAVE_ENTRANCE,
  /// Goma Cave's way out, where a wooden bridge crosses the river
  /// (SceneId_GomaCaveBridge).
  /// ★ Named from its area name: Goma Cave
  ACTOR_PLACE_GOMA_CAVE_BRIDGE,
  /// ★ Named from its area name: Altmiller Cave
  ACTOR_PLACE_ALTMILLER_CAVE,
  /// ★ Named from its area name: Gondowan Cave
  ACTOR_PLACE_GONDOWAN_CAVE_NORTH,
  /// ★ Named from its area name: Gondowan Cave
  ACTOR_PLACE_GONDOWAN_CAVE_SOUTH,
  /// The way into Mogall Forest where the Killer Ape waits
  /// (SceneId_MogallForestKillerApe).
  /// ★ Named from the game: "Killer Ape" (MsgEnemyName)
  ACTOR_PLACE_MOGALL_FOREST_KILLER_APE,
  /// ★ Named from its area name: Gateway Cave
  ACTOR_PLACE_GATEWAY_CAVE,
  /// ★ Named from its area name: Bilibin Cave Entrance
  ACTOR_PLACE_BILIBIN_CAVE_ENTRANCE,
  /// ★ Named from its area name: Bilibin Cave
  ACTOR_PLACE_BILIBIN_CAVE,
  /// ★ Named from its area name: West Lunpa Cave
  ACTOR_PLACE_WEST_LUNPA_CAVE,
  /// ★ Named from its area name: Altin Peak
  ACTOR_PLACE_ALTIN_PEAK,
  /// ★ Named from its area name: Lamakan Desert
  ACTOR_PLACE_LAMAKAN_DESERT_CAVE,
  /// ★ Named from its area name: Kolima Forest
  ACTOR_PLACE_KOLIMA_FOREST,
  /// The way into Mogall Forest by its broken sign (SceneId_MogallForestSign).
  /// ★ Named from the game: "The broken sign reads:"
  /// (MsgMogallForestBrokenSign)
  ACTOR_PLACE_MOGALL_FOREST_SIGN,
  /// ★ Named from its area name: Kalay Tunnel Entrance
  ACTOR_PLACE_KALAY_TUNNEL_ENTRANCE,
  /// After three signposts (SPRITE_SIGNPOST). Unused: they stand at
  /// the map's origin, out of the player's reach, and no code moves them.
  /// ★ Named from its area name: Kolima Bridge
  ACTOR_PLACE_KOLIMA_BRIDGE = ACTOR_PLACE_KALAY_TUNNEL_ENTRANCE + 4,
  /// ★ Named from its area name: Gondowan Passage
  ACTOR_PLACE_GONDOWAN_PASSAGE,
  /// The Tolbi-bound Ship at anchor off Crossbone Isle, after the wreck of
  /// Babi's ship (SPRITE_SHIPWRECK), which stands off Gateway Cave.
  /// ★ Named from its area name: Tolbi-bound Ship
  ACTOR_TOLBI_SHIP = ACTOR_PLACE_GONDOWAN_PASSAGE + 2,
  /// The storm that lifts the party, as the Suhalla Desert's do.
  /// ★ Named from what the player sees: a white whirlwind spinning on the map,
  /// drawn as the Suhalla Desert's sandstorms, spr_289.png (its render)
  ACTOR_STORM,
  /// Placed by the coast once he has handed over the Black Orb.
  /// ★ Named from the game: "Iodem, Iodem!" (MsgTolbiIodemIodem)
  ACTOR_IODEM
};

/// The trigger raised by touching a place's actor.
#define PLACE_TRIGGER(actor) (CONTACT_TRIGGER_BASE + (actor))

/// The world map's own event triggers: cell codes the map's cells carry, and
/// the value the storm raises. Each exit cell carries the number of the exit
/// it leads to, so the exit rows of gWorldMapEvents name their trigger by that
/// exit.
enum WorldMapTrigger {
  TRIGGER_TOLBI_SHIP = 74,     ///< the shore beside the anchored ship
  TRIGGER_CROSSBONE_ISLE = 75, ///< the island's shore, walked onto northward
  TRIGGER_FLINT = 90,          ///< the ground around Flint
  TRIGGER_CORONA = 91,         ///< the ground around Corona
  TRIGGER_VINE = 92,           ///< the ground around Vine
  TRIGGER_HAIL = 93,           ///< the ground around Hail
  TRIGGER_STORM = 95,          ///< the ground under the storm
  TRIGGER_SHIPWRECK = 96,      ///< the scuttled ship off Gateway Cave
  /// Raised by WorldMap_UpdateStormTimer when the party's time in the air
  /// runs out.
  TRIGGER_STORM_TIME_UP = 99,
  TRIGGER_TURTLE_BOOTS = 100 ///< the ground where the Turtle Boots are buried
};

/// The rowers' balance (WorldMap_GetRowingBalance) that takes the Tolbi-bound
/// Ship off course to the island.
enum { ROWING_BALANCE_OFF_COURSE = 11 };

/// SPRITE_WORLD_MAP_BRIDGE's animation once Kolima Bridge's drawbridge is down.
enum { WORLD_MAP_BRIDGE_ANIM_LOWERED = 2 };

/// The falling rock's animations (spr_216.png): it tumbles as it falls, then
/// shatters where it lands.
enum { FALLING_ROCK_ANIM_TUMBLE = 2, FALLING_ROCK_ANIM_SHATTER = 3 };

/// The actors of Hammet's caravan leaving Vault
/// (gWorldMapPlacementsHammetsCaravan, WORLD_MAP_HAMMETS_CARAVAN_LEAVES),
/// numbered from ACTOR_FIRST_PLACED; Garet's row takes his own actor.
enum WorldMapCaravanActor {
  /// Hammet, in his sprite, as ACTOR_HAMMET is out of West Lunpa Cave.
  /// ★ Named from the game: "Master Hammet!"
  /// (MsgWorldMapWeCantStayAnotherMinute)
  ACTOR_CARAVAN_HAMMET = ACTOR_FIRST_PLACED,
  /// One of Hammet's men, bald and bare-chested. "Master Hammet! This is
  /// terrible! The bridge has been destroyed..."
  /// ★ Named from the game: "Master Hammet! This is terrible!"
  /// (MsgWorldMapWeCantStayAnotherMinute)
  ACTOR_CARAVAN_MAN,
  /// The caravan's four wagons, in the sprite of Bunza's wagon.
  /// ★ Named from what the player sees: four covered wagons, each drawn by a
  /// horse (its render)
  ACTOR_CARAVAN_FIRST_WAGON,
  /// A blank actor (SPRITE_BLANK) that the falling rock is held above as it
  /// drops (WorldMap_UpdateFallingRock); it grows from a tenth of its size as
  /// it moves (gCaravanRockGuideScript).
  ACTOR_FALLING_ROCK_GUIDE = ACTOR_CARAVAN_FIRST_WAGON + 4,
  /// Drawn as the rocks in sol_sanctum_eruption.c; it lands by the caravan
  /// and shatters.
  /// ★ Named from the game: "being smashed by falling rocks here"
  /// (MsgWorldMapWeCantStayAnotherMinute)
  ACTOR_FALLING_ROCK
};

/// The Tolbi-bound Ship, the first actor of gWorldMapPlacementsCrossingEnds and
/// gWorldMapPlacementsCrossing
/// (WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_FIRST_ATTACK to
/// WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI and
/// WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS), shown on its way across the Karagol
/// Sea.
enum {
  /// ★ Named from its area name: Tolbi-bound Ship
  ACTOR_CROSSING_SHIP = ACTOR_FIRST_PLACED
};

/// Bunza's wagon at Kalay (gWorldMapPlacementsFromKalayTunnelEntranceSouth),
/// after Kalay's and the Kalay Tunnel Entrance's place actors.
enum {
  /// ★ Named from the game: "go back to Kalay in my wagon." (MsgLunpaBunza)
  ACTOR_KALAY_WAGON = ACTOR_FIRST_PLACED + 2
};

/// The one actor of gWorldMapPlacementsFromClear, before the ending: the
/// island Jenna and Kraden are adrift on ("How long will this island continue
/// to drift?"). It drifts east trailing a wake (WorldMap_SpawnIslandWake)
/// until the map leads into their scene.
enum {
  /// ★ Named from what the player sees: an island of green hills around a
  /// mountain, drifting on the sea, spr_504.png (its render)
  ACTOR_DRIFTING_ISLAND = ACTOR_FIRST_PLACED
};

/// The ending's actors (gWorldMapPlacementsJennaAndKradenAdrift), numbered from
/// ACTOR_FIRST_PLACED after the rows of Jenna and Sheba, which take their own
/// actors.
enum WorldMapEndingActor {
  /// Kraden, adrift with Jenna.
  /// ★ Named from the game: "Oh, Kraden... How long will this island continue
  /// to drift?" (MsgWorldMapKradenHowLongWillIsland)
  ACTOR_KRADEN = ACTOR_FIRST_PLACED,
  /// Alex, who comes to show them that Felix and Sheba are alive: "There is
  /// something I wish to show you."
  /// ★ Named from the game: "Alex!" (MsgMercuryGot)
  ACTOR_ALEX,
  /// Felix, in his sprite as at Mercury Lighthouse (SPRITE_FELIX,
  /// mercury_lighthouse_aerie.c), shown with Sheba as Jenna and Kraden call
  /// their names.
  /// ★ Named from the game: "Felix" (MsgCharacterName)
  ACTOR_FELIX
};

/// Animations the map plays by number beyond enum ActorAnimation, from each
/// sprite's animation table (graphics/sprite/spr_<number>.tsv).
enum {
  ANIMATION_JENNA_LIE =
      19, ///< lying on her back (spr_005.tsv's animation list)
  ANIMATION_KRADEN_LIE = 5, ///< lying on his back (spr_030.tsv)
  ANIMATION_FELIX_LIE = 6,  ///< lying face down, one arm out (spr_035.tsv)
  ANIMATION_SHEBA_LIE = 8   ///< lying on her back (spr_006.tsv)
};

// The world map's scene flags:
// FLAG_SCENE_TEMP_00: the falling rock has landed, in Hammet's caravan scene.
// FLAG_SCENE_TEMP_34: Iodem has handed over the Black Orb.
// FLAG_SCENE_TEMP_35: the map's ordinary placements are in use.
// FLAG_SCENE_TEMP_F0: the storm holds the party up in the air.
// FLAG_SCENE_TEMP_F1: the storm is done with, or put away.
// FLAG_SCENE_TEMP_F8: a byte: frames left in the air.

/// Walking near Vine, or talking to him: once a battle is nearly due, half the
/// time Vine fights; otherwise the next step brings an ordinary battle.
/// Reached by talking to ACTOR_VENUS_DJINNI, or by walking onto TRIGGER_VINE
/// until Vine is caught.
static void Event_MeetVine(void) {
  struct EventWork *work = gEventWork;

  if (gGameState.encounterSteps >=
      Math_Divide(work->encounterThreshold * 9, 10)) {
    if ((u32)Engine_RandomNext() < 0x8000) {
      Field_RequestDjinnBattle(DJINN_NO_HOP | ACTOR_VENUS_DJINNI, DJINNI_VINE);
      work->encounterBias = 0;
    } else {
      gGameState.encounterSteps = work->encounterThreshold;
    }
  }
}

/// Walking near Corona, or talking to him: once a battle is nearly due, Corona
/// fights.
/// Reached by talking to ACTOR_MARS_DJINNI, or by walking onto TRIGGER_CORONA
/// until Corona is caught.
static void Event_MeetCorona(void) {
  struct GameState *state = &gGameState;
  s32 *steps = &state->encounterSteps;
  struct EventWork *work = gEventWork;

  if (*steps >= Math_Divide(work->encounterThreshold * 9, 10)) {
    Field_RequestDjinnBattle(DJINN_NO_HOP | ACTOR_MARS_DJINNI, DJINNI_CORONA);
    work->encounterBias = 0;
  }
}

/// Walking near Hail, or talking to him: once a battle is nearly due, Hail
/// fights.
/// Reached by talking to ACTOR_MERCURY_DJINNI, or by walking onto TRIGGER_HAIL
/// until Hail is caught.
static void Event_MeetHail(void) {
  struct GameState *state = &gGameState;
  s32 *steps = &state->encounterSteps;
  struct EventWork *work = gEventWork;

  if (*steps >= Math_Divide(work->encounterThreshold * 9, 10)) {
    Field_RequestDjinnBattle(DJINN_NO_HOP | ACTOR_MERCURY_DJINNI, DJINNI_HAIL);
    work->encounterBias = 0;
  }
}

/// The update hook of a sparkle around the Black Orb: it frees the sparkle on
/// its second frame.
static void WorldMap_ReleaseSparkAfterDelay(union FieldObject *fieldObject) {
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  if (actor->timer <= 0) {
    actor->timer++;
  } else {
    Engine_ObjectDispatchRelease(actor);
  }
}

/// The update hook of the Black Orb held up over the wreck: it swells and
/// shrinks every four frames and gives off a sparkle every other frame.
static void WorldMap_PulseOrbAndSpawnSparks(union FieldObject *fieldObject) {
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct FieldActor *spark;
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
  spark = Engine_ObjectCreate(SPRITE_SPARKLE, x, y, z);
  Engine_AudioPlayCue(SE_246);
  if (spark == 0)
    return;
  spark->motionFlags = 0;
  spark->sprite->image.priority = 1;
  Engine_ActorSetSpriteFlags(spark, 0);
  Object_SetMode(spark, 1);
  spark->timer = 0;
  spark->update = WorldMap_ReleaseSparkAfterDelay;
}

/// Flint's update hook until the party meets him (WorldMap_SetUpFlint): he
/// flickers white and red and circles 16 pixels around his spot, bobbing 4
/// pixels up and down, facing the way he goes. Event_MeetFlint stops him by
/// setting his work to 1.
static void WorldMap_CircleFlint(union FieldObject *fieldObject) {
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  s16 *angle;

  if (gFrameCount & 2)
    Engine_ObjectSetPartPalettes(actor, SPRITE_COLOR_RED_TINT);
  else
    Engine_ObjectSetPartPalettes(actor, SPRITE_COLOR_WHITE);
  if ((s16)actor->work == 0) {
    angle = (s16 *)&actor->timer;
    actor->x.fixed = PIXELS(5584);
    actor->y.fixed =
        Iwram_MulQ16(Engine_MathSin(*angle << 3), PIXELS(4)) + PIXELS(16);
    actor->z.fixed = PIXELS(1328);
    Vector_AddPolarOffset(PIXELS(16), *angle, &actor->x);
    actor->facing = *angle + FACING_SOUTH;
    *angle += 0x400;
  }
}

/// Flint's object script (gWorldMapFlintScript): until he is caught or his
/// lesson has begun (FLAG_DJINN_LESSON), he circles his spot at half size.
static s32 WorldMap_SetUpFlint(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  if (Engine_GameFlagIsSet(FLAG_DJINNI_0 + DJINNI_FLINT) != 0) {
    return 0;
  }
  if (Engine_GameFlagIsSet(FLAG_DJINN_LESSON) != 0) {
    return 0;
  }
  actor->update = WorldMap_CircleFlint;
  actor->motionFlags = 0;
  actor->timer = 0;
  actor->work = 0;
  actor->scaleX = Q16_ONE / 2;
  actor->scaleY = Q16_ONE / 2;
  return 0;
}

const struct SceneEntrance *WorldMap_GetEntrances(void) {
  return gWorldMapEntrances;
}

const struct SceneRegion *WorldMap_GetRegions(void) { return NULL; }

const u32 *WorldMap_GetExits(void) { return gWorldMapExits; }

/// Kolima Bridge's object script (gWorldMapKolimaBridgeScript): it turns with
/// the map and shows the drawbridge let down once FLAG_KOLIMA_BRIDGE_LOWERED is
/// set.
static s32 WorldMap_SetUpKolimaBridge(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  WorldMap_AlignToMap(interpreter);
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_BRIDGE_LOWERED) != 0) {
    Object_SetMode(actor, WORLD_MAP_BRIDGE_ANIM_LOWERED);
  }
  return 1;
}

/// The object script of every picture on the map (gWorldMapAlignToMapScript):
/// it turns the sprite with the map's rotation.
static s32 WorldMap_AlignToMap(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  struct MapRenderWork *work = gMapWork[0];
  struct AnimationObject *sprite = actor->sprite;

  sprite->rotation = work->rotation;
  sprite->flags = 0;
  return 1;
}

/// Iodem's object script (gWorldMapIodemScript): he turns with the map and
/// blocks the way, so the leader cannot walk through him.
static s32 WorldMap_SetUpIodem(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  struct MapRenderWork *work = gMapWork[0];
  struct AnimationObject *sprite = actor->sprite;

  actor->collisionFlags |= 1;
  sprite->rotation = work->rotation;
  return 1;
}

/// Sets up the storm (ACTOR_STORM) as it is placed: it does not collide, and
/// while the Tolbi-bound Ship lies at anchor off the island
/// (FLAG_TOLBI_SHIP_AT_ISLAND) it is put away at the map's origin and
/// FLAG_SCENE_TEMP_F1 keeps it from catching the party.
/// Called by its object script.
static s32 WorldMap_SetUpStorm(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ObjectSetPartPalettes(actor, SPRITE_COLOR_RED_TINT);
  actor->collisionFlags = 0;
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_F1);
    actor->x.fixed = 0;
    actor->y.fixed = 0;
  }
  return 0;
}

/// Called each frame by the storm's object script (gWorldMapStormScript): it
/// hides the storm on odd frames, and WorldMap_UpdateLocationContacts shows it
/// again on the next, so it flickers.
static s32 WorldMap_BlinkActorSprite(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  if ((actor->spriteStorageKind & 1) && (gFrameCount & 1))
    actor->spriteStorageKind ^= 1;
  return 1;
}

/// Chooses the actors for the entrance: each scripted moment has a table of
/// its own while it is still to play, and every other arrival takes the
/// places, the Djinn, the storm and Iodem (gWorldMapPlacements) and sets
/// FLAG_SCENE_TEMP_35.
const struct ScenePlacement *WorldMap_GetPlacements(void) {
  switch (gGameState.entrance) {
  case WORLD_MAP_FROM_WEST_LUNPA_CAVE:
    if (Engine_GameFlagIsSet(FLAG_BUNZAS_WAGON_LEFT) == 0 &&
        Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
      return gWorldMapPlacementsFromWestLunpaCave;
    }
    break;
  case WORLD_MAP_HAMMETS_CARAVAN_LEAVES:
    if (Engine_GameFlagIsSet(FLAG_GUARDS_SUSPECT_KALAY) == 0) {
      return gWorldMapPlacementsHammetsCaravan;
    }
    break;
  case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_FIRST_ATTACK:
  case WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI:
    return gWorldMapPlacementsCrossingEnds;
  case WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH:
    return gWorldMapPlacementsFromKalayTunnelEntranceSouth;
  case WORLD_MAP_FROM_CLEAR:
    return gWorldMapPlacementsFromClear;
  case WORLD_MAP_BEFORE_SHEBAS_FALL:
    return gWorldMapPlacementsBeforeShebasFall;
  case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_SECOND_ATTACK:
  case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_THIRD_ATTACK:
  case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_KRAKEN:
  case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS_SAIL_TO_LAND:
  case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS:
    return gWorldMapPlacementsCrossing;
  case WORLD_MAP_JENNA_AND_KRADEN_ADRIFT:
    return gWorldMapPlacementsJennaAndKradenAdrift;
  default:
    break;
  }

  Engine_GameFlagSet(FLAG_SCENE_TEMP_35);
  return gWorldMapPlacements;
}

/// Chooses a passage exit from the side of the actor the leader approaches.
static void WorldMap_SelectExitByX(s32 trigger, s32 exitBefore, s32 exitAfter) {
  struct EventWork *work;
  struct FieldActor *subject;
  struct FieldActor *other;

  subject = Object_GetById(trigger - CONTACT_TRIGGER_BASE);
  other = Object_GetById(gGameState.selectedActor);
  work = gEventWork;
  if (other->x.fixed < subject->x.fixed) {
    work->requestedExit = exitBefore;
  } else {
    work->requestedExit = exitAfter;
  }
  Engine_AudioPlayCue(SE_MAP_EXIT);
}

/// Chooses a passage exit from the leader's north-south approach.
static void WorldMap_SelectExitByZ(s32 trigger, s32 exitBefore, s32 exitAfter) {
  struct EventWork *work;
  struct FieldActor *subject;
  struct FieldActor *other;

  subject = Object_GetById(trigger - CONTACT_TRIGGER_BASE);
  other = Object_GetById(gGameState.selectedActor);
  work = gEventWork;
  if (other->z.fixed < subject->z.fixed) {
    work->requestedExit = exitBefore;
  } else {
    work->requestedExit = exitAfter;
  }
  Engine_AudioPlayCue(SE_MAP_EXIT);
}

/// Touching the Bilibin Barricade's actor: the side the party comes from
/// decides the exit, from the west WORLD_MAP_TO_BILIBIN_BARRICADE_WEST, from
/// the east WORLD_MAP_TO_BILIBIN_BARRICADE_EAST.
static void Event_EnterBilibinBarricade(void) {
  WorldMap_SelectExitByX(PLACE_TRIGGER(ACTOR_PLACE_BILIBIN_BARRICADE),
                         WORLD_MAP_TO_BILIBIN_BARRICADE_WEST,
                         WORLD_MAP_TO_BILIBIN_BARRICADE_EAST);
}

/// The party enters Kolima Bridge from the world map. It works as
/// Event_EnterBilibinBarricade does, entering from the north by
/// WORLD_MAP_TO_KOLIMA_BRIDGE_WEST and from the south by
/// WORLD_MAP_TO_KOLIMA_BRIDGE_EAST.
static void Event_EnterKolimaBridge(void) {
  WorldMap_SelectExitByZ(PLACE_TRIGGER(ACTOR_PLACE_KOLIMA_BRIDGE),
                         WORLD_MAP_TO_KOLIMA_BRIDGE_WEST,
                         WORLD_MAP_TO_KOLIMA_BRIDGE_EAST);
}

/// As Event_EnterKolimaBridge, for Kalay Docks: from the north
/// WORLD_MAP_TO_KALAY_DOCKS_NORTH, from the south
/// WORLD_MAP_TO_KALAY_DOCKS_EAST.
static void Event_EnterKalayDocks(void) {
  WorldMap_SelectExitByZ(PLACE_TRIGGER(ACTOR_PLACE_KALAY_DOCKS),
                         WORLD_MAP_TO_KALAY_DOCKS_NORTH,
                         WORLD_MAP_TO_KALAY_DOCKS_EAST);
}

/// As Event_EnterKolimaBridge, for Gondowan Passage: from the north
/// WORLD_MAP_TO_GONDOWAN_PASSAGE_EAST, from the south
/// WORLD_MAP_TO_GONDOWAN_PASSAGE_WEST.
static void Event_EnterGondowanPassage(void) {
  WorldMap_SelectExitByZ(PLACE_TRIGGER(ACTOR_PLACE_GONDOWAN_PASSAGE),
                         WORLD_MAP_TO_GONDOWAN_PASSAGE_EAST,
                         WORLD_MAP_TO_GONDOWAN_PASSAGE_WEST);
}

/// As Event_EnterKolimaBridge, for Lalivero: from the north
/// WORLD_MAP_TO_LALIVERO_WEST, from the south WORLD_MAP_TO_LALIVERO_EAST.
static void Event_EnterLalivero(void) {

  WorldMap_SelectExitByZ(PLACE_TRIGGER(ACTOR_PLACE_LALIVERO),
                         WORLD_MAP_TO_LALIVERO_WEST,
                         WORLD_MAP_TO_LALIVERO_EAST);
}

const struct SceneEvent *WorldMap_GetEvents(void) { return gWorldMapEvents; }

/// Run each frame on the map (WorldMap_Initialize adds it): it hides every
/// placed actor outside a box around the view, 160 pixels either side, 300
/// north and 200 south, and raises the PLACE_TRIGGER of an actor within reach
/// of the leader, their radii scaled by their sprites, measured as the sum of
/// the distances east-west and north-south. With the debug flag
/// FLAG_DEBUG_STEPS_OFF it raises none.
static void WorldMap_UpdateLocationContacts(void) {
  struct FieldActor *leader;
  struct EventWork *work;
  struct FieldActor *actor;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;
  s32 leaderReach;
  u32 i;

  leader = Object_GetById(gGameState.selectedActor);
  leaderReach = leader->sprite->scale * leader->radius;
  work = gEventWork;
  actor = work->viewCenter;
  left = actor->x.fixed - PIXELS(160);
  right = actor->x.fixed + PIXELS(160);
  top = actor->z.fixed - PIXELS(300);
  bottom = actor->z.fixed + PIXELS(200);

  for (i = ACTOR_FIRST_PLACED; i <= CONTACT_LAST_ACTOR; i++) {
    s32 x;
    s32 z;
    s32 dx;
    s32 reach;
    s32 scale;

    actor = Engine_ActorLookup(i);
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
    if (gDebugMode != 0 && Engine_GameFlagIsSet(FLAG_DEBUG_STEPS_OFF) != 0) {
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
        Engine_GameFlagIsSet(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
      work->touchedTrigger = i + CONTACT_TRIGGER_BASE;
    }
  }
}

/// Touching Vault's actor: the first time, until FLAG_GUARDS_SUSPECT_KALAY is
/// set, WORLD_MAP_HAMMETS_CARAVAN_LEAVES plays Hammet's caravan leaving
/// (WORLD_MAP_TO_HAMMETS_CARAVAN_LEAVES); later it is Vault's door
/// (WORLD_MAP_TO_VAULT_SOUTH).
static void Event_EnterVault(void) {
  if (Engine_GameFlagIsSet(FLAG_GUARDS_SUSPECT_KALAY) == 0) {
    Engine_EventRequestExit(WORLD_MAP_TO_HAMMETS_CARAVAN_LEAVES);
  } else {
    Engine_AudioPlayCue(SE_MAP_EXIT);
    Engine_EventRequestExit(WORLD_MAP_TO_VAULT_SOUTH);
  }
}

/// Walking east from Crossbone Isle's shore back aboard the Tolbi-bound Ship
/// (WORLD_MAP_TO_TOLBI_SHIP_DECK_CROSSBONE_ISLE, to
/// TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS).
/// Reached by walking east into TRIGGER_TOLBI_SHIP while the ship lies at
/// anchor off the island (FLAG_TOLBI_SHIP_AT_ISLAND).
static void Event_ReturnToTolbiShip(void) {

  Engine_EventBegin();
  Engine_EventRequestExit(WORLD_MAP_TO_TOLBI_SHIP_DECK_CROSSBONE_ISLE);
}

/// The scene's initialization: it runs the moment the arrival entrance
/// selects, the story's moments listed at the top of this file. The two
/// lighthouse entrances and WORLD_MAP_AFTER_BLACK_ORB_DEFEAT leave at once;
/// every other arrival sets FLAG_RETREAT_BLOCKED, starts
/// WorldMap_UpdateLocationContacts, opens the screen from a centre line, and
/// until the Manticore of the Lamakan Desert is beaten
/// (FLAG_LAMAKAN_MANTICORE_BEATEN) swaps in the 512-pixel tile south of Vault
/// whose bridge over the river is broken in the middle (the world map laid
/// flat with and without the swap). The arrivals by Crossbone
/// Isle and from the Suhalla Desert's storm resume the time in the air a storm
/// left (FLAG_SCENE_TEMP_F8). WORLD_MAP_FROM_DEBUG turns encounters off, and
/// leaving Vale the first time sets FLAG_PARTY_LEFT_VALE and flag 0x85c,
/// which nothing reads.
s32 WorldMap_Initialize(void) {
  struct GameState *state = &gGameState;
  s16 *entrance = &state->entrance;

  if (*entrance == WORLD_MAP_FROM_DEBUG) {
    Engine_GameFlagSet(FLAG_NO_ENCOUNTERS);
    Engine_GameFlagSet(FLAG_NO_ENCOUNTERS_IN_PLACE);
    Engine_GameFlagSet(FLAG_DEBUG_STEPS_OFF);
  }
  if (*entrance == WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT) {
    WorldMap_PlayLighthouseTransition(0);
    Event_SetDestination((s32)&SceneId_MercuryLighthouseAerie,
                         MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT);
  } else if (*entrance == WORLD_MAP_AFTER_VENUS_LIGHTHOUSE_LIT) {
    WorldMap_PlayLighthouseTransition(1);
    Event_SetDestination((s32)&SceneId_VenusLighthouseAerie,
                         VENUS_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT);
  } else if (*entrance == WORLD_MAP_AFTER_BLACK_ORB_DEFEAT) {
    Engine_EventBegin();
    PartyInventory_Discard(ITEM_BLACK_ORB);
    Engine_EventRequestExit(WORLD_MAP_TO_LUNPA_SUHALLA);
  } else {
    Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_CENTRE_LINE, 0);
    gEventWork->transitionFrames = 16;
    Engine_TaskWait(1);
    Engine_CameraSetSpeed(PIXELS(8), PIXELS(1));
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    FieldScheduler_AddCallback(WorldMap_UpdateLocationContacts,
                               TASK_PRIORITY_SCENE);
    if (Engine_GameFlagIsSet(FLAG_LAMAKAN_MANTICORE_BEATEN) == 0) {
      FieldMap_SetWindowCellTile(128, 256, 176, 56);
    }
    // Each special entrance resumes its travel or story sequence.
    switch (*entrance) {
    case WORLD_MAP_FROM_VALE_GATE:
      if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) == 0) {
        Engine_GameFlagSet(FLAG_PARTY_LEFT_VALE);
        Engine_GameFlagSet(0x85c);
      }
      break;
    case WORLD_MAP_FROM_GATEWAY_CAVE:
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
        if (Engine_GameFlagIsSet(FLAG_BLACK_ORB_RETURNED) == 0 &&
            Engine_GameFlagIsSet(FLAG_SCENE_TEMP_34) != 0) {
          gWorldMapIodem = ACTOR_IODEM;
          Actor_SetPosition(ACTOR_IODEM, PIXELS(6036), PIXELS(3400));
          Object_GetById(gWorldMapIodem)->facing =
              FACING_SOUTHEAST + FACING_STEP;
          WorldMap_BlockGatewayCaveExit(gWorldMapIodem);
        }
      } else if (Engine_GameFlagIsSet(FLAG_BLACK_ORB_RETURNED) == 0 &&
                 Engine_GameFlagIsSet(FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN) ==
                     0) {
        WorldMap_RunBlackOrbScene();
      }
      break;
    case WORLD_MAP_FROM_WEST_LUNPA_CAVE:
      if (Engine_GameFlagIsSet(FLAG_BUNZAS_WAGON_LEFT) == 0 &&
          Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
        WorldMap_BoardBunzasWagon();
      }
      break;
    case WORLD_MAP_HAMMETS_CARAVAN_LEAVES:
      if (Engine_GameFlagIsSet(FLAG_GUARDS_SUSPECT_KALAY) == 0) {
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
    case WORLD_MAP_FROM_CLEAR:
      WorldMap_ShowIslandDrifting();
      break;
    case WORLD_MAP_BEFORE_SHEBAS_FALL:
      WorldMap_ShakeVenusLighthouse();
      break;
    case WORLD_MAP_FROM_TOLBI_SHIP_DECK_CROSSBONE_ISLE:
    case WORLD_MAP_FROM_CROSSBONE_ISLE:
    case WORLD_MAP_FROM_SUHALLA_DESERT_STORM:
      Engine_GameFlagSet(FLAG_NOT_ON_MAP);
      if (FieldGameFlag_GetByte(FLAG_SCENE_TEMP_F8) != 0) {
        gGameState.movementMode = LEADER_MOVEMENT_AIRBORNE;
        FieldScheduler_AddCallback(WorldMap_UpdateStormTimer,
                                   TASK_PRIORITY_SCENE);
      }
      break;
    case WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS:
      WorldMap_SailTolbiShipToIsland();
      break;
    case WORLD_MAP_JENNA_AND_KRADEN_ADRIFT:
      WorldMap_ShowEndingScene();
      break;
    default:
      Object_GetById(ACTOR_TOLBI_SHIP)->scaleX = 5 * Q16_ONE / 4;
      Object_GetById(ACTOR_TOLBI_SHIP)->scaleY = 5 * Q16_ONE / 4;
      break;
    }
  }
  return 0;
}

/// The actors placed at WORLD_MAP_FROM_WEST_LUNPA_CAVE
/// (gWorldMapPlacementsFromWestLunpaCave), numbered from ACTOR_FIRST_PLACED:
/// Bunza's wagon, then, after Lunpa's and West Lunpa Cave's place actors,
/// Bunza and Hammet.
enum {
  /// Bunza's wagon.
  /// ★ Named from the game: "go back to Kalay in my wagon." (MsgLunpaBunza)
  ACTOR_WAGON = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "Then what should we do, Bunza?" (MsgLunpaBunza)
  ACTOR_BUNZA = ACTOR_FIRST_PLACED + 3,
  /// ★ Named from the game: "Master Hammet!"
  /// (MsgWorldMapWeCantStayAnotherMinute)
  ACTOR_HAMMET
};

/// Out of West Lunpa Cave by wagon: Bunza and Hammet nod to each other and
/// climb aboard with the leader, and the wagon rolls off.
/// Called by WorldMap_Initialize at WORLD_MAP_FROM_WEST_LUNPA_CAVE, after the
/// trade reopens (FLAG_DODONPA_LOCKED_UP) and until the wagon has left;
/// sets FLAG_BUNZAS_WAGON_LEFT.
static void WorldMap_BoardBunzasWagon(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  WorldMap_BeginEvent();
  GameFlag_Set(FLAG_BUNZAS_WAGON_LEFT);
  Actor_SetPosition(ACTOR_BUNZA, PIXELS(5856), PIXELS(1180));
  Actor_SetDestinationOffset(ACTOR_BUNZA, 24, 8);
  Engine_ActorWaitForMove(ACTOR_BUNZA);
  Engine_EventWait(60);
  Actor_SetPosition(ACTOR_HAMMET, PIXELS(5856), PIXELS(1180));
  Actor_SetDestinationOffset(ACTOR_HAMMET, 12, 24);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_BUNZA, ANIMATION_NOD);
  Engine_ActorSetAnimation(ACTOR_HAMMET, ANIMATION_NOD);
  Engine_EventWait(120);
  Actor_SetPosition(ACTOR_WAGON, PIXELS(5880), PIXELS(1208));
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_HAMMET, ANIMATION_WALK);
  record = Object_GetById(ACTOR_WAGON);
  if (record != 0) {
    Actor_SetDestination(ACTOR_HAMMET, record->x.part.pixel,
                         record->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_HAMMET);
  Actor_SetPosition(ACTOR_HAMMET, 0, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_BUNZA, ANIMATION_WALK);
  record = Object_GetById(ACTOR_WAGON);
  if (record != 0) {
    Actor_SetDestination(ACTOR_BUNZA, record->x.part.pixel,
                         record->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_BUNZA);
  Actor_SetPosition(ACTOR_BUNZA, 0, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_WALK);
  record = Object_GetById(ACTOR_WAGON);
  if (record != 0) {
    Actor_SetDestination(ACTOR_PARTY_LEADER, record->x.part.pixel,
                         record->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_EventWait(60);
  Actor_SetSpeed(ACTOR_WAGON, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetDestinationOffset(ACTOR_WAGON, 56, 8);
  Engine_ActorWaitForMove(ACTOR_WAGON);
  Actor_SetDestinationOffset(ACTOR_WAGON, 40, 40);
  Engine_ActorWaitForMove(ACTOR_WAGON);
  Actor_SetDestinationOffset(ACTOR_WAGON, 8, 88);
  Engine_ActorWaitForMove(ACTOR_WAGON);
  Engine_EventCloseScreen();
  Engine_EventRequestExit(WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE_SOUTH);
  Engine_EventEnd();
}

/// Outside Vault, the first time the party comes to it: Hammet's caravan
/// rolls out to leave for Kalay, finds the bridge south destroyed, and turns
/// north for Lunpa, leaving Ivan behind, as the ground shakes and a rock falls
/// beside it. Then the party goes on into Vault (WORLD_MAP_TO_VAULT_SOUTH).
/// Called by WorldMap_Initialize at WORLD_MAP_HAMMETS_CARAVAN_LEAVES until
/// FLAG_GUARDS_SUSPECT_KALAY is set; sets it.
static void WorldMap_ShowHammetsCaravan(void) {
  struct FieldActor *actor;
  struct FieldActor *spriteActor;
  const s32 *action;

  actor = Object_GetById(ACTOR_FALLING_ROCK);
  Engine_EventBegin();
  FieldEffect_ScheduleRatioTransition(5 * Q16_ONE / 4, 1);
  Engine_TaskWait(4);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  WorldMap_BeginEvent();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 5884, 1576);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_SetPosition(ACTOR_CARAVAN_HAMMET, PIXELS(5848), PIXELS(1576));
  Engine_TaskWait(1);
  Actor_SetChildValue(ACTOR_CARAVAN_HAMMET, SPRITE_COLOR_HIDDEN);
  spriteActor = Actor_Get(ACTOR_CARAVAN_HAMMET);
  Engine_ActorSetSpriteFlags(spriteActor, 0);
  Actor_SetSpeed(ACTOR_CARAVAN_FIRST_WAGON, PIXELS(8) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_CARAVAN_FIRST_WAGON + 1, PIXELS(8) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_CARAVAN_FIRST_WAGON + 2, PIXELS(8) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_CARAVAN_FIRST_WAGON + 3, PIXELS(8) / 5, PIXELS(2) / 5);
  Event_PlayCue(SE_141);
  Actor_SetActionScript(ACTOR_CARAVAN_FIRST_WAGON, gCaravanFirstWagonArrives);
  Engine_EventWait(20);
  Actor_SetActionScript(ACTOR_CARAVAN_FIRST_WAGON + 1,
                        gCaravanSecondWagonArrives);
  Engine_EventWait(20);
  Actor_SetActionScript(ACTOR_CARAVAN_FIRST_WAGON + 2,
                        gCaravanThirdWagonArrives);
  Engine_EventWait(20);
  Actor_RunActionScriptAndWait(ACTOR_CARAVAN_FIRST_WAGON + 3,
                               gCaravanFourthWagonArrives);
  Event_PlayCue(AUDIO_CUE_FADE_SLOT3);
  spriteActor = Object_GetById(ACTOR_PARTY_LEADER);
  if (spriteActor != 0) {
    Actor_SetPosition(GARET, spriteActor->x.fixed, spriteActor->z.fixed);
  }
  Actor_WalkToAndWait(GARET, 5892, 1600);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetPosition(ACTOR_CARAVAN_HAMMET, PIXELS(5848), PIXELS(1592));
  Engine_TaskWait(1);
  // Hammet will not stay, and the wagons head south for the bridge.
  Engine_EventSetMessage(MsgWorldMapWeCantStayAnotherMinute);
  Event_ShowMessageAndWait(ACTOR_CARAVAN_HAMMET, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 10);
  Event_PlayCue(SE_141);
  Actor_SetActionScript(ACTOR_CARAVAN_FIRST_WAGON, gCaravanFirstWagonDeparts);
  Engine_EventWait(20);
  Actor_SetActionScript(ACTOR_CARAVAN_FIRST_WAGON + 1,
                        gCaravanSecondWagonDeparts);
  Engine_EventWait(20);
  Actor_SetActionScript(ACTOR_CARAVAN_FIRST_WAGON + 2,
                        gCaravanThirdWagonDeparts);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 10);
  Actor_RunActionScriptAndWait(ACTOR_CARAVAN_FIRST_WAGON + 3,
                               gCaravanFourthWagonDeparts);
  Event_PlayCue(AUDIO_CUE_FADE_SLOT3);
  Engine_EventWait(20);
  WorldMap_EndEvent();
  Engine_ActorSetAnimation(ACTOR_CARAVAN_FIRST_WAGON, ANIMATION_STAND);
  Engine_ActorSetAnimation(ACTOR_CARAVAN_FIRST_WAGON + 1, ANIMATION_STAND);
  Engine_ActorSetAnimation(ACTOR_CARAVAN_FIRST_WAGON + 2, ANIMATION_STAND);
  Engine_ActorSetAnimation(ACTOR_CARAVAN_FIRST_WAGON + 3, ANIMATION_STAND);
  Camera_MoveTo(PIXELS(5640), -1, PIXELS(1784), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  WorldMap_BeginEvent();
  Actor_SetPosition(ACTOR_CARAVAN_MAN, PIXELS(5640), PIXELS(1752));
  Engine_TaskWait(1);
  Actor_SetSpeed(ACTOR_CARAVAN_MAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkToAndWait(ACTOR_CARAVAN_MAN, 5640, 1736);
  Actor_WalkToAndWait(ACTOR_CARAVAN_MAN, 5624, 1736);
  Actor_WalkToAndWait(ACTOR_CARAVAN_MAN, 5624, 1784);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_CARAVAN_MAN, 2);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(ACTOR_CARAVAN_MAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_CARAVAN_MAN, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_CARAVAN_MAN, 3);
  Event_ShowMessageAndWait(ACTOR_CARAVAN_MAN, 0, 20);
  Actor_SetPosition(ACTOR_CARAVAN_HAMMET, PIXELS(5656), PIXELS(1784));
  Engine_TaskWait(1);
  Actor_SetChildValue(ACTOR_CARAVAN_HAMMET, SPRITE_COLOR_NORMAL);
  spriteActor = Actor_Get(ACTOR_CARAVAN_HAMMET);
  Engine_ActorSetSpriteFlags(spriteActor, SPRITE_FLAG_SHADOW);
  Actor_SetSpeed(ACTOR_CARAVAN_HAMMET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_CARAVAN_HAMMET, 5640, 1784);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(ACTOR_CARAVAN_HAMMET, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0,
                           10);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_SOUTHEAST + FACING_STEP, 60);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_WEST, 10);
  Actor_SetAttachedEffect(ACTOR_CARAVAN_HAMMET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_CARAVAN_MAN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_SetAttachedEffect(ACTOR_CARAVAN_HAMMET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Engine_ActorStartRepeatedMotion(ACTOR_CARAVAN_HAMMET, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0,
                           40);
  Engine_ActorRunRepeatedMotion(ACTOR_CARAVAN_MAN, 1);
  Actor_FaceDirection(ACTOR_CARAVAN_MAN, 0, 10);
  Event_ShowMessageAndWait(ACTOR_CARAVAN_MAN, 0, 10);
  Actor_ShowEmote(ACTOR_CARAVAN_HAMMET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0,
                           10);
  Engine_ActorRunRepeatedMotion(ACTOR_CARAVAN_HAMMET, 1);
  Engine_ActorSetAnimationAndWait(ACTOR_CARAVAN_HAMMET, ANIMATION_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0,
                           10);
  Actor_ShowEmote(ACTOR_CARAVAN_MAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(ACTOR_CARAVAN_MAN, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_CARAVAN_HAMMET, ANIMATION_NOD);
  WorldMap_EndEvent();
  Event_PlayCue(SE_107);
  Camera_SetSpeed(PIXELS(4), PIXELS(4));
  // The ground shakes, and a rock falls beside Hammet and his man.
  WorldMap_ShakeCameraOverCaravan();
  Event_PlayCue(AUDIO_CUE_FADE_SLOT3);
  Actor_ShowEmote(ACTOR_CARAVAN_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_CARAVAN_MAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_CARAVAN_MAN, 0, 40);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_CARAVAN_MAN, FACING_NORTHWEST + FACING_STEP, 0);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(5608), -1, PIXELS(1736), 1);
  Engine_CameraWaitForMove();
  Actor_SetPosition(ACTOR_FALLING_ROCK_GUIDE, PIXELS(5544), PIXELS(1704));
  Engine_TaskWait(1);
  Actor_SetSpeed(ACTOR_FALLING_ROCK_GUIDE, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_SetActionScript(ACTOR_FALLING_ROCK_GUIDE, gCaravanRockGuideScript);
  Engine_EventWait(160);
  // The falling rock drops from 64 pixels up, and bounces as it lands.
  actor->gravity = PIXELS(1) / 10;
  actor->bounce = PIXELS(1) / 10;
  actor->scaleX = 3 * Q16_ONE / 2;
  actor->scaleY = 3 * Q16_ONE / 2;
  actor->timer = 0;
  actor->y.fixed = PIXELS(64);
  actor->sprite->rotation = 0xf000;
  Engine_ActorSetSpriteFlags(actor, 0);
  Object_SetMode(actor, FALLING_ROCK_ANIM_TUMBLE);
  Engine_TaskWait(1);
  spriteActor = Actor_Get(ACTOR_FALLING_ROCK);
  Engine_ActorSetSpriteFlags(spriteActor, 0);
  FieldScheduler_AddCallback(WorldMap_UpdateFallingRock, TASK_PRIORITY_SCENE);
  do {
    Engine_TaskWait(1);
  } while (actor->timer == 0);
  spriteActor = Actor_Get(ACTOR_FALLING_ROCK);
  Engine_ActorSetSpriteFlags(spriteActor, 0);
  spriteActor = Actor_Get(ACTOR_FALLING_ROCK_GUIDE);
  Engine_ActorSetSpriteFlags(spriteActor, 0);
  Engine_EventWait(10);
  spriteActor = Actor_Get(ACTOR_CARAVAN_MAN);
  spriteActor->velocityY = PIXELS(6);
  spriteActor = Object_GetById(ACTOR_CARAVAN_HAMMET);
  spriteActor->velocityY = PIXELS(6);
  Event_PlayCue(SE_145);
  Camera_SetSpeed(PIXELS(4), PIXELS(4));
  WorldMap_ShakeCameraWhereRockFalls();
  WorldMap_ShakeCameraWhereRockFalls();
  Engine_EventWait(60);
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(5640), -1, PIXELS(1784), 1);
  Engine_CameraWaitForMove();
  WorldMap_BeginEvent();
  Actor_SetAttachedEffect(ACTOR_CARAVAN_MAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_CARAVAN_HAMMET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Engine_TaskRemoveCallback(WorldMap_UpdateFallingRock);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_FALLING_ROCK_GUIDE, 0, 0);
  Actor_SetPosition(ACTOR_FALLING_ROCK, 0, 0);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_WEST, 10);
  Engine_ActorJump(ACTOR_CARAVAN_HAMMET, 4, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0,
                           10);
  Actor_FaceDirection(ACTOR_CARAVAN_MAN, 0, 10);
  Event_ShowMessageAndWait(ACTOR_CARAVAN_MAN, 0, 20);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_NORTH, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0,
                           20);
  Engine_ActorJump(ACTOR_CARAVAN_MAN, 4, 20);
  Event_ShowMessageAndWait(ACTOR_CARAVAN_MAN, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_CARAVAN_HAMMET, 1);
  Actor_FaceDirection(ACTOR_CARAVAN_HAMMET, FACING_WEST, 10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CARAVAN_HAMMET, 0,
                           10);
  Actor_SetAttachedEffect(ACTOR_CARAVAN_MAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(80);
  Engine_ActorSetAnimationAndWait(ACTOR_CARAVAN_HAMMET, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_CARAVAN_MAN, 1);
  Engine_ActorSetAnimationAndWait(ACTOR_CARAVAN_MAN, ANIMATION_NOD);
  Actor_WalkToAndWait(ACTOR_CARAVAN_HAMMET, 5656, 1784);
  Actor_SetPosition(ACTOR_CARAVAN_HAMMET, 0, 0);
  Actor_WalkToAndWait(ACTOR_CARAVAN_MAN, 5624, 1736);
  Actor_WalkToAndWait(ACTOR_CARAVAN_MAN, 5640, 1736);
  Actor_WalkToAndWait(ACTOR_CARAVAN_MAN, 5640, 1752);
  Actor_SetPosition(ACTOR_CARAVAN_MAN, 0, 0);
  Event_PlayCue(SE_141);
  Actor_SetActionScript(ACTOR_CARAVAN_FIRST_WAGON, gCaravanFirstWagonReturns);
  Actor_SetActionScript(ACTOR_CARAVAN_FIRST_WAGON + 1,
                        gCaravanSecondWagonReturns);
  Engine_EventWait(40);
  Actor_SetActionScript(ACTOR_CARAVAN_FIRST_WAGON + 2,
                        gCaravanThirdWagonReturns);
  Engine_EventWait(40);
  Actor_RunActionScriptAndWait(ACTOR_CARAVAN_FIRST_WAGON + 3,
                               gCaravanFourthWagonReturns);
  WorldMap_EndEvent();
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(5900), PIXELS(1576));
  Actor_SetPosition(GARET, PIXELS(5908), PIXELS(1600));
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(5848), -1, PIXELS(1608), 1);
  Engine_CameraWaitForMove();
  action = gCaravanWagonHeadsNorth;
  Actor_SetActionScript(ACTOR_CARAVAN_FIRST_WAGON, action);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(5848), -1, PIXELS(1544), 1);
  Actor_SetActionScript(ACTOR_CARAVAN_FIRST_WAGON + 1, action);
  Engine_EventWait(20);
  Actor_SetActionScript(ACTOR_CARAVAN_FIRST_WAGON + 2, action);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_SetActionScript(ACTOR_CARAVAN_FIRST_WAGON + 3, action);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Object_RefreshSelectorById(ACTOR_CARAVAN_FIRST_WAGON + 3);
  Event_PlayCue(AUDIO_CUE_FADE_SLOT3);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(5880), -1, PIXELS(1608), 1);
  Engine_CameraWaitForMove();
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 80);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(GARET, ANIMATION_WALK);
  spriteActor = Object_GetById(ACTOR_PARTY_LEADER);
  if (spriteActor != 0) {
    Actor_SetDestination(GARET, spriteActor->x.part.pixel,
                         spriteActor->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Camera_MoveTo(PIXELS(5848), -1, PIXELS(1608), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 5848, 1576);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_GUARDS_SUSPECT_KALAY);
  Engine_EventRequestExit(WORLD_MAP_TO_VAULT_SOUTH);
  Engine_EventEnd();
}

/// Moves to the next camera-shake point and holds it for four frames.
static inline void WorldMap_MoveShakeCamera(s32 x, s32 z) {
  Camera_MoveTo(x, -1, z, 1);
  Engine_TaskWait(4);
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
/// FLAG_SCENE_TEMP_00 as it lands.
static void WorldMap_UpdateFallingRock(void) {
  struct FieldActor *actor;
  struct FieldActor *guide;

  actor = Object_GetById(ACTOR_FALLING_ROCK);
  guide = Object_GetById(ACTOR_FALLING_ROCK_GUIDE);
  actor->x.fixed = guide->x.fixed;
  actor->z.fixed = guide->z.fixed;
  if (actor->y.fixed < PIXELS(10)) {
    actor->y.fixed = PIXELS(10);
    if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) == 0) {
      Event_PlayCue(SE_145);
      Object_SetMode(actor, FALLING_ROCK_ANIM_SHATTER);
      GameFlag_Set(FLAG_SCENE_TEMP_00);
      actor->timer = 1;
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
  struct FieldActor *actor;

  actor = Object_GetById(ACTOR_CROSSING_SHIP);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  actor->scaleY = 5 * Q16_ONE / 4;
  actor->scaleX = 5 * Q16_ONE / 4;
  Engine_CameraFollowActor(ACTOR_CROSSING_SHIP, 1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_CROSSING_SHIP, 5288, 2328);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_TOLBI_SHIP_ROWING);
  Engine_EventRequestExit(WORLD_MAP_TO_TOLBI_SHIP_MAST_FIRST_ATTACK);
  Engine_EventEnd();
}

/// The middle legs of the crossing: the ship, seen from above, sails west
/// across the Karagol Sea from the far east of the map until
/// gCrossingShipSailsWest says it has gone far enough, and the party leaves
/// by exit, back aboard. Sets FLAG_TOLBI_SHIP_ROWING.
static inline void WorldMap_SailCrossingLeg(s32 exit) {
  struct FieldActor *actor;

  actor = Object_GetById(ACTOR_CROSSING_SHIP);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_CROSSING_SHIP, PIXELS(7944), PIXELS(200));
  actor->scaleX = 5 * Q16_ONE / 4;
  actor->scaleY = 5 * Q16_ONE / 4;
  Engine_TaskWait(1);
  Engine_CameraFollowActor(ACTOR_CROSSING_SHIP, 1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, PIXELS(3) / 5, PIXELS(3) / 10);
  actor->timer = 0;
  Actor_SetActionScript(ACTOR_CROSSING_SHIP, gCrossingShipSailsWest);
  do {
    Engine_TaskWait(1);
  } while (actor->timer == 0);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_TOLBI_SHIP_ROWING);
  Engine_EventRequestExit(exit);
  Engine_EventEnd();
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
  struct FieldActor *actor;

  actor = Object_GetById(ACTOR_CROSSING_SHIP);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_CROSSING_SHIP, PIXELS(7944), PIXELS(200));
  actor->scaleX = 5 * Q16_ONE / 4;
  actor->scaleY = 5 * Q16_ONE / 4;
  Engine_TaskWait(1);
  Engine_CameraFollowActor(ACTOR_CROSSING_SHIP, 1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, PIXELS(3) / 5, PIXELS(3) / 10);
  actor->timer = 0;
  if (WorldMap_GetRowingBalance() == ROWING_BALANCE_OFF_COURSE) {
    Actor_SetActionScript(ACTOR_CROSSING_SHIP, gCrossingShipVeersNorth);
  } else {
    Actor_SetActionScript(ACTOR_CROSSING_SHIP, gCrossingShipSailsWest);
  }
  do {
    Engine_TaskWait(1);
  } while (actor->timer == 0);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_TOLBI_SHIP_ROWING);
  Engine_EventRequestExit(WORLD_MAP_TO_TOLBI_SHIP_MAST_LAND_HO);
  Engine_EventEnd();
}

/// The crossing's last leg: the ship sails into Tolbi's harbour, and
/// WORLD_MAP_TO_TOLBI_SHIP_DECK leads back aboard as it docks
/// (TOLBI_SHIP_DECK_FROM_WORLD_MAP).
/// Called by WorldMap_Initialize at WORLD_MAP_FROM_TOLBI_SHIP_DECK_REACH_TOLBI;
/// sets FLAG_TOLBI_SHIP_REACHED_TOLBI and clears FLAG_TOLBI_SHIP_ROWING.
static void WorldMap_SailIntoTolbi(void) {
  struct FieldActor *actor;

  actor = Object_GetById(ACTOR_CROSSING_SHIP);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_CROSSING_SHIP, PIXELS(5096), PIXELS(2328));
  actor->scaleY = 5 * Q16_ONE / 4;
  actor->scaleX = 5 * Q16_ONE / 4;
  Engine_TaskWait(1);
  Engine_CameraFollowActor(ACTOR_CROSSING_SHIP, 1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_CROSSING_SHIP, 5064, 2328);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_TOLBI_SHIP_REACHED_TOLBI);
  GameFlag_Clear(FLAG_TOLBI_SHIP_ROWING);
  Engine_EventRequestExit(WORLD_MAP_TO_TOLBI_SHIP_DECK);
  Engine_EventEnd();
}

/// Demonstrates setting a Djinni and placing it on standby.
/// Called by Event_MeetFlint; sets and clears FLAG_DJINN_LESSON_PART_A and
/// _PART_B to choose the menu's two lessons.
static void WorldMap_ShowDjinnTutorial(void) {
  WorldMap_EndEvent();
  FieldEffect_ScheduleRatioTransition(Q16_ONE, 6);
  Event_WaitForZoomTransition();
  WorldMap_BeginEvent();
  Engine_ActorRunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
  Engine_EventSetMessage(MsgWorldMapAfterBringingDjinniIntoYour);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Engine_EventWait(30);
  Event_PlayCue(SE_MENU_CURSOR_MOVE);
  Menu_AnimateSelectionToEntry(0, 2);
  GameFlag_Clear(FLAG_DJINN_LESSON_PART_A);
  GameFlag_Clear(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Open();
  Engine_ActorJump(ACTOR_VENUS_DJINNI, 4, 30);
  Engine_EventSetMessage(MsgWorldMapNextIllShowHowCan);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  GameFlag_Clear(FLAG_DJINN_LESSON_PART_A);
  GameFlag_Set(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Open();
  Engine_EventWait(30);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 6);
}

/// Walking up to Flint: he joins the party and shows how Djinn are set and put
/// on standby; then comes the tutorial battle.
/// Reached by walking onto TRIGGER_FLINT until Flint is caught
/// (FLAG_DJINNI_0 + DJINNI_FLINT). Sets FLAG_DJINN_LESSON and its two part
/// flags for each lesson, and clears them all after the battle.
static void Event_MeetFlint(void) {
  struct FieldActor *djinni;
  struct FieldActor *leader;
  s32 x;
  s32 z;
  s32 count;
  s32 i;

  count = 0;
  djinni = Object_GetById(ACTOR_VENUS_DJINNI);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  x = (leader->x.fixed - PIXELS(5584)) / 2 + PIXELS(5584);
  z = (leader->z.fixed - PIXELS(1328)) / 2 + PIXELS(1328);
  // Introduce the Djinni and demonstrate how it joins the party.
  if (!GameFlag_IsSet(FLAG_DJINN_LESSON)) {
    Party_RestoreHpPp(1);
    GameFlag_Set(FLAG_DJINN_LESSON);
    Event_Begin();
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0)
      Actor_SetPosition(ACTOR_VENUS_DJINNI, leader->x.fixed, leader->z.fixed);
    Djinn_AddToOwner(ISAAC, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_FLINT));
    DjinnRecovery_AddStandby(0, 0, 0);
    WorldMap_BeginEvent();
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_VENUS_DJINNI, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    djinni->work = 1;
    Actor_FaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
    Task_Wait(16);
    Event_SetMessage(MsgWorldMapOh);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    WorldMap_EndEvent();
    FieldEffect_ScheduleRatioTransition(6 * Q16_ONE / 5, 6);
    Event_WaitForZoomTransition();
    WorldMap_BeginEvent();
    djinni->motionFlags = ACTOR_FALLS;
    djinni->gravity = PIXELS(1) / 4;
    djinni->speed = PIXELS(1);
    djinni->acceleration = PIXELS(1);
    djinni->velocityY = 0;
    djinni->terrainHeight = 0;
    FieldObject_SetPosition(djinni, PIXELS(5584), 0, PIXELS(1328));
    for (i = 0; i < 16; i++) {
      djinni->scaleX += Q16_ONE / 32;
      djinni->scaleY += Q16_ONE / 32;
      Task_Wait(1);
    }
    Actor_FaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_VENUS_DJINNI, 0);
    Task_Wait(16);
    djinni->update = 0;
    Engine_ObjectSetPartPalettes(djinni, SPRITE_COLOR_NORMAL);
    djinni->gravity = PIXELS(1);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Event_PlayCue(SE_131);
    Engine_PsynergyBegin(140, 0);
    for (i = 59; i >= 0; i--) {
      if (gFrameCount & 2)
        Engine_ObjectSetPartPalettes(djinni, SPRITE_COLOR_WHITE);
      else
        Engine_ObjectSetPartPalettes(djinni, SPRITE_COLOR_NORMAL);
      if ((gFrameCount & 15) == 0)
        WorldMap_CreateLinkedEffects(djinni);
      Task_Wait(1);
    }
    FieldEffect_End();
    Engine_ObjectSetPartPalettes(djinni, SPRITE_COLOR_NORMAL);
    Actor_RunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    Actor_WalkToAndWait(ACTOR_VENUS_DJINNI, x >> 16, z >> 16);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIMATION_LEADER_LEAN_BACK);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Actor_Jump(ACTOR_VENUS_DJINNI, 4, 30);
    UiWork_PushValueSlot(300, 4);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_Jump(ACTOR_VENUS_DJINNI, 2, 30);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    i = 0;
    djinni->motionFlags = 0;
    FieldObject_SetPosition(djinni, x, PIXELS(16), z);
    for (; i < 16; i++) {
      djinni->facing += FACING_STEP;
      Task_Wait(1);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIMATION_STAND);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    djinni->motionFlags = ACTOR_FALLS;
    djinni->velocityY = 0;
    djinni->terrainHeight = 0;
    for (i = 7; i >= 0; i--) {
      djinni->facing += FACING_STEP;
      Task_Wait(1);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIMATION_LEADER_LEAN_BACK);
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
    if (Event_ChooseYesNo(0, 0) == 1) {
      Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
      Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
      if (count == 6) {
        Event_SetMessage(MsgWorldMapMeanieDontCare);
        Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
        goto listened;
      }
      Event_SetMessage(count + MsgWorldMapComePromiseWont);
      Event_OpenMessage(ACTOR_VENUS_DJINNI, 0);
      count++;
      goto plead;
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIMATION_LEADER_LEAN_BACK);
    Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
    Actor_Jump(ACTOR_VENUS_DJINNI, 4, 20);
    Event_SetMessage(MsgWorldMapSeeWontRegret);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  listened:
    UiWork_PushValueSlot(300, 4);
    Event_PlayCue(MUS_JOINED);
    count = MsgWorldMapAbilityVenusDjinni;
    Message_ShowCentered(count++, 3);
    Event_SetMessage(count);
    Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Event_PlayCue(MUS_009);
    WorldMap_ShowDjinnTutorial();
    WorldMap_EndEvent();
    return;
  }
  Event_Begin();
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0)
    Actor_SetPosition(ACTOR_VENUS_DJINNI, leader->x.fixed, leader->z.fixed);
  djinni->velocityY = PIXELS(10);
  FieldObject_SetPosition(djinni, x, 0, z);
  Event_Wait(30);
  WorldMap_BeginEvent();
  Actor_FaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_VENUS_DJINNI, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIMATION_LEADER_LEAN_BACK);
  // Offer the tutorial again, then demonstrate setting and standby.
  Event_SetMessage(MsgWorldMapSeeDjinnUseful);
  Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
  Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Actor_RunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Event_PlayCue(SE_MENU_CURSOR_MOVE);
  Menu_AnimateSelectionToEntry(0, 2);
  GameFlag_Set(FLAG_DJINN_LESSON_PART_A);
  GameFlag_Clear(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Open();
  Event_SetMessage(MsgWorldMapGoSetStandby);
  FieldObject_SetPosition(djinni, PIXELS(5584), 0, PIXELS(1328));
  Event_Wait(30);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Actor_FaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Event_OpenMessage(ACTOR_VENUS_DJINNI, 0);
  if (Event_ChooseYesNo(0, 0) == 1) {
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIMATION_LEADER_LEAN_BACK);
    Actor_RunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
    Event_SetMessage(MsgWorldMapHmmmmExplainAgain);
    Event_OpenMessage(ACTOR_VENUS_DJINNI, 0);
    if (Event_ChooseYesNo(0, 0) != 1) {
      Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
      Actor_WalkToAndWait(ACTOR_VENUS_DJINNI, x >> 16, z >> 16);
      WorldMap_ShowDjinnTutorial();
      WorldMap_EndEvent();
      return;
    }
  }
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIMATION_LEADER_LEAN_BACK);
  Event_SetMessage(MsgWorldMapYeahWantLearn);
  Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
  Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Actor_ShowEmote(ACTOR_VENUS_DJINNI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  GameFlag_Set(FLAG_DJINN_LESSON_PART_A);
  GameFlag_Set(FLAG_DJINN_LESSON_PART_B);
  DjinnMenu_Open();
  Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  WorldMap_EndEvent();
  Djinn_JoinParty(ACTOR_VENUS_DJINNI, ELEMENT_VENUS,
                  DJINNI_INDEX(DJINNI_FLINT));
  Event_PlayCue(MUS_WORLD_MAP);
  Event_End();
  GameFlag_Clear(FLAG_DJINN_LESSON);
  GameFlag_Clear(FLAG_DJINN_LESSON_PART_A);
  GameFlag_Clear(FLAG_DJINN_LESSON_PART_B);
}

s32 gWorldMapIodem;

/// Brings Bunza's wagon to a stop and lets Isaac step down, after Hammet's
/// return home (kalay.c's Kalay_HammetReturnsToPalace).
/// Called by WorldMap_Initialize at WORLD_MAP_FROM_KALAY_TUNNEL_ENTRANCE_SOUTH.
static void WorldMap_StepDownFromBunzasWagon(void) {
  struct FieldActor *actor;

  actor = Object_GetById(ACTOR_KALAY_WAGON);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_TaskWait(1);
  actor->scaleX = 3 * Q16_ONE / 2;
  actor->scaleY = 3 * Q16_ONE / 2;
  actor->facing = FACING_SOUTH;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_SetPosition(ACTOR_KALAY_WAGON, PIXELS(5480), PIXELS(2104));
  Engine_TaskWait(1);
  Event_PlayCue(SE_141);
  Actor_SetSpeed(ACTOR_KALAY_WAGON, PIXELS(8) / 5, PIXELS(2) / 5);
  Engine_ActorSetAnimation(ACTOR_KALAY_WAGON, ANIMATION_WALK);
  Actor_MoveToAndWait(ACTOR_KALAY_WAGON, 5485, 2136);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(5560), -1, PIXELS(2136), 1);
  Actor_MoveToAndWait(ACTOR_KALAY_WAGON, 5534, 2136);
  Actor_MoveToAndWait(ACTOR_KALAY_WAGON, 5544, 2158);
  Actor_MoveToAndWait(ACTOR_KALAY_WAGON, 5608, 2168);
  Engine_ActorSetAnimation(ACTOR_KALAY_WAGON, ANIMATION_STAND);
  Event_PlayCue(AUDIO_CUE_FADE_SLOT3);
  Engine_EventWait(20);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(5592), PIXELS(2168));
  Engine_TaskWait(1);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 5576, 2168);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
  Event_PlayCue(SE_141);
  Engine_ActorSetAnimation(ACTOR_KALAY_WAGON, ANIMATION_WALK);
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
  Event_PlayCue(AUDIO_CUE_FADE_SLOT3);
  Engine_EventWait(40);
  Camera_MoveTo(PIXELS(5592), -1, PIXELS(2136), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 5592, 2136);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(WORLD_MAP_TO_KALAY_EAST);
  Engine_EventEnd();
}

/// Out of Gateway Cave: Iodem sees Babi's ship scuttled off the coast, gives
/// Isaac the Black Orb and prompts him to use it on the ship.
/// Called by WorldMap_Initialize at WORLD_MAP_FROM_GATEWAY_CAVE until the Black
/// Orb is handed back and before the wounded soldiers are seen in Lalivero
/// (FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN). Sets FLAG_SCENE_TEMP_34 and
/// FLAG_SAW_SHIP_AT_IDEJIMA, and turns Gateway Cave's exit into
/// Event_IodemCallsLeaderBack.
static void WorldMap_RunBlackOrbScene(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  gWorldMapIodem = ACTOR_IODEM;
  WorldMap_BlockGatewayCaveExit(ACTOR_IODEM);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  WorldMap_BeginEvent();
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ActorSetPosition(gWorldMapIodem, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(gWorldMapIodem, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(gWorldMapIodem, 5992, 3448);
  Engine_ActorFaceDirection(gWorldMapIodem, FACING_EAST, 20);
  Engine_ActorShowEmote(gWorldMapIodem, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Engine_ActorStartRepeatedMotion(gWorldMapIodem, 2);
  Engine_EventSetMessage(MsgWorldMapLook);
  Engine_EventShowMessageAndWait(gWorldMapIodem | SPEAKER_PORTRAIT_LEFT, 0, 10);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  WorldMap_EndEvent();
  FieldEffect_ScheduleRatioTransition(7 * Q16_ONE / 5, 10);
  Engine_CameraSetSpeed(PIXELS(8), PIXELS(1));
  Engine_CameraMoveTo(PIXELS(6024), -1, PIXELS(3432), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  WorldMap_BeginEvent();
  Engine_ActorSetAttachedEffect(gWorldMapIodem, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(gWorldMapIodem, 1);
  Engine_EventShowMessageAndWait(
      gWorldMapIodem | SPEAKER_PORTRAIT_LEFT | SPEAKER_PORTRAIT_RIGHT, 0, 40);
  Actor_WalkToAndWait(gWorldMapIodem, 5992, 3400);
  Engine_ActorWalkToAndWait(gWorldMapIodem, 6036, 3400);
  Engine_ActorFaceDirection(gWorldMapIodem, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gBlackOrbLeaderScript);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(gWorldMapIodem, 1);
  Engine_EventShowMessageAndWait(gWorldMapIodem, 0, 10);
  while (Object_GetById(ACTOR_PARTY_LEADER)->timer == 0) {
    Engine_TaskWait(1);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
  Engine_ActorShowEmote(gWorldMapIodem, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Engine_ActorRunRepeatedMotion(gWorldMapIodem, 2);
  Engine_EventShowMessageAndWait(gWorldMapIodem, 0, 10);
  Engine_ActorFaceDirection(gWorldMapIodem, FACING_WEST, 10);
  Engine_EventShowMessage(gWorldMapIodem, 0);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
  Object_GetById(gWorldMapIodem)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(gWorldMapIodem, 6028, 3400);
  Engine_EventWait(1);
  Object_GetById(gWorldMapIodem)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(20);
  Object_GetById(gWorldMapIodem)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorWalkToAndWait(gWorldMapIodem, 6036, 3400);
  Engine_EventWait(1);
  Object_GetById(gWorldMapIodem)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ItemShowFound(ITEM_BLACK_ORB, 3);
  Engine_PartyGiveItem(ITEM_BLACK_ORB, 0);
  Engine_ActorRunRepeatedMotion(gWorldMapIodem, 1);
  Engine_EventSetMessage(MsgWorldMapNowUseOnShip);
  Engine_EventShowMessageAndWait(gWorldMapIodem, 0, 10);
  Engine_ActorFaceDirection(gWorldMapIodem, FACING_SOUTHEAST + FACING_STEP, 0);
  WorldMap_EndEvent();
  FieldEffect_ScheduleRatioTransition(Q16_ONE, 10);
  Engine_EventWait(20);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_34);
  Engine_GameFlagSet(FLAG_SAW_SHIP_AT_IDEJIMA);
  gGameState.savedScene = (s32)&SceneId_WorldMap;
  gGameState.savedEntrance = WORLD_MAP_AFTER_BLACK_ORB_DEFEAT;
  Engine_EventEnd();
}

/// Turns Gateway Cave's exit (the event of ACTOR_PLACE_GATEWAY_CAVE's
/// PLACE_TRIGGER) into a touch event that runs Event_IodemCallsLeaderBack, and
/// stands Iodem (ACTOR_IODEM, whose placement row it finds by his sprite) at
/// pixel (6036, 3400); the actor given is not used.
/// Called by WorldMap_RunBlackOrbScene, and by WorldMap_Initialize when the
/// party returns to WORLD_MAP_FROM_GATEWAY_CAVE with the orb still to use.
static void WorldMap_BlockGatewayCaveExit(s32 actor) {
  s32 i;

  i = 0;
  while (1) {
    if (gWorldMapEvents[i].control == EVENT_EXIT &&
        gWorldMapEvents[i].trigger == PLACE_TRIGGER(ACTOR_PLACE_GATEWAY_CAVE)) {
      gWorldMapEvents[i].control = EVENT_TOUCH;
      gWorldMapEvents[i].value = (u32)Event_IodemCallsLeaderBack;
    }
    if (gWorldMapEvents[i].control == SCENE_EVENTS_END)
      break;
    i++;
  }
  for (i = 0;; i++) {
    if (gWorldMapPlacements[i].sprite == SPRITE_IODEM) {
      gWorldMapPlacements[i].x = PIXELS(6036);
      gWorldMapPlacements[i].z = PIXELS(3400);
      gWorldMapPlacements[i].facing = FACING_SOUTHEAST + FACING_STEP;
      return;
    }
  }
}

/// Makes touching Gateway Cave an exit again, to WORLD_MAP_TO_GATEWAY_CAVE.
/// Called by Event_UseBlackOrb once the orb is handed back.
static void WorldMap_RestoreGatewayCaveExit(void) {
  s32 i;

  i = 0;
  while (1) {
    if (gWorldMapEvents[i].control == EVENT_TOUCH &&
        gWorldMapEvents[i].trigger == PLACE_TRIGGER(ACTOR_PLACE_GATEWAY_CAVE)) {
      gWorldMapEvents[i].control = EVENT_EXIT;
      gWorldMapEvents[i].value = WORLD_MAP_TO_GATEWAY_CAVE;
      break;
    }
    if (gWorldMapEvents[i].control == SCENE_EVENTS_END)
      break;
    i++;
  }
}

/// Talking to Iodem, who gave the Black Orb (or reading his mind): he urges
/// its use. "Now use it... on the ship."
/// Reached by talking to ACTOR_IODEM, or by Mind Read on him.
static void Event_TalkAboutBlackOrb(void) {
  Engine_EventBegin();
  Actor_FaceActor(ACTOR_IODEM, ACTOR_PARTY_LEADER, 0);
  Engine_EventSetMessage(MsgWorldMapNowUseOnShip);
  Engine_EventShowMessage(gWorldMapIodem, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventEnd();
}

/// Walking back into Gateway Cave with the Black Orb unused: Iodem calls the
/// leader back to the coast. "Isaac, where you going? I said to use the Orb on
/// Babi's ship!"
/// Reached by touching Gateway Cave while WorldMap_BlockGatewayCaveExit has
/// made it a touch event.
static void Event_IodemCallsLeaderBack(void) {
  Engine_EventBegin();
  WorldMap_BeginEvent();
  Engine_EventSetMessage(MsgWorldMapIsaacWhereGoingSaidUse);
  Engine_EventShowMessage(gWorldMapIodem, 0);
  WorldMap_EndEvent();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 6008, 3400);
  Engine_EventEnd();
}

/// Searching the wreck off Gateway Cave: "The wreckage of a ship scuttled off
/// the coast." While Iodem waits for the Black Orb to be used
/// (FLAG_SCENE_TEMP_34) the main menu opens as the search ends, so that the orb
/// can be used.
/// Reached by searching TRIGGER_SHIPWRECK.
static void Event_SearchShipwreck(void) {

  Engine_EventBegin();
#if !TBS_PATCH_3
  WorldMap_BeginEvent();
#endif
  Engine_MessageShowCentered(MsgWorldMapWreckageShipScuttledOffCoast, 1);
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_34) != 0) {
    gEventWork->requestedMenu = 1;
  }
#if !TBS_PATCH_3
  WorldMap_EndEvent();
#endif
  Engine_EventEnd();
}

/// Using the Black Orb on the shipwreck: nothing happens, the orb is taken back
/// and the party is to press on to Babi Lighthouse.
/// Reached by using the Black Orb on TRIGGER_SHIPWRECK until it is handed back.
/// Clears FLAG_SCENE_TEMP_34 and sets FLAG_BLACK_ORB_RETURNED.
static void Event_UseBlackOrb(void) {
  struct FieldActor *iodem;
  struct FieldActor *orb;
  struct FieldActor *leader;
  struct FieldActor *actor;
  s32 message;
  struct AnimationObject *sprite;
  u8 *buffer;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Begin();
  WorldMap_EndEvent();
  FieldEffect_ScheduleRatioTransition(7 * Q16_ONE / 5, 6);
  Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  Camera_MoveTo(PIXELS(6024), -1, PIXELS(3432), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  iodem = NULL;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIMATION_WALK);
  leader->paused = 0;
  ObjectDispatch_StartIdle((struct DispatchObject *)leader);
  if (leader->z.fixed > PIXELS(3432)) {
    if (leader->x.fixed > PIXELS(5998)) {
      FieldObject_SetPosition(leader, PIXELS(5998), leader->y.fixed,
                              PIXELS(3453));
      FieldObject_WaitForMove(leader);
    }
  } else if (leader->x.fixed > PIXELS(6010)) {
    // BUG: iodem is NULL here; this branch reads its height through null.
    FieldObject_SetPosition(leader, PIXELS(6010), iodem->y.fixed, PIXELS(3400));
    FieldObject_WaitForMove(leader);
  }
  FieldObject_SetPosition(leader, PIXELS(5993), 0, PIXELS(3432));
  FieldObject_WaitForMove(leader);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIMATION_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
  WorldMap_BeginEvent();
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIMATION_LEADER_ARMS_RAISED);
  // Present the orb icon before its sparks and the ship's response.
  orb = Object_Create(SPRITE_BLANK, leader->x.fixed + PIXELS(2), PIXELS(38),
                      leader->z.fixed);
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
    Event_Wait(20);
    orb->update = WorldMap_PulseOrbAndSpawnSparks;
    Event_Wait(80);
  }
  Object_GetById(gWorldMapIodem)->facing = FACING_SOUTHEAST + FACING_STEP;
  Actor_ShowEmote(gWorldMapIodem, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(gWorldMapIodem, 2);
  message = MsgWorldMapMatter;
  Event_SetMessage(message);
  Event_ShowMessageAndWait(gWorldMapIodem, 0, 80);
  if (orb != NULL) {
    Engine_ObjectDispatchRelease(orb);
  }
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIMATION_STAND);
  Event_Wait(40);
  Actor_Jump(gWorldMapIodem, 6, 40);
  Event_ShowMessageAndWait(gWorldMapIodem, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(gWorldMapIodem, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(
      gWorldMapIodem | SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT, 0, 40);
  Actor_SetAnimationAndWait(gWorldMapIodem, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(
      gWorldMapIodem | SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT, 0, 20);
  Actor_FaceDirection(gWorldMapIodem, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(gWorldMapIodem, 0, 10);
  Actor_SetSpeed(gWorldMapIodem, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetAnimation(gWorldMapIodem, ANIMATION_WALK);
  iodem = Actor_Get(ACTOR_IODEM);
  FieldObject_SetPosition(iodem, PIXELS(6010), iodem->y.fixed, PIXELS(3400));
  FieldObject_WaitForMove(iodem);
  FieldObject_SetPosition(iodem, PIXELS(6001), 0, PIXELS(3416));
  FieldObject_WaitForMove(iodem);
  Actor_SetAnimation(ACTOR_IODEM, ANIMATION_STAND);
  Actor_FaceDirection(gWorldMapIodem, FACING_SOUTH + FACING_STEP, 10);
  Actor_RunRepeatedMotion(gWorldMapIodem, 1);
  Event_ShowMessageAndWait(gWorldMapIodem | SPEAKER_PORTRAIT_LEFT, 0, 20);
  Actor_SetSpeed(gWorldMapIodem, PIXELS(1), PIXELS(1) / 2);
  Object_GetById(ACTOR_IODEM)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetAnimation(ACTOR_IODEM, ANIMATION_WALK);
  FieldObject_SetPosition(iodem, PIXELS(5997), 0, PIXELS(3424));
  FieldObject_WaitForMove(iodem);
  Actor_SetAnimation(ACTOR_IODEM, ANIMATION_STAND);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_IODEM, ANIMATION_WALK);
  FieldObject_SetPosition(iodem, PIXELS(6001), 0, PIXELS(3416));
  FieldObject_WaitForMove(iodem);
  Actor_SetAnimation(ACTOR_IODEM, ANIMATION_STAND);
  Message_ShowCentered(message + 6, 1);
  gEventWork->message++;
  PartyInventory_Discard(ITEM_BLACK_ORB);
  Event_Wait(20);
  Actor_SetAnimation(gWorldMapIodem, ANIMATION_SHAKE_HEAD);
  Event_ShowMessageAndWait(gWorldMapIodem, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Actor_SetAnimationAndWait(gWorldMapIodem, ANIMATION_NOD);
  Actor_SetAnimation(gWorldMapIodem, ANIMATION_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(gWorldMapIodem, actor->x.part.pixel,
                         actor->z.part.pixel);
  }
  Actor_WaitForMove(gWorldMapIodem);
  Actor_SetPosition(gWorldMapIodem, 0, 0);
  WorldMap_EndEvent();
  FieldEffect_ScheduleRatioTransition(Q16_ONE, 6);
  Event_Wait(20);
  WorldMap_RestoreGatewayCaveExit();
  Engine_ActorDestroy(gWorldMapIodem);
  GameFlag_Clear(FLAG_SCENE_TEMP_34);
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
  PaletteGlow_Update(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
  Engine_EventBegin();
  FieldEffect_ScheduleRatioTransition(Q16_ONE, 300);
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_ActorSetAnimation(JENNA, ANIMATION_JENNA_LIE);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIMATION_KRADEN_LIE);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_TaskWait(1);
  FieldEffect_ScheduleRatioTransition(3 * Q16_ONE / 2, 16);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_WARM, 1);
  gEventWork->transitionFrames = 16;
  Engine_EventOpenScreen();
  Event_WaitForZoomTransition();
  WorldMap_BeginEvent();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  Engine_EventWait(20);
  // Jenna and Kraden talk on the drifting island before Alex arrives.
  Engine_EventSetMessage(MsgWorldMapKradenHowLongWillIsland);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 20);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 100);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  Event_ShowMessageAndWait(JENNA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(80);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 20);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 120);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  Event_ShowMessageAndWait(JENNA, 0, 40);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 20);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessageAndWait(JENNA, 0, 120);
  // Alex comes to show them something and leads them on.
  Actor_SetSpeed(ACTOR_ALEX, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetPosition(ACTOR_ALEX, PIXELS(7644), PIXELS(3460));
  Actor_WalkToAndWait(ACTOR_ALEX, 7572, 3468);
  Actor_WalkToAndWait(ACTOR_ALEX, 7560, 3488);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_ALEX, ANIMATION_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX,
                    0);
  Map_LoadDefaultCellsAndUpdateBlock();
  Engine_EventWait(20);
  Actor_SetActionScript(ACTOR_ALEX, gEndingAlexWalk);
  Engine_EventWait(80);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIMATION_STAND);
  Engine_ActorJump(ACTOR_KRADEN, 4, 40);
  Engine_ActorSetAnimation(JENNA, ANIMATION_STAND);
  Engine_ActorJump(JENNA, 4, 60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 40);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(JENNA, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetActionScript(ACTOR_KRADEN, gEndingKradenWalk);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(7) / 10, PIXELS(7) / 80);
  Camera_MoveTo(PIXELS(7736), -1, PIXELS(3528), 1);
  Actor_SetActionScript(JENNA, gEndingJennaWalk);
  do {
    Engine_ActorSetAnimation(ACTOR_FELIX, ANIMATION_FELIX_LIE);
    Engine_ActorSetAnimation(SHEBA, ANIMATION_SHEBA_LIE);
    Engine_TaskWait(1);
  } while (Object_GetById(JENNA)->timer == 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_WEST, 20);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  WorldMap_BeginEvent();
  Event_ShowTwoMessagesAndWait(JENNA, 7, 13, 2, 12, ACTOR_KRADEN, 9, 4, 4, 3,
                               0);
  Engine_EventWait(20);
  Map_LoadDefaultCellsAndUpdateBlock();
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(7768), -1, PIXELS(3528), 1);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(JENNA, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkTo(ACTOR_KRADEN, 7804, 3512);
  Actor_WalkToAndWait(JENNA, 7788, 3544);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIMATION_STAND);
  WorldMap_BeginEvent();
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 20);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | JENNA, 0, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_KRADEN, 2);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 60);
  Map_LoadDefaultCellsAndUpdateBlock();
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
  ColorBuffer_ApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(120);
  Engine_TaskWait(120);
  Event_SetDestination((s32)&SceneId_Title, TITLE_FROM_WORLD_MAP);
}

/// The look back before Sheba's fall: the view settles on Venus Lighthouse,
/// which shakes in clouds of dust (WorldMap_UpdateLighthouseQuake) as the
/// colours fade, and WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE leads back up to the
/// aerie, its music kept.
/// Called by WorldMap_Initialize at WORLD_MAP_BEFORE_SHEBAS_FALL; sets
/// FLAG_KEEP_MUSIC.
static void WorldMap_ShakeVenusLighthouse(void) {
  Engine_EventBegin();
  Engine_AudioPlayCue(SE_141);
  Engine_ColorBufferApplySource(0, 0);
  Engine_ColorBufferApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(2);
  gEventWork->transitionFrames = 1;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Engine_TaskWait(1);
  Engine_CameraSetSpeed(PIXELS(4), PIXELS(1) / 2);
  {
    s32 priority = TASK_PRIORITY_SCENE;
    SchedulerCallback callback = WorldMap_UpdateLighthouseQuake;
    FieldScheduler_AddCallback(callback, priority);
  }
  Engine_ColorBufferApplySource(0, 0);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_MUTED, 1);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, 2);
  Engine_ColorBufferInterpolate(40);
  Engine_EventWait(240);
  Engine_ColorBufferApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(80);
  Engine_TaskWait(90);
  Engine_EventRequestExit(WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE);
  Engine_GameFlagSet(FLAG_KEEP_MUSIC);
  Engine_EventEnd();
}

/// Run each frame while Venus Lighthouse shakes: it puffs up dust at a random
/// spot in the QUAKE_DUST box, and every third frame moves the camera a pixel
/// to one of four points around the lighthouse.
static void WorldMap_UpdateLighthouseQuake(void) {
  u32 x = (u32)Engine_RandomNext() * QUAKE_DUST_WIDTH >> 16;
  u32 z = (u32)Engine_RandomNext() * QUAKE_DUST_DEPTH >> 16;
  struct FieldActor *object;

  object =
      Engine_ObjectCreate(SPRITE_DUST_PUFF, PIXELS(x) + PIXELS(QUAKE_DUST_LEFT),
                          0, PIXELS(z) + PIXELS(QUAKE_DUST_TOP));
  if (object != NULL) {
    struct AnimationObject *sprite = object->sprite;
    s32 scale = (((u32)Engine_RandomNext() << 15) >> 16) + 6 * Q16_ONE / 5;

    sprite->flags = 0;
    sprite->image.priority = 2;
    object->motionFlags = 0;
    object->scaleX = scale;
    object->scaleY = scale;
    Object_SetMode(object, 1);
    Engine_ObjectSetScript(object, gQuakeDustScript);
  }
  if (gFrameCount % 3 == 0) {
    switch (((u32)Engine_RandomNext() << 2) >> 16) {
    case 0:
      Engine_CameraMoveTo(PIXELS(6087), -1, PIXELS(3177), 1);
      break;
    case 1:
      Engine_CameraMoveTo(PIXELS(6089), -1, PIXELS(3175), 1);
      break;
    case 2:
      Engine_CameraMoveTo(PIXELS(6089), -1, PIXELS(3177), 1);
      break;
    case 3:
      Engine_CameraMoveTo(PIXELS(6087), -1, PIXELS(3175), 1);
      break;
    }
  }
}

/// A leg of the Tolbi-bound Ship's crossing: out of the ship's interior
/// (TOLBI_SHIP_INTERIORS_TO_WORLD_MAP) the ship sails north to its anchorage
/// off the island, where ACTOR_TOLBI_SHIP stands, and the party goes back into
/// it (WORLD_MAP_TO_TOLBI_SHIP_INTERIORS_ROWING_DECK_ISLAND).
/// Called by WorldMap_Initialize at WORLD_MAP_FROM_TOLBI_SHIP_INTERIORS.
static void WorldMap_SailTolbiShipToIsland(void) {

  struct FieldActor *actor;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Engine_ActorSetAnimation(ACTOR_CROSSING_SHIP, ANIMATION_WALK);
  Actor_SetPosition(ACTOR_CROSSING_SHIP, PIXELS(4872), PIXELS(808));
  actor = Actor_Get(ACTOR_CROSSING_SHIP);
  {
    s32 value = FACING_NORTHWEST;

    actor->facing = value;
  }
  Engine_TaskWait(1);
  FieldEffect_ScheduleRatioTransition(6 * Q16_ONE / 5, 1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_CameraFollowActor(ACTOR_CROSSING_SHIP, 1);
  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_MoveToAndWait(ACTOR_CROSSING_SHIP, 4824, 712);
  Actor_MoveToAndWait(ACTOR_CROSSING_SHIP, 4776, 616);
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_MoveToAndWait(ACTOR_CROSSING_SHIP, 4776, 472);
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_MoveToAndWait(ACTOR_CROSSING_SHIP, 4760, 456);
  Actor_SetSpeed(ACTOR_CROSSING_SHIP, PIXELS(1) / 10, PIXELS(1) / 20);
  Actor_MoveToAndWait(ACTOR_CROSSING_SHIP, 4760, 440);
  Engine_ActorSetAnimation(ACTOR_CROSSING_SHIP, ANIMATION_STAND);
  Engine_EventWait(40);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(WORLD_MAP_TO_TOLBI_SHIP_INTERIORS_ROWING_DECK_ISLAND);
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
    if (Engine_GameFlagIsSet(flagBase + offset) != 0)
      return gWorldMapPassengerRowValues[offset];
  }
  return 0;
}

/// Spawns a puff of the wake behind ACTOR_DRIFTING_ISLAND every 16 frames.
static void WorldMap_SpawnIslandWake(void) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  struct AnimationObject *sprite;
  u32 value;

  if ((*(s32 *)&gFrameCount & 15) != 0)
    return;
  leader = Object_GetById(ACTOR_DRIFTING_ISLAND);
  actor = Engine_ObjectCreate(SPRITE_DUST_PUFF, leader->x.fixed - PIXELS(32),
                              leader->y.fixed, leader->z.fixed - PIXELS(16));
  if (actor == 0)
    return;
  actor->scaleX = Q16_ONE / 2;
  actor->scaleY = Q16_ONE / 2;
  sprite = actor->sprite;
  if (((u32)Engine_RandomNext() * 2 >> 16)) {
    s32 back = (((u32)Engine_RandomNext() * 48) >> 16) << 16;

    actor->x.fixed -= back >> 1;
    actor->z.fixed -= back;
  } else {
    value = (((u32)Engine_RandomNext() << 5) >> 16) << 16;
    actor->x.fixed += value;
    value = (s32)value >> 1;
    actor->z.fixed += value;
  }
  sprite->flags = 0;
  sprite->image.priority = leader->sprite->image.priority;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  actor->motionFlags = leader->motionFlags;
  ObjectGroup_SetChildValue(actor, SPRITE_COLOR_BLUE_TINT);
  Object_SetMode(actor, 2);
  Engine_ObjectSetScript(actor, gIslandWakeScript);
}

/// Before the ending, the island Jenna and Kraden are adrift on drifts east
/// with its wake under warm colours, and the map fades to white and leads into
/// their scene (WORLD_MAP_TO_JENNA_AND_KRADEN_ADRIFT).
/// Called by WorldMap_Initialize at WORLD_MAP_FROM_CLEAR, once the cleared game
/// is saved (menu/clear.c).
static void WorldMap_ShowIslandDrifting(void) {
  struct FieldActor *actor;
  struct FieldActor *spriteActor;

  actor = Object_GetById(ACTOR_DRIFTING_ISLAND);
  Engine_EventWait(60);
  Engine_EventBegin();
  FieldEffect_ScheduleRatioTransition(3 * Q16_ONE / 5, 1);
  actor->scaleX = 6 * Q16_ONE / 5;
  actor->scaleY = 6 * Q16_ONE / 5;
  Engine_CameraFollowActor(ACTOR_DRIFTING_ISLAND, 1);
  Engine_TaskWait(1);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  spriteActor = Actor_Get(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(spriteActor, 0);
  spriteActor = Actor_Get(ACTOR_DRIFTING_ISLAND);
  Engine_ActorSetSpriteFlags(spriteActor, 0);
  Actor_SetSpeed(ACTOR_DRIFTING_ISLAND, PIXELS(2) / 5, PIXELS(1) / 5);
  actor->timer = 0;
  Actor_SetActionScript(ACTOR_DRIFTING_ISLAND, gIslandDriftsEast);
  FieldScheduler_AddCallback(WorldMap_SpawnIslandWake, TASK_PRIORITY_SCENE);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_WARM, 1);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWait(120);
  FieldEffect_ScheduleRatioTransition(7 * Q16_ONE / 5, 300);
  Engine_EventWait(270);
  gEventWork->transitionFrames = 16;
  PLTT_BACKDROP = 0x7fff;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(WORLD_MAP_TO_JENNA_AND_KRADEN_ADRIFT);
}

/// Run each frame while a storm holds the party in the air: it spins the
/// leader and counts the frames left in the byte at FLAG_SCENE_TEMP_F8, paused
/// while a menu is open, raising TRIGGER_STORM_TIME_UP at the last.
static void WorldMap_UpdateStormTimer(void) {

  struct FieldActor *actor;
  struct EventWork *scene;
  s32 progress;

  actor = Object_GetById(gGameState.selectedActor);
  scene = gEventWork;
  actor->facing = (gFrameCount << 12);

  progress = FieldGameFlag_GetByte(FLAG_SCENE_TEMP_F8);
  if (progress != 0) {
    if (progress == 1) {
      scene->raisedTrigger = TRIGGER_STORM_TIME_UP;
    } else if (Engine_GameFlagIsSet(FLAG_MENU_OPEN) == 0) {
      progress -= 1;
    }
  }
  FieldGameFlag_SetByte(FLAG_SCENE_TEMP_F8, progress);
}

/// Draws the leader into the storm, lifts him and starts its timer.
/// Called by Event_WalkIntoStorm. Sets FLAG_SCENE_TEMP_F0 and gives the
/// party 180 frames in the air (FLAG_SCENE_TEMP_F8).
static void WorldMap_CatchInStorm(s32 targetActor) {
  s32 alreadyTraveling;
  struct FieldActor *actor;
  struct FieldActor *record;
  s32 id;

  id = gGameState.selectedActor;
  actor = Object_GetById(id);
  alreadyTraveling = GameFlag_IsSet(FLAG_SCENE_TEMP_F0);
  if (alreadyTraveling == 0) {
    Engine_EventBegin();
    Actor_SetAttachedEffect(id,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    Engine_ActorSetAnimation(id, ANIMATION_LEADER_LOOK_AROUND);
    record = Object_GetById(targetActor);
    if (record != 0) {
      Actor_SetDestination(id, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(id);
    Event_PlayCue(SE_SANDSTORM);
    FieldScheduler_AddCallback(WorldMap_UpdateStormTimer, TASK_PRIORITY_SCENE);
    actor->motionFlags = alreadyTraveling;
    Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed + PIXELS(32),
                             actor->z.fixed);
    Engine_ActorWaitForMove(id);
    actor->velocityY = alreadyTraveling;
    actor->motionFlags = ACTOR_FLOATS;
    gGameState.movementMode = LEADER_MOVEMENT_AIRBORNE;
    GameFlag_Set(FLAG_SCENE_TEMP_F0);
    FieldGameFlag_SetByte(FLAG_SCENE_TEMP_F8, 180);
    Engine_EventEnd();
    gEventWork->encounterEnemyGroup = 0; // drop any battle this step rolled
  }
}

/// Walking into the storm: the party is lifted up into the air.
/// Reached by walking onto TRIGGER_STORM until the storm is done with
/// (FLAG_SCENE_TEMP_F1).
static void Event_WalkIntoStorm(void) { WorldMap_CatchInStorm(ACTOR_STORM); }

/// Starting to cast Douse on the storm: Douse's rain cloud gathers 80 pixels
/// up, where the party floats (douse.c reads childOption).
/// Reached by the first phase of Douse on the storm (ACTOR_STORM) while it
/// holds the party in the air (FLAG_SCENE_TEMP_F0).
static void Event_StartDousingStorm(void) {
  ((struct FieldFxScene *)gEffectWork)->childOption = 1;
}

/// Douse on the storm: the party comes down and the battle with what is in it
/// begins (ENCOUNTER_ZONE_SCRIPTED, entries 5 and 7), as with the Suhalla
/// Desert's storms. Reached by Douse on the storm (ACTOR_STORM) while it holds
/// the party in the air. Clears FLAG_SCENE_TEMP_F0, sets FLAG_SCENE_TEMP_F1 and
/// empties the timer.
static void Event_DouseMapStorm(void) {
  struct FieldActor *selectedActor;

  if (gEventWork->raisedTrigger == TRIGGER_STORM_TIME_UP) {
    gEventWork->raisedTrigger = 0;
  }
  Engine_GameFlagClear(FLAG_SCENE_TEMP_F0);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_F1);
  FieldGameFlag_SetByte(FLAG_SCENE_TEMP_F8, 0);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 5);
  Battle_SetStart(BATTLE_START_SCENE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 7);
  selectedActor = Object_GetById(gGameState.selectedActor);
  selectedActor->motionFlags = ACTOR_FALLS;
}

/// The storm drops the party outside Suhalla when its time in the air runs out.
/// The party and the storm fly off, and the map reopens outside Suhalla, where
/// the desert's storms also drop the party. Reached by TRIGGER_STORM_TIME_UP,
/// raised as the timer runs out. Sets FLAG_ARRIVE_FALLING and sends the party
/// to WORLD_MAP_FROM_SUHALLA.
static void Event_StormDropsParty(void) {
  struct FieldActor *actor = Actor_Get(gGameState.selectedActor);
  struct FieldActor *other = Actor_Get(ACTOR_STORM);
  s32 frames;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Event_PlayCue(SE_219);
  Engine_ActorSetSpriteFlags(actor, 0);
  other->motionFlags = 0;
  actor->motionFlags = 0;
  actor->velocityY = 0;
  actor->frozen = 1;
  other->frozen = 1;
  for (frames = 59; frames >= 0; --frames) {
    actor->velocityY += PIXELS(1) / 5;
    other->velocityY += PIXELS(1) / 5;
    Engine_TaskWait(1);
  }
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
  GameFlag_Set(FLAG_ARRIVE_FALLING);
  Event_SetDestination((s32)&SceneId_WorldMap, WORLD_MAP_FROM_SUHALLA);
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

  struct FieldActor *record;
  s32 sx;
  s32 sy;
  u32 mode;

  record = Actor_Get(gGameState.selectedActor);
  sx = record->x.part.pixel;
  sy = record->z.part.pixel;
  if (gFrameCount % 3 == 0) {
    mode = (u32)(Random_Next() << 2) >> 16;
    switch (mode) {
    case 0:
      Camera_MoveTo((sx << 16) - PIXELS(1), -1, (sy << 16) + PIXELS(1), 1);
      break;
    case 1:
      Engine_CameraMoveTo((sx << 16) + PIXELS(1), -1, (sy << 16) - PIXELS(1),
                          1);
      break;
    case 2:
      Camera_MoveTo((sx << 16) + PIXELS(1), -1, (sy << 16) + PIXELS(1), 1);
      break;
    case 3:
      Camera_MoveTo((sx << 16) - PIXELS(1), -1, (sy << 16) - PIXELS(1), 1);
      break;
    }
  }
}

/// Queues one frame of the lighthouse transition and waits frames.
static inline void WorldMap_ShowTransitionFrame(u8 *buffer, s32 offset,
                                                s32 frames) {
  WORLD_MAP_QUEUE_TRANSITION_FRAME(buffer, offset);
  Engine_EventWait(frames);
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
/// sets FLAG_SKIP_SCENE_CHANGE_SOUND.
static void WorldMap_PlayLighthouseTransition(s32 palette) {
  u8 *buffer = Runtime_BumpAllocateAlternatePool(0x4000);

  Engine_TaskWait(1);
  Engine_GameFlagClear(FLAG_KEEP_PARTY_POSITION);
  WorldMap_BeginStillView();
  FieldResource_DecodeType01(gWorldMapPackedTiles, buffer);
  FieldResource_DecodeType01(gWorldMapPackedFrames, buffer + 0x1000);
  IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT[14 * 16],
                      (u32)(gWorldMapPalettes + palette * 32),
                      DMA_ENABLE << 16 | 16);
  IO_WRITE_QUEUE_PUSH((u32)BG_CHAR_BLOCK(0) + 0x1000, (u32)buffer,
                      (DMA_ENABLE | DMA_32BIT) << 16 | 0x400);
  FieldScheduler_AddCallback(WorldMap_UpdateTransitionBlend,
                             TASK_PRIORITY_SCENE);
  Engine_EventBegin();
  WORLD_MAP_QUEUE_TRANSITION_FRAME(buffer, 0x3a80);
  Object_GetById(gGameState.selectedActor)->spriteStorageKind = 0;
  gEventWork->transitionFrames = 16;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_AudioPlayCue(SE_246);

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
  Map_LoadAreaGraphics();
  FieldScheduler_AddCallback(WorldMap_JitterTransitionCamera,
                             TASK_PRIORITY_SCENE);
  Engine_AudioPlayCue(SE_141);
  gWorldMapBlend = BLEND_ALPHA(0, 13);
  Engine_EventWait(4);
  gWorldMapBlend = BLEND_ALPHA(0, 14);
  Engine_EventWait(4);
  gWorldMapBlend = BLEND_ALPHA(0, 15);
  Engine_EventWait(4);
  gWorldMapBlend = BLEND_ALPHA(0, 16);
  Engine_EventWait(45);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  MemFree(buffer);
  Engine_GameFlagSet(FLAG_SKIP_SCENE_CHANGE_SOUND);
}

/// Unused: an update hook that blinks an actor and now and then creates its
/// linked effects; no actor is given it.
UNUSED static void
WorldMap_UpdateBlinkingLinkedEffects(struct FieldActor *actor) {
  if ((gFrameCount & 2) != 0) {
    Engine_ObjectSetPartPalettes(actor, SPRITE_COLOR_WHITE);
  } else {
    Engine_ObjectSetPartPalettes(actor, SPRITE_COLOR_NORMAL);
  }
  if ((gFrameCount & 15) == 0) {
    WorldMap_CreateLinkedEffects(actor);
  }
}

/// Unused: an update hook that cycles an actor's palettes and now and then
/// creates its linked effects; no actor is given it.
UNUSED static void
WorldMap_UpdateCyclingLinkedEffects(struct FieldActor *actor) {
  if ((gFrameCount & 1) != 0) {
    Engine_ObjectSetPartPalettes(actor, ((u32)((gFrameCount >> 1)) % (u32)6));
  }
  if ((gFrameCount & 15) == 0) {
    WorldMap_CreateLinkedEffects(actor);
  }
}

/// Unused: an update hook that advances an actor's palette on odd frames; no
/// actor is given it.
UNUSED static void WorldMap_CycleActorPalettes(struct FieldActor *actor) {
  if ((gFrameCount & 1) != 0) {
    s32 slot = (gFrameCount >> 1) % 6;

    Engine_ObjectSetPartPalettes(actor, slot);
  }
}

#include "common/effect_vertical.h"
#include "common/world_map_linked_effects.h"

s32 gCaravanFirstWagonArrives[] = {
    SC_POS(PIXELS(5848), 0, PIXELS(1576)),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ARG(ANIMATION_WALK),
    SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1614)),
    SC_SYNC,
    SC_MOVE(PIXELS(5864), 0, PIXELS(1624)),
    SC_SYNC,
    SC_ARG(ANIMATION_STAND),
    SC_END,
};

s32 gCaravanFirstWagonDeparts[] = {
    SC_ARG(ANIMATION_WALK),
    SC_MOVE(PIXELS(5868), 0, PIXELS(1624)),
    SC_SYNC,
    SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
    SC_SYNC,
    SC_ARG(ANIMATION_STAND),
    SC_POS(PIXELS(5656), 0, PIXELS(1800)),
    SC_END,
};

s32 gCaravanFirstWagonReturns[] = {
    SC_ARG(ANIMATION_WALK),
    SC_MOVE(PIXELS(5800), 0, PIXELS(1800)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gCaravanWagonHeadsNorth[] = {
    SC_POS(PIXELS(5868), 0, PIXELS(1704)),
    SC_WAIT(1),
    SC_ARG(ANIMATION_WALK),
    SC_MOVE(PIXELS(5872), 0, PIXELS(1528)),
    SC_SYNC,
    SC_MOVE(PIXELS(5880), 0, PIXELS(1352)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gCaravanSecondWagonArrives[] = {
    SC_POS(PIXELS(5848), 0, PIXELS(1576)),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ARG(ANIMATION_WALK),
    SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1630)),
    SC_SYNC,
    SC_MOVE(PIXELS(5832), 0, PIXELS(1640)),
    SC_SYNC,
    SC_ARG(ANIMATION_STAND),
    SC_END,
};

s32 gCaravanSecondWagonDeparts[] = {
    SC_ARG(ANIMATION_WALK),
    SC_MOVE(PIXELS(5863), 0, PIXELS(1640)),
    SC_SYNC,
    SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
    SC_SYNC,
    SC_ARG(ANIMATION_STAND),
    SC_POS(PIXELS(5640), 0, PIXELS(1768)),
    SC_END,
};

s32 gCaravanSecondWagonReturns[] = {
    SC_ARG(ANIMATION_WALK),
    SC_MOVE(PIXELS(5640), 0, PIXELS(1795)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1800)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gCaravanThirdWagonArrives[] = {
    SC_POS(PIXELS(5848), 0, PIXELS(1576)),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ARG(ANIMATION_WALK),
    SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1614)),
    SC_SYNC,
    SC_MOVE(PIXELS(5832), 0, PIXELS(1624)),
    SC_SYNC,
    SC_ARG(ANIMATION_STAND),
    SC_END,
};

s32 gCaravanThirdWagonDeparts[] = {
    SC_ARG(ANIMATION_WALK),
    SC_MOVE(PIXELS(5863), 0, PIXELS(1624)),
    SC_SYNC,
    SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
    SC_SYNC,
    SC_ARG(ANIMATION_STAND),
    SC_POS(PIXELS(5656), 0, PIXELS(1768)),
    SC_END,
};

s32 gCaravanThirdWagonReturns[] = {
    SC_ARG(ANIMATION_WALK),
    SC_MOVE(PIXELS(5656), 0, PIXELS(1795)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1800)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gCaravanFourthWagonArrives[] = {
    SC_POS(PIXELS(5848), 0, PIXELS(1576)),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ARG(ANIMATION_WALK),
    SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1598)),
    SC_SYNC,
    SC_MOVE(PIXELS(5832), 0, PIXELS(1608)),
    SC_SYNC,
    SC_ARG(ANIMATION_STAND),
    SC_END,
};

s32 gCaravanFourthWagonDeparts[] = {
    SC_ARG(ANIMATION_WALK),
    SC_MOVE(PIXELS(5863), 0, PIXELS(1608)),
    SC_SYNC,
    SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
    SC_SYNC,
    SC_ARG(ANIMATION_STAND),
    SC_POS(PIXELS(5672), 0, PIXELS(1768)),
    SC_END,
};

s32 gCaravanFourthWagonReturns[] = {
    SC_ARG(3), SC_MOVE(PIXELS(5672), 0, PIXELS(1795)),
    SC_SYNC,   SC_MOVE(PIXELS(5800), 0, PIXELS(1800)),
    SC_SYNC,   SC_POS(0, 0, 0),
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
    SC_ARG(ANIMATION_WALK),
    SC_MOVE(PIXELS(6008), 0, PIXELS(3400)),
    SC_SYNC,
    SC_MOVE(PIXELS(6020), 0, PIXELS(3400)),
    SC_SYNC,
    SC_ARG(ANIMATION_STAND),
    SC_SET(SW_TIMER, 1),
    SC_WAIT(1),
    SC_END,
};

s32 gEndingAlexWalk[] = {
    SC_ARG(ANIMATION_WALK),
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
    SC_ARG(ANIMATION_STAND),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(1),
    SC_END,
};

s32 gEndingKradenWalk[] = {
    SC_ARG(ANIMATION_WALK),
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
    SC_ARG(ANIMATION_STAND),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

s32 gEndingJennaWalk[] = {
    SC_SET(SW_TIMER, 0),
    SC_ARG(ANIMATION_WALK),
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
    SC_ARG(ANIMATION_STAND),
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
    SC_ARG(ANIMATION_WALK),
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
    SC_ARG(ANIMATION_WALK),
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

struct SceneEntrance gWorldMapEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 5656, 0, 1256, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
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
    {WORLD_MAP_FROM_CLEAR, CONDITION_ALWAYS, 7464, 0, 200, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
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
    {SCENE_TABLE_END},
};

u32 gWorldMapExits[] = {
    SCENE_EXITS(WorldMap),
    SCENE_EXIT(WORLD_MAP_TO_VALE_GATE, ValeGate, VALE_GATE_FROM_WORLD_MAP),
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
    SCENE_EXIT(WORLD_MAP_TO_WORLD_MAP_FROM_VALE_GATE, WorldMap,
               WORLD_MAP_FROM_VALE_GATE),
    SCENE_EXIT(WORLD_MAP_TO_HAMMETS_CARAVAN_LEAVES, WorldMap,
               WORLD_MAP_HAMMETS_CARAVAN_LEAVES),
    SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_MAST_FIRST_ATTACK, TolbiShipMast,
               TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK),
    SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_MAST_SECOND_ATTACK, TolbiShipMast,
               TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK),
    SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_MAST_THIRD_ATTACK, TolbiShipMast,
               TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK),
    SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_INTERIORS, TolbiShipInteriors,
               TOLBI_SHIP_INTERIORS_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_MAST_LAND_HO, TolbiShipMast,
               TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO),
    SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
               TOLBI_SHIP_DECK_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE_SOUTH, KalayTunnelEntrance,
               KALAY_TUNNEL_ENTRANCE_FROM_WORLD_MAP_SOUTH),
    SCENE_EXIT(WORLD_MAP_TO_VENUS_LIGHTHOUSE_AERIE, VenusLighthouseAerie,
               VENUS_LIGHTHOUSE_AERIE_FROM_WORLD_MAP),
    SCENE_EXIT(WORLD_MAP_TO_TOLBI_SHIP_INTERIORS_ROWING_DECK_ISLAND,
               TolbiShipInteriors,
               TOLBI_SHIP_INTERIORS_ROWING_DECK_FROM_WORLD_MAP_ISLAND),
    SCENE_EXIT(WORLD_MAP_TO_JENNA_AND_KRADEN_ADRIFT, WorldMap,
               WORLD_MAP_JENNA_AND_KRADEN_ADRIFT),
    SCENE_EXIT(WORLD_MAP_TO_LUNPA_SUHALLA, LunpaSuhalla,
               LUNPA_SUHALLA_FROM_SUHALLA_GATE_CHECKPOINT),
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
struct ScenePlacement gWorldMapPlacements[] = {
    { VENUS_DJINNI, FLAG_DJINNI_0 + DJINNI_VINE, (s32)gWorldMapFlintScript,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MARS_DJINNI, FLAG_DJINNI_0 + DJINNI_CORONA, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MERCURY_DJINNI, FLAG_DJINNI_0 + DJINNI_HAIL, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5678), 0, PIXELS(1200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5848), 0, PIXELS(1576), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6976), 0, PIXELS(1624), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5416), 0, PIXELS(2840), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5892), 0, PIXELS(1128), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5422), 0, PIXELS(2232), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5036), 0, PIXELS(2328), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_SNOWY_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5928), 0, PIXELS(270), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_STUMP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7352), 0, PIXELS(1240), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_BLUE_ROOFED_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7544), 0, PIXELS(1976), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_TEMPLE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6888), 0, PIXELS(1712), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_TEMPLE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7880), 0, PIXELS(1560), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6584), 0, PIXELS(1192), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5592), 0, PIXELS(2136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5996), 0, PIXELS(2894), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(4904), 0, PIXELS(2280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_MERCURY_LIGHTHOUSE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6024), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_MOUNTAIN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5656), 0, PIXELS(1112), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6088), 0, PIXELS(3176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_BARRICADE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6952), 0, PIXELS(1362), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_BABI_LIGHTHOUSE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5996), 0, PIXELS(2808), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6424), 0, PIXELS(1302), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6484), 0, PIXELS(1184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(4840), 0, PIXELS(1960), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5656), 0, PIXELS(2392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5480), 0, PIXELS(2600), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_FOREST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7790), 0, PIXELS(2080), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5976), 0, PIXELS(3400), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5988), 0, PIXELS(774), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6224), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5856), 0, PIXELS(1174), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6920), 0, PIXELS(1664), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6092), 0, PIXELS(2200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_FOREST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7166), 0, PIXELS(1054), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_FOREST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7856), 0, PIXELS(1736), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5480), 0, PIXELS(2088), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SIGNPOST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SIGNPOST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SIGNPOST, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_WORLD_MAP_BRIDGE, CONDITION_ALWAYS, (s32)gWorldMapKolimaBridgeScript,
        PIXELS(7568), 0, PIXELS(1384), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_GATEHOUSE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(4942), 0, PIXELS(2690), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SHIPWRECK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6036), 0, PIXELS(3458), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SPRITE_SHIP, IF_FLAG_SET(FLAG_TOLBI_SHIP_AT_ISLAND), (s32)gWorldMapAlignToMapScript,
        PIXELS(4760), 0, PIXELS(440), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SANDSTORM, FLAG_SCENE_TEMP_F1, (s32)gWorldMapStormScript,
        PIXELS(4392), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_IODEM, CONDITION_ALWAYS, (s32)gWorldMapIodemScript,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
    { GARET, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacementsHammetsCaravan[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_HAMMET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5848), 0, PIXELS(1576), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacementsFromWestLunpaCave[] = {
    { SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_VILLAGE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5892), 0, PIXELS(1128), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5856), 0, PIXELS(1174), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BUNZA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_HAMMET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacementsCrossingEnds[] = {
    { SPRITE_SHIP, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5384), 0, PIXELS(2248), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5592), 0, PIXELS(2136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5422), 0, PIXELS(2232), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_DOCK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5028), 0, PIXELS(2328), FACING_WEST, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacementsCrossing[] = {
    { SPRITE_SHIP, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5432), 0, PIXELS(2312), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacementsFromKalayTunnelEntranceSouth[] = {
    { SPRITE_WORLD_MAP_TOWN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5592), 0, PIXELS(2136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WORLD_MAP_CAVE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(5480), 0, PIXELS(2088), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_WAGON, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacementsJennaAndKradenAdrift[] = {
    { JENNA, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7560), 0, PIXELS(3512), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SHEBA, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7816), 0, PIXELS(3528), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7544), 0, PIXELS(3496), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_ALEX, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7708), 0, PIXELS(3456), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_FELIX, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7800), 0, PIXELS(3544), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_SHIPWRECK, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7604), 0, PIXELS(3522), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacementsFromClear[] = {
    { SPRITE_WORLD_MAP_ISLAND, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(7464), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacementsBeforeShebasFall[] = {
    { SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, CONDITION_ALWAYS, (s32)gWorldMapAlignToMapScript,
        PIXELS(6080), 0, PIXELS(3168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gWorldMapEvents[] = {
    { EVENT_TOUCH, TRIGGER_FLINT, FLAG_DJINNI_0 + DJINNI_FLINT, EVENT_SCRIPT(Event_MeetFlint) },
    { EVENT_TOUCH, TRIGGER_STORM, FLAG_SCENE_TEMP_F1, EVENT_SCRIPT(Event_WalkIntoStorm) },
    { EVENT_TALK, ACTOR_VENUS_DJINNI, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MeetVine) },
    { EVENT_TALK, ACTOR_MARS_DJINNI, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MeetCorona) },
    { EVENT_TALK, ACTOR_MERCURY_DJINNI, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MeetHail) },
    { EVENT_TOUCH, TRIGGER_VINE, FLAG_DJINNI_0 + DJINNI_VINE, EVENT_SCRIPT(Event_MeetVine) },
    { EVENT_TOUCH, TRIGGER_CORONA, FLAG_DJINNI_0 + DJINNI_CORONA, EVENT_SCRIPT(Event_MeetCorona) },
    { EVENT_TOUCH, TRIGGER_HAIL, FLAG_DJINNI_0 + DJINNI_HAIL, EVENT_SCRIPT(Event_MeetHail) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_SHIPWRECK, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchShipwreck) },
    { USE_ITEM(ITEM_BLACK_ORB), TRIGGER_SHIPWRECK, FLAG_BLACK_ORB_RETURNED, EVENT_SCRIPT(Event_UseBlackOrb) },
    { SEARCH(SEARCH_GROUND), TRIGGER_TURTLE_BOOTS, FLAG_WORLD_MAP_TURTLE_BOOTS, FIND_ITEM(ITEM_TURTLE_BOOTS) },
    { EVENT_TALK, ACTOR_IODEM, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutBlackOrb) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_IODEM, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutBlackOrb) },
    { EVENT_EXIT, WORLD_MAP_TO_MOGALL_FOREST_SIGN, CONDITION_ALWAYS, WORLD_MAP_TO_MOGALL_FOREST_SIGN },
    { EVENT_EXIT, WORLD_MAP_TO_ALPINE_CROSSING_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_ALPINE_CROSSING_EAST },
    { EVENT_EXIT, WORLD_MAP_TO_ALPINE_CROSSING_NORTH, CONDITION_ALWAYS, WORLD_MAP_TO_ALPINE_CROSSING_NORTH },
    { EVENT_EXIT, WORLD_MAP_TO_ALPINE_CROSSING_WEST, CONDITION_ALWAYS, WORLD_MAP_TO_ALPINE_CROSSING_WEST },
    { EVENT_EXIT, WORLD_MAP_TO_LAMAKAN_DESERT_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_LAMAKAN_DESERT_EAST },
    { EVENT_EXIT, WORLD_MAP_TO_SUHALLA_DESERT_NORTH, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_DESERT_NORTH },
    { EVENT_EXIT, WORLD_MAP_TO_SUHALLA_DESERT_SOUTH, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_DESERT_SOUTH },
    { EVENT_EXIT, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_SOUTH, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_SOUTH },
    { EVENT_EXIT, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA_GATE_CHECKPOINT_EAST },
    { EVENT_EXIT, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_WEST, CONDITION_ALWAYS, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_WEST },
    { EVENT_EXIT, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_VENUS_LIGHTHOUSE_ENTRY_EAST },
    { EVENT_EXIT, WORLD_MAP_TO_BILIBIN_BARRICADE_EAST, CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN_BARRICADE_EAST },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_VALE_GATE), CONDITION_ALWAYS, WORLD_MAP_TO_VALE_GATE },
    { EVENT_TOUCH, PLACE_TRIGGER(ACTOR_PLACE_VAULT), CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterVault) },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_GOMA_CAVE_ENTRANCE), CONDITION_ALWAYS, WORLD_MAP_TO_GOMA_CAVE_ENTRANCE },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_GOMA_CAVE_BRIDGE), CONDITION_ALWAYS, WORLD_MAP_TO_GOMA_CAVE_BRIDGE },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_BILIBIN), CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN },
    { EVENT_TOUCH, PLACE_TRIGGER(ACTOR_PLACE_BILIBIN_BARRICADE), CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterBilibinBarricade) },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_KOLIMA), CONDITION_ALWAYS, WORLD_MAP_TO_KOLIMA_WEST },
    { EVENT_TOUCH, PLACE_TRIGGER(ACTOR_PLACE_KOLIMA_BRIDGE), CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterKolimaBridge) },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_KOLIMA_FOREST), CONDITION_ALWAYS, WORLD_MAP_TO_KOLIMA_FOREST_ENTRANCE },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_BILIBIN_CAVE_ENTRANCE), CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN_CAVE_ENTRANCE },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_BILIBIN_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_BILIBIN_CAVE_ICE },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_IMIL), CONDITION_ALWAYS, WORLD_MAP_TO_IMIL_SOUTH },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_MERCURY_LIGHTHOUSE), CONDITION_ALWAYS, WORLD_MAP_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_FUCHIN_TEMPLE), CONDITION_ALWAYS, WORLD_MAP_TO_FUCHIN_TEMPLE_GROUNDS },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_MOGALL_FOREST_SIGN), CONDITION_ALWAYS, WORLD_MAP_TO_MOGALL_FOREST_SIGN },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_MOGALL_FOREST_KILLER_APE), CONDITION_ALWAYS, WORLD_MAP_TO_MOGALL_FOREST_KILLER_APE },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_XIAN), CONDITION_ALWAYS, WORLD_MAP_TO_XIAN },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_ALTIN), CONDITION_ALWAYS, WORLD_MAP_TO_ALTIN },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_ALTIN_PEAK), CONDITION_ALWAYS, WORLD_MAP_TO_ALTIN_PEAK_WEST_EXIT },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_LAMA_TEMPLE), CONDITION_ALWAYS, WORLD_MAP_TO_LAMA_TEMPLE },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_LAMAKAN_DESERT_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_LAMAKAN_DESERT_CAVE },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_KALAY), CONDITION_ALWAYS, WORLD_MAP_TO_KALAY_EAST },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_LUNPA), CONDITION_ALWAYS, WORLD_MAP_TO_LUNPA },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_WEST_LUNPA_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_WEST_LUNPA_CAVE },
    { EVENT_TOUCH, PLACE_TRIGGER(ACTOR_PLACE_KALAY_DOCKS), CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterKalayDocks) },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_TOLBI_DOCKS), CONDITION_ALWAYS, WORLD_MAP_TO_TOLBI_DOCKS },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_TOLBI), CONDITION_ALWAYS, WORLD_MAP_TO_TOLBI },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_ALTMILLER_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_ALTMILLER_CAVE_ENTRANCE },
    { EVENT_TOUCH, PLACE_TRIGGER(ACTOR_PLACE_GONDOWAN_PASSAGE), CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterGondowanPassage) },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_KALAY_TUNNEL_ENTRANCE), CONDITION_ALWAYS, WORLD_MAP_TO_KALAY_TUNNEL_ENTRANCE },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_GONDOWAN_CAVE_NORTH), CONDITION_ALWAYS, WORLD_MAP_TO_GONDOWAN_CAVE_NORTH },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_GONDOWAN_CAVE_SOUTH), CONDITION_ALWAYS, WORLD_MAP_TO_GONDOWAN_CAVE_SOUTH },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_SUHALLA), CONDITION_ALWAYS, WORLD_MAP_TO_SUHALLA },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_GATEWAY_CAVE), CONDITION_ALWAYS, WORLD_MAP_TO_GATEWAY_CAVE },
    { EVENT_EXIT, PLACE_TRIGGER(ACTOR_PLACE_ROAD_TO_BABI_LIGHTHOUSE), CONDITION_ALWAYS, WORLD_MAP_TO_ROAD_TO_BABI_LIGHTHOUSE },
    { EVENT_TOUCH, PLACE_TRIGGER(ACTOR_PLACE_LALIVERO), CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterLalivero) },
    { TOUCH_WALKING_INTO(FACING_EAST), TRIGGER_TOLBI_SHIP, IF_FLAG_SET(FLAG_TOLBI_SHIP_AT_ISLAND), EVENT_SCRIPT(Event_ReturnToTolbiShip) },
    { EVENT_EXIT | EVENT_TOUCH_FACING | FACING_NORTH, TRIGGER_CROSSBONE_ISLE, CONDITION_ALWAYS, WORLD_MAP_TO_CROSSBONE_ISLE },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_STORM, IF_FLAG_SET(FLAG_SCENE_TEMP_F0), EVENT_SCRIPT(Event_StartDousingStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_STORM, IF_FLAG_SET(FLAG_SCENE_TEMP_F0), EVENT_SCRIPT(Event_DouseMapStorm) },
    { EVENT_RAISED, TRIGGER_STORM_TIME_UP, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StormDropsParty) },
    { SCENE_EVENTS_END },
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
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

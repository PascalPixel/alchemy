/// @file
/// Suhalla (MapId_Suhalla, area name Suhalla), the desert village of
/// Gondowan at the edge of the Suhalla Desert, on the way from Gondowan
/// Passage to Lalivero.
///
/// Its people come in two sets, each an object event template table and a map
/// event table, chosen by FLAG_BABI_SPOKE_AFTER_COLOSSO: before Babi's audience
/// after Colosso the village talks of the sandstorms that close the desert, of
/// its distrust of Angara and of Sheba, the hostage; after it, of the soldiers
/// the storms caught on the way to Babi Lighthouse, who lie in one of its
/// houses (suhalla_interiors.c). The same people stand in the same rows of both
/// object event template tables, so one enum names them; the woman the
/// sandstorms brought stands off the map until the audience.
///
/// Five doors, whose metatile behaviors are their warps' numbers
/// (SUHALLA_TO_INN to SUHALLA_TO_HOUSE_EAST), open as the leader walks north
/// into them (EventScript_EnterDoor, EventScript_EnterWideDoor) and lead into
/// the village's buildings. Three ways lead out onto the world map, and the
/// desert's sandstorms drop a caught party at WORLD_MAP_FROM_SUHALLA. A chest
/// and two jars hold a Hard Nut, a Lucky Medal and coins, and Growth grows a
/// sprout and changes its cell. Its map flag:
///
/// - FLAG_MAP_TEMP_01: Growth has grown the sprout; a return that keeps the
///   party's position, as after a battle, draws it grown again.
///
/// Open questions:
/// - None.

#include "game_state.h"
#include "unit/map/suhalla.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "script_layer.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "map_tables.h"
#include "item_ids.h"
#include "songs.h"
#include "flags.h"

/// The sprout's animation once Growth has grown it.
enum { SPROUT_ANIMATION_GROWN = 4 };

/// Suhalla's people and its sprout, numbering the rows of both object event
/// template tables from LOCALID_FIRST_PLACED: gSuhallaObjectEventTemplatesLater
/// places the same people, by sprite, in the same rows. Each note gives what
/// they say before Babi's audience, then after it.
enum SuhallaLocalId {
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), who warns the
  /// party
  /// off the desert; later "Welcome to Suhalla. I bet the sandstorms were
  /// terrible..."
  /// ★ Named from the game: "This is Suhalla."
  /// (MsgSuhallaThisIsSuhallaDontTryCrossing)
  LOCALID_MAN_THIS_IS_SUHALLA = LOCALID_FIRST_PLACED,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), who asks whether
  /// the party is trying to get to Lalivero (EventScript_AskIfGoingToLalivero);
  /// later "The desert is enveloped in sandstorms! It's frightening!"
  /// ★ Named from the game: "Are you trying to get to Lalivero?"
  /// (MsgSuhallaTryingGetLalivero)
  LOCALID_WOMAN_ASKING_ABOUT_LALIVERO,
  /// A bearded man in a white turban (SPRITE_MAN_IN_WHITE_TURBAN), who
  /// distrusts Angara for taking Sheba as a hostage; later "Soldiers from
  /// Tolbi were caught in the sandstorms, and nearly all perished!"
  /// ★ Named from the game: "We Gondowans will never trust anyone from
  /// Angara!" (MsgSuhallaWeGondowansWillNeverTrustAnyone)
  LOCALID_MAN_NEVER_TRUSTING_ANGARA,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS), placed off the
  /// map until Babi's audience, then one of those the sandstorms have brought
  /// here.
  /// ★ Named from the game: "We were brought here by the sandstorms."
  /// (MsgSuhallaWeWereBroughtHereByThe)
  LOCALID_WOMAN_BROUGHT_BY_SANDSTORMS,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who speaks of the
  /// desert spirits; later "This sandstorm is the fault of the Angarans!"
  /// ★ Named from the game: "desert spirits only create sandstorms"
  /// (MsgSuhallaItHasBeenSaidForAges)
  LOCALID_OLD_MAN_ON_DESERT_SPIRITS,
  /// An old woman with white hair (SPRITE_OLD_WOMAN), who doubts there is a
  /// way to southern Gondowan, then asks whether a sandstorm brought the party
  /// (EventScript_AskIfBroughtBySandstorm).
  /// ★ Named from the game: "You can't get to southern Gondowan from here"
  /// (MsgSuhallaYouCantGetToSouthernGondowan)
  LOCALID_OLD_WOMAN_ON_SOUTHERN_GONDOWAN,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR), who would like to
  /// see Tolbi's festivals; later "Why are the sandstorms attacking humans?"
  /// ★ Named from the game: "The workers say there're huge festivals in
  /// Tolbi." (MsgSuhallaTheWorkersSayTherereHugeFestivals)
  LOCALID_BOY_ON_TOLBI_FESTIVALS,
  /// A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS), who
  /// worries about the storms; later "Those soldiers kept mumbling about
  /// monsters..."
  /// ★ Named from the game: "The sandstorms suddenly increased..."
  /// (MsgSuhallaTheSandstormsSuddenlyIncreasedIHope)
  LOCALID_GIRL_SANDSTORMS_INCREASED,
  /// At 72, 144; Growth grows it (EventScript_GrowSprout).
  /// ★ Named from what the player sees: a small green sprout in the sand by the
  /// inn (its render)
  LOCALID_SPROUT
};

const struct MapWarpDest *Map_GetWarpDests(void) { return gSuhallaWarpDests; }

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) { return gSuhallaWarps; }

/// Suhalla's people, who stand elsewhere after Babi's audience.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  if (GameFlag_Test(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
    return gSuhallaObjectEventTemplatesLater;
  }
  return gSuhallaObjectEventTemplates;
}

/// A woman asks whether the party is trying to get to Lalivero. She's
/// LOCALID_WOMAN_ASKING_ABOUT_LALIVERO, in Suhalla before Babi's audience. "Are
/// you trying to get to Lalivero?" Yes, and the sandstorms make it impossible;
/// no, and the woman doubts it, for Angarans who come to Suhalla are always
/// going to Lalivero.
static void EventScript_AskIfGoingToLalivero(s32 localId) {
  s32 firstMessage = MsgSuhallaTryingGetLalivero;
  FieldScript_SetMessageId(firstMessage);
  FieldMessage_ShowAtObject(localId, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_WaitFrames(10);
    FieldScript_SetMessageId(firstMessage + 1);
  } else {
    FieldScript_SetMessageId(firstMessage + 2);
  }
  FieldScript_ShowMessageAndWait(localId, 0);
}

/// An old woman asks whether the sandstorm brought the party to Suhalla. She's
/// LOCALID_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, there after Babi's audience. Yes,
/// and everyone the storms catch winds up here; no, and the old woman wonders
/// why there are so many storms.
static void EventScript_AskIfBroughtBySandstorm(s32 localId) {
  s32 firstMessage = MsgSuhallaBroughtSuhallaSandstorm;
  FieldScript_SetMessageId(firstMessage);
  FieldMessage_ShowAtObject(localId, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_WaitFrames(10);
    FieldScript_SetMessageId(firstMessage + 1);
  } else {
    FieldScript_SetMessageId(firstMessage + 2);
  }
  FieldScript_ShowMessageAndWait(localId, 0);
}

/// Growth on Suhalla's sprout: the plant grows, and its cell takes the
/// metatile behavior of the grown plant's cell. Sets FLAG_MAP_TEMP_01.
static void EventScript_GrowSprout(void) {
  MapGrid_CopyCellAttributes(25, 9, 1, 1, 4, 9);
  Engine_GameFlagSet(FLAG_MAP_TEMP_01);
}

/// Walking north into one of Suhalla's doors (SUHALLA_TO_INN,
/// SUHALLA_TO_HOUSE_WEST, SUHALLA_TO_HOUSE_NORTHEAST and SUHALLA_TO_HOUSE_EAST,
/// whose metatile behaviors are their warps' numbers): the door opens, the
/// leader steps through and the party leaves by that warp into the village's
/// buildings.
static void EventScript_EnterDoor(void) {
  struct FieldWork *work = gFieldWork;
  s32 warpTrigger = work->coordEvent;
  u16 cellX = gSuhallaDoorCells[warpTrigger][0];
  u16 cellZ = gSuhallaDoorCells[warpTrigger][1];

  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gSuhallaDoorOpenSteps, cellX, cellZ);
  // Walk through the opening while the warp transition counts down.
  ObjectEvent_WalkBy(LOCALID_PARTY_LEADER, 0, -16);
  gFieldWork->transitionFrames = 16;
  Engine_ScriptRequestWarp(warpTrigger);
}

/// The same through the wide door of SUHALLA_TO_SANCTUM, into Suhalla Sanctum.
static void EventScript_EnterWideDoor(void) {
  struct FieldWork *work = gFieldWork;
  s32 warpTrigger = work->coordEvent;
  u16 cellX = gSuhallaDoorCells[warpTrigger][0];
  u16 cellZ = gSuhallaDoorCells[warpTrigger][1];

  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gSuhallaWideDoorOpenSteps, cellX, cellZ);
  ObjectEvent_WalkBy(LOCALID_PARTY_LEADER, 0, -16);
  gFieldWork->transitionFrames = 16;
  Engine_ScriptRequestWarp(warpTrigger);
}

/// Suhalla's doors, people, treasure and sprout, before or after Babi's
/// audience.
const struct MapEvent *Map_GetEvents(void) {
  if (GameFlag_Test(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
    return gSuhallaEventsLater;
  }
  return gSuhallaEvents;
}

/// Sets Suhalla up as the party enters: the screen opens with the backdrop
/// fade, and the sprout is drawn grown if Growth was used on it before a
/// return that keeps the party's position. SUHALLA_AFTER_BABI_SPOKE sets
/// FLAG_BABI_SPOKE_AFTER_COLOSSO.
s32 Map_Initialize(void) {
  struct FieldWork *work;

  if (gGameState.warpDest == SUHALLA_AFTER_BABI_SPOKE) {
    Engine_GameFlagSet(FLAG_BABI_SPOKE_AFTER_COLOSSO);
  }

  work = gFieldWork;
  work->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 24;

  if (GameFlag_Test(FLAG_MAP_TEMP_01) != 0) {
    EventScript_GrowSprout();
    Engine_ObjectEventSetAnimation(LOCALID_SPROUT, SPROUT_ANIMATION_GROWN);
  }
  return 0;
}

struct MapWarpDest gSuhallaWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SUHALLA_FROM_INN, CONDITION_ALWAYS, 136, 0, 152, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SUHALLA_FROM_SANCTUM, CONDITION_ALWAYS, 256, 0, 120, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {SUHALLA_FROM_HOUSE_WEST, CONDITION_ALWAYS, 88, 0, 296, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {SUHALLA_FROM_HOUSE_NORTHEAST, CONDITION_ALWAYS, 216, 0, 280, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {SUHALLA_FROM_HOUSE_EAST, CONDITION_ALWAYS, 296, 0, 296, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {SUHALLA_SIXTH_UNUSED, CONDITION_ALWAYS, 120, 0, 272, FACING_EAST, 0, -1,
     -1, -1, -1, 0},
    {SUHALLA_FROM_WORLD_MAP, CONDITION_ALWAYS, 40, 0, 176, FACING_EAST, 0, -1,
     -1, -1, -1, 0},
    {MAP_TABLE_END},
};

u32 gSuhallaWarps[] = {
    MAP_WARPS(Suhalla),
    MAP_WARP(SUHALLA_TO_INN, SuhallaInteriors, SUHALLA_INTERIORS_INN),
    MAP_WARP(SUHALLA_TO_SANCTUM, SuhallaInteriors, SUHALLA_INTERIORS_SANCTUM),
    MAP_WARP(SUHALLA_TO_HOUSE_WEST, SuhallaInteriors,
             SUHALLA_INTERIORS_HOUSE_WEST),
    MAP_WARP(SUHALLA_TO_HOUSE_NORTHEAST, SuhallaInteriors,
             SUHALLA_INTERIORS_HOUSE_NORTHEAST),
    MAP_WARP(SUHALLA_TO_HOUSE_EAST, SuhallaInteriors,
             SUHALLA_INTERIORS_HOUSE_EAST),
    MAP_WARP(SUHALLA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SUHALLA),
    MAP_WARP(SUHALLA_TO_WORLD_MAP_NORTH, WorldMap,
             WORLD_MAP_FROM_SUHALLA_NORTH),
    MAP_WARP(SUHALLA_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_SUHALLA_EAST),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gSuhallaObjectEventTemplates[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(104), 0, PIXELS(184), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(184), 0, PIXELS(304), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(248), 0, PIXELS(168), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(320), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(112), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(168), 0, PIXELS(208), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(144), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gSuhallaObjectEventTemplatesLater[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(104), 0, PIXELS(184), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(320), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(328), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(336), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(248), 0, PIXELS(168), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(320), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(72), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(168), 0, PIXELS(208), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(144), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

/// The cell each door's opening is drawn at, by metatile behavior.
s16 gSuhallaDoorCells[][2] = {
    { 0, 0 }, { 8, 7 }, { 15, 5 }, { 5, 16 }, { 13, 15 }, { 18, 16 },
};

u16 gSuhallaDoorOpenSteps[] = {
    MAPGRID_ANIMATION_STEP(25, 0, 1, 2, 8), MAPGRID_ANIMATION_STEP(26, 0, 1, 2, 1), MAPGRID_ANIMATION_STEPS_END,
};

u16 gSuhallaWideDoorOpenSteps[] = {
    MAPGRID_ANIMATION_STEP(26, 2, 2, 2, 8), MAPGRID_ANIMATION_STEP(28, 2, 2, 2, 1), MAPGRID_ANIMATION_STEPS_END,
};

struct MapEvent gSuhallaEvents[] = {
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), SUHALLA_TO_INN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), SUHALLA_TO_SANCTUM, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterWideDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_WEST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_NORTHEAST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_NORTH },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_EAST },
    { MAP_EVENT_TALK, LOCALID_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaThisIsSuhallaDontTryCrossing) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskIfGoingToLalivero) },
    { MAP_EVENT_TALK, LOCALID_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaWeGondowansWillNeverTrustAnyone) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaItHasBeenSaidForAges) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaYouCantGetToSouthernGondowan) },
    { MAP_EVENT_TALK, LOCALID_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheWorkersSayTherereHugeFestivals) },
    { MAP_EVENT_TALK, LOCALID_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheSandstormsSuddenlyIncreasedIHope) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheSuhallaHasLongBeenKnown) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgSuhallaThePeopleOfLaliveroHaveBeen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheRulerOfTolbiTookA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaIKnowTheDesertSpiritsWill) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaSouthernGondowanIsGuardedByThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaIveHeardThatTolbisColossoIs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaNoneOfTheGrownUpsIn) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_SUHALLA_CHEST_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { BG_EVENT(BG_EVENT_JAR), 101, FLAG_SUHALLA_JAR_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_JAR), 102, FLAG_SUHALLA_JAR_COINS, FIND_COINS(5) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_GROWTH), LOCALID_SPROUT, FLAG_MAP_TEMP_01, EVENT_SCRIPT(EventScript_GrowSprout) },
    { MAP_EVENTS_END },
};

struct MapEvent gSuhallaEventsLater[] = {
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), SUHALLA_TO_INN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), SUHALLA_TO_SANCTUM, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterWideDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_WEST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_NORTHEAST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterDoor) },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_NORTH },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_EAST },
    { MAP_EVENT_TALK, LOCALID_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaWelcomeToSuhallaIBetThe) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheDesertIsEnvelopedInSandstorms) },
    { MAP_EVENT_TALK, LOCALID_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaSoldiersFromTolbiWereCaughtIn) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_BROUGHT_BY_SANDSTORMS, CONDITION_ALWAYS, MSGNO(MsgSuhallaWeWereBroughtHereByThe) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaThisSandstormIsTheFaultOf) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskIfBroughtBySandstorm) },
    { MAP_EVENT_TALK, LOCALID_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaWhyAreTheSandstormsAttackingHumans) },
    { MAP_EVENT_TALK, LOCALID_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaThoseSoldiersKeptMumblingAboutMonsters) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaIBetTheseWarriorsWereBrought) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgSuhallaIWonderWhyThereAreSo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheSoldiersFromTolbiWereCollapsing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_BROUGHT_BY_SANDSTORMS, CONDITION_ALWAYS, MSGNO(MsgSuhallaAfterYouGetSwallowedByA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaLatelyAllTheSandstormsHaveBeen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaIfYouFightTheStormsYoull) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaGrownUpsSayTheSandstormsAttack) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheySayTheSoldiersWereRavaged) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_SUHALLA_CHEST_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { BG_EVENT(BG_EVENT_JAR), 101, FLAG_SUHALLA_JAR_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_JAR), 102, FLAG_SUHALLA_JAR_COINS, FIND_COINS(5) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_GROWTH), LOCALID_SPROUT, FLAG_MAP_TEMP_01, EVENT_SCRIPT(EventScript_GrowSprout) },
    { MAP_EVENTS_END },
};
// clang-format on

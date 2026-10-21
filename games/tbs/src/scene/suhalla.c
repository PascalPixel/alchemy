/// @file
/// SceneId_Suhalla, the desert village of Gondowan on the way to
/// Lalivero, at the edge of the Suhalla Desert.
///
/// Its people and their talk come in two sets, each a placement table and an
/// event table, chosen by whether Babi has held his audience after Colosso
/// (FLAG_BABI_SPOKE_AFTER_COLOSSO). Before it, the village talks of the
/// sandstorms that make the desert impossible to cross, of its distrust of
/// Angara, and of Sheba, the holy child of Lalivero, whom Tolbi's ruler took
/// as a hostage; one woman asks whether the party is trying to get to
/// Lalivero (Event_AskIfGoingToLalivero).
///
/// At that audience Babi sent the party to Venus Lighthouse with Iodem, who
/// commands his soldiers, and Sheba left Tolbi for Lalivero with soldiers
/// bound for Babi Lighthouse. Afterwards the village talks of those soldiers:
/// caught in the sandstorms, nearly all perished, and the survivors lie in a
/// house of the village (SceneId_SuhallaInteriors, where Iodem questions them).
/// The storms attack people as if they were alive, and one old woman asks
/// whether a sandstorm brought the party to Suhalla
/// (Event_AskIfBroughtBySandstorm), for everyone the storms catch winds up
/// here.
///
/// Five doors (SUHALLA_TO_INN to SUHALLA_TO_HOUSE_EAST) open as the leader
/// walks north into them and lead into the village's buildings
/// (SceneId_SuhallaInteriors), the wide door of SUHALLA_TO_SANCTUM into Suhalla
/// Sanctum. Three ways lead out onto the world map: SUHALLA_TO_WORLD_MAP, to
/// WORLD_MAP_FROM_SUHALLA, where the desert's sandstorms also drop the party,
/// SUHALLA_TO_WORLD_MAP_NORTH and SUHALLA_TO_WORLD_MAP_EAST. A chest and two
/// jars hold a Hard Nut, a Lucky Medal and 5 coins, and Growth on a sprout
/// grows it and changes its cell (FLAG_SCENE_TEMP_01, a scene flag, so the
/// grown sprout is redrawn only on a return that keeps the party's position, as
/// after a battle).

#include "game_state.h"
#include "unit/scene/suhalla.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "field_event.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "item_ids.h"
#include "songs.h"
#include "flags.h"

// FLAG_SCENE_TEMP_01: Growth has made the sprout grow.

/// Suhalla's people and its sprout, numbering the rows of both placement
/// tables from ACTOR_FIRST_PLACED: gSuhallaPlacementsLater places the same
/// people, by sprite, in the same rows. Each note gives what they say before
/// Babi's audience, then after it.
enum SuhallaActor {
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), who warns the
  /// party
  /// off the desert; later "Welcome to Suhalla. I bet the sandstorms were
  /// terrible..."
  /// ★ Named from the game: "This is Suhalla."
  /// (MsgSuhallaThisIsSuhallaDontTryCrossing)
  ACTOR_MAN_THIS_IS_SUHALLA = ACTOR_FIRST_PLACED,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), who asks whether
  /// the party is trying to get to Lalivero (Event_AskIfGoingToLalivero);
  /// later "The desert is enveloped in sandstorms! It's frightening!"
  /// ★ Named from the game: "Are you trying to get to Lalivero?"
  /// (MsgSuhallaTryingGetLalivero)
  ACTOR_WOMAN_ASKING_ABOUT_LALIVERO,
  /// A bearded man in a white turban (SPRITE_MAN_IN_WHITE_TURBAN), who
  /// distrusts Angara for taking Sheba as a hostage; later "Soldiers from
  /// Tolbi were caught in the sandstorms, and nearly all perished!"
  /// ★ Named from the game: "We Gondowans will never trust anyone from
  /// Angara!" (MsgSuhallaWeGondowansWillNeverTrustAnyone)
  ACTOR_MAN_NEVER_TRUSTING_ANGARA,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS), placed off the
  /// map until Babi's audience, then one of those the sandstorms have brought
  /// here.
  /// ★ Named from the game: "We were brought here by the sandstorms."
  /// (MsgSuhallaWeWereBroughtHereByThe)
  ACTOR_WOMAN_BROUGHT_BY_SANDSTORMS,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who speaks of the
  /// desert spirits; later "This sandstorm is the fault of the Angarans!"
  /// ★ Named from the game: "desert spirits only create sandstorms"
  /// (MsgSuhallaItHasBeenSaidForAges)
  ACTOR_OLD_MAN_ON_DESERT_SPIRITS,
  /// An old woman with white hair (SPRITE_OLD_WOMAN), who doubts there is a
  /// way to southern Gondowan, then asks whether a sandstorm brought the party
  /// (Event_AskIfBroughtBySandstorm).
  /// ★ Named from the game: "You can't get to southern Gondowan from here"
  /// (MsgSuhallaYouCantGetToSouthernGondowan)
  ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR), who would like to
  /// see Tolbi's festivals; later "Why are the sandstorms attacking humans?"
  /// ★ Named from the game: "The workers say there're huge festivals in
  /// Tolbi." (MsgSuhallaTheWorkersSayTherereHugeFestivals)
  ACTOR_BOY_ON_TOLBI_FESTIVALS,
  /// A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS), who
  /// worries about the storms; later "Those soldiers kept mumbling about
  /// monsters..."
  /// ★ Named from the game: "The sandstorms suddenly increased..."
  /// (MsgSuhallaTheSandstormsSuddenlyIncreasedIHope)
  ACTOR_GIRL_SANDSTORMS_INCREASED,
  /// At 72, 144; Growth grows it (Event_GrowSprout).
  /// ★ Named from what the player sees: a small green sprout in the sand by the
  /// inn (its render)
  ACTOR_SPROUT
};

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSuhallaEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gSuhallaExits; }

/// Suhalla's people, who stand elsewhere after Babi's audience.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
    return gSuhallaPlacementsLater;
  }
  return gSuhallaPlacements;
}

/// A woman asks whether the party is trying to get to Lalivero. She's
/// ACTOR_WOMAN_ASKING_ABOUT_LALIVERO, in Suhalla before Babi's audience. "Are
/// you trying to get to Lalivero?" Yes, and the sandstorms make it impossible;
/// no, and the woman doubts it, for Angarans who come to Suhalla are always
/// going to Lalivero.
static void Event_AskIfGoingToLalivero(s32 actorId) {
  s32 firstMessage = MsgSuhallaTryingGetLalivero;
  Engine_EventSetMessage(firstMessage);
  Engine_EventOpenMessage(actorId, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(10);
    Engine_EventSetMessage(firstMessage + 1);
  } else {
    Engine_EventSetMessage(firstMessage + 2);
  }
  Engine_EventShowMessage(actorId, 0);
}

/// An old woman asks whether the sandstorm brought the party to Suhalla. She's
/// ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, there after Babi's audience. Yes, and
/// everyone the storms catch winds up here; no, and the old woman wonders why
/// there are so many storms.
static void Event_AskIfBroughtBySandstorm(s32 actorId) {
  s32 firstMessage = MsgSuhallaBroughtSuhallaSandstorm;
  Engine_EventSetMessage(firstMessage);
  Engine_EventOpenMessage(actorId, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(10);
    Engine_EventSetMessage(firstMessage + 1);
  } else {
    Engine_EventSetMessage(firstMessage + 2);
  }
  Engine_EventShowMessage(actorId, 0);
}

/// Growth on Suhalla's sprout: the plant grows, and its map cell takes the
/// attributes of the grown plant's cell. Sets FLAG_SCENE_TEMP_01.
static void Event_GrowSprout(void) {
  Map_CopyCellAttributes(25, 9, 1, 1, 4, 9);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
}

/// Walking north into one of Suhalla's doors (SUHALLA_TO_INN,
/// SUHALLA_TO_HOUSE_WEST, SUHALLA_TO_HOUSE_NORTHEAST and SUHALLA_TO_HOUSE_EAST,
/// whose cell codes are their exits' numbers): the door opens, the leader
/// steps through and the party leaves by that exit into the village's
/// buildings.
static void Event_EnterDoor(void) {
  struct EventWork *work = gEventWork;
  s32 exitTrigger = work->touchedTrigger;
  u16 cellX = gSuhallaDoorCells[exitTrigger][0];
  u16 cellZ = gSuhallaDoorCells[exitTrigger][1];

  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gSuhallaDoorOpenSteps, cellX, cellZ);
  // Walk through the opening while the exit transition counts down.
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -16);
  gEventWork->transitionFrames = 16;
  Engine_EventRequestExit(exitTrigger);
}

/// The same through the wide door of SUHALLA_TO_SANCTUM, into Suhalla Sanctum.
static void Event_EnterWideDoor(void) {
  struct EventWork *work = gEventWork;
  s32 exitTrigger = work->touchedTrigger;
  u16 cellX = gSuhallaDoorCells[exitTrigger][0];
  u16 cellZ = gSuhallaDoorCells[exitTrigger][1];

  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gSuhallaWideDoorOpenSteps, cellX, cellZ);
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -16);
  gEventWork->transitionFrames = 16;
  Engine_EventRequestExit(exitTrigger);
}

/// Suhalla's doors, people, treasure and sprout, before or after Babi's
/// audience.
const struct SceneEvent *Scene_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
    return gSuhallaEventsLater;
  }
  return gSuhallaEvents;
}

/// Sets Suhalla up as the party enters: the screen opens with the backdrop
/// fade, and the sprout is drawn grown if Growth was used on it before a
/// return that keeps the party's position. SUHALLA_AFTER_BABI_AUDIENCE sets
/// FLAG_BABI_SPOKE_AFTER_COLOSSO.
s32 Scene_Initialize(void) {
  struct EventWork *work;

  if (gGameState.entrance == SUHALLA_AFTER_BABI_AUDIENCE) {
    Engine_GameFlagSet(FLAG_BABI_SPOKE_AFTER_COLOSSO);
  }

  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 24;

  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01) != 0) {
    Event_GrowSprout();
    Engine_ActorSetAnimation(ACTOR_SPROUT, 4);
  }
  return 0;
}

struct SceneEntrance gSuhallaEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
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
    {6, CONDITION_ALWAYS, 120, 0, 272, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SUHALLA_FROM_WORLD_MAP, CONDITION_ALWAYS, 40, 0, 176, FACING_EAST, 0, -1,
     -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSuhallaExits[] = {
    SCENE_EXITS(Suhalla),
    SCENE_EXIT(SUHALLA_TO_INN, SuhallaInteriors, SUHALLA_INTERIORS_INN),
    SCENE_EXIT(SUHALLA_TO_SANCTUM, SuhallaInteriors, SUHALLA_INTERIORS_SANCTUM),
    SCENE_EXIT(SUHALLA_TO_HOUSE_WEST, SuhallaInteriors,
               SUHALLA_INTERIORS_HOUSE_WEST),
    SCENE_EXIT(SUHALLA_TO_HOUSE_NORTHEAST, SuhallaInteriors,
               SUHALLA_INTERIORS_HOUSE_NORTHEAST),
    SCENE_EXIT(SUHALLA_TO_HOUSE_EAST, SuhallaInteriors,
               SUHALLA_INTERIORS_HOUSE_EAST),
    SCENE_EXIT(SUHALLA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SUHALLA),
    SCENE_EXIT(SUHALLA_TO_WORLD_MAP_NORTH, WorldMap,
               WORLD_MAP_FROM_SUHALLA_NORTH),
    SCENE_EXIT(SUHALLA_TO_WORLD_MAP_EAST, WorldMap,
               WORLD_MAP_FROM_SUHALLA_EAST),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gSuhallaPlacements[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(104), 0, PIXELS(184), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(184), 0, PIXELS(304), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(168), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(320), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(112), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(168), 0, PIXELS(208), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(144), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSuhallaPlacementsLater[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(104), 0, PIXELS(184), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(320), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(328), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(336), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(168), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(320), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(72), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(168), 0, PIXELS(208), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(144), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

/// The cell each door's opening is drawn at, by trigger.
s16 gSuhallaDoorCells[][2] = {
    { 0, 0 }, { 8, 7 }, { 15, 5 }, { 5, 16 }, { 13, 15 }, { 18, 16 },
};

u16 gSuhallaDoorOpenSteps[] = {
    MAP_CELL_STEP(25, 0, 1, 2, 8), MAP_CELL_STEP(26, 0, 1, 2, 1), MAP_CELL_STEPS_END,
};

u16 gSuhallaWideDoorOpenSteps[] = {
    MAP_CELL_STEP(26, 2, 2, 2, 8), MAP_CELL_STEP(28, 2, 2, 2, 1), MAP_CELL_STEPS_END,
};

struct SceneEvent gSuhallaEvents[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterWideDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_NORTH },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_EAST },
    { EVENT_TALK, ACTOR_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaThisIsSuhallaDontTryCrossing) },
    { EVENT_TALK, ACTOR_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskIfGoingToLalivero) },
    { EVENT_TALK, ACTOR_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaWeGondowansWillNeverTrustAnyone) },
    { EVENT_TALK, ACTOR_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaItHasBeenSaidForAges) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaYouCantGetToSouthernGondowan) },
    { EVENT_TALK, ACTOR_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheWorkersSayTherereHugeFestivals) },
    { EVENT_TALK, ACTOR_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheSandstormsSuddenlyIncreasedIHope) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheSuhallaHasLongBeenKnown) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgSuhallaThePeopleOfLaliveroHaveBeen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheRulerOfTolbiTookA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaIKnowTheDesertSpiritsWill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaSouthernGondowanIsGuardedByThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaIveHeardThatTolbisColossoIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaNoneOfTheGrownUpsIn) },
    { SEARCH(SEARCH_CHEST), 100, 0xfad, FIND_ITEM(ITEM_HARD_NUT) },
    { SEARCH(SEARCH_JAR), 101, 0xfae, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_JAR), 102, 0xfaf, FIND_COINS(5) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Event_GrowSprout) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSuhallaEventsLater[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterWideDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_NORTH },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_EAST },
    { EVENT_TALK, ACTOR_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaWelcomeToSuhallaIBetThe) },
    { EVENT_TALK, ACTOR_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheDesertIsEnvelopedInSandstorms) },
    { EVENT_TALK, ACTOR_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaSoldiersFromTolbiWereCaughtIn) },
    { EVENT_TALK, ACTOR_WOMAN_BROUGHT_BY_SANDSTORMS, CONDITION_ALWAYS, MSGNO(MsgSuhallaWeWereBroughtHereByThe) },
    { EVENT_TALK, ACTOR_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaThisSandstormIsTheFaultOf) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskIfBroughtBySandstorm) },
    { EVENT_TALK, ACTOR_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaWhyAreTheSandstormsAttackingHumans) },
    { EVENT_TALK, ACTOR_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaThoseSoldiersKeptMumblingAboutMonsters) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaIBetTheseWarriorsWereBrought) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgSuhallaIWonderWhyThereAreSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheSoldiersFromTolbiWereCollapsing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_BROUGHT_BY_SANDSTORMS, CONDITION_ALWAYS, MSGNO(MsgSuhallaAfterYouGetSwallowedByA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaLatelyAllTheSandstormsHaveBeen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaIfYouFightTheStormsYoull) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaGrownUpsSayTheSandstormsAttack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheySayTheSoldiersWereRavaged) },
    { SEARCH(SEARCH_CHEST), 100, 0xfad, FIND_ITEM(ITEM_HARD_NUT) },
    { SEARCH(SEARCH_JAR), 101, 0xfae, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_JAR), 102, 0xfaf, FIND_COINS(5) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Event_GrowSprout) },
    { SCENE_EVENTS_END },
};
// clang-format on

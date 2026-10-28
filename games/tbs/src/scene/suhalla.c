/// @file
/// Suhalla (SceneId_Suhalla, area name Suhalla), the desert village of Gondowan
/// at the edge of the Suhalla Desert, on the way from Gondowan Passage to
/// Lalivero.
///
/// Its people come in two sets, each an actor placement table and a
/// trigger table, chosen by FLAG_BABI_SPOKE_AFTER_COLOSSO: before Babi's
/// audience after Colosso the village talks of the sandstorms that close the
/// desert, of its distrust of Angara and of Sheba, the hostage; after it, of
/// the soldiers the storms caught on the way to Babi Lighthouse, who lie in one
/// of its houses (timber_rooms_a.c). The same people stand in the same rows of
/// both actor placement tables, so one enum names them; the woman the
/// sandstorms brought stands off the map until the audience.
///
/// Five doors, whose cell behaviors are their exits' numbers
/// (SUHALLA_TO_INN to SUHALLA_TO_HOUSE_EAST), open as the leader walks north
/// into them (Suhalla_EnterDoor, Suhalla_EnterWideDoor) and lead into
/// the village's buildings. Three ways lead out onto the World Map, and the
/// desert's sandstorms drop a caught party at WORLD_MAP_FROM_SUHALLA. A chest
/// and two jars hold a Hard Nut, a Lucky Medal and coins, and Growth grows a
/// sprout and changes its cell. Its scene flag:
///
/// - FLAG_SUHALLA_SPROUT_GROWN: Growth has grown the sprout; a return that
///   keeps the party's position, as after a battle, draws it grown again.

#include "main_routine_work.h"
#include "module/scene/suhalla.h"
#include "module/field/field_sequence.h"
#include "module/field/event_commands.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "event.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "scene_tables.h"
#include "item_ids.h"
#include "sounds.h"
#include "flags.h"

/// Growth has grown the sprout; a return that keeps the party's position draws
/// it grown again.
/// ★ Named from what the player sees: a small green sprout in the sand by the
/// inn (its render)
#define FLAG_SUHALLA_SPROUT_GROWN FLAG_SCENE_TEMP_01

/// The sprout's animation once Growth has grown it.
enum { SPROUT_ANIMATION_GROWN = 4 };

/// Suhalla's people and its sprout, numbering the rows of both actor
/// placement tables from ACTOR_FIRST_PLACED: gSuhallaActorPlacementsLater
/// places the same people, by sprite, in the same rows. Each note gives what
/// they say before Babi's audience, then after it.
enum SuhallaActorId {
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), who warns the
  /// party
  /// off the desert; later "Welcome to Suhalla. I bet the sandstorms were
  /// terrible..."
  /// ★ Named from the game: "This is Suhalla."
  /// (MsgSuhallaThisIsSuhallaDontTryCrossing)
  ACTOR_MAN_THIS_IS_SUHALLA = ACTOR_FIRST_PLACED,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), who asks whether
  /// the party is trying to get to Lalivero (Suhalla_AskIfGoingToLalivero);
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
  /// (Suhalla_AskIfBroughtBySandstorm).
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
  /// At 72, 144; Growth grows it (Suhalla_GrowSprout).
  /// ★ Named from what the player sees: a small green sprout in the sand by the
  /// inn (its render)
  ACTOR_SPROUT
};

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSuhallaEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gSuhallaExits; }

/// Suhalla's people, who stand elsewhere after Babi's audience.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
    return gSuhallaActorPlacementsLater;
  }
  return gSuhallaActorPlacements;
}

/// A woman asks whether the party is trying to get to Lalivero. She's
/// ACTOR_WOMAN_ASKING_ABOUT_LALIVERO, in Suhalla before Babi's audience. "Are
/// you trying to get to Lalivero?" Yes, and the sandstorms make it impossible;
/// no, and the woman doubts it, for Angarans who come to Suhalla are always
/// going to Lalivero.
static void Suhalla_AskIfGoingToLalivero(s32 actorId) {
  s32 firstMessage = MsgSuhallaTryingGetLalivero;
  Event_SetMessageId(firstMessage);
  FieldMessage_ShowAtActor(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(firstMessage + 1);
  } else {
    Event_SetMessageId(firstMessage + 2);
  }
  Event_ShowMessageAndWait(actorId, 0);
}

/// An old woman asks whether the sandstorm brought the party to Suhalla. She's
/// ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, there after Babi's audience. Yes,
/// and everyone the storms catch winds up here; no, and the old woman wonders
/// why there are so many storms.
static void Suhalla_AskIfBroughtBySandstorm(s32 actorId) {
  s32 firstMessage = MsgSuhallaBroughtSuhallaSandstorm;
  Event_SetMessageId(firstMessage);
  FieldMessage_ShowAtActor(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(firstMessage + 1);
  } else {
    Event_SetMessageId(firstMessage + 2);
  }
  Event_ShowMessageAndWait(actorId, 0);
}

/// Growth on Suhalla's sprout: the plant grows, and its cell takes the cell
/// behavior of the grown plant's cell. Sets FLAG_SUHALLA_SPROUT_GROWN.
static void Suhalla_GrowSprout(void) {
  MapGrid_CopyCellAttributes(25, 9, 1, 1, 4, 9);
  GameFlag_Set(FLAG_SUHALLA_SPROUT_GROWN);
}

/// Walking north into one of Suhalla's doors (SUHALLA_TO_INN,
/// SUHALLA_TO_HOUSE_WEST, SUHALLA_TO_HOUSE_NORTHEAST and SUHALLA_TO_HOUSE_EAST,
/// whose cell behaviors are their exits' numbers): the door opens, the
/// leader steps through and the party leaves by that exit into the village's
/// buildings.
static void Suhalla_EnterDoor(void) {
  struct FieldWork *work = gFieldWork;
  s32 exitTrigger = work->stepTrigger;
  u16 cellX = gSuhallaDoorCells[exitTrigger][0];
  u16 cellZ = gSuhallaDoorCells[exitTrigger][1];

  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gSuhallaDoorOpenSteps, cellX, cellZ);
  // Walk through the opening while the exit transition counts down.
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -16);
  gFieldWork->transitionFrames = 16;
  Import_EventSetExitRequest(exitTrigger);
}

/// The same through the wide door of SUHALLA_TO_SANCTUM, into Suhalla Sanctum.
static void Suhalla_EnterWideDoor(void) {
  struct FieldWork *work = gFieldWork;
  s32 exitTrigger = work->stepTrigger;
  u16 cellX = gSuhallaDoorCells[exitTrigger][0];
  u16 cellZ = gSuhallaDoorCells[exitTrigger][1];

  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gSuhallaWideDoorOpenSteps, cellX, cellZ);
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -16);
  gFieldWork->transitionFrames = 16;
  Import_EventSetExitRequest(exitTrigger);
}

/// Suhalla's doors, people, treasure and sprout, before or after Babi's
/// audience.
const struct Trigger *Scene_GetTriggers(void) {
  if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
    return gSuhallaTriggersLater;
  }
  return gSuhallaTriggers;
}

/// Sets Suhalla up as the party enters: the screen opens with the backdrop
/// fade, and the sprout is drawn grown if Growth was used on it before a
/// return that keeps the party's position. SUHALLA_AFTER_BABI_SPOKE sets
/// FLAG_BABI_SPOKE_AFTER_COLOSSO.
s32 Scene_Initialize(void) {
  struct FieldWork *work;

  if (gMainRoutineWork.entrance == SUHALLA_AFTER_BABI_SPOKE) {
    GameFlag_Set(FLAG_BABI_SPOKE_AFTER_COLOSSO);
  }

  work = gFieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 24;

  if (GameFlag_Get(FLAG_SUHALLA_SPROUT_GROWN) != 0) {
    Suhalla_GrowSprout();
    Import_ActorSetAnimation(ACTOR_SPROUT, SPROUT_ANIMATION_GROWN);
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
    {SUHALLA_SIXTH_UNUSED, CONDITION_ALWAYS, 120, 0, 272, FACING_EAST, 0, -1,
     -1, -1, -1, 0},
    {SUHALLA_FROM_WORLD_MAP, CONDITION_ALWAYS, 40, 0, 176, FACING_EAST, 0, -1,
     -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSuhallaExits[] = {
    SCENE_EXITS(Suhalla),
    SCENE_EXIT(SUHALLA_TO_INN, TimberRoomsA, TIMBER_ROOMS_A_INN),
    SCENE_EXIT(SUHALLA_TO_SANCTUM, TimberRoomsA, TIMBER_ROOMS_A_SANCTUM),
    SCENE_EXIT(SUHALLA_TO_HOUSE_WEST, TimberRoomsA, TIMBER_ROOMS_A_HOUSE_WEST),
    SCENE_EXIT(SUHALLA_TO_HOUSE_NORTHEAST, TimberRoomsA,
               TIMBER_ROOMS_A_HOUSE_NORTHEAST),
    SCENE_EXIT(SUHALLA_TO_HOUSE_EAST, TimberRoomsA, TIMBER_ROOMS_A_HOUSE_EAST),
    SCENE_EXIT(SUHALLA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SUHALLA),
    SCENE_EXIT(SUHALLA_TO_WORLD_MAP_NORTH, WorldMap,
               WORLD_MAP_FROM_SUHALLA_NORTH),
    SCENE_EXIT(SUHALLA_TO_WORLD_MAP_EAST, WorldMap,
               WORLD_MAP_FROM_SUHALLA_EAST),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gSuhallaActorPlacements[] = {
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
    { SCENE_TABLE_END },
};

struct ActorPlacement gSuhallaActorPlacementsLater[] = {
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
    { SCENE_TABLE_END },
};

/// The cell each door's opening is drawn at, by cell behavior.
s16 gSuhallaDoorCells[][2] = {
    { 0, 0 }, { 8, 7 }, { 15, 5 }, { 5, 16 }, { 13, 15 }, { 18, 16 },
};

u16 gSuhallaDoorOpenSteps[] = {
    MAPGRID_ANIMATION_STEP(25, 0, 1, 2, 8), MAPGRID_ANIMATION_STEP(26, 0, 1, 2, 1), MAPGRID_ANIMATION_STEPS_END,
};

u16 gSuhallaWideDoorOpenSteps[] = {
    MAPGRID_ANIMATION_STEP(26, 2, 2, 2, 8), MAPGRID_ANIMATION_STEP(28, 2, 2, 2, 1), MAPGRID_ANIMATION_STEPS_END,
};

struct Trigger gSuhallaTriggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_INN, CONDITION_ALWAYS, EVENT(Suhalla_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_SANCTUM, CONDITION_ALWAYS, EVENT(Suhalla_EnterWideDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_WEST, CONDITION_ALWAYS, EVENT(Suhalla_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_NORTHEAST, CONDITION_ALWAYS, EVENT(Suhalla_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT(Suhalla_EnterDoor) },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_NORTH },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_EAST },
    { TRIGGER_TALK, ACTOR_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaThisIsSuhallaDontTryCrossing) },
    { TRIGGER_TALK, ACTOR_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, EVENT(Suhalla_AskIfGoingToLalivero) },
    { TRIGGER_TALK, ACTOR_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaWeGondowansWillNeverTrustAnyone) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaItHasBeenSaidForAges) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaYouCantGetToSouthernGondowan) },
    { TRIGGER_TALK, ACTOR_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheWorkersSayTherereHugeFestivals) },
    { TRIGGER_TALK, ACTOR_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheSandstormsSuddenlyIncreasedIHope) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheSuhallaHasLongBeenKnown) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgSuhallaThePeopleOfLaliveroHaveBeen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheRulerOfTolbiTookA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaIKnowTheDesertSpiritsWill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaSouthernGondowanIsGuardedByThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaIveHeardThatTolbisColossoIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaNoneOfTheGrownUpsIn) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_SUHALLA_CHEST_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 101, FLAG_SUHALLA_JAR_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 102, FLAG_SUHALLA_JAR_COINS, FIND_COINS(5) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_SUHALLA_SPROUT_GROWN, EVENT(Suhalla_GrowSprout) },
    { TRIGGERS_END },
};

struct Trigger gSuhallaTriggersLater[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_INN, CONDITION_ALWAYS, EVENT(Suhalla_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_SANCTUM, CONDITION_ALWAYS, EVENT(Suhalla_EnterWideDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_WEST, CONDITION_ALWAYS, EVENT(Suhalla_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_NORTHEAST, CONDITION_ALWAYS, EVENT(Suhalla_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT(Suhalla_EnterDoor) },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_NORTH },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_EAST },
    { TRIGGER_TALK, ACTOR_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaWelcomeToSuhallaIBetThe) },
    { TRIGGER_TALK, ACTOR_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheDesertIsEnvelopedInSandstorms) },
    { TRIGGER_TALK, ACTOR_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaSoldiersFromTolbiWereCaughtIn) },
    { TRIGGER_TALK, ACTOR_WOMAN_BROUGHT_BY_SANDSTORMS, CONDITION_ALWAYS, MSGNO(MsgSuhallaWeWereBroughtHereByThe) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaThisSandstormIsTheFaultOf) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, EVENT(Suhalla_AskIfBroughtBySandstorm) },
    { TRIGGER_TALK, ACTOR_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaWhyAreTheSandstormsAttackingHumans) },
    { TRIGGER_TALK, ACTOR_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaThoseSoldiersKeptMumblingAboutMonsters) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaIBetTheseWarriorsWereBrought) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgSuhallaIWonderWhyThereAreSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheSoldiersFromTolbiWereCollapsing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_BROUGHT_BY_SANDSTORMS, CONDITION_ALWAYS, MSGNO(MsgSuhallaAfterYouGetSwallowedByA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaLatelyAllTheSandstormsHaveBeen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaIfYouFightTheStormsYoull) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaGrownUpsSayTheSandstormsAttack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheySayTheSoldiersWereRavaged) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_SUHALLA_CHEST_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 101, FLAG_SUHALLA_JAR_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 102, FLAG_SUHALLA_JAR_COINS, FIND_COINS(5) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_SUHALLA_SPROUT_GROWN, EVENT(Suhalla_GrowSprout) },
    { TRIGGERS_END },
};
// clang-format on

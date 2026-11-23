/// @file
/// Suhalla, the desert village of Gondowan at the edge of the Suhalla Desert.
/// Isaac's party, with Iodem, reaches it from the west after the Gondowan
/// Bridge, and leaves it eastward for the desert on the way to Lalivero.
///
/// Before Babi's audience after Colosso, the village talks of the sandstorms
/// that close the desert, of its distrust of Angara and of Sheba, the
/// hostage. After it, the village talks of the soldiers the storms caught on
/// the way to Babi Lighthouse, who lie in the house in the south-east corner
/// (timber_rooms_a.c), and a woman the sandstorms brought here stands by the
/// south-west house.
///
/// The people come in two sets, each an actor placement table and a trigger
/// table, chosen by FLAG_BABI_SPOKE_AFTER_COLOSSO. The same people stand in
/// the same rows of both actor placement tables, so one enum names them; the
/// woman the sandstorms brought stands off the map until the audience.
///
/// Five doors lead indoors: the inn in the north-west (its door at cell
/// (8, 9)), the Sanctum's arched double door in the north, inside a ring of
/// water (cell (15, 7)), and along the south the shop (cell (5, 18)), the old
/// couple's house (cell (13, 17)) and the house in the south-east corner
/// (cell (18, 18)). Each opens as the leader walks north into it
/// (Suhalla_EnterDoor, Suhalla_EnterSanctumDoor). Their cell behaviors are
/// their exits' numbers (SUHALLA_TO_INN to SUHALLA_TO_HOUSE_EAST).
///
/// The village's west, north and east edges lead out onto the World Map, and
/// the desert's sandstorms drop a caught party outside it
/// (WORLD_MAP_FROM_SUHALLA).
///
/// A jar on the west edge just south of the way in holds 5 coins. The Lucky
/// Medal is in a jar beside the Sanctum, reached by hopping over the water
/// behind it, and the Hard Nut is in a chest farther on, in the north-west
/// corner (cell (1, 1)). Growth on the sprout beside the inn's door grows a
/// vine up the inn's wall.
///
/// Its scene flag:
///
/// - FLAG_SUHALLA_SPROUT_GROWN: Growth has grown the sprout. A return that
///   keeps the party's position, as after a battle, draws it grown again.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern struct SceneEntrance gSuhallaEntrances[];
extern u32 gSuhallaExits[];
extern struct ActorPlacement gSuhallaActorPlacements[];
extern struct ActorPlacement gSuhallaActorPlacementsLater[];
extern s16 gSuhallaDoorCells[][2];
extern u16 gSuhallaDoorOpenSteps[];
extern u16 gSuhallaWideDoorOpenSteps[];
extern struct Trigger gSuhallaTriggers[];
extern struct Trigger gSuhallaTriggersLater[];

/// Growth has grown the sprout.
///
/// A return that keeps the party's position draws it grown again.
#define FLAG_SUHALLA_SPROUT_GROWN FLAG_SCENE_TEMP_01

/// The cell behaviors of the village's edges, each a way out onto the World
/// Map.
enum SuhallaCellBehavior {
  /// The west edge, where the party comes in from the Gondowan Bridge.
  CELL_BEHAVIOR_SUHALLA_WEST_EDGE = 10,
  CELL_BEHAVIOR_SUHALLA_NORTH_EDGE,
  /// The east edge, toward the Suhalla Desert.
  CELL_BEHAVIOR_SUHALLA_EAST_EDGE
};

/// Suhalla's people and its sprout, numbering the rows of both actor
/// placement tables from ACTOR_FIRST_PLACED.
///
/// gSuhallaActorPlacementsLater places the same people, by sprite, in the
/// same rows. Each note gives what they say before Babi's audience, then
/// after it.
enum SuhallaActorId {
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), who warns the
  /// party off the desert.
  ///
  /// Later he says "Welcome to Suhalla. I bet the sandstorms were
  /// terrible..."
  ACTOR_MAN_THIS_IS_SUHALLA = ACTOR_FIRST_PLACED,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), who asks whether
  /// the party is trying to get to Lalivero (Suhalla_AskIfGoingToLalivero).
  ///
  /// Later she says "The desert is enveloped in sandstorms! It's frightening!"
  ACTOR_WOMAN_ASKING_ABOUT_LALIVERO,
  /// A bearded man in a white turban (SPRITE_MAN_IN_WHITE_TURBAN), who
  /// distrusts Angara for taking Sheba as a hostage.
  ///
  /// Later he says "Soldiers from Tolbi were caught in the sandstorms, and
  /// nearly all perished!"
  ACTOR_MAN_NEVER_TRUSTING_ANGARA,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS), placed off the
  /// map until Babi's audience.
  ///
  /// After it she is one of those the sandstorms have brought here.
  ACTOR_WOMAN_BROUGHT_BY_SANDSTORMS,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who speaks of the
  /// desert spirits.
  ///
  /// Later he says "This sandstorm is the fault of the Angarans!"
  ACTOR_OLD_MAN_ON_DESERT_SPIRITS,
  /// An old woman with white hair (SPRITE_OLD_WOMAN), who doubts there is a
  /// way to southern Gondowan.
  ///
  /// Later she asks whether a sandstorm brought the party
  /// (Suhalla_AskIfBroughtBySandstorm).
  ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR), who would like to
  /// see Tolbi's festivals.
  ///
  /// Later he says "Why are the sandstorms attacking humans?"
  ACTOR_BOY_ON_TOLBI_FESTIVALS,
  /// A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS), who
  /// worries about the storms.
  ///
  /// Later she says "Those soldiers kept mumbling about monsters..."
  ACTOR_GIRL_SANDSTORMS_INCREASED,
  /// The sprout beside the inn's door, which Growth grows into a vine
  /// (Suhalla_GrowSprout).
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

/// The woman with fair hair, who wanders by the houses in the south, asks
/// whether the party is trying to get to Lalivero, and answers yes or no.
static void Suhalla_AskIfGoingToLalivero(s32 actorId) {
  s32 firstMessage = MsgSuhallaTryingGetLalivero;
  Event_SetMessageId(firstMessage);
  // "Are you trying to get to Lalivero?"
  Event_OpenMessage(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(10);
    // Yes: "Making it through the sandstorms is impossible. Don't bother."
    Event_SetMessageId(firstMessage + 1);
  } else {
    // No: "Didn't you come from Angara? Anytime Angarans come to Suhalla,
    // they're always going to Lalivero."
    Event_SetMessageId(firstMessage + 2);
  }
  Event_ShowMessage(actorId, 0);
}

/// After Babi's audience, the old woman by the south-west house asks whether
/// the sandstorm brought the party to Suhalla, and answers yes or no.
static void Suhalla_AskIfBroughtBySandstorm(s32 actorId) {
  s32 firstMessage = MsgSuhallaBroughtSuhallaSandstorm;
  Event_SetMessageId(firstMessage);
  // "You were brought to Suhalla by the sandstorm, right?"
  Event_OpenMessage(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(10);
    // Yes: "Everyone who gets caught in the sandstorms winds up in Suhalla..."
    Event_SetMessageId(firstMessage + 1);
  } else {
    // No: "I don't care what anyone says, there shouldn't be so many
    // sandstorms!..."
    Event_SetMessageId(firstMessage + 2);
  }
  Event_ShowMessage(actorId, 0);
}

/// Growth on the sprout beside the inn's door, in the north-west of Suhalla,
/// grows a green vine up the inn's wall (cell (4, 9)).
///
/// The vine's cell takes its attributes from a spare cell off the drawn map,
/// so it can be climbed.
static void Suhalla_GrowSprout(void) {
  MapGrid_CopyCellAttributes(25, 9, 1, 1, 4, 9);
  GameFlag_Set(FLAG_SUHALLA_SPROUT_GROWN);
}

/// Walking north into the door of the inn, or of the shop or one of the two
/// houses along Suhalla's south side, opens it and takes the party inside.
///
/// The door is drawn open, the leader walks a step in, and the scene leaves by
/// the exit numbered as the door's cell behavior.
static void Suhalla_EnterDoor(void) {
  struct FieldWork *work = gFieldWork;
  s32 exitTrigger = work->stepTrigger;
  u16 cellX = gSuhallaDoorCells[exitTrigger][0];
  u16 cellZ = gSuhallaDoorCells[exitTrigger][1];

  Sound_Play(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gSuhallaDoorOpenSteps, cellX, cellZ);
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -16);
  gFieldWork->transitionFrames = MAP_TRANSITION_FRAMES;
  Import_EventSetExitRequest(exitTrigger);
}

/// Walking north into the Sanctum's arched double door, in the north of
/// Suhalla inside the ring of water (cell (15, 7)), opens it and takes the
/// party into Suhalla's Sanctum.
///
/// It works as Suhalla_EnterDoor does, with a door two cells wide.
static void Suhalla_EnterSanctumDoor(void) {
  struct FieldWork *work = gFieldWork;
  s32 exitTrigger = work->stepTrigger;
  u16 cellX = gSuhallaDoorCells[exitTrigger][0];
  u16 cellZ = gSuhallaDoorCells[exitTrigger][1];

  Sound_Play(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gSuhallaWideDoorOpenSteps, cellX, cellZ);
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -16);
  gFieldWork->transitionFrames = MAP_TRANSITION_FRAMES;
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

/// Sets Suhalla up as the party enters: arriving from Babi's audience turns
/// the village to its later people, and after a battle a vine Growth has grown
/// stands again.
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
    Actor_SetAnimation(ACTOR_SPROUT, SPROUT_ANIMATION_GROWN);
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
    {SUHALLA_FROM_SHOP, CONDITION_ALWAYS, 88, 0, 296, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SUHALLA_FROM_HOUSE_SOUTH, CONDITION_ALWAYS, 216, 0, 280, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
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
    SCENE_EXIT(SUHALLA_TO_SHOP, TimberRoomsA, TIMBER_ROOMS_A_SHOP),
    SCENE_EXIT(SUHALLA_TO_HOUSE_SOUTH, TimberRoomsA,
               TIMBER_ROOMS_A_HOUSE_SOUTH),
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
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        104, 0, 184, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        184, 0, 304, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        296, 0, 328, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        -64, 0, -64, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        248, 0, 168, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 320, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        72, 0, 112, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        168, 0, 208, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        72, 0, 144, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gSuhallaActorPlacementsLater[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        104, 0, 184, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 320, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        296, 0, 328, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        88, 0, 336, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        248, 0, 168, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 320, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        72, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        168, 0, 208, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        72, 0, 144, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

/// The top-left cell each door's opening is drawn from, by its cell behavior
/// (the exit it leads to), two cells above where the leader walks into it.
/// Then a door's opening, drawn in two steps, and the Sanctum's wide door's.
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
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_SANCTUM, CONDITION_ALWAYS, EVENT(Suhalla_EnterSanctumDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_SHOP, CONDITION_ALWAYS, EVENT(Suhalla_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_SOUTH, CONDITION_ALWAYS, EVENT(Suhalla_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT(Suhalla_EnterDoor) },
    { TRIGGER_EXIT, CELL_BEHAVIOR_SUHALLA_WEST_EDGE, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP },
    { TRIGGER_EXIT, CELL_BEHAVIOR_SUHALLA_NORTH_EDGE, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_NORTH },
    { TRIGGER_EXIT, CELL_BEHAVIOR_SUHALLA_EAST_EDGE, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_EAST },
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
    { TRIGGER_TABLE_END },
};

struct Trigger gSuhallaTriggersLater[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_INN, CONDITION_ALWAYS, EVENT(Suhalla_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_SANCTUM, CONDITION_ALWAYS, EVENT(Suhalla_EnterSanctumDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_SHOP, CONDITION_ALWAYS, EVENT(Suhalla_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_SOUTH, CONDITION_ALWAYS, EVENT(Suhalla_EnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT(Suhalla_EnterDoor) },
    { TRIGGER_EXIT, CELL_BEHAVIOR_SUHALLA_WEST_EDGE, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP },
    { TRIGGER_EXIT, CELL_BEHAVIOR_SUHALLA_NORTH_EDGE, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_NORTH },
    { TRIGGER_EXIT, CELL_BEHAVIOR_SUHALLA_EAST_EDGE, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_EAST },
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
    { TRIGGER_TABLE_END },
};
// clang-format on

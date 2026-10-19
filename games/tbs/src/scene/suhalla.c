/// @file
/// Suhalla, the town at the edge of the Suhalla Desert. Once Babi's audience
/// after Colosso has been held, its people stand and talk differently: the
/// sandstorms have brought Tolbi's soldiers to the town.

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

/// Arriving by this entrance also marks Babi's audience as held.
enum { ENTRANCE_90 = 90 };

/// The sprout at 72, 144, the last of the placements.
enum { SPROUT = 16 };

/// Where the party can enter Suhalla.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSuhallaEntrances;
}

/// Suhalla has no regions.
const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

/// Where each exit leads.
const u32 *Scene_GetExits(void) { return gSuhallaExits; }

/// The townsfolk, who stand elsewhere after Babi's audience.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
    return gSuhallaPlacementsLater;
  }
  return gSuhallaPlacements;
}

/// A villager asks about Lalivero and answers yes or no.
/// "Are you trying to get to Lalivero?"
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

/// After Babi's audience a villager asks whether the sandstorm brought the
/// party to Suhalla, and answers yes or no.
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

/// Growth on the sprout: the plant grows, and its map cell takes the
/// attributes of the grown plant's cell.
static void Event_GrowSprout(void) {
  Map_CopyCellAttributes(25, 9, 1, 1, 4, 9);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
}

/// The leader walks north into a doorway: the door opens, the leader steps
/// through and the party leaves by that exit.
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

/// The same through the wide door of trigger 2.
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

/// The doors, the townsfolk, the treasure and the sprout, before or after
/// Babi's audience.
const struct SceneEvent *Scene_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
    return gSuhallaEventsLater;
  }
  return gSuhallaEvents;
}

/// Sets Suhalla up as the party enters, with the sprout grown if Growth was
/// used on it.
s32 Scene_Initialize(void) {
  struct EventWork *work;

  if (gGameState.entrance == ENTRANCE_90) {
    Engine_GameFlagSet(FLAG_BABI_SPOKE_AFTER_COLOSSO);
  }

  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 24;

  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01) != 0) {
    Event_GrowSprout();
    Engine_ActorSetAnimation(SPROUT, 4);
  }
  return 0;
}

struct SceneEntrance gSuhallaEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 136, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 256, 0, 120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 88, 0, 296, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 216, 0, 280, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 296, 0, 296, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 120, 0, 272, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 40, 0, 176, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSuhallaExits[] = {
    SCENE_EXITS(167),      SCENE_EXIT(1, 168, 1), SCENE_EXIT(2, 168, 2),
    SCENE_EXIT(3, 168, 3), SCENE_EXIT(4, 168, 4), SCENE_EXIT(5, 168, 5),
    SCENE_EXIT(10, 2, 27), SCENE_EXIT(11, 2, 56), SCENE_EXIT(12, 2, 57),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gSuhallaPlacements[] = {
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(104), 0, PIXELS(184), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(184), 0, PIXELS(304), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x8f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(168), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(320), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(112), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(168), 0, PIXELS(208), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(144), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSuhallaPlacementsLater[] = {
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(104), 0, PIXELS(184), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(320), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x8f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(328), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(336), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(168), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(320), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(72), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(168), 0, PIXELS(208), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
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
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaThisIsSuhallaDontTryCrossing) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskIfGoingToLalivero) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgSuhallaWeGondowansWillNeverTrustAnyone) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgSuhallaItHasBeenSaidForAges) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgSuhallaYouCantGetToSouthernGondowan) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheWorkersSayTherereHugeFestivals) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheSandstormsSuddenlyIncreasedIHope) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheSuhallaHasLongBeenKnown) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSuhallaThePeopleOfLaliveroHaveBeen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheRulerOfTolbiTookA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgSuhallaIKnowTheDesertSpiritsWill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgSuhallaSouthernGondowanIsGuardedByThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgSuhallaIveHeardThatTolbisColossoIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgSuhallaNoneOfTheGrownUpsIn) },
    { SEARCH(SEARCH_CHEST), 100, 0xfad, FIND_ITEM(ITEM_HARD_NUT) },
    { SEARCH(SEARCH_JAR), 101, 0xfae, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_JAR), 102, 0xfaf, FIND_COINS(5) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), SPROUT, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Event_GrowSprout) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSuhallaEventsLater[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterWideDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterDoor) },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaWelcomeToSuhallaIBetThe) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheDesertIsEnvelopedInSandstorms) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgSuhallaSoldiersFromTolbiWereCaughtIn) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgSuhallaWeWereBroughtHereByThe) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgSuhallaThisSandstormIsTheFaultOf) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskIfBroughtBySandstorm) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgSuhallaWhyAreTheSandstormsAttackingHumans) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgSuhallaThoseSoldiersKeptMumblingAboutMonsters) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaIBetTheseWarriorsWereBrought) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSuhallaIWonderWhyThereAreSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheSoldiersFromTolbiWereCollapsing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgSuhallaAfterYouGetSwallowedByA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgSuhallaLatelyAllTheSandstormsHaveBeen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgSuhallaIfYouFightTheStormsYoull) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgSuhallaGrownUpsSayTheSandstormsAttack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheySayTheSoldiersWereRavaged) },
    { SEARCH(SEARCH_CHEST), 100, 0xfad, FIND_ITEM(ITEM_HARD_NUT) },
    { SEARCH(SEARCH_JAR), 101, 0xfae, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_JAR), 102, 0xfaf, FIND_COINS(5) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), SPROUT, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Event_GrowSprout) },
    { SCENE_EVENTS_END },
};
// clang-format on

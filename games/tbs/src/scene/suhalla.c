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
#include "sound_ids.h"

// Suhalla, the town at the edge of the Suhalla Desert.

enum {
  FLAG_PROP_MOVED = 0x201,
  // Set on arriving by entrance 90; the townsfolk then stand and talk
  // differently.
  FLAG_SUHALLA_LATER = 0x96f
};

enum { ENTRANCE_90 = 90 };

// Sprite 0xee at 72, 144, which ability 12 moves aside.
enum { PROP = 16 };

// Where the party can enter Suhalla.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSuhallaEntrances;
}

// Suhalla has no regions.
const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

// Where each exit leads.
const u32 *Scene_GetExits(void) { return gSuhallaExits; }

// The townsfolk, earlier or later.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (Engine_GameFlagIsSet(FLAG_SUHALLA_LATER) != 0) {
    return gSuhallaPlacementsLater;
  }
  return gSuhallaPlacements;
}

// A villager asks whether the party is going to Lalivero.
static void AskIfGoingToLalivero(s32 obj) {
  s32 cue = MsgSuhallaTryingGetLalivero;
  Engine_EventSetMessage(cue);
  Engine_EventOpenMessage(obj, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(10);
    Engine_EventSetMessage(cue + 1);
  } else {
    Engine_EventSetMessage(cue + 2);
  }
  Engine_EventShowMessage(obj, 0);
}

// A villager asks whether the sandstorm brought the party to Suhalla.
static void AskIfBroughtBySandstorm(s32 obj) {
  s32 cue = MsgSuhallaBroughtSuhallaSandstorm;
  Engine_EventSetMessage(cue);
  Engine_EventOpenMessage(obj, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(10);
    Engine_EventSetMessage(cue + 1);
  } else {
    Engine_EventSetMessage(cue + 2);
  }
  Engine_EventShowMessage(obj, 0);
}

// Ability 12 on the prop: its cell takes cell 25, 9's attributes.
static void MoveProp(void) {

  Map_CopyCellAttributes(25, 9, 1, 1, 4, 9);
  Engine_GameFlagSet(FLAG_PROP_MOVED);
}

// The leader walks into a doorway: the door opens and the touched exit is
// taken.
static void EnterDoor(void) {
  struct EventWork *work = gEventWork;
  s32 no = work->touchedTrigger;
  u16 x = gSuhallaDoorCells[no][0];
  u16 y = gSuhallaDoorCells[no][1];

  Event_PlayCue(SOUND_DOOR_OPEN);
  Map_AnimateCells(gSuhallaDoorOpenSteps, x, y);
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -16);
  gEventWork->transitionFrames = 16;
  Engine_EventRequestExit(no);
}

// The same through the wide door of trigger 2.
static void EnterWideDoor(void) {
  struct EventWork *work = gEventWork;
  s32 no = work->touchedTrigger;
  u16 x = gSuhallaDoorCells[no][0];
  u16 y = gSuhallaDoorCells[no][1];

  Event_PlayCue(SOUND_DOOR_OPEN);
  Map_AnimateCells(gSuhallaWideDoorOpenSteps, x, y);
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -16);
  gEventWork->transitionFrames = 16;
  Engine_EventRequestExit(no);
}

// The doors, the townsfolk and the prop, earlier or later.
const struct SceneEvent *Scene_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_SUHALLA_LATER) != 0) {
    return gSuhallaEventsLater;
  }
  return gSuhallaEvents;
}

// Sets Suhalla up as the party enters, with the prop where it was moved.
s32 Scene_Initialize(void) {
  struct EventWork *work;

  if (gGameState.entrance == ENTRANCE_90) {
    Engine_GameFlagSet(FLAG_SUHALLA_LATER);
  }

  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 24;

  if (Engine_GameFlagIsSet(FLAG_PROP_MOVED) != 0) {
    MoveProp();
    Engine_ActorSetAnimation(PROP, 4);
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

// The cell each door's opening is drawn at, by trigger.
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
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EnterWideDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaThisIsSuhallaDontTryCrossing) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(AskIfGoingToLalivero) },
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
    { SEARCH(SEARCH_CHEST), 100, 0xfad, FIND_ITEM(0xc2) },
    { SEARCH(SEARCH_JAR), 101, 0xfae, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_JAR), 102, 0xfaf, FIND_COINS(5) },
    { PSYNERGY_ON_ACTOR(0xc), PROP, FLAG_PROP_MOVED, EVENT_SCRIPT(MoveProp) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSuhallaEventsLater[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EnterWideDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EnterDoor) },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaWelcomeToSuhallaIBetThe) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheDesertIsEnvelopedInSandstorms) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgSuhallaSoldiersFromTolbiWereCaughtIn) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgSuhallaWeWereBroughtHereByThe) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgSuhallaThisSandstormIsTheFaultOf) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(AskIfBroughtBySandstorm) },
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
    { SEARCH(SEARCH_CHEST), 100, 0xfad, FIND_ITEM(0xc2) },
    { SEARCH(SEARCH_JAR), 101, 0xfae, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_JAR), 102, 0xfaf, FIND_COINS(5) },
    { PSYNERGY_ON_ACTOR(0xc), PROP, FLAG_PROP_MOVED, EVENT_SCRIPT(MoveProp) },
    { SCENE_EVENTS_END },
};
// clang-format on

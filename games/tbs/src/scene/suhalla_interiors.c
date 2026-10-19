#include "game_state.h"
#include "field_event.h"
#include "unit/import.h"
#include "unit/scene/suhalla_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "suhalla_interiors.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "item_ids.h"
#include "flags.h"
#include "songs.h"

// Suhalla's houses, shop, inn and sanctum. After Babi's audience the inn
// holds two of Tolbi's soldiers, wounded in the sandstorms, and Iodem comes
// to question them about Sheba.

// Arriving by this entrance also marks Babi's audience as held.
enum { ENTRANCE_90 = 90 };

// The actors this scene's code names, by their place in the placement tables.
enum {
  SHOPKEEPER = 12,
  INNKEEPER = 13,
  PRIEST = 18, // keeps the sanctum
  IODEM = 19,
  SOLDIER_SOUTH = 20, // the wounded soldiers in the inn's beds
  SOLDIER_NORTH = 21
};

// Shop and inn ids.
enum { SHOP_SUHALLA = 31, INN_SUHALLA = 10 };

// Where the party can enter the houses.
u8 *Scene_GetEntrances(void) { return gSuhallaInteriorsEntrances; }

// The houses' two regions.
u8 *Scene_GetRegions(void) { return gSuhallaInteriorsRegions; }

// Where each exit leads.
u8 *Scene_GetExits(void) { return gSuhallaInteriorsExits; }

// The townsfolk, joined after Babi's audience by Iodem, the wounded soldiers
// and the travelers the sandstorms brought.
s32 Scene_GetPlacements(void) {
  if (Engine_GameFlagIsSet(FLAG_BABI_AUDIENCE_AFTER_COLOSSO) != 0) {
    return (s32)gSuhallaInteriorsPlacementsAfterAudience;
  }
  return (s32)gSuhallaInteriorsPlacements;
}

// Spoken to facing north, across the counter, the shopkeeper opens the shop;
// otherwise the shopkeeper says Suhalla's is the only store, or, after Babi's
// audience, asks whether the party is surprised to find so few shops.
static void Event_TalkToShopkeeper(s32 actorId) {
  u16 facing = (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
  if (facing == FACING_NORTH) {
    Engine_ShopOpen(SHOP_SUHALLA, actorId);
  } else if (Engine_GameFlagIsSet(FLAG_BABI_AUDIENCE_AFTER_COLOSSO)) {
    s32 question = MsgSuhallaArentSurprisedFind;
    Engine_EventSetMessage(question);
    Engine_EventOpenMessage(actorId, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      Engine_EventWait(10);
      Engine_EventSetMessage(question + 1);
    } else {
      Engine_EventSetMessage(question + 2);
    }
    Engine_EventShowMessage(actorId, 0);
  } else {
    Engine_EventSetMessage(MsgSuhallaOursOnlyStore);
    Engine_EventShowMessage(actorId, 0);
  }
}

// Spoken to facing north, across the counter, the innkeeper offers a room;
// otherwise the innkeeper has nothing to do until Tolbi's festival ends, or,
// after Babi's audience, has been busy since the sandstorms began.
static void Event_TalkToInnkeeper(s32 actorId) {
  u16 facing = (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
  if (facing == FACING_NORTH) {
    Engine_InnCheckIn(INN_SUHALLA, actorId);
  } else if (Engine_GameFlagIsSet(FLAG_BABI_AUDIENCE_AFTER_COLOSSO)) {
    Engine_EventSetMessage(MsgSuhallaBusyEverSince);
    Engine_EventShowMessage(actorId, 0);
  } else {
    Engine_EventSetMessage(MsgSuhallaSighNothinMaybe);
    Engine_EventShowMessage(actorId, 0);
  }
}

// Spoken to facing north, across the counter, the priest offers the
// sanctum's services; otherwise the priest talks of Lalivero under Tolbi's
// rule, or, after Babi's audience, wonders why there are so many sandstorms.
static void Event_TalkToPriest(s32 actorId) {
  u16 facing = (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
  if (facing == FACING_NORTH) {
    Engine_ShopConfirmAct(actorId);
  } else if (Engine_GameFlagIsSet(FLAG_BABI_AUDIENCE_AFTER_COLOSSO)) {
    Engine_EventSetMessage(MsgSuhallaWonderWhySandstorms);
    Engine_EventShowMessage(actorId, 0);
  } else {
    Engine_EventSetMessage(MsgSuhallaGetSickThinkingAboutLalivero);
    Engine_EventShowMessage(actorId, 0);
  }
}

// A traveler asks whether the party was blown here on the way to Babi
// Lighthouse, and answers yes or no.
static void Event_AskIfBlownHere(s32 actorId) {
  s32 question = MsgSuhallaSupposeFolkBlown;
  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(actorId, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(10);
    Engine_EventSetMessage(question + 1);
  } else {
    Engine_EventSetMessage(question + 2);
  }
  Engine_EventShowMessage(actorId, 0);
}

// The party leaves by the touched exit.
static void Event_LeaveByExit(void) {
  struct EventWork *state = gEventWork;
  s16 *touchedTrigger = (s16 *)&state->touchedTrigger;

  Engine_EventRequestExit(*touchedTrigger);
  Engine_AudioPlayCue(SE_MAP_EXIT);
}

// After Babi's audience, on entering the inn: Iodem steps out of the party and
// questions the two wounded soldiers. They were escorting Sheba to Lalivero
// when a sandstorm hit them, and tell of others who stopped the cyclones with
// water and slew the monsters causing them. Iodem resolves to search the
// desert for Sheba, and rejoins the party. Once the party has been to Babi
// Lighthouse (flag 0x98a, set there), Iodem does not come.
static void Event_IodemQuestionsWoundedSoldiers(void) {
  struct FieldActor *leader;
  GameFlag_Set(FLAG_IODEM_MET_WOUNDED_SOLDIERS);
  if (GameFlag_IsSet(0x98a) == 0) {
    Event_PlayCue(MUS_030);
    Event_Begin();
    // Frame the speakers and bring the leader over before the dialogue.
    Camera_MoveTo(PIXELS(368), -1, PIXELS(104), 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 368, 160);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    FieldMotion_LaunchFromFocusedObject(IODEM, 0, -16, FACING_NORTH);
    Actor_WaitForMove(IODEM);
    Camera_WaitForMove();
    Event_SetMessage(MsgSuhallaIodem);
    Event_Wait(10);
    Actor_RunRepeatedMotion(SOLDIER_SOUTH, 2);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(IODEM, 2);
    Event_Wait(20);
    Actor_SetSpeed(IODEM, 78643, 39321);
    Actor_WalkByAndWait(IODEM, 0, -16);
    Actor_FaceDirection(IODEM, FACING_EAST, 0);
    Event_Wait(30);
    Actor_FaceDirection(IODEM, FACING_NORTHEAST, 0);
    Event_Wait(30);
    Actor_FaceDirection(IODEM, FACING_EAST, 0);
    Event_Wait(30);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_SetSpeed(IODEM, 78643, 39321);
    Actor_WalkByAndWait(IODEM, 0, -24);
    Actor_WalkByAndWait(IODEM, 48, 0);
    Actor_FaceDirection(IODEM, FACING_NORTH, 0);
    Event_Wait(30);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(20);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(20);
    Actor_ShowEmote(SOLDIER_SOUTH, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_FaceDirection(IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(SOLDIER_NORTH, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(SOLDIER_NORTH, 4);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_FaceDirection(IODEM, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 7, 40);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(SOLDIER_SOUTH, 2);
    Actor_RunRepeatedMotion(SOLDIER_NORTH, 2);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 78643, 39321);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 368, 104);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 16, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_SetAnimationAndWait(IODEM, 4);
    Event_Wait(20);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(20);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 2, 50);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(SOLDIER_SOUTH, 2);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, SOLDIER_SOUTH, 0);
    Actor_FaceDirection(IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, SOLDIER_NORTH, 0);
    Actor_FaceDirection(IODEM, FACING_NORTH, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(IODEM, 3);
    Event_Wait(30);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(SOLDIER_NORTH, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(SOLDIER_NORTH, 4);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_FaceEachOther(IODEM, ACTOR_PARTY_LEADER, 30);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(SOLDIER_SOUTH, 2);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, SOLDIER_SOUTH, 0);
    Actor_FaceDirection(IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(40);
    Event_Wait(10);
    Actor_RunRepeatedMotion(SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, SOLDIER_NORTH, 0);
    Actor_FaceDirection(IODEM, FACING_NORTH, 0);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(IODEM, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(SOLDIER_SOUTH, 4);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT, 40);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, SOLDIER_SOUTH, 0);
    Actor_FaceDirection(IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(40);
    Event_Wait(10);
    Actor_RunRepeatedMotion(SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, SOLDIER_NORTH, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(SOLDIER_NORTH, 2);
    Event_Wait(30);
    Event_Wait(10);
    Actor_FaceDirection(IODEM, FACING_NORTH, 0);
    Event_Wait(30);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(SOLDIER_NORTH, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(SOLDIER_NORTH, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(SOLDIER_SOUTH, 4);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 1, 50);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, SOLDIER_SOUTH, 0);
    Actor_FaceDirection(IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_RunRepeatedMotion(SOLDIER_SOUTH, 2);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_NORTH, 0);
    Actor_RunRepeatedMotion(SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_Wait(10);
    Actor_FaceEachOther(IODEM, ACTOR_PARTY_LEADER, 0);
    Event_Wait(30);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(SOLDIER_SOUTH, 4);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 2, 80);
    Actor_ShowEmote(SOLDIER_NORTH, EMOTE_IN_FRONT | 2, 50);
    Event_ShowMessage(SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_NORTH, 0);
    Actor_RunRepeatedMotion(SOLDIER_NORTH, 3);
    Event_Wait(20);
    Event_Wait(10);
    Actor_FaceDirection(IODEM, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(IODEM, 2);
    Event_Wait(10);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(SOLDIER_SOUTH, 2);
    Event_Wait(20);
    Event_ShowMessage(SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, SOLDIER_SOUTH, 0);
    Actor_FaceDirection(IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(IODEM, 3);
    Event_Wait(30);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(SOLDIER_SOUTH, 2);
    Actor_RunRepeatedMotion(SOLDIER_NORTH, 2);
    Event_Wait(30);
    Event_Wait(10);
    Actor_FaceEachOther(IODEM, ACTOR_PARTY_LEADER, 20);
    Actor_WalkByAndWait(IODEM, -12, 0);
    Event_Wait(20);
    // Both answers converge on the following line after different waits.
    Event_OpenMessage(IODEM, 0);
    if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_Wait(20);
      Event_ShowMessage(IODEM, 0);
      gEventWork->message++;
    } else {
      Event_Wait(10);
      gEventWork->message++;
      Event_ShowMessage(IODEM, 0);
    }
    Event_Wait(10);
    Actor_RunRepeatedMotion(IODEM, 2);
    Event_Wait(20);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(10);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 2, 50);
    Event_ShowMessage(IODEM, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(IODEM, 3);
    Event_Wait(30);
    Event_PlayCue(MUS_030);
    Actor_SetSpeed(IODEM, 78643, 39321);
    Actor_SetAnimation(IODEM, 2);
    // Iodem walks into the leader and is hidden: he travels in the party.
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetDestination(IODEM, leader->x.part.pixel, leader->z.part.pixel);
    }
    Actor_WaitForMove(IODEM);
    Actor_SetPosition(IODEM, 0, 0);
    Event_Wait(10);
    // Back to the place's own music.
    Audio_PlayCueFromEventWork();
    Event_End();
  }
}

// The doors, the townsfolk and the ovens, before or after Babi's audience.
s32 Scene_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_BABI_AUDIENCE_AFTER_COLOSSO) != 0) {
    return (s32)gSuhallaInteriorsEventsAfterAudience;
  }
  return (s32)gSuhallaInteriorsEvents;
}

// Sets the houses up as the party enters: the shopkeeper and innkeeper behind
// their counters, and the two wounded soldiers lying in their beds.
s32 Scene_Initialize(void) {
  union SceneActor *work;
  if (gGameState.entrance == ENTRANCE_90)
    Engine_GameFlagSet(FLAG_BABI_AUDIENCE_AFTER_COLOSSO);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  gEventWork->transitionFrames = 24;
  ((union SceneActor *)Object_GetById(SHOPKEEPER))->bytes[89] |= 4;
  ((union SceneActor *)Object_GetById(INNKEEPER))->bytes[89] |= 4;
  work = (union SceneActor *)Object_GetById(SOLDIER_SOUTH);
  work->fields.record->field26 = 0;
  work->fields.record->angle = 0x4000;
  {
    struct SceneRecord *record = work->fields.record;
    s32 flags = ~12;

    flags &= record->flags;
    record->flags = flags | 4;
  }
  work = (union SceneActor *)Object_GetById(SOLDIER_NORTH);
  work->fields.record->field26 = 0;
  work->fields.record->angle = 0x4000;
  work->bytes[85] = 2;
  work->fields.y = 0;
  return 0;
}

struct SceneEntrance gSuhallaInteriorsEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 256, 0, 456, FACING_NORTH, 0, 0, 280, 336, 480, 0},
    {2, CONDITION_ALWAYS, 432, 0, 728, FACING_NORTH, 0, 312, 528, 552, 752, 0},
    {3, CONDITION_ALWAYS, 640, 0, 168, FACING_NORTH, 0, 496, 16, 736, 192, 0},
    {4, CONDITION_ALWAYS, 112, 0, 168, FACING_NORTH, 0, 0, 16, 240, 208, 0},
    {5, CONDITION_ALWAYS, 352, 0, 216, FACING_NORTH, 0, 256, 16, 496, 232, 0},
    {6, CONDITION_ALWAYS, 432, 0, 630, FACING_NORTH, 0, 312, 528, 552, 752, 0},
    {7, CONDITION_ALWAYS, 680, 0, 120, FACING_WEST, 0, 496, 16, 736, 192, 0},
    {10, CONDITION_ALWAYS, 104, 0, 360, FACING_NORTH, 0, 0, 280, 336, 480, 0},
    {11, CONDITION_ALWAYS, 152, 0, 360, FACING_NORTH, 0, 0, 280, 336, 480, 0},
    {12, CONDITION_ALWAYS, 472, 0, 344, FACING_SOUTH, 0, 344, 280, 584, 440, 0},
    {13, CONDITION_ALWAYS, 712, 0, 344, FACING_SOUTH, 0, 600, 280, 840, 440, 0},
    {14, CONDITION_ALWAYS, 88, 0, 352, FACING_SOUTH, 0, 0, 280, 336, 480, 0},
    {15, CONDITION_ALWAYS, 88, 0, 584, FACING_NORTH, 0, 16, 528, 256, 688, 0},
    {SCENE_TABLE_END},
};

// clang-format off
s16 gSuhallaInteriorsRegions[] = {
    83, 28, 356, 91, 36, 364, -1, 14,
    83, -20, 596, 91, -12, 604, -1, 15,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSuhallaInteriorsExits[] = {
    SCENE_EXITS(168),
    SCENE_EXIT(1, 167, 1),
    SCENE_EXIT(2, 167, 2),
    SCENE_EXIT(3, 167, 3),
    SCENE_EXIT(4, 167, 4),
    SCENE_EXIT(5, 167, 5),
    SCENE_EXIT(10, 168, 12),
    SCENE_EXIT(11, 168, 13),
    SCENE_EXIT(12, 168, 10),
    SCENE_EXIT(13, 168, 11),
    SCENE_EXIT(14, 168, 15),
    SCENE_EXIT(15, 168, 14),
    SCENE_EXITS_END,
};

struct ScenePlacement gSuhallaInteriorsPlacements[] = {
    { 0x66, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(368), 0, PIXELS(192), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(80), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(120), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(160), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x76, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(568), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x80, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(224), 0, PIXELS(344), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(432), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSuhallaInteriorsPlacementsAfterAudience[] = {
    { 0x66, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(400), 0, PIXELS(184), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(184), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(120), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(160), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x76, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(568), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x80, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(224), 0, PIXELS(344), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(688), 0, PIXELS(384), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(712), 0, PIXELS(408), FACING_NORTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(136), 0, PIXELS(616), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(432), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x39, FLAG_IODEM_MET_WOUNDED_SOLDIERS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSuhallaInteriorsEvents[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsMyFatherLovesPeopleSoMuch) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsLetMeKnowIfYouHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhenISeePeopleInTrouble) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIWantToHelpPeopleSo) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTolbiProbablyIgnoresSmallVillagesLike) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIfYouHeadEastAcrossSuhalla) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsSuhallaIsADesertVillageI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsGoodThingItsNotEasyTo) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToShopkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsAVillageThisSmallCantSupport) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIWonderIfAnyCustomersCan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsOnceTheFestivalEndsTheTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsForSomeReasonAnyoneWhoGets) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheHolyChildActorWillSave) },
    { SEARCH(SEARCH_OVEN), 100, 0xfb0, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_OVEN), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsItsACactusSteakIWonder)) },
    { SEARCH(SEARCH_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsHesMakingMulukhiyaSoupItTastes)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSuhallaInteriorsEventsAfterAudience[] = {
    { EVENT_TOUCH, 50, FLAG_IODEM_MET_WOUNDED_SOLDIERS, EVENT_SCRIPT(Event_IodemQuestionsWoundedSoldiers) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveByExit) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheWoundedAreSleepingSoDont) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIAmAmazedTheseSoldiersSurvived) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhyWereTheyHeadedTowardEastern) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhereIsActorWhatHasHappened) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheseOldSoldiersMustRestUntil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhoCouldHaveDoneThisTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsAColumnOfWaterShotUp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsActorWasNotFarBehindUs) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsDoNotApproachTheLargeCyclones) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsDeathStormsHuhThereWasA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsOnceYouGetSweptUpIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsYeWhoSeekSomethingAboutEyes) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToShopkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWeveHadMoreCustomersLatelyBecause) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsItWillTakeTimeForThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheSandstormsAreSendingCustomersOur) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsPleaseGreatSandstormsSendUsMore) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWeWentIntoTheSuhallaTo) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheSandstormsAreSoBadWeve) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskIfBlownHere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsEvenIfWeCantGoIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsSoShallWeGiveUpOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhenThatCycloneThrewUsInto) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIShudderToThinkSandstormsWill) },
    { SEARCH(SEARCH_OVEN), 100, 0xfb0, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_OVEN), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsItsACactusSteakIWonder)) },
    { SEARCH(SEARCH_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsHesMakingMulukhiyaSoupItTastes)) },
    { SCENE_EVENTS_END },
};
// clang-format on

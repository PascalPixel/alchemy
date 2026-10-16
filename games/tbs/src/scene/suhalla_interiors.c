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

u8 *SuhallaInteriors_GetEntrances(void) { return gSuhallaInteriorsEntrances; }

u8 *SuhallaInteriors_GetRegions(void) { return gSuhallaInteriorsRegions; }

u8 *SuhallaInteriors_GetExits(void) { return gSuhallaInteriorsExits; }

s32 SuhallaInteriors_SelectPlacements(void) {
  if (Engine_GameFlagIsSet(0x96f) != 0) {
    return (s32)gSuhallaInteriorsPlacements96f;
  }
  return (s32)gSuhallaInteriorsPlacements;
}

static void EV12_TALK(s32 no) {
  u16 facing = (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
  if (facing == 0xc000) {
    Engine_ShopOpen(31, no);
  } else if (Engine_GameFlagIsSet(0x96f)) {
    s32 msg = MsgSuhallaArentSurprisedFind;
    Engine_EventSetMessage(msg);
    Engine_EventOpenMessage(no, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      Engine_EventWait(10);
      Engine_EventSetMessage(msg + 1);
    } else {
      Engine_EventSetMessage(msg + 2);
    }
    Engine_EventShowMessage(no, 0);
  } else {
    Engine_EventSetMessage(MsgSuhallaOursOnlyStore);
    Engine_EventShowMessage(no, 0);
  }
}

static void EV13_TALK(s32 no) {
  u16 facing = (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
  if (facing == 0xc000) {
    Engine_InnCheckIn(10, no);
  } else if (Engine_GameFlagIsSet(0x96f)) {
    Engine_EventSetMessage(MsgSuhallaBusyEverSince);
    Engine_EventShowMessage(no, 0);
  } else {
    Engine_EventSetMessage(MsgSuhallaSighNothinMaybe);
    Engine_EventShowMessage(no, 0);
  }
}

static void EV18_TALK(s32 no) {
  u16 facing = (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
  if (facing == 0xc000) {
    Engine_ShopConfirmAct(no);
  } else if (Engine_GameFlagIsSet(0x96f)) {
    Engine_EventSetMessage(MsgSuhallaWonderWhySandstorms);
    Engine_EventShowMessage(no, 0);
  } else {
    Engine_EventSetMessage(MsgSuhallaGetSickThinkingAboutLalivero);
    Engine_EventShowMessage(no, 0);
  }
}

static void EV17_TALK(s32 obj) {
  s32 msg = MsgSuhallaSupposeFolkBlown;
  Engine_EventSetMessage(msg);
  Engine_EventOpenMessage(obj, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(10);
    Engine_EventSetMessage(msg + 1);
  } else {
    Engine_EventSetMessage(msg + 2);
  }
  Engine_EventShowMessage(obj, 0);
}

static void SuhallaInteriors_PlayCueAndRequestTouchedExit(void) {
  struct EventWork *state = gEventWork;
  s16 *cnt = (s16 *)&state->touchedTrigger;

  Engine_EventRequestExit(*cnt);
  Engine_AudioPlayCue(0x7B);
}

static void EV50_TOUCH(void) {
  struct FieldActor *record;
  GameFlag_Set(2480);
  if (GameFlag_IsSet(2442) == 0) {
    Event_PlayCue(30);
    Event_Begin();
    Camera_MoveTo(24117248, -1, 6815744, 1);
    Actor_WalkToAndWait(0, 368, 160);
    Actor_FaceDirection(0, FACING_NORTH, 0);
    FieldMotion_LaunchFromFocusedObject(19, 0, -16, FACING_NORTH);
    Actor_WaitForMove(19);
    Camera_WaitForMove();
    Event_SetMessage(MsgSuhallaIodem);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(19, 2);
    Event_Wait(20);
    Actor_SetSpeed(19, 78643, 39321);
    Actor_WalkByAndWait(19, 0, -16);
    Actor_FaceDirection(19, FACING_EAST, 0);
    Event_Wait(30);
    Actor_FaceDirection(19, FACING_NORTHEAST, 0);
    Event_Wait(30);
    Actor_FaceDirection(19, FACING_EAST, 0);
    Event_Wait(30);
    Actor_ShowEmote(19, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(19, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_SetSpeed(19, 78643, 39321);
    Actor_WalkByAndWait(19, 0, -24);
    Actor_WalkByAndWait(19, 48, 0);
    Actor_FaceDirection(19, FACING_NORTH, 0);
    Event_Wait(30);
    Event_ShowMessage(19, 0);
    Event_Wait(20);
    Actor_ShowEmote(19, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(19, 0);
    Event_Wait(20);
    Actor_ShowEmote(20, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceDirection(19, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Event_ShowMessage(19, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(21, 4);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_FaceDirection(19, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_ShowEmote(19, EMOTE_IN_FRONT | 7, 40);
    Event_ShowMessage(19, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(20, 2);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Actor_SetSpeed(0, 78643, 39321);
    Actor_WalkToAndWait(0, 368, 104);
    Actor_WalkByAndWait(0, 16, 0);
    Actor_FaceDirection(0, FACING_EAST, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_SetAnimationAndWait(19, 4);
    Event_Wait(20);
    Event_ShowMessage(19, 0);
    Event_Wait(20);
    Actor_ShowEmote(19, EMOTE_IN_FRONT | 2, 50);
    Event_ShowMessage(19, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceActor(0, 20, 0);
    Actor_FaceDirection(19, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Event_ShowMessage(19, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_FaceActor(0, 21, 0);
    Actor_FaceDirection(19, FACING_NORTH, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(19, 3);
    Event_Wait(30);
    Event_ShowMessage(19, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(21, 4);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_FaceEachOther(19, 0, 30);
    Event_ShowMessage(19, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceActor(0, 20, 0);
    Actor_FaceDirection(19, FACING_SOUTHEAST, 0);
    Event_Wait(40);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_FaceActor(0, 21, 0);
    Actor_FaceDirection(19, FACING_NORTH, 0);
    Event_Wait(40);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(19, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(0, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(19, EMOTE_IN_FRONT, 40);
    Event_Wait(10);
    Actor_FaceActor(0, 20, 0);
    Actor_FaceDirection(19, FACING_SOUTHEAST, 0);
    Event_Wait(40);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_ShowEmote(0, EMOTE_IN_FRONT, 50);
    Actor_FaceActor(0, 21, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(30);
    Event_Wait(10);
    Actor_FaceDirection(19, FACING_NORTH, 0);
    Event_Wait(30);
    Event_ShowMessage(19, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(21, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(19, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(19, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(19, EMOTE_IN_FRONT | 1, 50);
    Event_Wait(10);
    Actor_FaceActor(0, 20, 0);
    Actor_FaceDirection(19, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(19, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(19, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_Wait(10);
    Actor_FaceEachOther(19, 0, 0);
    Event_Wait(30);
    Event_ShowMessage(19, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 4);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_ShowEmote(0, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(19, EMOTE_IN_FRONT | 2, 80);
    Actor_ShowEmote(21, EMOTE_IN_FRONT | 2, 50);
    Event_ShowMessage(21, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessage(21, 0);
    Actor_RunRepeatedMotion(21, 3);
    Event_Wait(20);
    Event_Wait(10);
    Actor_FaceDirection(19, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(19, 2);
    Event_Wait(10);
    Event_ShowMessage(19, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(20, 2);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceActor(0, 20, 0);
    Actor_FaceDirection(19, FACING_SOUTHEAST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(19, 3);
    Event_Wait(30);
    Event_ShowMessage(19, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(20, 2);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(30);
    Event_Wait(10);
    Actor_FaceEachOther(19, 0, 20);
    Actor_WalkByAndWait(19, -12, 0);
    Event_Wait(20);
    Event_OpenMessage(19, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
      Event_Wait(20);
      Event_ShowMessage(19, 0);
      gEventWork->message++;
    } else {
      Event_Wait(10);
      gEventWork->message++;
      Event_ShowMessage(19, 0);
    }
    Event_Wait(10);
    Actor_RunRepeatedMotion(19, 2);
    Event_Wait(20);
    Event_ShowMessage(19, 0);
    Event_Wait(10);
    Actor_ShowEmote(19, EMOTE_IN_FRONT | 2, 50);
    Event_ShowMessage(19, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(19, 3);
    Event_Wait(30);
    Event_PlayCue(30);
    Actor_SetSpeed(19, 78643, 39321);
    Actor_SetAnimation(19, 2);
    record = Actor_Get(0);
    if (record != 0) {
      Actor_SetDestination(19, record->x.part.pixel, record->z.part.pixel);
    }
    Actor_WaitForMove(19);
    Actor_SetPosition(19, 0, 0);
    Event_Wait(10);
    Audio_PlayCueFromEventWork();
    Event_End();
  }
}

s32 SuhallaInteriors_SelectEvents(void) {
  if (Engine_GameFlagIsSet(0x96F) != 0) {
    return (s32)gSuhallaInteriorsEvents96f;
  }
  return (s32)gSuhallaInteriorsEvents;
}

s32 Scene_InitActorRecords(void) {
  union SceneActor *work;
  if (gGameState.entrance == 90)
    Engine_GameFlagSet(0x96f);
  gEventWork->startTransition = 521;
  gEventWork->transitionFrames = 24;
  ((union SceneActor *)Object_GetById(12))->bytes[89] |= 4;
  ((union SceneActor *)Object_GetById(13))->bytes[89] |= 4;
  work = (union SceneActor *)Object_GetById(20);
  work->fields.record->field26 = 0;
  work->fields.record->angle = 0x4000;
  {
    struct SceneRecord *record = work->fields.record;
    s32 flags = ~12;

    flags &= record->flags;
    record->flags = flags | 4;
  }
  work = (union SceneActor *)Object_GetById(21);
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

struct ScenePlacement gSuhallaInteriorsPlacements96f[] = {
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
    { 0x39, 0x9b0, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSuhallaInteriorsEvents[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsMyFatherLovesPeopleSoMuch) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsLetMeKnowIfYouHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhenISeePeopleInTrouble) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIWantToHelpPeopleSo) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTolbiProbablyIgnoresSmallVillagesLike) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIfYouHeadEastAcrossSuhalla) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsSuhallaIsADesertVillageI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsGoodThingItsNotEasyTo) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsAVillageThisSmallCantSupport) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIWonderIfAnyCustomersCan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsOnceTheFestivalEndsTheTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsForSomeReasonAnyoneWhoGets) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TALK) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheHolyChildActorWillSave) },
    { SEARCH(SEARCH_OVEN), 100, 0xfb0, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_OVEN), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsItsACactusSteakIWonder)) },
    { SEARCH(SEARCH_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsHesMakingMulukhiyaSoupItTastes)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSuhallaInteriorsEvents96f[] = {
    { EVENT_TOUCH, 50, 0x9b0, EVENT_SCRIPT(EV50_TOUCH) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(SuhallaInteriors_PlayCueAndRequestTouchedExit) },
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
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWeveHadMoreCustomersLatelyBecause) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsItWillTakeTimeForThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheSandstormsAreSendingCustomersOur) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsPleaseGreatSandstormsSendUsMore) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWeWentIntoTheSuhallaTo) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheSandstormsAreSoBadWeve) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_TALK) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsEvenIfWeCantGoIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsSoShallWeGiveUpOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhenThatCycloneThrewUsInto) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TALK) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIShudderToThinkSandstormsWill) },
    { SEARCH(SEARCH_OVEN), 100, 0xfb0, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_OVEN), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsItsACactusSteakIWonder)) },
    { SEARCH(SEARCH_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsHesMakingMulukhiyaSoupItTastes)) },
    { SCENE_EVENTS_END },
};
// clang-format on

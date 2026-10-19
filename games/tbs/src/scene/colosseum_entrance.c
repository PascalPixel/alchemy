#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/colosseum_entrance.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
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
#include "field_scene.h"
#include "text/msg_ids.h"
#include "field_obj.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"

/// Forward declarations.
static void ColosseumEntrance_ApplyArrivalState(void);
static void ColosseumEntrance_ShowMessageAndPause(s32 speaker);
static void ColosseumEntrance_FaceDirection(s32 actor, s32 facing);
static void ColosseumEntrance_UpdateEntranceCells(void);
static void ColosseumEntrance_ExplainPartySupport(void);
static void ColosseumEntrance_EscortToEastExit(void);
static void ColosseumEntrance_AnnounceFinals(void);
static void ColosseumEntrance_WalkToEastExit(void);
static void ColosseumEntrance_CelebrateAdvancement(void);
static void ColosseumEntrance_EscortToWestExit(void);
static void ColosseumEntrance_WalkToWestExit(void);
static void ColosseumEntrance_CelebrateBothWins(void);
static void ColosseumEntrance_InitializeContestants(void);

/// Turn over ten frames by calling ColosseumEntrance_FaceDirection
/// through the inline helper.
static inline void ColosseumEntrance_Turn(s32 actor, s32 facing) {
  /* FAKEMATCH: forwarding inline for Event_CrowdRecognizesIsaac's
   * one call (call.h's Call2 did the same); called directly
   * Event_CrowdRecognizesIsaac's later message skip swaps r2 and r3
   * (7 halfwords differ in tbs-en, 2026-10-13). */
  ColosseumEntrance_FaceDirection(actor, facing);
}

/// The attendant by the trials (ColosseumEntrance1, entrances 5 and 69).
enum { TRIAL_ATTENDANT = 17 };

/// The finals' waiting room (ColosseumEntrance1, entrance 12): an official and
/// the seven warriors who cleared the trials, as they introduce themselves.
enum {
  FINALS_OFFICIAL = 8,
  MORGAN = 12, ///< the highest-ranked
  AZART = 13,  ///< the second-best
  GALAHAD = 14,
  SATRAGE = 15,
  DEKKA = 16,
  NAVAMPA = 17,
  BUFORD = 18
};

/// Select the entrance table for this colosseum area.
const struct SceneEntrance *ColosseumEntrance_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_ColosseumEntrance1) {
    return gColosseumEntranceEntrances1;
  }
  if (scene == (s32)&SceneId_ColosseumEntrance3) {
    return gColosseumEntranceEntrances3;
  }
  return gColosseumEntranceEntrancesOther;
}

/// This scene has no additional region table.
const struct SceneRegion *ColosseumEntrance_GetRegions(void) { return 0; }

/// Return the shared colosseum exits.
const u32 *ColosseumEntrance_GetExits(void) { return gColosseumEntranceExits; }

/// Select actors for the current area and stage of Colosso.
const struct ScenePlacement *ColosseumEntrance_GetPlacements(void) {
  const struct ScenePlacement *table;

  if (gGameState.scene == (s32)&SceneId_ColosseumEntrance1) {
    switch (gGameState.entrance) {
    case 5:
    case 69:
      table = gColosseumEntrancePlacements1Entrance5;
      break;
    case 7:
    case 70:
      table = gColosseumEntrancePlacements1Entrance7;
      break;
    case 8:
    case 21:
    case 31:
    case 64:
    case 65:
    case 67:
      table = gColosseumEntrancePlacements1Entrance8;
      break;
    case 12:
      table = gColosseumEntrancePlacements1Entrance12;
      break;
    case 66:
    case 68:
      table = gColosseumEntrancePlacements1Entrance66;
      break;
    default:
      table = gColosseumEntrancePlacements1;
      break;
    }
  } else if (gGameState.scene == (s32)&SceneId_ColosseumEntrance3) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      table = gColosseumEntrancePlacements3AfterColosso;
    } else if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      table = gColosseumEntrancePlacements3FinalsDay;
    } else {
      table = gColosseumEntrancePlacements3;
    }
  } else {
    table = gColosseumEntrancePlacementsOther;
  }
  return table;
}

/// Select interactions for this area and arrival.
const struct SceneEvent *ColosseumEntrance_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_ColosseumEntrance2) {
    return gColosseumEntranceEvents2;
  }
  if (scene == (s32)&SceneId_ColosseumEntrance1) {
    if (gGameState.entrance == 12) {
      return gColosseumEntranceEvents1Entrance12;
    }
    return gColosseumEntranceEvents1;
  }
  if (scene == (s32)&SceneId_ColosseumEntrance3) {
    return gColosseumEntranceEvents3;
  }
  return gColosseumEntranceEventsOther;
}

/// Outside the gates: during the trials, a man asks whether the party hopes to
/// get in to see Colosso; on the finals day the colosseum is already full.
static void Event_DiscussSeating(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED)) {
    Engine_EventSetMessage(MsgColosseumColosseumAlreadyFullIfReally);
    Engine_EventShowMessage(10, 0);
  } else {
    Engine_EventSetMessage(MsgColosseumHopingGetInSeeColosso);
    Engine_EventAskYesNo(10, 0);
  }
  Engine_EventEnd();
}

/// Outside the gates: the Lucky Guess makes the battles better; on the finals
/// day, a good spot costs too much.
static void Event_DiscussLuckyGuessSeats(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED)) {
    Engine_ActorShowEmote(13, 258, 40);
    Engine_EventSetMessage(MsgColosseumWantWatchFinalsFromGood);
    Engine_EventShowMessage(13, 0);
  } else {
    Engine_EventSetMessage(MsgColosseumColossosLuckyGuessMakesBattles);
    Engine_EventShowMessage(13, 0);
  }
  Engine_EventEnd();
}

/// Outside the gates: a bettor put it all on a warrior no one picked; on the
/// finals day a scalper, busy with a customer, asks if the party wants seats.
static void Event_DiscussLuckyGuessWinnings(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventSetMessage(MsgColosseumHoldOnSecond);
    ColosseumEntrance_ShowMessageAndPause(14);
    Engine_ActorFaceEachOther(14, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_EventAskYesNo(14, 0);
    ColosseumEntrance_FaceDirection(14, FACING_EAST);
  } else {
    Engine_EventSetMessage(MsgColosseumWantWinBigPutOn);
    Engine_EventShowMessage(14, 0);
  }
  Engine_EventEnd();
}

/// The Lucky Guess man explains the game; on the finals day he recognizes Isaac
/// and refuses him, as finalists may not play (0x3c0: said once).
static void Event_OfferLuckyGuess(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_BABI_SAVED) != 0) {
    if (GameFlag_IsSet(0x3c0) != 0) {
      Engine_EventSetMessage(MsgColosseumLookImSorryButWe);
      Event_ShowMessage(16, 0);
    } else {
      Engine_EventSetMessage(MsgColosseumWantTryOutLuckyGuess);
      Event_OpenMessage(16, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        gEventWork->message++;
        Actor_ShowEmote(16, 0x100, 40);
        Event_OpenMessage(16, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
          gEventWork->message++;
        }
        Engine_EventWait(40);
        Event_ShowMessage(16, 0);
        GameFlag_Set(0x3c0);
      } else {
        Event_ShowMessage(16, 0);
      }
    }
  } else {
    Engine_EventSetMessage(MsgColosseumDoKnowLuckyGuess);
    Event_AskYesNo(16, 0);
  }
  Engine_EventEnd();
}

/// Azart, the second-best, will not lose to someone who skipped the trials;
/// then he goes back to pacing.
static void Event_TalkToAzart(void) {
  struct FieldActor *contestant;

  contestant = Object_GetById(AZART);
  Engine_EventBegin();
  Engine_ActorStop(AZART);
  Engine_ActorFaceEachOther(AZART, ACTOR_PARTY_LEADER, 20);
  Engine_EventSetMessage(MsgColosseumImRatedAsSecondBest);
  ColosseumEntrance_ShowMessageAndPause(AZART);
  Engine_ActorRunRepeatedMotion(AZART, 1);
  Engine_EventShowMessage(AZART, 0);
  {
    u16 *timer = (u16 *)&contestant->timer;
    u16 duration = 720;

    *timer = duration;
  }
  {
    u16 *work = &contestant->work;
    u16 value = 112;

    *work = value;
  }
  Actor_SetBuiltinAction(AZART, 2);
  Engine_EventEnd();
}

/// Galahad, the third seed, jumps. "Waah! Don't frighten me!"
static void Event_TalkToGalahad(void) {
  Engine_EventBegin();
  Actor_SetAttachedEffect(GALAHAD, 0x102);
  Engine_ActorRunRepeatedMotion(GALAHAD, 2);
  Engine_EventSetMessage(MsgColosseumWaahDontFrightenMe);
  ColosseumEntrance_ShowMessageAndPause(GALAHAD);
  Actor_ShowEmote(GALAHAD, 0x102, 40);
  Event_ShowMessage(GALAHAD, 0);
  Engine_EventEnd();
}

/// Satrage, the fourth-ranked, opens his eyes and names himself.
static void Event_TalkToSatrage(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgColosseumWhoHaveComeQuestionMe);
  ColosseumEntrance_ShowMessageAndPause(SATRAGE);
  Engine_ActorFaceEachOther(SATRAGE, ACTOR_PARTY_LEADER, 20);
  ColosseumEntrance_ShowMessageAndPause(SATRAGE);
  Engine_ActorSetAnimationAndWait(SATRAGE, 3);
  Engine_ActorSetAnimation(SATRAGE, 0);
  ColosseumEntrance_ShowMessageAndPause(SATRAGE);
  ColosseumEntrance_FaceDirection(SATRAGE, FACING_SOUTH + FACING_STEP);
  Engine_EventEnd();
}

/// Dekka psyches himself up; the first time, Navampa explains he will not
/// answer (0x3c1).
static void Event_TalkToDekka(void) {
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(DEKKA, 2);
  Engine_EventSetMessage(MsgColosseumDekkaMustWinFinalsDekka);
  Event_ShowMessageAndWait(DEKKA, 0, 20);
  if (GameFlag_IsSet(0x3c1) != 0) {
    Engine_EventWait(20);
  } else {
    ColosseumEntrance_FaceDirection(NAVAMPA, FACING_EAST);
    Engine_ActorRunRepeatedMotion(NAVAMPA, 1);
    ColosseumEntrance_ShowMessageAndPause(NAVAMPA);
    Engine_ActorFaceEachOther(NAVAMPA, ACTOR_PARTY_LEADER, 20);
    Engine_ActorSetAnimation(NAVAMPA, 4);
    ColosseumEntrance_ShowMessageAndPause(NAVAMPA);
    Actor_ShowEmote(NAVAMPA, 0x105, 40);
    ColosseumEntrance_ShowMessageAndPause(NAVAMPA);
    ColosseumEntrance_FaceDirection(NAVAMPA, FACING_SOUTH + FACING_STEP);
    GameFlag_Set(0x3c1);
  }
  Engine_EventEnd();
}

/// Navampa of Gondowan, the sixth-ranked, introduces himself.
static void Event_TalkToNavampa(void) {
  Engine_EventBegin();
  Engine_ActorFaceEachOther(NAVAMPA, ACTOR_PARTY_LEADER, 20);
  Engine_EventSetMessage(MsgColosseumAmNavampaGondowanSixthRanked);
  ColosseumEntrance_ShowMessageAndPause(NAVAMPA);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(NAVAMPA, 3);
  ColosseumEntrance_ShowMessageAndPause(NAVAMPA);
  Engine_ActorRunRepeatedMotion(NAVAMPA, 1);
  ColosseumEntrance_ShowMessageAndPause(NAVAMPA);
  ColosseumEntrance_FaceDirection(NAVAMPA, FACING_SOUTH + FACING_STEP);
  Engine_EventEnd();
}

/// Buford, the seventh seed: anyone could win today.
static void Event_TalkToBuford(void) {
  Engine_EventBegin();
  Engine_ActorFaceEachOther(BUFORD, ACTOR_PARTY_LEADER, 20);
  Engine_EventSetMessage(MsgColosseumImBufordSeventhSeed);
  ColosseumEntrance_ShowMessageAndPause(BUFORD);
  Actor_FaceDirection(BUFORD, FACING_NORTH + FACING_STEP, 20);
  Actor_FaceDirection(BUFORD, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_FaceDirection(BUFORD, FACING_WEST, 40);
  Engine_ActorFaceEachOther(BUFORD, ACTOR_PARTY_LEADER, 20);
  ColosseumEntrance_ShowMessageAndPause(BUFORD);
  Engine_ActorSetAnimationAndWait(BUFORD, 3);
  ColosseumEntrance_ShowMessageAndPause(BUFORD);
  ColosseumEntrance_FaceDirection(BUFORD, FACING_SOUTH + FACING_STEP);
  Engine_EventEnd();
}

/// The official asks whether Isaac is ready for the finals.
static void Event_AskIfReadyForFinals(void) {
  Engine_EventBegin();
  Engine_ActorFaceEachOther(FINALS_OFFICIAL, ACTOR_PARTY_LEADER, 20);
  Engine_EventSetMessage(MsgColosseumYouReadyForFinals);
  Engine_EventOpenMessage(FINALS_OFFICIAL, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0)
    gEventWork->message++;
  Engine_EventShowMessage(FINALS_OFFICIAL, 0);
  Engine_EventEnd();
}

/// Outside the gates on the finals day, the crowd recognizes Isaac as Babi's
/// pick, and a guide takes the party in; if Isaac will not fight, his
/// companions talk him round.
static void Event_CrowdRecognizesIsaac(void) {
  // The native sequence reuses this word for actor handles and the reply.
  s32 actorOrAnswer;

  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) == 0) {
    return;
  }

  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(17, 1);
  Actor_FaceDirection(17, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ActorFaceDirection(17, FACING_EAST, 60);
  Actor_ShowEmote(17, 0x100, 40);
  ColosseumEntrance_FaceDirection(17, FACING_SOUTHEAST + FACING_STEP);
  Engine_EventSetMessage(MsgColosseumHeyYouRe);
  Engine_ActorStartRepeatedMotion(17, 2);
  ColosseumEntrance_ShowMessageAndPause(17);
  Engine_ActorFaceDirection(18, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorFaceDirection(19, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorFaceDirection(20, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventGetViewCenter()->motionFlags = 0;
  Engine_CameraSetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Camera_MoveTo(PIXELS(256), -1, PIXELS(172), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_ShowEmote(18, 0x102, 40);
  ColosseumEntrance_ShowMessageAndPause(18);
  Engine_ActorSetAnimationAndWait(17, 3);
  ColosseumEntrance_ShowMessageAndPause(17);
  Actor_SetAttachedEffect(19, 0x102);
  Engine_EventWait(40);
  ColosseumEntrance_FaceDirection(19, FACING_EAST);
  ColosseumEntrance_ShowMessageAndPause(19);
  Actor_ShowEmote(20, 0x103, 40);
  ColosseumEntrance_FaceDirection(20, FACING_EAST);
  Engine_ActorStartRepeatedMotion(20, 2);
  Engine_EventShowMessageAndWait(20, 0, 20);
  Engine_ActorRunRepeatedMotion(17, 1);
  Engine_EventWait(10);
  Actor_FaceDirection(17, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_EventShowMessageAndWait(17, 0, 20);
  Engine_ActorRunRepeatedMotion(18, 1);
  Engine_ActorSetAnimationAndWait(18, 4);
  Engine_EventWait(20);
  // Gather Isaac and his companions for the invitation.
  ColosseumEntrance_FaceDirection(17, FACING_WEST);
  Engine_EventShowMessageAndWait(17, 0, 20);
  Engine_ActorRunRepeatedMotion(19, 1);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(19, 4);
  Engine_EventShowMessageAndWait(17, 0, 20);
  Engine_ActorRunRepeatedMotion(20, 2);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(20, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(17, 3);
  Engine_ActorSetAnimation(18, 3);
  Engine_ActorSetAnimation(19, 3);
  Engine_ActorSetAnimationAndWait(20, 3);
  Engine_ActorFaceDirection(17, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorFaceDirection(18, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorFaceDirection(19, FACING_SOUTHEAST + FACING_STEP, 0);
  ColosseumEntrance_FaceDirection(20, FACING_SOUTHEAST + FACING_STEP);
  Actor_SetSpeed(17, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(17, 258, 172);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 262, 188);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  actorOrAnswer = (s32)Object_GetById(ACTOR_PARTY_LEADER);
  if (actorOrAnswer != 0) {
    Engine_ActorSetPosition(GARET,
                            ((struct FieldActor *)actorOrAnswer)->x.fixed,
                            ((struct FieldActor *)actorOrAnswer)->z.fixed);
  }
  actorOrAnswer = (s32)Object_GetById(ACTOR_PARTY_LEADER);
  if (actorOrAnswer != 0) {
    Engine_ActorSetPosition(IVAN, ((struct FieldActor *)actorOrAnswer)->x.fixed,
                            ((struct FieldActor *)actorOrAnswer)->z.fixed);
  }
  actorOrAnswer = (s32)Object_GetById(ACTOR_PARTY_LEADER);
  if (actorOrAnswer != 0) {
    Engine_ActorSetPosition(MIA, ((struct FieldActor *)actorOrAnswer)->x.fixed,
                            ((struct FieldActor *)actorOrAnswer)->z.fixed);
  }
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Engine_ActorWalkTo(GARET, 246, 200);
  Actor_WalkTo(IVAN, 262, 200);
  Actor_WalkToAndWait(MIA, 278, 200);
  Engine_ActorSetAnimation(IVAN, 1);
  Engine_ActorSetAnimation(GARET, 1);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 40);
  Engine_ActorRunRepeatedMotion(17, 1);
  ColosseumEntrance_ShowMessageAndPause(17);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_ActorSetAnimationAndWait(17, 3);
  Event_OpenMessage(17, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);

  // Either leave with the guide or let the companions persuade Isaac.
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(17, 3);
    ColosseumEntrance_ShowMessageAndPause(17);
    Actor_SetActionScript(GARET,
                          gColosseumEntrance_GatherAtSpectatorLeaderScript);
    Actor_SetActionScript(IVAN,
                          gColosseumEntrance_GatherAtSpectatorLeaderScript);
    Actor_RunActionScriptAndWait(
        MIA, gColosseumEntrance_GatherAtSpectatorLeaderScript);
    Engine_CameraSetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
    Engine_CameraMoveTo(PIXELS(256), -1, PIXELS(100), 1);
    Actor_SetSpeed(17, PIXELS(1), PIXELS(1) / 2);
    Actor_SetActionScript(17, gColosseumEntrance_LeaveSpectatorAreaScript);
    Engine_EventWait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_SetActionScript(ACTOR_PARTY_LEADER,
                          gColosseumEntrance_LeaveSpectatorAreaScript);
    Engine_EventWait(80);
    gEventWork->transitionFrames = 40;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
  } else {
    Event_SkipMessages(1);
    Actor_ShowEmote(17, 0x102, 40);
    ColosseumEntrance_ShowMessageAndPause(17);
    Actor_ShowEmote(18, 0x102, 40);
    ColosseumEntrance_ShowMessageAndPause(18);
    Actor_ShowEmote(19, 0x102, 40);
    ColosseumEntrance_ShowMessageAndPause(19);
    Actor_ShowEmote(20, 0x102, 40);
    ColosseumEntrance_ShowMessageAndPause(19);
    Engine_ActorSetAnimationAndWait(17, 4);
    ColosseumEntrance_ShowMessageAndPause(17);
    Engine_ActorRunRepeatedMotion(IVAN, 1);
    Engine_EventWait(20);
    ColosseumEntrance_ShowMessageAndPause(IVAN);
    Actor_ShowEmote(MIA, 0x102, 40);
    ColosseumEntrance_ShowMessageAndPause(MIA);
    Actor_FaceDirection(17, FACING_NORTHWEST + FACING_STEP, 0);
    Engine_ActorFaceDirection(19, FACING_WEST, 0);
    Engine_ActorFaceDirection(20, FACING_EAST, 60);
    Engine_ActorFaceDirection(17, FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_ActorFaceDirection(19, FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_ActorFaceDirection(20, FACING_SOUTHEAST + FACING_STEP, 20);
    Engine_ActorSetAnimation(17, 3);
    Engine_ActorSetAnimation(18, 3);
    Engine_ActorSetAnimation(19, 3);
    Engine_ActorSetAnimationAndWait(20, 3);
    Actor_ShowEmote(MIA, 0x100, 60);
    ColosseumEntrance_Turn(MIA, FACING_NORTHWEST);
    ColosseumEntrance_ShowMessageAndPause(MIA);
    Engine_ActorSetAnimation(GARET, 4);
    Engine_EventWait(20);
    Event_OpenMessage(GARET, 0);

    actorOrAnswer = Engine_EventChooseYesNo(0, 0);
    if (actorOrAnswer == 0) {
      Engine_ActorSetAnimationAndWait(17, 3);
      ColosseumEntrance_ShowMessageAndPause(17);
      Actor_SetActionScript(GARET,
                            gColosseumEntrance_GatherAtSpectatorLeaderScript);
      Actor_SetActionScript(IVAN,
                            gColosseumEntrance_GatherAtSpectatorLeaderScript);
      Actor_RunActionScriptAndWait(
          MIA, gColosseumEntrance_GatherAtSpectatorLeaderScript);
      Engine_CameraSetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
      Engine_CameraMoveTo(PIXELS(256), -1, PIXELS(100), 1);
      Actor_SetSpeed(17, PIXELS(1), PIXELS(1) / 2);
      Actor_SetActionScript(17, gColosseumEntrance_LeaveSpectatorAreaScript);
      Engine_EventWait(10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      Actor_SetActionScript(ACTOR_PARTY_LEADER,
                            gColosseumEntrance_LeaveSpectatorAreaScript);
      Engine_EventWait(80);
      gEventWork->transitionFrames = 40;
      Engine_EventCloseScreen();
      Engine_EventWaitForScreen();
    } else {
      Event_SkipMessages(1);
      Engine_EventWait(20);
      Engine_ActorStartRepeatedMotion(GARET, 2);
      Engine_EventShowMessageAndWait(GARET, 0, 20);
      Engine_ActorShowEmote(IVAN, 0x102, 60);
      ColosseumEntrance_ShowMessageAndPause(IVAN);
      ColosseumEntrance_FaceDirection(MIA, FACING_WEST);
      Engine_ActorSetAnimation(MIA, 3);
      ColosseumEntrance_ShowMessageAndPause(MIA);
      Actor_SetActionScript(IVAN,
                            gColosseumEntrance_GatherAtSpectatorPartyScript);
      Actor_RunActionScriptAndWait(
          MIA, gColosseumEntrance_GatherAtSpectatorPartyScript);
      Engine_EventWait(20);
      Actor_RunActionScriptAndWait(
          0, gColosseumEntrance_GatherAtSpectatorPartyScript);
      Actor_WalkToAndWait(GARET, 262, 188);
      ColosseumEntrance_FaceDirection(GARET, FACING_NORTH);
      Engine_ActorSetAnimationAndWait(GARET, 3);
      ColosseumEntrance_ShowMessageAndPause(GARET);
      Engine_CameraSetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
      Engine_CameraMoveTo(PIXELS(256), -1, PIXELS(100), 1);
      Actor_SetSpeed(17, PIXELS(1), PIXELS(1) / 2);
      Actor_SetActionScript(17, gColosseumEntrance_LeaveSpectatorAreaScript);
      Engine_EventWait(10);
      Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
      Actor_SetActionScript(GARET, gColosseumEntrance_LeaveSpectatorAreaScript);
      Engine_EventWait(80);
      gEventWork->transitionFrames = 40;
      Engine_EventCloseScreen();
      Engine_EventWaitForScreen();
    }
  }

  Engine_EventRequestExit(2);
  Engine_GameFlagSet(0x93f);
  Engine_EventEnd();
}

/// Stepping forward in the waiting room: asked whether he is ready, Isaac can
/// still wait; if he is, the warriors grumble, line up by rank and march out to
/// the finals.
static void Event_FinalsBegin(void) {
  struct FieldActor *actor;
  const s32 *script;

  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_34) != 0) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_35);
    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 872, 120);
    ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST);
    Camera_SetSpeed(PIXELS(FINALS_OFFICIAL) / 5, PIXELS(1) / 5);
    Camera_MoveTo(PIXELS(816), -1, PIXELS(112), 1);
    Engine_CameraWaitForMove();
    Actor_FaceDirection(FINALS_OFFICIAL, FACING_SOUTHEAST + FACING_STEP, 20);
    Engine_ActorRunRepeatedMotion(FINALS_OFFICIAL, 1);
    Engine_EventSetMessage(MsgColosseumYouReadyForFinals);
    Event_OpenMessage(SPEAKER_WINDOW_BELOW | 8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(20);
      ColosseumEntrance_FaceDirection(MORGAN, FACING_NORTH + FACING_STEP);
      Engine_ActorRunRepeatedMotion(MORGAN, 1);
      Engine_EventSetMessage(MsgColosseumDayTiredWaiting);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_ABOVE | 12);
      ColosseumEntrance_FaceDirection(NAVAMPA, FACING_EAST);
      ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST);
      Engine_ActorSetAnimationAndWait(NAVAMPA, 3);
      Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
      Engine_ActorSetAnimation(SATRAGE, 1);
      Engine_ActorFaceDirection(SATRAGE, FACING_NORTH + FACING_STEP, 20);
      Engine_ActorRunRepeatedMotion(SATRAGE, 1);
      ColosseumEntrance_ShowMessageAndPause(SATRAGE);
      ColosseumEntrance_FaceDirection(DEKKA, FACING_WEST);
      Engine_ActorSetAnimationAndWait(DEKKA, 3);
      Engine_ActorRunRepeatedMotion(NAVAMPA, 2);
      ColosseumEntrance_FaceDirection(NAVAMPA, FACING_NORTHWEST);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_ABOVE | 17);
      ColosseumEntrance_FaceDirection(BUFORD, FACING_NORTHWEST + FACING_STEP);
      Engine_ActorStartRepeatedMotion(BUFORD, 2);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_ABOVE | 18);
      ColosseumEntrance_FaceDirection(11, FACING_SOUTHEAST + FACING_STEP);
      Engine_ActorSetAttachedEffect(11, 0x102);
      Engine_EventWait(60);
      Engine_ActorStartRepeatedMotion(11, 2);
      ColosseumEntrance_ShowMessageAndPause(SPEAKER_WINDOW_BELOW | 11);
      Engine_ActorStop(AZART);
      Engine_TaskWait(1);
      Engine_ActorStartRepeatedMotion(AZART, 2);
      Engine_ActorStartRepeatedMotion(GALAHAD, 2);
      Engine_ActorRunRepeatedMotion(DEKKA, 2);
      Engine_EventWait(20);
      Engine_ActorFaceDirection(AZART, FACING_EAST, 0);
      Engine_ActorFaceDirection(GALAHAD, FACING_WEST, 0);
      Engine_ActorFaceDirection(DEKKA, FACING_NORTHWEST + FACING_STEP, 40);
      Engine_ActorSetAnimation(AZART, 3);
      Engine_ActorSetAnimation(GALAHAD, 3);
      Engine_ActorSetAnimationAndWait(DEKKA, 3);
      Actor_SetSpeed(AZART, PIXELS(FINALS_OFFICIAL) / 5, PIXELS(4) / 5);
      Actor_SetSpeed(GALAHAD, PIXELS(FINALS_OFFICIAL) / 5, PIXELS(4) / 5);
      Actor_SetSpeed(DEKKA, PIXELS(FINALS_OFFICIAL) / 5, PIXELS(4) / 5);
      Actor_SetActionScript(AZART,
                            gColosseumEntrance_ApproachFinalsGroupScript);
      Actor_SetActionScript(DEKKA,
                            gColosseumEntrance_JoinFinalsFromSouthScript);
      Engine_EventWait(20);
      Engine_ActorFaceDirection(SATRAGE, FACING_NORTH + FACING_STEP, 0);
      Engine_ActorFaceDirection(NAVAMPA, FACING_NORTHWEST + FACING_STEP, 0);
      Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
      Engine_ActorFaceDirection(MORGAN, FACING_NORTH + FACING_STEP, 0);
      Engine_ActorFaceDirection(BUFORD, FACING_NORTHWEST + FACING_STEP, 0);
      Actor_RunActionScriptAndWait(GALAHAD,
                                   gColosseumEntrance_JoinFinalsFromEastScript);
      Engine_EventWait(20);
      Engine_ActorFaceDirection(FINALS_OFFICIAL, FACING_EAST, 0);
      Engine_ActorFaceDirection(11, FACING_WEST, 40);
      Engine_ActorSetAnimation(FINALS_OFFICIAL, 3);
      Engine_ActorSetAnimationAndWait(11, 3);
      Engine_CameraMoveTo(PIXELS(808), -1, PIXELS(86), 1);
      Engine_CameraWaitForMove();
      Engine_ActorRunRepeatedMotion(FINALS_OFFICIAL, 2);
      ColosseumEntrance_ShowMessageAndPause(FINALS_OFFICIAL);
      Engine_ActorSetSpeed(FINALS_OFFICIAL, PIXELS(1), PIXELS(1) / 2);
      Object_GetById(FINALS_OFFICIAL)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Engine_ActorWalkToAndWait(FINALS_OFFICIAL, 792, 72);
      Engine_EventWait(1);
      Object_GetById(FINALS_OFFICIAL)->steeringFlags |= ACTOR_FACES_MOTION;
      ColosseumEntrance_FaceDirection(FINALS_OFFICIAL, FACING_EAST);
      // Azart stops pacing before the procession.
      Engine_ActorStop(AZART);
      actor = Object_GetById(AZART);
      actor->update = NULL;
      actor->timer = 0;
      actor->work = 0;
      SET_VELOCITY(actor, 0, 0, 0);
      actor->targetX = ACTOR_NO_TARGET;
      actor->targetY = ACTOR_NO_TARGET;
      actor->targetZ = ACTOR_NO_TARGET;
      Engine_TaskWait(1);
      // Send contestants through the gate at staggered intervals.
      script = gColosseumEntrance_EnterFinalsScript;
      Actor_SetActionScript(SATRAGE, script);
      Engine_EventWait(20);
      Actor_SetActionScript(AZART, script);
      Engine_EventWait(20);
      Actor_SetActionScript(NAVAMPA, script);
      Engine_EventWait(20);
      Actor_SetActionScript(GALAHAD, script);
      Engine_EventWait(20);
      Actor_SetActionScript(DEKKA, script);
      Engine_EventWait(20);
      Actor_SetActionScript(MORGAN, script);
      Engine_EventWait(20);
      Actor_SetActionScript(BUFORD, script);
      Engine_EventWait(60);
      Actor_SetActionScript(ACTOR_PARTY_LEADER, script);
      Engine_EventWait(80);
      Engine_EventRequestExit(66);
    } else {
      gEventWork->message++;
      Engine_EventShowMessage(SPEAKER_WINDOW_BELOW | 8, 0);
    }
    Engine_EventEnd();
  }
}

/// Stepping back from the line arms the question again (scene flags 0x234 and
/// 0x235).
static void Event_RearmFinalsStart(void) {
  Engine_GameFlagClear(FLAG_SCENE_TEMP_35);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_34);
}

/// The trial attendant: before both trials have been watched, the screen goes
/// to the next warrior's trial; afterwards he talks about the techniques.
static void Event_TalkToTrialAttendant(void) {
  s32 clearedTrialSeen;

  Engine_EventBegin();
  clearedTrialSeen = GameFlag_IsSet(FLAG_COLOSSO_CLEARED_TRIAL_SEEN);
  if (clearedTrialSeen != 0) {
    Engine_ActorFaceEachOther(TRIAL_ATTENDANT, ACTOR_PARTY_LEADER, 40);
    Engine_EventSetMessage(MsgColosseumHavePerfectTechniquesLikeThose);
    ColosseumEntrance_ShowMessageAndPause(TRIAL_ATTENDANT);
    Actor_FaceDirection(TRIAL_ATTENDANT, FACING_SOUTHEAST + FACING_STEP, 20);
  } else {
    Engine_ActorStartRepeatedMotion(TRIAL_ATTENDANT, 2);
    Engine_EventSetMessage(MsgColosseumSilence);
    Event_ShowMessage(TRIAL_ATTENDANT, 0);
    Engine_EventGetViewCenter()->motionFlags = clearedTrialSeen;
    Engine_TaskWait(1);
    Camera_SetSpeed(PIXELS(32) / 5, PIXELS(4) / 5);
    Camera_MoveTo(PIXELS(540), -1, PIXELS(208), 1);
    Engine_CameraWaitForMove();
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gEventWork->transitionFrames = 32;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    if (GameFlag_IsSet(FLAG_COLOSSO_FAILED_TRIAL_SEEN) != 0) {
      Engine_EventRequestExit(70);
    } else {
      Engine_EventRequestExit(7);
    }
  }
  Engine_EventEnd();
}

/// Arriving at entrance 7: a warrior strains at the trial, gives up and runs.
static void ColosseumEntrance_ShowFailedTrial(void) {
  struct FieldActor *actor;
  s32 facing;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Event_PlayCue(SE_247);
  Engine_ActorSetAnimation(8, 2);
  Engine_ActorSetAnimation(9, 2);
  Engine_ActorSetAnimation(10, 2);
  Engine_ActorSetAnimation(11, 2);
  Engine_ActorSetAnimation(12, 2);
  Engine_ActorSetAnimation(13, 2);
  Engine_ActorSetAnimation(14, 0);
  Engine_ActorSetAnimation(15, 0);
  Engine_ActorSetAnimation(16, 0);
  Engine_ActorSetAnimation(17, 0);
  Engine_ActorSetAnimation(18, 0);
  actor = Object_GetById(21);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(19);
  actor->scaleX = -Q16_ONE;
  actor = Object_GetById(20);
  actor->scaleX = -Q16_ONE;
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(16, 1);
  Engine_EventWait(20);
  Actor_SetSpeed(16, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(16, 164, 904);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(16, 9);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(16, 10);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(16, 1);
  Engine_EventWait(20);
  Actor_WalkToAndWait(16, 164, 920);
  Actor_WalkToAndWait(16, 185, 920);
  Actor_FaceDirection(16, FACING_NORTH, 20);
  Actor_WalkToAndWait(16, 185, 916);
  Engine_ActorSetAnimation(16, 11);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(16, 1);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(16, 3);
  Engine_EventWait(40);
  Actor_SetActionScript(16, gColosseumEntrance_PulseScaleScript);
  Engine_EventWait(80);
  Actor_SetAttachedEffect(16, 0x102);
  Engine_EventWait(60);
  Actor_FaceDirection(14, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(15, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(17, FACING_EAST, 0);
  Actor_FaceDirection(18, FACING_WEST, 20);
  Actor_SetAttachedEffect(14, 0x102);
  Actor_SetAttachedEffect(15, 0x102);
  Actor_SetAttachedEffect(17, 0x102);
  Actor_SetAttachedEffect(18, 0x102);
  Engine_EventWait(60);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(15, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(17, FACING_SOUTHEAST + FACING_STEP, 0);
  facing = FACING_SOUTHEAST + FACING_STEP;
  ColosseumEntrance_FaceDirection(18, facing);
  Engine_ActorStop(16);
  actor = Object_GetById(16);
  actor->facing = FACING_NORTH + FACING_STEP;
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;
  Engine_EventWait(20);
  Engine_ActorSetAnimation(16, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(19, 5);
  Engine_ActorSetAnimation(20, 5);
  Engine_EventWait(60);
  Actor_FaceDirection(16, facing, 20);
  Engine_ActorSetAnimation(16, 8);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(14, 4);
  Engine_ActorSetAnimation(15, 4);
  Engine_ActorSetAnimation(17, 4);
  Engine_ActorSetAnimationAndWait(18, 4);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(16, 4);
  Engine_EventWait(10);
  Actor_SetSpeed(16, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(16, 162, 916);
  Actor_WalkToAndWait(16, 162, 890);
  Engine_ActorSetAnimation(19, 1);
  Engine_ActorSetAnimation(20, 1);
  Actor_WalkToAndWait(16, 184, 863);
  Actor_WalkToAndWait(16, 184, 796);
  Actor_SetPosition(16, 0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  gEventWork->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventRequestExit(69);
  Engine_EventEnd();
}

/// Back from a trial: the attendant says the warrior failed, or after the
/// second trial that he cleared it.
static void ColosseumEntrance_ExplainTrial(void) {
  Engine_EventBegin();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);

  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();

  Engine_EventWait(20);
  ColosseumEntrance_FaceDirection(TRIAL_ATTENDANT, FACING_SOUTH + FACING_STEP);
  Engine_EventSetMessage(MsgColosseumHaveMakeThroughCountlessMatches);

  if (Engine_GameFlagIsSet(FLAG_COLOSSO_CLEARED_TRIAL_SEEN) != 0) {
    gEventWork->message = (gEventWork->message + 1);
  }

  ColosseumEntrance_ShowMessageAndPause(TRIAL_ATTENDANT);
  ColosseumEntrance_FaceDirection(TRIAL_ATTENDANT,
                                  FACING_SOUTHEAST + FACING_STEP);
  Engine_GameFlagSet(FLAG_COLOSSO_FAILED_TRIAL_SEEN);

  Engine_EventEnd();
}

/// Arriving at entrance 70: a warrior heaves the trial object up in jolts and
/// drops it, shaking the ground: he clears the trial.
static void ColosseumEntrance_ShowClearedTrial(void) {
  u32 counter;
  struct FieldActor *trialEffect;
  struct FieldActor *actor;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Event_PlayCue(SE_247);
  Engine_ActorSetAnimation(8, 2);
  Engine_ActorSetAnimation(9, 2);
  Engine_ActorSetAnimation(10, 2);
  Engine_ActorSetAnimation(11, 2);
  Engine_ActorSetAnimation(12, 2);
  Engine_ActorSetAnimation(13, 2);
  Engine_ActorSetAnimation(14, 0);
  Engine_ActorSetAnimation(15, 0);
  Actor_SetPosition(16, 0, 0);
  Engine_ActorSetAnimation(17, 0);
  Engine_ActorSetAnimation(18, 0);
  actor = Object_GetById(21);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(19);
  actor->scaleX = -Q16_ONE;
  actor = Object_GetById(20);
  actor->scaleX = -Q16_ONE;
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(17, 1);
  Engine_EventWait(20);
  Actor_SetSpeed(17, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(17, 164, 904);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(17, 9);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(17, 10);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(17, 1);
  Engine_EventWait(20);
  Actor_WalkToAndWait(17, 164, 920);
  Actor_WalkToAndWait(17, 185, 920);
  Actor_FaceDirection(17, FACING_NORTH, 20);
  Actor_WalkToAndWait(17, 185, 916);
  Engine_ActorSetAnimation(17, 11);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(17, 1);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(17, 3);
  Engine_EventWait(40);
  Actor_SetActionScript(17, gColosseumEntrance_PulseScaleScript);
  Engine_EventWait(80);
  Actor_SetAttachedEffect(17, 0x102);
  Engine_EventWait(60);
  Actor_FaceDirection(14, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(15, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(18, FACING_WEST, 20);
  Actor_SetAttachedEffect(14, 0x102);
  Actor_SetAttachedEffect(15, 0x102);
  Actor_SetAttachedEffect(17, 0x102);
  Actor_SetAttachedEffect(18, 0x102);
  Engine_EventWait(60);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(15, FACING_SOUTHEAST + FACING_STEP, 0);
  ColosseumEntrance_FaceDirection(18, FACING_SOUTHEAST + FACING_STEP);
  Actor_SetAttachedEffect(17, 0x101);
  // The object rises a little with every heave.
  trialEffect = Object_GetById(21);
  trialEffect->motionFlags = 0;
  for (counter = 0; counter < 20; counter++) {
    trialEffect->y.fixed += PIXELS(3) / 5;
    Engine_TaskWait(4);
    trialEffect->y.fixed += -PIXELS(3) / 10;
    Engine_TaskWait(4);
  }
  Engine_ActorSetAnimation(19, 6);
  Engine_ActorSetAnimation(20, 6);
  Engine_EventWait(60);
  Actor_SetAttachedEffect(17, 0x100);
  Engine_ActorStop(17);
  Engine_ActorSetAnimation(17, 1);
  actor = Object_GetById(17);
  actor->facing = FACING_NORTH + FACING_STEP;
  trialEffect->motionFlags = 3;
  trialEffect->scaleX = Q16_ONE;
  trialEffect->scaleY = Q16_ONE;
  Engine_EventWait(10);
  Event_PlayCue(SE_107);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_EventWait(10);
  Event_PlayCue(SE_289);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Actor_SetSpeed(17, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(17, 208, 928);
  Event_PlayCue(MUS_092);
  Actor_FaceDirection(17, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ActorSetAnimation(17, 9);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(17, 10);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(17, 9);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(17, 10);
  Engine_EventWait(80);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  gEventWork->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_COLOSSO_CLEARED_TRIAL_SEEN);
  Engine_EventRequestExit(69);
  Engine_EventEnd();
}

/// Sets up the halls by the arrival, or the gates outside by Colosso's stage.
s32 ColosseumEntrance_Initialize(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_ColosseumEntrance1) {
    ColosseumEntrance_ApplyArrivalState();
  } else if (scene == (s32)&SceneId_ColosseumEntrance3) {
    ColosseumEntrance_UpdateEntranceCells();
  }
  return 0;
}

/// Each arrival in the halls has its own scene: the trials, the briefing before
/// the finals, the passages to the stages and the congratulations after them.
static void ColosseumEntrance_ApplyArrivalState(void) {
  s32 entrance;

  Engine_TaskWait(1);
  entrance = gGameState.entrance;
  switch (entrance) {
  case 5:
    Engine_ActorSetAnimation(8, 2);
    Engine_ActorSetAnimation(9, 2);
    break;
  case 69:
    Engine_ActorSetAnimation(8, 2);
    Engine_ActorSetAnimation(9, 2);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0)
      break;
    ColosseumEntrance_ExplainTrial();
    break;
  case 7:
    ColosseumEntrance_ShowFailedTrial();
    break;
  case 70:
    ColosseumEntrance_ShowClearedTrial();
    break;
  case 64:
    ColosseumEntrance_ExplainPartySupport();
    InventorySnapshot_Take();
    break;
  case 65:
    ColosseumEntrance_EscortToEastExit();
    break;
  case 66:
    ColosseumEntrance_WalkToEastExit();
    break;
  case 12:
    Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
    ColosseumEntrance_InitializeContestants();
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0)
      break;
    ColosseumEntrance_AnnounceFinals();
    break;
  case 21:
    Party_AddActiveOwner(GARET);
    Party_AddActiveOwner(IVAN);
    Party_AddActiveOwner(MIA);
    Engine_GameFlagSet(FLAG_COLOSSO_FIRST_MATCH_WON);
    ColosseumEntrance_CelebrateAdvancement();
    break;
  case 67:
    ColosseumEntrance_EscortToWestExit();
    break;
  case 68:
    ColosseumEntrance_WalkToWestExit();
    break;
  case 31:
    Party_AddActiveOwner(GARET);
    Party_AddActiveOwner(IVAN);
    Party_AddActiveOwner(MIA);
    Engine_GameFlagSet(FLAG_COLOSSO_SECOND_MATCH_WON);
    ColosseumEntrance_CelebrateBothWins();
    break;
  default:
    break;
  }
}

/// Show the current message and leave a short pause.
static void ColosseumEntrance_ShowMessageAndPause(s32 speaker) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(10);
}

/// Turn an actor to the given facing over ten frames.
static void ColosseumEntrance_FaceDirection(s32 actor, s32 facing) {
  Engine_ActorFaceDirection(actor, (u16)facing, 10);
}

/// Outside: once Colosso is over, the tiles by the gates change; before that,
/// actor 16 takes child value 2, and on the finals day a cell pair changes.
static void ColosseumEntrance_UpdateEntranceCells(void) {
  s32 column, row;

  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
    Map_CopyCellsTo(64, 0, 48, 5, 2, 2);
    column = 16;
    row = 8;
    Map_CopyCellAttributes(14, 8, 2, 1, column, row);
  } else {
    Engine_ActorSetChildValue(16, 2);
    if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0) {
      column = 14;
      row = 11;
      Map_CopyCellAttributes(30, 22, 1, 2, column, row);
    }
  }
}

/// Place the three companions behind Isaac for the officials' briefing.
static inline void ColosseumEntrance_PositionPartyForBriefing(void) {
  Actor_SetPosition(GARET, PIXELS(792), PIXELS(136));
  Actor_SetPosition(IVAN, PIXELS(824), PIXELS(136));
  Actor_SetPosition(MIA, PIXELS(808), PIXELS(152));
}

/// Before the first match: only Isaac may enter the finals, so the officials
/// seat his companions by the stage, where they can cheer him on.
static void ColosseumEntrance_ExplainPartySupport(void) {
  s32 flag;

  Engine_EventBegin();
  ColosseumEntrance_PositionPartyForBriefing();
  Engine_TaskWait(1);
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_ColorBufferApplySource(0, 0);
  ColorBuffer_ApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
  gEventWork->transitionFrames = 1;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ColorBufferApplySource(0, 0);
  ColorBuffer_ApplyTarget(0x10002, 0);
  Engine_ColorBufferInterpolate(40);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(40);
  ColosseumEntrance_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(20);
  ColorBuffer_ApplyTarget(0x10000, 0);
  Engine_ColorBufferInterpolate(40);
  Engine_EventWait(80);
  Actor_ShowEmote(IVAN, 0x100, 0);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(20);
  // The officials explain Isaac's selection and the companions' role.
  Engine_EventSetMessage(MsgColosseumIsaacOnlyOneEnteringFinals);
  ColosseumEntrance_ShowMessageAndPause(IVAN);
  Engine_ActorSetAnimationAndWait(8, 3);
  ColosseumEntrance_ShowMessageAndPause(8);
  Actor_ShowEmote(MIA, 0x102, 40);
  ColosseumEntrance_ShowMessageAndPause(MIA);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Actor_FaceDirection(9, FACING_EAST, 40);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 0);
  ColosseumEntrance_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP);
  Actor_ShowEmote(9, 0x105, 20);
  ColosseumEntrance_ShowMessageAndPause(9);
  Actor_ShowEmote(GARET, 0x103, 20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  ColosseumEntrance_ShowMessageAndPause(GARET);
  Actor_ShowEmote(10, 0x102, 40);
  ColosseumEntrance_ShowMessageAndPause(10);
  Engine_ActorSetAnimationAndWait(11, 3);
  ColosseumEntrance_ShowMessageAndPause(11);
  ColosseumEntrance_FaceDirection(IVAN, FACING_NORTHWEST);
  Engine_ActorSetAnimationAndWait(IVAN, 4);
  ColosseumEntrance_ShowMessageAndPause(IVAN);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  ColosseumEntrance_ShowMessageAndPause(MIA);
  Actor_ShowEmote(GARET, 0x103, 20);
  ColosseumEntrance_FaceDirection(GARET, FACING_EAST);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  ColosseumEntrance_ShowMessageAndPause(GARET);
  ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  ColosseumEntrance_FaceDirection(IVAN, FACING_WEST);
  ColosseumEntrance_ShowMessageAndPause(IVAN);
  Engine_ActorSetAnimation(MIA, 4);
  Engine_EventWait(20);
  ColosseumEntrance_ShowMessageAndPause(MIA);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  ColosseumEntrance_FaceDirection(GARET, FACING_SOUTHEAST);
  ColosseumEntrance_ShowMessageAndPause(GARET);
  Actor_ShowEmote(IVAN, 0x105, 60);
  Actor_FaceDirection(IVAN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
  Actor_ShowEmote(GARET, 0x102, 0);
  Actor_ShowEmote(MIA, 0x102, 80);
  Actor_ShowEmote(IVAN, 0x106, 0);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  ColosseumEntrance_FaceDirection(IVAN, FACING_NORTH);
  ColosseumEntrance_ShowMessageAndPause(IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 20);
  ColosseumEntrance_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP);
  Engine_ActorSetAnimationAndWait(8, 3);
  ColosseumEntrance_ShowMessageAndPause(8);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
  Actor_ShowEmote(GARET, 0x100, 0);
  Actor_ShowEmote(MIA, 0x100, 40);
  Engine_ActorRunRepeatedMotion(9, 1);
  ColosseumEntrance_ShowMessageAndPause(9);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 40);
  Engine_ActorRunRepeatedMotion(10, 1);
  ColosseumEntrance_ShowMessageAndPause(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  ColosseumEntrance_FaceDirection(MIA, FACING_NORTH);
  Engine_ActorSetAnimationAndWait(11, 3);
  ColosseumEntrance_ShowMessageAndPause(11);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Actor_ShowEmote(GARET, 0x105, 0);
  Actor_ShowEmote(IVAN, 0x105, 0);
  Actor_ShowEmote(MIA, 0x105, 80);
  Actor_ShowEmote(IVAN, 0x106, 0);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  ColosseumEntrance_FaceDirection(IVAN, FACING_NORTHEAST);
  ColosseumEntrance_ShowMessageAndPause(IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 20);
  Actor_ShowEmote(11, 0x101, 60);
  Event_OpenMessage(IVAN, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  flag = 0;
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    ColosseumEntrance_ShowMessageAndPause(IVAN);
    flag = 1;
  } else {
    gEventWork->message++;
    Engine_ActorSetAnimationAndWait(IVAN, 3);
    ColosseumEntrance_FaceDirection(IVAN, FACING_NORTH);
    ColosseumEntrance_ShowMessageAndPause(IVAN);
  }
  if (flag != 0) {
    gEventWork->message++;
  }
  Actor_FaceDirection(8, FACING_WEST, 0);
  Actor_FaceDirection(9, FACING_EAST, 0);
  Actor_FaceDirection(10, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(11, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_ShowEmote(8, 0x105, 0);
  Actor_ShowEmote(9, 0x105, 0);
  Actor_ShowEmote(10, 0x105, 0);
  Actor_ShowEmote(11, 0x105, 60);
  Engine_ActorRunRepeatedMotion(8, 1);
  ColosseumEntrance_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP);
  ColosseumEntrance_ShowMessageAndPause(8);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 20);
  Engine_ActorRunRepeatedMotion(9, 1);
  ColosseumEntrance_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP);
  ColosseumEntrance_ShowMessageAndPause(9);
  Engine_ActorRunRepeatedMotion(10, 1);
  ColosseumEntrance_FaceDirection(10, FACING_SOUTH + FACING_STEP);
  ColosseumEntrance_ShowMessageAndPause(10);
  ColosseumEntrance_FaceDirection(11, FACING_WEST);
  Engine_ActorSetAnimationAndWait(11, 3);
  ColosseumEntrance_ShowMessageAndPause(11);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  // Rejoin the companions before the official leads Isaac onward.
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetActionScript(GARET, gColosseumEntrance_RejoinPartyScript);
  Actor_SetActionScript(IVAN, gColosseumEntrance_RejoinPartyScript);
  Actor_RunActionScriptAndWait(MIA, gColosseumEntrance_RejoinPartyScript);
  Engine_EventWait(20);
  ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(11, 3);
  Actor_SetSpeed(11, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetAnimation(11, 2);
  Actor_MoveToAndWait(11, 830, 152);
  Actor_MoveToAndWait(11, 808, 164);
  Actor_SetDestination(11, 808, 312);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(808), -1, PIXELS(312), 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 808, 164);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 312);
  Engine_EventWait(60);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 40;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(64);
}

/// An official leads Isaac east to the next stage.
static void ColosseumEntrance_EscortToEastExit(void) {
  struct FieldActor *escort;

  if (GameFlag_IsSet(JENNA) != 0) {
    GameFlag_Set(0x16d);
    Party_RemoveActiveOwner(JENNA);
    Party_AddActiveOwner(MIA);
  }
  Engine_EventBegin();
  Actor_SetPosition(11, PIXELS(712), PIXELS(588));
  Engine_TaskWait(1);
  Engine_CameraFollowActor(11, 1);
  Actor_SetSpeed(11, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  escort = Object_GetById(11);
  escort->facing = FACING_EAST;
  Engine_EventOpenScreen();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
  Engine_ActorSetAnimation(11, 2);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 780, 588);
  Actor_MoveToAndWait(11, 812, 588);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 880, 588);
  Actor_MoveToAndWait(11, 912, 588);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 980, 588);
  Actor_SetDestination(11, 1012, 588);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  if (GameFlag_IsSet(FLAG_COLOSSO_SECOND_MATCH_WON) != 0) {
    Engine_EventRequestExit(31);
  } else {
    Engine_EventRequestExit(65);
  }
}

/// Arriving in the waiting room: the official tells the warriors to prepare.
static void ColosseumEntrance_AnnounceFinals(void) {
  struct FieldActor *contestant;

  contestant = Object_GetById(AZART);
  Engine_EventBegin();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(FINALS_OFFICIAL, 2);
  Engine_ActorStop(AZART);
  Engine_TaskWait(1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Engine_ActorSetAnimation(AZART, 1);
  Actor_FaceDirection(MORGAN, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(AZART, FACING_EAST, 0);
  Actor_FaceDirection(GALAHAD, FACING_WEST, 0);
  Actor_FaceDirection(SATRAGE, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(DEKKA, FACING_WEST, 0);
  Actor_FaceDirection(NAVAMPA, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(BUFORD, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventSetMessage(MsgColosseumPrepareYourselvesContestantsFinalsWill);
  ColosseumEntrance_ShowMessageAndPause(FINALS_OFFICIAL);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  {
    u16 *timer = (u16 *)&contestant->timer;
    u16 duration = 720;

    *timer = duration;
  }
  {
    u16 *work = &contestant->work;
    u16 value = 112;

    *work = value;
  }
  Actor_SetBuiltinAction(AZART, 2);
  Actor_FaceDirection(MORGAN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(GALAHAD, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(SATRAGE, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(DEKKA, FACING_EAST, 0);
  Actor_FaceDirection(NAVAMPA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(BUFORD, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventEnd();
}

/// Isaac walks east alone to the next stage.
static void ColosseumEntrance_WalkToEastExit(void) {
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Engine_EventOpenScreen();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 780, 428);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 880, 428);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 980, 428);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  if (GameFlag_IsSet(FLAG_COLOSSO_SECOND_MATCH_WON) != 0) {
    Engine_EventRequestExit(32);
  } else {
    Engine_EventRequestExit(12);
  }
}

/// After the first match: the officials congratulate Isaac and lead him on.
static void ColosseumEntrance_CelebrateAdvancement(void) {
  const s32 *actions;

  Engine_EventBegin();
  ColosseumEntrance_PositionPartyForBriefing();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_ActorSetAnimation(8, 3);
  Engine_EventSetMessage(MsgColosseumSeeYouveMadeThroughYour);
  ColosseumEntrance_ShowMessageAndPause(8);
  Engine_ActorRunRepeatedMotion(9, 1);
  ColosseumEntrance_ShowMessageAndPause(9);
  Engine_ActorRunRepeatedMotion(10, 1);
  ColosseumEntrance_ShowMessageAndPause(10);
  Engine_ActorRunRepeatedMotion(11, 1);
  Engine_ActorSetAnimation(11, 3);
  ColosseumEntrance_ShowMessageAndPause(11);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  actions = gColosseumEntrance_RejoinPartyScript;
  Actor_SetActionScript(GARET, actions);
  Actor_SetActionScript(IVAN, actions);
  Actor_RunActionScriptAndWait(MIA, actions);
  Engine_EventWait(20);
  ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(11, 3);
  Actor_SetSpeed(11, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetAnimation(11, 2);
  Actor_MoveToAndWait(11, 830, 152);
  Actor_MoveToAndWait(11, 808, 164);
  Actor_SetDestination(11, 808, 312);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(808), -1, PIXELS(312), 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 808, 164);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 312);
  Engine_EventWait(60);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(67);
}

/// An official leads Isaac west to the next stage.
static void ColosseumEntrance_EscortToWestExit(void) {
  struct FieldActor *escort;

  if (GameFlag_IsSet(JENNA) != 0) {
    GameFlag_Set(0x16d);
    Party_RemoveActiveOwner(JENNA);
    Party_AddActiveOwner(MIA);
  }
  Engine_EventBegin();
  Actor_SetPosition(11, PIXELS(868), PIXELS(588));
  Engine_TaskWait(1);
  Engine_CameraFollowActor(11, 1);
  Actor_SetSpeed(11, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  escort = Object_GetById(11);
  escort->facing = FACING_WEST;
  Engine_EventOpenScreen();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
  Engine_ActorSetAnimation(11, 2);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 800, 588);
  Actor_MoveToAndWait(11, 768, 588);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 700, 588);
  Actor_MoveToAndWait(11, 668, 588);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 600, 588);
  Actor_SetDestination(11, 568, 588);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(21);
}

/// Isaac walks west alone to the next stage.
static void ColosseumEntrance_WalkToWestExit(void) {
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Engine_EventOpenScreen();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 800, 428);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 700, 428);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 600, 428);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(22);
}

/// After the second match: one more win and Isaac is the champion.
static void ColosseumEntrance_CelebrateBothWins(void) {
  const s32 *actions;

  Engine_EventBegin();
  ColosseumEntrance_PositionPartyForBriefing();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_ActorSetAnimation(8, 3);
  Engine_EventSetMessage(MsgColosseumWonBothMatches);
  ColosseumEntrance_ShowMessageAndPause(8);
  Engine_ActorRunRepeatedMotion(9, 1);
  ColosseumEntrance_ShowMessageAndPause(9);
  Engine_ActorRunRepeatedMotion(10, 1);
  ColosseumEntrance_ShowMessageAndPause(10);
  Engine_ActorRunRepeatedMotion(11, 1);
  Engine_ActorSetAnimation(11, 3);
  ColosseumEntrance_ShowMessageAndPause(11);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  actions = gColosseumEntrance_RejoinPartyScript;
  Actor_SetActionScript(GARET, actions);
  Actor_SetActionScript(IVAN, actions);
  Actor_RunActionScriptAndWait(MIA, actions);
  Engine_EventWait(20);
  ColosseumEntrance_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(11, 3);
  Actor_SetSpeed(11, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetAnimation(11, 2);
  Actor_MoveToAndWait(11, 830, 152);
  Actor_MoveToAndWait(11, 808, 164);
  Actor_SetDestination(11, 808, 312);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(808), -1, PIXELS(312), 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 808, 164);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 808, 312);
  Engine_EventWait(60);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(64);
}

/// The warriors' child values and their waiting actions in the waiting room.
static void ColosseumEntrance_InitializeContestants(void) {
  Engine_ActorSetChildValue(MORGAN, 3);
  Engine_ActorSetChildValue(AZART, 0);
  Engine_ActorSetChildValue(GALAHAD, 4);
  Engine_ActorSetChildValue(SATRAGE, 1);
  Engine_ActorSetChildValue(DEKKA, 5);
  Engine_ActorSetChildValue(NAVAMPA, 2);
  Engine_ActorSetChildValue(BUFORD, 6);
  Object_SetActionById(AZART, 10);
  Object_SetActionById(GALAHAD, 20);
  Engine_ActorSetAnimation(SATRAGE, 0);
  Object_SetActionById(DEKKA, 40);
  Object_SetActionById(NAVAMPA, 50);
  Object_SetActionById(BUFORD, 60);
}

s32 gColosseumEntrance_GatherAtSpectatorLeaderScript[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(262), 0, PIXELS(188)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_END,
};

s32 gColosseumEntrance_GatherAtSpectatorPartyScript[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(246), 0, PIXELS(200)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_END,
};

s32 gColosseumEntrance_RejoinPartyScript[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(808), 0, PIXELS(120)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_END,
};

s32 gColosseumEntrance_LeaveSpectatorAreaScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(268), 0, PIXELS(146)),
    SC_SYNC,   SC_MOVE(PIXELS(262), 0, PIXELS(24)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gColosseumEntrance_ApproachFinalsGroupScript[] = {
    SC_ARG(2), SC_MOVE(PIXELS(776), 0, PIXELS(120)),          SC_SYNC,
    SC_ARG(1), SC_SET(SW_FACING, FACING_NORTH + FACING_STEP), SC_WAIT(20),
    SC_END,
};

s32 gColosseumEntrance_JoinFinalsFromEastScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(894), 0, PIXELS(106)),
    SC_SYNC,
    SC_MOVE(PIXELS(882), 0, PIXELS(136)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(136)),
    SC_SYNC,
    SC_MOVE(PIXELS(792), 0, PIXELS(152)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_WAIT(20),
    SC_END,
};

s32 gColosseumEntrance_JoinFinalsFromSouthScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(136)),
    SC_SYNC,
    SC_MOVE(PIXELS(824), 0, PIXELS(152)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_WAIT(20),
    SC_END,
};

s32 gColosseumEntrance_EnterFinalsScript[] = {
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_SET(SW_ACCEL, PIXELS(2) / 5),
    SC_ARG(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(88)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(58)),
    SC_SYNC,
    SC_ARG(1),
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gColosseumEntrance_PulseScaleScript[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 8),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 8),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gColosseumEntranceEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 72, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 72, 0, 72, FACING_SOUTH, 0, 16, 16, 528, 256, 0},
    {2, CONDITION_ALWAYS, 488, 0, 88, FACING_SOUTH, 0, 16, 16, 528, 256, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gColosseumEntranceEntrances1[] = {
    {0, CONDITION_ALWAYS, 56, 0, 296, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 184, 0, 72, FACING_SOUTH, 0, 0, 0, 240, 384, 0},
    {2, CONDITION_ALWAYS, 56, 0, 120, FACING_SOUTH, 0, 0, 0, 240, 384, 0},
    {3, CONDITION_ALWAYS, 120, 0, 176, FACING_SOUTH, 0, 0, 0, 240, 384, 0},
    {4, CONDITION_ALWAYS, 56, 0, 296, FACING_SOUTH, 0, 0, 0, 240, 384, 0},
    {5, CONDITION_ALWAYS, 392, 0, 120, FACING_SOUTH, 0, 272, 0, 656, 304, 0},
    {6, CONDITION_ALWAYS, 520, 0, 72, FACING_SOUTH, 0, 272, 0, 656, 304, 0},
    {7, CONDITION_ALWAYS, 184, 0, 912, FACING_SOUTH, 0, 0, 736, 368, 1024, 0},
    {8, CONDITION_ALWAYS, 808, 0, 296, FACING_NORTH, 0, 656, 0, 960, 320, 0},
    {12, CONDITION_ALWAYS, 784, 0, 108, FACING_SOUTHWEST, 0, 656, 0, 960, 320,
     0},
    {21, CONDITION_ALWAYS, 808, 0, 120, FACING_NORTH, 0, 656, 0, 960, 320, 0},
    {31, CONDITION_ALWAYS, 808, 0, 120, FACING_NORTH, 0, 656, 0, 960, 320, 0},
    {64, CONDITION_ALWAYS, 808, 0, 120, FACING_NORTH, 0, 656, 0, 960, 320, 0},
    {65, CONDITION_ALWAYS, 680, 0, 588, FACING_EAST, 0, 520, 490, 1020, 640, 0},
    {66, CONDITION_ALWAYS, 680, 0, 428, FACING_EAST, 0, 520, 330, 1020, 480, 0},
    {67, CONDITION_ALWAYS, 900, 0, 588, FACING_WEST, 0, 520, 490, 1020, 640, 0},
    {68, CONDITION_ALWAYS, 900, 0, 428, FACING_WEST, 0, 520, 330, 1020, 480, 0},
    {69, CONDITION_ALWAYS, 440, 0, 120, FACING_SOUTHEAST, 0, 272, 0, 656, 304,
     0},
    {70, CONDITION_ALWAYS, 184, 0, 912, FACING_SOUTH, 0, 0, 736, 368, 1024, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gColosseumEntranceEntrances3[] = {
    {1, CONDITION_ALWAYS, 96, 0, 288, FACING_NORTH, 0, 0, 0, 512, 320, 0},
    {2, CONDITION_ALWAYS, 272, 0, 88, FACING_SOUTH, 0, 0, 0, 512, 320, 0},
    {SCENE_TABLE_END},
};

u32 gColosseumEntranceExits[] = {
    SCENE_EXITS(141),        SCENE_EXIT(1, 138, 13),  SCENE_EXIT(2, 140, 4),
    SCENE_EXIT(0, 140, 0),   SCENE_EXIT(1, 140, 6),   SCENE_EXIT(2, 140, 5),
    SCENE_EXIT(3, 140, 1),   SCENE_EXIT(4, 141, 2),   SCENE_EXIT(5, 140, 2),
    SCENE_EXIT(6, 140, 1),   SCENE_EXIT(7, 140, 7),   SCENE_EXIT(69, 140, 69),
    SCENE_EXIT(70, 140, 70), SCENE_EXIT(64, 140, 65), SCENE_EXIT(65, 143, 3),
    SCENE_EXIT(66, 140, 66), SCENE_EXIT(67, 140, 67), SCENE_EXIT(12, 143, 2),
    SCENE_EXIT(21, 144, 3),  SCENE_EXIT(22, 144, 2),  SCENE_EXIT(31, 145, 3),
    SCENE_EXIT(32, 145, 2),  SCENE_EXIT(0, 142, 0),   SCENE_EXIT(1, 135, 12),
    SCENE_EXIT(2, 140, 64),  SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gColosseumEntrancePlacementsOther[] = {
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements1Entrance5[] = {
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(496), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(592), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(88), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x39, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(528), 0, PIXELS(88), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(184), FACING_SOUTHEAST, TALK_KEEP_FACING, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(432), 0, PIXELS(96), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(476), 0, PIXELS(96), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(168), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(454), 0, PIXELS(104), FACING_SOUTHEAST, TALK_KEEP_FACING, 0 },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(208), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(444), 0, PIXELS(72), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(224), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(200), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(224), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(168), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(216), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(428), 0, PIXELS(72), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(240), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(192), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(200), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(184), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(608), 0, PIXELS(104), FACING_SOUTHEAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements1Entrance7[] = {
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(250), 0, PIXELS(846), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(298), 0, PIXELS(912), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(250), 0, PIXELS(974), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(121), 0, PIXELS(974), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(912), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(121), 0, PIXELS(846), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(96), 0, PIXELS(900), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(114), 0, PIXELS(884), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(872), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(148), 0, PIXELS(864), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(164), 0, PIXELS(860), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(212), 0, PIXELS(872), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(888), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xfd, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(902), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements1[] = {
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(72), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(72), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(128), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements1Entrance8[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(808), 0, PIXELS(88), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(782), 0, PIXELS(96), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(832), 0, PIXELS(96), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(856), 0, PIXELS(120), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements1Entrance12[] = {
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(808), 0, PIXELS(72), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(808), 0, PIXELS(234), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(778), 0, PIXELS(72), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(72), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(760), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(720), 0, PIXELS(112), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(872), 0, PIXELS(88), FACING_NORTHWEST + FACING_STEP, 3, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(808), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(920), 0, PIXELS(134), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(856), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements1Entrance66[] = {
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(536), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(632), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(728), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(824), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(920), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1016), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements3[] = {
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(168), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(198), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(266), 0, PIXELS(144), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(152), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(74), 0, PIXELS(196), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(80), 0, PIXELS(238), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(198), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(180), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(180), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(48), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(88), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(80), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(56), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x9f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(140), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x9f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(64), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xa9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(72), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements3FinalsDay[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(182), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(294), 0, PIXELS(178), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(228), 0, PIXELS(192), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(198), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(74), 0, PIXELS(196), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(438), 0, PIXELS(186), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(424), 0, PIXELS(186), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(180), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(180), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(258), 0, PIXELS(161), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(244), 0, PIXELS(150), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(161), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(168), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(52), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x6d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(70), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(88), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(106), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x9f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(124), FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x9f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(142), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xa9, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(160), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gColosseumEntrancePlacements3AfterColosso[] = {
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(80), 0, PIXELS(238), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(286), 0, PIXELS(164), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gColosseumEntranceEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gColosseumEntranceEvents2[] = {
    { 0x21, 1, CONDITION_ALWAYS, 1 },
    { 0x21, 2, CONDITION_ALWAYS, 2 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gColosseumEntranceEvents1[] = {
    { 0x21, 1, CONDITION_ALWAYS, 1 },
    { 0x21, 2, CONDITION_ALWAYS, 2 },
    { 0x31, 4, CONDITION_ALWAYS, 4 },
    { 0x31, 5, CONDITION_ALWAYS, 5 },
    { 0x31, 6, CONDITION_ALWAYS, 6 },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceGoForItThatsItIm) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWatchingTheTrialsMakesMeReally) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWatchingSuchProudSkilledWarriorsIn) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceIveWaitedAWholeYearTo) },
    { EVENT_TALK, TRIAL_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToTrialAttendant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceEvenTheSmallerWarriorsCanWin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheseTrialsAreBasedOnPersonal) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhatFantasticPhysiquesWhyIfI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgColossoDidntThinkBattles) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceNoMatterHowGoodAWarrior) },
    { SEARCH(SEARCH_UNNAMED), 50, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianItIsLocked)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gColosseumEntranceEvents3[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, 8, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceTheyreHoldingThePreliminaryColossoHeats) },
    { EVENT_TALK, 8, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceTheFinalsAreFinallyHereI) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceColossoIsOverAndTheColosseum) },
    { EVENT_TALK, 9, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceSighTheColosseumIsPackedWith) },
    { EVENT_TALK, 9, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceIHeardBabiHasPickedSome) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheColosseumIsClosedUntilNext) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DiscussSeating) },
    { EVENT_TALK, 11, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceIfYouHitItBigIn) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceAWarriorNamedIsaacWasSelected) },
    { EVENT_TALK, 12, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceIveBeenBettingOnTheLucky) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceHavingAWarriorWhoWasntIn) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DiscussLuckyGuessSeats) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DiscussLuckyGuessWinnings) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OfferLuckyGuess) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceIWonderWhichWarriorWillBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceTheRealBattlesAreAboutTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhenTheColosseumGatesFinallyShut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceHavingToArriveSoEarlyJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceTheWarriorNamedIsaacIsSure) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheFightingAtThisYearsColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceThereAreSoManyPeopleWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceOfCourseItsTheRichWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceImGoingToPickTheBiggest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceThereCanBeUpsetsInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceImFeelingConfidentThisTimeIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhatKindOfWarriorIsThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceEveryoneKeepsRiskingMoreAndMore) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceMaybeIWillBuyASeat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceEveryYearIGoAfterA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceImGoingToBeInTrouble) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceOnceTheTrialsAreOverIts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceAllOfTheProfitsFromThe) },
    { EVENT_TOUCH, 10, FLAG_COLOSSO_OVER, EVENT_SCRIPT(Event_CrowdRecognizesIsaac) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gColosseumEntranceEvents1Entrance12[] = {
    {EVENT_TALK, MORGAN, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceIAmMorganIAmThe)},
    {EVENT_TALK, AZART, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToAzart)},
    {EVENT_TALK, GALAHAD, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToGalahad)},
    {EVENT_TALK, SATRAGE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToSatrage)},
    {EVENT_TALK, DEKKA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToDekka)},
    {EVENT_TALK, NAVAMPA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToNavampa)},
    {EVENT_TALK, BUFORD, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToBuford)},
    {EVENT_TALK, FINALS_OFFICIAL, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_AskIfReadyForFinals)},
    {EVENT_TALK, 9, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceYouCannotLeaveUntilTheFinals)},
    {EVENT_TALK, 10, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceIFeelWeveBeenWaitingForever)},
    {EVENT_TALK, 11, CONDITION_ALWAYS,
     MSGNO(MsgColosseumEntranceWhenTheGongSoundsTheFinals)},
    {EVENT_TOUCH, 12, FLAG_SCENE_TEMP_35, EVENT_SCRIPT(Event_FinalsBegin)},
    {EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RearmFinalsStart)},
    {SCENE_EVENTS_END},
};

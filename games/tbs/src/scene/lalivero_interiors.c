/// @file
/// Lalivero's buildings, in two scenes with the area name Lalivero: the town
/// whose master, Faran, raised Sheba, the girl Lalivero received from the sky.
///
/// Where they fall in the story: when the party first comes to Lalivero,
/// Sheba has been taken hostage and Faran has followed her captors into the
/// ruins; the town talks of little else, and of the lighthouse Babi is
/// building. The party comes back from Venus Lighthouse by the Lighthouse
/// Aerie's way down (entrance 90, which sets
/// FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE), and from then on each scene reads its
/// "after" tables: the people speak of the rough seas since the beacon was lit
/// and of the search for Sheba, who fell from the lighthouse into the sea.
///
/// - LaliveroInteriors1 (scene 179): the houses, three shops and the Sanctum,
///   whose area name is Lalivero Sanctum by entrances 11 and 12. Shopkeepers
///   trade across their counters (shops 32 to 34) and the priest offers the
///   Sanctum's services; entering the Sanctum by entrance 12 makes it the place
///   a defeated party wakes. Barrels hold a Lucky Medal and an Antidote. The
///   doors lead back out into Lalivero (lalivero.c, scene 178).
/// - LaliveroInteriors2 (scene 180): Faran's house, the inn, and the way out to
///   the edge of town where Babi's ship lies (exit 31, the Lemurian ship's
///   entrance 1). Before Venus Lighthouse, Faran's household waits for news,
///   and a maid too worried about Sheba to work apologizes once
///   (FLAG_MAID_APOLOGIZED) and goes back to her chores. Back from the
///   lighthouse, entrance 99 runs the report (ReportToFaran): Faran and Iodem
///   have found no sign of Sheba; the party tells what became of Jenna and the
///   others, in one of two versions by whether they saw the ship from the
///   lighthouse (FLAG_SAW_SHIP_FROM_VENUS_LIGHTHOUSE), and will go on searching
///   for Jenna; Faran is sure Sheba lived, Iodem tells him what an Adept is,
///   and Faran sends them off. Stepping on trigger 51 afterwards, Iodem asks
///   the party to wait for him outside Faran's house
///   (FLAG_IODEM_ASKED_TO_MEET), where he gives them the Black Orb
///   (lalivero.c).

#include "script.h"
#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/lalivero_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/shop/shop.h"
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
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "songs.h"
#include "flags.h"
static void TellFaranAboutJenna(void);

enum {
  /// Stepping into the sanctum makes it where a defeated party wakes.
  ENTRANCE_SANCTUM = 12,
  ENTRANCE_FARAN_HOUSE = 21,
  ENTRANCE_FROM_AERIE = 90,
  /// Back from Venus Lighthouse: the party is led straight to Faran.
  ENTRANCE_REPORT_TO_FARAN = 99
};

/// The actors this scene's code moves, by their place in the placement tables.
enum {
  /// In Faran's house and the inn (LaliveroInteriors2).
  FARAN = 11,
  IODEM = 12,
  INNKEEPER = 13,
  MAID = 14,
  // In the shops and the Sanctum (LaliveroInteriors1).
  SHOPKEEPER_32 = 18, ///< runs shop 32
  SHOPKEEPER_33 = 19,
  SHOPKEEPER_34 = 20,
  PRIEST = 21,
  ACTOR_15 = 15 ///< posed in animation 6
};

/// Flag 0x300, which several places use for a passing state as they use the
/// scene flags from 0x200: the maid has apologized.
enum { FLAG_MAID_APOLOGIZED = 0x300 };

/// Behind a counter: the actor is drawn under the counter's top and the
/// party talks across it.
#define ACTOR_BEHIND_COUNTER 4

/// LaliveroInteriors2: the maid sighs between chores.
static s32 Maid_Sigh(struct ScriptInterpreter *interpreter) {
  Actor_SetAttachedEffect(MAID, EMOTE_IN_FRONT | 2);
  return 0;
}

const struct SceneEntrance *LaliveroInteriors_GetEntrances(void) {
  return gLaliveroEntrances;
}

const struct SceneRegion *LaliveroInteriors_GetRegions(void) {
  if (gGameState.scene == (s32)&SceneId_LaliveroInteriors1) {
    return gLaliveroSanctumRegions;
  }
  return gLaliveroRegions;
}

const u32 *LaliveroInteriors_GetExits(void) { return gLaliveroExits; }

/// The people of each scene; Faran's house fills up after Venus Lighthouse.
const struct ScenePlacement *LaliveroInteriors_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_LaliveroInteriors2) {
    // Faran's house and the inn.
    if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE) != 0) {
      return gFaranHousePlacementsAfterLighthouse;
    }
    return gFaranHousePlacements;
  }
  return gLaliveroPlacements;
}

/// The side the leader faces: the facing rounded to the nearest of east,
/// south, west and north. Facing north, the leader is across the counter
/// from a shopkeeper.
static inline u16 Leader_GetSide(void) {
  return (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
}

/// Lalivero (LaliveroInteriors1): a shopkeeper across the counter opens shop
/// 32; from elsewhere, before Venus Lighthouse, they ask whether the party has
/// heard that Sheba was taken hostage and Faran went after her, and after it
/// tell of the rough sea.
static void TalkToShopkeeper32(s32 keeper) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  // Leader_GetSide's rounding, written with the mask 0xc000, which
  // loads from the literal pool where the helper's builds the mask.
  u16 side = (leader->facing + 0x2000) & 0xc000;

  if (side == FACING_NORTH) {
    Shop_Run(32, keeper);
  } else if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE)) {
    Engine_EventSetMessage(MsgLaliveroWentOceanLook);
    Engine_EventShowMessage(keeper, 0);
  } else {
    s32 message = MsgLaliveroTakenHostageFaran;

    Engine_EventSetMessage(message);
    Engine_EventOpenMessage(keeper, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      Engine_EventWait(10);
      Engine_EventSetMessage(message + 1);
    } else {
      Engine_EventSetMessage(message + 2);
    }
    Engine_EventShowMessage(keeper, 0);
  }
}

/// Lalivero (LaliveroInteriors1): shop 33 across the counter; otherwise small
/// talk.
static void TalkToShopkeeper33(s32 keeper) {
  if (Leader_GetSide() == FACING_NORTH) {
    Shop_Run(33, keeper);
  } else if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE)) {
    Engine_EventSetMessage(MsgLaliveroWantedSendShips);
    Engine_EventShowMessage(keeper, 0);
  } else {
    Engine_EventSetMessage(MsgLaliveroWarriorRightPlease);
    Engine_EventShowMessage(keeper, 0);
  }
}

/// Lalivero (LaliveroInteriors1): shop 34 across the counter; otherwise small
/// talk.
static void TalkToShopkeeper34(s32 keeper) {
  if (Leader_GetSide() == FACING_NORTH) {
    Shop_Run(34, keeper);
  } else if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE)) {
    Engine_EventSetMessage(MsgLaliveroImpossibleGoOcean);
    Engine_EventShowMessage(keeper, 0);
  } else {
    Engine_EventSetMessage(MsgLaliveroGuessPeopleAngara);
    Engine_EventShowMessage(keeper, 0);
  }
}

/// Lalivero Sanctum: the priest offers the Sanctum's services across the
/// counter; otherwise small talk of Babi's lighthouse.
static void TalkToPriest(s32 priest) {
  if (Leader_GetSide() == FACING_NORTH) {
    Shop_ConfirmAct(priest);
  } else if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE)) {
    Engine_EventSetMessage(MsgLaliveroBabiLighthouseStruck);
    Engine_EventShowMessage(priest, 0);
  } else {
    Engine_EventSetMessage(MsgLaliveroBabiIsntBuilding);
    Engine_EventShowMessage(priest, 0);
  }
}

/// Lalivero (LaliveroInteriors2), the inn: a room for the night across the
/// counter (inn 11); otherwise small talk.
static void TalkToInnkeeper(s32 innkeeper) {
  if (Leader_GetSide() == FACING_NORTH) {
    Inn_CheckIn(11, innkeeper);
  } else if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE)) {
    Engine_EventSetMessage(MsgLaliveroDidntKnowTime);
    Engine_EventShowMessage(innkeeper, 0);
  } else {
    Engine_EventSetMessage(MsgLaliveroWantStayLet);
    Engine_EventShowMessage(innkeeper, 0);
  }
}

/// Lalivero (LaliveroInteriors2), before Venus Lighthouse: the maid stops her
/// chores to apologize: worry for Sheba keeps her from concentrating. A leader
/// standing west of her steps aside to the south before she starts again.
static void TalkToMaid(void) {
  s16 side;

  side = Leader_GetSide();
  GameFlag_Set(FLAG_MAID_APOLOGIZED);
  Engine_EventBegin();
  Engine_ResetSceneEffectCounter();
  Engine_EventSetMessage(MsgLaliveroImSorry);
  Engine_EventWait(50);
  Actor_ShowEmote(MAID, EMOTE_IN_FRONT | 2, 50);
  Actor_FaceActor(MAID, ACTOR_PARTY_LEADER, 20);
  Event_ShowMessage(MAID, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MAID, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessage(MAID, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(MAID, 2);
  Engine_EventWait(30);
  Event_ShowMessage(MAID, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MAID, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessage(MAID, 0);
  if ((u16)side == FACING_WEST) {
    Engine_ActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, 16);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Engine_EventWait(20);
  }
  Actor_SetActionScript(MAID, gLaliveroMaidChores);
  Engine_EventEnd();
}

/// Lalivero (LaliveroInteriors2), trigger 51 in Faran's house after the
/// report: the camera finds Iodem, who asks the party to wait for him outside.
static void IodemCallsOut(void) {

  Engine_GameFlagSet(FLAG_IODEM_ASKED_TO_MEET);
  Engine_EventBegin();
  Engine_ResetSceneEffectCounter();
  Engine_EventWait(10);
  Engine_CameraMoveTo(PIXELS(120), -1, PIXELS(96), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(30);
  Engine_EventSetMessage(MsgLaliveroPleaseWaitForMeOutside);
  Engine_EventShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, IODEM, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventEnd();
}

/// Lalivero (LaliveroInteriors2): Iodem, talked to before he has given the
/// Black Orb, repeats his request.
static void TalkToIodem(void) {

  Engine_GameFlagSet(FLAG_IODEM_ASKED_TO_MEET);
  Engine_EventSetMessage(MsgLaliveroPleaseWaitForMeOutside);
  Engine_EventShowMessage(IODEM, 0);
}

/// Both scenes, a doorway out: take the touched trigger's exit with the exit
/// sound.
static void LeaveBuilding(void) {
  struct EventWork *work = gEventWork;
  s16 *trigger = (s16 *)&work->touchedTrigger;

  Engine_EventRequestExit(*trigger);
  Engine_AudioPlayCue(SE_MAP_EXIT);
}

/// What the party can talk to, touch and search in each scene, before and
/// after Venus Lighthouse.
const struct SceneEvent *LaliveroInteriors_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_LaliveroInteriors2) {
    if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE) != 0) {
      return gFaranHouseEventsAfterLighthouse;
    }
    return gFaranHouseEvents;
  }
  if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE) != 0) {
    return gLaliveroEventsAfterLighthouse;
  }
  return gLaliveroEvents;
}

/// Lalivero (LaliveroInteriors2), entrance 99 back from Venus Lighthouse: the
/// party reports to Faran and Iodem. No sign of Sheba, nor of Jenna and the
/// others; Faran's faith that Sheba lived, what an Adept is, and a farewell as
/// the party sets out to search for Jenna.
static void ReportToFaran(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  Party_RestoreHpPp(1);
  // The party walks in to Faran and Iodem, and the others step out.
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(96), PIXELS(184));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(FARAN, FACING_EAST, 0);
  Actor_FaceDirection(IODEM, FACING_WEST, 0);
  Engine_EventSetMessage(MsgLaliveroHowDidSearchForSheba);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
  Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 104, 136);
  Engine_EventWait(10);
  FieldMotion_LaunchFromFocusedObject(GARET, -16, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(MIA, 0, 24, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(IVAN, 16, 16, FACING_NORTH);
  Engine_ActorWaitForMove(GARET);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  // Faran and Iodem tell of the search for Sheba.
  Actor_FaceDirection(FARAN, FACING_SOUTH, 0);
  Engine_ActorFaceDirection(IODEM, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(FARAN, EMOTE_IN_FRONT | 5, 70);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(FARAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(FARAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 2, 40);
  Engine_EventShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IODEM, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(IODEM, 0);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(FARAN, EMOTE_IN_FRONT | 5, 50);
  Engine_EventShowMessage(FARAN, 0);
  // The party tells what became of Jenna and the others; the version that saw
  // the ship from the lighthouse knows Idejima was swept away with it.
  if (GameFlag_IsSet(FLAG_SAW_SHIP_FROM_VENUS_LIGHTHOUSE) == 0) {
    TellFaranAboutJenna();
  } else {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(FARAN, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(FARAN, 0);
    Engine_EventWait(10);
    Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 50);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 60);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(GARET, FACING_NORTH, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(GARET, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Engine_EventShowMessage(MIA, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(IVAN, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(IVAN, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 1, 40);
    Event_OpenMessage(IODEM, 0);
    Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(IODEM, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(IODEM, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(GARET, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Engine_EventShowMessage(MIA, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventShowMessage(IVAN, 0);
    Engine_EventWait(10);
    Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 30);
    Event_OpenMessage(IVAN | SPEAKER_PORTRAIT_RIGHT, 0);
  }
  // Will the party keep searching? Faran answers either way.
  Engine_EventSetMessage(MsgLaliveroDoNotWorryAboutSheba);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 20);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTH, 0);
    Engine_ActorFaceDirection(IVAN, FACING_NORTH, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(FARAN, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventShowMessage(FARAN, 0);
    (gEventWork->message)++;
  } else {
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTH, 0);
    Engine_ActorFaceDirection(IVAN, FACING_NORTH, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(FARAN, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    (gEventWork->message)++;
    Event_ShowMessage(FARAN, 0);
  }
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(FARAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  Engine_EventWait(40);
  Engine_EventWait(10);
  // Faran believes Sheba lived; Iodem explains Adepts.
  Actor_ShowEmote(FARAN, EMOTE_IN_FRONT | 2, 40);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Engine_EventWait(50);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(FARAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(FARAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 40);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(20);
  Engine_ActorFaceDirection(FARAN, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IODEM, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(IODEM, FACING_WEST, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(FARAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(FARAN, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 2, 50);
  Engine_EventShowMessage(IODEM, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(FARAN, EMOTE_IN_FRONT | 1, 65);
  Engine_ActorFaceDirection(FARAN, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(IODEM, EMOTE_IN_FRONT, 40);
  Engine_EventShowMessage(IODEM, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(IODEM, FACING_SOUTH, 0);
  Engine_EventWait(40);
  Engine_EventShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(FARAN, FACING_SOUTH, 0);
  Engine_EventWait(50);
  Actor_ShowEmote(FARAN, EMOTE_IN_FRONT | 6, 60);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(FARAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(FARAN, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IODEM, 2);
  Engine_EventWait(30);
  Actor_FaceDirection(IODEM, FACING_WEST, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(FARAN, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(IODEM, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(IODEM, EMOTE_IN_FRONT | 5, 60);
  Engine_EventShowMessage(IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(FARAN, FACING_SOUTH, 0);
  Engine_EventWait(40);
  Actor_ShowEmote(FARAN, EMOTE_IN_FRONT | 2, 40);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(IVAN, FACING_WEST, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 7, 60);
  Engine_ActorFaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(FARAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(10);
  // Faran sends them off.
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(FARAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(FARAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(FARAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Engine_ActorFaceDirection(IVAN, FACING_NORTHWEST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  // The others walk back into the leader.
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(MIA, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ActorSetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ActorSetDestination(IVAN, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  Engine_ActorSetPosition(IVAN, 0, 0);
  Engine_EventWait(10);
  Engine_EventEnd();
}

/// The party tells Faran and Iodem what became of Jenna and the others, when
/// it did not see the ship from the lighthouse: Felix said he sent Jenna
/// ahead to the ship.
static void TellFaranAboutJenna(void) {
  Engine_EventSetMessage(MsgLaliveroTellOthers);
  Engine_EventWait(20);

  Engine_ActorRunRepeatedMotion(FARAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(FARAN, 0);
  Engine_EventWait(10);

  Engine_ActorFaceActor(GARET, ACTOR_PARTY_LEADER, 50);
  Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 60);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);

  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(10);

  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);

  Engine_ActorRunRepeatedMotion(IODEM, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(IODEM, 0);
  Engine_EventWait(20);

  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  Engine_EventWait(25);

  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(30);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(30);

  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);

  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(10);

  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);

  Engine_ActorFaceActor(IVAN, ACTOR_PARTY_LEADER, 30);
  Engine_EventOpenMessage(IVAN | SPEAKER_PORTRAIT_RIGHT, 0);
}

/// Sets both scenes up as the party enters: shopkeepers and the innkeeper
/// behind their counters, the Sanctum as the place to wake after a defeat,
/// the maid back at her chores, and the report to Faran on coming back from
/// Venus Lighthouse.
s32 Scene_Initialize(void) {
  s32 scene;
  s16 entrance;
  struct FieldActor *actor;

  if (gGameState.entrance == ENTRANCE_FROM_AERIE) {
    Engine_GameFlagSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE);
  }
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  scene = gGameState.scene;
  if (scene == (s32)&SceneId_LaliveroInteriors1) {
    // The shops and the sanctum.
    actor = Object_GetById(SHOPKEEPER_34);
    actor->priorityFlags = 0;
    actor->collisionFlags |= ACTOR_BEHIND_COUNTER;
    actor->sprite->image.priority = 2;
    actor = Object_GetById(SHOPKEEPER_32);
    actor->priorityFlags = 0;
    actor->collisionFlags |= ACTOR_BEHIND_COUNTER;
    actor->sprite->image.priority = 2;
    actor = Object_GetById(SHOPKEEPER_33);
    actor->collisionFlags |= ACTOR_BEHIND_COUNTER;
    actor->priorityFlags = 0;
    actor->sprite->image.priority = 2;
    Engine_ActorSetAnimation(ACTOR_15, 6);
    entrance = gGameState.entrance;
    if (entrance == ENTRANCE_SANCTUM) {
      gGameState.savedScene = scene;
      gGameState.savedEntrance = entrance;
    }
  }
  if (gGameState.scene == (s32)&SceneId_LaliveroInteriors2) {
    actor = Object_GetById(INNKEEPER);
    actor->collisionFlags |= ACTOR_BEHIND_COUNTER;
    actor->priorityFlags = 0;
    actor->sprite->image.priority = 2;
    if (Engine_GameFlagIsSet(FLAG_MAID_APOLOGIZED) != 0) {
      Actor_SetActionScript(MAID, gLaliveroMaidChores);
    }
    if (gGameState.entrance == ENTRANCE_REPORT_TO_FARAN) {
      ReportToFaran();
      // Iodem and Faran stay, with behavior 6 as in their later placements.
      Engine_ActorSetActionCallback(Object_GetById(IODEM), 6);
      Engine_ActorSetActionCallback(Object_GetById(FARAN), 6);
      gGameState.entrance = ENTRANCE_FARAN_HOUSE;
    }
  }
  return 0;
}

/// The maid's chores once she has apologized: she paces between two spots,
/// turning to each side, and sighs now and then.
s32 gLaliveroMaidChores[] = {
    SC_SET(SW_COLLISION, 129),
    SC_SET(SW_SPEED, 0x26666),
    SC_SET(SW_ACCEL, 0x13333),
    SC_LABEL(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(128), 0, PIXELS(300)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_ARG(3),
    SC_WAIT(30),
    SC_ARG(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(300)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_ARG(3),
    SC_WAIT(50),
    SC_ARG(2),
    SC_MOVE(PIXELS(128), 0, PIXELS(300)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_ARG(3),
    SC_WAIT(20),
    SC_CALL(Maid_Sigh),
    SC_WAIT(30),
    SC_ARG(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(300)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_ARG(3),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct SceneEntrance gLaliveroEntrances[] = {
    {1, CONDITION_ALWAYS, 144, 0, 168, FACING_NORTH, 0, 0, 8, 240, 192, 0},
    {2, CONDITION_ALWAYS, 400, 0, 168, FACING_NORTH, 0, 256, 8, 496, 192, 0},
    {3, CONDITION_ALWAYS, 648, 0, 72, FACING_NORTH, 0, 488, 8, 728, 184, 0},
    {4, CONDITION_ALWAYS, 128, 0, 472, FACING_NORTH, 0, 0, 216, 240, 488, 0},
    {5, CONDITION_ALWAYS, 136, 0, 296, FACING_SOUTH, 0, 0, 216, 240, 488, 0},
    {6, CONDITION_ALWAYS, 368, 0, 472, FACING_NORTH, 0, 240, 216, 480, 488, 0},
    {7, CONDITION_ALWAYS, 392, 0, 296, FACING_SOUTH, 0, 240, 216, 480, 488, 0},
    {8, CONDITION_ALWAYS, 648, 0, 280, FACING_NORTH, 0, 480, 216, 720, 488, 0},
    {9, CONDITION_ALWAYS, 96, 0, 680, FACING_NORTH, 0, 8, 520, 312, 696, 0},
    {10, CONDITION_ALWAYS, 200, 0, 600, FACING_SOUTH, 0, 8, 520, 312, 696, 0},
    {11, CONDITION_ALWAYS, 432, 0, 728, FACING_NORTH, 0, 312, 520, 552, 744, 0},
    {12, CONDITION_ALWAYS, 432, 0, 630, FACING_NORTH, 0, 312, 520, 552, 744, 0},
    {21, CONDITION_ALWAYS, 96, 0, 168, FACING_NORTH, 0, 8, 8, 312, 192, 0},
    {22, CONDITION_ALWAYS, 216, 0, 88, FACING_SOUTH, 0, 8, 8, 312, 192, 0},
    {23, CONDITION_ALWAYS, 224, 0, 392, FACING_NORTH, 0, 8, 232, 312, 416, 0},
    {24, CONDITION_ALWAYS, 88, 0, 312, FACING_SOUTH, 0, 8, 232, 312, 416, 0},
    {25, CONDITION_ALWAYS, 384, 0, 368, FACING_NORTH, 0, 320, 240, 632, 400, 0},
    {26, CONDITION_ALWAYS, 456, 0, 376, FACING_NORTH, 0, 320, 240, 632, 400, 0},
    {27, CONDITION_ALWAYS, 504, 0, 376, FACING_NORTH, 0, 320, 240, 632, 400, 0},
    {28, CONDITION_ALWAYS, 472, 0, 72, FACING_SOUTH, 0, 344, 8, 584, 168, 0},
    {29, CONDITION_ALWAYS, 744, 0, 72, FACING_SOUTH, 0, 632, 8, 872, 168, 0},
    {30, CONDITION_ALWAYS, 832, 0, 872, FACING_NORTH, 0, 280, 504, 912, 896, 0},
    {31, CONDITION_ALWAYS, 464, 0, 832, FACING_SOUTH, 0, 280, 504, 912, 896, 0},
    {99, CONDITION_ALWAYS, 96, 0, 168, FACING_NORTH, 0, 8, 8, 312, 192, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct SceneRegion gLaliveroSanctumRegions[] = {
    {196, 28, 595, 204, 36, 603, CONDITION_ALWAYS, 10},
    {SCENE_TABLE_END},
};

struct SceneRegion gLaliveroRegions[] = {
    {212, 28, 83, 220, 36, 91, CONDITION_ALWAYS, 22},
    {SCENE_TABLE_END},
};

u32 gLaliveroExits[] = {
    SCENE_EXITS(179),
    SCENE_EXIT(1, 178, 2),
    SCENE_EXIT(2, 178, 10),
    SCENE_EXIT(3, 179, 5),
    SCENE_EXIT(4, 178, 5),
    SCENE_EXIT(5, 179, 3),
    SCENE_EXIT(6, 178, 4),
    SCENE_EXIT(7, 179, 8),
    SCENE_EXIT(8, 179, 7),
    SCENE_EXIT(9, 178, 9),
    SCENE_EXIT(10, 178, 11),
    SCENE_EXIT(11, 178, 6),
    SCENE_EXIT(0, 180, 0),
    SCENE_EXIT(21, 178, 7),
    SCENE_EXIT(22, 178, 8),
    SCENE_EXIT(23, 178, 3),
    SCENE_EXIT(24, 180, 25),
    SCENE_EXIT(25, 180, 24),
    SCENE_EXIT(26, 180, 28),
    SCENE_EXIT(27, 180, 29),
    SCENE_EXIT(28, 180, 26),
    SCENE_EXIT(29, 180, 27),
    SCENE_EXIT(30, 178, 1),
    SCENE_EXIT(31, 188, 1),
    SCENE_EXITS_END,
};

struct ScenePlacement gLaliveroPlacements[] = {
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(176), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(80), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x8f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(136), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(120), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, 3,
        PIXELS(616), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(664), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(400), 0, PIXELS(416), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(550), 0, PIXELS(368), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(304), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(600), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(616), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(92), 0, PIXELS(616), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x76, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(68), 0, PIXELS(340), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(432), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gFaranHousePlacements[] = {
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(112), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(240), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x94, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(88), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x46, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x39, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(336), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x80, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(300), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(544), 0, PIXELS(368), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gFaranHousePlacementsAfterLighthouse[] = {
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(112), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(240), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x94, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x46, CONDITION_ALWAYS, 6,
        PIXELS(104), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x39, CONDITION_ALWAYS, 6,
        PIXELS(128), 0, PIXELS(96), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x77, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(336), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x80, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(160), 0, PIXELS(304), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(544), 0, PIXELS(368), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gLaliveroEvents[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsOfCourseIHaveSeenActor) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThereAreSomeWhoDoNot) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsCouldFaranHaveTrickedUsAll) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfActorCantUseHerPowers) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorCameFlyingToTheRuins) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEvenActorCouldntHaveEscapedSix) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheySayAManNamedIodem) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsNoOnesCalledMeSoMaybe) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorSaidTheCaveAtThe) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheRuinsThatFormTheLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorIfThePowerThatYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorIsTheSaviorOfLalivero) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsALegendarySaviorWillAppearIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIdLikeToBelieveInActor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEvenThoughSheFellOutOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfSomethingHappensToActorWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhenIodemGetsHereHesGoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsItsBeenSoLongSinceI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIWonderHowActorKnewAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThoseCavesAreTooDangerousFor) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(TalkToShopkeeper32) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(TalkToShopkeeper33) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(TalkToShopkeeper34) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheLaliveransAreATimidPeople) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsICantRescueActorIfI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfAnythingHappensToActorIll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIBetBabisTowerWillBe) },
    { SEARCH(SEARCH_BARREL), 100, 0xfb9, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_BARREL), 101, 0xfba, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingMushroomSoupInLalivero)) },
    { SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsALaliveroSaladWithPotato)) },
    { SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsThePotIsFullOfA)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gLaliveroEventsAfterLighthouse[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheContinentBrokeApartAroundThe) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhatHappenedToThoseLivingOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIShudderToThinkWhatWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWereLuckyThatWeLiveNorth) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEveryoneSaysThatActorIsOkay) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIRealizedHowThoughtlessIWas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfSheIsIndeedTheChild) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeWantToBelieveInSomething) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorShouldBeFineSheWas) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorWillWakeWhenTheBeacon) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSeeingActorDiveMustHaveBeen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsICantWaitUntilActorIs) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsBabiLighthouseCrumbledWhenAllThe) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIAlwaysWantedABreakBut) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSoTheRuinsReallyAreConnected) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouFinallySolvedTheMysteryOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWillTheyContinueOnOrQuit) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsImWorriedThatWeWontGet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeCantPossiblyDigAHole) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheMysteryOfTheRuinsShould) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(TalkToShopkeeper32) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(TalkToShopkeeper33) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(TalkToShopkeeper34) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(TalkToPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsNoOneCouldPossiblySwimIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhenWillTheOceanBeCalm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheChampaArePeopleFromA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFirstTheLighthouseWasStruckDown) },
    { SEARCH(SEARCH_BARREL), 100, 0xfb9, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_BARREL), 101, 0xfba, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingMushroomSoupInLalivero)) },
    { SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsALaliveroSaladWithPotato)) },
    { SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsThePotIsFullOfA)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gFaranHouseEvents[] = {
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 25, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 28, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 29, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeReceivedActorFromTheSky) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIJustKnowThatFatherWill) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsHisWifeSeemsVeryCalmConsidering) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsItsSoPainfulToEvenThink) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIJustWishSomeoneWouldPlease) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThatCoupleRaisedActorAsThough) },
    { EVENT_TALK, 14, FLAG_MAID_APOLOGIZED, EVENT_SCRIPT(TalkToMaid) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40e, FLAG_MAID_APOLOGIZED, EVENT_SCRIPT(TalkToMaid) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(TalkToInnkeeper) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWorkingLikeThisKeepsMyMind) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeMustObeyTolbiThatsWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyDidThisHaveToHappen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsHousecleaningChangingSheetsItsAllDone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsOnceThisIncidentIsOverI) },
    { SEARCH(SEARCH_BARREL), 102, 0xfbb, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesFryingFreshwaterFishFromThe)) },
    { SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsRiceMadeWithGrainsFrom)) },
    { SEARCH(SEARCH_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingRoastChickenThisScent)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gFaranHouseEventsAfterLighthouse[] = {
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 25, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 28, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 29, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveBuilding) },
    { EVENT_TOUCH, 51, FLAG_IODEM_ASKED_TO_MEET, EVENT_SCRIPT(IodemCallsOut) },
    { EVENT_TALK, 12, FLAG_IODEM_FAREWELL_SEEN, EVENT_SCRIPT(TalkToIodem) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_IODEM_FAREWELL_SEEN, MSGNO(MsgLaliveroInteriorsIMustExplainTheirPromiseTo) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFaranSaysKnowingHerPowersActor) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouAreLeavingAfterAllArent) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheySayTheLighthouseCrumbledDown) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouWillFindJennaSomedayFarewell) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIsaacThatBoatBelongsToYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIDontNeedFameOrFortune) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyWontAnyoneStopIsaacAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTolbiIsToBeBlamedFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSoThereAreSomePeopleLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsPleaseUseThatShipToFind) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(TalkToInnkeeper) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIMustSayItIsNice) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFishingIsSoDifficultNowadaysBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyWouldTheLightingMakeSuch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsImSleepyTheseBrightNightsHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsGeeWhatAmIGoingTo) },
    { SEARCH(SEARCH_BARREL), 102, 0xfbb, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesFryingFreshwaterFishFromThe)) },
    { SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsRiceMadeWithGrainsFrom)) },
    { SEARCH(SEARCH_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingRoastChickenThisScent)) },
    { SCENE_EVENTS_END },
};
// clang-format on

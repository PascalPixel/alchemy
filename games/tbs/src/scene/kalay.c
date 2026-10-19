#include "game_state.h"
#include "unit/field/field.h"
#include "engine_call.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/kalay.h"
#include "unit/scene/common/object_orbit.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
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
#include "edition.h"
#include "callback_scheduler.h"
#include "field_event.h"
#include "field_scene.h"
#include "resource_3a8_effect.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "script.h"
#include "macros.h"

#include "field_obj.h"
#include "object_effect.h"
#include "orbit.h"
#include "fixed_math.h"
#include "field_runtime.h"

#include "flags.h"
#include "songs.h"

enum { KALAY_CALLBACK_PRIORITY = 3200 };

/// Kalay's maps share actor numbers; each is named on the map it is proved.
enum KalayActor {
  // Town
  ACTOR_LATE_TOURIST = 14, ///< the one the tour guides are missing
  ACTOR_TOLBI_EMIGRANT = 19,
  ACTOR_HEAD_GUIDE = 20, ///< the Colosso tour's guides
  ACTOR_SECOND_GUIDE = 21,
  ACTOR_WEAPON_MERCHANT = 22,
  ACTOR_ARMOR_MERCHANT = 23,
  ACTOR_ITEM_MERCHANT = 24,
  ACTOR_TOWN_NUT = 26,
  /// Hammet's palace gate
  ACTOR_PALACE_GUARD = 8,
  ACTOR_SECOND_PALACE_GUARD = 9,
  /// The cave by the palace's secret passage
  ACTOR_CAVE_GUARD = 8,
  ACTOR_WAGON = 9,
  ACTOR_HAMMET = 10,
  ACTOR_BUNZA = 11,
  ACTOR_CAVE_NUT = 12,
  // The tunnel
  ACTOR_FIRST_BLOCK = 8, ///< the blocks that open the two passages
  ACTOR_SECOND_BLOCK = 9,
  ACTOR_FROST_TARGET = 11,
  ACTOR_TUNNEL_DJINNI = 12
};

/// The tunnel's own flags, kept while the party is there.
enum KalayTunnelFlag {
  FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER = FLAG_SCENE_TEMP_00, ///< the fade overlay
  FLAG_TUNNEL_FROST_TARGET_FROZEN = FLAG_SCENE_TEMP_01,
  /// Each block's particles have started their sound.
  FLAG_TUNNEL_FIRST_PARTICLES_SOUNDED = FLAG_SCENE_TEMP_02,
  FLAG_TUNNEL_SECOND_PARTICLES_SOUNDED = FLAG_SCENE_TEMP_03,
  FLAG_TUNNEL_FIRST_PASSAGE_OPEN = 0x302,
  FLAG_TUNNEL_SECOND_PASSAGE_OPEN = 0x303
};

// Forward declarations.
static void Kalay_PrepareTown(void);
static void Kalay_PreparePalaceGate(void);
static void Kalay_PrepareCave(void);
static void Kalay_StartWindowTransition(void);
static void Kalay_PrepareTunnel(void);
static void Kalay_StartTouristsFidgeting(void);
static void Kalay_StartTouristFidgeting(s32 actorId);
static void Kalay_StartGuidesPacing(void);
static void Kalay_AlternateGuidesPacing(void);
static void Kalay_IvanMeetsPalaceGuards(void);
static void Kalay_HammetReturnsToPalace(void);
static void Kalay_UpdateTunnelPassages(void);
static void Kalay_UpdateTunnelEmitters(void);
static void Kalay_SpawnTunnelParticle(s32 x, s32 height, s32 z);
static void Kalay_UpdateNutOrbit(void);

/// Clear the flag a guide's script sets when it is done.
static __inline__ void Kalay_ClearScriptDone(struct FieldActor *actor) {
  s32 completed = 0;

  actor->timer = completed;
}

/// Talking to the Mars Djinni in the tunnel adds it to the party.
static void Kalay_RecruitDjinn(void) {
  Djinn_JoinParty(ACTOR_TUNNEL_DJINNI, ELEMENT_MARS, 3);
}

/// While the tour waits, a tourist now and then nods, shakes his head or turns.
static s32 Kalay_UpdateTouristFidget(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  u32 action;
  u32 nextTimer;

  if ((s16)actor->work == 0) {
    action = (u32)(Engine_RandomNext() * 8) >> 0x10;
    switch (action) {
    case 0:
      Object_SetMode(actor, 3);
      break;
    case 1:
      Object_SetMode(actor, 4);
      break;
    case 3:
    case 4:
      actor->facing += (u32)(Engine_RandomNext() << 0xF) >> 0x10;
      break;
    }
    nextTimer = (u32)(Engine_RandomNext() * 0x50) >> 0x10;
    actor->work = nextTimer;
  }
  if ((s16)actor->work != 0) {
    actor->work = (s16)actor->work - 1;
  }
  return 1;
}

// Shared source, included in link order.
#include "common/object_heading.h"

/// Kalay is six maps: the town, the palace gate, the cave by the palace's
/// secret passage, two more maps and the tunnel with the two blocks.
const struct SceneEntrance *Kalay_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Kalay1) {
    return gKalay_TownEntrances;
  }
  if (scene == (s32)&SceneId_Kalay2) {
    return gKalay_PalaceGateEntrances;
  }
  if (scene == (s32)&SceneId_Kalay3) {
    return gKalay_CaveEntrances;
  }
  if (scene == (s32)&SceneId_Kalay4) {
    return gKalay_Entrances4;
  }
  if (scene == (s32)&SceneId_Kalay5) {
    return gKalay_Entrances5;
  }
  if (scene == (s32)&SceneId_Kalay6) {
    return gKalay_TunnelEntrances;
  }
  return gKalay_NoEntrances;
}

/// Kalay has no scene regions.
const struct SceneRegion *Kalay_GetRegions(void) { return NULL; }

/// The numbered exits of all six maps.
u8 *Kalay_GetExits(void) { return (u8 *)gKalay_Exits; }

/// The people of the town, palace gate, cave and tunnel.
const struct ScenePlacement *Kalay_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Kalay1) {
    return gKalay_TownPlacements;
  }
  if (scene == (s32)&SceneId_Kalay2) {
    return gKalay_PalaceGatePlacements;
  }
  if (scene == (s32)&SceneId_Kalay3) {
    return gKalay_CavePlacements;
  }
  if (scene == (s32)&SceneId_Kalay6) {
    return gKalay_TunnelPlacements;
  }
  return gKalay_NoPlacements;
}

/// Searching the town well: the party peers in, and it is very clean.
static void Kalay_SearchWell(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgKalayVeryCleanMaintained, 1);
  Engine_EventEnd();
}

/// Catch pulls in the Nut floating in the cave.
static void Kalay_CatchCaveNut(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(ACTOR_CAVE_NUT, 0, 0);
  Engine_GameFlagSet(0xfd6);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

/// The conversations, exits and Psynergy targets of each map.
const struct SceneEvent *Kalay_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Kalay1) {
    return gKalay_TownEvents;
  }
  if (scene == (s32)&SceneId_Kalay2) {
    return gKalay_PalaceGateEvents;
  }
  if (scene == (s32)&SceneId_Kalay3) {
    return gKalay_CaveEvents;
  }
  if (scene == (s32)&SceneId_Kalay4) {
    return gKalay_Events4;
  }
  if (scene == (s32)&SceneId_Kalay5) {
    return gKalay_Events5;
  }
  if (scene == (s32)&SceneId_Kalay6) {
    return gKalay_TunnelEvents;
  }
  return gKalay_NoEvents;
}

/// A man tells why he moved to Kalay: he left Tolbi, afraid of Babi.
static void Kalay_TalkAboutLeavingHome(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayCameKalayBecauseDidntLike);
  Engine_EventAskYesNo(ACTOR_TOLBI_EMIGRANT, 0);
  Engine_EventEnd();
}

/// Spoken to from any side but the north, the cave guard asks whether the
/// party wants to go into the cave, and turns it away either way.
static void Kalay_AskAboutCaveAccess(void) {
  struct FieldActor *leader;
  u32 relativeFacing;
  u16 *message;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  relativeFacing = (leader->facing + 0xfffff000) << 16;
  if (relativeFacing > 0x60000000) {
    Engine_EventBegin();
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_CAVE_GUARD, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgKalayDoWantGoCaveUp);
    Engine_EventOpenMessage(ACTOR_CAVE_GUARD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_ActorSetAnimationAndWait(ACTOR_CAVE_GUARD, ANIM_SHAKE_HEAD);
      Engine_EventShowMessage(ACTOR_CAVE_GUARD, 0);
    } else {
      message = &gEventWork->message;
      *message = *message + 1;
      Engine_ActorSetAnimationAndWait(ACTOR_CAVE_GUARD, ANIM_NOD);
      Engine_EventShowMessage(ACTOR_CAVE_GUARD, 0);
    }
    Engine_EventEnd();
  }
}

/// In the tunnel, a man asks whether Layana was too hard on the party.
static void Kalay_TalkAboutLayana(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayLayanaWasVeryHardOn);
  Engine_EventAskYesNo(10, 0);
  Engine_EventEnd();
}

/// Across the counter, the leader facing north, the weapon shop opens; from
/// elsewhere the merchant grumbles that Hammet sells his best weapons away.
static void Kalay_TalkToWeaponMerchant(void) {
  struct FieldActor *leader;
  u32 relativeFacing;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  relativeFacing = (leader->facing - 0x2000) << 16;
  if (relativeFacing > 0x80000000) {
    Engine_ShopOpen(22, ACTOR_WEAPON_MERCHANT);
  } else {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgKalayLordHammetSellsHisBest);
    Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
    Engine_EventEnd();
  }
}

/// Across the counter, the leader facing west to north, the armor shop opens;
/// from elsewhere the merchant asks whether the party found its weapons.
static void Kalay_TalkToArmorMerchant(void) {
  struct FieldActor *leader;
  u32 relativeFacing;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  relativeFacing = (leader->facing - 0x6001) << 16;
  if (relativeFacing <= 0x7ffe0000) {
    Engine_ShopOpen(23, ACTOR_ARMOR_MERCHANT);
  } else {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgKalayDidFindNeededInWeapon);
    Engine_EventAskYesNo(ACTOR_ARMOR_MERCHANT, 0);
    Engine_EventEnd();
  }
}

/// Across the counter, the leader facing east, the item shop opens; from
/// elsewhere the merchant praises Lady Layana.
static void Kalay_TalkToItemMerchant(void) {
  struct FieldActor *leader;
  u32 relativeFacing;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  relativeFacing = (leader->facing - 0x2000) << 16;
  if (relativeFacing > 0xC0000000) {
    Engine_ShopOpen(24, ACTOR_ITEM_MERCHANT);
  } else {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgKalayLadyLayanaSharedInLord);
    Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
    Engine_EventEnd();
  }
}

/// Walking north into a house door, everyone stops, the door opens and the
/// leader walks in.
static void Kalay_OpenDoorAndExit(void) {
  struct EventWork *event;
  struct FieldActor *actor;
  s16 *trigger;
  u32 i;
  s32 door;

  event = gEventWork;
  Engine_EventBegin();
  for (i = ACTOR_FIRST_PLACED; i <= 65; i++) {
    actor = Object_GetById(i);
    if (actor != NULL) {
      actor->motionFlags = 0;
    }
  }
  trigger = &event->touchedTrigger;
  door = *trigger - 1;
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  {
    s32 x = gKalay_DoorCells[door].x;
    s32 y = gKalay_DoorCells[door].y;

    Engine_MapAnimateCells(gKalay_DoorCells[door].steps, x, y);
  }
  Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
  Engine_EventWait(10);
  Engine_EventRequestExit(*trigger);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

/// The palace door opens in two frames.
static __inline__ void Kalay_AnimatePalaceDoor(void) {
  Event_PlayCue(SE_188);
  Map_CopyCellsTo(36, 23, 43, 12, 2, 2);
  Engine_TaskWait(5);
  Map_CopyCellsTo(39, 23, 43, 12, 2, 2);
  Engine_TaskWait(5);
}

/// Walking north into the palace door, it opens and the leader walks in.
static void Kalay_EnterPalace(void) {
  Engine_EventBegin();
  Kalay_AnimatePalaceDoor();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
  Engine_EventWait(10);
  Engine_EventRequestExit(2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

/// Leaving town (triggers 9 and 10) after hearing of the Colosso tour, the
/// party watches the tour group stop: the guides count everyone, pace back and
/// forth while the tourists grumble, give up on the missing man and lead the
/// group off. The late tourist then runs after it. Otherwise the party leaves.
static void Kalay_WatchTourGroupDeparture(void) {
  s32 message;
  struct FieldActor *headWait;
  struct FieldActor *lastWait;
  struct FieldActor *closing;
  struct FieldActor *late;
  struct FieldActor *headEnd;
  struct FieldActor *lastEnd;
  struct FieldActor *leader;
  struct FieldActor *last;
  struct FieldActor *head;
  struct FieldActor *headBack;
  struct FieldActor *headAgain;
  struct FieldActor *headHome;
  struct EventWork *work;

  work = gEventWork;
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_KALAY_HEARD_OF_COLOSSO_TOUR) != 0 &&
      GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) == 0) {
    Event_CallWithLastActiveObjectId(gKalay_TourGroupPlacements);
    Actor_SetPosition(ACTOR_HEAD_GUIDE, PIXELS(252), PIXELS(272));
    Actor_SetPosition(27, PIXELS(284), PIXELS(264));
    Actor_SetPosition(28, PIXELS(284), PIXELS(280));
    Actor_SetPosition(29, PIXELS(300), PIXELS(264));
    Actor_SetPosition(30, PIXELS(300), PIXELS(280));
    Actor_SetPosition(32, PIXELS(316), PIXELS(264));
    Actor_SetPosition(31, PIXELS(316), PIXELS(280));
    Actor_SetPosition(33, PIXELS(332), PIXELS(264));
    Actor_SetPosition(34, PIXELS(332), PIXELS(280));
    Actor_SetPosition(ACTOR_SECOND_GUIDE, PIXELS(364), PIXELS(272));
    Event_PlayCue(MUS_017);
    Ui_SetMessageVoiceFromActor(ACTOR_HEAD_GUIDE);
    UiText_ShowCenteredMessage(MsgKalayOkListeningLets, 1, 0);
    Event_PlayCue(MUS_009);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    if (work->touchedTrigger == 9) {
      Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
    } else {
      Camera_SetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
    }
    // Stagger the speeds so the group arrives in rows.
    Actor_SetSpeed(ACTOR_HEAD_GUIDE, PIXELS(11) / 10, PIXELS(11) / 20);
    Actor_SetSpeed(27, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(28, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(29, PIXELS(9) / 10, PIXELS(9) / 20);
    Actor_SetSpeed(30, PIXELS(9) / 10, PIXELS(9) / 20);
    Actor_SetSpeed(32, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(31, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(33, PIXELS(7) / 10, PIXELS(7) / 20);
    Actor_SetSpeed(34, PIXELS(7) / 10, PIXELS(7) / 20);
    Actor_SetSpeed(ACTOR_SECOND_GUIDE, PIXELS(3) / 5, PIXELS(3) / 10);
    Actor_SetActionScript(ACTOR_HEAD_GUIDE, gKalay_TourGroupWalkScript);
    Actor_SetActionScript(27, gKalay_TourGroupWalkScript);
    Actor_SetActionScript(28, gKalay_TourGroupWalkScript);
    Actor_SetActionScript(29, gKalay_TourGroupWalkScript);
    Actor_SetActionScript(30, gKalay_TourGroupWalkScript);
    Actor_SetActionScript(32, gKalay_TourGroupWalkScript);
    Actor_SetActionScript(31, gKalay_TourGroupWalkScript);
    Actor_SetActionScript(33, gKalay_TourGroupWalkScript);
    Actor_SetActionScript(34, gKalay_TourGroupWalkScript);
    last = Actor_Get(ACTOR_SECOND_GUIDE);
    last->timer = 0;
    Actor_SetActionScript(ACTOR_SECOND_GUIDE, gKalay_TourGroupWalkScript);
    Camera_MoveTo(PIXELS(186), -1, PIXELS(272), 1);
    Object_RefreshSelectorById(ACTOR_HEAD_GUIDE);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_EAST, 0);
    do {
      Engine_TaskWait(1);
    } while (last->timer == 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(27, 2);
    Actor_FaceDirection(27, FACING_SOUTH + FACING_STEP, 20);
    Engine_EventSetMessage(MsgKalayWhyWeStoppingAtPlace);
    Event_ShowMessageAndWait(27, 0, 10);
    Engine_ActorRunRepeatedMotion(28, 2);
    Actor_FaceDirection(28, FACING_NORTHWEST + FACING_STEP, 10);
    Engine_ActorSetAnimation(28, ANIM_NOD);
    Event_ShowMessageAndWait(28, 0, 10);
    Actor_SetAttachedEffect(32, EMOTE_IN_FRONT | 2);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(32, 0, 10);
    Actor_ShowEmote(31, EMOTE_IN_FRONT, 40);
    Actor_FaceDirection(31, FACING_NORTHWEST + FACING_STEP, 10);
    Event_ShowMessageAndWait(31, 0, 10);
    Actor_FaceDirection(31, FACING_WEST, 10);
    Engine_ActorRunRepeatedMotion(31, 2);
    Engine_ActorSetAnimation(31, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(31, 0, 10);
    Actor_FaceDirection(31, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(32, FACING_SOUTH + FACING_STEP, 20);
    Engine_ActorSetAnimation(31, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(32, ANIM_NOD);
    Engine_ActorRunRepeatedMotion(ACTOR_HEAD_GUIDE, 2);
    Actor_SetAttachedEffect(ACTOR_HEAD_GUIDE, EMOTE_IN_FRONT | 2);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(ACTOR_HEAD_GUIDE, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_HEAD_GUIDE, 2);
    head = Actor_Get(ACTOR_HEAD_GUIDE);
    head->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_HEAD_GUIDE, 172, 264);
    Engine_EventWait(1);
    headBack = Actor_Get(ACTOR_HEAD_GUIDE);
    headBack->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_FaceDirection(27, FACING_WEST, 0);
    Actor_FaceDirection(28, FACING_WEST, 0);
    Actor_FaceDirection(32, FACING_WEST, 0);
    Actor_FaceDirection(31, FACING_WEST, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_NOD);
    Engine_EventWait(20);
    headAgain = Actor_Get(ACTOR_HEAD_GUIDE);
    headAgain->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_HEAD_GUIDE, 172, 272);
    Engine_EventWait(1);
    headHome = Actor_Get(ACTOR_HEAD_GUIDE);
    headHome->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_HEAD_GUIDE, 180, 272);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_EAST, 0);
    Event_ShowMessageAndWait(ACTOR_HEAD_GUIDE, 0, 10);
    Actor_ShowEmote(34, EMOTE_IN_FRONT | 5, 0);
    Engine_ActorRunRepeatedMotion(34, 1);
    Engine_ActorSetAnimationAndWait(34, ANIM_NOD);
    Event_ShowMessageAndWait(34, 0, 10);
    Engine_ActorRunRepeatedMotion(33, 1);
    Event_ShowMessageAndWait(33, 0, 10);
    Engine_ActorSetAnimation(33, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(33, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_SECOND_GUIDE, 2);
    Actor_ShowEmote(ACTOR_SECOND_GUIDE, EMOTE_IN_FRONT | 2, 0);
    Event_ShowMessageAndWait(ACTOR_SECOND_GUIDE, 0, 10);
    Engine_ActorJump(ACTOR_HEAD_GUIDE, 2, 20);
    Engine_ActorJump(ACTOR_HEAD_GUIDE, 4, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_HEAD_GUIDE, 2);
    Event_ShowMessageAndWait(ACTOR_HEAD_GUIDE, 0, 10);
    Actor_SetSpeed(ACTOR_SECOND_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_WalkToAndWait(ACTOR_SECOND_GUIDE, 265, 282);
    Actor_WalkToAndWait(ACTOR_SECOND_GUIDE, 251, 284);
    Actor_WalkToAndWait(ACTOR_SECOND_GUIDE, 246, 296);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTH, 0);
    // The tourists fidget while the guides count them.
    Kalay_StartTouristsFidgeting();
    Engine_EventWait(40);
    Actor_SetSpeed(ACTOR_SECOND_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_WalkToAndWait(ACTOR_SECOND_GUIDE, 228, 296);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTH, 40);
    Actor_WalkToAndWait(ACTOR_SECOND_GUIDE, 212, 296);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTH, 40);
    Actor_WalkToAndWait(ACTOR_SECOND_GUIDE, 192, 296);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTH, 40);
    Engine_ActorStartRepeatedMotion(ACTOR_SECOND_GUIDE, 2);
    Actor_ShowEmote(ACTOR_SECOND_GUIDE, EMOTE_IN_FRONT, 60);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_WalkToAndWait(ACTOR_SECOND_GUIDE, 184, 286);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTHWEST + FACING_STEP, 10);
    Actor_SetAttachedEffect(ACTOR_SECOND_GUIDE, EMOTE_IN_FRONT | 2);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_SECOND_GUIDE, ANIM_SHAKE_HEAD);
    Actor_ShowEmote(ACTOR_HEAD_GUIDE, EMOTE_IN_FRONT | 1, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_NOD);
    Engine_ActorRunRepeatedMotion(ACTOR_HEAD_GUIDE, 2);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_EAST, 60);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_NORTHWEST + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_SECOND_GUIDE, ANIM_NOD);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_EAST, 0);
    Actor_SetSpeed(ACTOR_HEAD_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_RunActionScriptAndWait(ACTOR_HEAD_GUIDE, gKalay_GuidePaceScript);
    Actor_WalkToAndWait(ACTOR_HEAD_GUIDE, 228, 296);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_NORTH, 40);
    Actor_WalkToAndWait(ACTOR_HEAD_GUIDE, 212, 296);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_NORTH, 40);
    Actor_WalkToAndWait(ACTOR_HEAD_GUIDE, 192, 296);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_NORTH, 40);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_NORTHWEST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_SECOND_GUIDE, FACING_SOUTHEAST + FACING_STEP, 10);
    Actor_SetAttachedEffect(ACTOR_HEAD_GUIDE, EMOTE_IN_FRONT | 2);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_SHAKE_HEAD);
    message = MsgKalayShortOnePerson;
    Engine_EventSetMessage(message);
    Event_ShowMessageAndWait(ACTOR_HEAD_GUIDE, 0, 40);
    // The guides pace while the tourists complain one by one.
    Kalay_StartGuidesPacing();
    Actor_WalkToAndWait(ACTOR_HEAD_GUIDE, 178, 272);
    Actor_FaceDirection(ACTOR_HEAD_GUIDE, FACING_EAST, 0);
    Engine_EventWait(240);
    Engine_ActorStop(27);
    Engine_TaskWait(1);
    Actor_FaceDirection(27, FACING_WEST, 10);
    Actor_ShowEmote(27, EMOTE_IN_FRONT | 1, 60);
    Event_ShowMessageAndWait(27, 0, 10);
    Kalay_StartTouristFidgeting(27);
    Engine_EventWait(80);
    Engine_ActorStop(28);
    Engine_TaskWait(1);
    Actor_FaceDirection(28, FACING_NORTH + FACING_STEP, 20);
    Engine_ActorRunRepeatedMotion(28, 2);
    Event_ShowMessageAndWait(28, 0, 10);
    Kalay_StartTouristFidgeting(28);
    Engine_EventWait(160);
    Engine_ActorStop(32);
    Engine_TaskWait(1);
    Actor_FaceDirection(32, FACING_SOUTH + FACING_STEP, 10);
    Actor_ShowEmote(32, EMOTE_IN_FRONT | 1, 60);
    Event_ShowMessageAndWait(32, 0, 10);
    Kalay_StartTouristFidgeting(32);
    Engine_EventWait(80);
    Engine_ActorStop(30);
    Engine_TaskWait(1);
    Actor_FaceDirection(30, FACING_NORTHWEST + FACING_STEP, 10);
    Engine_ActorRunRepeatedMotion(30, 1);
    Engine_EventSetMessage(message + 6);
    Event_ShowMessageAndWait(30, 0, 10);
    Scheduler_RemoveCallback(Kalay_AlternateGuidesPacing);
    Engine_ActorStop(ACTOR_HEAD_GUIDE);
    Engine_ActorStop(ACTOR_SECOND_GUIDE);
    Engine_TaskWait(1);
    headWait = Actor_Get(ACTOR_HEAD_GUIDE);
    headWait->timer = 0;
    lastWait = Actor_Get(ACTOR_SECOND_GUIDE);
    lastWait->timer = 0;
    Actor_SetSpeed(ACTOR_HEAD_GUIDE, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_SECOND_GUIDE, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetActionScript(ACTOR_HEAD_GUIDE, gKalay_HeadGuideStopScript);
    Actor_SetActionScript(ACTOR_SECOND_GUIDE, gKalay_SecondGuideStopScript);
    Engine_ActorStop(29);
    Engine_TaskWait(1);
    Actor_FaceDirection(29, FACING_SOUTH + FACING_STEP, 10);
    message += 5;
    Engine_ActorRunRepeatedMotion(29, 2);
    Engine_EventSetMessage(message);
    Event_ShowMessageAndWait(29, 0, 20);
    Kalay_StartTouristFidgeting(29);
    Kalay_StartTouristFidgeting(30);
    do {
      Engine_TaskWait(1);
    } while (Actor_Get(ACTOR_HEAD_GUIDE)->timer == 0 ||
             Actor_Get(ACTOR_SECOND_GUIDE)->timer != 1);
    Actor_SetActionScript(ACTOR_HEAD_GUIDE, gKalay_HeadGuideReturnScript);
    Actor_SetActionScript(ACTOR_SECOND_GUIDE, gKalay_SecondGuideReturnScript);
    Engine_ActorStop(31);
    Engine_TaskWait(1);
    Actor_FaceDirection(31, FACING_SOUTH + FACING_STEP, 10);
    Engine_ActorRunRepeatedMotion(31, 1);
    Engine_ActorSetAnimationAndWait(31, ANIM_SHAKE_HEAD);
    message = MsgKalayDecideEnteringColosso;
    Engine_EventSetMessage(message);
    Event_ShowMessageAndWait(31, 0, 10);
    Kalay_StartTouristFidgeting(31);
    Engine_ActorStop(34);
    Engine_ActorStop(33);
    Engine_TaskWait(1);
    Actor_ShowEmote(34, EMOTE_IN_FRONT | 5, 40);
    Actor_ShowEmote(33, EMOTE_IN_FRONT | 5, 60);
    Actor_FaceDirection(34, FACING_NORTHWEST + FACING_STEP, 10);
    Actor_FaceDirection(33, FACING_SOUTH + FACING_STEP, 10);
    message += 3;
    Engine_ActorSetAnimationAndWait(34, ANIM_SHAKE_HEAD);
    Engine_EventSetMessage(message);
    Event_ShowMessageAndWait(34, 0, 10);
    Engine_ActorRunRepeatedMotion(33, 1);
    Engine_ActorSetAnimation(33, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(33, 0, 10);
    Actor_ShowEmote(34, EMOTE_IN_FRONT | 2, 60);
    Actor_ShowEmote(ACTOR_HEAD_GUIDE, EMOTE_IN_FRONT | 3, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_HEAD_GUIDE, 2);
    Engine_EventSetMessage(MsgKalayThatsWeCantWaitAny);
    Event_ShowMessageAndWait(ACTOR_HEAD_GUIDE, 0, 10);
    Engine_ActorStop(27);
    Engine_ActorStop(28);
    Engine_ActorStop(29);
    Engine_ActorStop(30);
    Engine_ActorStop(32);
    Engine_ActorStop(31);
    Engine_ActorStop(33);
    Engine_ActorStop(34);
    Engine_ActorStop(ACTOR_HEAD_GUIDE);
    Engine_ActorStop(ACTOR_SECOND_GUIDE);
    Engine_TaskWait(1);
    Engine_ActorJump(27, 2, 0);
    Engine_ActorJump(28, 2, 0);
    Engine_ActorJump(29, 2, 0);
    Engine_ActorJump(30, 2, 0);
    Engine_ActorJump(32, 2, 0);
    Engine_ActorJump(31, 2, 0);
    Engine_ActorJump(33, 2, 0);
    Engine_ActorJump(34, 2, 0);
    Engine_ActorJump(ACTOR_SECOND_GUIDE, 2, 40);
    Actor_FaceDirection(27, FACING_WEST, 0);
    Actor_FaceDirection(28, FACING_WEST, 0);
    Actor_FaceDirection(29, FACING_WEST, 0);
    Actor_FaceDirection(30, FACING_WEST, 0);
    Actor_FaceDirection(32, FACING_WEST, 0);
    Actor_FaceDirection(31, FACING_WEST, 0);
    Actor_FaceDirection(33, FACING_WEST, 0);
    Actor_FaceDirection(34, FACING_WEST, 40);
    Engine_ActorJump(ACTOR_SECOND_GUIDE, 4, 40);
    Event_ShowMessageAndWait(ACTOR_SECOND_GUIDE, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_HEAD_GUIDE, 1);
    Event_ShowMessageAndWait(ACTOR_HEAD_GUIDE, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_SECOND_GUIDE, ANIM_NOD);
    Event_ShowMessageAndWait(ACTOR_SECOND_GUIDE, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_NOD);
    Event_ShowMessageAndWait(ACTOR_HEAD_GUIDE, 0, 10);
    Actor_ShowEmote(27, EMOTE_IN_FRONT | 2, 40);
    Engine_ActorStartRepeatedMotion(27, 1);
    Event_ShowMessageAndWait(27, 0, 10);
    Actor_ShowEmote(28, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessageAndWait(28, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_SECOND_GUIDE, ANIM_SHAKE_HEAD);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_SECOND_GUIDE, ANIM_NOD);
    Event_ShowMessageAndWait(ACTOR_SECOND_GUIDE, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEAD_GUIDE, ANIM_NOD);
    Event_ShowMessageAndWait(ACTOR_HEAD_GUIDE, 0, 10);
    Actor_FaceDirection(27, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(28, FACING_NORTHWEST + FACING_STEP, 4);
    Actor_FaceDirection(29, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(30, FACING_NORTHWEST + FACING_STEP, 4);
    Actor_FaceDirection(32, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(31, FACING_NORTHWEST + FACING_STEP, 4);
    Actor_FaceDirection(33, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(34, FACING_NORTHWEST + FACING_STEP, 4);
    Engine_ActorSetAnimation(27, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(28, ANIM_NOD);
    Engine_ActorSetAnimation(29, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(30, ANIM_NOD);
    Engine_ActorSetAnimation(32, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(31, ANIM_NOD);
    Engine_ActorSetAnimation(33, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(34, ANIM_NOD);
    Engine_ActorJump(ACTOR_HEAD_GUIDE, 2, 40);
    Event_ShowMessageAndWait(ACTOR_HEAD_GUIDE, 0, 10);
    Actor_FaceDirection(27, FACING_WEST, 0);
    Actor_FaceDirection(28, FACING_WEST, 4);
    Actor_FaceDirection(29, FACING_WEST, 0);
    Actor_FaceDirection(30, FACING_WEST, 4);
    Actor_FaceDirection(32, FACING_WEST, 0);
    Actor_FaceDirection(31, FACING_WEST, 4);
    Actor_FaceDirection(33, FACING_WEST, 0);
    Actor_FaceDirection(34, FACING_WEST, 4);
    Actor_SetSpeed(ACTOR_HEAD_GUIDE, PIXELS(11) / 10, PIXELS(11) / 20);
    Actor_SetSpeed(27, PIXELS(21) / 20, PIXELS(21) / 40);
    Actor_SetSpeed(28, PIXELS(21) / 20, PIXELS(21) / 40);
    Actor_SetSpeed(29, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(30, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(32, PIXELS(19) / 20, PIXELS(19) / 40);
    Actor_SetSpeed(31, PIXELS(19) / 20, PIXELS(19) / 40);
    Actor_SetSpeed(33, PIXELS(9) / 10, PIXELS(9) / 20);
    Actor_SetSpeed(34, PIXELS(9) / 10, PIXELS(9) / 20);
    Actor_SetSpeed(ACTOR_SECOND_GUIDE, PIXELS(17) / 20, PIXELS(17) / 40);
    Engine_ActorSetSpritePriority(27, 1);
    Engine_ActorSetSpritePriority(28, 1);
    Engine_ActorSetSpritePriority(29, 1);
    Engine_ActorSetSpritePriority(30, 1);
    Engine_ActorSetSpritePriority(32, 1);
    Engine_ActorSetSpritePriority(31, 1);
    Engine_ActorSetSpritePriority(33, 1);
    Engine_ActorSetSpritePriority(34, 1);
    Engine_ActorSetSpritePriority(ACTOR_HEAD_GUIDE, 1);
    Engine_ActorSetSpritePriority(ACTOR_SECOND_GUIDE, 1);
    Engine_ActorStop(27);
    Engine_ActorStop(28);
    Engine_ActorStop(29);
    Engine_ActorStop(30);
    Engine_ActorStop(32);
    Engine_ActorStop(31);
    Engine_ActorStop(33);
    Engine_ActorStop(34);
    Engine_ActorStop(ACTOR_HEAD_GUIDE);
    Engine_ActorStop(ACTOR_SECOND_GUIDE);
    Engine_TaskWait(1);
    // Send the group onward, then bring in the missing tourist.
    Actor_SetActionScript(ACTOR_HEAD_GUIDE, gKalay_TourGroupLeaveScript);
    Actor_SetActionScript(27, gKalay_TourGroupLeaveScript);
    Actor_SetActionScript(28, gKalay_TourGroupLeaveScript);
    Actor_SetActionScript(29, gKalay_TourGroupLeaveScript);
    Actor_SetActionScript(30, gKalay_TourGroupLeaveScript);
    Actor_SetActionScript(32, gKalay_TourGroupLeaveScript);
    Actor_SetActionScript(31, gKalay_TourGroupLeaveScript);
    Actor_SetActionScript(33, gKalay_TourGroupLeaveScript);
    Actor_SetActionScript(34, gKalay_TourGroupLeaveScript);
    closing = Actor_Get(ACTOR_SECOND_GUIDE);
    closing->timer = 0;
    Actor_SetActionScript(ACTOR_SECOND_GUIDE, gKalay_TourGroupLeaveScript);
    do {
      Engine_TaskWait(1);
    } while (Actor_Get(ACTOR_SECOND_GUIDE)->timer != 1);
    Engine_EventWait(80);
    // The late tourist finds that the rest of the group has already left.
    Actor_SetPosition(ACTOR_LATE_TOURIST, PIXELS(340), PIXELS(274));
    Engine_TaskWait(1);
    Actor_SetSpeed(ACTOR_LATE_TOURIST, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_LATE_TOURIST, 224, 274);
    Actor_FaceDirection(ACTOR_LATE_TOURIST, FACING_EAST, 40);
    Actor_FaceDirection(ACTOR_LATE_TOURIST, FACING_WEST, 40);
    Actor_FaceDirection(ACTOR_LATE_TOURIST, FACING_NORTH, 40);
    Actor_FaceDirection(ACTOR_LATE_TOURIST, FACING_SOUTH + FACING_STEP, 40);
    Actor_ShowEmote(ACTOR_LATE_TOURIST, EMOTE_IN_FRONT | 1, 60);
    Event_ShowMessageAndWait(ACTOR_LATE_TOURIST, 0, 10);
    Actor_FaceDirection(ACTOR_LATE_TOURIST, FACING_EAST, 40);
    Actor_FaceDirection(ACTOR_LATE_TOURIST, FACING_NORTH, 40);
    Actor_FaceDirection(ACTOR_LATE_TOURIST, FACING_WEST, 40);
    Actor_SetAttachedEffect(ACTOR_LATE_TOURIST, EMOTE_IN_FRONT | 2);
    Engine_ActorJump(ACTOR_LATE_TOURIST, 4, 40);
    Event_ShowMessageAndWait(ACTOR_LATE_TOURIST, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_LATE_TOURIST, 2);
    Event_ShowMessageAndWait(ACTOR_LATE_TOURIST, 0, 10);
    Engine_ActorJump(ACTOR_LATE_TOURIST, 4, 40);
    Actor_SetSpeed(ACTOR_LATE_TOURIST, PIXELS(6) / 5, PIXELS(3) / 5);
    closing = Actor_Get(ACTOR_LATE_TOURIST);
    closing->timer = 0;
    Actor_SetActionScript(ACTOR_LATE_TOURIST, gKalay_TourGroupLeaveScript);
    do {
      Engine_TaskWait(1);
    } while (Actor_Get(ACTOR_LATE_TOURIST)->timer != 1);
    Actor_SetPosition(ACTOR_LATE_TOURIST, PIXELS(359), PIXELS(314));
    late = Actor_Get(ACTOR_LATE_TOURIST);
    late->facing = FACING_NORTH + FACING_STEP;
    Actor_SetPosition(ACTOR_HEAD_GUIDE, PIXELS(455), PIXELS(434));
    headEnd = Actor_Get(ACTOR_HEAD_GUIDE);
    headEnd->facing = FACING_NORTH + FACING_STEP;
    Actor_SetPosition(ACTOR_SECOND_GUIDE, PIXELS(464), PIXELS(416));
    lastEnd = Actor_Get(ACTOR_SECOND_GUIDE);
    lastEnd->facing = FACING_SOUTH + FACING_STEP;
    Engine_ActorDestroy(27);
    Engine_ActorDestroy(28);
    Engine_ActorDestroy(29);
    Engine_ActorDestroy(30);
    Engine_ActorDestroy(31);
    Engine_ActorDestroy(32);
    Engine_ActorDestroy(33);
    Engine_ActorDestroy(34);
    Event_PlayCue(MUS_017);
    if (work->touchedTrigger == 9) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 224, 458);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      leader->facing = FACING_NORTH;
    } else {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 40, 248);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      leader->facing = FACING_SOUTH;
    }
    Audio_PlayCueFromEventWork();
    GameFlag_Set(FLAG_KALAY_COLOSSO_TOUR_LEFT);
  } else {
    Event_PlayCue(SE_MAP_EXIT);
    Engine_EventRequestExit(work->touchedTrigger);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
  }
  Engine_EventEnd();
}

/// Touching trigger 10 in the tunnel stops the fade overlay following the
/// leader.
static void Kalay_StopTrackingLeader(void) {
  if (GameFlag_IsSet(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER) != 0) {
    gActorEffectWork->actor = NULL;
    GameFlag_Clear(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
  }
}

/// Touching trigger 11 in the tunnel makes the fade overlay follow the leader.
static void Kalay_StartTrackingLeader(void) {
  struct FadeOverlayWork *work;

  if (GameFlag_IsSet(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER) == 0) {
    work = gActorEffectWork;
    work->actor = Object_GetById(ACTOR_PARTY_LEADER);
    GameFlag_Set(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
  }
}

/// Trigger 19 in the tunnel: the leader's depth priority follows his position.
static void Kalay_EnableLeaderAutomaticPriority(void) {
  u8 *priorityFlags;
  s32 flags;

  priorityFlags = &Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags;
  flags = ACTOR_PRIORITY_AUTOMATIC;
  flags |= *priorityFlags;
  *priorityFlags = flags;
}

/// Trigger 18 in the tunnel: the leader's depth priority stays fixed.
static void Kalay_DisableLeaderAutomaticPriority(void) {
  u8 *priorityFlags;
  s32 flags;

  priorityFlags = &Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags;
  flags = (u8)~ACTOR_PRIORITY_AUTOMATIC;
  flags &= *priorityFlags;
  *priorityFlags = flags;
}

/// Catch pulls in the Nut circling in the town.
static void Kalay_CatchTownNut(void) {
  Scheduler_RemoveCallback(Kalay_UpdateNutOrbit);
  Engine_TaskWait(1);
  Engine_ActorSetPosition(ACTOR_TOWN_NUT, 0, 0);
  Engine_GameFlagSet(FLAG_KALAY_TOWN_NUT_CAUGHT);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
}

/// Every map of Kalay sets flag 0x87a, then prepares its own people and cells.
s32 Kalay_Initialize(void) {
  s32 scene;

  Engine_GameFlagSet(FLAG_REACHED_KALAY);
  scene = gGameState.scene;
  if (scene == (s32)&SceneId_Kalay1) {
    Kalay_PrepareTown();
  } else if (scene == (s32)&SceneId_Kalay2) {
    Kalay_PreparePalaceGate();
  } else if (scene == (s32)&SceneId_Kalay3) {
    Kalay_PrepareCave();
  } else if (scene == (s32)&SceneId_Kalay5) {
    Kalay_StartWindowTransition();
  } else if (scene == (s32)&SceneId_Kalay6) {
    Kalay_PrepareTunnel();
  }
  return 0;
}

/// Once Lunpa's trade has reopened, Hammet's palace flags (0x321 and 0x912 to
/// 0x915) are set. Until it is caught, the Nut circles above the town.
static void Kalay_PrepareTown(void) {
  OrbitingSceneObject *nut;
  s32 found;
  u8 *heap;
  OrbitingSceneObjectSprite *sprite;

  if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    Engine_GameFlagSet(0x321);
    Engine_GameFlagSet(FLAG_LAYANA_SCOLDED_LUNPA_VISIT);
    Engine_GameFlagSet(FLAG_IVAN_REPORTED_TO_LAYANA);
    Engine_GameFlagSet(0x915);
  }
  if (Engine_GameFlagIsSet(FLAG_LUNPA_HEARD_OF_PRISONER) != 0) {
    Engine_GameFlagSet(0x321);
  }
  if (gGameState.entrance == 14) {
    Actor_SetPosition(25, PIXELS(424), PIXELS(88));
  }
  Engine_ActorSetChildValue(ACTOR_SECOND_GUIDE, 2);
  found = Engine_GameFlagIsSet(FLAG_KALAY_TOWN_NUT_CAUGHT);
  if (found != 0) {
    Engine_ActorSetPosition(ACTOR_TOWN_NUT, 0, 0);
  } else {
    nut = (OrbitingSceneObject *)Object_GetById(ACTOR_TOWN_NUT);
    sprite = nut->sprite;
    sprite->flags09Mode = 1;
    sprite->flags05Bit5 = 0;
    sprite->flags09High = 0;
    sprite->state = found;
    nut->active = 1;
    nut->mode = found;
    nut->y = PIXELS(10);
    nut->visible = 1;
    heap = Engine_HeapAllocate(17, 0x608);
    Engine_ItemLoadIcon(ITEM_NUT);
    Engine_VramLoad(sprite->palette, 128, heap + 0x400);
    Engine_HeapRelease(17);
    nut->orbitAngle = found;
    nut->orbitCenterX = nut->x;
    nut->orbitCenterY = nut->y;
    nut->orbitCenterZ = nut->z;
    FieldScheduler_AddCallback(Kalay_UpdateNutOrbit, KALAY_CALLBACK_PRIORITY);
  }
}

/// The palace flags as in the town; walking up to the gate the first time,
/// Ivan introduces himself to the guards.
static void Kalay_PreparePalaceGate(void) {
  if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    Engine_GameFlagSet(0x321);
    Engine_GameFlagSet(FLAG_LAYANA_SCOLDED_LUNPA_VISIT);
    Engine_GameFlagSet(FLAG_IVAN_REPORTED_TO_LAYANA);
    Engine_GameFlagSet(0x915);
  }
  if (Engine_GameFlagIsSet(FLAG_LUNPA_HEARD_OF_PRISONER) != 0) {
    Engine_GameFlagSet(0x321);
  }
  if (gGameState.entrance != 0) {
    if (Engine_GameFlagIsSet(FLAG_IVAN_REPORTED_TO_LAYANA) == 0) {
      Kalay_IvanMeetsPalaceGuards();
    }
  }
}

/// The cave's Nut floats until caught, the guard turns after flag 0x915,
/// and arriving through entrance 10 brings Hammet back by wagon.
static void Kalay_PrepareCave(void) {
  struct FieldActor *guard;

  if (Engine_GameFlagIsSet(0xfd6) == 0) {
    Scene_FloatNut(ACTOR_CAVE_NUT);
  }
  if (Engine_GameFlagIsSet(0x915) != 0) {
    guard = Object_GetById(ACTOR_CAVE_GUARD);
    guard->facing = 0;
  }
  if (gGameState.entrance == 10) {
    Kalay_HammetReturnsToPalace();
  }
}

/// The fifth map opens with a window transition. After flag 0x915 its passage
/// cells are open; entrances 1 to 3 start sound 170.
static void Kalay_StartWindowTransition(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (Engine_GameFlagIsSet(0x915) != 0) {
    Map_CopyCellsTo(58, 5, 58, 8, 2, 3);
    {
      Map_CopyCellAttributes(8, 11, 2, 1, 8, 10);
    }
    Map_CopyCellsTo(8, 12, 8, 11, 2, 1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
  }
  if (gGameState.entrance <= 3) {
    BattleFx_SetQueuedSoundAndPlay(SE_170);
  }
}

/// The tunnel: the fade overlay follows the leader unless he came in by
/// entrance 4; pushed blocks, frozen target and opened passages are restored.
static void Kalay_PrepareTunnel(void) {
  s32 resumedScene;
  struct FieldActor *actor;
  s32 trackingValue;
  s32 column;
  struct FadeOverlayWork *effectWork;
  // The resident engine stores fade-overlay work in pointer slot nine.
  u8 **workSlots = (u8 **)&gEventWork;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  BattleFx_StartFadeOverlay(0);
#if TBS_PATCH_1
  resumedScene = GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION);
  if (resumedScene != 0) {
    effectWork = (struct FadeOverlayWork *)workSlots[9];
    trackingValue = GameFlag_IsSet(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
    if (trackingValue != 0) {
      trackingValue = Object_GetById(ACTOR_PARTY_LEADER);
    }
    effectWork->actor = (struct FieldActor *)trackingValue;
  } else {
    GameFlag_Set(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
    if (gGameState.entrance == 4) {
      ((struct FadeOverlayWork *)workSlots[9])->actor = NULL;
      GameFlag_Clear(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
    }
  }
#else
  GameFlag_Set(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
  if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    effectWork = (struct FadeOverlayWork *)workSlots[9];
    trackingValue = GameFlag_IsSet(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
    if (trackingValue != 0) {
      trackingValue = Object_GetById(ACTOR_PARTY_LEADER);
    }
    effectWork->actor = (struct FieldActor *)trackingValue;
  } else if (gGameState.entrance == 4) {
    ((struct FadeOverlayWork *)workSlots[9])->actor = NULL;
    GameFlag_Clear(FLAG_TUNNEL_OVERLAY_FOLLOWS_LEADER);
  }
#endif
  if (GameFlag_IsSet(FLAG_TUNNEL_FIRST_PASSAGE_OPEN) != 0) {
    Actor_SetPosition(ACTOR_FROST_TARGET, PIXELS(150), PIXELS(728));
    if (GameFlag_IsSet(FLAG_TUNNEL_FROST_TARGET_FROZEN) != 0) {
      Actor_Get(ACTOR_FROST_TARGET);
      column = 9;
      Engine_ActorSetAnimation(ACTOR_FROST_TARGET, 5);
      Map_CopyCellAttributes(0, 0, 1, 1, column, 14);
      Map_CopyCellAttributes(0, 0, 1, 1, column, 45);
      {
        u8 *p = &Object_GetById(ACTOR_FROST_TARGET)->priorityFlags;
        s32 v = ACTOR_PRIORITY_UNDERFOOT;

        v |= *p;
        *p = v;
      }
    }
  }
  actor = Actor_Get(ACTOR_FIRST_BLOCK);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Actor_Get(ACTOR_SECOND_BLOCK);
  Engine_ActorSetSpriteFlags(actor, 0);
  FieldScheduler_AddCallback(Kalay_UpdateTunnelEmitters,
                             KALAY_CALLBACK_PRIORITY);
  if (GameFlag_IsSet(0x915) != 0) {
    Actor_SetPosition(10, PIXELS(426), PIXELS(730));
    actor = Actor_Get(10);
    {
      u16 shown = 0x5000;

      actor->facing = shown;
    }
    Map_CopyCellsTo(88, 48, 88, 45, 2, 3);
    Map_CopyCellsTo(24, 49, 24, 48, 2, 1);
    Map_CopyCellsTo(25, 42, 25, 47, 1, 1);
    Map_CopyCellAttributes(22, 50, 2, 1, 24, 49);
  }
  if (GameFlag_IsSet(FLAG_TUNNEL_FIRST_PASSAGE_OPEN) != 0) {
    Actor_SetPosition(ACTOR_FIRST_BLOCK, PIXELS(232), PIXELS(732));
    Map_CopyCellAttributes(7, 44, 1, 1, 0, 1);
    Map_CopyCellsTo(74, 58, 78, 41, 1, 5);
    Map_CopyCellsTo(16, 109, 13, 109, 3, 2);
    Map_CopyCellsTo(67, 64, 71, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 72, 44, 1, 2);
    Map_CopyCellsTo(67, 68, 73, 43, 1, 2);
    Map_CopyCellsTo(67, 68, 74, 43, 1, 2);
    Map_CopyCellsTo(67, 64, 75, 44, 1, 2);
    Map_CopyCellsTo(67, 66, 76, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 77, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 78, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 79, 44, 1, 2);
    Map_CopyCellsTo(67, 66, 80, 44, 1, 2);
    Map_CopyCellsTo(2, 0, 9, 42, 2, 2);
    Map_CopyCellsTo(68, 64, 71, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 72, 44, 1, 2);
    Map_CopyCellsTo(68, 68, 73, 43, 1, 2);
    Map_CopyCellsTo(68, 68, 74, 43, 1, 2);
    Map_CopyCellsTo(68, 64, 75, 44, 1, 2);
    Map_CopyCellsTo(68, 66, 76, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 77, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 78, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 79, 44, 1, 2);
    Map_CopyCellsTo(68, 66, 80, 44, 1, 2);
    Map_CopyCellsTo(4, 0, 9, 42, 2, 2);
    Map_CopyCellsTo(7, 11, 7, 42, 10, 8);
    Map_CopyCellsTo(71, 12, 71, 43, 10, 13);
    Map_CopyCellAttributes(6, 13, 12, 12, 6, 44);
    Map_CopyCellAttributes(0, 1, 1, 1, 7, 44);
  } else {
    switch (gGameState.entrance) {
    case 1:
    case 2:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    }
  }
  if (GameFlag_IsSet(FLAG_TUNNEL_SECOND_PASSAGE_OPEN) != 0) {
    Actor_SetPosition(ACTOR_SECOND_BLOCK, PIXELS(696), PIXELS(732));
    Map_CopyCellsTo(74, 58, 107, 41, 1, 5);
    Map_CopyCellsTo(45, 109, 42, 109, 3, 2);
    Map_CopyCellsTo(67, 64, 102, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 103, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 104, 44, 1, 2);
    Map_CopyCellsTo(67, 66, 105, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 106, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 107, 44, 1, 2);
    Map_CopyCellsTo(67, 64, 108, 44, 1, 2);
    Map_CopyCellsTo(67, 66, 109, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 102, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 103, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 104, 44, 1, 2);
    Map_CopyCellsTo(68, 66, 105, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 106, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 107, 44, 1, 2);
    Map_CopyCellsTo(68, 64, 108, 44, 1, 2);
    Map_CopyCellsTo(68, 66, 109, 44, 1, 2);
    Map_CopyCellsTo(38, 14, 38, 44, 8, 4);
    Map_CopyCellsTo(102, 14, 102, 44, 8, 12);
    Map_CopyCellAttributes(37, 13, 10, 12, 37, 43);
  } else {
    switch (gGameState.entrance) {
    case 3:
    case 4:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    }
  }
}

/// All eight tourists start fidgeting.
static void Kalay_StartTouristsFidgeting(void) {
  Kalay_StartTouristFidgeting(27);
  Kalay_StartTouristFidgeting(28);
  Kalay_StartTouristFidgeting(29);
  Kalay_StartTouristFidgeting(30);
  Kalay_StartTouristFidgeting(32);
  Kalay_StartTouristFidgeting(31);
  Kalay_StartTouristFidgeting(33);
  Kalay_StartTouristFidgeting(34);
}

/// A tourist starts fidgeting after a random pause.
static void Kalay_StartTouristFidgeting(s32 actorId) {
  struct FieldActor *actor;
  u32 random;

  actor = Object_GetById(actorId);
  actor->timer = actorId;
  random = Engine_RandomNext();
  actor->work = (random * 5) >> 12;
  Engine_ObjectSetScript(actor, (s32)gKalay_TouristFidgetScript);
}

/// The guides speed up and start pacing in turn.
static void Kalay_StartGuidesPacing(void) {
  gKalay_DanceStep = 0;
  Actor_SetSpeed(ACTOR_HEAD_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(ACTOR_SECOND_GUIDE, PIXELS(8) / 5, PIXELS(4) / 5);
  FieldScheduler_AddCallback(Kalay_AlternateGuidesPacing,
                             KALAY_CALLBACK_PRIORITY);
}

/// Each guide paces in turn, starting when the other finishes.
static void Kalay_AlternateGuidesPacing(void) {
  switch (gKalay_DanceStep) {
  case 0:
    Kalay_ClearScriptDone(Object_GetById(ACTOR_SECOND_GUIDE));
    Actor_SetActionScript(ACTOR_SECOND_GUIDE, gKalay_GuideCountScript);
    gKalay_DanceStep++;
    break;
  case 1:
    if (Object_GetById(ACTOR_SECOND_GUIDE)->timer != 0) {
      Kalay_ClearScriptDone(Object_GetById(ACTOR_HEAD_GUIDE));
      Actor_SetActionScript(ACTOR_HEAD_GUIDE, gKalay_GuidePaceScript);
      gKalay_DanceStep++;
    }
    break;
  case 2:
    if (Object_GetById(ACTOR_HEAD_GUIDE)->timer != 0) {
      Kalay_ClearScriptDone(Object_GetById(ACTOR_HEAD_GUIDE));
      Actor_SetActionScript(ACTOR_HEAD_GUIDE, gKalay_GuideCountScript);
      gKalay_DanceStep++;
    }
    break;
  case 3:
    if (Object_GetById(ACTOR_HEAD_GUIDE)->timer != 0) {
      Kalay_ClearScriptDone(Object_GetById(ACTOR_SECOND_GUIDE));
      Actor_SetActionScript(ACTOR_SECOND_GUIDE, gKalay_GuidePaceScript);
      gKalay_DanceStep++;
    }
    break;
  case 4:
    if (Object_GetById(ACTOR_SECOND_GUIDE)->timer != 0) {
      gKalay_DanceStep = 0;
    }
    break;
  }
}

/// At the palace gate the guards turn the party away until they recognize
/// Ivan, tanned and grown, and learn that he does not know Hammet is a
/// prisoner. Ivan insists his friends hear everything, and the guards open the
/// door for the party.
static void Kalay_IvanMeetsPalaceGuards(void) {
  struct FieldActor *actor;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(192), PIXELS(342));
  Engine_TaskWait(1);
  Camera_SetSpeed(PIXELS(1) / 5, PIXELS(1) / 40);
  Camera_MoveTo(PIXELS(192), -1, PIXELS(252), 1);
  gEventWork->transitionFrames = 40;
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 192, 278);
  Actor_SetSpeed(ACTOR_PALACE_GUARD, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_SECOND_PALACE_GUARD, PIXELS(1), PIXELS(1) / 2);
  actor = Actor_Get(ACTOR_PALACE_GUARD);
  actor->facing = FACING_SOUTHEAST + FACING_STEP;
  actor = Actor_Get(ACTOR_SECOND_PALACE_GUARD);
  actor->facing = FACING_SOUTH + FACING_STEP;
  Engine_TaskWait(1);
  Actor_Get(ACTOR_PALACE_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_SECOND_PALACE_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(ACTOR_PALACE_GUARD, 184, 232);
  Actor_WalkToAndWait(ACTOR_SECOND_PALACE_GUARD, 198, 232);
  Engine_ActorSetAnimation(ACTOR_PALACE_GUARD, ANIM_STAND);
  Engine_EventWait(20);
  Actor_Get(ACTOR_PALACE_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_SECOND_PALACE_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_PALACE_GUARD, ANIM_SHAKE_HEAD);
  Engine_EventSetMessage(MsgKalayLordHammetsPalaceLordAway);
  Event_ShowMessageAndWait(ACTOR_PALACE_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_SOUTH + FACING_STEP,
                      10);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_PALACE_GUARD, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_SECOND_PALACE_GUARD, 0, 10);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(IVAN, 212, 268);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_WalkToAndWait(IVAN, 202, 254);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(IVAN, 0, 10);
  Actor_ShowEmote(ACTOR_PALACE_GUARD, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(ACTOR_SECOND_PALACE_GUARD, EMOTE_IN_FRONT | 1, 40);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_SOUTHEAST + FACING_STEP,
                      20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 60);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Event_ShowMessageAndWait(IVAN, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_PALACE_GUARD, 2);
  Event_ShowMessageAndWait(ACTOR_PALACE_GUARD, 0, 10);
  Actor_ShowEmote(ACTOR_PALACE_GUARD, EMOTE_IN_FRONT | 5, 60);
  Engine_ActorStartRepeatedMotion(ACTOR_PALACE_GUARD, 2);
  Event_ShowMessageAndWait(ACTOR_PALACE_GUARD, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_PALACE_GUARD, 2);
  Event_ShowMessageAndWait(ACTOR_SECOND_PALACE_GUARD, 0, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(10);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 10);
  Event_AskYesNo(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_PALACE_GUARD, 1);
  Engine_ActorSetAnimationAndWait(ACTOR_PALACE_GUARD, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_PALACE_GUARD, 0, 10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_PALACE_GUARD, 1);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_PALACE_GUARD, ANIM_NOD);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_ShowMessageAndWait(ACTOR_SECOND_PALACE_GUARD, 0, 10);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 60);
  Actor_FaceDirection(IVAN, FACING_NORTH, 10);
  Event_ShowMessageAndWait(IVAN, 0, 10);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_SOUTHEAST + FACING_STEP,
                      10);
  Engine_ActorRunRepeatedMotion(ACTOR_PALACE_GUARD, 1);
  Event_OpenMessage(ACTOR_PALACE_GUARD, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorRunRepeatedMotion(IVAN, 2);
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
    gEventWork->message++;
  } else {
    gEventWork->message++;
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
  }
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_PALACE_GUARD, 2);
  Event_ShowMessageAndWait(ACTOR_SECOND_PALACE_GUARD, 0, 10);
  Actor_FaceDirection(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(IVAN, 0, 10);
  Actor_SetAttachedEffect(ACTOR_PALACE_GUARD, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  Event_ShowMessageAndWait(ACTOR_PALACE_GUARD, 0, 10);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Event_ShowMessageAndWait(IVAN, 0, 10);
  Actor_SetAttachedEffect(ACTOR_PALACE_GUARD, EMOTE_IN_FRONT | 2);
  Actor_SetAttachedEffect(ACTOR_SECOND_PALACE_GUARD, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_PALACE_GUARD, 2);
  Event_ShowMessageAndWait(ACTOR_PALACE_GUARD, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_PALACE_GUARD, 2);
  Actor_FaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_WEST, 10);
  Event_ShowMessageAndWait(ACTOR_SECOND_PALACE_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_EAST, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PALACE_GUARD, ANIM_NOD);
  Actor_FaceDirection(ACTOR_PALACE_GUARD, FACING_SOUTHEAST + FACING_STEP, 10);
  Event_ShowMessage(ACTOR_PALACE_GUARD, 0);
  Actor_FaceDirection(ACTOR_SECOND_PALACE_GUARD, FACING_SOUTHEAST + FACING_STEP,
                      10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 10);
  Event_AskYesNo(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Actor_FaceDirection(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Actor_SetSpeed(ACTOR_PALACE_GUARD, PIXELS(4) / 5, PIXELS(2) / 5);
  Engine_ActorSetSpeed(ACTOR_SECOND_PALACE_GUARD, PIXELS(4) / 5, PIXELS(2) / 5);
  actor = Actor_Get(ACTOR_PALACE_GUARD);
  actor->facing = 0;
  actor = Actor_Get(ACTOR_SECOND_PALACE_GUARD);
  actor->facing = FACING_WEST;
  Actor_Get(ACTOR_PALACE_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_SECOND_PALACE_GUARD)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(ACTOR_PALACE_GUARD, 168, 232);
  Actor_WalkToAndWait(ACTOR_SECOND_PALACE_GUARD, 212, 232);
  Engine_ActorSetAnimation(ACTOR_PALACE_GUARD, ANIM_STAND);
  Engine_EventWait(20);
  Actor_Get(ACTOR_PALACE_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_SECOND_PALACE_GUARD)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(IVAN, 192, 232);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Kalay_AnimatePalaceDoor();
  Actor_WalkToAndWait(IVAN, 192, 222);
  // Lead the party and guards through the opened palace door.
  Actor_SetPosition(IVAN, 0, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 192, 222);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_ActorSetAnimation(ACTOR_PALACE_GUARD, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_PALACE_GUARD, ANIM_NOD);
  Actor_WalkTo(ACTOR_PALACE_GUARD, 184, 232);
  Actor_WalkToAndWait(ACTOR_SECOND_PALACE_GUARD, 198, 232);
  Actor_WalkTo(ACTOR_PALACE_GUARD, 188, 212);
  Actor_WalkToAndWait(ACTOR_SECOND_PALACE_GUARD, 194, 212);
  Actor_SetPosition(ACTOR_PALACE_GUARD, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_PALACE_GUARD, 0, 0);
  gEventWork->transitionFrames = 24;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(5);
  Engine_EventEnd();
}

/// Arriving through the cave's entrance 10, Hammet comes home by Bunza's wagon
/// with the party. Hammet explains the secret passage to the palace and asks
/// Bunza to take the party round through the town, so the passage stays
/// secret; the cave guard opens it, Hammet goes in and the party rides off.
static void Kalay_HammetReturnsToPalace(void) {
  u8 *leaderPhase;
  u32 random;
  u32 motionPhase;
  struct FieldActor *object;
  struct EventWork *work;
  const s32 *motionActions;
  s32 southFacing;
  const s32 *exitActions;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(ACTOR_WAGON, PIXELS(440), PIXELS(522));
  object = Actor_Get(ACTOR_WAGON);
  Engine_ActorSetSpriteFlags(object, 0);
  Camera_MoveTo(PIXELS(440), -1, PIXELS(522), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Camera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Camera_MoveTo(PIXELS(440), -1, PIXELS(400), 1);
  Event_PlayCue(SE_141);
  Actor_SetSpeed(ACTOR_WAGON, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_WAGON, 440, 400);
  Camera_SetSpeed(PIXELS(3) / 4, PIXELS(3) / 32);
  Camera_MoveTo(PIXELS(440), -1, PIXELS(300), 1);
  Actor_SetSpeed(ACTOR_WAGON, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_WAGON, 440, 300);
  Engine_ActorSetAnimation(ACTOR_WAGON, 0);
  Event_PlayCue(SE_289);
  Engine_EventWait(40);
  Actor_SetSpeed(ACTOR_BUNZA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_BUNZA, PIXELS(439), PIXELS(306));
  Engine_ActorJump(ACTOR_BUNZA, 4, 0);
  Actor_WalkToAndWait(ACTOR_BUNZA, 439, 312);
  Actor_WalkToAndWait(ACTOR_BUNZA, 416, 312);
  Actor_WalkToAndWait(ACTOR_BUNZA, 400, 256);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTHEAST + FACING_STEP, 40);
  Camera_MoveTo(PIXELS(410), -1, PIXELS(280), 1);
  Actor_SetSpeed(ACTOR_HAMMET, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetPosition(ACTOR_HAMMET, PIXELS(439), PIXELS(306));
  Engine_ActorJump(ACTOR_HAMMET, 4, 0);
  Actor_WalkToAndWait(ACTOR_HAMMET, 439, 312);
  Actor_WalkToAndWait(ACTOR_HAMMET, 416, 312);
  Actor_WalkToAndWait(ACTOR_HAMMET, 388, 270);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_NORTH + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH + FACING_STEP, 10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(439), PIXELS(306));
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 439, 312);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 416, 312);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 388, 300);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  // Seed the party idle phases for the arrival conversation.
  leaderPhase = (u8 *)Actor_Get(ACTOR_PARTY_LEADER);
  random = Random_Next();
  motionPhase = random * 5;
  motionActions = gKalay_PartyFidgetScript;
  leaderPhase += offsetof(struct FieldActor, work);
  *(u16 *)leaderPhase = motionPhase >> 12;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, motionActions);
  Engine_ActorJump(ACTOR_BUNZA, 2, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Engine_EventSetMessage(MsgKalayWeveArrivedHammet);
  Event_ShowMessageAndWait(ACTOR_BUNZA, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  object = Actor_Get(ACTOR_PARTY_LEADER);
  if (object != 0) {
    Actor_SetPosition(GARET, object->x.fixed, object->z.fixed);
  }
  object = Actor_Get(ACTOR_PARTY_LEADER);
  if (object != 0) {
    Actor_SetPosition(IVAN, object->x.fixed, object->z.fixed);
  }
  object = Actor_Get(ACTOR_PARTY_LEADER);
  if (object != 0) {
    Actor_SetPosition(MIA, object->x.fixed, object->z.fixed);
  }
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(GARET, 378, 310);
  Actor_WalkTo(IVAN, 400, 288);
  Actor_WalkToAndWait(MIA, 410, 308);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 40);
  leaderPhase = (u8 *)Actor_Get(GARET);
  random = Random_Next();
  leaderPhase += offsetof(struct FieldActor, work);
  *(u16 *)leaderPhase = (random * 5) >> 12;
  leaderPhase = (u8 *)Actor_Get(IVAN);
  random = Random_Next();
  leaderPhase += offsetof(struct FieldActor, work);
  *(u16 *)leaderPhase = (random * 5) >> 12;
  leaderPhase = (u8 *)Actor_Get(MIA);
  random = Random_Next();
  leaderPhase += offsetof(struct FieldActor, work);
  *(u16 *)leaderPhase = (random * 5) >> 12;
  Actor_SetActionScript(GARET, motionActions);
  Actor_SetActionScript(IVAN, motionActions);
  Actor_SetActionScript(MIA, motionActions);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 60);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 2, 0, 10);
  Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 0, 40);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_HAMMET, 0, 10);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(IVAN);
  Engine_ActorStop(MIA);
  Engine_TaskWait(1);
  Actor_FaceDirection(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_HAMMET, 0, 10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 60);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 3, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_HAMMET, 0, 10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 60);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_BUNZA, 2);
  Event_ShowMessageAndWait(ACTOR_BUNZA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 10);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_NORTH + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_HAMMET, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 2, 0, 10);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_BUNZA, 0, 10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 3, 0, 10);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | 8, 20);
  Event_ShowMessageAndWait(ACTOR_HAMMET, 0, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_BUNZA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_BUNZA, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_HAMMET, 0, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 5, 60);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_NOD);
  Event_OpenMessage(ACTOR_HAMMET, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    gEventWork->message += 3;
  } else {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_BUNZA, 2);
    Event_ShowMessageAndWait(ACTOR_BUNZA, 0, 40);
    Event_ShowMessageAndWait(ACTOR_BUNZA, 0, 10);
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 6, 40);
    Actor_FaceDirection(MIA, FACING_NORTH, 20);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 3, 0, 10);
  }
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 10);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_CAVE_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(280), -1, PIXELS(200), 1);
  Actor_WalkToAndWait(ACTOR_HAMMET, 333, 222);
  Actor_WalkToAndWait(ACTOR_HAMMET, 284, 198);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_WEST, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Actor_FaceDirection(ACTOR_CAVE_GUARD, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_CAVE_GUARD, ANIM_NOD);
  Actor_SetSpeed(ACTOR_CAVE_GUARD, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_CAVE_GUARD, 268, 198);
  Actor_FaceDirection(ACTOR_CAVE_GUARD, FACING_NORTH, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_CAVE_GUARD, 2);
  // The guard opens the passage and Hammet goes in.
  Event_PlayCue(SE_125);
  Map_CopyCellsTo(71, 60, 76, 11, 2, 1);
  Map_CopyCellAttributes(71, 60, 2, 1, 16, 11);
  Engine_EventWait(20);
  Actor_WalkToAndWait(ACTOR_CAVE_GUARD, 246, 198);
  Actor_FaceDirection(ACTOR_CAVE_GUARD, 0, 20);
  Camera_GetEventObject()->motionFlags = 0;
  Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Camera_MoveTo(PIXELS(248), -1, PIXELS(170), 1);
  Actor_WalkToAndWait(ACTOR_HAMMET, 270, 198);
  Actor_WalkToAndWait(ACTOR_HAMMET, 270, 174);
  Actor_WalkToAndWait(ACTOR_HAMMET, 224, 170);
  Actor_WalkToAndWait(ACTOR_HAMMET, 210, 158);
  Actor_WalkToAndWait(ACTOR_HAMMET, 246, 148);
  Actor_WalkToAndWait(ACTOR_HAMMET, 246, 142);
  Actor_SetPosition(ACTOR_HAMMET, 0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Actor_SetPosition(ACTOR_WAGON, PIXELS(440), PIXELS(340));
  object = Actor_Get(ACTOR_WAGON);
  southFacing = FACING_SOUTH;
  object->facing = southFacing;
  Camera_MoveTo(PIXELS(380), -1, PIXELS(280), 0);
  Engine_MapRedraw();
  Engine_TaskWait(10);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_BUNZA, 420, 282);
  Actor_WalkToAndWait(ACTOR_BUNZA, 420, 312);
  Actor_WalkToAndWait(ACTOR_BUNZA, 439, 312);
  Actor_WalkToAndWait(ACTOR_BUNZA, 439, 306);
  Actor_SetPosition(ACTOR_BUNZA, 0, 0);
  // The party climbs back into the wagon.
  exitActions = gKalay_PartyGatherScript;
  Actor_SetActionScript(GARET, exitActions);
  Actor_SetActionScript(IVAN, exitActions);
  Actor_RunActionScriptAndWait(MIA, exitActions);
  Camera_MoveTo(PIXELS(410), -1, PIXELS(300), 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 416, 312);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 439, 312);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 439, 306);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Event_PlayCue(SE_141);
  Actor_SetSpeed(ACTOR_WAGON, PIXELS(4) / 5, PIXELS(2) / 5);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(440), -1, PIXELS(420), 1);
  Actor_WalkToAndWait(ACTOR_WAGON, 440, 420);
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(440), -1, PIXELS(600), 1);
  Actor_SetSpeed(ACTOR_WAGON, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_WAGON, 440, 500);
  Actor_WalkTo(ACTOR_WAGON, 440, 600);
  Event_PlayCue(SE_289);
  work = gEventWork;
  work->transitionFrames = 24;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(10);
  Engine_EventEnd();
}

// Shared source, included in link order.
#include "common/object_findcell.h"

/// In the tunnel, walking into a block pushes it one tile when there is room.
static void Kalay_PushTunnelBlock(void) {
  struct FieldPosition destination;
  struct FieldActor *leader;
  struct FieldActor *block;
  s32 direction;
  s32 probeStep;
  s32 moveStep;
  s32 leaderX;
  struct FieldPosition *target;
  s32 stoppedVelocity;
  s32 pushSpeed;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  direction = (leader->facing >> 12);
  leaderX = leader->x.part.pixel;
  probeStep = gKalay_TileStepByFacing[direction];
  block =
      SceneActor_FindActorAtCell((leaderX + (probeStep >> 16)) >> 4,
                                 (leader->z.part.pixel + (s16)probeStep) >> 4);
  if (block != NULL) {
    stoppedVelocity = 0;
    block->terrainId = 2;
    target = &destination;
    moveStep = gKalay_TileStepByFacing[direction];
    SET_POSITION(target, block->x.fixed + (moveStep & -0x10000), block->y.fixed,
                 block->z.fixed + (moveStep << 16));
    if (FieldObject_CheckMovementCollision(block, target) <= 0) {
      Object_SetMode(leader, 8);
      pushSpeed = PIXELS(1) / 5;
      Engine_TaskWait(15);
      Engine_AudioPlayCue(SE_185);
      block->speed = pushSpeed;
      block->acceleration = pushSpeed;
      Engine_ObjectSetPosition(block, target->x, target->y, target->z);
      leader->speed = pushSpeed;
      leader->acceleration = pushSpeed;
      Engine_ObjectSetPosition(leader, target->x, target->y, target->z);
      FieldObject_WaitForMove(block);
      BattleFx_PlayQueuedSound();
      block->x.fixed = target->x;
      block->z.fixed = target->z;
      block->velocityX = stoppedVelocity;
      block->velocityZ = stoppedVelocity;
      Object_SetMode(leader, 1);
      Kalay_UpdateTunnelPassages();
    }
  }
}

/// Once a block is pushed far enough west, its passage opens step by step and
/// the frost target appears for the first.
static void Kalay_UpdateTunnelPassages(void) {
  struct FieldActor *firstBlock;
  struct FieldActor *secondBlock;
  s32 firstPassageOpen;
  struct FieldActor *frostEffect;
  u32 i;
  s32 span;
  s32 height;
  s32 width;

  // Open each passage only after its corresponding block reaches the trigger.
  firstBlock = Object_GetById(ACTOR_FIRST_BLOCK);
  secondBlock = Object_GetById(ACTOR_SECOND_BLOCK);
  firstPassageOpen = GameFlag_IsSet(FLAG_TUNNEL_FIRST_PASSAGE_OPEN);
  if (firstPassageOpen != 0) {
  } else if ((firstBlock->x.fixed >> 19) > 29) {
  } else {
    frostEffect = Object_GetById(ACTOR_FROST_TARGET);
    Engine_EventBegin();
    Map_CopyCellAttributes(7, 44, 1, 1, firstPassageOpen, 1);
    i = 67;
    width = 1;
    height = 5;
    do {
      Map_CopyCellsTo(i, 58, 78, 41, width, height);
      Engine_EventWait(4);
      if (i == 70) {
        GameFlag_Set(FLAG_TUNNEL_FIRST_PASSAGE_OPEN);
      }
      i++;
    } while (i <= 74);
    span = 2;
    Map_CopyCellsTo(16, 109, 13, 109, 3, span);
    Engine_EventWait(40);
    frostEffect->scaleX = Q16_ONE / 10;
    frostEffect->scaleY = Q16_ONE / 10;
    Actor_SetPosition(ACTOR_FROST_TARGET, PIXELS(150), PIXELS(728));
    Actor_SetActionScript(ACTOR_FROST_TARGET, gKalay_FrostGrowScript);
    height = 1;
    Map_CopyCellsTo(67, 64, 71, 44, height, span);
    Map_CopyCellsTo(67, 64, 72, 44, height, span);
    Map_CopyCellsTo(67, 68, 73, 43, height, span);
    Map_CopyCellsTo(67, 68, 74, 43, height, span);
    Map_CopyCellsTo(67, 64, 75, 44, height, span);
    Map_CopyCellsTo(67, 66, 76, 44, height, span);
    Map_CopyCellsTo(67, 64, 77, 44, height, span);
    Map_CopyCellsTo(67, 64, 78, 44, height, span);
    Map_CopyCellsTo(67, 64, 79, 44, height, span);
    Map_CopyCellsTo(67, 66, 80, 44, height, span);
    Map_CopyCellsTo(2, 0, 9, 42, span, span);
    Engine_EventWait(40);
    Map_CopyCellsTo(68, 64, 71, 44, height, span);
    Map_CopyCellsTo(68, 64, 72, 44, height, span);
    Map_CopyCellsTo(68, 68, 73, 43, height, span);
    Map_CopyCellsTo(68, 68, 74, 43, height, span);
    Map_CopyCellsTo(68, 64, 75, 44, height, span);
    Map_CopyCellsTo(68, 66, 76, 44, height, span);
    Map_CopyCellsTo(68, 64, 77, 44, height, span);
    Map_CopyCellsTo(68, 64, 78, 44, height, span);
    Map_CopyCellsTo(68, 64, 79, 44, height, span);
    Map_CopyCellsTo(68, 66, 80, 44, height, span);
    Map_CopyCellsTo(4, 0, 9, 42, span, span);
    Engine_EventWait(40);
    span = 10;
    Map_CopyCellsTo(7, 11, 7, 42, span, 8);
    Map_CopyCellsTo(71, 12, 71, 43, span, 13);
    span = 44;
    Map_CopyCellAttributes(6, 13, 12, 12, 6, span);
    Engine_EventWait(40);
    Kalay_StartTrackingLeader();
    Map_CopyCellAttributes(0, 1, 1, 1, 7, span);
    Engine_EventEnd();
  }
  if (GameFlag_IsSet(FLAG_TUNNEL_SECOND_PASSAGE_OPEN) != 0) {
  } else if ((secondBlock->x.fixed >> 19) > 87) {
  } else {
    Engine_EventBegin();
    i = 67;
    width = 1;
    height = 5;
    do {
      Map_CopyCellsTo(i, 58, 107, 41, width, height);
      Engine_EventWait(4);
      if (i == 70) {
        GameFlag_Set(FLAG_TUNNEL_SECOND_PASSAGE_OPEN);
      }
      i++;
    } while (i <= 74);
    height = 2;
    Map_CopyCellsTo(45, 109, 42, 109, 3, height);
    Engine_EventWait(40);
    span = 1;
    Map_CopyCellsTo(67, 64, 102, 44, span, height);
    Map_CopyCellsTo(67, 64, 103, 44, span, height);
    Map_CopyCellsTo(67, 64, 104, 44, span, height);
    Map_CopyCellsTo(67, 66, 105, 44, span, height);
    Map_CopyCellsTo(67, 64, 106, 44, span, height);
    Map_CopyCellsTo(67, 64, 107, 44, span, height);
    Map_CopyCellsTo(67, 64, 108, 44, span, height);
    Map_CopyCellsTo(67, 66, 109, 44, span, height);
    Engine_EventWait(40);
    Map_CopyCellsTo(68, 64, 102, 44, span, height);
    Map_CopyCellsTo(68, 64, 103, 44, span, height);
    Map_CopyCellsTo(68, 64, 104, 44, span, height);
    Map_CopyCellsTo(68, 66, 105, 44, span, height);
    Map_CopyCellsTo(68, 64, 106, 44, span, height);
    Map_CopyCellsTo(68, 64, 107, 44, span, height);
    Map_CopyCellsTo(68, 64, 108, 44, span, height);
    Map_CopyCellsTo(68, 66, 109, 44, span, height);
    Engine_EventWait(40);
    span = 8;
    Map_CopyCellsTo(38, 14, 38, 44, span, 4);
    Map_CopyCellsTo(102, 14, 102, 44, span, 12);
    Map_CopyCellAttributes(37, 13, 10, 12, 37, 43);
    Engine_EventWait(40);
    Kalay_StartTrackingLeader();
    Engine_EventEnd();
  }
}

// UB: the native void epilogue returns through r0, leaving the nonzero return
// address as SC_CALL's status. Repeat once per frame until the object is
// destroyed, with an explicit status in the modern build.
// Advance a particle, damp its velocity, and expire its lifetime.
#ifdef UBFIX
static s32 Kalay_UpdateTunnelParticle(struct ScriptInterpreter *interpreter) {
#else
static void Kalay_UpdateTunnelParticle(struct ScriptInterpreter *interpreter) {
#endif
  struct FieldActor *particle = (struct FieldActor *)interpreter;
  s16 *lifetime;
  s32 remainingFrames;
  s32 velocityX;
  s32 velocityY;

  lifetime = &particle->timer;
  remainingFrames = *lifetime;
  if (remainingFrames == 0) {
    Object_Destroy((struct DispatchObject *)particle);
  } else if (remainingFrames == 1) {
    particle->velocityX = 0;
    particle->velocityY = 0;
    particle->x.fixed = 0;
    particle->y.fixed = 0;
  } else {
    particle->scaleX += Q16_ONE / 32;
    particle->scaleY += Q16_ONE / 32;
  }
  particle->x.fixed += particle->velocityX;
  particle->y.fixed += particle->velocityY;
  velocityX = particle->velocityX;
  velocityY = particle->velocityY;
  particle->velocityX = velocityX - velocityX / 256;
  particle->velocityY = velocityY - velocityY / 16;
  {
    s32 nextLifetime = *(u16 *)lifetime;
    nextLifetime -= 1;
    *(u16 *)lifetime = nextLifetime;
  }
#ifdef UBFIX
  return 1;
#endif
}

/// Particles stream from each block while its passage is closed.
static void Kalay_UpdateTunnelEmitters(void) {
  struct FieldActor *firstEmitter;
  struct FieldActor *secondEmitter;

  firstEmitter = Actor_Get(ACTOR_FIRST_BLOCK);
  secondEmitter = Actor_Get(ACTOR_SECOND_BLOCK);
  if ((u32)(firstEmitter->x.part.pixel - 381) <= 12 &&
      firstEmitter->z.part.pixel > 777) {
    firstEmitter->sprite->image.priority =
        Actor_Get(ACTOR_PARTY_LEADER)->sprite->image.priority;
  } else if (!GameFlag_IsSet(FLAG_TUNNEL_FIRST_PASSAGE_OPEN) &&
             firstEmitter->x.part.pixel <= 245 && (gFrameCount & 1) == 0) {
    if (!GameFlag_IsSet(FLAG_TUNNEL_FIRST_PARTICLES_SOUNDED)) {
      BattleFx_SetQueuedSoundAndPlay(-1);
      Event_PlayCue(SE_230);
      GameFlag_Set(FLAG_TUNNEL_FIRST_PARTICLES_SOUNDED);
    }
    Kalay_SpawnTunnelParticle(firstEmitter->x.fixed, firstEmitter->y.fixed,
                              firstEmitter->z.fixed);
  }
  if (!GameFlag_IsSet(FLAG_TUNNEL_SECOND_PASSAGE_OPEN) &&
      secondEmitter->x.part.pixel <= 709 && (gFrameCount & 1) == 0) {
    if (!GameFlag_IsSet(FLAG_TUNNEL_SECOND_PARTICLES_SOUNDED)) {
      BattleFx_SetQueuedSoundAndPlay(-1);
      Event_PlayCue(SE_230);
      GameFlag_Set(FLAG_TUNNEL_SECOND_PARTICLES_SOUNDED);
    }
    Kalay_SpawnTunnelParticle(secondEmitter->x.fixed, secondEmitter->y.fixed,
                              secondEmitter->z.fixed);
  }
}

/// Create a short-lived particle with random position and velocity offsets.
static void Kalay_SpawnTunnelParticle(s32 x, s32 height, s32 z) {
  s32 depth;
  struct FieldActor *particle;
  s32 random;
  s32 attributeMask;
  u8 *spriteBytes;

  depth = z;
  random = Engine_RandomNext();
  particle = Engine_ObjectCreate(
      222, (x - PIXELS(8)),
      (((((u32)(random << 3) >> 16) << 16) + height) + PIXELS(16)), depth);
  if (particle != 0) {
    ((u8 *)particle)[offsetof(struct FieldActor, motionFlags)] =
        (attributeMask = 0);
    spriteBytes = (u8 *)particle->sprite;
    attributeMask -= 13;
    spriteBytes[9] = ((spriteBytes[9] & attributeMask) | 8);
    ObjectGroup_SetChildValue(particle, 9);
    Engine_ActorSetSpriteFlags(particle, 0);
    random = Random_Next();
    particle->velocityX = ((((u32)(random << 1) >> 16) - 1) << 16);
    random = Random_Next();
    particle->velocityY =
        ((((u32)(((random << 1) + random) << 1) >> 16) - 3) << 16);
    {
      u16 *target = (u16 *)&particle->timer;
      u16 shown = 20;

      *target = shown;
      particle->unk61 = 1;
    }
    Object_SetMode(particle, 1);
    Engine_ObjectSetScript(particle, (s32)gKalay_TunnelParticleScript);
  }
}

/// The town Nut circles its spot and rocks as it goes.
static void Kalay_UpdateNutOrbit(void) {
  OrbitingSceneObject *effect =
      (OrbitingSceneObject *)Object_GetById(ACTOR_TOWN_NUT);
  struct AnimationObject *sprite = (struct AnimationObject *)effect->sprite;
  s32 offset = Engine_MathSin(effect->orbitAngle) * 2;
  s32 first;

  if (offset > 0) {
    offset = -offset;
  }
  effect->x = effect->orbitCenterX + Engine_MathCos(effect->orbitAngle) * 2;
  effect->y = effect->orbitCenterY + offset;
  sprite->rotation = Engine_MathCos(effect->orbitAngle + 0x8000) >> 3;
  first = Engine_RandomNext();
  effect->orbitAngle += ((u32)(first << 9) >> 16) +
                        ((u32)(Engine_RandomNext() << 9) >> 16) + 0x400;
}

/// Frost on the target sinks it into the floor, clearing its cells.
static void Kalay_FreezeFrostTarget(void) {
  struct FieldActor *effect = Object_GetById(ACTOR_FROST_TARGET);

  Map_CopyCellAttributes(0, 0, 1, 1, 9, 14);
  Map_CopyCellAttributes(0, 0, 1, 1, 9, 45);
  if (effect != 0) {
    Engine_ActorSetSpriteFlags(effect, 0);
    effect->y.fixed -= PIXELS(32);
    effect->priorityFlags = 2;
  }
  Engine_GameFlagSet(FLAG_TUNNEL_FROST_TARGET_FROZEN);
}

// Shared source, included in link order.
#include "common/object_orbit.h"

// One tile in each facing direction: x in the high 16 bits, z in the low 16.
// UB: negative x coordinates cannot be left-shifted as signed integers.
#ifdef UBFIX
#define TILE_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))
#else
#define TILE_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))
#endif

s32 gKalay_TileStepByFacing[] = {
    TILE_STEP(16, 0),  TILE_STEP(16, 0),  TILE_STEP(16, 0),  TILE_STEP(0, 16),
    TILE_STEP(0, 16),  TILE_STEP(0, 16),  TILE_STEP(0, 16),  TILE_STEP(-16, 0),
    TILE_STEP(-16, 0), TILE_STEP(-16, 0), TILE_STEP(-16, 0), TILE_STEP(0, -16),
    TILE_STEP(0, -16), TILE_STEP(0, -16), TILE_STEP(0, -16), TILE_STEP(16, 0),
};

s32 gKalay_TourGroupWalkScript[] = {
    SC_ARG(2),       SC_MOVE_BY(-PIXELS(80), 0, 0),
    SC_SYNC,         SC_ARG(1),
    SC_SET(0x23, 1), SC_WAIT(1),
    SC_END,
};

s32 gKalay_TourGroupLeaveScript[] = {
    SC_ARG(2),       SC_MOVE_BY(-PIXELS(72), 0, 0),
    SC_SYNC,         SC_MOVE_BY(-PIXELS(100), 0, 0),
    SC_SYNC,         SC_MOVE_BY(-PIXELS(100), 0, 0),
    SC_SYNC,         SC_ARG(1),
    SC_SET(0x23, 1), SC_POS(0, 0, 0),
    SC_WAIT(1),      SC_END,
    SC_ARG(2),       SC_MOVE(PIXELS(192), 0, PIXELS(222)),
    SC_SYNC,         SC_POS(0, 0, 0),
    SC_WAIT(1),      SC_END,
};

s32 gKalay_TouristFidgetScript[] = {
    SC_CALL(Kalay_UpdateTouristFidget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKalay_GuideCountScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(228), 0, PIXELS(296)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(40),
    SC_ARG(2),
    SC_MOVE(PIXELS(212), 0, PIXELS(296)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(40),
    SC_ARG(2),
    SC_MOVE(PIXELS(192), 0, PIXELS(296)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(40),
    SC_SET(0x23, 1),
    SC_ARG(2),
    SC_MOVE(PIXELS(178), 0, PIXELS(272)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_GuidePaceScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(164), 0, PIXELS(278)),
    SC_SYNC,
    SC_MOVE(PIXELS(182), 0, PIXELS(304)),
    SC_SYNC,
    SC_MOVE(PIXELS(238), 0, PIXELS(304)),
    SC_SYNC,
    SC_MOVE(PIXELS(246), 0, PIXELS(296)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xd000),
    SC_WAIT(40),
    SC_SET(0x23, 1),
    SC_END,
};

s32 gKalay_HeadGuideStopScript[] = {
    SC_SET(SW_TARGET_X, ACTOR_NO_TARGET),
    SC_SET(SW_TARGET_Y, ACTOR_NO_TARGET),
    SC_SET(SW_TARGET_Z, ACTOR_NO_TARGET),
    SC_SET(SW_VELOCITY_X, 0),
    SC_SET(SW_VELOCITY_Y, 0),
    SC_SET(SW_VELOCITY_Z, 0),
    SC_WAIT(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(184), 0, PIXELS(294)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_SET(0x23, 1),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_SecondGuideStopScript[] = {
    SC_SET(SW_TARGET_X, ACTOR_NO_TARGET),
    SC_SET(SW_TARGET_Y, ACTOR_NO_TARGET),
    SC_SET(SW_TARGET_Z, ACTOR_NO_TARGET),
    SC_SET(SW_VELOCITY_X, 0),
    SC_SET(SW_VELOCITY_Y, 0),
    SC_SET(SW_VELOCITY_Z, 0),
    SC_WAIT(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(198), 0, PIXELS(300)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_SET(0x23, 1),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_HeadGuideReturnScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(178), 0, PIXELS(272)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_SET(0x23, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_SecondGuideReturnScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(255), 0, PIXELS(289)),
    SC_SYNC,
    SC_MOVE(PIXELS(265), 0, PIXELS(280)),
    SC_SYNC,
    SC_MOVE(PIXELS(284), 0, PIXELS(272)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_SET(0x23, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gKalay_PartyFidgetScript[] = {
    SC_CALL(Scene_LookAboutRandomly),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKalay_PartyGatherScript[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(388), 0, PIXELS(300)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_END,
};

s32 gKalay_FrostGrowScript[] = {
    SC_ADD(SW_SCALE_X, 546),
    SC_ADD(SW_SCALE_Y, 546),
    SC_WAIT(1),
    SC_LOOP(108, SC_TOP),
    SC_END,
};

struct SceneEntrance gKalay_NoEntrances[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalay_TownEntrances[] = {
    {0, CONDITION_ALWAYS, 224, 0, 280, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 72, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 408, 0, 184, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 408, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 408, 0, 408, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 328, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 120, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 312, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 152, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 224, 0, 464, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 56, 0, 272, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 224, 0, 88, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 296, 0, 408, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {13, CONDITION_ALWAYS, 152, 0, 232, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {14, CONDITION_ALWAYS, 440, 0, 88, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalay_PalaceGateEntrances[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 192, 0, 280, FACING_NORTH, 0, 0, 0, 400, 352, 0},
    {2, CONDITION_ALWAYS, 192, 0, 232, FACING_SOUTH, 0, 0, 0, 400, 352, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalay_CaveEntrances[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 248, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 424, 0, 520, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 440, 0, 540, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalay_Entrances4[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 296, 0, 104, FACING_SOUTH, 0, 100, 50, 440, 445, 0},
    {2, CONDITION_ALWAYS, 280, 0, 392, FACING_NORTH, 0, 100, 50, 440, 445, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalay_Entrances5[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 296, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 632, 0, 456, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 152, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 568, 0, 152, FACING_SOUTH, 0, 390, 0, 630, 220, 0},
    {5, CONDITION_ALWAYS, 472, 0, 72, FACING_SOUTH, 0, 390, 0, 630, 220, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalay_TunnelEntrances[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 408, 0, 632, FACING_SOUTH, 0, 85, 400, 465, 880, 0},
    {2, CONDITION_ALWAYS, 296, 0, 488, FACING_SOUTH, 0, 85, 400, 465, 880, 0},
    {3, CONDITION_ALWAYS, 744, 0, 488, FACING_SOUTH, 0, 595, 400, 930, 880, 0},
    {4, CONDITION_ALWAYS, 856, 0, 856, FACING_NORTH, 0, 595, 400, 930, 880, 0},
    {SCENE_TABLE_END},
};

u32 gKalay_Exits[] = {
    SCENE_EXITS(99),        SCENE_EXIT(1, 100, 1),  SCENE_EXIT(2, 100, 2),
    SCENE_EXIT(3, 100, 3),  SCENE_EXIT(4, 100, 4),  SCENE_EXIT(5, 100, 6),
    SCENE_EXIT(6, 100, 7),  SCENE_EXIT(7, 100, 10), SCENE_EXIT(8, 101, 1),
    SCENE_EXIT(9, 2, 20),   SCENE_EXIT(10, 2, 19),  SCENE_EXIT(11, 102, 1),
    SCENE_EXIT(12, 100, 5), SCENE_EXIT(13, 100, 8), SCENE_EXIT(14, 156, 4),
    SCENE_EXIT(0, 102, 0),  SCENE_EXIT(1, 99, 11),  SCENE_EXIT(2, 103, 1),
    SCENE_EXIT(5, 103, 20), SCENE_EXIT(0, 153, 0),  SCENE_EXIT(1, 154, 2),
    SCENE_EXIT(2, 2, 60),   SCENE_EXIT(10, 2, 71),  SCENE_EXIT(0, 154, 0),
    SCENE_EXIT(1, 155, 5),  SCENE_EXIT(2, 153, 1),  SCENE_EXIT(0, 155, 0),
    SCENE_EXIT(1, 156, 2),  SCENE_EXIT(2, 156, 3),  SCENE_EXIT(3, 155, 4),
    SCENE_EXIT(4, 155, 3),  SCENE_EXIT(5, 154, 1),  SCENE_EXIT(0, 156, 0),
    SCENE_EXIT(1, 103, 11), SCENE_EXIT(2, 155, 1),  SCENE_EXIT(3, 155, 2),
    SCENE_EXIT(4, 99, 14),  SCENE_EXITS_END,
};

struct ScenePlacement gKalay_NoPlacements[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gKalay_TownPlacements[] = {
    { 0x99, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(188), 0, PIXELS(476), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x99, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(260), 0, PIXELS(476), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(222), 0, PIXELS(382), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(134), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(115), 0, PIXELS(453), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(484), 0, PIXELS(258), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(359), 0, PIXELS(314), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(404), 0, PIXELS(268), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(154), 0, PIXELS(119), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(198), 0, PIXELS(84), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(199), 0, PIXELS(233), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(46), 0, PIXELS(217), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(468), 0, PIXELS(434), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(464), 0, PIXELS(416), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(408), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x76, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(278), 0, PIXELS(318), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(440), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(56), 0, PIXELS(332), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalay_TourGroupPlacements[] = {
    { 0x84, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x94, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x42, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x43, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalay_PalaceGatePlacements[] = {
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x99, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(232), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x99, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(212), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalay_CavePlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x99, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(198), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x111, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x31, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x44, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, 0xfd6, 7,
        PIXELS(136), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalay_TunnelPlacements[] = {
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(826), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(808), 0, PIXELS(732), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x44, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x5c, 0x5b, ACTOR_STAND,
        PIXELS(632), 0, PIXELS(712), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gKalay_NoEvents[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalay_TownEvents[] = {
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_OpenDoorAndExit) },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_WatchTourGroupDeparture) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_WatchTourGroupDeparture) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKalayWelcomeInPeace) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersProtectPeace) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKalayHammetGenerousMerchant) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgKalayHammetOnJourney) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgKalayNorthernBridgeRebuilt) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKalayLamakanTrouble) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgKalayElderTownNews) },
    { EVENT_TALK, 15, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInnFestivalTourists) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgKalayFormerInnFestivalTourists) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgKalayChildWantsFestival) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKalayTraderTownNews) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgKalayHammetBusinessSkill) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_TalkAboutLeavingHome) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgKalayHammetCapturedRumor) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgKalayHammetWentNorth) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_TalkToWeaponMerchant) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_TalkToArmorMerchant) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_TalkToItemMerchant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayWarriorHonestEyes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKalayGuardsJudgeVisitors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKalayHammetPeaceAndLiberty) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKalayHammetCanBeRash) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersWentToVault) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayStrangeEvents) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKalayElderThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKalayWishSavedForFestival) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgKalayChildFestivalDream) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKalayTraderThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgKalayHammetValuesTrust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKalayTrustAgainstForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgKalayLunpaNorthOfVault) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgKalayRumorsHideTruth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgKalayWishBetterWeapons) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgKalayWeaponsHaveSoul) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgKalayPalaceWaterSupply) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 25, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 26, FLAG_KALAY_TOWN_NUT_CAUGHT, EVENT_SCRIPT(Kalay_CatchTownNut) },
    { SEARCH(SEARCH_JAR), 100, 0xf81, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_JAR), 101, 0xf82, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_SearchWell) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalay_PalaceGateEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_EnterPalace) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersHandleAffairs) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalaySoldiersRescueHammet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayPrayForHammetSafety) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKalayWarriorsWillSaveHammet) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalay_CaveEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_AskAboutCaveAccess) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayPassageClosedByOrders) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 12, 0xfd6, EVENT_SCRIPT(Kalay_CatchCaveNut) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalay_Events4[] = {
    { 0x31, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { SEARCH(SEARCH_CHEST), 100, 0xf22, FIND_ITEM(ITEM_APPLE) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalay_Events5[] = {
    { 0x31, 1, CONDITION_ALWAYS, 1 },
    { 0x31, 2, CONDITION_ALWAYS, 2 },
    { 0x21, 3, CONDITION_ALWAYS, 3 },
    { 0x31, 4, CONDITION_ALWAYS, 4 },
    { 0x21, 5, CONDITION_ALWAYS, 5 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalay_TunnelEvents[] = {
    { 0x21, 1, CONDITION_ALWAYS, 1 },
    { 0x21, 2, CONDITION_ALWAYS, 2 },
    { 0x21, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_TalkAboutLayana) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKalayTreasureBeyondDoors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, FLAG_TUNNEL_FIRST_PASSAGE_OPEN, EVENT_SCRIPT(Kalay_UpdateTunnelPassages) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, FLAG_TUNNEL_SECOND_PASSAGE_OPEN, EVENT_SCRIPT(Kalay_UpdateTunnelPassages) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_PushTunnelBlock) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_PushTunnelBlock) },
    { TOUCH_WALKING_INTO(FACING_WEST), 14, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_PushTunnelBlock) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_PushTunnelBlock) },
    { TOUCH_WALKING_INTO(FACING_WEST), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_PushTunnelBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, FLAG_TUNNEL_FROST_TARGET_FROZEN, EVENT_SCRIPT(Kalay_FreezeFrostTarget) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_StopTrackingLeader) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_StartTrackingLeader) },
    { EVENT_TOUCH, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_DisableLeaderAutomaticPriority) },
    { EVENT_TOUCH, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_EnableLeaderAutomaticPriority) },
    { SEARCH(SEARCH_CHEST), 100, 0xf1c, FIND_ITEM(ITEM_SPIRIT_GLOVES) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Kalay_RecruitDjinn) },
    { SEARCH(SEARCH_CHEST), 102, 0xf1e, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_CHEST), 103, 0xf1f, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_CHEST), 104, 0xf20, FIND_COINS(200) },
    { SEARCH(SEARCH_CHEST), 105, 0xf21, FIND_ITEM(ITEM_POTION) },
    { SCENE_EVENTS_END },
};
// clang-format on

u16 gKalay_DoorCellSteps[] = {
    MAP_CELL_STEP(35, 33, 1, 2, 5),
    MAP_CELL_STEP(37, 33, 1, 2, 5),
    MAP_CELL_STEPS_END,
};

struct MapCellAnim gKalay_DoorCells[] = {
    {gKalay_DoorCellSteps, 36, 10}, {gKalay_DoorCellSteps, 57, 7},
    {gKalay_DoorCellSteps, 57, 11}, {gKalay_DoorCellSteps, 57, 23},
    {gKalay_DoorCellSteps, 52, 25}, {gKalay_DoorCellSteps, 39, 14},
    {gKalay_DoorCellSteps, 51, 14}, {gKalay_DoorCellSteps, 41, 25},
};

s32 gKalay_TunnelParticleScript[] = {
    SC_CALL(Kalay_UpdateTunnelParticle),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// @file
/// Hammet Palace (HammetPalace, scene 103), the rooms of Lord Hammet's palace
/// in Kalay. The scene before its door, also Hammet Palace in the game's area
/// names, is kalay.c's Kalay2.
///
/// Hammet, the merchant who founded Kalay, has been held in Lunpa since soon
/// after Mt. Aleph erupted, and his wife, Lady Layana, keeps the palace. Two
/// scripted scenes in the room where she stands frame the party's part in his
/// rescue, and between them what she says follows the party's progress:
///
/// - Ivan's report (entrance 20), when the guards at the door bring the party
///   in (kalay.c). Layana had hoped Hammet came back with Ivan. The ransom was
///   paid long ago, yet Dodonpa, Lunpa's leader and grandson of Lunpa the
///   noble thief, still holds him, and Layana will not risk his life by
///   sending her soldiers, nor let the party go, who have a quest of their
///   own. Ivan presses her, and she tells of a prophecy, that Ivan would set
///   out with warriors from Vale, and of an oath: Adepts at the south-western
///   edge of the world once saved Hammet's life, and their Jupiter Adept gave
///   him the Shaman's Rod and Ivan, asking him to use the rod to find
///   something the party will need on their journey. She sends everyone back
///   to their posts (FLAG_IVAN_REPORTED_TO_LAYANA).
/// - Waiting for news. Until the party has heard in Lunpa that Hammet is held
///   there (place flag FLAG_PLACE_TEMP_21, set by kalay.c's scenes), Layana
///   says Dodonpa may have spies in Kalay. Afterwards she stands elsewhere in
///   her room, and the first time they speak she scolds `Isaac, you sneaked
///   into Lunpa? That's far too dangerous!` and asks them not to go back
///   (FLAG_LAYANA_SCOLDED_LUNPA_VISIT); from then on she talks of Dodonpa's
///   greed.
/// - Hammet's homecoming (entrance 9, the first time once Hammet is free,
///   FLAG_LUNPA_TRADE_REOPENED; FLAG_HAMMET_HOMECOMING_SEEN). Hammet welcomes
///   the party. Layana is harsh: she had told Ivan time and again not to go to
///   Lunpa, and the rescue was Kalay's to make, when their quest is to recover
///   the Elemental Stars. Ivan takes the blame and decides that they leave at
///   once on Felix's trail, and Hammet sends them to Hesperia, the continent
///   west of Angara, with the Shaman's Rod, as the Jupiter Adept said.
///   Afterwards Hammet repeats it, Layana and the palace's people speak of
///   keeping his return secret, and they tell of the door to the underground
///   passage, where Hammet keeps his treasure, opened for the party in thanks:
///   entering by entrance 10 or 11 with flag 0x915 set draws it open.
///
/// The people of the palace (actors 12 to 16) have one line before Hammet is
/// free and another after. Walking north into a door (triggers 2, 3, 4 and
/// 14) opens it; the other exits join the rooms plainly. Exit 1 leads out to
/// the front of the palace (Kalay2's entrance 2), and exit 11 down into the
/// Kalay Tunnel (its entrance 1). Searching finds a turban, the symbol of
/// Kalay's soldiers, and two books, `Business and You` and `The Good Host`; a
/// chest holds a Water Jacket and a barrel a Nut.
#include "game_state.h"
#include "scene_ids.h"
#include "unit/scene/hammet_palace.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
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
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "flags.h"
#include "songs.h"
#include "event.h"

// The palace's actors: the party keeps its slots, the rest follow the
// placement table from 8.
#define LAYANA 8
#define HAMMET 9
#define SOLDIER_1 10 ///< the soldiers of Kalay in the meeting
#define SOLDIER_2 11

// Entrance setup and the two long conversations use the helpers below.
static void HammetPalace_SetUpEntrance(void);
static void HammetPalace_IvanReportsToLayana(void);
static void HammetPalace_FacePartyOutward(s32 waitFrames);
static void HammetPalace_FacePartyNorth(s32 waitFrames);
static void HammetPalace_HammetsHomecoming(void);

/// Advances within the current conversation's consecutive message block.
static __inline__ void HammetPalace_AdvanceMessage(s32 amount) {

  gEventWork->message += amount;
}

/// Selects Hammet Palace's entrances, or the fallback table for any other
/// scene number.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_HammetPalace) {
    return gHammetPalaceEntrances;
  }
  return gHammetPalaceEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gHammetPalaceExits; }

/// Applies placement clipping before returning Hammet Palace's actor rows.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_HammetPalace) {
    FieldScene_PrepareActors(gHammetPalacePlacements);
    return gHammetPalacePlacements;
  }
  return gHammetPalacePlacementsOther;
}

/// Selects Hammet Palace's dialogue and doorway handlers.
const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_HammetPalace) {
    return gHammetPalaceEvents;
  }
  return gHammetPalaceEventsOther;
}

/// Talking to one of the people of Hammet Palace (actor 13) before Hammet is
/// free: he hopes for his release. `Lord Hammet will be released soon. Right?`
static void Event_AskWhetherHammetWillBeReleased(void) {
  EV_BEGIN();
  MESSAGE_SET(MsgKalayLordHammetWillReleasedSoon);
  Engine_EventAskYesNo(13, 0);
  EV_END();
}

/// Talking to one of the people of Hammet Palace (actor 16) before Hammet is
/// free: he worries for Hammet's silk trade. `Has the legacy of Lord Hammet's
/// silk trading come to an end?`
static void Event_AskWhetherSilkTradeHasEnded(void) {
  EV_BEGIN();
  MESSAGE_SET(MsgKalayHasLegacyLordHammetsSilk);
  Engine_EventAskYesNo(16, 0);
  EV_END();
}

/// Talking to Layana in Hammet Palace, the first time after the party has
/// heard in Lunpa that Hammet is held there (place flag FLAG_PLACE_TEMP_21).
/// `Isaac, you sneaked into Lunpa? That's far too dangerous!` She is glad
/// Hammet is unhurt, but asks the party not to go back, and to fulfill their
/// quest.
static void Event_LayanaScoldsLunpaVisit(void) {

  EV_BEGIN();
  Engine_ActorShowEmote(LAYANA, EMOTE_IN_FRONT, 60);
  MESSAGE_SET(MsgKalayIsaacSneakedIntoLunpaThats);
  Engine_EventShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorRunRepeatedMotion(LAYANA, 2);
  Engine_EventShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_SHAKE_HEAD);
  Engine_EventShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  Engine_EventShowMessageAndWait(LAYANA, 0, 10);
  Engine_GameFlagSet(FLAG_LAYANA_SCOLDED_LUNPA_VISIT);
  EV_END();
}

/// Walking north into a door of Hammet Palace (triggers 2, 3, 4 and 14): it
/// opens, and the leader walks through it to the next room.
static void Event_LeaveThroughDoor(void) {

  struct EventWork *work;

  work = gEventWork;
  EV_BEGIN();
  WAIT(10);
  if (work->touchedTrigger == 4) {
    SE(SE_188);
  } else {
    SE(SE_DOOR_OPEN);
  }
  Map_ClearLayerEntryFlag(1);
  Map_ClearLayerEntryFlag(2);
  WAIT(10);
  OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (work->touchedTrigger == 4) {
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  } else {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  }
  WAIT(16);
  Engine_EventRequestExit(work->touchedTrigger);
  // Restore the layer entries before returning control to the field engine.
  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
  EV_END();
}

/// Opens Hammet Palace with its window transition and runs entrance-specific
/// setup.
s32 Scene_Initialize(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  if (gGameState.scene == (u32)&SceneId_HammetPalace) {
    HammetPalace_SetUpEntrance();
  }
  return 0;
}

/// Sets up the room the party enters by. In Layana's room (entrance 9), Hammet
/// stands beside her once he is free, and his homecoming plays the first time;
/// before that she stands elsewhere once the party has heard of him in Lunpa
/// (FLAG_PLACE_TEMP_21). By the door to the underground passage (entrances 10
/// and 11) the door is drawn open once flag 0x915 is set. Entrance 20 is Ivan's
/// report.
static void HammetPalace_SetUpEntrance(void) {
  struct FieldActor *actor;
  s32 facing;
  s32 waitingX = PIXELS(906);
  s32 waitingZ = PIXELS(422);

  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);

  switch (gGameState.entrance) {
  case 9:
    if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
      // Hammet is home.
      actor = Object_GetById(LAYANA);
      facing = FACING_EAST + FACING_STEP;
      actor->facing = facing;

      if (Engine_GameFlagIsSet(FLAG_HAMMET_HOMECOMING_SEEN) == 0) {
        HammetPalace_HammetsHomecoming();
      }
    } else {
      Engine_ActorSetPosition(HAMMET, 0, 0);
      if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_21) != 0) {
        Engine_ActorSetPosition(LAYANA, waitingX, waitingZ);
        actor = Object_GetById(LAYANA);
        facing = FACING_NORTH + FACING_STEP;
        actor->facing = facing;
      }
    }
    break;

  case 10:
  case 11:
    if (Engine_GameFlagIsSet(0x915) != 0) {
      Map_CopyCellsTo(58, 70, 54, 70, 4, 3);
      {
        Map_CopyCellAttributes(55, 9, 2, 1, 55, 8);
      }
      Engine_MapRedraw();
      Engine_TaskWait(1);
    }
    break;

  case 20:
    Engine_ActorSetPosition(HAMMET, 0, 0);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      HammetPalace_IvanReportsToLayana();
    }
    break;

  default:
    break;
  }
}

/// Ivan's report in Hammet Palace, when the guards at the door bring the party
/// to Layana: she had hoped Hammet came back with him.
/// The ransom was paid long ago, yet Dodonpa holds Hammet still; Layana will
/// not risk him by sending the soldiers, nor the party. She tells of the
/// prophecy, the Jupiter Adept who gave Hammet the Shaman's Rod and Ivan, and
/// what the rod is to find, then sends everyone back to their posts.
static void HammetPalace_IvanReportsToLayana(void) {

  struct EventWork *work;
  s32 advanceAfterReply;
  const s32 *partyScript;

  EV_BEGIN();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  ColorBuffer_ApplySource(0x10002, 0);
  // Position the participants under the closed screen before revealing them.
  ColorBuffer_ApplyTarget(0x10002, 0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  work = gEventWork;
  work->transitionFrames = 24;
  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  OBJ_POS_SET(LAYANA, 856, 440);
  OBJ_POS_SET(ACTOR_PARTY_LEADER, 856, 486);
  OBJ_POS_SET(GARET, 848, 502);
  OBJ_POS_SET(IVAN, 872, 486);
  OBJ_POS_SET(MIA, 880, 502);
  OBJ_POS_SET(SOLDIER_1, 840, 518);
  OBJ_POS_SET(SOLDIER_2, 888, 518);
  Camera_MoveTo(PIXELS(864), -1, PIXELS(472), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  WAIT(40);
  Engine_ActorRunRepeatedMotion(LAYANA, 1);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  WAIT(10);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  WAIT(20);
  OBJ_DIR(IVAN, FACING_WEST, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  OBJ_DIR(GARET, FACING_NORTHEAST, 0);
  OBJ_DIR(MIA, FACING_NORTHWEST, 10);
  OBJ_MODE(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  WAIT(20);
  HammetPalace_FacePartyNorth(20);
  Actor_SetAttachedEffect(LAYANA, 0x102);
  Engine_ActorStartRepeatedMotion(LAYANA, 2);
  WAIT(60);
  OBJ_DIR(LAYANA, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  OBJ_EMOTE(SOLDIER_1, EMOTE_IN_FRONT, 0);
  Engine_ActorStartRepeatedMotion(SOLDIER_1, 2);
  WAIT(60);
  OBJ_DIR(SOLDIER_1, FACING_NORTHEAST + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(SOLDIER_2, 2);
  OBJ_DIR(SOLDIER_2, FACING_WEST + FACING_STEP, 40);
  OBJ_DIR(SOLDIER_1, FACING_NORTH + FACING_STEP, 0);
  OBJ_DIR(SOLDIER_2, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(SOLDIER_2, ANIM_NOD);
  WAIT(10);
  Engine_ActorRunRepeatedMotion(LAYANA, 2);
  OBJ_DIR(LAYANA, FACING_SOUTHEAST + FACING_STEP, 10);
  OBJ_EMOTE(LAYANA, 0x105, 60);
  Engine_ActorRunRepeatedMotion(SOLDIER_2, 2);
  Engine_ActorSetAnimationAndWait(SOLDIER_2, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimationAndWait(SOLDIER_2, ANIM_SHAKE_HEAD);
  ColorBuffer_ApplyTarget(0x10000, 0);
  Engine_ColorBufferInterpolate(40);
  Engine_TaskWait(60);
  OBJ_EMOTE(LAYANA, 0x105, 60);
  MESSAGE_SET(MsgKalayWhenHeardWereBackIvan);
  // The following replies advance through this conversation's message block.
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorRunRepeatedMotion(SOLDIER_1, 2);
  Event_ShowMessageAndWait(SOLDIER_1, 0, 10);
  Engine_ActorSetAnimationAndWait(SOLDIER_2, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SOLDIER_2, 0, 10);
  OBJ_DIR(IVAN, FACING_SOUTHWEST, 10);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | IVAN,
                           0, 10);
  Engine_ActorRunRepeatedMotion(SOLDIER_2, 2);
  Event_ShowMessageAndWait(SOLDIER_2, 0, 10);
  OBJ_MODE(SOLDIER_1, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SOLDIER_1, 0, 10);
  OBJ_DIR(IVAN, FACING_NORTH, 10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | IVAN, 0, 10);
  OBJ_EMOTE(LAYANA, 0x101, 60);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorRunRepeatedMotion(SOLDIER_2, 2);
  Event_ShowMessageAndWait(SOLDIER_2, 0, 10);
  OBJ_DIR(IVAN, FACING_SOUTHEAST, 10);
  Engine_ActorSetAnimationAndWait(SOLDIER_1, ANIM_NOD);
  Event_ShowMessageAndWait(SOLDIER_1, 0, 10);
  OBJ_DIR(IVAN, FACING_SOUTHWEST, 10);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | IVAN,
                           0, 10);
  Engine_ActorSetAnimationAndWait(SOLDIER_1, ANIM_NOD);
  OBJ_DIR(IVAN, FACING_WEST, 10);
  Event_AskYesNo(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | IVAN, 0);
  Actor_SetAttachedEffect(LAYANA, 0x102);
  WAIT(60);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  WAIT(10);
  OBJ_DIR(IVAN, FACING_NORTH, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Event_ShowMessageAndWait(GARET, 0, 10);
  OBJ_EMOTE(LAYANA, 0x105, 60);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(MIA, 0, 40);
  OBJ_DIR(LAYANA, FACING_EAST + FACING_STEP, 40);
  OBJ_SPEED(LAYANA, 2 * PIXELS(1) / 5, PIXELS(1) / 5);
  Actor_WalkToAndWait(LAYANA, 892, 440);
  WAIT(40);
  OBJ_DIR(LAYANA, FACING_NORTH + FACING_STEP, 20);
  OBJ_EMOTE(LAYANA, 0x105, 60);
  HammetPalace_FacePartyOutward(60);
  HammetPalace_FacePartyNorth(40);
  Actor_WalkToAndWait(LAYANA, 856, 440);
  WAIT(40);
  OBJ_DIR(LAYANA, FACING_WEST + FACING_STEP, 20);
  OBJ_EMOTE(LAYANA, 0x105, 60);
  OBJ_DIR(SOLDIER_1, FACING_NORTHEAST + FACING_STEP, 0);
  OBJ_DIR(SOLDIER_2, FACING_WEST + FACING_STEP, 40);
  OBJ_DIR(SOLDIER_1, FACING_NORTH + FACING_STEP, 0);
  OBJ_DIR(SOLDIER_2, FACING_NORTHWEST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(LAYANA, 1);
  Event_ShowMessageAndWait(LAYANA, 0, 20);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(LAYANA, 0, 20);
  Engine_ActorRunRepeatedMotion(SOLDIER_1, 2);
  Event_ShowMessageAndWait(SOLDIER_1, 0, 10);
  Engine_ActorRunRepeatedMotion(LAYANA, 2);
  OBJ_DIR(LAYANA, FACING_SOUTH + FACING_STEP, 10);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  OBJ_DIR(GARET, FACING_SOUTH, 0);
  OBJ_DIR(IVAN, FACING_SOUTHWEST, 0);
  OBJ_DIR(MIA, FACING_SOUTHWEST, 0);
  OBJ_DIR(SOLDIER_2, FACING_WEST + FACING_STEP, 40);
  OBJ_DIR(SOLDIER_1, FACING_NORTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(SOLDIER_1, 0, 20);
  Engine_ActorRunRepeatedMotion(SOLDIER_2, 2);
  WAIT(20);
  OBJ_DIR(SOLDIER_2, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(SOLDIER_2, ANIM_NOD);
  Event_ShowMessageAndWait(SOLDIER_2, 0, 10);
  OBJ_DIR(SOLDIER_2, FACING_WEST + FACING_STEP, 40);
  OBJ_MODE(SOLDIER_1, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(SOLDIER_2, ANIM_NOD);
  OBJ_DIR(SOLDIER_1, FACING_NORTH + FACING_STEP, 0);
  OBJ_DIR(SOLDIER_2, FACING_NORTHWEST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(SOLDIER_1, 2);
  Event_ShowMessageAndWait(SOLDIER_1, 0, 20);
  HammetPalace_FacePartyNorth(20);
  Engine_ActorRunRepeatedMotion(LAYANA, 2);
  WAIT(40);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Actor_SetAttachedEffect(LAYANA, 0x102);
  WAIT(60);
  Event_ShowMessageAndWait(LAYANA, 0, 20);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_SHAKE_HEAD);
  WAIT(20);
  OBJ_MODE(LAYANA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(LAYANA, 0, 20);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  OBJ_DIR(GARET, FACING_NORTHEAST, 20);
  Event_OpenMessage(GARET, 0);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  advanceAfterReply = 1;
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message++;
    advanceAfterReply = 0;
  }
  Event_ShowMessageAndWait(GARET, 0, 10);
  if (advanceAfterReply != 0) {
    gEventWork->message++;
  }
  OBJ_DIR(IVAN, FACING_WEST, 10);
  Actor_SetAttachedEffect(IVAN, 0x102);
  WAIT(60);
  OBJ_EMOTE(LAYANA, 0x101, 0);
  OBJ_DIR(LAYANA, FACING_SOUTHEAST + FACING_STEP, 40);
  OBJ_DIR(LAYANA, FACING_SOUTH + FACING_STEP, 20);
  OBJ_DIR(LAYANA, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  OBJ_DIR(IVAN, FACING_NORTH, 10);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(GARET, FACING_NORTH, 10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | IVAN, 0, 10);
  OBJ_EMOTE(LAYANA, 0x107, 60);
  Engine_EventShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_SHAKE_HEAD);
  Event_OpenMessage(LAYANA, 0);
  OBJ_DIR(IVAN, FACING_WEST, 0);
  OBJ_DIR(GARET, FACING_NORTHEAST, 0);
  OBJ_DIR(MIA, FACING_NORTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    WAIT(20);
    Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
    Event_ShowMessageAndWait(LAYANA, 0, 10);
    gEventWork->message++;
  } else {
    WAIT(20);
    Engine_ActorSetAnimationAndWait(LAYANA, ANIM_SHAKE_HEAD);
    gEventWork->message++;
    Event_ShowMessageAndWait(LAYANA, 0, 10);
  }
  HammetPalace_FacePartyNorth(20);
  OBJ_DIR(IVAN, FACING_SOUTHEAST, 0);
  OBJ_DIR(MIA, FACING_NORTHWEST, 40);
  OBJ_DIR(IVAN, FACING_NORTH, 0);
  OBJ_DIR(MIA, FACING_NORTH, 10);
  Event_ShowMessageAndWait(MIA, 0, 20);
  OBJ_EMOTE(LAYANA, 0x105, 60);
  Event_ShowMessageAndWait(LAYANA, 0, 40);
  Engine_ActorRunRepeatedMotion(LAYANA, 2);
  Event_ShowMessageAndWait(LAYANA, 0, 40);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  WAIT(20);
  OBJ_SPEED(IVAN, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(IVAN, 868, 472);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | IVAN, 0, 10);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  HammetPalace_FacePartyOutward(40);
  HammetPalace_FacePartyNorth(20);
  OBJ_EMOTE(LAYANA, 0x105, 60);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Engine_ActorRunRepeatedMotion(LAYANA, 1);
  OBJ_DIR(LAYANA, FACING_SOUTH + FACING_STEP, 10);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  OBJ_DIR(LAYANA, FACING_SOUTHEAST + FACING_STEP, 10);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Actor_SetAttachedEffect(MIA, 0x102);
  WAIT(40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | IVAN, 0, 10);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  Event_OpenMessage(LAYANA, 0);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  OBJ_DIR(GARET, FACING_NORTHEAST, 0);
  OBJ_DIR(IVAN, FACING_SOUTHWEST, 0);
  OBJ_DIR(MIA, FACING_NORTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    WAIT(20);
    Actor_SetAttachedEffect(LAYANA, 0x102);
    WAIT(40);
    Event_ShowMessageAndWait(LAYANA, 0, 10);
    gEventWork->message += 2;
  } else {
    WAIT(20);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    gEventWork->message++;
    Event_ShowMessageAndWait(GARET, 0, 10);
    Actor_SetAttachedEffect(LAYANA, 0x102);
    WAIT(40);
    Event_ShowMessageAndWait(LAYANA, 0, 10);
  }
  OBJ_EMOTE(MIA, 0x101, 40);
  OBJ_DIR(MIA, FACING_NORTH, 10);
  Event_ShowMessage(MIA, 0);
  OBJ_DIR(GARET, FACING_NORTH, 0);
  OBJ_DIR(IVAN, FACING_NORTH, 20);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Actor_SetAttachedEffect(MIA, 0x102);
  WAIT(40);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  OBJ_DIR(GARET, FACING_NORTHEAST, 0);
  OBJ_DIR(MIA, FACING_NORTHWEST, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | IVAN, 0, 10);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorRunRepeatedMotion(LAYANA, 1);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  HammetPalace_FacePartyNorth(10);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(LAYANA, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  OBJ_MODE(GARET, ANIM_SHAKE_HEAD);
  OBJ_MODE(IVAN, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Actor_SetAttachedEffect(LAYANA, 0x102);
  WAIT(60);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  HammetPalace_FacePartyOutward(40);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  HammetPalace_FacePartyNorth(20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | IVAN, 0, 10);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  OBJ_EMOTE(MIA, 0x101, 60);
  Event_ShowMessageAndWait(MIA, 0, 10);
  OBJ_EMOTE(LAYANA, 0x108, 60);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Event_ShowMessageAndWait(MIA, 0, 10);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  OBJ_EMOTE(GARET, 0x101, 40);
  Event_ShowMessageAndWait(GARET, 0, 10);
  OBJ_DIR(LAYANA, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorRunRepeatedMotion(SOLDIER_1, 2);
  Engine_ActorSetAnimationAndWait(SOLDIER_1, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SOLDIER_1, 0, 10);
  OBJ_EMOTE(LAYANA, 0x105, 40);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  OBJ_DIR(LAYANA, FACING_SOUTHEAST + FACING_STEP, 10);
  OBJ_MODE(LAYANA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorRunRepeatedMotion(LAYANA, 1);
  OBJ_DIR(LAYANA, FACING_SOUTH + FACING_STEP, 10);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | IVAN, 0, 10);
  OBJ_EMOTE(LAYANA, EMOTE_IN_FRONT, 40);
  OBJ_DIR(LAYANA, FACING_SOUTHEAST + FACING_STEP, 10);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  HammetPalace_FacePartyOutward(40);
  HammetPalace_FacePartyNorth(20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  OBJ_MODE(LAYANA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_ActorRunRepeatedMotion(LAYANA, 1);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  WAIT(60);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  WAIT(20);
  Engine_ActorRunRepeatedMotion(LAYANA, 1);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  OBJ_MODE(LAYANA, ANIM_NOD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  OBJ_EMOTE(IVAN, 0x105, 60);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | IVAN, 0, 10);
  OBJ_DIR(MIA, FACING_NORTHWEST, 10);
  Event_ShowMessageAndWait(MIA, 0, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  OBJ_DIR(IVAN, FACING_SOUTHEAST, 10);
  OBJ_MODE(IVAN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | IVAN, 0, 10);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  OBJ_DIR(IVAN, FACING_NORTH, 0);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  OBJ_DIR(MIA, FACING_NORTH, 10);
  Event_ShowMessageAndWait(MIA, 0, 10);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorStartRepeatedMotion(SOLDIER_1, 2);
  Event_ShowMessageAndWait(SOLDIER_1, 0, 10);
  Engine_ActorSetAnimationAndWait(SOLDIER_2, ANIM_NOD);
  Event_ShowMessageAndWait(SOLDIER_2, 0, 20);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  OBJ_MODE(IVAN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | IVAN, 0, 10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventShowMessageAndWait(GARET, 0, 10);
  Engine_ActorRunRepeatedMotion(LAYANA, 1);
  Event_OpenMessage(LAYANA, 0);
  OBJ_DIR(GARET, FACING_NORTHEAST, 0);
  OBJ_DIR(IVAN, FACING_SOUTHWEST, 0);
  OBJ_DIR(MIA, FACING_NORTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    WAIT(20);
    HammetPalace_FacePartyNorth(10);
    OBJ_MODE(GARET, ANIM_NOD);
    OBJ_MODE(IVAN, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    WAIT(10);
    Event_ShowMessageAndWait(GARET, 0, 10);
    gEventWork->message += 2;
  } else {
    WAIT(10);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    gEventWork->message++;
    Event_ShowMessageAndWait(GARET, 0, 10);
    HammetPalace_FacePartyNorth(10);
    OBJ_MODE(GARET, ANIM_NOD);
    OBJ_MODE(IVAN, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    WAIT(10);
    Event_ShowMessageAndWait(GARET, 0, 10);
  }
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  Event_ShowMessageAndWait(LAYANA, 0, 40);
  OBJ_DIR(LAYANA, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(LAYANA, 1);
  WAIT(10);
  Engine_ActorRunRepeatedMotion(SOLDIER_1, 1);
  WAIT(20);
  OBJ_DIR(LAYANA, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(LAYANA, 1);
  WAIT(10);
  Engine_ActorRunRepeatedMotion(SOLDIER_2, 1);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  OBJ_MODE(SOLDIER_1, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(SOLDIER_2, ANIM_NOD);
  OBJ_SPEED(SOLDIER_1, PIXELS(1), PIXELS(1) / 2);
  OBJ_SPEED(SOLDIER_2, PIXELS(1), PIXELS(1) / 2);
  OBJ_WALK(SOLDIER_1, 848, 540);
  Actor_WalkToAndWait(SOLDIER_2, 880, 540);
  OBJ_POS_SET(SOLDIER_1, 0, 0);
  OBJ_POS_SET(SOLDIER_2, 0, 0);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  WAIT(20);
  OBJ_SPEED(GARET, PIXELS(1), PIXELS(1) / 2);
  OBJ_SPEED(IVAN, PIXELS(1), PIXELS(1) / 2);
  OBJ_SPEED(MIA, PIXELS(1), PIXELS(1) / 2);
  partyScript = gHammetPalace_PartyActions;
  Actor_SetActionScript(GARET, partyScript);
  Actor_SetActionScript(IVAN, partyScript);
  Actor_RunActionScriptAndWait(MIA, partyScript);
  work = gEventWork;
  work->transitionFrames = 16;
  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  FLAG_SET(FLAG_IVAN_REPORTED_TO_LAYANA);
  EV_END();
}

/// Turns the party outward from the group, then optionally waits.
static void HammetPalace_FacePartyOutward(s32 waitFrames) {
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  OBJ_DIR(GARET, FACING_NORTHEAST, 0);
  OBJ_DIR(IVAN, FACING_SOUTHEAST, 0);
  OBJ_DIR(MIA, FACING_NORTHWEST, 0);
  if (waitFrames != 0) {
    WAIT(waitFrames);
  }
}

/// Turns all four party members north toward Layana, then optionally waits.
static void HammetPalace_FacePartyNorth(s32 waitFrames) {
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(GARET, FACING_NORTH, 0);
  OBJ_DIR(IVAN, FACING_NORTH, 0);
  OBJ_DIR(MIA, FACING_NORTH, 0);
  if (waitFrames != 0) {
    WAIT(waitFrames);
  }
}

/// Hammet's homecoming in Hammet Palace, the first time the party walks into
/// Layana's room once he is free: Hammet welcomes them, Layana scolds Ivan for
/// going to Lunpa against her word, and Ivan decides to leave at once on the
/// quest. Hammet sends them to Hesperia with the Shaman's Rod, as the Jupiter
/// Adept said.
static void HammetPalace_HammetsHomecoming(void) {

  struct FieldActor *leader;
  struct EventWork *work;
  s32 v5;
  const s32 *partyScript;

  EV_BEGIN();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Camera_MoveTo(PIXELS(864), -1, PIXELS(536), 0);
  // Pan from the entrance while the party walks into its speaking positions.
  Engine_MapRedraw();
  Engine_TaskWait(1);
  OBJ_POS_SET(ACTOR_PARTY_LEADER, 864, 630);
  gEventWork->startTransition = v5 = 0x100;
  gEventWork->transitionFrames = 40;
  Engine_EventOpenScreen();
  Camera_SetSpeed(0x6666, 0xccc);
  Camera_MoveTo(PIXELS(864), -1, PIXELS(472), 1);
  OBJ_SPEED(ACTOR_PARTY_LEADER, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJ_SPEED(GARET, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJ_SPEED(IVAN, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJ_SPEED(MIA, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 864, 498);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
  }
  OBJ_WALK(ACTOR_PARTY_LEADER, 856, 486);
  OBJ_WALK(GARET, 848, 502);
  OBJ_WALK(IVAN, 872, 486);
  Actor_WalkToAndWait(MIA, 880, 502);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  OBJ_MODE(GARET, ANIM_STAND);
  OBJ_MODE(IVAN, ANIM_STAND);
  WAIT(10);
  HammetPalace_FacePartyNorth(10);
  OBJ_EMOTE(HAMMET, v5, 20);
  OBJ_DIR(HAMMET, FACING_SOUTH + FACING_STEP, 20);
  MESSAGE_SET(MsgKalayItsIvanHisCompanionsPerfect);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | HAMMET, 0, 10);
  OBJ_EMOTE(LAYANA, v5, 20);
  OBJ_DIR(LAYANA, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(LAYANA, 0, 20);
  OBJ_EMOTE(LAYANA, 0x107, 60);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Actor_SetAttachedEffect(MIA, 0x102);
  WAIT(60);
  OBJ_EMOTE(HAMMET, 0x102, 60);
  OBJ_DIR(HAMMET, FACING_SOUTHWEST + FACING_STEP, 10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | HAMMET, 0, 10);
  OBJ_DIR(LAYANA, FACING_EAST + FACING_STEP, 10);
  OBJ_EMOTE(LAYANA, 0x108, 20);
  Event_ShowMessageAndWait(LAYANA, 0, 40);
  Engine_ActorRunRepeatedMotion(LAYANA, 2);
  Event_ShowMessageAndWait(LAYANA, 0, 20);
  OBJ_DIR(LAYANA, FACING_SOUTHEAST + FACING_STEP, 10);
  Event_OpenMessage(LAYANA, 0);
  v5 = 1;
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    WAIT(10);
    OBJ_MODE(LAYANA, ANIM_NOD);
  } else {
    WAIT(10);
    HammetPalace_AdvanceMessage(1);
    OBJ_MODE(LAYANA, ANIM_SHAKE_HEAD);
    v5 = 0;
  }
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  if (v5 != 0) {
    HammetPalace_AdvanceMessage(1);
  }
  Engine_ActorStartRepeatedMotion(HAMMET, 2);
  Actor_SetAttachedEffect(HAMMET, 0x102);
  WAIT(80);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | HAMMET, 0, 10);
  OBJ_DIR(LAYANA, FACING_EAST + FACING_STEP, 10);
  OBJ_EMOTE(LAYANA, 0x107, 40);
  Engine_ActorRunRepeatedMotion(IVAN, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | IVAN, 0, 20);
  OBJ_EMOTE(LAYANA, 0x105, 60);
  OBJ_DIR(LAYANA, FACING_SOUTHEAST + FACING_STEP, 60);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Event_ShowMessageAndWait(GARET, 0, 10);
  OBJ_DIR(HAMMET, FACING_SOUTH + FACING_STEP, 0);
  OBJ_DIR(IVAN, FACING_SOUTHWEST, 10);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | IVAN,
                           0, 10);
  OBJ_DIR(IVAN, FACING_NORTH, 10);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(LAYANA, ANIM_NOD);
  Event_ShowMessageAndWait(LAYANA, 0, 20);
  OBJ_DIR(IVAN, FACING_WEST, 20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | IVAN,
                           0, 10);
  OBJ_DIR(MIA, FACING_NORTHWEST, 10);
  OBJ_EMOTE(MIA, 0x105, 40);
  Event_ShowMessageAndWait(MIA, 0, 10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  WAIT(80);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  WAIT(20);
  OBJ_DIR(GARET, FACING_NORTHEAST, 10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    WAIT(20);
    HammetPalace_AdvanceMessage(1);
  } else {
    WAIT(20);
    Event_ShowMessageAndWait(GARET, 0, 10);
  }
  OBJ_MODE(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  HammetPalace_FacePartyNorth(20);
  Engine_ActorRunRepeatedMotion(HAMMET, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | HAMMET, 0, 10);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Event_ShowMessageAndWait(MIA, 0, 10);
  OBJ_DIR(HAMMET, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(HAMMET, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | HAMMET, 0, 10);
  OBJ_EMOTE(IVAN, 0x101, 60);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | IVAN, 0, 10);
  Engine_ActorRunRepeatedMotion(HAMMET, 1);
  OBJ_DIR(HAMMET, FACING_SOUTH + FACING_STEP, 10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | HAMMET, 0, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Engine_ActorSetAnimationAndWait(HAMMET, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | HAMMET, 0, 10);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Event_ShowMessageAndWait(MIA, 0, 10);
  Engine_ActorRunRepeatedMotion(LAYANA, 1);
  WAIT(20);
  Event_ShowMessageAndWait(LAYANA, 0, 10);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  WAIT(20);
  partyScript = gHammetPalace_PartyActions;
  Actor_SetActionScript(GARET, partyScript);
  Actor_SetActionScript(IVAN, partyScript);
  Actor_RunActionScriptAndWait(MIA, partyScript);
  work = gEventWork;
  work->transitionFrames = 16;
  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  FLAG_SET(FLAG_HAMMET_HOMECOMING_SEEN);
  EV_END();
}

s32 gHammetPalace_PartyActions[] = {
    SC_ARG(2),       SC_MOVE(PIXELS(856), 0, PIXELS(486)),
    SC_SYNC,         SC_ARG(1),
    SC_POS(0, 0, 0), SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gHammetPalaceEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gHammetPalaceEntrances[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {HAMMET_PALACE_FROM_KALAY_2_WEST, CONDITION_ALWAYS, 240, 0, 648,
     FACING_NORTH, 0, 45, 360, 440, 680, 0},
    {2, CONDITION_ALWAYS, 88, 0, 552, FACING_SOUTH, 0, 45, 360, 440, 680, 0},
    {3, CONDITION_ALWAYS, 392, 0, 552, FACING_SOUTH, 0, 45, 360, 440, 680, 0},
    {4, CONDITION_ALWAYS, 240, 0, 408, FACING_SOUTH, 0, 45, 360, 440, 680, 0},
    {5, CONDITION_ALWAYS, 152, 0, 432, FACING_SOUTH, 0, 45, 360, 440, 680, 0},
    {6, CONDITION_ALWAYS, 264, 0, 136, FACING_NORTH, 0, 40, 20, 305, 285, 0},
    {7, CONDITION_ALWAYS, 504, 0, 152, FACING_NORTH, 0, 380, 20, 755, 290, 0},
    {8, CONDITION_ALWAYS, 448, 0, 112, FACING_SOUTH, 0, 380, 20, 755, 290, 0},
    {9, CONDITION_ALWAYS, 864, 0, 576, FACING_NORTH, 0, 750, 370, 985, 635, 0},
    {10, CONDITION_ALWAYS, 928, 0, 224, FACING_WEST, 0, 800, 20, 1040, 250, 0},
    {HAMMET_PALACE_FROM_KALAY_TUNNEL_3, CONDITION_ALWAYS, 904, 0, 72,
     FACING_SOUTH, 0, 800, 20, 1040, 250, 0},
    {12, CONDITION_ALWAYS, 616, 0, 824, FACING_NORTH, 0, 465, 640, 705, 875, 0},
    {13, CONDITION_ALWAYS, 128, 0, 808, FACING_EAST, 0, 20, 730, 260, 890, 0},
    {14, CONDITION_ALWAYS, 168, 0, 808, FACING_SOUTH, 0, 20, 730, 260, 890, 0},
    {HAMMET_PALACE_FROM_KALAY_2_EAST, CONDITION_ALWAYS, 864, 0, 576,
     FACING_NORTH, 0, 750, 370, 985, 635, 0},
    {SCENE_TABLE_END},
};

u32 gHammetPalaceExits[] = {
    SCENE_EXITS(HammetPalace),
    SCENE_EXIT(HAMMET_PALACE_TO_KALAY_2_WEST, Kalay2,
               KALAY_2_FROM_HAMMET_PALACE),
    SCENE_EXIT(2, HammetPalace, 6),
    SCENE_EXIT(3, HammetPalace, 7),
    SCENE_EXIT(4, HammetPalace, 9),
    SCENE_EXIT(5, HammetPalace, 13),
    SCENE_EXIT(6, HammetPalace, 2),
    SCENE_EXIT(7, HammetPalace, 3),
    SCENE_EXIT(8, HammetPalace, 10),
    SCENE_EXIT(9, HammetPalace, 4),
    SCENE_EXIT(10, HammetPalace, 8),
    SCENE_EXIT(HAMMET_PALACE_TO_KALAY_TUNNEL_3, KalayTunnel3,
               KALAY_TUNNEL_3_FROM_HAMMET_PALACE),
    SCENE_EXIT(12, HammetPalace, 14),
    SCENE_EXIT(13, HammetPalace, 5),
    SCENE_EXIT(14, HammetPalace, 12),
    SCENE_EXITS_END,
};

struct ScenePlacement gHammetPalacePlacementsOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gHammetPalacePlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x2d, 0x0, ACTOR_STAND,
        PIXELS(856), 0, PIXELS(440), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x31, 0x0, ACTOR_STAND,
        PIXELS(872), 0, PIXELS(440), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x99, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x99, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x6c, 0x0, ACTOR_STAND,
        PIXELS(616), 0, PIXELS(113), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(712), 0, PIXELS(246), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xa7, 0x0, ACTOR_STAND,
        PIXELS(212), 0, PIXELS(522), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(169), 0, PIXELS(114), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x7f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(114), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gHammetPalaceEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gHammetPalaceEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveThroughDoor) },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { 0x31, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveThroughDoor) },
    { EVENT_TALK, LAYANA, FLAG_PLACE_TEMP_21, MSGNO(MsgHammetPalaceDodonpaIsAVeryShrewdMan) },
    { EVENT_TALK, LAYANA, FLAG_LAYANA_SCOLDED_LUNPA_VISIT, EVENT_SCRIPT(Event_LayanaScoldsLunpaVisit) },
    { EVENT_TALK, LAYANA, FLAG_LUNPA_TRADE_REOPENED, MSGNO(MsgHammetPalaceDodonpaPlansToUseHammetTo) },
    { EVENT_TALK, LAYANA, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceNoOneInKalayKnowsHammet) },
    { EVENT_TALK, HAMMET, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceTakeYourShamansRodAndSet) },
    { EVENT_TALK, 12, FLAG_LUNPA_TRADE_REOPENED, MSGNO(MsgHammetPalaceTheTownsfolkDontKnowAboutLord) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceIHearHammetWasSavedBy) },
    { EVENT_TALK, 13, FLAG_LUNPA_TRADE_REOPENED, EVENT_SCRIPT(Event_AskWhetherHammetWillBeReleased) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceTheySayHammetsReturnIsSupposed) },
    { EVENT_TALK, 14, FLAG_LUNPA_TRADE_REOPENED, MSGNO(MsgHammetPalaceLayanaIsPuttingUpAStrong) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceILeftTheDoorToThe) },
    { EVENT_TALK, 15, FLAG_LUNPA_TRADE_REOPENED, MSGNO(MsgHammetPalaceMerchantsAreSupposedToBeProtected) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceIWonderIfAnyOfDodonpas) },
    { EVENT_TALK, 16, FLAG_LUNPA_TRADE_REOPENED, EVENT_SCRIPT(Event_AskWhetherSilkTradeHasEnded) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceTheSecretPassageIsLockedBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), LAYANA, FLAG_LAYANA_SCOLDED_LUNPA_VISIT, MSGNO(MsgHammetPalaceIsThisTheRightDecisionHammet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), LAYANA, FLAG_LUNPA_TRADE_REOPENED, MSGNO(MsgHammetPalaceIDontCareAboutTheMoney) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), LAYANA, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceImSoHappyHammetMadeIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), HAMMET, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceLayanasWordsPainHerMoreThan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_LUNPA_TRADE_REOPENED, MSGNO(MsgHammetPalaceImSureTheVillagersWouldPanic) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceImSoRelievedHammetMadeIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, FLAG_LUNPA_TRADE_REOPENED, MSGNO(MsgHammetPalaceEveryonesWorriedAboutLordHammetBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceICantLetOnToThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, FLAG_LUNPA_TRADE_REOPENED, MSGNO(MsgHammetPalaceLadyLayanaWasCryingAgainLast) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceLayanaOrderedTheDoorToThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, FLAG_LUNPA_TRADE_REOPENED, MSGNO(MsgHammetPalaceAndToThinkIAlmostWent) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceWeWillNeverLetDodonpasHenchmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, FLAG_LUNPA_TRADE_REOPENED, MSGNO(MsgHammetPalaceLordHammetIsALegendAmong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceTheKeyToTheSecretPassage) },
    { SEARCH(SEARCH_CHEST), 100, 0xf86, FIND_ITEM(ITEM_WATER_JACKET) },
    { SEARCH(SEARCH_BARREL), 101, 0xf87, FIND_ITEM(ITEM_NUT) },
    { SEARCH(SEARCH_WARDROBE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHammetPalaceTheresATurbanItsTheSymbol)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHammetPalaceTheresABookEntitledBusinessAnd)) },
    { SEARCH(SEARCH_BOOKCASE), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHammetPalaceTheresABookEntitledTheGood)) },
    { SCENE_EVENTS_END },
};
// clang-format on

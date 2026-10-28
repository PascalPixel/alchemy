/// @file
/// West Lunpa Cave (SceneId_WestLunpaCave, area name West Lunpa Cave), the cave
/// between Lunpa and the World Map, with a gate across it, where the party
/// meets Bunza on the way out of Lunpa with Hammet.
///
/// The party comes in from the World Map (WEST_LUNPA_CAVE_FROM_WORLD_MAP) and
/// from Lunpa (WEST_LUNPA_CAVE_FROM_LUNPA);
/// MB_WEST_LUNPA_CAVE_WORLD_MAP_PASSAGE and MB_WEST_LUNPA_CAVE_LUNPA_PASSAGE
/// lead back out. WEST_LUNPA_CAVE_FROM_LUNPA_UNUSED, which no exit names,
/// repeats WEST_LUNPA_CAVE_FROM_LUNPA with a shorter camera.
///
/// The gate puzzle runs on scene flags, which Scene_Initialize redraws: the
/// lowering switch brings the gate down and the raising switch lifts it, each
/// doing nothing while the gate already stands where it would put it or while
/// the gate's puddle, frozen by Frost into a pillar, props it open. Freezing
/// that puddle with the gate down raises it onto the pillar with the
/// puzzle-solved tune. Frost also freezes a puddle to the south into a pillar
/// that can be climbed, and one to the north. Its scene flags:
///
/// - FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN: the gate's puddle is frozen,
///   propping the gate open.
/// - FLAG_WEST_LUNPA_CAVE_GATE_LOWERED: the lowering switch has lowered the
///   gate.
/// - FLAG_WEST_LUNPA_CAVE_GATE_RAISED: the gate is raised.
/// - FLAG_WEST_LUNPA_CAVE_NORTH_PUDDLE_FROZEN: the north puddle is frozen.
/// - FLAG_WEST_LUNPA_CAVE_SOUTH_PUDDLE_FROZEN: the south puddle is frozen into
///   a pillar, which changes one cell's metatile behavior.
///
/// Once Donpa has let the party leave Lunpa Fortress with Hammet
/// (FLAG_DODONPA_LOCKED_UP), stepping onto
/// MB_WEST_LUNPA_CAVE_BUNZA_HIDING_PLACE the first time
/// (FLAG_LUNPA_CAVE_REUNION_SEEN) plays the reunion: Bunza, a Kalay merchant,
/// comes out from behind a rock (WestLunpaCave_HammetFindsBunza), they talk
/// (Reunion_Converse), and the party's answers decide whether it rides home in
/// Bunza's wagon (WagonChoice_Run). Riding leaves by WEST_LUNPA_CAVE_BY_WAGON
/// to WORLD_MAP_FROM_WEST_LUNPA_CAVE, where world_map.c plays the departure;
/// staying, Hammet and Bunza leave alone. Either way FLAG_BUNZAS_WAGON_LEFT is
/// set, on the World Map or here.
///
/// Unused: a fourth puddle stands at the map's origin, outside the cave's
/// camera bounds, so Frost can never reach it
/// (WestLunpaCave_UnusedFreezePuddle).

#include "adventure.h"
#include "scene_ids.h"
#include "event_layer.h"
#include "unit/scene/west_lunpa_cave.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "west_lunpa_cave.h"
#include "text/msg_ids.h"
#include "fixed_math.h"

/// The gate's puddle is frozen into a pillar, propping the gate open.
/// ★ Named from what the player sees: a small blue puddle beneath the green
/// metal gate across the cave (its render)
#define FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN FLAG_SCENE_TEMP_00
/// The lowering switch has lowered the gate.
/// ★ Named from the game: "The gate in the cave can only be opened from the
/// inside." (MsgLunpaEastGuardTrustsCaveGate)
#define FLAG_WEST_LUNPA_CAVE_GATE_LOWERED FLAG_SCENE_TEMP_01
/// The gate is raised.
/// ★ Named from the game: "The gate in the cave can only be opened from the
/// inside." (MsgLunpaEastGuardTrustsCaveGate)
#define FLAG_WEST_LUNPA_CAVE_GATE_RAISED FLAG_SCENE_TEMP_02
/// The north puddle is frozen into a pillar.
/// ★ Named from what the player sees: a small blue puddle in the cave's
/// northern passage, past the gate (its render)
#define FLAG_WEST_LUNPA_CAVE_NORTH_PUDDLE_FROZEN FLAG_SCENE_TEMP_03
/// The south puddle is frozen into a pillar that can be climbed.
/// ★ Named from what the player sees: a small blue puddle at the foot of the
/// cave's southern passage (its render)
#define FLAG_WEST_LUNPA_CAVE_SOUTH_PUDDLE_FROZEN FLAG_SCENE_TEMP_04

static void Reunion_Converse(void);
static void WagonChoice_Run(void);
static u8 WestLunpaCave_LeaderAnswersYes(void);
static u8 WestLunpaCave_GaretAsksIfNotRiding(void);
static u8 WestLunpaCave_GaretAsksAboutUnfinishedBusiness(void);
static u8 Party_ConfirmsStaying(void);
static u8 WestLunpaCave_BunzaAsksAboutUnfinishedBusiness(void);
static u8 WestLunpaCave_GaretChecksNothingLeftToDo(void);
static u8 WestLunpaCave_MiaAsksIfRidingAfterAll(void);
static u8 WestLunpaCave_BunzaCannotWait(void);
static u8 WestLunpaCave_GaretAsksAboutThingsToDo(void);
static void Party_StaysBehind(void);
static void Party_RidesWagon(void);

// The cave's scene flags, the state of its gate puzzle:
// FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN: the gate puddle is frozen, propping
// the gate open. FLAG_WEST_LUNPA_CAVE_GATE_LOWERED: the lowering switch has
// lowered the gate. FLAG_WEST_LUNPA_CAVE_GATE_RAISED: the gate is raised.
// FLAG_WEST_LUNPA_CAVE_NORTH_PUDDLE_FROZEN: the north puddle is frozen into a
// pillar. FLAG_WEST_LUNPA_CAVE_SOUTH_PUDDLE_FROZEN: the south puddle is frozen
// into a pillar.

const struct SceneEntrance *Scene_GetEntrances(void) { return gCaveEntrances; }

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

/// Both passages and the wagon departure return through this exit table.
const u32 *Scene_GetExits(void) { return gCaveExits; }

/// Only the cave scene places puddles, Hammet, Bunza and the party members.
const struct ActorTemplate *Scene_GetActorTemplates(void) {
  if (gAdventure.sceneId == (s32)&SceneId_WestLunpaCave) {
    return gCaveActorTemplates;
  }
  return gCaveNoActorTemplates;
}

/// Routes switches, Frost targets and the reunion step trigger to their
/// handlers.
const struct Trigger *Scene_GetTriggers(void) { return gCaveTriggers; }

/// Unused: Frost can never target the puddle placed at (0, 0, 0), outside the
/// cave's camera bounds. It would freeze, and no flag would remember it.
static void WestLunpaCave_UnusedFreezePuddle(void) {
  struct Actor *puddle;

  puddle = Actor_GetById(ACTOR_UNUSED_PUDDLE);
  if (puddle != NULL) {
    Actor_SetSpriteFlags(puddle, 0);
  }
}

/// In West Lunpa Cave, Frost on the south puddle: it rises as an ice pillar the
/// party can climb.
static void WestLunpaCave_FreezeSouthPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_Get(ACTOR_SOUTH_PUDDLE);
  if (puddle != NULL) {
    puddle->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    puddle->motionFlags = 0;
  }
  MapGrid_CopyCellAttributes(7, 32, 1, 1, 8, 32);
  GameFlag_Set(FLAG_WEST_LUNPA_CAVE_SOUTH_PUDDLE_FROZEN);
}

/// Animates the raised gate resting on the frozen pillar, then opens the way
/// through its metatile behaviors.
static void WestLunpaCave_GateDrawPropped(void) {
  struct Actor *pillar;

  pillar = Actor_Get(ACTOR_GATE_PUDDLE);
  Import_ActorSetAnimation(ACTOR_GATE_PUDDLE, PUDDLE_ANIMATION_FROZEN);
  if (pillar != NULL) {
    Actor_SetSpriteFlags(pillar, 0);
    pillar->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }
  MapGrid_CopyCells(41, 87, 2, 5, 21, 59);
  WaitFrames(4);
  MapGrid_CopyCells(3, 93, 1, 1, 24, 62);
  MapGrid_CopyCells(1, 94, 1, 1, 21, 55);
  MapGrid_CopyCells(43, 87, 2, 5, 21, 58);
  WaitFrames(4);
  MapGrid_CopyCells(41, 87, 2, 5, 21, 58);
  WaitFrames(4);
  WaitFrames(4);
  MapGrid_CopyCellAttributes(21, 11, 2, 2, 21, 13);
  MapGrid_CopyCellAttributes(21, 11, 1, 1, 22, 15);
  MapGrid_CopyCellAttributes(19, 17, 1, 1, 21, 14);
}

/// In West Lunpa Cave, Frost on the puddle under the gate: its pillar props the
/// gate open, and if the gate was down the player sees it rise and hears the
/// puzzle solved.
static void WestLunpaCave_FreezeGatePuddle(void) {
  struct Actor *pillar;

  pillar = Actor_Get(ACTOR_GATE_PUDDLE);
  GameFlag_Set(FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN);
  if (pillar != NULL) {
    Actor_SetSpriteFlags(pillar, 0);
    pillar->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }
  if (Event_GetFlag(FLAG_WEST_LUNPA_CAVE_GATE_RAISED) == 0) {
    Event_PlaySong(SE_GATE_MOVE);
    WestLunpaCave_GateDrawPropped();
    Event_PlaySong(MUS_PUZZLE_SOLVED);
    GameFlag_Set(FLAG_WEST_LUNPA_CAVE_GATE_RAISED);
  }
}

/// In West Lunpa Cave, Frost on the north puddle: its pillar stays frozen when
/// the party returns.
static void WestLunpaCave_FreezeNorthPuddle(void) {
  GameFlag_Set(FLAG_WEST_LUNPA_CAVE_NORTH_PUDDLE_FROZEN);
}

/// Lowers the gate in four-frame steps and restores the blocked cell
/// attributes.
static void WestLunpaCave_GateLower(void) {
  if (GameFlag_Get(FLAG_WEST_LUNPA_CAVE_GATE_RAISED) != 0) {
    MapGrid_CopyCells(41, 86, 2, 6, 21, 57);
    WaitFrames(4);
    MapGrid_CopyCells(43, 86, 2, 6, 21, 57);
    WaitFrames(4);
    MapGrid_CopyCells(41, 86, 2, 6, 21, 58);
    WaitFrames(4);
    MapGrid_CopyCells(43, 86, 2, 6, 21, 58);
    WaitFrames(4);
  }
  MapGrid_CopyCells(2, 93, 1, 1, 24, 62);
  MapGrid_CopyCells(2, 94, 1, 1, 21, 55);
  MapGrid_CopyCells(41, 86, 2, 6, 21, 59);
  WaitFrames(4);
  MapGrid_CopyCells(1, 93, 1, 1, 24, 62);
  MapGrid_CopyCells(3, 94, 1, 1, 21, 55);
  MapGrid_CopyCells(43, 86, 2, 6, 21, 59);
  WaitFrames(4);
  Actor_SetSpritePriority(ACTOR_GATE_PUDDLE, GATE_PILLAR_PRIORITY);
  MapGrid_CopyCellAttributes(19, 17, 1, 1, 22, 15);
}

/// In West Lunpa Cave, searching the lowering switch: the leader flips it and
/// the gate comes down, unless it is down already or a frozen pillar props it
/// open. "flipped the switch."
static void WestLunpaCave_FlipLoweringSwitch(void) {
  if (Event_GetFlag(FLAG_WEST_LUNPA_CAVE_GATE_LOWERED) == 0) {
    if (GameFlag_Get(FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN) == 0) {
      FieldMessage_Show(MsgFieldFlippedSwitch, 1);
      Event_PlaySong(SE_GATE_MOVE);
      WestLunpaCave_GateLower();
      GameFlag_Set(FLAG_WEST_LUNPA_CAVE_GATE_LOWERED);
      GameFlag_Clear(FLAG_WEST_LUNPA_CAVE_GATE_RAISED);
    }
  }
}

/// Raises the unpropped gate and restores its passable cell attributes.
static void WestLunpaCave_GateRaise(void) {
  MapGrid_CopyCells(41, 87, 2, 5, 21, 59);
  WaitFrames(4);
  MapGrid_CopyCells(2, 93, 1, 1, 24, 62);
  MapGrid_CopyCells(2, 94, 1, 1, 21, 55);
  MapGrid_CopyCells(43, 87, 2, 5, 21, 58);
  WaitFrames(4);
  MapGrid_CopyCells(3, 93, 1, 1, 24, 62);
  MapGrid_CopyCells(1, 94, 1, 1, 21, 55);
  MapGrid_CopyCells(41, 87, 2, 5, 21, 58);
  MapGrid_CopyCellAttributes(21, 11, 2, 2, 21, 13);
  MapGrid_CopyCellAttributes(19, 17, 1, 1, 21, 14);
}

/// In West Lunpa Cave, searching the raising switch: the leader flips it and
/// the gate goes up, unless it is up already or propped open. "flipped the
/// switch."
static void WestLunpaCave_FlipRaisingSwitch(void) {
  if (GameFlag_Get(FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN) == 0) {
    if (Event_GetFlag(FLAG_WEST_LUNPA_CAVE_GATE_RAISED) == 0) {
      FieldMessage_Show(MsgFieldFlippedSwitch, 1);
      Event_PlaySong(SE_GATE_MOVE);
      WestLunpaCave_GateRaise();
      GameFlag_Set(FLAG_WEST_LUNPA_CAVE_GATE_RAISED);
      GameFlag_Clear(FLAG_WEST_LUNPA_CAVE_GATE_LOWERED);
    }
  }
}

/// In West Lunpa Cave, Hammet finds Bunza hiding behind a rock. It plays on
/// stepping onto metatile behavior 10, by the rock Bunza hides behind, after
/// Donpa has let the party leave the fortress with Hammet
/// (FLAG_DODONPA_LOCKED_UP). Bunza steps out, the party and Hammet gather
/// behind the leader, Garet and Mia think they saw someone, and Ivan asks
/// whether Dodonpa's henchmen have found them (a yes: Garet will fight; a no:
/// Garet wonders what else it could be). Hammet calls out and names Bunza,
/// Bunza knows Hammet's voice, and the reunion and the wagon talk follow. Sets
/// FLAG_LUNPA_CAVE_REUNION_SEEN, so it plays once.
static void WestLunpaCave_HammetFindsBunza(void) {
  struct Actor *leader;
  s32 sighting;
  s32 recognition;

  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    // Bring Hammet and the party into view as Bunza leaves his hiding place.
    GameFlag_Set(FLAG_LUNPA_CAVE_REUNION_SEEN);
    Event_Start();
    Actor_SetPosition(ACTOR_BUNZA, PIXELS(144), PIXELS(400));
    Actor_SetSpeed(ACTOR_BUNZA, PIXELS(3) / 2, PIXELS(3) / 4);
    Actor_WalkTo(ACTOR_BUNZA, 184, 400);
    Actor_WaitForMoveById(ACTOR_BUNZA);
    Import_ActorSetAnimation(ACTOR_BUNZA, ANIM_STAND);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Event_WaitFrames(30);
    FieldCamera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
    FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(432), 1);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetPosition(ACTOR_HAMMET, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(ACTOR_HAMMET, PIXELS(13) / 10, PIXELS(13) / 20);
    Actor_WalkTo(ACTOR_HAMMET, 168, 464);
    Actor_WaitForMoveById(ACTOR_HAMMET);
    Actor_FaceDirection(ACTOR_HAMMET, FACING_NORTH, 0);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetPosition(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(ACTOR_IVAN, PIXELS(13) / 10, PIXELS(13) / 20);
    Actor_WalkTo(ACTOR_IVAN, 152, 488);
    Actor_WaitForMoveById(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetPosition(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(ACTOR_MIA, PIXELS(13) / 10, PIXELS(13) / 20);
    Actor_WalkTo(ACTOR_MIA, 168, 488);
    Actor_WaitForMoveById(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetPosition(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(ACTOR_GARET, PIXELS(13) / 10, PIXELS(13) / 20);
    Actor_WalkTo(ACTOR_GARET, 184, 488);
    Actor_WaitForMoveById(ACTOR_GARET);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    Event_WaitFrames(30);

    // The party spots someone ahead, before Bunza recognizes Hammet's voice.
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    sighting = MsgLunpaThinkSawSomeone;
    Event_SetMessageId(sighting + SIGHTING_GARET_SAW_SOMEONE);
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_WaitFrames(10);
    Event_SetMessageId(sighting + SIGHTING_MIA_SAW_SOMETHING);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Event_SetMessageId(sighting + SIGHTING_IVAN_ASKS_IF_FOUND);
    Event_OpenMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    Event_WaitFrames(30);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_SetMessageId(sighting + SIGHTING_GARET_WILL_FIGHT);
      Event_ShowMessage(ACTOR_GARET, 0);
    } else {
      Event_SetMessageId(sighting + SIGHTING_GARET_ASKS_WHAT_ELSE);
      Event_ShowMessage(ACTOR_GARET, 0);
    }

    Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
    recognition = MsgLunpaMoment;
    Event_SetMessageId(recognition + RECOGNITION_HAMMET_CALLS_OUT);
    Event_ShowMessage(ACTOR_HAMMET, 0);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GARET, 2);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Import_ActorSetAnimation(ACTOR_HAMMET, ANIM_WALK);
    Actor_WalkBy(ACTOR_HAMMET, 0, -16);
    Actor_WaitForMoveById(ACTOR_HAMMET);
    Import_ActorSetAnimation(ACTOR_HAMMET, ANIM_STAND);
    Event_SetMessageId(recognition + RECOGNITION_HAMMET_NAMES_BUNZA);
    Event_ShowMessage(ACTOR_HAMMET, 0);
    Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 65);
    Event_SetMessageId(recognition + RECOGNITION_BUNZA_KNOWS_VOICE);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Event_WaitFrames(80);
    Actor_SetSpeed(ACTOR_BUNZA, PIXELS(2) / 5, PIXELS(1) / 5);
    Actor_WalkBy(ACTOR_BUNZA, -13, 0);
    Actor_WaitForMoveById(ACTOR_BUNZA);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH, 0);
    Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
    Event_SetMessageId(recognition + RECOGNITION_BUNZA_NAMES_HAMMET);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Actor_WalkTo(ACTOR_BUNZA, 168, 432);
    Event_WaitFrames(40);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Event_WaitFrames(20);
    Event_End();
    Reunion_Converse();
  }
}

/// The reunion, with the screen closed and reopened on the group: Hammet and
/// Ivan tell how the party came to his rescue, Bunza tells how he came to
/// Lunpa and found the cave, warns that Dodonpa's henchmen are combing the
/// area, and offers his wagon as a way out of Lunpa.
static void Reunion_Converse(void) {
  s32 reunion;
  s32 messageBase;

  // Arrange the reunion while the screen is closed, then reveal the group.
  Event_Start();
  Actor_SetPosition(ACTOR_GARET, PIXELS(200), PIXELS(544));
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(184), PIXELS(544));
  Actor_SetPosition(ACTOR_MIA, PIXELS(168), PIXELS(544));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(212), PIXELS(528));
  Actor_SetPosition(ACTOR_HAMMET, PIXELS(200), PIXELS(512));
  Actor_SetPosition(ACTOR_BUNZA, PIXELS(168), PIXELS(512));
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_EAST, 0);
  Import_ActorSetAnimation(ACTOR_BUNZA, ANIM_STAND);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  WaitFrames(1);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(20);

  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  reunion = MsgLunpaDodonpaNeverIntention;
  Event_SetMessageId(reunion + REUNION_BUNZA_ASKS_ABOUT_RELEASE);
  Event_ShowMessage(ACTOR_BUNZA, 0);
  Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Event_SetMessageId(reunion + REUNION_HAMMET_CREDITS_IVAN);
  Event_ShowMessage(ACTOR_HAMMET, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_HAMMET, 0);
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_WaitFrames(40);
  Event_SetMessageId(reunion + REUNION_IVAN_CREDITS_LEADER);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 80);
  Event_SetMessageId(reunion + REUNION_GARET_QUESTIONS_IVAN);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_GARET, 0);
  Event_SetMessageId(reunion + REUNION_IVAN_CREDITS_EVERYONE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH - FACING_STEP, 0);
  Event_WaitFrames(60);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitFrames(60);
  Event_SetMessageId(reunion + REUNION_HAMMET_THANKS_PARTY);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
  Event_WaitFrames(30);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(120);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH - FACING_STEP, 0);
  Event_WaitFrames(30);
  Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_SMILE, 80);
  Event_SetMessageId(reunion + REUNION_HAMMET_THANKS_IVAN);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_HAMMET, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(30);

  // Bunza explains how he reached the cave despite being refused entry.
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceActor(ACTOR_MIA, ACTOR_BUNZA, 0);
  Event_SetMessageId(reunion + REUNION_MIA_PRAISES_BUNZA);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
  Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
  Event_SetMessageId(reunion + REUNION_HAMMET_PRAISES_BUNZA);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  Event_SetMessageId(reunion + REUNION_BUNZA_CALLS_IT_CHANCE);
  Event_ShowMessage(ACTOR_BUNZA, 0);
  Actor_RunRepeatedMotion(ACTOR_BUNZA, 1);
  Event_WaitFrames(20);
  Event_SetMessageId(reunion + REUNION_BUNZA_AVOIDED_LUNPA);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_LinkPair(ACTOR_HAMMET, ACTOR_IVAN, 0);
  Event_WaitFrames(60);
  Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(30);
  Event_SetMessageId(reunion + REUNION_BUNZA_KNEW_OF_PRISON);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_WaitFrames(20);
  Actor_FaceActor(ACTOR_GARET, ACTOR_BUNZA, 0);
  Event_SetMessageId(reunion + REUNION_GARET_ASKS_WHY_BUNZA_CAME);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_FaceActor(ACTOR_BUNZA, ACTOR_GARET, 0);
  Event_WaitFrames(20);
  Event_SetMessageId(reunion + REUNION_BUNZA_RECALLS_ADVICE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_BUNZA, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 70);
  Event_SetMessageId(reunion + REUNION_MIA_ASKS_ABOUT_TRADE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Event_WaitFrames(40);
  Event_SetMessageId(reunion + REUNION_BUNZA_MEANS_WISDOM);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
  Event_SetMessageId(reunion + REUNION_HAMMET_ON_APPEARANCES);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
  Actor_FaceActor(ACTOR_BUNZA, ACTOR_HAMMET, 0);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Event_WaitFrames(30);
  Event_SetMessageId(reunion + REUNION_BUNZA_ON_UNPLEASANT_PLACES);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Event_WaitFrames(30);
  Event_SetMessageId(reunion + REUNION_HAMMET_ON_SELLING);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_SMILE, 70);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Event_WaitFrames(30);
  Event_SetMessageId(reunion + REUNION_IVAN_ASKS_ABOUT_SERVING);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_BUNZA, ACTOR_IVAN, 0);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Event_WaitFrames(30);
  Event_SetMessageId(reunion + REUNION_IVAN_ON_FATE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_SetMessageId(reunion + REUNION_GARET_ASKS_ABOUT_ENTRY);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Event_SetMessageId(reunion + REUNION_BUNZA_WAS_REFUSED);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_SetMessageId(reunion + REUNION_MIA_ASKS_WHY_BUNZA_STILL_CAME);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
  Actor_FaceActor(ACTOR_BUNZA, ACTOR_MIA, 0);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_SetMessageId(reunion + REUNION_BUNZA_MENTIONS_COMMOTION);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_LinkPair(ACTOR_IVAN, ACTOR_HAMMET, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_SetMessageId(reunion + REUNION_IVAN_EXPLAINS_COMMOTION);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
  Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
  Event_WaitFrames(20);
  Event_SetMessageId(reunion + REUNION_BUNZA_LINKS_COMMOTION);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_SetAttachedEffect(ACTOR_BUNZA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_SetMessageId(reunion + REUNION_BUNZA_HAD_TO_KNOW);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_BUNZA, 0);
  Event_SetMessageId(reunion + REUNION_GARET_ASKS_ABOUT_CAVE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_SetMessageId(reunion + REUNION_MIA_ON_GATE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
  Actor_RunRepeatedMotion(ACTOR_BUNZA, 1);
  Event_SetMessageId(reunion + REUNION_BUNZA_HID);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_RunRepeatedMotion(ACTOR_BUNZA, 1);
  Event_SetMessageId(reunion + REUNION_HAMMET_STARTLED_BUNZA);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
  Event_WaitFrames(30);
  Actor_FaceActor(ACTOR_BUNZA, ACTOR_HAMMET, 0);
  Event_WaitFrames(60);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Event_SetMessageId(reunion + REUNION_BUNZA_IS_GLAD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Event_WaitFrames(30);

  // Bunza checks the passage, worries about pursuit and proposes his wagon.
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH, 0);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_SetSpeed(ACTOR_BUNZA, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkTo(ACTOR_BUNZA, 144, 528);
  Actor_WaitForMoveById(ACTOR_BUNZA);
  Actor_WalkTo(ACTOR_BUNZA, 168, 560);
  Actor_WaitForMoveById(ACTOR_BUNZA);
  Event_WaitFrames(60);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Event_WaitFrames(40);
  Actor_WalkTo(ACTOR_BUNZA, 144, 528);
  Actor_WaitForMoveById(ACTOR_BUNZA);
  Actor_WalkTo(ACTOR_BUNZA, 168, 488);
  Event_SetMessageId(reunion + REUNION_IVAN_ASKS_WHY);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
  Actor_WaitForMoveById(ACTOR_BUNZA);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Import_ActorSetAnimation(ACTOR_BUNZA, ANIM_STAND);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_SetMessageId(reunion + REUNION_BUNZA_WARNS_OF_SEARCH);
  Event_ShowMessage(ACTOR_BUNZA, 0);
  Event_WaitFrames(20);
  Actor_LinkPair(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_SetMessageId(reunion + REUNION_BUNZA_FEARS_CAPTURE);
  Event_ShowMessage(ACTOR_BUNZA, 0);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_SetMessageId(reunion + REUNION_MIA_URGES_ESCAPE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_SetMessageId(reunion + REUNION_IVAN_WANTS_STEALTH);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_SetMessageId(reunion + REUNION_GARET_ASKS_TO_FIGHT);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_SetMessageId(reunion + REUNION_BUNZA_DISCOURAGES_FIGHT);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  } else {
    Event_SetMessageId(reunion + REUNION_BUNZA_AGREES_NOT_TO_FIGHT);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  }

  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_BUNZA, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_BUNZA, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_BUNZA, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
  Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
  Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  messageBase = MsgLunpaBunza;
  Event_SetMessageId(messageBase + PLAN_HAMMET_ASKS_PLAN);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  Event_SetMessageId(messageBase + PLAN_BUNZA_HAS_WAGON);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_SetMessageId(messageBase + PLAN_MIA_ASKS_ABOUT_WAGON);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
  Actor_FaceActor(ACTOR_BUNZA, ACTOR_MIA, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Event_SetMessageId(messageBase + PLAN_BUNZA_OFFERS_RIDE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
  Event_SetMessageId(messageBase + PLAN_IVAN_DOUBTS_WAGON);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Event_SetMessageId(messageBase + PLAN_BUNZA_REASSURES);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Event_WaitFrames(60);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_BUNZA, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_BUNZA, 0);
  Event_SetMessageId(messageBase + PLAN_GARET_AGREES);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_FaceActor(ACTOR_BUNZA, ACTOR_GARET, 0);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Event_WaitFrames(20);
  Event_SetMessageId(messageBase + PLAN_BUNZA_IS_UNSUSPECTED);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_LinkPair(ACTOR_HAMMET, ACTOR_IVAN, 0);
  Event_WaitFrames(60);
  Import_ActorSetAnimation(ACTOR_HAMMET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(60);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_FaceActor(ACTOR_BUNZA, ACTOR_PARTY_LEADER, 0);
  Import_ActorSetAnimation(ACTOR_BUNZA, ANIM_NOD);
  Event_WaitFrames(60);
  WagonChoice_Run();
}

/// Reconciles the party's answers until they either ride or stay behind.
/// Garet asks whether to ride: a yes brings Bunza's question whether the party
/// has unfinished business (a no rides), a no brings Garet's and Mia's
/// questions whether it really will not ride. Answers that point to staying
/// meet Garet's check that nothing is left to do (a yes rides) and Bunza's
/// apology that he cannot wait, after which Mia asks whether the party will
/// stay; a no to Mia's "You do want to go with them?" leaves Ivan confused and
/// Garet asking again.
///
/// The gotos are the conversation's own shape, not a forced layout: two of
/// them enter the middle of the other branch (a no to riding joins the
/// nothing-left check, and Mia's question joins Bunza's), which structured
/// C could only write by repeating those questions' calls.
static void WagonChoice_Run(void) {
  s32 wagon;
  s32 insisted;
  s32 confusion;

  wagon = MsgLunpaLetsTakeWagon;
  Event_SetMessageId(wagon + WAGON_BUNZA_LEADS_THE_WAY);
  Event_ShowMessageAndWait(ACTOR_BUNZA, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_SetMessageId(wagon + WAGON_GARET_ASKS_TO_RIDE);
  FieldMessage_ShowAtActor(ACTOR_GARET, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Actor_SetAngleToward(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
  Actor_SetAngleToward(ACTOR_HAMMET, ACTOR_PARTY_LEADER, 0);
  Actor_SetAngleToward(ACTOR_BUNZA, ACTOR_PARTY_LEADER, 0);

ask_to_ride:
  if (WestLunpaCave_LeaderAnswersYes()) {
  ask_about_business:
    if (!WestLunpaCave_BunzaAsksAboutUnfinishedBusiness()) {
      goto ride;
    }
    insisted = FALSE;
    if (!WestLunpaCave_GaretAsksAboutThingsToDo()) {
    insist_nothing_left:
      insisted = TRUE;
    check_nothing_left:
      WestLunpaCave_GaretChecksNothingLeftToDo();
      if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
        goto ride;
      }
    }
    if (WestLunpaCave_BunzaCannotWait() || !insisted) {
      goto stay;
    }
    goto check_nothing_left;
  } else {
    if (WestLunpaCave_GaretAsksIfNotRiding()) {
      if (WestLunpaCave_GaretAsksAboutUnfinishedBusiness()) {
        goto stay;
      }
      goto insist_nothing_left;
    }
    if (WestLunpaCave_MiaAsksIfRidingAfterAll()) {
      goto ask_about_business;
    }
    confusion = MsgLunpaTotallyConfusedChanged;
    Event_SetMessageId(confusion + CONFUSION_IVAN_IS_CONFUSED);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0);
    Event_SetMessageId(confusion + CONFUSION_GARET_ASKS_AGAIN);
    FieldMessage_ShowAtActor(ACTOR_GARET, 0);
    goto ask_to_ride;
  ride:
    Party_RidesWagon();
    return;
  }
stay:
  Party_ConfirmsStaying();
  Party_StaysBehind();
}

/// The yes-or-no menu answers 0 for Yes; this turns that into true or false.
static u8 WestLunpaCave_LeaderAnswersYes(void) {
  return Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0;
}

/// Asks whether the party really intends to refuse the ride.
static u8 WestLunpaCave_GaretAsksIfNotRiding(void) {
  Event_SetMessageId(MsgLunpaGaretAsksIfNotRiding);
  FieldMessage_ShowAtActor(ACTOR_GARET, 0);
  return Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0;
}

/// Asks whether unfinished business explains the refusal to ride.
static u8 WestLunpaCave_GaretAsksAboutUnfinishedBusiness(void) {
  Event_SetMessageId(MsgLunpaGaretAsksAboutUnfinishedBusiness);
  FieldMessage_ShowAtActor(ACTOR_GARET, 0);
  return Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0;
}

/// Reserved confirmation step; it always succeeds and displays no message.
static u8 Party_ConfirmsStaying(void) { return TRUE; }

/// Lets Bunza check whether the party has unfinished business before leaving.
static u8 WestLunpaCave_BunzaAsksAboutUnfinishedBusiness(void) {
  Event_SetMessageId(MsgLunpaBunzaAsksAboutUnfinishedBusiness);
  FieldMessage_ShowAtActor(ACTOR_BUNZA, 0);
  return Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0;
}

/// Opens Garet's confirmation prompt; WagonChoice_Run collects the answer.
static u8 WestLunpaCave_GaretChecksNothingLeftToDo(void) {
  Event_SetMessageId(MsgLunpaGaretChecksNothingLeft);
  FieldMessage_ShowAtActor(ACTOR_GARET, 0);
  return TRUE;
}

/// Asks whether the party has changed its mind about taking the wagon.
static u8 WestLunpaCave_MiaAsksIfRidingAfterAll(void) {
  Event_SetMessageId(MsgLunpaMiaAsksIfRidingAfter);
  FieldMessage_ShowAtActor(ACTOR_MIA, 0);
  return Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0;
}

/// Explains that Bunza cannot wait and asks again about the party's business.
static u8 WestLunpaCave_BunzaCannotWait(void) {
  s32 warning;

  Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  warning = MsgLunpaMeaningWontRide;
  Event_SetMessageId(warning + WARNING_IVAN_ASKS_IF_STAYING);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Event_SetMessageId(warning + WARNING_BUNZA_CANNOT_WAIT);
  Event_ShowMessage(ACTOR_BUNZA, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_SetMessageId(warning + WARNING_MIA_ASKS_ABOUT_BUSINESS);
  Event_OpenMessage(ACTOR_MIA, 0);
  return Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0;
}

/// Asks the leader to confirm that something still needs doing here.
static u8 WestLunpaCave_GaretAsksAboutThingsToDo(void) {
  Event_SetMessageId(MsgLunpaGaretAsksAboutThingsTo);
  FieldMessage_ShowAtActor(ACTOR_GARET, 0);
  return Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0;
}

/// Sends Hammet and Bunza ahead, then returns the companions to the leader.
static void Party_StaysBehind(void) {
  struct Actor *leader;
  s32 farewell;

  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  farewell = MsgLunpaInsistStick;
  Event_SetMessageId(farewell + FAREWELL_GARET_STAYS);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_SetMessageId(farewell + FAREWELL_MIA_STAYS);
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_HAMMET, 0);
  Event_WaitFrames(60);
  Event_SetMessageId(farewell + FAREWELL_IVAN_STAYS);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_HAMMET, ACTOR_IVAN, 0);
  Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  Event_SetMessageId(farewell + FAREWELL_HAMMET_LETS_IVAN_GO);
  Event_ShowMessage(ACTOR_HAMMET, 0);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Event_SetMessageId(farewell + FAREWELL_BUNZA_SAYS_GOODBYE);
  Event_ShowMessage(ACTOR_BUNZA, 0);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(60);
  Actor_SetDestinationOffset(ACTOR_HAMMET, -16, 0);
  Actor_WaitForMoveById(ACTOR_HAMMET);
  Import_ActorSetAnimation(ACTOR_HAMMET, ANIM_STAND);
  Event_WaitFrames(40);
  Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitFrames(30);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_WalkTo(ACTOR_BUNZA, 156, 528);
  Event_WaitFrames(20);
  Actor_WalkTo(ACTOR_HAMMET, 164, 528);
  Actor_WaitForMoveById(ACTOR_BUNZA);
  Actor_WalkTo(ACTOR_BUNZA, 168, 640);
  Actor_WaitForMoveById(ACTOR_HAMMET);
  Actor_WalkTo(ACTOR_HAMMET, 168, 640);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Event_WaitFrames(60);
  Actor_SetPosition(ACTOR_HAMMET, 0, 0);
  Actor_SetPosition(ACTOR_BUNZA, 0, 0);
  Event_WaitFrames(110);

  Event_SetMessageId(farewell + FAREWELL_GARET_SIGHS);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(30);
  Event_SetMessageId(farewell + FAREWELL_MIA_HOPES_FOR_SAFETY);
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_SetMessageId(farewell + FAREWELL_IVAN_REASSURES);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_WaitFrames(140);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_SetMessageId(farewell + FAREWELL_GARET_MOVES_ON);
  Event_ShowMessage(ACTOR_GARET, 0);

  // Merge the companions back into the leader and restore player camera
  // control.
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_IVAN, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Event_WaitFrames(30);
  Actor_PlaceCurrentWithinCameraBounds(ACTOR_PARTY_LEADER, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  GameFlag_Set(FLAG_BUNZAS_WAGON_LEFT);
}

/// Leads everyone down the passage and requests the wagon departure exit.
static void Party_RidesWagon(void) {
  struct Actor *leader;
  s32 departure;

  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 0);
  departure = MsgLunpaDontUnfinishedBusiness;
  Event_SetMessageId(departure + DEPARTURE_GARET_HEADS_FOR_KALAY);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_SetMessageId(departure + DEPARTURE_IVAN_THINKS_OF_LAYANA);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_HAMMET, ACTOR_IVAN, 0);
  Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Event_WaitFrames(20);
  Event_SetMessageId(departure + DEPARTURE_HAMMET_LONGS_FOR_LAYANA);
  Event_ShowMessage(ACTOR_HAMMET, 0);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Event_WaitFrames(30);
  Event_SetMessageId(departure + DEPARTURE_BUNZA_SETS_OFF);
  Event_ShowMessage(ACTOR_BUNZA, 0);
  Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Event_WaitFrames(20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(80);
  Actor_SetDestinationOffset(ACTOR_HAMMET, -16, 0);
  Actor_WaitForMoveById(ACTOR_HAMMET);
  Import_ActorSetAnimation(ACTOR_HAMMET, ANIM_STAND);
  Event_WaitFrames(40);
  Actor_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitFrames(30);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_WalkTo(ACTOR_BUNZA, 152, 528);
  Event_WaitFrames(20);
  Actor_WalkTo(ACTOR_HAMMET, 160, 528);
  Actor_WaitForMoveById(ACTOR_BUNZA);
  Actor_WalkTo(ACTOR_BUNZA, 168, 640);
  Actor_WaitForMoveById(ACTOR_HAMMET);
  Actor_WalkTo(ACTOR_HAMMET, 168, 640);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Event_WaitFrames(200);
  Actor_SetPosition(ACTOR_HAMMET, 0, 0);
  Actor_SetPosition(ACTOR_BUNZA, 0, 0);

  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_IVAN, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Event_WaitFrames(30);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, -16, 0);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_PlaceCurrentWithinCameraBounds(ACTOR_PARTY_LEADER, 1);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 168, 640);
  Event_WaitFrames(60);
  Event_FinishScreenTransition();
  Import_EventSetExitRequest(WEST_LUNPA_CAVE_BY_WAGON);
}

/// Restores the gate and frozen puddles from the persistent puzzle flags.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gAdventure.sceneId == (s32)&SceneId_WestLunpaCave) {
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_UNUSED_PUDDLE), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_SOUTH_PUDDLE), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_GATE_PUDDLE), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_NORTH_PUDDLE), 0);
    Actor_GetById(ACTOR_NORTH_PUDDLE)->scaleY = 19 * Q16_ONE / 20;
    if (GameFlag_Get(FLAG_WEST_LUNPA_CAVE_GATE_LOWERED) != 0) {
      WestLunpaCave_GateLower();
    }
    if (GameFlag_Get(FLAG_WEST_LUNPA_CAVE_GATE_RAISED) != 0) {
      WestLunpaCave_GateRaise();
    }
    if (GameFlag_Get(FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN) != 0) {
      WestLunpaCave_GateDrawPropped();
    }
    if (GameFlag_Get(FLAG_WEST_LUNPA_CAVE_NORTH_PUDDLE_FROZEN) != 0) {
      Import_ActorSetAnimation(ACTOR_NORTH_PUDDLE, PUDDLE_ANIMATION_FROZEN);
    }
    if (GameFlag_Get(FLAG_WEST_LUNPA_CAVE_SOUTH_PUDDLE_FROZEN) != 0) {
      Import_ActorSetAnimation(ACTOR_SOUTH_PUDDLE, PUDDLE_ANIMATION_FROZEN);
    }
  }
  return 0;
}

// clang-format off
const struct SceneEntrance gCaveEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { WEST_LUNPA_CAVE_FROM_WORLD_MAP, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, 88, 56, 648, 648, 0 },
    { WEST_LUNPA_CAVE_FROM_LUNPA, CONDITION_ALWAYS, 552, 0, 328, FACING_NORTH, 0, 88, 56, 648, 648, 0 },
    { WEST_LUNPA_CAVE_FROM_LUNPA_UNUSED, CONDITION_ALWAYS, 552, 0, 328, FACING_NORTH, 0, 88, 56, 648, 488, 0 },
    { SCENE_TABLE_END },
};

const u32 gCaveExits[] = {
    SCENE_EXITS(WestLunpaCave),
    SCENE_EXIT(WEST_LUNPA_CAVE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_WEST_LUNPA_CAVE),
    SCENE_EXIT(WEST_LUNPA_CAVE_TO_LUNPA, Lunpa, LUNPA_FROM_WEST_LUNPA_CAVE),
    SCENE_EXIT(WEST_LUNPA_CAVE_BY_WAGON, WorldMap, WORLD_MAP_FROM_WEST_LUNPA_CAVE),
    SCENE_EXITS_END,
};

const struct ActorTemplate gCaveNoActorTemplates[] = {
    { SCENE_TABLE_END },
};

const struct ActorTemplate gCaveActorTemplates[] = {
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(136), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(344), 0, PIXELS(248), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(424), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BUNZA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

/// Unused: an empty trigger table nothing returns.
const struct Trigger gCaveNoTriggers[] = {
    { TRIGGERS_END },
};

const struct Trigger gCaveTriggers[] = {
    { TRIGGER_EXIT, MB_WEST_LUNPA_CAVE_WORLD_MAP_PASSAGE, CONDITION_ALWAYS, WEST_LUNPA_CAVE_TO_WORLD_MAP },
    { TRIGGER_EXIT, MB_WEST_LUNPA_CAVE_LUNPA_PASSAGE, CONDITION_ALWAYS, WEST_LUNPA_CAVE_TO_LUNPA },
    { TRIGGER_STEP, MB_WEST_LUNPA_CAVE_BUNZA_HIDING_PLACE, FLAG_LUNPA_CAVE_REUNION_SEEN,
      EVENT(WestLunpaCave_HammetFindsBunza) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), MB_WEST_LUNPA_CAVE_LOWERING_SWITCH, CONDITION_ALWAYS,
      EVENT(WestLunpaCave_FlipLoweringSwitch) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), MB_WEST_LUNPA_CAVE_RAISING_SWITCH, CONDITION_ALWAYS,
      EVENT(WestLunpaCave_FlipRaisingSwitch) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_UNUSED_PUDDLE, CONDITION_ALWAYS,
      EVENT(WestLunpaCave_UnusedFreezePuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SOUTH_PUDDLE, CONDITION_ALWAYS,
      EVENT(WestLunpaCave_FreezeSouthPuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_GATE_PUDDLE, CONDITION_ALWAYS,
      EVENT(WestLunpaCave_FreezeGatePuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_NORTH_PUDDLE, CONDITION_ALWAYS,
      EVENT(WestLunpaCave_FreezeNorthPuddle) },
    { TRIGGERS_END },
};
// clang-format on

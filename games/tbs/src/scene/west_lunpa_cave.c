/// @file
/// West Lunpa Cave (SceneId_WestLunpaCave, area name West Lunpa Cave), the cave
/// between Lunpa and the world map, with a gate across it, where the party
/// meets Bunza on the way out of Lunpa with Hammet.
///
/// The party comes in from the world map (WEST_LUNPA_CAVE_FROM_WORLD_MAP) and
/// from Lunpa (WEST_LUNPA_CAVE_FROM_LUNPA);
/// MB_WEST_LUNPA_CAVE_WORLD_MAP_PASSAGE and MB_WEST_LUNPA_CAVE_LUNPA_PASSAGE
/// lead back out. WEST_LUNPA_CAVE_FROM_LUNPA_UNUSED, which no warp names,
/// repeats WEST_LUNPA_CAVE_FROM_LUNPA with a shorter camera.
///
/// The gate puzzle runs on map flags, which Scene_Initialize redraws: the
/// lowering switch brings the gate down and the raising switch lifts it, each
/// doing nothing while the gate already stands where it would put it or while
/// the gate's puddle, frozen by Frost into a pillar, props it open. Freezing
/// that puddle with the gate down raises it onto the pillar with the
/// puzzle-solved tune. Frost also freezes a puddle to the south into a pillar
/// that can be climbed, and one to the north. Its map flags:
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
/// comes out from behind a rock (EventScript_HammetFindsBunza), they talk
/// (Reunion_Converse), and the party's answers decide whether it rides home in
/// Bunza's wagon (WagonChoice_Run). Riding leaves by WEST_LUNPA_CAVE_BY_WAGON
/// to WORLD_MAP_FROM_WEST_LUNPA_CAVE, where world_map.c plays the departure;
/// staying, Hammet and Bunza leave alone. Either way FLAG_BUNZAS_WAGON_LEFT is
/// set, on the world map or here.
///
/// Unused: a fourth puddle stands at the map's origin, outside the cave's
/// camera bounds, so Frost can never reach it (EventScript_UnusedFreezePuddle).

#include "save_block.h"
#include "scene_ids.h"
#include "script_layer.h"
#include "unit/scene/west_lunpa_cave.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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

// The cave's map flags, the state of its gate puzzle:
// FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN: the gate puddle is frozen, propping
// the gate open. FLAG_WEST_LUNPA_CAVE_GATE_LOWERED: the lowering switch has
// lowered the gate. FLAG_WEST_LUNPA_CAVE_GATE_RAISED: the gate is raised.
// FLAG_WEST_LUNPA_CAVE_NORTH_PUDDLE_FROZEN: the north puddle is frozen into a
// pillar. FLAG_WEST_LUNPA_CAVE_SOUTH_PUDDLE_FROZEN: the south puddle is frozen
// into a pillar.

const struct MapWarpDest *Scene_GetWarpDests(void) { return gCaveWarpDests; }

const struct MapWarpRegion *Scene_GetWarpRegions(void) { return NULL; }

/// Both passages and the wagon departure return through this warp table.
const u32 *Scene_GetWarps(void) { return gCaveWarps; }

/// Only the cave map places puddles, Hammet, Bunza and the party members.
const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_WestLunpaCave) {
    return gCaveObjectEventTemplates;
  }
  return gCaveNoObjectEventTemplates;
}

/// Routes switches, Frost targets and the reunion coord event to their
/// handlers.
const struct MapEvent *Scene_GetEvents(void) { return gCaveEvents; }

/// Unused: Frost can never target the puddle placed at (0, 0, 0), outside the
/// cave's camera bounds. It would freeze, and no flag would remember it.
static void EventScript_UnusedFreezePuddle(void) {
  struct ObjectEvent *puddle;

  puddle = ObjectEvent_GetById(LOCALID_UNUSED_PUDDLE);
  if (puddle != NULL) {
    ObjectEvent_SetSpriteFlags(puddle, 0);
  }
}

/// In West Lunpa Cave, Frost on the south puddle: it rises as an ice pillar the
/// party can climb.
static void EventScript_FreezeSouthPuddle(void) {
  struct ObjectEvent *puddle;

  puddle = ObjectEvent_Get(LOCALID_SOUTH_PUDDLE);
  if (puddle != NULL) {
    puddle->priorityFlags = OBJ_EVENT_PRIORITY_AUTOMATIC;
    puddle->motionFlags = 0;
  }
  MapGrid_CopyCellAttributes(7, 32, 1, 1, 8, 32);
  GameFlag_Set(FLAG_WEST_LUNPA_CAVE_SOUTH_PUDDLE_FROZEN);
}

/// Animates the raised gate resting on the frozen pillar, then opens the way
/// through its metatile behaviors.
static void WestLunpaCave_GateDrawPropped(void) {
  struct ObjectEvent *pillar;

  pillar = ObjectEvent_Get(LOCALID_GATE_PUDDLE);
  Import_ObjectEventSetAnimation(LOCALID_GATE_PUDDLE, PUDDLE_ANIMATION_FROZEN);
  if (pillar != NULL) {
    ObjectEvent_SetSpriteFlags(pillar, 0);
    pillar->priorityFlags = OBJ_EVENT_PRIORITY_AUTOMATIC;
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
static void EventScript_FreezeGatePuddle(void) {
  struct ObjectEvent *pillar;

  pillar = ObjectEvent_Get(LOCALID_GATE_PUDDLE);
  GameFlag_Set(FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN);
  if (pillar != NULL) {
    ObjectEvent_SetSpriteFlags(pillar, 0);
    pillar->priorityFlags = OBJ_EVENT_PRIORITY_AUTOMATIC;
  }
  if (Script_GetFlag(FLAG_WEST_LUNPA_CAVE_GATE_RAISED) == 0) {
    Script_PlaySong(SE_GATE_MOVE);
    WestLunpaCave_GateDrawPropped();
    Script_PlaySong(MUS_PUZZLE_SOLVED);
    GameFlag_Set(FLAG_WEST_LUNPA_CAVE_GATE_RAISED);
  }
}

/// In West Lunpa Cave, Frost on the north puddle: its pillar stays frozen when
/// the party returns.
static void EventScript_FreezeNorthPuddle(void) {
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
  ObjectMotion_SetActionVariant(LOCALID_GATE_PUDDLE, GATE_PILLAR_PRIORITY);
  MapGrid_CopyCellAttributes(19, 17, 1, 1, 22, 15);
}

/// In West Lunpa Cave, searching the lowering switch: the leader flips it and
/// the gate comes down, unless it is down already or a frozen pillar props it
/// open. "flipped the switch."
static void EventScript_FlipLoweringSwitch(void) {
  if (Script_GetFlag(FLAG_WEST_LUNPA_CAVE_GATE_LOWERED) == 0) {
    if (GameFlag_Get(FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN) == 0) {
      FieldMessage_Show(MsgFieldFlippedSwitch, 1);
      Script_PlaySong(SE_GATE_MOVE);
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
static void EventScript_FlipRaisingSwitch(void) {
  if (GameFlag_Get(FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN) == 0) {
    if (Script_GetFlag(FLAG_WEST_LUNPA_CAVE_GATE_RAISED) == 0) {
      FieldMessage_Show(MsgFieldFlippedSwitch, 1);
      Script_PlaySong(SE_GATE_MOVE);
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
static void EventScript_HammetFindsBunza(void) {
  struct ObjectEvent *leader;
  s32 sighting;
  s32 recognition;

  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    // Bring Hammet and the party into view as Bunza leaves his hiding place.
    GameFlag_Set(FLAG_LUNPA_CAVE_REUNION_SEEN);
    FieldScript_Start();
    ObjectEvent_SetPosition(LOCALID_BUNZA, PIXELS(144), PIXELS(400));
    ObjectEvent_SetSpeed(LOCALID_BUNZA, PIXELS(3) / 2, PIXELS(3) / 4);
    ObjectEvent_WalkTo(LOCALID_BUNZA, 184, 400);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_BUNZA);
    Import_ObjectEventSetAnimation(LOCALID_BUNZA, ANIM_STAND);
    ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 1);
    FieldScript_WaitFrames(30);
    FieldCamera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
    FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(432), 1);
    leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    if (leader != NULL) {
      ObjectEvent_SetPosition(LOCALID_HAMMET, leader->x.fixed, leader->z.fixed);
    }
    ObjectEvent_SetSpeed(LOCALID_HAMMET, PIXELS(13) / 10, PIXELS(13) / 20);
    ObjectEvent_WalkTo(LOCALID_HAMMET, 168, 464);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_HAMMET);
    ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_NORTH, 0);
    leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    if (leader != NULL) {
      ObjectEvent_SetPosition(LOCALID_IVAN, leader->x.fixed, leader->z.fixed);
    }
    ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(13) / 10, PIXELS(13) / 20);
    ObjectEvent_WalkTo(LOCALID_IVAN, 152, 488);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IVAN);
    ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
    leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    if (leader != NULL) {
      ObjectEvent_SetPosition(LOCALID_MIA, leader->x.fixed, leader->z.fixed);
    }
    ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(13) / 10, PIXELS(13) / 20);
    ObjectEvent_WalkTo(LOCALID_MIA, 168, 488);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_MIA);
    ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
    leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    if (leader != NULL) {
      ObjectEvent_SetPosition(LOCALID_GARET, leader->x.fixed, leader->z.fixed);
    }
    ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(13) / 10, PIXELS(13) / 20);
    ObjectEvent_WalkTo(LOCALID_GARET, 184, 488);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
    FieldScript_WaitFrames(30);

    // The party spots someone ahead, before Bunza recognizes Hammet's voice.
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
    sighting = MsgLunpaThinkSawSomeone;
    FieldScript_SetMessageId(sighting + SIGHTING_GARET_SAW_SOMEONE);
    Script_ShowMessage(LOCALID_GARET, 0);
    FieldScript_WaitFrames(30);
    ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
    FieldScript_WaitFrames(10);
    FieldScript_SetMessageId(sighting + SIGHTING_MIA_SAW_SOMETHING);
    Script_ShowMessage(LOCALID_MIA, 0);
    ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
    ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_PARTY_LEADER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_IVAN, 0);
    FieldScript_SetMessageId(sighting + SIGHTING_IVAN_ASKS_IF_FOUND);
    Script_OpenMessage(LOCALID_IVAN, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_MIA, LOCALID_PARTY_LEADER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
    FieldScript_WaitFrames(30);
    if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
        0) {
      FieldScript_SetMessageId(sighting + SIGHTING_GARET_WILL_FIGHT);
      Script_ShowMessage(LOCALID_GARET, 0);
    } else {
      FieldScript_SetMessageId(sighting + SIGHTING_GARET_ASKS_WHAT_ELSE);
      Script_ShowMessage(LOCALID_GARET, 0);
    }

    ObjectEvent_ShowEmote(LOCALID_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          70);
    recognition = MsgLunpaMoment;
    FieldScript_SetMessageId(recognition + RECOGNITION_HAMMET_CALLS_OUT);
    Script_ShowMessage(LOCALID_HAMMET, 0);
    ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
    ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
    ObjectMotion_SetVariantCallback(LOCALID_IVAN, 2);
    ObjectMotion_SetVariantCallback(LOCALID_MIA, 2);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
    Import_ObjectEventSetAnimation(LOCALID_HAMMET, ANIM_WALK);
    ObjectEvent_WalkBy(LOCALID_HAMMET, 0, -16);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_HAMMET);
    Import_ObjectEventSetAnimation(LOCALID_HAMMET, ANIM_STAND);
    FieldScript_SetMessageId(recognition + RECOGNITION_HAMMET_NAMES_BUNZA);
    Script_ShowMessage(LOCALID_HAMMET, 0);
    ObjectEvent_ShowEmote(LOCALID_BUNZA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          65);
    FieldScript_SetMessageId(recognition + RECOGNITION_BUNZA_KNOWS_VOICE);
    Script_ShowMessage(LOCALID_BUNZA, 0);
    ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_NOD);
    FieldScript_WaitFrames(80);
    ObjectEvent_SetSpeed(LOCALID_BUNZA, PIXELS(2) / 5, PIXELS(1) / 5);
    ObjectEvent_WalkBy(LOCALID_BUNZA, -13, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_BUNZA);
    ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH, 0);
    ObjectEvent_ShowEmote(LOCALID_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
    FieldScript_SetMessageId(recognition + RECOGNITION_BUNZA_NAMES_HAMMET);
    Script_ShowMessage(LOCALID_BUNZA, 0);
    ObjectEvent_WalkTo(LOCALID_BUNZA, 168, 432);
    FieldScript_WaitFrames(40);
    FieldScript_FinishScreenTransition();
    FieldScript_WaitTransitionFrames();
    FieldScript_WaitFrames(20);
    FieldScript_End();
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
  FieldScript_Start();
  ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(200), PIXELS(544));
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(184), PIXELS(544));
  ObjectEvent_SetPosition(LOCALID_MIA, PIXELS(168), PIXELS(544));
  ObjectEvent_SetPosition(LOCALID_IVAN, PIXELS(212), PIXELS(528));
  ObjectEvent_SetPosition(LOCALID_HAMMET, PIXELS(200), PIXELS(512));
  ObjectEvent_SetPosition(LOCALID_BUNZA, PIXELS(168), PIXELS(512));
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_EAST, 0);
  Import_ObjectEventSetAnimation(LOCALID_BUNZA, ANIM_STAND);
  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  WaitFrames(1);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);

  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  reunion = MsgLunpaDodonpaNeverIntention;
  FieldScript_SetMessageId(reunion + REUNION_BUNZA_ASKS_ABOUT_RELEASE);
  Script_ShowMessage(LOCALID_BUNZA, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_NOD);
  FieldScript_SetMessageId(reunion + REUNION_HAMMET_CREDITS_IVAN);
  Script_ShowMessage(LOCALID_HAMMET, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_HAMMET, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 1);
  FieldScript_WaitFrames(40);
  FieldScript_SetMessageId(reunion + REUNION_IVAN_CREDITS_LEADER);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 80);
  FieldScript_SetMessageId(reunion + REUNION_GARET_QUESTIONS_IVAN);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_GARET, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_GARET, 0);
  FieldScript_SetMessageId(reunion + REUNION_IVAN_CREDITS_EVERYONE);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_IVAN, 0);
  ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_SOUTH - FACING_STEP, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(60);
  FieldScript_SetMessageId(reunion + REUNION_HAMMET_THANKS_PARTY);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_HAMMET, 0);
  FieldScript_WaitFrames(30);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(120);
  ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_SOUTH - FACING_STEP, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_HAMMET, EMOTE_IN_FRONT | EMOTE_SMILE, 80);
  FieldScript_SetMessageId(reunion + REUNION_HAMMET_THANKS_IVAN);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_HAMMET, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_HAMMET, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);

  // Bunza explains how he reached the cave despite being refused entry.
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  ObjectEvent_FaceObjectEvent(LOCALID_MIA, LOCALID_BUNZA, 0);
  FieldScript_SetMessageId(reunion + REUNION_MIA_PRAISES_BUNZA);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_MIA, 0);
  ObjectEvent_ShowEmote(LOCALID_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  ObjectEvent_FaceObjectEvent(LOCALID_HAMMET, LOCALID_BUNZA, 0);
  FieldScript_SetMessageId(reunion + REUNION_HAMMET_PRAISES_BUNZA);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_HAMMET, 0);
  ObjectEvent_ShowEmote(LOCALID_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  FieldScript_SetMessageId(reunion + REUNION_BUNZA_CALLS_IT_CHANCE);
  Script_ShowMessage(LOCALID_BUNZA, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_BUNZA, 1);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(reunion + REUNION_BUNZA_AVOIDED_LUNPA);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  ObjectEvent_LinkPair(LOCALID_HAMMET, LOCALID_IVAN, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceObjectEvent(LOCALID_HAMMET, LOCALID_BUNZA, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_BUNZA, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(30);
  FieldScript_SetMessageId(reunion + REUNION_BUNZA_KNEW_OF_PRISON);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_BUNZA, 0);
  FieldScript_SetMessageId(reunion + REUNION_GARET_ASKS_WHY_BUNZA_CAME);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_GARET, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_BUNZA, LOCALID_GARET, 0);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(reunion + REUNION_BUNZA_RECALLS_ADVICE);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_MIA, LOCALID_BUNZA, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 70);
  FieldScript_SetMessageId(reunion + REUNION_MIA_ASKS_ABOUT_TRADE);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_MIA, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_NOD);
  FieldScript_WaitFrames(40);
  FieldScript_SetMessageId(reunion + REUNION_BUNZA_MEANS_WISDOM);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  ObjectEvent_ShowEmote(LOCALID_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
  FieldScript_SetMessageId(reunion + REUNION_HAMMET_ON_APPEARANCES);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_HAMMET, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_BUNZA, LOCALID_HAMMET, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_SetMessageId(reunion + REUNION_BUNZA_ON_UNPLEASANT_PLACES);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_SetMessageId(reunion + REUNION_HAMMET_ON_SELLING);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_HAMMET, 0);
  ObjectEvent_ShowEmote(LOCALID_BUNZA, EMOTE_IN_FRONT | EMOTE_SMILE, 70);
  ObjectMotion_SetVariantCallback(LOCALID_IVAN, 1);
  FieldScript_WaitFrames(30);
  FieldScript_SetMessageId(reunion + REUNION_IVAN_ASKS_ABOUT_SERVING);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_BUNZA, LOCALID_IVAN, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_SetMessageId(reunion + REUNION_IVAN_ON_FATE);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_IVAN, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_SetMessageId(reunion + REUNION_GARET_ASKS_ABOUT_ENTRY);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_GARET, 0);
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_SHAKE_HEAD);
  FieldScript_SetMessageId(reunion + REUNION_BUNZA_WAS_REFUSED);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 1);
  FieldScript_SetMessageId(reunion + REUNION_MIA_ASKS_WHY_BUNZA_STILL_CAME);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_MIA, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_BUNZA, LOCALID_MIA, 0);
  ObjectEvent_ShowEmote(LOCALID_BUNZA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  FieldScript_SetMessageId(reunion + REUNION_BUNZA_MENTIONS_COMMOTION);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  ObjectEvent_LinkPair(LOCALID_IVAN, LOCALID_HAMMET, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_SetMessageId(reunion + REUNION_IVAN_EXPLAINS_COMMOTION);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_IVAN, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_NOD);
  ObjectEvent_FaceObjectEvent(LOCALID_HAMMET, LOCALID_BUNZA, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_BUNZA, 0);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(reunion + REUNION_BUNZA_LINKS_COMMOTION);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  ObjectEvent_SetAttachedEffect(LOCALID_BUNZA, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  FieldScript_SetMessageId(reunion + REUNION_BUNZA_HAD_TO_KNOW);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_BUNZA, 0);
  FieldScript_SetMessageId(reunion + REUNION_GARET_ASKS_ABOUT_CAVE);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_GARET, 0);
  ObjectEvent_ShowEmote(LOCALID_BUNZA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
  FieldScript_SetMessageId(reunion + REUNION_MIA_ON_GATE);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_MIA, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_BUNZA, 1);
  FieldScript_SetMessageId(reunion + REUNION_BUNZA_HID);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_BUNZA, 1);
  FieldScript_SetMessageId(reunion + REUNION_HAMMET_STARTLED_BUNZA);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_HAMMET, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceObjectEvent(LOCALID_BUNZA, LOCALID_HAMMET, 0);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_SHAKE_HEAD);
  FieldScript_SetMessageId(reunion + REUNION_BUNZA_IS_GLAD);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_NOD);
  FieldScript_WaitFrames(30);

  // Bunza checks the passage, worries about pursuit and proposes his wagon.
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH, 0);
  ObjectEvent_ShowEmote(LOCALID_BUNZA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  ObjectEvent_SetSpeed(LOCALID_BUNZA, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_WalkTo(LOCALID_BUNZA, 144, 528);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_BUNZA);
  ObjectEvent_WalkTo(LOCALID_BUNZA, 168, 560);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_BUNZA);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_WalkTo(LOCALID_BUNZA, 144, 528);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_BUNZA);
  ObjectEvent_WalkTo(LOCALID_BUNZA, 168, 488);
  FieldScript_SetMessageId(reunion + REUNION_IVAN_ASKS_WHY);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_IVAN, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_BUNZA);
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Import_ObjectEventSetAnimation(LOCALID_BUNZA, ANIM_STAND);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(reunion + REUNION_BUNZA_WARNS_OF_SEARCH);
  Script_ShowMessage(LOCALID_BUNZA, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_ShowEmote(LOCALID_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  FieldScript_SetMessageId(reunion + REUNION_BUNZA_FEARS_CAPTURE);
  Script_ShowMessage(LOCALID_BUNZA, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 1);
  FieldScript_SetMessageId(reunion + REUNION_MIA_URGES_ESCAPE);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_MIA, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  FieldScript_SetMessageId(reunion + REUNION_IVAN_WANTS_STEALTH);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  FieldScript_SetMessageId(reunion + REUNION_GARET_ASKS_TO_FIGHT);
  Script_OpenMessage(SPEAKER_WINDOW_ABOVE | LOCALID_GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_SetMessageId(reunion + REUNION_BUNZA_DISCOURAGES_FIGHT);
    Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  } else {
    FieldScript_SetMessageId(reunion + REUNION_BUNZA_AGREES_NOT_TO_FIGHT);
    Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  }

  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_BUNZA, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_BUNZA, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_MIA, LOCALID_BUNZA, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_BUNZA, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_HAMMET, LOCALID_BUNZA, 0);
  ObjectEvent_ShowEmote(LOCALID_HAMMET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  messageBase = MsgLunpaBunza;
  FieldScript_SetMessageId(messageBase + PLAN_HAMMET_ASKS_PLAN);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_HAMMET, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_BUNZA, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  FieldScript_SetMessageId(messageBase + PLAN_BUNZA_HAS_WAGON);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  FieldScript_SetMessageId(messageBase + PLAN_MIA_ASKS_ABOUT_WAGON);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_MIA, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_BUNZA, LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_NOD);
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  FieldScript_SetMessageId(messageBase + PLAN_BUNZA_OFFERS_RIDE);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_BUNZA, 0);
  FieldScript_SetMessageId(messageBase + PLAN_IVAN_DOUBTS_WAGON);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_SHAKE_HEAD);
  FieldScript_SetMessageId(messageBase + PLAN_BUNZA_REASSURES);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_BUNZA, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_BUNZA, 0);
  FieldScript_SetMessageId(messageBase + PLAN_GARET_AGREES);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_GARET, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_BUNZA, LOCALID_GARET, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(messageBase + PLAN_BUNZA_IS_UNSUSPECTED);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_BUNZA, 0);
  ObjectEvent_LinkPair(LOCALID_HAMMET, LOCALID_IVAN, 0);
  FieldScript_WaitFrames(60);
  Import_ObjectEventSetAnimation(LOCALID_HAMMET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(60);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_NOD);
  ObjectEvent_FaceObjectEvent(LOCALID_BUNZA, LOCALID_PARTY_LEADER, 0);
  Import_ObjectEventSetAnimation(LOCALID_BUNZA, ANIM_NOD);
  FieldScript_WaitFrames(60);
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
  FieldScript_SetMessageId(wagon + WAGON_BUNZA_LEADS_THE_WAY);
  FieldScript_ShowMessageAndWait(LOCALID_BUNZA, 0);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  FieldScript_SetMessageId(wagon + WAGON_GARET_ASKS_TO_RIDE);
  FieldMessage_ShowAtObject(LOCALID_GARET, 0);
  ObjectMotion_SetAngleToward(LOCALID_IVAN, LOCALID_PARTY_LEADER, 0);
  ObjectMotion_SetAngleToward(LOCALID_MIA, LOCALID_PARTY_LEADER, 0);
  ObjectMotion_SetAngleToward(LOCALID_HAMMET, LOCALID_PARTY_LEADER, 0);
  ObjectMotion_SetAngleToward(LOCALID_BUNZA, LOCALID_PARTY_LEADER, 0);

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
      if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER,
                                                       0) == 0) {
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
    FieldScript_SetMessageId(confusion + CONFUSION_IVAN_IS_CONFUSED);
    FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
    FieldScript_SetMessageId(confusion + CONFUSION_GARET_ASKS_AGAIN);
    FieldMessage_ShowAtObject(LOCALID_GARET, 0);
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
  return FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER,
                                                      0) == 0;
}

/// Asks whether the party really intends to refuse the ride.
static u8 WestLunpaCave_GaretAsksIfNotRiding(void) {
  FieldScript_SetMessageId(MsgLunpaGaretAsksIfNotRiding);
  FieldMessage_ShowAtObject(LOCALID_GARET, 0);
  return FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER,
                                                      0) == 0;
}

/// Asks whether unfinished business explains the refusal to ride.
static u8 WestLunpaCave_GaretAsksAboutUnfinishedBusiness(void) {
  FieldScript_SetMessageId(MsgLunpaGaretAsksAboutUnfinishedBusiness);
  FieldMessage_ShowAtObject(LOCALID_GARET, 0);
  return FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER,
                                                      0) == 0;
}

/// Reserved confirmation step; it always succeeds and displays no message.
static u8 Party_ConfirmsStaying(void) { return TRUE; }

/// Lets Bunza check whether the party has unfinished business before leaving.
static u8 WestLunpaCave_BunzaAsksAboutUnfinishedBusiness(void) {
  FieldScript_SetMessageId(MsgLunpaBunzaAsksAboutUnfinishedBusiness);
  FieldMessage_ShowAtObject(LOCALID_BUNZA, 0);
  return FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER,
                                                      0) == 0;
}

/// Opens Garet's confirmation prompt; WagonChoice_Run collects the answer.
static u8 WestLunpaCave_GaretChecksNothingLeftToDo(void) {
  FieldScript_SetMessageId(MsgLunpaGaretChecksNothingLeft);
  FieldMessage_ShowAtObject(LOCALID_GARET, 0);
  return TRUE;
}

/// Asks whether the party has changed its mind about taking the wagon.
static u8 WestLunpaCave_MiaAsksIfRidingAfterAll(void) {
  FieldScript_SetMessageId(MsgLunpaMiaAsksIfRidingAfter);
  FieldMessage_ShowAtObject(LOCALID_MIA, 0);
  return FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER,
                                                      0) == 0;
}

/// Explains that Bunza cannot wait and asks again about the party's business.
static u8 WestLunpaCave_BunzaCannotWait(void) {
  s32 warning;

  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  warning = MsgLunpaMeaningWontRide;
  FieldScript_SetMessageId(warning + WARNING_IVAN_ASKS_IF_STAYING);
  Script_ShowMessage(LOCALID_IVAN, 0);
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_SHAKE_HEAD);
  FieldScript_SetMessageId(warning + WARNING_BUNZA_CANNOT_WAIT);
  Script_ShowMessage(LOCALID_BUNZA, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  FieldScript_SetMessageId(warning + WARNING_MIA_ASKS_ABOUT_BUSINESS);
  Script_OpenMessage(LOCALID_MIA, 0);
  return FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER,
                                                      0) == 0;
}

/// Asks the leader to confirm that something still needs doing here.
static u8 WestLunpaCave_GaretAsksAboutThingsToDo(void) {
  FieldScript_SetMessageId(MsgLunpaGaretAsksAboutThingsTo);
  FieldMessage_ShowAtObject(LOCALID_GARET, 0);
  return FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER,
                                                      0) == 0;
}

/// Sends Hammet and Bunza ahead, then returns the companions to the leader.
static void Party_StaysBehind(void) {
  struct ObjectEvent *leader;
  s32 farewell;

  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  farewell = MsgLunpaInsistStick;
  FieldScript_SetMessageId(farewell + FAREWELL_GARET_STAYS);
  Script_ShowMessage(LOCALID_GARET, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 1);
  FieldScript_SetMessageId(farewell + FAREWELL_MIA_STAYS);
  Script_ShowMessage(LOCALID_MIA, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_HAMMET, 0);
  FieldScript_WaitFrames(60);
  FieldScript_SetMessageId(farewell + FAREWELL_IVAN_STAYS);
  Script_ShowMessage(LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_HAMMET, LOCALID_IVAN, 0);
  ObjectEvent_ShowEmote(LOCALID_HAMMET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  FieldScript_SetMessageId(farewell + FAREWELL_HAMMET_LETS_IVAN_GO);
  Script_ShowMessage(LOCALID_HAMMET, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_SHAKE_HEAD);
  FieldScript_SetMessageId(farewell + FAREWELL_BUNZA_SAYS_GOODBYE);
  Script_ShowMessage(LOCALID_BUNZA, 0);
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(60);
  ObjectEvent_SetDestinationOffset(LOCALID_HAMMET, -16, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_HAMMET);
  Import_ObjectEventSetAnimation(LOCALID_HAMMET, ANIM_STAND);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_NOD);
  ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(30);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_NOD);
  ObjectEvent_WalkTo(LOCALID_BUNZA, 156, 528);
  FieldScript_WaitFrames(20);
  ObjectEvent_WalkTo(LOCALID_HAMMET, 164, 528);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_BUNZA);
  ObjectEvent_WalkTo(LOCALID_BUNZA, 168, 640);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_HAMMET);
  ObjectEvent_WalkTo(LOCALID_HAMMET, 168, 640);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_SetPosition(LOCALID_HAMMET, 0, 0);
  ObjectEvent_SetPosition(LOCALID_BUNZA, 0, 0);
  FieldScript_WaitFrames(110);

  FieldScript_SetMessageId(farewell + FAREWELL_GARET_SIGHS);
  Script_ShowMessage(LOCALID_GARET, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_SetMessageId(farewell + FAREWELL_MIA_HOPES_FOR_SAFETY);
  Script_ShowMessage(LOCALID_MIA, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_SetMessageId(farewell + FAREWELL_IVAN_REASSURES);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(140);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  FieldScript_SetMessageId(farewell + FAREWELL_GARET_MOVES_ON);
  Script_ShowMessage(LOCALID_GARET, 0);

  // Merge the companions back into the leader and restore player camera
  // control.
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(LOCALID_GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(LOCALID_IVAN, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IVAN);
  ObjectEvent_SetPosition(LOCALID_IVAN, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(LOCALID_MIA, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_MIA);
  ObjectEvent_SetPosition(LOCALID_MIA, 0, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_PlaceCurrentWithinCameraBounds(LOCALID_PARTY_LEADER, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  GameFlag_Set(FLAG_BUNZAS_WAGON_LEFT);
}

/// Leads everyone down the passage and requests the wagon departure warp.
static void Party_RidesWagon(void) {
  struct ObjectEvent *leader;
  s32 departure;

  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 0);
  departure = MsgLunpaDontUnfinishedBusiness;
  FieldScript_SetMessageId(departure + DEPARTURE_GARET_HEADS_FOR_KALAY);
  Script_ShowMessage(LOCALID_GARET, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_SetMessageId(departure + DEPARTURE_IVAN_THINKS_OF_LAYANA);
  Script_ShowMessage(LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_HAMMET, LOCALID_IVAN, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_NOD);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(departure + DEPARTURE_HAMMET_LONGS_FOR_LAYANA);
  Script_ShowMessage(LOCALID_HAMMET, 0);
  ObjectEvent_FaceDirection(LOCALID_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_BUNZA, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_SetMessageId(departure + DEPARTURE_BUNZA_SETS_OFF);
  Script_ShowMessage(LOCALID_BUNZA, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(80);
  ObjectEvent_SetDestinationOffset(LOCALID_HAMMET, -16, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_HAMMET);
  Import_ObjectEventSetAnimation(LOCALID_HAMMET, ANIM_STAND);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetAnimationAndWait(LOCALID_HAMMET, ANIM_NOD);
  ObjectEvent_FaceDirection(LOCALID_HAMMET, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(30);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_NOD);
  ObjectEvent_WalkTo(LOCALID_BUNZA, 152, 528);
  FieldScript_WaitFrames(20);
  ObjectEvent_WalkTo(LOCALID_HAMMET, 160, 528);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_BUNZA);
  ObjectEvent_WalkTo(LOCALID_BUNZA, 168, 640);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_HAMMET);
  ObjectEvent_WalkTo(LOCALID_HAMMET, 168, 640);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 0);
  FieldScript_WaitFrames(200);
  ObjectEvent_SetPosition(LOCALID_HAMMET, 0, 0);
  ObjectEvent_SetPosition(LOCALID_BUNZA, 0, 0);

  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(LOCALID_GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(LOCALID_IVAN, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IVAN);
  ObjectEvent_SetPosition(LOCALID_IVAN, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(LOCALID_MIA, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_MIA);
  ObjectEvent_SetPosition(LOCALID_MIA, 0, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, -16, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  ObjectEvent_PlaceCurrentWithinCameraBounds(LOCALID_PARTY_LEADER, 1);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 168, 640);
  FieldScript_WaitFrames(60);
  FieldScript_FinishScreenTransition();
  Import_ScriptRequestWarp(WEST_LUNPA_CAVE_BY_WAGON);
}

/// Restores the gate and frozen puddles from the persistent puzzle flags.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gSaveBlock.sceneId == (s32)&SceneId_WestLunpaCave) {
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_UNUSED_PUDDLE), 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_SOUTH_PUDDLE), 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_GATE_PUDDLE), 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_NORTH_PUDDLE), 0);
    ObjectEvent_GetById(LOCALID_NORTH_PUDDLE)->scaleY = 19 * Q16_ONE / 20;
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
      Import_ObjectEventSetAnimation(LOCALID_NORTH_PUDDLE,
                                     PUDDLE_ANIMATION_FROZEN);
    }
    if (GameFlag_Get(FLAG_WEST_LUNPA_CAVE_SOUTH_PUDDLE_FROZEN) != 0) {
      Import_ObjectEventSetAnimation(LOCALID_SOUTH_PUDDLE,
                                     PUDDLE_ANIMATION_FROZEN);
    }
  }
  return 0;
}

// clang-format off
const struct MapWarpDest gCaveWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { WEST_LUNPA_CAVE_FROM_WORLD_MAP, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, 88, 56, 648, 648, 0 },
    { WEST_LUNPA_CAVE_FROM_LUNPA, CONDITION_ALWAYS, 552, 0, 328, FACING_NORTH, 0, 88, 56, 648, 648, 0 },
    { WEST_LUNPA_CAVE_FROM_LUNPA_UNUSED, CONDITION_ALWAYS, 552, 0, 328, FACING_NORTH, 0, 88, 56, 648, 488, 0 },
    { MAP_TABLE_END },
};

const u32 gCaveWarps[] = {
    MAP_WARPS(WestLunpaCave),
    MAP_WARP(WEST_LUNPA_CAVE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_WEST_LUNPA_CAVE),
    MAP_WARP(WEST_LUNPA_CAVE_TO_LUNPA, Lunpa, LUNPA_FROM_WEST_LUNPA_CAVE),
    MAP_WARP(WEST_LUNPA_CAVE_BY_WAGON, WorldMap, WORLD_MAP_FROM_WEST_LUNPA_CAVE),
    MAP_WARPS_END,
};

const struct ObjectEventTemplate gCaveNoObjectEventTemplates[] = {
    { MAP_TABLE_END },
};

const struct ObjectEventTemplate gCaveObjectEventTemplates[] = {
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(136), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(344), 0, PIXELS(248), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(424), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BUNZA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0 },
    { LOCALID_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { LOCALID_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

/// Unused: an empty event table nothing returns.
const struct MapEvent gCaveNoEvents[] = {
    { MAP_EVENTS_END },
};

const struct MapEvent gCaveEvents[] = {
    { MAP_EVENT_WARP, MB_WEST_LUNPA_CAVE_WORLD_MAP_PASSAGE, CONDITION_ALWAYS, WEST_LUNPA_CAVE_TO_WORLD_MAP },
    { MAP_EVENT_WARP, MB_WEST_LUNPA_CAVE_LUNPA_PASSAGE, CONDITION_ALWAYS, WEST_LUNPA_CAVE_TO_LUNPA },
    { MAP_EVENT_COORD, MB_WEST_LUNPA_CAVE_BUNZA_HIDING_PLACE, FLAG_LUNPA_CAVE_REUNION_SEEN,
      EVENT_SCRIPT(EventScript_HammetFindsBunza) },
    { BG_EVENT(BG_EVENT_SILENT), MB_WEST_LUNPA_CAVE_LOWERING_SWITCH, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_FlipLoweringSwitch) },
    { BG_EVENT(BG_EVENT_SILENT), MB_WEST_LUNPA_CAVE_RAISING_SWITCH, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_FlipRaisingSwitch) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_UNUSED_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_UnusedFreezePuddle) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_SOUTH_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_FreezeSouthPuddle) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_GATE_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_FreezeGatePuddle) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_NORTH_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_FreezeNorthPuddle) },
    { MAP_EVENTS_END },
};
// clang-format on

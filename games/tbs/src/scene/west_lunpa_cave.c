/// @file
/// West Lunpa Cave (SceneId_WestLunpaCave, area name West Lunpa Cave), the
/// cave between Lunpa and the world map, with a gate across it, where the
/// party meets Bunza on the way out of Lunpa with Hammet.
///
/// The party comes in from the world map (WEST_LUNPA_CAVE_FROM_WORLD_MAP) and
/// from Lunpa (WEST_LUNPA_CAVE_FROM_LUNPA); TRIGGER_WORLD_MAP_PASSAGE and
/// TRIGGER_LUNPA_PASSAGE lead back out. WEST_LUNPA_CAVE_FROM_LUNPA_UNUSED,
/// which no exit names, repeats WEST_LUNPA_CAVE_FROM_LUNPA with a shorter
/// camera.
///
/// The gate puzzle runs on scene flags, which Scene_Initialize redraws: the
/// lowering switch brings the gate down and the raising switch lifts it, each
/// doing nothing while the gate already stands where it would put it or while
/// the gate's puddle, frozen by Frost into a pillar, props it open. Freezing
/// that puddle with the gate down raises it onto the pillar with the
/// puzzle-solved tune. Frost also freezes a puddle to the south into a pillar
/// that can be climbed, and one to the north. Its scene flags:
///
/// - FLAG_SCENE_TEMP_00: the gate's puddle is frozen, propping the gate open.
/// - FLAG_SCENE_TEMP_01: the lowering switch has lowered the gate.
/// - FLAG_SCENE_TEMP_02: the gate is raised.
/// - FLAG_SCENE_TEMP_03: the north puddle is frozen.
/// - FLAG_SCENE_TEMP_04: the south puddle is frozen into a pillar, which
///   changes one cell's collision.
///
/// Once Donpa has let the party leave Lunpa Fortress with Hammet
/// (FLAG_DODONPA_LOCKED_UP), stepping onto TRIGGER_BUNZA_HIDING_PLACE the
/// first time (FLAG_LUNPA_CAVE_REUNION_SEEN) plays the reunion: Bunza, a Kalay
/// merchant, comes out from behind a rock (Event_HammetFindsBunza), they talk
/// (Reunion_Converse), and the party's answers decide whether it rides home in
/// Bunza's wagon (WagonChoice_Run). Riding leaves by WEST_LUNPA_CAVE_BY_WAGON
/// to WORLD_MAP_FROM_WEST_LUNPA_CAVE, where world_map.c plays the departure;
/// staying, Hammet and Bunza leave alone. Either way FLAG_BUNZAS_WAGON_LEFT
/// is set, on the world map or here.
///
/// Unused: a fourth puddle stands at the map's origin, outside the cave's
/// camera bounds, so Frost can never reach it (Event_UnusedFreezePuddle).
///
/// Open questions:
/// - None.

#include "game_state.h"
#include "scene_ids.h"
#include "field_event.h"
#include "unit/scene/west_lunpa_cave.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "west_lunpa_cave.h"
#include "text/msg_ids.h"
#include "fixed_math.h"

static void Reunion_Converse(void);
static void WagonChoice_Run(void);
static u8 Leader_AnswersYes(void);
static u8 Garet_AsksIfNotRiding(void);
static u8 Garet_AsksAboutUnfinishedBusiness(void);
static u8 Party_ConfirmsStaying(void);
static u8 Bunza_AsksAboutUnfinishedBusiness(void);
static u8 Garet_ChecksNothingLeftToDo(void);
static u8 Mia_AsksIfRidingAfterAll(void);
static u8 Bunza_CannotWait(void);
static u8 Garet_AsksAboutThingsToDo(void);
static void Party_StaysBehind(void);
static void Party_RidesWagon(void);

// The cave's scene flags, the state of its gate puzzle:
// FLAG_SCENE_TEMP_00: the gate puddle is frozen, propping the gate open.
// FLAG_SCENE_TEMP_01: the lowering switch has lowered the gate.
// FLAG_SCENE_TEMP_02: the gate is raised.
// FLAG_SCENE_TEMP_03: the north puddle is frozen into a pillar.
// FLAG_SCENE_TEMP_04: the south puddle is frozen into a pillar.

const struct SceneEntrance *Scene_GetEntrances(void) { return gCaveEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

/// Both passages and the wagon departure return through this exit table.
const u32 *Scene_GetExits(void) { return gCaveExits; }

/// Only the cave scene places puddles, Hammet, Bunza and the party members.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_WestLunpaCave) {
    return gCavePlacements;
  }
  return gCaveNoPlacements;
}

/// Routes switches, Frost targets and the reunion trigger to their handlers.
const struct SceneEvent *Scene_GetEvents(void) { return gCaveEvents; }

/// Unused: Frost can never target the puddle placed at (0, 0, 0), outside the
/// cave's camera bounds. It would freeze, and no flag would remember it.
static void Event_UnusedFreezePuddle(void) {
  struct FieldActor *puddle;

  puddle = Object_GetById(ACTOR_UNUSED_PUDDLE);
  if (puddle != NULL) {
    Engine_ActorSetSpriteFlags(puddle, 0);
  }
}

/// In West Lunpa Cave, Frost on the south puddle: it rises as an ice pillar the
/// party can climb.
static void Event_FreezeSouthPuddle(void) {
  struct FieldActor *puddle;

  puddle = Actor_Get(ACTOR_SOUTH_PUDDLE);
  if (puddle != NULL) {
    puddle->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    puddle->motionFlags = 0;
  }
  Map_CopyCellAttributes(7, 32, 1, 1, 8, 32);
  GameFlag_Set(FLAG_SCENE_TEMP_04);
}

/// Animates the raised gate resting on the frozen pillar, then opens collision.
static void Gate_DrawPropped(void) {
  struct FieldActor *pillar;

  pillar = Actor_Get(ACTOR_GATE_PUDDLE);
  Engine_ActorSetAnimation(ACTOR_GATE_PUDDLE, PUDDLE_ANIM_FROZEN);
  if (pillar != NULL) {
    Engine_ActorSetSpriteFlags(pillar, 0);
    pillar->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }
  Map_CopyCells(41, 87, 2, 5, 21, 59);
  Engine_TaskWait(4);
  Map_CopyCells(3, 93, 1, 1, 24, 62);
  Map_CopyCells(1, 94, 1, 1, 21, 55);
  Map_CopyCells(43, 87, 2, 5, 21, 58);
  Engine_TaskWait(4);
  Map_CopyCells(41, 87, 2, 5, 21, 58);
  Engine_TaskWait(4);
  Engine_TaskWait(4);
  Map_CopyCellAttributes(21, 11, 2, 2, 21, 13);
  Map_CopyCellAttributes(21, 11, 1, 1, 22, 15);
  Map_CopyCellAttributes(19, 17, 1, 1, 21, 14);
}

/// In West Lunpa Cave, Frost on the puddle under the gate: its pillar props the
/// gate open, and if the gate was down the player sees it rise and hears the
/// puzzle solved.
static void Event_FreezeGatePuddle(void) {
  struct FieldActor *pillar;

  pillar = Actor_Get(ACTOR_GATE_PUDDLE);
  GameFlag_Set(FLAG_SCENE_TEMP_00);
  if (pillar != NULL) {
    Engine_ActorSetSpriteFlags(pillar, 0);
    pillar->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_02) == 0) {
    Event_PlayCue(SE_GATE_MOVE);
    Gate_DrawPropped();
    Event_PlayCue(MUS_PUZZLE_SOLVED);
    GameFlag_Set(FLAG_SCENE_TEMP_02);
  }
}

/// In West Lunpa Cave, Frost on the north puddle: its pillar stays frozen when
/// the party returns.
static void Event_FreezeNorthPuddle(void) {
  Engine_GameFlagSet(FLAG_SCENE_TEMP_03);
}

/// Lowers the gate in four-frame steps and restores the blocked cell
/// attributes.
static void Gate_Lower(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_02) != 0) {
    Map_CopyCells(41, 86, 2, 6, 21, 57);
    Engine_TaskWait(4);
    Map_CopyCells(43, 86, 2, 6, 21, 57);
    Engine_TaskWait(4);
    Map_CopyCells(41, 86, 2, 6, 21, 58);
    Engine_TaskWait(4);
    Map_CopyCells(43, 86, 2, 6, 21, 58);
    Engine_TaskWait(4);
  }
  Map_CopyCells(2, 93, 1, 1, 24, 62);
  Map_CopyCells(2, 94, 1, 1, 21, 55);
  Map_CopyCells(41, 86, 2, 6, 21, 59);
  Engine_TaskWait(4);
  Map_CopyCells(1, 93, 1, 1, 24, 62);
  Map_CopyCells(3, 94, 1, 1, 21, 55);
  Map_CopyCells(43, 86, 2, 6, 21, 59);
  Engine_TaskWait(4);
  Engine_ActorSetSpritePriority(ACTOR_GATE_PUDDLE, GATE_PILLAR_PRIORITY);
  Map_CopyCellAttributes(19, 17, 1, 1, 22, 15);
}

/// In West Lunpa Cave, searching the lowering switch: the leader flips it and
/// the gate comes down, unless it is down already or a frozen pillar props it
/// open. "flipped the switch."
static void Event_FlipLoweringSwitch(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_01) == 0) {
    if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) == 0) {
      Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
      Event_PlayCue(SE_GATE_MOVE);
      Gate_Lower();
      GameFlag_Set(FLAG_SCENE_TEMP_01);
      GameFlag_Clear(FLAG_SCENE_TEMP_02);
    }
  }
}

/// Raises the unpropped gate and restores its passable cell attributes.
static void Gate_Raise(void) {
  Map_CopyCells(41, 87, 2, 5, 21, 59);
  Engine_TaskWait(4);
  Map_CopyCells(2, 93, 1, 1, 24, 62);
  Map_CopyCells(2, 94, 1, 1, 21, 55);
  Map_CopyCells(43, 87, 2, 5, 21, 58);
  Engine_TaskWait(4);
  Map_CopyCells(3, 93, 1, 1, 24, 62);
  Map_CopyCells(1, 94, 1, 1, 21, 55);
  Map_CopyCells(41, 87, 2, 5, 21, 58);
  Map_CopyCellAttributes(21, 11, 2, 2, 21, 13);
  Map_CopyCellAttributes(19, 17, 1, 1, 21, 14);
}

/// In West Lunpa Cave, searching the raising switch: the leader flips it and
/// the gate goes up, unless it is up already or propped open. "flipped the
/// switch."
static void Event_FlipRaisingSwitch(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) == 0) {
    if (GameFlag_IsSet(FLAG_SCENE_TEMP_02) == 0) {
      Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
      Event_PlayCue(SE_GATE_MOVE);
      Gate_Raise();
      GameFlag_Set(FLAG_SCENE_TEMP_02);
      GameFlag_Clear(FLAG_SCENE_TEMP_01);
    }
  }
}

/// In West Lunpa Cave, Hammet finds Bunza hiding behind a rock. It plays on
/// stepping onto trigger 10, by the rock Bunza hides behind, after Donpa has
/// let the party leave the fortress with Hammet (FLAG_DODONPA_LOCKED_UP). Bunza
/// steps out, the party and Hammet gather behind the leader, Garet and Mia
/// think they saw someone, and Ivan asks whether Dodonpa's henchmen have found
/// them (a yes: Garet will fight; a no: Garet wonders what else it could be).
/// Hammet calls out and names Bunza, Bunza knows Hammet's voice, and the
/// reunion and the wagon talk follow. Sets FLAG_LUNPA_CAVE_REUNION_SEEN, so it
/// plays once.
static void Event_HammetFindsBunza(void) {
  struct FieldActor *leader;
  s32 sighting;
  s32 recognition;

  if (GameFlag_IsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    // Bring Hammet and the party into view as Bunza leaves his hiding place.
    GameFlag_Set(FLAG_LUNPA_CAVE_REUNION_SEEN);
    Engine_EventBegin();
    Actor_SetPosition(ACTOR_BUNZA, PIXELS(144), PIXELS(400));
    Actor_SetSpeed(ACTOR_BUNZA, PIXELS(3) / 2, PIXELS(3) / 4);
    Actor_WalkTo(ACTOR_BUNZA, 184, 400);
    Engine_ActorWaitForMove(ACTOR_BUNZA);
    Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_STAND);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(30);
    Camera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
    Camera_MoveTo(PIXELS(192), -1, PIXELS(432), 1);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetPosition(ACTOR_HAMMET, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(ACTOR_HAMMET, PIXELS(13) / 10, PIXELS(13) / 20);
    Actor_WalkTo(ACTOR_HAMMET, 168, 464);
    Engine_ActorWaitForMove(ACTOR_HAMMET);
    Actor_FaceDirection(ACTOR_HAMMET, FACING_NORTH, 0);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(IVAN, PIXELS(13) / 10, PIXELS(13) / 20);
    Actor_WalkTo(IVAN, 152, 488);
    Engine_ActorWaitForMove(IVAN);
    Actor_FaceDirection(IVAN, FACING_NORTH, 0);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(MIA, PIXELS(13) / 10, PIXELS(13) / 20);
    Actor_WalkTo(MIA, 168, 488);
    Engine_ActorWaitForMove(MIA);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetSpeed(GARET, PIXELS(13) / 10, PIXELS(13) / 20);
    Actor_WalkTo(GARET, 184, 488);
    Engine_ActorWaitForMove(GARET);
    Actor_FaceDirection(GARET, FACING_NORTH, 0);
    Engine_EventWait(30);

    // The party spots someone ahead, before Bunza recognizes Hammet's voice.
    Engine_ActorRunRepeatedMotion(GARET, 1);
    sighting = MsgLunpaThinkSawSomeone;
    Engine_EventSetMessage(sighting + SIGHTING_GARET_SAW_SOMEONE);
    Event_ShowMessage(GARET, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    Engine_EventWait(10);
    Engine_EventSetMessage(sighting + SIGHTING_MIA_SAW_SOMETHING);
    Event_ShowMessage(MIA, 0);
    Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
    Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
    Engine_EventSetMessage(sighting + SIGHTING_IVAN_ASKS_IF_FOUND);
    Event_OpenMessage(IVAN, 0);
    Actor_FaceActor(MIA, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(30);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      Engine_EventSetMessage(sighting + SIGHTING_GARET_WILL_FIGHT);
      Event_ShowMessage(GARET, 0);
    } else {
      Engine_EventSetMessage(sighting + SIGHTING_GARET_ASKS_WHAT_ELSE);
      Event_ShowMessage(GARET, 0);
    }

    Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
    recognition = MsgLunpaMoment;
    Engine_EventSetMessage(recognition + RECOGNITION_HAMMET_CALLS_OUT);
    Event_ShowMessage(ACTOR_HAMMET, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(GARET, 2);
    Engine_ActorStartRepeatedMotion(IVAN, 2);
    Engine_ActorStartRepeatedMotion(MIA, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTH, 0);
    Actor_FaceDirection(IVAN, FACING_NORTH, 0);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_WALK);
    Actor_WalkBy(ACTOR_HAMMET, 0, -16);
    Engine_ActorWaitForMove(ACTOR_HAMMET);
    Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_STAND);
    Engine_EventSetMessage(recognition + RECOGNITION_HAMMET_NAMES_BUNZA);
    Event_ShowMessage(ACTOR_HAMMET, 0);
    Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 65);
    Engine_EventSetMessage(recognition + RECOGNITION_BUNZA_KNOWS_VOICE);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    Engine_EventWait(80);
    Actor_SetSpeed(ACTOR_BUNZA, PIXELS(2) / 5, PIXELS(1) / 5);
    Actor_WalkBy(ACTOR_BUNZA, -13, 0);
    Engine_ActorWaitForMove(ACTOR_BUNZA);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH, 0);
    Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
    Engine_EventSetMessage(recognition + RECOGNITION_BUNZA_NAMES_HAMMET);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Actor_WalkTo(ACTOR_BUNZA, 168, 432);
    Engine_EventWait(40);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_EventEnd();
    Reunion_Converse();
  }
}

/// The reunion, with the screen closed and reopened on the group: Hammet and
/// Ivan tell how the party came to his rescue, Bunza tells how he came to
/// Lunpa and found the cave, warns that Dodonpa's henchmen are combing the
/// area, and offers his wagon as a way out of Lunpa.
static void Reunion_Converse(void) {
  s32 reunion;
  s32 plan;

  // Arrange the reunion while the screen is closed, then reveal the group.
  Engine_EventBegin();
  Actor_SetPosition(GARET, PIXELS(200), PIXELS(544));
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(184), PIXELS(544));
  Actor_SetPosition(MIA, PIXELS(168), PIXELS(544));
  Actor_SetPosition(IVAN, PIXELS(212), PIXELS(528));
  Actor_SetPosition(ACTOR_HAMMET, PIXELS(200), PIXELS(512));
  Actor_SetPosition(ACTOR_BUNZA, PIXELS(168), PIXELS(512));
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_EAST, 0);
  Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_STAND);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
  Engine_TaskWait(1);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);

  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  reunion = MsgLunpaDodonpaNeverIntention;
  Engine_EventSetMessage(reunion + REUNION_BUNZA_ASKS_ABOUT_RELEASE);
  Event_ShowMessage(ACTOR_BUNZA, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Engine_EventSetMessage(reunion + REUNION_HAMMET_CREDITS_IVAN);
  Event_ShowMessage(ACTOR_HAMMET, 0);
  Actor_FaceActor(IVAN, ACTOR_HAMMET, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(40);
  Engine_EventSetMessage(reunion + REUNION_IVAN_CREDITS_LEADER);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 80);
  Engine_EventSetMessage(reunion + REUNION_GARET_QUESTIONS_IVAN);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
  Actor_FaceActor(IVAN, GARET, 0);
  Engine_EventSetMessage(reunion + REUNION_IVAN_CREDITS_EVERYONE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH - FACING_STEP, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(60);
  Engine_EventSetMessage(reunion + REUNION_HAMMET_THANKS_PARTY);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_EventWait(120);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH - FACING_STEP, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_SMILE, 80);
  Engine_EventSetMessage(reunion + REUNION_HAMMET_THANKS_IVAN);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
  Actor_FaceActor(IVAN, ACTOR_HAMMET, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);

  // Bunza explains how he reached the cave despite being refused entry.
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceActor(MIA, ACTOR_BUNZA, 0);
  Engine_EventSetMessage(reunion + REUNION_MIA_PRAISES_BUNZA);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | MIA, 0);
  Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
  Engine_EventSetMessage(reunion + REUNION_HAMMET_PRAISES_BUNZA);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  Engine_EventSetMessage(reunion + REUNION_BUNZA_CALLS_IT_CHANCE);
  Event_ShowMessage(ACTOR_BUNZA, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_BUNZA, 1);
  Engine_EventWait(20);
  Engine_EventSetMessage(reunion + REUNION_BUNZA_AVOIDED_LUNPA);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Engine_ActorFaceEachOther(ACTOR_HAMMET, IVAN, 0);
  Engine_EventWait(60);
  Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
  Actor_FaceActor(IVAN, ACTOR_BUNZA, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Engine_EventSetMessage(reunion + REUNION_BUNZA_KNEW_OF_PRISON);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  Actor_FaceActor(GARET, ACTOR_BUNZA, 0);
  Engine_EventSetMessage(reunion + REUNION_GARET_ASKS_WHY_BUNZA_CAME);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
  Actor_FaceActor(ACTOR_BUNZA, GARET, 0);
  Engine_EventWait(20);
  Engine_EventSetMessage(reunion + REUNION_BUNZA_RECALLS_ADVICE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_FaceActor(MIA, ACTOR_BUNZA, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 70);
  Engine_EventSetMessage(reunion + REUNION_MIA_ASKS_ABOUT_TRADE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | MIA, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Engine_EventWait(40);
  Engine_EventSetMessage(reunion + REUNION_BUNZA_MEANS_WISDOM);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
  Engine_EventSetMessage(reunion + REUNION_HAMMET_ON_APPEARANCES);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
  Actor_FaceActor(ACTOR_BUNZA, ACTOR_HAMMET, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventSetMessage(reunion + REUNION_BUNZA_ON_UNPLEASANT_PLACES);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventSetMessage(reunion + REUNION_HAMMET_ON_SELLING);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_SMILE, 70);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  Engine_EventWait(30);
  Engine_EventSetMessage(reunion + REUNION_IVAN_ASKS_ABOUT_SERVING);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  Actor_FaceActor(ACTOR_BUNZA, IVAN, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventSetMessage(reunion + REUNION_IVAN_ON_FATE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventSetMessage(reunion + REUNION_GARET_ASKS_ABOUT_ENTRY);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Engine_EventSetMessage(reunion + REUNION_BUNZA_WAS_REFUSED);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventSetMessage(reunion + REUNION_MIA_ASKS_WHY_BUNZA_STILL_CAME);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | MIA, 0);
  Actor_FaceActor(ACTOR_BUNZA, MIA, 0);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Engine_EventSetMessage(reunion + REUNION_BUNZA_MENTIONS_COMMOTION);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Engine_ActorFaceEachOther(IVAN, ACTOR_HAMMET, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventSetMessage(reunion + REUNION_IVAN_EXPLAINS_COMMOTION);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
  Actor_FaceActor(IVAN, ACTOR_BUNZA, 0);
  Engine_EventWait(20);
  Engine_EventSetMessage(reunion + REUNION_BUNZA_LINKS_COMMOTION);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_SetAttachedEffect(ACTOR_BUNZA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventSetMessage(reunion + REUNION_BUNZA_HAD_TO_KNOW);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_FaceActor(GARET, ACTOR_BUNZA, 0);
  Engine_EventSetMessage(reunion + REUNION_GARET_ASKS_ABOUT_CAVE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventSetMessage(reunion + REUNION_MIA_ON_GATE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | MIA, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_BUNZA, 1);
  Engine_EventSetMessage(reunion + REUNION_BUNZA_HID);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_BUNZA, 1);
  Engine_EventSetMessage(reunion + REUNION_HAMMET_STARTLED_BUNZA);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
  Engine_EventWait(30);
  Actor_FaceActor(ACTOR_BUNZA, ACTOR_HAMMET, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Engine_EventSetMessage(reunion + REUNION_BUNZA_IS_GLAD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Engine_EventWait(30);

  // Bunza checks the passage, worries about pursuit and proposes his wagon.
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH, 0);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_SetSpeed(ACTOR_BUNZA, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_WalkTo(ACTOR_BUNZA, 144, 528);
  Engine_ActorWaitForMove(ACTOR_BUNZA);
  Actor_WalkTo(ACTOR_BUNZA, 168, 560);
  Engine_ActorWaitForMove(ACTOR_BUNZA);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Engine_EventWait(40);
  Actor_WalkTo(ACTOR_BUNZA, 144, 528);
  Engine_ActorWaitForMove(ACTOR_BUNZA);
  Actor_WalkTo(ACTOR_BUNZA, 168, 488);
  Engine_EventSetMessage(reunion + REUNION_IVAN_ASKS_WHY);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  Engine_ActorWaitForMove(ACTOR_BUNZA);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_STAND);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventSetMessage(reunion + REUNION_BUNZA_WARNS_OF_SEARCH);
  Event_ShowMessage(ACTOR_BUNZA, 0);
  Engine_EventWait(20);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_EventSetMessage(reunion + REUNION_BUNZA_FEARS_CAPTURE);
  Event_ShowMessage(ACTOR_BUNZA, 0);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventSetMessage(reunion + REUNION_MIA_URGES_ESCAPE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | MIA, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_EventSetMessage(reunion + REUNION_IVAN_WANTS_STEALTH);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Engine_EventSetMessage(reunion + REUNION_GARET_ASKS_TO_FIGHT);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventSetMessage(reunion + REUNION_BUNZA_DISCOURAGES_FIGHT);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  } else {
    Engine_EventSetMessage(reunion + REUNION_BUNZA_AGREES_NOT_TO_FIGHT);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  }

  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_BUNZA, 0);
  Actor_FaceActor(GARET, ACTOR_BUNZA, 0);
  Actor_FaceActor(MIA, ACTOR_BUNZA, 0);
  Actor_FaceActor(IVAN, ACTOR_BUNZA, 0);
  Actor_FaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
  Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  plan = MsgLunpaBunza;
  Engine_EventSetMessage(plan + PLAN_HAMMET_ASKS_PLAN);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  Engine_EventSetMessage(plan + PLAN_BUNZA_HAS_WAGON);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_EventSetMessage(plan + PLAN_MIA_ASKS_ABOUT_WAGON);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | MIA, 0);
  Actor_FaceActor(ACTOR_BUNZA, MIA, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Engine_EventSetMessage(plan + PLAN_BUNZA_OFFERS_RIDE);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Actor_FaceActor(IVAN, ACTOR_BUNZA, 0);
  Engine_EventSetMessage(plan + PLAN_IVAN_DOUBTS_WAGON);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Engine_EventSetMessage(plan + PLAN_BUNZA_REASSURES);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_EventWait(60);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_BUNZA, 0);
  Actor_FaceActor(GARET, ACTOR_BUNZA, 0);
  Engine_EventSetMessage(plan + PLAN_GARET_AGREES);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
  Actor_FaceActor(ACTOR_BUNZA, GARET, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventSetMessage(plan + PLAN_BUNZA_IS_UNSUSPECTED);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
  Engine_ActorFaceEachOther(ACTOR_HAMMET, IVAN, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Actor_FaceActor(ACTOR_BUNZA, ACTOR_PARTY_LEADER, 0);
  Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_NOD);
  Engine_EventWait(60);
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
  Engine_EventSetMessage(wagon + WAGON_BUNZA_LEADS_THE_WAY);
  Engine_EventShowMessage(ACTOR_BUNZA, 0);
  Engine_ActorFaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventSetMessage(wagon + WAGON_GARET_ASKS_TO_RIDE);
  Engine_EventOpenMessage(GARET, 0);
  Engine_ActorFaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceActor(MIA, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceActor(ACTOR_HAMMET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceActor(ACTOR_BUNZA, ACTOR_PARTY_LEADER, 0);

ask_to_ride:
  if (Leader_AnswersYes()) {
  ask_about_business:
    if (!Bunza_AsksAboutUnfinishedBusiness()) {
      goto ride;
    }
    insisted = FALSE;
    if (!Garet_AsksAboutThingsToDo()) {
    insist_nothing_left:
      insisted = TRUE;
    check_nothing_left:
      Garet_ChecksNothingLeftToDo();
      if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
        goto ride;
      }
    }
    if (Bunza_CannotWait() || !insisted) {
      goto stay;
    }
    goto check_nothing_left;
  } else {
    if (Garet_AsksIfNotRiding()) {
      if (Garet_AsksAboutUnfinishedBusiness()) {
        goto stay;
      }
      goto insist_nothing_left;
    }
    if (Mia_AsksIfRidingAfterAll()) {
      goto ask_about_business;
    }
    confusion = MsgLunpaTotallyConfusedChanged;
    Engine_EventSetMessage(confusion + CONFUSION_IVAN_IS_CONFUSED);
    Engine_EventShowMessage(IVAN, 0);
    Engine_EventSetMessage(confusion + CONFUSION_GARET_ASKS_AGAIN);
    Engine_EventOpenMessage(GARET, 0);
    goto ask_to_ride;
  ride:
    Party_RidesWagon();
    return;
  }
stay:
  Party_ConfirmsStaying();
  Party_StaysBehind();
}

/// The scene menu encodes Yes as zero; expose it as a Boolean to the dialogue.
static u8 Leader_AnswersYes(void) {
  return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

/// Asks whether the party really intends to refuse the ride.
static u8 Garet_AsksIfNotRiding(void) {
  Engine_EventSetMessage(MsgLunpaGaretAsksIfNotRiding);
  Engine_EventOpenMessage(GARET, 0);
  return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

/// Asks whether unfinished business explains the refusal to ride.
static u8 Garet_AsksAboutUnfinishedBusiness(void) {
  Engine_EventSetMessage(MsgLunpaGaretAsksAboutUnfinishedBusiness);
  Engine_EventOpenMessage(GARET, 0);
  return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

/// Reserved confirmation step; it always succeeds and displays no message.
static u8 Party_ConfirmsStaying(void) { return TRUE; }

/// Lets Bunza check whether the party has unfinished business before leaving.
static u8 Bunza_AsksAboutUnfinishedBusiness(void) {
  Engine_EventSetMessage(MsgLunpaBunzaAsksAboutUnfinishedBusiness);
  Engine_EventOpenMessage(ACTOR_BUNZA, 0);
  return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

/// Opens Garet's confirmation prompt; WagonChoice_Run collects the answer.
static u8 Garet_ChecksNothingLeftToDo(void) {
  Engine_EventSetMessage(MsgLunpaGaretChecksNothingLeft);
  Engine_EventOpenMessage(GARET, 0);
  return TRUE;
}

/// Asks whether the party has changed its mind about taking the wagon.
static u8 Mia_AsksIfRidingAfterAll(void) {
  Engine_EventSetMessage(MsgLunpaMiaAsksIfRidingAfter);
  Engine_EventOpenMessage(MIA, 0);
  return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

/// Explains that Bunza cannot wait and asks again about the party's business.
static u8 Bunza_CannotWait(void) {
  s32 warning;

  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  warning = MsgLunpaMeaningWontRide;
  Engine_EventSetMessage(warning + WARNING_IVAN_ASKS_IF_STAYING);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Engine_EventSetMessage(warning + WARNING_BUNZA_CANNOT_WAIT);
  Event_ShowMessage(ACTOR_BUNZA, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_EventSetMessage(warning + WARNING_MIA_ASKS_ABOUT_BUSINESS);
  Event_OpenMessage(MIA, 0);
  return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

/// Asks the leader to confirm that something still needs doing here.
static u8 Garet_AsksAboutThingsToDo(void) {
  Engine_EventSetMessage(MsgLunpaGaretAsksAboutThingsTo);
  Engine_EventOpenMessage(GARET, 0);
  return Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}

/// Sends Hammet and Bunza ahead, then returns the companions to the leader.
static void Party_StaysBehind(void) {
  struct FieldActor *leader;
  s32 farewell;

  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  farewell = MsgLunpaInsistStick;
  Engine_EventSetMessage(farewell + FAREWELL_GARET_STAYS);
  Event_ShowMessage(GARET, 0);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventSetMessage(farewell + FAREWELL_MIA_STAYS);
  Event_ShowMessage(MIA, 0);
  Actor_FaceActor(IVAN, ACTOR_HAMMET, 0);
  Engine_EventWait(60);
  Engine_EventSetMessage(farewell + FAREWELL_IVAN_STAYS);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceActor(ACTOR_HAMMET, IVAN, 0);
  Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  Engine_EventSetMessage(farewell + FAREWELL_HAMMET_LETS_IVAN_GO);
  Event_ShowMessage(ACTOR_HAMMET, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
  Engine_EventSetMessage(farewell + FAREWELL_BUNZA_SAYS_GOODBYE);
  Event_ShowMessage(ACTOR_BUNZA, 0);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_EventWait(60);
  Actor_SetDestinationOffset(ACTOR_HAMMET, -16, 0);
  Engine_ActorWaitForMove(ACTOR_HAMMET);
  Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_STAND);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Actor_WalkTo(ACTOR_BUNZA, 156, 528);
  Engine_EventWait(20);
  Actor_WalkTo(ACTOR_HAMMET, 164, 528);
  Engine_ActorWaitForMove(ACTOR_BUNZA);
  Actor_WalkTo(ACTOR_BUNZA, 168, 640);
  Engine_ActorWaitForMove(ACTOR_HAMMET);
  Actor_WalkTo(ACTOR_HAMMET, 168, 640);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Engine_EventWait(60);
  Actor_SetPosition(ACTOR_HAMMET, 0, 0);
  Actor_SetPosition(ACTOR_BUNZA, 0, 0);
  Engine_EventWait(110);

  Engine_EventSetMessage(farewell + FAREWELL_GARET_SIGHS);
  Event_ShowMessage(GARET, 0);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventSetMessage(farewell + FAREWELL_MIA_HOPES_FOR_SAFETY);
  Event_ShowMessage(MIA, 0);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventSetMessage(farewell + FAREWELL_IVAN_REASSURES);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(140);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventSetMessage(farewell + FAREWELL_GARET_MOVES_ON);
  Event_ShowMessage(GARET, 0);

  // Merge the companions back into the leader and restore player camera
  // control.
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(IVAN, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_ActorSetAnimation(MIA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Engine_EventWait(30);
  Engine_CameraMoveToActor(ACTOR_PARTY_LEADER, 1);
  Engine_CameraWaitForMove();
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
  GameFlag_Set(FLAG_BUNZAS_WAGON_LEFT);
}

/// Leads everyone down the passage and requests the wagon departure exit.
static void Party_RidesWagon(void) {
  struct FieldActor *leader;
  s32 departure;

  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  departure = MsgLunpaDontUnfinishedBusiness;
  Engine_EventSetMessage(departure + DEPARTURE_GARET_HEADS_FOR_KALAY);
  Event_ShowMessage(GARET, 0);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventSetMessage(departure + DEPARTURE_IVAN_THINKS_OF_LAYANA);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceActor(ACTOR_HAMMET, IVAN, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventSetMessage(departure + DEPARTURE_HAMMET_LONGS_FOR_LAYANA);
  Event_ShowMessage(ACTOR_HAMMET, 0);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventSetMessage(departure + DEPARTURE_BUNZA_SETS_OFF);
  Event_ShowMessage(ACTOR_BUNZA, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_EventWait(80);
  Actor_SetDestinationOffset(ACTOR_HAMMET, -16, 0);
  Engine_ActorWaitForMove(ACTOR_HAMMET);
  Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_STAND);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Actor_WalkTo(ACTOR_BUNZA, 152, 528);
  Engine_EventWait(20);
  Actor_WalkTo(ACTOR_HAMMET, 160, 528);
  Engine_ActorWaitForMove(ACTOR_BUNZA);
  Actor_WalkTo(ACTOR_BUNZA, 168, 640);
  Engine_ActorWaitForMove(ACTOR_HAMMET);
  Actor_WalkTo(ACTOR_HAMMET, 168, 640);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Engine_EventWait(200);
  Actor_SetPosition(ACTOR_HAMMET, 0, 0);
  Actor_SetPosition(ACTOR_BUNZA, 0, 0);

  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(IVAN, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_ActorSetAnimation(MIA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Engine_EventWait(30);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, -16, 0);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_CameraMoveToActor(ACTOR_PARTY_LEADER, 1);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 168, 640);
  Engine_EventWait(60);
  Engine_EventCloseScreen();
  Engine_EventRequestExit(WEST_LUNPA_CAVE_BY_WAGON);
}

/// Restores the gate and frozen puddles from the persistent puzzle flags.
s32 Scene_Initialize(void) {
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gGameState.scene == (s32)&SceneId_WestLunpaCave) {
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_UNUSED_PUDDLE), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SOUTH_PUDDLE), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_GATE_PUDDLE), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_NORTH_PUDDLE), 0);
    Object_GetById(ACTOR_NORTH_PUDDLE)->scaleY = 19 * Q16_ONE / 20;
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01) != 0) {
      Gate_Lower();
    }
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02) != 0) {
      Gate_Raise();
    }
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00) != 0) {
      Gate_DrawPropped();
    }
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_03) != 0) {
      Engine_ActorSetAnimation(ACTOR_NORTH_PUDDLE, PUDDLE_ANIM_FROZEN);
    }
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_04) != 0) {
      Engine_ActorSetAnimation(ACTOR_SOUTH_PUDDLE, PUDDLE_ANIM_FROZEN);
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

const struct ScenePlacement gCaveNoPlacements[] = {
    { SCENE_TABLE_END },
};

const struct ScenePlacement gCavePlacements[] = {
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(136), 0, PIXELS(520), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(344), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(424), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BUNZA, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_HAMMET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_WEST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

/// Unused: an empty event table nothing returns.
const struct SceneEvent gCaveNoEvents[] = {
    { SCENE_EVENTS_END },
};

const struct SceneEvent gCaveEvents[] = {
    { EVENT_EXIT, TRIGGER_WORLD_MAP_PASSAGE, CONDITION_ALWAYS, WEST_LUNPA_CAVE_TO_WORLD_MAP },
    { EVENT_EXIT, TRIGGER_LUNPA_PASSAGE, CONDITION_ALWAYS, WEST_LUNPA_CAVE_TO_LUNPA },
    { EVENT_TOUCH, TRIGGER_BUNZA_HIDING_PLACE, FLAG_LUNPA_CAVE_REUNION_SEEN,
      EVENT_SCRIPT(Event_HammetFindsBunza) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_LOWERING_SWITCH, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_FlipLoweringSwitch) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_RAISING_SWITCH, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_FlipRaisingSwitch) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_UNUSED_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_UnusedFreezePuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SOUTH_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_FreezeSouthPuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_GATE_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_FreezeGatePuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_NORTH_PUDDLE, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_FreezeNorthPuddle) },
    { SCENE_EVENTS_END },
};
// clang-format on

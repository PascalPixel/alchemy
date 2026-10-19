/// @file
/// Fuchin Temple and the rooms that share its overlay: the temple grounds with
/// the waterfall grotto, Nyunpa's hall, Master Feh's school in Xian and Master
/// Hama's room in Lama Temple.
#include "game_state.h"
#include "scene_ids.h"
#include "unit/scene/fuchin_temple.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "field_obj.h"
#include "script.h"
#include "types.h"
#include "field_event.h"
#include "engine_call.h"
#include "field_scene.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "flags.h"
#include "songs.h"
#include "event.h"
#include "item_ids.h"
#include "objcmd.h"
#include "global_cells.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.

/// Nyunpa's test was declined; the students saw the training tree fall.
#define FLAG_NYUNPA_TEST_DECLINED 0x300
#define FLAG_FEH_SCHOOL_SAW_TREE_FALL 0x301

// The first scene, outside the temple.
#define FALLS_MONK 9 ///< the monk beneath the falls, at the waterfall grotto
#define HEALER 13

// The second scene: Nyunpa's hall and Master Feh's school.
#define NYUNPA 8
#define JUMPING_STUDENT 12
#define FEIZHI 17
#define FEH 18
#define TRAINING_TREE 19
#define TRAINING_TREE_TARGET 20 ///< where Force strikes the tree

/// The second scene by entrance 3: Hama's room in Lama Temple.
#define HAMA 8

// Forward declarations.
static void FuchinTemple_FeizhiLeavesForHsu(void);
static void FuchinTemple_FehSeesChi(void);
static UNUSED void FuchinTemple_HopLeaderByOffset(s32 offsetX, s32 offsetZ);
static void FuchinTemple_BeginMindReadDemonstration(s32 caster, s32 target);
static void FuchinTemple_FinishMindReadDemonstration(void);
static void FuchinTemple_SpawnNyunpasParticles(void);
static void FuchinTemple_AnimateRevealLesson(void);

// Shared source, included in link order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Clear the scripted actor's sprite flags and complete the script command.
static s32 FuchinTemple_ClearScriptedActorSpriteFlags(
    struct ScriptInterpreter *interpreter) {
  Engine_ActorSetSpriteFlags((struct FieldActor *)interpreter, 0);
  return 0;
}

// Keep an actor facing the party leader while the conversation is staged.
// UB: update hooks return void. The native produces an unused zero result;
// plain void changes five instructions in the English build.
#ifdef UBFIX
static void FuchinTemple_FaceLeader(union FieldObject *object) {
#else
static s32 FuchinTemple_FaceLeader(union FieldObject *object) {
#endif
  struct FieldActor *actor = (struct FieldActor *)object;
  struct FieldActor *player = Object_GetById(ACTOR_PARTY_LEADER);
  actor->facing = ArcTan2(player->z.fixed - actor->z.fixed,
                          player->x.fixed - actor->x.fixed);
#ifndef UBFIX
  return 0;
#endif
}

/// Choose the outside entrance table or the shared interior entrances.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_FuchinTemple1) {
    return gFuchinTempleEntrances1;
  }
  return gFuchinTempleEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gFuchinTempleExits; }

/// Select the room's actors and apply the post-lesson placement changes.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_FuchinTemple1) {
    return gFuchinTemplePlacements1;
  }
  if (gGameState.entrance == 3) {
    return gFuchinTemplePlacementsEntrance3;
  }
  // Once Hsu is known to be trapped, the tree and two students are gone, and
  // another student stands elsewhere.
  if (FLAG_CHECK(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
    gFuchinTemplePlacements[5].condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
    gFuchinTemplePlacements[7].condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
    gFuchinTemplePlacements[8].x = 0x1200000;
    gFuchinTemplePlacements[8].z = 0xf80000;
    gFuchinTemplePlacements[11].condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
    gFuchinTemplePlacements[12].condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
  }
  FieldScene_PrepareActors(gFuchinTemplePlacements);
  return gFuchinTemplePlacements;
}

/// Talking to the visitor from Kolima outside the temple: he asks how the
/// party got here. "How did you get here? Is the bridge open again?" Yes
/// cheers him ("Then I can go home to Kolima now?") and is remembered; no
/// means the party came through the forest. Later talks go on to the tale of
/// the forest.
static void Event_VisitorAsksAboutBridge(void) {
  EV_BEGIN();
  MESSAGE_SET(MsgXianHowDidGetHereBridge);
  if (FLAG_CHECK(FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN) != 0) {
    gEventWork->message += 4;
  }
  Event_OpenMessage(8, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    FLAG_SET(FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN);
  } else {
    gEventWork->message++;
  }
  Event_ShowMessage(8, 0);
  EV_END();
}

/// Talking to the monk (actor 12) outside the temple: he speaks of the one
/// seeking enlightenment, with up to two more questions. "Isn't it noble of him
/// to try to save the world by becoming enlightened?" Once Nyunpa is gone,
/// "Now, he truly is beyond the world's concerns. Is he watching over us?"
static void Event_MonkSpeaksOfNyunpa(void) {

  EV_BEGIN();
  if (Engine_GameFlagIsSet(FLAG_NYUNPA_ENLIGHTENED) != 0) {
    MESSAGE_SET(MsgXianNowHeTrulyBeyondWorlds);
    Engine_EventAskYesNo(12, 0);
    EV_END();
  } else {
    MESSAGE_SET(MsgXianIsntNobleHimTrySave);
    MESSAGE_OPEN(12, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      gEventWork->message++;
      MESSAGE_OPEN(12, 0);
      if (Engine_EventChooseYesNo(0, 0) == 1) {
        gEventWork->message++;
      }
    }
    MESSAGE_PUT(12, 0);
    EV_END();
  }
}

/// Talking to the monk beneath the falls once Nyunpa has offered the test: he
/// lets the party pass. "I have already heard of your test from Master Nyunpa.
/// You may enter." He leaps aside, and the way into the waterfall grotto opens.
static void Event_MonkLetsPartyIntoGrotto(void) {
  struct FieldActor *monk;
  struct FieldActor *movingMonk;
  struct EffectOptions options;

  monk = Object_GetById(FALLS_MONK);
  EV_BEGIN();
  MESSAGE_SET(MsgXianAlreadyHeardTest);
  Engine_EventShowMessageAndWait(FALLS_MONK, 0, 20);
  // The monk leaps aside and lands in a puff of dust.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 392);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Engine_AudioPlayCue(SE_132);
  movingMonk = Object_GetById(FALLS_MONK);
  movingMonk->velocityY = 0x140000;
  movingMonk = Object_GetById(FALLS_MONK);
  movingMonk->gravity = 0x40000;
  OBJ_SPEED(FALLS_MONK, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetDestination(FALLS_MONK, 152, 392);
  Engine_ActorWaitForMove(FALLS_MONK);
  movingMonk = Object_GetById(FALLS_MONK);
  movingMonk->gravity = 0x10000;
  Engine_ActorFaceDirection(FALLS_MONK, FACING_EAST, 0);
  Engine_AudioPlayCue(SE_132);
  // Only the palette option is enabled for these three particles.
  options.palette = 7;
  Effect_Spawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + 0x40000), 0x8000,
               0, 0, EFFECT_USE_PALETTE, &options);
  Effect_Spawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + 0x40000), 0, 0, 0,
               EFFECT_USE_PALETTE, &options);
  Effect_Spawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + 0x40000), -0x8000,
               0, 0, EFFECT_USE_PALETTE, &options);
  WAIT(30);
  BattleFx_PlayQueuedSound();
  Map_CopyCellAttributes(10, 24, 1, 1, 10, 22);
  Engine_GameFlagSet(FLAG_FUCHIN_FALLS_GROTTO_OPENED);
  EV_END();
}

/// Coming back out of the grotto (trigger 5), Isaac is sent on to Nyunpa and
/// the way back is open again. "Young master, did you complete the test?" Then
/// "Master Nyunpa is waiting for you... Proceed to the temple."
static void Event_MonkAsksWhetherTestIsDone(void) {
  EV_BEGIN();
  Engine_GameFlagSet(FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST);
  OBJ_FACE(FALLS_MONK, ACTOR_PARTY_LEADER, 0);
  WAIT(10);
  MESSAGE_SET(MsgXianYoungMasterDidCompleteTest);
  Engine_ActorRunRepeatedMotion(FALLS_MONK, 2);
  WAIT(20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Engine_EventAskYesNo(FALLS_MONK, 0);
  WAIT(10);
  OBJ_EMOTE(FALLS_MONK, EMOTE_IN_FRONT, 80);
  OBJ_DIR(FALLS_MONK, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(FALLS_MONK, 2);
  WAIT(20);
  Engine_EventShowMessageAndWait(FALLS_MONK, 0, 20);
  Engine_ActorFaceDirection(FALLS_MONK, FACING_EAST, 20);
  Engine_ActorSetAnimationAndWait(FALLS_MONK, ANIM_NOD);
  WAIT(20);
  Engine_EventShowMessageAndWait(FALLS_MONK, 0, 20);
  Map_CopyCellAttributes(10, 26, 1, 1, 10, 24);
  EV_END();
}

/// Talking to Nyunpa, or reading his mind: he reads the party's mind in turn,
/// guesses that they follow Saturos's group, warns of Mogall Forest and offers
/// the test of the grotto. "Mmmm... Who is that? Who speaks to my mind?"
/// Accepting it, he sends Isaac to the monk beneath the falls; refusing is
/// remembered, and the next talk asks again.
static void Event_NyunpaOffersTest(void) {
  s32 alreadyDeclinedTrial;
  struct FieldActor *nyunpa;

  EV_BEGIN();
  // Revisit an earlier refusal, or introduce the offer through Mind Read.
  alreadyDeclinedTrial = FLAG_CHECK(FLAG_NYUNPA_TEST_DECLINED);
  if (alreadyDeclinedTrial != 0) {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 504);
    WAIT(5);
    OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    Object_GetById(NYUNPA)->paused = 0;
    SE(SE_JUMP);
    nyunpa = Actor_Get(NYUNPA);
    nyunpa->velocityY = 0x80000;
    OBJ_MODE(NYUNPA, ANIM_STAND);
    WAIT(30);
    MESSAGE_SET(MsgXianMustSurviveTestGrottoBefore);
  } else {
    MESSAGE_SET(MsgXianMmmmWhoWhoSpeaksMy);
    FuchinTemple_BeginMindReadDemonstration(ACTOR_PARTY_LEADER, NYUNPA);
    WAIT(30);
    Event_ShowMessage(NYUNPA, 0);
    FuchinTemple_FinishMindReadDemonstration();
    WAIT(20);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 504);
    WAIT(5);
    OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    SE(SE_JUMP);
    Object_GetById(NYUNPA)->paused = alreadyDeclinedTrial;
    nyunpa = Actor_Get(NYUNPA);
    nyunpa->velocityY = 0x80000;
    OBJ_MODE(NYUNPA, ANIM_STAND);
    WAIT(30);
    Event_OpenMessage(NYUNPA, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      Engine_ActorRunRepeatedMotion(NYUNPA, 2);
      WAIT(20);
      Event_ShowMessage(NYUNPA, 0);
      WAIT(20);
      FuchinTemple_BeginMindReadDemonstration(NYUNPA, ACTOR_PARTY_LEADER);
      WAIT(30);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      WAIT(50);
      FuchinTemple_FinishMindReadDemonstration();
      WAIT(30);
      Engine_ActorSetAnimationAndWait(NYUNPA, ANIM_NOD);
      Event_ShowMessage(NYUNPA, 0);
    } else {
      gEventWork->message += 2;
      Event_ShowMessage(NYUNPA, 0);
    }
    Engine_ActorSetAnimationAndWait(NYUNPA, ANIM_NOD);
    WAIT(30);
    OBJ_EMOTE(NYUNPA, EMOTE_IN_FRONT, 60);
    MESSAGE_SET(MsgXianFollowThemDoNot);
    Event_OpenMessage(NYUNPA, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      OBJ_EMOTE(NYUNPA, 0x105, 60);
      FuchinTemple_BeginMindReadDemonstration(NYUNPA, ACTOR_PARTY_LEADER);
      WAIT(30);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      WAIT(50);
      FuchinTemple_FinishMindReadDemonstration();
      WAIT(30);
      Event_ShowMessage(NYUNPA, 0);
      gEventWork->message += 1;
    } else {
      gEventWork->message += 1;
      WAIT(20);
      Engine_ActorSetAnimationAndWait(NYUNPA, ANIM_NOD);
      WAIT(20);
      Event_ShowMessage(NYUNPA, 0);
    }
    WAIT(20);
    Engine_ActorSetAnimationAndWait(NYUNPA, ANIM_SHAKE_HEAD);
    WAIT(20);
    Event_ShowMessage(NYUNPA, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    WAIT(20);
    Event_ShowMessage(NYUNPA, 0);
    WAIT(20);
    OBJ_DIR(NYUNPA, FACING_NORTH, 20);
    OBJ_SPEED(NYUNPA, 3 * PIXELS(1) / 10, 3 * PIXELS(1) / 20);
    Actor_WalkToAndWait(NYUNPA, 168, 464);
    WAIT(60);
    OBJ_DIR(NYUNPA, FACING_SOUTH, 40);
    Event_ShowMessageAndWait(NYUNPA, 0, 10);
    OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x102, 60);
    Actor_WalkToAndWait(NYUNPA, 168, 472);
  }
  Event_OpenMessage(NYUNPA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    MESSAGE_SET(MsgXianThenCannotTellGiveUp);
    Event_ShowMessage(NYUNPA, 0);
    FLAG_SET(FLAG_NYUNPA_TEST_DECLINED);
  } else {
    // "Do not worry. You will be permitted to enter the grotto." Nyunpa
    // glows as he calls to the monk.
    MESSAGE_SET(MsgXianDoNotWorryWillPermitted);
    WAIT(30);
    Engine_ActorSetAnimationAndWait(NYUNPA, ANIM_NOD);
    WAIT(20);
    Camera_SetSpeed(0x8000, 0x1000);
    Engine_CameraMoveToActor(NYUNPA, 1);
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x10003, 1);
    Engine_ColorBufferInterpolate(30);
    Engine_EventWaitForScreen();
    Engine_CameraWaitForMove();
    FuchinTemple_SpawnNyunpasParticles();
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(30);
    Event_ShowMessage(NYUNPA, 0);
    Engine_ActorRunRepeatedMotion(NYUNPA, 2);
    WAIT(20);
    Event_ShowMessage(NYUNPA, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    WAIT(60);
    Event_ShowMessageAndWait(NYUNPA, 0, 10);
    FLAG_SET(FLAG_NYUNPA_TEST_ACCEPTED);
  }
  OBJ_MODE(NYUNPA, 5);
  EV_END();
}

/// Reading Nyunpa's mind once the test is accepted: he notices.
/// "You enjoy reading the minds of others, do you not?"
static void Event_NyunpaFeelsMindRead(void) {
  EV_BEGIN();
  MESSAGE_SET(MsgXianEnjoyReadingMindsOthersDo);
  Engine_EventAskYesNo(NYUNPA, 0);
  EV_END();
}

/// Talking to the student at the door (actor 9): the student asks whether the
/// party came to watch. "This is Master Feh's school... You came to watch?"
static void Event_StudentWelcomesToSchool(void) {
  EV_BEGIN();
  MESSAGE_SET(MsgXianMasterFehsSchoolCameWatch);
  Engine_EventAskYesNo(9, 0);
  EV_END();
}

/// Talking to the student who practises jumping: he lands, talks about jumping
/// (or Lama Temple, or Hsu), then jumps on.
static void Event_TalkToJumpingStudent(void) {
  EV_BEGIN();

  Object_GetById(JUMPING_STUDENT)->paused = 0;

  while (Object_GetById(JUMPING_STUDENT)->y.fixed > 0) {
    Engine_TaskWait(1);
  }

  Object_GetById(JUMPING_STUDENT)->y.fixed = 0;

  Object_GetById(JUMPING_STUDENT)->targetY = ACTOR_NO_TARGET;

  Object_GetById(JUMPING_STUDENT)->velocityY = 0;

  Object_GetById(JUMPING_STUDENT)->paused = 1;

  OBJ_FACE(JUMPING_STUDENT, ACTOR_PARTY_LEADER, 0);

  if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) != 0) {
    MESSAGE_SET(MsgXianHsuDidNotPracticeJumping);
  } else if (Engine_GameFlagIsSet(FLAG_FEH_SAW_CHI) != 0) {
    MESSAGE_SET(MsgXianLamaTempleFarWestIn);
  } else {
    MESSAGE_SET(MsgXianFlexibilityJumpingVeryImportantIn);
  }

  MESSAGE_PUT(JUMPING_STUDENT, 0);

  Object_GetById(JUMPING_STUDENT)->facing = 128 << 7;

  Object_GetById(JUMPING_STUDENT)->paused = 0;

  Actor_SetActionScript(JUMPING_STUDENT, gFuchinTemple_JumpingStudentScript);
  EV_END();
}

/// Talking to the student by the training tree (actor 15): the student asks
/// whether the party still wishes to see Master Feh. "Master Feh is very busy.
/// Do you still wish to see him?" Yes: if the tree falls to Chi from the line,
/// he will take the party to Feh.
static void Event_StudentAsksAboutFeh(void) {
  EV_BEGIN();
  MESSAGE_SET(MsgXianMasterFehVeryBusyDo);
  Engine_EventAskYesNo(15, 0);
  EV_END();
}

/// Rotate the training tree through its westward arc and scatter particles.
static void FuchinTemple_ToppleTreeWest(void) {
  struct FieldActor *tree = Object_GetById(TRAINING_TREE);
  u32 turnStep;
  s32 angle;

  for (turnStep = 8; turnStep > 3; turnStep--) {
    angle = turnStep << 12;
    tree->sprite->rotation = angle;
    Engine_TaskWait((turnStep - 4) * 2);
    tree->x.fixed += Engine_MathCos(angle) * 6;
    tree->z.fixed += Engine_MathSin(angle) * 6;
  }

  tree->y.fixed = 0x120000;
  tree->targetY = 0x120000;

  Engine_AudioPlayCue(SE_227);

  Effect_Spawn(tree->x.fixed - 0xc0000, tree->y.fixed, tree->z.fixed + 0x80000,
               0xffffcccd, 0x6666, 0, 0, 0);
  Effect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0x80000,
               0xffff3334, 0x4ccc, 0, 0, 0);
  Effect_Spawn(tree->x.fixed + 0xa0000, tree->y.fixed, tree->z.fixed + 0x80000,
               0xffff0000, 0x3333, 0, 0, 0);
}

/// Rotate the training tree through the opposite arc and reverse its horizontal
/// scale.
static void FuchinTemple_ToppleTreeEast(void) {
  struct FieldActor *tree;
  u32 turnStep;
  s32 angle;

  tree = Object_GetById(TRAINING_TREE);
  for (turnStep = 8; turnStep <= 12; turnStep++) {
    angle = turnStep << 12;
    tree->sprite->rotation = angle;
    Engine_TaskWait((12 - turnStep) * 2);
    tree->x.fixed -= Engine_MathCos(angle) * 6;
    tree->z.fixed -= Engine_MathSin(angle) * 6;
  }
  tree->y.fixed = 0x120000;
  tree->targetY = 0x120000;
  tree->scaleX = 0xffff3334;
  Engine_AudioPlayCue(SE_227);
  Effect_Spawn(tree->x.fixed - 0xc0000, tree->y.fixed, tree->z.fixed + 0x80000,
               0x10000, 0x3333, 0, 0, 0);
  Effect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0x80000, 0xcccc,
               0x4ccc, 0, 0, 0);
  Effect_Spawn(tree->x.fixed + 0xa0000, tree->y.fixed, tree->z.fixed + 0x80000,
               0x3333, 0x6666, 0, 0, 0);
}

/// Slide the training tree north in four steps, then scatter particles.
static void FuchinTemple_ToppleTreeNorth(void) {
  struct FieldActor *tree = Object_GetById(TRAINING_TREE);
  u32 stepIndex = 0;
  s32 delay = 8;
  u8 rotation;
  do {
    Engine_TaskWait(delay);
    tree->z.fixed += 0xffff0000;
    tree->targetZ = ACTOR_NO_TARGET;
    stepIndex++;
    delay -= 2;
  } while (stepIndex < 4);
  rotation = 0;
  tree->sprite->rotation = rotation;
  Engine_AudioPlayCue(SE_227);
  Effect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0xfff80000,
               0xffff3334, 0, 0xffffcccd, 0, 0);
  Effect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0xfff80000,
               0x0000cccc, 0, 0xffffcccd, 0, 0);
  Effect_Spawn(tree->x.fixed + 0xfffa0000, tree->y.fixed,
               tree->z.fixed + (160 << 12), 0x00003333, 0, 0xffff0000, 0, 0);
  Effect_Spawn(tree->x.fixed + (192 << 11), tree->y.fixed,
               tree->z.fixed + (160 << 12), 0x00003333, 0, 0xffff0000, 0, 0);
}

/// Slide the training tree south in four steps before the final displacement.
static void FuchinTemple_ToppleTreeSouth(void) {
  struct FieldActor *tree;
  u32 stepIndex;
  s32 particleSpeed;
  s32 delay;

  tree = Object_GetById(TRAINING_TREE);
  stepIndex = 0;
  delay = 8;
  do {
    Engine_TaskWait(delay);
    tree->z.fixed += 0x10000;
    tree->targetZ = ACTOR_NO_TARGET;
    stepIndex++;
    delay -= 2;
  } while (stepIndex < 4);

  tree->sprite->rotation = 0;
  tree->z.fixed += 0x180000;
  tree->targetZ = ACTOR_NO_TARGET;
  Engine_AudioPlayCue(SE_227);

  Effect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0xc0000, -0xcccc,
               0, 0x3333, 0, 0);
  particleSpeed = 0x3333;
  Effect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0xc0000, 0xcccc, 0,
               particleSpeed, 0, 0);
  Effect_Spawn(tree->x.fixed - 0x60000, tree->y.fixed, tree->z.fixed - 0x80000,
               particleSpeed, 0, 0x10000, 0, 0);
  Effect_Spawn(tree->x.fixed + 0x60000, tree->y.fixed, tree->z.fixed - 0x80000,
               particleSpeed, 0, 0x10000, 0, 0);
}

/// Route the student around the tree according to the leader's facing.
static void FuchinTemple_StudentWalksAroundTree(void) {
  u32 leaderFacing;
  u32 quadrantLimit;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  quadrantLimit = 0x3fff;
  if (((leaderFacing + -0x2000) << 16) <= 0x3fff0000) {
    Engine_ActorWalkToAndWait(15, 216, 168);
    Engine_ActorWalkToAndWait(15, 224, 168);
    OBJ_DIR(15, FACING_SOUTHEAST, 20);
  } else if ((u16)(leaderFacing - 0x6000) <= quadrantLimit) {
    Engine_ActorWalkToAndWait(15, 232, 160);
    OBJ_DIR(15, FACING_SOUTH + FACING_STEP, 20);
  } else if ((u16)(leaderFacing + 0x6000) <= quadrantLimit) {
    Engine_ActorWalkToAndWait(15, 216, 168);
    Engine_ActorWalkToAndWait(15, 224, 172);
    OBJ_DIR(15, FACING_NORTHEAST, 20);
  } else {
    Engine_ActorWalkToAndWait(15, 232, 160);
    OBJ_DIR(15, FACING_SOUTHEAST, 20);
  }
}

/// The student by the tree explains the rules: mode 0 when Psynergy felled it
/// but not from the line, 1 when Isaac pushed it by hand, 2 to praise a
/// warrior Feh has already seen. Then he sets the tree up again.
static void FuchinTemple_StudentExplainsChiRules(s32 mode) {
  s32 rulesMessage;

  OBJ_SPEED(15, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  WAIT(60);
  rulesMessage = MsgXianCannotPushHands;
  MESSAGE_SET(rulesMessage);
  if (mode == 0) {
    MESSAGE_SET(rulesMessage - 1);
    OBJ_EMOTE(15, 0x101, 60);
    Engine_EventShowMessageAndWait(15, 0, 20);
    Engine_ActorRunRepeatedMotion(15, 2);
    MESSAGE_SET(MsgXianWarriorsCannotUse);
    Engine_EventShowMessageAndWait(15, 0, 20);
    Engine_ActorSetAnimationAndWait(15, 4);
    WAIT(20);
    Engine_EventShowMessageAndWait(15, 0, 20);
    Engine_ActorSetAnimationAndWait(15, 3);
    WAIT(20);
  }
  if (mode == 2) {
    MESSAGE_SET(MsgXianGreatWarriorTrain);
    Engine_ActorRunRepeatedMotion(15, 2);
    WAIT(20);
  }
  Engine_EventShowMessageAndWait(15, 0, 20);
  // Restore the tree and let the student return to the demonstration spot.
  FuchinTemple_StudentWalksAroundTree();
  Engine_ActorRunRepeatedMotion(15, 3);
  OBJ_POS_SET(TRAINING_TREE, 232, 168);
  Engine_ActorSetPosition(TRAINING_TREE_TARGET, PIXELS(232), PIXELS(168));
  Object_GetById(TRAINING_TREE)->y.fixed = 0xc0000;
  Object_GetById(TRAINING_TREE)->targetY = ACTOR_NO_TARGET;
  Object_GetById(TRAINING_TREE)->scaleX = 0xcccc;
  Object_GetById(TRAINING_TREE)->sprite->rotation = 0x8000;
  Engine_AudioPlayCue(SE_124);
  WAIT(40);
  Engine_ActorWalkToAndWait(15, 216, 152);
  OBJ_DIR(15, FACING_SOUTHEAST, 30);
}

/// The students see the tree fall, Feh doubts it came from the line, and the
/// student sets the tree up again. "Look! The tree fell."
static void FuchinTemple_StudentsSeeTreeFall(void) {
  struct FieldActor *tree;

  OBJ_FACE(13, TRAINING_TREE, 0);
  OBJ_FACE(14, TRAINING_TREE, 0);
  OBJ_FACE(15, TRAINING_TREE, 0);
  OBJ_FACE(16, TRAINING_TREE, 0);
  OBJ_FACE(FEH, TRAINING_TREE, 0);
  WAIT(20);
  Engine_ActorRunRepeatedMotion(15, 2);
  WAIT(20);
  MESSAGE_SET(MsgXianLookTreeFell);
  Engine_EventShowMessageAndWait(15, 0, 20);
  Engine_EventShowMessageAndWait(16, 0, 20);
  OBJ_EMOTE(FEH, 0x105, 60);
  OBJ_EMOTE(16, 0x101, 60);
  Engine_EventShowMessageAndWait(16, 0, 20);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_SHAKE_HEAD);
  WAIT(20);
  Engine_EventShowMessageAndWait(FEH, 0, 20);
  OBJ_EMOTE(16, 0x102, 60);
  Engine_EventShowMessageAndWait(16, 0, 20);
  Engine_ActorFaceEachOther(15, FEH, 0);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_NOD);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(15, 3);
  WAIT(20);
  // Reset the tree after the group has reacted to the demonstration.
  OBJ_SPEED(15, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  FuchinTemple_StudentWalksAroundTree();
  Engine_ActorRunRepeatedMotion(15, 3);
  OBJ_POS_SET(TRAINING_TREE, 232, 168);
  Engine_ActorSetPosition(TRAINING_TREE_TARGET, PIXELS(232), PIXELS(168));
  tree = Object_GetById(TRAINING_TREE);
  tree->y.fixed = 0xc0000;
  tree = Object_GetById(TRAINING_TREE);
  tree->targetY = ACTOR_NO_TARGET;
  tree = Object_GetById(TRAINING_TREE);
  tree->scaleX = 0xcccc;
  tree = Object_GetById(TRAINING_TREE);
  {
    struct AnimationObject *sprite = tree->sprite;
    s32 rotation = 0x8000;

    sprite->rotation = rotation;
  }
  Engine_AudioPlayCue(SE_124);
  WAIT(40);
  Engine_ActorWalkToAndWait(15, 216, 152);
  OBJ_DIR(15, FACING_SOUTH, 30);
  Engine_GameFlagSet(FLAG_FEH_SCHOOL_SAW_TREE_FALL);
}

/// Move on the training tree: it topples, and the students react.
static void Event_MoveTree(void) {
  EV_BEGIN();

  if (Object_GetById(ACTOR_PARTY_LEADER)->facing > (128 << 7) &&
      Object_GetById(ACTOR_PARTY_LEADER)->facing < (192 << 8)) {
    FuchinTemple_ToppleTreeWest();
  } else {
    FuchinTemple_ToppleTreeEast();
  }

  if (Engine_GameFlagIsSet(FLAG_FEH_WAITS_TO_SEE_CHI) != 0) {
    FuchinTemple_StudentsSeeTreeFall();
  } else {
    FuchinTemple_StudentExplainsChiRules(0);
  }

  EV_END();
}

/// Walking into the training tree: Isaac pushes it over by hand. "You cannot
/// push it with your hands. Attack it with Chi."
static void Event_PushTreeByHand(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  u16 facing;

  EV_BEGIN();
  OBJ_MODE(ACTOR_PARTY_LEADER, 8);
  WAIT(20);

  facing = leader->facing;

  if ((u16)(facing - 0x2000) <= 0x3fffu) {
    FuchinTemple_ToppleTreeSouth();
  } else if ((u16)(facing - 0x6000) <= 0x3fffu) {
    FuchinTemple_ToppleTreeWest();
  } else if ((u16)(facing + (192 << 7)) <= 0x3fffu) {
    FuchinTemple_ToppleTreeNorth();
  } else {
    FuchinTemple_ToppleTreeEast();
  }

  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  FuchinTemple_StudentExplainsChiRules(1);
  EV_END();
}

/// Force on the training tree: it topples. From too close it does not count;
/// from the line, the students call Feh out the first time, and later Feh
/// himself sees it.
static void Event_ForceTree(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  u16 facing;

  EV_BEGIN();

  facing = leader->facing;
  if ((u16)(facing - 0x2000) <= 0x3fff) {
    FuchinTemple_ToppleTreeSouth();
  } else if ((u16)(facing - 0x6000) <= 0x3fff) {
    FuchinTemple_ToppleTreeWest();
  } else if ((u16)(facing + 0x6000) <= 0x3fff) {
    FuchinTemple_ToppleTreeNorth();
  } else {
    FuchinTemple_ToppleTreeEast();
  }

  Engine_CameraSetSpeed(0x10000, 0x2000);
  Engine_CameraMoveToActor(TRAINING_TREE_TARGET, 1);
  Engine_CameraWaitForMove();

  if (leader->z.part.pixel <= 209) {
    if (Engine_GameFlagIsSet(FLAG_FEIZHI_WENT_TO_HSU) == 0 ||
        Engine_GameFlagIsSet(FLAG_FEH_SAW_CHI) != 0) {
      FuchinTemple_StudentExplainsChiRules(0);
    } else {
      FuchinTemple_StudentsSeeTreeFall();
    }
    EV_END();
    return;
  }

  if (Engine_GameFlagIsSet(FLAG_FEH_SAW_CHI) != 0) {
    FuchinTemple_StudentExplainsChiRules(2);
  } else if (Engine_GameFlagIsSet(FLAG_FEIZHI_WENT_TO_HSU) == 0) {
    FuchinTemple_FeizhiLeavesForHsu();
  } else {
    FuchinTemple_FehSeesChi();
  }
  EV_END();
}

/// The first time the tree falls from the line, a student runs for Master Feh.
/// "What did you do, warrior?" The door opens on its own and Feizhi storms
/// out, sure that Hsu is in danger. Feh will not believe her visions, and she
/// leaves alone to find Hsu. Feh then asks Isaac to show him his Chi.
static void FuchinTemple_FeizhiLeavesForHsu(void) {
  struct FieldActor *actor;

  FLAG_SET(FLAG_FEIZHI_WENT_TO_HSU);
  WAIT(30);
  Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(15, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(16, ACTOR_PARTY_LEADER, 0);
  WAIT(20);
  OBJ_EMOTE(13, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(15, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(16, EMOTE_IN_FRONT, 0);
  WAIT(60);
  MESSAGE_SET(MsgXianDidDoWarrior);
  Event_ShowMessageAndWait(13, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
  Engine_ActorRunRepeatedMotion(15, 1);
  WAIT(20);
  Event_ShowMessageAndWait(15, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 15, 0);
  Engine_ActorRunRepeatedMotion(16, 2);
  WAIT(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 16, 0);
  Event_AskYesNo(16, 0);
  WAIT(50);
  // Bring the observers out through the school door.
  Engine_CameraFollowActor(16, 1);
  OBJ_SPEED(16, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(16, 176, 248);
  Actor_WalkToAndWait(16, 308, 248);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  OBJ_DIR(16, FACING_NORTH, 20);
  SE(SE_DOOR_OPEN);
  Map_AnimateCells(gFuchinTemple_CellStepsB, 78, 13);
  Engine_ActorRunRepeatedMotion(16, 2);
  WAIT(20);
  OBJ_SPEED(16, 3 * PIXELS(1) / 2, 3 * PIXELS(1) / 4);
  actor = Object_GetById(16);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(16, 308, 272);
  WAIT(1);
  actor = Actor_Get(16);

  actor->steeringFlags = ACTOR_FACES_MOTION | actor->steeringFlags;
  Event_ShowMessageAndWait(16, 0, 50);
  OBJ_POS_SET(FEIZHI, 304, 216);
  Actor_WalkToAndWait(FEIZHI, 304, 248);
  Actor_FaceActor(9, FEIZHI, 0);
  Actor_FaceActor(10, FEIZHI, 0);
  Actor_FaceActor(11, FEIZHI, 0);
  Actor_FaceActor(JUMPING_STUDENT, FEIZHI, 0);
  Actor_FaceActor(13, FEIZHI, 0);
  Actor_FaceActor(14, FEIZHI, 0);
  Actor_FaceActor(15, FEIZHI, 0);
  Actor_FaceActor(16, FEIZHI, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, FEIZHI, 0);
  WAIT(10);
  Engine_ActorStartRepeatedMotion(9, 2);
  Engine_ActorStartRepeatedMotion(10, 2);
  Engine_ActorStartRepeatedMotion(11, 2);
  Engine_ActorStartRepeatedMotion(JUMPING_STUDENT, 2);
  Engine_ActorStartRepeatedMotion(13, 2);
  Engine_ActorStartRepeatedMotion(14, 2);
  Engine_ActorStartRepeatedMotion(15, 2);
  Engine_ActorRunRepeatedMotion(16, 2);
  OBJ_EMOTE(FEIZHI, 0x103, 60);
  OBJ_POS_SET(FEH, 304, 216);
  OBJ_WALK(FEH, 304, 248);
  OBJ_WALK(FEIZHI, 280, 264);
  Engine_ActorWaitForMove(FEH);
  OBJ_DIR(FEH, FACING_SOUTH + FACING_STEP, 0);
  Engine_ActorWaitForMove(FEIZHI);
  SE(SE_159);
  Map_AnimateCells(gFuchinTemple_CellStepsC, 78, 13);
  Event_ShowMessageAndWait(FEH, 0, 20);
  Actor_FaceActor(9, FEIZHI, 0);
  Actor_FaceActor(10, FEIZHI, 0);
  Actor_FaceActor(11, FEIZHI, 0);
  Actor_FaceActor(JUMPING_STUDENT, FEIZHI, 0);
  Actor_FaceActor(13, FEIZHI, 0);
  Actor_FaceActor(14, FEIZHI, 0);
  Actor_FaceActor(15, FEIZHI, 0);
  Actor_FaceActor(16, FEIZHI, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, FEIZHI, 0);
  WAIT(10);
  Engine_ActorRunRepeatedMotion(FEIZHI, 2);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  Engine_ActorSetAnimationAndWait(FEIZHI, ANIM_NOD);
  WAIT(20);
  Engine_ActorRunRepeatedMotion(FEH, 1);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  OBJ_DIR(FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(FEIZHI, 0, 20);
  OBJ_EMOTE(FEH, 0x102, 60);
  Event_ShowMessageAndWait(FEH, 0, 20);
  Engine_ActorSetAnimationAndWait(FEIZHI, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(FEIZHI, 0, 20);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  Engine_ActorSetAnimationAndWait(FEIZHI, ANIM_NOD);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  Engine_ActorRunRepeatedMotion(FEIZHI, 2);
  WAIT(20);
  OBJ_DIR(FEIZHI, FACING_EAST, 20);
  Event_ShowMessageAndWait(FEIZHI, 0, 20);
  OBJ_DIR(16, FACING_WEST, 20);
  Engine_ActorSetAnimationAndWait(16, 3);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(FEIZHI, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(FEIZHI, 0, 20);
  Engine_ActorSetAnimationAndWait(16, 3);
  WAIT(20);
  Event_ShowMessageAndWait(16, 0, 20);
  OBJ_DIR(FEIZHI, FACING_WEST, 20);
  Event_ShowMessageAndWait(FEIZHI, 0, 20);
  Engine_ActorSetAnimationAndWait(9, 3);
  WAIT(20);
  Event_ShowMessageAndWait(9, 0, 20);
  OBJ_DIR(FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(FEIZHI, 1);
  WAIT(20);
  Event_ShowMessageAndWait(FEIZHI, 0, 20);
  OBJ_EMOTE(FEH, 0x102, 60);
  Event_ShowMessageAndWait(FEH, 0, 20);
  OBJ_EMOTE(FEIZHI, 0x101, 60);
  Event_ShowMessageAndWait(FEIZHI, 0, 20);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  OBJ_EMOTE(FEIZHI, EMOTE_IN_FRONT, 60);
  Event_ShowMessageAndWait(FEIZHI, 0, 20);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_SHAKE_HEAD);
  WAIT(20);
  OBJ_EMOTE(FEIZHI, 0x103, 60);
  Event_ShowMessageAndWait(FEIZHI, 0, 20);
  OBJ_EMOTE(FEH, EMOTE_IN_FRONT, 60);
  Event_ShowMessageAndWait(FEH, 0, 20);
  Engine_ActorSetAnimationAndWait(FEIZHI, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(FEIZHI, 0, 20);
  Engine_ActorRunRepeatedMotion(FEH, 2);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  Engine_ActorRunRepeatedMotion(FEIZHI, 2);
  WAIT(10);
  Actor_WalkToAndWait(FEIZHI, 256, 280);
  OBJ_DIR(FEIZHI, FACING_SOUTH, 20);
  OBJ_POS_SET(FEIZHI, 0, 0);
  Engine_ActorDestroy(FEIZHI);
  WAIT(30);
  Engine_ActorRunRepeatedMotion(9, 2);
  WAIT(20);
  Event_ShowMessageAndWait(9, 0, 20);
  Engine_ActorRunRepeatedMotion(15, 2);
  WAIT(20);
  Event_ShowMessageAndWait(15, 0, 20);
  Actor_FaceActor(16, FEH, 0);
  WAIT(20);
  Engine_ActorRunRepeatedMotion(16, 2);
  WAIT(20);
  Event_ShowMessageAndWait(16, 0, 20);
  Actor_FaceActor(FEH, 16, 0);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  OBJ_SPEED(FEH, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(FEH, 256, 248);
  OBJ_DIR(FEH, FACING_NORTH, 20);
  Engine_ActorStartRepeatedMotion(FEH, 1);
  OBJ_EMOTE(FEH, EMOTE_IN_FRONT, 60);
  Actor_WalkToAndWait(FEH, 240, 184);
  Engine_ActorRunRepeatedMotion(FEH, 2);
  WAIT(20);
  OBJ_POS_SET(TRAINING_TREE, 232, 168);
  OBJ_POS_SET(TRAINING_TREE_TARGET, 232, 168);
  // Restore the tree before leaving the group ready for another demonstration.
  actor = Actor_Get(TRAINING_TREE);
  actor->y.fixed = 0xc0000;
  actor = Actor_Get(TRAINING_TREE);
  actor->targetY = ACTOR_NO_TARGET;
  actor = Actor_Get(TRAINING_TREE);
  actor->scaleX = 0xcccc;
  actor = Actor_Get(TRAINING_TREE);
  {
    struct AnimationObject *sprite = actor->sprite;
    s32 rotation = 0x8000;

    sprite->rotation = rotation;
  }
  SE(SE_124);
  Event_ShowMessageAndWait(FEH, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Actor_WalkToAndWait(16, 256, 240);
  OBJ_DIR(16, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(16, 1);
  Event_ShowMessageAndWait(16, 0, 20);
  Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(JUMPING_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(15, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(16, ACTOR_PARTY_LEADER, 0);
  Engine_ActorRunRepeatedMotion(FEH, 2);
  WAIT(20);
  OBJ_DIR(FEH, FACING_SOUTH + FACING_STEP, 20);
  Actor_WalkToAndWait(FEH, 248, 208);
  OBJ_DIR(FEH, FACING_SOUTH + FACING_STEP, 20);
  Event_OpenMessage(FEH, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorRunRepeatedMotion(16, 1);
    WAIT(20);
    Event_ShowMessageAndWait(16, 0, 20);
    FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI);
  } else {
    Event_SkipMessages(1);
    WAIT(20);
    OBJ_EMOTE(FEH, 0x105, 60);
    OBJ_DIR(FEH, FACING_SOUTH, 20);
    Engine_ActorRunRepeatedMotion(16, 2);
    WAIT(20);
    Event_OpenMessage(16, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_ActorSetAnimationAndWait(16, 3);
      WAIT(20);
      OBJ_DIR(FEH, FACING_NORTHWEST + FACING_STEP, 20);
      Event_ShowMessageAndWait(FEH, 0, 20);
      FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI);
    } else {
      Event_SkipMessages(1);
      WAIT(20);
      Engine_ActorSetAnimationAndWait(FEH, ANIM_SHAKE_HEAD);
      WAIT(20);
      Event_ShowMessageAndWait(FEH, 0, 20);
      FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED);
    }
  }
  OBJ_DIR(10, FACING_WEST, 0);
  OBJ_DIR(11, FACING_WEST, 20);
  OBJ_MODE(10, 5);
  OBJ_MODE(11, 5);
  Actor_SetActionScript(JUMPING_STUDENT, gFuchinTemple_JumpingStudentScript);
}

/// Talking to Feh after the party declined: he asks again to see Isaac's Chi.
/// "Warrior, will you show me your Chi?"
static void Event_FehAsksToSeeChi(void) {

  EV_BEGIN();
  MESSAGE_SET(MsgXianWarriorWillShowMeYour);
  MESSAGE_OPEN(FEH, 0);

  if (Engine_EventChooseYesNo(0, 0) == 0) {
    WAIT(20);
    Engine_EventShowMessageAndWait(FEH, 0, 20);
    Engine_GameFlagSet(FLAG_FEH_WAITS_TO_SEE_CHI);
    EV_END();
  } else {
    gEventWork->message += 1;
    Engine_EventShowMessageAndWait(FEH, 0, 20);
    EV_END();
  }
}

/// The tree falls from the line with Feh watching, but Feh sees that it is not
/// Chi. "Ya! The tree fell!" He asks whether the party has been to Fuchin
/// Temple, and the school goes back to its training.
static void FuchinTemple_FehSeesChi(void) {
  struct FieldActor *actor;

  // Stop the temporary facing hooks while the group reacts to the tree.
  actor = Actor_Get(FEH);
  actor->update = 0;
  actor = Object_GetById(13);
  actor->update = 0;
  actor = Object_GetById(14);
  actor->update = 0;
  actor = Object_GetById(15);
  actor->update = 0;
  actor = Actor_Get(16);
  actor->update = 0;
  OBJ_MODE(11, 1);
  Camera_SetSpeed(0x8000, 0x1000);
  Camera_MoveTo(PIXELS(232), -1, PIXELS(200), 1);
  Engine_CameraWaitForMove();
  // Continue the Chi discussion with the students gathered around the tree.
  MESSAGE_SET(MsgXianYaTreeFell);
  OBJ_SPEED(10, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJ_SPEED(JUMPING_STUDENT, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJ_WALK(10, 152, 200);
  Actor_WalkToAndWait(JUMPING_STUDENT, 144, 248);
  Engine_ActorWaitForMove(10);
  Actor_FaceActor(9, TRAINING_TREE, 0);
  Actor_FaceActor(11, TRAINING_TREE, 0);
  Actor_FaceActor(13, TRAINING_TREE, 0);
  Actor_FaceActor(14, TRAINING_TREE, 0);
  Actor_FaceActor(15, TRAINING_TREE, 0);
  Actor_FaceActor(16, TRAINING_TREE, 0);
  Actor_FaceActor(FEH, TRAINING_TREE, 0);
  OBJ_SPEED(10, 3 * PIXELS(1) / 2, 3 * PIXELS(1) / 4);
  OBJ_SPEED(JUMPING_STUDENT, PIXELS(2), PIXELS(1));
  OBJ_WALK(10, 152, 200);
  Actor_WalkToAndWait(JUMPING_STUDENT, 144, 248);
  Actor_FaceActor(JUMPING_STUDENT, TRAINING_TREE, 0);
  Engine_ActorWaitForMove(10);
  Actor_FaceActor(10, TRAINING_TREE, 0);
  Engine_ActorRunRepeatedMotion(FEH, 2);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 40);
  Actor_FaceActor(9, FEH, 0);
  Actor_FaceActor(10, FEH, 0);
  OBJ_DIR(11, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceActor(JUMPING_STUDENT, FEH, 0);
  OBJ_DIR(13, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceActor(14, FEH, 0);
  Actor_FaceActor(15, FEH, 0);
  Actor_FaceActor(16, FEH, 0);
  WAIT(20);
  Event_ShowMessageAndWait(16, 0, 20);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_NOD);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(16, 3);
  WAIT(20);
  Event_ShowMessageAndWait(16, 0, 20);
  OBJ_EMOTE(FEH, 0x105, 60);
  OBJ_EMOTE(16, 0x101, 60);
  Event_ShowMessageAndWait(16, 0, 20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  OBJ_EMOTE(16, 0x102, 60);
  OBJ_EMOTE(15, 0x101, 60);
  OBJ_SPEED(15, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(15, 216, 176);
  OBJ_DIR(15, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(15, 0, 20);
  OBJ_DIR(FEH, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  Engine_ActorStartRepeatedMotion(9, 2);
  Engine_ActorStartRepeatedMotion(10, 2);
  Engine_ActorStartRepeatedMotion(11, 2);
  Engine_ActorStartRepeatedMotion(JUMPING_STUDENT, 2);
  Engine_ActorStartRepeatedMotion(13, 2);
  Engine_ActorStartRepeatedMotion(14, 2);
  Engine_ActorStartRepeatedMotion(15, 2);
  Engine_ActorStartRepeatedMotion(16, 2);
  WAIT(40);
  Engine_ActorRunRepeatedMotion(13, 2);
  Event_ShowMessageAndWait(13, 0, 20);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  OBJ_DIR(FEH, FACING_SOUTH + FACING_STEP, 20);
  Event_AskYesNo(FEH, 0);
  OBJ_EMOTE(9, 0x101, 0);
  WAIT(5);
  OBJ_EMOTE(10, 0x101, 0);
  WAIT(5);
  OBJ_EMOTE(11, 0x101, 0);
  WAIT(5);
  OBJ_EMOTE(JUMPING_STUDENT, 0x101, 0);
  WAIT(5);
  OBJ_EMOTE(13, 0x101, 0);
  WAIT(5);
  OBJ_EMOTE(14, 0x101, 0);
  WAIT(5);
  OBJ_EMOTE(15, 0x101, 0);
  WAIT(5);
  OBJ_EMOTE(16, 0x101, 0);
  WAIT(60);
  Engine_ActorRunRepeatedMotion(16, 2);
  Event_ShowMessageAndWait(16, 0, 20);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  OBJ_EMOTE(15, 0x101, 60);
  Event_ShowMessageAndWait(15, 0, 20);
  Actor_FaceActor(FEH, 15, 0);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 40);
  Engine_ActorFaceEachOther(11, 10, 0);
  Engine_ActorFaceEachOther(JUMPING_STUDENT, 14, 0);
  Engine_ActorFaceEachOther(13, 15, 0);
  WAIT(60);
  Actor_FaceActor(10, FEH, 0);
  Actor_FaceActor(11, FEH, 0);
  Actor_FaceActor(JUMPING_STUDENT, FEH, 0);
  Actor_FaceActor(13, FEH, 0);
  Actor_FaceActor(14, FEH, 0);
  Actor_FaceActor(15, FEH, 0);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  Engine_ActorRunRepeatedMotion(FEH, 2);
  WAIT(20);
  OBJ_MODE(9, 3);
  OBJ_MODE(10, 3);
  OBJ_MODE(11, 3);
  OBJ_MODE(JUMPING_STUDENT, ANIM_NOD);
  OBJ_MODE(13, 3);
  OBJ_MODE(14, 3);
  OBJ_MODE(15, 3);
  Engine_ActorSetAnimationAndWait(16, 3);
  WAIT(20);
  OBJ_DIR(FEH, FACING_SOUTH + FACING_STEP, 20);
  Event_AskYesNo(FEH, 0);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(9, 3);
  OBJ_MODE(10, 3);
  OBJ_MODE(11, 3);
  OBJ_MODE(JUMPING_STUDENT, ANIM_NOD);
  OBJ_MODE(13, 3);
  OBJ_MODE(14, 3);
  OBJ_MODE(15, 3);
  Engine_ActorSetAnimationAndWait(16, 3);
  WAIT(20);
  Engine_ActorRunRepeatedMotion(FEH, 2);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(FEH, ANIM_NOD);
  WAIT(20);
  OBJ_DIR(FEH, FACING_WEST, 20);
  Engine_ActorRunRepeatedMotion(FEH, 2);
  WAIT(20);
  Event_ShowMessageAndWait(FEH, 0, 20);
  OBJ_MODE(9, 3);
  OBJ_MODE(10, 3);
  OBJ_MODE(11, 3);
  OBJ_MODE(JUMPING_STUDENT, ANIM_NOD);
  OBJ_MODE(13, 3);
  OBJ_MODE(14, 3);
  OBJ_MODE(15, 3);
  Engine_ActorSetAnimationAndWait(16, 3);
  WAIT(20);
  OBJ_WALK(10, 120, 200);
  OBJ_WALK(JUMPING_STUDENT, 120, 248);
  Engine_ActorWaitForMove(10);
  OBJ_DIR(11, FACING_WEST, 20);
  OBJ_MODE(10, 5);
  OBJ_MODE(11, 5);
  Engine_ActorWaitForMove(JUMPING_STUDENT);
  Actor_SetActionScript(JUMPING_STUDENT, gFuchinTemple_JumpingStudentScript);
  OBJ_SPEED(15, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(15, 216, 168);
  Actor_WalkToAndWait(15, 232, 168);
  OBJ_DIR(15, FACING_NORTH, 20);
  Engine_ActorRunRepeatedMotion(15, 3);
  OBJ_POS_SET(TRAINING_TREE, 232, 168);
  actor = Actor_Get(TRAINING_TREE);
  actor->y.fixed = 0xc0000;
  actor = Actor_Get(TRAINING_TREE);
  actor->targetY = ACTOR_NO_TARGET;
  actor = Actor_Get(TRAINING_TREE);
  {
    struct AnimationObject *sprite = actor->sprite;
    s32 rotation = 0x8000;

    sprite->rotation = rotation;
  }
  SE(SE_124);
  WAIT(40);
  Actor_WalkToAndWait(15, 216, 152);
  OBJ_DIR(15, FACING_SOUTH, 30);
  GameFlag_Clear(FLAG_FEH_WAITS_TO_SEE_CHI);
  FLAG_SET(FLAG_FEH_SAW_CHI);
}

/// Talking to a student (actor 11) once Hsu is known to be trapped: the
/// student asks about the flood. "Water monsters flooded Altin. Did you defeat
/// them, warrior?"
static void Event_StudentAsksAboutAltin(void) {
  EV_BEGIN();
  MESSAGE_SET(MsgXianWaterMonstersFloodedAltinDid);
  Engine_EventAskYesNo(11, 0);
  EV_END();
}

/// Walking north into the door Feh and Feizhi came out of (trigger 3): it is
/// locked until Feizhi has left; then it opens and Isaac walks through. "It is
/// locked."
static void Event_OpenLockedDoor(void) {
  EV_BEGIN();
  if (FLAG_CHECK(FLAG_FEIZHI_WENT_TO_HSU) == 0 &&
      FLAG_CHECK(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
    Engine_MessageShowCentered(MsgXianItIsLocked, 1);
    EV_END();
  } else {
    SE(SE_DOOR_OPEN);
    Map_AnimateCells(gFuchinTemple_CellStepsB, 78, 13);
    OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 306, 248);
    OBJ_WALK(ACTOR_PARTY_LEADER, 304, 216);
    WAIT(20);
    Engine_EventRequestExit(4);
    EV_END();
  }
}

/// Talking to the healer: across the counter from the south, the Sanctum's
/// services; from elsewhere, a word on the healer's travels. "I'm traveling
/// around the world to spread good will."
static void Event_TalkToHealer(void) {
  struct FieldActor *leader;
  u32 facing;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  facing = leader->facing;
  EV_BEGIN();

  if (FACING_IS_NORTH(facing)) {
    Engine_SanctumOpen(13);
  } else {
    MESSAGE_SET(MsgXianImTravelingAroundWorldSpread);
    MESSAGE_PUT(HEALER, 0);
  }

  EV_END();
}

/// Reading Nyunpa's mind once he is enlightened: he entrusts the world to
/// Isaac. "Isaac... Is that you...? I am... counting on you... to bring
/// peace... to the world..."
static void Event_ReadNyunpasLastWish(void) {
  EV_BEGIN();
  Engine_ActorRunRepeatedMotion(NYUNPA, 2);
  MESSAGE_SET(MsgXianIsaacAmCountingOnBring);
  MESSAGE_PUT(NYUNPA, 0);
  EV_END();
}

/// Select the outdoor, school, or lesson event table.
const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_FuchinTemple1) {
    return gFuchinTempleEvents1;
  }
  if (gGameState.entrance == 3) {
    return gFuchinTempleEventsEntrance3;
  }
  return gFuchinTempleEvents;
}

/// Emit a falling particle beside the leader during the grotto entrance fade.
static void FuchinTemple_SpawnGrottoEntryParticle(void) {
  struct EffectOptions options;
  struct FieldActor *leader;
  s32 paletteDraw;
  s32 fallSpeed;

  leader = Object_GetById(ACTOR_PARTY_LEADER);

  options.palette = 7;
  paletteDraw = (u32)(Engine_RandomNext() * 7) >> 16;
  if ((paletteDraw & 7) == 0)
    options.palette = 5;

  options.startScaleX = 0xb333;
  options.startScaleY = 0xcccc;

  fallSpeed = ((u32)(Engine_RandomNext() * 8) >> 16) * 13107;

  Effect_Spawn(leader->x.fixed + ((8 - (gFrameCount & 15)) << 16),
               leader->y.fixed + (192 << 13), leader->z.fixed, 0, -fallSpeed, 0,
               144 << 12, &options);

  if ((gFrameCount & 1) != 0)
    Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  else
    Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 1);
}

/// Hop the leader one map cell north.
static UNUSED void FuchinTemple_HopLeaderNorth(void) {
  FuchinTemple_HopLeaderByOffset(0, -32);
}

/// Hop the leader one map cell south.
static UNUSED void FuchinTemple_HopLeaderSouth(void) {
  FuchinTemple_HopLeaderByOffset(0, 32);
}

/// Hop the leader one map cell west.
static UNUSED void FuchinTemple_HopLeaderWest(void) {
  FuchinTemple_HopLeaderByOffset(-32, 0);
}

/// Hop the leader by a pixel offset, waiting for the movement to finish.
static UNUSED void FuchinTemple_HopLeaderByOffset(s32 offsetX, s32 offsetZ) {
  EV_BEGIN();
  OBJ_SPEED(ACTOR_PARTY_LEADER, 5 * PIXELS(1) / 2, 5 * PIXELS(1) / 4);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, offsetX, offsetZ);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, 7);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_MODE(ACTOR_PARTY_LEADER, 6);
  EV_END();
}

/// Stepping into the grotto's entrance (trigger 3): spray falls over Isaac as
/// the screen fades, and he enters the grotto.
static void Event_EnterWaterfallGrotto(void) {
  struct FieldActor *leader;
  SchedulerCallback particleTask;

  EV_BEGIN();
  // Keep the entry effect alive until the leader has moved into the fade.
  particleTask = FuchinTemple_SpawnGrottoEntryParticle;
  FieldScheduler_AddCallback(particleTask, 0xc80);
  Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
  gEventWork->transitionFrames = 60;
  Engine_EventCloseScreen();
  Engine_AudioPlayCue(SE_CAST);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_WALK);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(leader, 0);
  Scheduler_RemoveCallback(particleTask);
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(3);
  EV_END();
}

/// Leaving by the first exit.
static void Event_LeaveTemple(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(1);
}

/// Walking north into the temple door (trigger 2): it opens, and Isaac walks
/// into Nyunpa's hall.
static void Event_EnterNyunpasHall(void) {
  EV_BEGIN();
  SE(SE_188);
  Map_AnimateCells(gFuchinTemple_CellStepsA, 77, 8);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  OBJ_SPEED(ACTOR_PARTY_LEADER, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  WAIT(16);
  Engine_EventRequestExit(2);
  EV_END();
}

/// Back from the grotto, Nyunpa praises Isaac. "Excellent, Isaac! I am glad to
/// see you master the secrets of Fuchin Temple!" Nyunpa names the power Ki,
/// "force" in the western world, and says it will show the monsters hiding in
/// Mogall Forest.
static void FuchinTemple_NyunpaTeachesKi(void) {
  struct FieldActor *nyunpa;

  EV_BEGIN();
  OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJ_WALK(ACTOR_PARTY_LEADER, 168, 504);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  WAIT(20);
  Engine_ActorStartRepeatedMotion(NYUNPA, 2);
  Actor_SetAttachedEffect(NYUNPA, 0x102);
  WAIT(60);
  Object_GetById(NYUNPA)->paused = 0;
  SE(SE_JUMP);
  nyunpa = Actor_Get(NYUNPA);
  nyunpa->velocityY = 0x80000;
  OBJ_MODE(NYUNPA, ANIM_STAND);
  // Praise the completed test before answering the follow-up questions.
  MESSAGE_SET(MsgXianExcellentIsaac);
  Event_ShowMessageAndWait(NYUNPA, 0, 20);
  Event_ShowMessageAndWait(NYUNPA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(NYUNPA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(NYUNPA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x101, 60);
  Event_OpenMessage(NYUNPA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    WAIT(10);
    Engine_ActorSetAnimationAndWait(NYUNPA, ANIM_NOD);
    WAIT(20);
    Event_ShowMessageAndWait(NYUNPA, 0, 20);
    gEventWork->message += 2;
  } else {
    WAIT(10);
    Engine_ActorRunRepeatedMotion(NYUNPA, 2);
    WAIT(20);
    gEventWork->message++;
    Event_ShowMessageAndWait(NYUNPA, 0, 20);
    Engine_ActorSetAnimationAndWait(NYUNPA, ANIM_NOD);
    WAIT(20);
    Event_ShowMessageAndWait(NYUNPA, 0, 20);
  }
  Event_ShowMessageAndWait(NYUNPA, 0, 20);
  Engine_ActorRunRepeatedMotion(NYUNPA, 2);
  WAIT(20);
  Event_ShowMessageAndWait(NYUNPA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x101, 60);
  Event_OpenMessage(NYUNPA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    WAIT(10);
    OBJ_EMOTE(NYUNPA, 0x102, 60);
    MESSAGE_SET(MsgXianCanReadMindsKnowCurious);
    Event_OpenMessage(NYUNPA, 0);
    do {
      if (Engine_EventChooseYesNo(0, 0) != 1) {
        break;
      }
      WAIT(10);
      OBJ_EMOTE(NYUNPA, 0x102, 60);
      MESSAGE_SET(MsgXianDontTryHard);
      Event_OpenMessage(NYUNPA, 0);
    } while (1);
  }
  // Explain why hidden monsters make the new ability useful.
  MESSAGE_SET(MsgXianMonstersWaitInHidingWould);
  WAIT(10);
  Engine_ActorSetAnimationAndWait(NYUNPA, ANIM_NOD);
  WAIT(20);
  Event_OpenMessage(NYUNPA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    WAIT(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    WAIT(20);
    Event_ShowMessageAndWait(NYUNPA, 0, 20);
    gEventWork->message++;
  } else {
    WAIT(10);
    Engine_ActorRunRepeatedMotion(NYUNPA, 2);
    gEventWork->message++;
    Event_ShowMessageAndWait(NYUNPA, 0, 20);
  }
  Engine_ActorSetAnimationAndWait(NYUNPA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(NYUNPA, 0, 20);
  Engine_ActorRunRepeatedMotion(NYUNPA, 2);
  WAIT(20);
  Event_ShowMessageAndWait(NYUNPA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(NYUNPA, ANIM_NOD);
  WAIT(20);
  OBJ_MODE(NYUNPA, 5);
  FLAG_SET(FLAG_NYUNPA_TAUGHT_FORCE);
  EV_END();
}

/// Entering Hama's room, the party is greeted. "So, Isaac, you and your Adept
/// companions have finally come." Hama tells Ivan he is a Jupiter Adept like
/// her, teaches him Reveal for crossing Lamakan Desert, and tells of Hsu,
/// trapped on Silk Road.
static void FuchinTemple_RunHamasRevealLesson(void) {
  struct FieldActor *leader;

  EV_BEGIN();
  OBJ_SPEED(ACTOR_PARTY_LEADER, 2 * PIXELS(1) / 5, PIXELS(1) / 5);
  OBJ_WALK(ACTOR_PARTY_LEADER, 472, 536);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_DIR(9, FACING_EAST, 20);
  Engine_ActorRunRepeatedMotion(9, 2);
  WAIT(20);
  MESSAGE_SET(MsgXianMasterHamaMeditatingPleaseExtremely);
  Event_ShowMessageAndWait(9, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  WAIT(30);
  OBJ_DIR(HAMA, FACING_SOUTH, 30);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  WAIT(20);
  OBJ_DIR(9, FACING_NORTH + FACING_STEP, 0);
  // Bring the other party members out from the leader for the lesson.
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
  }
  OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJ_SPEED(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJ_SPEED(IVAN, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJ_SPEED(MIA, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJ_WALK(ACTOR_PARTY_LEADER, 464, 504);
  OBJ_WALK(IVAN, 480, 504);
  OBJ_WALK(GARET, 496, 496);
  OBJ_WALK(MIA, 448, 496);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorWaitForMove(IVAN);
  Engine_ActorWaitForMove(MIA);
  OBJ_DIR(MIA, FACING_NORTHEAST, 0);
  Engine_ActorWaitForMove(GARET);
  OBJ_DIR(GARET, FACING_NORTHWEST, 0);
  WAIT(20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x101, 0);
  OBJ_EMOTE(GARET, 0x101, 0);
  OBJ_EMOTE(IVAN, 0x101, 0);
  OBJ_EMOTE(MIA, 0x101, 60);
  OBJ_DIR(GARET, FACING_SOUTH + FACING_STEP, 20);
  Event_AskYesNo(GARET, 0);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_DIR(GARET, FACING_NORTHWEST, 20);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT, 60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_NOD);
  OBJ_DIR(HAMA, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  OBJ_DIR(GARET, FACING_SOUTHWEST, 0);
  OBJ_DIR(MIA, FACING_SOUTHEAST, 0);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Actor_SetAttachedEffect(IVAN, 0x102);
  WAIT(60);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(GARET, FACING_NORTHWEST, 0);
  OBJ_DIR(MIA, FACING_NORTHEAST, 0);
  WAIT(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  WAIT(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IVAN, 50);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(IVAN, FACING_NORTH, 30);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_EMOTE(MIA, 0x101, 60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  OBJ_DIR(HAMA, FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  OBJ_DIR(GARET, FACING_SOUTHWEST, 0);
  WAIT(30);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(GARET, FACING_NORTHWEST, 20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Actor_FaceActor(HAMA, IVAN, 0);
  WAIT(10);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_FaceActor(MIA, IVAN, 0);
  OBJ_EMOTE(IVAN, 0x102, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  WAIT(20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  WAIT(20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  OBJ_EMOTE(HAMA, EMOTE_IN_FRONT, 60);
  Actor_FaceActor(HAMA, GARET, 0);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_FaceActor(HAMA, GARET, 0);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(GARET, FACING_NORTHWEST, 0);
  OBJ_DIR(MIA, FACING_NORTHEAST, 0);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  WAIT(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  WAIT(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  OBJ_DIR(HAMA, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Engine_ActorRunRepeatedMotion(HAMA, 2);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_EMOTE(GARET, 0x103, 0);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  WAIT(60);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x101, 0);
  OBJ_EMOTE(GARET, 0x101, 60);
  Event_ShowMessageAndWait(GARET, 0, 20);
  OBJ_EMOTE(HAMA, 0x102, 0);
  Engine_ActorStartRepeatedMotion(HAMA, 1);
  WAIT(60);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_EMOTE(GARET, 0x101, 60);
  OBJ_DIR(GARET, FACING_SOUTH + FACING_STEP, 20);
  Event_AskYesNo(GARET, 0);
  WAIT(20);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  WAIT(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  OBJ_DIR(GARET, FACING_NORTHWEST, 20);
  OBJ_DIR(HAMA, FACING_NORTH, 20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  WAIT(20);
  OBJ_DIR(HAMA, FACING_SOUTH, 20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_EMOTE(IVAN, 0x101, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Actor_SetAttachedEffect(MIA, 0x102);
  WAIT(60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  OBJ_DIR(HAMA, FACING_SOUTH + FACING_STEP, 20);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x101, 0);
  OBJ_EMOTE(GARET, 0x101, 0);
  OBJ_EMOTE(IVAN, 0x101, 0);
  OBJ_EMOTE(MIA, 0x101, 60);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  WAIT(20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, GARET, 0);
  Actor_FaceActor(IVAN, GARET, 0);
  Actor_FaceActor(MIA, GARET, 0);
  WAIT(20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  WAIT(60);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(IVAN, FACING_NORTH, 0);
  OBJ_DIR(MIA, FACING_NORTH + FACING_STEP, 0);
  WAIT(20);
  Event_AskYesNo(HAMA, 0);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x102, 0);
  OBJ_EMOTE(GARET, 0x102, 0);
  OBJ_EMOTE(IVAN, 0x102, 0);
  OBJ_EMOTE(MIA, 0x102, 0);
  WAIT(60);
  OBJ_DIR(GARET, FACING_SOUTHWEST, 20);
  Event_AskYesNo(GARET, 0);
  WAIT(20);
  OBJ_DIR(MIA, FACING_SOUTHEAST, 20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  OBJ_DIR(GARET, FACING_NORTHWEST, 0);
  OBJ_DIR(MIA, FACING_NORTHEAST, 30);
  OBJ_DIR(HAMA, FACING_EAST, 20);
  OBJ_EMOTE(HAMA, 0x105, 60);
  OBJ_EMOTE(IVAN, 0x101, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  OBJ_DIR(HAMA, FACING_NORTH, 20);
  OBJ_EMOTE(HAMA, 0x105, 60);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x101, 0);
  OBJ_EMOTE(GARET, 0x101, 0);
  OBJ_EMOTE(IVAN, 0x101, 0);
  OBJ_EMOTE(MIA, 0x101, 60);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x101, 0);
  OBJ_EMOTE(GARET, 0x101, 0);
  OBJ_EMOTE(IVAN, 0x101, 0);
  OBJ_EMOTE(MIA, 0x101, 60);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  WAIT(20);
  Engine_ActorRunRepeatedMotion(HAMA, 1);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  OBJ_DIR(GARET, FACING_SOUTHWEST, 0);
  OBJ_DIR(IVAN, FACING_WEST, 0);
  OBJ_DIR(MIA, FACING_SOUTHEAST, 0);
  WAIT(60);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(GARET, FACING_NORTHWEST, 0);
  OBJ_DIR(IVAN, FACING_NORTH, 0);
  OBJ_DIR(MIA, FACING_NORTHEAST, 0);
  WAIT(30);
  OBJ_DIR(HAMA, FACING_SOUTH, 20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  // Interrupt the discussion with the arrival from the southern path.
  OBJ_POS_SET(10, 472, 608);
  Event_ShowMessageAndWait(10, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(HAMA, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(9, EMOTE_IN_FRONT, 0);
  WAIT(60);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  OBJ_DIR(GARET, FACING_SOUTH, 0);
  OBJ_DIR(IVAN, FACING_SOUTH, 0);
  OBJ_DIR(MIA, FACING_SOUTH, 0);
  OBJ_DIR(9, FACING_EAST, 0);
  WAIT(30);
  OBJ_SPEED(10, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(10, 472, 536);
  OBJ_EMOTE(HAMA, 0x101, 60);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_DIR(IVAN, FACING_NORTH, 40);
  Actor_WalkToAndWait(IVAN, 488, 512);
  WAIT(10);
  OBJ_DIR(IVAN, FACING_SOUTHWEST, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 30);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 512);
  WAIT(10);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
  Engine_ActorRunRepeatedMotion(10, 2);
  WAIT(20);
  Event_ShowMessageAndWait(10, 0, 20);
  OBJ_EMOTE(HAMA, 0x101, 60);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Engine_ActorStartRepeatedMotion(10, 2);
  Actor_SetAttachedEffect(10, 0x102);
  WAIT(60);
  Event_ShowMessageAndWait(10, 0, 20);
  OBJ_EMOTE(HAMA, 0x101, 60);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_EMOTE(10, EMOTE_IN_FRONT, 60);
  Event_ShowMessageAndWait(10, 0, 20);
  Engine_ActorStartRepeatedMotion(HAMA, 1);
  OBJ_EMOTE(HAMA, EMOTE_IN_FRONT, 60);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  OBJ_DIR(GARET, FACING_NORTHWEST, 0);
  OBJ_DIR(IVAN, FACING_NORTHWEST, 0);
  OBJ_DIR(MIA, FACING_NORTHEAST, 0);
  WAIT(40);
  Engine_ActorSetAnimationAndWait(10, 3);
  WAIT(20);
  Event_ShowMessageAndWait(10, 0, 20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_EMOTE(HAMA, 0x101, 60);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Engine_ActorStartRepeatedMotion(17, 1);
  WAIT(20);
  Event_ShowMessageAndWait(10, 0, 20);
  OBJ_EMOTE(HAMA, 0x105, 60);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_EMOTE(HAMA, EMOTE_IN_FRONT, 30);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  WAIT(2);
  OBJ_MODE(IVAN, ANIM_NOD);
  WAIT(1);
  OBJ_MODE(MIA, ANIM_NOD);
  WAIT(5);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Actor_FaceActor(HAMA, ACTOR_PARTY_LEADER, 0);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_SPEED(10, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(10, 472, 504);
  WAIT(20);
  Engine_ActorStartRepeatedMotion(10, 1);
  WAIT(20);
  Event_ShowMessageAndWait(10, 0, 20);
  Actor_FaceActor(HAMA, 10, 0);
  OBJ_SPEED(HAMA, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  OBJ_DIR(GARET, FACING_SOUTHWEST, 0);
  OBJ_DIR(IVAN, FACING_SOUTHWEST, 0);
  OBJ_DIR(MIA, FACING_SOUTHEAST, 0);
  Engine_ActorWaitForMove(HAMA);
  WAIT(30);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_WALK(10, 472, 568);
  Actor_WalkToAndWait(HAMA, 472, 536);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_ActorWaitForMove(10);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorStartRepeatedMotion(HAMA, 2);
  OBJ_EMOTE(HAMA, 0x102, 60);
  OBJ_DIR(10, FACING_NORTH + FACING_STEP, 0);
  OBJ_DIR(HAMA, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Actor_WalkToAndWait(HAMA, 472, 512);
  OBJ_DIR(HAMA, FACING_EAST, 0);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  OBJ_DIR(IVAN, FACING_WEST, 0);
  OBJ_DIR(9, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  WAIT(20);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 448, 512);
  WAIT(1);
  {
    struct FieldActor *leaderActor = Actor_Get(ACTOR_PARTY_LEADER);
    leaderActor->steeringFlags =
        ACTOR_FACES_MOTION | leaderActor->steeringFlags;
  }
  WAIT(20);
  // Finish Ivan's lesson before adding Reveal to his ability list.
  FuchinTemple_AnimateRevealLesson();
  WAIT(60);
  Party_LearnAbility(IVAN, ABILITY_REVEAL);
  // Let Hama and her attendant leave, then gather the party into the leader.
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  WAIT(20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  OBJ_DIR(GARET, FACING_SOUTHWEST, 0);
  OBJ_DIR(IVAN, FACING_SOUTHWEST, 0);
  OBJ_DIR(MIA, FACING_SOUTHEAST, 0);
  OBJ_DIR(9, FACING_SOUTHEAST + FACING_STEP, 0);
  WAIT(20);
  Actor_WalkToAndWait(HAMA, 472, 552);
  Engine_ActorRunRepeatedMotion(HAMA, 2);
  WAIT(20);
  OBJ_DIR(HAMA, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  Engine_ActorSetAnimationAndWait(HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_SPEED(HAMA, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(HAMA, 480, 540);
  Event_ShowMessageAndWait(HAMA, 0, 20);
  OBJ_SPEED(HAMA, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJ_SPEED(9, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJ_WALK(HAMA, 472, 608);
  OBJ_WALK(9, 472, 544);
  Actor_WalkToAndWait(10, 472, 608);
  Engine_ActorWaitForMove(9);
  OBJ_WALK(9, 472, 608);
  OBJ_POS_SET(10, 0, 0);
  Engine_ActorWaitForMove(HAMA);
  OBJ_POS_SET(HAMA, 0, 0);
  Engine_ActorWaitForMove(9);
  OBJ_POS_SET(9, 0, 0);
  Actor_WalkToAndWait(IVAN, 488, 520);
  OBJ_EMOTE(IVAN, 0x101, 60);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  WAIT(20);
  OBJ_DIR(MIA, FACING_SOUTHEAST, 20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  OBJ_DIR(IVAN, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  OBJ_EMOTE(MIA, 0x105, 60);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  OBJ_DIR(GARET, FACING_SOUTHWEST, 20);
  Event_AskYesNo(GARET, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  WAIT(20);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  WAIT(20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(GARET, FACING_WEST, 0);
  Event_ShowMessageAndWait(MIA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x101, 0);
  OBJ_EMOTE(GARET, 0x101, 60);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x102, 0);
  OBJ_EMOTE(GARET, 0x102, 60);
  Event_OpenMessage(MIA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    WAIT(20);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    WAIT(20);
    Event_ShowMessageAndWait(GARET, 0, 20);
    gEventWork->message++;
  } else {
    WAIT(20);
    gEventWork->message++;
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    WAIT(20);
    Event_ShowMessageAndWait(MIA, 0, 20);
  }
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Event_ShowMessageAndWait(GARET, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  WAIT(20);
  OBJ_WALK(GARET, 448, 512);
  Actor_WalkToAndWait(MIA, 448, 512);
  OBJ_POS_SET(MIA, 0, 0);
  Engine_ActorWaitForMove(GARET);
  OBJ_POS_SET(GARET, 0, 0);
  WAIT(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 20);
  WAIT(30);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x102, 60);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Actor_SetAttachedEffect(IVAN, 0x102);
  WAIT(60);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_WalkToAndWait(IVAN, 448, 512);
  OBJ_POS_SET(IVAN, 0, 0);
  EV_END();
  FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED);
}

/// Inside: Nyunpa meditates, or teaches Ki after the test; the school's tree
/// and students stand as the story left them; Hama teaches Reveal on the first
/// visit. Outside: the monk beneath the falls stands where the story left him.
s32 Scene_Initialize(void) {
  struct FieldActor *trainingTree;
  s32 heardHsuTrapped;

  if (gGameState.scene == (s32)&SceneId_FuchinTemple2) {
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (gGameState.entrance == 1) {
      if (Engine_GameFlagIsSet(FLAG_NYUNPA_ENLIGHTENED)) {
        OBJ_MODE(8, 6);
      } else {
        OBJ_MODE(8, 5);
        if (Engine_GameFlagIsSet(FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND) &&
            !Engine_GameFlagIsSet(FLAG_NYUNPA_TAUGHT_FORCE) &&
            !Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
          FuchinTemple_NyunpaTeachesKi();
        }
      }
    } else if (gGameState.entrance == 2 || gGameState.entrance == 4) {
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
      heardHsuTrapped = Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED);
      if (heardHsuTrapped == 0) {
        trainingTree = Object_GetById(19);
        trainingTree->motionFlags = heardHsuTrapped;
        trainingTree->y.fixed = 0xc0000;
        trainingTree->targetY = 0xc0000;
        trainingTree->scaleX = 0xcccc;
        trainingTree->scaleY = 0x8000;
        trainingTree->sprite->rotation = 0x8000;
        if (Engine_GameFlagIsSet(FLAG_FEIZHI_WENT_TO_HSU)) {
          Engine_ActorSetPosition(18, PIXELS(248), PIXELS(208));
          if (!Engine_GameFlagIsSet(FLAG_FEH_SAW_CHI)) {
            Engine_ActorSetPosition(16, PIXELS(256), PIXELS(240));
            Object_GetById(18)->update = FuchinTemple_FaceLeader;
            Object_GetById(13)->update = FuchinTemple_FaceLeader;
            Object_GetById(14)->update = FuchinTemple_FaceLeader;
            Object_GetById(15)->update = FuchinTemple_FaceLeader;
            Object_GetById(16)->update = FuchinTemple_FaceLeader;
          }
        }
      } else {
        trainingTree = Object_GetById(19);
        trainingTree->motionFlags = 0;
        trainingTree->y.fixed = 0xc0000;
        trainingTree->targetY = 0xc0000;
        trainingTree->scaleX = 0xcccc;
        trainingTree->scaleY = 0x8000;
        trainingTree->collisionFlags |= 8;
        trainingTree->sprite->rotation = 0x8000;
        Map_CopyCellAttributes(14, 11, 1, 1, 14, 10);
      }
      OverlayObject_PrepareObject(0x1300000, 0x180000, 0xe00000, 223);
      OBJ_MODE(10, 5);
      OBJ_MODE(11, 5);
    } else if (gGameState.entrance == 3) {
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
      if (!Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
        FuchinTemple_RunHamasRevealLesson();
      } else if (!Engine_GameFlagIsSet(FLAG_HSU_RESCUED)) {
        Engine_ActorSetPosition(8, 0, 0);
        Engine_ActorSetPosition(9, 0, 0);
      }
    }
  } else {
    BattleFx_SetQueuedSoundAndPlay(SE_170);
    Object_GetById(9)->collisionFlags |= 16;
    if (gGameState.entrance == 3 &&
        Engine_GameFlagIsSet(FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND) &&
        !Engine_GameFlagIsSet(FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST)) {
      Map_CopyCellAttributes(10, 84, 1, 1, 10, 24);
    }
    if (Engine_GameFlagIsSet(FLAG_FUCHIN_FALLS_GROTTO_OPENED)) {
      OBJ_POS_SET(9, 152, 392);
      Engine_ActorFaceDirection(9, FACING_EAST, 0);
      Map_CopyCellAttributes(10, 26, 1, 1, 10, 22);
    }
  }
  return 0;
}

/// Start the Mind Read demonstration for the requested caster and target.
static void FuchinTemple_BeginMindReadDemonstration(s32 caster, s32 target) {
  u8 *effectWork;

  Engine_GameFlagSet(160 << 1);
  Engine_PsynergyBegin(ABILITY_MIND_READ, 1);
  effectWork = gEffectWork;
  Engine_PsynergySetTarget(caster, target);
  effectWork[0x23] = 0;
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_TaskWait(1);
}

/// Finish the demonstration's effect and lower the caster's hands.
static void FuchinTemple_FinishMindReadDemonstration(void) {
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_PsynergyPlayEffect(2);
  Engine_PsynergyLowerHands();
}

/// Emit eight rising particles from Nyunpa, sounding every other shot.
static void FuchinTemple_SpawnNyunpasParticles(void) {
  struct EffectOptions options;
  struct FieldActor *nyunpa;
  u32 shotIndex;

  nyunpa = Object_GetById(NYUNPA);
  options.priority = 1;
  options.type = 0x0119;
  options.script = gFuchinTemple_ShotScript;
  options.targetScaleX = 224 << 10;
  options.targetScaleY = 192 << 9;
  for (shotIndex = 0; shotIndex < 8; shotIndex++) {
    WAIT(10);
    if (shotIndex & 1) {
      Engine_AudioPlayCue(SE_130);
    }
    Effect_Spawn(nyunpa->x.fixed, nyunpa->y.fixed, nyunpa->z.fixed + 0xffe80000,
                 0, 0x9999, 0, 0x00360001, &options);
  }
  WAIT(60);
}

/// Alternate the lesson's child effect from the frame counter, not player
/// input.
static void FuchinTemple_AlternateLessonChildEffect(union FieldObject *object) {
  if ((*(u32 *)&gFrameCount >> 1) & 1) {
    ObjectGroup_SetChildValue((struct FieldActor *)object, 10);
  } else {
    ObjectGroup_SetChildValue((struct FieldActor *)object, 9);
  }
}

/// Animate Hama and Ivan's lesson effects, fade the palette and clear the
/// callbacks.
static void FuchinTemple_AnimateRevealLesson(void) {
  struct EffectOptions options;
  struct FieldActor *hama;
  u32 particleIndex;
  s32 particleX;
  s32 particleY;
  s32 riseSpeed;

  Engine_AudioPlayCue(SE_131);
  Object_GetById(HAMA)->update = FuchinTemple_AlternateLessonChildEffect;
  WAIT(40);
  Engine_ColorBufferApplySource(128 << 9, 0);
  Engine_ColorBufferApplyTarget(0x205c54, 1);
  Engine_ColorBufferInterpolate(60);
  WAIT(40);
  Engine_AudioPlayCue(SE_131);
  Object_GetById(IVAN)->update = FuchinTemple_AlternateLessonChildEffect;
  WAIT(120);
  hama = Object_GetById(HAMA);
  // Emit the lesson particles while both temporary child effects run.
  options.priority = 1;
  options.palette = 2;
  options.type = 0x011d;
  for (particleIndex = 0; particleIndex < 64; particleIndex++) {
    if ((particleIndex & 3) == 0) {
      Engine_AudioPlayCue(SE_246);
    }
    particleX = hama->x.fixed +
                ((((u32)(Engine_RandomNext() * 3) << 4) >> 16) << 16) +
                0xfff40000;
    particleY = hama->y.fixed +
                ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + 0xfff00000;
    riseSpeed = ((((u32)Engine_RandomNext() << 2) >> 16) << 15) + (128 << 8);
    Effect_Spawn(particleX, particleY, hama->z.fixed, 0, riseSpeed, 0,
                 152 << 13, &options);
    Engine_TaskWait(2);
  }
  Engine_AudioPlayCue(SE_220);
  WAIT(30);
  // Fade back and remove both hooks before the conversation resumes.
  Engine_ColorBufferApplyTarget(128 << 9, 1);
  Engine_ColorBufferInterpolate(60);
  WAIT(40);
  Object_GetById(HAMA)->update = NULL;
  Object_GetById(IVAN)->update = NULL;
  Engine_ActorSetChildValue(HAMA, 0);
  Engine_ActorSetChildValue(IVAN, 0);
}

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gFuchinTemple_JumpingStudentScript[] = {
    SC_ARG(0),
    SC_SET(SW_GRAVITY, 0x9999),
    SC_LABEL(1),
    SC_SET(SW_VELOCITY_Y, 0x30000),
    SC_WAIT(30),
    SC_SET(SW_VELOCITY_Y, 0x40000),
    SC_WAIT(30),
    SC_SET(SW_VELOCITY_Y, 0x50000),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(2),
    SC_SET(SW_FACING, 0x5000),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(2),
    SC_SET(SW_FACING, 0xb000),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(2),
    SC_SET(SW_FACING, 0xd000),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(2),
    SC_SET(SW_FACING, 0x3000),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(2),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gFuchinTemple_HiddenSpriteScript[] = {
    SC_CALL(FuchinTemple_ClearScriptedActorSpriteFlags),
    SC_WAIT(1),
    SC_END,
};

u16 gFuchinTemple_CellStepsA[] = {
    MAP_CELL_STEP(66, 46, 3, 2, 5),
    MAP_CELL_STEP(63, 46, 3, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gFuchinTemple_CellStepsB[] = {
    MAP_CELL_STEP(62, 19, 2, 2, 5),
    MAP_CELL_STEP(64, 19, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gFuchinTemple_CellStepsC[] = {
    MAP_CELL_STEP(62, 19, 2, 2, 5),
    MAP_CELL_STEP(60, 19, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

struct SceneEntrance gFuchinTempleEntrances1[] = {
    {0, CONDITION_ALWAYS, 296, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 296, 0, 504, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 296, 0, 304, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 168, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 232, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gFuchinTempleEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 168, 0, 552, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 168, 0, 552, FACING_NORTH, 0, 48, 384, 288, 576, 0},
    {2, CONDITION_ALWAYS, 256, 0, 264, FACING_NORTH, 0, 48, 80, 368, 288, 0},
    {3, CONDITION_ALWAYS, 472, 0, 552, FACING_NORTH, 0, 352, 384, 592, 576, 0},
    {4, CONDITION_ALWAYS, 304, 0, 248, FACING_SOUTH, 0, 48, 80, 368, 288, 0},
    {SCENE_TABLE_END},
};

u32 gFuchinTempleExits[] = {
    SCENE_EXITS(60),       SCENE_EXIT(1, 2, 12), SCENE_EXIT(2, 61, 1),
    SCENE_EXIT(3, 62, 1),  SCENE_EXIT(0, 61, 0), SCENE_EXIT(1, 60, 2),
    SCENE_EXIT(2, 72, 10), SCENE_EXIT(3, 88, 2), SCENE_EXIT(4, 73, 8),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gFuchinTemplePlacements1[] = {
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(296), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x95, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x97, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x97, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(392), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x97, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gFuchinTemplePlacements[] = {
    // Nyunpa
    { 0x33, 0x0, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(472), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    // Master Feh's students
    { 0x6c, 0x0, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(264), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x82, 0x0, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(200), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x83, 0x0, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x83, 0x0, (s32)gFuchinTemple_JumpingStudentScript,
        PIXELS(120), 0, PIXELS(248), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x83, 0x0, ACTOR_STAND,
        PIXELS(176), 0, PIXELS(168), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x66, 0x0, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(264), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x82, 0x0, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(152), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x82, 0x0, ACTOR_STAND,
        PIXELS(176), 0, PIXELS(216), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    // Feizhi and Master Feh, out of sight until they come out
    { 0x27, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x28, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    // The training tree, and where Force strikes it
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x16, CONDITION_ALWAYS, (s32)gFuchinTemple_HiddenSpriteScript,
        PIXELS(232), 0, PIXELS(168), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gFuchinTemplePlacementsEntrance3[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    // Hama
    { 0x3d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(472), 0, PIXELS(472), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x97, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(440), 0, PIXELS(536), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x27, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gFuchinTempleEvents1[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterNyunpasHall) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterWaterfallGrotto) },
    { EVENT_TOUCH, 5, FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST, EVENT_SCRIPT(Event_MonkAsksWhetherTestIsDone) },
    { EVENT_TALK, 8, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleTheFuchinTempleIsAPleasant) },
    { EVENT_TALK, FALLS_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleOhIsaacWhatTookYouSo) },
    { EVENT_TALK, 10, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleAhhhMasterNyunpaHeHasPassed) },
    { EVENT_TALK, 11, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleMasterNyunpaHasWastedAwayAt) },
    { EVENT_TALK, 12, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), EVENT_SCRIPT(Event_MonkSpeaksOfNyunpa) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_VisitorAsksAboutBridge) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_VisitorAsksAboutBridge) },
    { EVENT_TALK, FALLS_MONK, FLAG_NYUNPA_TEST_ACCEPTED, MSGNO(MsgFuchinTempleOmOmBlueLightsInThe) },
    { EVENT_TALK, FALLS_MONK, FLAG_FUCHIN_FALLS_GROTTO_OPENED, EVENT_SCRIPT(Event_MonkLetsPartyIntoGrotto) },
    { EVENT_TALK, FALLS_MONK, FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND, MSGNO(MsgFuchinTempleTheTestOfFuchinFallsIs) },
    { EVENT_TALK, FALLS_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterNyunpaIsWaitingForYou) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleGemsFromTheSkyBlueLights) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleNyunpaHasNotEatenInDays) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MonkSpeaksOfNyunpa) },
    { EVENT_TALK, HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealer) },
    { SEARCH(SEARCH_UNNAMED), 110, 0x350, FIND_PSYNERGY_STONE },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleGemsFallingFromTheSkyMountains) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), FALLS_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleIKnewMasterNyunpaWasFasting) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleMasterNyunpaReachedEnlightenmentDuringHis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleMasterNyunpaIsNothingButSkin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleHeCantLeaveUsYetHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN, MSGNO(MsgFuchinTempleIBeginningToThinkIllBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIDontFeelSafeGoingHome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), FALLS_MONK, FLAG_NYUNPA_TEST_ACCEPTED, MSGNO(MsgFuchinTempleManyStarsFellFromTheSky) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), FALLS_MONK, FLAG_FUCHIN_FALLS_GROTTO_OPENED, MSGNO(MsgFuchinTempleCanSuchYoungWarriorsEndureThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), FALLS_MONK, FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND, MSGNO(MsgFuchinTempleCanSuchYoungWarriorsEndureThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), FALLS_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleTheWarriorsHaveMasteredTheSecrets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleThereIsNoPointInTraining) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleICantImagineFastingItGets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIWillTrainToBecomeA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), HEALER, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleSinceImHereIdLikeTo) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gFuchinTempleEvents[] = {
    { TOUCH_FACING(FACING_SOUTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveTemple) },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenLockedDoor) },
    { EVENT_TALK, NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTemple) },
    { EVENT_TALK, NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TAUGHT_FORCE), MSGNO(MsgFuchinTempleUseThisEnergyToFindThe) },
    { EVENT_TALK, NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TEST_ACCEPTED), MSGNO(MsgFuchinTempleSpeakToTheMonkStandingBeneath) },
    { EVENT_TALK, NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TEST_DECLINED), EVENT_SCRIPT(Event_NyunpaOffersTest) },
    { EVENT_TALK, NYUNPA, CONDITION_ALWAYS, MSGNO(MsgXianNyunpaMeditates) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), EVENT_SCRIPT(Event_ReadNyunpasLastWish) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TAUGHT_FORCE), MSGNO(MsgFuchinTempleNowThatIHavePassedMy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, IF_FLAG_SET(FLAG_NYUNPA_TEST_ACCEPTED), EVENT_SCRIPT(Event_NyunpaFeelsMindRead) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, CONDITION_ALWAYS, EVENT_SCRIPT(Event_NyunpaOffersTest) },
    { EVENT_TALK, 9, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleThisIsMasterFehsKungFu) },
    { EVENT_TALK, 10, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleBouldersOvercameHsuKungFuShould) },
    { EVENT_TALK, 11, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), EVENT_SCRIPT(Event_StudentAsksAboutAltin) },
    { EVENT_TALK, JUMPING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), EVENT_SCRIPT(Event_TalkToJumpingStudent) },
    { EVENT_TALK, 14, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleFeizhiPredictedHsusDangerHerProphecy) },
    { EVENT_TALK, 16, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleHsuIsRestingFeizhiIsWatching) },
    { EVENT_TALK, FEH, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTemplePracticeEveryoneChiHasItsLimits) },
    { EVENT_TALK, 9, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleWarriorYouAreGreatYouHave) },
    { EVENT_TALK, 10, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleEveryoneWantsToLearnTheChi) },
    { EVENT_TALK, 11, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleConcentrateEnergyIntoTheHandsAnd) },
    { EVENT_TALK, JUMPING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_CHI), EVENT_SCRIPT(Event_TalkToJumpingStudent) },
    { EVENT_TALK, 13, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleIDidNotThinkMuchOf) },
    { EVENT_TALK, 14, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleWhereDidFeizhiGoIAm) },
    { EVENT_TALK, 15, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleMasterHsuIsLateICan) },
    { EVENT_TALK, 16, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleIWillLearnChiIWill) },
    { EVENT_TALK, FEH, IF_FLAG_SET(FLAG_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleWarriorUseYourChiFromThe) },
    { EVENT_TALK, 16, IF_FLAG_SET(FLAG_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleWarriorPleaseUseYourChiFrom) },
    { EVENT_TALK, FEH, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorLetMeSeeYourChi) },
    { EVENT_TALK, 13, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleFeizhiIsGoneItIsDangerous) },
    { EVENT_TALK, 14, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleFeizhiIsStrongerThanSheLooks) },
    { EVENT_TALK, 15, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorShowUsYourChiLike) },
    { EVENT_TALK, 16, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorThatWasGreatButMaster) },
    { EVENT_TALK, FEH, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), EVENT_SCRIPT(Event_FehAsksToSeeChi) },
    { EVENT_TALK, 13, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleFeizhiIsGoneItIsDangerousOutside) },
    { EVENT_TALK, 14, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleFeizhiIsStrongerThanSheLooksShe) },
    { EVENT_TALK, 15, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleIKnowWhyYouCanUse) },
    { EVENT_TALK, 16, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleWarriorChiIsAGreatPower) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StudentWelcomesToSchool) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleStrongKicksAreVeryImportantKick) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIWantToLearnSoI) },
    { EVENT_TALK, JUMPING_STUDENT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToJumpingStudent) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleEveryoneInXianCanDoMartial) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleJustListeningDoesNotMakeOne) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StudentAsksAboutFeh) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterFehIsVeryBusyHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTemplePeaceHasReturnedToXianBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleNoMatterHowMuchYouPractice) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleTheMonstersAreVeryLargeStriking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), JUMPING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleWhenABoulderFallsJumpRepeat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleWhereDidFeizhiGainThePower) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleMasterFehIsVeryWorriedAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), FEH, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleCanSuchAYoungWarriorReally) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleYourChiIsFuchinTemplesHow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleIDontCareWhichChiIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleMasterHamasChiCanShatterBoulders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), JUMPING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleFollowSilkRoadStraightPassThrough) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleWarriorYouUseWeaponsAndChi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleFeizhiIsNotInTheFeh) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleFeizhiWorriesOnlyAboutMasterHsu) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(FLAG_FEH_SAW_CHI), MSGNO(MsgFuchinTempleLearningChiIsVeryHardMaster) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), FEH, IF_FLAG_SET(FLAG_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleWarriorTheRulesRequireYouTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(FLAG_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleSoCloseWarriorUseChiFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), FEH, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorShowMeYourChiI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleFeizhiIKnowIKnowHow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleMyKickAndPunchAreVery) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleShowUsYourChiNowDo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleIAmShockedThatThisWarrior) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), FEH, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleIWantToSeeTheChi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleFeizhiIKnowHowYouFeel) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleMyKickAndPunchAreVeryLoud) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleWarriorShowMeYourChiNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleWarriorDoNotHideYourChi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleWarriorsFightWithWeaponsKungFu) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgFuchinTemplePracticingKicksHurtsMyFeetVery) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleThatPunchWasGoodYouSurprise) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), JUMPING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIMoveTooQuicklyYouCant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleWarriorsCameToLearnTheFeh) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleGreatKickKickFromTheHip) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleYouAreTheWarriorsWhoCame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterFehIsTalkingWithFeizhi) },
    { TOUCH_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushTreeByHand) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushTreeByHand) },
    { TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushTreeByHand) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushTreeByHand) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), TRAINING_TREE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveTree) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), TRAINING_TREE_TARGET, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ForceTree) },
    { SEARCH(SEARCH_BARREL), 100, 0xf69, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_JAR), 101, 0xf68, FIND_ITEM(ITEM_UNICORN_RING) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gFuchinTempleEventsEntrance3[] = {
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 3 },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterHamaIsMeditatingAskingFor) },
    { EVENT_TALK, HAMA, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleTheFutureHoldsManyHardshipsBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIDoNotThinkMasterHama) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), HAMA, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleICannotSeeYourFutureClearly) },
    { SEARCH(SEARCH_JAR), 102, 0xf6a, FIND_COINS(6) },
    { SCENE_EVENTS_END },
};
// clang-format on

s32 gFuchinTemple_ShotScript[] = {
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(44),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

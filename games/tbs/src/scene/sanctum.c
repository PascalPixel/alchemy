/// @file
/// A sanctum: Vale's, where the Great Healer asks Isaac to go to Sol Sanctum
/// and a refusal ends the game, or another town's, with its healer.

#include "battle_effect_runtime.h"
#include "flags.h"
#include "game_state.h"
#include "unit/scene/sanctum.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/sound/sound.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "types.h"
#include "field_event.h"
#include "animspr.h"
#include "field_effect.h"
#include "field_scene.h"
#include "facing_object.h"
#include "text/msg_ids.h"
#include "io_reg.h"
#include "save_state.h"
#include "field_service.h"
#include "window.h"
#include "uiflow.h"
#include "resource.h"
#include "mem.h"
#include "object_dispatch.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "frame.h"
#include "fixed_math.h"
#include "songs.h"

// Scene temporary flags: 00 is set once the Great Healer has warned Isaac
// that leaving the village cannot be undone; 01 marks the visit after a
// refusal, which skips the account of Sol Sanctum.

/// The first placed actor is the sanctum's healer: the Great Healer in Vale,
/// the town's healer elsewhere. At the quest (entrances 20 and 21) Dora says
/// "Mother would never doubt your courage", the Wise One appears, and a copy
/// of the healer draws its afterimage.
enum {
  ACTOR_HEALER = ACTOR_FIRST_PLACED,
  ACTOR_DORA = ACTOR_FIRST_PLACED + 3,
  ACTOR_WISE_ONE = ACTOR_FIRST_PLACED + 5,
  ACTOR_HEALER_AFTERIMAGE
};

static void Sanctum_RequestQuestAcceptance(void);
static void Sanctum_ReceiveWiseOnesMission(void);
static void Sanctum_ShowHealerAfterimage(void);
static void Sanctum_HideHealerAfterimage(void);
static void Sanctum_UpdateHealerAfterimage(union FieldObject *fieldObject);
static void Sanctum_SpawnOrbitingEffect(s32 ownerId, s32 angleStep);
static s32 Sanctum_ChooseRestartOption(void);
static void Sanctum_ReenactSolSanctum(void);
static void Sanctum_RaiseItemIcon(s32 item, s32 fixedX, s32 fixedY, s32 fixedZ);
static void Sanctum_SpawnActorSpark(s32 id);
static void Sanctum_RestoreCeremonyFloor(void);

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 44 to 42 bytes in tbs-en.
#ifdef UBFIX
/// Faces the actor whose ID this callback stores in the timer field.
static void Sanctum_FaceActorNamedByTimer(union FieldObject *fieldObject) {
#else
/// Faces the actor whose ID this callback stores in the timer field.
static s32 Sanctum_FaceActorNamedByTimer(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct FieldActor *target;

  target = Object_GetById((s16)actor->timer);
  actor->facing = CalculateFacingAngle(target->z.fixed - actor->z.fixed,
                                       target->x.fixed - actor->x.fixed);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Returns the entrances shared by all Sanctum visits.
struct SceneEntrance *Sanctum_GetEntrances(void) { return gSanctum_Entrances; }

struct SceneRegion *Sanctum_GetRegions(void) { return NULL; }

u32 *Sanctum_GetExits(void) { return gSanctum_Exits; }

/// Chooses the actors for the current entrance.
struct ScenePlacement *Sanctum_GetPlacements(void) {
  switch (gGameState.entrance) {
  case 1:
  case 2:
    return gSanctum_PlacementB;

  case 10:
  case 11:
  case 12:
  case 35:
    return gSanctum_PlacementC;

  case 20:
  case 21:
    return gSanctum_PlacementD;

  case 29:
  case 32:
    return gSanctum_PlacementE;

  default:
    return gSanctum_PlacementA;
  }
}

/// Chooses interactions for the entrance and current story flags.
struct SceneEvent *Sanctum_GetEvents(void) {
  switch (gGameState.entrance) {
  case 10:
  case 12:
    return gSanctum_SceneTableB;
  case 11:
    return gSanctum_SceneTableC;
  case 20:
  case 21:
  case 50:
    return gSanctum_SceneTableD;
  case 32:
    return gSanctum_SceneTableI;
  case 29:
    return gSanctum_SceneTableG;
  case 35:
    return gSanctum_SceneTableH;
  default:
    break;
  }

  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
    return gSanctum_SceneTableF;
  }
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    return gSanctum_SceneTableE;
  }
  return gSanctum_SceneTableA;
}

/// Talking to the attendant (actor 9) in Vale's Sanctum: about the golden
/// statue and the healer.
static void Event_TalkAboutGoldenStatue(void) {
  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
    Engine_EventSetMessage(MsgSanctumWhenStrayFromYourWorldly);
  } else {
    Engine_EventSetMessage(MsgSanctumHealerMustWorriedAboutNever);
  }

  if (gGameState.entrance == 11) {
    Engine_EventSetMessage(MsgSanctumPolishedGoldStatueReturnedUs);
  }

  Engine_ActorSetAnimation(9, 1);
  Engine_ActorFaceEachOther(9, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(2);
  Engine_EventShowMessage(9, 0);
  Engine_EventEnd();
}

/// Reading the attendant's mind (actor 9): about the statue and Psynergy.
static void Event_ReadMindAboutGoldenStatue(void) {

  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgSanctumWonderIfEverSeeOur);
  } else {
    Engine_EventSetMessage(MsgSanctumChildHasAwakenedOurTeachings);
  }

  if (gGameState.entrance == 11) {
    Engine_EventSetMessage(MsgSanctumDirtyGoldenStatueCleanedUp);
  }

  Engine_ActorStop(9);
  Engine_ActorSetAnimation(9, 1);
  Engine_EventWait(2);
  Engine_EventShowMessage(9, 0);
  Actor_SetBuiltinAction(9, 2);
  Engine_EventEnd();
}

/// Talking to the Great Healer after refusing: changing his mind, Isaac
/// accepts after all. "Are you sure?"
static void Event_ReconsiderQuest(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgSanctumAreYouSure);
  Engine_EventOpenMessage(ACTOR_HEALER, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(20);
  } else {
    Engine_EventWait(20);
    Engine_EventOpenMessage(ACTOR_HEALER, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(20);
      Engine_EventOpenMessage(ACTOR_HEALER, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        if (Object_GetById(ACTOR_HEALER)->facing >= 0xa000 &&
            Object_GetById(ACTOR_HEALER)->facing <= 0xe000) {
          Actor_SetSpeed(ACTOR_HEALER, PIXELS(1) / 2, PIXELS(1) / 4);
          Engine_ActorFaceDirection(ACTOR_HEALER, FACING_EAST, 0);
          Engine_EventWait(10);
          Object_GetById(ACTOR_HEALER)->steeringFlags &= ~ACTOR_FACES_MOTION;
          Engine_ActorWalkToAndWait(ACTOR_HEALER, 152, 120);
          Engine_EventWait(1);
          Object_GetById(ACTOR_HEALER)->steeringFlags |= ACTOR_FACES_MOTION;
          Engine_EventWait(20);
          Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
          Engine_EventWait(20);
          Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 168, 120);
          Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 192, 168);
          Engine_EventWait(20);
          Engine_ActorWalkToAndWait(ACTOR_HEALER, 168, 120);
          Engine_ActorFaceDirection(ACTOR_HEALER,
                                    FACING_SOUTHEAST + FACING_STEP, 0);
          Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
        } else {
          Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 192, 168);
          Engine_EventWait(20);
          Engine_ActorFaceDirection(ACTOR_HEALER,
                                    FACING_SOUTHEAST + FACING_STEP, 0);
          Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
        }
        Sanctum_ReceiveWiseOnesMission();
        Engine_ColorBufferApplyTarget(0, 0);
        Engine_ColorBufferInterpolate(120);
        Engine_EventWait(120);
        Engine_AudioPlayCue(MUS_086);
        AudioCommand_WaitForCompletion();
        Engine_GameFlagSet(FLAG_SANCTUM_QUEST_ACCEPTED);
        Engine_EventRequestExit(30);
      }
    }
  }
  Engine_EventEnd();
}

#if TBS_PATCH_1
#define SANCTUM_NEW_FRIENDS_SPEAKER 2
#else
#define SANCTUM_NEW_FRIENDS_SPEAKER 1
#endif

/// Talking to actor 9 after the party grows: the actor asks about the new
/// friends. "Isaac, are your new friends Adepts?"
static void Event_AskAboutNewAdepts(void) {
  Engine_EventBegin();
#if TBS_PATCH_1
  Event_CallWithLastActiveObjectId(gSanctum_PlacementSequenceB);
  Engine_TaskWait(1);
#endif
  Engine_EventSetMessage(MsgSanctumIsaacYourNewFriendsAdepts);
  Engine_EventOpenMessage(9, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventShowMessage(9, 0);
  } else {
    gEventWork->message++;
    Event_ShowTwoMessagesAndWait(SANCTUM_NEW_FRIENDS_SPEAKER, 16, 1, 24, 1, 3,
                                 7, 16, 1, 14, 0);
    Engine_EventShowMessage(9, 0);
  }
  Engine_EventEnd();
}

/// Talking to actor 10 after the party has left Vale: does Isaac feel a change
/// in his Psynergy?
static void Event_AskAboutPsynergyChanges(void) {

  Engine_EventBegin();
  Engine_ActorFaceActor(10, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Engine_EventSetMessage(MsgSanctumDoFeelAnyChangeIn);
  Engine_EventOpenMessage(10, 0);

  if (Engine_EventChooseYesNo(0, 0) == 1) {
    (gEventWork->message)++;
  }

  Engine_EventShowMessage(10, 0);
  Engine_EventEnd();
}

/// Prepares the current visit and starts its opening scene.
s32 Sanctum_Initialize(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(1);
  Engine_EventWait(1);
  switch (gGameState.entrance) {
  case 10:
  case 11:
  case 12:
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Actor_SetPosition(10, PIXELS(200), PIXELS(80));
    }
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    break;
  case 20:
    Sanctum_RestoreCeremonyFloor();
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Sanctum_RequestQuestAcceptance();
    }
    // Fall through.
  case 29:
  case 32:
  case 35:
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    break;
  case 21:
    Sanctum_RestoreCeremonyFloor();
    Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Sanctum_RequestQuestAcceptance();
    }
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    break;
  }
  return 0;
}

/// Walking toward the door after refusing: the Great Healer warns that the
/// choice is final. "Once you step outside the village, you cannot take back
/// your decision."
static void Event_WarnBeforeLeaving(void) {
  Engine_EventBegin();
  Engine_CameraSetSpeed(PIXELS(1), PIXELS(1) / 8);
  Engine_CameraMoveToActor(GARET, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgSanctumOnceStepOutsideVillageCannot);
  Engine_EventShowMessage(ACTOR_HEALER, 0);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
  Engine_EventEnd();
}

/// Leaving after refusing: the room turns sepia and the game offers to start
/// again. "And so the world began drifting..."
static void Event_LeaveAfterRefusing(void) {
  struct BattleEffectBuffers *paletteWork;

  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  Engine_ActorSetAnimation(GARET, 0);
  Engine_ActorSetAnimation(ACTOR_DORA, 0);
  Engine_ActorSetAnimation(12, 0);
  Engine_ActorSetAnimation(ACTOR_HEALER, 0);
  Engine_ActorSetAnimation(9, 0);
  Engine_ActorSetAnimation(10, 0);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_SEPIA, 0);
  Engine_ColorBufferInterpolate(120);
  Engine_EventWait(180);
  paletteWork = gPaletteBufferWork;
  // Set colors 13-15 of target palette 2.
  paletteWork->target[45] = 0x7c00;
  paletteWork->target[46] = 0x7c00;
  paletteWork->target[47] = 0x7c00;
  paletteWork->page = 0;
  paletteWork->duration = 1;
  paletteWork->step = 1;
  paletteWork->padding2a03 = 1;
  Engine_EventWait(1);
  UiText_ShowCenteredMessage(MsgSanctumWorldBeganDrifting, 1, 0);
  Engine_ColorBufferApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(120);
  Engine_EventWait(120);
  Engine_EventWait(60);
  if (Sanctum_ChooseRestartOption() == 0) {
    Engine_EventEnd();
    Engine_EventRequestExit(20);
  } else {
    Engine_EventEnd();
    Engine_EventRequestExit(50);
  }
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 66 to 64 bytes in tbs-en.
#ifdef UBFIX
/// Keeps this actor at the party leader's drawing priority.
static void Sanctum_MatchLeaderPriority(union FieldObject *fieldObject) {
#else
/// Keeps this actor at the party leader's drawing priority.
static s32 Sanctum_MatchLeaderPriority(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  actor->sprite->image.priority =
      Object_GetById(ACTOR_PARTY_LEADER)->sprite->image.priority;
  actor->sprite->shadow.priority =
      Object_GetById(ACTOR_PARTY_LEADER)->sprite->image.priority;
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Has the party and four witnesses nod, waiting for the final witness.
static inline void Sanctum_NodCeremonyGroup(void) {
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_DORA, 3);
  Engine_ActorSetAnimation(12, 3);
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorSetAnimationAndWait(10, 3);
}

/// Starts the six group motions and waits for the final witness.
static inline void Sanctum_StartCeremonyGroupMotion(void) {
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 1);
  Engine_ActorStartRepeatedMotion(12, 1);
  Engine_ActorStartRepeatedMotion(9, 1);
  Engine_ActorRunRepeatedMotion(10, 1);
}

/// Shows the same foreground reaction on the party and four witnesses.
static inline void Sanctum_ShowCeremonyGroupEmotes(void) {
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | 2, 0);
}

/// Discusses Sol Sanctum and asks Isaac to accept the quest.
static void Sanctum_RequestQuestAcceptance(void) {
  u32 effectIndex;

  Engine_EventBegin();
  // A repeat visit skips the account of Sol Sanctum.
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_01) != 0) {
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Camera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
    Engine_CameraWaitForMove();
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transitionFrames = 64;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(120);
  } else {
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_SEPIA, 0);
    Engine_ColorBufferInterpolate(1);
    Engine_EventWait(1);
    Camera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
    Engine_CameraWaitForMove();
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    // Reenact the journey before explaining the quest.
    Sanctum_ReenactSolSanctum();
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_EventWait(100);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Engine_ActorSetAnimation(GARET, ANIM_STAND);
    Engine_EventWait(30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Actor_FaceActor(GARET, 12, 0);
    Engine_EventWait(20);
    Actor_SetBuiltinAction(ACTOR_HEALER, ACTOR_ACTION_IDLE);
    Actor_SetBuiltinAction(12, ACTOR_ACTION_IDLE);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(12, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(9, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(10, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(12, 2);
    Engine_EventSetMessage(MsgSanctumWeHadIdeaTrueSanctum);
    Event_ShowMessage(12, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(12, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    Actor_FaceActor(GARET, ACTOR_DORA, 0);
    Engine_EventWait(20);
    Event_OpenMessage(ACTOR_DORA, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
      Engine_EventWait(20);
      Event_ShowMessage(ACTOR_DORA, 0);
      gEventWork->message++;
    } else {
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
      Engine_EventWait(20);
      gEventWork->message++;
      Event_ShowMessage(ACTOR_DORA, 0);
    }
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(GARET, 9, 0);
    Actor_FaceActor(ACTOR_DORA, 9, 0);
    Engine_EventWait(20);
    Event_OpenMessage(9, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(9, 3);
      Engine_EventWait(20);
      Event_ShowMessage(9, 0);
      gEventWork->message++;
    } else {
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(9, 4);
      Engine_EventWait(20);
      gEventWork->message++;
      Event_ShowMessage(9, 0);
    }
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_DORA, 3);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(30);
    Actor_ShowEmote(12, EMOTE_IN_FRONT | 1, 0);
    Engine_EventWait(60);
    Actor_FaceActor(12, ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_WalkToAndWait(12, 0xe0, 0x78);
    Engine_EventWait(10);
    Event_ShowMessage(12, 0);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
    Actor_FaceActor(9, ACTOR_HEALER, 0);
    Actor_FaceActor(10, ACTOR_HEALER, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_DORA, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 3);
    Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT, 0);
    Engine_EventWait(60);
    Actor_FaceActor(ACTOR_HEALER, 12, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 0);
    Engine_EventWait(60);
    Actor_FaceActor(12, ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(12, 3);
    Engine_EventWait(10);
    Event_ShowMessage(12, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_HEALER, FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 0);
    Engine_EventWait(60);
    Actor_FaceActor(12, ACTOR_HEALER, 0);
    Engine_EventWait(50);
    Actor_ShowEmote(12, EMOTE_IN_FRONT | 1, 0);
    Engine_EventWait(40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(10, EMOTE_IN_FRONT | 1, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 1);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(12, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(9, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(10, EMOTE_IN_FRONT, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(10);
    Event_ShowMessage(GARET, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(12, EMOTE_IN_FRONT | 2, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GARET, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_DORA, 1);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(30);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(12, 2);
    Engine_EventWait(20);
    Actor_FaceActor(12, ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(12, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_DORA, 0);
    Engine_EventWait(20);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 2);
    Engine_ActorSetAnimation(ACTOR_HEALER, 0);
    Actor_SetChildValue(ACTOR_HEALER, 0x100);
    Sanctum_ShowHealerAfterimage();
    Event_PlayCue(SE_196);
    Sanctum_SpawnOrbitingEffect(8, 0x1200);
    Engine_EventWait(32);
    Sanctum_SpawnOrbitingEffect(8, 0x1200);
    Engine_EventWait(16);
    for (effectIndex = 0; effectIndex < 6; effectIndex++) {
      Sanctum_SpawnOrbitingEffect(8, 0x1200);
      Engine_EventWait(8);
    }
    Engine_EventWait(8);
    Sanctum_SpawnOrbitingEffect(8, 0x1200);
    Engine_EventWait(32);
    Sanctum_SpawnOrbitingEffect(8, 0x1200);
    Engine_EventWait(96);
    Engine_EventWait(32);
    Actor_SetChildValue(ACTOR_HEALER, 0);
    Engine_EventWait(30);
    // Return to the witnesses and finish the departure conversation.
    Sanctum_HideHealerAfterimage();
    Engine_ActorSetAnimation(ACTOR_HEALER, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(12, 0x102);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GARET, 0x102);
    Actor_SetAttachedEffect(ACTOR_DORA, 0x102);
    Actor_SetAttachedEffect(9, 0x102);
    Actor_SetAttachedEffect(10, 0x102);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventWait(20);
    Event_ShowMessage(10, 0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(GARET, ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(30);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_ActorFaceEachOther(9, 0xa, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(10, 3);
    ObjectMotion_WaitForAnimationChange(0xa);
    Engine_EventWait(30);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    Engine_ActorFaceEachOther(GARET, 0xc, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_DORA, 3);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_ActorSetAnimation(12, 3);
    ObjectMotion_WaitForAnimationChange(0xc);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 2);
    Engine_ActorSetAnimation(ACTOR_HEALER, 0);
    Actor_SetChildValue(ACTOR_HEALER, 0x100);
    Sanctum_ShowHealerAfterimage();
    Event_PlayCue(SE_196);
    Sanctum_SpawnOrbitingEffect(8, 0x1200);
    Engine_EventWait(32);
    Sanctum_SpawnOrbitingEffect(8, 0x1200);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
    Actor_FaceActor(12, ACTOR_HEALER, 0);
    Actor_FaceActor(9, ACTOR_HEALER, 0);
    Actor_FaceActor(10, ACTOR_HEALER, 0);
    Engine_EventWait(16);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GARET, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_DORA, 1);
    Engine_ActorStartRepeatedMotion(12, 1);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorStartRepeatedMotion(10, 1);
    for (effectIndex = 0; effectIndex < 6; effectIndex++) {
      Sanctum_SpawnOrbitingEffect(8, 0x1200);
      Engine_EventWait(8);
    }
    Engine_EventWait(8);
    Sanctum_SpawnOrbitingEffect(8, 0x1200);
    Engine_EventWait(32);
    Sanctum_SpawnOrbitingEffect(8, 0x1200);
    Engine_EventWait(128);
    Actor_SetChildValue(ACTOR_HEALER, 0);
    Engine_EventWait(30);
    Sanctum_HideHealerAfterimage();
    Engine_ActorSetAnimation(ACTOR_HEALER, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 2);
    Engine_EventWait(30);
    Actor_ShowEmote(12, EMOTE_IN_FRONT | 5, 0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(9, 0xa, 0);
    Engine_EventWait(40);
    Actor_FaceActor(9, ACTOR_HEALER, 0);
    Actor_FaceActor(10, ACTOR_HEALER, 0);
    Event_ShowMessage(12, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_DORA, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GARET, 1);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
    Engine_ActorStartRepeatedMotion(9, 1);
    Engine_ActorStartRepeatedMotion(10, 1);
    Engine_ActorFaceEachOther(9, 0xa, 0);
    Actor_SetAttachedEffect(12, 0x102);
    Engine_EventWait(60);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(9, ACTOR_HEALER, 0);
    Actor_FaceActor(10, ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(GARET, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(12, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(10, EMOTE_IN_FRONT | 1, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_DORA, 3);
    Engine_ActorSetAnimation(12, 3);
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(10, 3);
    ObjectMotion_WaitForAnimationChange(0xa);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(12, 1);
    Engine_EventWait(20);
    Event_ShowMessage(12, 0);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_HEALER, 12, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(50);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(12, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(10, EMOTE_IN_FRONT | 2, 0);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_DORA, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(12, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(10, EMOTE_IN_FRONT | 1, 0);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 1);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GARET, 0x102);
    Actor_SetAttachedEffect(ACTOR_DORA, 0x102);
    Actor_SetAttachedEffect(12, 0x102);
    Actor_SetAttachedEffect(9, 0x102);
    Actor_SetAttachedEffect(10, 0x102);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(12, 4);
    Engine_EventWait(20);
    Event_ShowMessage(12, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(40);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
    Engine_ActorFaceEachOther(9, 0xa, 0);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(60);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
    Actor_FaceActor(12, ACTOR_HEALER, 0);
    Actor_FaceActor(9, ACTOR_HEALER, 0);
    Actor_FaceActor(10, ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_DORA, 0);
    Engine_EventWait(60);
    Actor_WalkToAndWait(ACTOR_HEALER, 0xc8, 0x88);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceEachOther(ACTOR_HEALER, GARET, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(50);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(50);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 1);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
    Engine_EventWait(60);
    Actor_SetAttachedEffect(ACTOR_DORA, 0x102);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
    Event_ShowMessage(ACTOR_DORA, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(30);
    Actor_ShowEmote(12, EMOTE_IN_FRONT | 2, 0);
    Engine_ActorStartRepeatedMotion(12, 1);
    Engine_EventWait(20);
    Event_ShowMessage(12, 0);
    Engine_EventWait(30);
    Actor_WalkToAndWait(ACTOR_HEALER, 0xa8, 0x78);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(12, ACTOR_HEALER, 0);
    Actor_FaceDirection(ACTOR_HEALER, FACING_NORTH + FACING_STEP, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(10);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Actor_SetAttachedEffect(GARET, 0x102);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(GARET, 0);
    Engine_EventWait(30);
  }
  // Ask Isaac to commit, or leave the group waiting for his answer.
  Engine_EventSetMessage(MsgSanctumIsaacWillAcceptResponsibilityFor);
  Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 1);
  Engine_EventWait(20);
  Event_OpenMessage(ACTOR_HEALER, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Sanctum_ReceiveWiseOnesMission();
    ColorBuffer_ApplyTarget(0, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_EventWait(120);
    Event_PlayCue(MUS_086);
    AudioCommand_WaitForCompletion();
    GameFlag_Set(FLAG_SANCTUM_QUEST_ACCEPTED);
    Engine_EventRequestExit(30);
  } else {
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(12, 1);
    Engine_ActorSetAnimationAndWait(12, 4);
    Engine_EventWait(20);
    Event_ShowMessage(12, 0);
    Engine_EventWait(20);
    Object_GetById(10)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
#ifdef UBFIX
    Object_GetById(10)->update = Sanctum_MatchLeaderPriority;
#else
    Object_GetById(10)->update =
        (void (*)(union FieldObject *))Sanctum_MatchLeaderPriority;
#endif
  }
  Engine_EventEnd();
}

/// Brings in the Wise One and prepares the party for its mission.
static void Sanctum_ReceiveWiseOnesMission(void) {

  u32 sparkIndex;
  struct FieldActor *wiseOne;

  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(12, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(9, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(10, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
  Camera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
  Engine_CameraWaitForMove();
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_HEALER, 3);
  Engine_ActorSetAnimation(12, 3);
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorSetAnimationAndWait(10, 3);
  Engine_EventWait(50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
  Actor_FaceActor(12, ACTOR_HEALER, 0);
  Actor_FaceActor(9, ACTOR_HEALER, 0);
  Actor_FaceActor(10, ACTOR_HEALER, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  // Garet and the witnesses react to Isaac accepting the burden.
  Engine_EventSetMessage(MsgSanctumAcceptIsaacCantMean);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(20);
  Actor_FaceActor(12, GARET, 0);
  Engine_ActorStartRepeatedMotion(12, 2);
  Engine_ActorShowEmote(12, EMOTE_IN_FRONT | 3, 0);
  Engine_EventWait(60);
  Event_ShowMessage(12, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Actor_FaceActor(GARET, 12, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(40);
  Actor_FaceActor(GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(12, ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
  Engine_EventWait(20);
  Engine_ActorFaceDirection(ACTOR_HEALER, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(80);
  Event_PlayCue(MUS_017);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PALE, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_EventWait(40);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(192), -1, PIXELS(104), 1);
  Engine_EventWait(120);
  Event_PlayCue(MUS_021);
  Event_PlayCue(SE_308);
  // The Wise One enters while the healer's afterimages appear.
  Actor_SetPosition(ACTOR_WISE_ONE, PIXELS(200), PIXELS(8));
  Actor_SetSpeed(ACTOR_WISE_ONE, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_MoveToAndWait(ACTOR_WISE_ONE, 200, 72);
  Event_PlayCue(SE_288);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 2);
  Engine_ActorSetAnimation(ACTOR_HEALER, 0);
  Sanctum_ShowHealerAfterimage();
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WISE_ONE, 0);
  Actor_FaceActor(GARET, ACTOR_WISE_ONE, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_WISE_ONE, 0);
  Actor_FaceActor(12, ACTOR_WISE_ONE, 0);
  Actor_FaceActor(9, ACTOR_WISE_ONE, 0);
  Actor_FaceActor(10, ACTOR_WISE_ONE, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
  Engine_ActorStartRepeatedMotion(12, 2);
  Engine_ActorStartRepeatedMotion(9, 2);
  Engine_ActorStartRepeatedMotion(10, 2);
  Engine_EventWait(40);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Engine_EventWait(20);
  Engine_EventWait(40);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Engine_EventWait(60);
  Engine_ActorSetChildValue(ACTOR_WISE_ONE, 0x100);
  Event_PlayCue(MUS_017);
  Event_PlayCue(SE_308);
  // Shrink the Wise One amid sparks before restoring the room.
  for (sparkIndex = 0; sparkIndex < 32; sparkIndex++) {
    Sanctum_SpawnActorSpark(13);
    Engine_EventWait(4);
    wiseOne = Object_GetById(ACTOR_WISE_ONE);
    wiseOne->scaleX += -Q16_ONE / 100;
    wiseOne = Object_GetById(ACTOR_WISE_ONE);
    wiseOne->scaleY += -Q16_ONE / 100;
  }
  Event_PlayCue(SE_288);
  Actor_SetChildValue(ACTOR_WISE_ONE, 0);
  Actor_SetPosition(ACTOR_WISE_ONE, 0, 0);
  Engine_EventWait(40);
  Camera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
  Engine_CameraWaitForMove();
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(60);
  Engine_EventWait(120);
  Sanctum_HideHealerAfterimage();
  Engine_ActorSetAnimation(ACTOR_HEALER, 1);
  Event_PlayCue(MUS_002);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 2);
  Engine_EventWait(20);
  Sanctum_StartCeremonyGroupMotion();
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_EventWait(30);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(30);
  Actor_FaceActor(12, ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
  Actor_FaceActor(9, ACTOR_HEALER, 0);
  Actor_FaceActor(10, ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Event_ShowMessage(12, 0);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_HEALER, 12, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Sanctum_ShowCeremonyGroupEmotes();
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Sanctum_NodCeremonyGroup();
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
  Engine_EventWait(180);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | 5, 0);
  Engine_EventWait(60);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_ActorFaceEachOther(9, 10, 0);
  Engine_EventWait(40);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(9, ACTOR_HEALER, 0);
  Actor_FaceActor(10, ACTOR_HEALER, 0);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  Sanctum_StartCeremonyGroupMotion();
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(80);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Event_ShowMessage(12, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | 5, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 1);
  Engine_ActorShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | 6, 0);
  Engine_EventWait(20);
  Sanctum_StartCeremonyGroupMotion();
  Engine_EventWait(40);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Sanctum_NodCeremonyGroup();
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(30);
  Sanctum_NodCeremonyGroup();
  Engine_EventWait(120);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(12, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | 5, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(12, 1);
  Event_ShowMessage(12, 0);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_HEALER, 12, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(12, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(10, EMOTE_IN_FRONT, 0);
  Engine_EventWait(60);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(40);
  Actor_WalkToAndWait(ACTOR_HEALER, 168, 176);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
  Actor_FaceActor(12, ACTOR_HEALER, 0);
  Actor_FaceActor(9, ACTOR_HEALER, 0);
  Actor_FaceActor(10, ACTOR_HEALER, 0);
  Actor_WalkToAndWait(ACTOR_HEALER, 200, 200);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(12, ACTOR_HEALER, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 0);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Actor_WalkTo(ACTOR_HEALER, 200, 0x110);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(10, FACING_SOUTH + FACING_STEP, 0);
  Engine_ActorWaitForMove(ACTOR_HEALER);
  Actor_SetPosition(ACTOR_HEALER, 0, 0);
  Engine_EventWait(60);
  Sanctum_ShowCeremonyGroupEmotes();
  Engine_EventWait(60);
  Engine_EventWait(120);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
  Actor_FaceActor(GARET, 10, 0);
  Actor_FaceActor(ACTOR_DORA, 9, 0);
  Actor_FaceActor(12, 9, 0);
  Engine_EventWait(120);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 5, 0);
  Engine_EventWait(60);
  Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(80);
  Actor_FaceActor(9, 10, 0);
  Engine_ActorRunRepeatedMotion(9, 1);
  Engine_EventWait(20);
  Event_ShowMessage(9, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(10, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(20);
  Engine_ActorShowEmote(10, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(20);
  Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventWait(30);
  Engine_ActorFaceDirection(10, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(20);
  Event_ShowMessage(10, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorSetAnimationAndWait(10, 3);
  Engine_EventWait(20);
  Actor_WalkTo(9, 200, 0x110);
  Actor_WalkToAndWait(10, 200, 0x110);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, 0, 0);
  Engine_ActorShowEmote(12, EMOTE_IN_FRONT | 5, 0);
  Engine_EventWait(60);
  Actor_WalkToAndWait(12, 200, 136);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
  Actor_FaceActor(GARET, 12, 0);
  Actor_FaceActor(ACTOR_DORA, 12, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(12, 4);
  Engine_EventWait(20);
  Event_ShowMessage(12, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_EventWait(20);
  Actor_WalkToAndWait(ACTOR_DORA, 168, 168);
  Actor_FaceActor(ACTOR_DORA, 12, 0);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Actor_FaceActor(GARET, ACTOR_DORA, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimationAndWait(12, 4);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
  Actor_FaceActor(GARET, 12, 0);
  Engine_EventWait(20);
  Event_ShowMessage(12, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Engine_ActorShowEmote(12, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Event_ShowMessage(12, 0);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Engine_ActorShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_DORA, 3);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(12, 3);
}

/// Copies the healer into the afterimage actor.
static void Sanctum_ShowHealerAfterimage(void) {
  u32 entryIndex;
  struct FieldActor *afterimage;
  struct FieldActor *actor;
  struct AnimationObject *sprite;

  actor = Object_GetById(ACTOR_HEALER);
  if (actor != 0) {
    Engine_ActorSetPosition(ACTOR_HEALER_AFTERIMAGE, actor->x.fixed,
                            actor->z.fixed);
  }
  Engine_ActorSetAnimation(ACTOR_HEALER_AFTERIMAGE, 0);
  afterimage = Object_GetById(ACTOR_HEALER_AFTERIMAGE);
  actor = Object_GetById(ACTOR_HEALER);
  afterimage->facing = actor->facing;
  actor = Object_GetById(ACTOR_HEALER_AFTERIMAGE);
  actor->update = Sanctum_UpdateHealerAfterimage;
  actor = Object_GetById(ACTOR_HEALER_AFTERIMAGE);
  sprite = actor->sprite;
  {
    for (entryIndex = 0; entryIndex < sprite->count; entryIndex++) {
      struct AnimationEntry *entry = sprite->entries[entryIndex];

      if (entry != 0 && entry->script != 0) {
        entry->param = 10;
      }
    }
  }
  sprite->dirty = 1;
  Object_GetById(ACTOR_HEALER_AFTERIMAGE)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 2;
}

static void Sanctum_HideHealerAfterimage(void) {
  struct FieldActor *state = Object_GetById(ACTOR_HEALER_AFTERIMAGE);
  state->update = 0;
  Engine_ActorSetPosition(ACTOR_HEALER_AFTERIMAGE, 0, 0);
}

/// Cycles the afterimage through four offsets from the healer.
static void Sanctum_UpdateHealerAfterimage(union FieldObject *fieldObject) {
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct FieldActor *healer;

  healer = Object_GetById(ACTOR_HEALER);
  actor->targetX = actor->x.fixed = healer->x.fixed;
  actor->targetY = actor->y.fixed = healer->y.fixed;
  actor->targetZ = actor->z.fixed = healer->z.fixed - PIXELS(2);
  switch (gFrameCount & 3) {
  case 0:
    actor->targetX = actor->x.fixed = healer->x.fixed - PIXELS(7) / 2;
    break;
  case 1:
    actor->targetX = actor->x.fixed = healer->x.fixed + PIXELS(3);
    break;
  case 2:
    actor->targetY = actor->y.fixed = healer->y.fixed + PIXELS(2);
    break;
  case 3:
    actor->targetZ = actor->z.fixed = healer->z.fixed;
    break;
  }
}

/// Advances an effect around its linked actor on an elliptical path.
static void Sanctum_UpdateOrbitingEffect(union FieldObject *fieldObject) {
  struct FieldActor *effect = (struct FieldActor *)fieldObject;
  struct FieldActor *anchor = effect->linkedObject;
  u16 *angle = &effect->timer;
  u16 orbitAngle = *angle;
  s32 x;
  s32 z;
  s32 currentX;

  x = anchor->x.fixed + Engine_MathCos(orbitAngle) * 14;
  effect->x.fixed = x;

  z = anchor->z.fixed + Engine_MathSin(orbitAngle) * 10;
  currentX = effect->x.fixed;
  effect->z.fixed = z;
  effect->targetZ = z;
  effect->targetX = currentX;

  *angle = (*angle + effect->work);
}

/// Creates an orbiting effect with the requested angular step.
static void Sanctum_SpawnOrbitingEffect(s32 ownerId, s32 angleStep) {
  s32 orbitStep;
  struct FieldActor *owner;
  struct FieldActor *effect;
  struct AnimationObject *sprite;
  struct AnimationObject *sourceSprite;

  orbitStep = angleStep;
  owner = Object_GetById(ownerId);
  if (owner != 0) {
    effect = Object_Create(0x11d, owner->x.fixed, owner->y.fixed + PIXELS(45),
                           owner->z.fixed);
    if (effect != 0) {
      sprite = effect->sprite;
      Engine_ObjectSetScript(effect, gSanctum_OwnerEffectScript);
      {
        s32 zero = 0;

        effect->motionFlags = zero;
        effect->timer = zero;
      }
      effect->work = orbitStep;
      effect->update = Sanctum_UpdateOrbitingEffect;
      {
        u16 clearFlags = 0;
        sprite->flags = clearFlags;
      }
      sourceSprite = owner->sprite;
      effect->linkedObject = owner;
      sprite->image.priority = sourceSprite->image.priority;
    }
  }
}

#if LANGUAGE != LANGUAGE_JAPANESE
#define QUESTION_X 2
#define QUESTION_WIDTH 25
#define CURSOR_X 24
#else
#define QUESTION_X 7
#define QUESTION_WIDTH 17
#define CURSOR_X 64
#endif

/// Offers the refusal ending's restart choices and returns the selection.
static s32 Sanctum_ChooseRestartOption(void) {
  s32 slot;
  struct MenuSprite cursor;
  struct UiWindow *window;
  s32 text;
  s32 choice;

  Engine_ActorSetPosition(ACTOR_HEALER, 0, 0);
  Engine_ActorSetPosition(9, 0, 0);
  Engine_ActorSetPosition(10, 0, 0);
  Engine_ActorSetPosition(GARET, 0, 0);
  Engine_ActorSetPosition(ACTOR_DORA, 0, 0);
  Engine_ActorSetPosition(12, 0, 0);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, 2);
  Engine_ColorBufferInterpolate(1);
  Engine_EventWait(1);
  window = UiWindow_Create(QUESTION_X, 7, QUESTION_WIDTH, 5, 1);
  text = MsgSanctumGreatHealer;
  UiText_DrawResource(text, window, 16, 0);
  if (SaveState_CountRecordsExcludingFlagged(1) == 0) {
    UiText_DrawResource(text + 2, window, 16, 16);
  } else {
    UiText_DrawResource(text + 1, window, 16, 16);
  }
  FieldUiTextResource_Initialize(&cursor, &slot);
  FieldUiTextResource_SetPosition(&cursor, 72, 60);
  choice = 0;
  while ((gKeysPressed & KEY_A) == 0) {
    s32 x;

    if (gKeysRepeat & KEYS_VERTICAL) {
      choice ^= 1;
    }
    x = gSanctum_CursorBob[(gFrameTick >> 1) & 15] + CURSOR_X;
    FieldUiTextResource_SetPosition(&cursor, x, (choice << 4) + 60);
    Engine_EventWait(1);
  }
  UiTextResource_Release(slot);
  UiWork_Finalize(window, 1);
  return choice;
}

s32 Math_RemainderUnsigned(s32 value, s32 modulus);

/// Has Isaac and Garet act out the events at Sol Sanctum.
static void Sanctum_ReenactSolSanctum(void) {

  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_SetSpeed(GARET, PIXELS(3) / 2, PIXELS(3) / 4);
  Engine_ActorRunRepeatedMotion(12, 2);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(12, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(15);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 184, 168);
  Object_GetById(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorWalkToAndWait(GARET, 200, 168);
  Engine_EventWait(1);
  Object_GetById(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Object_GetById(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ActorJump(GARET, 2, 0);
  Engine_EventWait(15);
  Engine_ActorFaceActor(GARET, ACTOR_HEALER, 0);
  Engine_EventWait(5);
  Engine_ActorJump(GARET, 2, 0);
  Engine_EventWait(25);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_ActorFaceActor(GARET, 12, 0);
  Engine_EventWait(5);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(5);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorFaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(15);
  Engine_ActorSetAnimation(ACTOR_DORA, 3);
  Engine_ActorSetAnimation(12, 3);
  Engine_ActorSetAnimation(ACTOR_HEALER, 3);
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorSetAnimationAndWait(10, 3);
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 12, 0);
  Engine_ActorFaceActor(GARET, 12, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Engine_ActorFaceActor(GARET, ACTOR_DORA, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Engine_EventWait(15);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(10);
  // Illustrate the account with the floating item icon.
  Sanctum_RaiseItemIcon(222, PIXELS(184), PIXELS(27), PIXELS(168));
  Engine_ActorFaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(10);
  Engine_ActorJump(GARET, 4, 0);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(15);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Engine_ActorJump(GARET, 4, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(15);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Engine_ActorFaceActor(GARET, 12, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(10);
  Object_GetById(ACTOR_HEALER)->timer = 1;
#ifdef UBFIX
  Object_GetById(ACTOR_HEALER)->update = Sanctum_FaceActorNamedByTimer;
#else
  Object_GetById(ACTOR_HEALER)->update =
      (void (*)(union FieldObject *))Sanctum_FaceActorNamedByTimer;
#endif
  Object_GetById(12)->timer = 1;
#ifdef UBFIX
  Object_GetById(12)->update = Sanctum_FaceActorNamedByTimer;
#else
  Object_GetById(12)->update =
      (void (*)(union FieldObject *))Sanctum_FaceActorNamedByTimer;
#endif
  Engine_ActorWalkToAndWait(GARET, 196, 180);
  Engine_ActorWalkToAndWait(GARET, 184, 184);
  Engine_ActorWalkToAndWait(GARET, 180, 180);
  Engine_ActorWalkToAndWait(GARET, 168, 168);
  Engine_ActorWalkToAndWait(GARET, 180, 156);
  Engine_ActorWalkTo(GARET, 200, 104);
  Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 192, 168);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_ActorWaitForMove(GARET);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(15);
  Object_GetById(12)->update = NULL;
  Object_GetById(ACTOR_HEALER)->update = NULL;
  Engine_ActorStartRepeatedMotion(ACTOR_HEALER, 2);
  Engine_ActorShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_HEALER, 0);
  Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
  Engine_EventWait(20);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
  Engine_EventWait(20);
  Engine_EventWait(15);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(12, 3);
  Engine_EventWait(60);
  Engine_ActorWalkToAndWait(GARET, 208, 168);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Engine_ActorFaceActor(GARET, 12, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(GARET, FACING_EAST, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorFaceActor(GARET, 12, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(12, 3);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Engine_ActorFaceDirection(GARET, FACING_EAST, 0);
  Engine_EventWait(10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  // Repeat the synchronized eight-point paths around the room.
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gSanctum_LeaderCircleScript);
  Actor_SetActionScript(GARET, gSanctum_GaretCircleScript);
  Object_RefreshSelectorById(0);
  Object_RefreshSelectorById(1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_SetSpeed(GARET, PIXELS(3) / 2, PIXELS(3) / 4);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Engine_ActorJump(GARET, 6, 0);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 9, 0);
  Engine_ActorFaceActor(GARET, ACTOR_HEALER, 0);
  Engine_EventWait(1);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 12, 0);
  Engine_ActorFaceActor(GARET, ACTOR_DORA, 0);
  Engine_EventWait(1);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Engine_ActorFaceActor(GARET, 9, 0);
  Engine_EventWait(1);
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 192, 168);
  Engine_ActorWalkToAndWait(GARET, 208, 168);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorFaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
  Engine_ActorFaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Engine_ActorFaceActor(GARET, 12, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_DORA, 3);
  Engine_ActorSetAnimationAndWait(12, 3);
  Engine_EventWait(30);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(GARET, FACING_EAST, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  Engine_EventWait(60);
}

/// Shows an item icon rising from the supplied fixed-point position.
static void Sanctum_RaiseItemIcon(s32 item, s32 fixedX, s32 fixedY,
                                  s32 fixedZ) {
  struct FieldActor *icon;
  struct AnimationObject *sprite;
  u8 *transferBuffer;
  u32 frame;
  u8 zero;

  icon = Engine_ObjectCreate(22, fixedX, fixedY, fixedZ);
  zero = 0;
  if (icon != 0) {
    Engine_ObjectSetScript(icon, gSanctum_ItemIconGrowScript);
    sprite = icon->sprite;
    sprite->flags = zero;
    sprite->count = zero;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    icon->velocityY = PIXELS(2);
    icon->gravity = PIXELS(1) / 4;
    transferBuffer = Runtime_AllocateHeapBlock(17, 0x608);
    Ui_PrepareTransferForItem(item);
    Engine_VramLoad(sprite->slot, 128, transferBuffer + 0x400);
    Runtime_ReleaseHeapBlock(17);
    for (frame = 0; frame < 60; frame++) {
      if ((u32)(icon->velocityY + 255) <= 0x1fe)
        icon->motionFlags = 0;
      Engine_TaskWait(1);
    }
    Engine_ObjectSetScript(icon, gSanctum_ItemIconEndScript);
  }
}

/// Moves a spark upward, cycles its scale, and expires its lifetime.
static void Sanctum_UpdateRisingSpark(union FieldObject *object) {
  struct FieldActor *spark = &object->actor;
  s32 scale;
  s32 timer;

  // The timer stores the upward step in sixteenths of a pixel.
  spark->y.fixed += spark->timer << 12;
  spark->targetY = spark->y.fixed;
  scale = 0;
  switch (((s16)spark->work >> 2) & 3) {
  case 0:
    scale = Q16_ONE;
    break;
  case 1:
  case 3:
    scale = 4 * Q16_ONE / 5;
    break;
  case 2:
    scale = 3 * Q16_ONE / 5;
    break;
  }
  spark->scaleX = scale;
  spark->scaleY = scale;
  timer = spark->work - 1;
  spark->work = timer;
  if ((s16)timer <= 0)
    Engine_ObjectSetScript(spark, gSanctum_SparkEndScript);
}

/// Creates a short-lived spark beside the specified actor.
static void Sanctum_SpawnActorSpark(s32 id) {
  struct FieldActor *actor;
  struct FieldActor *spark;
  struct AnimationObject *sprite;
  s32 x;
  s32 randomOffset;

  actor = Object_GetById(id);
  if (actor == 0)
    return;
  randomOffset = Math_RemainderUnsigned(Engine_RandomNext(), 20);
  x = actor->x.fixed;
  x += randomOffset << 16;
  x += -PIXELS(10);
  spark = Engine_ObjectCreate(
      0x11e, x,
      actor->y.fixed + ((Engine_RandomNext() & 15) << 16) + -PIXELS(8),
      actor->z.fixed);
  if (spark == 0)
    return;
  sprite = spark->sprite;
  spark->motionFlags = 0;
  spark->timer = Math_RemainderUnsigned(Engine_RandomNext(), 10) + 5;
  spark->work = Math_RemainderUnsigned(Engine_RandomNext(), 60) + 30;
  spark->update = Sanctum_UpdateRisingSpark;
  sprite->flags = 0;
  sprite->image.priority = actor->sprite->image.priority;
}

/// Restores the two floor rectangles changed during the ceremony.
static void Sanctum_RestoreCeremonyFloor(void) {
  {
    Map_CopyCellsTo(0, 64, 11, 68, 3, 2);
  }
  {
    Map_CopyCellAttributes(11, 10, 3, 2, 11, 8);
  }
  Engine_TaskWait(1);
}

static s32 Sanctum_IsLeaderFacingHealer(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

  if ((u32)((actor->facing + 0x5fff) << 16) <= 0x3ffe0000) {
    return 1;
  }
  return 0;
}

/// Talking to the healer: healing from the front, otherwise words that follow
/// the story.
static void Event_TalkToHealer(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Engine_SanctumOpen(ACTOR_HEALER);
    return;
  }

  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0)
    Engine_EventSetMessage(MsgSanctumPathSolSanctumStillClosed);
  else if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
    Engine_EventSetMessage(MsgSanctumMyControlOverPsynergyHas);
  else
    Engine_EventSetMessage(MsgSanctumWieldersPsynergyCalledAdeptsAdepts);
  Engine_EventShowMessage(ACTOR_HEALER, 0);
  Engine_EventEnd();
}

/// Talking to the healer on a later visit: healing from the front, otherwise
/// words for this visit.
static void Event_TalkToHealerOnVisit(void) {
  s32 entrance;

  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Engine_SanctumOpen(ACTOR_HEALER);
    return;
  }

  Engine_EventBegin();

  entrance = gGameState.entrance;

  switch (entrance) {
  case 10:
  case 12:
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Engine_EventSetMessage(MsgSanctumWasHandFateReturnedGold);
    } else {
      Engine_EventSetMessage(MsgSanctumWeWillHelpAnytimeAs);
    }
    break;
  case 11:
    Engine_EventSetMessage(MsgSanctumMayWrongButLatelyThere);
    break;
  case 20:
  case 21:
  case 50:
    Engine_EventEnd();
    Event_ReconsiderQuest();
    return;
  default:
    break;
  }

  Engine_EventShowMessage(ACTOR_HEALER, 0);
  Engine_EventEnd();
}

/// Talking to this sanctum's healer: about the curse, lifted or not.
static void Event_TalkAboutCurse(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Engine_SanctumOpen(ACTOR_HEALER);
    return;
  }

  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0)
    Engine_EventSetMessage(MsgSanctumCurseMayOverButWe);
  else
    Engine_EventSetMessage(MsgSanctumAmStartingFeelOnlyBeginning);
  Engine_EventShowMessage(ACTOR_HEALER, 0);
  Engine_EventEnd();
}

/// Talking to this sanctum's healer: about the monsters, and the party's help
/// in Altin.
static void Event_TalkAboutAltinMonsters(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Engine_SanctumOpen(ACTOR_HEALER);
    return;
  }

  Engine_EventBegin();
  Engine_EventSetMessage(MsgSanctumWasAfterEerieNightWhen);
  if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0)
    Engine_EventSetMessage(MsgSanctumSavedAltinFromMonstersClearly);
  Engine_EventShowMessage(ACTOR_HEALER, 0);
  Engine_EventEnd();
}

/// Talking to this sanctum's healer, come from Xian.
static void Event_TalkAboutJourneyFromXian(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Engine_SanctumOpen(ACTOR_HEALER);
    return;
  }

  Engine_EventBegin();
  Engine_EventSetMessage(MsgSanctumCameXianFromVeryDistant);
  Engine_EventShowMessage(ACTOR_HEALER, 0);
  Engine_EventEnd();
}

// clang-format off
u32 gSanctum_LeaderCircleScript[] = {
    SC_ARG(2),
    SC_SET(SW_SPEED, PIXELS(5) / 2),
    SC_SET(SW_ACCEL, PIXELS(1) / 2),
    SC_LABEL(1),
    SC_MOVE(PIXELS(204), 0, PIXELS(188)),
    SC_SYNC,
    SC_MOVE(PIXELS(216), 0, PIXELS(192)),
    SC_SYNC,
    SC_MOVE(PIXELS(236), 0, PIXELS(188)),
    SC_SYNC,
    SC_MOVE(PIXELS(240), 0, PIXELS(168)),
    SC_SYNC,
    SC_MOVE(PIXELS(236), 0, PIXELS(156)),
    SC_SYNC,
    SC_MOVE(PIXELS(216), 0, PIXELS(144)),
    SC_SYNC,
    SC_MOVE(PIXELS(204), 0, PIXELS(156)),
    SC_SYNC,
    SC_MOVE(PIXELS(192), 0, PIXELS(168)),
    SC_SYNC,
    SC_LOOP(4, SC_LABEL(1)),
    SC_ARG(1),
    SC_END,
};

u32 gSanctum_GaretCircleScript[] = {
    SC_ARG(2),
    SC_SET(SW_SPEED, PIXELS(5) / 2),
    SC_SET(SW_ACCEL, PIXELS(1) / 2),
    SC_LABEL(1),
    SC_MOVE(PIXELS(204), 0, PIXELS(156)),
    SC_SYNC,
    SC_MOVE(PIXELS(184), 0, PIXELS(144)),
    SC_SYNC,
    SC_MOVE(PIXELS(172), 0, PIXELS(156)),
    SC_SYNC,
    SC_MOVE(PIXELS(160), 0, PIXELS(168)),
    SC_SYNC,
    SC_MOVE(PIXELS(172), 0, PIXELS(188)),
    SC_SYNC,
    SC_MOVE(PIXELS(184), 0, PIXELS(192)),
    SC_SYNC,
    SC_MOVE(PIXELS(204), 0, PIXELS(188)),
    SC_SYNC,
    SC_MOVE(PIXELS(208), 0, PIXELS(168)),
    SC_SYNC,
    SC_LOOP(4, SC_LABEL(1)),
    SC_ARG(1),
    SC_END,
};

s32 gSanctum_ItemIconGrowScript[] = {
    SC_SET(SW_SCALE_X, Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 4),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 4),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 7 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 7 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 15 * Q16_ONE / 16),
    SC_SET(SW_SCALE_Y, 15 * Q16_ONE / 16),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(16),
    SC_SET(SW_SCALE_X, 11 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 11 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 9 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 11 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 11 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 9 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_END,
};

s32 gSanctum_ItemIconEndScript[] = {
    SC_WAIT(60),
    SC_FREE,
};

struct SceneEntrance gSanctum_Entrances[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 199, 0, 246, FACING_NORTH, 0, 64, 8, 336, 272, 0 },
    { 2, CONDITION_ALWAYS, 199, 0, 164, FACING_SOUTH, 0, 64, 8, 336, 272, 0 },
    { 10, CONDITION_ALWAYS, 199, 0, 246, FACING_NORTH, 0, 64, 8, 336, 272, 0 },
    { 11, CONDITION_ALWAYS, 199, 0, 246, FACING_NORTH, 0, 64, 8, 336, 272, 0 },
    { 12, CONDITION_ALWAYS, 199, 0, 164, FACING_SOUTH, 0, 64, 8, 336, 272, 0 },
    { 20, CONDITION_ALWAYS, 192, 0, 168, FACING_NORTH, 0, 128, 0, 320, 272, 0 },
    { 21, CONDITION_ALWAYS, 192, 0, 168, FACING_NORTH, 0, 128, 0, 320, 272, 0 },
    { 29, CONDITION_ALWAYS, 200, 0, 240, FACING_NORTH, 0, 128, 0, 320, 272, 0 },
    { 32, CONDITION_ALWAYS, 200, 0, 240, FACING_NORTH, 0, 128, 0, 320, 272, 0 },
    { 35, CONDITION_ALWAYS, 200, 0, 240, FACING_NORTH, 0, 128, 0, 320, 272, 0 },
    { SCENE_TABLE_END },
};

u32 gSanctum_Exits[] = {
    SCENE_EXITS(9),
    SCENE_EXIT(1, 4, 1),
    SCENE_EXIT(10, 20, 11),
    SCENE_EXIT(11, 23, 11),
    SCENE_EXIT(20, 9, 21),
    SCENE_EXIT(30, 5, 12),
    SCENE_EXIT(40, 75, 3),
    SCENE_EXIT(41, 30, 9),
    SCENE_EXIT(42, 72, 11),
    SCENE_EXIT(50, 1, 1),
    SCENE_EXITS_END,
};

struct ScenePlacement gSanctum_PlacementA[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gSanctum_PlacementB[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(130), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

#if TBS_PATCH_1
struct ScenePlacement gSanctum_PlacementSequenceB[] = {
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};
#endif

struct ScenePlacement gSanctum_PlacementC[] = {
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(130), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(200), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xdd, CONDITION_ALWAYS, 7,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSanctum_PlacementD[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(208), 0, PIXELS(168), FACING_NORTH + FACING_STEP, 3, 0 },
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(224), 0, PIXELS(216), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(198), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(208), FACING_NORTH + FACING_STEP, 3, 0 },
    { 0x75, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, 3, 0 },
    { 0xfc, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSanctum_PlacementE[] = {
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(132), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSanctum_SceneTableA[] = {
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealer) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSanctumOurPsynergyWasGivenToUs) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgSanctumPsynergyIsAPowerThatShould) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_SceneTableB[] = {
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealerOnVisit) },
    { EVENT_TALK, 9, FLAG_VAULT_THIEVES_JAILED, EVENT_SCRIPT(Event_TalkAboutGoldenStatue) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutGoldenStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumGoOnIfThatEasesThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindAboutGoldenStatue) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 10 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_SceneTableC[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 11 },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealerOnVisit) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutGoldenStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumIHopeTheEarthquakesArentA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindAboutGoldenStatue) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_SceneTableD[] = {
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReconsiderQuest) },
    { EVENT_TALK, GARET, CONDITION_ALWAYS, MSGNO(MsgSanctumItIsATerribleBurdenTo) },
    { EVENT_TALK, ACTOR_DORA, CONDITION_ALWAYS, MSGNO(MsgSanctumMotherWouldNeverDoubtYourCourage) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgSanctumIDontBlameYouICouldnt) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSanctumWasItRightToRefuseWhen) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgSanctumItsClearThatTheGreatHealer) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveAfterRefusing) },
    { EVENT_TOUCH, 2, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_WarnBeforeLeaving) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_SceneTableE[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealer) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSanctumThePowerOfMtAlephStems) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutPsynergyChanges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumThePowerOfMyPsynergyHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSanctumTheAngaranContinentWillCertainlySee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgSanctumMyPsynergyHasntChangedAtAll) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_SceneTableF[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealer) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutNewAdepts) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgSanctumWhyIsItThatDifferentAdepts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItllBeDifficultToReachSol) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSanctumISeeABeautifulGirlHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgSanctumItsNotFairIWantTo) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_SceneTableG[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 40 },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutAltinMonsters) },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutAltinMonsters) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgSanctumPleaseSaveTheHelplessInAltin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumAlmightyFateMustHaveHeardMe) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_SceneTableH[] = {
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutCurse) },
    { EVENT_TALK, 9, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgSanctumPleaseGiveMeStrengthAtTimes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgSanctumWillITurnIntoATree) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgSanctumSomeoneWillSaveUsJustKeep) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgSanctumHowAboutThatOurWishesHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumTheSignsOfDangerAreActually) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSanctumPeaceWillBeKeptAsLong) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 41 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_SceneTableI[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 42 },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutJourneyFromXian) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWhenYouStayInXianFor) },
    { SCENE_EVENTS_END },
};
// clang-format on

s32 gSanctum_CursorBob[] = {
    SC_WAIT(0),
    SC_WAIT(1),
    SC_SYNC,
    SC_SYNC,
    SC_POS(0x2, 0x3, 0x4),
    SC_MOVE(0x2, 0x2, 0x1),
    SC_SYNC,
    SC_SYNC,
};

s32 gSanctum_OwnerEffectScript[] = {
    SC_ADD(SW_Y, -0x8000),
    SC_ADD(SW_TARGET_Y, -0x8000),
    SC_WAIT(1),
    SC_LOOP(96, SC_TOP),
    SC_FREE,
};

s32 gSanctum_SparkEndScript[] = {
    SC_FREE,
};

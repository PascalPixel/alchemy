// Vale's storm, departure, and return scenes share this overlay with the bag
// scene.

#include "game_state.h"
#include "vram_tab.h"
#include "unit/scene/vale_babi.h"
#include "unit/scene/common/effect_psynergy.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "facing_object.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fxblend.h"
#include "map_scroll.h"
#include "field_event.h"
#include "field_scene.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "glyph.h"
#include "animspr.h"
#include "vram_block.h"
#include "b5_context.h"
#include "io_reg.h"
#include "call.h"
#include "field_effect.h"
#include "heap_state.h"
#include "ability_ids.h"
#include "sound_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "iowrite.h"
#include "field_obj.h"

// Entry setup selects the longer sequences defined after the touch handlers.
static void ValeBabi_RunIsaacsStormAwakening(void);
static void ValeBabi_RunBoulderWarning(void);
static void ValeBabi_RestoreRoomPatch(void);
static void ValeBabi_ShowMythrilBag(void);
static void ValeBabi_SpawnEffectPair(union FieldObject *object);
static void ValeBabi_DemonstrateCatch(void);

// Shared facing code retains its position before the overlay entry points.
#include "common/object_facing.h"

enum {

  ITEM_MYTHRIL_BAG = 224
};

extern struct SceneEvent gValeBabiEvents2[];

// Return the shared entrance table through this overlay's existing ABI.
s32 *ValeBabi_GetEntrances(void) { return gValeBabiEntrances; }

// The overlay has no clipping-region table.
s32 SceneData_ReturnZero(void) { return 0; }

// The storm changes which exits are available.
s32 ValeBabi_SelectExits(void) {
  if (Engine_GameFlagIsSet(0x834) != 0) {
    return (s32)gValeBabiExits2;
  }
  return (s32)gValeBabiExits;
}

// Select placements for the bag scene, storm, departure, or return to Vale.
s32 ValeBabi_SelectPlacements(void) {
  struct ScenePlacement *table;

  if (gGameState.entrance == 19)
    return (s32)gValeBabiPlacements4;
  if (Engine_GameFlagIsSet(0x87a) != 0)
    table = gValeBabiPlacements3;
  else if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
    table = gValeBabiPlacements2;
  else
    table = gValeBabiPlacements;
  FieldScene_PrepareActors(table);
  return (s32)table;
}

// Ask whether the party knows Kraden and discuss his travels.
static void ValeBabi_AskAboutKnowingKraden(s32 speakerId) {
  s32 firstMessage = MsgValeFolksSeemKnow;

  Engine_EventSetMessage(firstMessage);
  Engine_EventOpenMessage(speakerId, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_EventSetMessage(firstMessage + 1);
  } else {
    Engine_EventSetMessage(firstMessage + 2);
  }
  Engine_EventShowMessage(speakerId, 0);
}

// Match the event table to the entrance and the party's story progress.
s32 ValeBabi_SelectEvents(void) {
  if (gGameState.entrance == 19) {
    if (Engine_GameFlagIsSet(0x950) != 0) {
      return (s32)gValeBabiEvents6;
    }
    return (s32)gValeBabiEvents5;
  }

  if (Engine_GameFlagIsSet(0x834) != 0) {
    return (s32)gValeBabiEvents4;
  }
  if (Engine_GameFlagIsSet(0x87A) != 0) {
    return (s32)gValeBabiEvents3;
  }
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    return (s32)gValeBabiEvents2;
  }
  return (s32)gValeBabiEvents;
}

// Ask Isaac whether he has business with the speaker, then show the reply.
static void ValeBabi_AskIsaacHisBusiness(void) {
  Engine_EventBegin();
  Engine_ActorStartRepeatedMotion(16, 2);
  Engine_EventWait(30);
  Engine_EventSetMessage(MsgValeHeyBoy);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 16, 10);
  Event_ShowMessageAndWait(16, 0, 6);
  Actor_ShowEmote(16, 0x102, 0);
  Engine_ActorStartRepeatedMotion(16, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(16, 4);
  Engine_EventWait(20);
  Event_OpenMessage(16, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message++;
  }
  Engine_ActorStartRepeatedMotion(16, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(16, 0, 4);
  Engine_EventEnd();
}

// Discuss the masked man being Felix, with a follow-up question on refusal.
static void ValeBabi_AskWhetherMaskedManWasFelix(void) {
  struct EventWork *work;

  Engine_EventBegin();
  Engine_ActorFaceActor(0xE, ACTOR_PARTY_LEADER, 0xA);
  Engine_EventSetMessage(MsgValeTheMaskedManWasFelix);
  Engine_EventOpenMessage(0xE, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventShowMessage(0xE, 0);
  } else {
    work = gEventWork;
    work->message = (work->message + 1);
    Engine_EventAskYesNo(0xE, 0);
  }
  Engine_EventEnd();
}

// Stop storm lightning before fading through the requested exit.
static void ValeBabi_CloseScreenAndExit(s32 exitId) {
  if (Engine_GameFlagIsSet(0x834) != 0) {
    FieldFx_StopLightning();
  }
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(exitId);
}

// Play the exit cue before leaving through trigger 1.
static void ValeBabi_ExitNorthwestArea(void) {
  Engine_AudioPlayCue(SOUND_MAP_EXIT);
  ValeBabi_CloseScreenAndExit(1);
}

// Play the exit cue before leaving through trigger 2.
static void ValeBabi_ExitNorthArea(void) {
  Engine_AudioPlayCue(SOUND_MAP_EXIT);
  ValeBabi_CloseScreenAndExit(2);
}

// Play the exit cue before leaving through trigger 3.
static void ValeBabi_ExitNortheastArea(void) {
  Engine_AudioPlayCue(SOUND_MAP_EXIT);
  ValeBabi_CloseScreenAndExit(3);
}

// Play the exit cue before leaving through trigger 4.
static void ValeBabi_ExitWestAreaSouth(void) {
  Engine_AudioPlayCue(SOUND_MAP_EXIT);
  ValeBabi_CloseScreenAndExit(4);
}

// Play the alternate exit cue before leaving through trigger 5.
static void ValeBabi_ExitWestAreaNorth(void) {
  Engine_AudioPlayCue(0x80);
  ValeBabi_CloseScreenAndExit(5);
}

// Play the exit cue before leaving through trigger 6.
static void ValeBabi_ExitCenterAreaSouth(void) {
  Engine_AudioPlayCue(SOUND_MAP_EXIT);
  ValeBabi_CloseScreenAndExit(6);
}

// Play the alternate exit cue before leaving through trigger 7.
static void ValeBabi_ExitCenterAreaNorth(void) {
  Engine_AudioPlayCue(0x80);
  ValeBabi_CloseScreenAndExit(7);
}

// Play the exit cue before leaving through trigger 8.
static void ValeBabi_ExitSouthwestArea(void) {
  Engine_AudioPlayCue(0x81);
  ValeBabi_CloseScreenAndExit(8);
}

// Play the exit cue before leaving through trigger 9.
static void ValeBabi_ExitSouthAreaWest(void) {
  Engine_AudioPlayCue(0x81);
  ValeBabi_CloseScreenAndExit(9);
}

// Play the exit cue before leaving through trigger 10.
static void ValeBabi_ExitSouthAreaSouth(void) {
  Engine_AudioPlayCue(SOUND_MAP_EXIT);
  ValeBabi_CloseScreenAndExit(10);
}

// Play the exit cue before leaving through trigger 11.
static void ValeBabi_ExitSoutheastArea(void) {
  Engine_AudioPlayCue(SOUND_MAP_EXIT);
  ValeBabi_CloseScreenAndExit(11);
}

// Restore story-dependent actors, storm effects, and the selected arrival
// scene.
s32 ValeBabi_RestoreEntryState(void) {
  struct FieldActor *actor;

  if (gGameState.entrance == 19) {
    Engine_GameFlagClear(0x12f);
    ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  } else {
    if (Engine_GameFlagIsSet(0x834) != 0) {
      Engine_ActorSetPosition(11, 0, 0);
      Engine_ActorSetPosition(12, 0, 0);
      Engine_ActorSetPosition(13, 0, 0);
      Engine_ActorSetPosition(14, 0, 0);
      Engine_ActorSetPosition(15, 0, 0);
      Engine_ActorSetPosition(16, 0, 0);
    } else {
      ValeBabi_RestoreRoomPatch();
    }
    Engine_ActorSetSpritePriority(13, 1);
    if (Engine_GameFlagIsSet(0x87a) != 0) {
      actor = Object_GetById(17);
      Engine_ActorSetSpriteFlags(actor, 0);
      if (gGameState.entrance == 6 || gGameState.entrance == 7) {
        if (Engine_GameFlagIsSet(0x109) != 0) {
          if (Engine_GameFlagIsSet(0x203) != 0) {
            Map_ClearLayerEntryFlag(12);
          }
        } else {
          Map_ClearLayerEntryFlag(11);
          actor = Object_GetById(8);
          Engine_ActorSetSpriteFlags(actor, 0);
          Engine_ActorSetAnimation(8, 10);
        }
      }
    } else {
      if (gGameState.entrance == 21) {
        ValeBabi_RunBoulderWarning();
      } else {
        if (gGameState.entrance == 20) {
          Engine_GameFlagSet(0x834);
          ValeBabi_RunIsaacsStormAwakening();
        } else {
          if (gGameState.entrance == 22) {
            ValeBabi_ShowMythrilBag();
          } else {
            ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->startTransition =
                SCENE_TRANSITION(TRANSITION_WINDOW, 9);
            if (Engine_GameFlagIsSet(0x834) != 0) {
              FieldFx_StartLightning();
              ((struct FieldBlendWork *)WORK_SLOT(30))->loud = 1;
              FieldFx_LastLightning();
              Engine_TaskWait(30);
              Engine_EventOpenScreen();
              Engine_EventWaitForScreen();
              FieldFx_ResumeLightning();
            } else {
              Engine_MapRedraw();
              Engine_TaskWait(1);
            }
          }
        }
      }
    }
  }
  return 0;
}

// Wake Isaac, lead him outside into the storm, and send him toward the exit.
static void ValeBabi_RunIsaacsStormAwakening(void) {
  s32 base;
  struct FieldSprite *sprite;
  struct MapScrollWork *work;
  struct FieldActor *sceneActor;
  struct EventWork **control;

  control = (struct EventWork **)Ram_EventWork;
  /* FAKEMATCH: needed to match: stage the root reads before consuming
     the event record. Read in place (measured again 2026-10-13, with the
     once-used locals gone): 431 instructions for 430, differ 11 (map
     work first) or 13 (view centre first). */
  {
    struct EventWork *event = *control;

    work = *(struct MapScrollWork **)Ram_MapWork;
    sceneActor = event->viewCenter;
  }
  sprite = Object_GetById(17)->sprite;
  // Present the bedside conversation before revealing the wider storm scene.
  Engine_EventBegin();
  Engine_ActorSetPosition(11, 0, 0);
  Engine_ActorSetPosition(12, 0, 0);
  Engine_ActorSetPosition(13, 0, 0);
  Engine_ActorSetPosition(14, 0, 0);
  Engine_ActorSetPosition(15, 0, 0);
  Engine_ActorSetPosition(16, 0, 0);
  Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
  Engine_ActorSetAnimation(0, 18);
  sprite->rotation = 1365;
  Object_GetById(17)->motionFlags = 0;
  Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
  Engine_ActorSetPosition(17, 37748736, 42598400);
  Map_ClearLayerEntryFlag(7);
  Engine_ActorSetPosition(8, 34996224, 45088768);
  Graphics_EnableObjLayerAndCallbacks();
  Ui_SetRenderResultFromObject(8);
  base = MsgValeWake;
  UiText_ShowCenteredMessage(base, 1, 0);
  Engine_EventWait(40);
  MapRender_SetValues(65536, 65536, 65536);
  Ui_SetRenderResultFromObject(8);
  UiText_ShowCenteredMessage(base + 1, 1, 0);
  ObjectDispatch_StopCallbacksAndHideLayers();
  Engine_EventWait(40);
  work->minX = 0x01480000;
  // Widen the camera bounds and reveal the lightning outside.
  work->minY = 0x02580000;
  work->maxX = 0x02700000;
  work->maxY = 0x03300000;
  sceneActor->x.fixed = 0x02340000;
  sceneActor->y.fixed = 0;
  sceneActor->z.fixed = 0x02b30000;
  Engine_MapRedraw();
  Engine_TaskWait(1);
  (*control)->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  (*control)->transitionFrames = 64;
  FieldFx_StartLightning();
  (*(struct FieldBlendWork **)((u8 *)control + Ram_SceneMapStateOffset))->loud =
      1;
  FieldFx_LastLightning();
  Engine_TaskWait(30);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  FieldFx_ResumeLightning();
  Engine_ActorSetAnimationAndWait(8, 4);
  Engine_EventSetMessage(base + 2);
  Event_ShowMessageAndWait(36872, 0, 60);
  Engine_ActorRunRepeatedMotion(0, 2);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(36872, 0, 20);
  Engine_ActorRunRepeatedMotion(0, 2);
  Map_SetLayerEntryFlag(7);
  Engine_EventWait(20);
  Map_ClearLayerEntryFlag(8);
  Actor_SetSpeed(0, 65536, 32768);
  Engine_ActorSetAnimation(0, 19);
  Actor_MoveToAndWait(0, 557, 679);
  Map_SetLayerEntryFlag(8);
  Map_ClearLayerEntryFlag(9);
  Actor_MoveToAndWait(0, 555, 680);
  Engine_EventWait(30);
  Actor_FaceDirection(8, 53248, 0);
  Engine_ActorSetSpriteFlags(Object_GetById(0), 1);
  Engine_ActorJump(0, 4, 0);
  Actor_WalkToAndWait(0, 543, 674);
  Engine_ActorSetSpritePriority(0, 3);
  Actor_FaceDirection(0, 16384, 40);
  Engine_ActorSetAnimationAndWait(8, 4);
  Engine_EventWait(20);
  Event_ShowMessage(36872, 0);
  ValeBabi_DemonstrateCatch();
  // Resolve the Psynergy demonstration before the pair walks away together.
  Engine_ActorStartRepeatedMotion(8, 2);
  Event_ShowMessageAndWait(36872, 0, 20);
  Object_GetById(8)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(8, 542, 680);
  Engine_EventWait(1);
  Object_GetById(8)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(8, 2);
  ObjectDispatch_RegisterChildMetadata(Object_GetById(0), 226);
  Engine_GameFlagSet(33);
  Engine_AudioPlayCue(126);
  Engine_ActorSetChildValue(0, 7);
  Engine_EventWait(10);
  Engine_ActorSetChildValue(0, 0);
  Engine_EventWait(20);
  Object_GetById(8)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(8, 534, 688);
  Engine_EventWait(1);
  Object_GetById(8)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(20);
  Actor_SetSpeed(8, 98304, 49152);
  Actor_SetSpeed(0, 98304, 49152);
  Engine_CameraFollowActor(8, 1);
  Object_GetById(0)->priorityFlags |= 0x1;
  Actor_SetActionScript(8, gValeBabiSharedAction);
  Engine_EventWait(20);
  Actor_SetActionScript(0, gValeBabiSharedAction);
  Object_RefreshSelectorById(8);
  Actor_WalkToAndWait(8, 419, 661);
  Actor_WalkToAndWait(8, 408, 661);
  Engine_ActorSetAnimation(8, 1);
  Engine_ActorSetAnimation(0, 1);
  Actor_FaceDirection(8, 16384, 10);
  Event_OpenMessage(32776, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    (*control)->message++;
  }
  Engine_EventWait(20);
  Event_ShowMessageAndWait(32776, 0, 20);
  Engine_ActorSetAnimation(0, 3);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Actor_SetActionScript(8, gValeBabiActorExitAction);
  Actor_SetActionScript(0, gValeBabiLeaderExitAction);
  Engine_EventWait(20);
  (*control)->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  (*control)->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(20);
}

// Select the hardware blend targets for the storm sequence.
static __inline__ void SetBlendTarget(u32 value) { REG_BLDCNT = value; }

// Set the packed hardware alpha coefficients.
static __inline__ void SetBlendAlpha(u32 value) { REG_BLDALPHA = value; }

union PairObject {
  union FieldObject object;
  s32 words[28];
  struct {
    u8 unk00[0x68];
    union PairObject *parent;
  } link;
};

struct PairWork {
  u8 unk00[70];
  u16 vramBlock;
};

// Run the boulder warning and collapse, fading its sprite through alpha
// blending.
static void ValeBabi_RunBoulderWarning(void) {
  struct FieldActor *actor;
  struct FieldSprite *sprite;
  u32 i;
  volatile u16 *alpha;

  actor = Object_GetById(10);
  sprite = actor->sprite;
  Engine_EventBegin();
  Engine_ActorSetPosition(11, 0, 0);
  Engine_ActorSetPosition(12, 0, 0);
  Engine_ActorSetPosition(13, 0, 0);
  Engine_ActorSetPosition(14, 0, 0);
  Engine_ActorSetPosition(15, 0, 0);
  Engine_ActorSetPosition(16, 0, 0);
  Actor_SetPosition(8, PIXELS(431), PIXELS(391));
  Engine_ActorSetPosition(10, PIXELS(463), PIXELS(404));
  Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  actor->motionFlags = 0;
  sprite->priority = 1;
  Actor_SetActionScript(10, gValeBabiRampActor10Action);
  ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Map_CopyCellsTo(83, 15, 83, 19, 5, 4);
  Map_CopyCellsTo(90, 16, 90, 20, 5, 4);
  Map_CopyCellsTo(77, 23, 82, 23, 5, 7);
  Map_CopyCellsTo(83, 33, 85, 33, 2, 2);
  Map_CopyCellsTo(91, 28, 90, 28, 1, 1);
  Map_CopyCellsTo(91, 28, 88, 30, 1, 1);
  Map_CopyCellsTo(94, 27, 94, 23, 6, 4);
  Map_CopyCellsTo(92, 28, 87, 23, 4, 4);
  Map_CopyCellsTo(65, 53, 88, 24, 2, 2);
  DisplayBlend_EnableRunScript();
  SetBlendTarget(0x3f42);
  Io_Write16(0x100c, &REG_BLDALPHA);
  FieldFx_StartLightning();
  ((struct FieldBlendWork *)WORK_SLOT(30))->loud = 1;
  FieldFx_LastLightning();
  Engine_TaskWait(30);
  Engine_CameraFollowActor(8, 1);
  Actor_SetSpeed(8, 0x18000, 0xc000);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
  Actor_SetSpeed(9, 0x18000, 0xc000);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gValeBabiRampLeaderAction);
  Actor_SetActionScript(8, gValeBabiRampActor8Action);
  Engine_EventOpenScreen();
  Object_RefreshSelectorById(8);
  Engine_AudioPlayCue(158);
  Actor_ShowEmote(8, 0x100, 0);
  Engine_ActorRunRepeatedMotion(8, 2);
  Actor_FaceDirection(8, FACING_SOUTH, 10);
  Camera_SetSpeed(0x40000, 0x8000);
  Camera_MoveTo(PIXELS(414), -1, PIXELS(530), 1);
  Actor_SetPosition(9, PIXELS(414), PIXELS(530));
  Engine_ActorWalkToAndWait(9, 427, 483);
  Engine_CameraWaitForMove();
  Engine_EventSetMessage(MsgValeDoraHurryBoulder);
  Engine_EventShowMessageAndWait(32777, 0, 10);
  Engine_CameraSetSpeed(0x18000, 0x3000);
  Engine_CameraMoveTo(PIXELS(480), -1, PIXELS(444), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_FaceDirection(8, FACING_WEST, 20);
  Engine_ActorSetAnimation(8, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(9, 3);
  Engine_ActorWalkTo(9, 415, 589);
  Engine_EventWait(10);
  Actor_SetActionScript(8, gValeBabiRampActor8ActionB);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gValeBabiRampLeaderActionB);
  Engine_AudioPlayCue(234);
  Engine_EventWait(20);
  Actor_SetActionScript(10, gValeBabiRampActor10ActionB);
  alpha = &REG_BLDALPHA;

  // Bring the boulder fully into view before fading it away.
  i = 0;
ramp:
  *alpha = 0x100e + i;
  Engine_TaskWait(1);
  if (++i <= 3)
    goto ramp;
  Engine_AudioPlayCue(202);
  Engine_TaskWait(10);
  for (i = 0; i <= 15; i++) {
    SetBlendAlpha(0x100f - i);
    Engine_TaskWait(1);
  }
  Object_RefreshSelectorById(0);
  Engine_ActorSetAnimation(8, 1);
  Engine_ActorStartRepeatedMotion(8, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(8, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Actor_SetAttachedEffect(8, 0x102);
  Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Engine_EventWait(80);
  Engine_ActorSetPosition(9, 0, 0);
  Engine_ActorFaceEachOther(8, 0, 20);
  Engine_ActorSetAnimation(8, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(40);
  Engine_CameraSetSpeed(0xcccc, 0x1999);
  Engine_CameraFollowActor(8, 1);
  Actor_SetActionScript(8, gValeBabiRampFinalAction);
  Object_SetActionCallbackAndRefreshById(ACTOR_PARTY_LEADER,
                                         gValeBabiRampFinalAction);
  ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->startTransition =
      SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->transitionFrames = 32;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(21);
}

// Open inn service from the counter, or discuss Vale's current circumstances.
static void ValeBabi_RunInnkeeperTalk(void) {
  struct FieldActor *actor;

  actor = Object_GetById(0);
  if ((u32)(actor->facing + -0x2000) > 0x9000) {
    Engine_InnOpen(0, 13);
  } else {
    Engine_EventBegin();
    if (GameFlag_IsSet(0x87a) != 0) {
      Engine_ActorRunRepeatedMotion(13, 2);
      Actor_FaceActor(13, ACTOR_PARTY_LEADER, 10);
      if (GameFlag_IsSet(0x300) == 0) {
        Engine_EventSetMessage(MsgValeCameBack2);
        Event_ShowMessage(13, 0);
        GameFlag_Set(0x300);
      }
      Engine_EventSetMessage(MsgValeHomeJustToStay);
      Event_AskYesNo(13, 0);
      Actor_FaceDirection(13, 0x9000, 10);
    } else {
      if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Engine_EventSetMessage(MsgValeTheVisitorsCausedTheEruption);
      } else {
        Engine_EventSetMessage(MsgValeTheThreeTravelersSeemOdd);
      }
      Event_ShowMessage(13, 0);
    }
    Engine_EventEnd();
  }
}

// Acknowledge how much Isaac has grown and record the conversation.
static void ValeBabi_GreetIsaacOnHisReturn(void) {
  Engine_EventBegin();
  Engine_ActorFaceActor(0x10, ACTOR_PARTY_LEADER, 0xA);
  Engine_EventSetMessage(MsgValeYouveGrownSoMuch);
  Engine_EventShowMessage(0x10, 0);
  Engine_ActorFaceDirection(0x10, 0xB000, 0xA);
  Engine_GameFlagSet(0x301);
  Engine_EventEnd();
}

// Mind Read learns that Dora would not let Isaac remain in Vale.
static void ValeBabi_ReadInnkeepersThoughtsAboutDora(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeDoraWouldntLetHimStay);
  Engine_EventShowMessage(0xD, 0);
  Engine_GameFlagSet(0x81C);
  Engine_EventEnd();
}

// Mind Read learns of Dora's illness before visiting her.
static void ValeBabi_ReadThoughtsAboutDorasIllness(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgValeDoraWasStruckWithIllness);
  Engine_EventShowMessage(0x10, 0);
  Engine_GameFlagSet(0x81C);
  Engine_EventEnd();
}

// Visit the ill Dora, answer her questions, and see her get out of bed.
static void ValeBabi_RunSickbedVisit(void) {
  struct FieldActor *record;
  s32 msg;

  Engine_EventBegin();
  Actor_SetSpeed(0, 0x9999, 0x4ccc);
  Actor_WalkToAndWait(0, 0x239, 0x189);
  Actor_FaceDirection(0, 0x4000, 40);
  Engine_ActorStartRepeatedMotion(8, 2);
  Engine_EventSetMessage(MsgValeCoughCoughWho);
  Engine_EventShowMessageAndWait(8, 0, 80);
  Actor_ShowEmote(8, 0x101, 60);
  Engine_ActorStartRepeatedMotion(8, 1);
  Engine_EventShowMessageAndWait(8, 0, 60);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(80);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Engine_ActorSetDestination(8, 0x248, 0x196);
  Map_SetLayerEntryFlag(11);
  Map_ClearLayerEntryFlag(12);
  Engine_ActorSetAnimation(8, 12);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(40);
  Engine_EventShowMessageAndWait(8, 0, 40);
  Actor_ShowEmote(8, 0x108, 40);
  Engine_EventShowMessageAndWait(8, 0, 40);
  Actor_ShowEmote(0, 0x105, 60);
  Engine_ActorSetAnimationAndWait(8, 13);
  Actor_ShowEmote(8, 0x103, 0);
  Engine_ActorSetAnimation(8, 11);
  Engine_EventWait(40);
  Engine_EventShowMessageAndWait(8, 0, 40);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(20);
  Engine_EventShowMessageAndWait(8, 0, 20);
  Engine_ActorSetAnimationAndWait(8, 12);
  Engine_EventWait(20);
  Actor_ShowEmote(0, 0x102, 60);
  Engine_ActorSetAnimation(8, 13);
  Engine_EventOpenMessage(8, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message += 1;
  }
  if (GameFlag_IsSet(0x81c) != 0) {
    Actor_ShowEmote(8, 0x102, 60);
  }
  Engine_EventWait(20);
  Engine_EventShowMessage(8, 0);
  Actor_ShowEmote(8, 0x107, 60);
  msg = MsgValeWhoToldIll;
  Engine_EventSetMessage(msg);
  Engine_EventOpenMessage(8, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message += 1;
  }
  if (Engine_GameFlagIsSet(0x81c) != 0) {
    Actor_ShowEmote(8, 0x102, 60);
  }
  Engine_EventWait(20);
  Engine_EventShowMessage(8, 0);
  Engine_ActorShowEmote(8, 0x107, 60);
  Engine_EventSetMessage(msg + 3);
  Engine_EventShowMessage(8, 0);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(8, 13);
  Engine_ActorStartRepeatedMotion(8, 2);
  Engine_EventShowMessageAndWait(8, 0, 40);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(20);
  Engine_EventShowMessageAndWait(8, 0, 40);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(40);
  record = Object_GetById(0);
  record->facing = 0;
  Engine_TaskWait(1);
  Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(0, 0x22e, 0x184);
  Actor_SetSpeed(8, 0x13333, 0x9999);
  Engine_ActorSetAnimation(8, 14);
  Engine_ActorMoveToAndWait(8, 0x24a, 0x190);
  Engine_EventWait(40);
  Actor_WalkToAndWait(8, 0x244, 0x17e);
  Actor_FaceDirection(8, 0x8000, 40);
  Object_GetById(0)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ActorFaceDirection(8, 0xc000, 8);
  Engine_ActorFaceDirection(8, 0, 8);
  Actor_FaceDirection(8, 0x4000, 8);
  Actor_FaceDirection(8, 0x8000, 10);
  Engine_ActorJump(8, 4, 20);
  Engine_ActorJump(8, 6, 40);
  Engine_ActorJump(8, 4, 20);
  Engine_EventShowMessageAndWait(8, 0, 40);
  Actor_SetSpeed(8, 0x6666, 0x3333);
  Actor_WalkToAndWait(8, 0x23c, 0x180);
  Engine_EventShowMessageAndWait(8, 0, 20);
  Engine_ActorSetAnimationAndWait(0, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_GameFlagSet(0x81e);
  Engine_GameFlagSet(0x203);
  Engine_EventEnd();
}

// Show Dora's sickbed dialogue or her reassurance after the visit.
static void ValeBabi_TalkToDoraAtHome(void) {
  s32 message;

  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x203) != 0) {
    Object_SetTargetAndCallback(8, 0x10000, (s32)gValeBabiActor8Departure);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgValeDontWorryBest);
    Engine_EventShowMessage(8, 0);
  } else {
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(40);
    message = MsgValeUnnOhhKyle;
    Engine_EventSetMessage(message);
    Engine_EventShowMessageAndWait(8, 0, 40);
    Engine_MessageShowCentered((message + 1), 1);
  }
  Engine_EventEnd();
}

// Restore the room's tiles and collision patch outside the storm sequence.
static void ValeBabi_RestoreRoomPatch(void) {
  Map_CopyCellsTo(22, 85, 25, 85, 2, 2);
  Map_CopyCellAttributes(25, 15, 2, 2, 25, 25);
}

// Display the Mythril Bag icon while its actor lifts and examines it.
static void ValeBabi_ShowMythrilBag(void) {
  struct FieldActor *actor;
  struct FieldSprite *sprite;
  GlyphTransfer *glyph;
  u8 clear;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(18, 0x1e00000, 0xca0000);
  Engine_TaskWait(1);
  Engine_CameraFollowActor(18, 1);
  clear = 0;
  actor = Engine_ObjectCreate(22, 0x1480000, 0x20000, 0xc30000);
  actor->motionFlags = clear;
  sprite = actor->sprite;
  actor->y.fixed = 0x50000;
  sprite->partCount = clear;
  sprite->fullColor = 0;
  sprite->palette = 0;
  glyph = Engine_HeapAllocate(17, sizeof(*glyph));
  Engine_ItemLoadIcon(ITEM_MYTHRIL_BAG);
  Engine_VramLoad(sprite->vramBlock, 128, glyph->tiles);
  Engine_HeapRelease(17);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventOpenScreen();
  Actor_SetSpeed(18, 0x10000, 0x8000);
  Actor_WalkToAndWait(18, 0x1e0, 176);
  Actor_WalkToAndWait(18, 0x1a4, 164);
  Actor_WalkToAndWait(18, 0x146, 185);
  Actor_FaceDirection(18, 0x4000, 10);
  Engine_ObjectSetScript(actor, gValeBabiBagLiftScript);
  ObjectDispatch_WaitForValue16(actor);
  Object_SetScript(actor, gValeBabiBagShowScript);
  ObjectDispatch_WaitForValue16(actor);
  Engine_EventWait(20);
  Engine_ObjectDispatchRelease(actor);
  Engine_ActorJump(18, 2, 20);
  Actor_FaceDirection(18, 0, 40);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(22);
}

// Flash between two palettes and emit a paired effect every sixteen frames.
static void ValeBabi_FlashPaletteAndSpawnPair(union FieldObject *object) {
  if ((gFrameCount & 2) != 0) {
    Engine_ObjectSetPartPalettes(&object->actor, 7);
  } else {
    Engine_ObjectSetPartPalettes(&object->actor, 0);
  }
  if ((gFrameCount & 15) == 0) {
    ValeBabi_SpawnEffectPair(object);
  }
}

// Cycle six palettes on odd frames and periodically emit paired effects.
static void ValeBabi_CyclePaletteAndSpawnPair(union FieldObject *object) {
  if ((gFrameCount & 1) != 0) {
    Engine_ObjectSetPartPalettes(&object->actor, (gFrameCount >> 1) % 6);
  }
  if ((gFrameCount & 15) == 0) {
    ValeBabi_SpawnEffectPair(object);
  }
}

// Advance deterministically through six palettes on odd frames.
static void ValeBabi_CyclePaletteOnOddFrames(union FieldObject *object) {
  if ((gFrameCount & 1) != 0) {
    s32 slot = (gFrameCount >> 1) % 6;

    Engine_ObjectSetPartPalettes(&object->actor, slot);
  }
}

// Shared vertical-effect callbacks precede their paired-effect creator.
#include "common/effect_vertical.h"

// Copy the parent's terrain height into two oppositely moving vertical effects.
static void ValeBabi_SpawnEffectPair(union FieldObject *object) {
  union PairObject *parent = (union PairObject *)object;
  union PairObject *pair[2];
  union PairObject *child;
  struct AnimationObject *part;
  struct FieldSprite *sprite;
  struct PairWork *work = gEffectWork;
  s32 i;

  for (i = 0; i < 2; ++i) {
    child = (union PairObject *)Engine_ObjectCreate(
        26, parent->object.actor.x.fixed, parent->object.actor.y.fixed,
        parent->object.actor.z.fixed);
    pair[i] = child;
    if (child != NULL) {
      child->words[5] = parent->words[5];
      part = (struct AnimationObject *)child->object.actor.sprite;
      child->object.actor.motionFlags = 0;
      child->object.effect.spin = 0;
      child->link.parent = parent;
      if (part != NULL) {
        sprite = (struct FieldSprite *)part;
        Object_InitializeMode(part, 0);
        sprite->flags = 0;
        Resource_ResetEntry(sprite->vramBlock);
        sprite->vramBlock = work->vramBlock;
        sprite->unk1d0 = 1;
        sprite->tile = (gVramBlockCache[sprite->vramBlock].offset >> 5) & 0x3ff;
        sprite->fullColor = 0;
        sprite->shape = 1;
        part->part[0].size = 2;
        part->entries[0]->frame = 0;
      }
    }
  }
  {
    union PairObject *p = pair[0];
    struct FieldSprite *sp = p->object.actor.sprite;

    p->object.actor.update = Effect_AnimateVerticalNegative;
    sp->priority = 2;
  }
  {
    struct FieldActor *p = &pair[1]->object.actor;
    struct FieldSprite *sp = p->sprite;

    sp->priority = 2;
    p->update = Effect_AnimateVerticalPositive;
    p->priorityFlags = 2;
  }
}

// Shared Psynergy setup precedes the scene's effect wrappers.
#include "common/effect_psynergy.h"

// Apply the cycling-palette update to actor 8.
static void ValeBabi_CyclePaletteWithParticles(void) {
  ValeBabi_CyclePaletteAndSpawnPair((union FieldObject *)Object_GetById(8));
}

// Apply the cycling-palette update to actor 17.
static void ValeBabi_CyclePaletteWithoutParticles(void) {
  ValeBabi_CyclePaletteOnOddFrames((union FieldObject *)Object_GetById(17));
}

// Demonstrate Catch on actor 8, then release its temporary effect objects.
static void ValeBabi_DemonstrateCatch(void) {
  Engine_PsynergyBegin(ABILITY_CATCH, 1);
  Engine_PsynergySetTarget(8, 0x11);
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_PsynergyLowerHands();
  BattleEffect_CleanupSceneObjects();
}

s32 gValeBabiSharedAction[] = {
    SC_ARG(2), SC_MOVE(PIXELS(522), 0, PIXELS(748)),
    SC_SYNC,   SC_MOVE(PIXELS(419), 0, PIXELS(748)),
    SC_SYNC,   SC_MOVE(PIXELS(419), 0, PIXELS(691)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeBabiActorExitAction[] = {
    SC_ARG(2), SC_MOVE(PIXELS(387), 0, PIXELS(661)), SC_SYNC, SC_POS(0, 0, 0),
    SC_END,
};

s32 gValeBabiLeaderExitAction[] = {
    SC_ARG(2), SC_MOVE(PIXELS(408), 0, PIXELS(661)),
    SC_SYNC,   SC_MOVE(PIXELS(387), 0, PIXELS(661)),
    SC_SYNC,   SC_POS(0, 0, 0),
    SC_END,
};

s32 gValeBabiRampActor8Action[] = {
    SC_ARG(2), SC_MOVE(PIXELS(468), 0, PIXELS(390)),
    SC_SYNC,   SC_MOVE(PIXELS(508), 0, PIXELS(402)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeBabiRampLeaderAction[] = {
    SC_ARG(2), SC_MOVE(PIXELS(431), 0, PIXELS(391)),
    SC_SYNC,   SC_MOVE(PIXELS(468), 0, PIXELS(390)),
    SC_SYNC,   SC_MOVE(PIXELS(486), 0, PIXELS(393)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeBabiRampActor8ActionB[] = {
    SC_ARG(2), SC_MOVE(PIXELS(525), 0, PIXELS(414)),
    SC_SYNC,   SC_MOVE(PIXELS(509), 0, PIXELS(442)),
    SC_SYNC,   SC_MOVE(PIXELS(455), 0, PIXELS(450)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeBabiRampLeaderActionB[] = {
    SC_ARG(2), SC_MOVE(PIXELS(525), 0, PIXELS(414)),
    SC_SYNC,   SC_MOVE(PIXELS(509), 0, PIXELS(442)),
    SC_SYNC,   SC_MOVE(PIXELS(481), 0, PIXELS(443)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeBabiRampFinalAction[] = {
    SC_ARG(2), SC_MOVE(PIXELS(428), 0, PIXELS(460)),
    SC_SYNC,   SC_MOVE(PIXELS(415), 0, PIXELS(513)),
    SC_SYNC,   SC_MOVE(PIXELS(415), 0, PIXELS(589)),
    SC_SYNC,   SC_END,
};

s32 gValeBabiRampActor10Action[] = {
    SC_SET(SW_SCALE_X, 0xe666),  SC_SET(SW_SCALE_Y, 0x10000), SC_WAIT(4),
    SC_SET(SW_SCALE_X, 0xcccc),  SC_SET(SW_SCALE_Y, 0xcccc),  SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gValeBabiRampActor10ActionB[] = {
    SC_ARG(2),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x13333),
    SC_WAIT(3),
    SC_SET(SW_SCALE_X, 0xcccc),
    SC_SET(SW_SCALE_Y, 0xcccc),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 1310),
    SC_ADD(SW_SCALE_Y, 1310),
    SC_WAIT(1),
    SC_LOOP(8, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_Y, -0x1999),
    SC_ADD(SW_TARGET_Y, -0x1999),
    SC_ADD(SW_SCALE_X, -655),
    SC_ADD(SW_SCALE_Y, 2621),
    SC_WAIT(1),
    SC_LOOP(14, SC_TO(2)),
    SC_SET(SW_Y, PIXELS(8)),
    SC_SET(SW_TARGET_Y, PIXELS(8)),
    SC_LABEL(3),
    SC_ADD(SW_Y, 0x7333),
    SC_ADD(SW_TARGET_Y, 0x7333),
    SC_ADD(SW_SCALE_X, 65),
    SC_ADD(SW_SCALE_Y, -1310),
    SC_WAIT(1),
    SC_LOOP(40, SC_TO(3)),
    SC_POS(0, 0, 0),
    SC_ARG(1),
    SC_END,
};

s32 gValeBabiActor8Departure[] = {
    SC_CALL(OverlayObject_UpdateFacingTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_MOVE(PIXELS(567), 0, PIXELS(690)),
    SC_SYNC,
    SC_MOVE(PIXELS(543), 0, PIXELS(674)),
    SC_SYNC,
    SC_END,
};

s32 gValeBabiBagLiftScript[] = {
    SC_ADD(SW_Y, 0x8000),
    SC_WAIT(1),
    SC_LOOP(16, SC_TOP),
};

s32 gValeBabiBagShowScript[] = {
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_END,
};

struct SceneEntrance gValeBabiEntrances[] = {
    {0, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 160, 0, 228, FACING_NORTH, 0, 24, 0, 248, 256, 0},
    {2, CONDITION_ALWAYS, 480, 0, 176, FACING_NORTH, 0, 264, 8, 560, 240, 0},
    {3, CONDITION_ALWAYS, 721, 0, 192, FACING_NORTH, 0, 584, 0, 848, 224, 0},
    {4, CONDITION_ALWAYS, 144, 0, 494, FACING_NORTH, 0, 8, 256, 320, 528, 0},
    {5, CONDITION_ALWAYS, 119, 0, 340, FACING_SOUTH, 0, 8, 256, 320, 528, 0},
    {6, CONDITION_ALWAYS, 416, 0, 528, FACING_NORTH, 0, 344, 272, 640, 560, 0},
    {7, CONDITION_ALWAYS, 408, 0, 371, FACING_SOUTH, 0, 344, 272, 640, 560, 0},
    {8, CONDITION_ALWAYS, 184, 0, 633, FACING_EAST, 0, 8, 576, 320, 736, 0},
    {9, CONDITION_ALWAYS, 408, 0, 665, FACING_EAST, 0, 328, 600, 624, 816, 0},
    {10, CONDITION_ALWAYS, 528, 0, 683, FACING_SOUTH, 0, 328, 600, 624, 816, 0},
    {11, CONDITION_ALWAYS, 705, 0, 817, FACING_NORTH, 0, 640, 552, 952, 864, 0},
    {12, CONDITION_ALWAYS, 879, 0, 765, FACING_NORTH, 0, 640, 552, 952, 864, 0},
    {16, CONDITION_ALWAYS, 538, 0, 683, FACING_SOUTH, 0, 328, 600, 624, 816, 0},
    {19, CONDITION_ALWAYS, 760, 0, 328, FACING_SOUTH, 0, 672, 272, 960, 488, 0},
    {20, CONDITION_ALWAYS, 567, 0, 676, FACING_EAST, 0, 1024, 576, 1024, 576,
     0},
    {21, CONDITION_ALWAYS, 408, 0, 371, FACING_SOUTH, 0, 344, 272, 640, 560, 0},
    {22, CONDITION_ALWAYS, 480, 0, 176, FACING_NORTH, 0, 264, 8, 560, 240, 0},
    {SCENE_TABLE_END},
};

u32 gValeBabiExits[] = {
    SCENE_EXITS(8),          SCENE_EXIT(1, 5, 6),   SCENE_EXIT(2, 6, 3),
    SCENE_EXIT(3, 5, 2),     SCENE_EXIT(4, 5, 7),   SCENE_EXIT(5, 8, 8),
    SCENE_EXIT(6, 4, 8),     SCENE_EXIT(7, 8, 9),   SCENE_EXIT(8, 8, 5),
    SCENE_EXIT(9, 8, 7),     SCENE_EXIT(10, 3, 12), SCENE_EXIT(11, 3, 13),
    SCENE_EXIT(19, 138, 11), SCENE_EXIT(20, 8, 21), SCENE_EXIT(21, 3, 15),
    SCENE_EXIT(22, 6, 10),   SCENE_EXITS_END,
};

u32 gValeBabiExits2[] = {
    SCENE_EXITS(8),        SCENE_EXIT(1, 5, 6),   SCENE_EXIT(2, 6, 3),
    SCENE_EXIT(3, 5, 2),   SCENE_EXIT(4, 5, 7),   SCENE_EXIT(5, 8, 8),
    SCENE_EXIT(6, 3, 8),   SCENE_EXIT(7, 8, 9),   SCENE_EXIT(8, 8, 5),
    SCENE_EXIT(9, 8, 7),   SCENE_EXIT(10, 3, 12), SCENE_EXIT(11, 3, 13),
    SCENE_EXIT(20, 8, 21), SCENE_EXIT(21, 3, 15), SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gValeBabiPlacements[] = {
    { 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x35, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xd0, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6c, 0x0, ACTOR_STAND,
        PIXELS(151), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x78, 0x0, ACTOR_WANDER,
        PIXELS(87), 0, PIXELS(67), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x80, 0x0, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(391), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, 0x0, ACTOR_WANDER,
        PIXELS(159), 0, PIXELS(452), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(218), 0, PIXELS(373), FACING_EAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x22, 0x0, ACTOR_STAND,
        PIXELS(54), 0, PIXELS(669), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xe2, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTHEAST, TALK_KEEP_FACING, 0 },
    { 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeBabiPlacements2[] = {
    { 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x35, 0x0, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xd0, 0x0, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6c, 0x0, ACTOR_STAND,
        PIXELS(151), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x78, 0x0, ACTOR_WANDER,
        PIXELS(87), 0, PIXELS(67), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x80, 0x0, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(391), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6c, 0x0, ACTOR_WANDER,
        PIXELS(159), 0, PIXELS(452), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(218), 0, PIXELS(373), FACING_EAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeBabiPlacements3[] = {
    { 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(586), 0, PIXELS(406), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x35, 0x0, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xd0, 0x0, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6c, 0x0, ACTOR_STAND,
        PIXELS(151), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x78, 0x0, ACTOR_WANDER,
        PIXELS(87), 0, PIXELS(67), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x80, 0x0, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(391), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6c, 0x0, ACTOR_WANDER,
        PIXELS(159), 0, PIXELS(452), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(218), 0, PIXELS(373), FACING_EAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, 0x0, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(373), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x16, 0x0, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(378), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gValeBabiPlacements4[] = {
    { 0x70, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(792), 0, PIXELS(336), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x71, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(864), 0, PIXELS(360), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x70, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(904), 0, PIXELS(376), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x71, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(728), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(832), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gValeBabiEvents4[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitNorthwestArea) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitNorthArea) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitNortheastArea) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitWestAreaSouth) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitWestAreaNorth) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitCenterAreaSouth) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitCenterAreaNorth) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSouthwestArea) },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSouthAreaWest) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSouthAreaSouth) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSoutheastArea) },
    { SEARCH(SEARCH_WOODEN_BOX), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeWoodenBoxEmpty)) },
    { SEARCH(SEARCH_JAR), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { SEARCH(SEARCH_BARREL), 102, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultButDidntFindAnything)) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { 0xc4f3, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { 0xc4f3, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { SEARCH(SEARCH_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { 0xc4f3, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeBabiEvents[] = {
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeMyFatherAlsoDiedInThat) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeMySonIsStillYoungSo) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeOneOfTheMenFromThat) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgValeTwoOfThoseThreeGuestsStaying) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_AskIsaacHisBusiness) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_RunInnkeeperTalk) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitNorthwestArea) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitNorthArea) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitNortheastArea) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitWestAreaSouth) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitWestAreaNorth) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitCenterAreaSouth) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitCenterAreaNorth) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSouthwestArea) },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSouthAreaWest) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSouthAreaSouth) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSoutheastArea) },
    { SEARCH(SEARCH_WOODEN_BOX), 100, 0xf47, FIND_ITEM(0xb4) },
    { SEARCH(SEARCH_JAR), 101, 0xf48, FIND_COINS(5) },
    { SEARCH(SEARCH_BARREL), 102, 0xf49, FIND_COINS(1) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { 0xc4f3, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { 0xc4f3, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { SEARCH(SEARCH_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { 0xc4f3, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeBabiEvents2[] = {
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeComeBackAsAStrongWarrior) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeIWantedJennaToBeMy) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_AskWhetherMaskedManWasFelix) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgValeWereASmallHotelAndWe) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_RunInnkeeperTalk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgValeIWillThinkOfJennaAs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgValeNowThatJennaIsGoneWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgValeIWonderIfFelixChangedMuch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgValeFelixAndHisGangMustHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgValeWhoCouldHaveKnownThatMasked) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitNorthwestArea) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitNorthArea) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitNortheastArea) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitWestAreaSouth) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitWestAreaNorth) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitCenterAreaSouth) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitCenterAreaNorth) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSouthwestArea) },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSouthAreaWest) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSouthAreaSouth) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSoutheastArea) },
    { SEARCH(SEARCH_WOODEN_BOX), 100, 0xf47, FIND_ITEM(0xb4) },
    { SEARCH(SEARCH_JAR), 101, 0xf48, FIND_COINS(5) },
    { SEARCH(SEARCH_BARREL), 102, 0xf49, FIND_COINS(1) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { 0xc4f3, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { 0xc4f3, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { SEARCH(SEARCH_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { 0xc4f3, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeBabiEvents3[] = {
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeOwItsGonnaBeYearsBefore) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeWelcomeHomeIsaacOhWhatHappened) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgValeYouTraveledAroundTheEasternEdge) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgValeArentYouGladToSeeThe) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_RunInnkeeperTalk) },
    { EVENT_TALK, 16, 0x300, EVENT_SCRIPT(ValeBabi_GreetIsaacOnHisReturn) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgValeYouveGrownSoMuch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgValeIPromisedIdBecomeAGreat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgValeThatShyGirlWithIsaacSure) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgValeIWonderIfIsaacWillContinue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgValeValeWillNeverChangeSoYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40d, 0x300, EVENT_SCRIPT(ValeBabi_RunInnkeeperTalk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ReadInnkeepersThoughtsAboutDora) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x410, 0x301, EVENT_SCRIPT(ValeBabi_GreetIsaacOnHisReturn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ReadThoughtsAboutDorasIllness) },
    { EVENT_TALK, 8, 0x81e, EVENT_SCRIPT(ValeBabi_RunSickbedVisit) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_TalkToDoraAtHome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, 0x81e, EVENT_SCRIPT(ValeBabi_RunSickbedVisit) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x203, MSGNO(MsgValeKyleWatchOutTheBoulderThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgValeImSorryISeemedSoHarsh) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitNorthwestArea) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitNorthArea) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitNortheastArea) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitWestAreaSouth) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitWestAreaNorth) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitCenterAreaSouth) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitCenterAreaNorth) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSouthwestArea) },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSouthAreaWest) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSouthAreaSouth) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_ExitSoutheastArea) },
    { SEARCH(SEARCH_WOODEN_BOX), 100, 0xf47, FIND_ITEM(0xb4) },
    { SEARCH(SEARCH_JAR), 101, 0xf48, FIND_COINS(5) },
    { SEARCH(SEARCH_BARREL), 102, 0xf49, FIND_COINS(1) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { 0xc4f3, 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { 0xc4f3, 210, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { SEARCH(SEARCH_BOOKCASE), 211, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { 0xc4f3, 212, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { SEARCH(SEARCH_JAR), 108, 0x8ad, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_BARREL), 107, 0x8ae, FIND_COINS(123) },
    { SEARCH(SEARCH_UNNAMED), 109, 0x8af, FIND_PSYNERGY_STONE },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeBabiEvents5[] = {
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBabiOriginallyThereWereAncientRuinsAt) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgBabiNoMatterHowManyTimesI) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgBabiItIsSaidThatLongAgo) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgBabiOriginallyTheAncientVenusRuinsLay) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBabiBabiMakesHisScholarsPerformAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiNoOneHasEverReachedThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiStillMyExperimentsInAlchemyAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBabiKradensJourneyWasMeantToRevive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBabiTheVenusRuinsAreTheRemnants) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBabiLordBabiExpectedGreatResultsFrom) },
    { 0x21, 20, CONDITION_ALWAYS, 19 },
    { SEARCH(SEARCH_JAR), 103, 0xf9e, FIND_ITEM(0xc2) },
    { SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresABookEntitledEncyclopediaOf)) },
    { SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledExperimentsIn)) },
    { SEARCH(SEARCH_BOOKCASE), 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledStudyOf)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gValeBabiEvents6[] = {
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBabiOnceBabiLighthouseIsCompleteId) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(ValeBabi_AskAboutKnowingKraden) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgBabiAlchemyTeachesThatTheFourElements) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgBabiIfTheRiddleOfTheVenus) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBabiImSureKradenWouldStopBy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiTheRelationshipBetweenAlchemyAndThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiIWantToSeeKradenAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBabiTheresGotToBeAGood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBabiWhatSortOfUnfathomableRiddlesAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBabiThereMustBeAReasonWhy) },
    { 0x21, 20, CONDITION_ALWAYS, 19 },
    { SEARCH(SEARCH_JAR), 103, 0xf9e, FIND_ITEM(0xc2) },
    { SEARCH(SEARCH_BOOKCASE), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresABookEntitledEncyclopediaOf)) },
    { SEARCH(SEARCH_BOOKCASE), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledExperimentsIn)) },
    { SEARCH(SEARCH_BOOKCASE), 209, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledStudyOf)) },
    { SCENE_EVENTS_END },
};
// clang-format on

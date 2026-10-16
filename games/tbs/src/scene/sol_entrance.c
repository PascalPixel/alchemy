#include "game_state.h"
#include "unit/import.h"
#include "unit/scene/sol_entrance.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "sol_entrance.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_obj.h"

// Forward declarations.
static void SolEntrance_ApplyEntryState(void);
static void FieldScene_RunSceneEntryHook(void);
static void Scene_EnterSolSanctum(void);
static void Scene_KradenSuspectsHiddenPassage(void);
static void Scene_RunTransitionCue(void);
static void Scene_RunActorFormation(s32 a0);
static void Scene_UpdateCueTimer(void);

s32 gSolEntrance_CueTimer __attribute__((section(".bss")));

void InitializeSceneRecordBuffer();

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_SolEntrance2) {
    return gSolEntranceEntrances2;
  }
  if (scene == (s32)&SceneId_SolEntrance1) {
    return gSolEntranceEntrances1;
  }
  return gSolEntranceEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSolEntrance_Exits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  struct ScenePlacement *table;
  s32 lo = 11;

  if (gGameState.scene == (s32)&SceneId_SolEntrance1) {
    if (gGameState.entrance >= lo) {
      if (gGameState.entrance > 13) {
        if (gGameState.entrance <= 16) {
          return gSolEntrancePlacements1Entrances14To16;
        }
      } else {
        return gSolEntrancePlacements1Entrances11To13;
      }
    }
    table = gSolEntrancePlacements1;
    FieldScene_PrepareActors(table);
    return table;
  }
  if (gGameState.scene == (s32)&SceneId_SolEntrance2) {
    return gSolEntrancePlacements2;
  }
  return gSolEntrancePlacementsOther;
}

const struct SceneEvent *Scene_GetEvents(void) {
  s32 lo = 11;

  if (gGameState.scene == (s32)&SceneId_SolEntrance2) {
    return gSolEntranceEvents2;
  }
  if (gGameState.scene == (s32)&SceneId_SolEntrance1) {
    if (gGameState.entrance >= lo) {
      if (gGameState.entrance > 13) {
        if (gGameState.entrance <= 16) {
          return gSolEntranceEvents1Entrances14To16;
        }
      } else {
        return gSolEntranceEvents1Entrances11To13;
      }
    }
    return gSolEntranceEvents1;
  } else {
  }
  return gSolEntranceEventsOther;
}

static void EV02_TOUCH(void) {
  s32 v5;
  s32 v6;

  Engine_EventBegin();
  v5 = 3;
  v6 = 2;
  Event_PlayCue(181);
  Map_CopyCellsTo(16, 28, 21, 3, v5, v6);
  Engine_TaskWait(10);
  Map_CopyCellsTo(16, 30, 21, 3, v5, v6);
  Engine_TaskWait(10);
  Map_CopyCellsTo(16, 32, 21, 3, v5, v6);
  Engine_TaskWait(10);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 98);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
  Engine_EventWait(10);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(2);
  Engine_EventEnd();
}

static void EV20_SEARCH(void) {

  Engine_EventBegin();

  if (Engine_GameFlagIsSet(0x81a) != 0) {
    Engine_MessageShowCentered(MsgSolMinotaurReliefBothEyes, 1);
  } else {
    Engine_MessageShowCentered(MsgSolMinotaurReliefOneEye, 1);
    if (Engine_GameFlagIsSet(0xf01) != 0) {
      u16 *p = &gEventWork->unk172;
      u16 val = 1;
      *p = val;
    }
  }

  Engine_EventEnd();
}

static void EV1044_SCRIPT(void) {
  s32 id;
  s32 v5;
  s32 v6;

  if (GameFlag_IsSet(0xf01) == 0) {
  } else {
    if (GameFlag_IsSet(0x81a) != 0) {
    } else {
      Engine_EventBegin();
      Battle_ResetEffectCounter();
      v5 = 1;
      Event_PlayCue(182);
      Map_CopyCellsTo(0, 70, 30, 42, v5, v5);
      Engine_MapRedraw();
      Engine_EventWait(40);
      id = MsgSolSetSmallGem;
      Engine_MessageShowCentered(id, 1);
      Engine_EventWait(20);
      v6 = 3;
      Event_PlayCue(183);
      Map_CopyCellsTo(0, 29, 3, 1, v6, 2);
      Map_CopyCellAttributes(0, 29, 3, 2, v6, v5);
      Map_CopyCellsTo(1, 109, 4, 81, v5, v5);
      Engine_MapRedraw();
      Map_SetShake(0x10000, 0x10000, 0x10000);
      Engine_EventWait(20);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
      Map_SetShake(0x20000, 0x20000, 0x10000);
      Engine_EventWait(20);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 40);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
      Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 20);
      Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 40);
      Map_SetShake(-1, -1, 0xe666);
      Engine_EventWait(40);
      Engine_MessageShowCentered(id + 1, 1);
      GameFlag_Set(0x143);
      GameFlag_Set(0x81a);
      Engine_EventEnd();
    }
  }
}

static void EV21_SEARCH(void) {

  struct EventWork *work;

  Engine_EventBegin();

  if (Engine_GameFlagIsSet(0x821) != 0) {
    Engine_MessageShowCentered(MsgSolMinotaurReliefBothEyes, 1);
  } else if (Engine_GameFlagIsSet(0xf02) != 0) {
    work = gEventWork;
    Engine_MessageShowCentered(MsgSolMinotaurReliefOneEye, 1);
    {

      u16 *frame = &work->unk172;
      s32 one = 1;
      *frame = one;
    }
  } else {
    Engine_MessageShowCentered(MsgSolMinotaurReliefOneEye, 1);
  }

  Engine_EventEnd();
}

static void EV1045_SCRIPT(void) {
  s32 message;

  if (GameFlag_IsSet(3842) != 0 && GameFlag_IsSet(GATE_ID) == 0) {
    Engine_EventBegin();
    Battle_ResetEffectCounter();
    Engine_AudioPlayCue(182);
    Map_CopyCellsTo(0, 71, 100, 71, 1, 1);
    Engine_MapRedraw();
    Engine_EventWait(40);
    message = MsgSolSetSmallGem;
    Engine_MessageShowCentered(message, 1);
    Engine_EventWait(20);
    Engine_AudioPlayCue(183);
    Map_CopyCellsTo(122, 20, 120, 30, 1, 2);
    Map_CopyCellAttributes(122, 20, 1, 2, 120, 30);
    Engine_MapRedraw();
    Map_SetShake(65536, 65536, 65536);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
    Map_SetShake(131072, 131072, 65536);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 20);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 40);
    Map_SetShake(-1, -1, 58982);
    Engine_EventWait(40);
    Engine_MessageShowCentered(message + 1, 1);
    GameFlag_Set(0x143);
    GameFlag_Set(GATE_ID);
    Engine_EventEnd();
  }
}

static void Scene_UpdateOuterActor9Flags(void) {
  s32 *work = Object_GetById(9);
  s32 pos;

  if (work == 0) {
    return;
  }
  pos = work[2] >> 20;
  GameFlag_Clear(0x302);
  GameFlag_Clear(0x303);
  if (pos == 93) {
    Engine_GameFlagSet(0x303);
  } else if (pos == 95) {
    Engine_GameFlagSet(0x302);
  }
}

static void Scene_UpdateOuterActor10Flags(void) {
  s32 *work = Object_GetById(10);
  s32 pos;

  if (work == 0) {
    return;
  }
  pos = work[2] >> 20;
  GameFlag_Clear(0x300);
  GameFlag_Clear(0x301);
  if (pos == 115) {
    Engine_GameFlagSet(0x300);
  } else if (pos == 113) {
    Engine_GameFlagSet(0x301);
  }
}

static void Scene_UpdateFormationActor9Flags(void) {
  s32 *work = Object_GetById(9);
  s32 pos;

  if (work == 0) {
    return;
  }
  pos = work[2] >> 20;
  GameFlag_Clear(0x310);
  GameFlag_Clear(0x311);
  if (pos == 99) {
    Engine_GameFlagSet(0x311);
  } else if (pos == 101) {
    Engine_GameFlagSet(0x310);
  }
  Scene_RunActorFormation(0);
}

static void Scene_UpdateFormationActor10Flags(void) {
  s32 *work = Object_GetById(10);
  s32 pos;

  if (work == 0) {
    return;
  }
  pos = work[2] >> 20;
  GameFlag_Clear(0x312);
  GameFlag_Clear(0x313);
  if (pos == 103) {
    Engine_GameFlagSet(0x313);
  } else if (pos == 105) {
    Engine_GameFlagSet(0x312);
  }
  Scene_RunActorFormation(0);
}

static void EV11_PSYNERGY(void) {
  s32 *work = Object_GetById(11);
  s32 pos;

  if (work == 0) {
    return;
  }
  pos = work[2] >> 20;
  GameFlag_Clear(0x314);
  GameFlag_Clear(0x315);
  if (pos == 107) {
    Engine_GameFlagSet(0x315);
  } else if (pos == 109) {
    Engine_GameFlagSet(0x314);
  }
  Scene_RunActorFormation(0);
}

static void EV12_PSYNERGY(void) {
  s32 *work = Object_GetById(12);
  s32 pos;

  if (work == 0) {
    return;
  }
  pos = work[2] >> 20;
  GameFlag_Clear(0x316);
  GameFlag_Clear(0x317);
  if (pos == 111) {
    Engine_GameFlagSet(0x317);
  } else if (pos == 113) {
    Engine_GameFlagSet(0x316);
  }
  Scene_RunActorFormation(0);
}

static void EV13_PSYNERGY(void) {
  s32 *work = Object_GetById(13);
  s32 pos;

  if (work == 0) {
    return;
  }
  pos = work[2] >> 20;
  GameFlag_Clear(0x318);
  GameFlag_Clear(0x319);
  if (pos == 115) {
    Engine_GameFlagSet(0x319);
  } else if (pos == 117) {
    Engine_GameFlagSet(0x318);
  }
  Scene_RunActorFormation(0);
}

static void EV14_PSYNERGY(void) {
  s32 *work = Object_GetById(14);
  s32 pos;

  if (work == 0)
    return;
  pos = work[2] >> 20;
  GameFlag_Clear(0x31a);
  GameFlag_Clear(0x31b);
  if (pos == 119) {
    Engine_GameFlagSet(0x31b);
  } else if (pos == 121) {
    Engine_GameFlagSet(0x31a);
  }
  Scene_RunActorFormation(0);
}

// Shared source is included here in ROM order.
#include "common/object_findcell.h"

static void SolEntrance_PushFacedBlock(void) {
  struct FieldActor *leader;
  struct FieldActor *block;
  s32 zero;
  s32 step;
  u32 dir;
  union FieldCoordinate pos[3];
  union FieldCoordinate *p;

  leader = Object_GetById(0);
  dir = leader->facing >> 12;
  block = SceneActor_FindActorAtCell(
      (leader->x.part.pixel + (gSolPushSteps[dir] >> 16)) >> 4,
      (leader->z.part.pixel + (s16)gSolPushSteps[dir]) >> 4);
  if (block != NULL) {
    zero = 0;
    block->terrainId = 2;
    p = pos;
    step = gSolPushSteps[dir];
    p[0].fixed = block->x.fixed + (step & -0x10000);
    p[1].fixed = block->y.fixed;
    p[2].fixed = block->z.fixed + (step << 16);
    if (FieldObject_CheckMovementCollision(block, p) <= 0) {
      Object_SetMode(leader, 8);
      Engine_TaskWait(15);
      Engine_AudioPlayCue(185);
      block->speed = 0x3333;
      block->acceleration = 0x3333;
      Engine_ObjectSetPosition(block, p[0].fixed, p[1].fixed, p[2].fixed);
      leader->speed = 0x3333;
      leader->acceleration = 0x3333;
      Engine_ObjectSetPosition(leader, p[0].fixed, p[1].fixed, p[2].fixed);
      Engine_ObjectCommitPosition(block);
      block->x.fixed = p[0].fixed;
      block->z.fixed = p[2].fixed;
      block->velocityX = zero;
      block->velocityZ = zero;
      Object_SetMode(leader, 1);
      switch (gGameState.entrance) {
      case 11:
      case 12:
      case 13:
        Scene_UpdateOuterActor9Flags();
        Scene_UpdateOuterActor10Flags();
        break;
      case 14:
      case 15:
      case 16:
        Scene_UpdateFormationActor9Flags();
        Scene_UpdateFormationActor10Flags();
        EV11_PSYNERGY();
        EV12_PSYNERGY();
        EV13_PSYNERGY();
        EV14_PSYNERGY();
        break;
      }
    }
  }
}

s32 Scene_Initialize(void) {
  s32 scenario = gGameState.scene;

  if (scenario == (s32)&SceneId_SolEntrance2) {
    SolEntrance_ApplyEntryState();
  } else if (scenario == (s32)&SceneId_SolEntrance1) {
    FieldScene_RunSceneEntryHook();
  }
  return 0;
}

static void SolEntrance_ApplyEntryState(void) {
  GameFlag_Set(0x144);
  gEventWork->startTransition = 0x100;
  if (GameFlag_IsSet(0x814) != 0) {
    gSolEntrance_CueTimer = 0;
    Task_AddCallback(Scene_UpdateCueTimer, 0xc80);
  }
  if (GameFlag_IsSet(0x879) != 0) {
    Map_CopyCellAttributes(5, 6, 1, 1, 6, 6);
    Map_CopyCellAttributes(5, 6, 1, 1, 7, 6);
    Map_CopyCellAttributes(5, 6, 1, 1, 8, 6);
    Map_CopyCellAttributes(0, 1, 3, 1, 6, 5);
  }
  if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    Actor_SetPosition(8, PIXELS(120), PIXELS(232));
    Map_CopyCellAttributes(2, 10, 1, 1, 6, 14);
    Map_CopyCellAttributes(2, 10, 1, 1, 7, 14);
    Map_CopyCellAttributes(2, 10, 1, 1, 8, 14);
  }
}

static void FieldScene_RunSceneEntryHook(void) {
  s32 entrance;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (Engine_GameFlagIsSet(0x814) != 0) {
    BattleFx_SetQueuedSoundAndPlay(141);
    Map_SetShake(0x10000, 0x10000, 0x10000);
    InitializeSceneRecordBuffer();
  }

  entrance = gGameState.entrance;
  switch (entrance) {
  case 1:
  case 2:
    if (Engine_GameFlagIsSet(0x81a) != 0) {
      Map_CopyCellsTo(1, 109, 4, 81, 1, 1);
      Map_CopyCellsTo(0, 70, 30, 42, 1, 1);
      Map_CopyCellsTo(0, 29, 3, 1, 3, 2);
      Map_CopyCellAttributes(0, 29, 3, 2, 3, 1);
      Engine_MapRedraw();
    }
    break;

  case 3:
    Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
    break;

  case 8:
    gGameState.retreatScene = (s32)&SceneId_SolEntrance1;
    gGameState.retreatEntrance = 8;
    if (GameFlag_IsSet(0x802) == 0)
      Scene_EnterSolSanctum();
    break;

  case 11:
  case 12:
  case 13:
    Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(16), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(19), 0);
    if (Engine_GameFlagIsSet(0x804) == 0)
      Scene_KradenSuspectsHiddenPassage();
    if (Engine_GameFlagIsSet(0x303) != 0)
      Actor_SetPosition(9, PIXELS(1496), PIXELS(136));
    else if (GameFlag_IsSet(0x302) != 0)
      Actor_SetPosition(9, PIXELS(1528), PIXELS(136));
    if (Engine_GameFlagIsSet(0x301) != 0)
      Actor_SetPosition(10, PIXELS(1816), PIXELS(136));
    else if (Engine_GameFlagIsSet(0x300) != 0)
      Actor_SetPosition(10, PIXELS(1848), PIXELS(136));
    break;

  case 14:
  case 15:
  case 16:
    Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
    if (Engine_GameFlagIsSet(0x825) == 0)
      Scene_RunTransitionCue();
    Scene_RunActorFormation(1);
    Engine_GameFlagSet(0x234);
    if (GameFlag_IsSet(0x821) != 0) {
      Map_CopyCellsTo(0, 71, 100, 71, 1, 1);
      Map_CopyCellsTo(122, 20, 120, 30, 1, 2);
      Map_CopyCellAttributes(122, 20, 1, 2, 120, 30);
      Engine_MapRedraw();
    }
    break;
  }
}

static void Scene_EnterSolSanctum(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventOpenScreen();
  Engine_ActorSetAnimation(0, 0);
  Engine_EventWait(4);
  Camera_MoveTo(-1, -1, -1, 0);
  Camera_SetSpeed(0x9999, 0x1333);
  Camera_MoveTo(0x4c80000, -1, 0x880000, 1);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetPosition(ACTOR_KRADEN, record->x.fixed, record->z.fixed);
  }
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetPosition(JENNA, record->x.fixed, record->z.fixed);
  }
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetPosition(GARET, record->x.fixed, record->z.fixed);
  }
  Actor_SetSpeed(ACTOR_KRADEN, 0x9999, 0x4ccc);
  Actor_SetSpeed(JENNA, 0x9999, 0x4ccc);
  Actor_SetSpeed(GARET, 0x9999, 0x4ccc);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  Engine_ActorSetAnimation(JENNA, 2);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
  Actor_SetDestinationOffset(GARET, -16, 0);
  Actor_SetDestinationOffset(JENNA, 16, 0);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 0, -32);
  Engine_ActorWaitForMove(GARET);
  Engine_ActorSetAnimation(GARET, 0);
  Engine_ActorSetAnimation(JENNA, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(JENNA, FACING_NORTH, 0);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ActorJump(ACTOR_KRADEN, 4, 20);
  Engine_EventSetMessage(MsgSolKradenFirstTimeAtSol);
  Event_AskYesNo(0x4008, 0);
  Engine_EventWait(20);
  Camera_MoveTo(0x4c80000, -1, 0x940000, 1);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetDestination(GARET, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorSetAnimation(JENNA, 2);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetDestination(JENNA, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetDestination(ACTOR_KRADEN, record->x.part.pixel,
                         record->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetPosition(JENNA, 0, 0);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(JENNA, 1);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  GameFlag_Set(FLAG_SOL_SANCTUM_ENTERED);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
  Engine_EventEnd();
}

static void Scene_KradenSuspectsHiddenPassage(void) {
  u8 *record;

  Engine_EventBegin();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();

  record = (u8 *)Object_GetById(0);
  if (record != 0)
    Engine_ActorSetPosition(8, RECORD_A32(record), RECORD_B32(record));
  record = (u8 *)Object_GetById(0);
  if (record != 0)
    Engine_ActorSetPosition(5, RECORD_A32(record), RECORD_B32(record));
  record = (u8 *)Object_GetById(0);
  if (record != 0)
    Engine_ActorSetPosition(1, RECORD_A32(record), RECORD_B32(record));

  Actor_SetSpeed(ACTOR_KRADEN, 0x9999, 0x4ccc);
  Actor_SetSpeed(JENNA, 0x9999, 0x4ccc);
  Actor_SetSpeed(GARET, 0x9999, 0x4ccc);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  Engine_ActorSetAnimation(JENNA, 2);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
  Actor_SetDestinationOffset(GARET, -16, 0);
  Actor_SetDestinationOffset(JENNA, 16, 0);
  Actor_SetDestinationOffset(8, 0, -16);
  Engine_ActorWaitForMove(8);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  Engine_ActorSetAnimation(GARET, 0);
  Engine_ActorSetAnimation(JENNA, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 30);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 30);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 40);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
  Engine_ActorSetDestinationOffset(8, 0, -16);
  Engine_ActorWaitForMove(8);
  Engine_ActorSetAnimation(8, 1);
  Engine_EventWait(6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 40);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
  Engine_ActorSetDestinationOffset(8, 0, -32);
  Engine_ActorWaitForMove(8);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);

  Camera_SetSpeed(0x20000, 0x4000);
  Camera_MoveTo(0x06310000, -1, 0x00960000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Camera_SetSpeed(0x13333, 0x2666);
  Camera_MoveTo(0x06550000, -1, 0x00640000, 1);
  Engine_CameraWaitForMove();
  Camera_MoveTo(0x06b60000, -1, 0x00640000, 1);
  Engine_CameraWaitForMove();
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  Camera_MoveTo(0x06d80000, -1, 0x00960000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Camera_SetSpeed(0x26666, 0x4ccc);
  Camera_MoveTo(0x06840000, -1, 0x01000000, 1);
  Engine_CameraWaitForMove();
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(10);

  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 20);
  Engine_EventSetMessage(MsgSolWrongKraden);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | 2, 60);
  Engine_ActorStartRepeatedMotion(ACTOR_KRADEN, 2);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(5, 0x102);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Engine_ActorFaceEachOther(0, 5, 0);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorRunRepeatedMotion(5, 1);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 20);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Event_ShowMessageAndWait(JENNA, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 20);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(10);
  Actor_SetAttachedEffect(ACTOR_KRADEN, 0x102);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 60);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
  Engine_ActorJump(ACTOR_KRADEN, 2, 20);

  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 40);
  Actor_FaceDirection(GARET, FACING_EAST, 20);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgSolDangerousSplitStay);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventShowMessageAndWait(1, 0, 10);
  } else {
    Engine_EventSetMessage(MsgSolCantSeriouslyWant);
    Actor_FaceDirection(JENNA, FACING_WEST, 20);
    Event_ShowMessageAndWait(JENNA, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 40);
    Engine_ActorRunRepeatedMotion(1, 1);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(1, ANIM_NOD);
    Engine_EventWait(10);
    Actor_FaceDirection(GARET, FACING_EAST, 0);
    Actor_FaceDirection(JENNA, FACING_WEST, 30);
    Engine_ActorRunRepeatedMotion(1, 1);
    Engine_EventWait(10);
    Engine_EventShowMessageAndWait(1, 0, 10);
  }

  Actor_SetSpeed(ACTOR_KRADEN, 0x9999, 0x4ccc);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
  Engine_ActorSetDestinationOffset(8, 0, 48);
  Engine_ActorWaitForMove(8);
  Engine_ActorSetAnimation(8, 1);
  Engine_EventWait(6);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Engine_ActorFaceDirection(5, FACING_NORTHWEST, 0);
  Engine_ActorWaitForMove(8);
  Engine_ActorSetAnimation(8, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(JENNA, 3);
  Engine_ActorSetAnimationAndWait(0, ANIM_NOD);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(1, ANIM_WALK);

  record = (u8 *)Object_GetById(0);
  if (record != 0)
    Engine_ActorSetDestination(1, RECORD_A16(record), RECORD_B16(record));
  Engine_ActorSetAnimation(5, 2);
  record = (u8 *)Object_GetById(0);
  if (record != 0)
    Engine_ActorSetDestination(5, RECORD_A16(record), RECORD_B16(record));
  Engine_ActorSetAnimation(8, 2);
  record = (u8 *)Object_GetById(0);
  if (record != 0)
    Engine_ActorSetDestination(8, RECORD_A16(record), RECORD_B16(record));

  Engine_ActorWaitForMove(8);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetPosition(JENNA, 0, 0);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(5, 1);
  GameFlag_Set(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
  Engine_GameFlagClear(FLAG_ARRIVAL_EVENT_PENDING);
  Engine_EventEnd();
}

static void Scene_RunTransitionCue(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetPosition(8, record->x.fixed, record->z.fixed);
  }
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Engine_ActorSetAnimation(8, 2);
  Actor_SetDestinationOffset(8, 24, -10);
  Engine_ActorWaitForMove(8);
  Engine_ActorSetAnimation(8, 1);
  Engine_EventWait(6);
  Actor_FaceDirection(8, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Camera_SetSpeed(0x26666, 0x4ccc);
  Camera_MoveTo(0x6880000, -1, 0x20c0000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Camera_SetSpeed(0x19999, 0x3333);
  Camera_MoveTo(0x7580000, -1, 0x20c0000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Camera_SetSpeed(0x33333, 0x6666);
  Camera_MoveTo(0x6e90000, -1, 0x2240000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(8, 2);
  Actor_FaceDirection(8, FACING_EAST, 30);
  Engine_EventSetMessage(MsgSolMoreStatuesOutOfReach);
  Event_ShowMessageAndWait(0x4008, 0, 10);
  Actor_ShowEmote(8, EMOTE_IN_FRONT, 40);
  Engine_ActorRunRepeatedMotion(8, 1);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(0x4008, 0, 10);
  Engine_ActorSetAnimation(8, 2);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetDestination(8, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorWaitForMove(8);
  Actor_SetPosition(8, 0, 0);
  GameFlag_Set(0x825);
  Engine_EventEnd();
}

static void Scene_RunActorFormation(s32 a0) {
  Map_CopyCellAttributes(122, 20, 1, 1, 100, 32);
  Map_CopyCellAttributes(122, 20, 1, 1, 104, 32);
  Map_CopyCellAttributes(122, 20, 1, 1, 108, 32);
  Map_CopyCellAttributes(122, 20, 1, 1, 112, 32);
  Map_CopyCellAttributes(122, 20, 1, 1, 116, 32);
  Map_CopyCellAttributes(122, 20, 1, 1, 120, 32);
  if (GameFlag_IsSet(0x311) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 100, 32);
    if (a0 != 0) {
      Actor_SetPosition(9, PIXELS(1592), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(0x310) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 100, 32);
      if (a0 != 0) {
        Actor_SetPosition(9, PIXELS(1624), PIXELS(520));
      }
    }
  }
  if (GameFlag_IsSet(0x313) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 104, 32);
    if (a0 != 0) {
      Actor_SetPosition(10, PIXELS(1656), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(0x312) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 104, 32);
      if (a0 != 0) {
        Actor_SetPosition(10, PIXELS(1688), PIXELS(520));
      }
    }
  }
  if (GameFlag_IsSet(0x315) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 108, 32);
    if (a0 != 0) {
      Actor_SetPosition(11, PIXELS(1720), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(0x314) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 108, 32);
      if (a0 != 0) {
        Actor_SetPosition(11, PIXELS(1752), PIXELS(520));
      }
    }
  }
  if (GameFlag_IsSet(0x317) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 112, 32);
    if (a0 != 0) {
      Actor_SetPosition(12, PIXELS(1784), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(0x316) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 112, 32);
      if (a0 != 0) {
        Actor_SetPosition(12, PIXELS(1816), PIXELS(520));
      }
    }
  }
  if (GameFlag_IsSet(0x319) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 116, 32);
    if (a0 != 0) {
      Actor_SetPosition(13, PIXELS(1848), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(0x318) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 116, 32);
      if (a0 != 0) {
        Actor_SetPosition(13, PIXELS(1880), PIXELS(520));
      }
    }
  }
  if (GameFlag_IsSet(0x31b) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 120, 32);
    if (a0 != 0) {
      Actor_SetPosition(14, PIXELS(1912), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(0x31a) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 120, 32);
      if (a0 != 0) {
        Actor_SetPosition(14, PIXELS(1944), PIXELS(520));
      }
    }
  }
}

static void EV10_SEARCH(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
  Engine_EventEnd();
}

static void Scene_UpdateCueTimer(void) {
  s32 value;
  s32 *timer;
  s32 v3;

  timer = &gSolEntrance_CueTimer;
  if (*timer != 0) {
    v3 = (*timer - 1);
    *timer = (*timer - 1);
    if (v3 == 40) {
      Map_SetShake(-1, -1, 0xe666);
    }
  } else {
    value = Engine_RandomNext();
    if (((u32)(((value << 4) - value) << 3) >> 16) == 0) {
      Event_PlayCue(138);
      Map_SetShake(0x10000, 0x20000, 0x10000);
      *timer = 80;
    }
  }
}

struct SceneEntrance gSolEntranceEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSolEntranceEntrances2[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 80, 0, 288, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 120, 0, 112, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSolEntranceEntrances1[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 280, 0, 472, FACING_NORTH, 0, 5, 0, 555, 510, 0},
    {2, CONDITION_ALWAYS, 72, 0, 56, FACING_SOUTH, 0, 5, 0, 555, 510, 0},
    {3, CONDITION_ALWAYS, 744, 0, 152, FACING_NORTH, 0, 620, 0, 860, 195, 0},
    {4, CONDITION_ALWAYS, 696, 0, 568, FACING_NORTH, 0, 575, 265, 815, 610, 0},
    {5, CONDITION_ALWAYS, 696, 0, 328, FACING_SOUTH, 0, 575, 265, 815, 610, 0},
    {6, CONDITION_ALWAYS, 984, 0, 152, FACING_NORTH, 0, 860, 0, 1100, 195, 0},
    {7, CONDITION_ALWAYS, 984, 0, 56, FACING_SOUTH, 0, 860, 0, 1100, 195, 0},
    {8, CONDITION_ALWAYS, 1224, 0, 184, FACING_NORTH, 0, 1100, 20, 1340, 220,
     0},
    {9, CONDITION_ALWAYS, 1224, 0, 104, FACING_SOUTH, 0, 1100, 20, 1340, 220,
     0},
    {10, CONDITION_ALWAYS, 1320, 0, 456, FACING_NORTH, 0, 905, 285, 1365, 490,
     0},
    {11, CONDITION_ALWAYS, 1672, 0, 280, FACING_NORTH, 0, 1455, 0, 1890, 310,
     0},
    {12, CONDITION_ALWAYS, 1512, 0, 136, FACING_SOUTH, 0, 1455, 0, 1890, 310,
     0},
    {13, CONDITION_ALWAYS, 1832, 0, 136, FACING_SOUTH, 0, 1455, 0, 1890, 310,
     0},
    {14, CONDITION_ALWAYS, 1768, 0, 568, FACING_NORTH, 0, 1540, 440, 1995, 600,
     0},
    {15, CONDITION_ALWAYS, 1672, 0, 520, FACING_SOUTH, 0, 1540, 440, 1995, 600,
     0},
    {16, CONDITION_ALWAYS, 1928, 0, 520, FACING_SOUTH, 0, 1540, 440, 1995, 600,
     0},
    {SCENE_TABLE_END},
};

u32 gSolEntrance_Exits[] = {
    SCENE_EXITS(19),
    SCENE_EXIT_IF(1, 4, 2, 0x814),
    SCENE_EXIT_IF(1, 10, 15, 0x815),
    SCENE_EXIT_IF(1, 4, 2, 0xffffffff),
    SCENE_EXIT_IF(2, 16, 8, 0xffffffff),
    SCENE_EXIT(0, 16, 0),
    SCENE_EXIT_IF(1, 15, 2, 0xffffffff),
    SCENE_EXIT_IF(2, 16, 11, 0xffffffff),
    SCENE_EXIT_IF(3, 16, 12, 0xffffffff),
    SCENE_EXIT_IF(4, 16, 9, 0xffffffff),
    SCENE_EXIT_IF(5, 15, 1, 0xffffffff),
    SCENE_EXIT_IF(6, 16, 13, 0xffffffff),
    SCENE_EXIT_IF(7, 14, 1, 0xffffffff),
    SCENE_EXIT_IF(8, 19, 2, 0xffffffff),
    SCENE_EXIT_IF(9, 16, 4, 0xffffffff),
    SCENE_EXIT_IF(10, 16, 15, 0xffffffff),
    SCENE_EXIT_IF(11, 16, 2, 0xffffffff),
    SCENE_EXIT_IF(12, 16, 3, 0xffffffff),
    SCENE_EXIT_IF(13, 16, 6, 0xffffffff),
    SCENE_EXIT_IF(14, 14, 2, 0xffffffff),
    SCENE_EXIT_IF(15, 16, 10, 0xffffffff),
    SCENE_EXIT_IF(16, 11, 1, 0xffffffff),
    SCENE_EXITS_END,
};

struct ScenePlacement gSolEntrancePlacementsOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gSolEntrancePlacements1[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xcb, 0x0, ACTOR_STAND,
        PIXELS(744), 0, PIXELS(56), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSolEntrancePlacements1Entrances11To13[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1512), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1832), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1560), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1784), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1640), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1640), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1640), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1672), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1704), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1704), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1704), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSolEntrancePlacements1Entrances14To16[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1672), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1736), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1800), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1864), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1928), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSolEntrancePlacements2[] = {
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSolEntranceEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gSolEntranceEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
    { SEARCH(SEARCH_UNNAMED), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_SEARCH) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgValeItIsVeryDangerousToGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgValeIAmAllAloneInThis) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSolEntranceEvents1[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { 0x21, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { SEARCH(SEARCH_CHEST), 100, 0xf01, FIND_ITEM(0xe1) },
    { 0xe104, 0x414, CONDITION_ALWAYS, EVENT_SCRIPT(EV1044_SCRIPT) },
    { SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_SEARCH) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSolEntranceEvents1Entrances11To13[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { 0x21, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { PSYNERGY_ON_ACTOR(0x8c), 9, 0x234, EVENT_SCRIPT(Scene_UpdateOuterActor9Flags) },
    { PSYNERGY_ON_ACTOR(0x8c), 10, 0x234, EVENT_SCRIPT(Scene_UpdateOuterActor10Flags) },
    { 0x202, 22, CONDITION_ALWAYS, EVENT_SCRIPT(SolEntrance_PushFacedBlock) },
    { TOUCH_WALKING_INTO(FACING_EAST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(SolEntrance_PushFacedBlock) },
    { TOUCH_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(SolEntrance_PushFacedBlock) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSolEntranceEvents1Entrances14To16[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { 0x21, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { PSYNERGY_ON_ACTOR(0x8c), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_UpdateFormationActor9Flags) },
    { PSYNERGY_ON_ACTOR(0x8c), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_UpdateFormationActor10Flags) },
    { PSYNERGY_ON_ACTOR(0x8c), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(0x8c), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(0x8c), 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(0x8c), 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_PSYNERGY) },
    { 0x202, 24, CONDITION_ALWAYS, EVENT_SCRIPT(SolEntrance_PushFacedBlock) },
    { 0xe104, 0x415, CONDITION_ALWAYS, EVENT_SCRIPT(EV1045_SCRIPT) },
    { SEARCH(SEARCH_UNNAMED), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_SEARCH) },
    { SCENE_EVENTS_END },
};
// clang-format on

// Push step by direction: X in the upper halfword, Z in the lower (pixels).
#define PUSH_STEP(x, z) (((x) << 16) | ((z) & 0xffff))

s32 gSolPushSteps[] = {
    PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(0, 16),
    PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(-16, 0),
    PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(0, -16),
    PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(16, 0),
};

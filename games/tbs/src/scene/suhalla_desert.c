#include "game_state.h"
#include "engine_call.h"
#include "unit/scene/suhalla_desert.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "field_obj.h"
#include "types.h"
#include "field_event.h"
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
#include "fx_scene.h"
#include "global_cells.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "call.h"
#include "field_effect.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "objcmd.h"
#include "iowrite.h"

// 前方宣言
static void SuhallaDesert_ApplyVisitFlagBlend(void);
static void SuhallaDesert_ApplyAltarFlagBlend(void);

struct SelectedActorId {
  u8 reserved000[500];
  s32 actorId;
};

struct SelectedActorFacing {
  u8 reserved00[6];
  u16 presentation;
};

struct StateMarkerWork {
  u8 reserved000[386];
  u16 stateOneMarker;
};

static u16 sSuhallaDesertShownLevel;

#define ACTOR_ID 13

#define EncounterPalette (*(volatile u16 *)0x0500019e)

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/effect_create.h"
#include "common/effect_spawn.h"

// 戦闘開始の種類 (battle/party/party.c と同じ)

struct FieldGlobals {
  u8 *map;
  u8 unk04[0x48];
  struct EventWork *event;
};

void QueueSceneSound(s32 cue);

static void EV15_TALK(void) { BattleFx_SetPhaseRequest(15, 45); }

static s32 SuhallaDesert_ClearActorSpriteAndCollisionFlags(u8 *obj) {
  Engine_ActorSetSpriteFlags(obj, 0);
  ((struct FieldActor *)obj)->collisionFlags = 0;
  return 0;
}

static s32 SuhallaDesert_ToggleActorActive(u8 *obj) {
  ((struct FieldActor *)obj)->active ^= 1;
  return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_SuhallaDesert1) {
    return gSuhallaDesertEntrances1;
  }
  if (selector == (s32)&SceneId_SuhallaDesert2) {
    return gSuhallaDesertEntrances2;
  }
  if (selector == (s32)&SceneId_SuhallaDesert3) {
    return gSuhallaDesertEntrances3;
  }
  return gSuhallaDesertEntrancesOther;
}

s32 SceneData_ReturnZero(void) { return 0; }

u8 *SuhallaDesert_GetExits(void) { return gSuhallaDesertExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_SuhallaDesert1) {
    return gSuhallaDesertPlacements1;
  }
  if (selector == (s32)&SceneId_SuhallaDesert2) {
    return gSuhallaDesertPlacements2;
  }
  if (selector == (s32)&SceneId_SuhallaDesert3) {
    return gSuhallaDesertPlacements3;
  }
  return gSuhallaDesertPlacementsOther;
}

static void SuhallaDesert_SyncSelectedActorProgress(void) {
  struct SelectedActorFacing *actor;
  struct StateMarkerWork *scene;
  s32 progress;

  actor =
      Object_GetById(((struct SelectedActorId *)((s16 *)&gGameState))->actorId);
  scene = *(struct StateMarkerWork **)((u8 *)&gEventWork);
  actor->presentation = (gFrameCount << 12);

  progress = FieldGameFlag_GetByte(0x210);
  if (progress != 0) {
    if (progress == 1) {
      scene->stateOneMarker = 99;
    } else if (Engine_GameFlagIsSet(0x106) == 0) {
      progress -= 1;
    }
  }
  FieldGameFlag_SetByte(0x210, progress);
}

static void FieldScene_RunMiddleAuxiliarySequence(s32 a0) {
  s32 p10;
  s32 rec2;
  struct FieldActor *rec7;
  struct FieldActor *record;
  u8 *p6;
  u8 *base;

  base = (u8 *)((s16 *)&gGameState);
  p6 = *(u8 **)(base + 500);
  p10 = a0;
  rec7 = Object_GetById((s32)p6);
  Actor_Get(p10);
  rec2 = GameFlag_IsSet(0x20f);
  if (rec2 == 0) {
    Engine_EventBegin();
    Actor_SetAttachedEffect((s32)p6, 0x101);
    Engine_ActorSetAnimation((s32)p6, 9);
    record = Object_GetById(p10);
    if (record != 0) {
      Actor_SetDestination((s32)p6, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove((s32)p6);
    Event_PlayCue(244);
    FieldScheduler_AddCallback((s32)SuhallaDesert_SyncSelectedActorProgress,
                               0xc80);
    rec7->motionFlags = rec2;
    Engine_ObjectSetPosition((s32)rec7, rec7->x.fixed, rec7->y.fixed + 0x200000,
                             rec7->z.fixed);
    Engine_ActorWaitForMove((s32)p6);
    rec7->velocityY = rec2;
    rec7->motionFlags = 4;
    *(base + 498) = 2;
    GameFlag_Set(0x20f);
    FieldGameFlag_SetByte(0x218, p10);
    FieldGameFlag_SetByte(0x210, 180);
    Engine_EventEnd();
    gEventWork->encounterEnemyGroup = 0; // drop any battle this step rolled
  }
}

static void FieldScene_RunActor8Step(void) {
  FieldScene_RunMiddleAuxiliarySequence(8);
}

static void FieldScene_RunActor9Step(void) {
  FieldScene_RunMiddleAuxiliarySequence(9);
}

static void FieldScene_RunActor10Step(void) {
  FieldScene_RunMiddleAuxiliarySequence(10);
}

static void FieldScene_RunActor11Step(void) {
  FieldScene_RunMiddleAuxiliarySequence(11);
}

static void FieldScene_RunActor12Step(void) {
  FieldScene_RunMiddleAuxiliarySequence(12);
}

static void SuhallaDesert_RestoreActorScaleAndBlend(s32 a0) {
  s32 i;
  s32 nextFrame;
  s32 p10;
  s32 p9;
  s32 p11;
  struct FieldActor *rec;
  s32 rec2;
  struct FieldActor *rec7;
  struct FieldActor *record;

  p10 = a0;
  rec7 = Object_GetById(gGameState.selectedActor);
  rec = Object_GetById(p10);
  rec2 = Engine_GameFlagIsSet(0x340);
  p9 = rec7->x.part.pixel;
  p11 = rec7->z.part.pixel;
  Engine_EventBegin();
  Engine_AudioPlayCue(244);
  i = 0;
  do {
    rec->scaleX = ((i << 11) + 0x800);
    rec->scaleY = ((i << 12) + 0x1000);
    if (rec2 == 0) {
      nextFrame = i + 1;
      IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - nextFrame) << 8) | nextFrame,
                          0x20000);
    } else {
      nextFrame = i + 1;
    }
    Engine_TaskWait(1);
    i = nextFrame;
  } while (i <= 15);
  Engine_GameFlagSet((0x1fe + p10));
  Engine_GameFlagSet(0x340);
  if (Engine_GameFlagIsSet(0x9a0) == 0) {
  } else {
    if (GameFlag_IsSet(0x9b6) != 0) {
    } else {
      Engine_GameFlagSet(0x9b6);
      Actor_SetSpeed(13, 0x10000, 0x8000);
      Actor_SetSpeed(0, 0x18000, 0xc000);
      record = Object_GetById(0);
      if ((s32)record != 0) {
        Engine_ActorSetPosition(13, record->x.fixed, record->z.fixed);
      }
      Actor_FaceActor(13, 0x4000, 0);
      Object_GetById(13)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Engine_ActorWalkTo(13, p9, (p11 - 16));
      Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
      Engine_ActorWalkToAndWait(0, (p9 + 8), (p11 - 40));
      Engine_EventWait(1);
      Object_GetById(0)->steeringFlags |= ACTOR_FACES_MOTION;
      Engine_ActorSetAnimation(13, 1);
      Engine_ActorFaceDirection(0, 0x4000, 0);
      Engine_EventSetMessage(MsgSuhallaSandstorm);
      Actor_ShowEmote(13, 0x106, 60);
      Engine_EventShowMessage(13, 0);
      Actor_ShowEmote(13, 0x102, 60);
      Engine_EventShowMessage(13, 0);
      Actor_FaceDirection(13, 0x2000, 0);
      Actor_ShowEmote(13, 0x101, 60);
      Engine_EventShowMessage(13, 0);
      Engine_EventWait(10);
      Actor_FaceDirection(13, 0xc000, 30);
      Engine_ActorRunRepeatedMotion(13, 2);
      Engine_EventShowMessage(13, 0);
      Engine_ActorSetAnimationAndWait(13, 3);
      Engine_EventShowMessage(13, 0);
      Actor_SetSpeed(13, 0x10000, 0x8000);
      Engine_ActorSetAnimation(13, 2);
      record = Object_GetById(0);
      if ((s32)record != 0) {
        Engine_ActorSetDestination(13, record->x.part.pixel,
                                   record->z.part.pixel);
      }
      Engine_ActorWaitForMove(13);
      Engine_ActorSetPosition(13, 0, 0);
    }
  }
  Engine_EventEnd();
}

static void FieldScene_RunLateActor8Step(void) {
  SuhallaDesert_RestoreActorScaleAndBlend(8);
}

static void FieldScene_RunLateActor9Step(void) {
  SuhallaDesert_RestoreActorScaleAndBlend(9);
}

static void FieldScene_RunLateActor10Step(void) {
  SuhallaDesert_RestoreActorScaleAndBlend(10);
}

static void EV64_TOUCH(void) { SuhallaDesert_RestoreActorScaleAndBlend(11); }

static void EV65_TOUCH(void) { SuhallaDesert_RestoreActorScaleAndBlend(12); }

static void EV09_TOUCH(void) {
  struct FieldActor *record;

  if (GameFlag_IsSet(0x9a0) == 0) {
  } else {
    if (GameFlag_IsSet(0x1b7) != 0) {
    } else {
      if (GameFlag_IsSet(0x9b0) == 0) {
      } else {
        GameFlag_Set(0x9b5);
        Engine_EventBegin();
        Engine_EventSetMessage(MsgSuhallaNowhereFound);
        record = Actor_Get(ACTOR_PARTY_LEADER);
        if (record != 0) {
          Actor_SetPosition(ACTOR_ID, record->x.fixed, record->z.fixed);
        }
        Actor_FaceActor(ACTOR_ID, 0xc000, 0);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1b8, 0x4e8);
        Actor_FaceDirection(ACTOR_ID, 0x4000, 0);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1bc, 0x4d8);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 40);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 30);
        Engine_ActorSetAnimationAndWait(ACTOR_ID, 4);
        Event_ShowMessage(ACTOR_ID, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
        Actor_ShowEmote(ACTOR_ID, 0x105, 60);
        Event_ShowMessage(ACTOR_ID, 0);
        Engine_EventWait(30);
        Engine_ActorRunRepeatedMotion(ACTOR_ID, 2);
        Event_ShowMessage(ACTOR_ID, 0);
        Actor_FaceDirection(ACTOR_ID, 0xc000, 30);
        Event_AskYesNo(ACTOR_ID, 0);
        Engine_EventWait(30);
        Actor_ShowEmote(ACTOR_ID, 0x106, 60);
        Event_ShowMessage(ACTOR_ID, 0);
        Engine_ActorSetAnimationAndWait(ACTOR_ID, 3);
        Event_ShowMessage(ACTOR_ID, 0);
        Actor_SetSpeed(ACTOR_ID, 0xb333, 0x5999);
        Actor_WalkToAndWait(ACTOR_ID, 0x1b8, 0x4e8);
        Event_ShowMessage(ACTOR_ID, 0);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimation(ACTOR_ID, 2);
        record = Actor_Get(ACTOR_PARTY_LEADER);
        if (record != 0) {
          Actor_SetDestination(ACTOR_ID, record->x.part.pixel,
                               record->z.part.pixel);
        }
        Engine_ActorWaitForMove(ACTOR_ID);
        Actor_SetPosition(ACTOR_ID, 0, 0);
        Engine_EventEnd();
      }
    }
  }
}

static s32 SuhallaDesert_FollowLeaderWithSparks(struct FieldActor *actor) {
  struct EffectOptions options;
  struct FieldActor *leader;
  s32 dx;
  s32 dz;
  s32 phase;

  leader = Object_GetById(gGameState.selectedActor);
  actor->acceleration = 0x4000;
  actor->speed = 0x18000;
  actor->motionFlags = 0;
  Engine_ActorSetSpriteFlags(actor, 0);
  actor->active ^= 1;
  if (Engine_GameFlagIsSet(0x104)) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
  } else {
    actor->targetX = leader->x.fixed;
    actor->targetY = leader->terrainHeight;
    actor->targetZ = leader->z.fixed;
    dx = actor->x.fixed - leader->x.fixed;
    if (dx < 0) {
      dx = leader->x.fixed - actor->x.fixed;
    }
    dz = actor->z.fixed - leader->z.fixed;
    if (dx + (dz >= 0 ? actor->z.fixed - leader->z.fixed
                      : leader->z.fixed - actor->z.fixed) <
        0x80000) {
      struct EventWork *work = gEventWork;

      if (leader->motionFlags != 0) {
        work->raisedTrigger = 55;
      }
      actor->motionFlags = 3;
      actor->targetX = leader->x.fixed;
      actor->targetY = leader->y.fixed;
      actor->targetZ = leader->z.fixed;
    }
  }
  phase = gFrameCount & 7;
  if (phase == 0) {
    options.startScaleX = 0xcccc;
    options.startScaleY = 0xcccc;
    options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, phase,
                 phase, 0x880001, &options);
  }
  return 1;
}

static void EV81_TOUCH(void) {
  if (GameFlag_IsSet(2487) == 0) {
    GameFlag_Set(526);
    Actor_SetPosition(12, 240 << 15, 206 << 18);
    Engine_ActorEnableActionCallback(12, gSuhallaDesertActor12Action);
  }
}

static void SuhallaDesert_EnableEffectChildOption(void) {
  u8 *state = gEffectWork;
  ((struct BattleFxScene *)state)->childOption = 1;
}

static void SuhallaDesert_MeetActor(s32 a0, s32 actor) {
  struct GameState *state;

  if (gEventWork->raisedTrigger == 99) {
    gEventWork->raisedTrigger = 0;
  }
  Engine_GameFlagClear(0x20f);
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert1) {
    Engine_GameFlagSet((actor + 0x2f9));
  } else {
    if (gGameState.scene == (s32)&SceneId_SuhallaDesert2) {
      Engine_GameFlagSet((actor + 0x309));
    }
  }
  FieldGameFlag_SetByte(0x210, 0);
  BattleFx_SetWeightedResult(98, 5);
  Battle_SetStart(3);
  state = &gGameState;
  if (state->scene == (s32)&SceneId_SuhallaDesert2) {
    if (actor == 11) {
      BattleFx_SetWeightedResult(98, 7);
    } else {
      if (actor == 12) {
        BattleFx_SetWeightedResult(98, 6);
        Engine_ActorStop(12);
        Engine_ActorSetPosition(12, 0, 0);
      }
    }
  }
  Object_GetById(gGameState.selectedActor)->motionFlags = 3;
}

static void EV99_RAISED(void) {
  s32 other;
  struct FieldActor *leader;
  struct FieldActor *partner;
  s32 i;

  other = FieldGameFlag_GetByte(0x218);
  leader = Object_GetById(gGameState.selectedActor);
  partner = Object_GetById(other);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_AudioPlayCue(219);
  Engine_ActorSetSpriteFlags((struct FieldActor *)gGameState.selectedActor, 0);
  partner->motionFlags = 0;
  leader->motionFlags = 0;
  leader->velocityY = 0;
  leader->unk5d[4] = 1;
  partner->unk5d[4] = 1;
  for (i = 0; i < 60; i++) {
    leader->velocityY += 0x3333;
    partner->velocityY += 0x3333;
    Engine_TaskWait(1);
  }
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
  Engine_GameFlagSet(0x122);
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert2 &&
      FieldGameFlag_GetByte(0x218) == 11) {
    Event_SetDestination((s32)&SceneId_WorldMap, 77);
  } else {
    Event_SetDestination((s32)&SceneId_WorldMap, 27);
  }
}

static void EncounterPalette_Pulse(void) {
  u16 phase = gFrameCount & 63;
  s32 level;

  if (phase > 31)
    phase = 64 - phase;
  level = (phase >> 1) + 7;
  level |= (level << 10) | (level << 5);
  EncounterPalette = ((u32)level << 16) >> 16;
}

static s32 FieldScene_RunOpeningAuxiliarySequence(void) {
  s32 record;

  if (gGameState.scene == (s32)&SceneId_SuhallaDesert2) {
    Object_GetById(14)->priorityFlags = 2;
    Object_GetById(14)->motionFlags = 3;
    Actor_SetPosition(14, 0, 0);
    Map_CopyCellAttributes(16, 44, 1, 1, 15, 44);
    MapObject_SetPosition(100, 0, 0);
    Map_CopyCellAttributes(12, 71, 1, 1, 127, 127);
    Map_CopyCellAttributes(11, 71, 1, 1, 12, 71);
    record = Engine_TaskRemoveCallback(EncounterPalette_Pulse);
    Io_Write16(sSuhallaDesertShownLevel, &EncounterPalette);
    return record;
  }
}

static s32 FieldScene_RunScene3c0SequenceA(void) {
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert2) {
    Object_GetById(14)->priorityFlags = 2;
    Object_GetById(14)->motionFlags = 0;
    Actor_SetPosition(14, 0xf80000, 0x2c80000);
    Map_CopyCellAttributes(31, 95, 1, 1, 15, 44);
    MapObject_SetPosition(100, -1, -1);
    BattleFx_EmitRandomParticle();
    Map_CopyCellAttributes(127, 127, 1, 1, 12, 71);
    return FieldScheduler_AddCallback(EncounterPalette_Pulse, 0xc80);
  }
}

const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert3) {
    return gSuhallaDesertEvents3;
  }
  return gSuhallaDesertEventsOther;
}

s32 SuhallaDesert_RunSceneScript(void) {
  u8 *map;

  map = (*(struct FieldGlobals *)gMapWork).map;
  (*(struct FieldGlobals *)gMapWork).event->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  if (FieldGameFlag_GetByte(0x210) != 0) {
    gGameState.movementMode = 2;
    FieldScheduler_AddCallback(SuhallaDesert_SyncSelectedActorProgress, 0xc80);
  }
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert1 ||
      gGameState.scene == (s32)&SceneId_SuhallaDesert2) {
    sSuhallaDesertShownLevel = EncounterPalette;
    FieldScene_RunOpeningAuxiliarySequence();
  }
  if (gGameState.scene == (s32)&SceneId_SuhallaDesert1) {
    SuhallaDesert_ApplyVisitFlagBlend();
  } else if (gGameState.scene == (s32)&SceneId_SuhallaDesert2) {
    SuhallaDesert_ApplyAltarFlagBlend();
  } else {
    Engine_AudioPlayCue(0x120);
  }
  if (gGameState.entrance == 0) {
    *(u16 *)(map + 20) &= ~0x200;
  }
  return 0;
}

static void SuhallaDesert_ApplyVisitFlagBlend(void) {
  struct FieldActor *actor;
  s32 id;
  s32 alpha = 0;

  if (Engine_GameFlagIsSet(0x301)) {
    Engine_GameFlagSet(0x206);
  }
  if (Engine_GameFlagIsSet(0x302)) {
    Engine_GameFlagSet(0x207);
  }
  if (Engine_GameFlagIsSet(0x303)) {
    Engine_GameFlagSet(0x208);
  }
  if (Engine_GameFlagIsSet(0x304)) {
    Engine_GameFlagSet(0x209);
  }
  if (Engine_GameFlagIsSet(0x305)) {
    Engine_GameFlagSet(0x20a);
  }
  for (id = 8; id <= 12; id++) {
    actor = Object_GetById(id);
    if (actor != 0) {
      if (!Engine_GameFlagIsSet(0x109)) {
        actor->scaleX = 0x800;
        actor->scaleY = 0x800;
      }
      actor->sprite->flags = 0;
    }
  }
  IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f42, 0x20000);
  if (Engine_GameFlagIsSet(0x340)) {
    alpha = 16;
    QueueSceneSound(244);
  }
  IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - alpha) << 8) | alpha, 0x20000);
}

static void SuhallaDesert_ApplyAltarFlagBlend(void) {
  struct FieldActor *actor;
  struct FieldSprite *sprite;
  s32 id;
  s32 alpha = 0;

  if (Engine_GameFlagIsSet(0x311)) {
    Engine_GameFlagSet(0x206);
  }
  if (Engine_GameFlagIsSet(0x312)) {
    Engine_GameFlagSet(0x207);
  }
  if (Engine_GameFlagIsSet(0x313)) {
    Engine_GameFlagSet(0x208);
  }
  for (id = 8; id <= 10; id++) {
    actor = Object_GetById(id);
    if (actor != 0) {
      if (!Engine_GameFlagIsSet(0x109)) {
        actor->scaleX = 0x800;
        actor->scaleY = 0x800;
      }
      sprite = actor->sprite;
      sprite->flags = 0;
    }
  }
  actor = Object_GetById(11);
  if (actor != 0) {
    u8 *part;

    sprite = actor->sprite;
    part = *(u8 **)((u8 *)sprite + 40);
    if (part != 0) {
      part[5] = 10;
    }
    sprite->unk20[5] = 1;
    sprite->flags = 0;
  }
  if (Engine_GameFlagIsSet(0x315)) {
    Engine_GameFlagSet(0x9b7);
  }
  IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f42, 0x20000);
  if (Engine_GameFlagIsSet(0x340)) {
    alpha = 16;
    QueueSceneSound(244);
  }
  IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - alpha) << 8) | alpha, 0x20000);
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

s32 gSuhallaDesertPlacements1_Data[] = {
    SC_CALL(SuhallaDesert_ClearActorSpriteAndCollisionFlags),
    SC_CALL(SuhallaDesert_ToggleActorActive),
    SC_END,
};

s32 gSuhallaDesertPlacements2_1[] = {
    SC_CALL(SuhallaDesert_ClearActorSpriteAndCollisionFlags),
    SC_CALL(SuhallaDesert_ToggleActorActive),
    SC_END,
};

struct SceneEntrance gSuhallaDesertEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaDesertEntrances1[] = {
    {1, CONDITION_ALWAYS, 264, 0, 56, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 24, 0, 936, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaDesertEntrances2[] = {
    {1, CONDITION_ALWAYS, 488, 0, 456, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 488, 0, 1208, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 440, 0, 624, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 56, 0, 624, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaDesertEntrances3[] = {
    {1, CONDITION_ALWAYS, 72, 0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 440, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 632, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 648, 0, 280, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSuhallaDesertExits[] = {
    SCENE_EXITS(164),      SCENE_EXIT(1, 2, 28),  SCENE_EXIT(2, 165, 1),
    SCENE_EXIT(0, 165, 0), SCENE_EXIT(1, 164, 2), SCENE_EXIT(2, 2, 55),
    SCENE_EXIT(3, 166, 4), SCENE_EXIT(4, 166, 1), SCENE_EXIT(0, 166, 0),
    SCENE_EXIT(1, 165, 4), SCENE_EXIT(2, 166, 3), SCENE_EXIT(3, 166, 2),
    SCENE_EXIT(4, 165, 3), SCENE_EXITS_END,
};

struct ScenePlacement gSuhallaDesertPlacementsOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gSuhallaDesertPlacements1[] = {
    { 0x121, 0x301, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(56), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x121, 0x302, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(456), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x121, 0x303, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(120), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x121, 0x304, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(264), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x121, 0x305, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(440), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x39, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSuhallaDesertPlacements2[] = {
    { 0x121, 0x311, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(216), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x121, 0x312, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(296), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x121, 0x313, (s32)gSuhallaDesertPlacements1_Data,
        PIXELS(408), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x121, 0x314, (s32)gSuhallaDesertPlacements2_1,
        PIXELS(376), 0, PIXELS(632), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x121, 0x315, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x39, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x5c, 0x5d, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSuhallaDesertPlacements3[] = {
    { SCENE_TABLE_END },
};

s32 gSuhallaDesertActor12Action[] = {
    SC_CALL(SuhallaDesert_FollowLeaderWithSparks),
    SC_END,
};

struct SceneEvent gSuhallaDesertEventsOther[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
#if !EDITION_INTERNATIONAL
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
#else
    { 0xc401, 4, CONDITION_ALWAYS, 4 },
#endif
    { EVENT_TOUCH, 9, 0x9b5, EVENT_SCRIPT(EV09_TOUCH) },
    { EVENT_TOUCH, 41, 0x301, EVENT_SCRIPT(FieldScene_RunActor8Step) },
    { EVENT_TOUCH, 42, 0x302, EVENT_SCRIPT(FieldScene_RunActor9Step) },
    { EVENT_TOUCH, 43, 0x303, EVENT_SCRIPT(FieldScene_RunActor10Step) },
    { EVENT_TOUCH, 44, 0x304, EVENT_SCRIPT(FieldScene_RunActor11Step) },
    { EVENT_TOUCH, 45, 0x305, EVENT_SCRIPT(FieldScene_RunActor12Step) },
    { EVENT_TOUCH, 51, 0x311, EVENT_SCRIPT(FieldScene_RunActor8Step) },
    { EVENT_TOUCH, 52, 0x312, EVENT_SCRIPT(FieldScene_RunActor9Step) },
    { EVENT_TOUCH, 53, 0x313, EVENT_SCRIPT(FieldScene_RunActor10Step) },
    { EVENT_TOUCH, 61, 0x206, EVENT_SCRIPT(FieldScene_RunLateActor8Step) },
    { EVENT_TOUCH, 62, 0x207, EVENT_SCRIPT(FieldScene_RunLateActor9Step) },
    { EVENT_TOUCH, 63, 0x208, EVENT_SCRIPT(FieldScene_RunLateActor10Step) },
    { EVENT_TOUCH, 64, 0x209, EVENT_SCRIPT(EV64_TOUCH) },
    { EVENT_TOUCH, 65, 0x20a, EVENT_SCRIPT(EV65_TOUCH) },
    { EVENT_TOUCH, 71, 0x206, EVENT_SCRIPT(FieldScene_RunLateActor8Step) },
    { EVENT_TOUCH, 72, 0x207, EVENT_SCRIPT(FieldScene_RunLateActor9Step) },
    { EVENT_TOUCH, 73, 0x208, EVENT_SCRIPT(FieldScene_RunLateActor10Step) },
    { EVENT_TOUCH, 54, 0x314, EVENT_SCRIPT(FieldScene_RunActor11Step) },
    { EVENT_TOUCH, 81, 0x20e, EVENT_SCRIPT(EV81_TOUCH) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TALK) },
    { SEARCH(SEARCH_UNNAMED), 100, 0x351, FIND_PSYNERGY_STONE },
    { SEARCH(SEARCH_CHEST), 103, 0xfb1, FIND_ITEM(0x8d) },
    { SEARCH(SEARCH_CHEST), 104, 0xfb2, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_CHEST), 101, 0xfb3, FIND_ITEM(0xc0) },
    { SEARCH(SEARCH_CHEST), 102, 0xef6, 0x500006 },
    { 0x10002115, 8, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_EnableEffectChildOption) },
    { 0x10002115, 9, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_EnableEffectChildOption) },
    { 0x10002115, 10, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_EnableEffectChildOption) },
    { 0x10002115, 11, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_EnableEffectChildOption) },
    { 0x10002115, 12, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_EnableEffectChildOption) },
    { PSYNERGY_ON_ACTOR(0x21), 8, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_MeetActor) },
    { PSYNERGY_ON_ACTOR(0x21), 9, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_MeetActor) },
    { PSYNERGY_ON_ACTOR(0x21), 10, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_MeetActor) },
    { PSYNERGY_ON_ACTOR(0x21), 11, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_MeetActor) },
    { PSYNERGY_ON_ACTOR(0x21), 12, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuhallaDesert_MeetActor) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3c0SequenceA) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunOpeningAuxiliarySequence) },
    { EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(EV99_RAISED) },
    { EVENT_RAISED, 55, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor12Step) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gSuhallaDesertEvents3[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {SCENE_EVENTS_END},
};

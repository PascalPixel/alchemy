#include "game_state.h"
#include "version.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/altmiller_cave.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/shop/shop.h"
#include "unit/runtime/runtime.h"
#include "iowrite.h"

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
#include "staged_actor.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "edition.h"
#include "object_dispatch.h"
#include "scene_ids.h"
#include "dma.h"
#include "io_reg.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "call.h"
#include "display_transition.h"
#include "effect_slot.h"
#include "item_ids.h"
#include "objcmd.h"
#include "frame.h"

// Forward declarations.
static void AltmillerCave_ReleaseWallBurst(void);

typedef struct {
  s32 x;
  s32 y;
  s32 z;
} Vec;

struct ActorWheel {
  s16 phase;
  s16 ticks;
  s16 unk04;
  u16 angle;
  u16 speed;
  u16 travel;
};

#define AltmillerCave_Wheel ((struct ActorWheel *)gAltmillerCave_ClearTarget)

struct RockPuzzle {
  u8 state;
  s8 chosen;
};

#define AltmillerCave_RockPuzzle ((struct RockPuzzle *)&gSceneState[1])

u16 gAltmillerActorWheelWork[6] __attribute__((nocommon, aligned(4)));

// Shared source is included here in ROM order.
#include "common/object_staged_actor.h"

static void EV13_TALK(void) { Field_RequestDjinnBattle(13, 0x41); }

u8 *AltmillerCave_GetEntrances(void) { return gAltmillerCave_SceneTableA; }

s32 SceneData_ReturnZero(void) { return 0; }

u8 *AltmillerCave_GetExits(void) { return gAltmillerCave_SceneTableB; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_AltmillerCave2) {
    return gAltmillerCavePlacements2;
  }
  if (scene == (s32)&SceneId_AltmillerCave4) {
    return gAltmillerCavePlacements4;
  }
  if (scene == (s32)&SceneId_AltmillerCave6) {
    return gAltmillerCavePlacements6;
  }
  return gAltmillerCavePlacementsOther;
}

static void AltmillerCave_ApplyFadeBlend(void) {
  s8 level = ((s8 *)gSceneState)[4];

  Io_Write16(0x3f42, &REG_BLDCNT);
  if (level == 0) {
    s32 alpha = 0x1000;

    REG_BLDALPHA = alpha;
  } else if (level == 1) {
    s32 alpha = 0xe00;

    REG_BLDALPHA = alpha;
  } else if (level == 2) {
    s32 alpha = 0xc00;

    REG_BLDALPHA = alpha;
  } else if (level == 3) {
    s32 alpha = 0xa00;

    REG_BLDALPHA = alpha;
  } else if (level == 4) {
    s32 alpha = 0x800;

    REG_BLDALPHA = alpha;
  } else {
    s32 alpha = 0x600;

    REG_BLDALPHA = alpha;
  }
}

static void AltmillerCave_SetFadeLevel(s32 level) {
  u8 *work;
  u8 *fade = &gSceneState[4];

  work = (u8 *)gEventWork;
  *fade = level;
  if (*(s16 *)(work + 0xcb8) == 0) {
    AltmillerCave_ApplyFadeBlend();
  }
}

static void EV60_TOUCH(void) { AltmillerCave_SetFadeLevel(0); }

static void EV61_TOUCH(void) { AltmillerCave_SetFadeLevel(1); }

static void EV62_TOUCH(void) { AltmillerCave_SetFadeLevel(2); }

static void EV63_TOUCH(void) { AltmillerCave_SetFadeLevel(3); }

static void EV64_TOUCH(void) { AltmillerCave_SetFadeLevel(4); }

static void EV65_TOUCH(void) { AltmillerCave_SetFadeLevel(5); }

static void SceneEffect_SetupBlendByFlag201(void) {
  u8 **base = (u8 **)&gEventWork;
  struct DisplayTransitionState *state;

  {
    u8 *tmp = *base;
    *(s32 *)(tmp + 0x1c0) = 0x100;
    *(s32 *)(tmp + 0x1c8) = 24;
  }
  Engine_TaskWait(1);
  DisplayTransition_InitializeBattleEffectState(0x4d);
  // UB: slot 31 lies past scalar gEventWork. Its named load adds eight
  // bytes in the native EN trial (0x98 to 0xa0).
#ifdef UBFIX
  state = gDisplayTransition;
#else
  state = (struct DisplayTransitionState *)base[4];
#endif
  {
    u16 *slot = &state->value;
    s32 c = 5;
    *slot = c;
  }
  if (Engine_GameFlagIsSet(0x201) != 0) {
    {
      s16 *slot = &state->mask;
      s32 c = 0x1d1d;
      *slot = c;
    }
    {
      s16 *slot = &state->active;
      s32 c = 0x3f;
      *slot = c;
    }
    AltmillerCave_ApplyFadeBlend();
    return;
  }
  {
    s16 *slot = &state->mask;
    s32 c = 0x3f3f;
    *slot = c;
  }
  {
    s16 *slot = &state->active;
    s32 c = 31;
    *slot = c;
  }
  {
    s32 a = 0x3f42;
    REG_BLDCNT = a;
  }
  {
    s32 b = 0xc04;
    REG_BLDALPHA = b;
  }
}

static void AltmillerCave_ApplyRoomVisuals(void) {
  if (gGameState.scene == (s32)&SceneId_AltmillerCave1) {
    s32 alpha = 0x1000;

    REG_BLDALPHA = alpha;
  }
  if (gGameState.scene == (s32)&SceneId_AltmillerCave6) {
    Engine_ActorSetChildValue(16, 1);
    Engine_ActorSetChildValue(17, 4);
    Engine_ActorSetChildValue(18, 11);
    Engine_ActorSetChildValue(19, 2);
    Engine_ActorSetChildValue(20, 3);
  }
}

static void SceneState_RunFlag200SetupAndPlaceActors16To20(void) {
  u8 *work = gEffectWork;
  s16 *table;

  if (Engine_GameFlagIsSet(0x200) != 0) {
    SceneEffect_SetupBlendByFlag201();
    work[0x34] = 1;
  }
  table = (s16 *)&gGameState;
  if (table[0xe0] == (s32)&SceneId_AltmillerCave6) {
    Engine_ActorSetChildValue(16, 6);
    Engine_ActorSetChildValue(17, 6);
    Engine_ActorSetChildValue(18, 6);
    Engine_ActorSetChildValue(19, 6);
    Engine_ActorSetChildValue(20, 6);
  }
}

static void AltmillerCave_RespawnActorObject(void) {
  u8 *work;
  struct FieldActor *old;
  struct FieldActor *copy;

  work = gEffectWork;
  old = *(struct FieldActor **)(work + 16);
  Engine_ActorFaceDirection(*(s16 *)(work + 24), 0x4000, 0);
  Animation_ApplyChildValues((struct DispatchObject *)old, 0);
  Engine_EventWait(20);
  copy = Engine_ObjectCreate(0, old->x.fixed, old->y.fixed, old->z.fixed);
  if (copy != 0) {
    Dma_Set(old, copy, DMA_ENABLE32 | DMA_32BIT | sizeof(struct FieldActor) / 4,
            REG_DMA3);
    old->update = 0;
    *(struct FieldActor **)(work + 16) = copy;
    old->active = 0;
  }
}

static void EV12_PSYNERGY(void) {
  struct FieldActor *actor = Object_GetById(12);

  if (actor->x.fixed >> 20 == 30 && actor->z.fixed >> 20 == 20) {
    actor->motionFlags = 2;
    actor->terrainHeight = 0;
    actor->priorityFlags = 2;
    Map_CopyCellAttributes(30, 20, 1, 1, 32, 20);
#if TBS_PATCH_1
    Engine_GameFlagSet(0x212);
#endif
  }
}

static void EV77_TOUCH(void) {
  StagedActor_AdvancePair();
  EV12_PSYNERGY();
}

static void EV11_SCRIPT(void) {
  u8 *obj = gEffectWork;
  struct FieldActor *p = Object_GetById(11);
  Vec v;

  v.x = p->x.fixed;
  v.y = p->y.fixed;
  v.z = p->z.fixed;

  if (FieldObject_CheckMovementCollision(p, &v) > 0) {
    obj[0x35] = 1;
  }
}

static void EV11_PSYNERGY(void) {
  u8 *obj = gEffectWork;
  struct FieldActor *p = Object_GetById(11);
  s32 t;

  obj += 0x35;
  t = *obj;
  t = (s8)t;
  if (t == 0) {
    Map_CopyCellAttributes(0x4c, 0x10, 1, 1, 0x49, 0x11);
    if (p != 0) {
      s32 c = 2;
      p->motionFlags = c;
      p->priorityFlags = t;
    }
    Engine_GameFlagSet(0x211);
  }
}

static void AltmillerCave_HopSelectedActor(void) {
  struct FieldActor *actor = Object_GetById(gGameState.selectedActor);
  s32 flags = actor->motionFlags;
  union FieldCoordinate pos[3];
  union FieldCoordinate *p = pos;

  p[0].fixed = actor->x.fixed;
  p[1].fixed = actor->y.fixed;
  p[2].fixed = actor->z.fixed;
  {
    s32 angle = actor->facing & 0xf000;

    Vector_AddPolarOffset(0x200000, angle, p);
  }
  if (FieldObject_CheckMovementCollision(actor, p) == 0) {
    Engine_EventBegin();
    Object_SetMode(actor, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(152);
    Object_SetMode(actor, 7);
    actor->speed = 0x30000;
    actor->acceleration = 0x20000;
    actor->velocityY = 0x40000;
    actor->motionFlags &= 126;
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ObjectMotionSetPositionAndCommit(0, p[0].part.pixel,
                                            p[2].part.pixel);
    Object_SetMode(actor, 6);
    Engine_ActorSetSpriteFlags(actor, 1);
    actor->motionFlags = flags;
    Engine_EventEnd();
  }
}

static void FieldScene_RunGuardedSixWordStep(void) {
  struct StagedActorProbe s;

  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&s) != 0) {
    SceneActor_MoveAndRedraw(s);
  }
  Engine_EventEnd();
}

static void AltmillerCave_UpdateScalePulse(union FieldObject *fieldObject) {
  struct FieldActor *obj = (struct FieldActor *)fieldObject;
  s32 v = gAltmillerCave_PulseScales[(obj->timer >> 2) & 3];

  obj->scaleX = v;
  obj->scaleY = v;
  obj->timer++;
  obj->timer &= 15;
}

static void SceneActor_SetPositionFromTransformedBase(s32 a, s32 b, s32 c) {
  s32 k1 = 0x1f80000;
  s32 k2 = 0x180000;
  s32 k3 = 0x900000;
  struct FieldActor *obj = Object_GetById(a);
  s32 buf[3];
  s32 *bp = buf;

  bp[0] = k1;
  bp[2] = k2;
  Vector_AddPolarOffset(b, c, bp);
  obj->x.fixed = bp[0];
  obj->y.fixed = bp[2];
  obj->z.fixed = k3;
}

static void SceneActor_PlaceFiveActorsInRow(u8 *p) {
  s32 i = 0;

  do {
    SceneActor_SetPositionFromTransformedBase(i + 11, 0x180000, p);
    p -= 13107;
    i++;
  } while (i <= 4);
}

static void AltmillerCave_SpinActorWheel(void) {
  struct ActorWheel *wheel = AltmillerCave_Wheel;
  s32 flag = 1;
  s32 state = wheel->phase;

  if (state == 0) {
    wheel->speed += 16;
    if (wheel->speed > 0xbff) {
      wheel->phase++;
      wheel->ticks = 0;
    }
  } else if (state == 1) {
    if (wheel->ticks == 30) {
      wheel->phase++;
    }
  } else if (state == 2) {
    wheel->speed -= 8;
    if (wheel->speed <= 0x2ff) {
      wheel->phase++;
    }
  } else if (state == 3) {
    s32 v = ((s8 *)gSceneState)[2];
    s32 r = Math_Divide(v << 16, 5);
    if ((unsigned int)(((wheel->angle - r) << 16) + 0xc2ff0000) <= 0x5fe0000) {
      wheel->angle = r + 0x4000;
      wheel->phase = 0x63;
      wheel->speed = 0;
      Object_GetById(v + 11)->update = AltmillerCave_UpdateScalePulse;
    }
  } else if (state == 0x63) {
    flag = 0;
  }
  if (flag != 0) {
    AltmillerCave_Wheel->angle += AltmillerCave_Wheel->speed;
    SceneActor_PlaceFiveActorsInRow(AltmillerCave_Wheel->angle);
    AltmillerCave_Wheel->travel += AltmillerCave_Wheel->speed;
    if (AltmillerCave_Wheel->travel > 0x3000) {
      AltmillerCave_Wheel->travel = 0;
      Engine_AudioPlayCue(0x87);
    }
  }
  AltmillerCave_Wheel->ticks++;
}

static void AltmillerCave_TurnRock(s32 action) {
  s32 won = 0;
  s32 i;
  struct FieldActor *actor;
  s32 size;
  s8 state;
  s32 chosen;
  u8 previous;
  s32 reach;
  u16 angle;
  u8 *rock;

  Event_Begin();
  Battle_ResetEffectCounter();
  Event_SetMessage(MsgAltmillerRotatedRock);
  Event_ShowMessage(16, 0);
  Event_End();
  for (i = 0; i <= 4; i++) {
    actor = Actor_Get(i + 11);
    actor->update = NULL;
    actor->scaleX = 0x10000;
    actor->scaleY = 0x10000;
  }
  {
    struct ActorWheel *wheel;

    state = AltmillerCave_RockPuzzle->state;
    previous = AltmillerCave_RockPuzzle->chosen;
    wheel = AltmillerCave_Wheel;
    chosen = AltmillerCave_RockPuzzle->chosen;
    wheel->angle = (chosen << 16) / 5 + 0x4000;
  }
  if (state == 0) {
    if (action == 16) {
      state = 1;
      Event_PlayCue(110);
    } else {
      Event_PlayCue(114);
    }
    gSceneState[0] = 0;
  } else if (state == 1) {
    if (action == 16) {
      Event_PlayCue(110);
    } else if (action == 20) {
      state = 2;
      Event_PlayCue(110);
      Task_Wait(30);
      angle = AltmillerCave_Wheel->angle;
      for (i = 0; i <= 4; i++) {
        SceneActor_SetPositionFromTransformedBase(i + 11, 0x180000, angle);
        Event_PlayCue(151);
        actor = Actor_Get(i + 11);
        actor->scaleX = 0;
        size = 0x6666;
        do {
          actor->scaleY = size;
          actor->scaleX = size;
          Task_Wait(1);
          size += 0xc00;
        } while (actor->scaleX <= 0xffff);
        angle -= 0x3333;
      }
      Task_Wait(30);
      won = 1;
    } else {
      Event_PlayCue(114);
      state = 0;
    }
  } else if (state == 2) {
    if (action != chosen + 16) {
      state = 0;
      Event_PlayCue(114);
      Task_Wait(30);
      for (i = 0; i <= 4; i++) {
        actor = Actor_Get(i + 11);
        Event_PlayCue(151);
        for (size = actor->scaleX; actor->scaleX > 0x6666; size -= 0xc00) {
          actor->scaleY = size;
          actor->scaleX = size;
          Task_Wait(1);
        }
        Actor_SetPosition(i + 11, 0, 0);
      }
    } else {
      Event_PlayCue(110);
      won = 1;
      Task_Wait(30);
    }
  }
  rock = &gSceneState[1];
  rock[0] = state;
  if (won) {
    u8 rounds = ++rock[-1];

    rock[1] = ((s8)(((u32)(Random_Next() << 2) >> 16) + previous + 1) + 5) % 5;
    AltmillerCave_Wheel->phase = 0;
    AltmillerCave_Wheel->ticks = 0;
    AltmillerCave_Wheel->speed = 0x200;
    AltmillerCave_Wheel->travel = 0x3000;
    Task_AddCallback(AltmillerCave_SpinActorWheel, TASK_PRIORITY_SCENE);
    if (rounds <= 2) {
      while (AltmillerCave_Wheel->phase != 99) {
        Task_Wait(1);
      }
      Task_Wait(10);
      Event_PlayCue(110);
    } else {
      rock[0] = 99;
      while (AltmillerCave_Wheel->phase != 2) {
        Task_Wait(1);
      }
      AltmillerCave_Wheel->phase = 2;
      AltmillerCave_Wheel->ticks = 0;
      Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
      Engine_EventWait(20);
      Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
      AltmillerCave_Wheel->phase = 99;
      Event_PlayCue(190);
      reach = 0x180000;
      angle = AltmillerCave_Wheel->angle;
      do {
        for (i = 0; i <= 4; i++) {
          actor = Actor_Get(i + 11);
          actor->scaleX -= 16;
          actor->scaleY -= 16;
          SceneActor_SetPositionFromTransformedBase(i + 11, reach, angle);
          angle -= 0x3333;
        }
        reach -= 0x3333;
        angle += 0xc00;
        Task_Wait(1);
      } while (reach > 0);
      for (i = 0; i <= 4; i++) {
        Actor_SetPosition(i + 11, 0, 0);
      }
      AltmillerCave_ReleaseWallBurst();
      Event_PlayCue(80);
    }
    Engine_TaskRemoveCallback(AltmillerCave_SpinActorWheel);
  }
}

s32 Math_DivideUnsigned(s32 value, s32 divisor);

static void FieldScene_RunExtendedActorPresentation(void) {
  u8 *display;
  struct FieldActor *actor;
  struct FieldActor *object;
  s32 i;
  s32 eva;
  s32 evb;

  GameFlag_Set(0x962);
  Engine_PartyRemoveItem(237);
  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgAltmillerCameBackPleaseDraughtQuickly);
  Event_ShowMessage(8, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 160);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_EventWait(50);
  Engine_EventWait(10);
  Camera_MoveTo(PIXELS(264), -1, PIXELS(200), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 264, 208);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  FieldMotion_LaunchFromFocusedObject(GARET, -16, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(MIA, 0, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(IVAN, 16, 16, FACING_NORTH);
  Engine_ActorWaitForMove(GARET);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | 8, 40);
  Event_ShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(20);
  actor = Actor_Get(8);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(60);
  Event_PlayCue(17);
  actor->sprite->rotation = 0;
  Engine_ActorJump(8, 10, 70);
  for (i = 0; i < 30; i++) {
    actor->sprite->priority = 0;
    Engine_EventWait(2);
    actor->sprite->priority = 2;
    Engine_EventWait(2);
  }
  Engine_EventWait(40);
  {
    s32 x;
    s32 z;

    x = Actor_Get(8)->x.part.pixel;
    z = Actor_Get(8)->z.part.pixel;
    Actor_SetPosition(8, 0, 0);
    Actor_SetPosition(9, PIXELS(x), PIXELS(z));
  }
  actor->sprite->flags = 0;
  Event_PlayCue(30);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_WalkByAndWait(9, 32, 32);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(MIA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(IVAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, 16);
  Actor_WalkBy(GARET, 0, 16);
  Actor_WalkBy(MIA, 0, 16);
  Actor_WalkByAndWait(IVAN, 0, 16);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(MIA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(IVAN)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(MIA, ANIM_STAND);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Engine_TaskWait(1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 5, 40);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 40);
  Engine_ActorJump(GARET, 4, 13);
  Engine_ActorJump(GARET, 4, 30);
  Event_ShowMessage(GARET, 0);
  Actor_FaceActor(IVAN, GARET, 30);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 40);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 30);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(GARET, 0);
  Engine_ActorSetAnimationAndWait(9, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_OpenMessage(9, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventSetMessage(MsgAltmillerWouldHaveRevealedMyselfSooner);
    Event_ShowMessage(9, 0);
  } else {
    Engine_EventSetMessage(MsgAltmillerMustTraveledIndeedIfDidnt);
    Event_ShowMessage(9, 0);
  }
  Engine_EventSetMessage(MsgAltmillerHowDidBecomeParalyzedWhile);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(20);
  Actor_SetSpeed(IVAN, 0x10000, 0x8000);
  Actor_WalkByAndWait(IVAN, 0, -48);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Engine_EventWait(40);
  Engine_PsynergyBegin(ABILITY_MIND_READ, 1);
  Engine_PsynergySetTarget(2, 9);
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_EventWait(150);
  Engine_PsynergyPlayEffect(2);
  Engine_PsynergyLowerHands();
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(40);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 40);
  Engine_EventWait(10);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorJump(9, 4, 13);
  Engine_ActorJump(9, 4, 30);
  Actor_FaceDirection(9, FACING_EAST, 0);
  Engine_EventWait(30);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 50);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Event_ShowMessage(9, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(9, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(9, 0);
  Engine_EventWait(50);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(10);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(20);
  Engine_EventWait(20);
  Actor_SetSpeed(IVAN, 0x10000, 0x8000);
  Actor_WalkByAndWait(IVAN, 0, 48);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(9, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(9, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 80);
  Engine_EventWait(20);
  Actor_FaceDirection(9, FACING_NORTH, 0);
  Engine_EventWait(50);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 40);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgAltmillerThatsKradenSaidAlchemyCould);
    Event_ShowMessage(GARET, 0);
  } else {
    Engine_EventSetMessage(MsgAltmillerForgotKradenSaidAlchemyCould);
    Event_ShowMessage(GARET, 0);
  }
  Engine_EventSetMessage(MsgAltmillerWhatWasThat);
  Engine_EventWait(10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT, 40);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Event_OpenMessage(9, 0);
  Engine_EventSetMessage(MsgAltmillerBabi);
  Engine_EventChooseYesNo(0, 0);
  display = (u8 *)gDisplayTransition;
  {
    u16 *slot = (u16 *)(display + 0x52a);
    s32 value = 32;

    *slot = value;
  }
  for (eva = 0, evb = 16; eva <= 16; eva++, evb--) {
    {
      s32 control = 0x3f42;

      REG_BLDCNT = control;
    }
    REG_BLDALPHA = (eva << 8) | evb;
    Engine_TaskWait(7);
  }
  {
    u16 *slot = (u16 *)(display + 0x536);
    s32 value = 0x3f3f;

    *slot = value;
  }
  Actor_SetPosition(10, PIXELS(280), PIXELS(304));
  Actor_SetPosition(11, PIXELS(296), PIXELS(304));
  Event_ShowMessage(10, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Engine_EventWait(10);
  Engine_CameraFollowActor(10, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(70);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(11, 0);
  Engine_ActorFollow(ACTOR_PARTY_LEADER, 10);
  Engine_ActorFollow(GARET, 10);
  Engine_ActorFollow(MIA, 10);
  Engine_ActorFollow(IVAN, 10);
  Actor_SetSpeed(10, 0x16666, 0xb333);
  Actor_SetSpeed(11, 0x16666, 0xb333);
  Actor_SetActionScript(10, gAltmillerActionA);
  Engine_EventWait(3);
  Actor_WalkByAndWait(11, -16, 0);
  Actor_SetActionScript(11, gAltmillerActionA);
  Object_RefreshSelectorById(10);
  Actor_WalkByAndWait(10, 0, -16);
  Object_RefreshSelectorById(11);
  Actor_FaceDirection(10, FACING_EAST, 0);
  Actor_FaceDirection(11, FACING_EAST, 0);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(MIA);
  Engine_ActorStop(IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Camera_MoveTo(PIXELS(248), -1, PIXELS(216), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventWait(20);
  Event_ShowMessage(10, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(50);
  Engine_EventWait(10);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(11, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Engine_EventWait(10);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(10, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(9, 0);
  Actor_WalkByAndWait(11, 16, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(11, 2);
  Engine_EventWait(20);
  Event_ShowMessage(11, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(20);
  Event_ShowMessage(9, 0);
  Actor_WalkByAndWait(10, 16, 0);
  Engine_EventWait(20);
  Event_ShowMessage(10, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Engine_ActorJump(11, 4, 13);
  Engine_ActorJump(11, 4, 30);
  Event_ShowMessage(11, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(10, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(10, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  Engine_EventWait(30);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(11, FACING_SOUTHEAST, 0);
  Engine_EventWait(70);
  Actor_FaceDirection(11, FACING_EAST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(10, FACING_EAST, 0);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(11, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 40);
  Engine_EventWait(20);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Engine_EventWait(30);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(10, 11, 70);
  Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(11, FACING_SOUTHEAST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(10, ANIM_NOD);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(11, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(11, 2);
  Engine_EventWait(20);
  Event_ShowMessage(11, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | 8, 40);
  Event_ShowMessage(11, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(10, FACING_EAST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(11, FACING_EAST, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventWait(20);
  Event_ShowMessage(10, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(10, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(11, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Engine_EventWait(40);
  Engine_ActorFollow(ACTOR_PARTY_LEADER, 9);
  Engine_ActorFollow(GARET, 9);
  Engine_ActorFollow(MIA, 9);
  Engine_ActorFollow(IVAN, 9);
  Actor_SetSpeed(9, 0x10000, 0x8000);
  Actor_WalkByAndWait(9, -16, 0);
  Actor_WalkBy(10, 0, 48);
  Actor_WalkBy(11, 0, 48);
  Actor_WalkByAndWait(9, -16, 0);
  Actor_WalkByAndWait(9, 0, 32);
  Engine_ActorSetAnimation(10, ANIM_STAND);
  Engine_ActorSetAnimation(11, ANIM_STAND);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(MIA);
  Engine_ActorStop(IVAN);
  Engine_EventWait(10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT, 40);
  Actor_FaceDirection(9, FACING_EAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(10, FACING_NORTH, 0);
  Actor_FaceDirection(11, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 30);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Actor_FaceActor(GARET, 9, 30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(20);
  Event_ShowMessage(9, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  Engine_EventWait(70);
  Actor_FaceDirection(9, FACING_EAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(9, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(9, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Engine_EventWait(30);
  Actor_SetActionScript(11, gAltmillerActionB);
  Actor_SetActionScript(10, gAltmillerActionC);
  Actor_SetActionScript(9, gAltmillerActionD);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  Object_RefreshSelectorById(9);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Camera_MoveTo(PIXELS(248), -1, PIXELS(248), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_FaceDirection(9, FACING_NORTH, 0);
  Engine_EventWait(20);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(11, FACING_WEST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(10, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(11, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessage(11, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 7, 40);
  Actor_FaceDirection(9, FACING_EAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | 2, 40);
  Engine_EventWait(30);
  Actor_FaceActor(10, 11, 60);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(10, ANIM_NOD);
  Engine_EventWait(40);
  Actor_FaceDirection(9, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(9, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Engine_EventWait(30);
  Actor_FaceDirection(9, FACING_EAST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(10, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(11, ANIM_NOD);
  Engine_EventWait(40);
  Actor_FaceDirection(10, FACING_EAST, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(11, FACING_EAST, 0);
  Engine_EventWait(40);
  Actor_WalkBy(9, 32, 0);
  Actor_WalkBy(11, 0, 64);
  Actor_WalkByAndWait(10, 16, 0);
  Actor_WalkBy(10, 0, 64);
  Engine_ActorWaitForMove(9);
  Actor_WalkByAndWait(9, 0, 64);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, 0, 0);
  Actor_SetPosition(11, 0, 0);
  Engine_EventWait(20);
  for (eva = 0, evb = 16; eva <= 16; eva++, evb--) {
    {
      s32 control = 0x3f42;

      REG_BLDCNT = control;
    }
    REG_BLDALPHA = (evb << 8) | eva;
    Engine_TaskWait(7);
  }
  {
    u16 *slot = (u16 *)(display + 0x52a);
    s32 value = 5;

    *slot = value;
  }
  {
    u16 *slot = (u16 *)(display + 0x536);
    s32 value = 31;

    *slot = value;
  }
  Engine_TaskWait(1);
  Io_Write16(0x3f42, &REG_BLDCNT);
  Io_Write16(0xc04, &REG_BLDALPHA);
  Engine_CameraMoveToActor(0, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Event_OpenMessage(GARET, 0);
  Actor_FaceActor(MIA, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgAltmillerSoWellDoIt);
    Event_ShowMessage(GARET, 0);
  } else {
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgAltmillerDontWantEitherDo);
    Event_ShowMessage(GARET, 0);
  }
  Engine_EventSetMessage(MsgAltmillerEitherWayWereStuckHere);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(GARET, 0);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventSetMessage(MsgAltmillerGreatItsDecidedJustHave);
    Event_ShowMessage(GARET, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(MIA, IVAN, 50);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    Actor_FaceDirection(IVAN, FACING_NORTH, 0);
    Engine_EventWait(30);
    Event_ShowMessage(IVAN, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_ActorSetAnimation(MIA, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(30);
  } else {
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
    Engine_EventWait(30);
    Engine_EventSetMessage(MsgAltmillerDontLetMeDownLike);
    Event_ShowMessage(GARET, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(MIA, IVAN, 50);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    Actor_FaceDirection(IVAN, FACING_NORTH, 0);
    Engine_EventWait(30);
    Event_ShowMessage(IVAN, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    Engine_EventWait(30);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 70);
  }
  Event_PlayCue(17);
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  object = Actor_Get(ACTOR_PARTY_LEADER);
  if (object != NULL) {
    Actor_SetDestination(GARET, object->x.part.pixel, object->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  object = Actor_Get(ACTOR_PARTY_LEADER);
  if (object != NULL) {
    Actor_SetDestination(IVAN, object->x.part.pixel, object->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_ActorSetAnimation(MIA, ANIM_WALK);
  object = Actor_Get(ACTOR_PARTY_LEADER);
  if (object != NULL) {
    Actor_SetDestination(MIA, object->x.part.pixel, object->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
}

static void FieldScene_RunBranchingCutsceneSequence(void) {
  struct FieldActor *leader;
  s32 line;
  GameFlag_Set(0x960);
  Engine_AudioPlayCue(24);
  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Engine_EventSetMessage(MsgAltmillerSee);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Camera_MoveTo(0xf80000, -1, 0xb80000, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 192);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  FieldMotion_LaunchFromFocusedObject(1, -16, 16, 0xc000);
  FieldMotion_LaunchFromFocusedObject(3, 0, 16, 0xc000);
  FieldMotion_LaunchFromFocusedObject(2, 16, 16, 0xc000);
  Engine_ActorWaitForMove(1);
  Engine_EventWait(20);
  Actor_ShowEmote(2, 0x102, 0);
  Engine_EventWait(40);
  Event_ShowMessage(IVAN, 0);
  Engine_ActorRunRepeatedMotion(3, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(3, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(8, 0x100, 40);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, IVAN, 50);
  Actor_FaceDirection(GARET, 0xc000, 0);
  Actor_FaceDirection(2, 0xc000, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(1, 0);
  Engine_EventWait(10);
  Actor_SetAttachedEffect(8, 0x102);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(20);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(MIA, 0xc000, 0);
  Actor_FaceDirection(2, 0xc000, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(2, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(8, 0x100, 40);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, 4);
  Engine_EventWait(20);
  Engine_EventShowMessage(3, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(8, 0x105, 40);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 30);
  Actor_ShowEmote(GARET, 0x101, 40);
  Event_ShowMessage(GARET, 0);
  Engine_ActorFaceDirection(1, 0xc000, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(1, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(8, 0x102, 40);
  Event_ShowMessage(8, 0);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(3, 4);
  Engine_EventWait(20);
  Engine_EventShowMessage(3, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(8, 0x106, 40);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 0);
  Actor_ShowEmote(MIA, 0x101, 0);
  Engine_ActorShowEmote(2, 0x101, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(2, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(2, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, 0x102, 40);
  Engine_EventShowMessage(1, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(20);
  Event_ShowMessage(8, 0);
  Engine_ActorRunRepeatedMotion(3, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(3, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(8, 0x102, 40);
  Event_OpenMessage(8, 0);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgAltmillerKiddingHaventActually);
    Engine_EventWait(20);
    Actor_ShowEmote(GARET, 0x100, 40);
    Actor_SetSpeed(GARET, 0x20000, 0x10000);
    Actor_WalkByAndWait(1, 0, -16);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 30);
    Engine_EventShowMessage(1, 0);
  } else {
    Engine_EventSetMessage(MsgAltmillerDontTrustAnyone);
    Engine_EventWait(10);
    Actor_SetSpeed(GARET, 0x10000, 0x8000);
    Actor_WalkByAndWait(1, 0, -16);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 30);
    Engine_ActorSetAnimationAndWait(GARET, 3);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
  }

  line = MsgAltmillerSaidCouldntMove;
  Engine_EventSetMessage(line);
  Actor_ShowEmote(IVAN, 0x103, 40);
  Actor_SetSpeed(IVAN, 0x20000, 0x10000);
  Actor_WalkByAndWait(2, 0, -16);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(IVAN, ACTOR_PARTY_LEADER, 30);
  Event_ShowMessage(IVAN, 0);
  Engine_ActorSetAnimationAndWait(3, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(3, 3);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, MIA, 0);
  Actor_FaceActor(GARET, MIA, 0);
  Engine_ActorFaceActor(2, 3, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(3, 0);
  Engine_EventWait(20);
  Actor_SetSpeed(GARET, 0x10000, 0x8000);
  Engine_ActorWalkByAndWait(1, 0, 16);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(1, 4);
  Engine_EventWait(30);
  Engine_ActorFaceActor(1, 0, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(1, 0);
  Engine_EventWait(30);
  Engine_ActorFaceEachOther(MIA, IVAN, 30);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(30);
  Actor_SetSpeed(IVAN, 0x10000, 0x8000);
  Engine_ActorWalkByAndWait(2, 0, 16);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_FaceDirection(GARET, 0xc000, 0);
  Actor_FaceDirection(MIA, 0xc000, 0);
  Actor_FaceDirection(2, 0xc000, 0);
  line += 7;
  Engine_EventSetMessage(line);
  Engine_EventWait(30);
  Actor_ShowEmote(8, 0x100, 40);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(2, 3);
  Engine_EventWait(30);
  Engine_EventShowMessage(2, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(8, 0x108, 40);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(2, 3);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(3, 3);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_FaceDirection(1, 0xc000, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, 0x101, 40);
  Engine_EventShowMessage(3, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_FaceDirection(1, 0xc000, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(MIA, 0xc000, 0);
  Engine_ActorFaceDirection(2, 0xc000, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(1, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(1, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(2, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(2, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(8, 0x108, 50);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, 0x101, 40);
  Engine_EventShowMessage(1, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(8, 0x102, 40);
  Event_ShowMessage(8, 0);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(3, 4);
  Engine_EventWait(20);
  Engine_EventShowMessage(3, 0);
  Engine_EventWait(10);
  Actor_FaceActor(ACTOR_PARTY_LEADER, MIA, 40);
  Engine_ActorSetAnimationAndWait(0, 3);
  Engine_EventWait(30);
  Actor_FaceActor(GARET, MIA, 0);
  Engine_ActorFaceActor(2, 3, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(30);
  Engine_AudioPlayCue(17);
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Engine_ActorSetAnimation(1, 2);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(1, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(1);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(2, 2);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(2, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(2);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_ActorSetAnimation(3, 2);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(3, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(3);
  Actor_SetPosition(MIA, 0, 0);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
}

static void EV08_TALK(void) {
  s32 question;

  Engine_EventBegin();
  question = MsgAltmillerForgetOrderRock;
  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(8, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_EventSetMessage(question + 1);
    Engine_EventShowMessage(8, 0);
  } else {
    Engine_EventWait(20);
    Engine_EventSetMessage(question + 2);
    Engine_EventShowMessage(8, 0);
  }
  Engine_EventEnd();
}

static void EV19_TOUCH(void) {
  if (Engine_GameFlagIsSet(0x960) == 0)
    return;
  if (Engine_GameFlagIsSet(0x962) != 0)
    return;

  Engine_GameFlagSet(0x961);
  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltmillerWait);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(30);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 8, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(20);
  Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_AltmillerCave2) {
    return gAltmillerCaveEvents2;
  }
  if (scene == (s32)&SceneId_AltmillerCave3) {
    return gAltmillerCaveEvents3;
  }
  if (scene == (s32)&SceneId_AltmillerCave4) {
    return gAltmillerCaveEvents4;
  }
  if (scene == (s32)&SceneId_AltmillerCave5) {
    return gAltmillerCaveEvents5;
  }
  if (scene == (s32)&SceneId_AltmillerCave6) {
    return gAltmillerCaveEvents6;
  }
  return gAltmillerCaveEventsOther;
}

static void AltmillerCave_MatchLeaderPriority(union FieldObject *fieldObject) {
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  if (actor != NULL) {
    actor->priorityFlags = 0;
    actor->sprite->priority = Object_GetById(0)->sprite->priority;
  }
}

s32 AltmillerCave_ApplyEntryState(void) {
  s8 *state;
  struct FieldActor *actor;
  s32 i;

  if (gGameState.entrance == 0) {
    if (gGameState.scene == (s32)&SceneId_AltmillerCave2) {
      gGameState.entrance = 10;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCave3) {
      gGameState.entrance = 20;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCave4) {
      gGameState.entrance = 30;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCave5) {
      gGameState.entrance = 40;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCave6) {
      gGameState.entrance = 50;
    }
  }
  Engine_GameFlagSet(0x200);
  GameFlag_Clear(0x201);
  if (gGameState.scene == (s32)&SceneId_AltmillerCave1) {
    if (gGameState.entrance == 1) {
      if (!Engine_GameFlagIsSet(0x109)) {
        *(u8 *)((u32)gSceneState + 4) = 0;
      }
      Engine_GameFlagSet(0x201);
    }
    if (gGameState.entrance == 2) {
      if (!Engine_GameFlagIsSet(0x109)) {
        *(u8 *)((u32)gSceneState + 4) = 5;
      }
      Engine_GameFlagSet(0x201);
    }
  }
  if (gGameState.scene == (s32)&SceneId_AltmillerCave2) {
    if (Engine_GameFlagIsSet(0x962)) {
      Engine_ActorSetPosition(8, 0, 0);
    } else {
      actor = Object_GetById(8);
      ((s8 *)actor->sprite)[9] = (((s8 *)actor->sprite)[9] & ~0xc) | 4;
      ((u8 *)actor->sprite)[38] = 2;
      actor->sprite->rotation = 0x4000;
    }
  }
  if (gGameState.scene == (s32)&SceneId_AltmillerCave4) {
    Engine_GameFlagClear(0x200);
    FieldScene_RedrawActorFootprint(8);
    FieldScene_RedrawActorFootprint(9);
    FieldScene_RedrawActorFootprint(10);
    if (Engine_GameFlagIsSet(0x211)) {
      Engine_ActorSetAnimation(11, 5);
      Map_CopyCellAttributes(76, 16, 1, 1, 73, 17);
    } else {
      Object_GetById(11)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    }
    Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
#if TBS_PATCH_1
    if (Engine_GameFlagIsSet(0x212)) {
      Map_CopyCellAttributes(30, 20, 1, 1, 32, 20);
    }
#endif
  }
  if (gGameState.scene == (s32)&SceneId_AltmillerCave6) {
    Engine_GameFlagClear(0x200);
    FieldScene_RedrawActorFootprint(8);
    FieldScene_RedrawActorFootprint(9);
    FieldScene_RedrawActorFootprint(10);
    /* FAKEMATCH: the three callbacks are stored as s32, so the entrance read
     * after them may alias them and stays below the last store. 2026-10-13:
     * stored through the update field itself (with a cast of the function) or
     * as void *, 2 of 403 differ: the last store moves below the entrance's
     * ldrsh. */
    *(s32 *)&Object_GetById(8)->update = (s32)AltmillerCave_MatchLeaderPriority;
    *(s32 *)&Object_GetById(9)->update = (s32)AltmillerCave_MatchLeaderPriority;
    *(s32 *)&Object_GetById(10)->update =
        (s32)AltmillerCave_MatchLeaderPriority;
    if (gGameState.entrance == 52) {
      Dma_Fill32(0, gAltmillerCave_ClearTarget, 12);
      if (!Engine_GameFlagIsSet(0x109)) {
        u8 *race = gSceneState;

        race[0] = 0;
        race[1] = 0;
        race[2] = 4;
      }
    }
    state = (s8 *)&gSceneState[1];
    if (state[0] == 99) {
      Map_CopyCellsLayered(41, 55, 3, 2, 30, 55);
      Map_CopyCellAttributes(42, 8, 1, 1, 31, 8);
    }
    if (state[0] == 2) {
      SceneActor_PlaceFiveActorsInRow((state[1] << 16) / 5 + 0x4000);
    }
    for (i = 0; i < 5; i++) {
      actor = Object_GetById(i + 11);
      actor->motionFlags = 0;
      actor->collisionFlags = 0;
      actor->scaleX = 0x10000;
      actor->scaleY = 0x10000;
      Engine_ActorSetSpriteFlags(Object_GetById(i + 11), 0);
      Engine_ActorSetAnimation(i + 11, i + 1);
    }
    Engine_ActorSetChildValue(11, 1);
    Engine_ActorSetChildValue(12, 4);
    Engine_ActorSetChildValue(13, 11);
    Engine_ActorSetChildValue(14, 2);
    Engine_ActorSetChildValue(15, 3);
    Engine_ActorSetChildValue(16, 6);
    Engine_ActorSetChildValue(17, 6);
    Engine_ActorSetChildValue(18, 6);
    Engine_ActorSetChildValue(19, 6);
    Engine_ActorSetChildValue(20, 6);
    Object_GetById(16)->sprite->priority = 3;
    Object_GetById(20)->sprite->priority = 3;
    Object_GetById(16)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Object_GetById(20)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Engine_ActorSetSpriteFlags(Object_GetById(16), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(20), 0);
  }
  if (Engine_GameFlagIsSet(0x200)) {
    SceneEffect_SetupBlendByFlag201();
  } else {
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    gEventWork->transitionFrames = 24;
  }
  return 0;
}

static void OverlayObject_UpdateThreeStateMotion(struct EffectSlot *obj) {
  s32 position[3];
  s32 x;
  s32 z;
  u8 *p;
  s32 state;

  p = (u8 *)&obj->state;
  state = *(s8 *)p;
  if (state == 0) {
    z = obj->originZ;
    x = obj->originX;
    obj->z = z;
    position[2] = z;
    obj->x = x;
    position[0] = x;
    Vector_AddPolarOffset(0x780000, Engine_RandomNext(), position);
    obj->targetX = position[0];
    obj->targetZ = position[2];
    obj->acceleration = 0x50000;
    obj->maxSpeed = 0x50000;
    obj->limitTurnStep = state;
    (*p)++;
    if ((gFrameTick & 3) == 0)
      Engine_AudioPlayCue(0x86);
  } else if (state == 1) {
    if (EffectSlot_HasPendingTarget(obj) == 0) {
      s32 value = *p;
      value--;
      *p = value;
    }
  } else if (state == 2) {
    if (EffectSlot_HasPendingTarget(obj) == 0)
      BattleFx_ClearOwnedSlot(obj);
  }
}

static void AltmillerCave_ReleaseWallBurst(void) {
  u8 *work;
  s32 pos[3];
  s32 *p;
  struct EffectSlot *object;
  s32 n;

  BattleFx_InitializeSlots();
  work = gEffectWork;
  Shop_RestoreSceneTiles(0x202108);
  p = pos;
  p[0] = 0x1f80000;
  p[1] = 0x180000;
  p[2] = 0x900000;
  Camera_WorldToScreen(p);
  object = (struct EffectSlot *)(work + 88);
  for (n = 23; n >= 0; n--) {
    s32 speed;

    EffectSlot_Initialize(object, 284, p[0], p[2]);
    EffectSlot_SetCallback(object, OverlayObject_UpdateThreeStateMotion);
    EffectSlot_SetObjectMode(object, 7);
    ObjectGroup_SetChildValueUnlessFifteen(
        object->object, (u32)(Engine_RandomNext() * 7) >> 16);
    speed = Math_DivideUnsigned(Engine_RandomNext(), 3) + 0x18000;
    object->scaleY = speed;
    object->scaleX = speed;
    Engine_TaskWait(1);
    object++;
  }
  Engine_TaskWait(80);
  Map_CopyCellsLayered(41, 55, 3, 2, 30, 55);
  Map_CopyCellAttributes(42, 8, 1, 1, 31, 8);
  Engine_TaskWait(50);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Engine_TaskWait(30);
  {
    s32 mode = 2;
    u8 *state = work + 152;

    for (n = 23; n >= 0; n--) {
      if (((s8 *)state)[5] != 0) {
        *state = mode;
      }
      state += 72;
    }
  }
  Engine_MapWaitWorkValuesBelow256();
  Shop_InitEffect();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8,  32, 8,  -8,  -32, 8,  32, -32, -16, 32, 0,
    -8,  -32, 8,  32, -32, -8,  32, 8,  -8,  -32, 8,  32,
};

u16 *gAltmillerCave_ClearTarget = gAltmillerActorWheelWork;

struct SceneEntrance gAltmillerCave_SceneTableA[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 456, 0, 504, FACING_NORTH, 0, 0, 0, 544, 528, 0},
    {2, CONDITION_ALWAYS, 280, 0, 360, FACING_SOUTH, 0, 0, 0, 544, 528, 0},
    {3, CONDITION_ALWAYS, 120, 0, 696, FACING_NORTH, 0, 0, 552, 240, 720, 0},
    {4, CONDITION_ALWAYS, 120, 0, 600, FACING_SOUTH, 0, 0, 552, 240, 720, 0},
    {10, CONDITION_ALWAYS, 392, 0, 504, FACING_SOUTH, 0, 48, 16, 496, 672, 0},
    {11, CONDITION_ALWAYS, 136, 0, 104, FACING_SOUTH, 0, 48, 16, 496, 672, 0},
    {20, CONDITION_ALWAYS, 232, 0, 216, FACING_SOUTH, 0, 64, 48, 592, 576, 0},
    {21, CONDITION_ALWAYS, 328, 0, 264, FACING_SOUTH, 0, 64, 48, 592, 576, 0},
    {30, CONDITION_ALWAYS, 200, 0, 424, FACING_SOUTH, 0, 48, 80, 576, 480, 0},
    {31, CONDITION_ALWAYS, 520, 0, 328, FACING_SOUTH, 0, 48, 80, 576, 480, 0},
    {40, CONDITION_ALWAYS, 680, 0, 152, FACING_SOUTH, 0, 0, 0, 768, 512, 0},
    {41, CONDITION_ALWAYS, 40, 0, 248, FACING_SOUTH, 0, 0, 0, 768, 512, 0},
    {50, CONDITION_ALWAYS, 296, 0, 392, FACING_SOUTH, 0, 160, 304, 576, 704, 0},
    {51, CONDITION_ALWAYS, 424, 0, 392, FACING_SOUTH, 0, 160, 304, 576, 704, 0},
    {52, CONDITION_ALWAYS, 504, 0, 232, FACING_NORTH, 0, 384, 64, 624, 240, 0},
    {53, CONDITION_ALWAYS, 504, 0, 152, FACING_SOUTH, 0, 384, 64, 624, 240, 0},
    {54, CONDITION_ALWAYS, 184, 0, 216, FACING_NORTH, 0, 64, 64, 304, 240, 0},
    {SCENE_TABLE_END},
};

u32 gAltmillerCave_SceneTableB[] = {
    SCENE_EXITS(146),        SCENE_EXIT(1, 2, 24),    SCENE_EXIT(2, 146, 3),
    SCENE_EXIT(3, 146, 2),   SCENE_EXIT(4, 147, 10),  SCENE_EXIT(0, 147, 0),
    SCENE_EXIT(10, 146, 4),  SCENE_EXIT(11, 148, 20), SCENE_EXIT(0, 148, 0),
    SCENE_EXIT(20, 147, 11), SCENE_EXIT(21, 149, 30), SCENE_EXIT(0, 149, 0),
    SCENE_EXIT(30, 148, 21), SCENE_EXIT(31, 150, 40), SCENE_EXIT(0, 150, 0),
    SCENE_EXIT(40, 149, 31), SCENE_EXIT(41, 151, 50), SCENE_EXIT(0, 151, 0),
    SCENE_EXIT(50, 150, 41), SCENE_EXIT(51, 151, 52), SCENE_EXIT(52, 151, 51),
    SCENE_EXIT(53, 151, 54), SCENE_EXIT(54, 151, 53), SCENE_EXITS_END,
};

struct ScenePlacement gAltmillerCavePlacementsOther[] = {
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltmillerCavePlacements2[] = {
    {GARET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST,
     TALK_FACE_PARTY, 0},
    {IVAN, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY,
     0},
    {MIA, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY,
     0},
    {0x41, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(232), 0, PIXELS(160),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0x34, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(-64), 0, PIXELS(-64),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0xa3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(-64), 0, PIXELS(-64),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {0xa3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(-64), 0, PIXELS(-64),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltmillerCavePlacements4[] = {
    {0x129, CONDITION_ALWAYS, 7, PIXELS(328), 0, PIXELS(256), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0x12a, CONDITION_ALWAYS, 7, PIXELS(352), 0, PIXELS(296), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0x12a, CONDITION_ALWAYS, 7, PIXELS(368), 0, PIXELS(200), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(376), 0, PIXELS(280),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0xd3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(472), 0, PIXELS(328),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0x5d, 0x71, ACTOR_STAND, PIXELS(264), 0, PIXELS(200), FACING_SOUTHWEST,
     TALK_FACE_PARTY, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltmillerCavePlacements6[] = {
    {0x129, CONDITION_ALWAYS, 7, PIXELS(392), 0, PIXELS(416), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0x12a, CONDITION_ALWAYS, 7, PIXELS(416), 0, PIXELS(456), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0x129, CONDITION_ALWAYS, 7, PIXELS(456), 0, PIXELS(480), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0x124, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(488), PIXELS(8), PIXELS(1752),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x124, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(472), PIXELS(40),
     PIXELS(1752), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x124, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(504), PIXELS(56),
     PIXELS(1752), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x124, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(536), PIXELS(40),
     PIXELS(1752), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x124, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(520), PIXELS(8), PIXELS(1752),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x125, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(440), 0, PIXELS(190),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x125, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(472), 0, PIXELS(174),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x125, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(504), 0, PIXELS(174),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x125, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(536), 0, PIXELS(174),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x125, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(568), 0, PIXELS(190),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SCENE_TABLE_END},
};

u32 gAltmillerCave_PulseScales[] = {
    0x10000,
    0x11999,
    0x13333,
    0x11999,
};

s32 gAltmillerActionA[] = {
    SC_ARG(2), SC_MOVE_BY(0, 0, PIXELS(-32)),
    SC_SYNC,   SC_MOVE_BY(PIXELS(-64), 0, 0),
    SC_SYNC,   SC_MOVE_BY(0, 0, PIXELS(-64)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gAltmillerActionB[] = {
    SC_ARG(2), SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,   SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,   SC_MOVE_BY(PIXELS(48), 0, 0),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gAltmillerActionC[] = {
    SC_ARG(2), SC_MOVE_BY(0, 0, PIXELS(16)),
    SC_SYNC,   SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,   SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,   SC_MOVE_BY(PIXELS(32), 0, 0),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gAltmillerActionD[] = {
    SC_ARG(2), SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,   SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,   SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,   SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

// clang-format off
struct SceneEvent gAltmillerCaveEventsOther[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {0x31, 4, CONDITION_ALWAYS, 4},
    {EVENT_TOUCH, 60, CONDITION_ALWAYS, EVENT_SCRIPT(EV60_TOUCH)},
    {EVENT_TOUCH, 61, CONDITION_ALWAYS, EVENT_SCRIPT(EV61_TOUCH)},
    {EVENT_TOUCH, 62, CONDITION_ALWAYS, EVENT_SCRIPT(EV62_TOUCH)},
    {EVENT_TOUCH, 63, CONDITION_ALWAYS, EVENT_SCRIPT(EV63_TOUCH)},
    {EVENT_TOUCH, 64, CONDITION_ALWAYS, EVENT_SCRIPT(EV64_TOUCH)},
    {EVENT_TOUCH, 65, CONDITION_ALWAYS, EVENT_SCRIPT(EV65_TOUCH)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_ApplyRoomVisuals)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(SceneState_RunFlag200SetupAndPlaceActors16To20)},
    {PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_RespawnActorObject)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCaveEvents2[] = {
    {0x21, 10, CONDITION_ALWAYS, 10},
    {0x31, 11, CONDITION_ALWAYS, 11},
    {EVENT_TALK, 8, 0x960, EVENT_SCRIPT(FieldScene_RunBranchingCutsceneSequence)},
    {EVENT_TALK, 8, 0xf30, EVENT_SCRIPT(EV08_TALK)},
    {EVENT_TALK, 8, 0x962, EVENT_SCRIPT(FieldScene_RunExtendedActorPresentation)},
    {EVENT_TOUCH, 19, 0x961, EVENT_SCRIPT(EV19_TOUCH)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, 0x960,
        EVENT_SCRIPT(FieldScene_RunBranchingCutsceneSequence)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0xf30, MSGNO(MsgAltmillerNeedDraught)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, 0x962,
        EVENT_SCRIPT(FieldScene_RunExtendedActorPresentation)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(SceneState_RunFlag200SetupAndPlaceActors16To20)},
    {PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_RespawnActorObject)},
    {SEARCH(SEARCH_CHEST), 100, 0xef5, 0x500005},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCaveEvents3[] = {
    {0x21, 20, CONDITION_ALWAYS, 20},
    {0x31, 21, CONDITION_ALWAYS, 21},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(SceneState_RunFlag200SetupAndPlaceActors16To20)},
    {PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_RespawnActorObject)},
    {SEARCH(SEARCH_CHEST), 100, 0xf9c, FIND_ITEM(ITEM_VIAL)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCaveEvents4[] = {
    {0x21, 30, CONDITION_ALWAYS, 30},
    {0x31, 31, CONDITION_ALWAYS, 31},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 72, 0x211, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {TOUCH_WALKING_INTO(FACING_WEST), 73, 0x211, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 74, 0x211, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {TOUCH_WALKING_INTO(FACING_EAST), 75, 0x211, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {0x202, 71, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunGuardedSixWordStep)},
    {TOUCH_WALKING_INTO(FACING_EAST), 77, CONDITION_ALWAYS, EVENT_SCRIPT(EV77_TOUCH)},
    {PSYNERGY_ON_ACTOR(0x8c), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_PSYNERGY)},
    {0x10001815, 11, 0x211, EVENT_SCRIPT(EV11_SCRIPT)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x211, EVENT_SCRIPT(EV11_PSYNERGY)},
    {EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCaveEvents5[] = {
    {0x21, 40, CONDITION_ALWAYS, 40},
    {0x31, 41, CONDITION_ALWAYS, 41},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(SceneState_RunFlag200SetupAndPlaceActors16To20)},
    {PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_RespawnActorObject)},
    {SEARCH(SEARCH_CHEST), 100, 0xf9d, FIND_ITEM(0xc0)},
    {SCENE_EVENTS_END},
};
// clang-format on

struct SceneEvent gAltmillerCaveEvents6[] = {
    {0x21, 50, CONDITION_ALWAYS, 50},
    {EVENT_EXIT, 51, CONDITION_ALWAYS, 51},
    {EVENT_EXIT, 52, CONDITION_ALWAYS, 52},
    {EVENT_EXIT, 53, CONDITION_ALWAYS, 53},
    {EVENT_EXIT, 54, CONDITION_ALWAYS, 54},
    {0x202, 70, CONDITION_ALWAYS,
     EVENT_SCRIPT(FieldScene_RunGuardedSixWordStep)},
    {EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {SEARCH(SEARCH_CHEST), 100, 0xf30, FIND_ITEM(0xed)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_ApplyRoomVisuals)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
     EVENT_SCRIPT(SceneState_RunFlag200SetupAndPlaceActors16To20)},
    {SCENE_EVENTS_END},
};

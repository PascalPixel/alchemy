
#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"
/// @file
#include "types.h"
#include "flags.h"
#include "system/main_routine_work.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "field/event_inline.h"
#include "shared/tables.h"
#include "shared/mars_lighthouse.h"
#include "shared/actors.h"
#include "shared/effects.h"
#include "system/ram_buffer.h"
#include "event_work.h"
#include "actor.h"
#include "version.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "call.h"
#include "system/input.h"
#include "system/frame.h"
#include "system/heap_release.h"
#include "item_icon.h"
#include "module/field/field.h"
#include "module/scene/magma_rock.h"
#include "module/system/system.h"
#include "field/particle.h"
#include "graphics/resource.h"
#include "system/io_reg.h"
#include "field_event.h"
#include "fixed_math.h"
#include "system/iwram_call.h"
#include "system/task.h"

void Actor_SelectAnimation(struct Actor *object, s32 argument);
extern void Actor_StartScript(struct FieldEffect *effect, const s32 *script);
extern s32 gFieldEffect_DurationScripts[];
void Actor_SetSpriteFlags(struct Actor *object, u32 value);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party =
      ((struct FieldEffect *)Actor_GetById(gMainRoutineWork.currentOwner));
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - 0x10000) / count;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000280(void) { Djinn_JoinParty(0x14, 0, 0x10); }

void Func_02000290(void) { Djinn_JoinParty(0x18, 1, 0x13); }

#include "shared/distance.c"

#undef FIELD_AT

#ifndef TLA_PROX_LOCAL_H
#define TLA_PROX_LOCAL_H

/// @file
/// Overlay 6A8's scene layer: the declarations its scenes share and the
/// inline helpers some of their calls go through, as overlay 694's do.

#include "types.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "event_work.h"

#define EVENT_WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

#endif

extern s16 gData_02004b2c[];
extern u32 gProx_Room2Placements[];
extern s16 gProx_Room3Placements[];
extern s32 gData_02004f04[];
extern u32 gProx_Room5Placements[];
extern u32 gData_020054a4[];
extern u32 gProx_Room6Placements[];
extern struct Trigger gProx_Triggers[];
extern struct Trigger gProx_Room1Triggers[];
extern struct Trigger gProx_Room2Triggers[];
extern struct Trigger gProx_Room3Triggers[];
extern struct Trigger gProx_Room5Triggers[];
extern struct Trigger gProx_Room6Triggers[];

extern u32 gData_02004980[];

extern u32 gData_02004a08[];
extern s32 gProx_EnterDoorScript[];

void Func_020027e4(void);

void Prox_InitializeRoom3(void);

void Func_020029c8(void);

void Func_02002a38(void);
extern u32 gData_020047ac[];
extern u32 gData_02004848[];
extern u32 gData_020048e4[];
void Func_02001c24(s32 value);
extern u32 gData_02005f2c[];
extern u32 gData_02005f70[];
extern u32 gData_02005fac[];
extern u32 gData_02005fe8[];
extern u32 gData_020060cc[];
extern u32 gData_0200617c[];
extern u32 gData_020061fc[];
extern u32 gData_0200622c[];
void Func_020038e8(void);
void Particles_UpdateFalling(void);

void Func_02000290(void);

void Prox_PraiseReturnedWarrior(void);

void Prox_FearForWeyard(void);
void Prox_TalkToPlayingChild(void);

void Prox_TalkToTrainingChild(void);

void Prox_SearchForParents(void);

void Prox_DecideToGoToLighthouse(void);

void Prox_MeetPuelle(void);

void Func_020015d0(void);

void Func_02001770(void);
void Func_020017e4(void);
void Func_02001800(void);

void Func_02001a9c(void);

void Func_02001ab8(void);
void Func_02001ad4(void);
void Func_02001af0(void);

void Func_02001b0c(void);

void Func_02001b28(void);

void Func_02001b60(void);
void Func_02001b7c(void);
void Prox_SayFarewell(void);
void Leader_SlideOnIce(void);
extern u32 gData_02004644[];
extern u32 gData_02004680[];

void Func_020018e0(void);

#include "text/msg_ids.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void Actor_SelectAnimation(struct Actor *object, s32 mode);

s32 Func_020002d8(struct Actor *object, struct Actor *target, s32 range,
                  s32 force) {
  s32 result = 0;
  s32 *targetPos;
  s32 *objectPos;

  if (BYTE_AT(object, 99) != 0) {
    object->movementState = 1;
    return 1;
  }
  targetPos = &target->x;
  objectPos = &object->x;
  if (SceneActor_CalculateFixedPointDistance(targetPos, objectPos) < range ||
      force != 0) {
    u32 angle = (u16)ArcTan2(target->z - object->z, *targetPos - *objectPos);
    u32 left = (angle - 0x1000) & 0xf000;
    u32 right = (angle + 0x1000) & 0xf000;
    u32 ahead = angle & 0xf000;
    u32 facing = object->facing & 0xf000;

    if (ahead == facing || right == facing || left == facing || force != 0) {
      /* FAKEMATCH(temporary): the count and movementState's address held in
       * locals before the test. */
      s16 count = ((struct EventWork *)Ram_HeapSlots->eventWork)->blockedSteps;
      u8 *movementState = &object->movementState;

      if (count > 120) {
        *movementState = 0;
        Actor_SelectAnimation(object, 2);
      } else {
        *movementState = 1;
        Actor_SelectAnimation(object, 1);
        result = 1;
      }
    }
  } else {
    object->movementState = 0;
    Actor_SelectAnimation(object, 2);
  }
  return result;
}

#if TLA_PATCH_1

/// Once the leader is within 11 of the actor, stops it
/// (Actor_SelectAnimation mode 2). Otherwise it runs Func_020002d8 toward
/// object 18, 16 or 17 by its action (0, 1, other) at 24, and if that returns
/// 0, toward the leader at 32, flagged when action bit 1 is set while the event
/// work's halfword 0x160 or byte 4 of the window tiles is nonzero.
s32 Func_020003bc(struct Actor *actor) {
  u8 *tiles = Ram_HeapSlots->windowTiles;
  u8 *work = Ram_HeapSlots->eventWork;
  s32 flag = 0;
  struct Actor *target;

  target = Actor_Get(gMainRoutineWork.currentOwner);
  if (SceneActor_CalculateFixedPointDistance(&target->x, &actor->x) <= 11) {
    actor->movementState = flag;
    Actor_SelectAnimation(actor, 2);
    return 0;
  }
  if (actor->timer == 0)
    target = Actor_Get(18);
  else if (actor->timer == 1)
    target = Actor_Get(16);
  else
    target = Actor_Get(17);
  if (Func_020002d8(actor, target, 24, 0) == 0) {
    target = Actor_Get(gMainRoutineWork.currentOwner);
    if ((*(s16 *)(work + 352) != 0 || tiles[4] != 0) && (actor->timer & 2))
      flag = 1;
    Func_020002d8(actor, target, 32, flag);
  }
  return 0;
}

#else

s32 Func_020003bc(struct Actor *actor) {
  u8 *tiles = Ram_HeapSlots->windowTiles;
  u8 *work = Ram_HeapSlots->eventWork;
  s16 *action = &actor->timer;
  s32 flag = 0;
  struct Actor *target;

  if (*action == 0)
    target = Actor_Get(18);
  else if (*action == 1)
    target = Actor_Get(16);
  else
    target = Actor_Get(17);
  if (Func_020002d8(actor, target, 24, 0) == 0) {
    target = Actor_Get(gMainRoutineWork.currentOwner);
    if ((*(s16 *)(work + 352) != 0 || tiles[4] != 0) && (*action & 2))
      flag = 1;
    Func_020002d8(actor, target, 32, flag);
  }
  return 0;
}

#endif

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern u32 gData_02004b14[];
extern u32 gData_0200506c[];
/* VIEW(prox): variable gData_0200521c as extern struct ActorPlacement
 * gData_0200521c[]; the definition has s16 gData_0200521c[] = { 175, - 1, 3, 0,
 * 0, 104, 0, 0, 0, 196, 0x8000, 0, 172, - 1, 1, 0, 0, 128, 0, 0, 0, 106,
 * 0xb000, 1, 178, - 1, 1, 0, 0...; tried as the definition's extern s16
 * gData_0200521c[], it does not compile in tla-ja:
 * games/tla/src/scene/prox.c:371: request for member `talkFacing' in something
 * not a structure or union. */
extern struct ActorPlacement gData_0200521c__prox[] __asm__("gData_0200521c");

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Prox1 && Event_GetFlag(0x9c5) &&
      !Event_GetFlag(0x9cc))
    return gData_02004b2c;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Prox2)
    return gProx_Room2Placements;
  if (scene == (s32)&SceneId_Prox3) {
    if (Event_GetFlag(0xa3d))
      return gData_02004f04;
    return gProx_Room3Placements;
  }
  if (scene == (s32)&SceneId_Prox4) {
    if (Event_GetFlag(0xa3d)) {
      gData_0200521c__prox[1].talkFacing = 3;
      gData_0200521c__prox[4].talkFacing = 3;
      return gData_0200521c__prox;
    }
    return gData_0200506c;
  }
  if (scene == (s32)&SceneId_Prox5) {
    if (Event_GetFlag(0xa3d))
      return gData_020054a4;
    return gProx_Room5Placements;
  }
  if (scene == (s32)&SceneId_Prox6)
    return gProx_Room6Placements;
  return gData_02004b14;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Actor_ReturnHome();

void Func_02000588(s32 a0) { Call3(Actor_ReturnHome, a0, 0x40, 0x300); }

extern struct Trigger gData_02005a20[];

const u8 *Scene_GetEvents(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Prox1)
    return gProx_Room1Triggers;
  if (scene == (s32)&SceneId_Prox2)
    return gProx_Room2Triggers;
  if (scene == (s32)&SceneId_Prox3)
    return gProx_Room3Triggers;
  if (scene == (s32)&SceneId_Prox4)
    return gData_02005a20;
  if (scene == (s32)&SceneId_Prox5)
    return gProx_Room5Triggers;
  if (scene == (s32)&SceneId_Prox6)
    return gProx_Room6Triggers;
  return gProx_Triggers;
}

s32 Func_0200446c();

void Prox_AskAboutIgloos(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgProx33);
  Event_AskYesNo(15, 0);
  Func_0200446c();
}

DEFINE_LEADER_IS_FACING_NORTH(Leader_IsFacingNorth)

void Prox_TalkAboutParents(void) {

  if (Leader_IsFacingNorth()) {
    Inn_CheckIn(0xe, 0x16);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (Event_GetFlag(0xa3d)) {
      FieldEvent_SetMessageId((s32)MsgProx135);
    } else {
      FieldEvent_SetMessageId((s32)MsgProx56);
    }
    Event_ShowMessage(22, 0);
    Func_0200446c();
  }
}

s32 Shop_Run();

void Prox_PraiseReturnedWarrior(void) {

  if (Leader_IsFacingNorth()) {
    Shop_Run(0x1f, 0xa);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (Event_GetFlag(0xa3d)) {
      FieldEvent_SetMessageId((s32)MsgProx132);
    } else {
      FieldEvent_SetMessageId((s32)MsgProx53);
    }
    Event_ShowMessage(10, 0);
    Func_0200446c();
  }
}

s32 Sanctum_Run();

void Prox_FearForWeyard(void) {

  if (Leader_IsFacingNorth()) {
    Sanctum_Run(8);
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    if (Event_GetFlag(0xa3d)) {
      FieldEvent_SetMessageId((s32)MsgProx139);
    } else {
      FieldEvent_SetMessageId((s32)MsgProx60);
    }
    Event_ShowMessage(8, 0);
    Func_0200446c();
  }
}

/// The bytes of an actor these handlers use. unk63 is the last of the four
/// scene-owned bytes the Broken Seal's actors keep at this place.
struct TalkActor {
  u8 unk00[6];
  u16 angle; ///< 1/65536 turns
  u8 unk08[0x5b];
  u8 unk63; ///< Func_020002d8 reads it
};

void Prox_TalkToPlayingChild(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = ((struct TalkActor *)Actor_Get(16));
  s16 angle = object->angle;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_FaceActor(16, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId((s32)MsgProx113);
  } else {
    FieldEvent_SetMessageId((s32)MsgProx117);
  }
  Event_ShowMessage(16, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_0200446c();
}

void Prox_TalkToRestlessChild(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = ((struct TalkActor *)Actor_Get(17));
  s16 angle = object->angle;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_FaceActor(17, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId((s32)MsgProx114);
  } else {
    FieldEvent_SetMessageId((s32)MsgProx118);
  }
  Event_ShowMessage(17, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_0200446c();
}

void Prox_TalkToTrainingChild(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct TalkActor *object = ((struct TalkActor *)Actor_Get(18));
  s16 angle = object->angle;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object->unk63 = 1;
  if (work->unk166 == 0) {
    Actor_FaceActor(18, gMainRoutineWork.currentOwner, 0);
    FieldEvent_SetMessageId((s32)MsgProx115);
  } else {
    FieldEvent_SetMessageId((s32)MsgProx119);
  }
  Event_ShowMessage(18, 0);
  if (work->unk166 == 0)
    object->angle = angle;
  object->unk63 = 0;
  Func_0200446c();
}

void Prox_SearchForParents(void) {
  s32 t0;
  s32 t8;

  t0 = Event_GetFlag(0xa3d);
  if (t0 != 0) {
  } else {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(SE_DOOR_OPEN);
    MapGrid_CopyCellsTo(89, 0, 77, 16, 1, 2);
    Event_SetFlag(FLAG_SCENE_TEMP_00);
    FieldEvent_Wait(10);
    {
      s32 y = 0xc0;
      Actor_SetPositionAndFacing(8, PIXELS(520), PIXELS(360), y << 6);
    }
    t8 = ((s32)FieldCamera_GetActor());
    FIELD_AT((t8 + 0x55), u8, 0) = t0;
    FieldCamera_SetSpeed(1.6);
    FieldCamera_MoveTo(552, CAMERA_KEEP, 388, 1);
    FieldEvent_Wait(10);
    Actor_SetSpeed(8, 1.2);
    Actor_WalkToAndWait(8, 526, 384);
    Actor_FaceDirection(9, FACING_WEST, 0);
    Actor_WalkToAndWait(8, 540, 388);
    Actor_FaceDirectionAndWait(8, FACING_EAST);
    Actor_SetAnimation(8, 4);
    FieldEvent_SetMessageId((s32)MsgProx47);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 8, 0);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Event_ShowMessage(9, 0);
    Actor_SetAnimation(8, 3);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 8, 0);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    Actor_FaceDirection(9, FACING_NORTH_NORTHWEST, 0);
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(8, FACING_NORTH_NORTHWEST, 40);
    Actor_FaceDirectionAndWait(8, FACING_EAST);
    Actor_SetAnimation(8, 4);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 8, 0);
    Actor_FaceDirectionAndWait(9, FACING_WEST);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessage(9, 0);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 8, 0);
    Actor_SetAnimation(9, 3);
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(9, FACING_NORTH_NORTHWEST, 0);
    Actor_FaceDirectionAndWait(8, FACING_NORTH_NORTHWEST);
    Event_SetFlag(0x9c0);
    Func_0200446c();
  }
}

void FieldCamera_WaitForArrival(void);

void Prox_DecideToGoToLighthouse(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(160, CAMERA_KEEP, 160, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(10, 1);
  Actor_FaceDirectionAndWait(10, FACING_NORTH_NORTHWEST);
  FieldEvent_SetMessageId((s32)MsgProx62);
  Event_ShowMessage(10, 0);
  Actor_SetAttachedEffect(12, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(13, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(14, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(15, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(16, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(17, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(10, FACING_SOUTH_SOUTHWEST);
  Event_ShowMessage(10, 0);
  Event_ShowMessage(10, 0);
  Actor_SetAttachedEffect(12, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(13, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(14, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(15, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(16, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(17, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Actor_SetAnimation(12, 4);
  Event_ShowMessage(12, 0);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHEAST, 0);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_SetAnimationAndWait(13, 4);
  Event_ShowMessage(13, 0);
  FieldActor_Jump(15, 4, 10);
  Event_ShowMessage(15, 0);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHWEST, 0);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(16, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  Event_ShowMessage(16, 0);
  FieldActor_Jump(14, 2, 0);
  Actor_SetAnimation(14, 3);
  Event_ShowMessage(14, 0);
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_FaceDirectionAndWait(10, FACING_SOUTH_SOUTHWEST);
  Event_ShowMessage(10, 0);
  Actor_FaceDirectionAndWait(11, FACING_EAST);
  Event_ShowMessage(11, 0);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Actor_SetAnimation(11, 4);
  Event_ShowMessage(11, 0);
  Actor_SetAttachedEffect(10, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_FaceDirection(13, FACING_EAST, 0);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Actor_FaceDirection(15, FACING_EAST, 0);
  Actor_FaceDirection(16, FACING_WEST, 0);
  Actor_FaceDirection(17, FACING_EAST, 40);
  Actor_FaceDirection(12, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(13, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(14, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(15, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(16, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(17, FACING_NORTH_NORTHEAST, 20);
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Event_ShowMessage(11, 0);
  Actor_FaceDirection(10, FACING_NORTH_NORTHWEST, 80);
  Actor_SetAnimationAndWait(10, 4);
  Event_ShowMessage(10, 0);
  Event_SetFlag(0x9c1);
  Func_0200446c();
}

s32 Actor_MoveToObject();
extern u32 gData_020049c4[];

void Prox_MeetPuelle(void) {
  u32 i;
  s32 result;
  s32 owner;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(160, CAMERA_KEEP, 160, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 134, 156);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  FieldActor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHWEST);
  FieldEvent_SetMessageId((s32)MsgProx75);
  Event_ShowMessage(11, 0);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(10, FACING_WEST, 40);
  Event_ShowMessage(10, 0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 144, 154);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_MoveToObject(0x12, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(0, gMainRoutineWork.currentOwner);
  System_Wait(1);
  Actor_FaceDirection(14, FACING_NORTH_NORTHWEST, 0);
  Actor_SetSpeed(18, 0.8);
  Actor_SetSpeed(0, 0.8);
  Actor_SetScript(0, (s32)gData_020049c4);
  Actor_RunScriptAndWait(18, (s32)gData_02004980);
  Actor_FaceDirection(11, FACING_SOUTH_SOUTHEAST, 0);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessage(11, 0);
  Actor_SetAnimation(18, 3);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  Actor_SetAnimation(0, 3);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 0, 0);
  Actor_FaceDirection(11, FACING_EAST, 0);
  Actor_FaceDirection(10, FACING_WEST, 40);
  Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHEAST);
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(10, FACING_SOUTH_SOUTHWEST);
  Event_ShowMessage(10, 0);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_FaceDirection(13, FACING_EAST, 0);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Actor_FaceDirection(15, FACING_EAST, 0);
  Actor_FaceDirection(16, FACING_WEST, 0);
  Actor_FaceDirection(17, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(0, FACING_SOUTH);
  Actor_SetSpeed(0, 2.4);
  Actor_Get(0)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(0, 160, 140);
  FieldEvent_Wait(1);
  Actor_Get(0)->actionFlags |= 1;
  Actor_SetAnimationAndWait(0, 0x15);
  Actor_FaceDirection(12, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(13, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(14, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(15, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(16, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(17, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(18, FACING_NORTH_NORTHWEST);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  Actor_SetAnimation(0, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimation(0, 1);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
  Actor_SetAnimation(18, 3);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_SetSpeed(0, 0.8);
  Actor_WalkToAndWait(0, 160, 154);
  Actor_FaceDirection(0, FACING_NORTHWEST, 0);
  Actor_SetAnimationAndWait(18, 4);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  FieldActor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  FieldActor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_ShowEmote(0, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 0, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(18, FACING_WEST, 40);
  Actor_SetAnimationAndWait(0, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 0, 0);
  // Waits up to 80 frames, or until a key is held.
  i = 0;
  goto test_wait;
next_wait:
  i++;
test_wait:
  if (i < 80) {
    System_Wait(1);
    if (gInput.held == 0)
      goto next_wait;
  }
  Actor_RunRepeatedMotion(12, 1);
  Event_ShowMessage(12, 0);
  Actor_ShowEmote(0, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
  Actor_SetAnimationAndWait(0, 3);
  FieldActor_ShowEmote(15, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessage(15, 0);
  Actor_FaceDirectionAndWait(0, FACING_SOUTH);
  Actor_SetAnimationAndWait(0, 3);
  Actor_FaceDirection(10, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(11, FACING_EAST);
  Actor_SetAnimation(13, 3);
  Event_ShowMessage(13, 0);
  Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
  Actor_SetAnimationAndWait(0, 3);
  Actor_FaceDirection(11, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHWEST, 0);
  Actor_SetAnimationAndWait(16, 3);
  Event_ShowMessage(16, 0);
  Actor_FaceDirectionAndWait(0, FACING_SOUTHWEST);
  Actor_ShowEmote(0, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Actor_SetAnimation(14, 3);
  Event_ShowMessage(14, 0);
  Actor_FaceDirectionAndWait(0, FACING_SOUTH);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(17, 4);
  Event_ShowMessage(17, 0);
  Actor_FaceDirection(0, FACING_SOUTHWEST, 20);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(18, FACING_NORTH_NORTHWEST, 20);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Event_ShowMessage(11, 0);
  Actor_FaceDirection(0, FACING_NORTH, 40);
  Actor_SetAnimationAndWait(10, 3);
  Event_ShowMessage(10, 0);
  Actor_FaceDirectionAndWait(18, FACING_NORTH_NORTHEAST);
  Actor_SetAnimation(18, 3);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  Actor_SetAnimation(11, 3);
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(0, FACING_NORTH);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 0, 0);
  Actor_SetAnimation(10, 3);
  Event_ShowMessage(10, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  Actor_SetAnimation(11, 4);
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(0, FACING_WEST);
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 0, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  result =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  owner = gMainRoutineWork.currentOwner;
  if (result == 0) {
    Actor_WaitForAnimationChange(owner);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 0, 0);
    EVENT_WORK->messageId++;
  } else {
    FieldActor_WaitForMoveById(owner);
    EVENT_WORK->messageId++;
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 0, 0);
  }
  FieldActor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_FaceDirectionAndWait(18, FACING_WEST);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  Actor_FaceDirection(11, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Actor_SetAnimation(11, 3);
  Actor_SetAnimationAndWait(10, 3);
  Actor_FaceDirection(11, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(10, FACING_SOUTH_SOUTHWEST);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessage(10, 0);
  Actor_SetAnimation(11, 3);
  Event_ShowMessage(11, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(0, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(18, FACING_NORTHWEST);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimationAndWait(18, 3);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_FaceDirection(13, FACING_EAST, 0);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Actor_FaceDirection(15, FACING_EAST, 0);
  Actor_FaceDirection(16, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(17, FACING_EAST);
  Actor_SetAnimation(12, 3);
  Actor_SetAnimation(13, 3);
  Actor_SetAnimation(14, 3);
  Actor_SetAnimation(15, 3);
  Actor_SetAnimation(16, 3);
  Actor_SetAnimationAndWait(17, 3);
  Actor_FaceDirectionAndWait(18, FACING_WEST);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | 18, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(0, FACING_EAST);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimationAndWait(0, 3);
  Actor_SetScript(0, (s32)gData_02004a08);
  Actor_RunScriptAndWait(18, (s32)gData_02004a08);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 160, 154);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  Actor_RunRepeatedMotion(10, 1);
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 20);
  Actor_SetAnimation(10, 4);
  Event_ShowMessage(10, 0);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirectionAndWait(11, FACING_EAST);
  Event_ShowMessage(11, 0);
  Actor_FaceDirectionAndWait(10, FACING_WEST);
  Actor_SetAnimationAndWait(10, 3);
  Actor_FaceDirectionAndWait(10, FACING_SOUTH_SOUTHWEST);
  Event_ShowMessage(10, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(11, 3);
  Event_ShowMessage(11, 0);
  Event_SetFlag(0x9c2);
  Func_0200446c();
}

void Func_020015d0(void) {
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(20, 2);
  object = Actor_Get(20);
  object->terrainId = 1;
  FieldActor_SetSpritePriority(20, 3);
  object = Actor_Get(gMainRoutineWork.currentOwner);
  object->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 408, 326);
  FieldEvent_Wait(1);
  object = Actor_Get(gMainRoutineWork.currentOwner);
  object->actionFlags |= 1;
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Event_PlaySound(SE_RATTLE);
  object = (struct Actor *)((s32)FieldCamera_GetActor());
  object->y += 0x20000;
  object = Actor_Get(20);
  object->actionFlags &= 0xfe;
  Actor_WalkToAndWait(20, 408, 200);
  FieldEvent_Wait(1);
  object = Actor_Get(20);
  object->actionFlags |= 1;
  Actor_SetPosition(20, 0, 0);
  Event_PlaySound(SE_KOLIMA_BRIDGE_HOP);
  MapGrid_SetShake(1, 2, 1);
  FieldEvent_Wait(30);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_PlaySound(SE_LANDING_THUD);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_ENEMY_CRY);
  FieldEvent_Wait(20);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  FieldActor_SetSpritePriority(20, 2);
  object = Actor_Get(20);
  object->priorityFlags |= 1;
  MapGrid_CopyCellsTo(72, 3, 69, 3, 3, 3);
  MapGrid_CopyCellsTo(48, 36, 69, 8, 3, 2);
  MapGrid_CopyCellAttributes(72, 13, 1, 3, 70, 20);
  Event_SetFlag(0xa2d);
  Func_0200446c();
}

void Func_02001770(void) {
  if (Event_GetFlag(0xa3d) == 0 && Event_GetFlag(64) == 0) {
    u8 *work = Ram_HeapSlots->eventWork;

    Event_Start();
    FieldEvent_ResolvePendingActions(1);
    {
      s32 y = 0x80;
      Actor_SetPositionAndFacing(20, PIXELS(408), PIXELS(145), y << 7);
    }
    System_Wait(1);
    FieldActor_Jump(20, 4, 20);
    Actor_FaceActor(20, gMainRoutineWork.currentOwner, 0);
    FIELD_AT(work, u16, 362) = 200;
    Func_0200446c();
  }
}

void Func_020017e4(void) { MapGrid_CopyCellAttributes(25, 2, 1, 1, 25, 9); }

void Func_02001800(void) { MapGrid_CopyCellAttributes(25, 3, 1, 1, 25, 9); }

struct RisingObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  u8 unk10[8];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[0x44];
  s16 drift;
  u8 unk66[2];
  s32 life;
};

void Actor_Release(struct RisingObject *object);

void Func_0200181c(struct RisingObject *object) {
  object->x += object->drift << 8;
  object->y += 0x8000;
  object->scaleX += 0x51e;
  object->scaleY += 0x51e;
  object->drift += 2;
  if (--object->life == 0)
    Actor_Release(object);
}

void Func_02001860(s32 a0, s32 a1, s32 a2) {
  s32 t0;

  t0 = FieldActor_Create(0x11d, a0, a1, a2);
  if (t0 != 0) {
    FIELD_AT((t0 + 0x5c), u8, 0) = 2;
    FIELD_AT((t0 + 0x55), u8, 0) = 0;
    FIELD_AT(((t0 + 0x55) + 0xf), u16, 0) = 0;
    FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
    FieldActor_SetSpriteColor(t0, 7);
    FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
        ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
    Actor_SetSpriteFlags(t0, 0);
    FIELD_AT(t0, s32, 24) = 0x8000;
    FIELD_AT(t0, s32, 28) = 0x8000;
    FIELD_AT(t0, s32, 104) = 0x3c;
    FIELD_AT(t0, s32, 108) = (s32)Func_0200181c;
    Call2(Actor_SelectAnimation, t0, 5);
    Actor_SetBlendMode(t0, 1);
  }
}

void Func_020018e0(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Prox2) {
    if ((gFrameCount & 63) == 0)
      Func_02001860(536 << 16, 40 << 16, 232 << 16);
    if (((gFrameCount + 60) & 63) == 0)
      Func_02001860(168 << 16, 40 << 16, 296 << 16);
    if (((gFrameCount + 20) & 63) == 0) {
      Func_02001860(280 << 16, 42 << 16, 184 << 16);
      Func_02001860(152 << 16, 48 << 16, 424 << 16);
    }
    if (((gFrameCount + 40) & 63) == 0)
      Func_02001860(336 << 16, 42 << 16, 362 << 16);
  } else if (scene == (s32)&SceneId_Prox3) {
    if ((gFrameCount & 63) == 0)
      Func_02001860(568 << 16, 104 << 16, 296 << 16);
  }
}

struct EventWork154 {
  u8 unk000[0x154];
  s16 returnEntrance;
};

void Prox_EnterDoor(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f) {
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  s32 cell;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (!Event_GetFlag(FLAG_SCENE_TEMP_00)) {
    if (mode == 2)
      Event_PlaySound(SE_THUMP);
    else
      Event_PlaySound(SE_DOOR_OPEN);
    MapGrid_CopyCellsTo(a, b, c, d, mode, f);
    FieldEvent_Wait(20);
  }
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  if (mode == 1) {
    object = Actor_Get(gMainRoutineWork.currentOwner);
    cell = object->x >> 16;
    FieldActor_SetMoveTargetFixed(object, ((8 - cell % 16) << 16) + object->x,
                                  object->y, object->z);
    FieldActor_WaitForMove(object);
    object->facing = 0xc000;
  }
  Actor_SetScript(gMainRoutineWork.currentOwner, gProx_EnterDoorScript);
  System_Wait(12);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->returnEntrance);
}

void Func_02001a9c(void) { Prox_EnterDoor(0x30, 0x27, 0x39, 0x1a, 1, 2); }

void Func_02001ab8(void) { Prox_EnterDoor(0x31, 0x27, 0x41, 0x18, 1, 2); }

void Func_02001ad4(void) { Prox_EnterDoor(0x32, 0x27, 0x38, 0x13, 1, 2); }

void Func_02001af0(void) { Prox_EnterDoor(0x33, 0x27, 0x3d, 0xd, 1, 2); }

void Func_02001b0c(void) { Prox_EnterDoor(0x33, 0x27, 0x50, 0x10, 1, 2); }

void Func_02001b28(void) { Call6(Prox_EnterDoor, 0x34, 0x27, 0x32, 9, 2, 2); }

void Func_02001b44(void) { Prox_EnterDoor(0x57, 2, 0x50, 7, 1, 2); }

void Func_02001b60(void) { Prox_EnterDoor(0x59, 0, 0x4d, 0x10, 1, 2); }

void Func_02001b7c(void) {
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  s32 entrance = work->returnEntrance;
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 cell;
  s32 id;

  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  cell = object->x >> 16;
  FieldActor_SetMoveTargetFixed(object, ((8 - cell % 16) << 16) + object->x,
                                object->y, object->z - 0x40000);
  FieldActor_WaitForMove(object);
  object->facing = 0xc000;
  Event_PlaySound(SE_STAIRS_UP);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  id = gMainRoutineWork.currentOwner;
  if (entrance == 6)
    Actor_SetAnimation(id, 25);
  else
    Actor_SetAnimation(id, 26);
  System_Wait(4);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(entrance);
}

void Func_02001c24(s32 a0) {
  struct Actor *object;

  object = Actor_Get(gMainRoutineWork.currentOwner);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (a0 != 0) {
    Actor_SetSpriteFlags(((s32)Actor_Get(gMainRoutineWork.currentOwner)), 0);
    FieldActor_SetPositionAndResetMotion(object, (object->x + -65536),
                                         object->y, (object->z + -393216));
    System_Wait(1);
    Event_StartScreenTransition();
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 0x17);
  } else {
    FieldActor_SetPositionAndResetMotion(object, (object->x + 0x10000),
                                         object->y, (object->z + -393216));
    System_Wait(1);
    Event_StartScreenTransition();
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 0x18);
  }
  Actor_SetSpriteFlags(((s32)Actor_Get(gMainRoutineWork.currentOwner)), 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.5);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, 8);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Func_0200446c();
}
#undef BYTE_AT
#undef FIELD_AT

// object the overlay links.

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void FieldCamera_WaitForArrival(void);
s32 Func_0200446c();

void Prox_GiveThanksForBeacon(void) {
  s32 t144;
  s32 t159;
  s32 t202;
  s32 t207;
  s32 angleC000;
  s32 angleB000;
  s32 angle3000;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldActor_Stop(20);
  System_Wait(1);
  Actor_SetPosition(gMainRoutineWork.currentOwner, 184, 484);
  /* FAKEMATCH(temporary): the three facings held in locals set before their
   * first placement, where as constants each placement rebuilds them and the
   * later turns cannot share them. */
  angleC000 = 0xc000;
  angleB000 = 0xb000;
  Actor_SetPositionAndFacing(0, PIXELS(168), PIXELS(484), angleC000);
  Actor_SetPositionAndFacing(27, PIXELS(200), PIXELS(484), angleB000);
  angle3000 = 0x3000;
  Actor_SetPositionAndFacing(17, PIXELS(138), PIXELS(412), angle3000);
  Actor_SetPositionAndFacing(20, PIXELS(220), PIXELS(600), angleB000);
  Actor_SetPosition(5, 184, 684);
  System_Wait(1);
  EVENT_WORK->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(184, CAMERA_KEEP, 448, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId((s32)MsgProx80);
  Event_ShowMessage(26, 0);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(27, 3);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_FaceDirection(26, FACING_SOUTH_SOUTHWEST, 0);
  FieldActor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessage(26, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirection(0, FACING_EAST, 20);
  FieldActor_ShowEmote(gMainRoutineWork.currentOwner,
                       EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  FieldActor_ShowEmote(0, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  Event_ShowMessage(0, 0);
  Actor_SetAttachedEffect(25, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Event_ShowMessage(25, 0);
  Actor_SetAnimation(27, 4);
  Event_ShowMessage(27, 0);
  Actor_SetAnimation(19, 4);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 19, 0);
  FieldActor_ShowEmote(0, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessage(0, 0);
  Actor_FaceDirection(27, FACING_WEST, 20);
  FieldActor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Event_ShowMessage(27, 0);
  Event_ShowMessage(17, 0);
  Actor_FaceDirection(27, FACING_NORTH_NORTHWEST, 0);
  FieldActor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessage(27, 0);
  Actor_FaceDirectionAndWait(26, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimation(26, 4);
  Event_ShowMessage(26, 0);
  Event_ShowMessage(0, 0);
  Actor_SetAttachedEffect(19, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 19, 0);
  Actor_FaceDirectionAndWait(0, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(0, 3);
  Event_ShowMessage(0, 0);
  Actor_FaceDirection(26, FACING_SOUTH_SOUTHWEST, 0);
  Actor_SetAttachedEffect(26, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Event_ShowMessage(26, 0);
  Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHEAST, 0);
  Actor_ShowEmote(25, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Event_ShowMessage(25, 0);
  Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(26, FACING_SOUTH_SOUTHEAST, 0);
  Event_ShowMessage(26, 0);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Event_ShowMessage(0, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHWEST, 0);
  Actor_ShowEmote(25, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Event_ShowMessage(25, 0);
  Actor_SetAnimation(26, 4);
  FieldEvent_ShowMessageThenWait(26, 0, 20);
  Actor_FaceDirectionAndWait(0, FACING_EAST);
  Actor_ShowEmote(0, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | 0, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  if (!FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner,
                                             0)) {
    Actor_SetAnimationAndWait(26, 3);
    Event_ShowMessage(26, 0);
    EVENT_WORK->messageId++;
  } else {
    EVENT_WORK->messageId++;
    Actor_SetAnimationAndWait(26, 4);
    Event_ShowMessage(26, 0);
  }
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  Actor_SetAnimation(27, 4);
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_ShowEmote(25, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessage(25, 0);
  Actor_FaceDirection(27, FACING_NORTH_NORTHWEST, 20);
  Actor_SetAnimationAndWait(27, 3);
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 27, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(0, FACING_EAST);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimationAndWait(0, 3);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 0, 0);
  Actor_SetAttachedEffect(26, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Event_ShowMessage(26, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Actor_FaceDirection(27, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(25, FACING_WEST);
  Actor_SetAnimationAndWait(25, 3);
  Actor_FaceDirectionAndWait(25, FACING_SOUTH_SOUTHWEST);
  Event_ShowMessage(25, 0);
  FieldActor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessage(27, 0);
  Actor_SetAnimation(27, 4);
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(0, FACING_EAST, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_ShowEmote(gMainRoutineWork.currentOwner,
                       EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(0, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldActor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(25, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_SetAnimationAndWait(27, 3);
  Event_ShowMessage(27, 0);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimation(26, 3);
  Actor_SetAnimation(25, 3);
  Actor_FaceDirection(27, FACING_WEST, 0);
  Actor_ShowEmote(27, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessage(27, 0);
  Actor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  FieldEvent_ShowMessageThenWait(26, 0, 20);
  Event_ShowMessage(5, 0);
  Actor_SetAttachedEffect(27, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(0, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(27, FACING_SOUTH_SOUTHWEST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 27, 0);
  Actor_FaceDirectionAndWait(0, FACING_EAST);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | 0, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  t144 =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (t144 == 0) {
    FieldCamera_SetSpeed(2.4);
    FieldCamera_MoveTo(184, CAMERA_KEEP, 512, 1);
    Actor_FaceDirectionAndWait(0, FACING_SOUTH);
    Actor_SetSpeed(0, 1.6);
    Actor_WalkToAndWait(0, 168, 520);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 0, 0);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 5, 0);
    EVENT_WORK->messageId += 2;
  } else {
    EVENT_WORK->messageId += 2;
    FieldCamera_SetSpeed(2.4);
    FieldCamera_MoveTo(184, CAMERA_KEEP, 512, 1);
    Actor_FaceDirectionAndWait(0, FACING_SOUTH);
    Actor_SetSpeed(0, 1.6);
    Actor_WalkToAndWait(0, 168, 520);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 0, 0);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 5, 0);
  }
  t159 = ((s32)FieldCamera_GetActor());
  FIELD_AT((t159 + 0x55), u8, 0) = 0;
  FieldCamera_MoveTo(158, CAMERA_KEEP, 432, 1);
  FieldCamera_WaitForArrival();
  FieldActor_Jump(17, 4, 10);
  Event_ShowMessage(17, 0);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(172, CAMERA_KEEP, 520, 1);
  FieldActor_WatchAndSetScript(0, 0x11);
  FieldActor_WatchAndSetScript(gMainRoutineWork.currentOwner, 0x11);
  FieldActor_WatchAndSetScript(27, 0x11);
  Actor_SetSpeed(17, 1.6);
  Actor_WalkToAndWait(17, 140, 478);
  Actor_WalkToAndWait(17, 150, 540);
  Actor_WalkToAndWait(17, 160, 560);
  Actor_WalkToAndWait(17, 190, 640);
  FieldActor_Stop(0);
  FieldActor_Stop(gMainRoutineWork.currentOwner);
  FieldActor_Stop(27);
  System_Wait(1);
  Actor_SetPosition(17, 0, 0);
  FieldEvent_Wait(10);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(184, CAMERA_KEEP, 448, 1);
  Actor_WalkToAndWait(0, 168, 484);
  Actor_FaceDirectionAndWait(0, FACING_EAST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 0, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(27, FACING_WEST);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 27, 0);
  Actor_FaceDirection(25, FACING_SOUTH_SOUTHWEST, 0);
  Actor_SetAnimationAndWait(25, 3);
  Event_ShowMessage(25, 0);
  Actor_FaceDirection(0, FACING_NORTH, 0);
  Actor_FaceDirection(27, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_NORTH);
  Actor_SetAnimationAndWait(26, 3);
  Event_ShowMessage(26, 0);
  Actor_SetAnimation(0, 3);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimationAndWait(27, 3);
  Actor_SetSpeed(0, 1.6);
  Actor_SetSpeed(27, 1.6);
  Actor_SetAnimation(0, 2);
  t202 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if (t202 != 0) {
    Actor_SetDestination(0, FIELD_AT(t202, s16, 10), FIELD_AT(t202, s16, 18));
  }
  FieldActor_WaitForMoveById(0);
  Actor_SetPosition(0, 0, 0);
  Actor_SetAnimation(27, 2);
  t207 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if (t207 != 0) {
    Actor_SetDestination(27, FIELD_AT(t207, s16, 10), FIELD_AT(t207, s16, 18));
  }
  FieldActor_WaitForMoveById(27);
  Actor_SetPosition(27, 0, 0);
  {
    struct EventWork *work = EVENT_WORK;

    work->screenEffect = 0x209;
    work->delay = 0x10;
  }
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  Event_SetFlag(0x9c5);
  Func_0200446c();
}

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

void Actor_SetWideSprite(s32 id, s32 wide);
void Map_ClearLayerEntryFlag(s32 layer);
void Map_SetLayerEntryFlag(u32 layer);
void FieldEffect_StartBlankOverlay(s32 a);
void Weather_StartRain(s32 a);

s32 Scene_Initialize(void) {
  s32 scene;

  if (Event_GetFlag(0xa3d)) {
    Event_SetFlag(0x17c);
    Event_SetFlag(FLAG_RETREAT_BLOCKED);
  }
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Prox4) {
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
    Func_020029c8();
  } else if (scene == (s32)&SceneId_Prox5) {
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
    Func_02002a38();
  } else if (scene == (s32)&SceneId_Prox6) {
    s32 *effect =
        &((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect;
    /* FAKEMATCH(pin): the call's argument pinned to the r0 it is passed in. */
    register s32 skip __asm__("r0");
    s32 value = 0x209;

    skip = 0;
    *effect = value;
    FieldEffect_StartBlankOverlay(skip);
  } else {
    if (!Event_GetFlag(0xa3d))
      Event_SetFlag(FLAG_SCENE_TEMP_01);
    if (Event_GetFlag(FLAG_SCENE_TEMP_01)) {
      Weather_StartRain(1);
    } else {
      REG_BLDALPHA = 0xc08;
      REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
      Task_Add(Func_020018e0, 0x480);
      Map_SetLayerEntryFlag(0);
      Map_SetLayerEntryFlag(1);
      Map_ClearLayerEntryFlag(2);
      Map_ClearLayerEntryFlag(3);
      Map_ClearLayerEntryFlag(4);
      Map_ClearLayerEntryFlag(5);
      Map_ClearLayerEntryFlag(6);
    }
    scene = gMainRoutineWork.scene;
    if (scene == (s32)&SceneId_Prox2) {
      if (!Event_GetFlag(FLAG_AREA_TEMP_00))
        Actor_SetWideSprite(64, 1);
      Func_020027e4();
    } else if (scene == (s32)&SceneId_Prox3) {
      Prox_InitializeRoom3();
    }
    if (Event_GetFlag(FLAG_SCENE_TEMP_01)) {
      BattleEffect_ApplyColorToTargetBuffer(0x10004, 1);
      PaletteFade_StartInterpolation(16);
      wait(16);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

struct Walker {
  u8 unk00[0x63];
  u8 arrived;
  u16 frames;
  u8 unk66[6];
  void (*update)(void);
};

void Func_0200279c(s32 actor, s32 x, s32 z, s32 frames) {
  struct Walker *object = ((struct Walker *)Actor_Get(actor));

  object->frames = frames;
  object->arrived = 0;
  object->update = Func_020003bc;
  Actor_SetPosition(actor, x, z);
}

void Func_020027e4(void) {

  if (Event_GetFlag(0xa3d)) {
    if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
      Func_0200279c(0x10, 536, 360, 0);
      Func_0200279c(0x11, 536, 424, 1);
      Func_0200279c(0x12, 580, 420, 2);
      System_Wait(1);
      Actor_SetScript(16, (s32)gData_020047ac);
      Actor_SetScript(17, (s32)gData_02004848);
      Actor_SetScript(18, (s32)gData_020048e4);
      System_Wait(1);
    }
  }
  FieldActor_SetSpritePriority(20, 1);
  FieldActor_SetSpriteColor(((s32)Actor_Get(20)), 0xf);
}

void Prox_InitializeRoom3(void) {
  struct Actor *object;

  FieldActor_SetSpritePriority(19, 1);
  FieldActor_SetSpriteColor(((s32)Actor_Get(19)), 0xf);
  if (!Event_GetFlag(0xa3d)) {
    if (!Event_GetFlag(0x9c0)) {
      Actor_SetPosition(8, 0, 0);
    }
    if (Event_GetFlag(FLAG_SCENE_TEMP_00)) {
      MapGrid_CopyCellsTo(89, 0, 77, 16, 1, 2);
    }
    if (Event_GetFlag(0x9c1)) {
      object = Actor_Get(10);
      object->facing = 0xb000;
      System_Wait(1);
    }
  }
  if (Event_GetFlag(0xa3d)) {
    MapGrid_CopyCellsTo(72, 3, 69, 3, 3, 3);
    MapGrid_CopyCellsTo(48, 36, 69, 8, 3, 2);
    MapGrid_CopyCellAttributes(72, 13, 1, 3, 70, 20);
    Actor_SetPosition(20, 0, 0);
  } else {
    if (Event_GetFlag(0xa2d)) {
      MapGrid_CopyCellsTo(72, 3, 69, 3, 3, 3);
      MapGrid_CopyCellsTo(48, 36, 69, 8, 3, 2);
      MapGrid_CopyCellAttributes(72, 13, 1, 3, 70, 20);
      if (!Event_GetFlag(0x40)) {
        Actor_SetPosition(20, 0, 0);
      }
    }
  }
}

void Func_020029a4(void) {
  struct Actor *object;

  object = Actor_Get(gMainRoutineWork.currentOwner);
  FieldActor_SetSpritePriority(
      16,
      ((u32)(FIELD_AT(FIELD_AT((u8 *)object, s32, 80), u8, 9) << 28) >> 30));
}

void Func_020029c8(void) {

  FieldEffect_StartBlankOverlay(0);
  Task_AddFrame((s32)Func_020029a4, 0x480);
  if (gMainRoutineWork.entrance == 0x14) {
    if (!Event_GetFlag(0x9c5)) {
      Prox_GiveThanksForBeacon();
    }
  }
}

void Func_02002a08(void) {
  s32 variant =
      ((struct FieldSprite *)Actor_Get(gMainRoutineWork.currentOwner)->sprite)
          ->priority;

  FieldActor_SetSpritePriority(8, variant);
  FieldActor_SetSpritePriority(10, variant);
}

// Unless flag 0x109 is set, calls Func_02001c24 with 0 for entrance 6 and with
// 1 for entrances 7 and 8, then schedules Func_02002a08.

void Func_02002a38(void) {
  s32 priority;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
    s32 entrance = (u16)gMainRoutineWork.entrance;

    if (gMainRoutineWork.entrance == 6)
      Func_02001c24(0);
    else if ((u32)((entrance - 7) << 16) <= 0x10000)
      Func_02001c24(1);
  }
  FieldEffect_StartBlankOverlay(0);
  // The priority in a local is built before the task function's address (passed
  // directly, the address loads first).
  priority = 0x480;
  Task_AddFrame(Func_02002a08, priority);
}

s32 Actor_MoveToObject();
extern u32 gData_02005ee8[];
extern u32 gData_02006038[];
extern u32 gData_02006088[];
extern u32 gData_020060fc[];
extern u32 gData_02006268[];

void Prox_SayFarewell(void) {
  u32 i;

  if (Event_GetFlag(0x9c5) != 0) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    FieldActor_Jump(5, 2, 10);
    FieldActor_Jump(5, 4, 10);
    FieldEvent_SetMessageId((s32)MsgProx153);
    Event_ShowMessage(5, 0);
    FieldCamera_SetSpeed(3.2);
    FieldCamera_MoveTo(144, CAMERA_KEEP, 228, 1);
    FieldCamera_WaitForArrival();
    FieldActor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 10, 0);
    Event_PlaySound(MUS_037);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 136, 196);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Actor_MoveToObject(0, gMainRoutineWork.currentOwner);
    Actor_MoveToObject(0xb, gMainRoutineWork.currentOwner);
    System_Wait(1);
    Actor_SetSpeed(0, 0.8);
    Actor_SetSpeed(11, 0.8);
    Actor_SetScript(0, (s32)gData_02005ee8);
    Actor_RunScriptAndWait(11, (s32)gData_02005f2c);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessage(9, 0);
    Actor_FaceDirectionAndWait(5, FACING_SOUTHWEST);
    Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
    Event_ShowMessage(5, 0);
    Actor_FaceDirection(6, FACING_NORTHEAST, 0);
    FieldActor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    Event_ShowMessage(6, 0);
    Actor_FaceDirectionAndWait(7, FACING_WEST);
    Actor_SetAnimationAndWait(7, 3);
    Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
    Event_ShowMessage(7, 0);
    Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    Event_ShowMessage(5, 0);
    Actor_SetAnimation(3, 4);
    Event_ShowMessage(3, 0);
    Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Actor_FaceDirectionAndWait(10, FACING_NORTH_NORTHEAST);
    Event_ShowMessage(10, 0);
    Actor_FaceDirectionAndWait(5, FACING_SOUTH);
    FieldActor_Jump(5, 4, 10);
    Event_ShowMessage(5, 0);
    FieldActor_ShowEmote(1, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
    Actor_FaceDirectionAndWait(1, FACING_NORTHWEST);
    Event_ShowMessage(1, 0);
    Actor_ShowEmote(2, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    Actor_FaceDirectionAndWait(2, FACING_NORTHWEST);
    Event_ShowMessage(2, 0);
    Actor_ShowEmote(0, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
    Actor_FaceDirectionAndWait(0, FACING_EAST);
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | 0, 0, 10);
    Actor_FaceDirection(8, FACING_SOUTH_SOUTHEAST, 20);
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 8, 0, 20);
    if (Event_GetFlag(0x824) != 0) {
      Actor_FaceDirectionAndWait(0, FACING_SOUTH);
      Actor_SetAnimation(0, 4);
      Event_ShowMessage(0, 0);
      Actor_FaceDirectionAndWait(8, FACING_NORTH_NORTHEAST);
      Event_ShowMessage(8, 0);
      EVENT_WORK->messageId++;
    } else {
      EVENT_WORK->messageId += 2;
      Actor_FaceDirectionAndWait(8, FACING_NORTH_NORTHEAST);
      Event_ShowMessage(8, 0);
    }
    Actor_FaceDirectionAndWait(0, FACING_SOUTH);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 11, 0);
    Actor_FaceDirection(11, FACING_SOUTH_SOUTHEAST, 20);
    Actor_FaceDirection(11, FACING_WEST, 20);
    Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHWEST);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 11, 0);
    Actor_FaceDirectionAndWait(10, FACING_WEST);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | 10, 0);
    Actor_FaceDirectionAndWait(9, FACING_EAST);
    Actor_SetAnimation(9, 3);
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | 9, 0);
    Actor_FaceDirectionAndWait(8, FACING_EAST);
    Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | 8, 0);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 11, 0);
    Actor_FaceDirectionAndWait(0, FACING_EAST);
    Actor_SetAnimation(0, 3);
    Event_OpenMessage(SPEAKER_WINDOW_BELOW | 0, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner,
                                              0) == 0) {
      Actor_SetAnimationAndWait(0, 3);
      Event_ShowMessage(SPEAKER_WINDOW_BELOW | 0, 0);
      EVENT_WORK->messageId++;
    } else {
      EVENT_WORK->messageId++;
      Actor_SetAnimationAndWait(0, 4);
      Event_ShowMessage(SPEAKER_WINDOW_BELOW | 0, 0);
    }
    Actor_FaceDirectionAndWait(5, FACING_NORTHWEST);
    FieldActor_Jump(5, 2, 10);
    FieldActor_Jump(5, 4, 10);
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | 5, 0);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(5, 3);
    Actor_SetAnimation(6, 3);
    Actor_SetAnimation(7, 3);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(1, 3);
    Actor_SetSpeed(0, 0.8);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
    Actor_SetSpeed(11, 0.8);
    Actor_SetSpeed(5, 0.8);
    Actor_SetSpeed(6, 0.8);
    Actor_SetSpeed(7, 0.8);
    Actor_SetSpeed(8, 0.8);
    Actor_SetSpeed(9, 0.8);
    Actor_SetSpeed(10, 0.8);
    Actor_SetSpeed(3, 0.8);
    Actor_SetSpeed(2, 0.8);
    Actor_SetSpeed(1, 0.8);
    FieldActor_SetSpritePriority(0, 2);
    FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
    FieldActor_SetSpritePriority(11, 2);
    FieldActor_SetSpritePriority(5, 2);
    FieldActor_SetSpritePriority(6, 2);
    FieldActor_SetSpritePriority(7, 2);
    FieldActor_SetSpritePriority(8, 2);
    FieldActor_SetSpritePriority(9, 2);
    FieldActor_SetSpritePriority(10, 2);
    FieldActor_SetSpritePriority(3, 2);
    FieldActor_SetSpritePriority(2, 2);
    FieldActor_SetSpritePriority(1, 2);
    FieldCamera_SetSpeed(0.4);
    FieldCamera_MoveTo(144, CAMERA_KEEP, 246, 1);
    Actor_SetScript(9, (s32)gData_02005f70);
    Actor_SetScript(10, (s32)gData_02005fac);
    FieldEvent_Wait(10);
    Actor_SetScript(8, (s32)gData_02005fe8);
    Actor_SetScript(3, (s32)gData_02006038);
    FieldEvent_Wait(10);
    Actor_SetScript(6, gData_02005fe8);
    Actor_SetScript(7, (s32)gData_02005fe8);
    FieldEvent_Wait(10);
    Actor_SetScript(2, gData_02006038);
    Actor_SetScript(1, (s32)gData_02006088);
    FieldEvent_Wait(30);
    Actor_SetScript(5, (s32)gData_020060cc);
    FieldEvent_Wait(20);
    Actor_SetScript(0, (s32)gData_020060fc);
    Actor_SetScript(gMainRoutineWork.currentOwner, (s32)gData_0200617c);
    FieldEvent_Wait(10);
    Actor_RunScriptAndWait(11, (s32)gData_020061fc);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    Actor_FaceDirection(11, FACING_NORTH_NORTHWEST, 0);
    FieldActor_RefreshSelectorById(5);
    Actor_FaceDirection(5, FACING_WEST, 0);
    Actor_FaceDirection(1, FACING_EAST, 40);
    Actor_FaceDirection(5, FACING_NORTHWEST, 0);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_FaceDirection(0, FACING_EAST, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner,
                                              0) == 0) {
      Actor_SetAnimationAndWait(11, 3);
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
      EVENT_WORK->messageId++;
    } else {
      EVENT_WORK->messageId++;
      Actor_SetAnimationAndWait(11, 4);
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    }
    Actor_FaceDirection(0, FACING_NORTH, 20);
    Event_ShowMessage(0, 0);
    Actor_SetAnimation(11, 4);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    FieldActor_Jump(1, 2, 10);
    Event_ShowMessage(1, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_FaceDirection(11, FACING_SOUTH_SOUTHEAST, 20);
    Event_ShowMessage(11, 0);
    Actor_SetAnimationAndWait(5, 3);
    Event_ShowMessage(5, 0);
    Actor_SetAnimationAndWait(11, 4);
    Event_ShowMessage(11, 0);
    FieldActor_ShowEmote(1, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    FieldEvent_ShowMessageThenWait(1, 0, 20);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(11, 0);
    Actor_FaceDirection(0, FACING_EAST, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 40);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
    Event_ShowMessage(0, 0);
    Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHWEST);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner,
                                              0) == 0) {
      Actor_SetAnimationAndWait(11, 4);
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
      EVENT_WORK->messageId++;
    } else {
      EVENT_WORK->messageId++;
      Actor_SetAnimationAndWait(11, 3);
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    }
    FieldActor_ShowEmote(1, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    Event_ShowMessage(1, 0);
    Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHEAST);
    Event_ShowMessage(11, 0);
    Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Event_ShowMessage(5, 0);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessage(0, 0);
    Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHWEST);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(5, 3);
    FieldEvent_Wait(20);
    Event_ShowMessage(1, 0);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHEAST);
    Event_ShowMessage(11, 0);
    Actor_SetAttachedEffect(1, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAnimationAndWait(1, 4);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(11, 0);
    Actor_SetAnimationAndWait(5, 3);
    Event_ShowMessage(5, 0);
    FieldActor_Jump(1, 2, 10);
    Event_ShowMessage(1, 0);
    Actor_SetAnimationAndWait(11, 3);
    Actor_FaceDirectionAndWait(0, FACING_SOUTH);
    Event_ShowMessage(0, 0);
    Actor_FaceDirectionAndWait(5, FACING_WEST);
    Actor_SetAnimation(5, 3);
    Event_ShowMessage(5, 0);
    Actor_FaceDirectionAndWait(1, FACING_EAST);
    Actor_SetAnimation(5, 3);
    Actor_SetAnimationAndWait(1, 3);
    FieldEvent_Wait(10);
    Actor_SetSpeed(5, 1.6);
    Actor_SetSpeed(1, 1.6);
    Actor_SetScript(5, (s32)gData_0200622c);
    Actor_RunScriptAndWait(1, (s32)gData_02006268);
    FieldEvent_Wait(20);
    Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHWEST);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_WalkToAndWait(11, 156, 256);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Event_ShowMessage(0, 0);
    Actor_SetAnimation(0, 4);
    Event_ShowMessage(0, 0);
    Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHWEST);
    Event_ShowMessage(0, 0);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_FaceDirection(0, FACING_EAST, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner,
                                              0) == 0) {
      Actor_SetAnimationAndWait(11, 3);
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
      EVENT_WORK->messageId++;
    } else {
      EVENT_WORK->messageId++;
      Actor_SetAnimationAndWait(11, 4);
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    }
    Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
    Actor_SetAnimation(11, 4);
    FieldEvent_ShowMessageThenWait(
        SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0, 20);
    Event_ShowMessage(0, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(11, 3);
    FieldEvent_Wait(20);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_FaceDirection(0, FACING_EAST, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 40);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
    Event_ShowMessage(0, 0);
    Actor_SetAnimation(11, 4);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    FieldActor_ShowEmote(0, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(gMainRoutineWork.currentOwner,
                    EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(20);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_FaceDirection(11, FACING_SOUTH_SOUTHEAST, 20);
    FieldActor_Jump(11, 4, 10);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHWEST);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHEAST);
    FieldActor_Jump(11, 2, 10);
    FieldActor_Jump(11, 4, 10);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_FaceDirection(0, FACING_EAST, 0);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_WEST);
    Actor_FaceDirectionAndWait(11, FACING_NORTH_NORTHWEST);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Actor_FaceDirectionAndWait(11, FACING_SOUTH_SOUTHEAST);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | 11, 0);
    Actor_SetAnimation(11, 9);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 0x21);
    Actor_SetAnimation(0, 0x28);
    Map_SetLayerEntryFlag(2);
    Map_SetLayerEntryFlag(3);
    Map_SetLayerEntryFlag(4);
    Map_SetLayerEntryFlag(5);
    Map_SetLayerEntryFlag(6);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    FieldEvent_Wait(40);
    PaletteFade_ApplyTarget(0x10009, 0);
    PaletteFade_Interpolate(120);
    System_Wait(120);
    // Waits up to 120 frames, or until a key is held.
    i = 0;
    goto test_wait;
  next_wait:
    i++;
  test_wait:
    if (i < 120) {
      System_Wait(1);
      if (gInput.held == 0)
        goto next_wait;
    }
    PaletteFade_ApplyTarget(0, 0);
    PaletteFade_Interpolate(16);
    System_Wait(16);
    Event_SetFlag(0x9cc);
    FieldEvent_SetExitRequest(3);
  }
}

void Actor_SelectAnimation(struct Actor *object, s32 mode);
extern s16 gData_02004740[];

DEFINE_ACTOR_STEER_BY_PAD(Actor_SteerByPad, gData_02004740,
                          Actor_SelectAnimation)

void Func_020038e8(void) {
  void *work = Ram_HeapSlots->eventWork;

  if (FIELD_AT(work, s16, 0xca2) != 0) {
    Event_SetFlag(FLAG_MENU_OPEN);
    Leader_StopMoving();
    Event_ResolveAllPendingActions();
    Event_ClearFlag(FLAG_MENU_OPEN);
  }
}
#undef FIELD_AT

// object the overlay links.

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

void Actor_SelectAnimation(struct Actor *actor, s32 mode);
s32 Map_GetTerrainHeight(u8 layer, s32 x, s32 z);
extern s16 gData_02004780[];

DEFINE_LEADER_SLIDE_ON_ICE(Leader_SlideOnIce, Func_020038e8, gData_02004780,
                           Actor_SelectAnimation, Actor_SteerByPad,
                           Field_CheckSpheresOverlap)

extern volatile s16 gProx_SlideStopped;

s32 Actor_HasArrived(struct Actor *object);

#define SLIDE_FROM_EDGE Leader_SlideOnIceFromEdge
#define SLIDE_STOPPED gProx_SlideStopped
#define SLIDE_STOP_LEADER Func_020038e8
#define SLIDE_ANGLES gData_02004780
#define SLIDE_SET_ANIMATION Actor_SelectAnimation
#define SLIDE_HAS_ARRIVED Actor_HasArrived
#define SLIDE_KEEP_SLIDING Actor_SteerByPad
#define SLIDE_SPHERES_OVERLAP Field_CheckSpheresOverlap
#include "shared/slide_on_ice_from_edge.c"
#undef SLIDE_FROM_EDGE
#undef SLIDE_STOPPED
#undef SLIDE_STOP_LEADER
#undef SLIDE_ANGLES
#undef SLIDE_SET_ANIMATION
#undef SLIDE_HAS_ARRIVED
#undef SLIDE_KEEP_SLIDING
#undef SLIDE_SPHERES_OVERLAP

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Leader_SetSlideStopped(s32 a0) { gProx_SlideStopped = a0; }

DEFINE_LEADER_SLIDE_ON_ICE_AHEAD(Leader_SlideOnIceAhead, Func_020038e8,
                                 Actor_SelectAnimation, Actor_SteerByPad,
                                 Field_CheckSpheresOverlap)

struct ParticleSprite {
  u8 unk0[8];
  u16 attr;
  u8 unkA[30];
};

struct ParticleWork {
  u8 tiles[0x500];
  struct Particle particles[64];
  struct ParticleSprite sprites[64];
  s16 next;
  u16 tile;
};

void EffectSprite_DrawAtWorldPosition(struct ParticleSprite *sprite,
                                      struct Particle *particle);
void BattleEffect_StepVector3(struct Particle *particle, s32 unk1, s32 step);

DEFINE_PARTICLES_UPDATE_FALLING(Particles_UpdateFalling,
                                EffectSprite_DrawAtWorldPosition)

/// One falling rock: where it is, how it moves and how long it has fallen.
struct Rock {
  s32 position[3];
  s32 velocity[3];
  s32 timer;
};

/// A falling rock's sprite.
struct RockSprite {
  u8 unk00[5];
  u8 unk05;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x14];
  u16 unk1e;
  u8 unk20[8];
};

/// The falling rocks' work: their tiles, 64 rocks and their sprites, in a
/// ring that next walks round.
struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

/// Spawns count rocks around (x, y, z), each pushed out at a random angle and
/// thrown up and out along it.
DEFINE_ROCKS_SPAWN(Rocks_Spawn)
#undef FIELD_AT

// object the overlay links, and the overlay's data.

extern char ResourceId_FallingRock;

/* VIEW(s4254): tag struct RockSprite as struct RockSprite__s4254; prox.c has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja: games/tla/src/scene/prox.c:2168: structure has no
 * member named `unk05_5'. */
struct RockSprite__s4254 {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 unk05_5 : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

void Func_02004604(struct RockSprite__s4254 *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);

void Func_02004254(void) {
  struct RockWork *work = HeapSlot_Alloc(220, sizeof(struct RockWork));
  s32 entry;
  s32 tile;
  s32 i;
  struct Rock *rock;
  struct RockSprite__s4254 *sprite;

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_FallingRock),
                      work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  for (i = 63; i >= 0; i--, sprite++, rock++) {
    Func_02004604(sprite, 8, 8, 0x40000000, tile);
    sprite->unk05_5 = 1;
    sprite->palette = 0;
    sprite->priority = 0;
    sprite->unk1e = 240;
    rock->timer = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  FieldTask_AddFrame(Particles_UpdateFalling, 0x480);
}

struct SparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

void Func_02004310(void) {
  struct SparkWork *work = Ram_HeapSlots->menuRuntime;

  FieldTask_Remove(Particles_UpdateFalling);
  VramCache_FreeEntry(work->resource);
  HeapSlot_Release(220);
}

#include "text/msg_ids.h"

void Func_02000280(void);
void Prox_AskAboutIgloos(void);
void Prox_TalkAboutParents(void);
void Prox_TalkToRestlessChild(void);

extern u32 gData_020046bc[];

// clang-format off
u32 gData_02004644[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02004680[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020046bc[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s32 gProx_EnterDoorScript[] = {
    22, 26, 0, 23, 7, -0x8000,
    23, 9, -1024, 23, 10, -1024,
    0, 1, 13, 24, 0, 17,
};

s16 gData_02004740[] = {-1, 0, -32768, -1, -16384, -8192, -24576, -16384, 16384, 8192, 24576, 16384, -1, 0, -32768, -1, -1, 0, -32768, -1, -16384, -1, -1, -16384, 16384, -1, -1, 16384, -1, 0, -32768, -1};

s16 gData_02004780[] = {-1, 0, -32768, -1, -16384, 0, -32768, -16384, 16384, 0, -32768, 16384, -1, 0, -32768, -1};

s32 gFieldEffect_DurationScripts[] = {
    (s32)gData_02004644,
    (s32)gData_02004680,
    (s32)gData_020046bc,
};

DEFINE_G_DATA_020047AC(gData_020047ac, 0x2300000, 0x17c0000, 0x2340000,
    0x1a80000, 0x2180000, 0x1a80000, 0x2180000, 0x17c0000)

DEFINE_G_DATA_020047AC(gData_02004848, 0x2180000, 0x17c0000, 0x2300000,
    0x17c0000, 0x2340000, 0x1a80000, 0x2180000, 0x1a80000)

DEFINE_G_DATA_020047AC(gData_020048e4, 0x2180000, 0x1a80000, 0x2180000,
    0x17c0000, 0x2300000, 0x17c0000, 0x2340000, 0x1a80000)

u32 gData_02004980[] = {
    39, 2, 4, 0xb00000, 0, 0x9a0000,
    1, 39, 1, 0, 10, 22,
    3, 0xb000, 0, 10, 17,
};

u32 gData_020049c4[] = {
    39, 2, 4, 0xa00000, 0, 0x9a0000,
    1, 39, 1, 0, 10, 22,
    3, 0xa000, 0, 10, 17,
};

u32 gData_02004a08[] = {
    39, 2, 4, 0x900000, 0, 0x9a0000,
    1, 39, 1, 0, 1, 3,
    0, 1, 17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(Prox1),
    SCENE_EXIT(1, WorldMap, 62),
    SCENE_EXIT(2, Prox2, 1),
    SCENE_EXIT(3, StaffCredit, 0),
    SCENE_EXITS(Prox2),
    SCENE_EXIT(1, Prox1, 2),
    SCENE_EXIT(2, Prox4, 8),
    SCENE_EXIT(3, Prox4, 1),
    SCENE_EXIT(4, Prox4, 10),
    SCENE_EXIT(5, Prox4, 4),
    SCENE_EXIT(6, Prox4, 5),
    SCENE_EXIT(7, Prox4, 2),
    SCENE_EXIT(8, Prox6, 7),
    SCENE_EXIT(9, Prox3, 2),
    SCENE_EXIT(10, Prox5, 9),
    SCENE_EXIT(11, Prox3, 1),
    SCENE_EXITS(Prox3),
    SCENE_EXIT(1, Prox2, 11),
    SCENE_EXIT(2, Prox2, 9),
    SCENE_EXIT(3, WorldMap, 63),
    SCENE_EXIT(4, Prox4, 7),
    SCENE_EXITS(Prox4),
    SCENE_EXIT(1, Prox2, 3),
    SCENE_EXIT(2, Prox2, 7),
    SCENE_EXIT(3, Prox5, 6),
    SCENE_EXIT(4, Prox2, 5),
    SCENE_EXIT(5, Prox2, 6),
    SCENE_EXIT(6, Prox5, 7),
    SCENE_EXIT(7, Prox3, 4),
    SCENE_EXIT(8, Prox2, 2),
    SCENE_EXIT(9, Prox5, 8),
    SCENE_EXIT(10, Prox2, 4),
    SCENE_EXITS(Prox5),
    SCENE_EXIT(6, Prox4, 3),
    SCENE_EXIT(7, Prox4, 6),
    SCENE_EXIT(8, Prox4, 9),
    SCENE_EXIT(9, Prox2, 10),
    SCENE_EXITS(Prox6),
    SCENE_EXIT(7, Prox2, 8),
    SCENE_EXITS_END,
};

u32 gData_02004b14[] = {
    0xffff, 0, 0, 0, 0, 0,
};

s16 gData_02004b2c[] = {
    6, -1, 1, 0, 0, 88, 0, 0,
    0, 248, 0xc000, 2, 7, -1, 1, 0,
    0, 104, 0, 0, 0, 248, 0xc000, 2,
    62, -1, 1, 0, 0, 120, 0, 0,
    0, 248, 0xd000, 2, 60, -1, 1, 0,
    0, 136, 0, 0, 0, 248, 0xd000, 2,
    61, -1, 1, 0, 0, 152, 0, 0,
    0, 248, 0xb000, 2, 3, -1, 1, 0,
    0, 168, 0, 0, 0, 248, 0xc000, 2,
    2, -1, 1, 0, 0, 184, 0, 0,
    0, 248, 0xc000, 2, 1, -1, 1, 0,
    0, 200, 0, 0, 0, 248, 0xc000, 2,
    5, -1, 1, 0, 0, 168, 0, 0,
    0, 216, 0xa000, 2, 56, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    0, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

u32 gProx_Room2Placements[] = {
    0xffff00af, 1, 0xf80000, 0, 0x1c80000, 0x33000,
    0xffff00ac, 1, 0xd80000, 0, 0x1500000, 0x15000,
    0xffff00ae, 3, 0x700000, 0, 0xf80000, 0,
    0xffff00b0, 1, 0x1c40000, 0, 0x12c0000, 0x15000,
    0xffff00ab, 1, 0x1140000, 0, 0x940000, 0x1b000,
    0xffff00ad, 1, 0x2440000, 0, 0x15c0000, 0x38000,
    0xffff00b2, 1, 0xf00000, 0, 0xf40000, 0x33000,
    0xffff00b3, 1, 0x1760000, 0, 0x1a40000, 0x15000,
    0xffff00b2, 1, 0, 0, 0, 0x20000,
    0xffff00b3, 1, 0, 0, 0, 0x2c000,
    0xffff00b2, 1, 0, 0, 0, 0x28000,
    0xffff0149, 1, 0x19c0000, 0, 0x1d80000, 0x24000,
    0xffff011e, 7, 0x2380000, 0, 0xb00000, 0x24000,
    0xffff, 0, 0, 0, 0, 0,
};

s16 gProx_Room3Placements[] = {
    175, -1, 1, 0, 0, 540, 0, 0,
    0, 388, 0xb000, 3, 172, -1, 1, 0,
    0, 568, 0, 0, 0, 388, 0xb000, 1,
    44, -1, 1, 0, 0, 180, 0, 0,
    0, 132, 0x5000, 3, 45, -1, 1, 0,
    0, 140, 0, 0, 0, 132, 0x3000, 3,
    155, -1, 1, 0, 0, 200, 0, 0,
    0, 178, 0xb000, 1, 155, -1, 1, 0,
    0, 184, 0, 0, 0, 178, 0xb000, 1,
    155, -1, 1, 0, 0, 168, 0, 0,
    0, 178, 0xd000, 1, 153, -1, 1, 0,
    0, 152, 0, 0, 0, 178, 0xd000, 1,
    153, -1, 1, 0, 0, 136, 0, 0,
    0, 178, 0xd000, 1, 153, -1, 1, 0,
    0, 120, 0, 0, 0, 178, 0xd000, 1,
    56, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 0, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    286, -1, 7, 0, 0, 436, 0, 0,
    0, 208, 0x4000, 2, 243, 64, 1, 0,
    0, 404, 0, 0, 0, 320, 0x4000, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

s32 gData_02004f04[] = {
    (s32)0xffff00af, 1, 0, 0, 0, 0x2b000,
    (s32)0xffff00ac, 1, 0, 0, 0, 0x2b000,
    (s32)0xffff002c, 1, 0, 0, 0, 0x25000,
    (s32)0xffff002d, 1, 0, 0, 0, 0x23000,
    (s32)0xffff009b, 1, 0xa00000, 0, 0x780000, 0x1b000,
    (s32)0xffff009b, 1, 0xb00000, 0, 0x780000, 0x1b000,
    (s32)0xffff009b, 3, 0xb00000, 0, 0xa20000, 0xd000,
    (s32)0xffff0099, 1, 0x800000, 0, 0xd80000, 0xd000,
    (s32)0xffff0099, 1, 0x920000, 0, 0x780000, 0x1d000,
    (s32)0xffff0099, 1, 0x780000, 0, 0xa00000, 0x33000,
    (s32)0xffff0038, 1, 0, 0, 0, 0x24000,
    -0x10000, 1, 0, 0, 0, 0x24000,
    (s32)0xffff011e, 7, 0x1b40000, 0, 0xd00000, 0x24000,
    0x4000f3, 1, 0x1940000, 0, 0x1400000, 0x4000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_0200506c[] = {
    0xffff00af, 1, 0x580000, 0, 0xc00000, 0x8000,
    0xffff00ac, 1, 0x800000, 0, 0x6a0000, 0x1b000,
    0xffff00b2, 1, 0x980000, 0, 0x700000, 0x5000,
    0xffff00b0, 3, 0x16c0000, 0, 0x7b0000, 0x13000,
    0xffff00ac, 1, 0x1d40000, 0, 0xa00000, 0x3000,
    0xffff00ab, 3, 0x2d80000, 0, 0xc80000, 0x18000,
    0xffff00ad, 1, 0x2ae0000, 0, 0x6f0000, 0x1d000,
    0xffff00b2, 1, 0x2240000, 0, 0x3300000, 0x8000,
    0xffff00b3, 1, 0x1d20000, 0, 0x32a0000, 0x1d000,
    0xffff00b2, 1, 0x1e00000, 0, 0x3200000, 0x15000,
    0xffff00ac, 1, 0x2120000, 0, 0x3200000, 0x35000,
    0xffff00ac, 3, 0xa40000, 0, 0x20c0000, 0x10000,
    0xffff00ab, 3, 0xde0000, 0, 0x1ea0000, 0x15000,
    0xffff00ad, 1, 0xa80000, 0, 0x18a0000, 0x1d000,
    0xffff00ae, 1, 0x10c0000, 0, 0x31a0000, 0x15000,
    0xffff00b0, 1, 0x880000, 0, 0x3300000, 0x13000,
    0x5700f4, 1, 0x2380000, 0, 0x1a80000, 0x8000,
    0xffff, 0, 0, 0, 0, 0,
};

s16 gData_0200521c[] = {
    175, -1, 3, 0, 0, 104, 0, 0,
    0, 196, 0x8000, 0, 172, -1, 1, 0,
    0, 128, 0, 0, 0, 106, 0xb000, 1,
    178, -1, 1, 0, 0, 152, 0, 0,
    0, 112, 0x5000, 0, 176, -1, 3, 0,
    0, 364, 0, 0, 0, 123, 0x3000, 1,
    172, -1, 1, 0, 0, 468, 0, 0,
    0, 160, 0x3000, 0, 171, -1, 3, 0,
    0, 728, 0, 0, 0, 200, 0x8000, 1,
    173, -1, 3, 0, 0, 666, 0, 0,
    0, 134, 0xd000, 1, 178, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x5000, 2,
    179, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0xd000, 2, 178, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x5000, 2,
    172, -1, 1, 0, 0, 530, 0, 0,
    0, 800, 0x5000, 3, 172, -1, 1, 0,
    0, 234, 0, 0, 0, 440, 0x5000, 0,
    171, -1, 3, 0, 0, 224, 0, 0,
    0, 554, 0x5000, 0, 173, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0xd000, 2,
    174, -1, 1, 0, 0, 268, 0, 0,
    0, 794, 0x5000, 1, 176, -1, 1, 0,
    0, 136, 0, 0, 0, 816, 0x3000, 1,
    244, 87, 1, 0, 0, 568, 0, 0,
    0, 424, 0x8000, 0, 44, -1, 1, 0,
    0, 192, 0, 0, 0, 428, 0x5000, 3,
    45, -1, 1, 0, 0, 176, 0, 0,
    0, 428, 0x3000, 1, 56, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    0, -1, 1, 0, 0, 0, 0, 0,
    0, 0, 0x4000, 2, 5, -1, 1, 0,
    0, 0, 0, 0, 0, 0, 0x4000, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

u32 gProx_Room5Placements[] = {
    0xffff00ae, 1, 0x2740000, 0, 0xc80000, 0x15000,
    0xffff00ae, 1, 0x1740000, 0, 0x3280000, 0x3b000,
    0xffff00b0, 1, 0x1720000, 0, 0x2e40000, 0x15000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_020054a4[] = {
    0xffff00ae, 3, 0x29a0000, 0, 0x7c0000, 0x15000,
    0xffff00ae, 1, 0x1740000, 0, 0x3280000, 0x3b000,
    0xffff00b0, 1, 0x1720000, 0, 0x2e40000, 0x15000,
    0xffff00ad, 1, 0x2980000, 0, 0x1c80000, 0x5000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gProx_Room6Placements[] = {
    0xffff00c9, 1, 0xb00000, 0, 0x3090000, 0x15000,
    0xffff, 0, 0, 0, 0, 0,
};

struct Trigger gProx_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 10, 0x9cc, EVENT(Prox_SayFarewell)},
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_02001a9c)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_02001ab8)},
    {0xce01, 4, CONDITION_ALWAYS, 4},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_02001ad4)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Func_02001af0)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Func_02001b0c)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Func_02001b28)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {0xc602, 10, CONDITION_ALWAYS, EVENT(Func_02001b44)},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_TALK, 8, 0xa3d, MSGNO(MsgProx01)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgProx81)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0xa3d, MSGNO(MsgProx09)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgProx89)},
    {TRIGGER_TALK, 9, 0xa3d, MSGNO(MsgProx02)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgProx82)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0xa3d, MSGNO(MsgProx10)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgProx90)},
    {TRIGGER_TALK, 10, 0xa3d, MSGNO(MsgProx03)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgProx83)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0xa3d, MSGNO(MsgProx11)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgProx91)},
    {TRIGGER_TALK, 11, 0xa3d, MSGNO(MsgProx04)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgProx84)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0xa3d, MSGNO(MsgProx12)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgProx92)},
    {TRIGGER_TALK, 12, 0xa3d, MSGNO(MsgProx05)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgProx85)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0xa3d, MSGNO(MsgProx13)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgProx93)},
    {TRIGGER_TALK, 13, 0xa3d, MSGNO(MsgProx06)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgProx86)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0xa3d, MSGNO(MsgProx14)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgProx94)},
    {TRIGGER_TALK, 14, 0xa3d, MSGNO(MsgProx07)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgProx87)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0xa3d, MSGNO(MsgProx15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgProx95)},
    {TRIGGER_TALK, 15, 0xa3d, MSGNO(MsgProx08)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgProx88)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0xa3d, MSGNO(MsgProx16)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgProx96)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(Prox_TalkToPlayingChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT(Prox_TalkToPlayingChild)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(Prox_TalkToRestlessChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, EVENT(Prox_TalkToRestlessChild)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, EVENT(Prox_TalkToTrainingChild)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT(Prox_TalkToTrainingChild)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 100, 0x300, EVENT(Func_02000588)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 19, CONDITION_ALWAYS, 0},
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_02001b60)},
    {TRIGGER_STEP, 9, 0xa2d, EVENT(Func_020015d0)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Leader_SlideOnIce)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_BEFORE_CAST), 255, CONDITION_ALWAYS, EVENT(Func_020017e4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_AFTER_CAST), 255, CONDITION_ALWAYS, EVENT(Func_02001800)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 8, CONDITION_ALWAYS, EVENT(Func_02001770)},
    {TRIGGER_RAISED, 200, 0xa3d, EVENT(Func_02000280)},
    {TRIGGER_STEP, 11, 0x9c0, EVENT(Prox_SearchForParents)},
    {TRIGGER_STEP, 12, 0x9c1, EVENT(Prox_DecideToGoToLighthouse)},
    {TRIGGER_STEP, 13, 0x9c2, EVENT(Prox_MeetPuelle)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgProx48)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgProx50)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgProx49)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgProx51)},
    {TRIGGER_TALK, 12, 0xa3d, MSGNO(MsgProx63)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgProx141)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0xa3d, MSGNO(MsgProx69)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgProx147)},
    {TRIGGER_TALK, 13, 0xa3d, MSGNO(MsgProx64)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgProx142)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0xa3d, MSGNO(MsgProx70)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgProx148)},
    {TRIGGER_TALK, 14, 0xa3d, MSGNO(MsgProx65)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgProx143)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0xa3d, MSGNO(MsgProx71)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgProx149)},
    {TRIGGER_TALK, 15, 0xa3d, MSGNO(MsgProx66)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgProx144)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0xa3d, MSGNO(MsgProx72)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgProx150)},
    {TRIGGER_TALK, 16, 0xa3d, MSGNO(MsgProx67)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgProx145)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0xa3d, MSGNO(MsgProx73)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgProx151)},
    {TRIGGER_TALK, 17, 0xa3d, MSGNO(MsgProx68)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgProx146)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0xa3d, MSGNO(MsgProx74)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgProx152)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgProx76)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgProx78)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgProx77)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgProx79)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02005a20[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_TALK, 8, 0xa3d, MSGNO(MsgProx17)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgProx97)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0xa3d, MSGNO(MsgProx20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgProx100)},
    {TRIGGER_TALK, 9, 0xa3d, MSGNO(MsgProx18)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgProx98)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0xa3d, MSGNO(MsgProx21)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgProx101)},
    {TRIGGER_TALK, 10, 0xa3d, MSGNO(MsgProx19)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgProx99)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0xa3d, MSGNO(MsgProx22)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgProx102)},
    {TRIGGER_TALK, 11, 0xa3d, MSGNO(MsgProx24)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgProx104)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0xa3d, MSGNO(MsgProx27)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgProx107)},
    {TRIGGER_TALK, 12, 0xa3d, MSGNO(MsgProx25)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgProx105)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0xa3d, MSGNO(MsgProx28)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgProx108)},
    {TRIGGER_TALK, 13, 0xa3d, MSGNO(MsgProx29)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgProx109)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0xa3d, MSGNO(MsgProx31)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgProx111)},
    {TRIGGER_TALK, 14, 0xa3d, MSGNO(MsgProx30)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgProx110)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0xa3d, MSGNO(MsgProx32)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgProx112)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(Prox_AskAboutIgloos)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgProx37)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgProx34)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgProx38)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgProx35)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgProx39)},
    {TRIGGER_TALK, 18, 0xa3d, MSGNO(MsgProx36)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgProx116)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0xa3d, MSGNO(MsgProx40)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgProx120)},
    {TRIGGER_TALK, 19, 0xa3d, MSGNO(MsgProx41)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgProx123)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, 0xa3d, MSGNO(MsgProx44)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgProx128)},
    {TRIGGER_TALK, 20, 0xa3d, MSGNO(MsgProx42)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgProx124)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0xa3d, MSGNO(MsgProx45)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgProx129)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgProx43)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgProx46)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgProx121)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgProx126)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgProx122)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgProx127)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Prox_TalkAboutParents)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, 0xa3d, MSGNO(MsgProx58)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgProx137)},
    {TRIGGER_TALK, 23, 0xa3d, MSGNO(MsgProx57)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgProx136)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, 0xa3d, MSGNO(MsgProx59)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgProx138)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, EVENT(Func_02000290)},
    {0x173, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProx154))},
    {0x173, 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProx155))},
    {0x173, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProx156))},
    {0x173, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProx157))},
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_Room5Triggers[] = {
    {0xca02, 6, CONDITION_ALWAYS, EVENT(Func_02001b7c)},
    {0xca02, 7, CONDITION_ALWAYS, EVENT(Func_02001b7c)},
    {0xca02, 8, CONDITION_ALWAYS, EVENT(Func_02001b7c)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_TALK, 8, 0xa3d, MSGNO(MsgProx23)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgProx103)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0xa3d, MSGNO(MsgProx26)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgProx106)},
    {TRIGGER_TALK, 9, 0xa3d, MSGNO(MsgProx52)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgProx131)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0xa3d, MSGNO(MsgProx54)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgProx133)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Prox_PraiseReturnedWarrior)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0xa3d, MSGNO(MsgProx55)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgProx134)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgProx125)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgProx130)},
    {0x173, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProx154))},
    {0x173, 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProx155))},
    {0x173, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProx156))},
    {0x173, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProx157))},
    {TRIGGER_TABLE_END},
};

struct Trigger gProx_Room6Triggers[] = {
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Prox_FearForWeyard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0xa3d, MSGNO(MsgProx61)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgProx140)},
    {TRIGGER_TABLE_END},
};

u32 gData_02005ee8[] = {
    39, 2, 4, 0x780000, 0, 0xc40000,
    1, 39, 1, 0, 10, 22,
    3, 0x4000, 0, 10, 17,
};

u32 gData_02005f2c[] = {
    39, 2, 4, 0x980000, 0, 0xc40000,
    1, 39, 1, 0, 10, 22,
    3, 0x5000, 0, 10, 17,
};

u32 gData_02005f70[] = {
    39, 2, 4, 0x880000, 0, 0x15c0000,
    1, 39, 1, 0, 1, 3,
    0, 1, 17,
};

u32 gData_02005fac[] = {
    39, 2, 4, 0x980000, 0, 0x15c0000,
    1, 39, 1, 0, 1, 3,
    0, 1, 17,
};

u32 gData_02005fe8[] = {
    39, 2, 4, 0x880000, 0, 0xf80000,
    1, 4, 0x880000, 0, 0x15c0000, 1,
    39, 1, 0, 1, 3, 0,
    1, 17,
};

u32 gData_02006038[] = {
    39, 2, 4, 0x980000, 0, 0xf80000,
    1, 4, 0x980000, 0, 0x15c0000, 1,
    39, 1, 0, 1, 3, 0,
    1, 17,
};

u32 gData_02006088[] = {
    39, 2, 4, 0x980000, 0, 0xf80000,
    1, 4, 0x980000, 0, 0x1100000, 1,
    39, 1, 0, 1, 17,
};

u32 gData_020060cc[] = {
    39, 2, 4, 0xa80000, 0, 0x1100000,
    1, 39, 1, 0, 1, 17,
};

u32 gData_020060fc[] = {
    39, 2, 4, 0x780000, 0, 0xdc0000,
    1, 39, 1, 0, 10, 22,
    3, 0x2000, 0, 5, 22, 3,
    0, 0, 20, 22, 3, 0xe000,
    0, 5, 22, 3, 0xc000, 0,
    1, 17,
};

u32 gData_0200617c[] = {
    39, 2, 4, 0x8c0000, 0, 0xdc0000,
    1, 39, 1, 0, 10, 22,
    3, 0x6000, 0, 5, 22, 3,
    0x8000, 0, 20, 22, 3, 0xa000,
    0, 5, 22, 3, 0xc000, 0,
    1, 17,
};

u32 gData_020061fc[] = {
    39, 2, 4, 0x980000, 0, 0xf00000,
    1, 39, 1, 0, 10, 17,
};

u32 gData_0200622c[] = {
    39, 2, 4, 0xa80000, 0, 0x15c0000,
    1, 39, 1, 0, 1, 3,
    0, 1, 17,
};

u32 gData_02006268[] = {
    39, 2, 4, 0x980000, 0, 0x15c0000,
    1, 39, 1, 0, 1, 3,
    0, 1, 17,
};
// clang-format on
#undef FIELD_AT

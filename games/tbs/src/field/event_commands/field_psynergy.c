/// @file
/// Running a field Psynergy by its kind, and helpers its kinds share.
// Part of event_commands.c.

#include "../event_commands_internal.h"

// The dispatchers use the shared field effect. Individual update routines use
// their actor or FieldParticle contract; neither is an
// event.

/// Runs one step of the current field Psynergy: an enum FieldPsynergyHandler.
void FieldPsynergy_RunHandler(s32 kind) {
  if (kind == FIELD_PSYNERGY_HANDLER_UNTARGETED)
    FieldPsynergy_Run();
  else if (kind == FIELD_PSYNERGY_HANDLER_ON_TARGET)
    FieldPsynergy_Dispatch();
  else if (kind == FIELD_PSYNERGY_HANDLER_FINISH)
    FieldPsynergy_FinishMindRead();
  else if (kind == FIELD_PSYNERGY_HANDLER_EXTRA)
    FieldPsynergy_RunExtraHandler();
}

/// Casts the current field Psynergy with no target: runs the body of its kind
/// (effectWork->fieldPsynergy).
void FieldPsynergy_Run(void) {
  struct FieldEffectWork *effectWork;
  struct FieldWork *runtime;
  s32 kind;
  s32 targetId;
  s32 nearestTargetId;

  effectWork = gFieldEffectWork;
  runtime = gFieldWork;
  kind = effectWork->fieldPsynergy;
  targetId = effectWork->secondActorId;

  switch (kind) {
  case FIELD_PSYNERGY_MOVE:
    FieldPsynergy_Move();
    return;
  case FIELD_PSYNERGY_LIFT:
    FieldPsynergy_Lift();
    return;
  case FIELD_PSYNERGY_CARRY:
    FieldPsynergy_Carry();
    return;
  case FIELD_PSYNERGY_FORCE:
    FieldPsynergy_Force();
    return;
  case FIELD_PSYNERGY_DOUSE:
    FieldPsynergy_Douse();
    return;
  case FIELD_PSYNERGY_WHIRLWIND:
    FieldPsynergy_Whirlwind();
    return;
  case FIELD_PSYNERGY_FROST:
    FieldPsynergy_Frost();
    return;
  case FIELD_PSYNERGY_PLY:
    FieldPsynergy_Ply();
    return;
  case FIELD_PSYNERGY_GROWTH:
    FieldPsynergy_Growth();
    return;
  case FIELD_PSYNERGY_CATCH:
    FieldPsynergy_Catch();
    return;
  case FIELD_PSYNERGY_HALT:
    // Resume the actor stopped before, then stop the nearest one.
    if (gMainRoutineWork.haltedActor != -1) {
      FieldPsynergy_ResumeHaltedActor(gMainRoutineWork.haltedActor);
      gMainRoutineWork.haltedActor = -1;
    }

    nearestTargetId =
        Field_FindNearbyPsynergyTarget(gMainRoutineWork.leaderActorId, kind);
    nearestTargetId = FieldPsynergy_FilterHaltTarget(nearestTargetId);
    if (Scene_FindActorTalkTrigger(nearestTargetId) != 0) {
      FieldPsynergy_SetupActorPair(gMainRoutineWork.leaderActorId,
                                   nearestTargetId);
      FieldPsynergy_HaltOnTarget(nearestTargetId);
      FieldPsynergy_HaltActor(nearestTargetId);
      gMainRoutineWork.haltedActor = nearestTargetId;
    } else {
      FieldPsynergy_Halt();
    }
    return;
  case FIELD_PSYNERGY_MIND_READ:
    // The wave and colour effect, stopping a running Reveal first.
    if (runtime->revealActive != 0)
      FieldPsynergy_EndReveal();
    FieldPsynergy_StartMindReadWave(effectWork->firstActorId, targetId);
    return;
  case FIELD_PSYNERGY_REVEAL:
    FieldPsynergy_Reveal();
    return;
  case FIELD_PSYNERGY_CLOAK:
    FieldPsynergy_StartCloak();
    return;
  case FIELD_PSYNERGY_RETREAT:
    FieldPsynergy_Retreat();
    return;
  case FIELD_PSYNERGY_AVOID:
    FieldPsynergy_Avoid();
    return;
  }
}

/// Casts the current field Psynergy on its target (secondActorId): runs the
/// handler of its kind.
void FieldPsynergy_Dispatch(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct FieldWork *runtime = gFieldWork;
  s32 kind = effectWork->fieldPsynergy;
  s32 targetId = effectWork->secondActorId;

  effectWork->enabled = 0;
  switch (kind) {
  case FIELD_PSYNERGY_MIND_READ:
    if (runtime->revealActive != 0)
      FieldPsynergy_EndReveal();
    if (gMainRoutineWork.haltedActor != effectWork->secondActorId)
      ((struct Actor *)effectWork->child)->paused = 1;
    FieldPsynergy_StartMindReadWave(effectWork->firstActorId, targetId);
    break;
  case FIELD_PSYNERGY_MOVE:
    // Push the target with the direction keys.
    FieldPsynergy_SteerPushedTarget(targetId);
    break;
  case FIELD_PSYNERGY_LIFT:
    FieldPsynergy_LiftOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_CARRY:
    FieldPsynergy_SteerLiftedTarget(targetId);
    break;
  case FIELD_PSYNERGY_FORCE:
    FieldPsynergy_ForceOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_DOUSE:
    FieldPsynergy_DouseOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_FROST:
    FieldPsynergy_FrostOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_GROWTH:
    FieldPsynergy_GrowthOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_HALT:
    if (gMainRoutineWork.haltedActor != -1) {
      FieldPsynergy_ResumeHaltedActor(gMainRoutineWork.haltedActor);
      gMainRoutineWork.haltedActor = -1;
    }
    FieldPsynergy_HaltActor(targetId);
    gMainRoutineWork.haltedActor = targetId;
    FieldPsynergy_HaltOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_PLY:
    FieldPsynergy_PlyOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_WHIRLWIND:
    FieldPsynergy_WhirlwindOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_CATCH:
    FieldPsynergy_CatchOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_REVEAL:
    FieldPsynergy_Reveal();
    break;
  case FIELD_PSYNERGY_CLOAK:
    FieldPsynergy_StartCloak();
    break;
  case FIELD_PSYNERGY_RETREAT:
    FieldPsynergy_Retreat();
    break;
  case FIELD_PSYNERGY_AVOID:
    FieldPsynergy_Avoid();
    break;
  }
}

/// Finishes the current field Psynergy when it is Mind Read: ends it and lets
/// its target move again, unless the target is the actor kept stopped.
void FieldPsynergy_FinishMindRead(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;

  if (effectWork->fieldPsynergy == FIELD_PSYNERGY_MIND_READ) {
    FieldPsynergy_StopMindReadWave();
    if (gMainRoutineWork.haltedActor != effectWork->secondActorId) {
      ((struct Actor *)effectWork->child)->paused = 0;
    }
  }
}

/// The extra handler of Reveal, Cloak and Avoid: what each does when it ends.
void FieldPsynergy_RunExtraHandler(void) {
  u32 kind;

  kind = ((struct FieldEffectWork *)gFieldEffectWork)->fieldPsynergy;
  switch (kind) {
  case FIELD_PSYNERGY_REVEAL:
    FieldPsynergy_EndReveal();
    return;
  case FIELD_PSYNERGY_CLOAK:
    FieldPsynergy_EndCloak();
    return;
  case FIELD_PSYNERGY_AVOID:
    FieldPsynergy_ShowAvoidWoreOff();
    return;
  }
}

/// Runs the event tied to a field Psynergy: a result below
/// TRIGGER_RESULT_MESSAGE_END is a message, shown at target; a larger one is a
/// routine, called with param and target.
s32 Field_RunPsynergyEvent(const void *event, s32 param, s32 target) {
  s32 script;

  if (event != NULL) {
    // Every routine's address is above TRIGGER_RESULT_MESSAGE_END, so one word
    // holds either.
    script = ((const struct Trigger *)event)->value;
    if (script != 0) {
      if (script < TRIGGER_RESULT_MESSAGE_END) {
        Event_Start();
        Event_SetMessageId(((const struct Trigger *)event)->value);
        Event_ShowMessageAndWait(target, 0);
        Event_End();
      } else {
        ((void (*)(s32, s32))script)(param, target);
      }
    }
    if (GameFlag_GetFar(FLAG_NOTHING_HAPPENS) != 0) {
      Event_Start();
      FieldMessage_ShowFar(MsgNothingHappens, FIELD_MESSAGE_ZOOM);
      Event_End();
    }
  }
  return 0;
}

/// Per-frame update: sets every part of an actor's sprite to param frame % 6,
/// which cycles its colours.
void Actor_CycleSpriteColors(struct Actor *actor) {
  struct Sprite *sprite;
  s32 count;
  s32 i;

  if (actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
    sprite = actor->sprite;
    if (sprite != 0 && sprite->retainCacheEntry == 0) {
      count = sprite->count;
      for (i = 0; i < count; i++)
        sprite->entries[i]->colorMode = gFrameCount % 6;
      sprite->dirty = 1;
    }
  }
}

#include "../../scene/shared/actor_polar_target.h"

/// Carry, Lift, Douse, Move and the Djinni's hop into battle throw things this
/// way.
DEFINE_SET_TARGET_POSITION_FROM_MAGNITUDE_ANGLE(, Actor_SetMoveTargetFar)

// From here on actors are passed in each function's own view of them.

/// Counts the free blocks (owner 0xff) of the sprite VRAM cache's 512 blocks.
s32 VramCache_CountFreeBlocks(void) {
  u8 *marker = gVramCacheBlockOwners;
  s32 freeCount = 0;
  s32 remaining = VRAM_BLOCK_COUNT;

  do {
    if (*marker++ == VRAM_BLOCK_OWNER_FREE) {
      freeCount++;
    }
    remaining--;
  } while (remaining != 0);
  return freeCount;
}

/// Makes sourceSprite share sharedSprite's VRAM block, releasing its own; with
/// no sharedSprite it keeps its block.
struct Sprite *Sprite_ShareVramCacheEntry(struct Sprite *sourceSprite,
                                          struct Sprite *sharedSprite) {
  struct Sprite *shared;
  struct Sprite *sprite;

  sprite = sourceSprite;
  shared = NULL;
  if (sprite != NULL) {
    if (sharedSprite == NULL) {
      sprite->retainCacheEntry = 1;
    } else {
      VramCache_FreeEntry(sprite->slot);
      sprite->slot = sharedSprite->slot;
      sprite->retainCacheEntry = 1;
      sprite = sharedSprite;
    }
    shared = sprite;
  }
  return shared;
}

/// Creates an actor for a field Psynergy, at the caster's ground height, with
/// sprite priority 0 and animation 1.
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct Actor *actor;

  actor = Actor_CreateFar(kind, x, y, z);
  if (actor != NULL) {
    if (actor->spriteStorageKind == ACTOR_SPRITE_NONE) {
      Actor_ReleaseFar(actor);
      return NULL;
    }
    actor->groundHeight = ((struct Actor *)effectWork->mainActor)->groundHeight;
    actor->motionFlags = ACTOR_FLOATS;
    actor->priorityFlags = ACTOR_PRIORITY_OVERHEAD;
    actor->sprite->image.priority = 0;
    Actor_SetSpriteFlagsFar(actor, 0);
    Actor_SelectAnimationFar(actor, 1);
  }
  return actor;
}

/// Sets the movement state and animation timer steps of every actor but the
/// player's and except.
void Actor_SetOthersPaused(struct Actor *except, s32 state, s32 value) {
  s16 *player;
  struct Actor *actor;
  s32 actorId;

  actorId = 0;
  player = (s16 *)gPlayerActorId;
  do {
    actor = Actor_Lookup(actorId);
    if (actorId != *player && actor != NULL && actor != except) {
      actor->paused = state;
      Actor_SetAnimationSpeedFar(actor, value);
    }
    actorId++;
    // BUG: one past the actor table: Actor_Lookup(ACTOR_ID_COUNT) is the first
    // treasure spot's actor, so the map's first chest or Psynergy Stone is
    // paused too.
#ifdef BUGFIX
  } while (actorId < ACTOR_ID_COUNT);
#else
  } while (actorId <= ACTOR_ID_COUNT);
#endif
}

/// @file
/// Running a field Psynergy by its kind, and helpers its effects share.
// Part of event_commands.c.

#include "../event_commands_internal.h"

// The dispatchers use the common field effect. Individual update hooks use
// their actor or FieldParticle contract; neither is a map-script
// callback.

/// Runs one step of the current Psynergy effect: an enum FieldPsynergyHandler.
/// Exported as FieldPsynergy_RunHandlerFar; Goma Cave's entrance imports it
/// and passes FIELD_PSYNERGY_HANDLER_ON_TARGET to play Whirlwind on the stump's
/// ivy from its own script.
void FieldPsynergy_RunHandler(s32 kind) {
  if (kind == FIELD_PSYNERGY_HANDLER_UNTARGETED)
    FieldPsynergy_Run();
  else if (kind == FIELD_PSYNERGY_HANDLER_ON_TARGET)
    FieldPsynergy_Dispatch();
  else if (kind == FIELD_PSYNERGY_HANDLER_FINISH)
    FieldPsynergy_FinishMindRead();
  else if (kind == FIELD_PSYNERGY_HANDLER_EXTRA)
    Event_RunFieldPsynergy();
}

void _call_via_r3(s32, s32);

/// Casts the current Psynergy effect with no target: runs the body of its
/// kind (effectWork->fieldPsynergy).
///
/// Called by Field_UsePsynergy when no trigger names a target
/// (FLAG_PSYNERGY_TARGET clear), by Field_UseItem for an item with a field
/// action, and by FieldPsynergy_RunHandler(FIELD_PSYNERGY_HANDLER_UNTARGETED).
/// Halt also moves gSaveBlock.haltedActor to the actor it stops.
void FieldPsynergy_Run(void) {
  struct FieldEffectWork *effectWork;
  struct FieldWork *runtime;
  s32 kind;
  s32 targetId;
  s32 nearestTargetId;

  effectWork = *(gHeapSlots + HEAP_SLOT_FIELD_EFFECT);
  runtime = *(gHeapSlots + HEAP_SLOT_FIELD);
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
    // Free the object stopped before, then stop the nearest one.
    if (gSaveBlock.haltedActor != -1) {
      FieldPsynergy_ResumeHaltedActor(gSaveBlock.haltedActor);
      gSaveBlock.haltedActor = -1;
    }

    nearestTargetId =
        Field_FindNearbyPsynergyTarget(gSaveBlock.leaderActorId, kind);
    nearestTargetId = FieldEffect_FilterActorIdByFlags(nearestTargetId);
    if (Scene_FindActorTalkTrigger(nearestTargetId) != 0) {
      FieldPsynergy_SetupActorPair(gSaveBlock.leaderActorId, nearestTargetId);
      FieldPsynergy_HaltOnTarget(nearestTargetId);
      FieldPsynergy_HaltActor(nearestTargetId);
      gSaveBlock.haltedActor = nearestTargetId;
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

/// Casts the current Psynergy effect on its target (secondObjectId): runs the
/// handler of its kind.
///
/// Called by Field_UsePsynergy when a trigger names the target
/// (FLAG_PSYNERGY_TARGET still set after the before event), and by
/// FieldPsynergy_RunHandler(FIELD_PSYNERGY_HANDLER_ON_TARGET). Clears
/// effectWork->enabled first: that is how the runners FieldPsynergy_Run shares
/// (Force, Frost, Ply, Whirlwind, Catch) tell that a trigger decides what
/// happens to the target. Halt also moves gSaveBlock.haltedActor to the
/// target.
void FieldPsynergy_Dispatch(void) {
  struct FieldEffectWork *effectWork = *(gHeapSlots + HEAP_SLOT_FIELD_EFFECT);
  struct FieldWork *runtime = *(gHeapSlots + HEAP_SLOT_FIELD);
  s32 kind = effectWork->fieldPsynergy;
  s32 targetId = effectWork->secondActorId;

  effectWork->enabled = 0;
  switch (kind) {
  case FIELD_PSYNERGY_MIND_READ:
    if (runtime->revealActive != 0)
      FieldPsynergy_EndReveal();
    if (gSaveBlock.haltedActor != effectWork->secondActorId)
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
    if (gSaveBlock.haltedActor != -1) {
      FieldPsynergy_ResumeHaltedActor(gSaveBlock.haltedActor);
      gSaveBlock.haltedActor = -1;
    }
    FieldPsynergy_HaltActor(targetId);
    gSaveBlock.haltedActor = targetId;
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

/// Finishes the current effect when it is Mind Read: ends the effect and lets
/// its target move again, unless the target is the object kept stopped.
/// Called by Field_UsePsynergy after the after event, and by
/// FieldPsynergy_RunHandler(FIELD_PSYNERGY_HANDLER_FINISH): Mind Read is the
/// one kind whose effect stays up through the after event, the conversation it
/// reads.
void FieldPsynergy_FinishMindRead(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;

  if (effectWork->fieldPsynergy == FIELD_PSYNERGY_MIND_READ) {
    FieldPsynergy_FinishWaveAndReleaseHeapBlock();
    if (gSaveBlock.haltedActor != effectWork->secondActorId) {
      ((struct Actor *)effectWork->child)->paused = 0;
    }
  }
}

/// The extra handler of Reveal, Cloak and Avoid: what each does when it ends.
/// Called by Field_RunPsynergyEndEvents, which ends a Psynergy that runs out by
/// itself, and by FieldPsynergy_RunHandler(FIELD_PSYNERGY_HANDLER_EXTRA).
void Event_RunFieldPsynergy(void) {
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
    Event_ShowStatusMessage();
    return;
  }
}

/// Runs the event tied to an effect: a result below 0x10000 is a message,
/// shown at target; a larger one is a routine, called with param and target
/// (through r3). Afterwards, with FLAG_NOTHING_HAPPENS set, says "But nothing
/// happens.".
/// Returns 0.
///
/// Called by Field_UsePsynergy and Field_RunPsynergyEndEvents for the before
/// and after events, and by Force, Frost and Reveal for the events of their own
/// phases.
s32 Field_RunPsynergyEvent(const void *event, s32 param, s32 target) {
  s32 script;

  if (event != NULL) {
    // Every routine's address is above 0x10000, so one word holds either.
    script = ((const struct TriggerView *)event)->result;
    if (script != 0) {
      if (script < 0x10000) {
        Event_Start();
        Event_SetMessageId(((const struct TriggerView *)event)->result);
        Event_ShowMessageAndWait(target, 0);
        Event_End();
      } else {
        _call_via_r3(param, target);
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

/// Per-frame update: sets every part of an object's sprite to param
/// frame % 6, which cycles its colours. Move, Lift and Carry give it to the
/// target they hold, Retreat to its caster. A sprite that keeps its VRAM
/// cache entry (retainCacheEntry) is left alone.
void Actor_CycleSpriteColors(struct Actor *actor) {
  struct Actor *owner = (struct Actor *)actor;
  s32 kind;
  struct Sprite *animation;
  s32 initialCount;
  s32 remainingParts;
  struct SpriteAnim **parts;
  struct SpriteAnim *spritePart;
  /* FAKEMATCH(temporary): a pointer local to gFrameCount, volatile as it is
   * declared, read on each pass; as gFrameCount read in the loop, 6 of 46
   * instructions differ in tbs-en. */
  volatile u32 *frameCount;
  s32 param;

  kind = owner->spriteStorageKind;
  if (kind == ACTOR_SPRITE_ONE) {
    animation = owner->sprite;
    if (animation != 0 && animation->retainCacheEntry == 0) {
      initialCount = animation->count;
      if (initialCount != 0) {
        frameCount = &gFrameCount;
        parts = animation->entries;
        remainingParts = initialCount;
        do {
          spritePart = *parts++;
          param = *frameCount % 6;
          remainingParts--;
          spritePart->colorMode = param;
        } while (remainingParts != 0);
      }
      animation->dirty = 1;
    }
  }
}

/// Sends an object toward the point magnitude (16.16 pixels) away from it in
/// direction angle: sets its move target. Carry, Lift, Douse, Move and the
/// Djinni's hop into battle throw things this way.
void Actor_SetTargetPositionFromMagnitudeAngle(struct Actor *actor,
                                               s32 magnitude, s32 angle) {
  s32 pos[3];

  if (actor != 0) {
    SET_VECTOR(pos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(magnitude, angle, pos);
    Actor_SetMoveTargetFar(actor, pos[0], pos[1], pos[2]);
  }
}

// From here on objects are passed in each function's own view of them.

/// Counts the free blocks (owner 0xff) of the sprite VRAM cache's 512 blocks.
/// FieldPsynergy_LoadResources subtracts this from VRAM_BLOCK_COUNT
/// to record the blocks already occupied before the effect starts.
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

/// Makes sourceSprite share sharedSprite's VRAM block, releasing its own;
/// with no sharedSprite it keeps its block. Retains the shared cache entry and
/// returns the sprite whose block is shared from now on (sharedSprite, or
/// sourceSprite when there was none), or NULL for no sourceSprite. Chains of
/// afterimages pass one block along this way. Each sprite retains the entry
/// so releasing an afterimage cannot free graphics still used by the others.
/// Sprite teardown leaves the shared entry for the caller to release.
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

/// Creates an object for an effect, at the caster's ground height, with
/// sprite priority 0 and animation 1. An object without animation is released
/// and NULL returned. The effects spawn their orbs, hands and particles
/// through it, and check for NULL: the object table may be full. The object
/// floats (motion flags 4), is depth-sorted as if 320 pixels lower (priority
/// flag bit 2) and has no shadow; its creator releases it, or its script
/// frees it.
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
    actor->motionFlags = 4;
    actor->priorityFlags = 4;
    actor->sprite->image.priority = 0;
    Actor_SetSpriteFlagsFar(actor, 0);
    Actor_SelectAnimationFar(actor, 1);
  }
  return actor;
}

/// Sets the movement state and animation timer steps of every object but the
/// player's and except. FieldPsynergy_StartCast pauses them
/// (animation speed 0) while an effect plays and lets them go again at
/// normal speed.
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
  } while (actorId <= ACTOR_LAST);
}

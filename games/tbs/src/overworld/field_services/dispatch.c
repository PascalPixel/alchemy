/// @file
/// Running a field Psynergy by its kind, and helpers its effects share.
// Part of field_services.c.

#include "../field_services_internal.h"

// The dispatchers use the common field effect. Individual update hooks use
// their object event or FieldEffectSprite contract; neither is a map-script
// callback.

/// Runs one step of the current Psynergy effect: an enum PsynergyEffectStep.
/// Exported as Field_DispatchTypeHandlerFar; Goma Cave's entrance imports it
/// and passes PSYNERGY_STEP_ON_TARGET to play Whirlwind on the stump's ivy from
/// its own script.
void Field_DispatchTypeHandler(s32 kind) {
  if (kind == PSYNERGY_STEP_UNTARGETED)
    FieldEffect_Run();
  else if (kind == PSYNERGY_STEP_ON_TARGET)
    FieldEffect_DispatchRequestKind();
  else if (kind == PSYNERGY_STEP_FINISH)
    FieldEffect_FinishMindRead();
  else if (kind == PSYNERGY_STEP_EXTRA)
    FieldScript_RunTypeHandler();
}

void _call_via_r3(s32, s32);

/// Casts the current Psynergy effect with no target: runs the body of its
/// kind (effectWork->fieldPsynergy).
///
/// Called by Field_UsePsynergy when no map event names a target
/// (FLAG_PSYNERGY_TARGET clear), by Field_UseItem for an item with a field
/// action, and by Field_DispatchTypeHandler(PSYNERGY_STEP_UNTARGETED). Halt
/// also moves gSaveBlock.pausedObject to the object event it stops.
void FieldEffect_Run(void) {
  struct FieldEffectWork *effectWork;
  struct FieldWork *runtime;
  s32 kind;
  s32 targetId;
  s32 nearestTargetId;

  effectWork = *(gHeapSlots + HEAP_SLOT_FIELD_EFFECT);
  runtime = *(gHeapSlots + HEAP_SLOT_FIELD);
  kind = effectWork->fieldPsynergy;
  targetId = effectWork->secondObjectId;

  switch (kind) {
  case FIELD_PSYNERGY_MOVE:
    FieldEffect_RunMove();
    return;
  case FIELD_PSYNERGY_LIFT:
    FieldEffect_RunLift();
    return;
  case FIELD_PSYNERGY_CARRY:
    FieldEffect_RunCarry();
    return;
  case FIELD_PSYNERGY_FORCE:
    FieldEffect_RunForce();
    return;
  case FIELD_PSYNERGY_DOUSE:
    FieldEffect_RunDouse();
    return;
  case FIELD_PSYNERGY_WHIRLWIND:
    FieldEffect_RunWhirlwind();
    return;
  case FIELD_PSYNERGY_FROST:
    FieldEffect_RunFrost();
    return;
  case FIELD_PSYNERGY_PLY:
    FieldEffect_RunPly();
    return;
  case FIELD_PSYNERGY_GROWTH:
    FieldEffect_RunGrowth();
    return;
  case FIELD_PSYNERGY_CATCH:
    FieldEffect_RunCatch();
    return;
  case FIELD_PSYNERGY_HALT:
    // Free the object stopped before, then stop the nearest one.
    if (gSaveBlock.pausedObject != -1) {
      FieldEffect_ResumeObject(gSaveBlock.pausedObject);
      gSaveBlock.pausedObject = -1;
    }

    nearestTargetId =
        Field_FindNearbyPsynergyTarget(gSaveBlock.leaderLocalId, kind);
    nearestTargetId = FieldEffect_FilterObjectIdByFlags(nearestTargetId);
    if (Map_FindObjectEventTalkEvent(nearestTargetId) != 0) {
      FieldEffect_SetupObjectPair(gSaveBlock.leaderLocalId, nearestTargetId);
      FieldEffect_RunHaltOnTarget(nearestTargetId);
      FieldEffect_PauseObject(nearestTargetId);
      gSaveBlock.pausedObject = nearestTargetId;
    } else {
      FieldEffect_RunHalt();
    }
    return;
  case FIELD_PSYNERGY_MIND_READ:
    // The wave and colour effect, stopping a running Reveal first.
    if (runtime->revealActive != 0)
      FieldEffect_EndReveal();
    FieldEffect_StartMindReadWave(effectWork->firstObjectId, targetId);
    return;
  case FIELD_PSYNERGY_REVEAL:
    FieldEffect_RunReveal();
    return;
  case FIELD_PSYNERGY_CLOAK:
    FieldEffect_StartCloak();
    return;
  case FIELD_PSYNERGY_RETREAT:
    FieldEffect_RunRetreat();
    return;
  case FIELD_PSYNERGY_AVOID:
    FieldEffect_RunAvoid();
    return;
  }
}

/// Casts the current Psynergy effect on its target (secondObjectId): runs the
/// handler of its kind.
///
/// Called by Field_UsePsynergy when a map event names the target
/// (FLAG_PSYNERGY_TARGET still set after the before event), and by
/// Field_DispatchTypeHandler(PSYNERGY_STEP_ON_TARGET). Clears
/// effectWork->enabled first: that is how the runners FieldEffect_Run shares
/// (Force, Frost, Ply, Whirlwind, Catch) tell that a map event decides what
/// happens to the target. Halt also moves gSaveBlock.pausedObject to the
/// target.
void FieldEffect_DispatchRequestKind(void) {
  struct FieldEffectWork *effectWork = *(gHeapSlots + HEAP_SLOT_FIELD_EFFECT);
  struct FieldWork *runtime = *(gHeapSlots + HEAP_SLOT_FIELD);
  s32 kind = effectWork->fieldPsynergy;
  s32 targetId = effectWork->secondObjectId;

  effectWork->enabled = 0;
  switch (kind) {
  case FIELD_PSYNERGY_MIND_READ:
    if (runtime->revealActive != 0)
      FieldEffect_EndReveal();
    if (gSaveBlock.pausedObject != effectWork->secondObjectId)
      ((struct Object *)effectWork->child)->paused = 1;
    FieldEffect_StartMindReadWave(effectWork->firstObjectId, targetId);
    break;
  case FIELD_PSYNERGY_MOVE:
    // Push the target with the direction keys.
    FieldEffect_SteerPushedTarget(targetId);
    break;
  case FIELD_PSYNERGY_LIFT:
    FieldEffect_RunLiftOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_CARRY:
    FieldEffect_SteerLiftedTarget(targetId);
    break;
  case FIELD_PSYNERGY_FORCE:
    FieldEffect_RunForceOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_DOUSE:
    FieldEffect_RunDouseOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_FROST:
    FieldEffect_RunFrostOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_GROWTH:
    FieldEffect_RunGrowthOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_HALT:
    if (gSaveBlock.pausedObject != -1) {
      FieldEffect_ResumeObject(gSaveBlock.pausedObject);
      gSaveBlock.pausedObject = -1;
    }
    FieldEffect_PauseObject(targetId);
    gSaveBlock.pausedObject = targetId;
    FieldEffect_RunHaltOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_PLY:
    FieldEffect_RunPlyOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_WHIRLWIND:
    FieldEffect_RunWhirlwindOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_CATCH:
    FieldEffect_RunCatchOnTarget(targetId);
    break;
  case FIELD_PSYNERGY_REVEAL:
    FieldEffect_RunReveal();
    break;
  case FIELD_PSYNERGY_CLOAK:
    FieldEffect_StartCloak();
    break;
  case FIELD_PSYNERGY_RETREAT:
    FieldEffect_RunRetreat();
    break;
  case FIELD_PSYNERGY_AVOID:
    FieldEffect_RunAvoid();
    break;
  }
}

/// Finishes the current effect when it is Mind Read: ends the effect and lets
/// its target move again, unless the target is the object kept stopped.
/// Called by Field_UsePsynergy after the after event, and by
/// Field_DispatchTypeHandler(PSYNERGY_STEP_FINISH): Mind Read is the one kind
/// whose effect stays up through the after event, the conversation it reads.
void FieldEffect_FinishMindRead(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;

  if (effectWork->fieldPsynergy == FIELD_PSYNERGY_MIND_READ) {
    FieldEffect_FinishWaveAndReleaseHeapBlock();
    if (gSaveBlock.pausedObject != effectWork->secondObjectId) {
      ((struct Object *)effectWork->child)->paused = 0;
    }
  }
}

/// The extra handler of Reveal, Cloak and Avoid: what each does when it ends.
/// Called by Field_RunPsynergyEvents, which ends a Psynergy that runs out by
/// itself, and by Field_DispatchTypeHandler(PSYNERGY_STEP_EXTRA).
void FieldScript_RunTypeHandler(void) {
  u32 kind;

  kind = ((struct FieldEffectWork *)gFieldEffectWork)->fieldPsynergy;
  switch (kind) {
  case FIELD_PSYNERGY_REVEAL:
    FieldEffect_EndReveal();
    return;
  case FIELD_PSYNERGY_CLOAK:
    FieldEffect_EndCloak();
    return;
  case FIELD_PSYNERGY_AVOID:
    FieldScript_ShowStatusMessage();
    return;
  }
}

/// Runs the event tied to an effect: a result below 0x10000 is a message,
/// shown at target; a larger one is a routine, called with param and target
/// (through r3). Afterwards, with FLAG_NOTHING_HAPPENS set, says "But nothing
/// happens.".
/// Returns 0.
///
/// Called by Field_UsePsynergy and Field_RunPsynergyEvents for the before and
/// after events, and by Force, Frost and Reveal for the events of their own
/// phases.
s32 Field_RunEventAction(const void *event, s32 param, s32 target) {
  s32 script;

  if (event != NULL) {
    // Every routine's address is above 0x10000, so one word holds either.
    script = ((const struct MapEventView *)event)->result;
    if (script != 0) {
      if (script < 0x10000) {
        FieldScript_Start();
        FieldScript_SetMessageId(((const struct MapEventView *)event)->result);
        FieldScript_ShowMessageAndWait(target, 0);
        FieldScript_End();
      } else {
        _call_via_r3(param, target);
      }
    }
    if (GameFlag_GetFar(FLAG_NOTHING_HAPPENS) != 0) {
      FieldScript_Start();
      FieldMessage_ShowFar(MsgNothingHappens, FIELD_MESSAGE_ZOOM);
      FieldScript_End();
    }
  }
  return 0;
}

/// Per-frame update: sets every part of an object's sprite to param
/// frame % 6, which cycles its colours. Move, Lift and Carry give it to the
/// target they hold, Retreat to its caster. A sprite that keeps its VRAM
/// cache entry (retainCacheEntry) is left alone.
void Object_CycleSpriteColors(union ObjectEventOrEffect *fieldObject) {
  struct Object *owner = (struct Object *)fieldObject;
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
  if (kind == OBJECT_SPRITE_ONE) {
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
void ObjectMotion_SetTargetPositionFromMagnitudeAngle(struct Object *object,
                                                      s32 magnitude,
                                                      s32 angle) {
  s32 pos[3];

  if (object != 0) {
    SET_VECTOR(pos, object->x, object->y, object->z);
    Vector_AddPolarOffset(magnitude, angle, pos);
    Object_SetMoveTargetFar(object, pos[0], pos[1], pos[2]);
  }
}

// From here on objects are passed in each function's own view of them.

/// Counts the free blocks (owner 0xff) of the sprite VRAM cache's 512 blocks.
/// FieldEffect_LoadActionEffectResources subtracts this from VRAM_BLOCK_COUNT
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
struct Object *Object_Spawn(s32 kind, s32 x, s32 y, s32 z) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct Object *object;

  object = Object_CreateFar(kind, x, y, z);
  if (object != NULL) {
    if (object->spriteStorageKind == OBJECT_SPRITE_NONE) {
      Object_ReleaseFar((struct Object *)object);
      return NULL;
    }
    object->groundHeight =
        ((struct Object *)effectWork->mainObject)->groundHeight;
    object->motionFlags = 4;
    object->priorityFlags = 4;
    object->sprite->image.priority = 0;
    Object_SetSpriteFlagsFar((struct Object *)object, 0);
    Object_SelectAnimationFar(object, 1);
  }
  return object;
}

/// Sets the movement state and animation timer steps of every object but the
/// player's and except. FieldEffect_StartCast pauses them
/// (animation speed 0) while an effect plays and lets them go again at
/// normal speed.
void ObjectEvent_SetOthersPaused(struct Object *except, s32 state, s32 value) {
  s16 *player;
  struct Object *object;
  s32 localId;

  localId = 0;
  player = (s16 *)gPlayerObjectId;
  do {
    object = ObjectEvent_GetByLocalId(localId);
    if (localId != *player && object != NULL && object != except) {
      object->paused = state;
      Object_SetAnimationSpeedFar((struct Object *)object, value);
    }
    localId++;
  } while (localId <= OBJECT_LAST);
}

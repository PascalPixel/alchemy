/// @file
/// Running a field Psynergy by its kind, and helpers its effects share.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

// The dispatchers use the common effect scene. Individual update hooks use
// their actor or EffectSlot contract; neither is a scene-script callback.

/// Runs one step of the current Psynergy effect: 0 casts it with no target,
/// 1 casts it on its target (secondObjectId), 2 finishes it and 3 runs its
/// kind's extra handler. Exported as Field_DispatchTypeHandlerFar; Goma Cave's
/// entrance imports it and passes 1 to play Whirlwind on the stump's ivy from
/// its own event.
void Field_DispatchTypeHandler(s32 kind) {
  if (kind == 0)
    BattleFx_Run();
  else if (kind == 1)
    BattleFx_DispatchRequestKind();
  else if (kind == 2)
    BattleFx_ClearChildValueOnMismatch();
  else if (kind == 3)
    FieldEvent_RunTypeHandler();
}

void _call_via_r3(s32, s32);

/// Casts the current Psynergy effect with no target: runs the body of its
/// kind (scene->fieldKind).
///
/// Called by Field_UsePsynergy when no scene event names a target
/// (FLAG_PSYNERGY_TARGET clear), by Field_UseItem for an item with a field
/// action, and by Field_DispatchTypeHandler(0). Halt also moves
/// gGameState.pausedObject to the actor it stops.
void BattleFx_Run(void) {
  struct BattleFxScene *scene;
  struct FieldWork *runtime;
  s32 kind;
  s32 targetId;
  s32 nearestTargetId;

  scene = *(gWorkSlot + HEAP_SLOT_FX);
  runtime = *(gWorkSlot + HEAP_SLOT_EVENT);
  kind = scene->fieldKind;
  targetId = scene->secondObjectId;

  switch (kind) {
  case FIELD_KIND_MOVE:
    FieldFx_RunMove();
    return;
  case FIELD_KIND_LIFT:
    FieldFx_RunLift();
    return;
  case FIELD_KIND_CARRY:
    FieldFx_RunCarry();
    return;
  case FIELD_KIND_FORCE:
    FieldFx_RunForce();
    return;
  case FIELD_KIND_DOUSE:
    FieldFx_RunDouse();
    return;
  case FIELD_KIND_WHIRLWIND:
    FieldFx_RunWhirlwind();
    return;
  case FIELD_KIND_FROST:
    FieldFx_RunFrost();
    return;
  case FIELD_KIND_PLY:
    FieldFx_RunPly();
    return;
  case FIELD_KIND_GROWTH:
    FieldFx_RunGrowth();
    return;
  case FIELD_KIND_CATCH:
    FieldFx_RunCatch();
    return;
  case FIELD_KIND_HALT:
    // Free the object stopped before, then stop the nearest one.
    if (gGameState.pausedObject != -1) {
      BattleFx_ResumeObject(gGameState.pausedObject);
      gGameState.pausedObject = -1;
    }

    nearestTargetId =
        Field_FindNearbyPsynergyTarget(gGameState.selectedActor, kind);
    nearestTargetId = BattleFx_FilterObjectIdByFlags(nearestTargetId);
    if (Scene_FindActorTalkEvent(nearestTargetId) != 0) {
      BattleFx_SetupObjectPair(gGameState.selectedActor, nearestTargetId);
      BattleFx_MarkChildAndRunFallbackTransition(nearestTargetId);
      BattleEffect_PauseObject(nearestTargetId);
      gGameState.pausedObject = nearestTargetId;
    } else {
      BattleEffect_RunFallbackObjectTransition();
    }
    return;
  case FIELD_KIND_MIND_READ:
    // The wave and colour effect, stopping a running Reveal first.
    if (runtime->revealActive != 0)
      ResetSceneTransitionEffect();
    RunSceneTransitionEffect(scene->firstObjectId, targetId);
    return;
  case FIELD_KIND_REVEAL:
    FieldFx_RunReveal();
    return;
  case FIELD_KIND_CLOAK:
    FieldFx_StartCloak();
    return;
  case FIELD_KIND_RETREAT:
    FieldFx_RunRetreat();
    return;
  case FIELD_KIND_AVOID:
    FieldFx_RunAvoid();
    return;
  }
}

/// Casts the current Psynergy effect on its target (secondObjectId): runs the
/// handler of its kind.
///
/// Called by Field_UsePsynergy when a scene event names the target
/// (FLAG_PSYNERGY_TARGET still set after the before event), and by
/// Field_DispatchTypeHandler(1). Clears scene->enabled first: that is how the
/// runners BattleFx_Run shares (Force, Frost, Ply, Whirlwind, Catch) tell that
/// a scene event decides what happens to the target. Halt also moves
/// gGameState.pausedObject to the target.
void BattleFx_DispatchRequestKind(void) {
  struct BattleFxScene *scene = *(gWorkSlot + HEAP_SLOT_FX);
  struct FieldWork *runtime = *(gWorkSlot + HEAP_SLOT_EVENT);
  s32 kind = scene->fieldKind;
  s32 targetId = scene->secondObjectId;

  scene->enabled = 0;
  switch (kind) {
  case FIELD_KIND_MIND_READ:
    if (runtime->revealActive != 0)
      ResetSceneTransitionEffect();
    if (gGameState.pausedObject != scene->secondObjectId)
      ((struct ObjectRuntime *)scene->child)->movementState = 1;
    RunSceneTransitionEffect(scene->firstObjectId, targetId);
    break;
  case FIELD_KIND_MOVE:
    // Push the target with the direction keys.
    BattleFx_SteerPushedTarget(targetId);
    break;
  case FIELD_KIND_LIFT:
    FieldFx_RunLiftOnTarget(targetId);
    break;
  case FIELD_KIND_CARRY:
    BattleFx_SteerLiftedTarget(targetId);
    break;
  case FIELD_KIND_FORCE:
    FieldFx_RunForceOnTarget(targetId);
    break;
  case FIELD_KIND_DOUSE:
    FieldFx_RunDouseOnTarget(targetId);
    break;
  case FIELD_KIND_FROST:
    FieldFx_RunFrostOnTarget(targetId);
    break;
  case FIELD_KIND_GROWTH:
    FieldFx_RunGrowthOnTarget(targetId);
    break;
  case FIELD_KIND_HALT:
    if (gGameState.pausedObject != -1) {
      BattleFx_ResumeObject(gGameState.pausedObject);
      gGameState.pausedObject = -1;
    }
    BattleEffect_PauseObject(targetId);
    gGameState.pausedObject = targetId;
    BattleFx_MarkChildAndRunFallbackTransition(targetId);
    break;
  case FIELD_KIND_PLY:
    FieldFx_RunPlyOnTarget(targetId);
    break;
  case FIELD_KIND_WHIRLWIND:
    FieldFx_RunWhirlwindOnTarget(targetId);
    break;
  case FIELD_KIND_CATCH:
    FieldFx_RunCatchOnTarget(targetId);
    break;
  case FIELD_KIND_REVEAL:
    FieldFx_RunReveal();
    break;
  case FIELD_KIND_CLOAK:
    FieldFx_StartCloak();
    break;
  case FIELD_KIND_RETREAT:
    FieldFx_RunRetreat();
    break;
  case FIELD_KIND_AVOID:
    FieldFx_RunAvoid();
    break;
  }
}

/// Finishes the current effect when it is Mind Read: ends the effect and lets
/// its target move again, unless the target is the object kept stopped.
/// Called by Field_UsePsynergy after the after event, and by
/// Field_DispatchTypeHandler(2): Mind Read is the one kind whose effect stays
/// up through the after event, the conversation it reads.
void BattleFx_ClearChildValueOnMismatch(void) {
  struct BattleFxScene *scene = gEffectWork;

  if (scene->fieldKind == FIELD_KIND_MIND_READ) {
    BattleFx_FinishSceneAndReleaseHeapBlock();
    if (gGameState.pausedObject != scene->secondObjectId) {
      ((struct ObjectRuntime *)scene->child)->movementState = 0;
    }
  }
}

/// The extra handler of Reveal, Cloak and Avoid: what each does when it ends.
/// Called by Field_RunPsynergyEvents, which ends a Psynergy that runs out by
/// itself, and by Field_DispatchTypeHandler(3).
void FieldEvent_RunTypeHandler(void) {
  u32 kind;

  kind = ((struct BattleFxScene *)gEffectWork)->fieldKind;
  switch (kind) {
  case FIELD_KIND_REVEAL:
    ResetSceneTransitionEffect();
    return;
  case FIELD_KIND_CLOAK:
    FieldFx_EndCloak();
    return;
  case FIELD_KIND_AVOID:
    FieldEvent_ShowStatusMessage();
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
    script = ((const struct EffectDescriptor *)event)->result;
    if (script != 0) {
      if (script < 0x10000) {
        FieldEvent_Begin();
        Event_SetMessageId(((const struct EffectDescriptor *)event)->result);
        FieldEvent_ShowMessageAndWait(target, 0);
        FieldEvent_End();
      } else {
        _call_via_r3(param, target);
      }
    }
    if (GameFlag_TestFar(FLAG_NOTHING_HAPPENS) != 0) {
      FieldEvent_Begin();
      UiText_ShowPositionedMessageAndWaitFar(MsgNothingHappens, 1);
      FieldEvent_End();
    }
  }
  return 0;
}

/// Per-frame update: sets every part of an object's sprite to param
/// frame % 6, which cycles its colours. Move, Lift and Carry give it to the
/// target they hold, Retreat to its caster. A sprite that keeps its VRAM
/// cache entry (retainCacheEntry) is left alone.
void ObjectGroup_ApplyRandomChildValues(union FieldObject *fieldObject) {
  struct ObjectRuntime *owner = (struct ObjectRuntime *)fieldObject;
  s32 kind;
  struct AnimationObject *animation;
  s32 initialCount;
  s32 remainingParts;
  struct AnimationEntry **parts;
  struct AnimationEntry *spritePart;
  /* FAKEMATCH(temporary): a pointer local to gFrameCount, volatile as it is
   * declared, read on each pass; as gFrameCount read in the loop, 6 of 46
   * instructions differ in tbs-en. */
  volatile u32 *frameCount;
  s32 param;

  kind = owner->animationKind;
  if (kind == 1) {
    animation = owner->animation;
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
          spritePart->param = param;
        } while (remainingParts != 0);
      }
      animation->dirty = 1;
    }
  }
}

/// Sends an object toward the point magnitude (16.16 pixels) away from it in
/// direction angle: sets its move target. Carry, Lift, Douse, Move and the
/// Djinni's hop into battle throw things this way.
void Motion_SetTargetPositionFromMagnitudeAngle(struct ObjectRuntime *object,
                                                s32 magnitude, s32 angle) {
  s32 pos[3];

  if (object != 0) {
    SET_VECTOR(pos, object->x, object->y, object->z);
    Vector_AddPolarOffset(magnitude, angle, pos);
    Object_SetMoveTargetFar(object, pos[0], pos[1], pos[2]);
  }
}

// From here on objects are passed in each function's own view of them.

/// Counts the free blocks (owner 0xff) of the 512 resource blocks.
/// BattleFx_LoadActionEffectResources subtracts this from VRAM_BLOCK_COUNT to
/// record the blocks already occupied before the effect starts.
s32 ResourceTable_CountFreeBlocks(void) {
  u8 *marker = gResourceBlockOwners;
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
struct AnimationObject *
Sprite_ShareVramCache(struct AnimationObject *sourceSprite,
                      struct AnimationObject *sharedSprite) {
  struct AnimationObject *shared;
  struct AnimationObject *sprite;

  sprite = sourceSprite;
  shared = NULL;
  if (sprite != NULL) {
    if (sharedSprite == NULL) {
      sprite->retainCacheEntry = 1;
    } else {
      Resource_ResetEntry(sprite->slot);
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
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z) {
  struct BattleFxScene *work = gEffectWork;
  struct ObjectRuntime *object;

  object = Object_CreateFar(kind, x, y, z);
  if (object != NULL) {
    if (object->animationKind == 0) {
      ObjectDispatch_ReleaseFar((struct DispatchObject *)object);
      return NULL;
    }
    object->terrainHeight =
        ((struct ObjectRuntime *)work->mainObject)->terrainHeight;
    object->flags = 4;
    object->priorityFlags = 4;
    ((struct AnimationObject *)object->animation)->image.priority = 0;
    ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, 0);
    ObjectDispatch_SelectAnimationFar(object, 1);
  }
  return object;
}

/// Sets the movement state and animation timer steps of every object but the
/// player's and except. EventObject_Initialize stops them (state 1, speed 0)
/// while an effect plays and lets them go again (state 0, speed 0x10).
void ObjectGroup_SetActionForOthers(struct ObjectRuntime *except, s32 state,
                                    s32 value) {
  s16 *player;
  struct ObjectRuntime *object;
  s32 actorId;

  actorId = 0;
  player = (s16 *)gPlayerObjectId;
  do {
    object = ObjectTable_Get(actorId);
    if (actorId != *player && object != NULL && object != except) {
      object->movementState = state;
      ObjectDispatch_SetAnimationSpeedFar((struct DispatchObject *)object,
                                          value);
    }
    actorId++;
  } while (actorId <= OBJ_LAST);
}

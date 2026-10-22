/// @file
/// A Djinni's hop before its battle, joining the party, the effect slots.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// Update hook shared by the Djinni battle-start and Mercury afterimages.
void ObjectMotion_MoveHalfwayTowardTarget(union FieldObject *fieldObject) {
  FieldEffect_MoveAfterimageHalfway(fieldObject);
}

#define DJINNI_CLONE_MAX 8 ///< afterimages of the leaping Djinni

/// A Djinni's battle start: the Djinni (actor id) bounces twice, turns to face
/// the leader, hops back 16 pixels three times, then leaps 64 pixels at the
/// party trailing eight afterimages tinted for its element. Only
/// Field_RequestDjinnBattle calls it, when DJINN_NO_HOP is clear. Nothing
/// here releases the afterimages: they last until the scene is left for the
/// battle.
void Djinn_RunBattleStartHop(s32 djinniId) {
  struct FieldActor *djinni;
  struct FieldActor *copy;
  struct FieldActor *prev;
  void *sharedSprite;
  s32 dir;
  s32 back;
  s32 dist;
  s32 sprite;
  s32 tint;
  s32 i;
  UNUSED struct FieldActor *copies[DJINNI_CLONE_MAX];

  // The way the leader faces, to the nearest quarter turn.
  djinni = Object_GetById(djinniId);
  dir = (Object_GetById(gGameState.selectedActor)->facing + 0x2000) & 0xc000;
  FieldEvent_Start();
  WaitFrames(10);
  AudioCommand_PlayFar(SE_173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  AudioCommand_PlayFar(SE_175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  back = dir + 0x8000;
  WaitFrames(20);
  ObjectMotion_ArmCallback(djinniId, back, 0);
  WaitFrames(10);
  // Sprite priority 0, over every background, until the end. With
  // ACTOR_FACES_MOTION off it keeps facing the leader as it hops away, and
  // with motionFlags bit 1 alone it falls without following the terrain.
  djinni->sprite->image.priority = 0;
  djinni->facing = back;
  Object_GetById(djinniId)->steeringFlags &= ~1;
  djinni->motionFlags = 2;

  // Hop 16 pixels away from the leader, three times.
  dist = 16 << 16;
  Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)djinni,
                                             dist, dir);
  AudioCommand_PlayFar(SE_JUMP);
  ObjectMotion_Launch(djinniId, 4, 0);
  FieldObject_WaitForMoveFar(djinni);
  Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)djinni,
                                             dist, dir);
  AudioCommand_PlayFar(SE_JUMP);
  ObjectMotion_Launch(djinniId, 4, 0);
  FieldObject_WaitForMoveFar(djinni);
  Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)djinni,
                                             dist, dir);
  AudioCommand_PlayFar(SE_JUMP);
  ObjectMotion_Launch(djinniId, 4, 0);
  FieldObject_WaitForMoveFar(djinni);
  WaitFrames(20);

  // Afterimages of the same sprite at 15/16 size, each closing half the
  // distance to the one before it every frame, tinted by the Djinni's
  // sprite: the Venus sprite (90) takes param 2, Mars (92) 10, and Mercury
  // (91) and Jupiter (93) 9 (the scenes' Djinni placements show which
  // sprite is which element). They share one VRAM cache entry.
  sprite = djinni->sprite->entries[0]->animId;
  tint = 9;
  if (sprite == 90)
    tint = 2;
  if (sprite == 92)
    tint = 10;
  if (sprite == 91)
    tint = 9;
  sharedSprite = NULL;
  prev = djinni;
  for (i = 0; i < DJINNI_CLONE_MAX; i++) {
    copy = (struct FieldActor *)Object_CreateFar(
        sprite, djinni->x.fixed, djinni->y.fixed, djinni->z.fixed);
    copies[i] = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->motionFlags = 0;
      copy->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      copy->steeringFlags |= 1;
      copy->update = ObjectMotion_MoveHalfwayTowardTarget;
      copy->facing = djinni->facing;
      copy->sprite->image.priority = 0;
      Animation_ApplyChildValuesFar((struct DispatchObject *)copy, tint);
      ObjectDispatch_SelectAnimationFar(copy, 0);
      ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)copy, 0);
      sharedSprite = Sprite_ShareVramCache(copy->sprite, sharedSprite);
      copy->linkedObject = prev;
      prev = copy;
    }
  }

  // Leap 64 pixels at the leader, 12 pixels a frame upward, and after 24
  // frames freeze wherever it is, landed or not.
  Motion_SetTargetPositionFromMagnitudeAngle((struct ObjectRuntime *)djinni,
                                             64 << 16, dir + 0x8000);
  AudioCommand_PlayFar(SE_136);
  ObjectMotion_Launch(djinniId, 12, 0);
  WaitFrames(24);
  djinni->motionFlags = 0;
  djinni->velocityX = 0;
  djinni->velocityZ = 0;
  djinni->velocityY = 0;
  djinni->targetX = ACTOR_NO_TARGET;
  djinni->targetZ = ACTOR_NO_TARGET;
  djinni->targetY = ACTOR_NO_TARGET;
  ObjectDispatch_SelectAnimationFar(djinni, 0);
  djinni->sprite->image.priority = 2;
  FieldEvent_End();
}

/// A Djinni joins the party: djinn (0 to 19) of element goes to the member with
/// the fewest Djinn, or to no one and only its flag while FLAG_DJINN_LESSON
/// is set. Unless slot is -1, the Djinni's actor slot first plays its
/// element's capture effect and is freed. Ends with
/// `<Djinni> joined <member>.`
///
/// Nothing happens when Djinn_AddToLeastLoadedOwner refuses the Djinni: -1
/// when it was found before, -2 when no member has room; its actor then
/// stays. Called by Djinn_ResolvePendingEvent after a won Djinni battle, by
/// 13 scenes directly and by the debug rooms.
void Djinn_JoinParty(s32 slot, s32 element, s32 djinn) {
  struct FieldWork *work;
  s32 owner;

  work = (struct FieldWork *)gEventWork;
  if (GameFlag_TestFar(FLAG_DJINN_LESSON) != 0) {
    owner = 0;
    GameFlag_SetBitFar(djinn + element * 20 + FLAG_DJINNI_0);
  } else {
    owner = Djinn_AddToLeastLoadedOwnerFar(element, djinn);
  }

  if (owner >= 0) {
    FieldEvent_Start();
    WorldMap_StartEvent();
    if (slot != -1) {
      // On the world map the actors far from the view are removed while the
      // effect plays and placed again after it.
      if (work->mode == MODE_WORLD_MAP)
        ObjectTable_ClearOutOfBounds();

      if (element == 0) // Venus
        FieldEffect_RunVenusDjinnCapture(slot);
      else if (element == 1) // Mercury
        FieldEffect_RunMercuryDjinnCapture(slot);
      else if (element == 2) // Mars
        Djinn_RunMarsJoinEffect(slot);
      else if (element == 3) // Jupiter
        Djinn_RunJupiterJoinEffect(slot);

      work->actors[slot] = 0;
      if (work->mode == MODE_WORLD_MAP)
        ObjectTable_Respawn();
    }

    Djinn_ShowJoinedMessageFar(owner, element, djinn);
    WorldMap_EndEvent();
    FieldEvent_End();
  }
}

/// Puts a Djinni's actor back where its battle started from: for a pending
/// Djinni battle (pendingDjinnEvent 300 to 380, FLAG_KEEP_PARTY_POSITION set,
/// the battle over) the scene actor placed for that Djinni (condition = its
/// flag) goes back to its placement, or 32 pixels north of the leader when the
/// event has DJINN_NO_HOP (0x800) set, and stands (animation 1).
/// Field_RunScene calls it as it rebuilds the scene, only while an event is
/// still pending after Djinn_ResolvePendingEvent's first call, that is after
/// a won battle; the second call then makes this actor join the party.
void FieldObject_PlaceSceneActors(void) {
  s32 flags = gGameState.pendingDjinnEvent & 0xf000;
  u32 event = gGameState.pendingDjinnEvent & 0xfff;
  s32 id;

  // Kind 0 is a Djinni battle; a mimic chest's event (0x1000) is skipped.
  if (GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION) != 0 && flags == 0) {
    flags = event & 0x800;
    event &= 0x7ff;
    // event is unsigned: events below DJINNI_EVENT_0 fail the range test too.
    if (event - DJINNI_EVENT_0 <= 80 && gGameState.sceneChangeReason > 0) {
      for (id = 8; id <= 65; id++) {
        struct ScenePlacement *desc = Scene_FindActorPlacement(id);
        struct ObjectRuntime *obj;

        if (desc == NULL ||
            desc->condition - FLAG_DJINNI_0 != event - DJINNI_EVENT_0)
          continue;
        obj = ObjectTable_Get(id);
        if (obj == NULL)
          continue;
        // Without DJINN_NO_HOP, back at its placement in a new actor's
        // ground state: no ground height yet, following the terrain and
        // falling.
        if (flags == 0) {
          obj->terrainHeight = flags;
          obj->flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
          Object_SetPositionAndResetMotionFar(obj, desc->x, desc->y, desc->z);
        } else {
          struct ObjectRuntime *leader =
              ObjectTable_Get(gGameState.selectedActor);

          Object_SetPositionAndResetMotionFar(obj, leader->x, leader->y,
                                              leader->z - (32 << 16));
        }
        ObjectDispatch_SelectAnimationFar(obj, 1);
      }
    }
  }
}

/// Settles a pending Djinni battle as a scene loads, from the battle's result
/// in sceneChangeReason (the enemies defeated, 999 when the party fled, -1
/// when it lost). Field_RunScene calls it with shown 0 while it builds the
/// scene and, while the event is still pending, with shown nonzero once the
/// scene is on screen.
///
/// After a won battle the first call leaves the event pending and the second
/// makes the Djinni join the party, 40 frames in (Djinn_JoinParty).
/// Otherwise the first call sets the Djinni's FLAG_DJINNI_HIDING_0 flag, which
/// hides the Djinni's actor until the scene
/// is entered normally again (object/actors.c), and drops the event. A mimic
/// chest's event (kind 0x1000, object/events.c) sets the chest's flag when
/// the battle ended with exactly one enemy defeated, and is dropped.
void Djinn_ResolvePendingEvent(s32 shown) {
  struct GameState *st = &gGameState;
  s16 *pending = &st->pendingDjinnEvent;
  s32 kind = *pending & 0xf000;
  s32 flag = *pending & 0xfff;

  if (shown == 0) {
    if (kind == 0) {
      flag &= 0x7ff;
      if (flag < DJINNI_EVENT_0 || flag > DJINNI_EVENT_0 + 80)
        return;
      // Won: the event waits for the call once the scene is shown.
      if (st->sceneChangeReason > 0 && st->sceneChangeReason != 999)
        return;
      // DJINNI_EVENT_0 + n less 172 is FLAG_DJINNI_HIDING_0 + n.
      GameFlag_SetBitFar(flag - 172);
      *pending = kind;
    } else if (kind == 0x1000) {
      if (st->sceneChangeReason == 1)
        GameFlag_SetBitFar(flag);
      *pending = shown;
    }
    return;
  }
  if (kind == 0) {
    flag &= 0x7ff;
    if (flag >= DJINNI_EVENT_0 && flag <= DJINNI_EVENT_0 + 80) {
      flag &= 0x7ff;
      if (st->sceneChangeReason > 0) {
        s32 djinni = flag - DJINNI_EVENT_0;
        s32 element = djinni / 20;
        s32 index = djinni % 20;
        s32 actor;

        for (actor = 8; actor <= 65; actor++) {
          struct ScenePlacement *found = Scene_FindActorPlacement(actor);
          if (found != 0 &&
              found->condition - FLAG_DJINNI_0 == flag - DJINNI_EVENT_0) {
            WaitFrames(40);
            Djinn_JoinParty(actor, element, index);
            break;
          }
        }
      }
    }
  }
  gGameState.pendingDjinnEvent = 0;
}

/// Scheduler callback: updates the 24 effect slots.
void FieldEffect_UpdateAllSlots(void) { FieldEffect_UpdateSceneSlots(); }

/// Allocates and clears the effect slots (HEAP_SLOT_FX) and starts their
/// update callback. The block is a whole effect scene, gEffectWork, the slot
/// a Psynergy effect's scene also takes. Called by the Djinn joining effects
/// (jupiter.c, mars.c, venus.c and mercury.c) and Altmiller Cave's scene;
/// each ends with FieldEffect_ClearActiveSlotsAndScheduleUpdates.
void FieldEffect_InitializeSlots(void) {
  void *work;

  work = Runtime_AllocateHeapBlock(HEAP_SLOT_FX, sizeof(struct FieldFxScene));
  Dma_Fill32(0, work, sizeof(struct FieldFxScene));
  // 3200 is SCHEDULER_KEY_MAIN_LOOP: the slots update once a frame.
  Scheduler_AddOrUpdateCallback(FieldEffect_UpdateAllSlots, 3200);
}

/// Stops the effect slots: removes their callback, frees every slot still in
/// use and releases their heap block, gEffectWork with it. Then waits a
/// frame. Pairs with FieldEffect_InitializeSlots.
void FieldEffect_ClearActiveSlotsAndScheduleUpdates(void) {
  struct FieldFxScene *scene = gEffectWork;
  struct EffectSlot *slot;
  s32 i;

  Scheduler_RemoveCallback(FieldEffect_UpdateAllSlots);
  for (i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++) {
    slot = &scene->slots[i];
    if (slot->active != 0)
      FieldEffect_ClearOwnedSlot(slot);
  }
  Runtime_ReleaseHeapBlock(HEAP_SLOT_FX);
  WaitFrames(1);
}

/// Per-frame update: spins an object 1/8 turn a frame.
void FieldEffect_AdvanceSpinAngle(union FieldObject *fieldObject) {
  struct MotionObject *object = (struct MotionObject *)fieldObject;
  object->angle += 0x2000;
}

/// The states of a Jupiter Djinni's light particle, in order.
enum DjinniLightState {
  DJINNI_LIGHT_WIDENING = 0, ///< circles as its radius grows
  DJINNI_LIGHT_CIRCLING = 1,
  DJINNI_LIGHT_DRIFTING = 2, ///< its centre drifts toward the leader
  DJINNI_LIGHT_CLOSING = 3,  ///< circles as its radius shrinks
  DJINNI_LIGHT_DONE = 4      ///< frees its slot
};

/// Effect slot callback of a Jupiter Djinni's light particles: each circles
/// its centre while it widens for 60 frames, circles 40, drifts toward a point
/// 20 pixels over the leader for 40, closes in for 60 and is freed. It turns
/// 1/32 turn a frame throughout, and its radius grows, then shrinks, by 1
/// pixel a frame. The slot's origin is the centre, in screen coordinates
/// (Camera_WorldToScreen).
void FieldEffect_RunAngledApproachPhases(struct EffectSlot *particle) {
  struct MotionObject *target;
  struct EffectVector position;

  target = (struct MotionObject *)Object_GetById(gGameState.selectedActor);
  if (particle->state == DJINNI_LIGHT_WIDENING) {
    particle->orbitRadius += 1;
    particle->orbitAngle += 1;
    if ((s16)particle->age == 60) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == DJINNI_LIGHT_CIRCLING) {
    particle->orbitAngle += 1;
    if ((s16)particle->age == 40) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == DJINNI_LIGHT_DRIFTING) {
    particle->orbitAngle += 1;
    position.x = target->x;
    position.y = target->y + 0x140000;
    position.z = target->z;
    // The centre moves 1/8 of the way to the leader's screen point a frame.
    Camera_WorldToScreen(&position);
    particle->originX += (position.x - particle->originX) / 8;
    particle->originZ += (position.z - particle->originZ) / 8;
    if ((s16)particle->age == 40) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == DJINNI_LIGHT_CLOSING) {
    particle->orbitRadius -= 1;
    particle->orbitAngle += 1;
    if ((s16)particle->age == 60) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == DJINNI_LIGHT_DONE) {
    FieldEffect_ClearOwnedSlot(particle);
  }
  // orbitAngle is in 1/32 turns: << 11 makes it 1/65536 turns.
  position.x = particle->originX;
  position.z = particle->originZ;
  Vector_AddPolarOffset(particle->orbitRadius << 16, particle->orbitAngle << 11,
                        &position);
  particle->x = position.x;
  particle->z = position.z;
}

#undef DJINNI_CLONE_MAX

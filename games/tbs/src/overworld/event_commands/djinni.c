/// @file
/// A Djinni's hop before its battle, joining the party, the field effect
/// sprites.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// Update routine shared by the Djinni battle-start and Mercury afterimages.
void Actor_MoveHalfwayTowardTarget(struct Actor *actor) {
  FieldEffect_MoveAfterimageHalfway(actor);
}

#define DJINNI_CLONE_MAX 8 ///< afterimages of the leaping Djinni

/// A Djinni's battle start: the Djinni (actor id) bounces twice, turns to face
/// the leader, hops back 16 pixels three times, then leaps 64 pixels at the
/// party trailing eight afterimages tinted for its element. Only
/// Field_RequestDjinnBattle calls it, when DJINN_NO_HOP is clear. Nothing
/// here releases the afterimages: they last until the scene is left for the
/// battle.
void Djinn_RunBattleStartHop(s32 djinniId) {
  struct Actor *djinni;
  struct Actor *copy;
  struct Actor *prev;
  void *sharedSprite;
  s32 dir;
  s32 back;
  s32 dist;
  s32 sprite;
  s32 tint;
  s32 i;
  UNUSED struct Actor *copies[DJINNI_CLONE_MAX];

  // The way the leader faces, to the nearest quarter turn.
  djinni = Actor_GetById(djinniId);
  dir =
      (Actor_GetById(gMainRoutineWork.leaderActorId)->facing + 0x2000) & 0xc000;
  Event_Start();
  wait(10);
  Sound_PlayFar(SE_NOTE_LOW);
  Actor_RunRepeatedMotion(djinniId, 1);
  Sound_PlayFar(SE_NOTE_HIGH);
  Actor_RunRepeatedMotion(djinniId, 1);
  back = dir + 0x8000;
  wait(20);
  Actor_ArmCallback(djinniId, back, 0);
  wait(10);
  // Sprite priority 0, over every background, until the end. With
  // ACTOR_FACES_MOTION off it keeps facing the leader as it hops away, and
  // with motionFlags bit 1 alone it falls without following the ground.
  djinni->sprite->image.priority = 0;
  djinni->facing = back;
  Actor_GetById(djinniId)->steeringFlags &= ~1;
  djinni->motionFlags = 2;

  // Hop 16 pixels away from the leader, three times.
  dist = 16 << 16;
  Actor_SetTargetPositionFromMagnitudeAngle(djinni, dist, dir);
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(djinniId, 4, 0);
  Actor_WaitForMoveFar(djinni);
  Actor_SetTargetPositionFromMagnitudeAngle(djinni, dist, dir);
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(djinniId, 4, 0);
  Actor_WaitForMoveFar(djinni);
  Actor_SetTargetPositionFromMagnitudeAngle(djinni, dist, dir);
  Sound_PlayFar(SE_JUMP);
  Actor_Jump(djinniId, 4, 0);
  Actor_WaitForMoveFar(djinni);
  wait(20);

  // Afterimages of the same sprite at 15/16 size, each closing half the
  // distance to the one before it every frame, tinted by the Djinni's
  // sprite: the Venus sprite (90) takes param 2, Mars (92) 10, and Mercury
  // (91) and Jupiter (93) 9 (the scenes' Djinni actor placements show which
  // sprite is which element). They share one VRAM cache entry.
  sprite = djinni->sprite->entries[0]->animationId;
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
    copy = Actor_CreateFar(sprite, djinni->x.fixed, djinni->y.fixed,
                           djinni->z.fixed);
    copies[i] = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->motionFlags = 0;
      copy->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      copy->steeringFlags |= 1;
      copy->update = Actor_MoveHalfwayTowardTarget;
      copy->facing = djinni->facing;
      copy->sprite->image.priority = 0;
      Actor_SetSpriteColorModeFar(copy, tint);
      Actor_SelectAnimationFar(copy, 0);
      Actor_SetSpriteFlagsFar(copy, 0);
      sharedSprite = Sprite_ShareVramCacheEntry(copy->sprite, sharedSprite);
      copy->watchedActor = prev;
      prev = copy;
    }
  }

  // Leap 64 pixels at the leader, 12 pixels a frame upward, and after 24
  // frames freeze wherever it is, landed or not.
  Actor_SetTargetPositionFromMagnitudeAngle(djinni, 64 << 16, dir + 0x8000);
  Sound_PlayFar(SE_WHOOSH);
  Actor_Jump(djinniId, 12, 0);
  wait(24);
  djinni->motionFlags = 0;
  djinni->velocityX = 0;
  djinni->velocityZ = 0;
  djinni->velocityY = 0;
  djinni->targetX = ACTOR_NO_TARGET;
  djinni->targetZ = ACTOR_NO_TARGET;
  djinni->targetY = ACTOR_NO_TARGET;
  Actor_SelectAnimationFar(djinni, 0);
  djinni->sprite->image.priority = 2;
  Event_End();
}

/// A Djinni joins the party: djinn (0 to 19) of element goes to the party
/// member with the fewest Djinn, or to no one and only its flag while
/// FLAG_DJINN_LESSON is set. Unless slot is -1, the Djinni's actor slot first
/// plays its element's capture effect and is released. Ends with
/// `<Djinni> joined <member>.`
///
/// Nothing happens when Djinn_GiveToParty refuses the Djinni: -1
/// when it was found before, -2 when no party member has room; its actor then
/// stays. Called by Djinn_ResolvePending after a won Djinni battle, by
/// 13 scenes directly and by the test scenes.
void Djinn_JoinParty(s32 slot, s32 element, s32 djinn) {
  struct FieldWork *work;
  s32 owner;

  work = gFieldWork;
  if (GameFlag_GetFar(FLAG_DJINN_LESSON) != 0) {
    owner = 0;
    GameFlag_SetFar(djinn + element * 20 + FLAG_DJINN_FIRST);
  } else {
    owner = Djinn_GiveToPartyFar(element, djinn);
  }

  if (owner >= 0) {
    Event_Start();
    WorldMap_Freeze();
    if (slot != -1) {
      // On the World Map the actors far from the view are removed while
      // the Djinni's joining plays and placed again after it.
      if (work->mode == ACTOR_SYSTEM_WORLD_MAP)
        Actor_ClearOutOfBounds();

      if (element == 0) // Venus
        FieldEffect_RunVenusDjinnCapture(slot);
      else if (element == 1) // Mercury
        FieldEffect_RunMercuryDjinnCapture(slot);
      else if (element == 2) // Mars
        Djinn_RunMarsJoinEffect(slot);
      else if (element == 3) // Jupiter
        Djinn_RunJupiterJoinEffect(slot);

      work->actors[slot] = 0;
      if (work->mode == ACTOR_SYSTEM_WORLD_MAP)
        Actor_Respawn();
    }

    Djinn_ShowJoinedMessageFar(owner, element, djinn);
    WorldMap_Unfreeze();
    Event_End();
  }
}

/// Puts a Djinni's actor back where its battle started from: for a
/// pending Djinni battle (pendingDjinni 300 to 380,
/// FLAG_KEEP_PARTY_POSITION set, the battle over) the scene's actor placed
/// for that Djinni (condition = its flag) goes back to its placement's
/// position, or 32 pixels north of the leader when the event has DJINN_NO_HOP
/// (0x800) set, and stands (animation 1). FieldSequence_Run calls it as it
/// rebuilds the scene, only while an event is still pending after
/// Djinn_ResolvePending's first call, that is after a won battle; the second
/// call then makes this actor join the party.
void Actor_PlaceMapActors(void) {
  s32 flags = gMainRoutineWork.pendingDjinni & 0xf000;
  u32 event = gMainRoutineWork.pendingDjinni & 0xfff;
  s32 id;

  // Kind 0 is a Djinni battle; a mimic chest's event (0x1000) is skipped.
  if (GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION) != 0 && flags == 0) {
    flags = event & 0x800;
    event &= 0x7ff;
    // event is unsigned: events below DJINNI_BATTLE_EVENT_FIRST fail the range
    // test too.
    if (event - DJINNI_BATTLE_EVENT_FIRST <= 80 &&
        gMainRoutineWork.sequenceResult > 0) {
      for (id = 8; id <= 65; id++) {
        struct ActorPlacement *desc = Scene_FindActorPlacement(id);
        struct Actor *object;

        if (desc == NULL || desc->condition - FLAG_DJINN_FIRST !=
                                event - DJINNI_BATTLE_EVENT_FIRST)
          continue;
        object = Actor_Lookup(id);
        if (object == NULL)
          continue;
        // Without DJINN_NO_HOP, back at its placement's position in a new
        // actor's ground state: no ground height yet, following the ground and
        // falling.
        if (flags == 0) {
          object->groundHeight = flags;
          object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
          Actor_SetPositionAndResetMotionFar(object, desc->x, desc->y, desc->z);
        } else {
          struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);

          Actor_SetPositionAndResetMotionFar(object, leader->x.fixed,
                                             leader->y.fixed,
                                             leader->z.fixed - (32 << 16));
        }
        Actor_SelectAnimationFar(object, 1);
      }
    }
  }
}

/// Settles a pending Djinni battle as a scene loads, from the battle's result
/// in sequenceResult (the enemies defeated, 999 when the party fled, -1
/// when it lost). FieldSequence_Run calls it with shown 0 while it builds the
/// scene and, while the event is still pending, with shown nonzero once the
/// scene is on screen.
///
/// After a won battle the first call leaves the event pending and the second
/// makes the Djinni join the party, 40 frames in (Djinn_JoinParty).
/// Otherwise the first call sets the Djinni's FLAG_DJINNI_HIDING flag, which
/// hides the Djinni's actor until the scene
/// is entered normally again (overworld/actors.c), and drops the event.
/// A mimic chest's event (kind 0x1000, overworld/triggers.c) sets the chest's
/// flag when the battle ended with exactly one enemy defeated, and is dropped.
void Djinn_ResolvePending(s32 shown) {
  struct MainRoutineWork *st = &gMainRoutineWork;
  s16 *pending = &st->pendingDjinni;
  s32 kind = *pending & 0xf000;
  s32 flag = *pending & 0xfff;

  if (shown == 0) {
    if (kind == 0) {
      flag &= 0x7ff;
      if (flag < DJINNI_BATTLE_EVENT_FIRST ||
          flag > DJINNI_BATTLE_EVENT_FIRST + 80)
        return;
      // Won: the event waits for the call once the scene is shown.
      if (st->sequenceResult > 0 && st->sequenceResult != 999)
        return;
      // DJINNI_BATTLE_EVENT_FIRST + n less 172 is FLAG_DJINN_HIDING_FIRST + n.
      GameFlag_SetFar(flag - 172);
      *pending = kind;
    } else if (kind == 0x1000) {
      if (st->sequenceResult == 1)
        GameFlag_SetFar(flag);
      *pending = shown;
    }
    return;
  }
  if (kind == 0) {
    flag &= 0x7ff;
    if (flag >= DJINNI_BATTLE_EVENT_FIRST &&
        flag <= DJINNI_BATTLE_EVENT_FIRST + 80) {
      flag &= 0x7ff;
      if (st->sequenceResult > 0) {
        s32 djinni = flag - DJINNI_BATTLE_EVENT_FIRST;
        s32 element = djinni / 20;
        s32 index = djinni % 20;
        s32 actorId;

        for (actorId = 8; actorId <= 65; actorId++) {
          struct ActorPlacement *found = Scene_FindActorPlacement(actorId);
          if (found != 0 && found->condition - FLAG_DJINN_FIRST ==
                                flag - DJINNI_BATTLE_EVENT_FIRST) {
            wait(40);
            Djinn_JoinParty(actorId, element, index);
            break;
          }
        }
      }
    }
  }
  gMainRoutineWork.pendingDjinni = 0;
}

/// Task: updates the 24 field effect sprites.
void FieldEffect_UpdateAllSlots(void) { FieldEffect_UpdateEffectSlots(); }

/// Allocates and clears the field effect sprites (HEAP_SLOT_FIELD_EFFECT) and
/// starts their update task. The block is a whole field effect,
/// gFieldEffectWork, the slot a field Psynergy also takes. Called by
/// the Djinn joining effects (jupiter.c, mars.c, venus.c and mercury.c) and
/// Altmiller Cave's event; each ends with
/// FieldEffect_ClearActiveSlotsAndScheduleUpdates.
void FieldEffect_InitializeSlots(void) {
  void *work;

  work = HeapSlot_Alloc(HEAP_SLOT_FIELD_EFFECT, sizeof(struct FieldEffectWork));
  Dma_Fill32(0, work, sizeof(struct FieldEffectWork));
  // 3200 is TASK_PASS_FRAME: the slots update once a frame.
  Task_Add(FieldEffect_UpdateAllSlots, TASK_PASS_FRAME);
}

/// Stops the field effect sprites: removes their task, frees every slot
/// still in use and releases their heap block, gFieldEffectWork with it. Then
/// waits a frame. Pairs with FieldEffect_InitializeSlots.
void FieldEffect_ClearActiveSlotsAndScheduleUpdates(void) {
  struct FieldEffectWork *effectWork = gFieldEffectWork;
  struct FieldParticle *effect;
  s32 i;

  Task_Remove(FieldEffect_UpdateAllSlots);
  for (i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++) {
    effect = &effectWork->particles[i];
    if (effect->active != 0)
      FieldEffect_ClearOwnedSlot(effect);
  }
  HeapSlot_Release(HEAP_SLOT_FIELD_EFFECT);
  wait(1);
}

/// Per-frame update: spins an actor 1/8 turn a frame.
void FieldEffect_AdvanceSpinAngle(struct Actor *actor) {
  struct Actor *object = (struct Actor *)actor;
  object->facing += 0x2000;
}

/// The states of a Jupiter Djinni's light particle, in order.
enum DjinniLightState {
  DJINNI_LIGHT_WIDENING = 0, ///< circles as its radius grows
  DJINNI_LIGHT_CIRCLING = 1,
  DJINNI_LIGHT_DRIFTING = 2, ///< its centre drifts toward the leader
  DJINNI_LIGHT_CLOSING = 3,  ///< circles as its radius shrinks
  DJINNI_LIGHT_DONE = 4      ///< frees its slot
};

/// Field effect sprite callback of a Jupiter Djinni's light particles: each
/// circles its centre while it widens for 60 frames, circles 40, drifts toward
/// a point 20 pixels over the leader for 40, closes in for 60 and is freed. It
/// turns 1/32 turn a frame throughout, and its radius grows, then shrinks, by 1
/// pixel a frame. The slot's origin is the centre, in screen coordinates
/// (Camera_WorldToScreen).
void FieldEffect_RunAngledApproachPhases(struct FieldParticle *particle) {
  struct Actor *target;
  struct Vector3 position;

  target = Actor_GetById(gMainRoutineWork.leaderActorId);
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
    position.x = target->x.fixed;
    position.y = target->y.fixed + 0x140000;
    position.z = target->z.fixed;
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

/// @file
/// A Djinni's hop before its battle, joining the party, the field effect
/// sprites.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// Update routine shared by the Djinni battle-start and Mercury afterimages.
void Actor_MoveHalfwayTowardTarget(struct Actor *actor) {
  FieldEffect_MoveAfterimageHalfway(actor);
}

#define DJINNI_CLONE_MAX 8 ///< afterimages of the leaping Djinni

/// A Djinni's battle start: the Djinni (actor id) bounces twice, turns to face
/// the leader, hops back 16 pixels three times, then leaps 64 pixels at the
/// party trailing eight afterimages tinted for its element.
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
      (Actor_GetById(gMainRoutineWork.leaderActorId)->facing + TRIG_TURN / 8) &
      0xc000;
  Event_Start();
  wait(10);
  Sound_PlayFar(SE_NOTE_LOW);
  Actor_RunRepeatedMotion(djinniId, 1);
  Sound_PlayFar(SE_NOTE_HIGH);
  Actor_RunRepeatedMotion(djinniId, 1);
  back = dir + TRIG_TURN / 2;
  wait(20);
  Actor_FaceDirectionById(djinniId, back, 0);
  wait(10);
  // Sprite priority 0, over every background, until the end.
  djinni->sprite->image.priority = 0;
  djinni->facing = back;
  Actor_GetById(djinniId)->steeringFlags &= ~ACTOR_FACES_MOTION;
  djinni->motionFlags = ACTOR_FALLS;

  // Hop 16 pixels away from the leader, three times.
  dist = px(16);
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
  // distance to the one before it every frame, tinted by the Djinni's sprite:
  // SPRITE_VENUS_DJINNI takes param 2, SPRITE_MARS_DJINNI 10, and
  // SPRITE_MERCURY_DJINNI and SPRITE_JUPITER_DJINNI 9.
  sprite = djinni->sprite->entries[0]->animationId;
  tint = 9;
  if (sprite == SPRITE_VENUS_DJINNI)
    tint = 2;
  if (sprite == SPRITE_MARS_DJINNI)
    tint = 10;
  if (sprite == SPRITE_MERCURY_DJINNI)
    tint = 9;
  sharedSprite = NULL;
  prev = djinni;
  for (i = 0; i < DJINNI_CLONE_MAX; i++) {
    copy = Actor_CreateFar(sprite, djinni->x.fixed, djinni->y.fixed,
                           djinni->z.fixed);
    copies[i] = copy;
    if (copy != NULL) {
      copy->scaleY = Q16_ONE * 15 / 16;
      copy->scaleX = Q16_ONE * 15 / 16;
      copy->motionFlags = 0;
      copy->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      copy->steeringFlags |= ACTOR_FACES_MOTION;
      copy->update = Actor_MoveHalfwayTowardTarget;
      copy->facing = djinni->facing;
      copy->sprite->image.priority = 0;
      Actor_SetSpriteColorModeFar(copy, tint);
      Actor_SelectAnimationFar(copy, ANIM_STILL);
      Actor_SetSpriteFlagsFar(copy, 0);
      sharedSprite = Sprite_ShareVramCacheEntry(copy->sprite, sharedSprite);
      copy->watchedActor = prev;
      prev = copy;
    }
  }

  // Leap 64 pixels at the leader, 12 pixels a frame upward, and after 24
  // frames freeze wherever it is, landed or not.
  Actor_SetTargetPositionFromMagnitudeAngle(djinni, px(64),
                                            dir + TRIG_TURN / 2);
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
  Actor_SelectAnimationFar(djinni, ANIM_STILL);
  djinni->sprite->image.priority = 2;
  Event_End();
}

/// A Djinni joins the party: djinn (0 to 19) of element goes to the party
/// member with the fewest Djinn, or to no one and only its flag while
/// FLAG_DJINN_LESSON is set.
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
        Djinn_RunVenusJoinEffect(slot);
      else if (element == 1) // Mercury
        Djinn_RunMercuryJoinEffect(slot);
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

/// Puts a Djinni's actor back where its battle started from: for a pending
/// Djinni battle (pendingDjinni 300 to 380, FLAG_KEEP_PARTY_POSITION set, the
/// battle over) the scene's actor placed for that Djinni (condition = its flag)
/// goes back to its placement's position, or 32 pixels north of the leader when
/// the event has DJINN_NO_HOP set, and stands (animation 1).
void Djinn_RestoreActorAfterBattle(void) {
  s32 flags = gMainRoutineWork.pendingDjinni & DJINN_PENDING_KIND_MASK;
  u32 event = gMainRoutineWork.pendingDjinni & DJINN_PENDING_EVENT_MASK;
  s32 id;

  // Kind 0 is a Djinni battle; a mimic chest's event (CONDITION_FLAG_SET) is
  // skipped.
  if (GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION) != 0 && flags == 0) {
    flags = event & DJINN_NO_HOP;
    event &= DJINN_PENDING_BATTLE_EVENT_MASK;
    // event is unsigned: events below DJINNI_BATTLE_EVENT_FIRST fail the range
    // test too.
    if (event - DJINNI_BATTLE_EVENT_FIRST <= 80 &&
        gMainRoutineWork.sequenceResult > 0) {
      for (id = ACTOR_FIRST_PLACED; id <= ACTOR_LAST_ID; id++) {
        struct ActorPlacement *desc = Scene_FindActorPlacement(id);
        struct Actor *actor;

        if (desc == NULL || desc->condition - FLAG_DJINN_FIRST !=
                                event - DJINNI_BATTLE_EVENT_FIRST)
          continue;
        actor = Actor_Lookup(id);
        if (actor == NULL)
          continue;
        // Without DJINN_NO_HOP, back at its placement's position in a new
        // actor's ground state: no ground height yet, following the ground and
        // falling.
        if (flags == 0) {
          actor->groundHeight = flags;
          actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
          Actor_SetPositionAndResetMotionFar(actor, desc->x, desc->y, desc->z);
        } else {
          struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);

          Actor_SetPositionAndResetMotionFar(actor, leader->x.fixed,
                                             leader->y.fixed,
                                             leader->z.fixed - px(32));
        }
        Actor_SelectAnimationFar(actor, ANIM_STAND);
      }
    }
  }
}

/// Settles a pending Djinni battle as a scene loads, from the battle's result
/// in sequenceResult (the enemies defeated, 999 when the party fled, -1 when it
/// lost).
void Djinn_ResolvePending(s32 shown) {
  struct MainRoutineWork *st = &gMainRoutineWork;
  s32 kind = st->pendingDjinni & DJINN_PENDING_KIND_MASK;
  s32 flag = st->pendingDjinni & DJINN_PENDING_EVENT_MASK;

  if (shown == 0) {
    if (kind == 0) {
      flag &= DJINN_PENDING_BATTLE_EVENT_MASK;
      if (flag < DJINNI_BATTLE_EVENT_FIRST ||
          flag > DJINNI_BATTLE_EVENT_FIRST + 80)
        return;
      // Won: the event waits for the call once the scene is shown.
      if (st->sequenceResult > 0 && st->sequenceResult != 999)
        return;
      // DJINNI_BATTLE_EVENT_FIRST + n less 172 is FLAG_DJINN_HIDING_FIRST + n.
      GameFlag_SetFar(flag - 172);
      st->pendingDjinni = kind;
    } else if (kind == CONDITION_FLAG_SET) {
      if (st->sequenceResult == 1)
        GameFlag_SetFar(flag);
      st->pendingDjinni = shown;
    }
    return;
  }
  if (kind == 0) {
    flag &= DJINN_PENDING_BATTLE_EVENT_MASK;
    if (flag >= DJINNI_BATTLE_EVENT_FIRST &&
        flag <= DJINNI_BATTLE_EVENT_FIRST + 80) {
      flag &= DJINN_PENDING_BATTLE_EVENT_MASK;
      if (st->sequenceResult > 0) {
        s32 djinni = flag - DJINNI_BATTLE_EVENT_FIRST;
        s32 element = djinni / 20;
        s32 index = djinni % 20;
        s32 actorId;

        for (actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_ID;
             actorId++) {
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
/// starts their update task.
void FieldEffect_InitializeSlots(void) {
  void *work;

  work = HeapSlot_Alloc(HEAP_SLOT_FIELD_EFFECT, sizeof(struct FieldEffectWork));
  Dma_Fill32(0, work, sizeof(struct FieldEffectWork));
  Task_Add(FieldEffect_UpdateAllSlots, TASK_PASS_FRAME);
}

/// Stops the field effect sprites: removes their task, frees every slot still
/// in use and releases their heap block, gFieldEffectWork with it.
void FieldEffect_ReleaseSlots(void) {
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
  actor->facing += TRIG_TURN / 8;
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
/// a point 20 pixels over the leader for 40, closes in for 60 and is freed.
void FieldEffect_RunAngledApproachPhases(struct FieldParticle *particle) {
  struct Actor *target;
  struct Vector3 position;

  target = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (particle->state == DJINNI_LIGHT_WIDENING) {
    particle->orbitRadius += 1;
    particle->orbitAngle += 1;
    if (particle->age == 60) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == DJINNI_LIGHT_CIRCLING) {
    particle->orbitAngle += 1;
    if (particle->age == 40) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == DJINNI_LIGHT_DRIFTING) {
    particle->orbitAngle += 1;
    position.x = target->x.fixed;
    position.y = target->y.fixed + px(20);
    position.z = target->z.fixed;
    // The centre moves 1/8 of the way to the leader's screen point a frame.
    Camera_WorldToScreen(&position);
    particle->originX += (position.x - particle->originX) / 8;
    particle->originZ += (position.z - particle->originZ) / 8;
    if (particle->age == 40) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == DJINNI_LIGHT_CLOSING) {
    particle->orbitRadius -= 1;
    particle->orbitAngle += 1;
    if (particle->age == 60) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == DJINNI_LIGHT_DONE) {
    FieldEffect_ClearOwnedSlot(particle);
  }
  // orbitAngle is in 1/32 turns: << 11 makes it 1/65536 turns.
  position.x = particle->originX;
  position.z = particle->originZ;
  Vector_AddPolarOffset(px(particle->orbitRadius), particle->orbitAngle << 11,
                        &position);
  particle->x = position.x;
  particle->z = position.z;
}

#undef DJINNI_CLONE_MAX

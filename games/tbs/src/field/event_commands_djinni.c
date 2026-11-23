/// @file
/// A Djinni's hop before its battle, a Djinni joining the party, and the
/// field effect sprites the joining lights are drawn with.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// Update routine shared by the Djinni battle-start and Mercury afterimages.
void Actor_MoveHalfwayTowardTarget(struct Actor *actor) {
  FieldEffect_MoveAfterimageHalfway(actor);
}

#define DJINNI_AFTERIMAGES 8 ///< afterimages of the leaping Djinni

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
  FIXED dist;
  s32 sprite;
  s32 tint;
  s32 i;
  UNUSED struct Actor *copies[DJINNI_AFTERIMAGES];

  djinni = Actor_GetById(djinniId);
  dir = FACING_NEAREST_CARDINAL(
      Actor_GetById(gMainRoutineWork.leaderActorId)->facing);
  Event_Start();
  wait(10);
  Sound_PlayFar(SE_NOTE_LOW);
  Actor_RunRepeatedMotion(djinniId, 1);
  Sound_PlayFar(SE_NOTE_HIGH);
  Actor_RunRepeatedMotion(djinniId, 1);
  back = dir + DEGtoANG(180);
  wait(20);
  Actor_FaceDirectionById(djinniId, back, 0);
  wait(10);
  // Sprite priority 0, over every background, until the end.
  djinni->sprite->image.priority = 0;
  djinni->facing = back;
  Actor_GetById(djinniId)->steeringFlags &= ~ACTOR_FACES_MOTION;
  djinni->motionFlags = ACTOR_FALLS;

  // Hop 16 pixels away from the leader, three times.
  dist = toFIXED(16);
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

  // Afterimages of the same sprite at AFTERIMAGE_SCALE, each closing half the
  // distance to the one before it every frame, tinted by the Djinni's sprite.
  sprite = djinni->sprite->entries[0]->animationId;
  tint = SPRITE_COLOR_BLUE_TINT;
  if (sprite == SPRITE_VENUS_DJINNI)
    tint = SPRITE_COLOR_RBG;
  if (sprite == SPRITE_MARS_DJINNI)
    tint = SPRITE_COLOR_RED_TINT;
  // DEAD: tint is already SPRITE_COLOR_BLUE_TINT for SPRITE_MERCURY_DJINNI; the
  // ROM keeps the test and the store.
  if (sprite == SPRITE_MERCURY_DJINNI)
    tint = SPRITE_COLOR_BLUE_TINT;
  sharedSprite = NULL;
  prev = djinni;
  for (i = 0; i < DJINNI_AFTERIMAGES; i++) {
    copy = Actor_CreateFar(sprite, djinni->x.fixed, djinni->y.fixed,
                           djinni->z.fixed);
    // DEAD: nothing reads copies; the ROM keeps the store and its slot.
    copies[i] = copy;
    if (copy != NULL) {
      copy->scaleY = AFTERIMAGE_SCALE;
      copy->scaleX = AFTERIMAGE_SCALE;
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
  Actor_SetTargetPositionFromMagnitudeAngle(djinni, toFIXED(64),
                                            dir + DEGtoANG(180));
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
/// FLAG_DJINN_LESSON is set. Its actor, actorId (-1 for none), plays its
/// element's joining and leaves the scene.
void Djinn_JoinParty(s32 actorId, s32 element, s32 djinn) {
  struct FieldWork *work;
  s32 owner;

  work = gFieldWork;
  if (GameFlag_GetFar(FLAG_DJINN_LESSON) != 0) {
    owner = 0;
    GameFlag_SetFar(djinn + element * DJINN_PER_ELEMENT + FLAG_DJINN_FIRST);
  } else {
    owner = Djinn_GiveToPartyFar(element, djinn);
  }

  if (owner >= 0) {
    Event_Start();
    WorldMap_Freeze();
    if (actorId != -1) {
      // On the World Map the actors far from the view are removed while
      // the Djinni's joining plays and placed again after it.
      if (work->mode == ACTOR_SYSTEM_WORLD_MAP)
        Actor_ClearOutOfBounds();

      if (element == ELEMENT_VENUS)
        Djinn_RunVenusJoinEffect(actorId);
      else if (element == ELEMENT_MERCURY)
        Djinn_RunMercuryJoinEffect(actorId);
      else if (element == ELEMENT_MARS)
        Djinn_RunMarsJoinEffect(actorId);
      else if (element == ELEMENT_JUPITER)
        Djinn_RunJupiterJoinEffect(actorId);

      work->actors[actorId] = 0;
      if (work->mode == ACTOR_SYSTEM_WORLD_MAP)
        Actor_Respawn();
    }

    Djinn_ShowJoinedMessageFar(owner, element, djinn);
    WorldMap_Unfreeze();
    Event_End();
  }
}

/// Puts a Djinni's actor back where its battle started from: for a pending
/// Djinni battle (pendingDjinni DJINNI_BATTLE_EVENT_FIRST plus the Djinni's
/// number, FLAG_KEEP_PARTY_POSITION set, the battle over) the scene's actor
/// placed for that Djinni (condition = its flag) goes back to its placement's
/// position, or 32 pixels north of the leader when the event has DJINN_NO_HOP
/// set, and stands.
void Djinn_RestoreActorAfterBattle(void) {
  s32 flags = gMainRoutineWork.pendingDjinni & DJINN_PENDING_KIND_MASK;
  u32 event = gMainRoutineWork.pendingDjinni & DJINN_PENDING_EVENT_MASK;
  s32 id;

  // Only a Djinni battle; a mimic chest's is skipped.
  if (GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION) != 0 &&
      flags == PENDING_DJINNI_BATTLE) {
    flags = event & DJINN_NO_HOP;
    event &= DJINN_PENDING_BATTLE_EVENT_MASK;
    // event is unsigned: events below DJINNI_BATTLE_EVENT_FIRST fail the range
    // test too.
    if (event - DJINNI_BATTLE_EVENT_FIRST <=
            ELEMENT_COUNT * DJINN_PER_ELEMENT &&
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
                                             leader->z.fixed - toFIXED(32));
        }
        Actor_SelectAnimationFar(actor, ANIM_STAND);
      }
    }
  }
}

/// Settles a pending Djinni or mimic battle as a scene loads, from the battle's
/// result in sequenceResult (the enemies defeated, BATTLE_OUTCOME_FLED when the
/// party fled, BATTLE_OUTCOME_LOST when it lost): with shown 0, before the
/// scene shows, a lost or fled Djinni hides again and a beaten mimic's flag is
/// set; once it shows, a beaten Djinni joins the party.
void Djinn_ResolvePending(s32 shown) {
  struct MainRoutineWork *mainWork = &gMainRoutineWork;
  s32 kind = mainWork->pendingDjinni & DJINN_PENDING_KIND_MASK;
  s32 flag = mainWork->pendingDjinni & DJINN_PENDING_EVENT_MASK;

  if (shown == 0) {
    if (kind == PENDING_DJINNI_BATTLE) {
      flag &= DJINN_PENDING_BATTLE_EVENT_MASK;
      if (flag < DJINNI_BATTLE_EVENT_FIRST ||
          flag > DJINNI_BATTLE_EVENT_FIRST + ELEMENT_COUNT * DJINN_PER_ELEMENT)
        return;
      // Won: the event waits for the call once the scene is shown.
      if (mainWork->sequenceResult > 0 &&
          mainWork->sequenceResult != BATTLE_OUTCOME_FLED)
        return;
      GameFlag_SetFar(flag -
                      (DJINNI_BATTLE_EVENT_FIRST - FLAG_DJINN_HIDING_FIRST));
      mainWork->pendingDjinni = kind;
    } else if (kind == PENDING_MIMIC_CHEST) {
      if (mainWork->sequenceResult == 1)
        GameFlag_SetFar(flag);
      mainWork->pendingDjinni = shown;
    }
    return;
  }
  if (kind == PENDING_DJINNI_BATTLE) {
    flag &= DJINN_PENDING_BATTLE_EVENT_MASK;
    if (flag >= DJINNI_BATTLE_EVENT_FIRST &&
        flag <= DJINNI_BATTLE_EVENT_FIRST + ELEMENT_COUNT * DJINN_PER_ELEMENT) {
      flag &= DJINN_PENDING_BATTLE_EVENT_MASK;
      if (mainWork->sequenceResult > 0) {
        s32 djinni = flag - DJINNI_BATTLE_EVENT_FIRST;
        s32 element = djinni / DJINN_PER_ELEMENT;
        s32 index = djinni % DJINN_PER_ELEMENT;
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
  actor->facing += DEGtoANG(45);
}

/// The frames a Jupiter Djinni's light widens, and later closes in over, so
/// that it ends where it began; and the frames it circles and drifts.
#define DJINNI_LIGHT_RADIUS_FRAMES 60
#define DJINNI_LIGHT_CIRCLING_FRAMES 40
#define DJINNI_LIGHT_DRIFTING_FRAMES 40

/// The states of a Jupiter Djinni's light particle, in order.
enum DjinniLightState {
  DJINNI_LIGHT_WIDENING = 0, ///< circles as its radius grows
  DJINNI_LIGHT_CIRCLING = 1,
  DJINNI_LIGHT_DRIFTING = 2, ///< its centre drifts toward the leader
  DJINNI_LIGHT_CLOSING = 3,  ///< circles as its radius shrinks
  DJINNI_LIGHT_DONE = 4      ///< frees its slot
};

/// Field effect sprite callback of a Jupiter Djinni's light particles, the ring
/// of coloured lights a joining Jupiter Djinni bursts into: each circles its
/// centre while it widens, circles, drifts toward a point 20 pixels over the
/// leader and closes in on it, and is freed. It turns 1/32 turn a frame
/// throughout, and its radius grows, then shrinks, by 1 pixel a frame.
void FieldEffect_RunAngledApproachPhases(struct FieldParticle *particle) {
  struct Actor *target;
  struct Vector3 position;

  target = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (particle->state == DJINNI_LIGHT_WIDENING) {
    particle->orbitRadius += 1;
    particle->orbitAngle += 1;
    if (particle->age == DJINNI_LIGHT_RADIUS_FRAMES) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == DJINNI_LIGHT_CIRCLING) {
    particle->orbitAngle += 1;
    if (particle->age == DJINNI_LIGHT_CIRCLING_FRAMES) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == DJINNI_LIGHT_DRIFTING) {
    particle->orbitAngle += 1;
    position.x = target->x.fixed;
    position.y = target->y.fixed + toFIXED(20);
    position.z = target->z.fixed;
    // The centre moves 1/8 of the way to the leader's screen point a frame.
    Camera_WorldToScreen(&position);
    particle->originX += (position.x - particle->originX) / 8;
    particle->originZ += (position.z - particle->originZ) / 8;
    if (particle->age == DJINNI_LIGHT_DRIFTING_FRAMES) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == DJINNI_LIGHT_CLOSING) {
    particle->orbitRadius -= 1;
    particle->orbitAngle += 1;
    if (particle->age == DJINNI_LIGHT_RADIUS_FRAMES) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == DJINNI_LIGHT_DONE) {
    FieldEffect_ClearOwnedSlot(particle);
  }
  position.x = particle->originX;
  position.z = particle->originZ;
  Vector_AddPolarOffset(toFIXED(particle->orbitRadius),
                        particle->orbitAngle * (TRIG_TURN / 32), &position);
  particle->x = position.x;
  particle->z = position.z;
}

#undef DJINNI_AFTERIMAGES
#undef DJINNI_LIGHT_RADIUS_FRAMES
#undef DJINNI_LIGHT_CIRCLING_FRAMES
#undef DJINNI_LIGHT_DRIFTING_FRAMES

/// @file
/// A Djinni's battle start and its joining the party, the field effect
/// sprites' slots, and a Jupiter Djinni's light.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "flags.h"
#include "fixed_math.h"
#include "sounds.h"
#include "gamepak/far_call/game.h"
#include "field/event_commands.h"
#include "field/emote.h"
#include "field/field.h"
#include "field/object.h"
#include "system/system.h"
#include "field/scene_call.h"
#include "macros.h"

#define DJINNI_AFTERIMAGES 8 ///< afterimages of the leaping Djinni

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

/// Per-frame update: spins an actor 1/8 turn a frame.
void FieldEffect_AdvanceSpinAngle(struct Actor *actor) {
  actor->facing += DEGtoANG(45);
}

/// Per-frame update: halves the actor's distance to its watched actor along x,
/// y and z.
void Actor_MoveHalfwayTowardTarget(struct Actor *actor) {
  struct Actor *target = actor->watchedActor;

  actor->x.fixed += (target->x.fixed - actor->x.fixed) / 2;
  {
    s32 position = actor->y.fixed;

    position += (target->y.fixed - position) / 2;
    actor->y.fixed = position;
  }
  {
    FIXED position = actor->z.fixed;

    position += (target->z.fixed - position) / 2;
    actor->z.fixed = position;
  }
}

/// The sound of the Djinni's leap: SE_WHOOSH until patch 1, SE_HIGH_JUMP from
/// it.
#if TLA_PATCH_1
#define LEAP_SOUND SE_HIGH_JUMP
#else
#define LEAP_SOUND SE_WHOOSH
#endif

/// A Djinni's battle start: the Djinni (actor id) bounces twice, turns to face
/// the leader with an anger vein over it, jumps three times where it stands,
/// spinning and each time higher, then leaps 16 pixels at the leader trailing
/// eight afterimages tinted for its element, and stops once it has come down
/// within 20 pixels of the leader's height.
void Djinn_RunBattleStartHop(s32 djinniId) {
  struct Actor *djinni;
  struct Actor *leader;
  struct Actor *copy;
  struct Actor *prev;
  void *sharedSprite;
  s32 dir;
  s32 back;
  s32 sprite;
  s32 tint;
  s32 i;
  UNUSED struct Actor *copies[DJINNI_AFTERIMAGES];

  djinni = Actor_GetById(djinniId);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  dir = (leader->facing + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
  Event_Start();
  wait(10);
  Sound_PlayFar(SE_NOTE_LOW);
  Actor_RunRepeatedMotion(djinniId, 1);
  Sound_PlayFar(SE_NOTE_HIGH);
  Actor_RunRepeatedMotion(djinniId, 1);
  wait(10);
  back = dir + DEGtoANG(180);
  Actor_FaceDirectionById(djinniId, back, 0);
  wait(10);
  djinni->facing = back;
  Actor_ShowEmoteById(djinniId, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
  Actor_GetById(djinniId)->steeringFlags &= ~ACTOR_FACES_MOTION;
  djinni->motionFlags = ACTOR_FALLS;
  djinni->update = (ObjectUpdate)FieldEffect_AdvanceSpinAngle;
#if TLA_PATCH_1
  Sound_PlayFar(SE_JUMP);
#endif
  djinni->gravity = toFIXED(1) / 4;
  djinni->velocityY = toFIXED(9) / 2;
  wait(33);
  ((struct Sprite *)djinni->sprite)->image.priority = 0;
#if TLA_PATCH_1
  Sound_PlayFar(SE_JUMP);
#endif
  djinni->velocityY = toFIXED(4.8);
  wait(35);
#if TLA_PATCH_1
  Sound_PlayFar(SE_JUMP);
#endif
  djinni->velocityY = toFIXED(4.9);
  wait(38);

  djinni->update = NULL;
  djinni->facing = back;
  // Afterimages of the same sprite at AFTERIMAGE_SCALE, each closing half the
  // distance to the one before it every frame, tinted by the Djinni's sprite.
  sprite = ((struct Sprite *)djinni->sprite)->entries[0]->animationId;
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
      ((struct Sprite *)copy->sprite)->image.priority = 0;
      Actor_SetSpriteColorModeFar(copy, tint);
      Actor_SelectAnimationFar(copy, ANIM_STILL);
      Actor_SetSpriteFlagsFar(copy, 0);
      sharedSprite = Sprite_ShareVramCacheEntry(copy->sprite, sharedSprite);
      copy->watchedActor = prev;
      prev = copy;
    }
  }

  djinni->speedLimit = toFIXED(0.3);
  djinni->acceleration = toFIXED(0.3);
  Actor_SetTargetPositionFromMagnitudeAngle(djinni, toFIXED(16),
                                            dir + DEGtoANG(180));
  Sound_PlayFar(LEAP_SOUND);
  Actor_Jump(djinniId, 6, 5);
  wait(24);
  for (i = 0; i < 120; i++) {
    if (ABS(djinni->y.fixed - leader->y.fixed) < toFIXED(20))
      break;
    wait(1);
  }
  djinni->motionFlags = 0;
  djinni->velocityX = 0;
  djinni->velocityZ = 0;
  djinni->velocityY = 0;
  djinni->targetX = ACTOR_NO_TARGET;
  djinni->targetZ = ACTOR_NO_TARGET;
  djinni->targetY = ACTOR_NO_TARGET;
  Actor_SelectAnimationFar(djinni, ANIM_STILL);
  ((struct Sprite *)djinni->sprite)->image.priority = 2;
#if TLA_PATCH_1
  djinni->gravity = toFIXED(1);
#endif
  Event_End();
}

/// A Djinni joins. The element's last Djinni stands for the one of The Broken
/// Seal's chosen for it (SCENE_STATE_CHOSEN_DJINN; none: nothing happens).
/// While FLAG_DJINN_LESSON is set it joins the leader by its flag alone,
/// otherwise the party member with the fewest; if it found one, the element's
/// join effect plays at the actor (none for actor -1), the Djinni is added,
/// and the actor's event slot is cleared (from patch 1; the Japanese release
/// leaves it set).
void Djinn_JoinParty(s32 actor, s32 element, s32 djinni) {
  s32 owner;

  if (djinni == DJINN_PER_ELEMENT - 1) {
    s32 recorded = gSceneLocalState[element + SCENE_STATE_CHOSEN_DJINN];

    if (recorded == 0)
      return;
    djinni = recorded - 1;
  }
  if (GameFlag_Get(FLAG_DJINN_LESSON)) {
    owner = gMainRoutineWork.leaderActorId;
    GameFlag_Set(FLAG_DJINNI(element * DJINN_PER_ELEMENT + djinni));
  } else {
    owner = Djinn_GiveToPartyFar(element, djinni);
  }
  if (owner < 0)
    return;
  Event_Start();
  if (actor != -1) {
    if (element == ELEMENT_VENUS)
      Djinn_RunVenusJoinEffect(actor);
    else if (element == ELEMENT_MERCURY)
      Djinn_RunMercuryJoinEffect(actor);
    else if (element == ELEMENT_MARS)
      Djinn_RunMarsJoinEffect(actor);
    else if (element == ELEMENT_JUPITER)
      Djinn_RunJupiterJoinEffect(actor);
  }
  WorldMap_Freeze();
  Djinn_ShowJoinedMessageFar(owner, element, djinni);
  WorldMap_ReloadPendingAreaGraphics();
#if TLA_PATCH_1
  gFieldWork->actors[actor] = NULL;
#endif
  Event_End();
}

/// Puts a Djinni's actor back where its battle started from: for a pending
/// Djinni battle (pendingDjinni from DJINNI_BATTLE_EVENT_FIRST, one for each
/// Djinni, FLAG_KEEP_PARTY_POSITION set, the battle over) the scene's actor
/// placed for that Djinni (condition = its flag) goes back to its placement's
/// position, or 32 pixels north of the leader when the event has DJINN_NO_HOP
/// set, and stands. FieldSequence_Run calls it as it rebuilds the scene, only
/// while an event is still pending after Djinn_ResolvePending's first call,
/// that is after a won battle; the second call then makes this actor join the
/// party.
void Djinn_RestoreActorAfterBattle(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  u32 raw = (u16)party->pendingDjinni;
  s32 flags = party->pendingDjinni & DJINN_PENDING_KIND_MASK;
  u32 scene = DJINN_PENDING_EVENT_MASK;
  s32 id;

  scene &= raw;
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0 && flags == 0) {
    flags = scene & DJINN_NO_HOP;
    scene &= DJINN_PENDING_BATTLE_EVENT_MASK;
    if (scene - DJINNI_BATTLE_EVENT_FIRST <=
            ELEMENT_COUNT * DJINN_PER_ELEMENT &&
        party->sequenceResult > 0) {
      s32 *leaderId;

      for (id = ACTOR_FIRST_PLACED, leaderId = &party->leaderActorId;
           id <= FIELD_ACTOR_COUNT - 1; id++) {
        struct ActorPlacement *placement =
            (struct ActorPlacement *)Scene_FindActorPlacement(id);
        struct Actor *actor;

        if (placement == NULL || placement->condition - FLAG_DJINN_FIRST !=
                                     scene - DJINNI_BATTLE_EVENT_FIRST)
          continue;
        actor = Actor_Lookup(id);
        if (actor == NULL)
          continue;
        if (flags == 0) {
          actor->groundHeight = flags;
          actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
          Actor_SetPositionAndResetMotionFar(actor, placement->x, placement->y,
                                             placement->z);
        } else {
          struct Actor *leader = Actor_Lookup(*leaderId);

          Actor_SetPositionAndResetMotionFar(actor, leader->x.fixed,
                                             leader->y.fixed,
                                             leader->z.fixed - toFIXED(32));
        }
        Actor_SelectAnimationFar(actor, ANIM_STAND);
      }
    }
  }
}

/// Settles a pending Djinni battle as a scene loads, from the battle's result
/// in sequenceResult (the enemies defeated, BATTLE_OUTCOME_FLED when the party
/// fled, -1 when it lost), as The Broken Seal's; a mimic chest's event
/// (CONDITION_FLAG_SET) falls through to the clear.
void Djinn_ResolvePending(s32 shown) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s16 *pending = &work->pendingDjinni;
  s32 kind = *pending & DJINN_PENDING_KIND_MASK;
  s32 flag = *pending & DJINN_PENDING_EVENT_MASK;

  if (shown == 0) {
    if (kind == 0) {
      flag &= DJINN_PENDING_BATTLE_EVENT_MASK;
      if (flag < DJINNI_BATTLE_EVENT_FIRST ||
          flag > DJINNI_BATTLE_EVENT_FIRST + ELEMENT_COUNT * DJINN_PER_ELEMENT)
        return;
      if (work->sequenceResult > 0 &&
          work->sequenceResult != BATTLE_OUTCOME_FLED)
        return;
      GameFlag_Set(flag -
                   (DJINNI_BATTLE_EVENT_FIRST - FLAG_DJINN_HIDING_FIRST));
      *pending = kind;
      return;
    } else if (kind == CONDITION_FLAG_SET) {
      if (work->sequenceResult > 0 &&
          work->sequenceResult != BATTLE_OUTCOME_FLED)
        GameFlag_Set(flag);
    } else
      return;
  } else if (kind == 0) {
    flag &= DJINN_PENDING_BATTLE_EVENT_MASK;
    if (flag >= DJINNI_BATTLE_EVENT_FIRST &&
        flag <= DJINNI_BATTLE_EVENT_FIRST + ELEMENT_COUNT * DJINN_PER_ELEMENT) {
      flag &= DJINN_PENDING_BATTLE_EVENT_MASK;
      if (work->sequenceResult > 0) {
        s32 djinn = flag - DJINNI_BATTLE_EVENT_FIRST;
        s32 element = djinn / DJINN_PER_ELEMENT;
        s32 index = djinn % DJINN_PER_ELEMENT;
        s32 actor;

        for (actor = ACTOR_FIRST_PLACED; actor <= FIELD_ACTOR_COUNT - 1;
             actor++) {
          struct ActorPlacement *found =
              (struct ActorPlacement *)Scene_FindActorPlacement(actor);
          if (found != 0 && found->condition - FLAG_DJINN_FIRST ==
                                flag - DJINNI_BATTLE_EVENT_FIRST) {
            wait(40);
            Djinn_JoinParty(actor, element, index);
            break;
          }
        }
      }
    }
  }
  gMainRoutineWork.pendingDjinni = 0;
}

/// Task: updates the field effect sprites.
void FieldEffect_UpdateAllSlots(void) {
  struct FieldEffectWork *scene;
  struct FieldParticle *slot;
  s32 remainingSlots;

  scene = Ram_HeapSlots->fieldEffectWork;
  remainingSlots = ARRAY_COUNT(scene->particles) - 1;
  slot = scene->particles;
  do {
    remainingSlots -= 1;
    FieldParticle_Update(slot);
    slot++;
  } while (remainingSlots >= 0);
}

/// Allocates and clears the field effect sprites (HEAP_SLOT_FIELD_EFFECT) and
/// starts their update task.
void FieldEffect_InitializeSlots(void) {
  void *work;

  work = HeapSlot_Alloc(HEAP_SLOT_FIELD_EFFECT, sizeof(struct FieldEffectWork));
  Dma_Fill32(0, work, sizeof(struct FieldEffectWork));
  Task_Add((s32)FieldEffect_UpdateAllSlots, TASK_PASS_FRAME);
}

/// Stops the field effect sprites: removes their task, frees every slot still
/// in use and releases their heap block, the field Psynergy's work with it.
void FieldEffect_ReleaseSlots(void) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct FieldParticle *slot;
  s32 i;

  Task_Remove(FieldEffect_UpdateAllSlots);
  for (i = 0; i < (s32)ARRAY_COUNT(scene->particles); i++) {
    slot = &scene->particles[i];
    if (slot->active != 0)
      FieldEffect_ClearOwnedSlot(slot);
  }
  HeapSlot_Release(HEAP_SLOT_FIELD_EFFECT);
  wait(1);
}

/// Per-frame update of a joining Venus or Jupiter Djinni: spins it an eighth of
/// a turn, as FieldEffect_AdvanceSpinAngle does.
void Djinn_AdvanceSpinAngle(struct Actor *djinni) {
  djinni->facing += DEGtoANG(45);
}

/// Field effect sprite callback of a Jupiter Djinni's light particles: each
/// circles its centre while it widens, circles, drifts toward a point 20 pixels
/// over the leader and closes in, and is freed. It turns 1/32 turn a frame
/// throughout, and its radius grows, then shrinks, by 1 pixel a frame. The
/// slot's origin is the centre, in screen coordinates (Camera_WorldToScreen).
void FieldEffect_RunAngledApproachPhases(struct FieldParticle *particle) {
  struct Actor *target;
  struct Vector3 position;

  target = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (particle->state == DJINNI_LIGHT_WIDENING) {
    particle->orbitRadius += 1;
    particle->orbitAngle += 1;
    if ((s16)particle->age == DJINNI_LIGHT_RADIUS_FRAMES) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == DJINNI_LIGHT_CIRCLING) {
    particle->orbitAngle += 1;
    if ((s16)particle->age == DJINNI_LIGHT_CIRCLING_FRAMES) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == DJINNI_LIGHT_DRIFTING) {
    particle->orbitAngle += 1;
    SET_POSITION(&position, target->x.fixed, target->y.fixed + toFIXED(20),
                 target->z.fixed);
    // The centre moves 1/8 of the way to the leader's screen point a frame.
    Camera_WorldToScreen(&position);
    particle->originX += (position.x - particle->originX) / 8;
    particle->originZ += (position.z - particle->originZ) / 8;
    if ((s16)particle->age == DJINNI_LIGHT_DRIFTING_FRAMES) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == DJINNI_LIGHT_CLOSING) {
    particle->orbitRadius -= 1;
    particle->orbitAngle += 1;
    if ((s16)particle->age == DJINNI_LIGHT_RADIUS_FRAMES) {
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
#undef LEAP_SOUND
#undef DJINNI_LIGHT_RADIUS_FRAMES
#undef DJINNI_LIGHT_CIRCLING_FRAMES
#undef DJINNI_LIGHT_DRIFTING_FRAMES

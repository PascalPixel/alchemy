/// @file
/// Routines whose twins are in The Broken Seal's field/event_commands/djinni.c.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"

/// Per-frame update: spins an object 1/8 turn a frame.
void FieldEffect_AdvanceSpinAngle(struct MotionObject *object) {
  object->angle += 0x2000;
}

// Objects: follow a watched object.


/// Halves the distance to the watched object along x, y and z.
/// obj: object to move.
void Actor_MoveHalfwayTowardTarget(struct Actor *obj) {
  struct Actor *target = obj->watchedObject;

  obj->x += (target->x - obj->x) / 2;
  {
    s32 position = obj->y;

    position += (target->y - position) / 2;
    obj->y = position;
  }
  {
    s32 position = obj->z;

    position += (target->z - position) / 2;
    obj->z = position;
  }
}


void Actor_SetSingleChildFlagsFar(struct Actor *object, u32 flags);

#if TLA_PATCH_1
#define LEAP_SOUND 153
#else
#define LEAP_SOUND 136
#endif

struct Actor *Actor_GetById(u32 objectId);
void Event_Start(void);
void Event_End(void);
void Motion_SetVarCbAndRefresh(u32 objectId, s32 variant);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void Actor_Jump(s32 id, s32 speed, s32 frames);
/* VIEW(motes): routine Func_080200c0 as struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z); visual_attributes has struct BattleEffectWatchedObject *Func_080200c0(s32 kind, s32 x, s32 y, s32 z). */
extern struct Actor *Func_080200c0__motes(s32 kind, s32 x, s32 y, s32 z) __asm__("Func_080200c0");
void Animation_ApplyChildValuesFar(struct Actor *object, u32 value);
void *Sprite_ShareVramCacheEntry(void *sprite, void *last);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);

void Func_080d7524(s32 djinniId) {
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
  struct Actor *copies[8];

  djinni = Actor_GetById(djinniId);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  dir = (leader->facing + 0x2000) & 0xc000;
  Event_Start();
  wait(10);
  Sound_PlayFar(173);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  Sound_PlayFar(175);
  Motion_SetVarCbAndRefresh(djinniId, 1);
  wait(10);
  back = dir + 0x8000;
  Actor_ArmCallback(djinniId, back, 0);
  wait(10);
  djinni->facing = back;
  Func_080d47b4(djinniId, 0x103, 50);
  Actor_GetById(djinniId)->actionFlags &= ~1;
  djinni->motionFlags = 2;
  djinni->update = (void (*)(void))FieldEffect_AdvanceSpinAngle;
#if TLA_PATCH_1
  Sound_PlayFar(152);
#endif
  djinni->gravity = 0x4000;
  djinni->velocityY = 0x48000;
  wait(33);
  ((struct AnimationObject *)djinni->sprite)->image.priority = 0;
#if TLA_PATCH_1
  Sound_PlayFar(152);
#endif
  djinni->velocityY = 0x4cccc;
  wait(35);
#if TLA_PATCH_1
  Sound_PlayFar(152);
#endif
  djinni->velocityY = 0x4e666;
  wait(38);

  djinni->update = NULL;
  djinni->facing = back;
  sprite = ((struct AnimationObject *)djinni->sprite)->entries[0]->animId;
  tint = 9;
  if (sprite == 243)
    tint = 2;
  if (sprite == 245)
    tint = 10;
  // DEAD: tint is already 9 for sprite 244; the source keeps the test.
  if (sprite == 244)
    tint = 9;
  sharedSprite = NULL;
  prev = djinni;
  for (i = 0; i < 8; i++) {
    copy = Func_080200c0__motes(sprite, djinni->x, djinni->y, djinni->z);
    // DEAD: nothing reads copies; the source keeps the store.
    copies[i] = copy;
    if (copy != NULL) {
      copy->scaleY = 0xf000;
      copy->scaleX = 0xf000;
      copy->motionFlags = 0;
      copy->priorityFlags = 2;
      copy->actionFlags |= 1;
      copy->update = (void (*)(void))Actor_MoveHalfwayTowardTarget;
      copy->facing = djinni->facing;
      ((struct AnimationObject *)copy->sprite)->image.priority = 0;
      Animation_ApplyChildValuesFar(copy, tint);
      Actor_SetMode(copy, 0);
      Actor_SetSingleChildFlagsFar(copy, 0);
      sharedSprite = Sprite_ShareVramCacheEntry(copy->sprite, sharedSprite);
      copy->watchedObject = prev;
      prev = copy;
    }
  }

  djinni->speedLimit = 0x4ccc;
  djinni->acceleration = 0x4ccc;
  Motion_SetTargetPositionFromMagnitudeAngle(djinni, 0x100000, dir + 0x8000);
  Sound_PlayFar(LEAP_SOUND);
  Actor_Jump(djinniId, 6, 5);
  wait(24);
  for (i = 0; i < 120; i++) {
    if (ABS(djinni->y - leader->y) < 0x140000)
      break;
    wait(1);
  }
  djinni->motionFlags = 0;
  djinni->velocityX = 0;
  djinni->velocityZ = 0;
  djinni->velocityY = 0;
  djinni->targetX = 0x80000000;
  djinni->targetZ = 0x80000000;
  djinni->targetY = 0x80000000;
  Actor_SetMode(djinni, 0);
  ((struct AnimationObject *)djinni->sprite)->image.priority = 2;
#if TLA_PATCH_1
  djinni->gravity = 0x10000;
#endif
  Event_End();
}

s32 Djinn_AddToLeastLoadedOwnerFar(s32 element, s32 djinni);
void Func_080d82e0(s32 actor);
void BattleEffect_RunMercuryDjinnCapture(s32 actor);
void Djinn_RunMarsJoinEffect(s32 actor);
void Djinn_RunJupiterJoinEffect(s32 actor);
void Func_080cb82c(void);
void Func_08038328(s32 owner, s32 element, s32 djinni);
void Func_080cb8a4(void);

extern s8 gData_02001000[];

struct EventActors {
  u8 unk00[20];
  void *actors[64];
};

/// A Djinni joins. Djinni 19 means the one recorded for the element in the
/// save (none: nothing happens). With flag 366 set it joins the current owner
/// by its flag (48 + element * 20 + djinni), otherwise the least loaded owner;
/// if it found one, the element's join effect plays at the actor (none for
/// actor -1), the Djinni is added, and the actor's event slot is cleared
/// (from patch 1; the Japanese release leaves it set).
void Func_080d7788(s32 actor, s32 element, s32 djinni) {
  s32 owner;

  if (djinni == 19) {
    s32 recorded = gData_02001000[element + 0x1950];

    if (recorded == 0)
      return;
    djinni = recorded - 1;
  }
  if (GameFlag_Test(366)) {
    owner = gMainRoutineWork.currentOwner;
    GameFlag_SetBit(element * 20 + djinni + 48);
  } else {
    owner = Djinn_AddToLeastLoadedOwnerFar(element, djinni);
  }
  if (owner < 0)
    return;
  Event_Start();
  if (actor != -1) {
    if (element == 0)
      Func_080d82e0(actor);
    else if (element == 1)
      BattleEffect_RunMercuryDjinnCapture(actor);
    else if (element == 2)
      Djinn_RunMarsJoinEffect(actor);
    else if (element == 3)
      Djinn_RunJupiterJoinEffect(actor);
  }
  Func_080cb82c();
  Func_08038328(owner, element, djinni);
  Func_080cb8a4();
#if TLA_PATCH_1
  ((struct EventActors *)Ram_HeapSlots->eventWork)->actors[actor] = NULL;
#endif
  Event_End();
}

struct ActorDescriptor {
  s16 action;
  s16 scene;
  u8 unk04[4];
  s32 x;
  s32 y;
  s32 z;
};

void Func_080d784c(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  u32 raw = (u16)party->pendingDjinnEvent;
  s32 flags = party->pendingDjinnEvent & 0xf000;
  u32 scene = 0xfff;
  s32 id;

  scene &= raw;
  if (GameFlag_Test(0x109) != 0 && flags == 0) {
    flags = scene & 0x800;
    scene &= 0x7ff;
    if (scene - 300 <= 80 && party->sequenceResult > 0) {
      s32 *owner;

      for (id = 8, owner = &party->currentOwner; id <= 79; id++) {
        struct ActorDescriptor *desc = Scene_FindActorPlacement(id);
        struct Actor *obj;

        if (desc == NULL || desc->scene - 48 != scene - 300)
          continue;
        obj = ObjectTable_Get(id);
        if (obj == NULL)
          continue;
        if (flags == 0) {
          obj->terrainHeight = flags;
          obj->motionFlags = 3;
          Actor_SetPositionAndResetMotionFar(obj, desc->x, desc->y, desc->z);
        } else {
          struct Actor *selection = ObjectTable_Get(*owner);

          Actor_SetPositionAndResetMotionFar(obj, selection->x, selection->y,
                                             selection->z - (32 << 16));
        }
        Actor_SetMode(obj, 1);
      }
    }
  }
}

struct MapActor {
  s16 id;
  s16 event;
};

/// The Broken Seal's Djinn_ResolvePendingEvent, with this game's actors up to
/// 79, the plain flag calls, the event at gMainRoutineWork's 0x254 and its
/// 0x1000 kind tested like the first, falling into the common clear.
void Func_080d793c(s32 capture) {
  struct MainRoutineWork *st = &gMainRoutineWork;
  s16 *pending = &st->pendingDjinnEvent;
  s32 kind = *pending & 0xf000;
  s32 flag = *pending & 0xfff;

  if (capture == 0) {
    if (kind == 0) {
      flag &= 0x7ff;
      if (flag < 300 || flag > 380)
        return;
      if (st->sequenceResult > 0 && st->sequenceResult != 999)
        return;
      GameFlag_SetBit(flag - 172);
      *pending = kind;
      return;
    } else if (kind == 0x1000) {
      if (st->sequenceResult > 0 && st->sequenceResult != 999)
        GameFlag_SetBit(flag);
    } else
      return;
  } else if (kind == 0) {
    flag &= 0x7ff;
    if (flag >= 300 && flag <= 380) {
      flag &= 0x7ff;
      if (st->sequenceResult > 0) {
        s32 djinn = flag - 300;
        s32 element = djinn / 20;
        s32 index = djinn % 20;
        s32 actor;

        for (actor = 8; actor <= 79; actor++) {
          struct MapActor *found =
              (struct MapActor *)Scene_FindActorPlacement(actor);
          if (found != 0 && found->event - 48 == flag - 300) {
            wait(40);
            Func_080d7788(actor, element, index);
            break;
          }
        }
      }
    }
  }
  gMainRoutineWork.pendingDjinnEvent = 0;
}


/* VIEW(motes): tag struct BattleFxScene as struct BattleFxScene__motes; field_psynergy_work.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:3869: dereferencing pointer to incomplete type. */
struct BattleFxScene__motes {
  u8 unk00[0x50];
  struct FieldParticle slots[24];
  u8 unk710[0xe4];
};

#define BATTLE_EFFECT_SLOT 224

void BattleEffect_UpdateAllSlots(void) {
  struct BattleFxScene__motes *scene;
  struct FieldParticle *slot;
  s32 remainingSlots;

  scene = Ram_HeapSlots->effectWork;
  remainingSlots = 0x17;
  slot = scene->slots;
  do {
    remainingSlots -= 1;
    FieldParticle_Update(slot);
    slot++;
  } while (remainingSlots >= 0);
}

void BattleEffect_InitializeSlots(void) {
  void *work;

  work = HeapSlot_Alloc(BATTLE_EFFECT_SLOT, sizeof(struct BattleFxScene__motes));
  Dma_Fill32(0, work, sizeof(struct BattleFxScene__motes));
  Task_Add((s32)BattleEffect_UpdateAllSlots, TASK_PASS_VBLANK);
}

void BattleEffect_ClearOwnedSlot(struct FieldParticle *);

void BattleEffect_ClearActiveSlotsAndScheduleUpdates(void) {
  struct BattleFxScene__motes *scene = Ram_HeapSlots->effectWork;
  struct FieldParticle *slot;
  s32 i;

  Task_Remove((u32)BattleEffect_UpdateAllSlots);
  for (i = 0; i < 24; i++) {
    slot = &scene->slots[i];
    if (slot->active != 0)
      BattleEffect_ClearOwnedSlot(slot);
  }
  HeapSlot_Release(BATTLE_EFFECT_SLOT);
  wait(1);
}

void BattleEffect_AdvanceSpinAngle(struct MotionObject *object) {
  object->angle += 0x2000;
}


struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

/* VIEW(motes): routine Camera_WorldToScreen as void Camera_WorldToScreen(struct EffectVector *position); ground_particles has void Camera_WorldToScreen(union FieldCoordinate *position). */
extern void Camera_WorldToScreen__motes(struct EffectVector *position) __asm__("Camera_WorldToScreen");
/* VIEW(motes): routine Vector_AddPolarOffset as void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectVector *position); f0d489c has void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position). */
extern void Vector_AddPolarOffset__motes(s32 magnitude, s32 angle,
                           struct EffectVector *position) __asm__("Vector_AddPolarOffset");
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);

/// Field effect sprite callback of a Jupiter Djinni's light particles: each
/// circles its centre while it widens for 60 frames, circles 40, drifts toward
/// a point 20 pixels over the leader for 40, closes in for 60 and is freed. It
/// turns 1/32 turn a frame throughout, and its radius grows, then shrinks, by 1
/// pixel a frame. The slot's origin is the centre, in screen coordinates
/// (Camera_WorldToScreen).
void FieldEffect_RunAngledApproachPhases(struct FieldParticle *particle) {
  struct Actor *owner;
  struct EffectVector position;

  owner = Actor_GetById(gMainRoutineWork.currentOwner);
  // Each age test reads age signed (ldrsh); as particle->age == 60 and so on,
  // 54 of 122 instructions differ in tla-en.
  if (particle->state == 0) {
    particle->orbitRadius += 1;
    particle->orbitAngle += 1;
    if ((s16)particle->age == 60) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 1) {
    particle->orbitAngle += 1;
    if ((s16)particle->age == 40) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 2) {
    particle->orbitAngle += 1;
    position.x = owner->x;
    position.y = owner->y + 20 * Q16_ONE;
    position.z = owner->z;
    Camera_WorldToScreen__motes(&position);
    particle->originX += (position.x - particle->originX) / 8;
    particle->originZ += (position.z - particle->originZ) / 8;
    if ((s16)particle->age == 40) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 3) {
    particle->orbitRadius -= 1;
    particle->orbitAngle += 1;
    if ((s16)particle->age == 60) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 4) {
    BattleEffect_ClearOwnedSlot(particle);
  }
  position.x = particle->originX;
  position.z = particle->originZ;
  Vector_AddPolarOffset__motes(particle->orbitRadius << 16, particle->orbitAngle << 11,
                        &position);
  particle->x = position.x;
  particle->z = position.z;
}

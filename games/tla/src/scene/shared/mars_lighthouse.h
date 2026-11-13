/// @file
/// What the rooms of Mars Lighthouse share: the rides, the beam and its
/// targets, the ice the leader slides on, and the small actor routines
/// their objects run.
/// Each routine is a macro a scene expands where its copy sits, with the
/// names of its own data and calls.
#ifndef ALCHEMY_SCENE_SHARED_MARS_LIGHTHOUSE_H
#define ALCHEMY_SCENE_SHARED_MARS_LIGHTHOUSE_H

/// A ridden object's update: it drifts and slows, and lands the leader where it
/// touches down.
#define DEFINE_RIDE_UPDATE_CARRIED(name, id1, call1, call2)                    \
  void name(struct Actor *object) {                                            \
    struct id1 *work = (struct id1 *)(gInventorySnapshot + 0x188);             \
    struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);            \
    struct EventWork *event = Ram_HeapSlots->eventWork;                        \
    s32 dx = object->bounce;                                                   \
    s32 dz;                                                                    \
    s32 i;                                                                     \
    s32 direction;                                                             \
    s32 id;                                                                    \
                                                                               \
    object->x += dx;                                                           \
    object->y += object->gravity;                                              \
    dz = object->unk4c;                                                        \
    object->z += dz;                                                           \
    object->bounce = dx - dx / 18;                                             \
    object->unk4c = dz - dz / 16;                                              \
    object->scaleX += object->speedLimit;                                      \
    object->scaleY += object->acceleration;                                    \
    ((struct SpriteParts *)object->sprite)->unk12 += object->timer;            \
    if (actor->y == actor->terrainHeight && call1(object, actor) &&            \
        gMainRoutineWork.renderMode == 0) {                                    \
      if (object->unk4c != 0) {                                                \
        work->value68 = 0;                                                     \
        {                                                                      \
          u16 *cell = &work->direction;                                        \
                                                                               \
          if (object->unk4c > 0) {                                             \
            direction = 1;                                                     \
            *cell = direction;                                                 \
          } else {                                                             \
            direction = 0xffff;                                                \
            *cell = direction;                                                 \
          }                                                                    \
        }                                                                      \
      }                                                                        \
      object->priorityFlags = 2;                                               \
      {                                                                        \
        s32 speed = object->unk4c;                                             \
                                                                               \
        object->unk4c = 0;                                                     \
        object->z -= speed;                                                    \
        object->gravity += speed >> 2;                                         \
      }                                                                        \
      work->landed = 1;                                                        \
      event->exit = work->value0a;                                             \
    }                                                                          \
    for (i = 0; i < 4 && (id = work->slots[i].id) != 0; i++) {                 \
      actor = Actor_Get(id);                                                   \
                                                                               \
      if (actor->y == actor->terrainHeight && call2(object, actor)) {          \
        if (work->slots[i].blocked == 0) {                                     \
          s32 speed;                                                           \
                                                                               \
          object->priorityFlags = 2;                                           \
          speed = object->unk4c;                                               \
          object->unk4c = 0;                                                   \
          object->z -= speed;                                                  \
          object->gravity += speed >> 2;                                       \
        } else {                                                               \
          work->blocked = 1;                                                   \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }

/// Starts the beam over its targets and stops.
#define DEFINE_BEAM_START(name, call1)                                         \
  void name(u16 *targets, u16 *stops, s32 a2, s32 a3) {                        \
    struct MarsLighthouseBeam *beam =                                          \
        (struct MarsLighthouseBeam *)(gInventorySnapshot + 0x188);             \
    struct MarsLighthouseBeamTarget *target = beam->targets;                   \
    s32 id;                                                                    \
    s32 i;                                                                     \
    s32 value;                                                                 \
    s32 one;                                                                   \
    s32 flags;                                                                 \
    s32 height;                                                                \
    struct Actor *object;                                                      \
    s32 alpha;                                                                 \
    s32 control;                                                               \
                                                                               \
    if (Event_GetFlag(0x109) == 0) {                                           \
      for (i = 0; i < 4 && (id = *targets++) > 0; i++) {                       \
        flags = *targets;                                                      \
        height = flags & 15;                                                   \
        flags &= 0xf0;                                                         \
        object = Actor_Get(id);                                                \
        targets++;                                                             \
        if (object != NULL) {                                                  \
          Actor_SetSpriteFlags(object, 0);                                     \
          ((u8 *)object)[98] = one = 1;                                        \
          if ((flags & 0x80) == 0)                                             \
            object->collisionFlags = 0;                                        \
          target->active = one;                                                \
          target->flags = flags;                                               \
          target->object = Actor_Get(id);                                      \
          target->height = height << 16;                                       \
          target->index = i;                                                   \
          target++;                                                            \
        }                                                                      \
      }                                                                        \
      if (stops != NULL) {                                                     \
        for (i = 0; i < 4; i++) {                                              \
          value = *stops++;                                                    \
          beam->stops[i].unk00 = value;                                        \
          if (value <= 0)                                                      \
            break;                                                             \
          beam->stops[i].unk02 = *stops++;                                     \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    beam->unk10 = a2;                                                          \
    beam->unk0a = a3;                                                          \
    if (REG_BLDCNT == 0) {                                                     \
      alpha = 0xc08;                                                           \
      REG_BLDALPHA = alpha;                                                    \
      control = 0x3f10;                                                        \
      REG_BLDCNT = control;                                                    \
    }                                                                          \
    Task_AddFrame(call1, 0x480);                                               \
  }

/// Ride_UpdateCarried for the rides that set the direction through their work.
#define DEFINE_RIDE_UPDATE_LANDING(name, id1, call1, call2)                    \
  void name(struct Actor *object) {                                            \
    struct id1 *work = (struct id1 *)(gInventorySnapshot + 0x188);             \
    struct Actor *other = Actor_Get(gMainRoutineWork.currentOwner);            \
    struct EventWork *event = Ram_HeapSlots->eventWork;                        \
    s32 dx = object->bounce;                                                   \
    s32 dz;                                                                    \
    s32 i;                                                                     \
    struct RideDirection *direction;                                           \
                                                                               \
    object->z += dx;                                                           \
    object->y += object->gravity;                                              \
    dz = object->unk4c;                                                        \
    object->x += dz;                                                           \
    object->bounce = dx - dx / 18;                                             \
    object->unk4c = dz - dz / 16;                                              \
    object->scaleX += object->speedLimit;                                      \
    object->scaleY += object->acceleration;                                    \
    ((struct SpriteParts *)object->sprite)->unk12 += object->timer;            \
    if (other->y == other->terrainHeight && call1(object, other) &&            \
        gMainRoutineWork.renderMode == 0) {                                    \
      if (object->unk4c != 0) {                                                \
        work->direction.unk02 = 0;                                             \
        direction = &work->direction;                                          \
        if (object->unk4c > 0)                                                 \
          direction->unk00 = 1;                                                \
        else                                                                   \
          direction->unk00 = 0xffff;                                           \
      }                                                                        \
      object->priorityFlags = 2;                                               \
      object->z -= object->bounce;                                             \
      object->gravity += object->bounce >> 2;                                  \
      object->unk4c = 0;                                                       \
      work->landed = 1;                                                        \
      event->exit = work->value0a;                                             \
    }                                                                          \
    for (i = 0; i < 4; i++) {                                                  \
      s32 id = work->slots[i].id;                                              \
                                                                               \
      if (id == 0)                                                             \
        break;                                                                 \
      other = Actor_Get(id);                                                   \
      if (other->y == other->terrainHeight && call2(object, other)) {          \
        if (work->slots[i].blocked == 0) {                                     \
          object->priorityFlags = 2;                                           \
          object->z -= object->unk4c;                                          \
          object->gravity += object->unk4c >> 2;                               \
          object->unk4c = 0;                                                   \
        } else {                                                               \
          work->blocked = 1;                                                   \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }

/// An actor pops up out of the ground, holds and settles back.
#define DEFINE_ACTOR_POP_UP_SLOW(name, call1)                                  \
  s32 name(struct Actor *object) {                                             \
    s32 frame = object->timer;                                                 \
                                                                               \
    if (frame == 0) {                                                          \
      object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;    \
      object->scaleX = frame;                                                  \
      object->scaleY = frame;                                                  \
      object->speedLimit = 0x10000;                                            \
      object->acceleration = 0x10000;                                          \
      Actor_SetSpritePriorityDirect(object, 1);                                \
    } else if (frame <= 30) {                                                  \
      if (frame == 30) {                                                       \
        Event_PlaySound(136);                                                  \
        call1(object->watchedObject, 2);                                       \
      }                                                                        \
    } else if (frame <= 60) {                                                  \
      object->scaleX = 0x10000;                                                \
      object->scaleY = 0x10000;                                                \
      {                                                                        \
        s32 half = (object->timer / 2) & 1;                                    \
                                                                               \
        Actor_SetSpriteColorMode(object->watchedObject, half * 7);             \
      }                                                                        \
    } else {                                                                   \
      call1(object->watchedObject, 1);                                         \
      Event_PlaySound(184);                                                    \
      object->y = Map_GetTerrainHeight(0, object->x, object->z);               \
      object->watchedObject = 0;                                               \
      return 0;                                                                \
    }                                                                          \
    object->timer++;                                                           \
    return 1;                                                                  \
  }

/// Actor_PopUpSlow without the wait before it rises.
#define DEFINE_ACTOR_POP_UP(name, call1)                                       \
  s32 name(struct Actor *object) {                                             \
    s32 frame = object->timer;                                                 \
                                                                               \
    if (frame == 0) {                                                          \
      Event_PlaySound(136);                                                    \
      call1(object->watchedObject, 2);                                         \
      object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;    \
      object->scaleX = frame;                                                  \
      object->scaleY = frame;                                                  \
      object->speedLimit = 0x10000;                                            \
      object->acceleration = 0x8000;                                           \
      Actor_SetSpritePriorityDirect(object, 1);                                \
    } else if (frame <= 32) {                                                  \
      object->scaleX = 0x10000;                                                \
      object->scaleY = 0x10000;                                                \
      {                                                                        \
        s32 half = (object->timer / 2) & 1;                                    \
                                                                               \
        Actor_SetSpriteColorMode(object->watchedObject, half * 7);             \
      }                                                                        \
    } else {                                                                   \
      call1(object->watchedObject, 1);                                         \
      Event_PlaySound(184);                                                    \
      object->y = Map_GetTerrainHeight(0, object->x, object->z);               \
      object->watchedObject = 0;                                               \
      return 0;                                                                \
    }                                                                          \
    object->timer++;                                                           \
    return 1;                                                                  \
  }

/// The party takes 10 damage from a fall; who goes down is noted.
#define DEFINE_PARTY_TAKE_FALL_DAMAGE(name)                                    \
  void name(void) {                                                            \
    struct MarsLighthouseEventWork *work = Ram_HeapSlots->eventWork;           \
    s32 leader = FieldParty_GetLeaderActor();                                  \
    s32 count;                                                                 \
    s32 i;                                                                     \
    s16 hp[8];                                                                 \
                                                                               \
    (void *)Actor_Get(leader);                                                 \
    count = Party_CountMembers();                                              \
    for (i = 0; i < count; i++)                                                \
      hp[i] = Unit_Get(gMainRoutineWork.activeOwners[i])->hp;                  \
    Party_AddHp(-10, 0);                                                       \
    work->downCount = 0;                                                       \
    work->standingCount = 0;                                                   \
    Actor_SetAttachedEffect(leader, 258);                                      \
    for (i = 0; i < count; i++) {                                              \
      struct MainRoutineWork *main = &gMainRoutineWork;                        \
      struct BattleUnit *unit = Unit_Get(main->activeOwners[i]);               \
                                                                               \
      if (unit->hp > 0) {                                                      \
        work->standingCount++;                                                 \
      } else if (hp[i] != 0) {                                                 \
        work->downOwners[(s16)work->downCount++] = main->activeOwners[i];      \
        work->unk16a = 0xffff;                                                 \
        ((u8 *)unit)[0x131] = 0;                                               \
      }                                                                        \
    }                                                                          \
  }

/// Turns an actor toward the direction pad, at most a sixteenth of a turn a
/// frame.
#define DEFINE_ACTOR_STEER_BY_PAD(name, data1, call1)                          \
  void name(struct Actor *object) {                                            \
    s32 angle;                                                                 \
    s32 turn;                                                                  \
                                                                               \
    if (object->timer != 0) {                                                  \
      object->timer--;                                                         \
      return;                                                                  \
    }                                                                          \
    *(u8 *)((u8 *)object + 0x5a) = 0;                                          \
    if (Event_GetFlag(0x106))                                                  \
      angle = -1;                                                              \
    else                                                                       \
      angle = data1[(gInput.held >> 4) & 15];                                  \
    if (angle == -1) {                                                         \
      call1(object, 9);                                                        \
      return;                                                                  \
    }                                                                          \
    turn = (s16)(angle - object->facing);                                      \
    if (turn > 0x1000)                                                         \
      turn = 0x1000;                                                           \
    if (turn < -0x1000)                                                        \
      turn = -0x1000;                                                          \
    object->facing += turn;                                                    \
    call1(object, 2);                                                          \
    Actor_SetAnimationSpeed(object, 48);                                       \
  }

/// Launches an actor ahead of another, 32 pixels out.
#define DEFINE_ACTOR_LAUNCH_AHEAD(name, call1)                                 \
  void name(struct Actor *self) {                                              \
    struct Actor *actor = Actor_Get(((u8 *)self)[0x62]);                       \
    s32 angle = (s16)self->unk66;                                              \
    struct FieldSprite *sprite = actor->sprite;                                \
    /* volatile: the call reads position[0] before [1] and [2] (as a plain */  \
    /* array, 2 of 86 differ in tla-en). */                                    \
    volatile s32 position[3];                                                  \
                                                                               \
    position[0] = self->x;                                                     \
    position[1] = self->y - PIXELS(12);                                        \
    position[2] = self->z;                                                     \
    FieldVector_AddPolarOffset(PIXELS(32), angle, (struct Vector3 *)position); \
    actor->facing = angle;                                                     \
    actor->motionFlags = 0;                                                    \
    FieldActor_SetPositionAndResetMotion(actor, position[0], position[1],      \
                                         position[2]);                         \
    actor->update = (void (*)(void))call1;                                     \
    actor->actionFlags = 0;                                                    \
    actor->speedLimit = 0x80000;                                               \
    actor->acceleration = 0x2000;                                              \
    sprite->rotation = angle - 0x4000;                                         \
    actor->scaleX = 0;                                                         \
    /* A once-only block: its end holds the order around it (as the plain */   \
    /* statement, 4 of 86 differ in tla-en). */                                \
    do {                                                                       \
      actor->scaleY = 0;                                                       \
    } while (0);                                                               \
    actor->timer = 0;                                                          \
    actor->watchedObject = self->watchedObject;                                \
    Event_PlaySound(104);                                                      \
  }

/// A drifting object's update, stopping against the leader.
#define DEFINE_RIDE_UPDATE_DRIFT(name, call1)                                  \
  void name(struct Actor *object) {                                            \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 dx = object->bounce;                                                   \
    s32 dz;                                                                    \
    struct SpriteParts *parts;                                                 \
    s32 rate;                                                                  \
    s32 scale;                                                                 \
                                                                               \
    object->x += dx;                                                           \
    object->y += object->gravity;                                              \
    dz = object->unk4c;                                                        \
    object->z += dz;                                                           \
    object->bounce = dx - dx / 18;                                             \
    object->unk4c = dz - dz / 16;                                              \
    rate = object->speedLimit;                                                 \
    scale = object->scaleX;                                                    \
    parts = object->sprite;                                                    \
    object->scaleX = scale + rate;                                             \
    object->scaleY += object->acceleration;                                    \
    parts->unk12 += object->timer;                                             \
    if (leader->y == leader->terrainHeight && call1(object, leader)) {         \
      object->z -= object->unk4c;                                              \
      object->gravity += object->unk4c >> 2;                                   \
      object->unk4c = 0;                                                       \
    }                                                                          \
  }

/// Spawns a ride object moving back at `speed`.
#define DEFINE_RIDE_SPAWN_BACK(name, call1, call2, call3, call4)               \
  void name(s32 *position, s32 speed, s32 mode) {                              \
    struct Actor *object = call1(366, position[0], position[1], position[2]);  \
                                                                               \
    if (object != NULL) {                                                      \
      object->bounce = 0;                                                      \
      object->gravity = 0;                                                     \
      object->unk4c = -speed;                                                  \
      call2(object, 3);                                                        \
      object->update = call3;                                                  \
      if (mode)                                                                \
        object->unk5c[0] = 2;                                                  \
      call4(object, 2);                                                        \
    }                                                                          \
  }

/// Runs `call1`, then parks an actor out of the way.
#define DEFINE_ACTOR_PARK(name, call1)                                         \
  s32 name(struct Actor *object) {                                             \
    call1(object);                                                             \
    SET_POSITION(object, 0, 0, 0);                                             \
    SET_VELOCITY(object, 0, 0, 0);                                             \
    return 0;                                                                  \
  }

/// Steps the ride's counter, wrapping at `limit`.
#define DEFINE_RIDE_ADVANCE_COUNTER(name)                                      \
  s32 name(s32 limit) {                                                        \
    struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);   \
                                                                               \
    if (!Event_GetFlag(260)) {                                                 \
      if (++work->counter >= limit)                                            \
        work->counter = 0;                                                     \
    }                                                                          \
    return work->counter;                                                      \
  }

/// Spawns a spark thrown up at `speed`.
#define DEFINE_RIDE_SPAWN_SPARK(name, call1, call2, call3)                     \
  void name(s32 *position, s32 speed) {                                        \
    struct Actor *object =                                                     \
        call1(285, position[0], position[1], position[2] + 0xfffe0000);        \
    s32 spin;                                                                  \
                                                                               \
    if (object != NULL) {                                                      \
      object->bounce = Trig_Cos((u16)(Random16() * 3 >> 4) + 0x3000);          \
      object->gravity = (u16)Random16();                                       \
      object->unk4c = (u16)(Random16() << 1) + speed;                          \
      spin = (u16)Random16() + 0xffff8000;                                     \
      object->timer = spin;                                                    \
      call2(object, 2);                                                        \
      object->update = (void (*)(void))call3;                                  \
      Actor_SetBlendMode(object, 1);                                           \
    }                                                                          \
  }

/// Attaches an actor to another with a script and an update.
#define DEFINE_ACTOR_ATTACH(name, call1, call2)                                \
  void name(s32 id, s32 target, s32 kind, s32 a3, const void *script) {        \
    struct Actor *source = Actor_Get(id);                                      \
    struct Actor *object = Actor_Get(target);                                  \
                                                                               \
    FieldActor_SetPositionAndResetMotion(object, source->x, source->y,         \
                                         source->z);                           \
    Actor_StartScript(object, script);                                         \
    Actor_SetSpriteFlags(object, 0);                                           \
    object->motionFlags = 0;                                                   \
    call1(object, 1);                                                          \
    object->watchedObject = source;                                            \
    object->timer = 0;                                                         \
    object->unk66 = a3;                                                        \
    BYTE_AT(object, 98) = kind;                                                \
    object->update = call2;                                                    \
  }

/// Runs `call1` while the ride is active; 1 when it ran.
#define DEFINE_RIDE_IF_ACTIVE(name, call1)                                     \
  s32 name(void) {                                                             \
    if (((struct RideWork *)(gInventorySnapshot + 0x188))->active) {           \
      call1();                                                                 \
      return 1;                                                                \
    }                                                                          \
    return 0;                                                                  \
  }

/// Sets a ride entry's height and flags from a packed value.
#define DEFINE_RIDE_SET_ENTRY(name)                                            \
  void name(s32 index, s32 value) {                                            \
    struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);   \
    struct RideEntry *entry = &work->entries[index];                           \
                                                                               \
    entry->d = (value & 15) << 16;                                             \
    entry->b = value & 0xf0;                                                   \
  }

/// Sets a ride slot's value.
#define DEFINE_RIDE_SET_SLOT(name)                                             \
  void name(u32 index, s32 value) {                                            \
    struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);   \
                                                                               \
    if (index <= 3)                                                            \
      work->slots[index].value = value;                                        \
  }

/// Actor_SetupScripted as a two-argument callback.
#define DEFINE_ACTOR_SETUP_SCRIPTED_FOR(name, call1, data1)                    \
  void name(struct Actor *object, s32 unused) {                                \
    object->motionFlags = 0;                                                   \
    object->collisionFlags = 0;                                                \
    Actor_SetSpriteFlags(object, 0);                                           \
    call1(object, 2);                                                          \
    Actor_StartScript(object, data1);                                          \
    FieldActor_SetSpriteColor(object, 10);                                     \
    object->timer = 0;                                                         \
    object->speedLimit = 0;                                                    \
    object->acceleration = 0;                                                  \
  }

/// Stops an actor and starts its script, part 2 and colour 10.
#define DEFINE_ACTOR_SETUP_SCRIPTED(name, call1, data1)                        \
  void name(struct Actor *object) {                                            \
    object->motionFlags = 0;                                                   \
    object->collisionFlags = 0;                                                \
    Actor_SetSpriteFlags(object, 0);                                           \
    call1(object, 2);                                                          \
    Actor_StartScript(object, data1);                                          \
    FieldActor_SetSpriteColor(object, 10);                                     \
    object->timer = 0;                                                         \
    object->speedLimit = 0;                                                    \
    object->acceleration = 0;                                                  \
  }

/// 1 when two actors stand side by side.
#define DEFINE_ACTOR_IS_BESIDE(name)                                           \
  s32 name(struct Actor *a, struct Actor *b) {                                 \
    s32 dy;                                                                    \
                                                                               \
    if (ABS(a->x - b->x) < 0xc0000) {                                          \
      dy = a->y - b->y;                                                        \
      if (dy > -0x80000 && dy < 0x180000 && ABS(a->z - b->z) < 0xc0000)        \
        return 1;                                                              \
    }                                                                          \
    return 0;                                                                  \
  }

/// 1 when the first actor stands just above the second.
#define DEFINE_ACTOR_IS_ABOVE(name)                                            \
  s32 name(struct Actor *a, struct Actor *b) {                                 \
    s32 dy;                                                                    \
                                                                               \
    if (ABS(a->x - b->x) < 0xc0000) {                                          \
      dy = a->y - b->y;                                                        \
      if (dy > 0 && dy < 0x100000 && ABS(a->z - b->z) < 0x80000)               \
        return 1;                                                              \
    }                                                                          \
    return 0;                                                                  \
  }

/// Stops the leader while an action waits.
#define DEFINE_LEADER_STOP_IF_PENDING(name)                                    \
  void name(void) {                                                            \
    u8 *event = Ram_HeapSlots->eventWork;                                      \
                                                                               \
    if (S16_AT(event, 0xca2)) {                                                \
      Flag_Set(0x106);                                                         \
      Leader_StopMoving();                                                     \
      Event_ResolveAllPendingActions();                                        \
      Flag_Clear(0x106);                                                       \
    }                                                                          \
  }

/// Actor_ResetScripted as a two-argument callback.
#define DEFINE_ACTOR_RESET_SCRIPTED_FOR(name, data1)                           \
  void name(struct Actor *object, s32 unused) {                                \
    object->motionFlags = 0;                                                   \
    object->collisionFlags = 0;                                                \
    Actor_SetSpriteFlags(object, 0);                                           \
    Actor_StartScript(object, data1);                                          \
    object->timer = 0;                                                         \
    object->speedLimit = 0;                                                    \
    object->acceleration = 0;                                                  \
  }

/// Stops an actor and starts its script.
#define DEFINE_ACTOR_RESET_SCRIPTED(name, data1)                               \
  void name(struct Actor *object) {                                            \
    object->motionFlags = 0;                                                   \
    object->collisionFlags = 0;                                                \
    Actor_SetSpriteFlags(object, 0);                                           \
    Actor_StartScript(object, data1);                                          \
    object->timer = 0;                                                         \
    object->speedLimit = 0;                                                    \
    object->acceleration = 0;                                                  \
  }

/// 1 when two actors touch.
#define DEFINE_ACTOR_IS_TOUCHING(name)                                         \
  s32 name(struct Actor *a, struct Actor *b) {                                 \
    if (ABS(a->x - b->x) < 0x80000 && ABS(a->y - b->y) < 0x80000 &&            \
        ABS(a->z - b->z) < 0x80000)                                            \
      return 1;                                                                \
    return 0;                                                                  \
  }

/// Releases the ride's handle.
#define DEFINE_RIDE_RELEASE_HANDLE(name, call1)                                \
  void name(void) {                                                            \
    struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);   \
                                                                               \
    call1(work->handle);                                                       \
    work->handle = NULL;                                                       \
  }

/// Leaves by exit `n1` after `call1` and `call2`, then runs `call3`.
#define DEFINE_SCENE_LEAVE_BY_EXIT(name, call1, call2, n1, call3)              \
  void name(void) {                                                            \
    call1();                                                                   \
    call2();                                                                   \
    {                                                                          \
      u8 *party = (u8 *)&gMainRoutineWork;                                     \
                                                                               \
      party[0x2c2] = 2;                                                        \
    }                                                                          \
    FieldEvent_SetExitRequest(n1);                                             \
    FieldEvent_Wait(40);                                                       \
    call3();                                                                   \
  }

/// Spawns a ride object moving forward at `speed`.
#define DEFINE_RIDE_SPAWN_FRONT(name, call1, call2, n1, call3, call4)          \
  void name(s32 *position, s32 speed, s32 mode) {                              \
    struct Actor *object = call1(366, position[0], position[1], position[2]);  \
                                                                               \
    if (object != NULL) {                                                      \
      object->bounce = 0;                                                      \
      object->gravity = 0;                                                     \
      object->unk4c = speed;                                                   \
      call2(object, n1);                                                       \
      object->update = call3;                                                  \
      if (mode)                                                                \
        object->unk5c[0] = 2;                                                  \
      call4(object, 2);                                                        \
    }                                                                          \
  }

/// Blinks an actor's child between two values.
#define DEFINE_ACTOR_BLINK(name, n1, n2, n3)                                   \
  void name(struct Actor *object) {                                            \
    switch (gFrameCount & 7) {                                                 \
    case 0:                                                                    \
      Actor_SetSpriteColorMode(object, n1);                                    \
      break;                                                                   \
    case n2:                                                                   \
      Actor_SetSpriteColorMode(object, n3);                                    \
      break;                                                                   \
    }                                                                          \
  }

/// The leader slides cell by cell over ice until something stops it.
#define DEFINE_LEADER_SLIDE_ON_ICE(name, call1, data1, call2, call3, call4)    \
  void name(void) {                                                            \
    struct Vector3 probe;                                                      \
    s32 startBehavior;                                                         \
    s32 x;                                                                     \
    s32 z;                                                                     \
    s32 startX;                                                                \
    s32 startZ;                                                                \
    u8 *layer;                                                                 \
    s32 radius;                                                                \
    struct Actor *actor;                                                       \
    struct Vector3 *position;                                                  \
    struct Vector3 *next;                                                      \
    s32 heading;                                                               \
    s32 nextBehavior;                                                          \
    s32 distance;                                                              \
    struct Actor *other;                                                       \
    u8 *flags;                                                                 \
    s32 i;                                                                     \
    s32 dx;                                                                    \
    s32 dz;                                                                    \
                                                                               \
    actor = ObjectTable_Get(gMainRoutineWork.currentOwner);                    \
                                                                               \
  top:                                                                         \
    call1();                                                                   \
    actor->speedLimit = PIXELS(1);                                             \
    actor->acceleration = PIXELS(1) / 2;                                       \
    actor->targetX = 0x80000000;                                               \
    actor->targetZ = 0x80000000;                                               \
    actor->velocityX = 0;                                                      \
    actor->velocityZ = 0;                                                      \
    position = &probe;                                                         \
    position->x = startX = (actor->x & 0xfff00000) + PIXELS(8);                \
    position->y = actor->y;                                                    \
    position->z = startZ = (actor->z & 0xfff00000) + PIXELS(8);                \
    x = startX;                                                                \
    z = startZ;                                                                \
    layer = &actor->terrainId;                                                 \
    startBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, x, z);              \
    dz = z - actor->z;                                                         \
    dx = x - actor->x;                                                         \
    distance = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz);                    \
    if (distance < PIXELS(4)) {                                                \
      heading = data1[(gInput.held >> 4) & 15];                                \
      if ((heading << 16) == -0x10000) {                                       \
        return;                                                                \
      }                                                                        \
    } else {                                                                   \
      heading = (s16)ArcTan2(z - actor->z, x - actor->x);                      \
      heading = (s16)((heading + 0x2000) & -0x4000);                           \
    }                                                                          \
    FieldVector_AddPolarOffset(PIXELS(16), heading, &probe);                   \
    nextBehavior =                                                             \
        FieldMapGrid_GetCellBehaviorAt(*layer, position->x, position->z);      \
    if (nextBehavior == 255 ||                                                 \
        Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y >    \
            PIXELS(8)) {                                                       \
      actor->facing = heading;                                                 \
      actor->velocityX = 0;                                                    \
      actor->velocityZ = 0;                                                    \
      actor->x = startX;                                                       \
      actor->z = startZ;                                                       \
      goto done;                                                               \
    }                                                                          \
    position->x = startX;                                                      \
    position->z = startZ;                                                      \
    actor->speedLimit = PIXELS(2);                                             \
    actor->acceleration = 0x1999;                                              \
    actor->timer = 0;                                                          \
    FieldActor_SetMoveTarget(actor, startX, actor->y, startZ);                 \
    call2(actor, 2);                                                           \
    Actor_SetAnimationSpeed(actor, 48);                                        \
    FieldActor_WaitForMove(actor);                                             \
    actor->update = call3;                                                     \
    goto test;                                                                 \
  body:                                                                        \
    if (Map_GetTerrainHeight(*layer, next->x, next->z) - actor->y >            \
        PIXELS(8)) {                                                           \
      goto stop;                                                               \
    }                                                                          \
    radius = actor->radius - 2;                                                \
    other = Ram_HeapSlots->scriptObjects;                                      \
    flags = &other->collisionFlags;                                            \
    for (i = 0; i < 64; i++, flags += 128, other++) {                          \
      if (other->script != NULL && (*flags & 1) && other != actor) {           \
        /* FAKEMATCH(barrier): the empty asm clobbering sfp and r4 and reading \
         * next and actor; as no asm, 85 of 304 instructions differ in tla-en. \
         */                                                                    \
        asm(""                                                                 \
            :                                                                  \
            : "r"(next), "r"(next), "r"(actor), "r"(actor), "r"(actor),        \
              "r"(actor), "r"(actor), "r"(actor), "r"(actor), "r"(actor)       \
            : SFP_CLOBBER, "r4");                                              \
        if (call4(&other->x, radius, &probe, other->radius - 2) >= 0) {        \
          goto stop;                                                           \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    x = next->x;                                                               \
    z = next->z;                                                               \
    actor->speedLimit = PIXELS(2);                                             \
    actor->acceleration = 0x1999;                                              \
    FieldActor_SetMoveTarget(actor, next->x, next->y, next->z);                \
    FieldActor_WaitForMove(actor);                                             \
    if (nextBehavior != startBehavior) {                                       \
      goto blocked;                                                            \
    }                                                                          \
  test:                                                                        \
    {                                                                          \
      struct Vector3 *step;                                                    \
                                                                               \
      step = &probe;                                                           \
      FieldVector_AddPolarOffset(PIXELS(16), heading, step);                   \
      /* FAKEMATCH(dead): the unread store ends CSE's knowledge of the         \
       * address, so next rebuilds it; as no store, 59 of 304 instructions     \
       * differ in tla-en.                                                     \
       */                                                                      \
      step = NULL;                                                             \
    }                                                                          \
    next = &probe;                                                             \
    nextBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, next->x, next->z);   \
    if (nextBehavior != 255) {                                                 \
      goto body;                                                               \
    }                                                                          \
  stop:                                                                        \
    actor->speedLimit = PIXELS(2);                                             \
    actor->acceleration = PIXELS(1);                                           \
    FieldActor_SetMoveTarget(actor, x, actor->y, z);                           \
    FieldActor_WaitForMove(actor);                                             \
    System_Wait(2);                                                            \
    goto top;                                                                  \
                                                                               \
  blocked:                                                                     \
    System_Wait(10);                                                           \
  done:                                                                        \
    actor->update = NULL;                                                      \
    actor->actionFlags |= 1;                                                   \
    actor->acceleration = PIXELS(1) / 4;                                       \
    call2(actor, 1);                                                           \
  }

/// Leader_SlideOnIce in the direction the leader faces.
#define DEFINE_LEADER_SLIDE_ON_ICE_AHEAD(name, call1, call2, call3, call4)     \
  void name(void) {                                                            \
    struct Vector3 probe;                                                      \
    s32 heading;                                                               \
    s32 startBehavior;                                                         \
    s32 x;                                                                     \
    s32 z;                                                                     \
    s32 radius;                                                                \
    struct Actor *actor;                                                       \
    struct Vector3 *next;                                                      \
    s32 nextBehavior;                                                          \
    u8 *layer;                                                                 \
    struct Actor *other;                                                       \
    struct Vector3 *position;                                                  \
    u8 *flags;                                                                 \
    s32 i;                                                                     \
    s32 cell;                                                                  \
    s32 angle;                                                                 \
    s32 zero;                                                                  \
    struct Vector3 *step;                                                      \
    s32 startX;                                                                \
    s32 startZ;                                                                \
                                                                               \
    actor = ObjectTable_Get(gMainRoutineWork.currentOwner);                    \
    heading = (s16)((actor->facing + 0x2000) & -0x4000);                       \
                                                                               \
  top:                                                                         \
    call1();                                                                   \
    actor->speedLimit = PIXELS(1);                                             \
    actor->acceleration = PIXELS(1) / 2;                                       \
    actor->targetX = 0x80000000;                                               \
    actor->targetZ = 0x80000000;                                               \
    /* Default: one zero, spilled and reloaded at each use. */                 \
    zero = 0;                                                                  \
    actor->velocityX = zero;                                                   \
    actor->velocityZ = zero;                                                   \
    /* FAKEMATCH(temporary): cell holds the masked x; as one expression, 2 of  \
     * 246 instructions differ in tla-en. */                                   \
    cell = actor->x & 0xfff00000;                                              \
    i = PIXELS(8);                                                             \
    position = &probe;                                                         \
    position->x = startX = cell + i;                                           \
    position->y = actor->y;                                                    \
    position->z = startZ = (actor->z & 0xfff00000) + i;                        \
    x = startX;                                                                \
    z = startZ;                                                                \
    startBehavior = FieldMapGrid_GetCellBehaviorAt(                            \
        *(layer = &actor->terrainId), startX, startZ);                         \
    FieldVector_AddPolarOffset(PIXELS(16), heading, position);                 \
    nextBehavior =                                                             \
        FieldMapGrid_GetCellBehaviorAt(*layer, position->x, position->z);      \
    if (nextBehavior == 255 ||                                                 \
        Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y >    \
            i) {                                                               \
      actor->facing = heading;                                                 \
      actor->velocityX = 0;                                                    \
      actor->velocityZ = 0;                                                    \
      actor->x = startX;                                                       \
      actor->z = startZ;                                                       \
      goto done;                                                               \
    }                                                                          \
    position->x = x;                                                           \
    position->z = z;                                                           \
    actor->speedLimit = PIXELS(2);                                             \
    actor->acceleration = 0x1999;                                              \
    actor->timer = zero;                                                       \
    call2(actor, 2);                                                           \
    Actor_SetAnimationSpeed(actor, 48);                                        \
    actor->update = call3;                                                     \
    /* FAKEMATCH(barrier): the empty asm clobbering sfp and reading            \
     * nextBehavior twice; as no asm, 18 of 246 instructions differ in tla-en. \
     */                                                                        \
    asm("" : : "r"(nextBehavior), "r"(nextBehavior) : SFP_CLOBBER);            \
    goto test;                                                                 \
  body:                                                                        \
    if (Map_GetTerrainHeight(*layer, next->x, next->z) - actor->y >            \
        PIXELS(8)) {                                                           \
      goto stop;                                                               \
    }                                                                          \
    radius = actor->radius - 2;                                                \
    other = Ram_HeapSlots->scriptObjects;                                      \
    flags = &other->collisionFlags;                                            \
    for (i = 0; i < 64; i++, flags += 128, other++) {                          \
      if (other->script != NULL && (*flags & 1) && other != actor) {           \
        /* FAKEMATCH(barrier): the empty asm clobbering sfp and r4 and reading \
         * next and actor; as no asm, 170 of 246 instructions differ in        \
         * tla-en. */                                                          \
        asm(""                                                                 \
            :                                                                  \
            : "r"(next), "r"(next), "r"(actor), "r"(actor), "r"(actor),        \
              "r"(actor), "r"(actor), "r"(actor), "r"(actor), "r"(actor)       \
            : SFP_CLOBBER, "r4");                                              \
        if (call4(&other->x, radius, &probe, other->radius - 2) >= 0) {        \
          goto stop;                                                           \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    x = next->x;                                                               \
    z = next->z;                                                               \
    actor->speedLimit = PIXELS(2);                                             \
    actor->acceleration = 0x1999;                                              \
    FieldActor_SetMoveTarget(actor, next->x, next->y, next->z);                \
    FieldActor_WaitForMove(actor);                                             \
    if (nextBehavior != startBehavior) {                                       \
      goto blocked;                                                            \
    }                                                                          \
  test:                                                                        \
    /* FAKEMATCH(barrier): the asm tied to heading, with an r4 clobber so PRE  \
     * leaves it, loads the angle ahead of the address; as heading in the      \
     * call, 44 of 246 instructions differ in tla-en. */                       \
    asm("" : "=r"(angle) : "0"(heading) : "r4");                               \
    step = &probe;                                                             \
    FieldVector_AddPolarOffset(PIXELS(16), angle, step);                       \
    /* FAKEMATCH(dead): the unread store ends CSE's knowledge of the address,  \
     * so next rebuilds it; as &probe in the call, 97 of 246 instructions      \
     * differ in tla-en. */                                                    \
    step = NULL;                                                               \
    next = &probe;                                                             \
    nextBehavior = FieldMapGrid_GetCellBehaviorAt(*layer, next->x, next->z);   \
    if (nextBehavior != 255) {                                                 \
      goto body;                                                               \
    }                                                                          \
  stop:                                                                        \
    actor->speedLimit = PIXELS(2);                                             \
    actor->acceleration = PIXELS(1);                                           \
    FieldActor_SetMoveTarget(actor, x, actor->y, z);                           \
    FieldActor_WaitForMove(actor);                                             \
    System_Wait(2);                                                            \
    goto top;                                                                  \
                                                                               \
  blocked:                                                                     \
    System_Wait(10);                                                           \
  done:                                                                        \
    actor->update = NULL;                                                      \
    actor->actionFlags |= 1;                                                   \
    actor->acceleration = PIXELS(1) / 4;                                       \
    call2(actor, 1);                                                           \
  }

/// Draws the beam's targets, firing each in turn.
#define DEFINE_BEAM_DRAW_TARGETS(name, call1, call2, call3, call4, call5)      \
  void name(void) {                                                            \
    s32 saved[2];                                                              \
    State *state;                                                              \
    Entry *entry;                                                              \
    Camera *camera;                                                            \
    Position *position;                                                        \
    s32 cameraX, cameraZ;                                                      \
    s32 v;                                                                     \
    s32 i;                                                                     \
                                                                               \
    state = (State *)(gInventorySnapshot + 0x188);                             \
    camera = IWRAM->camera;                                                    \
    entry = state->entries;                                                    \
    position = &camera->position;                                              \
    cameraX = position->x;                                                     \
    cameraX &= 0xffff0000;                                                     \
    cameraZ = position->z & 0xffff0000;                                        \
    saved[1] = (*(s32 **)camera)[1];                                           \
    saved[0] = IWRAM->unk_6c.word;                                             \
    if (state->unk_6 != 0 && state->unk_8 == 0 && state->callback != 0)        \
      state->callback();                                                       \
    v = (u16)state->unk_6;                                                     \
    state->unk_8 = v;                                                          \
    state->unk_6 = 0;                                                          \
    for (i = 3; i >= 0; i--, entry++) {                                        \
      Actor *actor;                                                            \
      s32 flag, x, y, unk6, value, dx, dz;                                     \
      u8 *mark;                                                                \
                                                                               \
      if (entry->active == 0)                                                  \
        continue;                                                              \
      actor = entry->actor;                                                    \
      if (actor == 0)                                                          \
        continue;                                                              \
      flag = GameFlag_Get(0x106);                                              \
      if (*(s16 *)(saved[0] + 358) != 0)                                       \
        flag |= 1;                                                             \
      mark = &actor->flag5b;                                                   \
      *mark = flag;                                                            \
      if (state->unk_e != 0)                                                   \
        *mark = GameFlag_Get(0x106);                                           \
      dx = actor->x;                                                           \
      dz = actor->z;                                                           \
      dx -= cameraX;                                                           \
      /* FAKEMATCH(barrier): the empty asm on dx changes its allocation rank;  \
       * as no asm, 94 of 182 instructions differ in tla-en. */                \
      asm("" : : "r"(dx));                                                     \
      y = actor->y;                                                            \
      dz -= cameraZ;                                                           \
      y = dz - y;                                                              \
      unk6 = entry->unk_6;                                                     \
      x = dx >> 16;                                                            \
      y = (y >> 16) - 8;                                                       \
      if (unk6 == 0) {                                                         \
        if ((u32)(x + 7) > 334)                                                \
          continue;                                                            \
        if (y <= -48)                                                          \
          continue;                                                            \
        if (y > 239)                                                           \
          continue;                                                            \
      }                                                                        \
      y = (u16)entry->timer;                                                   \
      if (entry->timer <= 0) {                                                 \
        y = entry->flags & 240;                                                \
        switch (y) {                                                           \
        case 128:                                                              \
          call1(&entry->actor->x, entry->unk_c, unk6);                         \
          break;                                                               \
        case 48:                                                               \
          call2(&entry->actor->x, entry->unk_c, unk6);                         \
          break;                                                               \
        case 32:                                                               \
          call3(&entry->actor->x, entry->unk_c, unk6);                         \
          break;                                                               \
        case 16:                                                               \
          call4(&entry->actor->x, entry->unk_c, unk6);                         \
          break;                                                               \
        case 0:                                                                \
          call5(&entry->actor->x, entry->unk_c, unk6);                         \
          break;                                                               \
        }                                                                      \
        value = 8;                                                             \
      } else {                                                                 \
        value = y - 1;                                                         \
      }                                                                        \
      entry->timer = value;                                                    \
    }                                                                          \
  }

/// An actor rolls forward, crumbling the cells it passes.
#define DEFINE_ACTOR_ROLL_AND_CRUMBLE(name, id1, call1, call2, call3)          \
  void name(struct Actor *actor) {                                             \
    struct Vector3 position;                                                   \
    struct WorkEvent *work = Ram_HeapSlots->eventWork;                         \
    s32 state = actor->timer;                                                  \
                                                                               \
    if (state == 0) {                                                          \
      s32 dz;                                                                  \
      s32 dx;                                                                  \
      s32 tileX;                                                               \
      struct MapCell *cellsA;                                                  \
      struct MapCell *cellsB;                                                  \
      s32 widthA;                                                              \
      s32 widthB;                                                              \
      s32 offset;                                                              \
      s32 zB;                                                                  \
      s32 tileZ;                                                               \
      s32 heightA;                                                             \
      s32 heightB;                                                             \
      s32 x;                                                                   \
      s32 z;                                                                   \
      s32 xA;                                                                  \
      s32 xB;                                                                  \
      s32 zA;                                                                  \
      s32 flag;                                                                \
      struct MapWork *map;                                                     \
      struct MapCell *cell;                                                    \
      struct WorkEvent *work2;                                                 \
      struct id1 *other;                                                       \
      struct Actor *target;                                                    \
                                                                               \
      flag = Event_GetFlag(0x106);                                             \
      if (work->unk166 != 0)                                                   \
        flag |= 1;                                                             \
      actor->movementState = flag;                                             \
                                                                               \
      position.x = actor->x;                                                   \
      position.y = actor->y;                                                   \
      position.z = actor->z;                                                   \
      FieldVector_AddPolarOffset(0x80000, actor->facing, &position);           \
      if (call1(0, position.x, position.z) == 7) {                             \
        actor->targetZ = 0x80000000;                                           \
        actor->targetY = 0x80000000;                                           \
        actor->targetX = 0x80000000;                                           \
        actor->acceleration = 0x4000;                                          \
        actor->timer = 1;                                                      \
        Event_PlaySound(145);                                                  \
        /* FAKEMATCH(wrapper): forwarded, so each argument builds its own      \
         * 0x50000; as a direct call, 6 of 362 instructions differ in tla-en.  \
         */                                                                    \
        MapGrid_SetShake(0x50000, 0x50000, 0x10000);                           \
        /* FAKEMATCH(wrapper): forwarded, so each argument builds its own -1;  \
         * as a direct call, 17 of 362 instructions differ in tla-en. */       \
        MapGrid_SetShake(-1, -1, 0xe666);                                      \
        if (actor->watchedObject != NULL)                                      \
          ((void (*)(struct Actor *))actor->watchedObject)(actor);             \
      }                                                                        \
                                                                               \
      map = Ram_HeapSlots->mapWork;                                            \
      tileX = actor->x >> 20;                                                  \
      tileZ = actor->z >> 20;                                                  \
      cellsA = &map->layers[1].cells[tileX + (tileZ << 7)];                    \
      cellsB = &map->layers[2].cells[tileX + (tileZ << 7)];                    \
      widthA = *(s32 *)&map->layers[1].unk08[0] >> 20;                         \
      heightA = *(s32 *)&map->layers[1].unk08[4] >> 20;                        \
      widthB = *(s32 *)&map->layers[2].unk08[0] >> 20;                         \
      heightB = *(s32 *)&map->layers[2].unk08[4] >> 20;                        \
                                                                               \
      for (dz = -1, zB = tileZ + heightB - 2, z = tileZ - 1,                   \
          zA = tileZ + heightA - 1;                                            \
           dz <= 1; zB++, z++, zA++, dz++) {                                   \
        for (dx = -1, xB = tileX + widthB - 1, x = tileX - 1,                  \
            xA = tileX + widthA - 1;                                           \
             dx <= 1; xB++, x++, xA++, dx++) {                                 \
          offset = ((dz << 7) + dx) * 4;                                       \
          cell = (struct MapCell *)(offset + (s32)cellsA);                     \
          if (cell->collisionCode == 77) {                                     \
            cell->collisionCode = 0;                                           \
            MapGrid_CopyCellsLayered(64, 64, 1, 1, xA, zA);                    \
            MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);                    \
            Event_PlaySound(0x23d);                                            \
            call2((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,          \
                  actor->facing);                                              \
          }                                                                    \
          cell = (struct MapCell *)(offset + (s32)cellsB);                     \
          if (cell->collisionCode == 77) {                                     \
            cell->collisionCode = 0;                                           \
            MapGrid_CopyCellsLayered(64, 64, 1, 2, xB, zB);                    \
            MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);                    \
            Event_PlaySound(0x23c);                                            \
            call2((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,          \
                  actor->facing);                                              \
          }                                                                    \
        }                                                                      \
      }                                                                        \
                                                                               \
      if (actor->scaleX < 0x10000) {                                           \
        actor->scaleX += 0x1000;                                               \
        actor->scaleY += 0x1000;                                               \
      }                                                                        \
      FieldActor_SetMoveTarget(actor, position.x, position.y, position.z);     \
                                                                               \
      work2 = Ram_HeapSlots->eventWork;                                        \
      other = (struct id1 *)(gInventorySnapshot + 0x188);                      \
      target = Actor_Get(gMainRoutineWork.currentOwner);                       \
      if (ABS(actor->x - target->x) < 0x140000 &&                              \
          (u32)(actor->y - target->y + 0x7ffff) <= 0x1ffffe &&                 \
          ABS(actor->z - target->z) < 0x140000) {                              \
        other->unk04 = 2;                                                      \
        other->unk6c = actor;                                                  \
        work2->unk154 = other->unk0a + 2;                                      \
      }                                                                        \
    } else if (state == 1) {                                                   \
      actor->movementState = 0;                                                \
      if (actor->scaleX > 0) {                                                 \
        actor->scaleX -= 0x1000;                                               \
        actor->scaleY -= 0x1000;                                               \
        call3(actor);                                                          \
      } else {                                                                 \
        actor->z = 0;                                                          \
        actor->y = 0;                                                          \
        actor->x = 0;                                                          \
        actor->velocityZ = 0;                                                  \
        actor->velocityY = 0;                                                  \
        actor->velocityX = 0;                                                  \
        actor->update = NULL;                                                  \
      }                                                                        \
    }                                                                          \
  }

/// Two embers fly up from a point.
#define DEFINE_EMBERS_SPAWN(name, data1)                                       \
  void name(s32 x, s32 y, s32 z, s32 speed) {                                  \
    s32 i;                                                                     \
                                                                               \
    i = 1;                                                                     \
  next:                                                                        \
    {                                                                          \
      struct Actor *object = FieldActor_Create(325, x, y, z);                  \
                                                                               \
      if (object != NULL) {                                                    \
        s32 angle;                                                             \
        s32 rise;                                                              \
        s32 radius;                                                            \
        u32 size;                                                              \
        s32 scale;                                                             \
                                                                               \
        rise = speed + (Random16() >> 4) - (Random16() >> 4);                  \
        angle = 0x4000 + rise;                                                 \
        radius = Random16() * 8;                                               \
        size = Random16();                                                     \
        Actor_StartScript(object, data1);                                      \
        Actor_SetSpriteFlags(object, 0);                                       \
        /* FAKEMATCH(temporary): the scale is taken before the velocity        \
         * stores, as in the native; as object->scaleX = (size >> 1) + 0x4000, \
         * 14 of 98 instructions differ in tla-en. */                          \
        scale = (size >> 1) + 0x4000;                                          \
        object->velocityY = rise + 0x14000;                                    \
        object->velocityZ = Iwram_MulQ16(radius, Trig_Cos(angle));             \
        object->velocityX = Iwram_MulQ16(radius, Math_Sin(angle));             \
        object->acceleration = 0;                                              \
        object->gravity = 0x547a;                                              \
        object->scaleX = scale;                                                \
        object->scaleY = object->scaleX;                                       \
        object->bounce = 0x8000;                                               \
        Actor_SetSpriteColorMode(object, 1);                                   \
      }                                                                        \
    }                                                                          \
    if (--i >= 0)                                                              \
      goto next;                                                               \
  }

/// Stops an actor and starts its script with sprite part `part`.
#define DEFINE_ACTOR_SETUP_SCRIPTED_PART(name, data1)                          \
  void name(struct Actor *object, s32 part) {                                  \
    s32 zero = 0;                                                              \
                                                                               \
    object->motionFlags = zero;                                                \
    object->collisionFlags = zero;                                             \
                                                                               \
    ((struct SpriteParts *)object->sprite)->part = part;                       \
    Actor_SetSpriteFlags(object, 0);                                           \
    Actor_StartScript(object, data1);                                          \
    object->timer = zero;                                                      \
    object->speedLimit = zero;                                                 \
    object->acceleration = zero;                                               \
  }

#endif

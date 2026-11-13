/// @file
/// The vents of Aqua Rock and Taopo Swamp: each puffs in turn on a counter.
/// Each routine is a macro a scene expands where its copy sits, with the
/// names of its own data and calls.
#ifndef ALCHEMY_SCENE_SHARED_VENTS_H
#define ALCHEMY_SCENE_SHARED_VENTS_H

/// Steps the vents: each active one puffs every ten frames.
/// 2 copies: aqua_rock_inside/s3844, taopo_swamp.
#define DEFINE_VENTS_UPDATE(name, call1)                                       \
  void name(void) {                                                            \
    struct VentWork *work = VENT_WORK;                                         \
    struct VentEntry *entry = work->entries;                                   \
    s32 i;                                                                     \
                                                                               \
    work->hook();                                                              \
    for (i = 15; i >= 0; i--, entry++) {                                       \
      if (entry->active != 0) {                                                \
        if (entry->timer <= 0) {                                               \
          call1(&entry->x, entry->value);                                      \
          entry->timer = 9;                                                    \
        } else {                                                               \
          entry->timer--;                                                      \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }

/// Stops an actor and starts its scripted puff.
/// 2 copies: aqua_rock_inside/s3844, taopo_swamp.
#define DEFINE_ACTOR_START_SCRIPTED(name, call1, data1)                        \
  void name(struct Actor *object, s32 priority) {                              \
    object->motionFlags = 0;                                                   \
    object->collisionFlags = 0;                                                \
    object->priorityFlags = 2;                                                 \
    ((struct FieldSprite *)object->sprite)->priority = priority;               \
    Actor_SetSingleChildFlags(object, 0);                                      \
    call1(object, 2);                                                          \
    Actor_StartScript(object, data1);                                          \
    FieldActor_SetSpriteColor(object, 10);                                          \
    object->timer = 0;                                                         \
    object->speedLimit = 0;                                                    \
    object->acceleration = 0;                                                  \
  }

/// Steps the vents' counter, wrapping at `limit`.
/// 2 copies: aqua_rock_inside/s3844, taopo_swamp.
#define DEFINE_VENTS_ADVANCE_COUNTER(name)                                     \
  s32 name(s32 limit) {                                                        \
    struct VentWork *work = VENT_WORK;                                         \
                                                                               \
    if (Event_GetFlag(0x104) == 0) {                                           \
      if (++work->counter >= limit)                                            \
        work->counter = 0;                                                     \
    }                                                                          \
    return work->counter;                                                      \
  }

/// Sets a vent's actor.
/// 2 copies: aqua_rock_inside/s3844, taopo_swamp.
#define DEFINE_VENTS_SET_ID(name)                                              \
  void name(s32 i, s32 id) {                                                   \
    struct VentWork *work = VENT_WORK;                                         \
                                                                               \
    if ((u32)i <= 3)                                                           \
      work->ids[i] = id;                                                       \
  }

#endif

/// @file
/// Small actor routines several scenes copy: distances, creating an actor,
/// facing and hiding.
/// Each routine is a macro a scene expands where its copy sits, with the
/// names of its own data and calls.
#ifndef ALCHEMY_SCENE_SHARED_ACTORS_H
#define ALCHEMY_SCENE_SHARED_ACTORS_H

/// The distance in whole pixels between two 16.16 positions, across the ground
/// only.
#define DEFINE_POSITION_FLAT_DISTANCE(name)                                    \
  s32 name(s32 *a, s32 *b) {                                                   \
    s32 dx = (a[0] - b[0]) >> 16;                                              \
    s32 dz = (a[2] - b[2]) >> 16;                                              \
                                                                               \
    return Iwram_Sqrt(dx * dx + dz * dz);                                      \
  }

/// The number of entries in a list ended by 0xffff, less one.
#define DEFINE_LIST_COUNT_ENTRIES(name)                                        \
  s32 name(u16 *list) {                                                        \
    s32 count = 0;                                                             \
                                                                               \
    while (*list != 0xffff) {                                                  \
      list++;                                                                  \
      count++;                                                                 \
    }                                                                          \
    count--;                                                                   \
    return count;                                                              \
  }

/// Creates an actor of `kind` at a point, standing still, drawn over the room.
#define DEFINE_SCENE_CREATE_ACTOR(name)                                        \
  struct Actor *name(s32 a0, s32 a1, s32 a2, s32 a3) {                         \
    struct Actor *t0;                                                          \
                                                                               \
    t0 = FieldActor_Create(a3, a0, a1, a2);                                    \
    if (t0 != 0) {                                                             \
      Scene_SetActorSpritePriority(t0, 1);                                     \
      t0->motionFlags = 0;                                                     \
      t0->collisionFlags = 8;                                                  \
      Actor_SetSpriteFlags(t0, 0);                                             \
      FieldActor_SetSpriteColor(t0, 0xf);                                      \
      t0->priorityFlags = 0x22 | t0->priorityFlags;                            \
      return t0;                                                               \
    }                                                                          \
    return 0;                                                                  \
  }

/// Resets a tile sprite to the map's tile, frame 0.
#define DEFINE_TILE_SPRITE_RESET(name)                                         \
  s32 name(struct Actor *object) {                                             \
    u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);                   \
    struct TileSprite *sprite = object->sprite;                                \
                                                                               \
    sprite->tile = *tile;                                                      \
    sprite->frame = 0;                                                         \
    return 1;                                                                  \
  }

/// Steps an actor's pictures through a four-frame cycle.
#define DEFINE_ACTOR_CYCLE_PICTURES(name)                                      \
  void name(struct Actor *object) {                                            \
    struct AnimatedSprite *sprite = (struct AnimatedSprite *)object->sprite;   \
    struct SpritePictures *pictures = sprite->pictures;                        \
                                                                               \
    switch (object->timer & 3) {                                               \
    case 0:                                                                    \
      pictures->picture = 7;                                                   \
      sprite->picture = 1;                                                     \
      sprite->nextPicture = 2;                                                 \
      break;                                                                   \
    case 1:                                                                    \
      sprite->picture = 1;                                                     \
      sprite->nextPicture = 0;                                                 \
      break;                                                                   \
    case 2:                                                                    \
      pictures->picture = 7;                                                   \
      sprite->picture = 1;                                                     \
      sprite->nextPicture = 0;                                                 \
      break;                                                                   \
    case 3:                                                                    \
      pictures->picture = 0;                                                   \
      sprite->picture = 1;                                                     \
      sprite->nextPicture = 0;                                                 \
      break;                                                                   \
    }                                                                          \
    object->timer++;                                                           \
  }

/// The distance in whole pixels between two 16.16 positions of x, y and z.
#define DEFINE_POSITION_DISTANCE(name)                                         \
  s32 name(s32 *a, s32 *b) {                                                   \
    s32 dx = (*a++ - *b++) >> 16;                                              \
    s32 dy = (*a++ - *b++) >> 16;                                              \
    s32 dz = (*a - *b) >> 16;                                                  \
    s32 dxsq = dx * dx;                                                        \
    s32 dysq = dy * dy;                                                        \
    s32 dzsq = dz * dz;                                                        \
                                                                               \
    return Iwram_Sqrt(dxsq + dysq + dzsq);                                     \
  }

/// 1 when `other` is within `range` of `self` and `self` faces it, within one
/// eighth of a turn.
#define DEFINE_ACTOR_IS_FACING_NEAR(name, call1)                               \
  s32 name(struct Actor *self, struct Actor *other, s32 range) {               \
    s32 result = 0;                                                            \
    s32 *to;                                                                   \
    s32 *from;                                                                 \
                                                                               \
    to = &other->x;                                                            \
    from = &self->x;                                                           \
    if (call1(to, from) < range) {                                             \
      u32 angle = (u16)ArcTan2(other->z - self->z, *to - *from);               \
      u32 left = (angle - 0x1000) & 0xf000;                                    \
      u32 right = (angle + 0x1000) & 0xf000;                                   \
      u32 ahead = angle & 0xf000;                                              \
      u32 facing = self->facing & 0xf000;                                      \
                                                                               \
      if (ahead == facing || right == facing || left == facing)                \
        result = 1;                                                            \
    }                                                                          \
    return result;                                                             \
  }

/// Hides an actor behind the room.
#define DEFINE_ACTOR_HIDE(name, call1)                                         \
  void name(s32 id) {                                                          \
    call1(id);                                                                 \
    Actor_SetSpriteColor(Actor_GetById(id), 15);                               \
    Actor_SetSpritePriority(id, 1);                                            \
    Actor_GetById(id)->priorityFlags |= 2;                                     \
  }

/// Lifts the leader by `height` and has the camera hold still, unless the party
/// keeps its place.
#define DEFINE_LEADER_LIFT(name)                                               \
  void name(s32 height) {                                                      \
    struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);           \
                                                                               \
    if (!Event_GetFlag(0x109)) {                                               \
      object->motionFlags = 0;                                                 \
      Actor_SetSpriteFlags(object, 0);                                         \
      object->y += height;                                                     \
      object->speedLimit = 0x10000;                                            \
      object->velocityY = 0;                                                   \
      gMainRoutineWork.renderMode = 2;                                         \
      object->actionFlags = 1;                                                 \
      EventCamera_FollowActor(gMainRoutineWork.currentOwner, 0);               \
      Map_CenterOnFollowedPoint();                                             \
      System_Wait(1);                                                          \
    }                                                                          \
  }

/// Bobs an actor up and down, rocking its sprite.
#define DEFINE_ACTOR_BOB(name)                                                 \
  s32 name(struct Actor *object) {                                             \
    s32 step = object->riseCounter & 63;                                       \
    struct FieldSprite *sprite = object->sprite;                               \
                                                                               \
    object->motionFlags = 3;                                                   \
    if (step == 0)                                                             \
      object->velocityY = PIXELS(3);                                           \
    if (step == 16)                                                            \
      object->velocityY = PIXELS(2);                                           \
    if (step == 24)                                                            \
      object->velocityY = PIXELS(3);                                           \
    sprite->rotation = Trig_Cos(step << 12) / 64;                              \
    object->riseCounter++;                                                     \
    return -1;                                                                 \
  }

/// Lets an actor fall until it lands, at most a second, then stops it.
#define DEFINE_ACTOR_WAIT_TO_LAND(name)                                        \
  void name(s32 id) {                                                          \
    s32 n = 60;                                                                \
    struct Actor *object = Actor_Get(id);                                      \
                                                                               \
    object->motionFlags = 3;                                                   \
    while (n != 0) {                                                           \
      System_Wait(1);                                                          \
      if (object->velocityY == 0)                                              \
        break;                                                                 \
      n--;                                                                     \
    }                                                                          \
    System_Wait(10);                                                           \
    object->motionFlags = 0;                                                   \
  }

/// Runs `call1`, then raises the held actor above the room.
#define DEFINE_SCENE_RAISE_HELD_ACTOR(name, call1, data1)                      \
  void name(void) {                                                            \
    u8 *event = Ram_HeapSlots->eventWork;                                      \
    struct Actor *object;                                                      \
    s32 flags;                                                                 \
                                                                               \
    call1(data1);                                                              \
    object = *(struct Actor **)(event + 0xcbc);                                \
    flags = 1;                                                                 \
    flags |= object->priorityFlags;                                            \
    flags &= 0xfd;                                                             \
    object->priorityFlags = flags;                                             \
  }

/// Shows or hides `count` actors from `first`.
#define DEFINE_ACTORS_SET_HIDDEN(name)                                         \
  void name(s32 show, s32 first, u32 count) {                                  \
    u32 i;                                                                     \
                                                                               \
    for (i = 0; i < count; i++) {                                              \
      struct Actor *actor = Actor_GetById(first + i);                          \
                                                                               \
      if (show)                                                                \
        actor->priorityFlags &= ~0x10;                                         \
      else                                                                     \
        actor->priorityFlags |= 0x10;                                          \
    }                                                                          \
  }

/// Lets an actor fall to the ground, with the landing sound.
#define DEFINE_ACTOR_DROP_AND_WAIT(name)                                       \
  void name(struct Actor *object) {                                            \
    u8 *flags = &object->motionFlags;                                          \
                                                                               \
    *flags = 3;                                                                \
    do {                                                                       \
      wait(1);                                                                 \
    } while (object->velocityY != 0);                                          \
    Sound_Play(SE_THUMP);                                                      \
    wait(10);                                                                  \
    *flags = 0;                                                                \
  }

/// 1 when the leader faces north, within one eighth of a turn.
#define DEFINE_LEADER_IS_FACING_NORTH(name)                                    \
  s32 name(void) {                                                             \
    u16 turn;                                                                  \
    s32 facing;                                                                \
                                                                               \
    turn = Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x5fff;      \
    facing = 1;                                                                \
    if (turn >= 0x3fff)                                                        \
      facing = 0;                                                              \
    return facing;                                                             \
  }

/// Stops an actor where it stands.
#define DEFINE_ACTOR_FREEZE(name)                                              \
  void name(s32 id) {                                                          \
    struct Actor *object = Actor_Get(id);                                      \
                                                                               \
    if (object != NULL) {                                                      \
      Actor_SetSpriteFlags(object, 0);                                         \
      object->collisionFlags = 0;                                              \
      object->motionFlags = 0;                                                 \
    }                                                                          \
  }

/// The distance in whole pixels between two 16.16 positions of x and y, the
/// second raised 6 pixels.
#define DEFINE_POSITION_DISTANCE_ABOVE(name)                                   \
  s32 name(s32 *a, s32 *b) {                                                   \
    s32 dx = (*a++ - *b++) >> 16;                                              \
    s32 dy = (*a - *b + 0x60000) >> 16;                                        \
                                                                               \
    return Iwram_Sqrt(dx * dx + dy * dy);                                      \
  }

/// Fades the screen in and clears flag 0x12f.
#define DEFINE_SCENE_FADE_IN_AND_CLEAR_FLAG(name)                              \
  void name(void) {                                                            \
    struct EventWork *work = Ram_HeapSlots->eventWork;                         \
                                                                               \
    work->screenEffect = 0x209;                                                \
    GameFlag_Clear(0x12f);                                                     \
  }

/// "The entrance is locked" (MsgShamanEntranceLocked).
#define DEFINE_SCENE_SHOW_ENTRANCE_LOCKED(name)                                \
  void name(void) {                                                            \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    FieldMessage_Show((s32)MsgShamanEntranceLocked, 1);                        \
    Event_End();                                                               \
  }

/// Scene_CreateActor through the imported creator, with extra priority flags.
#define DEFINE_SCENE_CREATE_IMPORTED_ACTOR(name, n1)                           \
  struct Actor *name(s32 x, s32 y, s32 z, s32 type) {                          \
    struct Actor *object = Import_ActorCreate(type, x, y, z);                  \
                                                                               \
    if (object != NULL) {                                                      \
      Scene_SetActorSpritePriority(object, 1);                                 \
      object->motionFlags = 0;                                                 \
      object->collisionFlags = 8;                                              \
      Actor_SetSpriteFlags(object, 0);                                         \
      Actor_SetSpriteColor(object, 15);                                        \
      object->priorityFlags |= n1;                                             \
      return object;                                                           \
    }                                                                          \
    return NULL;                                                               \
  }

/// Runs `call1` for each id from `n2` to `n2 + n1`, then `call2`.
#define DEFINE_ACTORS_FOR_RANGE(name, n1, call1, n2, call2)                    \
  void name(void) {                                                            \
    u32 i;                                                                     \
                                                                               \
    for (i = 0; i <= n1; i++)                                                  \
      call1(i + n2);                                                           \
    call2();                                                                   \
  }

/// Runs `call1` unless flag `n1` is set, setting it when the party keeps its
/// place.
#define DEFINE_SCENE_RUN_UNLESS_FLAG(name, n1, call1)                          \
  void name(void) {                                                            \
    if (GameFlag_Get(265)) {                                                   \
      if (GameFlag_Get(n1) == 0)                                               \
        call1();                                                               \
    } else {                                                                   \
      GameFlag_Set(n1);                                                        \
    }                                                                          \
  }

/// Puts the leader on map layer `n1`.
#define DEFINE_LEADER_SET_LAYER(name, n1)                                      \
  void name(void) {                                                            \
    struct Actor *object0;                                                     \
                                                                               \
    object0 = Actor_Get(gMainRoutineWork.currentOwner);                        \
    object0->terrainId = n1;                                                   \
    return;                                                                    \
  }

/// Puts the leader on map layer `n1`, through its byte.
#define DEFINE_LEADER_SET_LAYER_BYTE(name, n1)                                 \
  void name(void) {                                                            \
    s32 t0;                                                                    \
                                                                               \
    t0 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));                      \
    FIELD_AT((t0 + 0x22), u8, 0) = n1;                                         \
  }

/// Flickers an actor's child between `n1` and 0.
#define DEFINE_ACTOR_FLICKER_CHILD(name, n1)                                   \
  void name(struct Actor *object) {                                            \
    if (gFrameCount & 2)                                                       \
      Actor_SetSpriteColorMode(object, n1);                                    \
    else                                                                       \
      Actor_SetSpriteColorMode(object, 0);                                     \
  }

/// Flashes an actor in colour `n1` on the frames its mask picks.
#define DEFINE_ACTOR_FLASH_COLOR(name, n1)                                     \
  void name(struct Actor *object) {                                            \
    if (gFrameCount & object->unk5c[7])                                        \
      FieldActor_SetSpriteColor(object, n1);                                   \
    else                                                                       \
      FieldActor_SetSpriteColor(object, 0);                                    \
  }

/// Turns actor 4, then runs `call1`.
#define DEFINE_SCENE_TURN_ACTOR_AND_RUN(name, call1)                           \
  void name(void) {                                                            \
    s32 id, a;                                                                 \
                                                                               \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    a = 192;                                                                   \
    id = 4;                                                                    \
    /* FAKEMATCH(barrier): the id held at an empty asm. */                     \
    __asm__ volatile("" : "+r"(id));                                           \
    Actor_ArmCallback(id, a << 8, 0);                                          \
    call1();                                                                   \
  }

#endif

/// @file
/// The whirlwinds the stone faces of Jupiter Lighthouse blow out, which catch
/// the leader and carry it.
/// Each routine is a macro a scene expands where its copy sits, with the
/// names of its own data and calls.
#ifndef ALCHEMY_SCENE_SHARED_WHIRLWIND_H
#define ALCHEMY_SCENE_SHARED_WHIRLWIND_H

/// The leader spins in the whirlwind that caught it, then drops beside it and
/// the camera follows again.
#define DEFINE_WHIRLWIND_CARRY_LEADER(name)                                    \
  void name(void) {                                                            \
    struct EventWork *work = Ram_HeapSlots->eventWork;                         \
    struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);            \
    struct Actor *linked = actor->watchedObject;                               \
    struct Actor *shadow;                                                      \
    u8 *p;                                                                     \
    s32 angle;                                                                 \
    s32 i;                                                                     \
                                                                               \
    Event_Start();                                                             \
    FieldEvent_ResolvePendingActions(0);                                       \
    i = 0;                                                                     \
    work->unkd46 = i;                                                          \
                                                                               \
    actor->motionFlags = 4;                                                    \
    Actor_SetSpriteFlags(actor, 0);                                            \
                                                                               \
    while (*((u8 *)linked + 0x63) != 0) {                                      \
      actor->x = linked->x;                                                    \
      actor->y = linked->y + i;                                                \
      actor->z = linked->z;                                                    \
      if (i <= 0x3ffff)                                                        \
        i += 0x1999;                                                           \
      actor->facing = (u8)gFrameCount << 12;                                   \
      p = &actor->priorityFlags;                                               \
      *p ^= 2 << (gFrameCount & 1);                                            \
      System_Wait(1);                                                          \
    }                                                                          \
    work->unkd44 = 1;                                                          \
    work->unkd46 = 1;                                                          \
    angle = linked->facing;                                                    \
    actor->x = (((linked->x >> 19) & ~1) + (angle >> 13) - 1) << 19;           \
    actor->z = (((linked->z >> 19) & ~1) - ((angle >> 13) & 2) + 1) << 19;     \
    actor->velocityY = 0x60000;                                                \
    actor->priorityFlags = 1;                                                  \
    actor->motionFlags = 3;                                                    \
    Actor_SetSpriteFlags(actor, 1);                                            \
    FieldCamera_MoveTo(-1, -1, -1, 0);                                         \
    shadow = EventCamera_GetActor();                                           \
    shadow->speedLimit = 0x80000;                                              \
    shadow->acceleration = 0x10000;                                            \
    shadow->motionFlags = 0;                                                   \
    FieldActor_SetMoveTarget(shadow, actor->x,                                 \
                             Map_GetTerrainHeight(0, actor->x, actor->z),      \
                             actor->z);                                        \
    i = 0;                                                                     \
    if (actor->y > actor->terrainHeight + 0x100000) {                          \
      do {                                                                     \
        System_Wait(1);                                                        \
        i++;                                                                   \
        if (i > 59)                                                            \
          break;                                                               \
      } while (actor->y > actor->terrainHeight + 0x100000);                    \
    }                                                                          \
    Event_PlaySound(SE_LANDING_THUD);                                          \
    i = 0;                                                                     \
    if (actor->velocityY != 0) {                                               \
      do {                                                                     \
        System_Wait(1);                                                        \
        i++;                                                                   \
        if (i > 59)                                                            \
          break;                                                               \
      } while (actor->velocityY != 0);                                         \
    }                                                                          \
    FieldActor_WaitForMove(shadow);                                            \
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;              \
    EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);                 \
    work->unkd46 = 1;                                                          \
    work->exit = 0;                                                            \
    Event_End();                                                               \
  }

/// Whirlwind_CarryLeader as the lower floors' copies compile it.
#define DEFINE_WHIRLWIND_CARRY_LEADER_LOWER(name)                              \
  void name(void) {                                                            \
    struct EventWork *work = Ram_HeapSlots->eventWork;                         \
    struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);            \
    struct Actor *linked = actor->watchedObject;                               \
    struct Actor *shadow;                                                      \
    s32 facing;                                                                \
    s32 i;                                                                     \
                                                                               \
    Event_Start();                                                             \
    FieldEvent_ResolvePendingActions(0);                                       \
    i = 0;                                                                     \
    work->unkd46 = 0;                                                          \
    actor->motionFlags = 4;                                                    \
    Actor_SetSpriteFlags(actor, 0);                                            \
    while (*((u8 *)linked + 0x63) != 0) {                                      \
      actor->x = linked->x;                                                    \
      actor->y = linked->y + i;                                                \
      actor->z = linked->z;                                                    \
      if (i <= 0x3ffff)                                                        \
        i += 0x1999;                                                           \
      actor->facing = (u8)gFrameCount << 12;                                   \
      actor->priorityFlags ^= 2 << (gFrameCount & 1);                          \
      System_Wait(1);                                                          \
    }                                                                          \
    work->unkd44 = 1;                                                          \
    work->unkd46 = 1;                                                          \
    facing = linked->facing;                                                   \
    actor->x = (((linked->x >> 19) & ~1) + (facing >> 13) - 1) << 19;          \
    actor->z = (((linked->z >> 19) & ~1) - ((facing >> 13) & 2) + 1) << 19;    \
    actor->velocityY = 0x60000;                                                \
    actor->priorityFlags = 1;                                                  \
    actor->motionFlags = 3;                                                    \
    Actor_SetSpriteFlags(actor, 1);                                            \
    FieldCamera_MoveTo(-1, -1, -1, 0);                                         \
    shadow = EventCamera_GetActor();                                           \
    shadow->speedLimit = 0x80000;                                              \
    shadow->acceleration = 0x10000;                                            \
    shadow->motionFlags = 0;                                                   \
    FieldActor_SetMoveTarget(shadow, actor->x,                                 \
                             Map_GetTerrainHeight(0, actor->x, actor->z),      \
                             actor->z);                                        \
    i = 0;                                                                     \
    while (i <= 59 && actor->y > actor->terrainHeight + 0x100000) {            \
      i++;                                                                     \
      System_Wait(1);                                                          \
    }                                                                          \
    Event_PlaySound(SE_LANDING_THUD);                                          \
    i = 0;                                                                     \
    while (i <= 59 && actor->velocityY != 0) {                                 \
      i++;                                                                     \
      System_Wait(1);                                                          \
    }                                                                          \
    FieldActor_WaitForMove(shadow);                                            \
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;              \
    EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);                 \
    work->unkd46 = 1;                                                          \
    work->exit = 0;                                                            \
    Event_End();                                                               \
  }

/// A whirlwind blows out of the stone face at `source`.
#define DEFINE_WHIRLWIND_SPAWN(name, call1, call2, call3)                      \
  struct GustObject *name(struct GustObject *source) {                         \
    struct Vector3 position;                                                   \
    struct EffectOptions options;                                              \
    struct GustObject *object;                                                 \
                                                                               \
    Event_PlaySound(SE_222);                                                   \
    position.x = source->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,          \
                                    Trig_Cos(source->angle));                  \
    position.z = source->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,          \
                                    Math_Sin(source->angle));                  \
    object = ((struct GustObject *)FieldActor_Create(280, position.x,          \
                                                     source->y, position.z));  \
    call1(object, 2);                                                          \
    Actor_SetSpriteFlags(object, 0);                                           \
    object->motionFlags = 0;                                                   \
    object->priorityFlags &= ~1;                                               \
    object->angle = source->angle;                                             \
    object->unk64 = 0;                                                         \
    object->unk62 = 0;                                                         \
    object->unk63 = 0;                                                         \
    object->update = call2;                                                    \
    object->speedLimit = 0x18000;                                              \
    options.priority = 1;                                                      \
    options.palette = 7;                                                       \
    call3(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,       \
          &options);                                                           \
    return object;                                                             \
  }

/// Whirlwind_Spawn as the upper floors' copies compile it.
#define DEFINE_WHIRLWIND_SPAWN_UPPER(name, call1, call2, call3)                \
  struct GustObject *name(struct GustObject *source) {                         \
    struct Vector3 position;                                                   \
    struct EffectOptions options;                                              \
    struct GustObject *object;                                                 \
                                                                               \
    Event_PlaySound(SE_222);                                                   \
    position.x = source->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,          \
                                    Trig_Cos(source->angle));                  \
    position.z = source->z + Iwram_MulQ16(0x80000, Math_Sin(source->angle));   \
    object = ((struct GustObject *)FieldActor_Create(280, position.x,          \
                                                     source->y, position.z));  \
    call1(object, 2);                                                          \
    Actor_SetSpriteFlags(object, 0);                                           \
    object->motionFlags = 0;                                                   \
    object->priorityFlags &= ~1;                                               \
    object->angle = source->angle;                                             \
    object->unk64 = 0;                                                         \
    object->unk62 = 0;                                                         \
    object->unk63 = 0;                                                         \
    object->update = call2;                                                    \
    object->speedLimit = 0x18000;                                              \
    options.priority = 1;                                                      \
    options.palette = 7;                                                       \
    call3(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,       \
          &options);                                                           \
    return object;                                                             \
  }

/// A whirlwind's update: it travels forward, catches the leader it reaches and
/// ends off screen or at a wall.
#define DEFINE_WHIRLWIND_UPDATE(name, call1, call2, data1, data2)              \
  void name(struct Actor *self) {                                              \
    u8 *work = Ram_HeapSlots->eventWork;                                       \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 position[3];                                                           \
    s32 distance;                                                              \
    s32 near;                                                                  \
                                                                               \
    if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||              \
        *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {              \
      Actor_SetAnimationSpeed(self, 0);                                        \
      return;                                                                  \
    }                                                                          \
    Actor_SetAnimationSpeed(self, 16);                                         \
    if ((++*(u16 *)&self->timer & 31) == 31)                                   \
      Event_PlaySound(SE_WATER_LEVEL);                                         \
    {                                                                          \
      s8 *to = self->sprite;                                                   \
      s8 *from = leader->sprite;                                               \
      u32 a = from[9] & 12;                                                    \
      to[9] = (s8)((to[9] & ~12) | a);                                         \
    }                                                                          \
    if (call1(self->x, self->z, 2) != 255) {                                   \
      position[0] = self->x;                                                   \
      position[1] = self->y;                                                   \
      position[2] = self->z;                                                   \
      Camera_WorldToScreen(position);                                          \
      distance = position[0];                                                  \
      if (distance > 0x1100000)                                                \
        goto stop;                                                             \
      if (distance < -0x200000)                                                \
        goto stop;                                                             \
      {                                                                        \
        if (*((u8 *)self + 0x62) == 0) {                                       \
          distance = leader->y - self->y;                                      \
          if (distance < 0)                                                    \
            distance = self->y - leader->y;                                    \
          near = 0;                                                            \
          if (call2(&leader->x, &self->x) <= 12 && distance < 0xc0000)         \
            near = 1;                                                          \
          if (near && !Event_GetFlag(260)) {                                   \
            leader->facing = self->facing;                                     \
            leader->watchedObject = self;                                      \
            leader->priorityFlags &= ~1;                                       \
            {                                                                  \
              s32 delay = 200;                                                 \
              *(u16 *)(work + 362) = delay;                                    \
            }                                                                  \
            gMainRoutineWork.renderMode = 0;                                   \
            *((u8 *)self + 0x62) = 1;                                          \
            *((u8 *)self + 0x63) = 1;                                          \
          }                                                                    \
        }                                                                      \
        {                                                                      \
          s32 (*multiply)(s32, s32);                                           \
          s32 cosine = Trig_Cos(self->facing);                                 \
          s32 speed = self->speedLimit;                                        \
          /* FAKEMATCH(dead): the routine address taken inside a once-only     \
           * block, so the speed loads first; as a plain call, 2 of 197        \
           * instructions differ in tla-en. */                                 \
          do {                                                                 \
            multiply = Iwram_MulQ16;                                           \
          } while (0);                                                         \
          self->x += multiply(cosine, speed);                                  \
        }                                                                      \
        self->z += Iwram_MulQ16(Math_Sin(self->facing), self->speedLimit);     \
        return;                                                                \
      }                                                                        \
    }                                                                          \
  stop:                                                                        \
    {                                                                          \
      s32 zero = 0;                                                            \
      *((u8 *)self + 0x63) = zero;                                             \
      self->update = zero;                                                     \
      Actor_StartScript(self, data1);                                          \
      Event_PlaySound(SE_SPARKLING_PASSAGE);                                   \
      data2 = zero;                                                            \
    }                                                                          \
  }

/// Whirlwind_Update as the lower floors' copies compile it.
#define DEFINE_WHIRLWIND_UPDATE_LOWER(name, call1, call2, data1, data2)        \
  void name(struct Actor *self) {                                              \
    struct EventWork *work = Ram_HeapSlots->eventWork;                         \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 position[3];                                                           \
    s32 distance;                                                              \
    s32 near;                                                                  \
                                                                               \
    if (work->unk166 != 0 || *(s16 *)&work->unk15a != 0 ||                     \
        *(s16 *)&work->unk15c[2] != 0 || *(s16 *)work->unk168 != 0) {          \
      Actor_SetAnimationSpeed(self, 0);                                        \
      return;                                                                  \
    }                                                                          \
    Actor_SetAnimationSpeed(self, 16);                                         \
    if ((++*(u16 *)&self->timer & 31) == 31)                                   \
      Event_PlaySound(SE_WATER_LEVEL);                                         \
    ((struct FieldSprite *)self->sprite)->priority =                           \
        ((struct FieldSprite *)leader->sprite)->priority;                      \
    if (call1(self->x, self->z, 2) != 255) {                                   \
      position[0] = self->x;                                                   \
      position[1] = self->y;                                                   \
      position[2] = self->z;                                                   \
      Camera_WorldToScreen(position);                                          \
      distance = position[0];                                                  \
      if (distance > 0x1100000)                                                \
        goto stop;                                                             \
      if (distance < -0x200000)                                                \
        goto stop;                                                             \
      {                                                                        \
        if (*((u8 *)self + 0x62) == 0) {                                       \
          distance = leader->y - self->y;                                      \
          if (distance < 0)                                                    \
            distance = self->y - leader->y;                                    \
          near = 0;                                                            \
          if (call2(&leader->x, &self->x) <= 12 && distance < 0xc0000)         \
            near = 1;                                                          \
          if (near && !Event_GetFlag(260)) {                                   \
            leader->facing = self->facing;                                     \
            leader->watchedObject = self;                                      \
            leader->priorityFlags &= ~1;                                       \
            work->unk16a = 200;                                                \
            gMainRoutineWork.renderMode = 0;                                   \
            *((u8 *)self + 0x62) = 1;                                          \
            *((u8 *)self + 0x63) = 1;                                          \
          }                                                                    \
        }                                                                      \
        /* IwramMulQ16 reached from IwramIrqMain rather than at its fixed      \
         * address: */                                                         \
        /* the address is a linked word read after the cosine, as the native   \
         */                                                                    \
        /* reads it after the speed (at Iwram_MulQ16 it is built before). */   \
        self->x += ((s32 (*)(s32, s32))(IwramIrqMain + Iwram_MulQ16Offset))(   \
            Trig_Cos(self->facing), self->speedLimit);                         \
        self->z += ((s32 (*)(s32, s32))(IwramIrqMain + Iwram_MulQ16Offset))(   \
            Math_Sin(self->facing), self->speedLimit);                         \
        return;                                                                \
      }                                                                        \
    }                                                                          \
  stop:                                                                        \
    {                                                                          \
      s32 zero = 0;                                                            \
                                                                               \
      /* FAKEMATCH(temporary): one word zero clears the ride byte, then the    \
       * update; as self->update = 0; *((u8 *)self + 0x63) = 0;, 4 of 197      \
       * instructions differ in tla-en (byte first, 15). */                    \
      *((u8 *)self + 0x63) = zero;                                             \
      self->update = (void *)zero;                                             \
    }                                                                          \
    Actor_StartScript(self, data1);                                            \
    Event_PlaySound(SE_SPARKLING_PASSAGE);                                     \
    data2 = 0;                                                                 \
  }

#endif

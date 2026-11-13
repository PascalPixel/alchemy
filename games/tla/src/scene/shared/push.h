/// @file
/// Pushing an object across the map grid: finding it, finding where it
/// stops and moving it with its cells.
/// Each routine is a macro a scene expands where its copy sits, with the
/// names of its own data and calls.
#ifndef ALCHEMY_SCENE_SHARED_PUSH_H
#define ALCHEMY_SCENE_SHARED_PUSH_H

/// Finds the pushable object in front of the leader: its slot, kind and the
/// direction pushed.
/// 4 copies: eastern_alhafra/s4004, jupiter_lighthouse_room_3/s0dcc,
/// jupiter_lighthouse_room_8/s0c44, mars_lighthouse_room_f/s3d80.
#define DEFINE_PUSH_TARGET_FIND_AHEAD(name, data1, data2, data3)               \
  struct Actor *name(s32 *direction, s32 *slot, s32 *kind) {                   \
    struct EventWork *work = Ram_HeapSlots->eventWork;                         \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    struct Actor **entry;                                                      \
    struct Actor *object;                                                      \
    u32 i, k;                                                                  \
    s32 x, z, left, top, right, bottom;                                        \
                                                                               \
    *direction = leader->facing >> 12;                                         \
    i = 8;                                                                     \
    entry = &work->objects[8];                                                 \
  outer:                                                                       \
    object = *entry;                                                           \
    k = 0;                                                                     \
  inner:                                                                       \
    if (((struct ObjectAnimationState *)object->sprite)->value28[0] ==         \
        data1[k]) {                                                            \
      x = (PIXELS(leader->x) + (data2[*direction] >> 16)) >> 4;                \
      z = (PIXELS(leader->z) + (s16)data2[*direction]) >> 4;                   \
      left = (PIXELS(object->x) + data3[k][0]) >> 4;                           \
      top = (PIXELS(object->z) + data3[k][1]) >> 4;                            \
      right = (PIXELS(object->x) + data3[k][2]) >> 4;                          \
      bottom = (PIXELS(object->z) + data3[k][3]) >> 4;                         \
      if (left <= x && x < right && top <= z && z < bottom) {                  \
        *kind = k;                                                             \
        if (k & 1) {                                                           \
          if (left != leader->x >> 20) {                                       \
            *slot = i;                                                         \
            return object;                                                     \
          }                                                                    \
        } else {                                                               \
          if (top != leader->z >> 20) {                                        \
            *slot = i;                                                         \
            return object;                                                     \
          }                                                                    \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    k++;                                                                       \
    if (k <= 5)                                                                \
      goto inner;                                                              \
    entry++;                                                                   \
    i++;                                                                       \
    if (i <= 63)                                                               \
      goto outer;                                                              \
    return NULL;                                                               \
  }

/// Pushes an object to its target, the leader following, and moves its cells
/// with it.
/// 2 copies: islets/s3410, mars_lighthouse_room_f/s3d80.
#define DEFINE_PUSH_TARGET_PUSH(name, data1, call1, call2, call3, data2)       \
  void name(struct PushTarget target) {                                        \
    u8 *map = (u8 *)((struct HeapSlotWords *)Ram_HeapSlots)->mapWork;          \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    struct Actor *object = Actor_Get(target.id);                               \
    s32 cell[3];                                                               \
    s32 from[3];                                                               \
    s32 footprint;                                                             \
    s32 width;                                                                 \
    s32 height;                                                                \
    s32 originX;                                                               \
    s32 originZ;                                                               \
    s32 direction;                                                             \
    s32 speed;                                                                 \
    s32 acceleration;                                                          \
                                                                               \
    direction = leader->facing >> 16;                                          \
    footprint = target.footprint;                                              \
    speed = leader->speedLimit;                                                \
    acceleration = leader->acceleration;                                       \
    from[0] = object->x;                                                       \
    from[2] = object->z;                                                       \
    cell[0] = (object->x + (data1[footprint][0] << 16)) >> 20;                 \
    cell[2] = (object->z + (data1[footprint][1] << 16)) >> 20;                 \
    width = (ABS(data1[footprint][0]) + ABS(data1[footprint][2])) >> 4;        \
    height = (ABS(data1[footprint][1]) + ABS(data1[footprint][3])) >> 4;       \
    call1(0, cell[0], cell[2], width, height, 0);                              \
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x1999);             \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 8);                      \
    System_Wait(15);                                                           \
    {                                                                          \
      s32 deltaX = (target.x - from[0]) >> 17;                                 \
      s32 deltaZ = (target.z - from[2]) >> 17;                                 \
                                                                               \
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, deltaX,        \
                                 deltaZ);                                      \
    }                                                                          \
    Actor_Get(gMainRoutineWork.currentOwner)->update = call2;                  \
    System_Wait(4);                                                            \
    if ((u32)(direction - 6) <= 7)                                             \
      call3(object, 3);                                                        \
    else                                                                       \
      call3(object, 2);                                                        \
    Event_PlaySound(239);                                                      \
    Actor_SetSpeed(target.id, 0x8000, 0x1999);                                 \
    FieldActor_SetMoveTarget(object, target.x, target.y, target.z);            \
    FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);                 \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);                      \
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);             \
    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner,                  \
                               (data2[direction] << 16) >> 31,                 \
                               (data2[direction] << 16) >> 17);                \
    if (target.callback != 0)                                                  \
      target.callback();                                                       \
    FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);                 \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);                      \
    leader->update = 0;                                                        \
    leader->speedLimit = speed;                                                \
    leader->acceleration = acceleration;                                       \
    FieldActor_WaitForMove(object);                                                 \
    Event_PlaySound(298);                                                      \
    Event_PlaySound(213);                                                      \
    FieldActor_SetPositionAndResetMotion(object, target.x, object->y,          \
                                         target.z);                            \
    call3(object, 1);                                                          \
    target.x = target.x + (data1[footprint][0] << 16);                         \
    target.z = target.z + (data1[footprint][1] << 16);                         \
    target.x >>= 20;                                                           \
    target.z >>= 20;                                                           \
    originX = *(s32 *)(map + 328) >> 20;                                       \
    originZ = *(s32 *)(map + 332) >> 20;                                       \
    MapGrid_CopyCellAttributes(target.x, target.z, width, height,              \
                               originX + target.x, originZ + target.z);        \
    call1(0, target.x, target.z, width, height, 255);                          \
    call1(2, target.x, target.z, width, height, 255);                          \
    cell[0] = (from[0] + (data1[footprint][0] << 16)) >> 20;                   \
    cell[2] = (from[2] + (data1[footprint][1] << 16)) >> 20;                   \
    originX += cell[0];                                                        \
    originZ += cell[2];                                                        \
    MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0],       \
                               cell[2]);                                       \
    call1(2, cell[0], cell[2], width, height, 0);                              \
    Event_NoOp();                                                              \
  }

/// PushTarget_Push for the rooms that fill the cells with
/// Map_FillCellRectByte2.
/// 2 copies: jupiter_lighthouse_room_3/s0dcc, jupiter_lighthouse_room_8/s0c44.
#define DEFINE_PUSH_TARGET_PUSH_FILL(name, data1, call1, call2, data2)         \
  void name(struct PushTarget target) {                                        \
    u8 *map = (u8 *)((struct HeapSlotWords *)Ram_HeapSlots)->mapWork;          \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    struct Actor *object = Actor_Get(target.id);                               \
    s32 cell[3];                                                               \
    s32 from[3];                                                               \
    s32 footprint;                                                             \
    s32 width;                                                                 \
    s32 height;                                                                \
    s32 originX;                                                               \
    s32 originZ;                                                               \
    s32 direction;                                                             \
    s32 speed;                                                                 \
    s32 acceleration;                                                          \
                                                                               \
    direction = leader->facing >> 16;                                          \
    footprint = target.footprint;                                              \
    speed = leader->speedLimit;                                                \
    acceleration = leader->acceleration;                                       \
    from[0] = object->x;                                                       \
    from[2] = object->z;                                                       \
    cell[0] = (object->x + (data1[footprint][0] << 16)) >> 20;                 \
    cell[2] = (object->z + (data1[footprint][1] << 16)) >> 20;                 \
    width = (ABS(data1[footprint][0]) + ABS(data1[footprint][2])) >> 4;        \
    height = (ABS(data1[footprint][1]) + ABS(data1[footprint][3])) >> 4;       \
    Map_FillCellRectByte2(0, cell[0], cell[2], width, height, 0);              \
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x1999);             \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 8);                      \
    System_Wait(15);                                                           \
    {                                                                          \
      s32 deltaX = (target.x - from[0]) >> 17;                                 \
      s32 deltaZ = (target.z - from[2]) >> 17;                                 \
                                                                               \
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, deltaX,        \
                                 deltaZ);                                      \
    }                                                                          \
    Actor_Get(gMainRoutineWork.currentOwner)->update = call1;                  \
    System_Wait(4);                                                            \
    if ((u32)(direction - 6) <= 7)                                             \
      call2(object, 3);                                                        \
    else                                                                       \
      call2(object, 2);                                                        \
    Event_PlaySound(239);                                                      \
    Actor_SetSpeed(target.id, 0x8000, 0x1999);                                 \
    FieldActor_SetMoveTarget(object, target.x, target.y, target.z);            \
    FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);                 \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);                      \
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);             \
    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner,                  \
                               (data2[direction] << 16) >> 31,                 \
                               (data2[direction] << 16) >> 17);                \
    if (target.callback != 0)                                                  \
      target.callback();                                                       \
    FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);                 \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);                      \
    leader->update = 0;                                                        \
    leader->speedLimit = speed;                                                \
    leader->acceleration = acceleration;                                       \
    FieldActor_WaitForMove(object);                                                 \
    Event_PlaySound(298);                                                      \
    Event_PlaySound(213);                                                      \
    FieldActor_SetPositionAndResetMotion(object, target.x, object->y,          \
                                         target.z);                            \
    call2(object, 1);                                                          \
    target.x = target.x + (data1[footprint][0] << 16);                         \
    target.z = target.z + (data1[footprint][1] << 16);                         \
    target.x >>= 20;                                                           \
    target.z >>= 20;                                                           \
    originX = *(s32 *)(map + 328) >> 20;                                       \
    originZ = *(s32 *)(map + 332) >> 20;                                       \
    MapGrid_CopyCellAttributes(target.x, target.z, width, height,              \
                               originX + target.x, originZ + target.z);        \
    Map_FillCellRectByte2(0, target.x, target.z, width, height, 255);          \
    Map_FillCellRectByte2(2, target.x, target.z, width, height, 255);          \
    cell[0] = (from[0] + (data1[footprint][0] << 16)) >> 20;                   \
    cell[2] = (from[2] + (data1[footprint][1] << 16)) >> 20;                   \
    originX += cell[0];                                                        \
    originZ += cell[2];                                                        \
    MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0],       \
                               cell[2]);                                       \
    Map_FillCellRectByte2(2, cell[0], cell[2], width, height, 0);              \
    Event_NoOp();                                                              \
  }

/// Stops an object at its push target, or at what blocks it.
/// 2 copies: eastern_alhafra/s44c4, mars_lighthouse_room_f/s3d80.
#define DEFINE_ACTOR_STOP_AT_PUSH_TARGET(name, call1, data1, call2)            \
  void name(struct Actor *object) {                                            \
    s32 position[3];                                                           \
    struct Actor *hit;                                                         \
    u32 i;                                                                     \
    s32 angle = object->facing >> 12;                                          \
                                                                               \
    angle = ((angle + 2) & 12) << 12;                                          \
    position[0] = object->x;                                                   \
    position[1] = object->y;                                                   \
    position[2] = object->z;                                                   \
    FieldVector_AddPolarOffset(0x100000, angle, position);                     \
    hit = call1(position, 1);                                                  \
    if (hit != 0) {                                                            \
      i = 0;                                                                   \
    next:                                                                      \
      if (*((struct ObjectAnimationState *)hit->sprite)->value28 == data1[i])  \
        return;                                                                \
      i++;                                                                     \
      if (i <= 5)                                                              \
        goto next;                                                             \
      FieldActor_SetPositionAndResetMotion(object, object->x, object->y,       \
                                           object->z);                         \
    }                                                                          \
    position[0] = object->x;                                                   \
    position[1] = object->y;                                                   \
    position[2] = object->z;                                                   \
    if (call2(object, position) > 0)                                           \
      FieldActor_SetPositionAndResetMotion(object, object->x, object->y,       \
                                           object->z);                         \
  }

/// Finds how far the object ahead slides before something blocks it; 1 when it
/// moves.
/// 3 copies: jupiter_lighthouse_room_3/s0dcc, jupiter_lighthouse_room_8/s0c44,
/// mars_lighthouse_room_f/s3d80.
#define DEFINE_PUSH_TARGET_FIND_SLIDE(name, call1, data1, data2, call2)        \
  s32 name(struct PushTarget *target) {                                        \
    struct Actor *object;                                                      \
    s32 direction;                                                             \
    s32 position[3];                                                           \
    s32 footprint;                                                             \
    s32 width;                                                                 \
    s32 height;                                                                \
    s32 dx;                                                                    \
    s32 dz;                                                                    \
    s32 count;                                                                 \
    s32 row;                                                                   \
    s32 col;                                                                   \
    u8 *layer;                                                                 \
                                                                               \
    object = call1(&direction, &target->id, &target->footprint);               \
    if (object != 0) {                                                         \
      footprint = target->footprint;                                           \
      width = (ABS(data1[footprint][0]) + ABS(data1[footprint][2])) >> 4;      \
      height = (ABS(data1[footprint][1]) + ABS(data1[footprint][3])) >> 4;     \
      dx = data2[direction] & 0xffff0000;                                      \
      dz = data2[direction] << 16;                                             \
      target->callback = 0;                                                    \
      count = 0;                                                               \
      layer = &object->terrainId;                                              \
      *layer = 2;                                                              \
      target->x = object->x + dx;                                              \
      target->z = object->z + dz;                                              \
      position[1] = object->y;                                                 \
      for (;;) {                                                               \
        position[2] = target->z + (data1[footprint][1] << 16);                 \
        for (row = 0; row < height; row++) {                                   \
          position[0] = target->x + (data1[footprint][0] << 16);               \
          for (col = 0; col < width; col++) {                                  \
            /* FAKEMATCH(wrapper): the call through Func_02002338_; as         \
             * MapGrid_CheckCollision(object, position), 2 of 171 instructions differ   \
             * in tla-en. */                                                   \
            if (call2(object, position) == 2)                                  \
              goto blocked;                                                    \
            position[0] += 0x100000;                                           \
          }                                                                    \
          position[2] += 0x100000;                                             \
        }                                                                      \
        target->x += dx;                                                       \
        count++;                                                               \
        target->z += dz;                                                       \
      }                                                                        \
    blocked:                                                                   \
      *layer = 0;                                                              \
      if (count == 0)                                                          \
        return 0;                                                              \
      target->x = object->x + dx * count;                                      \
      target->y = object->y;                                                   \
      target->z = object->z + dz * count;                                      \
      return 1;                                                                \
    }                                                                          \
    return 0;                                                                  \
  }

/// Pushes the block ahead onto another, if nothing blocks the way.
/// 2 copies: magma_rock_inside/magma_rock_inside, venus_lighthouse.
#define DEFINE_BLOCK_PUSH_ONTO(name, data1, call1, call2)                      \
  void name(void) {                                                            \
    struct FieldPosition dst;                                                  \
    struct StagedActor *lead;                                                  \
    struct StagedActor *next;                                                  \
    struct StagedActor *blocker;                                               \
    s32 dir;                                                                   \
    u32 step;                                                                  \
    s32 rate;                                                                  \
                                                                               \
    lead = (struct StagedActor *)Actor_GetById(gMainRoutineWork.currentOwner); \
    dir = lead->facing >> 12;                                                  \
    step = data1[dir];                                                         \
    dst.x = lead->x.fixed + (step & 0xffff0000);                               \
    dst.y = lead->y.fixed;                                                     \
    step <<= 16;                                                               \
    dst.z = lead->z.fixed + step;                                              \
    next = Actor_FindInCell(&dst, lead);                                       \
    if (next == 0)                                                             \
      return;                                                                  \
                                                                               \
    step = data1[dir];                                                         \
    dst.x = next->x.fixed + (step & 0xffff0000);                               \
    dst.y = next->y.fixed;                                                     \
    step <<= 16;                                                               \
    dst.z = next->z.fixed + step;                                              \
    blocker = Actor_FindInCell(&dst, next);                                    \
    if (blocker != 0 && (blocker->collisionFlags & 1) != 0)                    \
      return;                                                                  \
                                                                               \
    dst.x = next->x.fixed;                                                     \
    dst.y = next->y.fixed + 0x100000;                                          \
    dst.z = next->z.fixed;                                                     \
    blocker = Actor_FindInCell(&dst, next);                                    \
    if (blocker != 0 && (blocker->collisionFlags & 1) != 0)                    \
      return;                                                                  \
                                                                               \
    next->transitionMode = 2;                                                  \
    step = data1[dir];                                                         \
    dst.x = next->x.fixed + (step & 0xffff0000);                               \
    dst.y = next->y.fixed;                                                     \
    step <<= 16;                                                               \
    dst.z = next->z.fixed + step;                                              \
    if (call1(next, &dst) > 0)                                                 \
      return;                                                                  \
                                                                               \
    if (next->transitionBusy != 0)                                             \
      return;                                                                  \
                                                                               \
    call2(lead, 8);                                                            \
    rate = 0x3333;                                                             \
    System_Wait(15);                                                           \
    Event_PlaySound(185);                                                      \
    next->moveRateX = rate;                                                    \
    next->moveRateZ = rate;                                                    \
    FieldActor_SetMoveTarget(next, dst.x, dst.y, dst.z);                       \
    lead->moveRateX = rate;                                                    \
    lead->moveRateZ = rate;                                                    \
    FieldActor_SetMoveTarget(lead, dst.x, dst.y, dst.z);                       \
    FieldActor_WaitForMove(next);                                                   \
    Event_NoOp();                                                              \
    next->x.fixed = dst.x;                                                     \
    {                                                                          \
      /* FAKEMATCH(temporary): z read ahead of velocityX's store; as dst.z at  \
       * its store, 4 of 175 instructions differ in tla-en. */                 \
      s32 z = dst.z;                                                           \
                                                                               \
      next->velocityX = 0;                                                     \
      next->z.fixed = z;                                                       \
      next->velocityZ = 0;                                                     \
    }                                                                          \
    lead->targetX = 0x80000000;                                                \
    lead->targetZ = 0x80000000;                                                \
    lead->velocityX = 0;                                                       \
    lead->velocityZ = 0;                                                       \
    lead->x.fixed = lead->x.part.pixel << 16;                                  \
    lead->z.fixed = lead->z.part.pixel << 16;                                  \
    call2(lead, 1);                                                            \
  }

#endif

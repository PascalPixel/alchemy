#include "room.h"

extern u16 gData_020074c8;

void Func_02004700(s32 value) { gData_020074c8 = value; }

#include "types.h"
#include "actor.h"
#include "fixed_math.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "system/main_routine_work.h"
#include "event_inline.h"

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

void Func_0200410c(void);
s32 Func_02005538(s32 *a, s32 radiusA, struct Position *b, s32 radiusB);
void Actor_SetAnimationSpeed(struct Actor *actor, s32 speed);
void Func_02004080(struct Actor *object);

void Func_0200470c(void) {
  struct Position probe;
  s32 heading;
  s32 startBehavior;
  s32 x;
  s32 z;
  s32 radius;
  struct Actor *actor;
  struct Position *next;
  s32 nextBehavior;
  u8 *layer;
  struct Actor *other;
  struct Position *position;
  u8 *flags;
  s32 i;
  s32 cell;
  s32 angle;
  s32 zero;
  struct Position *step;
  s32 startX;
  s32 startZ;

  actor = ObjectTable_Get(gMainRoutineWork.currentOwner);
  heading = (s16)((actor->facing + 0x2000) & -0x4000);

top:
  Func_0200410c();
  actor->speedLimit = PIXELS(1);
  actor->acceleration = PIXELS(1) / 2;
  actor->targetX = 0x80000000;
  actor->targetZ = 0x80000000;
  // Default: one zero, spilled and reloaded at each use.
  zero = 0;
  actor->velocityX = zero;
  actor->velocityZ = zero;
  /* FAKEMATCH(temporary): cell holds the masked x; as one expression, 2 of 246
   * instructions differ in tla-en. */
  cell = actor->x & 0xfff00000;
  /* FAKEMATCH(temporary): i holds half a cell here and the search's count
   * later; as a separate PIXELS(8), 18 of 246 instructions differ in tla-en. */
  i = PIXELS(8);
  position = &probe;
  position->x = startX = cell + i;
  position->y = actor->y;
  position->z = startZ = (actor->z & 0xfff00000) + i;
  x = startX;
  z = startZ;
  startBehavior = MapGrid_GetCellBehaviorAt(*(layer = &actor->terrainId), startX, startZ);
  FieldVector_AddPolarOffset(PIXELS(16), heading, position);
  nextBehavior = MapGrid_GetCellBehaviorAt(*layer, position->x, position->z);
  if (nextBehavior == 255 ||
      Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y > i) {
    actor->facing = heading;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->x = startX;
    actor->z = startZ;
    goto done;
  }
  position->x = x;
  position->z = z;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  actor->timer = zero;
  Func_02005480(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  actor->update = Func_02004080;
  /* FAKEMATCH(barrier): the empty asm clobbering sfp and reading nextBehavior
   * twice; as no asm, 18 of 246 instructions differ in tla-en. */
  asm("" : : "r"(nextBehavior), "r"(nextBehavior) : SFP_CLOBBER);
  goto test;
body:
  if (Map_GetTerrainHeight(*layer, next->x, next->z) - actor->y > PIXELS(8)) {
    goto stop;
  }
  radius = actor->radius - 2;
  other = Ram_HeapSlots->scriptObjects;
  flags = &other->collisionFlags;
  for (i = 0; i < 64; i++, flags += 128, other++) {
    if (other->script != NULL && (*flags & 1) && other != actor) {
      /* FAKEMATCH(barrier): the empty asm clobbering sfp and r4 and reading
       * next and actor; as no asm, 170 of 246 instructions differ in tla-en. */
      asm(""
          :
          : "r"(next), "r"(next), "r"(actor), "r"(actor), "r"(actor),
            "r"(actor), "r"(actor), "r"(actor), "r"(actor), "r"(actor)
          : SFP_CLOBBER, "r4");
      if (Func_02005538(&other->x, radius, &probe, other->radius - 2) >= 0) {
        goto stop;
      }
    }
  }
  x = next->x;
  z = next->z;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  FieldActor_SetMoveTarget(actor, next->x, next->y, next->z);
  Actor_WaitForMove(actor);
  if (nextBehavior != startBehavior) {
    goto blocked;
  }
test:
  /* FAKEMATCH(barrier): the asm tied to heading, with an r4 clobber so PRE
   * leaves it, loads the angle ahead of the address; as heading in the call, 44
   * of 246 instructions differ in tla-en. */
  asm("" : "=r"(angle) : "0"(heading) : "r4");
  step = &probe;
  FieldVector_AddPolarOffset(PIXELS(16), angle, step);
  /* FAKEMATCH(dead): the unread store ends CSE's knowledge of the address, so
   * next rebuilds it; as &probe in the call, 97 of 246 instructions differ in
   * tla-en. */
  step = NULL;
  next = &probe;
  nextBehavior = MapGrid_GetCellBehaviorAt(*layer, next->x, next->z);
  if (nextBehavior != 255) {
    goto body;
  }
stop:
  actor->speedLimit = PIXELS(2);
  actor->acceleration = PIXELS(1);
  FieldActor_SetMoveTarget(actor, x, actor->y, z);
  Actor_WaitForMove(actor);
  System_Wait(2);
  goto top;

blocked:
  System_Wait(10);
done:
  actor->update = NULL;
  actor->actionFlags |= 1;
  actor->acceleration = PIXELS(1) / 4;
  Func_02005480(actor, 1);
}

#include "field_sprite.h"

extern u16 gData_02005f58[];
extern s8 gData_02005f64[][4];

void Func_02004a44(s32 layer, s32 x, s32 z, u32 width, u32 height, s32 value);

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 Func_02004938(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_Get(id);
  s16 shape = *(s16 *)((struct FieldSprite *)object->sprite)->record;
  s16 i = 0;
  u32 j;
  s32 width;
  s32 height;
  s32 x;
  s32 z;
  s32 mapX;
  s32 mapZ;
  s32 toX;
  s32 toZ;

  if ((u16)shape != gData_02005f58[(u16)i]) {
  next:
    j = (u16)((i << 16) + 0x10000 >> 16);
    i = j;
    if (j <= 5 && (u16)shape != gData_02005f58[j])
      goto next;
  }
  j = (u16)i;
  if (j == 6)
    return 0;
  width = (ABS(gData_02005f64[j][0]) + ABS(gData_02005f64[j][2])) >> 4;
  height = (ABS(gData_02005f64[j][1]) + ABS(gData_02005f64[j][3])) >> 4;
  x = (object->x + (gData_02005f64[j][0] << 16)) >> 20;
  z = (object->z + (gData_02005f64[j][1] << 16)) >> 20;
  mapX = *(s32 *)(map + 328) >> 20;
  i = mapX;
  mapZ = *(s32 *)(map + 332) >> 20;
  toX = x + (u16)i;
  toZ = z + (u16)mapZ;
  MapGrid_CopyCellAttributes(x, z, width, height, toX, toZ);
  Func_02004a44(0, x, z, width, height, 255);
  Func_02004a44(2, x, z, width, height, 255);
  return 1;
}

void Func_02004a44(s32 layer, s32 x, s32 z, u32 width, u32 height, s32 value) {
  struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
  u8 *cells = map->layers[layer].cells;
  u32 row;
  u32 column;

  cells += (x + (z << 7)) << 2;
  for (row = 0; row < height; row++) {
    u8 *cell = cells + (row << 9);

    for (column = 0; column < width; column++) {
      cell[2] = value;
      cell += 4;
    }
  }
}

extern s32 gData_02005f7c[];

struct Actor *Func_02004bf4(s32 *direction, s32 *slot, s32 *kind);
s32 Func_020054f8(struct Actor *object, s32 *position);

static inline s32 Func_020054f8_(struct Actor *object, s32 *position) {
  return Func_020054f8(object, position);
}

/// Finds the object the leader is pushing and steps its footprint, one cell
/// at a time in the leader's direction, until a cell of it is blocked
/// (Func_020054f8 returns 2). Leaves the object's resting place in target and
/// returns 1, or 0 when nothing is pushed or it cannot move at all.
s32 Func_02004a8c(struct PushTarget *target) {
  struct Actor *object;
  s32 direction;
  s32 position[3];
  s32 footprint;
  s32 width;
  s32 height;
  s32 dx;
  s32 dz;
  s32 count;
  s32 row;
  s32 col;
  u8 *layer;

  object = Func_02004bf4(&direction, &target->id, &target->footprint);
  if (object != 0) {
    footprint = target->footprint;
    width = (ABS(gData_02005f64[footprint][0]) +
             ABS(gData_02005f64[footprint][2])) >>
            4;
    height = (ABS(gData_02005f64[footprint][1]) +
              ABS(gData_02005f64[footprint][3])) >>
             4;
    dx = gData_02005f7c[direction] & 0xffff0000;
    dz = gData_02005f7c[direction] << 16;
    target->callback = 0;
    count = 0;
    layer = &object->terrainId;
    *layer = 2;
    target->x = object->x + dx;
    target->z = object->z + dz;
    position[1] = object->y;
    for (;;) {
      position[2] = target->z + (gData_02005f64[footprint][1] << 16);
      for (row = 0; row < height; row++) {
        position[0] = target->x + (gData_02005f64[footprint][0] << 16);
        for (col = 0; col < width; col++) {
          /* FAKEMATCH(wrapper): the call through Func_020054f8_; as
           * Func_020054f8(object, position), 2 of 171 instructions differ in
           * tla-en. */
          if (Func_020054f8_(object, position) == 2)
            goto blocked;
          position[0] += 0x100000;
        }
        position[2] += 0x100000;
      }
      target->x += dx;
      count++;
      target->z += dz;
    }
  blocked:
    *layer = 0;
    if (count == 0)
      return 0;
    target->x = object->x + dx * count;
    target->y = object->y;
    target->z = object->z + dz * count;
    return 1;
  }
  return 0;
}

#include "event_work.h"

#undef PIXELS
/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

struct Actor *Func_02004bf4(s32 *direction, s32 *slot, s32 *kind) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor **entry;
  struct Actor *object;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = leader->facing >> 12;
  i = 8;
  entry = &work->objects[8];
outer:
  object = *entry;
  k = 0;
inner:
  if (((struct ObjectAnimationState *)object->sprite)->value28[0] ==
      gData_02005f58[k]) {
    x = (PIXELS(leader->x) + (gData_02005f7c[*direction] >> 16)) >> 4;
    z = (PIXELS(leader->z) + (s16)gData_02005f7c[*direction]) >> 4;
    left = (PIXELS(object->x) + gData_02005f64[k][0]) >> 4;
    top = (PIXELS(object->z) + gData_02005f64[k][1]) >> 4;
    right = (PIXELS(object->x) + gData_02005f64[k][2]) >> 4;
    bottom = (PIXELS(object->z) + gData_02005f64[k][3]) >> 4;
    if (left <= x && x < right && top <= z && z < bottom) {
      *kind = k;
      if (k & 1) {
        if (left != leader->x >> 20) {
          *slot = i;
          return object;
        }
      } else {
        if (top != leader->z >> 20) {
          *slot = i;
          return object;
        }
      }
    }
  }
  k++;
  if (k <= 5)
    goto inner;
  entry++;
  i++;
  if (i <= 63)
    goto outer;
  return NULL;
}

#undef PIXELS

void Func_02005668(void);
void Func_02004fc0(struct Actor *object);

/// The heap slots read as words: the map slot comes through u32, not its
/// pointer type, so its load stays behind the stores of the block.
struct HeapSlotWords {
  u32 slots[8];
  u32 mapWork;
};

void Func_02004d10(struct PushTarget target) {
  u8 *map = (u8 *)((struct HeapSlotWords *)Ram_HeapSlots)->mapWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_Get(target.id);
  s32 cell[3];
  s32 from[3];
  s32 footprint;
  s32 width;
  s32 height;
  s32 originX;
  s32 originZ;
  s32 direction;
  s32 speed;
  s32 acceleration;

  direction = leader->facing >> 16;
  footprint = target.footprint;
  speed = leader->speedLimit;
  acceleration = leader->acceleration;
  from[0] = object->x;
  from[2] = object->z;
  cell[0] = (object->x + (gData_02005f64[footprint][0] << 16)) >> 20;
  cell[2] = (object->z + (gData_02005f64[footprint][1] << 16)) >> 20;
  width = (ABS(gData_02005f64[footprint][0]) +
           ABS(gData_02005f64[footprint][2])) >>
          4;
  height = (ABS(gData_02005f64[footprint][1]) +
            ABS(gData_02005f64[footprint][3])) >>
           4;
  Func_02004a44(0, cell[0], cell[2], width, height, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x1999);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 8);
  System_Wait(15);
  {
    s32 deltaX = (target.x - from[0]) >> 17;
    s32 deltaZ = (target.z - from[2]) >> 17;

    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, deltaX,
                                       deltaZ);
  }
  Actor_Get(gMainRoutineWork.currentOwner)->update = Func_02004fc0;
  System_Wait(4);
  if ((u32)(direction - 6) <= 7)
    Func_02005480(object, 3);
  else
    Func_02005480(object, 2);
  Event_PlaySound(239);
  Actor_SetSpeed(target.id, 0x8000, 0x1999);
  FieldActor_SetMoveTarget(object, target.x, target.y, target.z);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner,
                                     (gData_02005f7c[direction] << 16) >> 31,
                                     (gData_02005f7c[direction] << 16) >> 17);
  if (target.callback != 0)
    target.callback();
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  leader->update = 0;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Actor_WaitForMove(object);
  Event_PlaySound(298);
  Event_PlaySound(213);
  FieldActor_SetPositionAndResetMotion(object, target.x, object->y, target.z);
  Func_02005480(object, 1);
  target.x = target.x + (gData_02005f64[footprint][0] << 16);
  target.z = target.z + (gData_02005f64[footprint][1] << 16);
  target.x >>= 20;
  target.z >>= 20;
  originX = *(s32 *)(map + 328) >> 20;
  originZ = *(s32 *)(map + 332) >> 20;
  MapGrid_CopyCellAttributes(target.x, target.z, width, height, originX + target.x, originZ + target.z);
  Func_02004a44(0, target.x, target.z, width, height, 255);
  Func_02004a44(2, target.x, target.z, width, height, 255);
  cell[0] = (from[0] + (gData_02005f64[footprint][0] << 16)) >> 20;
  cell[2] = (from[2] + (gData_02005f64[footprint][1] << 16)) >> 20;
  originX += cell[0];
  originZ += cell[2];
  MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0], cell[2]);
  Func_02004a44(2, cell[0], cell[2], width, height, 0);
  Func_02005668();
}

void Func_02004fc0(struct Actor *object) {
  s32 position[3];
  struct Actor *hit;
  u32 i;
  s32 angle = object->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  FieldVector_AddPolarOffset(0x100000, angle, position);
  hit = Func_02005680(position, 1);
  if (hit != 0) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)hit->sprite)->value28 ==
        gData_02005f58[i])
      return;
    i++;
    if (i <= 5)
      goto next;
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  if (Func_020054f8(object, position) > 0)
    FieldActor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}

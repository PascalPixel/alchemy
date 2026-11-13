// ---- games/tla/src/scene/mars_lighthouse_room_f/s3d80.c ----
#include "../shared/routines.h"
#include "../shared/tables.h"
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "event_work.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/field/field.h"
#include "module/field/field_effect.h"
#include "module/scene/mars_lighthouse_room_f.h"
#include "module/scene/scene.h"
#include "module/system/system.h"

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

DEFINE_FUNC_02001E24(Func_02003d80, Func_020035b8)

#include "room.h"

void GameFlag_SetByte(s32 flag, s32 value);
void Func_02003b78(struct WarpWork *work, struct WarpEntry *entry);
void Func_02003be8(struct WarpWork *work, struct WarpEntry *entry);

s32 Func_02003f18(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct WarpWork *work = &gData_020070a8;
  struct WarpEntry *entry = work->entries;
  s32 index;

  index = 0;
  if (index >= work->count)
    goto end;
top:
  if (entry->x == x && entry->z == z) {
    if (!Event_GetFlag(entry->flag)) {
      Func_02003b78(work, entry);
      Event_SetFlag(entry->flag);
      work->current = index;
      goto end;
    }
    if (index == work->current)
      goto end;
    work->current = index;
    Func_02003be8(work, entry);
    GameFlag_SetByte(work->positionFlag, x);
    GameFlag_SetByte(work->positionFlag + 8, z);
    return 1;
  }
  index++;
  entry++;
  if (index < work->count)
    goto top;
end:
  return 0;
}

void Func_02003fc8(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u8 *event = Ram_HeapSlots->eventWork;
  struct WarpWork *work = &gData_020070a8;
  s32 x = FieldGameFlag_GetByte(work->positionFlag);
  s32 z = FieldGameFlag_GetByte(work->positionFlag + 8);

  if (x != 0 || z != 0) {
    struct Actor *other;

    GameFlag_SetByte(work->positionFlag, 0);
    GameFlag_SetByte(work->positionFlag + 8, 0);
    object->x = ((x + dx) << 20) + 0x80000;
    object->z = ((z + dz) << 20) + 0x80000;
    other = *(struct Actor **)(event + 460);
    other->x = object->x;
    other->z = object->z;
    Map_CenterOnFollowedPoint();
    Func_02003d80();
    WORD_AT(Ram_HeapSlots->eventWork, 428) = value;
  }
}

#include "input.h"

/* VIEW(s3d80): variable gData_02005ef8 as extern const s16 gData_02005ef8[]; the definition has s32 gData_02005ef8[] = { 0xffff, - 0x8000, (s32) 0xe000c000, (s32) 0xc000a000, 0x20004000, 0x40006000, 0xffff, - 0x8000, }; tried as the definition's extern struct Trigger gData_02005ef8[], it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_f/s3d80.c:160: incompatible types in assignment. */
extern const s16 gData_02005ef8__s3d80[] __asm__("gData_02005ef8");

DEFINE_FUNC_02002D04(Func_02004080, gData_02005ef8__s3d80, Func_02005480)

DEFINE_FUNC_02003284(Func_0200410c)

#include "fixed_math.h"
#include "scene_tables.h"
#include "system/iwram_call.h"

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

/* VIEW(s3d80): variable gData_02005f38 as extern s16 gData_02005f38[]; the definition has s32 gData_02005f38[] = { 0xffff, - 0x8000, 0xc000, (s32) 0xc0008000, 0x4000, 0x40008000, 0xffff, - 0x8000, }; tried as the definition's extern s32 gData_02005f38[], it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_f/s3d80.c:243: `gData_02005f38__s3d80' undeclared (first use in this function). */
extern s16 gData_02005f38__s3d80[] __asm__("gData_02005f38");

void Func_02004140(void) {
  struct Position probe;
  s32 startBehavior;
  s32 x;
  s32 z;
  s32 startX;
  s32 startZ;
  u8 *layer;
  s32 radius;
  struct Actor *actor;
  struct Position *position;
  struct Position *next;
  s32 heading;
  s32 nextBehavior;
  s32 distance;
  struct Actor *other;
  u8 *flags;
  s32 i;
  s32 dx;
  s32 dz;

  actor = ObjectTable_Get(gMainRoutineWork.currentOwner);

top:
  Func_0200410c();
  actor->speedLimit = PIXELS(1);
  actor->acceleration = PIXELS(1) / 2;
  actor->targetX = 0x80000000;
  actor->targetZ = 0x80000000;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  position = &probe;
  position->x = startX = (actor->x & 0xfff00000) + PIXELS(8);
  position->y = actor->y;
  position->z = startZ = (actor->z & 0xfff00000) + PIXELS(8);
  x = startX;
  z = startZ;
  layer = &actor->terrainId;
  startBehavior = MapGrid_GetCellBehaviorAt(*layer, x, z);
  dz = z - actor->z;
  dx = x - actor->x;
  distance = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz);
  if (distance < PIXELS(4)) {
    heading = gData_02005f38__s3d80[(gInput.held >> 4) & 15];
    if ((heading << 16) == -0x10000) {
      return;
    }
  } else {
    heading = (s16)ArcTan2(z - actor->z, x - actor->x);
    heading = (s16)((heading + 0x2000) & -0x4000);
  }
  FieldVector_AddPolarOffset(PIXELS(16), heading, &probe);
  nextBehavior = MapGrid_GetCellBehaviorAt(*layer, position->x, position->z);
  if (nextBehavior == 255 ||
      Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y >
          PIXELS(8)) {
    actor->facing = heading;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->x = startX;
    actor->z = startZ;
    goto done;
  }
  position->x = startX;
  position->z = startZ;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = 0x1999;
  actor->timer = 0;
  FieldActor_SetMoveTarget(actor, startX, actor->y, startZ);
  Func_02005480(actor, 2);
  Actor_SetAnimationSpeed(actor, 48);
  Actor_WaitForMove(actor);
  actor->update = Func_02004080;
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
       * next and actor; as no asm, 85 of 304 instructions differ in tla-en. */
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
  {
    struct Position *step;

    step = &probe;
    FieldVector_AddPolarOffset(PIXELS(16), heading, step);
    /* FAKEMATCH(dead): the unread store ends CSE's knowledge of the address, so
     * next rebuilds it; as no store, 59 of 304 instructions differ in tla-en.
     */
    step = NULL;
  }
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

#include "version.h"

#include "call.h"

void Func_020043ec(void) {
  struct Actor *object;
  s32 startCell;
  s32 targetX;
  s32 targetZ;
  s32 x;
  s32 z;
  u8 *layer;
  s32 radius;
  s32 position[3];
  s32 dx, dz;
  s32 cell;
  s16 angle;
  s32 i;
  s32 j;
  struct Actor *entry;
  u8 *flags;
  s32 *q;
  s32 *p;

  /* FAKEMATCH(volatile): gData_020074c8 read as a volatile s16, reread after
   * each test and for lsls #16; as a plain u16, 54 of 347 instructions differ
   * in tla-en. */
  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  gData_020074c8 = 0;
retry:
  Func_0200410c();
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  p = position;
  p[0] = x = (object->x & 0xfff00000) + 0x80000;
  p[1] = object->y;
  p[2] = z = (object->z & 0xfff00000) + 0x80000;
  targetZ = z;
  targetX = x;
  layer = &object->terrainId;
  startCell = GetMapCellCollision(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = gData_02005f38__s3d80[(gInput.held >> 4) & 15];
    // -1 for no direction, tested as its halfword.
    if ((u16)angle == 0xffff)
#if !TLA_PATCH_1
      return;
#else
      goto end;
#endif
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  FieldVector_AddPolarOffset(0x100000, angle, position);
  cell = GetMapCellCollision(*layer, p[0], p[2]);
  if (cell == 255 ||
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y > 0x80000) {
    object->facing = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  p[0] = x;
  p[2] = z;
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  object->timer = 0;
  Actor_SetMoveTarget(object, x, object->y, z);
  Func_02005480(object, 2);
  Actor_SetAnimationSpeed(object, 48);
#if !TLA_PATCH_1
  Actor_WaitForMove(object);
#else
  for (i = 0; i <= 179 && Func_02005530(object) == 0; i++) {
    if (gData_020074c8 != 0)
      goto end;
    System_Wait(1);
  }
#endif
  object->update = (void (*)(void))Func_02004080;
  goto walk;
walkNext:
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  if (gData_020074c8 != 0)
#if !TLA_PATCH_1
    goto blocked;
#else
    goto end;
#endif
  radius = object->radius - 2;
  entry = Ram_HeapSlots->scriptObjects;
  j = 0;
  flags = &entry->collisionFlags;
overlap:
  if (entry->script != NULL && (1 & *flags) && entry != object) {
    /* FAKEMATCH(wrapper): the overlap call goes through Value4; as a direct
     * call, 2 of 347 instructions differ in tla-en. */
    if (Value4((s32 (*)())Func_02005538, (s32)&entry->x, radius, (s32)position,
               entry->radius - 2) >= 0)
      goto blocked;
  }
  j++;
  flags += sizeof(*entry);
  entry++;
  if (j <= 63)
    goto overlap;
  targetX = q[0];
  targetZ = q[2];
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  Actor_SetMoveTarget(object, q[0], q[1], q[2]);
  i = 0;
  goto test;
next:
  System_Wait(1);
  i++;
test:
  if (i <= 179 && Func_02005530(object) == 0 && gData_020074c8 == 0)
    goto next;
  if (gData_020074c8 != 0)
    goto end;
  if (cell != startCell)
    goto changed;
walk:
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse hoisting the
   * position's address; as no asm, 94 of 347 instructions differ in tla-en. */
  __asm__("" : : : SFP_CLOBBER);
  FieldVector_AddPolarOffset(0x100000, angle, position);
  q = position;
  cell = GetMapCellCollision(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Actor_SetMoveTarget(object, targetX, object->y, targetZ);
  Actor_WaitForMove(object);
  System_Wait(2);
  goto retry;
changed:
  System_Wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_02005480(object, 1);
}

// ---- games/tla/src/scene/mars_lighthouse_room_f/s4700.c ----

void Func_02004700(s32 value) { gData_020074c8 = value; }

#include "system/main_routine_work.h"

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
/* VIEW(s4700): variable gData_02005f64 as extern s8 gData_02005f64[][4]; the definition has u32 gData_02005f64[] = { 0x820f8e0, 0x2008e0f8, 0x20f0e0, 0x2008e0f8, 0x820f8e0, 0x2008e0f8, }; tried as the definition's extern u32 gData_02005f64[], it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_f/s3d80.c:668: subscripted value is neither array nor pointer. */
extern s8 gData_02005f64__s4700[][4] __asm__("gData_02005f64");

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
  width = (ABS(gData_02005f64__s4700[j][0]) + ABS(gData_02005f64__s4700[j][2])) >> 4;
  height = (ABS(gData_02005f64__s4700[j][1]) + ABS(gData_02005f64__s4700[j][3])) >> 4;
  x = (object->x + (gData_02005f64__s4700[j][0] << 16)) >> 20;
  z = (object->z + (gData_02005f64__s4700[j][1] << 16)) >> 20;
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

/* VIEW(s4700): variable gData_02005f7c as extern s32 gData_02005f7c[]; the definition has s16 gData_02005f7c[] = { 0, 16, 0, 16, 0, 16, 16, 0, 16, 0, 16, 0, 16, 0, 0, - 16, 0, - 16, 0, - 16, 0, - 16, - 16, 0, - 16, 0, - 16, 0, - 1...; tried as the definition's extern s16 gData_02005f7c[], Func_02004a8c moves: 12 of 171 instructions, Func_02004bf4 moves: 41 of 132 instructions, Func_02004d10 moves: 27 of 304 instructions in tla-ja. */
extern s32 gData_02005f7c__s4700[] __asm__("gData_02005f7c");

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
    width = (ABS(gData_02005f64__s4700[footprint][0]) +
             ABS(gData_02005f64__s4700[footprint][2])) >>
            4;
    height = (ABS(gData_02005f64__s4700[footprint][1]) +
              ABS(gData_02005f64__s4700[footprint][3])) >>
             4;
    dx = gData_02005f7c__s4700[direction] & 0xffff0000;
    dz = gData_02005f7c__s4700[direction] << 16;
    target->callback = 0;
    count = 0;
    layer = &object->terrainId;
    *layer = 2;
    target->x = object->x + dx;
    target->z = object->z + dz;
    position[1] = object->y;
    for (;;) {
      position[2] = target->z + (gData_02005f64__s4700[footprint][1] << 16);
      for (row = 0; row < height; row++) {
        position[0] = target->x + (gData_02005f64__s4700[footprint][0] << 16);
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

DEFINE_FUNC_02000DAC(Func_02004bf4, gData_02005f58, gData_02005f7c__s4700,
    gData_02005f64__s4700)

#undef PIXELS

DEFINE_FUNC_02003A5C(Func_02004d10, gData_02005f64__s4700, Func_02004a44,
    Func_02004fc0, Func_02005480, gData_02005f7c__s4700)

DEFINE_FUNC_02004774(Func_02004fc0, Func_02005680, gData_02005f58,
    Func_020054f8)
#undef ABS
#undef PIXELS
#define PIXELS(value) ((value) << 16)

// ---- games/tla/src/scene/mars_lighthouse_room_f/s5048.c ----
#define MapLayer MapLayer__s5048
#include "map.h"
#undef MapLayer
/* VIEW(s5048): tag struct MapLayer as struct MapLayer__s5048; room.h has another definition; not tried: struct MapLayer__s5048 was not found. */
#include "field_event.h"
#include "vram_tab.h"

/// Queues the lit actors' sprites: for each listed actor with a position, projects it
/// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
/// 64 one when the big switch is set, at its animation picture and depth.
DEFINE_FUNC_02000AAC(Func_02005048, gData_020074cc, Data_0200f510,
    gData_020074ce, gData_020074d0, gData_020075d0, gData_020075d2,
    Func_02005698, Func_02005440)

#include "iwram_call.h"
#include "task.h"

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_FUNC_02000A98(Func_02005204, gData_020074d0, gData_020074cc,
    Func_02005048, gData_020074ce, gData_020075d0, gData_020075d2)

DEFINE_FUNC_02000CE8(Func_02005284, gData_020074d0, gData_020074cc,
    Func_02005048, gData_020074ce, gData_020075d0, gData_020075d2)

DEFINE_FUNC_02000D68(Func_02005304, gData_020074d0, gData_020074cc,
    Func_02005048, gData_020074ce, gData_020075d0, gData_020075d2)

DEFINE_FUNC_02000DF0(Func_0200538c, gData_020074ce, gData_020074d0)

void Func_020053bc(s32 value) { gData_020075d2 = value; }

#include "objcmd.h"

void Func_0200004c(void);
void Func_020006f4(void);
void Func_02003478(void);
void Func_0200351c(void);

s32 gData_020056c8[] = {
    SC_CALL(Func_0200004c),
    SC_END,
};

// clang-format off
DEFINE_G_DATA_020056D4(gData_020056d4, 680, 760)

DEFINE_G_DATA_020056D4(gData_02005748, 584, 664)

u32 gData_020057bc[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_LABEL(1),
    SC_WAIT(1),
    SC_FLAG_CHECK(0x220),
    SC_IFNOT(SC_TO(1)),
    SC_LABEL(2),
    SC_MOVE(PIXELS(176), 0, PIXELS(576)),
    SC_SYNC,
    SC_MOVE(PIXELS(248), 0, PIXELS(576)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(176), 0, PIXELS(576)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(576)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(2)),
};

u32 gData_02005890[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(32)),
    SC_SET(SW_GROUND, PIXELS(32)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(88), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LABEL(1),
    SC_MOVE(PIXELS(168), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(88), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

u32 gData_0200593c[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, PIXELS(32)),
    SC_SET(SW_GROUND, PIXELS(32)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_LABEL(1),
    SC_MOVE(PIXELS(264), PIXELS(32), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(80), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(232), PIXELS(80), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(80), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(32), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(360), PIXELS(32), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

DEFINE_G_DATA_02004FF0(gData_02005a3c, 2)

u32 gData_02005a88[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(968), 0, PIXELS(80)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LABEL(1),
    SC_MOVE(PIXELS(872), 0, PIXELS(80)),
    SC_SYNC,
    SC_MOVE(PIXELS(776), 0, PIXELS(80)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(872), 0, PIXELS(80)),
    SC_SYNC,
    SC_MOVE(PIXELS(968), 0, PIXELS(80)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

s16 gData_02005b5c[] = {
    24, 25, 26, 24, 25, 0,
};

u32 gData_02005b68[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02005bac[] = {
    0, 42, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u16 gData_02005bf0[] = {
    8, 67, 0,
};

u16 gData_02005bf6[] = {
    10, 1, 0,
};

u16 gData_02005bfc[] = {
    18, 17, 19, 49, 20, 19, 21, 3,
    0,
};

u16 gData_02005c0e[] = {
    22, 0, 0,
};

u16 gData_02005c14[] = {
    13, 17, 14, 33, 15, 49, 16, 131,
    0,
};

u16 gData_02005c26[] = {
    19, 1, 0,
};

u16 gData_02005c2c[] = {
    13, 3, 0,
};

u16 gData_02005c32[] = {
    14, 1, 0,
};

u16 gData_02005c38[] = {
    8, 3, 9, 3, 10, 3, 0,
};

u16 gData_02005c46[] = {
    12, 0, 0,
};

u16 gData_02005c4c[] = {
    13, 3, 14, 3, 15, 3, 0,
};

u16 gData_02005c5a[] = {
    10, 0, 0,
};

u16 gData_02005c60[] = {
    24, 3, 25, 3, 26, 3, 0,
};

u16 gData_02005c6e[] = {
    18, 0, 19, 0, 20, 0, 21, 0,
    22, 0, 0,
};

DEFINE_G_DATA_020054CC(gData_02005c84, Func_02003478, 0xf80000, 0x200000,
    0x2a00000, 0x160000, 0xf80000, 0x2b00000, Func_020006f4, Func_0200351c)

DEFINE_G_DATA_020054CC(gData_02005ce0, Func_02003478, 0xb80000, 0x400000,
    0x2300000, 0x360000, 0xb80000, 0x2400000, Func_02000d80, Func_0200351c)

DEFINE_G_DATA_0200547C(gData_02005d3c, Func_02003478, 872, 128, 22, 872, 144,
    Func_0200351c)

DEFINE_G_DATA_02001540(gData_02005d8c, 30)

u32 gData_02005dbc[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02005e00[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

DEFINE_G_DATA_020012A4(gData_02005e44, 22)

DEFINE_G_DATA_020012A4(gData_02005e80, 44)

DEFINE_G_DATA_020012A4(gData_02005ebc, 126)

s32 gData_02005ef8[] = {
    0xffff, -0x8000, (s32)0xe000c000, (s32)0xc000a000, 0x20004000, 0x40006000,
    0xffff, -0x8000,
};

s16 gData_02005f18[] = {
    -1, 0, -0x8000, -1, -0x4000, -1, -1, -0x4000,
    0x4000, -1, -1, 0x4000, -1, 0, -0x8000, -1,
};

s32 gData_02005f38[] = {
    0xffff, -0x8000, 0xc000, (s32)0xc0008000, 0x4000, 0x40008000,
    0xffff, -0x8000,
};

u16 gData_02005f58[] = {
    258, 259, 293, 294, 332, 331,
};

u32 gData_02005f64[] = {
    0x820f8e0, 0x2008e0f8, 0x20f0e0, 0x2008e0f8, 0x820f8e0, 0x2008e0f8,
};

DEFINE_G_DATA_0200198C(gData_02005f7c)
// clang-format on

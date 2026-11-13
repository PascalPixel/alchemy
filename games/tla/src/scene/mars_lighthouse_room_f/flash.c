// ---- games/tla/src/scene/mars_lighthouse_room_f/flash.c ----
#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"
#include "event_inline.h"

struct RideEntry {
  s16 a;
  u8 unk02[2];
  s16 b;
  s16 c;
  u8 unk08[4];
  s32 d;
};

struct RideRow {
  u8 unk00[20];
  s16 a;
  u8 unk16[2];
  s16 b;
  s16 c;
  u8 unk1c[4];
  s32 d;
};

struct RideSlot {
  u16 value;
  u8 unk02[2];
};

struct RideWork {
  s32 counter;
  s16 active;
  u8 unk06[8];
  s16 value0e;
  u8 unk10[4];
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unk64[8];
  struct Actor *unk6c;
  void *handle;
};

extern u8 gData_020023c4[];
extern u16 gData_02004cf0[];
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_020041ec(void *handle);
void Func_02002148(void);
void Func_0200236c(struct Actor *object, s32 unused);
void Func_02002ad0(struct Actor *object);

void Func_02002954(struct Actor *object) {
  switch (gFrameCount & 7) {
  case 0:
    Animation_ApplyChildValues(object, 10);
    break;
  case 4:
    Animation_ApplyChildValues(object, 0);
    break;
  }
}

#include "version.h"

#include "call.h"
#include "main_routine_work.h"
#include "vector3.h"
#include "fixed_math.h"

struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[0x68 - 6];
  s16 unk68;
  s16 unk6a;
};

void Func_02002800(void);
void Event_Start(void);
void Func_02005480(struct Actor *object, s32 mode);
s32 Func_020054d8(s32 terrain, s32 x, s32 z);
s32 Func_020054f8(struct Actor *actor);
void Actor_JumpDownLedge(s32 owner, s32 a, s32 b);
s32 Func_02005538(s32 *from, s32 radius, struct Vector3 *to, s32 radius2);

#if TLA_PATCH_1
void Func_0200297c(void) {
#else
s32 Func_0200297c(void) {
  /* FAKEMATCH(signature): declared returning s32, though it returns nothing,
   * so the return register's clobber and use rank the last store's address
   * copy ahead of its zero; as void, 2 of 186 instructions differ in
   * tla-ja. */
#endif
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct Work020023c4 *work;
#if TLA_PATCH_1
  struct Vector3 position;
#else
  /* FAKEMATCH(temporary): the position held in a union beside a halfword;
   * as a plain struct Vector3, 16 of 186 instructions differ in tla-ja. */
  union {
    struct Vector3 v;
    s16 h;
  } positionU;
#define position positionU.v
#endif

  if (main->renderMode == 0) {
    struct Actor *actor;

    // UB: Func_02002800 takes no arguments and returns nothing; the bytes call
    // it with two through an int-returning type.
#ifdef UBFIX
    Func_02002800();
#else
    ((s32 (*)(s32, s32))Func_02002800)(-10, 0);
#endif
    actor = Actor_Get(main->currentOwner);
    work = (struct Work020023c4 *)(gData_020023c4 + 0x188);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    actor->update = Func_02002954;
    actor->facing = TRIG_TURN / 4;
    Func_02005480(actor, 49);
    for (;;) {
      if (Func_020054d8(actor->terrainId, actor->x, actor->z) == 7) {
        s32 i;

        /* FAKEMATCH(temporary): velocityX stored through a union; as the plain
         * store, 2 of 236 instructions differ in tla-en (2 of 186 in
         * tla-ja). */
        ((union {
           s32 value;
           u16 half;
         } *)&actor->velocityX)
            ->value = work->unk68 << 17;
        actor->velocityZ = work->unk6a << 17;
        actor->acceleration = Q16_ONE / 16;
        for (i = 0; i <= 9; i++) {
          position.x = actor->x + (work->unk68 << 19);
          position.y = actor->y;
          position.z = actor->z + (work->unk6a << 19);
          if (Func_020054f8(actor) != 0) {
            actor->velocityX = 0;
            actor->velocityZ = 0;
            break;
          }
          System_Wait(1);
        }
        break;
      } else {
        s32 result;
#if TLA_PATCH_1
        s32 radius;
        struct Actor *other;
        u8 *flags;
        s32 x, z, off;
        s32 i;
#else
        s32 x, z, off, oldZ;
#endif

        position.x = actor->x + (work->unk68 << 19);
        position.y = actor->y;
        position.z = actor->z + (work->unk6a << 19);
        result = Func_020054f8(actor);
        if (result > 0)
          break;
        if (result < 0) {
          actor->facing = TRIG_TURN / 4;
          Actor_JumpDownLedge(gMainRoutineWork.currentOwner, 6, -1);
          break;
        }
#if TLA_PATCH_1
        radius = actor->radius - 2;
        other = Ram_HeapSlots->scriptObjects;
        flags = &other->collisionFlags;
        for (i = 0; i <= 63; i++, flags += 128, other++) {
          /* FAKEMATCH(wrapper): Func_02005538 forwarded through Value4; as a
           * direct call, 110 of 236 instructions differ in tla-en. */
          if (other->script != 0 && (*flags & 1) && other != actor &&
              Value4((s32 (*)())Func_02005538, (s32)&other->x, radius,
                     (s32)&position, other->radius - 2) >= 0) {
            System_Wait(1);
            goto done;
          }
        }
#endif
        // A once-only block: its end holds the order around it (as the plain
        // statement, 6 of 236 instructions differ in tla-en; tla-ja matches
        // either way).
        do {
          x = actor->x += work->unk68 << 17;
        } while (0);
#if TLA_PATCH_1
        z = actor->z + (work->unk6a << 17);
#else
        // The old z read in a once-only block, whose end keeps the read ahead
        // of the step's (as a plain statement, 2 of 186 instructions differ
        // in tla-ja; as the patch-1 sum, 4).
        do {
          oldZ = actor->z;
        } while (0);
        z = oldZ + (work->unk6a << 17);
#endif
        actor->z = z;
        off = (x & 0xfffff) + 0xfff80000;
        if (off > Q16_ONE)
          off = Q16_ONE;
        if (off < (s32)0xffff0000)
          off = 0xffff0000;
        actor->x = x - off;
        off = (z & 0xfffff) + 0xfff80000;
        if (off > Q16_ONE)
          off = Q16_ONE;
        if (off < (s32)0xffff0000)
          off = 0xffff0000;
        actor->z = z - off;
        work->unk04 = 0;
        System_Wait(1);
      }
    }
#if TLA_PATCH_1
  done:
#endif
    actor->update = 0;
    Animation_ApplyChildValues(actor, 0);
  }
  work->unk04 = 0;
}
#if !TLA_PATCH_1
#undef position
#endif

void Func_02002b88(void) {
  if (((struct RideWork *)(gData_020023c4 + 0x188))->active)
    Func_0200297c();
}

#include "room.h"
#include "event_work.h"

void ScreenTransition_Start(s32 mode, s32 frames);

s32 Func_02002ba0(void) {
  /* FAKEMATCH(signature): declared returning s32 with no return; as void, 2
   * of 138 instructions differ in tla-en. */
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct RideWork *ride;
  struct Actor *leader;
  // Unused: gives the stack frame its 12 bytes.
  UNUSED s32 unused[3];

  // Read unsigned: as event->screenStatus, 18 of 138 instructions differ in
  // tla-en.
  if (*(u16 *)&event->screenStatus == 0) {
    ScreenTransition_Start(event->screenEffect, event->delay);
    event->screenStatus = 1;
  }
  Flag_Clear(0x106);
  event->unk166 = 0;
  event->unk15a = 0;
  *(u16 *)&event->unk15c[2] = 0;
  *(u16 *)&event->unk15c[0] = 0;
  event->encounterEnemyGroup = 0;
  // UB: Func_02002800 takes no arguments and returns nothing; the bytes call
  // it with two through an int-returning type.
#ifdef UBFIX
  Func_02002800();
#else
  /* FAKEMATCH(wrapper): called through a cast to the int-returning
   * two-argument type, which sets r1 before negating r0; as Func_02002800(),
   * 3 of 138 instructions differ in tla-en. */
  ((s32 (*)(s32, s32))Func_02002800)(-10, 0);
#endif
  FieldCamera_SetSpeed(0x70000, 0xe000);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ride = (struct RideWork *)(gData_020023c4 + 0x188);
  Flag_Set(0x106);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Flag_Clear(0x106);
  leader->update = (void (*)(void))Func_02002954;
  leader->facing = 0x4000;
  Func_02005480(leader, 49);
  while (ride->unk6c->timer == 0) {
    if (Func_020054d8(0, leader->x, leader->z) != 7) {
      leader->x += ((s32 *)ride->unk6c->unk70)[0];
      leader->z += ((s32 *)ride->unk6c->unk70)[2];
    } else {
      if (((s32 *)ride->unk6c->unk70)[0] != 0)
        leader->x = (leader->x & 0xfff00000) + 0x80000;
      if (((s32 *)ride->unk6c->unk70)[2] != 0)
        leader->z = (leader->z & 0xfff00000) + 0x80000;
    }
    ride->active = 0;
    System_Wait(1);
  }
  FieldEvent_Wait(30);
  leader->update = 0;
  Animation_ApplyChildValues(leader, 0);
  ride->active = 0;
}

s32 Func_02002ce0(void) {
  if (((struct RideWork *)(gData_020023c4 + 0x188))->active) {
    Func_02002ba0();
    return 1;
  }
  return 0;
}

s32 Func_02002cfc(void) {
  return ((struct RideWork *)(gData_020023c4 + 0x188))->active;
}

#include "iwram_call.h"
#include "field.h"

struct Sprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 scale; ///< 16.16, as FieldSprite has it
  u8 vramBlock;
  u8 retainCacheEntry : 1;
  u8 unk11 : 7;
  u16 rotation;
  u8 unk14[2];
  u8 unk16[5];
  u8 unk1b;
  u8 unk1c[12];
  void *record;
};

struct Work {
  u8 unk00[12];
  u16 vramBlock;
  u8 unk0e[98];
  struct Actor *owner;
};

extern u8 gData_02005dbc[];
s32 Resource_ResetEntry(u32 slot);

#define WORK ((struct Work *)(gData_020023c4 + 0x188))

void Func_02002d08(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;

  if (work->owner == NULL)
    return;
  // A once-only loop, as the original runs it.
  for (i = 0; i >= 0; i--) {
    struct Actor *object;
    s32 a, b, c, d, e;
    s32 x, y, z;

    a = Random16Far();
    b = Random16Far();
    x = actor->x + a * 16;
    x -= b * 16;
    c = Random16Far();
    d = Random16Far();
    e = Random16Far();
    z = actor->z + d * 16 - e * 16;
    y = c * 8 + actor->y + Q16_ONE * 4;
    object = (struct Actor *)Import_ActorCreate(489, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      struct Sprite *sprite;
      struct Sprite *source;

      angle = Random16Far();
      radius = Random16Far();
      sum = Random16Far();
      sum += Random16Far();
      scale = (sum >> 2) + Q16_ONE / 2;
      Actor_StartScript(object, gData_02005dbc);
      Actor_SetSingleChildFlags(object, 0);
      object->velocityY = angle * 4 + Q16_ONE * 2;
      radius <<= 3;
      object->velocityZ = Iwram_MulQ16(radius, Math_Cosine(angle));
      object->velocityX = Iwram_MulQ16(radius, Math_Sine(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->bounce = Q16_ONE / 2;
      sprite = object->sprite;
      source = work->owner->sprite;
      Resource_ResetEntry(sprite->vramBlock);
      sprite->retainCacheEntry = 1;
      sprite->vramBlock = work->vramBlock;
      ResourceMetadata_ClearRecord(sprite->record);
      sprite->record = NULL;
      sprite->unk1b = 0;
      sprite->unk14[0] = source->unk14[0];
      sprite->unk14[1] = source->unk14[1];
      sprite->shape = source->shape;
      sprite->size = source->size;

      sprite->tile = source->tile;
    }
  }
}

extern const u8 gData_02005e00[];

/// Throws two sparks (object kind 325) from (x, y, z), each upward at about
/// speed plus 0x14000 with a random spread, at a random angle and size.
void Func_02002e8c(s32 x, s32 y, s32 z, s32 speed) {
  s32 i;

  i = 1;
next:
  {
    struct Actor *object = FieldActor_Create(325, x, y, z);

    if (object != NULL) {
      s32 angle;
      s32 rise;
      s32 radius;
      u32 size;
      s32 scale;

      rise = speed + (Random16Far() >> 4) - (Random16Far() >> 4);
      angle = 0x4000 + rise;
      radius = Random16Far() * 8;
      size = Random16Far();
      Actor_StartScript(object, gData_02005e00);
      Actor_SetSingleChildFlags(object, 0);
      /* FAKEMATCH(temporary): the scale is taken before the velocity stores, as
       * in the native; as object->scaleX = (size >> 1) + 0x4000, 14 of 98
       * instructions differ in tla-en. */
      scale = (size >> 1) + 0x4000;
      object->velocityY = rise + 0x14000;
      object->velocityZ = Iwram_MulQ16(radius, Math_Cosine(angle));
      object->velocityX = Iwram_MulQ16(radius, Math_Sin(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = object->scaleX;
      object->bounce = 0x8000;
      Animation_ApplyChildValues(object, 1);
    }
  }
  if (--i >= 0)
    goto next;
}

void Func_02002f6c(struct Actor *object) {
  object->movementState = Event_GetFlag(260);
}
#undef position
#undef WORK

// ---- games/tla/src/scene/mars_lighthouse_room_f/s2f80.c ----
#define MapLayer MapLayer__s2f80
#include "map.h"
#undef MapLayer
/* VIEW(s2f80): tag struct MapLayer as struct MapLayer__s2f80; room.h has another definition; not tried: struct MapLayer__s2f80 was not found. */

#define ABS(x) ((x) < 0 ? -(x) : (x))

/* VIEW(s2f80): tag struct Work020023c4 as struct Work020023c4__s2f80; flash.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mars_lighthouse_room_f/flash.c:602: structure has no member named `unk6c'. */
struct Work020023c4__s2f80 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[4];
  u16 unk0a;
  u8 unk0c[0x6c - 0x0c];
  struct Actor *unk6c;
};

struct WorkEvent {
  u8 unk000[340];
  u16 unk154;
  u8 unk156[358 - 342];
  s16 unk166;
};

s32 Func_020054d8(s32 terrain, s32 x, s32 z);
void Func_02002d08(struct Actor *actor);
void Func_02002e8c(s32 x, s32 y, s32 z, s32 speed);
extern u8 gData_020023c4[];

void Func_02002f80(struct Actor *actor) {
  struct Vector3 position;
  struct WorkEvent *work = Ram_HeapSlots->eventWork;
  s32 state = actor->timer;

  if (state == 0) {
    s32 dz;
    s32 dx;
    s32 tileX;
    struct MapCell *cellsA;
    struct MapCell *cellsB;
    s32 widthA;
    s32 widthB;
    s32 offset;
    s32 zB;
    s32 tileZ;
    s32 heightA;
    s32 heightB;
    s32 x;
    s32 z;
    s32 xA;
    s32 xB;
    s32 zA;
    s32 flag;
    struct MapWork *map;
    struct MapCell *cell;
    struct WorkEvent *work2;
    struct Work020023c4__s2f80 *other;
    struct Actor *target;

    flag = Event_GetFlag(0x106);
    if (work->unk166 != 0)
      flag |= 1;
    actor->movementState = flag;

    position.x = actor->x;
    position.y = actor->y;
    position.z = actor->z;
    FieldVector_AddPolarOffset(0x80000, actor->facing, &position);
    if (Func_020054d8(0, position.x, position.z) == 7) {
      actor->targetZ = 0x80000000;
      actor->targetY = 0x80000000;
      actor->targetX = 0x80000000;
      actor->acceleration = 0x4000;
      actor->timer = 1;
      Event_PlaySound(145);
      /* FAKEMATCH(wrapper): forwarded, so each argument builds its own 0x50000;
       * as a direct call, 6 of 362 instructions differ in tla-en. */
      MapGrid_SetShake(0x50000, 0x50000, 0x10000);
      /* FAKEMATCH(wrapper): forwarded, so each argument builds its own -1; as a
       * direct call, 17 of 362 instructions differ in tla-en. */
      MapGrid_SetShake(-1, -1, 0xe666);
      if (actor->watchedObject != NULL)
        ((void (*)(struct Actor *))actor->watchedObject)(actor);
    }

    map = Ram_HeapSlots->mapWork;
    tileX = actor->x >> 20;
    tileZ = actor->z >> 20;
    cellsA = &map->layers[1].cells[tileX + (tileZ << 7)];
    cellsB = &map->layers[2].cells[tileX + (tileZ << 7)];
    widthA = *(s32 *)&map->layers[1].unk08[0] >> 20;
    heightA = *(s32 *)&map->layers[1].unk08[4] >> 20;
    widthB = *(s32 *)&map->layers[2].unk08[0] >> 20;
    heightB = *(s32 *)&map->layers[2].unk08[4] >> 20;

    for (dz = -1, zB = tileZ + heightB - 2, z = tileZ - 1,
        zA = tileZ + heightA - 1;
         dz <= 1; zB++, z++, zA++, dz++) {
      for (dx = -1, xB = tileX + widthB - 1, x = tileX - 1,
          xA = tileX + widthA - 1;
           dx <= 1; xB++, x++, xA++, dx++) {
        offset = ((dz << 7) + dx) * 4;
        cell = (struct MapCell *)(offset + (s32)cellsA);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          MapGrid_CopyCellsLayered(64, 64, 1, 1, xA, zA);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(0x23d);
          Func_02002e8c((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                        actor->facing);
        }
        cell = (struct MapCell *)(offset + (s32)cellsB);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          MapGrid_CopyCellsLayered(64, 64, 1, 2, xB, zB);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(0x23c);
          Func_02002e8c((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                        actor->facing);
        }
      }
    }

    if (actor->scaleX < 0x10000) {
      actor->scaleX += 0x1000;
      actor->scaleY += 0x1000;
    }
    FieldActor_SetMoveTarget(actor, position.x, position.y, position.z);

    work2 = Ram_HeapSlots->eventWork;
    other = (struct Work020023c4__s2f80 *)(gData_020023c4 + 0x188);
    target = Actor_Get(gMainRoutineWork.currentOwner);
    if (ABS(actor->x - target->x) < 0x140000 &&
        (u32)(actor->y - target->y + 0x7ffff) <= 0x1ffffe &&
        ABS(actor->z - target->z) < 0x140000) {
      other->unk04 = 2;
      other->unk6c = actor;
      work2->unk154 = other->unk0a + 2;
    }
  } else if (state == 1) {
    actor->movementState = 0;
    if (actor->scaleX > 0) {
      actor->scaleX -= 0x1000;
      actor->scaleY -= 0x1000;
      Func_02002d08(actor);
    } else {
      actor->z = 0;
      actor->y = 0;
      actor->x = 0;
      actor->velocityZ = 0;
      actor->velocityY = 0;
      actor->velocityX = 0;
      actor->update = NULL;
    }
  }
}
#undef ABS

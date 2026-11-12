#include "types.h"
#include "system.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"
#include "frame.h"
#include "ram_buffer.h"
#include "map.h"

void Func_02003bac(s32 a, s32 b);
void Func_02003bb4(s32 owner, u16 value);
void Field_DispatchTypeHandler(s32 type);
void Func_02003bbc(void);
void Func_02003bc4(void);

void Func_02000c08(s16 value) {
  struct MainRoutineWork *party;

  Func_02003bac(151, 1);
  party = &gMainRoutineWork;
  Func_02003bb4(party->currentOwner, value);
  Field_DispatchTypeHandler(1);
  Func_02003bbc();
  Func_02003bc4();
}

struct MagmaRockEffectParams {
  s32 unk00;
  u8 unk04[0x14];
  u16 kind;
  u8 unk1a[2];
  const void *script;
  u8 unk20[8];
};

extern const u8 gData_02003d74[];

void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 flags, s32 *params);
void Func_02000dc0(struct Actor *object);

void Func_02000c40(struct Actor *object) {
  struct Actor *linked = object->watchedObject;
  s16 *timer = &object->action;

  if (*timer < 50) {
    u32 phase = gFrameCount & 3;

    if (phase == 0) {
      struct MagmaRockEffectParams params;
      s32 dx;
      s32 dy;

      params.kind = 403;
      params.unk00 = 1;
      params.script = gData_02003d74;
      {
        s32 mask = 15;

        dx = mask & Random16Far();
      }
      dy = Random16Far() & 31;
      Func_0200015c(object->x + ((dx - 8) << 16),
                    object->y + (dy << 16) - 0x80000, object->z, 0, phase,
                    phase, 0x320001, &params);
    }
    if (gFrameCount & 4)
      Actor_SetSpriteColor(object, 10);
    else
      Actor_SetSpriteColor(object, 0);
  } else if (*timer < 100) {
    if (gFrameCount & 4)
      Actor_SetSpriteColor(object, 10);
    else
      Actor_SetSpriteColor(object, 0);
  } else if (*timer < 140) {
    if (gFrameCount & 2)
      Actor_SetSpriteColor(object, 10);
    else
      Actor_SetSpriteColor(object, 0);
  } else if (*timer < 144) {
    Actor_SetSpriteColor(object, 7);
  } else {
    Actor_SetSpriteColor(object, 0);
    object->update = NULL;
    linked->x = object->x;
    linked->y = object->y;
    linked->z = object->z;
    linked->update = Func_02000dc0;
  }
  (*timer)++;
}

struct MagmaRockEffectConfig {
  s32 kind;
  s32 variant;
  u8 unk08[16];
  u16 id;
  u8 unk1a[2];
  const void *data;
  u8 unk20[8];
};

extern u32 gData_02003d50[];

void Func_02003c4c(struct Actor *object, s32 value);

/// Every fourth frame, spawns effect 403 at the object with a random z offset
/// and z velocity.
void Func_02000d40(struct Actor *object) {
  u32 frame = gFrameCount;

  if ((frame & 3) == 0) {
    struct MagmaRockEffectConfig config;
    s32 dz;
    s32 velocityZ;
    s32 z;

    config.id = 403;
    config.kind = 2;
    config.data = gData_02003d50;
    if ((frame & 15) == 0 && !Event_GetFlag(608))
      Func_02003c4c(object, 136);
    dz = ((Random16Far() & 15) - 8) << 15;
    velocityZ = ((Random16Far() & 31) - 16) << 12;
    z = object->z;
    Func_0200015c(object->x, object->y, z + dz, 0, 0, velocityZ, 0x320001,
                  &config);
  }
}

struct MagmaRockEventWork {
  u8 unk000[362];
  u16 unk16a;
};

s32 Math_Cosine(s32 angle);
s32 Func_020005a0(s32 *a, s32 *b);

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))

void Func_02000dc0(struct Actor *object) {
  s32 step;
  struct Actor *partner;
  struct Actor *leader;

  step = Math_Cosine(object->angle);
  partner = Actor_Get(HALF_AT(object, 102) + BYTE_AT(object, 98));
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  object->x += step * 2;
  Func_02000d40(object);
  if (leader->x - 0x80000 < object->x + 0x60000 &&
      object->x - 0x60000 < leader->x + 0x80000 &&
      leader->z - 0x80000 < object->z + 0x60000 &&
      object->z - 0x60000 < leader->z + 0x80000 &&
      leader->y < object->y + 0x80000 &&
      object->y - 0x80000 < leader->y + 0xf0000) {
    ((struct MagmaRockEventWork *)Ram_HeapSlots->eventWork)->unk16a = 200;
    object->update = NULL;
    object->x = 0;
    object->y = 0;
    object->z = 0;
  }
  if (Func_020005a0(&object->x, &partner->x) <= 8 &&
      object->y >> 20 == partner->y >> 20) {
    object->x = 0;
    object->update = NULL;
    object->angle ^= 0x9000;
    object->y = 0;
    object->z = 0;
    BYTE_AT(object, 98) ^= 1;
    partner->action = 0;
    partner->update = (void *)Func_02000c40;
    partner->watchedObject = object;
  }
}

// Unless flag 0x109 is set, places the object at the watched object first +
// side, gives it Func_02000dc0 and faces it along the pair first and first + 1.
#include "scene_tables.h"

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);

void Func_02000ee4(u16 id, s16 first, s16 side) {
  struct Actor *object = Actor_Get(id);
  struct Actor *a;
  struct Actor *b;
  u16 left;
  u16 right;
  s32 linked;
  s32 next;
  u8 *flags;
  u8 facing;

  if (Event_GetFlag(0x109))
    return;
  left = first;
  right = side;
  linked = left + right;
  object->x = Actor_Get(linked)->x;
  object->y = Actor_Get(linked)->y;
  object->z = Actor_Get(linked)->z;
  Scene_SetActorSpritePriority(object, 2);
  HALF_AT(object, 102) = first;
  /* FAKEMATCH(temporary): the byte is computed before its address; as
   * BYTE_AT(object, 98) = side ^ 1, 6 of 104 instructions differ in tla-en. */
  facing = side ^ 1;
  BYTE_AT(object, 98) = facing;
  object->update = (void *)Func_02000dc0;
  next = left + 1;
  flags = &object->motionFlags;
  if (right != 0) {
    a = Actor_Get(left);
    b = Actor_Get(next);
    if (a->x < b->x)
      object->angle = FACING_WEST;
    else
      object->angle = 0;
  } else {
    a = Actor_Get(left);
    b = Actor_Get(next);
    if (a->x > b->x)
      object->angle = FACING_WEST;
    else
      object->angle = 0;
  }
  *flags = 0;
}

void Func_02000fcc(struct Actor *object) {
  if (gFrameCount & object->unk5c[7])
    Actor_SetSpriteColor(object, 10);
  else
    Actor_SetSpriteColor(object, 0);
}

/// @file
/// Scene code of overlay 0x67c, in ROM order, from s3844's offset up to the next object the overlay links.

#include "local.h"
#include "types.h"
#include "iwram_call.h"
#include "system/io_reg.h"
#include "event_inline.h"

#define VENT_TOUCHING Func_02003844
#define VENT_BOUNCE Func_0200389c
#include "../../field/object/vent_bounce.c"

void Func_02003994(struct Actor *object, s32 priority) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->priorityFlags = 2;
  ((struct FieldSprite *)object->animation)->priority = priority;
  Actor_SetSingleChildFlags(object, 0);
  Func_02003cc4(object, 2);
  Func_02003ccc(object, gData_020044c8);
  Actor_SetSpriteColor(object, 10);
  object->action = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

void Func_020039f0(s32 *position, s32 base) {
  struct Actor *object = Import_ActorCreate_(0x11d, position[0], position[1],
                                             position[2] - 0x20000);
  s32 angle;

  if (object != 0) {
    object->bounce =
        Math_Cosine(((u32)(Random16Far() * 0x3000) >> 16) + 0x3000);
    object->gravity = (u16)Random16Far();
    object->unk4c = (u16)(Random16Far() << 1) + base;
    /* FAKEMATCH(temporary): the s32 local keeps the zero extension and the pool
     * word 0xffff8000; as a direct store, the add narrows to 16 bits (movs,
     * lsls for 0x8000, no extension), 10 of 50 instructions differ in
     * tla-en. */
    angle = (u16)Random16Far() - 0x8000;
    object->action = angle;
    Func_02003994(object, 3);
    object->update = (void (*)(void))Func_0200389c;
    Animation_SetStateFlags(object, 1);
  }
}

void Func_02003a70(void) {
  struct VentWork *work = VENT_WORK;
  struct VentEntry *entry = work->entries;
  s32 i;

  work->hook();
  for (i = 15; i >= 0; i--, entry++) {
    if (entry->active != 0) {
      if (entry->timer <= 0) {
        Func_020039f0(&entry->x, entry->value);
        entry->timer = 9;
      } else {
        entry->timer--;
      }
    }
  }
}

struct WaterOutlet {
  u16 state;
  u16 index;
  s32 x;
  s32 y;
  s32 z;
  s32 level;
};

struct WaterWork {
  u32 unk00;
  void (*handler)(void);
  struct WaterOutlet outlets[16];
  u16 levels[4];
};

void Func_02003ab0(const u16 *spots, const u16 *levels, void (*handler)(void)) {
  struct WaterWork *work = (struct WaterWork *)(gData_020023c4 + 0x188);
  struct WaterOutlet *outlet = work->outlets;

  if (!Event_GetFlag(0x109)) {
    s32 x;
    s32 i;

    Iwram_ClearWords(work, 0x150);
    x = *spots++;
    i = 0;
    while (x > 0) {
      s32 z = spots[0];
      s32 level = spots[1];

      outlet->state = 0;
      outlet->x = x << 16;
      outlet->z = z << 16;
      outlet->y = Map_GetTerrainHeight(0, x << 16, (z << 16) + 0x100000);
      outlet->level = level << 16;
      outlet->index = i++;
      spots += 2;
      outlet++;
      if (i > 15)
        break;
      x = *spots++;
    }
    if (levels != 0) {
      x = *levels++;
      i = 0;
      if (x > 0) {
      loop2:
        work->levels[i++] = x;
        if (i > 3)
          goto done2;
        x = *levels++;
        if (x > 0)
          goto loop2;
      }
    }
  done2:;
  }
  work->handler = handler;
  if (REG_BLDCNT == 0) {
    {
      s32 alpha = 0xc08;
      REG_BLDALPHA = alpha;
    }
    {
      s32 cnt = 0x3f10;
      REG_BLDCNT = cnt;
    }
  }
  Task_AddFrame(Func_02003a70, 0x480);
}

s32 Func_02003ba4(s32 index) {
  u8 *base = (u8 *)VENT_WORK;

  return ((struct VentRow *)(base + index * 20))->active;
}

s32 Func_02003bb8(s32 index, s32 value) {
  u8 *base = (u8 *)VENT_WORK;

  ((struct VentRow *)(base + index * 20))->active = value;
}

s32 Func_02003bcc(void) { return VENT_WORK->counter; }

s32 Func_02003bd8(s32 limit) {
  struct VentWork *work = VENT_WORK;

  if (Event_GetFlag(0x104) == 0) {
    if (++work->counter >= limit)
      work->counter = 0;
  }
  return work->counter;
}

void Func_02003c00(s32 index, s32 value) {
  u8 *base = (u8 *)VENT_WORK;

  ((struct VentRow *)(base + index * 20))->value = value;
}

void Func_02003c14(s32 i, s32 id) {
  struct VentWork *work = VENT_WORK;

  if ((u32)i <= 3)
    work->ids[i] = id;
}

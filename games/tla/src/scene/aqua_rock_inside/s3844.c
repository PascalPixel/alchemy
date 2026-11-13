/// @file
/// Scene code of overlay 0x67c, in ROM order.

#include "../shared/vents.h"
#include "../shared/tables.h"
#include "../aqua_rock_inside.h"
#include "types.h"
#include "iwram_call.h"
#include "system/io_reg.h"
#include "event_inline.h"
#include "field.h"
#include "module/scene/aqua_rock_inside.h"

#define VENT_TOUCHING Func_02003844
#define VENT_BOUNCE Func_0200389c
#include "../../field/object/vent_bounce.c"

DEFINE_ACTOR_START_SCRIPTED(Actor_StartScripted, Actor_SelectAnimation,
                            gData_020044c8)

void Vent_Puff(s32 *position, s32 base) {
  struct Actor *object = Import_ActorCreate_(0x11d, position[0], position[1],
                                             position[2] - 0x20000);
  s32 angle;

  if (object != 0) {
    object->bounce = Trig_Cos(((u32)(Random16Far() * 0x3000) >> 16) + 0x3000);
    object->gravity = (u16)Random16Far();
    object->unk4c = (u16)(Random16Far() << 1) + base;
    /* FAKEMATCH(temporary): the s32 local keeps the zero extension and the pool
     * word 0xffff8000; as a direct store, the add narrows to 16 bits (movs,
     * lsls for 0x8000, no extension), 10 of 50 instructions differ in
     * tla-en. */
    angle = (u16)Random16Far() - 0x8000;
    object->timer = angle;
    Actor_StartScripted(object, 3);
    object->update = (void (*)(void))Func_0200389c;
    Actor_SetBlendMode(object, 1);
  }
}

DEFINE_VENTS_UPDATE(Vents_Update, Vent_Puff)

void Func_02003ab0(const u16 *spots, const u16 *levels, void (*handler)(void)) {
  struct WaterWork *work = (struct WaterWork *)(gInventorySnapshot + 0x188);
  struct WaterOutlet *outlet = work->outlets;

  if (!Event_GetFlag(0x109)) {
    s32 x;
    s32 i;

    Iwram_ClearWordsEntry(work, 0x150);
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
  Task_AddFrame(Vents_Update, 0x480);
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

DEFINE_VENTS_ADVANCE_COUNTER(Vents_AdvanceCounter)

void Func_02003c00(s32 index, s32 value) {
  u8 *base = (u8 *)VENT_WORK;

  ((struct VentRow *)(base + index * 20))->value = value;
}

DEFINE_VENTS_SET_ID(Vents_SetId)

#include "scene_tables.h"

void Orbiter_Spiral(void);
void Orbit_Update(void);
void Actor_AimAtWatched(void);
void Actor_StepToWatched(void);

// clang-format off
u32 gData_02003f04[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003f40[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003f7c[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003fb8[] = {
    0x424b4243, 0x425b4253, 0x426b4263, 0x51434273, 0x5153514b, 0x5163515b,
    0x5173516b, 0x604b6043,
};

u32 gData_02003fd8[] = {
    0, 6, 38, 17,
};

u32 gData_02003fe8[] = {
    5, 0, 0xa00000, 0, 1, 38,
    17,
};

DEFINE_G_DATA_02003584(gData_02004004, Orbiter_Spiral, 38, 17)

DEFINE_G_DATA_02004014(gData_02004014, Orbit_Update, Actor_AimAtWatched,
    Actor_StepToWatched)

u32 gData_02004034[] = {
    5, 0, 0x180000, 0, 1, 22,
    26, 4, 17,
};

u32 gData_02004058[] = {
    5, 0, 0x100000, 0, 1, 22,
    26, 4, 17,
};

s32 gData_0200407c[] = {
    22, 26, 0, 5, 0, -0x180000,
    0, 1, 22, 26, 0, 17,
};

s32 gData_020040ac[] = {
    22, 26, 0, 5, 0, -0x100000,
    0, 1, 22, 26, 3, 17,
};

u32 gData_020040dc[] = {
    0, 1, 13, 30, 0, 38,
};

u16 gData_020040f4[] = {
    280, 344, 2, 312, 344, 2, 0,
};

u16 gData_02004102[] = {
    8, 0,
};

u16 gData_02004106[] = {
    152, 536, 2, 216, 536, 2, 0,
};

u32 gData_02004114[] = {
    0xb000a, 0,
};

u32 gData_0200411c[] = {
    0, 20, 38,
};

u32 gWaterDropScript[] = {
    23, 6, 0x4000, 0, 1, 13,
    30, 0, 38,
};
// clang-format on

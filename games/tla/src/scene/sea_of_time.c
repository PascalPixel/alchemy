/// @file
/// The scene code of overlay 0x687 that its scene tables belong to, in ROM
/// order; the rest of the overlay is in sea_of_time/.

#include "sea_of_time/sea_of_time.h"

s32 Func_0200033c(struct Actor *object) {
  ((struct SeaOfTimeSprite *)object->sprite)->visible = 0;
  object->terrainId = 1;
  return 1;
}

s32 Func_0200034c(u32 layer, s32 x, s32 y) {
  struct SeaOfTimeMapWork *map =
      (struct SeaOfTimeMapWork *)Ram_HeapSlots->mapWork;
  u8 *cell;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (y << 7)) << 2;
  return (cell[2] << 8) | cell[3];
}

s32 Func_02000388(u32 layer, s32 x, s32 y, s32 value) {
  struct SeaOfTimeMapWork *map =
      (struct SeaOfTimeMapWork *)Ram_HeapSlots->mapWork;
  u8 *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (y << 7)) << 2;
  old = (cell[2] << 8) | cell[3];
  cell[2] = value >> 8;
  cell[3] = value;
  return old;
}

s32 Func_020003d0(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "scene_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_020021e4[];
extern const u8 gData_02002214[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov687Room2)
    return (const struct ActorPlacement *)gData_02002214;
  return (const struct ActorPlacement *)gData_020021e4;
}

void Func_0200043c(void) {
  struct Actor *object = Actor_GetById(8);
  s32 turn[1];
  s32 center[3];
  s32 *pos;
  s32 sine;
  s32 dx;
  s32 dz;
  s32 x;
  s32 z;

  center[0] = ((object->x >> 21) << 21) + 0x100000;
  center[2] = ((object->z >> 21) << 21) + 0x100000;
  pos = &object->x;
  if (Func_020003d0(pos, center) > 4) {
    if (*pos < center[0])
      *pos += 0x3333;
    else
      *pos -= 0x3333;
    if (object->z < center[2])
      object->z += 0x3333;
    else
      object->z -= 0x3333;
    if (Func_0200034c(1, center[0] >> 20, center[2] >> 20) >> 8 == 40) {
      turn[0] = SeaOfTime_Cos(-0x200);
      sine = Math_Sine(-0x200);
    } else {
      turn[0] = SeaOfTime_Cos(0x200);
      sine = Math_Sine(0x200);
    }
    dx = object->x - center[0];
    dz = object->z - center[2];
    x = Iwram_MulQ16(dx, turn[0]) - Iwram_MulQ16(dz, sine);
    z = Iwram_MulQ16(dx, sine) + Iwram_MulQ16(dz, turn[0]);
    x += center[0];
    z += center[2];
    object->x = x;
    object->z = z;
  }
}

struct SeaOfTimeEventWork0154 {
  u8 unk000[0x154];
  s16 unk154;
};

void Func_0200054c(void) {
  struct SeaOfTimeEventWork0154 *work =
      (struct SeaOfTimeEventWork0154 *)Ram_HeapSlots->eventWork;
  struct Actor *object = Actor_GetById(8);
  struct {
    s32 x, y, z;
  } center;
  s32 ends[4];
  struct EffectOptions spark;
  s32 turn[1];
  s32 *pos;
  s32 dx;
  s32 dz;
  s32 x;
  s32 z;
  s32 phase;
  s32 frame;
  s32 *counter;
  s32 c;
  s32 s;
  s32 flip;
  s32 speed;
  s32 px;
  s32 py;
  s32 pz;

  switch (work->unk154) {
  case 30:
    center.x = 0;
    center.z = -1;
    break;
  case 32:
    center.x = -1;
    center.z = 0;
    break;
  case 34:
    center.x = 0;
    center.z = 1;
    break;
  case 36:
    center.x = 1;
    center.z = 0;
    break;
  case 31:
    center.x = -1;
    center.z = -1;
    break;
  case 33:
    center.x = -1;
    center.z = 1;
    break;
  case 35:
    center.x = 1;
    center.z = 1;
    break;
  case 37:
    center.x = 1;
    center.z = -1;
    break;
  }
  center.x = (((object->x >> 21) + center.x) << 21) + 0x100000;
  center.z = (((object->z >> 21) + center.z) << 21) + 0x100000;
  pos = &object->x;
  if (Func_020003d0(pos, &center) > 46) {
    if (*pos < center.x)
      *pos += 0x8000;
    else
      *pos -= 0x8000;
    if (object->z < center.z)
      object->z += 0x8000;
    else
      object->z -= 0x8000;
  }
  if (Func_0200034c(1, center.x >> 20, center.z >> 20) >> 8 == 40) {
    turn[0] = SeaOfTime_Cos(-0x200);
    s = Math_Sine(-0x200);
  } else {
    turn[0] = SeaOfTime_Cos(0x200);
    s = Math_Sine(0x200);
  }
  dx = object->x - center.x;
  dz = object->z - center.z;
  {
    s32 (*mul)(s32 a, s32 b) = Iwram_MulQ16;
    x = mul(dx, turn[0]);
    x -= mul(dz, s);
    z = mul(dx, s);
    z += mul(dz, turn[0]);
  }
  x += center.x;
  z += center.z;
  object->x = x;
  object->z = z;

  counter = &gFrameCount;
  frame = *counter;
  phase = 3;
  phase &= frame;
  if (phase == 0) {
    if ((frame & 10) == 0)
      Sound_Play(0x255);
    spark.priority = 2;
    spark.palette = 7;
    spark.startScaleX = 0xcccc;
    spark.startScaleY = 0xcccc;
    c = Math_Cosine(object->facing);
    {
      s32 sine = Math_Sine(object->facing);
      ends[0] = c * 10;
      ends[1] = sine * 10;
      ends[2] = -(c * 10);
      ends[3] = -(sine * 10);
    }
    ends[0] += object->x;
    ends[1] += object->z;
    speed = 0x8000;
    ends[2] += object->x;
    ends[3] += object->z;
    if (ends[1] != object->z) {
      ends[1] += -0xc0000;
      ends[3] += -0xc0000;
    }
    flip = *counter & 4;
    px = ends[0];
    py = object->y;
    pz = ends[1];
    if (flip) {
      Func_0200015c(px + 0x30000, py, pz, speed, 0, 0, 0xb0000, &spark);
      Func_0200015c(ends[2] - 0x30000, object->y, ends[3], -speed, 0, 0,
                    0xb0000, &spark);
    } else {
      Func_0200015c(px - 0x30000, py, pz, -speed, 0, 0, 0xb0000, &spark);
      Func_0200015c(ends[2] + 0x30000, object->y, ends[3], speed, 0, 0, 0xb0000,
                    &spark);
    }
  }
}

#include "actor.h"
#include "ram_buffer.h"
#include "event_inline.h"
#include "frame.h"
#include "event_work.h"
#include "system/field_effect.h"

s32 Func_02001f9c(struct Actor *actor);

/// Walks actor 8 along a run of cells in the facing the event work names,
/// throwing sparks from both sides of it every fourth frame.
void Func_02000818(void) {
  s32 pos[4];
  struct EffectOptions spark;
  struct EventWork *work;
  struct Actor *actor;
  s16 angle;
  s32 x, z, dx, dz, targetX, targetZ;
  s32 cosine, sine;
  s32 x0, y0, z0, flip;

  work = Ram_HeapSlots->eventWork;
  actor = Actor_GetById(8);
  angle = (work->exit - 45) << 14;
  Func_02001fac();
  Event_ResolvePendingActions(0);
  x = actor->x >> 20;
  z = actor->z >> 20;
  dx = Math_Cosine(angle) >> 16;
  dz = Math_Sine(angle) >> 16;
  actor->speedLimit = 0x40000;
  actor->acceleration = 0x40000;
  targetZ = 0;
  targetX = 0;
  if (Func_0200034c(1, x + (dz < 0 ? -dz : dz), z + (dx < 0 ? -dx : dx)) >> 8 ==
      work->exit) {
    targetX = (dz < 0 ? -dz : dz) * 15 << 16;
    targetZ = (dx < 0 ? -dx : dx) * 15 << 16;
  }
  targetX += x << 20;
  targetZ += z << 20;
  while (Func_0200034c(1, x, z) >> 8 == work->exit) {
    x += dx;
    z += dz;
    targetX += dx << 20;
    targetZ += dz << 20;
    Actor_SetMoveTarget(actor, targetX, actor->y, targetZ);
    while (Func_02001f9c(actor) == 0) {
      s32 phase = 3;
      phase &= gFrameCount;
      if (phase == 0) {
        if ((gFrameCount & 10) == 0) {
          Sound_Play(0x255);
        }
        spark.priority = 2;
        spark.palette = 7;
        spark.startScaleX = 0x16666;
        spark.startScaleY = 0x13333;
        cosine = Math_Cosine(actor->facing);
        sine = Math_Sine(actor->facing);
        pos[0] = cosine * 12;
        pos[1] = sine * 12;
        pos[2] = -(cosine * 12);
        pos[3] = -(sine * 12);
        pos[0] += actor->x;
        pos[1] += actor->z;
        pos[2] += actor->x;
        pos[3] += actor->z;
        if (pos[1] != actor->z) {
          pos[1] -= 0xc0000;
          pos[3] -= 0xc0000;
        }
        flip = gFrameCount & 4;
        y0 = actor->y;
        x0 = pos[0];
        z0 = pos[1];
        if (flip) {
          Func_0200015c(x0 + 0x40000, y0, z0, 0x8000, 0, 0, 0xb0000, &spark);
          Func_0200015c(pos[2] - 0x40000, actor->y, pos[3], -0x8000, 0, 0,
                        0xb0000, &spark);
        } else {
          Func_0200015c(x0 - 0x40000, y0, z0, -0x8000, 0, 0, 0xb0000, &spark);
          Func_0200015c(pos[2] + 0x40000, actor->y, pos[3], 0x8000, 0, 0,
                        0xb0000, &spark);
        }
      }
      wait(1);
    }
  }
  Func_02001fb4();
}

#include "iwram_call.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"

struct Spot {
  s32 x;
  s32 z;
};

struct Spots {
  struct Spot spot[6];
};

extern u32 gData_020022c4;
extern u16 gData_020022be;
extern s16 gData_020022c0;
extern u16 gData_020022bc;
extern struct Spots gData_02002118;
extern u8 gData_02002110[];

struct EventWork687 {
  u8 unk000[0x16a];
  u16 unk16a;
};

/// Tracks actor 8 around the ring of cells: counts steps made in one direction,
/// spawning an effect every fourth frame once past 3, and rewards a full count.
void Func_02000a80(void) {
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(8);
  s16 index;
  s32 now;
  s32 last;
  s32 nowRaw;
  u32 phase;
  u32 *frame;
  u32 *stamp;

  index = (Func_0200034c(0, actor->x >> 20, actor->z >> 20) >> 8) - 1;
  frame = &gFrameCount;
  stamp = &gData_020022c4;

  if (*frame - *stamp > 50) {
    gData_020022be = 0;
    gData_020022c0 = 0;
  }
  last = gData_020022bc;
  now = *(s16 *)(work + 340);
  nowRaw = *(u16 *)(work + 340);
  if (last != now) {
    // Each read below takes its own expression: the native reloads for each.
    s16 *delta = (s16 *)&gData_020022c0;
    s32 raw;
    s32 diff;
    u16 *count;
    s32 step;

    if (*delta == 0) {
      *delta = last - nowRaw;
    }
    if (*delta == 7) {
      *delta = -1;
    }
    raw = *(u16 *)delta;
    if ((s16)raw == -7) {
      *delta = 1;
    }
    diff = last - now;
    raw = *(u16 *)delta;
    step = (s16)raw;
    count = &gData_020022be;
    if (diff == step || now - last == step * 7) {
      (*count)++;
      *stamp = *frame;
    } else {
      *count = 0;
      *delta = 0;
    }
  }
  gData_020022bc = *(u16 *)(work + 340);
  if (gData_020022be > 3) {
    phase = gFrameCount & 3;
    if (phase == 0) {
      struct Spots spots = gData_02002118;
      struct EffectOptions params;
      u32 z;
      u32 x;

      struct EffectOptions *p = &params;

      p->type = 0x2a1;
      p->priority = 2;
      x = Random16Far() & 31;
      z = Random16Far() & 31;
      Event_PlaySound(246);
      x <<= 16;
      z <<= 16;
      Func_0200015c(spots.spot[(u16)index].x + x, 0,
                    spots.spot[(u16)index].z + z, 0, 0x13333, phase, 0x120001,
                    p);
    }
  }
  if (gData_020022be == gData_02002110[(u16)index]) {
    s32 zero = 0;

    ((struct EventWork687 *)work)->unk16a = 200;
    gData_020022be = zero;
    gData_020022c0 = zero;
  }
}

void Func_02000c50(void) {
  u32 *state = gSceneState;

  if (gFrameCount - gData_020022c8 > 120)
    ((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)->alarm = 201;
  *state++ = gData_020022c8;
  *state++ = gData_020022cc;
  *state = gFrameCount;
}

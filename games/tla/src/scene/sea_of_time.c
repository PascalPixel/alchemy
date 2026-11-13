
#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"
#include "shared/actors.h"
#include "sea_of_time/sea_of_time.h"

DEFINE_SCENE_CREATE_IMPORTED_ACTOR(Scene_CreateImportedActor, 34)

#include "../field/field_effect/move.c"

// ---- scene/sea_of_time.c ----
#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect/spawn.c"

// ---- scene/sea_of_time.c ----

// The Sea of Time, east of Lemuria: currents and swirls that carry the ship
// round and sweep it back to the entrance, the laps and waves Yepp's songs
// tell the way by, and rocks Grind breaks; the rest of the overlay is in
// sea_of_time/.

#include "shared/actors.h"
#include "sea_of_time/sea_of_time.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/scene/scene.h"
#include "module/scene/sea_of_time.h"

s32 Actor_HasArrived(struct Actor *actor);
void Actor_SelectAnimation(struct Actor *object, s32 value);
extern u16 gSeaOfTime_RippleLines[160];

s32 SeaOfTime_HideSprite(struct Actor *object) {
  ((struct SeaOfTimeSprite *)object->sprite)->visible = 0;
  object->terrainId = 1;
  return 1;
}

s32 SeaOfTime_GetCellCode(u32 layer, s32 x, s32 y) {
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

s32 SeaOfTime_SetCellCode(u32 layer, s32 x, s32 y, s32 value) {
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

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

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

extern struct ActorPlacement gSeaOfTime_Placements[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov687Room2)
    return (const struct ActorPlacement *)gSeaOfTime_Room2Placements;
  return (const struct ActorPlacement *)gSeaOfTime_Placements;
}

void SeaOfTime_CircleSwirl(void) {
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
  if (Position_FlatDistance(pos, center) > 4) {
    if (*pos < center[0])
      *pos += 0x3333;
    else
      *pos -= 0x3333;
    if (object->z < center[2])
      object->z += 0x3333;
    else
      object->z -= 0x3333;
    if (SeaOfTime_GetCellCode(1, center[0] >> 20, center[2] >> 20) >> 8 == 40) {
      turn[0] = SeaOfTime_Cos(-0x200);
      sine = Trig_Sin(-0x200);
    } else {
      turn[0] = SeaOfTime_Cos(0x200);
      sine = Trig_Sin(0x200);
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

void SeaOfTime_RideCurrent(void) {
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
  if (Position_FlatDistance(pos, &center) > 46) {
    if (*pos < center.x)
      *pos += 0x8000;
    else
      *pos -= 0x8000;
    if (object->z < center.z)
      object->z += 0x8000;
    else
      object->z -= 0x8000;
  }
  if (SeaOfTime_GetCellCode(1, center.x >> 20, center.z >> 20) >> 8 == 40) {
    turn[0] = SeaOfTime_Cos(-0x200);
    s = Trig_Sin(-0x200);
  } else {
    turn[0] = SeaOfTime_Cos(0x200);
    s = Trig_Sin(0x200);
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
      Event_PlaySound(0x255);
    spark.priority = 2;
    spark.palette = 7;
    spark.startScaleX = 0xcccc;
    spark.startScaleY = 0xcccc;
    c = Trig_Cos(object->facing);
    {
      s32 sine = Trig_Sin(object->facing);
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
      FieldEffect_Spawn(px + 0x30000, py, pz, speed, 0, 0, 0xb0000, &spark);
      FieldEffect_Spawn(ends[2] - 0x30000, object->y, ends[3], -speed, 0, 0,
                        0xb0000, &spark);
    } else {
      FieldEffect_Spawn(px - 0x30000, py, pz, -speed, 0, 0, 0xb0000, &spark);
      FieldEffect_Spawn(ends[2] + 0x30000, object->y, ends[3], speed, 0, 0,
                        0xb0000, &spark);
    }
  }
}

#include "actor.h"
#include "ram_buffer.h"
#include "event_inline.h"
#include "frame.h"
#include "system/field_effect.h"

// Walks actor 8 along a run of cells in the facing the event work names,
// throwing sparks from both sides of it every fourth frame.
void SeaOfTime_FollowChannel(void) {
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
  Event_Start();
  Event_ResolvePendingActions(0);
  x = actor->x >> 20;
  z = actor->z >> 20;
  dx = Trig_Cos(angle) >> 16;
  dz = Trig_Sin(angle) >> 16;
  actor->speedLimit = 0x40000;
  actor->acceleration = 0x40000;
  targetZ = 0;
  targetX = 0;
  if (SeaOfTime_GetCellCode(1, x + (dz < 0 ? -dz : dz),
                            z + (dx < 0 ? -dx : dx)) >>
          8 ==
      work->exit) {
    targetX = (dz < 0 ? -dz : dz) * 15 << 16;
    targetZ = (dx < 0 ? -dx : dx) * 15 << 16;
  }
  targetX += x << 20;
  targetZ += z << 20;
  while (SeaOfTime_GetCellCode(1, x, z) >> 8 == work->exit) {
    x += dx;
    z += dz;
    targetX += dx << 20;
    targetZ += dz << 20;
    Actor_SetMoveTarget(actor, targetX, actor->y, targetZ);
    while (Actor_HasArrived(actor) == 0) {
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
        cosine = Trig_Cos(actor->facing);
        sine = Trig_Sin(actor->facing);
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
          FieldEffect_Spawn(x0 + 0x40000, y0, z0, 0x8000, 0, 0, 0xb0000,
                            &spark);
          FieldEffect_Spawn(pos[2] - 0x40000, actor->y, pos[3], -0x8000, 0, 0,
                            0xb0000, &spark);
        } else {
          FieldEffect_Spawn(x0 - 0x40000, y0, z0, -0x8000, 0, 0, 0xb0000,
                            &spark);
          FieldEffect_Spawn(pos[2] + 0x40000, actor->y, pos[3], 0x8000, 0, 0,
                            0xb0000, &spark);
        }
      }
      wait(1);
    }
  }
  Event_End();
}

#include "iwram_call.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"

/* VIEW(sea_of_time): variable gSeaOfTime_LapCount as extern u16
 * gSeaOfTime_LapCount; the definition has u16 gSeaOfTime_LapCount[] = { 0, };
 * tried as the definition's extern u16 gSeaOfTime_LapCount[], it does not
 * compile in tla-ja: games/tla/src/scene/sea_of_time.c:402:
 * `gSeaOfTime_LapCount' has an incomplete type. */
extern u16 gSeaOfTime_LapCount__sea_of_time __asm__("gSeaOfTime_LapCount");
/* VIEW(sea_of_time): variable gSeaOfTime_LapTurn as extern s16
 * gSeaOfTime_LapTurn; the definition has u32 gSeaOfTime_LapTurn[] = { 0, };
 * tried as the definition's extern u32 gSeaOfTime_LapTurn[], it does not
 * compile in tla-ja: games/tla/src/scene/sea_of_time.c:403:
 * `gSeaOfTime_LapTurn' has an incomplete type. */
extern s16 gSeaOfTime_LapTurn__sea_of_time __asm__("gSeaOfTime_LapTurn");
/* VIEW(sea_of_time): variable gSeaOfTime_LastLapCell as extern u16
 * gSeaOfTime_LastLapCell; the definition has u16 gSeaOfTime_LastLapCell[] = {
 * 0, }; tried as the definition's extern u16 gSeaOfTime_LastLapCell[], it does
 * not compile in tla-ja: games/tla/src/scene/sea_of_time.c:437:
 * `gSeaOfTime_LastLapCell' has an incomplete type. */
extern u16
    gSeaOfTime_LastLapCell__sea_of_time __asm__("gSeaOfTime_LastLapCell");
/* VIEW(sea_of_time): variable gSeaOfTime_LapSpots as extern struct Spots
 * gSeaOfTime_LapSpots; the definition has u16 gSeaOfTime_LapSpots[] = { 0, 672,
 * 0, 704, 0, 896, 0, 320, 0, 512, 0, 192, 0, 320, 0, 416, 0, 320, 0, 832, 0,
 * 96, 0, 160, }; tried as the definition's extern u16 gSeaOfTime_LapSpots[], it
 * does not compile in tla-ja: games/tla/src/scene/sea_of_time.c:441: invalid
 * initializer. */
extern struct Spots
    gSeaOfTime_LapSpots__sea_of_time __asm__("gSeaOfTime_LapSpots");
/* VIEW(sea_of_time): variable gSeaOfTime_LapsNeeded as extern u8
 * gSeaOfTime_LapsNeeded[]; the definition has u32 gSeaOfTime_LapsNeeded[] = {
 * 0x18101810, 0x1810, }; tried as the definition's extern u32
 * gSeaOfTime_LapsNeeded[], SeaOfTime_CountLaps moves: 5 of 188 instructions in
 * tla-ja. */
extern u8 gSeaOfTime_LapsNeeded__sea_of_time[] __asm__("gSeaOfTime_LapsNeeded");

// Tracks actor 8 around the ring of cells: counts steps made in one direction,
// spawning an effect every fourth frame once past 3, and rewards a full count.
void SeaOfTime_CountLaps(void) {
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(8);
  s16 index;
  s32 now;
  s32 last;
  s32 nowRaw;
  u32 phase;
  u32 *frame;
  u32 *stamp;

  index = (SeaOfTime_GetCellCode(0, actor->x >> 20, actor->z >> 20) >> 8) - 1;
  frame = &gFrameCount;
  stamp = &gSeaOfTime_LapStamp;

  if (*frame - *stamp > 50) {
    gSeaOfTime_LapCount__sea_of_time = 0;
    gSeaOfTime_LapTurn__sea_of_time = 0;
  }
  last = gSeaOfTime_LastLapCell__sea_of_time;
  now = *(s16 *)(work + 340);
  nowRaw = *(u16 *)(work + 340);
  if (last != now) {
    // Each read below takes its own expression: the native reloads for each.
    s16 *delta = (s16 *)&gSeaOfTime_LapTurn__sea_of_time;
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
    count = &gSeaOfTime_LapCount__sea_of_time;
    if (diff == step || now - last == step * 7) {
      (*count)++;
      *stamp = *frame;
    } else {
      *count = 0;
      *delta = 0;
    }
  }
  gSeaOfTime_LastLapCell__sea_of_time = *(u16 *)(work + 340);
  if (gSeaOfTime_LapCount__sea_of_time > 3) {
    phase = gFrameCount & 3;
    if (phase == 0) {
      struct Spots spots = gSeaOfTime_LapSpots__sea_of_time;
      struct EffectOptions params;
      u32 z;
      u32 x;

      struct EffectOptions *p = &params;

      p->type = 0x2a1;
      p->priority = 2;
      x = Random16() & 31;
      z = Random16() & 31;
      Event_PlaySound(246);
      x <<= 16;
      z <<= 16;
      FieldEffect_Spawn(spots.spot[(u16)index].x + x, 0,
                        spots.spot[(u16)index].z + z, 0, 0x13333, phase,
                        0x120001, p);
    }
  }
  if (gSeaOfTime_LapCount__sea_of_time ==
      gSeaOfTime_LapsNeeded__sea_of_time[(u16)index]) {
    s32 zero = 0;

    ((struct EventWork687 *)work)->unk16a = 200;
    gSeaOfTime_LapCount__sea_of_time = zero;
    gSeaOfTime_LapTurn__sea_of_time = zero;
  }
}

void SeaOfTime_TrackWaves(void) {
  u32 *state = gSceneState;

  if (gFrameCount - gSeaOfTime_WaveStartFrame > 120)
    ((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)->alarm = 201;
  *state++ = gSeaOfTime_WaveStartFrame;
  *state++ = gSeaOfTime_WaveState;
  *state = gFrameCount;
}

// Two fades of BG palette 3: toward RGB(11, 6, 23), then, in
// SeaOfTime_StopWaves, back to its saved colours.
#include "system/io_reg.h"

extern u32 gSeaOfTime_WaveState;
/* VIEW(s0c94): routine Event_End as s32 Event_End(void); sea_of_time.h has void
 * Event_End(void); tried as the definition's void Event_End(void), which the
 * object already declares, it does not compile in tla-ja:
 * games/tla/src/scene/sea_of_time.c:542: void value not ignored as it ought to
 * be. */
extern s32 Event_End__s0c94(void) __asm__("Event_End");

// Fades BG palette 3's entries 1 to 13 toward RGB(11, 6, 23), one unit per
// channel every 10 frames, then hands the rest of the event to
// SeaOfTime_TrackWaves's task.
s32 SeaOfTime_StartWaves(void) {
  volatile u16 *palette;
  s32 changed;
  s32 i;
  s32 frame;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetFlag(0x200);
  Event_PlaySound(0x22d);
  do {
    wait(10);
    changed = 0;
    for (i = 1, palette = &BG_PLTT_COLOR(3, 1); i != 14; i++, palette++) {
      u16 color = *palette;
      u16 r = color & 31;
      u16 g = (color >> 5) & 31;
      u16 b = (color >> 10) & 31;

      if (r < 11) {
        r++;
        changed = 1;
      }
      if (r > 11) {
        r--;
        changed = 1;
      }
      if (g < 6) {
        g++;
        changed = 1;
      }
      if (g > 6) {
        g--;
        changed = 1;
      }
      if (b < 23) {
        b++;
        changed = 1;
      }
      if (b > 23) {
        b--;
        changed = 1;
      }
      *palette = RGB(r, g, b);
    }
    frame = gFrameCount;
    gSeaOfTime_WaveState += frame & 1;
  } while (changed);
  gSeaOfTime_WaveState = 360;
  gSeaOfTime_WaveStartFrame = frame;
  Task_Add(SeaOfTime_TrackWaves, 0x480);
  return Event_End__s0c94();
}

extern struct SavedColors gSeaOfTime_SavedColors;

s32 SeaOfTime_StopWaves(void) {
  s32 changed;
  volatile u16 *palette;
  struct SavedColors *saved;
  u32 i;
  u32 j;

  Task_Remove(SeaOfTime_TrackWaves);
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(0x22c);
  gSeaOfTime_WaveState = 1;
  // Each of the 14 colours steps one unit per channel toward its saved colour
  // every 10 frames, until none changes.
fade:
  wait(10);
  changed = 0;
  palette = &BG_PLTT_COLOR(3, 0);
  // i counts palette entries from 1 and j saved colours from 0; the saved
  // colours are read through a pointer set after i, each pass.
  for (i = 1, saved = &gSeaOfTime_SavedColors, j = 0; i != 15; i++, j++) {
    u16 color = palette[i];
    u16 target = saved->colors[j];
    u16 r = color & 31;
    u16 g = (color >> 5) & 31;
    u16 b = (color >> 10) & 31;
    u16 tr = target & 31;
    u16 tg = (target >> 5) & 31;
    u16 tb = (target >> 10) & 31;

    if (r < tr) {
      r++;
      changed = 1;
    }
    if (r > tr) {
      r--;
      changed = 1;
    }
    if (g < tg) {
      g++;
      changed = 1;
    }
    if (g > tg) {
      g--;
      changed = 1;
    }
    if (b < tb) {
      b++;
      changed = 1;
    }
    if (b > tb) {
      b--;
      changed = 1;
    }
    palette[i] = RGB(r, g, b);
  }
  if (changed)
    goto fade;
  gSeaOfTime_WaveState = 1;
  GameFlag_Clear(0x200);
  /* FAKEMATCH(signature): typed to return s32, passing on Event_End's,
   * though its target Event_End returns nothing; as void, 6 of 150 instructions
   * differ in tla-en. */
  return Event_End__s0c94();
}

// next object the overlay links.

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

// Sends actor 8 to a point 8 pixels along z and, until Actor_HasArrived says
// it is done, raises a pair of effects at its two sides every fourth frame.
void SeaOfTime_SinkIntoWhirlpool(void) {
  s32 sides[4];
  struct EffectOptions options;
  struct Actor *actor;
  s32 cosine, sine;
  s32 x, y, z;
  s32 velocity, reverseVelocity, alternate;

  actor = Actor_GetById(8);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetMoveTarget(actor, actor->x, actor->y, actor->z + 0x80000);
  // A goto loop: as while (Actor_HasArrived(actor) == 0), loop.c moves 0xb0000
  // out of the loop and 191 of 165 instructions differ.
  while (Actor_HasArrived(actor) == 0) {
    if ((gFrameCount & 3) == 0) {
      options.priority = 2;
      options.palette = 7;
      options.startScaleX = 0x16666;
      options.startScaleY = 0x13333;
      cosine = Trig_Cos(actor->facing);
      sine = Trig_Sin(actor->facing);
      sides[0] = cosine * 12;
      sides[1] = sine * 12;
      sides[2] = -(cosine * 12);
      sides[3] = -(sine * 12);
      sides[0] += actor->x;
      sides[1] += actor->z;
      sides[2] += actor->x;
      sides[3] += actor->z;
      velocity = 0x8000;
      if (sides[1] != actor->z) {
        sides[1] -= 0xc0000;
        sides[3] -= 0xc0000;
      }
      alternate = gFrameCount & 4;
      reverseVelocity = -velocity;
      x = sides[0];
      y = actor->y;
      z = sides[1];
      if (alternate) {
        FieldEffect_Spawn(x + 0x40000, y, z, velocity, 0, 0,
                          EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                              EFFECT_USE_START_SCALE,
                          &options);
        FieldEffect_Spawn(
            sides[2] - 0x40000, actor->y, sides[3], reverseVelocity, 0, 0,
            EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE,
            &options);
      } else {
        FieldEffect_Spawn(x - 0x40000, y, z, reverseVelocity, 0, 0,
                          EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                              EFFECT_USE_START_SCALE,
                          &options);
        FieldEffect_Spawn(
            sides[2] + 0x40000, actor->y, sides[3], velocity, 0, 0,
            EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE,
            &options);
      }
    }
    wait(1);
  }
  Event_End();
}

#include "io_reg.h"

void SeaOfTime_SweptBack(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (!Event_GetFlag(0x8dd)) {
    Import_EventSetExitRequest(4);
    Event_SetFlag(0x8dd);
  } else {
    Import_EventSetExitRequest(1);
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void SeaOfTime_UpdateSpray(struct Actor *object) {
  struct EffectOptions spark;
  s32 even = gFrameCount & 1;

  if (even == 0) {
    s32 dx = ((Random16() & 31) << 16) + -0xe0000;

    spark.startScaleX = 0xb333;
    spark.startScaleY = 0xb333;
    spark.palette = 7;
    spark.priority = 1;
    FieldEffect_Spawn(object->x + dx, 0, object->z + 0x100000, 0, even, even,
                      0xb0000, &spark);
  }
}

void SeaOfTime_CrossToLemuria(void) {
  volatile u16 fill;
  struct EffectOptions burst;
  struct EffectOptions rain;
  struct Actor *object;
  s32 changed;
  volatile u16 *palette;
  struct SavedColors *saved;
  u32 i;
  u32 j;

  Event_Start();
  Event_ResolvePendingActions(0);
  GameFlag_Set(0x8dd);
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Actor_GetById(8)->facing = FACING_NORTH;
  Actor_WalkTo(8, 352, 264);
  Camera_SetSpeedParameters(0x10000, 0x2000);
  FieldCamera_MoveWithinMapBounds(352 << 16, -1, 196 << 16, 1);
  FieldCamera_WaitForArrival();
  Actor_WaitForMoveById(8);
  Sound_Play(78);
  Sound_Play(556);
  // Fills palette 3's entries 1 to 14 with one colour.
  fill = RGB(11, 6, 23);
  Dma_Set((const void *)&fill, (void *)&BG_PLTT_COLOR(3, 1),
          (DMA_ENABLE | DMA_SRC_FIXED) << 16 | 14, REG_DMA3);
  MapGrid_CopyCellPictureIndicesRect(49, 70, 19, 73, 6, 6);
  // Steps each colour's red, green and blue by one toward the saved palette
  // every 10 frames until none changes.
fade:
  wait(10);
  changed = 0;
  palette = &BG_PLTT_COLOR(3, 0);
  // i counts palette entries from 1 and j saved colours from 0; the saved
  // colours are read through a pointer set after i, each pass.
  for (i = 1, saved = &gSeaOfTime_SavedColors, j = 0; i != 15; i++, j++) {
    u16 color = palette[i];
    u16 target = saved->colors[j];
    u16 r = color & 31;
    u16 g = (color >> 5) & 31;
    u16 b = (color >> 10) & 31;
    u16 tr = target & 31;
    u16 tg = (target >> 5) & 31;
    u16 tb = (target >> 10) & 31;

    if (r < tr) {
      r++;
      changed = 1;
    }
    if (r > tr) {
      r--;
      changed = 1;
    }
    if (g < tg) {
      g++;
      changed = 1;
    }
    if (g > tg) {
      g--;
      changed = 1;
    }
    if (b < tb) {
      b++;
      changed = 1;
    }
    if (b > tb) {
      b--;
      changed = 1;
    }
    palette[i] = RGB(r, g, b);
  }
  if (changed)
    goto fade;
  Event_Wait(30);
  // For 360 frames, spawns an effect every 16 frames, then every 8, then
  // every other frame, and plays sound 164 every 64 frames. As a C loop,
  // loop.c moves the mask, the palette and the options address out of it,
  // which the native does not (differ 252).
  i = 0;
sparks:
  if ((i & 63) == 0)
    Sound_Play(164);
  if (i < 120) {
    if (i & 15)
      goto next;
  } else if (i < 240) {
    if (i & 7)
      goto next;
  } else if (i & 1)
    goto next;
  {
    s32 x = Random16();
    s32 mask = 31;
    s32 z = Random16();

    burst.palette = 7;
    FieldEffect_Spawn(((x & mask) << 16) + 0x1500000, 0,
                      ((z & mask) << 16) + 0xb00000, 0, 0, 0, 0x10000, &burst);
    wait(1);
  }
next:
  i++;
  if (i != 360)
    goto sparks;
  // Then 16 larger effects, one a frame. As a C loop, loop.c also moves the
  // mask and the options address out, which the native does not (differ 228).
  // Entered at its test so that PRE leaves the address in the body.
  i = 0;
  while (i != 16) {
    {
      s32 x = Random16();
      s32 mask = 31;
      s32 z = Random16();
      s32 offsetZ = (z & mask) << 16;
      s32 offsetX = (x & mask) << 16;

      rain.startScaleX = 0x18000;
      rain.startScaleY = 0x18000;
      rain.palette = 7;
      rain.priority = 1;
      FieldEffect_Spawn(offsetX + 0x1500000, 0, offsetZ + 0xb00000, 0, 0x10000,
                        0, 0xb0000, &rain);
      wait(1);
    }
    i++;
  }
  object = Import_ActorCreate(159, 0x15c0000, 0, 0xb80000);
  // The object's per-frame hook, struct Actor's update.
  *(void (**)(struct Actor *))((u8 *)object + 0x6c) = SeaOfTime_UpdateSpray;
  Event_Wait(60);
  Actor_SelectAnimation(object, 1);
  Event_Wait(40);
  gMainRoutineWork.battleRule24b = 2;
  Event_SetAfterBattleDestination((s32)&SceneId_Ov687Room2, 4);
  Field_RequestBattleFromEncounterTable(12, 6);
  Event_End();
}

void SeaOfTime_RemoveRock(s32 x, s32 z, s32 actor) {
  s32 cx = x >> 20;
  s32 cz = z >> 20;

  SeaOfTime_SetCellCode(1, cx - 1, cz - 1, 0);
  SeaOfTime_SetCellCode(1, cx, cz - 1, 0);
  SeaOfTime_SetCellCode(1, cx - 1, cz, 0);
  SeaOfTime_SetCellCode(1, cx, cz, 0);
  Actor_SetPosition(actor, 0, 0);
}

#define DEFINE_GRIND_ROCK(name, n1, n2)                                        \
  void name(void) {                                                            \
    struct Actor *object;                                                      \
                                                                               \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    object = Actor_GetById(n1);                                                \
    SeaOfTime_RemoveRock(object->x, Actor_GetById(n1)->z, n1);                 \
    GameFlag_Set(n2);                                                          \
    Event_End();                                                               \
  }

DEFINE_GRIND_ROCK(SeaOfTime_GrindFirstRock, 9, 0xa90)

DEFINE_GRIND_ROCK(SeaOfTime_GrindSecondRock, 10, 0xa91)

DEFINE_GRIND_ROCK(SeaOfTime_GrindThirdRock, 11, 0xa92)

DEFINE_GRIND_ROCK(SeaOfTime_GrindFourthRock, 12, 0xa93)

DEFINE_GRIND_ROCK(SeaOfTime_GrindFifthRock, 13, 0xa94)

DEFINE_GRIND_ROCK(SeaOfTime_GrindLastRock, 9, 0xa95)

#define DEFINE_LEAVE_BY_EXIT(name, n1)                                         \
  void name(void) {                                                            \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    Import_EventSetExitRequest(n1);                                            \
    Event_FinishScreenTransition();                                            \
    Event_WaitTransitionFrames();                                              \
    Event_End();                                                               \
  }

DEFINE_LEAVE_BY_EXIT(SeaOfTime_LeaveByExit4, 4)

DEFINE_LEAVE_BY_EXIT(SeaOfTime_LeaveByExit5, 5)

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov687Room1)
    return gSeaOfTime_Room1Triggers;
  if (gMainRoutineWork.entrance <= 4)
    return gSeaOfTime_EntranceTriggers;
  return gSeaOfTime_Triggers;
}

#include "map.h"

// The Sea of Time's frame callback: rotates two runs of the background
// palette, pulses the red of colour 15 (or steps it with the alarm), copies
// the palette and ripples the layer's horizontal scroll line by line.
void SeaOfTime_AnimateSea(void) {
  u16 *line = gSeaOfTime_RippleLines;
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[2];
  u8 *block = Ram_HeapSlots->block32;
  struct EventWork_02001650 *event = Ram_HeapSlots->eventWork;
  s32 i;
  s16 color;

  if (event->unk166 != 0)
    return;
  if (*(s8 *)(block + 0x2a01) != 0)
    return;
  if (event->unk1b2 == 0)
    return;

  if (gFrameCount - gSeaOfTime_WaveStartFrame > gSeaOfTime_WaveState) {
    s16 target;

    color = BG_PLTT[62];
    target = gSeaOfTime_SavedColors.colors[13];

    for (i = 14; i != 0; i--) {
      BG_PLTT[48 + i] = BG_PLTT[47 + i];
      gSeaOfTime_SavedColors.colors[i - 1] =
          gSeaOfTime_SavedColors.colors[i - 2];
    }
    BG_PLTT[49] = color;
    gSeaOfTime_SavedColors.colors[0] = target;
    gSeaOfTime_WaveStartFrame = gFrameCount;
  }

  if ((gFrameCount & 1) == 0) {
    color = BG_PLTT[78];

    for (i = 14; i != 0; i--)
      BG_PLTT[64 + i] = BG_PLTT[63 + i];
    BG_PLTT[65] = color;

    if ((gFrameCount & 3) == 0) {
      u16 color = BG_PLTT[15];
      s32 red = color & 31;
      s32 green = (color >> 5) & 31;
      s32 blue = (color >> 10) & 31;
      s32 flag = GameFlag_Get(0x200);
      /* FAKEMATCH(temporary): each channel shifted up after the flag test; as
       * (u16)green << 5 and so on in the store, 167 of 272 instructions differ
       * in tla-en. */
      u32 greenBits = green << 16;
      u32 redBits = red << 16;
      u32 blueBits = blue << 16;

      if (flag == 0) {
        u32 step = (u16)gSeaOfTime_RedPulseStep[0];
        u32 sum = (u16)(redBits >> 16) + step;
        red = (s16)sum;
        if ((u16)sum < 18) {
          red = 18;
          gSeaOfTime_RedPulseStep[0] = 1;
        } else if ((u16)sum > 31) {
          red = 31;
          gSeaOfTime_RedPulseStep[0] = -1;
        }
      } else {
        if (((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)->alarm ==
            200) {
          u16 current = redBits >> 16;
          if (current != 0)
            red = (s16)(u16)(current - 1);
        } else if (((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)
                       ->alarm == 201) {
          u16 current = redBits >> 16;
          if (current < 30)
            red = current + 1;
        }
      }
      {
        u16 last = red;
        BG_PLTT[15] = (blueBits >> 6) | (greenBits >> 11) | (last & 31);
      }
    }
  }

  Dma_Set((const void *)BG_PLTT, Ram_HeapSlots->block32, DMA_COPY_WORDS(112),
          REG_DMA3);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  *(s32 *)((u8 *)layer + 0x1c) = 0;
  for (i = 0; i != 160; i++)
    *line++ =
        (s16)(layer->x >> 16) +
        (Trig_Sin((i + *(u8 *)&gFrameCount + (s16)(layer->y >> 16)) << 9) >>
         15);
  line = gSeaOfTime_RippleLines;
  REG_BG1HOFS = *line;
  Dma_Set(line + 1, (void *)&REG_BG1HOFS, 0xa2600001, (volatile u32 *)REG_DMA0);
}

void SeaOfTime_PlaceRock(s32 x, s32 z, s32 actor) {
  Actor_GetById(actor)->collisionFlags |= 16;
  Actor_SetPosition(actor, (x << 19) + 0x80000, (z << 19) + 0x80000);
  x >>= 1;
  z >>= 1;
  SeaOfTime_SetCellCode(1, x, z, 0xff00);
  SeaOfTime_SetCellCode(1, x + 1, z, 0xff00);
  SeaOfTime_SetCellCode(1, x, z + 1, 0xff00);
  SeaOfTime_SetCellCode(1, x + 1, z + 1, 0xff00);
}

void SeaOfTime_RaiseRock(s32 x, s32 z, s32 actor) {
  struct EffectOptions spark;
  s32 px;
  s32 pz;
  s32 i;

  FieldCamera_SetSpeed(0x60000, 0xc000);
  px = (x << 19) + 0x80000;
  pz = (z << 19) + 0x80000;
  FieldCamera_MoveTo(px, -1, pz, 1);
  FieldCamera_WaitForArrival();
  Event_PlaySound(144);
  MapGrid_SetShake(0, 0x100000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  spark.startScaleX = 0x13333;
  spark.startScaleY = 0x18000;
  spark.priority = 1;
  spark.type = 276;
  FieldEffect_Spawn(px, 0, pz, 0, 0, 0, 0x1a0000, &spark);
  Actor_GetById(actor)->scaleY = 0x1999;
  SeaOfTime_PlaceRock(x, z, actor);
  for (i = 0; i != 10; i++) {
    Actor_GetById(actor)->scaleY += 0x1999;
    Event_Wait(1);
  }
  Actor_GetById(actor)->scaleY = 0x10000;
  Event_Wait(20);
}

// The Sea of Time's opening: places the camera and the first effects, showers
// sparks for 360 frames and larger effects for 16, then fades BG palette 3 to
// RGB(11, 6, 23).
void SeaOfTime_Open(void) {
  struct Actor *actor;
  u32 i;
  s32 changed;
  volatile u16 *palette;
  struct EffectOptions params;
  struct EffectOptions burst;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(0);
  MapGrid_CopyCellsTo(49, 70, 19, 73, 6, 6);
  actor = FieldActor_Create(159, 0x15c0000, 0, 0xb80000);
  actor->update = (void (*)(void))SeaOfTime_UpdateSpray;
  FieldCamera_MoveTo(0x1600000, -1, 0xc40000, 0);
  Event_StartScreenTransition();
  wait(1);
  Task_Add(SeaOfTime_AnimateSea, 0xc80);
  Event_WaitTransitionFrames();
  Event_Wait(60);
  SeaOfTime_RaiseRock(21, 23, 9);
  SeaOfTime_RaiseRock(37, 17, 10);
  SeaOfTime_RaiseRock(47, 27, 12);
  SeaOfTime_RaiseRock(59, 19, 13);
  MapGrid_WaitForShakeToSettle();
  FieldCamera_MoveTo(0x1600000, -1, 0xc40000, 1);
  FieldCamera_WaitForArrival();
  Event_SetFlag(0x200);
  ((struct EventWork687 *)Ram_HeapSlots->eventWork)->unk16a = 200;
  Event_PlaySound(0x212);
  Event_Wait(40);
  for (i = 0; i != 360; i++) {
    if ((i & 31) == 0)
      Event_PlaySound(164);
    if (i < 120) {
      if (i & 15)
        continue;
    } else if (i < 240) {
      if (i & 7)
        continue;
    } else if (i & 1)
      continue;
    {
      s32 mask = 31;
      s32 x = Random16();
      s32 y = Random16();

      params.startScaleX = 0xcccc;
      params.startScaleY = 0xcccc;
      params.priority = 1;
      params.palette = 7;
      x &= mask;
      x <<= 16;
      x += 0x1500000;
      FieldEffect_Spawn(x, 0, ((y & mask) << 16) + 0xb00000, 0, 0, 0, 0xb0000,
                        &params);
      wait(1);
    }
  }
  for (i = 0; i != 16; i++) {
    s32 mask = 31;
    s32 x = Random16();
    s32 y = Random16();

    burst.startScaleX = 0x18000;
    burst.startScaleY = 0x18000;
    burst.palette = 7;
    burst.priority = 1;
    x &= mask;
    x <<= 16;
    x += 0x1500000;
    FieldEffect_Spawn(x, 0, ((y & mask) << 16) + 0xb00000, 0, 0x10000, 0,
                      0xb0000, &burst);
    wait(1);
  }
  Actor_Release(actor);
  Event_PlaySound(0x22d);
  do {
    wait(10);
    changed = 0;
    for (i = 1, palette = &BG_PLTT_COLOR(3, 1); i != 14; i++, palette++) {
      u16 color = *palette;
      u16 r = color & 31;
      u16 g = (color >> 5) & 31;
      u16 b = (color >> 10) & 31;

      if (r < 11) {
        r++;
        changed = 1;
      }
      if (r > 11) {
        r--;
        changed = 1;
      }
      if (g < 6) {
        g++;
        changed = 1;
      }
      if (g > 6) {
        g--;
        changed = 1;
      }
      if (b < 23) {
        b++;
        changed = 1;
      }
      if (b > 23) {
        b--;
        changed = 1;
      }
      *palette = RGB(r, g, b);
    }
    gSeaOfTime_WaveState += gFrameCount & 1;
  } while (changed);
  Task_Remove(SeaOfTime_AnimateSea);
  Scene_PlayMusic();
  Event_SetFlag(0x940);
  Event_End();
}

// The overlay's start: sets the screen effect, restores the clock and palette
// in the first room, draws the unlit cells in the second, and schedules the
// frame callback.

// The overlay's start: sets the screen effect, restores the clock and palette
// in the first room, draws the unlit cells in the second, and schedules the
// frame callback.
s32 Scene_Initialize(void) {
  ((struct SeaOfTimeEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 256;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov687Room1) {
    gSeaOfTime_WaveStartFrame = gFrameCount;
    gSeaOfTime_WaveState = 1;
    Dma_Set((const void *)0x05000062, gSeaOfTime_SavedColors.colors, 0x8000000e,
            (volatile u32 *)0x040000d4);
    if (GameFlag_Get(0x200)) {
      u32 *state = gSceneState;
      u32 offset, i;

      gSeaOfTime_WaveStartFrame = *state++;
      gSeaOfTime_WaveState = *state++;
      gFrameCount = *state;
      i = 1;
      offset = 98;
      do {
        *(volatile u16 *)(offset + 0x05000000) = 0x5ccb;
        i++;
        offset += 2;
      } while (i != 14);
      Task_Add(SeaOfTime_TrackWaves, 0x480);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov687Room2) {
    gSeaOfTime_WaveStartFrame = gFrameCount;
    gSeaOfTime_WaveState = 0;
    if (gMainRoutineWork.entrance <= 4) {
      if (!GameFlag_Get(0x940)) {
        Dma_Set((const void *)0x05000062, gSeaOfTime_SavedColors.colors,
                0x8000000e, (volatile u32 *)0x040000d4);
      } else {
        if (!GameFlag_Get(0xa90))
          SeaOfTime_PlaceRock(21, 23, 9);
        if (!GameFlag_Get(0xa91))
          SeaOfTime_PlaceRock(37, 17, 10);
        if (!GameFlag_Get(0xa93))
          SeaOfTime_PlaceRock(47, 27, 12);
        if (!GameFlag_Get(0xa94))
          SeaOfTime_PlaceRock(59, 19, 13);
        {
          void *block = Ram_HeapSlots->block32;

          *(volatile u16 *)0x0500001e &= 0xffe0;
          Dma_Set((const void *)0x05000000, block, 0x84000070,
                  (volatile u32 *)0x040000d4);
        }
        BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
      }
      if (gMainRoutineWork.entrance == 4 && !GameFlag_Get(265))
        SeaOfTime_Open();
    } else {
      if (!GameFlag_Get(0xa95))
        SeaOfTime_PlaceRock(89, 85, 9);
    }
  }
  {
    s32 priority = 0xc80;

    Task_Add(SeaOfTime_AnimateSea, priority);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  void *block = Ram_HeapSlots->block32;

  Dma_Fill32(-1, block, 0x1c0);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  return 0;
}

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_MediumScript[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_LongScript[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gSeaOfTime_LapsNeeded[] = {
    0x18101810, 0x1810,
};

u16 gSeaOfTime_LapSpots[] = {
    0, 672, 0, 704, 0, 896, 0, 320,
    0, 512, 0, 192, 0, 320, 0, 416,
    0, 320, 0, 832, 0, 96, 0, 160,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gSeaOfTime_ShipRests[] = {
    46,
    (s32)SeaOfTime_HideSprite,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(197),
    SCENE_EXIT_IF(1, 2, 42, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 198, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 197, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 42, 20, EXIT_FLAG_NONE),
    SCENE_EXITS(198),
    SCENE_EXIT_IF(1, 197, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 198, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 2, 44, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 2, 43, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 198, 2, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gSeaOfTime_Placements[] = {
    {8, CONDITION_ALWAYS, (s32)gSeaOfTime_ShipRests, PIXELS(120), 0,
     PIXELS(280), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gSeaOfTime_Room2Placements[] = {
    {8, CONDITION_ALWAYS, (s32)gSeaOfTime_ShipRests, PIXELS(120), 0,
     PIXELS(280), FACING_NORTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

u16 gSeaOfTime_LastLapCell[] = {
    0,
};

u16 gSeaOfTime_LapCount[] = {
    0,
};

u32 gSeaOfTime_LapTurn[] = {
    0,
};

u32 gSeaOfTime_LapStamp[] = {
    0,
};

volatile u32 gSeaOfTime_WaveStartFrame = 0;

u32 gSeaOfTime_WaveState = 0;

struct Trigger gSeaOfTime_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 49, CONDITION_ALWAYS, EVENT(SeaOfTime_SweptBack)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 32, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 35, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 36, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 37, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 40, CONDITION_ALWAYS, EVENT(SeaOfTime_CircleSwirl)},
    {TRIGGER_STEP, 41, CONDITION_ALWAYS, EVENT(SeaOfTime_CircleSwirl)},
    {TRIGGER_STEP, 44, CONDITION_ALWAYS, EVENT(SeaOfTime_SinkIntoWhirlpool)},
    {TRIGGER_STEP, 45, 0x200, EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, 46, 0x200, EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, 47, 0x200, EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, 48, 0x200, EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, 50, 0x200, EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_STEP, 51, 0x200, EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_STEP, 52, 0x200, EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_STEP, 53, 0x200, EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_STEP, 54, 0x200, EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_STEP, 55, 0x200, EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_STEP, 56, 0x200, EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_STEP, 57, 0x200, EVENT(SeaOfTime_CountLaps)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(SeaOfTime_StartWaves)},
    {TRIGGER_RAISED, 201, CONDITION_ALWAYS, EVENT(SeaOfTime_StopWaves)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSeaOfTime_EntranceTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, 5, 0x940, EVENT(SeaOfTime_CrossToLemuria)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 32, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 35, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 36, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 37, CONDITION_ALWAYS, EVENT(SeaOfTime_RideCurrent)},
    {TRIGGER_STEP, 48, CONDITION_ALWAYS, EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, 41, CONDITION_ALWAYS, EVENT(SeaOfTime_CircleSwirl)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 9, CONDITION_ALWAYS,
     EVENT(SeaOfTime_GrindFirstRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 10, CONDITION_ALWAYS,
     EVENT(SeaOfTime_GrindSecondRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 11, CONDITION_ALWAYS,
     EVENT(SeaOfTime_GrindThirdRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 12, CONDITION_ALWAYS,
     EVENT(SeaOfTime_GrindFourthRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 13, CONDITION_ALWAYS,
     EVENT(SeaOfTime_GrindFifthRock)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSeaOfTime_Triggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(SeaOfTime_LeaveByExit4)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(SeaOfTime_LeaveByExit5)},
    {TRIGGER_STEP, 45, CONDITION_ALWAYS, EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, 46, CONDITION_ALWAYS, EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, 47, CONDITION_ALWAYS, EVENT(SeaOfTime_FollowChannel)},
    {TRIGGER_STEP, 48, CONDITION_ALWAYS, EVENT(SeaOfTime_FollowChannel)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 9, CONDITION_ALWAYS,
     EVENT(SeaOfTime_GrindLastRock)},
    {TRIGGER_TABLE_END},
};

s16 gSeaOfTime_RedPulseStep[] = {
    1,
};

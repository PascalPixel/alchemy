/// @file
/// The scene code of overlay 0x663 that its scene tables belong to, in ROM order; the rest of the overlay is in airs_rock/.

#include "system/main_routine_work.h"
#include "field/field.h"
#include "event_work.h"
#include "system/frame.h"

// For each id of a list ended by 0xffff: clears the object's mark and, on a
// cell with attribute bit 6, blocks the cells ahead of and behind it and sets
// the mark.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "event_inline.h"

s32 Func_02000348(u32 layer, s32 x, s32 z);

void Func_020003e8(u16 *ids) {
  struct Actor *object;
  u8 *blocked;
  struct MapWork *map;
  struct MapCell **cells;
  struct MapCell *cell;
  s32 x, z;

  while (*ids != 0xffff) {
    object = Actor_Get(*ids);
    blocked = (u8 *)object + 0x63;
    *blocked = 0;
    if (Func_02000348(0, object->x >> 20, object->z >> 20) & 0x40) {
      map = Ram_HeapSlots->mapWork;
      if (map == NULL)
        return;
      cells = &map->layers[2].cells;
      z = (object->z - object->y) >> 20;
      x = object->x >> 20;
      cell = *cells;
      cell += x + ((z + 1) << 7);
      cell->collisionCode = 255;
      cell = *cells;
      cell += x + ((z - 1) << 7);
      cell->collisionCode = 255;
      *blocked = 1;
    }
    ids++;
  }
}

struct Unk_02000468 {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x63 - 0x14];
  u8 unk63;
};

void Func_02000468(struct Unk_02000468 *unk) {
  struct MapWork *map;

  if (unk->unk63 != 0) {
    map = Ram_HeapSlots->mapWork;
    if (map != NULL) {
      struct MapCell **cells = &map->layers[2].cells;
      /* FAKEMATCH(dead): cell's initial value, overwritten unread; as a plain
         declaration, 14 of 40 instructions differ in tla-en. */
      struct MapCell *cell = *cells;
      s32 x = unk->x >> 20;
      s32 row = (unk->z - unk->y) >> 20;

      cell = *cells;
      cell += x + ((row + 1) << 7);
      cell->collisionCode = 0;
      cell = *cells;
      cell += x + ((row - 1) << 7);
      cell->collisionCode = 0;
      unk->unk63 = 0;
    }
  }
}

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

#include "main_routine_work.h"

extern char SceneId_Ov663Room1;
extern char SceneId_Ov663Room2;
extern s16 gData_02003e84[];
extern s16 gData_02003ec4[];
extern s16 gData_02003f44[];

s16 *Func_020004c0(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov663Room1)
    return gData_02003e84;
  if (scene == (s32)&SceneId_Ov663Room2)
    return gData_02003ec4;
  return gData_02003f44;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern s32 gData_020040ec[];
extern u32 gData_02004194[];
extern u32 gData_02004314[];

void *Func_02000508(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov663Room1)
    return gData_020040ec;
  if (scene == (s32)&SceneId_Ov663Room2)
    return gData_02004194;
  return gData_02004314;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02003bc8(void);
void Func_02003bd0(void);
extern u8 MsgFieldWindStoneWarning[];

void Func_02000548(void) {
  Func_02003bc8();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgFieldWindStoneWarning, 1);
  Func_02003bd0();
}
#undef FIELD_AT

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02003b70();

void Func_02000568(void) {
  Func_02003bc8();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(0x9e);
  MapGrid_CopyCellsTo(64, 64, 83, 35, 1, 2);
  Func_02003b70(0);
  FieldEvent_Wait(45);
  Actor_SetSpeed(FIELD_AT(&gMainRoutineWork, s32, 532), 0x8000,
                           0x4000);
  Actor_WalkTo(FIELD_AT(&gMainRoutineWork, s32, 532), 312,
                                   680);
  FieldEvent_Wait(5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldActor_WaitForMoveById(FIELD_AT(&gMainRoutineWork, s32, 532));
  Event_PlaySound(0x7b);
  FieldEvent_SetExitRequest(4);
  Func_02003bd0();
}
#undef FIELD_AT

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020005ec(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(10));
  Func_02003bc8();
  FieldEvent_ResolvePendingActions(0);
  if ((FIELD_AT(t0, s32, 8) >> 20) == 0x1f) {
    Event_SetFlag(0x310);
  } else {
    Event_ClearFlag(0x310);
  }
  Func_02003bd0();
}
#undef FIELD_AT

s32 Func_02000384();

/// Object 11's column sets or clears flag 0x311 and writes the low byte of the
/// cell value Func_02000348 returns (its high byte 0xff when the object is
/// away) into two places; with the leader 208 pixels up a block copy follows.
void Func_02000620(void) {
  struct Actor *object;
  struct Actor *leader;
  s32 value;

  object = (struct Actor *)((s32)Actor_Get(11));
  value = Func_02000348(3, 70, 67);
  Func_02003bc8();
  FieldEvent_ResolvePendingActions(0);
  if ((object->x >> 20) == 32) {
    Event_SetFlag(0x311);
    value &= 0xff;
    Func_02000384(4, 70, 67, value);
    Func_02000384(2, 33, 54, value);
  } else {
    Event_ClearFlag(0x311);
    value = (value & 0xff) | 0xff00;
    Func_02000384(4, 70, 67, value);
    Func_02000384(2, 33, 54, value);
  }
  leader = (struct Actor *)((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if ((leader->y >> 19) == 26)
    MapGrid_CopyCellAttributes(70, 67, 1, 1, 33, 54);
  Func_02003bd0();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_020006d4(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(13));
  Func_02003bc8();
  FieldEvent_ResolvePendingActions(0);
  if ((FIELD_AT(t0, s32, 8) >> 19) == 0x17) {
    Event_SetFlag(0x879);
  }
  Func_02003bd0();
}
#undef FIELD_AT

void Func_02003b28(struct Actor *object, s32 mode);
void Func_02003c98(void);

/// The leader jumps up 12 pixels (cue 390, mode 6 then 1), climbs 28 pixels
/// over 14 frames with cue 217 while turned to angle 0x5000, then lands 16
/// pixels on and plays cue 390 again.
void Func_02000700(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 z = leader->z;
  u32 i;

  Event_PlaySound(390);
  Func_02003b28(leader, 6);
  System_Wait(10);
  Func_02003b28(leader, 1);
  Actor_SetSingleChildFlags(leader, 0);
  leader->motionFlags |= 2;
  leader->velocityY = 0x40000;
  FieldActor_SetMoveTarget(leader, leader->x, leader->y, z + 0xc0000);
  System_Wait(6);
  Event_PlaySound(217);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x5000, 0);
  leader->motionFlags = 0;
  for (i = 0; i < 14; i++) {
    leader->y += -0x20000;
    leader->targetY = leader->y;
    System_Wait(1);
  }
  leader->motionFlags = 3;
  leader->velocityY = 0x30000;
  FieldActor_SetMoveTarget(leader, leader->x, leader->y, z + 0x100000);
  Actor_WaitForMove(leader);
  Actor_SetSingleChildFlags(leader, 1);
  Event_PlaySound(390);
  Func_02003c98();
}

extern s32 gData_020043d4[];
extern s32 gData_020044d0[];
extern s32 gData_0200459c[];

s32 *Func_020007e4(void) {
  s32 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov663Room1)
    return gData_020043d4;
  if (scene == (s32)&SceneId_Ov663Room2)
    return gData_020044d0;
  return gData_0200459c;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000824(void) { Event_ResolvePendingActions(0); }
#undef FIELD_AT

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000830(void) {
  FIELD_AT(((s32)Ram_HeapSlots->effectWork + 0x34), u8, 0) = 1;
}
#undef FIELD_AT

#include "field_event.h"
#include "iwram_call.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

s32 Math_Cosine(s32 angle);

/// Circles the object around the centre in words 68 and 76, the radius
/// growing with its height, one step of 256 per lap.
void Func_02000840(struct FieldActor *object) {
  s32 radius = object->y;
  s32 step = WORD_AT(object, 48);

  radius >>= 1;
  radius += 0x100000;
  object->x = WORD_AT(object, 68) +
              Iwram_MulQ16(Math_Cosine((step & 255) << 11), radius);
  object->z =
      WORD_AT(object, 76) +
      Iwram_MulQ16(Math_Sin((WORD_AT(object, 48) & 255) << 11), radius) / 3;
  object->y += WORD_AT(object, 72);
  WORD_AT(object, 48)++;
}

/// The same circle on a smaller radius, twice as wide, blinking the part
/// attribute every other frame.
void Func_020008ac(struct FieldActor *object) {
  s32 radius = object->y;
  s32 step = WORD_AT(object, 48);

  radius >>= 2;
  radius += 0xc0000;
  object->x = WORD_AT(object, 68) +
              Iwram_MulQ16(Math_Cosine((step & 255) << 11), radius) * 2;
  object->z =
      WORD_AT(object, 76) +
      Iwram_MulQ16(Math_Sin((WORD_AT(object, 48) & 255) << 11), radius);
  object->y += WORD_AT(object, 72);
  WORD_AT(object, 48)++;
  Actor_SetSpriteColor(object, ((gFrameCount & 2) >> 1) * 9);
}

void Func_0200092c(struct Actor *object) {
  u8 *bytes = (u8 *)object;
  u32 on = ((gFrameCount & 3) >> bytes[99]) & 1;

  Actor_SetSpriteColor(object, (u8)(on * bytes[98]));
}

// The Whirlwind gust in Air's Rock.

#include "field_sprite.h"
#include "system/field_effect.h"
#include "system/vector3.h"

extern s16 gData_02004674[][4];
extern const u8 gData_02003d1c[];
void Func_02003b38(struct Actor *object, const u8 *script);
u32 Random16Far(void);
s32 Func_02003ba8(struct Actor *object);
void Func_02003b50(void);
void Func_02003b48(struct Actor *object);
void Func_02003c38(s32 actor, s32 a);
void Func_02003c30(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

struct EffectWork {
  u8 unknown[20];
  struct Actor *target;
};

/// The actor word at 84 seen as bit fields, for one store (see Func_0200095c).
struct MotionView {
  u8 pad[84];
  u32 kind : 8;
  u32 flags : 8;
};

/// A map cell, or a step between cells, kept as one 8-byte pair: written by the
/// word, it keeps the native's store before the travel loop.
struct Cell {
  s32 x, z;
};

/// Whirlwind on a target in Air's Rock: a gust (object type 280) rises from the
/// target and grows, shedding sparks every fourth frame while the target
/// flickers; then it travels in the leader's facing, two cells a step, until
/// Func_02000348 finds a cell with something on it, throwing leaves as it goes.
/// With nothing there (255) it ends with cue 136; otherwise it stops one and a
/// half cells short and blows the thing found along, copying its cells each
/// fourth frame, and sets that thing's flag (0x850 + hit - 1) with cue 298.
/// Then the gust shrinks and rises away over 8 frames.
void Func_0200095c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectWork *effect = (struct EffectWork *)Ram_HeapSlots->effectWork;
  struct Actor *gust, *spark;
  struct Vector3 velocity;
  struct EffectOptions leaves, dust;
  struct Cell delta, cell;
  s32 cellZ, cellX, newZ, newX, flag = 0, steps = 0, hit, angle, i;

  /* FAKEMATCH(wrapper): calls forwarded through call.h's Value4, Call2, Call4
   * and Call6, so each builds its own constants; as direct calls, 382 of 632
   * instructions differ in tla-en. */
  Func_02003bc8();
  Event_ResolvePendingActions(0);
  Sound_Play(207);
  gust = FieldActor_Create(280, leader->x, leader->y, leader->z);
  Func_02003b28(gust, 2);
  Actor_SetSingleChildFlags(gust, 0);
  gust->scaleX = 0x1b333;
  gust->scaleY = 0x3333;
  // Stored through a bit field view: its alias set frees the load after it, as
  // the native orders it (as gust->motionFlags, 2 of 632 differ in tla-en).
  ((struct MotionView *)gust)->flags = flag;
  ((struct FieldSprite *)gust->animation)->priority =
      ((struct FieldSprite *)effect->target->animation)->priority;
  Actor_SetMoveTarget(gust, effect->target->x, effect->target->y,
                      effect->target->z + 0xffff0000);
  while (gust->scaleY <= 0x14ccb) {
    gust->scaleY += 0x1999;
    Event_Wait(1);
  }
  Script_WaitForEventTimeout(gust);
  gust->z = effect->target->z + 0xffff0000;
  while (gust->scaleY <= 0x1cccb) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct Actor *)Import_ActorCreate(0x2a0, effect->target->x, 0,
                                                 effect->target->z);
      spark->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0xcccc;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16Far() & 0xffff;
      Func_02003b28(spark, 7);
      Func_02003b38(spark, gData_02003d1c);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetPartAttribute(spark, 2);
      spark->update = Func_02000840;
    }
    if (gFrameCount & 1) {
      // 655, not 455: 200 << 1 + 255 in the listing.
      gust->scaleX += 655;
      gust->scaleY += 655;
      Actor_SetPartAttribute(effect->target, 15);
    } else {
      Actor_SetPartAttribute(effect->target, 4);
    }
    Event_Wait(1);
  }
  Actor_SetPartAttribute(effect->target, 0);
  Sound_Play(136);
  cell.x = effect->target->x >> 20;
  cell.z = effect->target->z >> 20;
  angle = (leader->angle + 0x2000) & 0xc000;
  delta.x = Math_Cosine(angle) >> 16;
  delta.z = Math_Sine(angle) >> 16;
  do {
    hit = Func_02000348(2, cell.x, cell.z);
    hit >>= 8;
    cell.x += delta.x;
    cell.z += delta.z;
    steps += 2;
  } while (hit == 0);
  flag = Func_02000348(2, cell.x, cell.z);
  flag >>= 8;
  flag -= 1;
  steps -= 2;
  newX = ((s16 *)&effect->target->x)[1] + ((steps * delta.x) << 3);
  newZ = ((s16 *)&effect->target->z)[1] + ((steps * delta.z) << 3);
  ((struct FieldSprite *)gust->animation)->priority = 1;
  cell.x = newX;
  cell.z = newZ;
  gust->speedLimit = 0x40000;
  gust->acceleration = 0x40000;
  Actor_SetMoveTarget(gust, cell.x << 16, gust->y, cell.z << 16);
  FieldCamera_SetSpeed(0x50000, 0x50000);
  FieldCamera_MoveTo((s32)(newX << 16), (s32)(-1), (s32)(newZ << 16), (s32)(1));
  while (Func_02003ba8(gust) == 0) {
    if ((gFrameCount & 1) == 0) {
      leaves.priority = 1;
      leaves.type = 674;
      leaves.palette = 2;
      velocity.x = Math_Cosine((gFrameCount & 0x1ff) << 12) << 1;
      velocity.y = gust->y + ((Random16Far() & 31) << 16);
      velocity.z = Math_Sine((gFrameCount & 0x1ff) << 12);
      Func_0200015c(gust->x, velocity.y, gust->z, velocity.x, 0, velocity.z,
                    EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                    &leaves);
    }
    Event_Wait(1);
  }
  if (hit == 255) {
    Sound_Play(136);
  } else {
    cell.x = ((s16 *)&gust->x)[1] + delta.x * 48;
    cell.z = ((s16 *)&gust->z)[1] + delta.z * 48;
    gust->speedLimit = 0x8000;
    gust->acceleration = 0x8000;
    Actor_SetMoveTarget(gust, cell.x << 16, gust->y, cell.z << 16);
    Sound_Play(235);
    hit -= 30;
    while (Func_02003ba8(gust) == 0) {
      if ((gFrameCount & 1) == 0) {
        dust.priority = 1;
        dust.startScaleX = dust.startScaleY =
            (Random16Far() & 0xffff) + 0x10000;
        velocity.x = Math_Cosine((gFrameCount & 0x1ff) << 12) << 1;
        velocity.y = 0x10000;
        velocity.z = Math_Sine((gFrameCount & 0x1ff) << 12);
        Func_0200015c(gust->x, gust->y, gust->z, velocity.x, velocity.y,
                      velocity.z, EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
                      &dust);
      }
      if ((gFrameCount & 3) == 0) {
        s32 sourceX;
        cell.x = gData_02004674[hit][2] + (gust->x >> 20);
        cell.z = gData_02004674[hit][3] + (gust->z >> 20);
        cellX = cell.x;
        cellZ = cell.z;
        if (hit & 1) {
          sourceX = (u16)gData_02004674[hit][0];
          MapGrid_CopyCellsTo((s16)sourceX, gData_02004674[hit][1], cellX,
                              cellZ - 1, 1, 3);
          MapGrid_CopyCellAttributes(gData_02004674[hit][0],
                                     gData_02004674[hit][1], 1, 2, cellX,
                                     cellZ);
          MapGrid_CopyCellAttributes(gData_02004674[hit][0],
                                     gData_02004674[hit][1], 1, 2,
                                     gust->x >> 20, gust->z >> 20);
        } else {
          sourceX = (u16)gData_02004674[hit][0];
          MapGrid_CopyCellsTo((s16)sourceX, gData_02004674[hit][1], cellX,
                              cell.z - (hit >> 1), 1, 1);
          MapGrid_CopyCellAttributes(gData_02004674[hit][0],
                                     gData_02004674[hit][1], 1, 1, cellX,
                                     cellZ);
          MapGrid_CopyCellAttributes(gData_02004674[hit][0],
                                     gData_02004674[hit][1], 1, 1,
                                     gust->x >> 20, gust->z >> 20);
        }
        Func_02003b50();
      }
      Event_Wait(1);
    }
    if (hit == 2) {
      MapGrid_CopyCellsTo(gData_02004674[2][0], gData_02004674[2][1], cell.x,
                          cell.z, 1, 1);
      Func_02003b50();
    }
    Sound_Play(298);
    Event_SetFlag(flag + 0x850);
  }
  gust->acceleration = 0;
  gust->speedLimit = 0;
  gust->targetZ = 0;
  gust->targetY = 0;
  gust->targetX = 0;
  for (i = 0; i != 8; i++) {
    gust->scaleX -= 0x2666;
    gust->scaleY -= 0x4000;
    gust->y += 0x38000;
    Event_Wait(1);
  }
  Func_02003b48(gust);
  Event_Wait(30);
  Func_02003c38(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  Func_02003bd0();
}

/// The wave scroll that overlay 696's Func_02000378 also has.
#define WAVE_SCROLL Func_02000ef4
#define WAVE_SCROLL_LINES gData_020046a0
#include "../field/field_effect/wave_scroll.c"

/// Moves the object: left of x 320 it speeds up rightward and grows, past it
/// it speeds up leftward and shrinks; its priority follows its direction and
/// its sprite spins.
void Func_02000f9c(struct Actor *object) {
  s32 scale;

  object->y += object->gravity;
  object->x += object->bounce;
  object->z += object->unk4c;
  if ((object->x >> 19) <= 39) {
    object->bounce += 0x18000;
    scale = object->scaleX + 0xccc;
  } else {
    object->bounce -= 0x18000;
    scale = object->scaleX - 0x51e;
  }
  object->scaleX = scale;
  object->scaleY = scale;
  if (object->bounce > 0)
    Scene_SetActorSpritePriority(object, 1);
  else
    Scene_SetActorSpritePriority(object, 2);
  ((struct FieldSprite *)object->animation)->rotation += 0x1800;
}

#include "system/dma.h"
#include "system/io_reg.h"

struct SpriteParts {
  u8 unk00[9];
  u8 unk09Low : 2;
  u8 part : 2;
};
struct ObjBits {
  u8 unk00[35];
  u8 flags : 8;
};
extern char ResourceId_WhirlwindMap;
extern char ResourceId_WhirlwindTiles;
void *Resource_GetTableEntry(s32 id);
void Func_02003af0(void *resource, void *dest);

void Func_020024b0(void);
void Func_0200257c(void);
void Func_02000ef4(void);
void Func_02003b00(void);
void Func_02003af8(void);

void Func_02001010(void) {
  struct MainRoutineWork *work;
  u16 *buffer = Heap_Alloc(0x680);
  u16 *cell;
  struct Actor *spark;
  u32 i;
  s32 x;

  Func_02003bc8();
  FieldEvent_ResolvePendingActions(0);
  REG_DISPCNT &= ~0x100;
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_WhirlwindTiles),
                buffer);
  Dma_Set(buffer, (void *)0x600e800, 0x840000d0, (volatile u32 *)0x040000d4);
  Dma_Set((u8 *)buffer + 0x340, (void *)0x600ec00, 0x840000d0,
          (volatile u32 *)0x040000d4);
  Heap_Release(buffer);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  FieldActor_SetSpritePriority(8, 1);
  {
    s32 control = 0x40d;

    REG_BG0CNT = control;
  }
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 2;
  Dma_Fill16(0x10, (void *)0x6002000, 0x500);
  buffer = Heap_Alloc(0x240);
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_WhirlwindMap), buffer);
  i = 0;
  cell = buffer;
offset:
  *cell += 0xff40;
  i++;
  cell++;
  if (i != 0x120)
    goto offset;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(0x600200f + (i << 6)), 0x84000008,
          (volatile u32 *)0x040000d4);
  i++;
  cell += 16;
  if (i != 18)
    goto row;
  Heap_Release(buffer);
  Event_PlaySound(246);
  ((u8 *)Actor_Get(8))[98] = 9;
  ((u8 *)Actor_Get(8))[99] = 1;
  Actor_Get(8)->update = Func_0200092c;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = FieldActor_Create(
          0x2a0, Actor_Get(8)->x, Actor_Get(8)->y, Actor_Get(8)->z);
      {
        s32 priority = ((s8 *)Actor_Get(8)->animation)[9] & 12;

        cell = spark->animation;
        ((struct SpriteParts *)cell)->part = priority >> 2;
      }
      ((struct ObjBits *)spark)->flags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0x18000;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16Far() & 0xffff;
      Func_02003b28(spark, 7);
      Func_02003b38(spark, gData_02003d1c);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetSpriteColor(spark, 2);
      spark->update = Func_020008ac;
    }
    FieldEvent_Wait(1);
  }
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  FieldCamera_SetSpeed(0x10000, 0x2000);
  FieldCamera_MoveTo(0x14a0000, -1, 0x490000, 1);
  Func_02003c30();
  Task_AddFrame(Func_02000ef4, 0xc80);
  Event_PlaySound(163);
  REG_BLDALPHA = 0xf00;
  REG_BLDCNT = 0x3f41;
  REG_DISPCNT |= 0x100;
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  i = 0;
glow:
  REG_BLDALPHA = ((15 - (i >> 2)) << 8) | (i >> 2);
  ((u8 *)Actor_Get(8))[98] = Random16Far() & 7;
  i++;
  FieldEvent_Wait(1);
  if (i != 64)
    goto glow;
  FieldEvent_Wait(60);
  REG_BLDALPHA = 15;
  REG_BLDCNT = 0xc42;
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  {
    u32 j = 0;

  burst:
    j++;
    if ((j & 3) == 0) {
      struct Vector3 velocity;
      struct EffectOptions options;

      options.priority = 1;
      options.update = Func_02000f9c;
      velocity.x = 0;
      velocity.y = 0;
      velocity.z = 0xfffe0000;
      x = ((Random16Far() % 144) << 16) + 0x1000000;
      Func_0200015c(x, 0, (Random16Far() % 144) << 16, velocity.x, velocity.y,
                   velocity.z, EFFECT_USE_UPDATE | EFFECT_USE_PRIORITY | 1,
                   &options);
    }
    REG_BLDALPHA = (((j >> 4) & 15) << 8) | (16 - ((j >> 4) & 15));
    ((u8 *)Actor_Get(8))[98] = Random16Far() & 7;
    FieldEvent_Wait(1);
    if (j != 255)
      goto burst;
  }
  MapGrid_CopyCellsTo(68, 0, 79, 0, 11, 10);
  MapGrid_CopyCellAttributes(38, 9, 1, 1, 16, 9);
  MapGrid_CopyCellAttributes(38, 9, 1, 1, 24, 9);
  Func_02003b50();
  FieldEvent_Wait(1);
  REG_BLDCNT = 0x3f41;
  REG_BLDALPHA = 15;
  MapGrid_SetShake(-1, -1, 0xe666);
  Event_PlaySound(390);
  ((u8 *)Actor_Get(8))[98] = 6;
  i = 0;
fade:
  REG_BLDALPHA = ((i >> 2) << 8) | (16 - (i >> 2));
  FieldEvent_Wait(1);
  i++;
  if (i != 65)
    goto fade;
  FieldTask_Remove(Func_02000ef4);
  {
    s32 zero = 0;

    ((u8 *)Actor_Get(8))[99] = zero;
    System_Wait(30);
    Actor_Get(8)->update = (void (*)(void))zero;
    Actor_SetSpriteColor(Actor_Get(8), 6);
    MapGrid_WaitForShakeToSettle();
    FieldEvent_Wait(60);
    REG_BLDCNT = zero;
  }
  Func_02003b00();
  Func_02003af8();
  work = &gMainRoutineWork;
  WindowColor_Apply(work->paletteGlow[0], work->paletteGlow[1]);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 1;
  {
    s32 *ownerId = &work->currentOwner;

    Actor_Get(*ownerId)->priorityFlags &= ~32;
    Actor_Get(*ownerId)->priorityFlags |= 1;
    {
      u8 *flags = &Actor_Get(8)->priorityFlags;
      u8 v = *flags | 1;

      *flags = v;
    }
    FieldCamera_FollowActor(*ownerId, 1);
  }
  Func_02003c30();
  Event_SetFlag(0x858);
  Func_02003bd0();
}

#include "input.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

void Func_02001560(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *ride = leader->watchedObject;
  struct Actor *rider, *dust;
  u8 *riding, *motion;
  s32 rise, dx, dz, moved, i, height;
  s32 angle;
  u16 facing;
  s32 still;

  Func_02003bc8();
  Event_ResolvePendingActions(0);
  motion = &leader->motionFlags;
  *motion = 4;
  Actor_SetSingleChildFlags(leader, 0);
  GameFlag_SetBit(0x250);
  Sound_Play(137);
  riding = (u8 *)ride + 99;
  rise = 0;
  while (*riding != 0) {
    leader->x = ride->x;
    leader->y = ride->y + rise;
    leader->z = ride->z;
    if (rise <= 0xbffff)
      rise += 0x1999;
    leader->angle = (u8)gFrameCount << 12;
    leader->priorityFlags ^= 2 << (gFrameCount & 1);
    wait(1);
  }
  Sound_Play(0x120);
  facing = ride->angle;
  if (facing == 0xc000) {
    rider = ride->watchedObject;
    dx = 0;
    dz = 0;
    leader->priorityFlags = 4;
    leader->x = (((leader->x >> 19) & ~1) + 1) << 19;
    i = 0;
    leader->y = (((ride->y >> 19) & ~1) + 3) << 19;
    leader->z = (((leader->z >> 19) & ~1) + 1) << 19;
  waiting:
      moved = 0;
      if (gInput.held & 0x40) {
        dx = 0;
        dz = 0xfff00000;
        leader->angle = 0xc000;
        moved = 1;
      }
      if (gInput.held & 0x10) {
        dx = 0x100000;
        dz = 0;
        leader->angle = dz;
        moved = 1;
      }
      if (gInput.held & 0x20) {
        dx = 0xfff00000;
        leader->angle = 0x8000;
        dz = 0;
        moved = 1;
      }
      if (moved && Func_02000348(2, (ride->x + dx) >> 20,
                                 (ride->z - ride->y + dz) >> 20) >> 8 != 255)
        goto found;
      wait(1);
      if (++i != 60)
        goto waiting;
    dx = 0;
    leader->angle = 0xc000;
    dz = 0xfff00000;
    if ((u32)(Func_02000348(2, ride->x >> 20, (ride->z - ride->y + dz) >> 20)
              << 8) >> 16 != 255)
      goto found;
    dx = 0x100000;
    dz = 0;
    leader->angle = dz;
    if ((u32)(Func_02000348(2, (ride->x + dx) >> 20,
                            (ride->z - ride->y) >> 20)
              << 8) >> 16 != 255)
      goto found;
    dx = 0xfff00000;
    leader->angle = 0x8000;
  found:
    height = Map_GetTerrainHeight(((u8 *)rider)[98], ride->x + dx,
                                  ride->z + dz);
    leader->y = height;
    leader->terrainHeight = height;
    Func_02003b28(leader, 6);
    wait(6);
    Sound_Play(152);
    Func_02003b28(leader, 7);
    leader->speedLimit = 0x20000;
    leader->acceleration = 0x8000;
    leader->velocityY = 0x40000;
    *motion = 2;
    leader->priorityFlags = 1;
    leader->terrainId = ((u8 *)rider)[98];
    FieldActor_SetMoveTarget(leader, leader->x + dx, leader->y, leader->z + dz);
    OverlayObject_WaitUntilIdle(leader);
    Actor_WaitForMove(leader);
    Actor_SetSingleChildFlags(leader, 1);
    *motion = 3;
  } else {
    angle = ride->angle;
    leader->x = (((ride->x >> 19) & ~1) + (angle >>= 13) - 1) << 19;
    leader->z = (((ride->z >> 19) & ~1) - (angle & 2) + 1) << 19;
    still = 0;
    Actor_SetSingleChildFlags(leader, 1);
    leader->velocityY = 0x60000;
    leader->priorityFlags = 1;
    *motion = 3;
    FieldCamera_MoveTo(-1, -1, -1, 0);
    dust = FieldCamera_GetActor();
    dust->speedLimit = 0x80000;
    dust->acceleration = 0x10000;
    dust->motionFlags = still;
    FieldActor_SetMoveTarget(dust, leader->x,
                  Map_GetTerrainHeight(0, leader->x, leader->z), leader->z);
    OverlayObject_WaitUntilIdle(leader);
    Actor_WaitForMove(dust);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Event_ClearFlag(0x250);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  wait(10);
  Func_02003bd0();
}

s32 Func_020018ac(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

s32 Func_020018d8(s32 *a, s32 *b) {
  s32 dx = (*a++ - *b++) >> 16;
  s32 dy = (*a - *b + 0x60000) >> 16;

  return Iwram_Sqrt(dx * dx + dy * dy);
}

#include "field/event_work.h"

extern u8 gData_02003de0[], gData_02003dcc[];
struct Shot {
  u8 unk00[0x62];
  u8 unk62; ///< set once it has hit the leader
  u8 unk63; ///< set with unk62, cleared when it ends
};

void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
void Func_02003cd0(struct Actor *object, s32 cue);

void Func_02001908(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u8 *work = Ram_HeapSlots->eventWork;
  s32 speed = 0x18000;
  s32 cue = 539;
  struct Actor *other, *launcher;
  s32 i;
  s32 *leaderPosition;
  s32 *shotPosition;
  struct Actor **objects;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0 ||
      gMainRoutineWork.renderMode == 5) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, 16);
  if (Func_02000348(2, shot->x >> 20, (shot->z - shot->y) >> 20) >> 8 == 212) {
  end:
    Func_02003cd0(shot, cue);
    ((struct Shot *)shot)->unk63 = 0;
    shot->update = 0;
    if (shot->angle == 0xc000)
      Func_02003b38(shot, gData_02003de0);
    else
      Func_02003b38(shot, gData_02003dcc);
    return;
  }
  if (((struct Shot *)shot)->unk62 == 0) {
    // i first says whether the shot is beside the leader; the loop below
    // reuses it as the object index.
    i = 0;
    leaderPosition = &leader->x;
    shotPosition = &shot->x;
    if (gMainRoutineWork.renderMode == 2) {
      if (Func_020018d8(leaderPosition, shotPosition) <= 16 &&
          leader->z >> 20 == shot->z >> 20)
        i = 1;
    } else {
      if (Func_020018ac(leaderPosition, shotPosition) <= 8 &&
          leader->y >> 20 == shot->y >> 20)
        i = 1;
    }
    if (i && !GameFlag_Test(0x104)) {
      leader->angle = shot->angle;
      leader->watchedObject = shot;
      leader->priorityFlags &= ~1;
      ((struct FieldSprite *)leader->animation)->priority =
          ((struct FieldSprite *)shot->animation)->priority;
      ((struct EventWork *)work)->unk16a = 200;
      gMainRoutineWork.renderMode = 0;
      ((struct Shot *)shot)->unk62 = 1;
      ((struct Shot *)shot)->unk63 = 1;
    }
  }
  objects = (struct Actor **)(work + 0x34);
  for (i = 8; i != 64; i++) {
    other = *objects++;
    if (other->script != 0) {
      launcher = shot->watchedObject;
      if (launcher->x >> 20 != other->x >> 20 ||
          launcher->y >> 20 != other->y >> 20 ||
          launcher->z >> 20 != other->z >> 20) {
        if (Func_020018ac(&other->x, &shot->x) <= 8 &&
            other->y >> 20 == shot->y >> 20)
          goto end;
      }
    }
  }
  if (shot->angle == 0xc000) {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), speed);
    shot->y -= Iwram_MulQ16(Math_Sin(shot->angle), speed);
  } else {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), speed);
    shot->z += Iwram_MulQ16(Math_Sin(shot->angle), speed);
  }
}

#include "call.h"

// A launcher's update, the Air's Rock Inside launcher with a new shot each
// time: while none of the event work's four busy counters is set it counts
// frames in its action parameter, and at 240 launches a shot 8 pixels ahead
// along its angle with a dust effect below it.
void Func_02001b54(struct Actor *object) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *shot;
  struct Vector3 position;
  struct EffectOptions options;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0)
    return;
  if (object->action == 240) {
    if (!Event_GetFlag(0x250))
      Func_02003cd0(object, 202);
    if (object->angle == 0xc000) {
      position.x = object->x;
      position.z = object->z;
    } else {
      /* FAKEMATCH(wrapper): forwarded, so each product builds its own constant and
       * they share the routine's address; as direct calls, 106 of 153
       * instructions differ in tla-en. */
      position.x = object->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                      Math_Cosine(object->angle));
      position.z = object->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                      Math_Sin(object->angle));
    }
    shot = FieldActor_Create(280, position.x,
                                                       object->y, position.z);
    Func_02003b28(shot, 2);
    Actor_SetSingleChildFlags(shot, 0);
    ((struct FieldSprite *)shot->animation)->priority = 1;
    shot->motionFlags = 0;
    ((u8 *)shot)[0x62] = 0;
    ((u8 *)shot)[0x63] = 0;
    shot->angle = object->angle;
    shot->update = Func_02001908;
    shot->watchedObject = object;
    object->action = 0;
    options.priority = 1;
    options.palette = 7;
    Func_0200015c(position.x, object->y, position.z - 0x60000, 0, 0, 0,
                  EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
  object->action++;
}

struct AirsRockEventWork {
  u8 unk000[0xcbc];
  s32 valueCbc;
};

extern s16 gData_02003d94[];

void Func_02003cb8(const void *data);

void Func_02001ca4(void) {
  struct AirsRockEventWork *work = Ram_HeapSlots->eventWork;

  Func_02003cb8(gData_02003d94);
  Func_02000468(work->valueCbc);
}

void Func_02003cc0(void);

void Func_02001cc8(void) {
  struct Actor *object =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
  s32 x, z;

  Func_02003bc8();
  FieldEvent_ResolvePendingActions(0);
  Func_02003cc0();
  Func_020003e8(((u8 *)gData_02003d94));
  if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
    object->terrainId = 2;
    object->motionFlags = 3;
    OverlayObject_WaitUntilIdle(object);
    OverlayObject_WaitUntilIdle(object);
    Event_PlaySound(188);
    object->motionFlags = 0;
    object->terrainId = 0;
  }
  x = object->x >> 20;
  z = object->z >> 20;
  if (x == 16 && z == 43)
    Event_SetFlag(0x85f);
  else if (x == 45 && z == 45)
    Event_SetFlag(0x860);
  else if (x == 21 && z == 48)
    Event_SetFlag(0x861);
  else if (x == 39 && z == 49)
    Event_SetFlag(0x862);
  else if (x == 49 && z == 18)
    Event_SetFlag(0x887);
  Func_02003bd0();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02003ca8();
extern s16 gData_02003e4c[];

void Func_02001da8(void) {
  Func_02003bc8();
  FieldEvent_ResolvePendingActions(0);
  Func_02003ca8((s32)((u8 *)gData_02003e4c));
  Func_02003bd0();
}
#undef FIELD_AT

void Func_02003c90(void *sprite, void *source);
extern u8 gData_02003dfc[];

void Func_02001dc4(s32 x, s32 z) {
  struct Actor *first, *rock;
  struct EffectOptions options;
  struct Vector3 scatter;
  s32 i;
  s32 radius = 0x400000;
  s32 shakeA = -1;
  s32 shakeC = 0xe666;

  first = FieldActor_Create(255, x, 0, z);
  options.priority = 1;
  options.startScaleX = 0x20000;
  options.startScaleY = 0x20000;
  for (i = 0; i != 12; i++) {
    scatter.x = x + ((Random16Far() & 63) << 16) - 0x200000;
    scatter.y = (Random16Far() & 15) << 12;
    scatter.z = z + ((Random16Far() & 63) << 16) - 0x300000;
    if (i & 3)
      Func_0200015c(scatter.x, 0, scatter.z, 0, scatter.y, 0,
                    EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | 1, &options);
    else
      Func_0200015c(scatter.x, 0, scatter.z, 0, scatter.y, 0,
                    EFFECT_USE_PRIORITY | 1, &options);
  }
  for (i = 0; i != 6; i++) {
    rock = first;
    if (i != 0) {
      rock = FieldActor_Create(255, x, 0, z);
      Func_02003c90(rock->animation, first->animation);
    }
    Actor_SetSingleChildFlags(rock, 0);
    Func_02003b38(rock, gData_02003dfc);
    Func_02003b28(rock, 2);
    rock->velocityY = ((Random16Far() & 3) << 16) + 0x40000;
    rock->gravity = 0x3333;
    rock->bounce = 0x6666;
    rock->scaleX = 0xcccc;
    rock->scaleY = 0xcccc;
    rock->speedLimit = 0xcccc;
    scatter.y = Random16Far() & 7;
    scatter.x = Iwram_MulQ16(Math_Cosine(scatter.y << 12), radius);
    scatter.z = Iwram_MulQ16(Math_Sin(scatter.y << 12), radius);
    ((struct FieldSprite *)rock->animation)->priority = 1;
    FieldActor_SetMoveTarget(rock, rock->x + scatter.x, rock->y,
                        rock->z + scatter.z);
  }
  MapGrid_SetShake(0x50000, 0x70000, 0x10000);
  MapGrid_SetShake(shakeA, shakeA, shakeC);
}

#include "bg_scroll.h"

/// A field sprite's byte 9: the top of its first tile and its priority (OAM
/// attribute 2), read as bit-fields.
struct SpriteAttr2High {
  u8 unk00[9];
  u8 tileHigh : 2;
  u8 priority : 2;
};
/// An object's priority flags read as a whole-byte bit-field.
struct ObjectPriorityByte {
  u8 unk00[35];
  u8 priorityFlags : 8;
};
extern char ResourceId_LightningMap;
extern char ResourceId_LightningTiles;
void Func_02005dcc(s32 a, s32 b, s32 c);

/// The pillar of light in Air's Rock, overlay 664's Func_02003654 with object
/// 14 glowing: the camera moves to (0x218, 0xd8), the leader turns, and the
/// light flashes twice (cue 138), each time copying cells into the map
/// (Map_CopyCellPictureIndicesRect at (52, 12) and (52, 22)) and lighting a spot
/// (Func_02001dc4), then (cue 145) three more spots light and the cells at
/// (62, 2) open; sets flag 0x87b.
void Func_02001f7c(void) {
  u16 *buffer = Heap_Alloc(0x800);
  u16 *cell;
  struct Actor *spark;
  u32 i;

  Func_02003bc8();
  FieldEvent_ResolvePendingActions(0);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  /* FAKEMATCH(dead): BG0CNT's value in a once-only block of its own, built
   * before its address; as a plain block, 2 of 520 instructions differ in
   * tla-en. */
  do {
    s32 control = BGCNT_SCREEN_BASE(4);

    REG_BG0CNT = control;
  } while (0);
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_LightningTiles),
                buffer);
  Dma_Set(buffer, (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(512), REG_DMA3);
  Heap_Release(buffer);
  Dma_Fill16(0, (void *)BG_SCREEN_BLOCK(4), 0x800);
  buffer = Heap_Alloc(0x80);
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_LightningMap), buffer);
  i = 0;
  cell = buffer;
mask:
  *cell &= 0xfcff;
  i++;
  cell++;
  if (i != 64)
    goto mask;
  cell = buffer;
  i = 0;
row:
  Dma_Set(cell, (void *)(BG_SCREEN_BLOCK(4) + (i << 6)), DMA_COPY_WORDS(2),
          REG_DMA3);
  i++;
  cell += 4;
  if (i != 16)
    goto row;
  Heap_Release(buffer);
  Event_PlaySound(246);
  ((u8 *)Actor_Get(14))[98] = 9;
  ((u8 *)Actor_Get(14))[99] = 1;
  Actor_Get(14)->update = Func_0200092c;
  for (i = 0; i != 45; i++) {
    if ((gFrameCount & 3) == 0) {
      spark = (FieldActor_Create(0x2a0, Actor_Get(14)->x,
                                  Actor_Get(14)->y, Actor_Get(14)->z));
      {
        s32 priority = ((s8 *)Actor_Get(14)->animation)[9] & 12;

        /* FAKEMATCH(temporary): the sprite held in the row pointer; as
         * spark->animation, 20 of 520 instructions differ in tla-en. */
        cell = spark->animation;
        ((struct SpriteAttr2High *)cell)->priority = priority >> 2;
      }
      // Through the bit-field views the flags' load is issued before the
      // priority store, as the native does; as plain bytes it waits for it.
      ((struct ObjectPriorityByte *)spark)->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0x18000;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16Far() & 0xffff;
      Func_02003b28(spark, 7);
      Func_02003b38(spark, gData_02003d1c);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetSpriteColor(spark, 2);
      spark->update = Func_020008ac;
    }
    FieldEvent_Wait(1);
  }
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(60);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  FieldCamera_SetSpeed(0x10000, 0x2000);
  FieldCamera_MoveTo(PIXELS(536), -1, PIXELS(216), 1);
  Func_02003c30();
  Actor_Get(14)->update = 0;
  Actor_SetSpriteColor(Actor_Get(14), 6);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  gBgScroll[0].x = 0xff88;
  gBgScroll[0].y = 0x10;
  System_Wait(30);
  Event_PlaySound(138);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  Actor_Get(gMainRoutineWork.currentOwner)->angle = FACING_SOUTH;
  System_Wait(1);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellsTo(52, 12, 29, 9, 9, 9);
  MapGrid_CopyCellsTo(52, 76, 29, 73, 9, 9);
  Func_02001dc4(0x2280000, 0xe40000);
  i = 0;
fade1:
  REG_BLDALPHA = BLEND_ALPHA(16 - (i >> 1), 16);
  System_Wait(1);
  i++;
  if (i != 32)
    goto fade1;
  gBgScroll[0].x = 0xffac;
  gBgScroll[0].y = 0;
  System_Wait(1);
  Event_PlaySound(138);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(8);
  REG_DISPCNT |= DISPCNT_BG0_ON;
  MapGrid_CopyCellsTo(52, 22, 29, 9, 9, 9);
  MapGrid_CopyCellsTo(52, 86, 29, 73, 9, 9);
  Func_02001dc4(0x2080000, 0xf20000);
  i = 0;
fade2:
  REG_BLDALPHA = BLEND_ALPHA(16 - (i >> 1), 16);
  System_Wait(1);
  i++;
  if (i != 32)
    goto fade2;
  Event_PlaySound(145);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  FieldEvent_Wait(20);
  Func_02001dc4(0x2000000, 0xc60000);
  Func_02001dc4(0x2320000, 0xf20000);
  Func_02001dc4(0x2090000, 0xdf0000);
  MapGrid_CopyCellsTo(62, 2, 29, 9, 9, 9);
  MapGrid_CopyCellAttributes(62, 2, 9, 9, 29, 9);
  MapGrid_CopyCellsTo(62, 66, 29, 73, 9, 9);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10003, 0);
  PaletteFade_Interpolate(60);
  System_Wait(60);
  PaletteFade_ApplySource(0x10003, 0);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(60);
  System_Wait(70);
  Func_02003b00();
  Func_02003af8();
  WindowColor_Apply(gMainRoutineWork.paletteGlow[0], gMainRoutineWork.paletteGlow[1]);
  REG_BLDCNT = 0;
  REG_DISPCNT |= DISPCNT_BG0_ON;
  gBgScroll[0].x = 0;
  gBgScroll[0].y = 0;
  Event_SetFlag(0x87b);
  Func_02003bd0();
}

extern s8 gData_02004694;
extern s8 gData_02004695;
extern s16 gData_020046a0[];

void Func_020024b0(void) {
  s32 *scroll = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x178);
  s16 *line = gData_020046a0;
  s32 i;
  volatile u16 *channel;

  REG_BLDALPHA = gData_02004694 | 0x1000;
  if ((*Ram_FrameCount & 15) == 0) {
    gData_02004694 += gData_02004695;
    if (gData_02004694 == 8 || gData_02004694 == 4)
      gData_02004695 *= -1;
  }
  for (i = 0; i != 160; i++) {
    *line =
        (scroll[0] >> 16) +
        (Math_Sin((i + (u8)*Ram_FrameCount + (scroll[1] >> 16)) << 9) >> 13);
    line++;
  }
  line = gData_020046a0;
  REG_BG1HOFS = line[0];
  channel = REG_DMA0;
  channel[5] &= 0xc5ff;
  channel[5] &= 0x7fff;
  channel[5];
  Dma_Set(line + 1, (void *)&REG_BG1HOFS, 0xa2600001, (volatile u32 *)channel);
}

extern s8 gData_02004696;
extern s8 gData_02004697;

void Func_0200257c(void) {
  s32 *scroll = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x178);
  s16 *line = gData_020046a0;
  s32 i, wave, spread, value;
  s16 *table;
  volatile u16 *channel;

  for (i = 0; i != 160; i++) {
    spread = (160 - i) >> 2;
    wave = Math_Sin((i + (u8)gFrameCount) << 8);
    /* FAKEMATCH(temporary): value holds line[0]'s first value, then
     * wave + y for line[2]; as the plain statements, 41 of 135 instructions
     * differ in tla-en. */
    value = (Iwram_MulQ16(wave, (144 - i) << 10) >> 11) - 4;
    line[0] = value;
    line[0] += Random16Far() % (spread * 2) - spread;
    if (line[0] > 56)
      line[0] -= 56;
    else if (line[0] < -64)
      line[0] += 64;
    line[1] = 0;
    value = wave + (scroll[1] >> 16);
    line[2] = (scroll[0] >> 16) + (value >> 13);
    line += 3;
  }
  REG_BLDALPHA = gData_02004696 | 0x1000;
  if ((gFrameCount & 15) == 0) {
    gData_02004696 += gData_02004697;
    if (gData_02004696 == 8 || gData_02004696 == 4)
      gData_02004697 *= -1;
  }
  table = gData_020046a0;
  *(volatile u16 *)REG_BG0HOFS = table[0];
  table += 2;
  REG_BG1HOFS = table[0];
  channel = REG_DMA0;
  channel[5] &= 0xc5ff;
  channel[5] &= 0x7fff;
  channel[5];
  Dma_Set(table + 1, REG_BG0HOFS, 0xa2600003, (volatile u32 *)channel);
}

void Func_020026b8(void) {
  u16 *buffer = Heap_Alloc(0x680);
  s32 *beam = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x178);
  u16 *cell;
  struct Actor *spark;
  s32 i;
  s32 z;

  Func_02003bc8();
  FieldEvent_ResolvePendingActions(0);
  REG_DISPCNT &= ~0x100;
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_WhirlwindTiles),
                buffer);
  Dma_Set(buffer, (void *)0x600e800, 0x840000d0, (volatile u32 *)0x040000d4);
  Dma_Set((u8 *)buffer + 0x340, (void *)0x600ec00, 0x840000d0,
          (volatile u32 *)0x040000d4);
  Heap_Release(buffer);
  {
    s32 control = 0x40e;

    REG_BG0CNT = control;
  }
  // A once-only loop: its loop notes order the fill's address ahead of its
  // value.
  do {
    Dma_Fill16(0x10, (void *)0x6002000, 0x500);
  } while (0);
  buffer = Heap_Alloc(0x240);
  Func_02003af0(Resource_GetTableEntry((s32)&ResourceId_WhirlwindMap), buffer);
  cell = buffer;
  for (i = 0; i != 0x120; i++) {
    *cell += 0xff40;
    cell++;
  }
  cell = buffer;
  for (i = 0; i != 18; i++) {
    Dma_Set(cell, (void *)(0x600200f + (i << 6)), 0x84000008,
            (volatile u32 *)0x040000d4);
    cell += 16;
  }
  Heap_Release(buffer);
  Event_PlaySound(246);
  ((u8 *)Actor_Get(12))[98] = 9;
  ((u8 *)Actor_Get(12))[99] = 1;
  Actor_Get(12)->update = Func_0200092c;
  for (i = 0; i != 45; i++) {
    s32 low = gFrameCount & 3;

    if (low == 0) {
      spark = (FieldActor_Create(0x2a0, Actor_Get(12)->x,
                                                  Actor_Get(12)->y,
                                                  Actor_Get(12)->z));
      {
        s32 priority = ((s8 *)Actor_Get(12)->animation)[9] & 12;

        cell = spark->animation;
        ((struct SpriteParts *)cell)->part = priority >> 2;
      }
      ((struct ObjBits *)spark)->flags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0x18000;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16Far() & 0xffff;
      Func_02003b28(spark, 7);
      Func_02003b38(spark, gData_02003d1c);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetSpriteColor(spark, 2);
      spark->update = Func_020008ac;
    }
    FieldEvent_Wait(1);
  }
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  FieldCamera_SetSpeed(0x10000, 0x2000);
  FieldCamera_MoveTo(0x980000, -1, 0x900000, 1);
  Func_02003c30();
  FieldTask_Remove(Func_020024b0);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 0;
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(1);
  System_Wait(1);
  PaletteFade_ApplySource(0x7fff, 0);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(8);
  Task_AddFrame(Func_0200257c, 0xc80);
  Event_PlaySound(163);
  REG_DISPCNT |= 0x100;
  MapGrid_SetShake(0x30000, 0x30000, 0x10000);
  z = 0xffff8000;
  for (i = 0x4000; i != 0x48000; i += 0x400, z += 0xfffffc00) {
    beam[6] = i;
    beam[7] = z;
    ((u8 *)Actor_Get(12))[98] = Random16Far() & 7;
    System_Wait(1);
  }
  FieldTask_Remove(Func_0200257c);
  Task_AddFrame(Func_02000ef4, 0xc80);
  REG_BLDALPHA = 8;
  REG_BLDCNT = 0x3f42;
  for (i = 0; i != 257; i++) {
    REG_BLDALPHA = (8 - (i >> 5)) | 0x1000;
    ((u8 *)Actor_Get(12))[98] = Random16Far() & 7;
    FieldEvent_Wait(1);
  }
  REG_DISPCNT &= ~0x200;
  System_Wait(30);
  Event_PlaySound(390);
  REG_BLDALPHA = 0x10;
  REG_BLDCNT = 0x3f41;
  MapGrid_SetShake(-1, -1, 0xe666);
  ((u8 *)Actor_Get(12))[98] = 6;
  for (i = 0; i != 129; i++) {
    REG_BLDALPHA = ((i >> 3) << 8) | (0x10 - (i >> 3));
    FieldEvent_Wait(1);
  }
  FieldTask_Remove(Func_02000ef4);
  {
    s32 zero = 0;

    ((u8 *)Actor_Get(12))[99] = zero;
    System_Wait(30);
    Actor_Get(12)->update = (void (*)(void))zero;
    Actor_SetSpriteColor(Actor_Get(12), 6);
    MapGrid_WaitForShakeToSettle();
    FieldEvent_Wait(60);
    REG_BLDCNT = zero;
  }
  Func_02003b00();
  Func_02003af8();
  WindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 1;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags &= ~32;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  Actor_Get(12)->priorityFlags |= 1;
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  Event_SetFlag(0x87a);
  Func_02003bd0();
}

extern s32 gData_02004698;

void Func_02002b88(void) {
  struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
  s32 level;

  actor->priorityFlags |= 32;
  level = ((actor->y + 0x200000) >> 16) / 112;
  if (gData_02004698 != level) {
    switch (level) {
  case 3:
    MapGrid_CopyCellAttributes(64, 64, 21, 3, 27, 54);
    break;
  case 2:
    MapGrid_CopyCellAttributes(64, 67, 21, 3, 27, 54);
    break;
  case 1:
    MapGrid_CopyCellAttributes(64, 70, 21, 3, 27, 54);
    break;
  case 0:
    MapGrid_CopyCellAttributes(64, 73, 21, 3, 27, 54);
    break;
    }
    gData_02004698 = level;
  }
}

void Func_02002c24(s32 height) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);

  if (!Event_GetFlag(0x109)) {
    object->motionFlags = 0;
    Actor_SetSingleChildFlags(object, 0);
    object->y += height;
    object->speedLimit = 0x10000;
    object->velocityY = 0;
    gMainRoutineWork.renderMode = 2;
    object->actionFlags = 1;
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
    Func_02003b50();
    System_Wait(1);
  }
}

extern char SceneId_Ov663Room1, SceneId_Ov663Room2, SceneId_Ov663Room3;
void Func_02003cb0(const u8 *table);
void Func_02003ca0(const u8 *table);
void Func_02003c80(s32 a);

#define LEADER() Actor_GetById(gMainRoutineWork.currentOwner)
#define BEAM ((s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x178))

/// An object that sways (Func_02001b54) from the given phase.
#define SWAY(id, phase)                                                        \
  (Actor_GetById(id)->action = (phase),                                        \
   Actor_GetById(id)->update = Func_02001b54,                                  \
   Actor_SetSingleChildFlags(Actor_GetById(id), 0))

/// A step that hangs at the given height and sways.
#define HANG(id, height, phase)                                                \
  (Actor_GetById(id)->motionFlags = 0, Actor_GetById(id)->y = (height),        \
   SWAY(id, phase))

s32 Func_02002ca0(void) {
  struct Actor *leader;
  s32 *beam;
  volatile struct DmaRegisters *dma;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  gMainRoutineWork.retreatScene = (s32)&SceneId_Ov663Room1;
  gMainRoutineWork.retreatEntrance = 1;
  LEADER()->priorityFlags |= 32;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov663Room1) {
    switch (gMainRoutineWork.entrance) {
    case 2:
    case 3:
    case 4:
      Func_02002c24(0x580000);
    }
    if (GameFlag_Test(0x850)) {
      MapGrid_CopyCellsTo(0x40, 0, 113, 11, 3, 3);
      MapGrid_CopyCellAttributes(0x40, 0, 3, 2, 113, 12);
      MapGrid_CopyCellAttributes(0x40, 0, 3, 2, 49, 12);
    }
    if (GameFlag_Test(0x851)) {
      MapGrid_CopyCellsTo(0x40, 0, 70, 39, 1, 4);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 70, 40);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 6, 40);
    }
    if (GameFlag_Test(0x852)) {
      MapGrid_CopyCellsTo(0x40, 0, 103, 38, 1, 4);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 103, 39);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 39, 39);
    }
    if (GameFlag_Test(0x853)) {
      MapGrid_CopyCellsTo(0x40, 0, 87, 43, 1, 4);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 87, 44);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 23, 44);
    }
    if (GameFlag_Test(0x854)) {
      MapGrid_CopyCellsTo(0x40, 0, 70, 52, 1, 4);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 70, 52);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 6, 53);
    }
    if (GameFlag_Test(0x855)) {
      MapGrid_CopyCellsTo(0x40, 0, 79, 49, 3, 3);
      MapGrid_CopyCellAttributes(0x40, 0, 3, 2, 79, 50);
      MapGrid_CopyCellAttributes(0x40, 0, 3, 2, 15, 50);
    }
    if (GameFlag_Test(0x856)) {
      MapGrid_CopyCellsTo(0x40, 0, 103, 0x30, 1, 4);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 103, 49);
      MapGrid_CopyCellAttributes(0x40, 0, 1, 3, 39, 49);
    }
    if (GameFlag_Test(0x857)) {
      MapGrid_CopyCellsTo(0x40, 0, 111, 52, 3, 3);
      MapGrid_CopyCellAttributes(0x40, 0, 3, 2, 111, 53);
      MapGrid_CopyCellAttributes(0x40, 0, 3, 2, 47, 53);
    }
    if (GameFlag_Test(0x858)) {
      MapGrid_CopyCellsTo(68, 0, 79, 0, 11, 10);
      MapGrid_CopyCellAttributes(38, 9, 1, 1, 0x10, 9);
      MapGrid_CopyCellAttributes(38, 9, 1, 1, 24, 9);
      Actor_SetPartAttribute(Actor_GetById(8), 6);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov663Room2) {
    switch (gMainRoutineWork.entrance) {
    case 3:
      if (!GameFlag_Test(265))
        LEADER()->z += 0x100000;
      Func_02002c24(0xffb80000);
      goto swaying;
    case 1:
    case 2:
      if (!GameFlag_Test(265))
        LEADER()->z += 0x100000;
      Func_02002c24(0xffd80000);
      goto swaying;
    case 5:
      if (!GameFlag_Test(265)) {
        LEADER()->x += 0x300000;
        LEADER()->y += 0x100000;
      }
    case 6:
      Func_02002c24(0x380000);
    case 4:
    swaying:
      SWAY(8, 60);
      SWAY(9, 180);
      SWAY(10, 0);
      SWAY(11, 60);
      if (GameFlag_Test(0x85f))
        Actor_SetPosition(12, 0x1080000, 0x2b80000);
      if (GameFlag_Test(0x860))
        Actor_SetPosition(13, 0x2d80000, 0x2d80000);
      if (GameFlag_Test(0x861))
        Actor_SetPosition(15, 0x1580000, 0x3080000);
      if (GameFlag_Test(0x862))
        Actor_SetPosition(0x10, 0x2780000, 0x3180000);
      Func_02003cb0(gData_02003d94);
      Func_020003e8(gData_02003d94);
      Func_02003ca0(gData_02003e4c);
      break;
    case 11:
      if (!GameFlag_Test(265)) {
        LEADER()->x += 0x100000;
        LEADER()->y += 0xfff00000;
      }
    case 10:
      if (!GameFlag_Test(265))
        LEADER()->z += 0x100000;
      Func_02002c24(0xffc80000);
    case 12:
      if (GameFlag_Test(0x887))
        Actor_SetPosition(22, 0x3180000, 0x1280000);
      Func_02003cb0(gData_02003d94);
      Func_020003e8(gData_02003d94);
      HANG(17, 0x100000, 60);
      HANG(19, 0x500000, 90);
      HANG(18, 0xffd00000, 100);
      HANG(20, 0x900000, 180);
      Actor_GetById(21)->motionFlags = 0;
      Actor_GetById(21)->y = 0xff700000;
      Actor_SetSpritePriority(21, 2);
      Actor_GetById(21)->priorityFlags |= 2;
      Actor_GetById(21)->action = 180;
      ((u8 *)Actor_GetById(21))[98] = 1;
      Actor_GetById(21)->update = Func_02001b54;
      Actor_SetSingleChildFlags(Actor_GetById(21), 0);
      break;
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov663Room3) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 3:
      Task_AddFrame(Func_02002b88, 0x480);
      Actor_GetById(8)->motionFlags = 0;
      Actor_GetById(8)->y = 0xfff80000;
      Actor_SetSpritePriority(8, 2);
      Actor_GetById(8)->action = 60;
      ((u8 *)Actor_GetById(8))[98] = 0;
      Actor_GetById(8)->update = Func_02001b54;
      Actor_SetSingleChildFlags(Actor_GetById(8), 0);
      Actor_GetById(9)->motionFlags = 0;
      Actor_GetById(9)->y = 0xfff80000;
      Actor_SetSpritePriority(9, 2);
      Actor_GetById(9)->action = 180;
      ((u8 *)Actor_GetById(9))[98] = 0;
      Actor_GetById(9)->update = Func_02001b54;
      Actor_SetSingleChildFlags(Actor_GetById(9), 0);
      Actor_GetById(10)->terrainId = 2;
      Actor_SetSpritePriority(10, 1);
      Actor_GetById(10)->y = 0xd00000;
      Actor_GetById(10)->terrainHeight = Actor_GetById(10)->y;
      Actor_GetById(11)->terrainId = 2;
      Actor_SetSpritePriority(11, 1);
      Actor_GetById(11)->y = 0xd00000;
      Actor_GetById(11)->terrainHeight = Actor_GetById(11)->y;
      if (GameFlag_Test(0x310))
        Actor_SetPosition(10, 0x1f80000, 0x3680000);
      if (GameFlag_Test(785))
        Actor_SetPosition(11, 0x2080000, 0x3680000);
      Func_02000620();
      beam = (s32 *)Ram_HeapSlots->mapWork;
      beam = (s32 *)((u8 *)beam + 0x178);
      beam[2] = 0x5800000;
      beam[3] = 0x2400000;
      beam[4] = 0x4000;
      beam[5] = 0x4000;
      Func_02003b50();
      System_Wait(1);
      break;
    case 6:
      if (!GameFlag_Test(265))
        LEADER()->y += 0x180000;
    case 7:
      Func_02002c24(0x480000);
    case 4:
    case 5:
      Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
      break;
    case 8:
    case 9:
      if (!GameFlag_Test(265))
        LEADER()->z += 0x100000;
      Func_02002c24(0xffe80000);
      goto beam;
    case 10:
      if (!GameFlag_Test(265))
        LEADER()->x += 0x300000;
    case 11:
      Func_02002c24(0x900000);
    beam:
      REG_BG1CNT &= ~3;
      REG_BG1CNT |= 1;
      REG_BG2CNT &= ~3;
      REG_BG2CNT |= 2;
      REG_BG3CNT &= ~3;
      REG_BG3CNT |= 3;
      Actor_GetById(12)->priorityFlags |= 2;
      Func_02003c80(0);
      if (GameFlag_Test(0x879))
        Actor_SetPosition(13, 0xb80000, 0x1180000);
      if (!GameFlag_Test(0x87a)) {
        *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
        REG_BLDALPHA = 0x1008;
        REG_BLDCNT = 0x3f42;
        dma = (volatile struct DmaRegisters *)0x040000b0;
        while ((s32)dma->control < 0)
          ;
        Task_AddFrame(Func_020024b0, 0xc80);
        beam = (s32 *)Ram_HeapSlots->mapWork;
        beam = (s32 *)((u8 *)beam + 0x178);
        beam[6] = 0x4000;
        beam[7] = 0xffff8000;
        {
          s16 *scale = (s16 *)beam;
          s32 value = 3;

          scale[20] = value;
          value = 0x180;
          scale[21] = value;
        }
      } else {
        REG_BLDALPHA = 0x1000;
        REG_BLDCNT = 0x3f42;
        REG_DISPCNT &= ~0x200;
        Actor_SetPartAttribute(Actor_GetById(12), 6);
      }
      break;
    case 12:
      if (!GameFlag_Test(265))
        LEADER()->y += 0xffe00000;
    case 13:
      if (!GameFlag_Test(265))
        LEADER()->z += 0x100000;
      Func_02002c24(0xffd00000);
    case 14:
      beam = (s32 *)Ram_HeapSlots->mapWork;
      beam = (s32 *)((u8 *)beam + 0x178);
      beam[2] = 0x4800000;
      beam[3] = 0x3200000;
      beam[4] = 0xd000;
      beam[5] = 0xd000;
      if (!GameFlag_Test(0x87b)) {
        MapGrid_CopyCellsTo(29, 9, 62, 2, 9, 9);
        MapGrid_CopyCellAttributes(29, 9, 9, 9, 62, 2);
        MapGrid_CopyCellsTo(29, 73, 62, 66, 9, 9);
        MapGrid_CopyCellsTo(52, 2, 29, 9, 9, 9);
        MapGrid_CopyCellAttributes(52, 2, 9, 9, 29, 9);
        MapGrid_CopyCellsTo(52, 66, 29, 73, 9, 9);
      } else {
        Actor_SetPartAttribute(Actor_GetById(14), 6);
      }
      Actor_SetSpritePriority(14, 1);
      Func_02003b50();
      System_Wait(1);
      if (GameFlag_Test(265)) {
        leader = LEADER();
        leader->terrainId = 0;
        leader->y = leader->terrainHeight =
            Map_GetTerrainHeight(0, leader->x, leader->z - leader->y);
        FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 0);
      }
      break;
    case 99:
      MapGrid_CopyCellsTo(29, 9, 62, 2, 9, 9);
      MapGrid_CopyCellAttributes(29, 9, 9, 9, 62, 2);
      MapGrid_CopyCellsTo(29, 73, 62, 66, 9, 9);
      MapGrid_CopyCellsTo(52, 2, 29, 9, 9, 9);
      MapGrid_CopyCellAttributes(52, 2, 9, 9, 29, 9);
      MapGrid_CopyCellsTo(52, 66, 29, 73, 9, 9);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Func_02001f7c();
      break;
    }
  }
  return 0;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 MsgCurePoison2[];

s32 Func_02003a7c(void) {
  if (FIELD_AT(&gMainRoutineWork, s16, 480) == (s32)MsgCurePoison2) {
    Func_02002b88();
  }
  return 0;
}

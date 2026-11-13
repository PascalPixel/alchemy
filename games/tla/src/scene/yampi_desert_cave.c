/// @file
/// The scene code of overlay 0x662, in ROM order; the rest of the overlay is in yampi_desert_cave/.

#include "shared/routines.h"
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "macros.h"
#include "event_inline.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "module/scene/yampi_desert_cave.h"
#include "scene_ids.h"

extern s8 gData_02001974[];

s32 Func_02000038(s32 id) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);
  s16 sprite = ((struct ObjectAnimationState *)object->sprite)->value28[0];
  s16 i;
  u32 kind;
  s32 w, h, x, z;
  s32 az;
  s32 bx, bz;
  s32 toX, toZ, fromX, fromZ;
  s32 layer1X, layer1Z;

  i = 0;
  if ((u16)sprite != gData_02001968[i]) {
  loop:
    /* FAKEMATCH(temporary): kind set to i, then stepped; as kind = (u16)(i +
     * 1), 12 of 160 instructions differ in tla-en. */
    kind = (u16)i;
    kind = (u16)(kind + 1);
    i = (s16)kind;
    if (kind <= 5 && (u16)sprite != gData_02001968[kind])
      goto loop;
  }
  kind = (u16)i;
  if (kind == 6)
    return 0;
  w = (ABS(gData_02001974[kind * 4]) + ABS(gData_02001974[kind * 4 + 2])) >> 4;
  // i is reused: the footprint's second byte here, layers[1]'s x offset in
  // cells below.
  i = gData_02001974[kind * 4 + 1];
  h = (ABS(i) + ABS(gData_02001974[kind * 4 + 3])) >> 4;
  x = (object->x + (gData_02001974[kind * 4] << 16)) >> 20;
  z = (object->z + (i << 16)) >> 20;
  i = *(s32 *)&map->layers[1].unk08[0] >> 20;
  az = *(s32 *)&map->layers[1].unk08[4] >> 20;
  bx = *(s32 *)&map->layers[2].unk08[0];
  bz = *(s32 *)&map->layers[2].unk08[4];
  /* FAKEMATCH(temporary): the first call's last two arguments held in locals;
   * as direct arguments, 36 of 160 instructions differ in tla-en. */
  layer1X = (u16)i + x;
  layer1Z = (u16)az + z;
  Map_CopyCellAttributeRect(x, z, w, h, layer1X, layer1Z);
  /* FAKEMATCH(temporary): the second call's arguments held in locals; as direct
   * arguments, 10 of 160 instructions differ in tla-en. */
  fromX = (u16)(bx >> 20) + x;
  fromZ = (u16)(bz >> 20) + z;
  toX = x + 64;
  toZ = z + 64;
  Map_CopyCellAttributeRect(fromX, fromZ, w, h, toX, toZ);
  Map_FillCellRectByte2(0, x, z, w, h, 255);
  Map_FillCellRectByte2(2, x, z, w, h, 255);
  return 1;
}

#include "../field/map/cellfill.c"

extern s32 gData_0200198c[];

struct Actor *Func_0200033c(s32 *direction, s32 *slot, s32 *kind);

static inline s32 Func_02001850_Call(struct Actor *object, s32 *position) {
  return Func_02001850(object, position);
}

/// Finds how far a pushed object can slide in the leader's direction, probing
/// its footprint cell by cell until Func_02001850 reports a wall. Returns 1
/// with the end position in position[0] to position[2], or 0 if it cannot
/// move.
s32 Func_020001d4(s32 *position) {
  s32 probe[3];
  s32 direction;
  struct Actor *object;
  s32 kind, w, h, dx, dz, count, i, j;
  u8 *state;
  s32 offset;

  object = Func_0200033c(&direction, &position[4], &position[3]);
  if (object != NULL) {
    kind = position[3];
    w = (ABS(gData_02001974[kind * 4]) + ABS(gData_02001974[kind * 4 + 2])) >>
        4;
    h = (ABS(gData_02001974[kind * 4 + 1]) +
         ABS(gData_02001974[kind * 4 + 3])) >>
        4;
    dx = gData_0200198c[direction] & 0xffff0000;
    dz = gData_0200198c[direction] << 16;
    position[5] = 0;
    count = 0;
    state = &object->terrainId;
    *state = 2;
    position[0] = object->x + dx;
    position[2] = object->z + dz;
    probe[1] = object->y;
    // The row loop is built from labels: as a loop it grows by 14 bytes.
  next:
    offset = kind;
    probe[2] = position[2] + (gData_02001974[offset * 4 + 1] << 16);
    offset *= 4;
    j = 0;
    if (j >= h)
      goto advance;
  row:
    probe[0] = position[0] + (gData_02001974[offset] << 16);
    for (i = 0; i < w; i++) {
      /* FAKEMATCH(wrapper): the call through Func_02001850_Call; as
         Func_02001850(object, probe), 2 of 171 instructions differ in tla-en.
       */
      if (Func_02001850_Call(object, probe) == 2)
        goto hit;
      probe[0] += 0x100000;
    }
    probe[2] += 0x100000;
    j++;
    if (j < h)
      goto row;
  advance:
    position[0] += dx;
    count++;
    position[2] += dz;
    goto next;
  hit:
    *state = 0;
    if (count == 0)
      return 0;
    position[0] = object->x + dx * count;
    position[1] = object->y;
    position[2] = object->z + dz * count;
    return 1;
  } else {
    return 0;
  }
}

#include "field/field.h"
#include "text/msg_ids.h"
#include "system/main_routine_work.h"

#include "event_work.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

struct Actor *Func_0200033c(s32 *direction, s32 *slot, s32 *kind) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
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
      gData_02001968[k]) {
    x = (PIXELS(leader->x) + (gData_0200198c[*direction] >> 16)) >> 4;
    z = (PIXELS(leader->z) + (s16)gData_0200198c[*direction]) >> 4;
    left = (PIXELS(object->x) + gData_02001974[k * 4]) >> 4;
    top = (PIXELS(object->z) + gData_02001974[k * 4 + 1]) >> 4;
    right = (PIXELS(object->x) + gData_02001974[k * 4 + 2]) >> 4;
    bottom = (PIXELS(object->z) + gData_02001974[k * 4 + 3]) >> 4;
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

#include "yampi_desert_cave/yampi_desert_cave.h"

void Func_02000458(struct YampiDesertCaveRect rect) {
  // Read as s32: the load then keeps its place after the argument stores.
  struct MapWork *map =
      (struct MapWork *)((struct HeapView *)Ram_HeapSlots)->word;
  s32 w, h;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(rect.word[4]);
  // Always 0: angle is a u16. Direction 0 of the table of steps.
  s32 i = leader->facing >> 16;
  s32 kind = rect.word[3];
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  s32 cell[3];
  s32 from[3];
  s32 j;
  s32 offset;
  s32 ax, az, bx, bz;

  from[0] = object->x;
  from[2] = object->z;
  cell[0] = (object->x + (gData_02001974[kind * 4] << 16)) >> 20;
  cell[2] = (object->z + (gData_02001974[kind * 4 + 1] << 16)) >> 20;
  w = (ABS(gData_02001974[kind * 4]) + ABS(gData_02001974[kind * 4 + 2])) >> 4;
  h = (ABS(gData_02001974[kind * 4 + 1]) + ABS(gData_02001974[kind * 4 + 3])) >> 4;
  Map_FillCellRectByte2(0, cell[0], cell[2], w, h, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x1999);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 8);
  wait(15);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner,
                             (rect.word[0] - from[0]) >> 17,
                             (rect.word[2] - from[2]) >> 17);
  Actor_GetById(gMainRoutineWork.currentOwner)->update = Func_02000754;
  wait(4);
  Func_02001828(object, 2);
  Sound_Play(239);
  Actor_SetSpeedParameters(rect.word[4], 0x8000, 0x1999);
  Actor_SetMoveTarget(object, rect.word[0], rect.word[1], rect.word[2]);
  /* FAKEMATCH(dead): a loop of one pass with an untaken two-test branch, so the
   * second half starts afresh in cse; as no loop, 277 of 342 instructions
   * differ in tla-en. */
  for (j = 0; j < 1; j++) {
    if (j != 0 && j != 5)
      w = 1;
  }
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);
  offset = gData_0200198c[i] << 16;
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner,
                                     offset >> 31, offset >> 17);
  if (rect.word[5] != 0)
    ((void (*)(void))rect.word[5])();
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  leader->update = NULL;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  Script_WaitForEventTimeout(object);
  Sound_Play(298);
  Sound_Play(213);
  Actor_SetPositionAndResetMotion(object, rect.word[0], object->y,
                                  rect.word[2]);
  Func_02001828(object, 1);
  rect.word[0] = (rect.word[0] + (gData_02001974[kind * 4] << 16)) >> 20;
  rect.word[2] = (rect.word[2] + (gData_02001974[kind * 4 + 1] << 16)) >> 20;
  ax = *(s32 *)&map->layers[1].unk08[0] >> 20;
  az = *(s32 *)&map->layers[1].unk08[4] >> 20;
  bx = *(s32 *)&map->layers[2].unk08[0] >> 20;
  bz = *(s32 *)&map->layers[2].unk08[4] >> 20;
  MapGrid_CopyCellAttributes(rect.word[0], rect.word[2], w, h,
                             ax + rect.word[0], az + rect.word[2]);
  MapGrid_CopyCellAttributes(rect.word[0] + bx, rect.word[2] + bz, w, h,
                             rect.word[0] + 64, rect.word[2] + 64);
  Map_FillCellRectByte2(0, rect.word[0], rect.word[2], w, h, 255);
  Map_FillCellRectByte2(2, rect.word[0], rect.word[2], w, h, 255);
  cell[0] = (from[0] + (gData_02001974[kind * 4] << 16)) >> 20;
  cell[2] = (from[2] + (gData_02001974[kind * 4 + 1] << 16)) >> 20;
  MapGrid_CopyCellAttributes(ax + cell[0], az + cell[2], w, h, cell[0],
                             cell[2]);
  MapGrid_CopyCellAttributes(cell[0] + 64, cell[2] + 64, w, h, cell[0] + bx,
                             cell[2] + bz);
  Event_NoOp();
}

void Func_02000754(struct Actor *object) {
  s32 position[3];
  struct Actor *hit;
  u32 i;
  s32 angle = object->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  hit = Func_02001948(position, 1);
  if (hit != 0) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)hit->sprite)->value28 ==
        gData_02001968[i])
      return;
    i++;
    if (i <= 5)
      goto next;
    Actor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
  }
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  if (Func_02001850(object, position) > 0)
    Actor_SetPositionAndResetMotion(object, object->x, object->y, object->z);
}

#include "dma.h"
#include "io_reg.h"

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
DEFINE_FUNC_02000038_4(Func_020007dc, gData_02001fd8, gData_02001fdc,
    gData_02001fe0, gData_02001fcc, Func_02000acc)

#include "task.h"

DEFINE_FUNC_020000F4_2(Func_02000898, gData_02001fe8, gData_02001fe4,
    Func_02000978, gData_02001fd8, gData_02001fd4, 15, gData_02001fd0, 16,
    gData_02001fc8, Func_0200099c)

void Func_020008fc(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  Task_RemoveFar(Func_0200099c);
}

DEFINE_FUNC_020000F4_2(Func_0200091c, gData_02001fe8, gData_02001fe4,
    Func_02000978, gData_02001fd8, gData_02001fd4, 2, gData_02001fd0, 2,
    gData_02001fc8, Func_0200099c)

DEFINE_FUNC_020001D4(Func_02000978)

void Func_0200099c(void) {
  s32 step = 0;
  u16 *color;
  volatile u16 *dst;
  u32 i;
  s32 r, g, b;

  if (gData_02001fd0 != 0 && --gData_02001fd0 == 0) {
    step = 1;
    gData_02001fd0 = gData_02001fd4;
    if (gData_02001fd4 > 2)
      gData_02001fd4--;
  }
  if (!step)
    return;
  color = gData_02001fe0;
  for (i = 0; i < gData_02001fe4; i++) {
    dst = &BG_PLTT[gData_02001fcc[gData_02001fe8 * gData_02001fe4 + i]];
    r = *color++;
    g = *color++;
    b = *color++;
    SET_RGB(*dst, r, g, b);
  }
  dst = &BG_PLTT[gData_02001fd8[gData_02001fe4]];
  if (gData_02001fc8)
    color = gData_02001fdc + gData_02001fe4 * 3;
  else
    color = gData_02001fe0 + gData_02001fe4 * 3;
  r = *color++;
  g = *color++;
  b = *color++;
  SET_RGB(*dst, r, g, b);
  gData_02001fc8 ^= 1;
  if (++gData_02001fe8 == gData_02001fe4)
    gData_02001fe8 = 0;
}

void Func_02000acc(void) {
  struct YampiDesertCaveEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;

  saved = work->value1ac;
  BattleEffect_ApplyColorToTargetBuffer(0x202108, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  ((struct YampiDesertCaveEventWork *)Ram_HeapSlots->eventWork)->value1ac =
      0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_0200091c();
  Func_02001950();
  wait(40);
  Func_020008fc();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
  BattleEffect_StartBufferInterpolation(16);
  wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}

void Func_02000c44(s32 id, u32 kind, s32 flag, s32 layer);

/// Walks a list of three-halfword entries (object id, column, row) ended by
/// 0xffff, clearing each object's collision bit and child flags before passing
/// the entry to Func_02000c44. A struct of three halfwords would be padded to
/// 8 bytes, so the list is read as u16 triples. The twin of overlay 66a's
/// Func_02001230.
void Func_02000b54(const u16 *list) {
  const u16 *entry;

  if (list[0] == 0xffff)
    return;
  entry = list;
loop:
  {
    u32 id = entry[0];
    struct Actor *object = Actor_GetById(id);
    s16 column = entry[1];
    s16 row = entry[2];

    object->collisionFlags &= ~1;
    Actor_SetSingleChildFlags(object, 0);
    Func_02000c44(id, (u16)column, (u16)row, object->terrainId);
    entry += 3;
  }
  if (entry[0] != 0xffff)
    goto loop;
}

// Finds the current target's table row and shrinks it.
DEFINE_FUNC_02000BC0_69(Func_02000bc0, Func_02000c44)

#include "field_event.h"

s32 Func_02001940(struct Actor *object);

DEFINE_FUNC_02000128(Func_02000c44, Func_02001940, Func_02001828)

#include "battle.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

void Func_02000cec(void) {
  GameFlag_SetBit(0x271);
  Func_080d2b4c(17, 17);
  return;
}

void Event_End();

void Func_02000d04(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Func_080d47b4(gMainRoutineWork.currentOwner, 0x101, 32);
  Event_End();
  return;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

u32 *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room2)
    return (u32 *)gData_02001bd0;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3)
    return (u32 *)gData_02001c18;
  return gData_02001b58;
}

void Func_02000d84(void) {
  struct YampiDesertCaveRect rect;

  if (Func_020001d4(&rect))
    Func_02000458(rect);
}

DEFINE_FUNC_02000DAC_2(Func_02000dac, 10)

DEFINE_FUNC_02000DAC_2(Func_02000de0, 9)

DEFINE_FUNC_02000E14(Func_02000e14, 0x300, L_02008e3a, 11)

DEFINE_FUNC_02000E14(Func_02000e3c, 0x301, L_02008e66, 15)

void Func_02000e68(s32 unused, s32 id) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room2) {
    s32 row = 12;

    Map_CopyCellAttributeRect(98, 3, 3, 3, 97, row);
    Map_CopyCellAttributeRect(33, 12, 1, 1, 34, row);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3) {
    if (id == 12) {
      MapGrid_CopyCellAttributes(102, 15, 1, 1, 103, 15);
      MapGrid_CopyCellAttributes(43, 14, 1, 1, 39, 16);
    }
    if (id == 13) {
      struct Actor *object = Actor_GetById(13);

      s32 column = object->x >> 20;
      s32 row = object->z >> 20;

      Map_CopyCellAttributeRect(7, 10, 1, 1, column, row);
    }
  }
}

/// Rooms 2 and 3: by where object id stands, sets flags 0x303 to 0x305 and
/// makes the rooms' Map_CopyCellAttributeRect calls.
void Func_02000f10(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room2) {
    s32 row = object->z >> 20;

    if (row == 13) {
      object->motionFlags = 0;
      GameFlag_SetBit(0x303);
      Map_CopyCellAttributeRect(32, 13, 1, 1, 34, row);
      MapGrid_CopyCellAttributes(95, 3, 3, 3, 97, 12);
    } else {
      row = 12;
      Map_CopyCellAttributeRect(92, 3, 3, 3, 97, row);
      Map_CopyCellAttributeRect(32, 13, 1, 1, 34, row);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3) {
    if (id == 12) {
      s32 column;

      MapGrid_CopyCellAttributes(103, 16, 1, 1, 103, 15);
      column = object->x >> 20;
      if (column == 38) {
        GameFlag_SetBit(0x305);
        Map_CopyCellAttributeRect(38, 15, 1, 1, column, 16);
      } else {
        MapGrid_CopyCellAttributes(38, 15, 1, 1, 39, 16);
      }
    }
    if (id == 13) {
      if (object->x >> 20 == 10 && object->z >> 20 == 11)
        GameFlag_SetBit(0x304);
      MapGrid_CopyCellAttributes(12, 11, 1, 1, object->x >> 20,
                                 object->z >> 20);
    }
  }
}

/// Func_02001120's counterpart: the leader's terrain becomes 2, and in rooms 1
/// and 3 the same calls run with their first and last pairs swapped, each
/// followed by more.
void Func_02001040(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 2;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room1) {
    MapGrid_CopyCellAttributes(55, 71, 4, 1, 43, 70);
    MapGrid_CopyCellAttributes(43, 71, 4, 1, 55, 71);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3) {
    Map_CopyCellAttributeRect(110, 45, 5, 2, 90, 50);
    Map_CopyCellAttributeRect(113, 47, 2, 1, 90, 52);
    Map_CopyCellAttributeRect(107, 48, 3, 2, 90, 53);
    MapGrid_CopyCellAttributes(97, 58, 5, 2, 110, 45);
    MapGrid_CopyCellAttributes(97, 58, 2, 1, 113, 47);
    MapGrid_CopyCellAttributes(97, 58, 3, 2, 107, 48);
  }
}

void Func_02001120(void) {
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 0;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room1)
    MapGrid_CopyCellAttributes(43, 70, 4, 1, 55, 71);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3) {
    MapGrid_CopyCellAttributes(90, 50, 5, 2, 110, 45);
    MapGrid_CopyCellAttributes(90, 52, 2, 1, 113, 47);
    MapGrid_CopyCellAttributes(90, 53, 3, 2, 107, 48);
  }
}

void Func_020011b4(s32 a, s32 b) {
  Func_02000bc0(gData_02001d20, a, b);
  MapGrid_CopyCellAttributes(47, 12, 1, 1, 46, 12);
}

extern const s16 gData_020019cc[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 GameFlag_GetByte_(s32 flag) {
  return ((s32)GameFlag_GetByte(flag));
}

void Func_020011e0(void) {
  s32 step = GameFlag_GetByte_(0x278);
  s32 index = (step / 32) * 2;
  s32 column = gData_020019cc[index];
  s32 row = gData_020019cc[index + 1];

  if (Actor_GetById(16)->movementState == 0) {
    Actor_SetHorizontalPositionWithTerrain(16, column << 19, row << 19);
    if (GameFlag_Test(0x104) == 0)
      step++;
    GameFlag_SetByte(0x278, step);
  }
}

#include "version.h"

/// The chest that follows the step count: when the tracked object stands on
/// its cell, mode 1 stops the follower and mode 2 drops the chest.
void Func_02001238(s32 mode) {
  s32 index = (((s32)GameFlag_GetByte(0x278)) / 32) * 2;
  s16 column = gData_020019cc[index];
  s32 px = column * 8;
  s16 row = gData_020019cc[index + 1];
  s32 pz = row * 8;
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *tracked =
      *(struct Actor **)(Ram_HeapSlots->battleFxWork[0] + 0x1930);
  s32 x = 0, z = 0;

  if (tracked != NULL) {
    x = tracked->x >> 20;
    z = tracked->z >> 20;
  }
  if (px / 16 != x || pz / 16 != z)
    return;
  if (mode == 1) {
    Task_RemoveFar(Func_020011e0);
    Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  }
  if (mode == 2) {
    struct Actor *chest = Actor_GetById(17);

    Actor_SetHorizontalPositionWithTerrain(17, column << 19, row << 19);
    chest->y = 0x200000;
    chest->motionFlags = 3;
    chest->gravity = 0x170a;
    wait(50);
    // Through an s32 and a pointer the 17 is built with a move; as a halfword
    // constant it comes from the pool.
    {
      s16 *counter = (s16 *)(work + 0x160);
      s32 value = 17;

      *counter = value;
    }
#if TLA_PATCH_1
    GameFlag_ClearBit(0x270);
#endif
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/// The guardian speaks, then the party leaves for overlay 664's fourth room.
/// Overlay 6af's Func_02000a8c is the same scene for actor 8.
void Func_0200130c(void) {
  s32 leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(9, 2);
  Event_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  BattleEffect_StartBufferInterpolation(60);
  Event_Wait(60);
  FieldMessage_ShowCentered((s32)MsgIAmThe, 0, 6);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(60);
  Event_Wait(60);
  leader = ((s32)Actor_GetById(FIELD_AT(&gMainRoutineWork, s32, 0x214)));
  if (leader != 0)
    Actor_ResetAndSetPosition(9, FIELD_AT(leader, s16, 10),
                              FIELD_AT(leader, s16, 18));
  Event_Wait(20);
  /* FAKEMATCH(dead): a once-only block around the first two stores keeps the
   * call's first argument ahead of the scene store's address, where without it
   * the argument follows the store; as plain stores, 2 of 71 instructions
   * differ in tla-en. */
  do {
    FIELD_AT(&gMainRoutineWork, u16, 0x254) = 0x19cd;
    FIELD_AT(&gMainRoutineWork, u8, 0x24b) = 2;
  } while (0);
  FIELD_AT(&gMainRoutineWork, u16, 0x1f6) = (s32)&SceneId_Ov664Room4;
  Field_RequestBattleFromEncounterTable(0x67, 1);
}

void Func_020013c8(s32 a) { YampiDesertCave_Call1920(a, 64, 0x302); }

const u8 *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room2)
    return gData_02001ddc;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3)
    return gData_02001e78;
  return gData_02001d28;
}

// Sets up Yampi Desert Cave's three rooms as each one loads: the first
// room's palette cycle and Retreat destination, the third room's background
// priorities, and in each room the objects its flags place.

extern u16 gData_020019ec[];
extern u16 gData_020019fc[];
extern u16 gData_02001a28[];

void Actor_SetWideSprite(s32 id, s32 wide);

/// Sets up Yampi Desert Cave's three rooms as each one loads: the first room's
/// palette cycle and Retreat destination, the third room's background
/// priorities, and in each room the objects its flags place.
s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Func_02001928(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room1) {
    Func_020007dc(gData_020019ec, gData_020019fc, gData_02001a28,
                  gData_02001a54);
    Func_020018c8(8);
    Func_020018c8(9);
    Func_020018c8(10);
    Func_02000038(8);
    Func_02000038(9);
    Func_02000038(10);
    if (GameFlag_Test(0x300)) {
      Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, 13);
    }
    if (gMainRoutineWork.entrance == 1) {
      if (GameFlag_Test(0xa29)) {
        gMainRoutineWork.retreatScene = (s32)&SceneId_Ov661Room1;
        gMainRoutineWork.retreatEntrance = 1;
      } else if (GameFlag_Test(0xa2a)) {
        gMainRoutineWork.retreatScene = (s32)&SceneId_Ov661Room8;
        gMainRoutineWork.retreatEntrance = 2;
      } else if (GameFlag_Test(0xa2b)) {
        gMainRoutineWork.retreatScene = (s32)&SceneId_Ov661Room9;
        gMainRoutineWork.retreatEntrance = 2;
      }
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room2) {
    if (GameFlag_Test(0x303)) {
      Actor_SetPosition(8, 138 << 18, 216 << 16);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, 13);
      MapGrid_CopyCellAttributes(95, 3, 3, 3, 98, 13);
    } else {
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, 12);
    }
    if (!GameFlag_Test(0x302))
      Actor_SetWideSprite(64, 1);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3) {
    if ((u16)(gMainRoutineWork.entrance - 3) <= 2) {
      REG_BG1CNT &= ~3;
      REG_BG1CNT |= 1;
      REG_BG2CNT &= ~3;
      REG_BG2CNT |= 3;
      REG_BG3CNT &= ~3;
      REG_BG3CNT |= 3;
    }
    Func_020018c8(8);
    Func_020018c8(9);
    Func_020018c8(10);
    Func_020018c8(11);
    Func_02000038(8);
    Func_02000038(9);
    Func_02000038(10);
    Func_02000038(11);
    Func_02000b54(gData_02001d20);
    if (!GameFlag_Test(0x206))
      MapGrid_CopyCellAttributes(48, 12, 1, 1, 46, 12);
    if (GameFlag_Test(0x301))
      Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
    if (GameFlag_Test(0x304)) {
      Actor_SetPosition(13, 168 << 16, 184 << 16);
      MapGrid_CopyCellAttributes(12, 11, 1, 1, 10, 11);
    } else {
#if TLA_PATCH_1
      struct Actor *actor = Actor_GetById(13);

      MapGrid_CopyCellAttributes(12, 11, 1, 1, actor->x >> 20, actor->z >> 20);
#else
      MapGrid_CopyCellAttributes(12, 11, 1, 1, 8, 10);
#endif
    }
    if (GameFlag_Test(0x305)) {
      Actor_SetPosition(12, 154 << 18, 132 << 17);
      MapGrid_CopyCellAttributes(38, 15, 1, 1, 38, 16);
    } else {
      MapGrid_CopyCellAttributes(38, 15, 1, 1, 39, 16);
    }
    if (!GameFlag_Test(65) && !GameFlag_Test(145)) {
      s32 step = FieldGameFlag_GetByte(0x278);
      s32 placed = GameFlag_Test(0x271);

      if (placed) {
        step = step / 32 * 2;
        Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
        Actor_SetHorizontalPositionWithTerrain(17, gData_020019cc[step] << 19,
                                               gData_020019cc[step + 1] << 19);
      } else {
        if (GameFlag_Test(0x109))
          GameFlag_SetByte(0x278, 0);
        Task_Add(Func_020011e0, 0x480);
        GameFlag_SetBit(0x270);
        Actor_SetSpritePriority(16, 3);
        Actor_GetById(16)->collisionFlags = 0;
      }
    }
  }
  Func_02001928(0);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

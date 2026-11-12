/// @file
/// The scene code of overlay 0x662 that its scene tables belong to, in ROM order; the rest of the overlay is in yampi_desert_cave/.

#include "field/field.h"
#include "text/msg_ids.h"
#include "system/main_routine_work.h"

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "event_inline.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXELS(v) (((s16 *)&(v))[1])

extern u16 gData_02001968[];
extern s8 gData_02001974[][4];
extern s32 gData_0200198c[];

struct Actor *Func_0200033c(s32 *direction, s32 *slot, s32 *kind) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor **entry;
  struct Actor *object;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = leader->angle >> 12;
  i = 8;
  entry = &work->objects[8];
outer:
  object = *entry;
  k = 0;
inner:
  if (((struct ObjectAnimationState *)object->animation)->value28[0] ==
      gData_02001968[k]) {
    x = (PIXELS(leader->x) + (gData_0200198c[*direction] >> 16)) >> 4;
    z = (PIXELS(leader->z) + (s16)gData_0200198c[*direction]) >> 4;
    left = (PIXELS(object->x) + gData_02001974[k][0]) >> 4;
    top = (PIXELS(object->z) + gData_02001974[k][1]) >> 4;
    right = (PIXELS(object->x) + gData_02001974[k][2]) >> 4;
    bottom = (PIXELS(object->z) + gData_02001974[k][3]) >> 4;
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
#include "map.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

void Func_02000754(struct Actor *object);
void Func_02001828(struct Actor *object, s32 mode);
void Func_02001938(void);
void Map_FillCellRectByte2(s32 layer, s32 x, s32 z, s32 w, s32 h, s32 value);

struct HeapView {
  s32 pad[8];
  s32 word;
};

void Func_02000458(struct YampiDesertCaveRect rect) {
  // Read as s32: the load then keeps its place after the argument stores.
  struct MapWork *map =
      (struct MapWork *)((struct HeapView *)Ram_HeapSlots)->word;
  s32 w, h;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(rect.word[4]);
  // Always 0: angle is a u16. Direction 0 of the table of steps.
  s32 i = leader->angle >> 16;
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
  cell[0] = (object->x + (gData_02001974[kind][0] << 16)) >> 20;
  cell[2] = (object->z + (gData_02001974[kind][1] << 16)) >> 20;
  w = (ABS(gData_02001974[kind][0]) + ABS(gData_02001974[kind][2])) >> 4;
  h = (ABS(gData_02001974[kind][1]) + ABS(gData_02001974[kind][3])) >> 4;
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
  rect.word[0] = (rect.word[0] + (gData_02001974[kind][0] << 16)) >> 20;
  rect.word[2] = (rect.word[2] + (gData_02001974[kind][1] << 16)) >> 20;
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
  cell[0] = (from[0] + (gData_02001974[kind][0] << 16)) >> 20;
  cell[2] = (from[2] + (gData_02001974[kind][1] << 16)) >> 20;
  MapGrid_CopyCellAttributes(ax + cell[0], az + cell[2], w, h, cell[0],
                             cell[2]);
  MapGrid_CopyCellAttributes(cell[0] + 64, cell[2] + 64, w, h, cell[0] + bx,
                             cell[2] + bz);
  Func_02001938();
}

struct Actor *Func_02001948(s32 *position, s32 mode);
s32 Func_02001850(struct Actor *object, s32 *position);

void Func_02000754(struct Actor *object) {
  s32 position[3];
  struct Actor *hit;
  u32 i;
  s32 angle = object->angle >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  hit = Func_02001948(position, 1);
  if (hit != 0) {
    i = 0;
  next:
    if (*((struct ObjectAnimationState *)hit->animation)->value28 ==
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

extern u16 *gData_02001fdc;
extern u16 *gData_02001fe0;
extern u16 *gData_02001fcc;

void Func_02000acc(void);

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
void Func_020007dc(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gData_02001fd8 = slots;
  gData_02001fdc = colours;
  gData_02001fe0 = a;
  gData_02001fcc = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      int value;

      /* FAKEMATCH(dead): the colour's build and store in a once-only block, so
       * both lists are stepped right after their loads and the next entry's
       * marker loads before 0xffff is built; as plain statements, 22 of 80
       * instructions differ in tla-en. */
      do {
        value = (blue << 10) | (green << 5) | red;
        ((u16 *)BG_PLTT)[index] = value;
      } while (0);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  if ((s8)gMainRoutineWork.unk2c2 == 2)
    Func_02000acc();
}

#include "task.h"

void Func_02000898(void) {
  gData_02001fe8 = 0;
  gData_02001fe4 = Func_02000978(gData_02001fd8);
  gData_02001fd4 = 15;
  gData_02001fd0 = 16;
  gData_02001fc8 = 1;
  Task_Add(Func_0200099c, TASK_PASS_VBLANK);
}

void Func_020008fc(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  Task_RemoveFar(Func_0200099c);
}

void Func_0200091c(void) {
  gData_02001fe8 = 0;
  gData_02001fe4 = Func_02000978(gData_02001fd8);
  gData_02001fd4 = 2;
  gData_02001fd0 = 2;
  gData_02001fc8 = 1;
  Task_Add(Func_0200099c, TASK_PASS_VBLANK);
}

s32 Func_02000978(u16 *list) {
  s32 count = 0;

  while (*list != 0xffff) {
    list++;
    count++;
  }
  count--;
  return count;
}

#include "system/io_reg.h"

/// Reads one colour's three channels, red first, stepping p past them. Its
/// once-only block orders the shifts as the game does; see the note at its use.
#define READ_COLOR(r, g, b, p)                                                 \
  do {                                                                         \
    (r) = *(p)++;                                                              \
    (g) = *(p)++;                                                              \
    (b) = *(p)++;                                                              \
  } while (0)

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
  i = 0;
  while (i < gData_02001fe4) {
    dst = &BG_PLTT[gData_02001fcc[gData_02001fe8 * gData_02001fe4 + i]];
    // READ_COLOR's block, at both uses, orders the reads as the game does; as
    // three plain reads, 6 of 120 instructions differ in tla-en.
    READ_COLOR(r, g, b, color);
    *dst = b << 10 | g << 5 | r;
    /* FAKEMATCH(dead): i++ in a once-only block; as a plain i++, 2 of 120
     * instructions differ in tla-en. */
    do {
      i++;
    } while (0);
  }
  dst = &BG_PLTT[gData_02001fd8[gData_02001fe4]];
  if (gData_02001fc8)
    color = gData_02001fdc + gData_02001fe4 * 3;
  else
    color = gData_02001fe0 + gData_02001fe4 * 3;
  READ_COLOR(r, g, b, color);
  *dst = b << 10 | g << 5 | r;
  gData_02001fc8 ^= 1;
  if (++gData_02001fe8 == gData_02001fe4)
    gData_02001fe8 = 0;
}

struct YampiDesertCaveEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

void Func_02001950(void);

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

struct GabombaStatueEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

// Finds the current target's table row and shrinks it.
void Func_02000bc0(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;

  while (*list != 0xffff) {
    u32 entry = *list;
    s32 id = work->objectId;

    if (id == entry) {
      struct Actor *object = Actor_GetById(id);
      s32 a;
      s32 flag;

      list++;
      a = list[0];
      flag = list[1];
      if (count < 8)
        object->scaleY -= 0x1f00;
      if (mode == 1) {
        GameFlag_SetBit(flag);
        Func_02000c44(id, a, flag, object->terrainId);
        object->scaleY = 0x10000;
      }
      break;
    }
    list += 3;
  }
}

#include "field_event.h"

s32 Func_02001940(struct FieldActor *object);

void Func_02000c44(s32 id, u32 kind, s32 flag, s32 layer) {
  struct FieldActor *object = ((struct FieldActor *)Actor_GetById(id));
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;

  cell += Func_02001940(object);
  if (GameFlag_Test(flag)) {
    cell->collisionCode = 0;
    object->priorityFlags |= 2;
    switch (kind) {
    case 0:
      Func_02001828(object, 2);
      break;
    case 1:
      Func_02001828(object, 4);
      break;
    case 2:
      Func_02001828(object, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (kind) {
    case 0:
      Func_02001828(object, 1);
      break;
    case 1:
      Func_02001828(object, 3);
      break;
    case 2:
      Func_02001828(object, 5);
      break;
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

void Func_02000cec(void) {
  GameFlag_SetBit(0x271);
  Func_080d2b4c(17, 17);
  return;
}

void Func_02001870(void);
void Func_02001878();

void Func_02000d04(void) {
  Func_02001870();
  Event_ResolvePendingActions(0);
  Func_080d47b4(gMainRoutineWork.currentOwner, 0x101, 32);
  Func_02001878();
  return;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

u32 *Func_02000d44(void) {
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

void Func_02000dac(void) {
  Func_02000898();
  Func_02001958();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[0x2c2] = 2;
  }
  Func_020018d8((s32)&SceneId_Ov662Room1, 10);
  Event_Wait(40);
  Func_020008fc();
}

void Func_02000de0(void) {
  Func_02000898();
  Func_02001958();
  {
    u8 *party = (u8 *)&gMainRoutineWork;

    party[0x2c2] = 2;
  }
  Func_020018d8((s32)&SceneId_Ov662Room1, 9);
  Event_Wait(40);
  Func_020008fc();
}

void Func_02000e14(s32 arg0) {
  s32 result0;

  result0 = Event_GetFlag(0x300);
  if (result0 != 0)
    goto L_02008e3a;
  if (arg0 != 3)
    goto L_02008e3a;
  Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  GameFlag_SetBit(0x300);
L_02008e3a:
  return;
}

void Func_02000e3c(s32 arg0) {
  s32 result0;

  result0 = Event_GetFlag(0x301);
  if (result0 != 0)
    goto L_02008e66;
  if (arg0 != 3)
    goto L_02008e66;
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  GameFlag_SetBit(0x301);
L_02008e66:
  return;
}

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

void GameFlag_SetByte(s32 flag, s32 value);

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

extern char SceneId_Ov664Room4;

void Func_020018e0(s32 a, s32 b);

/// The guardian speaks, then the party leaves for overlay 664's fourth room.
/// Overlay 6af's Func_02000a8c is the same scene for actor 8.
void Func_0200130c(void) {
  s32 leader;

  Func_02001870();
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
  Func_020018e0(0x67, 1);
}

void Func_020013c8(s32 a) { YampiDesertCave_Call1920(a, 64, 0x302); }

const u8 *Func_020013d8(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room2)
    return gData_02001ddc;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov662Room3)
    return gData_02001e78;
  return gData_02001d28;
}

// Sets up Yampi Desert Cave's three rooms as each one loads: the first
// room's palette cycle and Retreat destination, the third room's background
// priorities, and in each room the objects its flags place.

extern char SceneId_Ov661Room1;
extern char SceneId_Ov661Room8;
extern char SceneId_Ov661Room9;
extern u16 gData_020019ec[];
extern u16 gData_020019fc[];
extern u16 gData_02001a28[];
extern u16 gData_02001a54[];

void Actor_SetWideSprite(s32 id, s32 wide);
s32 Func_02000038(s32 id);
void Func_020018c8(s32 id);
void Func_02001928(s32 value);

/// Sets up Yampi Desert Cave's three rooms as each one loads: the first room's
/// palette cycle and Retreat destination, the third room's background
/// priorities, and in each room the objects its flags place.
s32 Func_02001418(void) {
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
      /* FAKEMATCH(wrapper): the form kept; as a direct call, 2 of 390
       * instructions differ in tla-en. */
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
      /* FAKEMATCH(wrapper): the form kept; as a direct call, 2 of 390
       * instructions differ in tla-en. */
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
      /* FAKEMATCH(wrapper): the form kept; as a direct call, 2 of 390
       * instructions differ in tla-en. */
      Actor_SetPosition(12, 154 << 18, 132 << 17);
      MapGrid_CopyCellAttributes(38, 15, 1, 1, 38, 16);
    } else {
      MapGrid_CopyCellAttributes(38, 15, 1, 1, 39, 16);
    }
    if (!GameFlag_Test(65) && !GameFlag_Test(145)) {
      /* FAKEMATCH(wrapper): the form kept; as ((s32)GameFlag_GetByte(0x278)),
       * 18 of 390 instructions differ in tla-en. */
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

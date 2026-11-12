/// @file
/// Scene code of overlay 0x681, in ROM order, from s4274's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "map.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_0200495c();

void Func_02004274(s32 a0) { Func_0200495c(1, a0); }

extern char SceneId_Ov681Room1;
extern char SceneId_Ov681Room2;
extern char SceneId_Ov681Room5;
extern u8 gData_020023c4[];
extern u32 gData_020053e4[];

u8 *Unit_Get(s32 owner);
void Func_020044f8(s32 level);
void Func_020048a0(void);
s32 Func_02004904(void);
void Func_02004d9c(struct Actor *object, s32 mode);
void Func_02004dc4(void);
void Func_02004de4(u16 *script);
void Func_02000aac(void);
void Func_02000a60(void);

static __inline__ void CopyWords_(void *destination, const void *source,
                                  s32 size) {
  Iwram_CopyWords(destination, source, size);
}

s32 Func_02004280(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room1) {
    u8 *map = (u8 *)Ram_HeapSlots->mapWork;

    if (gMainRoutineWork.entrance <= 4) {
      u8 *unit;
      s32 total;
      s32 *stage;
      s32 i;

      total = 0;
      unit = Unit_Get(gMainRoutineWork.currentOwner);

      for (i = 0; i <= 13; i++)
        total += unit[i];
      Func_020044f8(total);
      Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 32;
      map[261] = 1;
      {
        volatile u16 control = *(volatile u16 *)0x0400000c & ~3;
        // Once-only block: keeps the sky cell window's store after its read.
        do {
        } while (0);

        *(volatile u16 *)0x0400000c = control | 1;
      }
      Func_020048a0();
      stage = (s32 *)(gData_020023c4 + 0x288);
      /* FAKEMATCH(wrapper): the call forwarded through Call3; as a direct
       * Actor_SetHorizontalPositionWithTerrain call, 2 of 229 instructions
       * differ in tla-en. */
      if (*stage == 3 && !GameFlag_Test(0x3f6))
        Actor_SetPosition(64, 0x680000, 0x580000);
      /* FAKEMATCH(wrapper): the call forwarded through Call3; as a direct
       * Actor_SetHorizontalPositionWithTerrain call, 2 of 229 instructions
       * differ in tla-en. */
      if (*stage == 9)
        Actor_SetPosition(65, 0x680000, 0x480000);
      if (GameFlag_Test(*stage + 0xa50)) {
        struct Actor *climber = Actor_GetById(8);
        u32 angle = Func_02004904();

        climber->collisionFlags = 0;
        Func_02004d9c(climber, ((angle & 0xc000) >> 14) + 2);
        climber->update = (angle & 0x4000) != 0 ? Func_02000aac : Func_02000a60;
        climber->collisionFlags = 1;
        GameFlag_SetBit(0x200);
      }
    } else if (gMainRoutineWork.entrance > 8) {
      u8 *layer;

      *(s32 *)(gData_020023c4 + 0x288) = 1;
      layer = map + 376;
      FIELD_AT(layer, u16, 40) = 127;
      FIELD_AT(layer, s32, 24) = 0;
      FIELD_AT(layer, s32, 28) = 0;
      FIELD_AT(layer, s32, 32) = 0;
      FIELD_AT(layer, s32, 36) = 0;
      FIELD_AT(layer, s32, 16) = 0x10000;
      FIELD_AT(layer, s32, 20) = 0x10000;
      FIELD_AT(layer, u16, 42) = 0x3f80;
      // Once-only block: keeps the scale defaults before the position sums.
      do {
      } while (0);
      ((struct MapLayer *)layer)->x =
          Iwram_MulQ16(((struct MapWork *)map)->viewX,
                       FIELD_AT(layer, s32, 16)) +
          FIELD_AT(layer, s32, 8);
      ((struct MapLayer *)layer)->y =
          Iwram_MulQ16(((struct MapWork *)map)->viewZ,
                       FIELD_AT(layer, s32, 20)) +
          FIELD_AT(layer, s32, 12);
      Func_02004dc4();
      System_Wait(1);
      /* FAKEMATCH(wrapper): the copy forwarded through CopyWords_, so the
         blend buffer's address is loaded straight into r0 at each call; as a
         direct Iwram_CopyWords call, 17 of 229 instructions differ in
         tla-en. */
      CopyWords_(Ram_MapCollision + 0x1e00, gData_020053e4, 24);
      Func_02004de4((u16 *)(Ram_MapCollision + 0x1e00));
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room2) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 5:
    case 8:
      Func_02004274(100);
      break;
    case 9:
    case 10:
      Func_02004274(102);
      break;
    case 3:
    case 4:
      Func_02004274(103);
      break;
    case 6:
    case 7:
      Func_02004274(101);
      break;
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov681Room5) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 3:
    case 5:
      Func_02004274(100);
      break;
    case 2:
    case 4:
    case 7:
      Func_02004274(101);
      break;
    case 6:
      Func_02004274(102);
      break;
    }
  }
  return 0;
}

#define GRID_WIDTH 28

// One grid cell: the step that stands on it (0 for none, 99 for the border)
// and the column and row of the next step.
typedef struct {
  s8 unk_0;
  s8 unk_1;
  s8 unk_2;
} Unk_Cell;

typedef struct {
  Unk_Cell cells[GRID_WIDTH * GRID_WIDTH];
} Unk_Grid;

// One step of the path: its number, then the step found below, to the left,
// to the right and above it.
typedef struct {
  u8 unk_0;
  u8 unk_1;
  u8 unk_2;
  u8 unk_3;
  u8 unk_4;
  u8 unk_5;
  u8 unk_6;
  u8 unk_7;
} Unk_Step;

// At gData_020023c4 + 0x288.
typedef struct {
  s32 unk_0;
  Unk_Step steps[15];
} Unk_Path;

#define gPath ((Unk_Path *)(gData_020023c4 + 0x288))

extern u32 gRandomState;
void *Heap_AllocEwram(s32 size);
void Heap_Release(void *buffer);
u32 Random16Far(void);

static __inline__ void *Heap_AllocEwram_(s32 size) {
  /* FAKEMATCH(wrapper): forwarded inline, as scene_call.h describes; as a
   * direct call, 102 of 418 instructions differ in tla-en. */
  return Heap_AllocEwram(size);
}

#define AT(x, y) grid->cells[(y) * GRID_WIDTH + (x)]
#define AT_ADDRESS(x, y)                                                       \
  ((Unk_Cell *)(((y) * GRID_WIDTH + (x)) * 4 + (s32)grid))

void Func_020044f8(s32 seed) {
  Unk_Grid *grid;
  Unk_Path *path;
  Unk_Step *steps;
  Unk_Cell *cell;
  s32 x, y, nx, ny, step, tries, dir, v, next;
  u32 saved;

  saved = gRandomState;
  path = gPath;
  steps = path->steps;
  if (GameFlag_Test(0x109))
    return;
  if (GameFlag_Test(0x3f0))
    return;
  gRandomState = seed;
restart:
  grid = Heap_AllocEwram_(sizeof *grid);
  Iwram_ClearWords(grid, sizeof *grid);
  // BUG: this runs 29 times, so row 28 and column 28 lie past the 28 by 28
  // cells and the last row's marks land after the block.
  {
    Unk_Cell *a, *b, *e;
    s32 o, t;
    x = GRID_WIDTH / 2;
    y = GRID_WIDTH / 2;
    o = 0;
    b = &AT(GRID_WIDTH, 0);
    // step counts the border's rows here, before the walk numbers its steps.
    step = GRID_WIDTH;
    a = grid->cells;
    e = grid->cells;
  border:
    a->unk_0 = 99;
    a += GRID_WIDTH;
    b->unk_0 = 99;
    b += GRID_WIDTH;
    e->unk_0 = 99;
    e++;
    t = o + GRID_WIDTH * GRID_WIDTH * 4;
    ((s8 *)grid)[t] = 99;
    o += sizeof(Unk_Cell);
    step--;
    if (step >= 0)
      goto border;
  }
  for (step = 1; step <= 14;) {
    tries = 0;
  retry:
    if (++tries > 16)
      goto restart;
    nx = x;
    ny = y;
    dir = (Random16Far() * 4) >> 16;
    if (dir == 0 && (step == 1 || step == 13))
      goto retry;
    switch (dir) {
    case 0:
      if (AT(x, y + 1).unk_0 != 0 || AT(x, y + 2).unk_0 != 0 ||
          AT(x - 1, y + 1).unk_0 != 0 || AT(x + 1, y + 1).unk_0 != 0)
        goto retry;
      ny = y + 1;
      break;
    case 1:
      if (AT(x - 1, y).unk_0 != 0 || AT(x - 2, y).unk_0 != 0 ||
          AT(x - 1, y - 1).unk_0 != 0 || AT(x - 1, y + 1).unk_0 != 0)
        goto retry;
      nx = x - 1;
      break;
    case 2:
      if (AT(x + 1, y).unk_0 != 0 || AT(x + 2, y).unk_0 != 0 ||
          AT(x + 1, y - 1).unk_0 != 0 || AT(x + 1, y + 1).unk_0 != 0)
        goto retry;
      nx = x + 1;
      break;
    case 3:
      if (AT(x, y - 1).unk_0 != 0 || AT(x, y - 2).unk_0 != 0 ||
          AT(x - 1, y - 1).unk_0 != 0 || AT(x + 1, y - 1).unk_0 != 0)
        goto retry;
      ny = y - 1;
      break;
    }
    // The same cell variable the second loop reads its steps from.
    cell = AT_ADDRESS(x, y);
    cell->unk_0 = step;
    step++;
    cell->unk_1 = nx;
    cell->unk_2 = ny;
    x = nx;
    y = ny;
  }

  x = GRID_WIDTH / 2;
  y = GRID_WIDTH / 2;
  for (step = 1; step <= 14; step = next) {
    cell = AT_ADDRESS(x, y);
    next = step + 1;

    {
      s32 c;

      v = step;
      c = AT(x, y + 1).unk_0;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        Unk_Cell *p = cell;
      scanUp:
        p -= GRID_WIDTH;
        c = p->unk_0;
        if (c == v - 1) {
          v = c;
          goto scanUp;
        }
      }
      if (v == 0)
        v = 1;
    }
    steps[step].unk_2 = v;

    {
      s32 c;

      v = step;
      c = AT(x, y - 1).unk_0;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        Unk_Cell *p = AT_ADDRESS(x, y);
      scanDown:
        p += GRID_WIDTH;
        c = p->unk_0;
        if (c == v - 1) {
          v = c;
          goto scanDown;
        }
      }
      if (v == 0)
        v = 1;
    }
    steps[step].unk_5 = v;

    {
      s32 c;

      v = step;
      c = AT(x - 1, y).unk_0;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        nx = x;
      scanRight:
        c = AT(++nx, y).unk_0;
        if (c == v - 1) {
          v = c;
          goto scanRight;
        }
      }
      if (v == 0)
        v = 1;
    }
    steps[step].unk_3 = v;

    {
      s32 c;

      v = step;
      c = AT(x + 1, y).unk_0;

      if (c == next)
        v = next;
      else if (c == step - 1)
        v = c;
      else {
        nx = x;
      scanLeft:
        c = AT(--nx, y).unk_0;
        if (c == v - 1) {
          v = c;
          goto scanLeft;
        }
      }
      if (v == 0)
        v = 1;
    }
    steps[step].unk_4 = v;
    x = cell->unk_1;
    steps[step].unk_0 = step;
    y = cell->unk_2;
  }
  steps[14].unk_5 = 251;
  steps[1].unk_2 = 250;
  path->unk_0 = 1;
  Heap_Release(grid);
  Event_SetFlag(0x3f0);
  gRandomState = saved;
}

void Func_0200485c(s32 direction) {
  s32 *state = (s32 *)(gData_020023c4 + 0x288);
  u8 *row = (u8 *)state + *state * 8;
  s32 next = row[((direction - 1) & 3) + 6];

  switch (next) {
  case 250:
  case 251:
    FieldEvent_SetExitRequest(next);
    break;
  case 0:
    next = 1;
  default:
    *state = next;
    FieldEvent_SetExitRequest(direction);
    break;
  }
  Event_PlaySound(123);
}

struct GaiaRockInsideLayer {
  u8 unk00[8];
  s32 x;
  s32 z;
  u8 unk10[0x28];
};

struct GaiaRockInsideMapWork {
  u8 unk000[0x108];
  struct GaiaRockInsideLayer layers[2];
};

extern s8 gData_020053fc[][2];

void Func_02004e0c(s32 x, s32 z, s32 width, s32 height, s32 cellX, s32 cellZ);

/// From the third state on, for each of the two map layers: Func_02004e0c
/// with the state's origin plus the layer's cell, a 15 by 10 size and the
/// layer's cell plus one.
void Func_020048a0(void) {
  s32 state = *(s32 *)(gData_020023c4 + 0x288);
  s8 *origin = gData_020053fc[state];
  struct GaiaRockInsideMapWork *map =
      (struct GaiaRockInsideMapWork *)Ram_HeapSlots->mapWork;

  if (state > 1) {
    s32 originX = origin[0];
    s32 originZ = origin[1];
    struct GaiaRockInsideLayer *layer = map->layers;
    s32 i;

    for (i = 1; i >= 0; i--, layer++) {
      /* FAKEMATCH(pin): x fixed to r2, so x is held in r2 and z in r3; as a
       * plain local, 18 of 45 instructions differ in tla-en. */
      register s32 x asm("r2") = layer->x >> 20;
      s32 z = layer->z >> 20;

      Func_02004e0c(originX + x, originZ + z, 15, 10, x + 1, z + 1);
    }
  }
}

struct GaiaRockInsideNode {
  u8 unk0[2];
  u8 next[4];
  u8 unk6[2];
};

struct GaiaRockInsidePath {
  s32 current;
  struct GaiaRockInsideNode nodes[1];
};

#define GAIA_ROCK_INSIDE_PATH                                                  \
  ((struct GaiaRockInsidePath *)(gData_020023c4 + 0x288))

/// The direction from the current node whose link is 251 or the next node:
/// 0x4000, 0x8000, 0 or 0xc000 for the four links in turn, or -1 if none is.
s32 Func_02004904(void) {
  struct GaiaRockInsidePath *path = GAIA_ROCK_INSIDE_PATH;
  s32 current = path->current;
  /* FAKEMATCH(pin): the node fixed to r3 and each link to r2; as a plain local,
   * 34 of 41 instructions differ in tla-en. */
  register struct GaiaRockInsideNode *node asm("r3") = &path->nodes[current];
  s32 next = current + 1;
  u32 link;

  link = node->next[0];
  if (link == 251 || link == next)
    return 0x4000;
  link = node->next[1];
  if (link == 251 || link == next)
    return 0x8000;
  link = node->next[2];
  if (link == 251 || link == next)
    return 0;
  link = node->next[3];
  if (link == 251 || link == next)
    return 0xc000;
  return -1;
}

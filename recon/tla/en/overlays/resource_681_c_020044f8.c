// Near miss, differ 437 of 418 (tla-en, 2026-10-31): the shape is the native's;
// what is left is allocation and three compiler habits. In the walk the native
// keeps the grid, the next column and the step in r5-r7, tries in r9 and x in
// r10, and saves the next row (r1) on the stack around Random16Far; here r5-r7
// go to the grid and the next column and row, tries is the one saved around
// the call (r4) and the step sits in r10. The native rebuilds 99 and 0xc40
// inside the border loop and stores the bottom row through grid + (0xc40 + 4i)
// from its own 4i counter; this compiler hoists both constants and walks that
// row with a pointer. The native computes y << 3 once before the direction
// dispatch for all four arms (the store after it computes it again); this
// compiler computes it in each arm and on the default edges. The rest is
// scheduling.
//
// Lays out the maze path once (flag 0x3f0, not after flag 0x109): seeded by
// the given value, a random walk of fourteen steps from the middle of a 28 by
// 28 grid never touches its own path, retrying a step up to sixteen times and
// the whole walk after that (with a fresh grid; the old one is not released);
// then for each step it records which step lies beyond each of its four sides,
// and marks the ends. The four scans are label loops, as the native's are:
// written as C loops, loop.c rotates them.
#include "types.h"
#include "iwram_call.h"

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

extern u32 gRandomState;
extern u8 gData_020023c4[];
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void *Heap_AllocEwram(s32 size);
void Heap_Release(void *buffer);
u32 Random16Far(void);

static __inline__ void *Heap_AllocEwram_(s32 size) {
  /* FAKEMATCH(wrapper): forwarded inline, as scene_call.h describes; as a
   * direct call, 475 of 418 instructions differ in tla-en. */
  return Heap_AllocEwram(size);
}

static __inline__ void ClearWords_(void *destination, s32 size) {
  /* FAKEMATCH(wrapper): forwarded inline, as scene_call.h describes; as a
   * direct call, 468 of 418 instructions differ in tla-en. */
  Iwram_ClearWords(destination, size);
}

#define AT(x, y) grid->cells[(y) * GRID_WIDTH + (x)]

void Func_020044f8(s32 seed) {
  Unk_Grid *grid;
  Unk_Path *path;
  Unk_Step *steps;
  Unk_Cell *cell;
  Unk_Cell *p;
  s32 x, y, nx, ny, step, n, tries, dir, i, v, next, k;
  u32 saved;

  saved = gRandomState;
  path = (Unk_Path *)(gData_020023c4 + 0x288);
  steps = path->steps;
  if (GameFlag_Test(0x109))
    return;
  if (GameFlag_Test(0x3f0))
    return;
  gRandomState = seed;
restart:
  grid = Heap_AllocEwram_(sizeof *grid);
  ClearWords_(grid, sizeof *grid);
  x = GRID_WIDTH / 2;
  y = GRID_WIDTH / 2;
  // BUG: i runs to 28, so row 28 and column 28 lie past the 28 by 28 cells and
  // the last row's marks land after the block.
  for (i = 0; i <= GRID_WIDTH; i++) {
    AT(0, i).unk_0 = 99;
    AT(GRID_WIDTH, i).unk_0 = 99;
    AT(i, 0).unk_0 = 99;
    AT(i, GRID_WIDTH).unk_0 = 99;
  }
  for (step = 1; step <= 14; step++) {
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
    cell = &AT(x, y);
    cell->unk_0 = step;
    cell->unk_1 = nx;
    cell->unk_2 = ny;
    x = nx;
    y = ny;
  }

  x = GRID_WIDTH / 2;
  y = GRID_WIDTH / 2;
  for (n = 1; n <= 14; n = next) {
    cell = &AT(x, y);
    next = n + 1;

    v = n;
    if (AT(x, y + 1).unk_0 == next)
      v = next;
    else if (AT(x, y + 1).unk_0 == n - 1)
      v = AT(x, y + 1).unk_0;
    else {
      p = cell;
    scanUp:
      p -= GRID_WIDTH;
      if (p->unk_0 == v - 1) {
        v = p->unk_0;
        goto scanUp;
      }
    }
    if (v == 0)
      v = 1;
    steps[n].unk_2 = v;

    v = n;
    if (AT(x, y - 1).unk_0 == next)
      v = next;
    else if (AT(x, y - 1).unk_0 == n - 1)
      v = AT(x, y - 1).unk_0;
    else {
      p = &AT(x, y);
    scanDown:
      p += GRID_WIDTH;
      if (p->unk_0 == v - 1) {
        v = p->unk_0;
        goto scanDown;
      }
    }
    if (v == 0)
      v = 1;
    steps[n].unk_5 = v;

    v = n;
    if (AT(x - 1, y).unk_0 == next)
      v = next;
    else if (AT(x - 1, y).unk_0 == n - 1)
      v = AT(x - 1, y).unk_0;
    else {
      k = x;
    scanRight:
      k++;
      if (AT(k, y).unk_0 == v - 1) {
        v = AT(k, y).unk_0;
        goto scanRight;
      }
    }
    if (v == 0)
      v = 1;
    steps[n].unk_3 = v;

    v = n;
    if (AT(x + 1, y).unk_0 == next)
      v = next;
    else if (AT(x + 1, y).unk_0 == n - 1)
      v = AT(x + 1, y).unk_0;
    else {
      k = x;
    scanLeft:
      k--;
      if (AT(k, y).unk_0 == v - 1) {
        v = AT(k, y).unk_0;
        goto scanLeft;
      }
    }
    if (v == 0)
      v = 1;
    steps[n].unk_4 = v;
    steps[n].unk_0 = n;
    x = cell->unk_1;
    y = cell->unk_2;
  }
  steps[14].unk_5 = 251;
  steps[1].unk_2 = 250;
  path->unk_0 = 1;
  Heap_Release(grid);
  GameFlag_SetBit(0x3f0);
  gRandomState = saved;
}

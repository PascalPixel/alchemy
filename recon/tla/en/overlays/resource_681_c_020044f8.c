// Near miss, differ 44 of 418 (tla-en, 2026-11-06): register names only.
// The native has the cell in ip, the row's y << 3 in lr and the step's offset in r9;
// here the two offsets outrank the cell (5 refs over 75 and 86
// against 4 over 111), take ip and lr, and the cell takes r9. That also moves the reload
// registers' rotation in the third and fourth scans. Pinning the cell to ip gives the
// names but loses one reload, which moves the rotation (38). Sharing the cell with the
// walk's store gives it 8 refs over 116, which takes r7 before the step.
// path is one local holding a constant: its doubled live length is what puts steps
// behind tries, and at the end it reloads from the pool.
// Also measured (2026-11-06): a second counter for the second loop, 129 (the walk's step
// leaves r7); the cell shared with the walk's store, 244 (it takes r7 in the walk);
// the first scan read through cell[GRID_WIDTH], 67; the last two stores swapped, 66.
//
// Lays out the maze path once (flag 0x3f0, not after flag 0x109): seeded by
// the given value, a random walk of fourteen steps from the middle of a 28 by
// 28 grid never touches its own path, retrying a step up to sixteen times and
// the whole walk after that (with a fresh grid; the old one is not released);
// then for each step it records which step lies beyond each of its four sides,
// and marks the ends.
#include "types.h"
#include "call.h"
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

#define gPath ((Unk_Path *)(gData_020023c4 + 0x288))

extern u32 gRandomState;
extern u8 gData_020023c4[];
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void *Heap_AllocEwram(s32 size);
void Heap_Release(void *buffer);
u32 Random16Far(void);

static __inline__ void *Heap_AllocEwram_(s32 size) {
  /* FAKEMATCH(wrapper): forwarded inline, as scene_call.h describes; as a
   * direct call, 422 of 418 instructions differ in tla-en. */
  return Heap_AllocEwram(size);
}

#define AT(x, y) grid->cells[(y) * GRID_WIDTH + (x)]
#define AT_ADDRESS(x, y) ((Unk_Cell *)(((y) * GRID_WIDTH + (x)) * 4 + (s32)grid))

void Func_020044f8(s32 seed) {
  Unk_Grid *grid;
  Unk_Path *path;
  Unk_Step *steps;
  Unk_Cell *cell;
  s32 x, y, nx, ny, step, tries, dir, i, v, next;
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
  // BUG: this runs 29 times, so row 28 and column 28 lie past the 28 by 28 cells
  // and the last row's marks land after the block.
  {
    Unk_Cell *a, *b, *e;
    s32 o, t;
    x = GRID_WIDTH / 2;
    y = GRID_WIDTH / 2;
    b = &AT(GRID_WIDTH, 0);
    i = GRID_WIDTH;
    o = 0;
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
    i--;
    if (i >= 0)
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
    {
      Unk_Cell *here = AT_ADDRESS(x, y);
      here->unk_0 = step;
      step++;
      here->unk_1 = nx;
      here->unk_2 = ny;
    }
    x = nx;
    y = ny;
  }

  x = GRID_WIDTH / 2;
  y = GRID_WIDTH / 2;
  for (step = 1; step <= 14; step = next) {
    cell = AT_ADDRESS(x, y);
    next = step + 1;

    {
    s32 c = AT(x, y + 1).unk_0;

    v = step;
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
    s32 c = AT(x, y - 1).unk_0;

    v = step;
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
    s32 c = AT(x - 1, y).unk_0;

    v = step;
    if (c == next)
      v = next;
    else if (c == step - 1)
      v = c;
    else {
      nx = x;
    scanRight:
      nx++;
      c = AT(nx, y).unk_0;
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
    s32 c = AT(x + 1, y).unk_0;

    v = step;
    if (c == next)
      v = next;
    else if (c == step - 1)
      v = c;
    else {
      nx = x;
    scanLeft:
      nx--;
      c = AT(nx, y).unk_0;
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
  GameFlag_SetBit(0x3f0);
  gRandomState = saved;
}

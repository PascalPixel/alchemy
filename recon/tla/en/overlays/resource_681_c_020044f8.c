// Draft, differ 350 of 418, skeleton gap 54 (tla-en, 2026-11-01): x, y, nx, the
// border's registers and the walk's n, next and cell now agree with the native.
// What is left is two things, step and tries, and where y << 3 is computed. The
// Heap_AllocEwram wrapper is the one device kept: without it the draft falls to
// 422.
//
// Allocation: the native's order is grid (r5), nx (r6), step (r7), y (r8), x
// (r10), tries (r9), steps (r11), with ny last in r1 by caller-save. Giving the
// walk its own column (wx) fixed x (388 to 360), because the step phase's x then
// has its own live length; splitting the row too lost (382), and step sharing with
// n, the border counter, or next is a local reused for a second value and also
// lost or was rejected for that (390, 401; next gains 365 only by accident).
// Setting i before o in the border gave the offset r6 (360 to 350). Still wrong:
// ny (33 refs over 68 insns, density 2.4) and tries (66 over 172, 2.3) outrank
// step (55 over 220, 1.25), so step takes r9, ny r7 and tries r4. tries in r9
// means the native found caller-save unprofitable: 4 * calls >= refs, so tries had
// at most 4 refs, its raw count with no loop weight, where this compiler weights
// it to 66 by the retry label's many back edges. Spellings that do not change
// this: ny++ for ny = y + 1, nx after the call (417, frame shrinks), ny before nx,
// dir block-local, declaration orders, the prologue's order. A label step loop
// lost (379).
//
// y << 3: the native computes it once after the dir check, shared by all four
// arms, and again at the join; the walk shares one across the first three sides
// and the third scan, and computes a second for the fourth side and its scan.
// Here PRE shares one reaching register: on the default edges and in each arm,
// deleting the join's, and in r6 through the walk. This is most of the skeleton
// gap; the rest is high-register reloads for step and tries, and prologue order.
// LCM cannot leave both a dispatch copy and the join's, so this is no PRE
// placement: the pattern reads as hoisting from the dominated blocks whose paths
// meet no other computation, which leaves the last occurrence alone. An unread
// k = y * 8 before the switch is a dead store and no answer. If no source form
// exists, both this and tries are compiler questions, out of scope for a draft.
//
// Border: a label loop, as 99 and 0xc40 are rebuilt in its body, with the
// pointers and offset stepped by hand and the sum in its own local (a for loop
// hoists both constants, and folding the sum into the address adds a register).
//
// The arms: ny = y - 1 needs no special form: in the last arm the native's
// reload scratch for y happens to be r1, ny's register, so the copy goes. A real
// loop for retry hoists the row arithmetic (431): retry stays a label.
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
   * direct call, 422 of 418 instructions differ in tla-en. */
  return Heap_AllocEwram(size);
}

#define AT(x, y) grid->cells[(y) * GRID_WIDTH + (x)]

void Func_020044f8(s32 seed) {
  Unk_Grid *grid;
  Unk_Path *path;
  Unk_Step *steps;
  Unk_Cell *cell;
  Unk_Cell *p;
  s32 x, y, wx, nx, ny, step, n, tries, dir, i, v, next, k, c;
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
  Iwram_ClearWords(grid, sizeof *grid);
  x = GRID_WIDTH / 2;
  y = GRID_WIDTH / 2;
  // BUG: this runs 29 times, so row 28 and column 28 lie past the 28 by 28 cells
  // and the last row's marks land after the block.
  {
    Unk_Cell *a, *b, *e;
    s32 o, t;
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

  wx = GRID_WIDTH / 2;
  y = GRID_WIDTH / 2;
  for (n = 1; n <= 14; n = next) {
    cell = &AT(wx, y);
    next = n + 1;

    c = AT(wx, y + 1).unk_0;
    v = n;
    if (c == next)
      v = next;
    else if (c == n - 1)
      v = c;
    else {
      p = cell;
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
    steps[n].unk_2 = v;

    c = AT(wx, y - 1).unk_0;
    v = n;
    if (c == next)
      v = next;
    else if (c == n - 1)
      v = c;
    else {
      p = &AT(wx, y);
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
    steps[n].unk_5 = v;

    c = AT(wx - 1, y).unk_0;
    v = n;
    if (c == next)
      v = next;
    else if (c == n - 1)
      v = c;
    else {
      k = wx;
    scanRight:
      k++;
      c = AT(k, y).unk_0;
      if (c == v - 1) {
        v = c;
        goto scanRight;
      }
    }
    if (v == 0)
      v = 1;
    steps[n].unk_3 = v;

    c = AT(wx + 1, y).unk_0;
    v = n;
    if (c == next)
      v = next;
    else if (c == n - 1)
      v = c;
    else {
      k = wx;
    scanLeft:
      k--;
      c = AT(k, y).unk_0;
      if (c == v - 1) {
        v = c;
        goto scanLeft;
      }
    }
    if (v == 0)
      v = 1;
    steps[n].unk_4 = v;
    steps[n].unk_0 = n;
    wx = cell->unk_1;
    y = cell->unk_2;
  }
  path = (Unk_Path *)(gData_020023c4 + 0x288);
  steps[14].unk_5 = 251;
  steps[1].unk_2 = 250;
  path->unk_0 = 1;
  Heap_Release(grid);
  GameFlag_SetBit(0x3f0);
  gRandomState = saved;
}

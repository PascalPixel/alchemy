// Draft, differ 620 of 418: lays out the maze path once (flag 0x3f0, not after
// flag 0x109): seeded by the given value, a random walk of fourteen steps from
// the middle of a 28 by 28 grid never touches its own path, retrying a step up
// to sixteen times and the whole walk after that; then for each step it
// records which step lies beyond each of its four sides, and marks the ends.
#include "types.h"
#include "iwram_call.h"

struct MazeCell {
  s8 step; ///< 0 when free, 99 on the border
  s8 nextX;
  s8 nextZ;
  u8 unk3;
};

struct MazeStep {
  u8 step;
  u8 unk1;
  u8 south; ///< the step reached through +z
  u8 west;  ///< the step reached through -x
  u8 east;  ///< the step reached through +x
  u8 north; ///< the step reached through -z
  u8 unk6[2];
};

extern u32 gRandomState;
extern u8 gData_020023c4[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void *Runtime_BumpAllocateAlternatePool(s32 size);
u32 Random16Far(void);
void MemFree(void *buffer);

void Func_020044f8(s32 seed) {
  u32 saved = gRandomState;
  struct MazeStep *steps = (struct MazeStep *)(gData_020023c4 + 0x288 + 4);
  struct MazeCell(*grid)[28];
  s32 x;
  s32 z;
  s32 step;
  s32 tries;
  s32 i;

  if (GameFlag_Test(0x109) || GameFlag_Test(0x3f0))
    return;
  gRandomState = seed;
restart:
  grid = Runtime_BumpAllocateAlternatePool(0xc40);
  Iwram_ClearWords(grid, 0xc40);
  x = 14;
  z = 14;
  for (i = 0; i <= 28; i++) {
    grid[i][0].step = 99;
    grid[i][28].step = 99;
    grid[0][i].step = 99;
    grid[28][i].step = 99;
  }
  step = 1;
nextStep:
  tries = 0;
retry:
  {
    s32 nextX;
    s32 nextZ;
    u32 direction;

    if (++tries > 16)
      goto restart;
    nextZ = z;
    direction = (Random16Far() << 2) >> 16;
    nextX = x;
    if (direction == 0 && (step == 1 || step == 13))
      goto retry;
    switch (direction) {
    case 0:
      if (grid[z + 1][x].step != 0 || grid[z + 2][x].step != 0 ||
          grid[z + 1][x - 1].step != 0 || grid[z + 1][x + 1].step != 0)
        goto retry;
      nextZ = z + 1;
      break;
    case 1:
      if (grid[z][x - 1].step != 0 || grid[z][x - 2].step != 0 ||
          grid[z - 1][x - 1].step != 0 || grid[z + 1][x - 1].step != 0)
        goto retry;
      nextX = x - 1;
      break;
    case 2:
      if (grid[z][x + 1].step != 0 || grid[z][x + 2].step != 0 ||
          grid[z - 1][x + 1].step != 0 || grid[z + 1][x + 1].step != 0)
        goto retry;
      nextX = x + 1;
      break;
    case 3:
      if (grid[z - 1][x].step != 0 || grid[z - 2][x].step != 0 ||
          grid[z - 1][x - 1].step != 0 || grid[z - 1][x + 1].step != 0)
        goto retry;
      nextZ = z - 1;
      break;
    }
    grid[z][x].step = step++;
    grid[z][x].nextX = nextX;
    grid[z][x].nextZ = nextZ;
    x = nextX;
    z = nextZ;
    if (step <= 14)
      goto nextStep;
  }
  x = 14;
  z = 14;
  step = 1;
link:
  {
    struct MazeCell *cell = &grid[z][x];
    s32 next = step + 1;
    s32 value;
    struct MazeCell *p;
    s32 k;

    value = step;
    if (grid[z + 1][x].step == next) {
      value = next;
    } else if (grid[z + 1][x].step == step - 1) {
      value = grid[z + 1][x].step;
    } else {
      p = cell;
    walkNorth:
      p -= 28;
      if (p->step == value - 1) {
        value = p->step;
        goto walkNorth;
      }
    }
    if (value == 0)
      value = 1;
    steps[step].south = value;

    value = step;
    if (grid[z - 1][x].step == next) {
      value = next;
    } else if (grid[z - 1][x].step == step - 1) {
      value = grid[z - 1][x].step;
    } else {
      p = &grid[z][x];
    walkSouth:
      p += 28;
      if (p->step == value - 1) {
        value = p->step;
        goto walkSouth;
      }
    }
    if (value == 0)
      value = 1;
    steps[step].north = value;

    value = step;
    if (grid[z][x - 1].step == next) {
      value = next;
    } else if (grid[z][x - 1].step == step - 1) {
      value = grid[z][x - 1].step;
    } else {
      k = x;
    walkEast:
      k++;
      if (grid[z][k].step == value - 1) {
        value = grid[z][k].step;
        goto walkEast;
      }
    }
    if (value == 0)
      value = 1;
    steps[step].west = value;

    value = step;
    if (grid[z][x + 1].step == next) {
      value = next;
    } else if (grid[z][x + 1].step == step - 1) {
      value = grid[z][x + 1].step;
    } else {
      k = x;
    walkWest:
      k--;
      if (grid[z][k].step == value - 1) {
        value = grid[z][k].step;
        goto walkWest;
      }
    }
    if (value == 0)
      value = 1;
    steps[step].east = value;

    steps[step].step = step;
    step = next;
    x = cell->nextX;
    z = cell->nextZ;
    if (step <= 14)
      goto link;
  }
  steps[14].east = 251;
  steps[1].south = 250;
  *(s32 *)(gData_020023c4 + 0x288) = 1;
  MemFree(grid);
  GameFlag_SetBit(0x3f0);
  gRandomState = saved;
}

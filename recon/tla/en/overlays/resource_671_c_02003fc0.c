// Draft, differ 421 of 278: lays a random path of `length` cells across the
// Gabomba Statue grid (gSceneState, 11 by 8 cells of four bytes) from
// (x, z) to (endX, endZ), backtracking through per-step stacks (the
// Data_02006fc8 to gData_02007068 arrays: direction, x, z, kind, tried
// directions and flags) when a cell has no free neighbour; each step joins
// the cells (Func_02004224 picks the piece, Func_02004270 writes it).
// Returns 0, or -1 when the parity of the distance rules a path out or the
// search gives up. Left: a rough reading of the search: the native keeps the position in r8 and r9, the step in r5, the counters and targets on the stack and reloads each stack array's address at every use; the alignment is poor.
#include "types.h"
#include "iwram_call.h"

extern u8 gSceneState[];
extern u8 Data_02006fc8[], Data_02006fe8[], Data_02007008[], Data_02007028[],
    Data_02007048[], gData_02007068[];
extern s8 gData_02005ac0[], gData_02005ac8[];
u16 Random16Far(void);
s32 Func_02004224(s32 from, s32 to, s32 kind);
void Func_02004270(s32 x, s32 z, s32 piece, s32 flags);

#define GRID_CELL(x, z) (gSceneState + ((z) * 11 + (x)) * 4)

s32 Func_02003fc0(u32 x, u32 z, s32 endX, s32 endZ, s32 length,
                  s32 direction, s32 last, s32 bit) {
  s32 spare, step, tries, attempts, piece, dir;
  u32 nextX, nextZ;
  u8 tried;

  length--;
  spare = endX - x;
  if (spare < 0)
    spare = -spare;
  spare = length - spare;
  dir = endZ - z;
  if (dir < 0)
    dir = -dir;
  spare -= dir;
  Iwram_ClearWords(Data_02006fc8, 32);
  Iwram_ClearWords(Data_02006fe8, 32);
  Iwram_ClearWords(Data_02007008, 32);
  Iwram_ClearWords(Data_02007028, 32);
  Iwram_ClearWords(Data_02007048, 32);
  Iwram_ClearWords(gData_02007068, 32);
  if (spare & 1)
    return -1;
  step = 0;
  attempts = 0;
next:
  if (++attempts > 1280)
    return -1;
  Data_02007028[step] = GRID_CELL(x, z)[0];
  gData_02007068[step] = GRID_CELL(x, z)[2];
  Data_02006fe8[step] = x;
  Data_02007008[step] = z;
  Data_02006fc8[step] = direction;
  Data_02007048[step] |= (gData_02005ac8 + 1)[Data_02007028[step]] |
                         1 << ((direction + 2) & 3);
  tries = 0;
pick:
  if (++tries > 63)
    goto back;
  dir = direction;
  if (Random16Far() > 0x7fff)
    dir = (u32)(Random16Far() << 2) >> 16;
  tried = Data_02007048[step];
  if ((tried >> dir) & 1)
    goto pick;
  nextX = x + (gData_02005ac0 + 1)[dir * 2];
  nextZ = z + (gData_02005ac0 + 1)[dir * 2 + 1];
  if (nextX > 10 || nextZ > 7)
    goto blocked;
  if (step == length - 1 && (nextX != endX || nextZ != endZ))
    goto blocked;
  if (GRID_CELL(nextX, nextZ)[1] != 0 ||
      (GRID_CELL(nextX, nextZ)[2] >> bit) & 1 ||
      GRID_CELL(nextX, nextZ)[0] == 10)
    goto blocked;
  Data_02007048[step] = tried | 1 << dir;
  piece = Func_02004224(direction, dir, Data_02007028[step]);
  Func_02004270(x, z, piece, GRID_CELL(x, z)[2] | 1 << bit);
  step++;
  x = nextX;
  z = nextZ;
  direction = dir;
  if (step == length) {
    piece = Func_02004224(dir, last, GRID_CELL(nextX, nextZ)[0]);
    Func_02004270(nextX, nextZ, piece, GRID_CELL(nextX, nextZ)[2] | 1 << bit);
    return 0;
  }
  Data_02007048[step] = 0;
  goto next;
blocked:
  Data_02007048[step] |= 1 << dir;
  if (Data_02007048[step] != 0x0f)
    goto pick;
back:
  Data_02007048[step] = 0;
  if (step == 0)
    return -1;
  step--;
  x = Data_02006fe8[step];
  z = Data_02007008[step];
  direction = Data_02006fc8[step];
  Func_02004270(x, z, Data_02007028[step], gData_02007068[step]);
  goto next;
}

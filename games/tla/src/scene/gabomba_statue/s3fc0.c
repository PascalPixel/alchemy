#include "types.h"
#include "iwram_call.h"

struct PuzzleCell {
  u8 piece;
  u8 unk1;
  u8 shape;
  u8 marked;
};

extern struct PuzzleCell gSceneState[];
extern u8 Data_02006fc8[], Data_02006fe8[], Data_02007008[], Data_02007028[],
    Data_02007048[], gData_02007068[];
extern s8 gData_02005ac0[];
extern u8 gData_02005ac8[];
s32 abs(s32);
u32 Random16Far(void);
s32 Func_02004224(s32 from, s32 to, s32 kind);
void Func_02004270(s32 x, s32 z, s32 piece, s32 flags);
// Lays a random path of `length` cells across the Gabomba Statue grid (gSceneState,
// 11 by 8 cells of four bytes) from (x0, z0) to (endX, endZ), backtracking through
// per-step stacks (Data_02006fc8 to gData_02007068: direction, x, z, kind, tried
// directions and flags) when a cell has no free neighbour; each step joins the
// cells (Func_02004224 picks the piece, Func_02004270 writes it). Returns 0, or -1
// when the parity of the distance rules a path out or the search gives up.
s32 Func_02003fc0(s32 x0, s32 z0, s32 endX, s32 endZ, s32 length, s32 direction,
                  s32 last, s32 bit) {
  u8 *cells, *kinds;
  s32 piece;
  s32 spare, step, dir, used, dz, index, one;
  u32 x, z, nextX, nextZ, tries, attempts;
  u8 tried;

  length--;
  dz = endZ - z0;
  spare = length - abs(endX - x0) - abs(dz);
  Iwram_ClearWords(Data_02006fc8, 32);
  Iwram_ClearWords(Data_02006fe8, 32);
  Iwram_ClearWords(Data_02007008, 32);
  Iwram_ClearWords(Data_02007028, 32);
  Iwram_ClearWords(Data_02007048, 32);
  Iwram_ClearWords(gData_02007068, 32);
  if (spare & 1)
    return -1;
  x = x0;
  cells = (u8 *)gSceneState;
  one = 1;
  step = 0;
  attempts = 0;
  z = z0;
  while (1) {
    if (++attempts > 1280)
      return -1;
    {
      s32 offset = (z * 11 + x) * 4;
      Data_02007028[step] = cells[offset];
      gData_02007068[step] = ((struct PuzzleCell *)(cells + offset))->shape;
    }
    Data_02006fe8[step] = x;
    Data_02007008[step] = z;
    Data_02006fc8[step] = direction;
    {
      u8 *joins = gData_02005ac8 + 1;
      kinds = Data_02007028;
      Data_02007048[step] = Data_02007048[step] | joins[kinds[step]] |
                            1 << ((direction + 2) & 3);
    }
    tries = 0;
    while (1) {
      u32 value;
      if (++tries > 63)
        goto back;
      value = Random16Far();
      dir = direction;
      if (value > 0x7fff)
        dir = (Random16Far() * 4) >> 16;
      tried = Data_02007048[step];
      if ((tried >> dir) & one)
        continue;
      {
        s8 *steps = gData_02005ac0 + 1;
        index = dir * 2;
        nextX = x + steps[index];
        index += 1;
        nextZ = z + steps[index];
      }
      if (nextX > 10 || nextZ > 7)
        goto blocked;
      if (step == length - 1 && (nextX != endX || nextZ != endZ))
        goto blocked;
      {
        u8 *cell = cells + (nextZ * 11 + nextX) * 4;
        if (cell[1] != 0)
          goto blocked;
        used = (cell[2] >> bit) & one;
        if (used != 0)
          goto blocked;
        if (cell[0] == 10)
          goto blocked;
      }
      tried |= one << dir;
      Data_02007048[step] = tried;
      kinds = Data_02007028;
      piece = Func_02004224(direction, dir, kinds[step]);
      {
        s32 offset = (z * 11 + x) * 4;
        Func_02004270(x, z, piece, (cells + offset)[2] | one << bit);
      }
      step++;
      x = nextX;
      z = nextZ;
      direction = dir;
      if (step == length)
        goto done;
      Data_02007048[step] = 0;
      goto top;
    blocked:
      Data_02007048[step] |= 1 << dir;
      if (Data_02007048[step] != 0x0f)
        continue;
    back:
    Data_02007048[step] = 0;
    if (step == 0)
      return -1;
    step--;
    x = Data_02006fe8[step];
    z = Data_02007008[step];
    direction = Data_02006fc8[step];
    Func_02004270(x, z, Data_02007028[step], gData_02007068[step]);
    goto top;
    }
    top:;
  }
done:
  {
    s32 offset = (nextZ * 11 + nextX) * 4;
    piece = Func_02004224(dir, last, cells[offset]);
    Func_02004270(nextX, nextZ, piece, (cells + offset)[2] | 1 << bit);
  }
  return 0;
}

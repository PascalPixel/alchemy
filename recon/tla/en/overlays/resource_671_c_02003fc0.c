// Draft, differ 239 of 278, skeleton gap 35 (tla-en, 2026-11-01)
// The control flow, calls, arguments and memory accesses are the native's;
// what is left is allocation, and the skeleton gap is its spills and moves.
// Forms that closed distance: `one`, a set-once local, keeps 1 out of the
// registers, so reload rebuilds it at each use as the native does (the plain
// constant is one CSE pseudo that outranks x and z). The blocked and done sites
// keep a literal 1, or PRE shares 1 << bit. z is a loop copy of the parameter,
// as x is: that shortens its life enough to rank with x, and z0 falls into the
// native's r8 by itself. `piece` shared by both calls goes to global-alloc,
// which frees r2 for 1 << bit in local-alloc, as in the native. xIndex and
// zIndex give the native's index-then-add-1 with one table address.
// Left: the native gives step, dir, used, z, nextZ, x and nextX r5, r6, r7, r8,
// r10, r9 and r11, piece r4, and keeps attempts and tries at sp+0 and sp+4.
// Here used and dir are swapped, nextZ takes r4 (saved around calls at sp+0),
// which sends piece to ip, nextX r8, tries r10, z r9, x r11. Priorities here:
// nextZ 1.19, nextX 1.18, tries 0.69, z 0.68, x 0.67, piece 0.57. The native's
// order must be piece, z, nextZ, x, nextX, tries. Open: what keeps nextZ, x and
// z out of r4. A block-local piece takes r2, because a copy suggestion is
// allocated first. nextZ refs 20 against 3 calls makes r4 profitable.
// Tried: tries = 0 before the Data_02007048 store loses tries to the stack as
// the native does (live 128; gap 34, differ 277, the 11 pseudo then takes r11).
// A struct of two words for x, z and next is a DImode pseudo, which Thumb
// cannot keep in high registers (334). A `width = 11` local lets PRE share
// z * width (329). delta = table + 1 merges table + index (265). Reading into
// stepX and stepZ before the adds (248, 256 with zIndex after the read).
// else dir = direction in place of the default before the call (262).
// Pointer-sum grid access (267). Earlier: the pick
// search as a do-while measures the same as these gotos; the search as a loop,
// with done after it, lets loop.c move the blocks ending in a jump out.
//
// Lays a random path of `length` cells across the
// Gabomba Statue grid (gSceneState, 11 by 8 cells of four bytes) from
// (x, z) to (endX, endZ), backtracking through per-step stacks (the
// Data_02006fc8 to gData_02007068 arrays: direction, x, z, kind, tried
// directions and flags) when a cell has no free neighbour; each step joins
// the cells (Func_02004224 picks the piece, Func_02004270 writes it).
// Returns 0, or -1 when the parity of the distance rules a path out or the
// search gives up.
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
s32 Func_02003fc0(s32 x0, s32 z0, s32 endX, s32 endZ, s32 length, s32 direction,
                  s32 last, s32 bit) {
  s32 spare, step, dir, used, dz, one, z, xIndex, zIndex, piece;
  u32 x, nextX, nextZ, tries, attempts;
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
  if (spare & 1) {
  fail:
    return -1;
  }
  one = 1;
  step = 0;
  attempts = 0;
  x = x0;
  z = z0;
  while (1) {
    if (++attempts > 1280)
      goto fail;
    Data_02007028[step] = gSceneState[z * 11 + x].piece;
    gData_02007068[step] = gSceneState[z * 11 + x].shape;
    Data_02006fe8[step] = x;
    Data_02007008[step] = z;
    Data_02006fc8[step] = direction;
    Data_02007048[step] = Data_02007048[step] |
                          (gData_02005ac8 + 1)[Data_02007028[step]] |
                          1 << ((direction + 2) & 3);
    tries = 0;
  pick:
      if (++tries > 63)
        goto back;
      dir = direction;
      if (Random16Far() > 0x7fff)
        dir = (Random16Far() * 4) >> 16;
      tried = Data_02007048[step];
      if ((tried >> dir) & one)
        goto pick;
      xIndex = dir * 2;
      zIndex = xIndex + 1;
      nextX = x + (gData_02005ac0 + 1)[xIndex];
      nextZ = z + (gData_02005ac0 + 1)[zIndex];
      if (nextX > 10 || nextZ > 7)
        goto blocked;
      if (step == length - 1 && (nextX != endX || nextZ != endZ))
        goto blocked;
      if (gSceneState[nextZ * 11 + nextX].unk1 != 0)
        goto blocked;
      used = (gSceneState[nextZ * 11 + nextX].shape >> bit) & one;
      if (used != 0)
        goto blocked;
      if (gSceneState[nextZ * 11 + nextX].piece == 10)
        goto blocked;
      Data_02007048[step] = tried | one << dir;
      piece = Func_02004224(direction, dir, Data_02007028[step]);
      Func_02004270(x, z, piece, gSceneState[z * 11 + x].shape | one << bit);
      step++;
      x = nextX;
      z = nextZ;
      direction = dir;
      if (step == length)
        goto done;
      Data_02007048[step] = 0;
      continue;
    blocked:
      Data_02007048[step] |= 1 << dir;
      if (Data_02007048[step] != 0x0f)
        goto pick;
  back:
    Data_02007048[step] = 0;
    if (step == 0)
      goto fail;
    x = Data_02006fe8[step - 1];
    z = Data_02007008[step - 1];
    direction = Data_02006fc8[step - 1];
    Func_02004270(x, z, Data_02007028[step - 1], gData_02007068[step - 1]);
    step--;
  }
done:
  piece = Func_02004224(dir, last, gSceneState[nextZ * 11 + nextX].piece);
  Func_02004270(nextX, nextZ, piece, gSceneState[nextZ * 11 + nextX].shape | 1 << bit);
  return 0;
}

// Draft, differ 305 of 278, skeleton gap 77 (tla-en, 2026-11-01)
// The control flow, calls, arguments and memory accesses are the native's;
// what is left is allocation, and the skeleton gap is its spills and moves.
// Step reaches the native's r5 and the cell, 1 and 1 << bit are recomputed
// at the last step. Those two come from the outer while (1): its loop-end
// note stops CSE1 following the jump to `done`, and `step - 1` in the backing
// up with step-- after the call keeps step from being a biv (step-- before
// the reads gives loop.c its pointer givs again, differ 383).
// Left: the native gives step, dir, used, z, nextZ, x and nextX r5, r6, r7,
// r8, r10, r9 and r11 and keeps tries on the stack (sp+4). Here used takes
// r6, dir r7, nextZ r4 saved around calls, the constant 1 r10, tries r9, and
// z goes to the stack, which adds 8 bytes of frame. The native has the call's
// result in r4 between the two calls, which shuts nextZ out of r4; here it
// goes straight to r2, because local-alloc honours its copy to the argument
// register first. What would keep r2 busy there is not known.
// Tried, as leads: a local `one = 1` set once before the search takes the
// constant out of the registers (differ 274, gap 76, x then reaches r9), and
// z then loses only to the 11 pseudo CSE carries over the first call, whose
// local forms (283 to 343) make things worse. A local index for dir * 2 with
// index++ reaches differ 289, gap 75, but one local for two values is a
// `temporary`. `dz` is a plain distance: the native takes endZ - z before the
// first abs. z copied from a parameter (332); the pick search as a do-while
// measures the same as these gotos; the search itself as a loop, with done
// after it, lets loop.c move the blocks ending in a jump out (360 to 383).
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
s32 Func_02003fc0(s32 x0, s32 z, s32 endX, s32 endZ, s32 length, s32 direction,
                  s32 last, s32 bit) {
  s32 spare, step, dir, used, dz;
  u32 x, nextX, nextZ, tries, attempts;
  u8 tried;

  length--;
  dz = endZ - z;
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
  step = 0;
  attempts = 0;
  x = x0;
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
      if ((tried >> dir) & 1)
        goto pick;
      nextX = x + (gData_02005ac0 + 1)[dir * 2];
      nextZ = z + (gData_02005ac0 + 1)[dir * 2 + 1];
      if (nextX > 10 || nextZ > 7)
        goto blocked;
      if (step == length - 1 && (nextX != endX || nextZ != endZ))
        goto blocked;
      if (gSceneState[nextZ * 11 + nextX].unk1 != 0)
        goto blocked;
      used = (gSceneState[nextZ * 11 + nextX].shape >> bit) & 1;
      if (used != 0)
        goto blocked;
      if (gSceneState[nextZ * 11 + nextX].piece == 10)
        goto blocked;
      Data_02007048[step] = tried | 1 << dir;
      Func_02004270(x, z, Func_02004224(direction, dir, Data_02007028[step]),
                    gSceneState[z * 11 + x].shape | 1 << bit);
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
  Func_02004270(nextX, nextZ,
                Func_02004224(dir, last, gSceneState[nextZ * 11 + nextX].piece),
                gSceneState[nextZ * 11 + nextX].shape | 1 << bit);
  return 0;
}

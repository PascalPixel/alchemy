// Near miss, differ 342 of 278 (tla-en, 2026-10-31): the control flow,
// calls, arguments and memory accesses are the native's; what is left is
// allocation. global.c ranks `used` (the shape bit, which CSE reuses as the
// 0 stored after the calls) and the cell pointer above step, so step takes
// r4 saved around every call (sp+0) where the native has r5, used takes r5
// (native r7), x goes to the stack and z, nextX and nextZ take r11, r10 and
// r8 (native: x r9, z r8, nextX r11, nextZ r10); tries takes fp where the
// native keeps it at sp+4. CSE1 follows the one-use jump to the last step
// (`done`, after a barrier) and carries the cell pointer, the constant 1 and
// 1 << bit across the calls into it, where the native recomputes all three
// from nextX and nextZ.
// Tried: the outer search as for (;;) or do-while (loop.c turns
// Data_02007048[step] and Data_02007028[step] into pointer givs, 421 to
// 426); only the direction draw as the do-while (373); x used without a copy
// (337 with z copied, but the prologue then swaps r7 and r8; 368 with x
// copied and z not); one variable for spare and dir, x0 reused for used (377
// to 388); gSceneState as a struct member or pointer sum (342, 366); tried
// as u32 or s32, signed counters (342 to 344). The pick search as a do-while
// that ends at the flags test is what keeps the native's cmp with 0x7fff and
// bls: as a goto loop combine rewrites it as cmp with 0x8000 and bcc.
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
  s32 spare, step, piece, dir, used;
  u32 x, z, nextX, nextZ, attempts, tries;
  u8 tried;

  length--;
  spare = length - abs(endX - x0) - abs(endZ - z0);
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
  z = z0;
next:
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
  do {
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
    piece = Func_02004224(direction, dir, Data_02007028[step]);
    Func_02004270(x, z, piece, gSceneState[z * 11 + x].shape | 1 << bit);
    step++;
    x = nextX;
    z = nextZ;
    direction = dir;
    if (step == length)
      goto done;
    Data_02007048[step] = 0;
    goto next;
  blocked:
    Data_02007048[step] |= 1 << dir;
  } while (Data_02007048[step] != 0x0f);
back:
  Data_02007048[step] = 0;
  if (step == 0)
    goto fail;
  step--;
  x = Data_02006fe8[step];
  z = Data_02007008[step];
  direction = Data_02006fc8[step];
  Func_02004270(x, z, Data_02007028[step], gData_02007068[step]);
  goto next;
done:
  piece = Func_02004224(dir, last, gSceneState[nextZ * 11 + nextX].piece);
  Func_02004270(nextX, nextZ, piece,
                gSceneState[nextZ * 11 + nextX].shape | 1 << bit);
  return 0;
}

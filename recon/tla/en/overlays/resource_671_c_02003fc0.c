// Draft, differ 229 of 278, skeleton gap 24 (tla-en, 2026-11-01)
// The control flow, calls, arguments and memory accesses are the native's;
// what is left is allocation, and most of the skeleton gap is its spills and
// moves. Forms that closed distance: the outer loop is a label and goto, with
// a real loop around the search through to the back-step, so nothing hoists
// the outer loop's table addresses and the ClearWords calls each load their
// own: x0 and z0 fall into the native's r7 and r8. Func_02004224 nested in
// Func_02004270's arguments (a block-local temporary, no `piece` variable).
// `one`, a set-once local, keeps 1 out of the registers, so reload rebuilds it
// at each use as the native does; without it 326 here. The done site keeps a
// literal 1. xIndex and zIndex give the native's index-then-add-1 with one
// table address.
// Left: the native gives step, dir, used, z, nextZ, x and nextX r5, r6, r7, r8,
// r10, r9 and r11, the piece r4, attempts sp+0 and tries sp+4. Here used and
// dir are swapped, nextZ takes r4 (saved around calls at sp+0, 4 * 3 calls <
// 20 refs), nextX r8, tries r10, z r9, x r11. Three things to find:
// 1. The piece. The native has no save of r4, so r4 must be taken before nextZ,
// z and x reach their caller-save retry. A block-local temporary in r4 would do
// it, because local-alloc's r4 conflicts with all of them, but it takes r2 by
// its copy suggestion. Global, it ranks 0.57 against nextZ's 1.19 and lands in
// ip once r4 has gone (ip is the alternate class; r4 is the first free LO_REGS).
// 2. dir and used. The tail's gSceneState (block-local, crosses a call) takes
// r6 in local-alloc, which denies dir r6. The native reloads it at both uses,
// so there it had no block-local pseudo. Ending the search loop before the
// success path carries it from there (54 insns, global), but that also gives
// the Data_02007048 and table pseudos registers (352).
// 3. The order. z, nextZ, x, nextX for r8, r10, r9, r11 needs nextZ and nextX
// between z (0.68) and x (0.67) or either side of x: 15 refs on 67 and 68 insns
// would do it exactly (0.672, 0.662), and here they have 20. tries (0.69) must
// also fall below x.
// Tried: tries = 0 before the Data_02007048 statement loses tries to the stack
// as the native does (gap 21, differ 266), but its zero moves up against the
// native's. The same tree with `while (1)` outside keeps the tables in r7 and
// r8 across the calls (cse2 turns the hoisted load into a copy of the ClearWords
// pseudo). Goto at both levels has no loop end before done, and the gSceneState
// of the search reaches done in a register (353). z as the parameter itself
// (239, gap 31). Func_02004224 returning u8 (292). Other tried and neutral: a
// block-scoped piece; u32 parameters; x and z in place of nextX and nextZ at
// done; dir[2] and dir[2] + 1 indexes (274); reading both steps before the adds
// (279); else dir = direction (275). 1 << bit at both sites (261).
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
  s32 spare, step, dir, used, dz, one, z, xIndex, zIndex;
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
top:
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
    while (1) {
      if (++tries > 63)
        goto back;
      dir = direction;
      if (Random16Far() > 0x7fff)
        dir = (Random16Far() * 4) >> 16;
      tried = Data_02007048[step];
      if ((tried >> dir) & one)
        continue;
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
      Func_02004270(x, z, Func_02004224(direction, dir, Data_02007028[step]),
                    gSceneState[z * 11 + x].shape | one << bit);
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
      goto fail;
    x = Data_02006fe8[step - 1];
    z = Data_02007008[step - 1];
    direction = Data_02006fc8[step - 1];
    Func_02004270(x, z, Data_02007028[step - 1], gData_02007068[step - 1]);
    step--;
    goto top;
    }
done:
  Func_02004270(nextX, nextZ,
                Func_02004224(dir, last,
                              gSceneState[nextZ * 11 + nextX].piece),
                gSceneState[nextZ * 11 + nextX].shape | 1 << bit);
  return 0;
}

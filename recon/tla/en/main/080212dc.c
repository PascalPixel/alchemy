// Draft, differ 16 of 95 (tla-en, 2026-11-08)
// The ARM routine behind the Thumb entry Func_080212d8 (bx pc), an ARM copy
// of the perspective map's scanline table builder (☀️'s
// WorldMap_BuildScanlineTable, ⚓️'s Thumb Func_0802dd70). Plain C now, no
// devices: the rest of the routine is instruction for instruction GCC
// 2.95.2's output of this C.
// Left, and out of reach of C: 16 rows in two hand edits of that output.
// (1) IwramSqrt and IwramTransformVector are called by bl to two ldr pc,
// [pc, #-4] veneers placed after the literal pool in the middle of the
// routine (listing offsets 0x154 and 0x15c), so a copy in RAM still reaches
// IWRAM; GCC emits neither, and they can't come from another object or from
// asm() (S1). They and the branch offsets they move are 8 rows. Their
// target words differ by edition: tla-ja's second veneer holds 0x030002d4
// (IwramSqrt) like the first, so its bl for the transform reaches IwramSqrt;
// the other five editions hold 0x03000354 (IwramTransformVector). The
// routine's instructions are the same in all six. (2) Two
// MulQ16 calls (scale times distance, horizontal squared) are the two smull
// that GAS 2.10 flags in this draft's output ("rdhi, rdlo and rm must all be
// different": smull r2, r5, r5, r6 and smull r2, r3, r3, r3); the listing
// gives each a free RdHi (r1, r0) and shifts it back into the draft's
// register (lsl r5, r1 and lsl r3, r0), 8 rows. MulQ16's high word and its
// shifted value are one pseudo, so no caller can split them; a helper with
// a separate shifted temporary was measured too and local-alloc ties it to
// the dying high word (still 16). The other four smull, which GAS accepts,
// match as they are. A question for Pascal: whether this routine is proven
// assembly under S1 (far-call veneers inside the function, reached by bl).
// What settled it (2026-11-08): IwramRatioMulQ14 called through a local that
// holds it for its whole life, with no asm: GCC keeps the call in a register
// and loads the pointer where the listing does (the empty asms gave 30, the
// fixed address 0x03000230 from iwram_call.h is built by mov and add and
// hoisted, 66). Also settled: the frame (focus, horizon, spilled depth, then
// the 32-byte areas at sp+12 and sp+44; horizon declared before focus gives
// their slots), half in a local (as a literal, 32), line++ between the two
// last stores (at the loop's end, 18), and Data_030011f0 as its own symbol
// (as gCameraSceneParameters[4], CSE shares the base, 26). The zeros stored
// need no local; a for loop gives 22.
#include "types.h"
#include "macros.h"
#include "mul_q16.h"

extern s32 gCameraSceneParameters;
extern s32 Data_030011f0;

s32 IwramRatioMulQ14(s32 divisor, s32 value);
void IwramTransformVectorVeneer(const void *vector, s32 *result);
s32 IwramSqrtVeneer(s32 value);

struct ScanVertex {
  s32 x;
  s32 z;
  s32 dx;
  s32 dz;
  s16 u;
  s16 v;
};

void Func_080212dc(s32 depth, s32 *position, struct ScanVertex *scanline) {
  s32 groundPoint[8];
  s32 cameraPoint[8];
  s32 horizon;
  s32 focus;
  s32 line;
  s32 half;
  s32 distance;
  s32 scale;
  // Called through this local: the routine reaches IwramRatioMulQ14 by
  // register, not by bl.
  s32 (*ratio)(s32, s32) = IwramRatioMulQ14;

  groundPoint[1] = 0;
  groundPoint[0] = position[0];
  groundPoint[2] = position[2];
  IwramTransformVectorVeneer(groundPoint, cameraPoint);
  horizon = cameraPoint[1] - MulQ16(cameraPoint[2], depth);
  line = 0;
  half = 0x8000;
  focus = -gCameraSceneParameters;
  while (line < 160) {
    distance = ratio(focus, (Data_030011f0 - line) << 16);
    scale = ratio(distance != depth ? distance - depth : 1, horizon);
    if (scale < 0) {
      s32 horizontal;
      s32 vertical;
      s32 result;
      s32 width;

      width = MulQ16(-scale, half);
      scanline->x = ratio(gCameraSceneParameters, width);
      vertical = MulQ16(scale, distance);
      horizontal = (cameraPoint[2] - scale) >> 4;
      vertical = (vertical - cameraPoint[1]) >> 4;
      result = IwramSqrtVeneer(MulQ16(horizontal, horizontal) +
                               MulQ16(vertical, vertical))
               << 12;
      if (vertical < 0)
        result = -result;
      scanline->z = MulQ16(result, half);
    } else {
      scanline->x = 0;
      scanline->z = 0;
    }
    scanline->dx = 0;
    line++;
    scanline->dz = 0;
    scanline++;
  }
}

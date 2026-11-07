// Blocked, 5 November 2026: the ARM routine behind the Thumb veneer
// Func_080212d8 (bx pc), an ARM copy of the perspective map's scanline table
// builder (☀️'s WorldMap_BuildScanlineTable, ⚓️'s Thumb Func_0802dd70).
// Differ 30 of 95 by --arm (2026-11-07). What stops it is not the shape:
// IwramSqrt and IwramTransformVector are called by bl to two ldr pc, [pc, #-4]
// veneers that sit after the literal pool in the middle of the routine
// (listing offsets 0x154 and 0x15c), with a branch over them. GCC 2.95.2 emits
// neither, and they can't come from another object or from asm() (S1). A
// question for Pascal; they and the branch offsets they move are about 6 rows.
// Settled: the frame (focus, horizon, spilled depth, then the 32-byte areas at
// sp+12 and sp+44; horizon declared before focus gives their slots), line in
// r8, 0x8000 in r9 and zero in r10 (zero copied from line, the first 0 its own
// constant), and the loop tail's order (line++ between the two stores). The
// call through a register for IwramRatioMulQ14 is kept by empty asms here (no
// tag: a draft); the first argument of its third call is computed before the
// pointer is loaded. Left: the listing loads that pointer into r3 after the
// line's operands for the first call and into r2 for the other two, where one
// variable gives r3 throughout (10 rows); and two MulQ16 calls (scale times
// distance, horizontal squared) put the high word in a fresh register (4 rows).
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
  s32 zero;
  s32 distance;
  s32 scale;
  s32 vertical;
  s32 result;
  s32 (*ratio)(s32, s32);

  groundPoint[1] = 0;
  groundPoint[0] = position[0];
  groundPoint[2] = position[2];
  IwramTransformVectorVeneer(groundPoint, cameraPoint);
  horizon = cameraPoint[1] - MulQ16(cameraPoint[2], depth);
  line = 0;
  half = 0x8000;
  focus = -gCameraSceneParameters;
  zero = line;
  while (line <= 159) {
    ratio = IwramRatioMulQ14;
    __asm__("" : "=r"(ratio) : "0"(ratio));
    distance = ratio(focus, (Data_030011f0 - line) << 16);
    ratio = IwramRatioMulQ14;
    __asm__("" : "=r"(ratio) : "0"(ratio));
    scale = ratio(distance != depth ? distance - depth : 1, horizon);
    if (scale < 0) {
      s32 horizontal;

      vertical = MulQ16(-scale, half);
      ratio = IwramRatioMulQ14;
      __asm__("" : "=r"(ratio) : "0"(ratio));
      scanline->x = ratio(gCameraSceneParameters, vertical);
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
      scanline->x = zero;
      scanline->z = zero;
    }
    scanline->dx = zero;
    line++;
    scanline->dz = zero;
    scanline++;
  }
}

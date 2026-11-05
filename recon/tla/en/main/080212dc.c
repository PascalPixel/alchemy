// Blocked, 5 November 2026: the ARM routine behind the Thumb veneer
// Func_080212d8 (bx pc), an ARM copy of the perspective map's scanline table
// builder (☀️'s WorldMap_BuildScanlineTable, ⚓️'s Thumb Func_0802dd70). Differ
// 97 of 95 by --arm. What stops it is not the shape: IwramSqrt and
// IwramTransformVector are called by bl to two ldr pc, [pc, #-4] veneers that sit
// after the literal pool in the middle of the routine (listing offsets 0x154 and
// 0x15c), with a branch over them. GCC 2.95.2 emits neither, and they can't come
// from another object or from asm() (S1). A question for Pascal. What the draft
// has settled: two 32-byte areas in the frame, which the listing has at sp+12 and
// sp+44, and a call through a register for IwramRatioMulQ14, kept by empty asms
// here (no tag: a draft). Left: the frame's third word is the spilled depth, and
// the listing keeps zero in r10 and 0x8000 in r9.
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
  s32 focus;
  s32 horizon;
  s32 line;
  s32 half;
  s32 zero;
  s32 distance;
  s32 scale;
  s32 vertical;
  s32 result;
  s32 (*ratio)(s32, s32);

  zero = 0;
  groundPoint[1] = zero;
  groundPoint[0] = position[0];
  groundPoint[2] = position[2];
  IwramTransformVectorVeneer(groundPoint, cameraPoint);
  horizon = cameraPoint[1] - MulQ16(cameraPoint[2], depth);
  line = zero;
  half = 0x8000;
  focus = -gCameraSceneParameters;
  zero = line;
  for (; line <= 159; line++) {
    ratio = IwramRatioMulQ14;
    __asm__("" : "=r"(ratio) : "0"(ratio));
    distance = ratio(focus, (Data_030011f0 - line) << 16);
    ratio = IwramRatioMulQ14;
    __asm__("" : "=r"(ratio) : "0"(ratio));
    scale = ratio(distance != depth ? distance - depth : 1, horizon);
    if (scale < 0) {
      s32 horizontal;

      ratio = IwramRatioMulQ14;
      __asm__("" : "=r"(ratio) : "0"(ratio));
      scanline->x = ratio(gCameraSceneParameters, MulQ16(-scale, half));
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
    scanline->dz = zero;
    scanline++;
  }
}

// Draft, ARM C (game ARM route, GCC 2.95.2): the ARM routine behind the Thumb
// veneer Func_080212d8 (bx pc), an ARM copy of the perspective map's scanline
// table builder (☀️'s WorldMap_BuildScanlineTable, ⚓️'s Thumb Func_0802dd70).
// Stopped by its two calls that aren't C: IwramSqrt and IwramTransformVector
// are called by bl to ldr pc, [pc, #-4] veneers that sit in the function's
// literal pool, in the middle of the loop's then-arm (listing offsets 0x154
// and 0x15c). GCC 2.95.2 places only constants in a pool, and an assembly file
// can't put a veneer in another object's mid-function pool. The rest: the
// native has a 76-byte frame where the draft has 36, keeps 0x8000 in r9, and
// calls IwramRatioMulQ14 through a register (ldr, mov lr, pc, bx) where
// GCC 2.95.2 emits bl (long_call and #pragma long_calls have no effect, and a
// cast symbol folds to bl). Veneer names are this draft's own.
#include "types.h"
#include "macros.h"
#include "mul_q16.h"

struct RuntimeUnusedEntry {
  u32 stub;
  u32 target;
};

struct RuntimeMathBinaryEntry {
  u32 stub;
  s32 (*target)(s32 x, s32 y);
};

struct RuntimeMathUnaryEntry {
  u32 stub;
  s32 (*target)(s32 angle);
};

struct RuntimeMathTable {
  struct RuntimeUnusedEntry preceding[8];
  struct RuntimeMathBinaryEntry atan2;
  struct RuntimeUnusedEntry squareRoots[2];
  struct RuntimeMathUnaryEntry sin;
  struct RuntimeMathUnaryEntry cos;
};

struct Position {
  s32 x;
  s32 y;
  s32 z;
};

struct ScanVertex {
  s32 x;
  s32 z;
  s32 dx;
  s32 dz;
  s16 u;
  s16 v;
};

struct BgAffine {
  s16 pa;
  s16 pb;
  s16 pc;
  s16 pd;
  s32 x;
  s32 y;
};

extern const struct RuntimeMathTable waitFar;
extern s32 gCameraSceneParameters;
extern s32 Data_030011ec;
extern s32 Data_030011f0;
extern u32 Data_03001244;
extern u32 gFrameCount;

s32 IwramRatioMulQ14(s32 divisor, s32 value) __attribute__((long_call));
void IwramTransformVectorFarArm(const void *vector, s32 *result);
s32 Iwram_SqrtFarArm(s32 value);

void Func_080212dc(s32 depth, s32 *position, struct ScanVertex *scanline) {
  s32 groundPoint[3];
  s32 cameraPoint[3];
  s32 horizon;
  s32 focus;
  s32 *cameraCoordinates;
  s32 line;
  s32 distance;
  s32 difference;
  s32 scale;
  s32 horizontalSquared;
  s32 verticalSquared;
  s32 diagonal;
  s32 vertical;
  s32 zero;
  s32 horizontal;

  zero = 0;
  SET_VECTOR(groundPoint, position[0], zero, position[2]);
  IwramTransformVectorFarArm(groundPoint, cameraPoint);
  cameraCoordinates = cameraPoint;
  horizon = cameraCoordinates[1] - MulQ16(cameraCoordinates[2], depth);
  focus = -gCameraSceneParameters;
  for (line = 0; line < 160; line++) {
    distance = IwramRatioMulQ14(focus, (Data_030011f0 - line) * 65536);
    difference = distance - depth;
    if (difference == 0)
      difference = 1;
    scale = IwramRatioMulQ14(difference, horizon);
    if (scale < 0) {
      diagonal = MulQ16(-scale, 0x8000);
      scanline->x = IwramRatioMulQ14(gCameraSceneParameters, diagonal);
      diagonal = MulQ16(scale, distance);
      horizontal = (cameraCoordinates[2] - scale) >> 4;
      vertical = (diagonal - cameraCoordinates[1]) >> 4;
      horizontalSquared = MulQ16(horizontal, horizontal);
      verticalSquared = MulQ16(vertical, vertical);
      verticalSquared = Iwram_SqrtFarArm(horizontalSquared + verticalSquared)
                        << 12;
      if (vertical < 0)
        verticalSquared = -verticalSquared;
      scanline->z = MulQ16(verticalSquared, 0x8000);
    } else {
      scanline->x = 0;
      scanline->z = 0;
    }
    scanline->dx = 0;
    scanline->dz = 0;
    scanline++;
  }
}

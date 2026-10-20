// Draft, ARM C (game ARM route, GCC 2.95.2) of the two ARM routines behind
// the Thumb veneer Func_080212d8 (bx pc): an ARM copy of the perspective
// map's scanline table builder (☀️'s WorldMap_BuildScanlineTable, ⚓️'s
// Thumb Func_0802dd70) at 0x080212dc and ☀️'s Transform_UpdateVertices
// (games/tbs/src/field/vertices.c) at 0x08021470, written plain here (no
// FAKEMATCH devices). The listing is mixed-mode, which alchemy-devtools diff cannot
// compare; measured by disassembling the listing's ARM words (pools and
// the veneers between them left out) and the --arm draft's object with
// objdump and diffing the rows: 246 to 245 instructions, 99 rows differ
// (about 80 in the first routine, 19 in the second). Left in the first: the
// native has a 76-byte frame (each 3-word point at the start of a 32-byte
// area) where the draft has 36, keeps 0x8000 in r9, and calls
// IwramRatioMulQ14 through a register (ldr, mov lr, pc, bx) where GCC
// 2.95.2 emits bl (long_call and #pragma long_calls have no effect, and a
// cast symbol folds to bl). In the second: the native calls atan2 by bl to
// a call-via-ip stub after its pool (stmfd, add lr, pc, #1, bx ip; ☀️ and
// this compiler emit mov lr, pc, bx ip), the two loops share one
// epilogue, and ☀️'s tagged one-temporary order in the odd-frame sum is left
// out (three rows). Veneer and stub names are this draft's own.
#include "types.h"
#include "macros.h"
#include "mulq16.h"

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

extern const struct RuntimeMathTable WaitFramesFar;
extern s32 gCameraSceneParameters;
extern s32 Data_030011ec;
extern s32 Data_030011f0;
extern u32 Data_03001244;
extern u32 gFrameCount;

s32 IwramRatioMulQ14(s32 divisor, s32 value) __attribute__((long_call));
void IwramTransformVectorFarArm(const void *vector, s32 *result);
s32 IwramSqrtFarArm(s32 value);

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
      verticalSquared = IwramSqrtFarArm(horizontalSquared + verticalSquared)
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

void Func_0802146c(struct Position *eye, struct Position *target,
                   struct ScanVertex *line, struct BgAffine *affine) {
  u32 angle;
  s32 originX;
  s32 originZ;
  s32 sinYaw;
  s32 cosYaw;
  s32 negativeSinYaw;
  s32 scanline;
  s32 depth;

  {
    s32 targetDeltaX = target->x;
    s32 targetZ = target->z;
    s32 eyeZ = eye->z;
    s32 eyeX = eye->x;

    depth = targetZ - eyeZ;
    targetDeltaX -= eyeX;
    angle = (u16)WaitFramesFar.atan2.target(targetDeltaX >> 4, depth >> 4);
  }
  originX = MulQ16(target->x, 0x8000);
  originZ = MulQ16(target->z, 0x8000);
  if (angle != Data_03001244) {
    Data_03001244 = angle;
    sinYaw = WaitFramesFar.sin.target(angle);
    cosYaw = WaitFramesFar.cos.target(angle);
    negativeSinYaw = -sinYaw;
    for (scanline = 159; scanline >= 0; scanline--) {
      s32 widthScale = line->x;
      s32 centerX;
      s32 horizontalOffset;
      s32 depthOffset;
      s32 affinePa;
      s32 affinePc;

      depth = line->z;
      affinePa = -(MulQ16(widthScale, cosYaw) / 256);
      horizontalOffset = (s16)affinePa;
      line->u = affinePa;
      affine->pa = affinePa;
      affinePc = MulQ16(widthScale, sinYaw) / 256;
      line->v = affinePc;
      affine->pc = affinePc;
      centerX = Data_030011ec;
      horizontalOffset = (horizontalOffset << 8) * centerX;
      depthOffset = ((s16)affinePc << 8) * -centerX;
      line->dx = -(horizontalOffset + MulQ16(negativeSinYaw, depth));
      line->dz = depthOffset + MulQ16(cosYaw, depth);
      affine->x = (originX + line->dx) >> 8;
      affine->y = (originZ + line->dz) >> 8;
      affine[1] = affine[0];
      line++;
      affine += 2;
    }
  } else {
    for (scanline = 159; scanline >= 0; scanline--) {
      if (gFrameCount & 1) {
        affine->x =
            ((originX + line->dx) >> 8) + line->v / 4 + line->u / 2;
        affine->y =
            ((originZ + line->dz) >> 8) + line->u / 4 + line->v / 2;
      } else {
        affine->x = (originX + line->dx) >> 8;
        affine->y = (originZ + line->dz) >> 8;
      }
      affine->pa = line->u;
      affine->pc = line->v;
      affine[1] = affine[0];
      line++;
      affine += 2;
    }
  }
}

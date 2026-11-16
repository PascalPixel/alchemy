/// @file
/// An ARM routine that builds the per-scanline affine table of a perspective
/// ground plane, as The Broken Seal's Matrix_UpdateVertices. Its caller
/// copies 0x298 bytes from Matrix_UpdateVerticesCode
/// (world_map_lines_entry.s) into a heap slot and enters it in Thumb from
/// there, so its stub Matrix_CallInThumb (world_map_lines_call_thumb.s)
/// travels with it.

#include "types.h"
#include "fixed_math.h"
#include "system/mul_fx.h"
#include "system/frame.h"
#include "system/vector3.h"

struct FarCallVeneer {
  u32 stub;
  u32 target;
};

struct FarCallArcTanVeneer {
  u32 stub;
  s32 (*target)(s32 x, s32 y);
};

struct FarCallTrigVeneer {
  u32 stub;
  s32 (*target)(s32 angle);
};

struct FarCallRuntimeTrigView {
  struct FarCallVeneer preceding[8];
  struct FarCallArcTanVeneer arcTan2;
  struct FarCallVeneer squareRoots[2];
  struct FarCallTrigVeneer sin;
  struct FarCallTrigVeneer cos;
};

/// One end of a World Map line as it is scanned: its position, its step and
/// its texture coordinates.
struct ScanVertex {
  s32 x;
  s32 z;
  s32 dx;
  s32 dz;
  s16 u;
  s16 v;
};

/// A background's affine parameters: the matrix and the reference point.
struct BgAffine {
  s16 pa;
  s16 pb;
  s16 pc;
  s16 pd;
  s32 x;
  s32 y;
};

extern const struct FarCallRuntimeTrigView FarCall_RuntimeTable;
extern s32 Data_030011ec;
extern u32 gScanlineAngle;

s32 Matrix_CallInThumb(s32 x, s32 y);

/// Builds the world map's per-scanline affine table for one frame: the ground
/// plane seen from eye towards target. line points at the 160 scanlines' ground
/// terms, whose rotation terms stay cached while the yaw (gScanlineAngle) is
/// unchanged; affine receives each line's parameters twice, for BG2 and then
/// BG3, as the HBlank DMA writes both.
void Matrix_UpdateVertices(struct Vector3 *eye, struct Vector3 *target,
                           struct ScanVertex *line, struct BgAffine *affine) {
  u32 angle;
  FIXED originX;
  FIXED originZ;
  s32 sinYaw;
  s32 cosYaw;
  FIXED negativeSinYaw;
  s32 scanline;
  s32 depth;
  const struct FarCallRuntimeTrigView *table = &FarCall_RuntimeTable;

  {
    s32 targetDeltaX = target->x;
    s32 targetZ = target->z;
    s32 eyeZ = eye->z;
    s32 eyeX = eye->x;

    depth = targetZ - eyeZ;
    targetDeltaX -= eyeX;
    {
      /* FAKEMATCH(pin): x held in r0 from its shift. */
      register s32 x __asm__("r0") = targetDeltaX >> 4;
      s32 y = depth >> 4;
      CAMELOT_ASM("ip holds the target Matrix_CallInThumb jumps to, a "
                  "linkage no procedure-call standard produces")
      register void *entry __asm__("ip") = table->arcTan2.target;

      CAMELOT_ASM("the empty asm keeps ip set for the call, which C cannot say")
      __asm__("" : : "r"(entry));
      angle = (u16)Matrix_CallInThumb(x, y);
    }
  }
  originX = MulFX(target->x, FIXED_ONE / 2);
  originZ = MulFX(target->z, FIXED_ONE / 2);
  if (angle != gScanlineAngle) {
    gScanlineAngle = angle;
    sinYaw = table->sin.target(angle);
    cosYaw = table->cos.target(angle);
    negativeSinYaw = -sinYaw;
    for (scanline = 159; scanline >= 0; scanline--) {
      FIXED widthScale = line->x;
      s32 centerX;
      s32 horizontalOffset;
      s32 depthOffset;
      s32 affinePa;
      s32 affinePc;

      depth = line->z;
      affinePa = -(MulFX(widthScale, cosYaw) / 256);
      horizontalOffset = (s16)affinePa;
      line->u = affinePa;
      affine->pa = affinePa;
      affinePc = MulFX(widthScale, sinYaw) / 256;
      line->v = affinePc;
      affine->pc = affinePc;
      centerX = Data_030011ec;
      horizontalOffset = (horizontalOffset << 8) * centerX;
      depthOffset = ((s16)affinePc << 8) * -centerX;
      line->dx = -(horizontalOffset + MulFX(negativeSinYaw, depth));
      line->dz = depthOffset + MulFX(cosYaw, depth);
      affine->x = (originX + line->dx) >> 8;
      affine->y = (originZ + line->dz) >> 8;
      affine[1] = affine[0];
      line++;
      affine += 2;
    }
  } else {
    for (scanline = 159; scanline >= 0; scanline--) {
      if (gFrameCount & 1) {
        // sampleOffset is reused for u; two plain expressions differ.
        s32 sampleOffset;

        sampleOffset = ((originX + line->dx) >> 8) + line->v / 4 + line->u / 2;
        affine->x = sampleOffset;
        sampleOffset = line->u;
        affine->y =
            ((originZ + line->dz) >> 8) + sampleOffset / 4 + line->v / 2;
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

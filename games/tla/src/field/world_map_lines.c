/// @file
/// An ARM routine that builds the per-scanline affine table of a perspective
/// ground plane, as The Broken Seal's Transform_UpdateVertices. Its caller
/// copies 0x298 bytes from Transform_UpdateVerticesCode
/// (world_map_lines_entry.s) into a heap slot and enters it in Thumb from
/// there, so its stub Transform_CallInThumb (world_map_lines_call_thumb.s)
/// travels with it.

#include "types.h"
#include "fixed_math.h"
#include "system/mul_q16.h"
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

s32 Transform_CallInThumb(s32 x, s32 y);

/// Builds the world map's per-scanline affine table for one frame: the ground
/// plane seen from eye towards target. line points at the 160 scanlines' ground
/// terms, whose rotation terms stay cached while the yaw (gScanlineAngle) is
/// unchanged; affine receives each line's parameters twice, for BG2 and then
/// BG3, as the HBlank DMA writes both.
void Transform_UpdateVertices(struct Vector3 *eye, struct Vector3 *target,
                              struct ScanVertex *line,
                              struct BgAffine *affine) {
  u32 angle;
  s32 originX;
  s32 originZ;
  s32 sinYaw;
  s32 cosYaw;
  s32 negativeSinYaw;
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
      // Transform_CallInThumb jumps to the target it finds in ip; the asm
      // keeps ip set for the call, which C cannot say.
      register void *entry __asm__("ip") = table->arcTan2.target;

      __asm__("" : : "r"(entry));
      angle = (u16)Transform_CallInThumb(x, y);
    }
  }
  originX = MulQ16(target->x, Q16_ONE / 2);
  originZ = MulQ16(target->z, Q16_ONE / 2);
  if (angle != gScanlineAngle) {
    gScanlineAngle = angle;
    sinYaw = table->sin.target(angle);
    cosYaw = table->cos.target(angle);
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

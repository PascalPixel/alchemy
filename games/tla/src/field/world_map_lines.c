/// @file
/// The World Map's tilt: each line of the screen draws the flat map at its
/// own scale and turn, so the ground seems to stretch away to a horizon. An
/// ARM routine, as The Broken Seal's Matrix_UpdateVertices: its caller copies
/// its 664 bytes from Matrix_UpdateVerticesCode (world_map_lines_entry.s) into
/// a heap slot and enters it in Thumb from there, so its stub
/// Matrix_CallInThumb (world_map_lines_call_thumb.s) travels with it.

#include "types.h"
#include "fixed_math.h"
#include "system/mul_fx.h"
#include "system/frame.h"
#include "system/vector3.h"
#include "system/projection.h"

#define VISIBLE_SCANLINES 160

/// An eight-byte entry of the System bank's far-call table holds the ldr/bx
/// instructions and a target word.
struct FarCallVeneer {
  u32 stub;
  u32 target;
};

/// A System table entry whose two-argument target this routine calls, the
/// arctangent.
struct FarCallArcTanVeneer {
  u32 stub;
  s32 (*target)(s32 x, s32 y);
};

/// A System table entry whose one-argument target this routine calls, the
/// sine or the cosine.
struct FarCallTrigVeneer {
  u32 stub;
  s32 (*target)(s32 angle);
};

/// The start of the System bank's far-call table, as far as Trig_Cos.
struct FarCallSystemTable {
  struct FarCallVeneer preceding[8];
  struct FarCallArcTanVeneer arcTan2;
  struct FarCallVeneer squareRoots[2];
  struct FarCallTrigVeneer sin;
  struct FarCallTrigVeneer cos;
};

/// One scanline of the world map's ground plane.
struct ScanVertex {
  s32 x;  ///< the line's width scale
  s32 z;  ///< the line's depth
  s32 dx; ///< the texture x offset from the origin, before >> 8
  s32 dz; ///< the texture y offset from the origin, before >> 8
  s16 u;  ///< the cached BG2PA
  s16 v;  ///< the cached BG2PC
};

/// One background's affine registers, BG2PA to BG2Y.
struct BgAffine {
  s16 pa;
  s16 pb; ///< not written by this routine
  s16 pc;
  s16 pd; ///< not written by this routine
  s32 x;
  s32 y;
};

extern const struct FarCallSystemTable FarCall_RuntimeTable;
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
  const struct FarCallSystemTable *table = &FarCall_RuntimeTable;

  {
    s32 targetDeltaX = target->x;
    s32 targetZ = target->z;
    s32 eyeZ = eye->z;
    s32 eyeX = eye->x;

    depth = targetZ - eyeZ;
    targetDeltaX -= eyeX;
    {
      /* FAKEMATCH(pin): x fixed to a register. */
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
  // The texture origin: the target at half scale.
  originX = MulFX(target->x, FIXED_ONE / 2);
  originZ = MulFX(target->z, FIXED_ONE / 2);
  if (angle != gScanlineAngle) {
    // The camera turned: recompute every scanline's rotation terms.
    gScanlineAngle = angle;
    sinYaw = table->sin.target(angle);
    cosYaw = table->cos.target(angle);
    negativeSinYaw = -sinYaw;
    for (scanline = 0; scanline < VISIBLE_SCANLINES; scanline++) {
      FIXED widthScale = line->x;
      s32 centerX;
      s32 horizontalOffset;
      s32 depthOffset;
      s32 affinePa;
      s32 affinePc;

      depth = line->z;
      // The BG affine parameters are 8.8 fixed point: / 256 takes the 16.16
      // products there.
      affinePa = -(MulFX(widthScale, cosYaw) / 256);
      horizontalOffset = (s16)affinePa;
      line->u = affinePa;
      affine->pa = affinePa;
      affinePc = MulFX(widthScale, sinYaw) / 256;
      line->v = affinePc;
      affine->pc = affinePc;
      centerX = *&gProjection.centerX;
      // UB: affine coefficients can be negative, and shifting a negative
      // value left is undefined; UBFIX multiplies by 256.
#ifdef UBFIX
      horizontalOffset = (horizontalOffset * 256) * centerX;
      depthOffset = ((s16)affinePc * 256) * -centerX;
#else
      horizontalOffset = (horizontalOffset << 8) * centerX;
      depthOffset = ((s16)affinePc << 8) * -centerX;
#endif
      // The line's start: back from its centre by centerX pixels of the line's
      // step, then along the view by its depth.
      line->dx = -(horizontalOffset + MulFX(negativeSinYaw, depth));
      line->dz = depthOffset + MulFX(cosYaw, depth);
      affine->x = (originX + line->dx) >> 8;
      affine->y = (originZ + line->dz) >> 8;
      affine[1] = affine[0];
      line++;
      affine += 2;
    }
  } else {
    // Same angle: the cached terms stand and only the position moves.
    for (scanline = 0; scanline < VISIBLE_SCANLINES; scanline++) {
      if (gFrameCount & 1) {
        // On odd frames the sample point shifts by half a pixel step along
        // the line and a quarter step across it, so alternate frames sample
        // between each other's texels.
        /* FAKEMATCH(temporary): sampleOffset takes a copy of line->u for the
         * second sum. */
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

#undef VISIBLE_SCANLINES

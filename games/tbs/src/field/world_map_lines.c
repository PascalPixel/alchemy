#include "module/field/world_map_lines.h"
#include "types.h"
#include "frame.h"
#include "mul_q16.h"
#include "projection.h"
#include "static_assert.h"
#include "vector3.h"

#define VISIBLE_SCANLINES 160 ///< visible scanlines

/// An eight-byte runtime entry contains the ldr/bx instructions and a target
/// word. Entries this routine does not call keep their target word opaque.
struct FarCallVeneer {
  u32 stub;
  u32 target;
};

/// A runtime table entry whose two-argument target this routine calls, the
/// arctangent.
struct FarCallAtan2Veneer {
  u32 stub;
  s32 (*target)(s32 x, s32 y);
};

/// A runtime table entry whose one-argument target this routine calls, the
/// sine or the cosine.
struct FarCallTrigVeneer {
  u32 stub;
  s32 (*target)(s32 angle);
};

/// The runtime table's prefix through Trig_Cos, in its assembly order. This
/// routine runs from a RAM copy and calls the target words, not the ROM stubs.
struct FarCallRuntimeTrigView {
  struct FarCallVeneer preceding[8];
  struct FarCallAtan2Veneer atan2;
  struct FarCallVeneer squareRoots[2];
  struct FarCallTrigVeneer sin;
  struct FarCallTrigVeneer cos;
};

STATIC_ASSERT(sizeof(struct FarCallVeneer) == 8);
STATIC_ASSERT(sizeof(struct FarCallAtan2Veneer) == 8);
STATIC_ASSERT(sizeof(struct FarCallTrigVeneer) == 8);
STATIC_ASSERT(offsetof(struct FarCallRuntimeTrigView, atan2) == 64);
STATIC_ASSERT(offsetof(struct FarCallRuntimeTrigView, sin) == 88);
STATIC_ASSERT(offsetof(struct FarCallRuntimeTrigView, cos) == 96);

extern const struct FarCallRuntimeTrigView FarCall_RuntimeTable;
/// The yaw, in 1/65536 turns, that the cached rotation terms in each
/// ScanVertex were computed for.
extern u32 gScanlineAngle;

/// One scanline of the world map's ground plane. WorldMap_BuildScanlineTable
/// fills x (the line's width scale) and z (its depth); the rest caches the
/// rotation terms this routine computes from them for the current angle.
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

/// Builds the world map's per-scanline affine table for one frame: the ground
/// plane seen from eye towards target. line points at the 160 scanlines'
/// ground terms, whose rotation terms stay cached while the yaw
/// (gScanlineAngle) is unchanged; affine receives each line's parameters
/// twice, for BG2 and then BG3, as the HBlank DMA writes both. Copied to RAM
/// (heap slot 46) by WorldMap_InitializePerspective, which runs it once, and
/// run from there each frame by WorldMap_UpdateView into the affine page of
/// the frame's parity. Writes gScanlineAngle when the yaw changes.
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

  {
    s32 targetDeltaX = target->x;
    s32 targetZ = target->z;
    s32 eyeZ = eye->z;
    s32 eyeX = eye->x;

    depth = targetZ - eyeZ;
    targetDeltaX -= eyeX;
    angle =
        (u16)FarCall_RuntimeTable.atan2.target(targetDeltaX >> 4, depth >> 4);
  }
  // The texture origin: the target at half scale.
  originX = MulQ16(target->x, 0x8000);
  originZ = MulQ16(target->z, 0x8000);
  if (angle != gScanlineAngle) {
    // The camera turned: recompute every scanline's rotation terms.
    gScanlineAngle = angle;
    sinYaw = FarCall_RuntimeTable.sin.target(angle);
    cosYaw = FarCall_RuntimeTable.cos.target(angle);
    negativeSinYaw = -sinYaw;
    for (scanline = VISIBLE_SCANLINES - 1; scanline >= 0; scanline--) {
      s32 widthScale = line->x;
      s32 centerX;
      s32 horizontalOffset;
      s32 depthOffset;
      s32 affinePa;
      s32 affinePc;

      depth = line->z;
      // The BG affine parameters are 8.8 fixed point: / 256 takes the 16.16
      // products there.
      affinePa = -(MulQ16(widthScale, cosYaw) / 256);
      horizontalOffset = (s16)affinePa;
      line->u = affinePa;
      affine->pa = affinePa;
      affinePc = MulQ16(widthScale, sinYaw) / 256;
      line->v = affinePc;
      affine->pc = affinePc;
      // Read through its own address, the native builds one literal for the
      // centre; as gProjection.centerX it loads gProjection and reads at
      // offset 12 (1 of 157 instructions differ in tbs-en).
      centerX = *&gProjection.centerX;
      // UB: affine coefficients can be negative. Multiplication by 256 is
      // representable for every signed halfword; retain native shifts below.
#ifdef UBFIX
      horizontalOffset = (horizontalOffset * 256) * centerX;
      depthOffset = ((s16)affinePc * 256) * -centerX;
#else
      horizontalOffset = (horizontalOffset << 8) * centerX;
      depthOffset = ((s16)affinePc << 8) * -centerX;
#endif
      // The line's start: back from its centre by centerX pixels of the
      // line's step, then along the view by its depth. BG2X and BG2Y take
      // 8 fractional bits, so the 16.16 sums are >> 8.
      line->dx = -(horizontalOffset + MulQ16(negativeSinYaw, depth));
      line->dz = depthOffset + MulQ16(cosYaw, depth);
      affine->x = (originX + line->dx) >> 8;
      affine->y = (originZ + line->dz) >> 8;
      affine[1] = affine[0];
      line++;
      affine += 2;
    }
  } else {
    // Same angle: the cached terms stand and only the position moves.
    for (scanline = VISIBLE_SCANLINES - 1; scanline >= 0; scanline--) {
      if (gFrameCount & 1) {
        // On odd frames the sample point shifts by half a pixel step along
        // the line and a quarter step across it, so alternate frames sample
        // between each other's texels.
        /* FAKEMATCH(temporary): one variable holds the x sum and then u, both
         * in r3 as native; as two plain expressions, 3 of 157 instructions
         * differ in tbs-en. */
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

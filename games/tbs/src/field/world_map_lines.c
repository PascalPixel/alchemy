/// @file
/// The World Map's tilt: each line of the screen draws the flat map at its
/// own scale and turn, so the ground seems to stretch away to a horizon. The
/// routine runs from its copy in HEAP_SLOT_RAM_CODE (field_world_map.c), so it
/// calls the arctangent, sine and cosine through the System bank's far-call
/// table.

#include "field/world_map_lines.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "system/frame.h"
#include "system/io_reg.h"
#include "system/mul_fx.h"
#include "system/projection.h"
#include "system/vector3.h"

/// A 16.16 value in the 8.8 fixed point of the affine registers, rounded
/// toward zero, and an 8.8 value back in 16.16.
#define FIXEDtoQ_8_8(value) ((value) / (FIXED_ONE / Q_8_8(1)))
#define Q_8_8toFIXED(value) ((value) * (FIXED_ONE / Q_8_8(1)))

/// An entry of the System bank's far-call table: a stub that jumps to its
/// target, and the target.
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

STATIC_ASSERT(sizeof(struct FarCallVeneer) == 8);
STATIC_ASSERT(sizeof(struct FarCallArcTanVeneer) == 8);
STATIC_ASSERT(sizeof(struct FarCallTrigVeneer) == 8);
STATIC_ASSERT(offsetof(struct FarCallSystemTable, arcTan2) == 64);
STATIC_ASSERT(offsetof(struct FarCallSystemTable, sin) == 88);
STATIC_ASSERT(offsetof(struct FarCallSystemTable, cos) == 96);

/// The System bank's far-call table, through which the world map reaches its
/// arctangent, sine and cosine.
extern const struct FarCallSystemTable FarCall_RuntimeTable;

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

  {
    s32 targetDeltaX = target->x;
    s32 targetZ = target->z;
    s32 eyeZ = eye->z;
    s32 eyeX = eye->x;

    depth = targetZ - eyeZ;
    targetDeltaX -= eyeX;
    angle =
        (u16)FarCall_RuntimeTable.arcTan2.target(targetDeltaX >> 4, depth >> 4);
  }
  // The texture origin: the target at half scale.
  originX = MulFX(target->x, FIXED_ONE / 2);
  originZ = MulFX(target->z, FIXED_ONE / 2);
  if (angle != gScanlineAngle) {
    // The camera turned: recompute every scanline's rotation terms.
    gScanlineAngle = angle;
    sinYaw = FarCall_RuntimeTable.sin.target(angle);
    cosYaw = FarCall_RuntimeTable.cos.target(angle);
    negativeSinYaw = -sinYaw;
    for (scanline = 0; scanline < DISPLAY_HEIGHT; scanline++) {
      FIXED widthScale = line->x;
      s32 centerX;
      s32 horizontalOffset;
      s32 depthOffset;
      s32 affinePa;
      s32 affinePc;

      depth = line->z;
      affinePa = -FIXEDtoQ_8_8(MulFX(widthScale, cosYaw));
      horizontalOffset = (s16)affinePa;
      line->u = affinePa;
      affine->pa = affinePa;
      affinePc = FIXEDtoQ_8_8(MulFX(widthScale, sinYaw));
      line->v = affinePc;
      affine->pc = affinePc;
      /* FAKEMATCH(dead): the projection's centre read through its taken
       * address. */
      centerX = *&gProjection.centerX;
      // UB: affine coefficients can be negative, and shifting a negative
      // value left is undefined; UBFIX multiplies.
#ifdef UBFIX
      horizontalOffset = Q_8_8toFIXED(horizontalOffset) * centerX;
      depthOffset = Q_8_8toFIXED((s16)affinePc) * -centerX;
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
    for (scanline = 0; scanline < DISPLAY_HEIGHT; scanline++) {
      if (gFrameCount & 1) {
        // On odd frames the sample point shifts by half a pixel step along
        // the line and a quarter step across it, so alternate frames sample
        // between each other's texels.
        s32 sampleOffset;

        sampleOffset = ((originX + line->dx) >> 8) + line->v / 4 + line->u / 2;
        affine->x = sampleOffset;
        /* FAKEMATCH(dead): sampleOffset set to line->u, never read. */
        sampleOffset = line->u;
        affine->y = ((originZ + line->dz) >> 8) + line->u / 4 + line->v / 2;
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

#undef FIXEDtoQ_8_8
#undef Q_8_8toFIXED

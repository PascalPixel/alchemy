/// @file
/// The World Map's tilt: each line of the screen draws the flat map at its
/// own scale and turn, so the ground seems to stretch away to a horizon. An
/// ARM routine, as The Broken Seal's WorldMap_UpdateAffineLines: its caller
/// copies the routine from WorldMap_UpdateAffineLinesCode
/// (world_map_lines_entry.s) into a heap slot and enters it in Thumb from
/// there, so its stub Matrix_CallInThumb (world_map_lines_call_thumb.s) travels
/// with it.

#include "types.h"
#include "fixed_math.h"
#include "system/mul_fx.h"
#include "system/frame.h"
#include "system/vector3.h"
#include "system/projection.h"
#include "system/io_reg.h"
#include "field/world_map_affine.h"

/// A 16.16 value in the 8.8 fixed point of the affine registers, rounded
/// toward zero, and an 8.8 value back in 16.16.
#define FIXEDtoQ_8_8(value) ((value) / (FIXED_ONE / Q_8_8(1)))
#define Q_8_8toFIXED(value) ((value) * (FIXED_ONE / Q_8_8(1)))

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

/// The far-call table of system routines, read here for its arctangent.
extern const struct FarCallSystemTable FarCall_RuntimeTable;
/// The camera's yaw the scanline terms were built for, declared here as in
/// field.h, whose anonymous unions this file's ARM compiler cannot read.
extern u32 gScanlineAngle;

/// Calls the Thumb routine whose address is in ip with (x, y) from this ARM
/// code and returns its result: here the far-call table's arctangent, for the
/// camera's yaw.
s32 Matrix_CallInThumb(s32 x, s32 y);

/// Builds the world map's per-scanline affine table for one frame: the ground
/// plane seen from eye towards target. line points at the 160 scanlines' ground
/// terms, whose rotation terms stay cached while the yaw
/// (gScanlineAngle) is unchanged; affine receives each line's parameters twice,
/// for BG2 and then BG3, as the HBlank DMA writes both.
void WorldMap_UpdateAffineLines(struct Vector3 *eye, struct Vector3 *target,
                                struct WorldMapScanline *line,
                                struct WorldMapAffineLine *affine) {
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
      s32 x = targetDeltaX >> 4;
      s32 y;
      CAMELOT_ASM("ip holds the target Matrix_CallInThumb jumps to, a "
                  "linkage no procedure-call standard produces")
      register void *entry __asm__("ip");

      /* FAKEMATCH(barrier): an empty asm on targetDeltaX. */
      __asm__("" : : "r"(targetDeltaX));
      y = depth >> 4;
      entry = table->arcTan2.target;
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
    for (scanline = 0; scanline < DISPLAY_HEIGHT; scanline++) {
      FIXED widthScale = line->scale;
      s32 centerX;
      s32 horizontalOffset;
      s32 depthOffset;
      s32 affinePa;
      s32 affinePc;

      depth = line->depth;
      affinePa = -FIXEDtoQ_8_8(MulFX(widthScale, cosYaw));
      horizontalOffset = (s16)affinePa;
      line->pa = affinePa;
      affine->pa = affinePa;
      affinePc = FIXEDtoQ_8_8(MulFX(widthScale, sinYaw));
      line->pc = affinePc;
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
      line->offsetX = -(horizontalOffset + MulFX(negativeSinYaw, depth));
      line->offsetY = depthOffset + MulFX(cosYaw, depth);
      affine->x = (originX + line->offsetX) >> 8;
      affine->y = (originZ + line->offsetY) >> 8;
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

        sampleOffset =
            ((originX + line->offsetX) >> 8) + line->pc / 4 + line->pa / 2;
        affine->x = sampleOffset;
        /* FAKEMATCH(dead): sampleOffset set to line->pa, never read. */
        sampleOffset = line->pa;
        affine->y =
            ((originZ + line->offsetY) >> 8) + line->pa / 4 + line->pc / 2;
      } else {
        affine->x = (originX + line->offsetX) >> 8;
        affine->y = (originZ + line->offsetY) >> 8;
      }
      affine->pa = line->pa;
      affine->pc = line->pc;
      affine[1] = affine[0];
      line++;
      affine += 2;
    }
  }
}

#undef FIXEDtoQ_8_8
#undef Q_8_8toFIXED

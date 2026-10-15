#include "unit/field/vertices.h"
#include "types.h"
#include "frame.h"
#include "mulq16.h"
#include "project.h"

#define LINES 160 // visible scanlines

// An 8-byte far-call stub (ldr r4 / bx r4) followed by its target. This code
// runs from a RAM copy, so it calls the runtime through the stubs' targets.
struct FarCallEntry {
  u32 stub;
  void *target;
};

extern struct FarCallEntry FarCall_RuntimeTable[];
extern u32 gScanlineAngle;

#define FAR_CALL(index, type) ((type)FarCall_RuntimeTable[index].target)
#define ArcTan2(x, z) FAR_CALL(8, s32 (*)(s32, s32))(x, z)
#define Sin(angle) FAR_CALL(11, s32 (*)(s32))(angle)
#define Cos(angle) FAR_CALL(12, s32 (*)(s32))(angle)

struct Position {
  s32 x;
  s32 y;
  s32 z;
};

// One scanline of the world map's ground plane. WorldMap_BuildScanlineTable
// fills x (the line's width scale) and z (its depth); the rest caches the
// rotation terms this routine computes from them for the current angle.
struct ScanVertex {
  s32 x;
  s32 z;
  s32 dx;
  s32 dz;
  s16 u;
  s16 v;
};

// One background's affine registers, BG2PA to BG2Y.
struct BgAffine {
  s16 pa;
  s16 pb;
  s16 pc;
  s16 pd;
  s32 x;
  s32 y;
};

// Builds the world map's per-scanline affine table for one frame: the ground
// plane seen from eye towards target. line points at the 160 scanlines'
// ground terms, whose rotation terms stay cached while the yaw
// (gScanlineAngle) is unchanged;
// affine receives each line's parameters twice, for BG2 and then BG3, as the
// HBlank DMA writes both. Copied to RAM and run from there.
void Transform_UpdateVertices(struct Position *eye, struct Position *target,
                              struct ScanVertex *line,
                              struct BgAffine *affine) {
  u32 angle;
  s32 originX;
  s32 originZ;
  s32 sin;
  s32 cos;
  s32 nsin;
  s32 i;
  s32 z;

  {
    s32 tx = target->x;
    s32 tz = target->z;
    s32 ez = eye->z;
    s32 ex = eye->x;

    z = tz - ez;
    tx -= ex;
    angle = (u16)ArcTan2(tx >> 4, z >> 4);
  }
  // The texture origin: the target at half scale.
  originX = MulQ16(target->x, 0x8000);
  originZ = MulQ16(target->z, 0x8000);
  if (angle != gScanlineAngle) {
    // The camera turned: recompute every scanline's rotation terms.
    gScanlineAngle = angle;
    sin = Sin(angle);
    cos = Cos(angle);
    nsin = -sin;
    for (i = LINES - 1; i >= 0; i--) {
      s32 x = line->x;
      s32 cx;
      s32 a;
      s32 b;
      s32 u;
      s32 v;

      z = line->z;
      u = -(MulQ16(x, cos) / 256);
      a = (s16)u;
      line->u = u;
      affine->pa = u;
      v = MulQ16(x, sin) / 256;
      line->v = v;
      affine->pc = v;
      /* FAKEMATCH: the centre is read through its own address, so the native
       * single literal for it is built; member access loads gProjection and
       * reads at offset 12 (differ 2). */
      cx = *&gProjection.centerX;
      a = (a << 8) * cx;
      b = ((s16)v << 8) * -cx;
      line->dx = -(a + MulQ16(nsin, z));
      line->dz = b + MulQ16(cos, z);
      affine->x = (originX + line->dx) >> 8;
      affine->y = (originZ + line->dz) >> 8;
      affine[1] = affine[0];
      line++;
      affine += 2;
    }
  } else {
    // Same angle: the cached terms stand and only the position moves.
    for (i = LINES - 1; i >= 0; i--) {
      if (gFrameCount & 1) {
        // On odd frames the sample point shifts by half a pixel step along
        // the line and a quarter step across it, so alternate frames sample
        // between each other's texels.
        /* FAKEMATCH: one scratch variable holds the x sum and then u, as the
         * native keeps both in r3; with separate values three instructions of
         * affine->y come in another order (differ 6). */
        s32 work;

        work = ((originX + line->dx) >> 8) + line->v / 4 + line->u / 2;
        affine->x = work;
        work = line->u;
        affine->y = ((originZ + line->dz) >> 8) + work / 4 + line->v / 2;
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

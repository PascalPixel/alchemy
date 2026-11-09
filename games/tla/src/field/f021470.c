/// @file
/// An ARM routine that builds the per-scanline affine table of a perspective
/// ground plane, as The Broken Seal's Transform_UpdateVertices. Its caller
/// copies 0x298 bytes from Data_0802146c (f021470_entries.s) into a heap slot
/// and enters it in Thumb from there, so its stub Func_080216f0
/// (f021470_call_thumb.s) travels with it.

#include "types.h"
#include "mul_q16.h"

struct FarCallVeneer {
  u32 stub;
  u32 target;
};

struct FarCallAtan2Veneer {
  u32 stub;
  s32 (*target)(s32 x, s32 y);
};

struct FarCallTrigVeneer {
  u32 stub;
  s32 (*target)(s32 angle);
};

struct FarCallRuntimeTrigView {
  struct FarCallVeneer preceding[8];
  struct FarCallAtan2Veneer atan2;
  struct FarCallVeneer squareRoots[2];
  struct FarCallTrigVeneer sin;
  struct FarCallTrigVeneer cos;
};

struct Vector3 {
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

extern const struct FarCallRuntimeTrigView FarCall_RuntimeTable;
extern s32 Data_030011ec;
extern u32 Data_03001244;
extern u32 gFrameCount;

s32 Func_080216f0(s32 x, s32 y);

void Func_08021470(struct Vector3 *eye, struct Vector3 *target,
                   struct ScanVertex *line, struct BgAffine *affine) {
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
      /* FAKEMATCH(pin): the call's arguments and ip, where its stub reads the
       * target, pinned; as a plain call to the stub, 3 of 156 instructions
       * differ in tla-en. */
      register s32 x __asm__("r0") = targetDeltaX >> 4;
      register s32 y __asm__("r1") = depth >> 4;
      register void *entry __asm__("ip") = table->atan2.target;
      /* FAKEMATCH(barrier): an empty asm keeps ip's target alive into the
       * call; as the pins alone, 3 of 156 instructions differ in tla-en. */
      __asm__("" : : "r"(x), "r"(y), "r"(entry));
      angle = (u16)Func_080216f0(x, y);
    }
  }
  originX = MulQ16(target->x, 0x8000);
  originZ = MulQ16(target->z, 0x8000);
  if (angle != Data_03001244) {
    Data_03001244 = angle;
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

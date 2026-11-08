// Differ 12 of 132 (tla-en, 2026-11-08, -mtune=gba).
// 2026-11-08: a pointer to result[1] set after the call (far = &result[1],
// reading *far and far[1]) gives the native's allocation exactly but its
// offsets are 4 off (16); the same pointer at result[0] is merged by CSE
// with the call's argument (40), and an 8-minute permute found nothing
// valid under 12. Removing the zero local, 20.
// Trial: ☀️'s WorldMap_BuildScanlineTable (games/tbs/src/field/field.c)
// word for word over ⚓️'s projection record differs 70 of 132 (register
// choice: the line counter and the camera point's address swap fp and a
// stack slot); this draft, written from the listing, comes closer.
// For each of the 160 scanlines, projects a ground point through the camera
// (gCameraSceneParameters) and stores its x and y offsets in 16.16, or zeros
// when the line is above the horizon. The pointer to result[2] (found by the
// permuter) and height declared before nearZ give the native's registers
// and stack slots; comparing distance with a local zero (permuter, 20 to 12)
// is a local holding one constant, ordinary C under S2. Left (allocation only): the
// native passes add r1, sp, #16 to the transform call and spills a fresh
// copy of the result's address after it, where agscc spills one pointer
// before the call. Plain result[] reads 55; a pointer to result[0] 40 to 49.
#include "types.h"
#include "iwram_call.h"

extern u32 gCameraSceneParameters[];

struct ScanlineRow {
  s32 x;
  s32 y;
  s32 unk08;
  s32 unk0c;
  s32 unk10;
};

void Func_0802dd70(s32 depth, s32 *position, struct ScanlineRow *row) {
  s32 vector[3];
  s32 result[3];
  s32 height;
  s32 nearZ;
  s32 i;
  s32 z;
  s32 ratio;
  s32 dx;
  s32 zero; // A local that holds one constant: distance is compared with it.
  s32 sum;
  s32 *far;

  zero = 0;
  vector[0] = position[0];
  vector[1] = 0;
  vector[2] = position[2];
  Iwram_TransformVector(vector, result);
  far = &result[2];
  height = result[1] - Iwram_MulQ16(*far, depth);
  nearZ = -gCameraSceneParameters[0];
  for (i = 0; i < 160; i++) {
    s32 distance;

    z = Iwram_RatioMulQ14(nearZ, (gCameraSceneParameters[4] - i) << 16);
    distance = z - depth;
    if (distance == zero)
      distance = 1;
    ratio = Iwram_RatioMulQ14(distance, height);
    if (ratio < 0) {
      row->x = Iwram_RatioMulQ14(gCameraSceneParameters[0],
                                 Iwram_MulQ16(-ratio, 0x8000));
      z = Iwram_MulQ16(ratio, z);
      dx = (result[2] - ratio) >> 4;
      z -= result[1];
      sum = Iwram_MulQ16(dx, dx);
      z >>= 4;
      sum += Iwram_MulQ16(z, z);
      {
        s32 length = Iwram_Sqrt(sum) << 12;

        if (z < 0)
          length = -length;
        row->y = Iwram_MulQ16(length, 0x8000);
      }
    } else {
      row->x = 0;
      row->y = 0;
    }
    row->unk08 = 0;
    row->unk0c = 0;
    row++;
  }
}

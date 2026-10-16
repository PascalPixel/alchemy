#include "types.h"
#include "iwram_call.h"

struct Projection {
  s32 focal;
  s32 near;
  s32 far;
  s32 centerX;
  s32 centerY;
};

struct ProjectedPoint {
  s32 x;
  s32 y;
  s32 z;
};

extern struct Projection gCameraSceneParameters;
s32 Render_ProjectPoint(const void *point, struct ProjectedPoint *screen) {
  struct ProjectedPoint view;
  struct Projection *projection;
  s32 depth;
  s32 scale;
  s32 x;

  Iwram_TransformVector(point, &view.x);
  projection = &gCameraSceneParameters;
  depth = -view.z;
  if (depth >= projection->near && depth <= projection->far) {
    screen->z = depth >> 16;
    if (projection->focal != 0) {
      u32 shifted = (u32)depth >> 11;

      scale = Iwram_UnsignedDivide(projection->focal << 5, shifted);
    } else {
      scale = 0x151eb;
    }
    x = projection->centerX + Iwram_MulQ16(view.x, scale) / 0x10000;
    screen->y = projection->centerY - Iwram_MulQ16(view.y, scale) / 0x10000;
    screen->x = x;
    return scale;
  }
  *screen = view;
  return 0;
}

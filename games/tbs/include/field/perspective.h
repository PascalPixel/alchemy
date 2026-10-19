#ifndef ALCHEMY_PERSPECTIVE_H
#define ALCHEMY_PERSPECTIVE_H

#include "types.h"
#include "static_assert.h"

struct PerspectiveWork {
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 *script;
  u16 fade;
  u8 fadeStep;
  u8 unk017[0xe4 - 0x17];
  s32 viewX;
  s32 viewY;
  s32 scaleX;
  s32 scaleY;
  s32 limitX;
  s32 limitY;
  s8 animationActive;
  u8 unk0fd[3];
  u16 windowTop;
  u16 windowBottom;
  u16 shownTop;
  u16 shownBottom;
  u16 windowPending;
  u8 unk10a[6];
  void *tiles;
  u8 unk114[4];
  u16 pitch;
  u16 yaw;
  u32 *resources;
  u8 unk120[0x18];
  u16 tileIds[256];
  u16 quarterTiles[8];
  s32 farPlane;
  s32 distance;
  s32 previousZoom;
  s32 zoom;
  u16 zoomTransitionFrames;
  s16 zoomTransitionStep;
};

STATIC_ASSERT(sizeof(struct PerspectiveWork) == 0x35c);
STATIC_ASSERT(offsetof(struct PerspectiveWork, viewX) == 0xe4);
STATIC_ASSERT(offsetof(struct PerspectiveWork, pitch) == 0x118);
STATIC_ASSERT(offsetof(struct PerspectiveWork, animationActive) == 0xfc);
STATIC_ASSERT(offsetof(struct PerspectiveWork, resources) == 0x11c);
STATIC_ASSERT(offsetof(struct PerspectiveWork, tileIds) == 0x138);
STATIC_ASSERT(offsetof(struct PerspectiveWork, quarterTiles) == 0x338);
STATIC_ASSERT(offsetof(struct PerspectiveWork, farPlane) == 0x348);
STATIC_ASSERT(offsetof(struct PerspectiveWork, zoom) == 0x354);
STATIC_ASSERT(offsetof(struct PerspectiveWork, previousZoom) == 0x350);
STATIC_ASSERT(offsetof(struct PerspectiveWork, zoomTransitionFrames) == 0x358);
STATIC_ASSERT(offsetof(struct PerspectiveWork, zoomTransitionStep) == 0x35a);

#endif

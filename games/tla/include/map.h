#ifndef ALCHEMY_MAP_H
#define ALCHEMY_MAP_H

#include "types.h"
#include "static_assert.h"

struct BlendScriptState {
  u16 *script;
  u16 *cursor;
  u16 delay;
  u16 paused;
};

struct MapAnimation {
  const u16 *start;
  const u16 *cursor;
  u16 timer;
  u16 paused;
};

struct MapCell {
  u16 metatileAndFlags;
  u8 collisionCode;
  u8 attributeB;
};

struct MapLayer {
  s32 x, y;
  u8 unk08[0x28];
  struct MapCell *cells;
  u8 *terrainIds;
};

struct MapWork {
  /// The followed point, read as x, height and z in 16.16 fixed-point pixels.
  s32 *origin;
  /// Screen shake amplitudes and the factor that scales them each frame;
  /// Map_SetScreenShake keeps a component given as negative.
  s32 shakeX, shakeY, shakeDecay;
  u8 *script;
  u16 flags;
  u8 wideTiles;
  u8 unk017;
  struct MapAnimation anim[16];
  struct BlendScriptState blend;
  s32 viewX;
  s32 viewZ;
  s32 minX;
  s32 minZ;
  s32 maxX;
  s32 maxZ;
  u8 unk0fc[0xb]; ///< 0xfc: a pointer to a -1-ended rectangle list
  /// The last BLDCNT the blend script wrote; with mode 1 in bits 7..6 its
  /// values go to BLDALPHA, else to BLDY.
  u8 blendControl;
  struct MapLayer layers[4];
  u8 unk1e8[0x774];
  s32 farPlane;
  s32 zoom;
  s32 zoomFrom;
  s32 zoomTo;
  s16 zoomFrames;
  s16 zoomStep;
};

struct MapViewWork {
  u8 unk000[0x100];
  s8 animationActive;
  u8 unk101[3];
  u16 windowTop;
  u16 windowBottom;
  u16 irqFirstLine;
  u16 irqSecondLine;
  u16 irqPhase;
  u8 unk10e[0x12];
  u32 *resources;
  u8 unk124[0x84c];
  s8 extraTiles;
  u8 pictureShown;
  u8 picturePending;
  s8 area;
};

struct WorldPosition {
  s32 x;
  u8 unk04[4];
  s32 y;
};

s32 GetWorldMapCollision(struct WorldPosition *position);

STATIC_ASSERT(sizeof(struct MapLayer) == 56);
STATIC_ASSERT(offsetof(struct MapWork, layers) == 0x108);

STATIC_ASSERT(sizeof(struct MapAnimation) == 12);
STATIC_ASSERT(offsetof(struct MapWork, anim) == 0x18);
STATIC_ASSERT(offsetof(struct MapWork, blend) == 0xd8);
STATIC_ASSERT(offsetof(struct MapWork, blendControl) == 0x107);
STATIC_ASSERT(offsetof(struct MapWork, minX) == 0xec);
STATIC_ASSERT(offsetof(struct MapWork, farPlane) == 0x95c);
STATIC_ASSERT(offsetof(struct MapWork, zoomStep) == 0x96e);

#endif

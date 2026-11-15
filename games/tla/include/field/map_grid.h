#ifndef ALCHEMY_MAP_GRID_H
#define ALCHEMY_MAP_GRID_H

#include "types.h"
#include "static_assert.h"

/// The map's running blend script, which sets the blend registers over time
/// (BlendScript_Run, each frame from BlendScript_Start).
struct BlendScriptState {
  u16 *script;
  u16 *cursor;
  u16 delay;
  u16 paused;
};

/// One of a map's 16 tile animations (TilesetAnim_StartAll): the start of its
/// script, where it is, the frames left before its next step, and whether it
/// is paused.
struct MapAnimation {
  const u16 *start;
  const u16 *cursor;
  u16 timer;
  u16 paused;
};

/// One cell of a map layer: its cell picture and flags, its collision
/// code and a second attribute byte.
struct MapCell {
  u16 cellPictureAndFlags;
  u8 collisionCode;
  u8 attributeB;
};

/// One background layer's setup in a layout's header: its origin, its
/// parallax and drift, and where it wraps.
struct MapLayer {
  s32 x, y;
  u8 unk08[8];
  s32 unk10;
  s32 unk14;
  s32 unk18;
  s32 unk1c;
  u8 unk20[8];
  u16 unk28;
  u16 unk2a;
  u8 unk2c[4];
  struct MapCell *cells;
  u8 *terrainIds;
};

/// The map grid's work (heap slot HEAP_SLOT_MAP_GRID): the point the view
/// follows, the screen shake, the tile animations and blend script, and the
/// view.
struct MapWork {
  /// The followed point, read as x, height and z in 16.16 fixed-point pixels.
  s32 *origin;
  /// Screen shake amplitudes and the factor that scales them each frame;
  /// MapGrid_SetScreenShake keeps a component given as negative.
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
  /// The camera boxes: a -1-ended list of s16 rectangles (x0, z0, x1, z1 in
  /// pixels) that FieldCamera_FollowLeaderInBounds keeps the view out of;
  /// NULL for none.
  const s16 *cameraBoxes;
  u8 unk100[7];
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

/// A view of the map work as the screen transitions read it: the display
/// control bits, the window edges and interrupt lines of a transition, and
/// the map's resources.
struct MapViewWork {
  u32 unk000[5]; ///< map work fields no window view names
  u16 dispcnt;   ///< display control bits ORed into DISPCNT
  u8 unk016[0xea];
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

/// A position on the world map, as its walkability check reads it.
struct WorldPosition {
  s32 x;
  u8 unk04[4];
  s32 y;
};

s32 WorldMap_GetCollision(struct WorldPosition *position);

void Map_FillCellRectByte2(s32 layer, s32 x, s32 y, u32 width, u32 height,
                           s32 value);

STATIC_ASSERT(sizeof(struct MapLayer) == 56);
STATIC_ASSERT(offsetof(struct MapWork, layers) == 0x108);

STATIC_ASSERT(sizeof(struct MapAnimation) == 12);
STATIC_ASSERT(offsetof(struct MapWork, anim) == 0x18);
STATIC_ASSERT(offsetof(struct MapWork, blend) == 0xd8);
STATIC_ASSERT(offsetof(struct MapWork, blendControl) == 0x107);
STATIC_ASSERT(offsetof(struct MapWork, minX) == 0xec);
STATIC_ASSERT(offsetof(struct MapWork, farPlane) == 0x95c);
STATIC_ASSERT(offsetof(struct MapWork, zoomStep) == 0x96e);
STATIC_ASSERT(offsetof(struct MapViewWork, dispcnt) == 0x14);
STATIC_ASSERT(offsetof(struct MapViewWork, windowTop) == 0x104);
STATIC_ASSERT(offsetof(struct MapViewWork, irqPhase) == 0x10c);

#endif

#ifndef ALCHEMY_MAP_GRID_H
#define ALCHEMY_MAP_GRID_H

#include "fixed_math.h"
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

/// The map grid: 128 by 128 cells, each 16 pixels square, wrapping at its
/// edges.
#define MAPGRID_SIZE 128
#define MAPGRID_CELL_PIXELS 16
/// A layer window's row mask (MapLayer.maskY) over the whole grid, which wraps
/// there rather than at a window of its own.
#define MAPGRID_ROW_MASK ((MAPGRID_SIZE - 1) * MAPGRID_SIZE)
/// The bytes between one layer's BG map and the next, from screen block 5.
#define MAPGRID_LAYER_SCREEN_BYTES 2048

/// A layout's three layers, by what they hold, as The Broken Seal's. Most
/// layouts give them priorities 3, 2 and 1.
enum MapGridLayerIndex {
  MAPGRID_LAYER_GROUND = 0,   ///< `BG3`: the ground, at the back
  MAPGRID_LAYER_MIDDLE = 1,   ///< `BG2`: walls and edges, under the actors
  MAPGRID_LAYER_OVERHEAD = 2, ///< `BG1`: roofs and tree tops, over the actors
};

/// A cell's collision code that blocks whatever the heights, as The Broken
/// Seal's CELL_BEHAVIOR_WALL.
#define CELL_BEHAVIOR_WALL 255

/// One cell of a map layer: its cell picture and flags, its collision
/// code and a second attribute byte.
struct MapCell {
  u16 cellPictureAndFlags;
  u8 collisionCode;
  u8 attributeB;
};

/// A row of gMaps: a layout's resources, each counted from ResourceId_EmptyMap.
struct MapResources {
  u16 header;
  u16 palette;
  u16 tiles[3];       ///< for char blocks 1, 2 and 3
  u16 animationTiles; ///< into gMapGridAnimTiles
};
/// Every layout's resources, by map number (field/map_grid_data.c).
extern const struct MapResources gMaps[];

/// One background layer of the field map: its scroll position, origin,
/// parallax against the view, drift and wrap, and its window of cells.
struct MapLayer {
  FIXED x, y;             ///< 16.16 fixed-point pixels
  FIXED offsetX, offsetY; ///< 16.16 fixed-point pixels
  FIXED scaleX, scaleY;   ///< 16.16 fixed point, FIXED_ONE moves with the view
  FIXED speedX, speedY;   ///< 16.16 fixed-point pixels per frame
  FIXED phaseX, phaseY;   ///< 16.16 fixed-point pixels
  u16 maskX, maskY;       ///< a drifting layer's wrap mask, in 8-pixel tiles
  u16 originX, originY;   ///< the cell column and row of the layer's window
  struct MapCell *cells;  ///< the layer's first cell in the map cell buffer
  u8 *terrainIds;
};

/// The map grid's work (heap slot HEAP_SLOT_MAP_GRID): the point the view
/// follows, the screen shake, the tile animations and blend script, and the
/// view.
struct MapWork {
  /// The followed point, read as x, height and z in 16.16 fixed-point pixels.
  FIXED *origin;
  /// Screen shake amplitudes and the factor that scales them each frame;
  /// MapGrid_SetScreenShake keeps a component given as negative.
  s32 shakeX, shakeY, shakeDecay;
  u8 *script;
  /// Display control bits ORed into DISPCNT; ScreenTransition_HideBackgrounds
  /// clears those of backgrounds 1 to 3.
  u16 dispcnt;
  u8 wideTiles;
  /// The ground's marks: bits 1..0 footprints, 3..2 the kind of wading
  /// (rooms with a second level set 1).
  u8 groundEffects;
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
  /// Set while a BG1 animation runs; the tileset animations are then neither
  /// suspended nor resumed.
  s8 animationActive;
  /// The sprites' base priority; the scrolling camera draws one above it.
  s8 basePriority;
  u8 unusedAfterBasePriority[2];
  /// Each layer's BG priority, written into its BGCNT; zero hides the layer.
  u8 enabled[3];
  /// The last BLDCNT the blend script wrote; with mode 1 in bits 7..6 its
  /// values go to BLDALPHA, else to BLDY.
  u8 blendControl;
  struct MapLayer layers[4];
  u8 unusedAfterLayers[1908]; ///< no code names it
  s32 farPlane;
  s32 zoom;
  s32 zoomFrom;
  s32 zoomTo;
  s16 zoomFrames;
  s16 zoomStep;
};

/// The bytes of MapWork a field map uses: up to its third layer, the last the
/// field scrolls.
#define MAPGRID_FIELD_WORK_SIZE offsetof(struct MapWork, layers[3])

/// A view of the map work as the screen transitions read it: the display
/// control bits, the window edges and interrupt lines of a transition, and
/// the map's resources.
struct MapViewWork {
  u32 unused[5];              ///< map work fields no window view names
  u16 dispcnt;                ///< display control bits ORed into DISPCNT
  u8 unusedAfterDispcnt[234]; ///< no code names it
  s8 animationActive;
  u8 unusedAfterAnimationActive[3]; ///< no code names it
  u16 windowTop;
  u16 windowBottom;
  u16 irqFirstLine;
  u16 irqSecondLine;
  u16 irqPhase;
  u8 unusedAfterIrqPhase[18]; ///< no code names it
  u32 *resources;
  u8 unusedAfterResources[2124]; ///< no code names it
  s8 extraTiles;
  u8 pictureShown;
  u8 picturePending;
  s8 area;
};

/// A position on the world map, as its walkability check reads it.
struct WorldPosition {
  FIXED x;            ///< 16.16 fixed-point pixels
  u8 unusedAfterX[4]; ///< no code names it
  FIXED y;
};

/// The world map's collision values (WorldMap_GetCollision), 1 to 15, as The
/// Broken Seal's, with the sea this game sails.
enum WorldMapCollision {
  /// The ship sails on WORLD_MAP_COLLISION_SEA_FIRST to
  /// WORLD_MAP_COLLISION_SEA_LAST and on shallow water (WorldMap_IsSailable).
  WORLD_MAP_COLLISION_SEA_FIRST = 1,
  WORLD_MAP_COLLISION_SEA_LAST = 4,
  /// The party walks only on WORLD_MAP_COLLISION_WALKABLE_FIRST to
  /// WORLD_MAP_COLLISION_WALKABLE_LAST (WorldMap_IsWalkable).
  WORLD_MAP_COLLISION_WALKABLE_FIRST = 5,
  /// Shallow water: a ripple follows the leader as it walks, and the ship
  /// sails there too.
  WORLD_MAP_COLLISION_SHALLOW_WATER = 6,
  /// Where neither collision layer has a value.
  WORLD_MAP_COLLISION_DEFAULT = 7,
  /// Forest: the leader's sprite shows its second layer and no shadow.
  WORLD_MAP_COLLISION_FOREST = 9,
  WORLD_MAP_COLLISION_WALKABLE_LAST = 12
};

s32 WorldMap_GetCollision(struct WorldPosition *position);

void PushedActor_FillGridAttributeRectangle(s32 layer, s32 x, s32 y, u32 width,
                                            u32 height, s32 value);

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

/// A cell animation step's source column that ends the steps.
#define MAPGRID_ANIMATION_STEPS_END 0xffff

/// A cell of the map's shape grid as the raising and lowering of cells store
/// it; only so the shape store has an alias set of its own, the grid holds
/// bytes.
struct ShapeCell {
  u8 shape;
};

s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);

#endif

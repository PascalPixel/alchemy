#ifndef ALCHEMY_FIELD_MAP_H
#define ALCHEMY_FIELD_MAP_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "field/map_grid.h"

struct Actor;

struct ReflectionSprite {
  u8 unused[2]; ///< no code names it
  u16 unk02;
  u8 unusedB[12]; ///< no code names it
  s32 unk10;
  u8 unk14;
};

struct ReflectionAnimation {
  u8 unused[0x28]; ///< no code names it
  struct ReflectionSprite *sprite;
};

struct WorldMapBlock {
  u8 unused[2];        ///< no code names it
  u8 place;            ///< WORLD_MAP_NO_PLACE where the map has no block
  u8 unusedAfterPlace; ///< the terrain kind and flags
                       ///< (WorldMap_GetBattleEnvironment)
};

/// The world map's blocks, a row of 32 for each z, in the cell picture buffer
/// (gCellPictures) while the world map is shown.
struct WorldMapBlocks {
  struct WorldMapBlock blocks[32 * 32];
};

/// WorldMapBlock.place where the world map has no block: nothing may go there.
#define WORLD_MAP_NO_PLACE 255

struct WorldMover {
  u8 unused[12]; ///< no code names it
  s32 height;
};

struct ScanlineRow {
  s32 scale;
  s32 depth;
  s32 offsetX;
  s32 offsetY;
  s32 unusedAfterOffsetY; ///< no code names it
};

struct SpriteViewerLayer {
  s16 spriteId;
  s16 animationIndex;
  u8 enabled;
  s8 priority;
  s8 colorMode;
};

struct ViewerSprite {
  u8 unused[0x1a]; ///< no code names it
  u8 flags;
  u8 unusedAfterFlags[0x1d]; ///< no code names it
};

struct ProbeShape {
  u8 unused[4]; ///< no code names it
  u8 kind;
};

struct ProbeObjects {
  u8 unused[40]; ///< no code names it
  struct ProbeShape *shape;
};

struct Vec2 {
  s32 x;
  s32 y;
};

struct Actor *Actor_FindAtCell(s32 *position, s32 check_y);

extern u8 gBgTileBuffer[];
/// The world map area's four tile sets, decoded one after another into
/// gBgTileBuffer and copied to the two halves of BG char blocks 2 and 3, as
/// The Broken Seal's.
#define WORLD_MAP_TILE_SET_BYTES 0x2000
#define WORLD_MAP_TILE_SET(n)                                                  \
  (Ram_BgTileBuffer + (n) * WORLD_MAP_TILE_SET_BYTES)
extern u8 gMapGridShapes[];

void WorldMap_UpdateRegionTint(s32 region, s32 gradual);

s32 ScreenTransition_IsScanlineActiveFar(void);

extern u8 gDecodeBuffer[];

void TilesetAnim_Update(void);

s32 MapCollision_FindOrAddTagFar(s32 layer, s32 unused, s32 value);

s32 Actor_LiftTargetBlockFar(s32 layer, s32 unused, s32 value);

extern const u8 Render_DrawMapCode[];
extern u8 Render_DrawMapCodeSize[];

s32 SpriteViewer_FindNextSpriteWithPictures(s32 spriteId, s32 direction);
void SpriteViewer_PlaceRing(void);
extern struct BattleCells gSpriteObjects;

void BlendScript_Start(u16 *script);
s32 MapGrid_CheckCollisionAt(FIXED px, FIXED pz, s32 layer, s32 height);
void WorldMap_PreloadCurrentTileBlock(void);
void BlendScript_Resume(void);
void BlendScript_Suspend(void);

#endif

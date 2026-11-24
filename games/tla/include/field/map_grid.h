#ifndef ALCHEMY_MAP_GRID_H
#define ALCHEMY_MAP_GRID_H

/// @file
/// The map grid: its layers and cells, the cell behaviors and attributes the
/// field reads, its tile animations and blend script, and the world map's
/// collision and battle environments.

#include "field/field_coordinate.h"
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

/// The map grid: 128 by 128 cells, each MAPGRID_CELL_PIXELS square, wrapping
/// at its edges.
#define MAPGRID_SIZE 128
/// A layer window's row mask (MapLayer.maskY) over the whole grid, which wraps
/// there rather than at a window of its own.
#define MAPGRID_ROW_MASK ((MAPGRID_SIZE - 1) * MAPGRID_SIZE)
/// The bytes between one layer's BG map and the next, from screen block 5.
#define MAPGRID_LAYER_SCREEN_BYTES 2048
/// The low four bits of a shape's first byte: its kind (enum TerrainKind),
/// which height function reads its samples (gMapGrid_TerrainHeightFunctions).
#define MAPGRID_SHAPE_KIND_MASK 0xf
/// A 16.16 height in the 8-pixel steps MapGrid_SetCellHeight and the collision
/// tags take.
#define CELL_HEIGHT_STEPS(height) ((height) >> 19)
/// The collision code in MapCell_GetAttributes's halfword, above its
/// attribute byte.
#define CELL_CODE(attributes) ((attributes) >> 8)
/// MapCell_GetAttributes's halfword: the collision code over the second
/// attribute byte (MapCell.attributeB), and the halfword built from the two.
#define CELL_CODE_BITS 0xff00
#define CELL_ATTRIBUTE_BITS 0xff
#define CELL_ATTRIBUTES(code, bits) (((code) << 8) | (bits))

/// Where a layout's blend script is decoded, in the map collision buffer after
/// the collision and the tileset animations; a scene may copy its own there.
#define MAPGRID_BLEND_SCRIPT (Ram_MapCollision + 7680)
/// A blend script's commands besides its BLDCNT values: the end, which starts
/// it over, and a jump, its low byte the halfword pair to go on from.
#define BLEND_SCRIPT_LOOP 65535
#define BLEND_SCRIPT_JUMP 0xfe00

/// A layout's three layers, by what they hold. Most layouts give them
/// priorities 3, 2 and 1.
enum MapGridLayerIndex {
  MAPGRID_LAYER_GROUND = 0,   ///< `BG3`: the ground, at the back
  MAPGRID_LAYER_MIDDLE = 1,   ///< `BG2`: walls and edges, under the actors
  MAPGRID_LAYER_OVERHEAD = 2, ///< `BG1`: roofs and tree tops, over the actors
};

/// A cell's collision code: the codes the field code reads by name. Cloak,
/// Sand, the Cyclone and Hover read theirs on the overhead layer
/// (event_commands_field_psynergy.c).
enum CellBehavior {
  CELL_BEHAVIOR_NORMAL = 0, ///< open ground, where nothing happens
  /// 1 to 230: the scene's own, each the step trigger it raises as the
  /// leader walks onto it (Field_UpdateWalking), or as he climbs a wall drawn
  /// over it on the overhead layer (GaiaRock_TriggerWallCell); those above are
  /// the field's own.
  CELL_BEHAVIOR_EVENT_FIRST = 1,
  CELL_BEHAVIOR_EVENT_LAST = 230,
  CELL_BEHAVIOR_CLOAK = 231, ///< Cloak hides the leader there
  CELL_BEHAVIOR_SAND = 232,  ///< Sand lets the leader move there as sand
  /// A cell the Cyclone acts on (MapGrid_GetCycloneCellKind's kind 1).
  CELL_BEHAVIOR_CYCLONE = 233,
  /// A cell the Cyclone carries the leader to, its centre kept in
  /// FieldState.cycloneCarryX and Z (kind 2).
  CELL_BEHAVIOR_CYCLONE_CARRY = 234,
  /// A cell the Cyclone lifts the leader onto, its centre kept in
  /// FieldState.cycloneLiftX and Z (kind 3).
  CELL_BEHAVIOR_CYCLONE_LIFT = 235,
  CELL_BEHAVIOR_HOVER = 236, ///< Hover holds the leader up over it
  /// 242 to 247: examine triggers with lines of their own, the same on every
  /// map (Field_ExamineCell, gField_ExamineTriggerMessageOffsets).
  CELL_BEHAVIOR_JAR = 242,
  CELL_BEHAVIOR_BARREL = 243,
  CELL_BEHAVIOR_WOODEN_BOX = 244,
  CELL_BEHAVIOR_GRAVE = 245,
  CELL_BEHAVIOR_BOOKS = 246,
  CELL_BEHAVIOR_WELL = 247,
  CELL_BEHAVIOR_JUMP_FORWARD = 254, ///< jumped across
  CELL_BEHAVIOR_WALL = 255          ///< blocks whatever the heights
};

/// MapCell.attributeB's bits.
enum MapCellAttribute {
  /// Scoop digs here, or everywhere else where the scene's attributes turn it
  /// round (FieldState.sceneAttributeLow, MapGrid_CanScoop).
  MAP_CELL_SCOOP = 0x01,
  /// Pushing against the cell pushes the actor ahead (Field_RunPushCell).
  MAP_CELL_PUSH = 0x02,
  /// Random battles here roll on the scene's second encounter zone
  /// (Field_UpdateWalking).
  MAP_CELL_SECOND_ZONE = 0x04,
  /// Hazardous ground; with HAZARD_COUNT_ON_CELLS the hazard count rises by
  /// the pixels walked on it.
  MAP_CELL_HAZARD = 0x08,
  MAP_CELL_WATER = 0x10, ///< water (MapGrid_IsWaterCell)
  MAP_CELL_LEDGE = 0x20, ///< a ledge, jumped down by pushing KEY_DOWN
  /// A wall the leader can climb (MapGrid_IsClimbableCell, Actor_MoveByKeys).
  MAP_CELL_CLIMBABLE = 0x40,
  MAP_CELL_JUMP = 0x80 ///< a gap the leader jumps across, pushing forward
};

/// One cell of a map layer: its cell tiles and flags, its collision
/// code and a second attribute byte.
struct MapCell {
  u16 picture : 12;      ///< its cell tiles
  u16 actorLayer : 2;    ///< the map layer an actor here takes, plus one
  u16 actorPriority : 2; ///< the sprite priority an actor here is drawn at
  u8 collisionCode;
  u8 attributeB;
};

/// A struct MapCell read as one word, as code that ORs bits into a cell writes
/// it: its collision code (the cell behavior), the attribute bits above it and
/// the flags of its cell tiles.
#define MAP_CELL_WORD(behavior, attributes, pictureFlags)                      \
  ((u32)(attributes) << 24 | (behavior) << 16 | (pictureFlags))
/// The flags of a cell's tiles for an actor standing on the cell: the sprite
/// priority it is drawn at and the map layer it takes, which the actors'
/// placement reads (MapCell's actorPriority and actorLayer).
#define CELL_DRAWS_ACTOR(priority, layer)                                      \
  ((priority) << 14 | ((layer) + 1) << 12)

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
  /// The window's wrap masks over its cell columns and rows, maskY's a row
  /// times MAPGRID_SIZE: MAPGRID_SIZE - 1 and MAPGRID_ROW_MASK for the whole
  /// grid.
  u16 maskX, maskY;
  u16 originX, originY;  ///< the cell column and row of the layer's window
  struct MapCell *cells; ///< the layer's first cell in the map cell buffer
  u8 *terrainIds;
};

/// The map grid's state (heap slot HEAP_SLOT_MAP_GRID): the point the view
/// follows, the screen shake, the tile animations and blend script, and the
/// view.
struct MapState {
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

/// The bytes of MapState a field map uses: up to its third layer, the last the
/// field scrolls.
#define MAPGRID_FIELD_STATE_SIZE offsetof(struct MapState, layers[3])

/// MapState.groundEffects' kind of wading, its bits 3..2; none where nothing
/// wades.
#define GROUND_EFFECTS_WADING_MASK (3 << 2)
#define GROUND_EFFECTS_WADING(effects) ((u32)((effects) << 28) >> 30)

/// A view of the map state as the screen transitions read it: the display
/// control bits, the window edges and interrupt lines of a transition, and
/// the map's resources.
struct MapViewState {
  u32 unused[5];              ///< map state fields no window view names
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

/// The world map's collision values (WorldMap_GetCollision), 1 to 15, the sea
/// the ship sails among them.
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

/// A world map battle environment (gWorldMap_TerrainBehaviorTable,
/// WorldMap_GetBattleEnvironment): what its ground means for battles, named by
/// the battle background BattleBackground_SelectForEnvironment gives it.
enum BattleEnvironment {
  BATTLE_ENVIRONMENT_NONE = 0,
  BATTLE_ENVIRONMENT_GRASSLAND = 1, ///< ResourceId_GrasslandBBackdrop
  /// ResourceId_DenseForestBackdrop; encounters take a region's forest zone.
  BATTLE_ENVIRONMENT_FOREST = 2,
  BATTLE_ENVIRONMENT_DESERT = 3,    ///< ResourceId_DesertBackdrop
  BATTLE_ENVIRONMENT_BEACH = 4,     ///< ResourceId_RockyBeachBackdrop
  BATTLE_ENVIRONMENT_SNOWFIELD = 5, ///< ResourceId_SnowfieldBBackdrop
  /// ResourceId_SnowfieldBBackdrop, for the snowfield's forest.
  BATTLE_ENVIRONMENT_SNOW_FOREST = 6,
  /// ResourceId_RockyBeachBackdrop, for shallow water
  /// (WORLD_MAP_COLLISION_SHALLOW_WATER).
  BATTLE_ENVIRONMENT_SHALLOWS = 7,
  /// The sea the ship sails, collisions 1 to 3 outside the snowfields, where
  /// its encounters are (gWorldMap_ShipEncounterRules);
  /// ResourceId_ShipDeckBackdrop.
  BATTLE_ENVIRONMENT_SEA = 8,
  /// All the sea of the snowfield regions, where no rule gives encounters.
  BATTLE_ENVIRONMENT_SNOWFIELD_SEA = 9,
  /// WORLD_MAP_COLLISION_SEA_LAST outside the snowfields, where no rule gives
  /// encounters; a battle there would take the ship deck's backdrop.
  // XXX: which waters BATTLE_ENVIRONMENT_SEA_LAST covers; the World Map cells
  // whose collision is WORLD_MAP_COLLISION_SEA_LAST outside the snowfield
  // regions, read from its collision tiles (WorldMap_GetCollision), would
  // settle it.
  BATTLE_ENVIRONMENT_SEA_LAST = 10,
  BATTLE_ENVIRONMENT_BLIZZARD = 11, ///< ResourceId_BlizzardBackdrop
  /// ResourceId_BlizzardBackdrop, for the blizzard's forest.
  BATTLE_ENVIRONMENT_BLIZZARD_FOREST = 12
};

s32 WorldMap_GetCollision(struct WorldPosition *position);

void MapGrid_FillCollisionRectangle(s32 layer, s32 x, s32 y, u32 width,
                                    u32 height, s32 value);

STATIC_ASSERT(sizeof(struct MapLayer) == 56);
STATIC_ASSERT(offsetof(struct MapState, layers) == 264);

STATIC_ASSERT(sizeof(struct MapAnimation) == 12);
STATIC_ASSERT(offsetof(struct MapState, anim) == 24);
STATIC_ASSERT(offsetof(struct MapState, blend) == 216);
STATIC_ASSERT(offsetof(struct MapState, blendControl) == 263);
STATIC_ASSERT(offsetof(struct MapState, minX) == 236);
STATIC_ASSERT(offsetof(struct MapState, farPlane) == 2396);
STATIC_ASSERT(offsetof(struct MapState, zoomStep) == 2414);
STATIC_ASSERT(offsetof(struct MapViewState, dispcnt) == 20);
STATIC_ASSERT(offsetof(struct MapViewState, windowTop) == 260);
STATIC_ASSERT(offsetof(struct MapViewState, irqPhase) == 268);

/// A cell animation step's source column that ends the steps.
#define MAPGRID_ANIMATION_STEPS_END 65535

/// A cell of the map's shape grid: one byte, the tag of the cell's collision
/// shape.
struct ShapeCell {
  u8 shape;
} __attribute__((packed));

/// The ground height of a layer at (x, z), 16.16 pixels
/// (MapGrid_GetTerrainHeight, through the overlay's own import row).
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);

#endif

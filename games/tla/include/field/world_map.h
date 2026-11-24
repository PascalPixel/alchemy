#ifndef ALCHEMY_WORLD_MAP_H
#define ALCHEMY_WORLD_MAP_H

/// @file
/// The map screen's state and the destinations it marks.

#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "field/world_map_marker.h"

/// gWorldMapState, the map screen's state while it is open: its
/// resources, the cursor and the marker sprites.
struct WorldMapState {
  u16 vramBlock;      ///< owns the marker tiles
  u16 windowSlot;     ///< the resource slot of the name window's tiles
  FIXED x;            ///< the cursor's screen column, 16.16 pixels
  FIXED z;            ///< the cursor's screen row, 16.16 pixels
  u8 unusedAfterZ[6]; ///< no code names them
  s16 shown;          ///< the WorldMapPlace whose name is drawn; -1 for none
  u8 unusedAfterShown[4]; ///< no code names them
  FIXED speed;            ///< the cursor's speed, 16.16 pixels per frame
  void *window;           ///< the name window
  struct WorldMapMarker markers[129]; ///< the marker sprites
  s32 *cursor;                        ///< the cursor the picture scrolls to
  FIXED scrollX;                      ///< the picture's scroll, 16.16 pixels
  s32 scrollZ;
  /// The box of the place name the map screen shows, in pixels.
  struct {
    s16 x;
    s16 y;
    s16 width;
    s16 height;
  } nameBox;
  /// Nonzero draws the whole world (RESOURCE_WORLD_MAP_PICTURE) at the
  /// origin rather than about the cursor (MapScreen_Draw).
  s16 centered;
  u16 mode; ///< 0 to look, nonzero to choose a destination
  /// Where the nearest-place search looks from in place of the cursor, 16.16
  /// pixels, when nonzero; the field code only ever clears it.
  FIXED searchX;
  FIXED searchZ;
};

STATIC_ASSERT(offsetof(struct WorldMapState, window) == 28);
STATIC_ASSERT(offsetof(struct WorldMapState, cursor) == 1580);
STATIC_ASSERT(offsetof(struct WorldMapState, mode) == 1602);
STATIC_ASSERT(offsetof(struct WorldMapState, searchZ) == 1608);

/// The game flag that marks a World Map place visited, from its
/// WorldMapPlace.flag. The first of those flags, a place's WorldMapPlace.flag
/// being added to it.
#define FLAG_PLACE_VISITED_FIRST 0x180
#define FLAG_PLACE_VISITED(place) ((place) + FLAG_PLACE_VISITED_FIRST)
/// WorldMapPlace.value's markers, besides the marker pictures 1 to 14.
enum WorldMapMarkerKind {
  WORLD_MAP_MARKER_NONE = 0, ///< the place is not drawn
  /// The ship's marker, drawn at the ship once it sails and the party is
  /// ashore.
  WORLD_MAP_MARKER_SHIP = 15
};

/// A world map destination, keyed by its flag (WorldMap_MarkPlaceVisited sets
/// FLAG_PLACE_VISITED(flag)); WorldMapPlaces_GetTable returns them, ended by a
/// zero flag.
struct WorldMapPlace {
  u8 flag;
  u8 value; ///< its markers: low nibble, high nibble on the map screen
  /// The actor the World Map places there (WorldMap_UpdatePlacedActors): its
  /// sprite, place in pixels and facing, and the flag it is shown by (-1
  /// always; bit 12 set: shown while the rest is set, else while clear).
  ///@{
  s16 sprite;
  s16 x;
  s16 z;
  u16 facing;
  s16 shownFlag;
  ///@}
  /// The scene and entrance whose arrival marks it visited
  /// (WorldMap_MarkPlaceVisited), and whose area name the map screen shows
  /// for it; an entrance of -1 matches any.
  s16 visitScene;
  s16 visitEntrance;
  s16 scene;    ///< where choosing it leads
  s16 entrance; ///< that scene's entrance
};

STATIC_ASSERT(offsetof(struct WorldMapPlace, visitScene) == 12);
STATIC_ASSERT(offsetof(struct WorldMapPlace, scene) == 16);
STATIC_ASSERT(sizeof(struct WorldMapPlace) == 20);

/// gMainRoutineState.mapScreenFlag under a label of its own,
/// gMapScreenChoice: the destination last chosen on the map screen. Written
/// through the member, MapScreen_Show compiles otherwise.
struct WorldMapChoice {
  s16 flag; ///< the chosen WorldMapPlace's flag; -1 when cancelled
};

extern struct WorldMapState gWorldMapState;
/// The destination last chosen on the world map (struct WorldMapChoice).
extern struct WorldMapChoice gMapScreenChoice;
const struct WorldMapPlace *WorldMapPlaces_GetTable(void);

/// The most places WorldMapPlaces_GetTable lists.
#define MAP_SCREEN_PLACE_COUNT 128
/// The step trigger the actor of gWorldMap_PlacedActors' first place raises
/// when touched; each next place's actor raises the next.
#define STEP_TRIGGER_AREA_BASE 100
/// BG1's map screen image, of 256 colours: 32 by 24 tiles from char block 1 on,
/// which it scrolls through, wrapping, its map numbering them from the fifth
/// row, where the still image starts.
#define MAP_SCREEN_FIRST_TILE (BG_CHAR_SIZE / TILE_SIZE_8BPP)
#define MAP_SCREEN_TILE_ROWS 24
#define MAP_SCREEN_MAP_START (4 * BG_SCREEN_WIDTH)

/// A World Map position, in whole pixels, as a point on the map
/// screen.
#define MAP_SCREEN_X(x) ((x) * 853 / 16384 - 269)
#define MAP_SCREEN_Y(z) ((z) * 640 / 16384 - 112)

/// The depth the map screen's markers and cursor are drawn at.
#define MAP_SCREEN_MARKER_DEPTH 246
u32 WorldMapPlace_GetMarker(const struct WorldMapPlace *place);

/// WorldMap_UpdateAffineLines' Thumb entry and ARM code
/// (world_map_lines_entry.s), copied to RAM to run.
extern const u8 WorldMap_UpdateAffineLinesCode[];

#endif

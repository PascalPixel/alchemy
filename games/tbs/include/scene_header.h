#ifndef ALCHEMY_SCENE_HEADER_H
#define ALCHEMY_SCENE_HEADER_H

/// @file
/// Scene headers, warp regions and scene entry tables, with the field work they
/// fill (field_work.h).

#include "types.h"
#include "static_assert.h"
#include "object.h"
#include "map_tables.h"
#include "field_work.h"

struct MapGridCell;
struct ObjectEvent;

/// A scene's row in gSceneHeaders: its overlay's resource, its area,
/// its scene type and its map. One row per scene number; field/scene_headers.s
/// writes them, and its SceneId_ symbols are the row numbers.
struct SceneHeader {
  s16 resourceId; ///< the overlay Overlay_Load loads into gOverlayArea
  /// The area: entering a new one clears the FLAG_AREA_TEMP_xx flags
  /// and the step counters and moves Retreat's destination;
  /// FLAG_AREA_VISITED(area) marks it visited (Scene_ResetFlagsOnEnter).
  s8 area;
  /// An enum SceneType, copied to gSaveBlock.sceneType:
  /// SCENE_TYPE_TOWN for towns and their interiors (53 rows: Vale,
  /// Bilibin, Kalay, Tolbi and others), SCENE_TYPE_FIELD for the rest.
  s8 sceneType;
  u16 mapIndex; ///< the map loaded for it, copied to gSaveBlock.mapId
  u16 reserved; ///< 0 in every row; not read
};

STATIC_ASSERT(sizeof(struct SceneHeader) == 8);

/// A scene's warp region: the box that, while its condition holds, takes the
/// party leader out through its warp.
struct MapWarpRegion {
  s16 minX, minY, minZ; ///< pixels, inclusive; minX -1 ends the table
  s16 maxX, maxY, maxZ; ///< pixels, exclusive
  s16 condition;        ///< -1 always, a flag | 0x1000 set, else the flag clear
  s16 value;            ///< the warp kind, stored in FieldWork's warp
};

STATIC_ASSERT(sizeof(struct MapWarpRegion) == 16);

/// The entry table at the start of each scene overlay: its initialize
/// function and the functions returning its warp destinations, warps, object
/// event templates, map events and warp regions, each behind a veneer word.
/// The field reads each entry's veneer as the address word in its last four
/// bytes, so member i of the six sits at 8 * i + 4; the field calls them as
/// gOverlayArea.initialize(), gOverlayArea.events() and so on.
struct SceneEntries {
  u32 initializeVeneer;
  s32 (*initialize)(void);
  u32 warpDestsVeneer;
  const struct MapWarpDest *(*warpDests)(void);
  u32 warpsVeneer;
  const u32 *(*warps)(void);
  u32 objectEventTemplatesVeneer;
  const struct ObjectEventTemplate *(*objectEventTemplates)(void);
  u32 eventsVeneer;
  const struct MapEvent *(*events)(void);
  u32 regionsVeneer;
  const struct MapWarpRegion *(*regions)(void);
};

STATIC_ASSERT(sizeof(struct SceneEntries) == 48);
STATIC_ASSERT(offsetof(struct SceneEntries, warps) == 0x14);
STATIC_ASSERT(offsetof(struct SceneEntries, regions) == 0x2c);

/// A position on the field.
struct FieldPosition {
  s32 x, y, z; ///< 16.16 fixed-point pixels
};

STATIC_ASSERT(sizeof(struct FieldPosition) == 12);

/// Every scene's header, by scene number (field/scene_headers.s).
extern const struct SceneHeader gSceneHeaders[];
/// The running scene's overlay, loaded into these 0x8000 bytes of EWRAM at
/// each scene change; its code opens with its 48-byte entry table. Every
/// overlay is linked to run at this one fixed address (0x02008000).
extern struct SceneEntries gOverlayArea;

#endif

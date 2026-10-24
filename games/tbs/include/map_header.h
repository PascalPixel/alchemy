#ifndef ALCHEMY_MAP_HEADER_H
#define ALCHEMY_MAP_HEADER_H

/// @file
/// Map headers, warp regions and map entry tables, with the field work they
/// fill (field_work.h).

#include "types.h"
#include "static_assert.h"
#include "object.h"
#include "map_tables.h"
#include "field_work.h"

struct MapGridCell;
struct ObjectEvent;

/// A map's row in gMapHeaders: its overlay's resource, its map section,
/// its map type and its layout. One row per map number; field/map_headers.s
/// writes them, and its MapId_ symbols are the row numbers.
struct MapHeader {
  s16 resourceId; ///< the overlay Overlay_Load loads into gOverlayArea
  /// The map section: entering a new one clears the FLAG_MAPSEC_TEMP_xx flags
  /// and the step counters and moves Retreat's destination;
  /// FLAG_MAPSEC_VISITED(mapSec) marks it visited (Map_ResetFlagsOnEnter).
  s8 mapSec;
  /// An enum MapType, copied to gSaveBlock.mapType:
  /// MAP_TYPE_TOWN for towns and their interiors (53 rows: Vale,
  /// Bilibin, Kalay, Tolbi and others), MAP_TYPE_FIELD for the rest.
  s8 mapType;
  u16 layoutIndex; ///< the layout loaded for it, copied to gSaveBlock.layoutId
  u16 reserved;    ///< 0 in every row; not read
};

STATIC_ASSERT(sizeof(struct MapHeader) == 8);

/// A map's warp region: the box that, while its condition holds, takes the
/// party leader out through its warp.
struct MapWarpRegion {
  s16 minX, minY, minZ; ///< pixels, inclusive; minX -1 ends the table
  s16 maxX, maxY, maxZ; ///< pixels, exclusive
  s16 condition;        ///< -1 always, a flag | 0x1000 set, else the flag clear
  s16 value;            ///< the warp kind, stored in FieldWork's warp
};

STATIC_ASSERT(sizeof(struct MapWarpRegion) == 16);

/// The entry table at the start of each map overlay: its initialize
/// function and the functions returning its warp destinations, warps, object
/// event templates, map events and warp regions, each behind a veneer word.
/// The field reads each entry's veneer as the address word in its last four
/// bytes, so member i of the six sits at 8 * i + 4; the field calls them as
/// gOverlayArea.initialize(), gOverlayArea.events() and so on.
struct MapEntries {
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

STATIC_ASSERT(sizeof(struct MapEntries) == 48);
STATIC_ASSERT(offsetof(struct MapEntries, warps) == 0x14);
STATIC_ASSERT(offsetof(struct MapEntries, regions) == 0x2c);

/// A position on the field.
struct FieldPosition {
  s32 x, y, z; ///< 16.16 fixed-point pixels
};

STATIC_ASSERT(sizeof(struct FieldPosition) == 12);

/// Every map's header, by map number (field/map_headers.s).
extern const struct MapHeader gMapHeaders[];
/// The running map's overlay, loaded into these 0x8000 bytes of EWRAM at
/// each map change; its code opens with its 48-byte entry table. Every overlay
/// is linked to run at this one fixed address (0x02008000).
extern struct MapEntries gOverlayArea;

#endif

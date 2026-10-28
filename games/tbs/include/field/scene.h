#ifndef ALCHEMY_SCENE_H
#define ALCHEMY_SCENE_H

/// @file
/// Scenes, exit regions and scene entry tables, with the field work they
/// fill (field_work.h).

#include "types.h"
#include "static_assert.h"
#include "actor.h"
#include "scene_tables.h"
#include "field_work.h"

struct MapGridCell;
struct Actor;

/// A scene's row in gScenes: its overlay's resource, its area,
/// its scene type and its map. One row per scene number; field/scenes.s
/// writes them, and its SceneId_ symbols are the row numbers.
struct Scene {
  s16 resourceId; ///< the overlay Overlay_Load loads into gOverlayArea
  /// The area: entering a new one clears the FLAG_AREA_TEMP_xx flags
  /// and the step counters and moves Retreat's destination;
  /// FLAG_AREA_VISITED(area) marks it visited (Scene_Arrive).
  s8 area;
  /// An enum SceneType, copied to gMainRoutineWork.sceneType:
  /// SCENE_TYPE_TOWN for towns and their interiors (53 rows: Vale,
  /// Bilibin, Kalay, Tolbi and others), SCENE_TYPE_OTHER for the rest.
  s8 sceneType;
  u16 mapIndex; ///< the map loaded for it, copied to gMainRoutineWork.mapId
  u16 reserved; ///< 0 in every row; not read
};

STATIC_ASSERT(sizeof(struct Scene) == 8);

/// A scene's exit region: the box that, while its condition holds, takes the
/// party leader out through its exit.
struct SceneExitRegion {
  s16 minX, minY, minZ; ///< pixels, inclusive; minX -1 ends the table
  s16 maxX, maxY, maxZ; ///< pixels, exclusive
  s16 condition; ///< CONDITION_ALWAYS, a flag | CONDITION_FLAG_SET set, else
                 ///< the flag clear
  s16 value;     ///< the exit kind, stored in FieldWork's exit
};

STATIC_ASSERT(sizeof(struct SceneExitRegion) == 16);

/// The entry table at the start of each scene overlay: its initialize
/// function and the functions returning its entrances, exits,
/// actor placements, triggers and exit regions, each behind a veneer word.
/// The field reads each entry's veneer as the address word in its last four
/// bytes, so member i of the six sits at 8 * i + 4; the field calls them as
/// gOverlayArea.initialize(), gOverlayArea.events() and so on.
struct SceneEntryPoints {
  u32 initializeVeneer;
  s32 (*initialize)(void);
  u32 entrancesVeneer;
  const struct SceneEntrance *(*entrances)(void);
  u32 exitsVeneer;
  const u32 *(*exits)(void);
  u32 actorPlacementsVeneer;
  const struct ActorPlacement *(*actorPlacements)(void);
  u32 triggersVeneer;
  const struct Trigger *(*triggers)(void);
  u32 regionsVeneer;
  const struct SceneExitRegion *(*regions)(void);
};

STATIC_ASSERT(sizeof(struct SceneEntryPoints) == 48);
STATIC_ASSERT(offsetof(struct SceneEntryPoints, exits) == 0x14);
STATIC_ASSERT(offsetof(struct SceneEntryPoints, regions) == 0x2c);

/// A position on the field.
struct FieldPosition {
  s32 x, y, z; ///< 16.16 fixed-point pixels
};

STATIC_ASSERT(sizeof(struct FieldPosition) == 12);

/// Every scene's row, by scene number (field/scenes.s).
extern const struct Scene gScenes[];
/// The running scene's overlay, loaded into these 0x8000 bytes of EWRAM at
/// each scene change; its code opens with its 48-byte entry table. Every
/// overlay is linked to run at this one fixed address (0x02008000).
extern struct SceneEntryPoints gOverlayArea;

#endif

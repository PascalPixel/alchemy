#ifndef ALCHEMY_SCENE_TABLES_H
#define ALCHEMY_SCENE_TABLES_H

/// @file
/// The scene table formats: the scene table, entrances, exits, actor
/// placements and triggers, as The Broken Seal's scene_tables.h has them.

#include "types.h"
#include "main_routine_work.h"

struct Scene {
  s16 resourceId;
  s8 group;
  s8 variant;
  u16 mapIndex;
  s16 attributes;
};

extern const struct Scene gScenes[];

enum Facing {
  FACING_EAST = 0x0000,
  FACING_SOUTHEAST = 0x2000,
  FACING_SOUTH = 0x4000,
  FACING_SOUTHWEST = 0x6000,
  FACING_WEST = 0x8000,
  FACING_NORTHWEST = 0xa000,
  FACING_NORTH = 0xc000,
  FACING_NORTHEAST = 0xe000
};

enum { SCENE_TABLE_END = -1, CONDITION_ALWAYS = -1 };

/// Entrance positions.
struct SceneEntrance {
  s16 entrance;     ///< the entrance number; SCENE_TABLE_END ends the table
  s16 requiredFlag; ///< a flag that must be set, or -1 for none
  s16 x;            ///< pixels
  s16 y;            ///< pixels
  s16 z;            ///< pixels
  u16 facing;       ///< 1/65536 turns (enum Facing)
  s16 unused1;
  s16 cameraLeft;   ///< pixels, -1 for none
  s16 cameraTop;    ///< pixels, -1 for none
  s16 cameraRight;  ///< pixels, -1 for none
  s16 cameraBottom; ///< pixels, -1 for none
  s16 unused2;
};

/// Exits: the current scene number, followed by each exit's destination.
#define SCENE_EXITS(scene) ((u32)(scene))
#define SCENE_EXIT(exit, scene, entrance)                                      \
  ((u32)(exit) << 20 | (u32)(entrance) << 12 | (u32)(scene))
#define SCENE_EXIT_IF(exit, scene, entrance, flag)                             \
  (SCENE_EXIT(exit, scene, entrance) | 0x10000000), (u32)(flag)
#define SCENE_EXITS_END 0x1ff

/// Actor placements.
struct ActorPlacement {
  s16 sprite; ///< the actor's sprite; -1 ends the table
  s16 condition; ///< CONDITION_ALWAYS (-1), or a flag condition
  s32 behavior;
  s32 x;      ///< 16.16 fixed-point pixels
  s32 y;      ///< 16.16 fixed-point pixels
  s32 z;      ///< 16.16 fixed-point pixels
  u16 facing; ///< 1/65536 turns (enum Facing)
  u8 talkFacing;
  u8 flags;
};

#define PIXELS(value) ((value) << 16)

/// A row of a scene's trigger table.
struct Trigger {
  /// The kind and its detail; TRIGGERS_END ends the table. Item triggers
  /// read it as bits: 15..8 item | 4 on an actor | 3..0 kind (4)
  u32 control;
  /// What the trigger answers: an actor id or a tile code. Item triggers read it
  /// as bits: 15..12 facing | 11 needs facing | 7..0 target
  s16 subject;
  s16 condition; ///< CONDITION_ALWAYS (-1), or a flag condition
  u32 value;     ///< a message (FIND_MESSAGE) or an event (EVENT)
};

#define FIND_MESSAGE(message) (0x00400000 | (message))
#define EVENT(event) ((u32)(event))
#define TRIGGERS_END 0xffffffff

struct SceneRegion;

/// The table a scene overlay starts with (gOverlayArea): a veneer word and an
/// entry point for each of its tables.
// As ☀️'s.
struct SceneEntries {
  u32 initializeVeneer;
  s32 (*initialize)(void);
  u32 entrancesVeneer;
  const struct SceneEntrance *(*entrances)(void);
  u32 exitsVeneer;
  const u32 *(*exits)(void);
  u32 placementsVeneer;
  const struct ActorPlacement *(*placements)(void);
  u32 triggersVeneer;
  const struct Trigger *(*triggers)(void);
  u32 regionsVeneer;
  const struct SceneRegion *(*regions)(void);
};

#endif

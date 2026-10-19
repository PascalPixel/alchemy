#ifndef ALCHEMY_FIELD_SCENE_H
#define ALCHEMY_FIELD_SCENE_H

/// @file
/// The scene table formats: entrances, exits, placements and events, and the
/// macros that write them.

#include "static_assert.h"
#include "scene_ids.h"
#include "character_ids.h"

enum SceneId {
  SCENE_WORLD_MAP = 2,
  SCENE_VAULT = 20,
  SCENE_VAULT_JAIL = 22,
  SCENE_VAULT_REVISIT = 23,
  SCENE_LUNPA = 104,
  SCENE_LUNPA_SUHALLA = 105,
  SCENE_WEST_LUNPA_CAVE = 106,
  SCENE_LUNPA_FORTRESS_GATE = 159,
  SCENE_LUNPA_FORTRESS = 160,
  SCENE_SUHALLA_GATE = 169
};

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

enum { FACING_STEP = 0x1000 };

#define FACING_IS_NORTH(facing)                                                \
  ((u32)((facing) - (FACING_NORTHWEST + 1)) <=                                 \
   FACING_NORTHEAST - FACING_NORTHWEST - 2)

enum {
  SCENE_TABLE_END = -1,
  CONDITION_ALWAYS = -1,
  CONDITION_FLAG_SET = 0x1000
};

#define IF_FLAG_SET(flag) ((flag) | CONDITION_FLAG_SET)

enum { FLAG_PARTY_LEFT_VALE = 0x815 };

struct SceneEntrance {
  s16 entrance;
  s16 requiredFlag;
  s16 x;
  s16 y;
  s16 z;
  u16 facing;
  s16 unused1;
  s16 cameraLeft;
  s16 cameraTop;
  s16 cameraRight;
  s16 cameraBottom;
  s16 unused2;
};

STATIC_ASSERT(sizeof(struct SceneEntrance) == 24);
STATIC_ASSERT(offsetof(struct SceneEntrance, cameraLeft) == 14);

#define SCENE_EXITS(scene) ((u32)(scene))
#define SCENE_EXIT(exit, scene, entrance)                                      \
  ((u32)(exit) << 20 | (u32)(entrance) << 12 | (u32)(scene))
#define SCENE_EXIT_IF(exit, scene, entrance, flag)                             \
  (SCENE_EXIT(exit, scene, entrance) | 0x10000000), (u32)(flag)
#define SCENE_EXITS_END 0x1ff

struct ScenePlacement {
  s16 sprite;
  s16 condition;
  s32 behavior;
  s32 x;
  s32 y;
  s32 z;
  u16 facing;
  u8 talkFacing;
  u8 flags;
};

STATIC_ASSERT(sizeof(struct ScenePlacement) == 24);
STATIC_ASSERT(offsetof(struct ScenePlacement, talkFacing) == 22);

enum ActorBehavior { ACTOR_STAND = 1, ACTOR_WANDER = 2 };

enum ActorTalkFacing {
  TALK_FACE_PARTY = 0,
  TALK_FACE_PARTY_AND_BACK = 1,
  TALK_KEEP_FACING = 2
};

#define PLACEMENT_SHARE_PREVIOUS_SPRITE 0x01

enum { ACTOR_PARTY_LEADER = 0, ACTOR_FIRST_PLACED = 8 };

#define PIXELS(value) ((value) << 16)

struct SceneEvent {
  u32 control;
  s16 trigger;
  s16 condition;
  u32 value;
};

STATIC_ASSERT(sizeof(struct SceneEvent) == 12);

enum SceneEventKind {
  EVENT_TALK = 0,
  EVENT_EXIT = 1,
  EVENT_TOUCH = 2,
  EVENT_SEARCH = 3,
  EVENT_ITEM = 4,
  EVENT_PSYNERGY = 5,
  EVENT_RAISED = 6
};

#define EVENT_TOUCH_WALKING 0x200
#define EVENT_TOUCH_FACING 0x400
#define TOUCH_WALKING (EVENT_TOUCH | EVENT_TOUCH_WALKING)
#define TOUCH_FACING(facing) (EVENT_TOUCH | EVENT_TOUCH_FACING | (facing))
#define TOUCH_WALKING_INTO(facing)                                             \
  (EVENT_TOUCH | EVENT_TOUCH_WALKING | EVENT_TOUCH_FACING | (facing))

#define EVENT_PSYNERGY_ON_ACTOR 0x10
#define PSYNERGY_ON_ACTOR(ability)                                             \
  (EVENT_PSYNERGY | EVENT_PSYNERGY_ON_ACTOR | (ability) << 8)
/// A Psynergy event on an actor at one phase of the cast, as PSYNERGY_IN_AREA
/// is for an area.
#define PSYNERGY_PHASE_ON_ACTOR(ability, phase)                                \
  ((u32)(phase) << 28 | PSYNERGY_ON_ACTOR(ability))

#define EVENT_PSYNERGY_IN_AREA 0x80
#define PSYNERGY_IN_AREA(ability, phase)                                       \
  ((u32)(phase) << 28 | (ability) << 8 | EVENT_PSYNERGY_IN_AREA |              \
   EVENT_PSYNERGY)

// Using an item (one byte) on a tile code, or on an actor, as
// Field_FindItemEvent reads them.
#define EVENT_ITEM_ON_ACTOR 0x10
#define USE_ITEM(item) (EVENT_ITEM | (item) << 8)
#define USE_ITEM_ON_ACTOR(item) (EVENT_ITEM | EVENT_ITEM_ON_ACTOR | (item) << 8)

#define SEARCH(target) (EVENT_SEARCH | (target) << 4)

enum SearchTarget {
  SEARCH_UNNAMED = 0,
  SEARCH_CHEST = 1,
  SEARCH_JAR,
  SEARCH_BARREL,
  SEARCH_WALL,
  SEARCH_GROUND,
  SEARCH_ROCK,
  SEARCH_HOLE,
  SEARCH_GRAVE,
  SEARCH_TREE,
  SEARCH_UNDERBRUSH,
  SEARCH_DOOR,
  SEARCH_CHIMNEY,
  SEARCH_WOODEN_BOX,
  SEARCH_BED,
  SEARCH_BOOKCASE,
  SEARCH_STONE_COFFIN,
  SEARCH_FIREPLACE,
  SEARCH_WATER,
  SEARCH_STONE_PILLAR,
  SEARCH_STALACTITE,
  SEARCH_BOARDS,
  SEARCH_FOUNTAIN,
  SEARCH_OVEN,
  SEARCH_TABLE,
  SEARCH_STONE_STATUE,
  SEARCH_STONE_TABLET,
  SEARCH_SHELF,
  SEARCH_WARDROBE,
  SEARCH_FIREWOOD,
  SEARCH_BOOKS,
  SEARCH_WELL
};

#define FIND_ITEM(item) (0x00100000 | (item))
#define FIND_COINS(coins) (0x00200000 | (coins))
#define FIND_PSYNERGY_STONE 0x00300000
#define FIND_MESSAGE(message) (0x00400000 | (message))

#define EVENT_SCRIPT(script) ((u32)(script))
#define SCENE_EVENTS_END 0xffffffff

struct SceneRegion;

#endif

#ifndef ALCHEMY_FIELD_SCENE_H
#define ALCHEMY_FIELD_SCENE_H

#include "LAYOUT_GUARD.H"

/*
 * The tables a field scene hands to the map engine. The engine asks for
 * them when the scene loads: where the party appears for each entrance,
 * where each exit leads, which actors stand in the scene, and what happens
 * when the party talks to an actor, steps on a trigger, searches a spot or
 * uses Psynergy on a target.
 */

/* Scenes, named after the area modules that load them. */
enum SceneId {
    SCENE_KUUPUAPPU_MURA = 20,
    SCENE_KUUPUAPPU_RUNPA = 22,
    SCENE_KUUPUAPPU_MURA_SAI = 23,
    SCENE_RUNPA_MURA = 104,
    SCENE_RUNPA_SUHARA = 105,
    SCENE_SUHARA_GATE = 169
};

/* Directions as facing angles; a quarter turn is 0x4000. */
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

/* A sixteenth of a turn, for facings between the eight directions. */
enum {
    FACING_STEP = 0x1000
};

/* Whether a facing lies strictly between northwest and northeast. */
#define FACING_IS_NORTH(facing) \
    ((u32)((facing) - (FACING_NORTHWEST + 1)) <= FACING_NORTHEAST - FACING_NORTHWEST - 2)

enum {
    /* Ends the entrance, placement and event tables. */
    SCENE_TABLE_END = -1,
    /* A record that applies whatever the flags hold. */
    CONDITION_ALWAYS = -1,
    /* Marks a condition met while its flag is set; without it the flag must be clear. */
    CONDITION_FLAG_SET = 0x1000
};

#define IF_FLAG_SET(flag) ((flag) | CONDITION_FLAG_SET)

/* Where the party appears when it arrives by one entrance. */
struct SceneEntrance {
    s16 entrance;
    s16 required_flag;
    s16 x;
    s16 y;
    s16 z;
    u16 facing;
    s16 unused1;
    /* Camera limits in pixels; SCENE_TABLE_END leaves a limit unchanged. */
    s16 camera_left;
    s16 camera_top;
    s16 camera_right;
    s16 camera_bottom;
    s16 unused2;
};

LAYOUT_SIZE_GUARD(SceneEntrance_Size, struct SceneEntrance, 24);
LAYOUT_OFFSET_GUARD(SceneEntrance_CameraLeft, struct SceneEntrance, camera_left, 14);

/*
 * The exit list is a stream of words. A group opens with the scene it
 * belongs to; each exit in the group names the scene and entrance it leads
 * to. An exit that depends on a flag carries the flag in the next word.
 */
#define SCENE_EXITS(scene) ((u32)(scene))
#define SCENE_EXIT(exit, scene, entrance) \
    ((u32)(exit) << 20 | (u32)(entrance) << 12 | (u32)(scene))
#define SCENE_EXIT_IF(exit, scene, entrance, flag) \
    (SCENE_EXIT(exit, scene, entrance) | 0x10000000), (u32)(flag)
#define SCENE_EXITS_END 0x1ff

/* One actor the engine places when the scene loads. */
struct ScenePlacement {
    s16 sprite;
    s16 condition;
    s32 behavior;
    s32 x;
    s32 y;
    s32 z;
    u16 facing;
    u8 talk_facing;
    u8 flags;
};

LAYOUT_SIZE_GUARD(ScenePlacement_Size, struct ScenePlacement, 24);
LAYOUT_OFFSET_GUARD(ScenePlacement_TalkFacing, struct ScenePlacement, talk_facing, 22);

enum ActorBehavior {
    ACTOR_STAND = 1,
    ACTOR_WANDER = 2
};

/* How an actor turns when the party talks to it. */
enum ActorTalkFacing {
    TALK_FACE_PARTY = 0,
    TALK_FACE_PARTY_AND_BACK = 1
};

/* The actor takes its sprite slot from the actor placed before it. */
#define PLACEMENT_SHARE_PREVIOUS_SPRITE 0x01

/* Actors placed from the table take ids from here on; lower ids are the party. */
enum {
    ACTOR_PARTY_LEADER = 0,
    ACTOR_FIRST_PLACED = 8
};

#define PIXELS(value) ((value) << 16)

/* One interaction the scene answers. */
struct SceneEvent {
    u32 control;
    s16 trigger;
    s16 condition;
    u32 value;
};

LAYOUT_SIZE_GUARD(SceneEvent_Size, struct SceneEvent, 12);

/* The low four control bits select what starts the event. */
enum SceneEventKind {
    /* Talking to an actor; the value is a message or a script. */
    EVENT_TALK = 0,
    /* Stepping on a trigger; the value is the exit taken. */
    EVENT_EXIT = 1,
    /* Stepping on a trigger; the value is a script. */
    EVENT_TOUCH = 2,
    /* Searching a spot; the value is what the search finds. */
    EVENT_SEARCH = 3,
    /* Using Psynergy on a target; the value is a message or a script. */
    EVENT_PSYNERGY = 5
};

/* A Psynergy event aims at an actor rather than a map trigger. */
#define EVENT_PSYNERGY_ON_ACTOR 0x10
#define PSYNERGY_ON_ACTOR(ability) \
    (EVENT_PSYNERGY | EVENT_PSYNERGY_ON_ACTOR | (ability) << 8)

/* Searches report what the party checked; the target sits above the kind. */
#define SEARCH(target) (EVENT_SEARCH | (target) << 4)

enum SearchTarget {
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

/* What a search finds. */
#define FIND_ITEM(item) (0x00100000 | (item))
#define FIND_COINS(coins) (0x00200000 | (coins))
#define FIND_PSYNERGY_STONE 0x00300000
#define FIND_MESSAGE(message) (0x00400000 | (message))

#define EVENT_SCRIPT(script) ((u32)(script))
#define SCENE_EVENTS_END 0xffffffff

/* Map regions are not used by every scene. */
struct SceneRegion;

/* The scene's interface to the map engine, in entry-table order. */
s32 Scene_Initialize(void);
const struct SceneEntrance *Scene_GetEntrances(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
const struct SceneEvent *Scene_GetEvents(void);
const struct SceneRegion *Scene_GetRegions(void);

#endif

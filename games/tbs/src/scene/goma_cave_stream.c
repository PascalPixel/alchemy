// The first and last rooms of Goma Cave, the cave in the Goma Range between Vault and Bilibin:
// SceneIdGomaCaveStream and SceneIdGomaCaveBridge, both under the area name Goma Cave.
//
// The getters give SceneIdGomaCaveBridge its own tables and any other scene
// SceneIdGomaCaveStream's. The two share one exit table.
//
// - SceneIdGomaCaveStream, the room a stream runs the length of. The party
//       walks in from the Goma Cave Entrance
//       (GOMA_CAVE_STREAM_FROM_GOMA_CAVE_ENTRANCE) at its bottom.
//
//       A stump answers Move. Once it stands on column 24 it spins in place,
//       leaps, lands with a shake and a ring of dust, then hops on into the
// water with a splash and sinks until its top shows. The metatiles where it
// lies change (FLAG_GOMA_CAVE_STREAM_STUMP_MOVED, a story flag, so it  stays
// there for good).
//
//       Exits lead back to the Goma Cave Entrance, into the room of two pillars
//       (goma_cave_pillar.c; GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_WEST and
//       _EAST) and north into SceneIdGomaCaveBridge.
//
//       The same map holds a small cave off Vale's plaza, walled off from the
//       rest of the room in a camera box of its own. It has a chest of Power
//       Bread at its north end (its render, places/goma-cave-stream/map-5.png).
//
//       Vale's VALE_PLAZA_TO_CHEST_CAVE leads in at
//       GOMA_CAVE_STREAM_CHEST_CAVE_FROM_VALE_PLAZA, which shows no area name.
// Its only way out is the exit metatile at its south tip, metatile (38, 55)
// in  goma_cave_stream's map.txt, two metatiles south of the arrival, which
// leads  back to the plaza (GOMA_CAVE_STREAM_CHEST_CAVE_TO_VALE_PLAZA).
//
// - SceneIdGomaCaveBridge, the way out, where a wooden bridge crosses the
//       river. It has no actors and only its two exits. They lead back south
//       into SceneIdGomaCaveStream and out onto the World Map toward Bilibin
//       (GOMA_CAVE_BRIDGE_TO_WORLD_MAP).

#include "ability_ids.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script.h"
#include "graphics/graphics.h"
#include "scene/goma_cave.h"

extern SCENE_ENTRANCE GomaCaveStreamEntrances[];
extern SCENE_ENTRANCE GomaCaveBridgeEntrances[];
extern Uint32         GomaCaveStreamAndBridgeExits[];
extern ACTOR_PLACE    GomaCaveStreamActorPlacements[];
extern ACTOR_PLACE    GomaCaveBridgeActorPlacements[];
extern CUE            GomaCaveStreamCues[];
extern CUE            GomaCaveBridgeCues[];

// SceneIdGomaCaveStream's actors, numbering its actor placement rows from ACTOR_FIRST_PLACED.
typedef enum GOMA_CAVE_STREAM_ACTOR_ID
{
    // The stump Move pushes, as Garet calls the one at the Goma Cave Entrance.
    ACTOR_STUMP = ACTOR_FIRST_PLACED
} GOMA_CAVE_STREAM_ACTOR_ID;

// SceneIdGomaCaveStream's entrance that no exit names, standing the leader just east of the stump,
// facing it.
//
// Only the debug scene selector, which exits to any entrance by number, reaches it.
enum
{
    ENTRANCE_BESIDE_STUMP = 99
};

static void scLeapStump(Sint32 stumpId);

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

const SCENE_ENTRANCE *scGetEntrances(void)
{
    if (MainRoutineStateWork.next == SCENE_GOMA_CAVE_BRIDGE)
    {
        return GomaCaveBridgeEntrances;
    }
    return GomaCaveStreamEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

const Uint32 *scGetExits(void)
{
    return GomaCaveStreamAndBridgeExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
    if (MainRoutineStateWork.next == SCENE_GOMA_CAVE_BRIDGE)
    {
        return GomaCaveBridgeActorPlacements;
    }
    return GomaCaveStreamActorPlacements;
}

// Isaac casts Move on the stump by the stream: once it has been pushed onto column 24 it leaps into
// the water, and the metatiles where it comes to lie change.
static void scGomaCaveStreamMoveStump(void)
{
    if (fdActorGet(ACTOR_STUMP)->x.fixed / toFIXED(16) == 24)
    {
        scLeapStump(ACTOR_STUMP);
        fdActorGet(ACTOR_STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        fdMapGridCopyMetatileAttributes(19, 74, 9, 3, 19, 17);
        fdActorSetSpriteFlags(fdActorGet(ACTOR_STUMP), 0);
        gmFlagSet(FLAG_GOMA_CAVE_STREAM_STUMP_MOVED);
    }
}

const CUE *scGetCues(void)
{
    if (MainRoutineStateWork.next == SCENE_GOMA_CAVE_BRIDGE)
    {
        return GomaCaveBridgeCues;
    }
    return GomaCaveStreamCues;
}

// Sets up SceneIdGomaCaveStream as the party enters (SceneIdGomaCaveBridge needs nothing).
Sint32 scInit(void)
{
    FieldStateWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    if (MainRoutineStateWork.next == SCENE_GOMA_CAVE_STREAM)
    {
        if (MainRoutineStateWork.entrance == GOMA_CAVE_STREAM_CHEST_CAVE_FROM_VALE_PLAZA)
        {
            gmFlagClear(FLAG_SHOW_AREA_NAME);
        }
        else
        {
            fdActorGet(ACTOR_STUMP)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
            if (gmFlagGet(FLAG_GOMA_CAVE_STREAM_STUMP_MOVED) != 0)
            {
                fdActorSetPos(ACTOR_STUMP, 346, 292);
                fdActorSetSpriteFlags(fdActorGet(ACTOR_STUMP), 0);
                fdActorGet(ACTOR_STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
                fdActorSetAnimation(ACTOR_STUMP, STUMP_ANIMATION_SINK);
                fdMapGridCopyMetatileAttributes(19, 74, 9, 3, 19, 17);
            }
        }
    }
    return 0;
}

// Where the stump lands from the ledge and where it bounces on to and sinks, in pixels, and the
// ground's height there.
#define STUMP_LANDING_X    376
#define STUMP_LANDING_Z    288
#define STUMP_RESTING_X    346
#define STUMP_RESTING_Z    292
#define STUMP_FLOOR_HEIGHT 0
#include "shared/stump_leap.c"

#include "shared/field_effect_duration_scripts.c"

const Sint32 *FieldEffectDurationScripts[] = {
    FieldEffectShortScript,
    FieldEffectMediumScript,
    FieldEffectLongScript,
};

SCENE_ENTRANCE GomaCaveStreamEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {GOMA_CAVE_STREAM_FROM_GOMA_CAVE_ENTRANCE,
     CONDITION_ALWAYS, 376,
     0, 776,
     FACING_NORTH, 0,
     88, 8,
     664, 832,
     0},
    {GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_WEST,
     CONDITION_ALWAYS, 168,
     0, 552,
     FACING_SOUTH, 0,
     88, 8,
     664, 832,
     0},
    {GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_EAST,
     CONDITION_ALWAYS, 600,
     0, 520,
     FACING_SOUTH, 0,
     88, 8,
     664, 832,
     0},
    {GOMA_CAVE_STREAM_FROM_GOMA_CAVE_BRIDGE,
     CONDITION_ALWAYS, 376,
     0, 168,
     FACING_SOUTH, 0,
     88, 8,
     664, 832,
     0},
    {GOMA_CAVE_STREAM_CHEST_CAVE_FROM_VALE_PLAZA,
     CONDITION_ALWAYS, 614,
     0, 848,
     FACING_NORTH, 0,
     480, 605,
     720, 900,
     0},
    {ENTRANCE_BESIDE_STUMP, CONDITION_ALWAYS, 424, 0, 280, FACING_WEST, 0, 88, 8, 664, 832, 0},
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE GomaCaveBridgeEntrances[] = {
    {GOMA_CAVE_BRIDGE_FROM_GOMA_CAVE_STREAM,
     CONDITION_ALWAYS, 154,
     0, 350,
     FACING_NORTH, 0,
     16, 48,
     632, 360,
     0},
    {GOMA_CAVE_BRIDGE_FROM_WORLD_MAP,
     CONDITION_ALWAYS, 552,
     0, 312,
     FACING_NORTH, 0,
     16, 48,
     632, 360,
     0},
    {SCENE_TABLE_END},
};

Uint32 GomaCaveStreamAndBridgeExits[] = {
    SCENE_EXITS(GomaCaveStream),
    SCENE_EXIT(GOMA_CAVE_STREAM_TO_GOMA_CAVE_ENTRANCE,
               GomaCaveEntrance,
               GOMA_CAVE_ENTRANCE_FROM_GOMA_CAVE_STREAM),
    SCENE_EXIT(GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_WEST,
               GomaCavePillar,
               GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_WEST),
    SCENE_EXIT(GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_EAST,
               GomaCavePillar,
               GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST),
    SCENE_EXIT(GOMA_CAVE_STREAM_TO_GOMA_CAVE_BRIDGE,
               GomaCaveBridge,
               GOMA_CAVE_BRIDGE_FROM_GOMA_CAVE_STREAM),
    SCENE_EXIT(GOMA_CAVE_STREAM_CHEST_CAVE_TO_VALE_PLAZA, ValePlaza, VALE_PLAZA_FROM_CHEST_CAVE),
    SCENE_EXITS(GomaCaveBridge),
    SCENE_EXIT(GOMA_CAVE_BRIDGE_TO_GOMA_CAVE_STREAM,
               GomaCaveStream,
               GOMA_CAVE_STREAM_FROM_GOMA_CAVE_BRIDGE),
    SCENE_EXIT(GOMA_CAVE_BRIDGE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_GOMA_CAVE_BRIDGE),
    SCENE_EXITS_END,
};

// clang-format off
ACTOR_PLACE GomaCaveStreamActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        400, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};
// clang-format on

ACTOR_PLACE GomaCaveBridgeActorPlacements[] = {
    {SCENE_TABLE_END},
};

CUE GomaCaveStreamCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, GOMA_CAVE_STREAM_CHEST_CAVE_TO_VALE_PLAZA},
    {CUE_EXIT, 5, CONDITION_ALWAYS, GOMA_CAVE_STREAM_TO_GOMA_CAVE_ENTRANCE},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
     6, CONDITION_ALWAYS,
     GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_WEST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
     7, CONDITION_ALWAYS,
     GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_EAST},
    {CUE_EXIT, 8, CONDITION_ALWAYS, GOMA_CAVE_STREAM_TO_GOMA_CAVE_BRIDGE},
    {EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_GOMA_CAVE_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_STUMP, FLAG_GOMA_CAVE_STREAM_STUMP_MOVED,
     SCRIPT_REF(scGomaCaveStreamMoveStump)},
    {CUE_TABLE_END},
};

CUE GomaCaveBridgeCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, GOMA_CAVE_BRIDGE_TO_GOMA_CAVE_STREAM},
    {CUE_EXIT, 2, CONDITION_ALWAYS, GOMA_CAVE_BRIDGE_TO_WORLD_MAP},
    {CUE_TABLE_END},
};

// Goma Cave (SceneIdGomaCavePond), the room below the room of two pillars and a stump, in the cave
// in the Goma Range that the mayor of Vault tells the party to pass through on the way to Bilibin.
//
// The room is reached only from the room above (goma_cave_pillar.c). Its three exits lead back up
// to that room's GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_WEST, _SOUTH and _EAST. It has no scripts of
// its own.
//
// Its one placed actor is the stump the party pushes over the edge up there with Move. Once it has
// fallen (FLAG_GOMA_CAVE_STUMP_FELL), entering finds it sunk in the pond until only its top shows
// (STUMP_ANIMATION_SINK), drawn underfoot with no collision flags.
//
// The five by five metatiles around it are open water in the room's render
// (places/goma-cave-pond/map-1.png). They are replaced by a copy that adds
// METATILE_BEHAVIOR_JUMP_FORWARD metatiles, so the leader can jump across the water by way of the
// stump.
//
// The flag is an area temp flag, so the stump is back up in the room above once the party leaves
// Goma Cave's four scenes.

#include "debug/debug.h"
#include "field/script.h"
#include "graphics/graphics.h"
#include "scene/goma_cave.h"

extern SCENE_ENTRANCE GomaCavePondEntrances[];
extern Uint32         GomaCavePondExits[];
extern ACTOR_PLACE    GomaCavePondActorPlacements[];
extern CUE            GomaCavePondCues[];

// The room's one placed actor.
typedef enum GOMA_CAVE_POND_ACTOR_ID
{
    // The stump the party pushes over in the room above, lying in this room.
    ACTOR_FALLEN_STUMP = ACTOR_FIRST_PLACED
} GOMA_CAVE_POND_ACTOR_ID;

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return GomaCavePondEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

const Uint32 *scGetExits(void)
{
    return GomaCavePondExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
    return GomaCavePondActorPlacements;
}

const CUE *scGetCues(void)
{
    return GomaCavePondCues;
}

// Sets up the room as the party enters.
Sint32 scInit(void)
{
    FIELD_STATE *state;

    state                  = FieldStateWork;
    state->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    state->transitionFrame = 24;
    if (gmFlagGet(FLAG_GOMA_CAVE_STUMP_FELL) != 0)
    {
        fdActorSetPos(ACTOR_FALLEN_STUMP, 216, 136);
        fdActorSetAnimation(ACTOR_FALLEN_STUMP, STUMP_ANIMATION_SINK);
        fdActorSetSpriteFlags(fdActorGet(ACTOR_FALLEN_STUMP), 0);
        fdActorGet(ACTOR_FALLEN_STUMP)->priorityFlags  = ACTOR_PRIORITY_UNDERFOOT;
        fdActorGet(ACTOR_FALLEN_STUMP)->collisionFlags = 0;
        fdMapGridCopyMetatileAttributes(11, 36, 5, 5, 11, 6);
    }
    return 0;
}

SCENE_ENTRANCE GomaCavePondEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_WEST,
     CONDITION_ALWAYS, 120,
     0, 88,
     FACING_SOUTH, 0,
     24, 8,
     504, 424,
     0},
    {GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_SOUTH,
     CONDITION_ALWAYS, 424,
     0, 344,
     FACING_SOUTH, 0,
     24, 8,
     504, 424,
     0},
    {GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_EAST,
     CONDITION_ALWAYS, 408,
     0, 136,
     FACING_SOUTH, 0,
     24, 8,
     504, 424,
     0},
    {SCENE_TABLE_END},
};

Uint32 GomaCavePondExits[] = {
    SCENE_EXITS(GomaCavePond),
    SCENE_EXIT(GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_WEST,
               GomaCavePillar,
               GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_WEST),
    SCENE_EXIT(GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_SOUTH,
               GomaCavePillar,
               GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_SOUTH),
    SCENE_EXIT(GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_EAST,
               GomaCavePillar,
               GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST),
    SCENE_EXITS_END,
};

// clang-format off
ACTOR_PLACE GomaCavePondActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};
// clang-format on

CUE GomaCavePondCues[] = {
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_WEST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP,
     6, CONDITION_ALWAYS,
     GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_SOUTH},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, GOMA_CAVE_POND_TO_GOMA_CAVE_PILLAR_EAST},
    {CUE_TABLE_END},
};

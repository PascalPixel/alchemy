// Kolima Forest (SceneIdKolimaForestCliff), the third of the forest's five scenes, between
// SceneIdKolimaForestGlade and SceneIdKolimaForestFloodgate.
//
// It has three logs, rolled by walking into them. The one pushed west to the water's edge tips into
// the river and floats there, metatiles the party can cross to the far side. From the bank the
// leader can hop east (step cue 11, scKolimaForestHopEast).
//
// The forest keeps its cursed colours until Tret lifts the curse (FLAG_KOLIMA_CURSE_LIFTED).
//
// FLAG_KOLIMA_FOREST_CLIFF_LOG_IN_RIVER: the log is in the river, until the party next comes in.
//
// The scene's name, Cliff, is ours, for what sets the scene apart. A rock cliff runs across its
// north side above the river and the logs (places/kolima-forest-cliff_cursed/map-0.png).

#include "flags.h"
#include "debug/debug.h"
#include "field/script.h"
#include "field/speaker.h"
#include "graphics/graphics.h"
#include "scene/kolima_palette.h"

extern SCENE_ENTRANCE KolimaForestCliffEntrances[];
extern Uint32         KolimaForestCliffExits[];
extern ACTOR_PLACE    KolimaForestCliffActorPlacements[];
extern CUE            KolimaForestCliffCues[];

// The scene's actors, numbering the rows of KolimaForestCliffActorPlacements.
enum
{
    // The three logs. The third is the one that can reach the river.
    ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
    ACTOR_RIVER_LOG = ACTOR_FIRST_LOG + 2
};

#include "shared/pushed_actor.c"

// The river log has tipped into the river and floats there as ground the party can cross.
#define FLAG_KOLIMA_FOREST_CLIFF_LOG_IN_RIVER FLAG_SCENE_TEMP_01

// The column at the water's edge, where the river log pushed west tips in.
#define CLIFF_RIVER_EDGE_COLUMN               12
// How far the curse tints the cliff until it is lifted: green and blue lose 1 / CLIFF_CURSE_DIVISOR
// of themselves (scKolimaPaletteCurse).
#define CLIFF_CURSE_DIVISOR                   6

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return KolimaForestCliffEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return 0;
}

const Uint32 *scGetExits(void)
{
    return KolimaForestCliffExits;
}

// The three logs.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    return KolimaForestCliffActorPlacements;
}

// In Kolima Forest (SceneIdKolimaForestCliff), walking into a log rolls it ahead until something
// stops it.
static void scKolimaForestCliffPushLog(void)
{
    PUSHED_ACTOR_PROBE result;

    fdScriptStart();
    if (scPushedActorFindClearPos(&result))
    {
        scPushedActorRoll(result);
        if (result.actorSlot == ACTOR_RIVER_LOG
            && (FIELD_METATILE(result.pos.x)) == CLIFF_RIVER_EDGE_COLUMN)
        {
            fdActorSetAnimation(ACTOR_RIVER_LOG, LOG_ANIMATION_ROLL_WEST);
            fdActorSetDestinationOffset(ACTOR_RIVER_LOG, -18, 6);
            fdScriptWait(30);
            sySoundPlay(SE_LOG_FALL);
            fdActorSetAnimation(ACTOR_RIVER_LOG, LOG_ANIMATION_DROP_IN);
            // It floats under the party's feet, and the river's metatiles under it become ground to
            // cross on.
            fdActorGet(ACTOR_RIVER_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
            fdMapGridCopyMetatileAttributes(32, 20, 2, 4, 11, 16);
            scMapGridFillCollisionRectangle(2, 12, 16, 1, 4, METATILE_BEHAVIOR_NORMAL);
            gmFlagSet(FLAG_KOLIMA_FOREST_CLIFF_LOG_IN_RIVER);
            fdActorSetSpriteFlags(fdActorGet(ACTOR_RIVER_LOG), 0);
        }
    }
    fdScriptEnd();
}

#include "shared/kolima_jump.c"

// The logs, the two ways out and the hop east from the bank.
const CUE *scGetCues(void)
{
    return KolimaForestCliffCues;
}

// Sets up the logs and the forest's colours on entry.
Sint32 scInit(void)
{
    FIELD_STATE *state = FieldStateWork;

    state->transition  = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    scPushedActorBlockFootprint(ACTOR_RIVER_LOG);

    // The log the party tipped in still floats in the river, as ground to cross on.
    if (gmFlagGet(FLAG_KOLIMA_FOREST_CLIFF_LOG_IN_RIVER) != 0)
    {
        fdActorGet(ACTOR_RIVER_LOG)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        fdMapGridCopyMetatileAttributes(32, 20, 2, 4, 11, 16);
        scMapGridFillCollisionRectangle(2, 12, 16, 1, 4, METATILE_BEHAVIOR_NORMAL);
        fdActorSetSpriteFlags(fdActorGet(ACTOR_RIVER_LOG), 0);
    }

    scPushedActorBlockFootprint(ACTOR_FIRST_LOG);
    scPushedActorBlockFootprint(ACTOR_FIRST_LOG + 1);

    if (gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0)
    {
        scKolimaPaletteCurse(CLIFF_CURSE_DIVISOR);
    }
    return 0;
}

#include "shared/kolima_palette.c"

#include "shared/pushed_actor_tables.c"
// clang-format off

SCENE_ENTRANCE KolimaForestCliffEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_CLIFF_FROM_KOLIMA_FOREST_GLADE, CONDITION_ALWAYS, 320, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_CLIFF_FROM_KOLIMA_FOREST_FLOODGATE, CONDITION_ALWAYS, 480, 0, 104, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

// On into the forest (SceneIdKolimaForestFloodgate) and back

// (SceneIdKolimaForestGlade).
Uint32 KolimaForestCliffExits[] = {
    SCENE_EXITS(KolimaForestCliff),
    SCENE_EXIT(KOLIMA_FOREST_CLIFF_TO_KOLIMA_FOREST_FLOODGATE, KolimaForestFloodgate, KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_CLIFF),
    SCENE_EXIT(KOLIMA_FOREST_CLIFF_TO_KOLIMA_FOREST_GLADE, KolimaForestGlade, KOLIMA_FOREST_GLADE_FROM_KOLIMA_FOREST_CLIFF),
    SCENE_EXITS_END,
};

ACTOR_PLACE KolimaForestCliffActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        256, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        224, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        312, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};
// clang-format on

CUE KolimaForestCliffCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, KOLIMA_FOREST_CLIFF_TO_KOLIMA_FOREST_FLOODGATE},
    {CUE_EXIT, 2, CONDITION_ALWAYS, KOLIMA_FOREST_CLIFF_TO_KOLIMA_FOREST_GLADE},
    {CUE_STEP | STEP_CUE_FLAG_WALKING,
     10, CONDITION_ALWAYS,
     SCRIPT_REF(scKolimaForestCliffPushLog)},
    {STEP_CUE_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, SCRIPT_REF(scKolimaForestHopEast)},
    {CUE_TABLE_END},
};

// The healthy colours, kept by scKolimaPaletteSaveHealthy: the palette fade's 448 background and
// sprite colours, in the array's first half.
Uint8 KolimaPaletteHealthy[1792] BSS_DATA ALIGNED(16);
// The cursed colours, kept by scKolimaPaletteSaveCursed.
Uint8                                     KolimaPaletteCursed[896] BSS_DATA;

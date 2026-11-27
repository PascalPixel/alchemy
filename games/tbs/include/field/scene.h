#ifndef ALCHEMY_SCENE_H
#define ALCHEMY_SCENE_H

// Scenes, exit regions and scene entry tables, with the field state they fill (field_state.h).

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "field/actor.h"
#include "field/field_state.h"
#include "field/scene_tables.h"

struct ACTOR;

// A scene's row in ScenesWork: its overlay's resource, its area, its scene type and its map.
typedef struct SCENE
{
    Sint16 resourceId; // the overlay syOverlayLoad loads into OverlayArea
    // The area: entering a new one clears the FLAG_AREA_TEMP_xx flags and the step counters and
    // moves Retreat's destination; FLAG_AREA_VISITED(area) marks it visited (gmArrive).
    Sint8  area;
    // An SCENE_TYPE, copied to MainRoutineStateWork.sceneType: SCENE_TYPE_TOWN for towns and their
    // rooms (53 rows: Vale, Bilibin, Kalay, Tolbi and others), SCENE_TYPE_OTHER for the rest.
    Sint8  sceneType;
    Uint16
        // the map loaded for it, copied to MainRoutineStateWork.mapId
           mapIndex;
    Uint16 reserved; // 0 in every row; not read
} SCENE;

STATIC_ASSERT(sizeof(SCENE) == 8);

// A scene's exit region: the box that, while its condition holds, takes the party leader out
// through its exit.
typedef struct SCENE_EXIT_REGION
{
    Sint16 minX, minY, minZ; // pixels, inclusive; minX -1 ends the table
    Sint16 maxX, maxY, maxZ; // pixels, exclusive
    // CONDITION_ALWAYS, a flag | CONDITION_FLAG_SET set, else the flag clear
    Sint16 condition;
    Sint16 value; // the exit kind, stored in FIELD_STATE's exit
} SCENE_EXIT_REGION;

STATIC_ASSERT(sizeof(SCENE_EXIT_REGION) == 16);

// The entry table at the start of each scene overlay: its initialize function and the functions
// returning its entrances, exits, actor placements, cues and exit regions, each behind a stub word.
typedef struct SCENE_ENTRY_POINTS
{
    Uint32                   initializeStub;
    Sint32                   (*initialize)(void);
    Uint32                   entrancesStub;
    const SCENE_ENTRANCE    *(*entrances)(void);
    Uint32                   exitsStub;
    const Uint32            *(*exits)(void);
    Uint32                   actorPlacementsStub;
    const ACTOR_PLACE       *(*actorPlacements)(void);
    Uint32                   cuesStub;
    const CUE               *(*cues)(void);
    Uint32                   regionsStub;
    const SCENE_EXIT_REGION *(*regions)(void);
} SCENE_ENTRY_POINTS;

STATIC_ASSERT(sizeof(SCENE_ENTRY_POINTS) == 48);
STATIC_ASSERT(offsetof(SCENE_ENTRY_POINTS, exits) == 20);
STATIC_ASSERT(offsetof(SCENE_ENTRY_POINTS, regions) == 44);

// A position on the field.
typedef struct FIELD_POS
{
    FIXED x, y, z; // 16.16 fixed-point pixels
} FIELD_POS;

STATIC_ASSERT(sizeof(FIELD_POS) == 12);

// Every scene's row, by scene number (field/scenes.s).
extern const SCENE        ScenesWork[];
// The running scene's overlay, loaded into these 32 KB of EWRAM at each scene change; its code
// opens with its 48-byte entry table.
extern SCENE_ENTRY_POINTS OverlayArea;

#endif

#ifndef ALCHEMY_FIELD_FIELD_MODE_H
#define ALCHEMY_FIELD_FIELD_MODE_H

// What the field_mode object defines for the code outside it.

#include "types.h"
#include "field/encounter_zone.h"
#include "field/field.h"
#include "field/scene_tables.h"
#include "field/actor.h"
#include "field/field_mode_functions.h"

struct ACTOR;

// How the leader moves (MAIN_ROUTINE_STATE.movementMode): which actor script
// fdActorStartMovementModeScript runs for it.
typedef enum LEADER_MOVEMENT_MODE
{
    // Walking: the pad moves the leader on a layered map.
    LEADER_MOVEMENT_WALK = 0,
    LEADER_MOVEMENT_WORLD_MAP =
        1, // walking on the World Map
           // Only the configured keys are checked; the leader is moved by others.
    LEADER_MOVEMENT_KEYS        = 2,
    LEADER_MOVEMENT_ROPE        = 3,  // crossing a tied rope
    LEADER_MOVEMENT_TIGHTROPE   = 4,  // crossing a tightrope
    LEADER_MOVEMENT_SAND        = 5,  // as sand, by Sand, on a layered map
    LEADER_MOVEMENT_SHIP_ON_MAP = 6,  // the ship on a layered map
    LEADER_MOVEMENT_SHIP        = 7,  // the ship on the World Map
    LEADER_MOVEMENT_FLYING_SHIP = 8,  // the winged ship on the World Map
    LEADER_MOVEMENT_WORLD_SAND  = 9,  // as sand on the World Map
    LEADER_MOVEMENT_HOVER       = 10, // hovering, by Hover
    LEADER_MOVEMENT_WALKING     = 11, // ActorWalkingScript's
    LEADER_MOVEMENT_NONE        = 12  // the idle script: it does not move
} LEADER_MOVEMENT_MODE;

// A window transition's shape: SCENE_TRANSITION(TRANSITION_WINDOW, mode)'s low byte, which
// fdScreenTransitionUpdateScanlineTable draws.
typedef enum WINDOW_TRANSITION_MODE
{
    WINDOW_TRANSITION_BARS            = 0, // a bar on each line, at random
    WINDOW_TRANSITION_SHAPE           = 1, // a shape from ScreenTransitionShapes
    WINDOW_TRANSITION_NOISE           = 2, // ragged lines from the left edge
    WINDOW_TRANSITION_CIRCLE          = 3, // a circle at the screen's centre
    // Alternate lines wiping in from the left and the right.
    WINDOW_TRANSITION_INTERLACED_WIPE = 4,
    // As WINDOW_TRANSITION_LEADER with inside and outside swapped and the progress run the other
    // way; it flickers.
    WINDOW_TRANSITION_LEADER_INVERSE  = 7,
    // An ellipse around a point (the state's centre) whose outline ripples: Reveal's.
    WINDOW_TRANSITION_RIPPLE          = 8,
    // An ellipse wider than tall around the party leader, 16 pixels above its feet.
    WINDOW_TRANSITION_LEADER          = 9,
    // An ellipse a little taller than wide, 8 pixels above the leader's feet.
    WINDOW_TRANSITION_LEADER_LOW      = 77
} WINDOW_TRANSITION_MODE;

// A rule choosing a scene's field music, ended by id -1: a scene or an area (byScene), an entrance
// (-1 for any), a flag that must be set (-1 for none) and the music.
typedef struct MAP_MUSIC_RULE
{
    Sint16 id;
    Sint16 entrance : 15;
    Uint16 byScene  : 1;
    Sint16 flags;
    Sint16 music;
} MAP_MUSIC_RULE;

// A rule naming the area a scene is in, ended by id -1: a scene or an area (useSceneId), an
// entrance (-1 for any) and the area name's index.
typedef struct AREA_NAME_RULE
{
    Sint16 id;
    Sint16 entrance   : 15;
    Uint16 useSceneId : 1;
    Sint32 areaNameIndex;
} AREA_NAME_RULE;

// A distance beyond any field Psynergy's reach, where the search for its nearest target starts.
#define BEYOND_REACH 9999

// ACTOR_PLACE (scene_tables.h), a row of a scene's actor placement table, as fdFindActorPlacement
// reads it: its sprite unsigned and its spot in plain words.
typedef struct ACTION_DESCRIPTOR
{
    Uint16 sprite;
    Sint16 condition;
    Sint32 behavior;
    Sint32 x;
    Sint32 y;
    Sint32 z;
    Uint16 facing;
    Uint8  talkFacing;
    Uint8  flags;
} ACTION_DESCRIPTOR;

// What the map screen is shown for (fdMapScreenShow).
typedef enum MAP_SCREEN_MODE
{
    // The player looks (FIELD_RAISED_CUE_MAP_SCREEN).
    MAP_SCREEN_LOOK     = 0,
    // Teleport (FIELD_RAISED_CUE_TELEPORT): A chooses the place whose name shows and B cancels.
    MAP_SCREEN_TELEPORT = 1
} MAP_SCREEN_MODE;

Sint32 fdMapScreenShow(Sint32 mode);
void   fdWorldMapZoomOutWhileLHeld(void);
void   fdBattleBackgroundSelectForEnvironment(Sint32 terrain);
Sint32 fdGameFlagIsConditionActive(Sint32 condition);
void  *fdFindActorTalkCue(Sint32 id);
Sint32 fdUseItem(Sint32 arg, Sint32 slot);
void   fdActorReleaseIfPresent(ACTOR *actor);

// How fdItemDisplayShowItem and fdItemDisplayShowPicture show a found item.
typedef enum ITEM_DISPLAY_FLAG
{
    ITEM_DISPLAY_SPARKLES = 1, // sparkles rise from it
    ITEM_DISPLAY_RAISED   = 2  // the leader raises it overhead
} ITEM_DISPLAY_FLAG;

void fdItemDisplayRaiseAboveLeader(ACTOR *actor);
void fdPsynergyLift(void);

// A field Psynergy or item use, packed into one value, as FIELD_STATE.psynergy and FIELD_STATE.item
// hold it. bits: 13..10 the user (13 is ACT_PACKED) | 9..0 the Psynergy or item.
#define ACT_ID_MASK    0x3ff                          // bits 9..0
#define ACT_UNIT_SHIFT 10                             // bits 13..10
#define ABILITY_ID(c)  ((c) & ACT_ID_MASK)            // the Psynergy
#define ACT_ITEM_ID(c) ((c) & ACT_ID_MASK)
#define ACT_UNIT(c)    (((c) >> ACT_UNIT_SHIFT) & 15) // who uses it
#define ACT_PACKED     0x2000                         // use it without paying PP

#endif

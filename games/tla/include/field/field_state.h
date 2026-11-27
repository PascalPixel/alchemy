#ifndef ALCHEMY_FIELD_STATE_H
#define ALCHEMY_FIELD_STATE_H

// FIELD_STATE, the running scene's state, with the field Psynergy's state at its end.

#include "fixed_math.h"
#include "types.h"
#include "edition.h"
#include "static_assert.h"
#include "system/globals.h"
#include "field/scene_tables.h"

struct ACTOR_PLACE;
struct ACTOR;
struct MAP_METATILE;
struct CUE_VIEW;
struct WINDOW;

// The actor ids FIELD_STATE.actors maps: the party's eight characters, the actors a scene places,
// and from FIELD_TREASURE_ACTOR on the map's treasure spots.
#define FIELD_ACTOR_COUNT     80

// FIELD_STATE.scoopedItem: coins rather than an item, their count in the low bits.
#define SCOOPED_COINS         0x8000
// The actor that shows what Scoop dug up.
#define SCOOPED_ITEM_ACTOR    79
// The first actor id of a treasure spot (fdPlaceTreasureSpots).
#define FIELD_TREASURE_ACTOR  64
// FIELD_STATE.talk and fdTalkToActor: with this bit a container is opened without its "checked the
// ..." message, as Scoop opens what it digs up; the actor id is in the bits under it.
#define FIELD_TALK_SILENT     0x1000
// fdTalkToActor's id: the actor id, under FIELD_TALK_SILENT.
#define FIELD_TALK_ACTOR_MASK 0xfff
// FIELD_STATE.talk: set with the actor id under it to ask for that actor's talk.
#define FIELD_TALK_REQUEST    0x4000
// The ticks of a scene's default fade in, from the backdrop colour.
#define MAP_TRANSITION_TICKS  16

// The values of FIELD_STATE.raisedCue that the field mode handles itself, rather than passing them
// to the scene (fdRunRaisedCue).
typedef enum FIELD_RAISED_CUE
{
    // Party members fell to poison or hazardous ground this step.
    FIELD_RAISED_CUE_PARTY_FELL     = -1,
    // Teleport: the picture of the whole world, shown once to choose where to go
    // (fdPsynergyTeleport).
    FIELD_RAISED_CUE_TELEPORT       = -887,
    // R on the world map: the picture of the whole world (fdMapScreenShow).
    FIELD_RAISED_CUE_MAP_SCREEN     = -888,
    // L on the world map: the view zooms out while L is held.
    FIELD_RAISED_CUE_WORLD_MAP_ZOOM = -889,
    FIELD_RAISED_CUE_ROPE_OFF       = -890,
    FIELD_RAISED_CUE_ROPE_ON        = -891,
    FIELD_RAISED_CUE_TIGHTROPE_OFF  = -892,
    FIELD_RAISED_CUE_TIGHTROPE_ON   = -893
} FIELD_RAISED_CUE;

// The raised cue the menu keys pass to the scene while FLAG_MENUS_RAISE_CUE is set.
#define FIELD_RAISED_CUE_MENUS 250

// The raised cues the world map's movement passes to its scene, whose scripts change how the party
// travels.
typedef enum WORLD_MAP_RAISED_CUE
{
    // A on the world map: scWorldMapBoardShip, the party boards the ship where it lies.
    WORLD_MAP_RAISED_BOARD_SHIP = 251,
    // A while sailing: scWorldMapLeaveShip, the party goes ashore.
    WORLD_MAP_RAISED_LEAVE_SHIP = 252,
    // A dash key while sailing, once the ship has its wings: scWorldMapSetUpFlyingShip, the ship
    // takes off.
    WORLD_MAP_RAISED_TAKE_OFF   = 253,
    // The winged ship come down to the water: scWorldMapSetUpShip, it sails again.
    WORLD_MAP_RAISED_LAND       = 254
} WORLD_MAP_RAISED_CUE;

// How the actors are moved and drawn (FIELD_STATE.mode, fdActorSystemInit).
typedef enum ACTOR_SYSTEM_MODE
{
    // A town, a dungeon or the sprite viewer: the scrolling camera.
    ACTOR_SYSTEM_MAP       = 2,
    // The world map: the fixed camera, which projects 3D points onto the screen.
    ACTOR_SYSTEM_WORLD_MAP = 3,
    // A battle: the fixed camera, and the battle's own motion update (fdActorUpdateAllMotion).
    ACTOR_SYSTEM_BATTLE    = 4
} ACTOR_SYSTEM_MODE;

// The ticks the leader pushes against a step cue, a ledge, a gap or a wall before it runs, jumps or
// climbs (FIELD_STATE.blockedSteps).
#define FIELD_PUSH_TICKS 12

// FIELD_STATE.terrainKind: the ground the ship was last over, which sets where fdWorldMapIsFlyable
// lets it fly.
typedef enum SHIP_TERRAIN_KIND
{
    // The sea from WORLD_MAP_COLLISION_SEA_FIRST, short of WORLD_MAP_COLLISION_SEA_LAST.
    SHIP_TERRAIN_SEA      = 0,
    // WORLD_MAP_COLLISION_WALKABLE_FIRST and shallow water.
    SHIP_TERRAIN_SHALLOWS = 1,
    SHIP_TERRAIN_SEA_LAST = 2, // WORLD_MAP_COLLISION_SEA_LAST
                               // WORLD_MAP_COLLISION_DEFAULT to WORLD_MAP_COLLISION_WALKABLE_LAST,
                               // the rest of the ground the party walks on.
    SHIP_TERRAIN_GROUND   = 3
} SHIP_TERRAIN_KIND;

// A field scene's state: fdModeRun allocates it in HEAP_SLOT_FIELD, zeroes it and frees it on the
// way out, and the field mode, the script commands and the scene overlays all reach it through
// RamHeapSlots->fieldState.
typedef struct FIELD_STATE
{
    // The actor placement tables placed so far, recorded by ScriptSpawnActorPlacements.
    const ACTOR_PLACE   *tables[4];
    const CUE_VIEW      *cues; // the scene's cue table
    // The actors by actor id: 0 to 7 the party's characters, 8 on the placed actors,
    // FIELD_TREASURE_ACTOR on the treasure spots.
    struct ACTOR        *actors[FIELD_ACTOR_COUNT];
    // The twelve requests: each nonzero value asks the field mode for one thing; fdClearRequests
    // clears them and fdCountRequests sums them.
    Sint16               stepCue;      // a step cue (1 to 230) for fdRunStepCueOrExit
    Sint16               pushMetatile; // a metatile's attributes for fdRunPushMetatile
    // The exit taken, which ends the scene; EXIT_KEEP_DESTINATION for a warp.
    Sint16               exit;
    Sint16               fieldMenu; // open the field menu
    // The A button: a talk, an examine or the field menu, by what is ahead.
    Sint16               aButton;
    Sint16               pauseMenu; // the Start button: open the Pause menu
    Sint16               talk;      // an actor's talk, as actor id | FIELD_TALK_REQUEST
    // a metatile's metatile behavior, for fdExamineMetatile
    Sint16               examineMetatile;
    Sint16               encounter; // the enemy group of a random encounter
    Sint16               psynergy;  // a field Psynergy to use, as unit << 10 | Psynergy
    Sint16               item;      // an item to use, packed as fdUseItem reads it
    // A raised cue's number for fdRunRaisedCue, or an enum FIELD_RAISED_CUE.
    Sint16               raisedCue;
    Sint16               fallenCount;     // the characters who fell this step
    Sint16               survivorCount;   // 0 when the whole party fell
    Sint16               fallenOwners[9]; // the characters who fell, from the active party
    Sint16               itemSlot;        // the inventory slot fdUseItem receives
    // Ticks the leader has pushed against something, counted by the player controls; step cues and
    // push metatiles need more than FIELD_PUSH_TICKS.
    Sint16               blockedSteps;
    // The keys held: a ledge (KEY_DOWN) and a climb (KEY_UP or KEY_DOWN) read them, and they are
    // kept when a rope or tightrope was raised.
    Uint16               heldKeys;
    Uint8                unusedAfterHeldKeys[2]; // no code names it
    Sint8                mode;                   // an ACTOR_SYSTEM_MODE
    // The area's graphics are to be reloaded on the World Map (fdWorldMapFreeze,
    // fdWorldMapReloadPendingAreaGraphics).
    Sint8                areaGraphicsPending;
    Uint8                encounterZones[8]; // encounter zone for each kind of ground
    Sint32               encounterBias;     // randomized distance adjustment between battles
    // encounter steps before a battle; toFIXED(16)
    FIXED                encounterThreshold;
    // Scales the leader's speed into walkingDistance, in 16.16 fixed point: 409 on entry, 0 in a
    // town.
    FIXED                walkingDistanceScale;
    // Distance walked toward the next toFIXED(1), at which the party regains PP, Djinn recover and
    // poison hurts (fdUpdateWalking).
    FIXED                walkingDistance;
    // the metatile the leader last stepped onto
    struct MAP_METATILE *currentMetatile;
    struct MAP_METATILE *prevMetatile; // the metatile before it
    // A SCENE_TRANSITION(style, variant): a TRANSITION_BACKDROP_FADE on entry.
    Sint32               transition;
    Uint16               unused;
    Uint16               transitionActive; // set once the opening transition has started
    // The transition's ticks: MAP_TRANSITION_TICKS on entry and at each script.
    Sint32               transitionFrame;
    Sint32               fastForward; // debug: skip timed script waits and actor messages
    Uint8                unusedAfterFastForward[8];
    Sint16               msg;         // the next message, advanced as each window opens
    // Set to 0xffff by fdScriptStart at each script; nothing reads it.
    Uint16               unusedAfterMsg;
    // Both set to -1 by fdScriptStart at each script; nothing reads them.
    Sint16               unusedBeforeCamera[2];
    struct ACTOR        *camera;     // the actor the map view follows
    Uint8                unusedAfterCamera[16];
    Sint32               speaker;    // the actor speaking; the leader at each script
    struct WINDOW       *msgWindow;  // the open message window, NULL for none
    struct WINDOW       *sideWindow; // the speaker's portrait window
    // The leader's two-row actor placement table (its row and an end row), which
    // fdActorResetForLeader builds.
    ACTOR_PLACE          leaderPlacements[2];
    struct WINDOW       *noticeWindow; // a scene's timed notice
    Uint16               noticeFrame;
    // The palette a field Psynergy's tint fades back to, as a palette fade's target (BG half, then
    // sprite half), saved as the tint starts.
    Uint16               savedFadeTarget[448 * 3];
    // Set while a raised or step cue runs; fdScriptStart then ends the lasting Psynergy.
    Sint16               busy;
    Sint8                revealActive;       // nonzero while Reveal is active
    Sint8                effectTimerRunning; // lets the Reveal countdown advance
    Sint16               revealFrame;        // the ticks Reveal has left
    Sint16               revealCenterX;      // where Reveal was cast, in pixels
    Sint16               revealCenterY;
    // FIELD_PSYNERGY_RING's ring is shown (fdPsynergyRing); A, Select, Start or a shortcut ends it.
    Sint8                ringActive;
    Sint8                ringTimerRunning; // off while another field Psynergy is cast
    Sint16               ringFrame;        // ticks left of the ring; 0 ends it
    // The first of the two names the next yes/no choice shows in place of Yes and No
    // (mnYesNoMenuRun), 0 for those.
    Sint16               choiceMsg;
    Sint16               choiceWidth;               // the choice window's width, 0 for the default
    Sint16               choiceSelection;           // the answer the choice starts on
    Sint8                psynergyActing;            // set while Psynergy takes effect
    Sint8                castStarted;               // set once a cast has been announced
    Sint16               queuedSound;               // a sound fdPlayQueuedMusic plays; -1 for none
    Uint8                unusedAfterQueuedSound[2]; // no code names it
    // the block fdActorLiftTargetBlock lifted, for fdActorPlaceMovedBlock
    struct ACTOR        *movedBlock;
    // The metatiles a Psynergy touches, as metatile coordinates, ended by -1.
    Sint16               psynergyMetatiles[56];
    Sint8                psynergyHitObject;
    Sint8                targetX;
    Sint8                targetY;
    Sint8                targetZ;
    // The item Scoop dug up (or its coins, SCOOPED_COINS | count), given when the leader examines
    // it (SCOOPED_ITEM_ACTOR).
    Sint16               scoopedItem;
    Sint8                sceneAttributeLow;
    Sint8                sceneAttributeHigh;
    Sint8                blazeLongSpray;
    Sint8                hoverReady;  // a party member knows Hover and the ship flies
    Sint8                terrainKind; // an SHIP_TERRAIN_KIND
    Sint8                climbing;
    // The centre of the Cyclone teleporter metatile (kind 2) beside the caster, in pixels, that the
    // Cyclone carries the leader to; -1 for none.
    Sint16               cycloneCarryX;
    Sint16               cycloneCarryZ;
    // The centre of the Cyclone lift metatile (kind 3) beside the caster, in pixels, that the
    // Cyclone lifts the leader onto; -1 for none.
    Sint16               cycloneLiftX;
    Sint16               cycloneLiftZ;
    Sint16               glow;
    Sint16               glowHeld;
    Sint16               standing;
    Uint16               sandTrailPriority;
    Uint16               psynergyDrawFlags;
    Uint8                padding[2];
} FIELD_STATE;

// FIELD_STATE.psynergyDrawFlags, which a scene sets for Pound and Scoop to draw in its room.
#define PSYNERGY_DRAW_NO_SCENE_SPRITE 0x1 // nor Pound's glow on the ground
// The fist or hand at priority 1, not sorted by depth.
#define PSYNERGY_DRAW_FIXED_PRIORITY  0x2

STATIC_ASSERT(sizeof(FIELD_STATE) == 3408);
STATIC_ASSERT(offsetof(FIELD_STATE, cues) == 16);
STATIC_ASSERT(offsetof(FIELD_STATE, actors) == 20);
STATIC_ASSERT(offsetof(FIELD_STATE, stepCue) == 340);
STATIC_ASSERT(offsetof(FIELD_STATE, exit) == 344);
STATIC_ASSERT(offsetof(FIELD_STATE, encounter) == 356);
STATIC_ASSERT(offsetof(FIELD_STATE, raisedCue) == 362);
STATIC_ASSERT(offsetof(FIELD_STATE, blockedSteps) == 388);
STATIC_ASSERT(offsetof(FIELD_STATE, mode) == 394);
STATIC_ASSERT(offsetof(FIELD_STATE, encounterZones) == 396);
STATIC_ASSERT(offsetof(FIELD_STATE, transition) == 428);
STATIC_ASSERT(offsetof(FIELD_STATE, transitionFrame) == 436);
STATIC_ASSERT(offsetof(FIELD_STATE, msg) == 452);
STATIC_ASSERT(offsetof(FIELD_STATE, camera) == 460);
STATIC_ASSERT(offsetof(FIELD_STATE, speaker) == 480);
STATIC_ASSERT(offsetof(FIELD_STATE, leaderPlacements) == 492);
STATIC_ASSERT(offsetof(FIELD_STATE, noticeWindow) == 540);
STATIC_ASSERT(offsetof(FIELD_STATE, savedFadeTarget) == 546);
STATIC_ASSERT(offsetof(FIELD_STATE, busy) == 3234);
STATIC_ASSERT(offsetof(FIELD_STATE, revealActive) == 3236);
STATIC_ASSERT(offsetof(FIELD_STATE, choiceMsg) == 3248);
STATIC_ASSERT(offsetof(FIELD_STATE, psynergyActing) == 3254);
STATIC_ASSERT(offsetof(FIELD_STATE, queuedSound) == 3256);
STATIC_ASSERT(offsetof(FIELD_STATE, psynergyMetatiles) == 3264);
STATIC_ASSERT(offsetof(FIELD_STATE, targetX) == 3377);
STATIC_ASSERT(offsetof(FIELD_STATE, sceneAttributeLow) == 3382);
STATIC_ASSERT(offsetof(FIELD_STATE, hoverReady) == 3385);
STATIC_ASSERT(offsetof(FIELD_STATE, glow) == 3396);
STATIC_ASSERT(offsetof(FIELD_STATE, psynergyDrawFlags) == 3404);

// The running field scene's state, in HEAP_SLOT_FIELD; fdModeRun takes it and frees it.
#define FieldStateWork ((FIELD_STATE *)RamHeapSlots->fieldState)

#endif

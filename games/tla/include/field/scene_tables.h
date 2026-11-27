#ifndef ALCHEMY_SCENE_TABLES_H
#define ALCHEMY_SCENE_TABLES_H

// The scene table formats: the scene table, entrances, exits, actor placements and cues.

#include "fixed_math.h"
#include "scene_exits.h"
#include "scene_ids.h"
#include "types.h"
#include "game/main_routine_state.h"

// A scene's row in ScenesWork: its overlay's resource, its area, its scene type, its map and its
// attributes.
typedef struct SCENE
{
    Sint16 resourceId; // the overlay syOverlayLoad loads into OverlayArea
    // The area: entering a new one clears the FLAG_AREA_TEMP_xx flags and the step counters and
    // moves Retreat's destination (gmArrive).
    Sint8  area;
    // an SCENE_TYPE, copied to MainRoutineStateWork.sceneType
    Sint8  sceneType;
    // the map loaded for it, copied to MainRoutineStateWork.mapId
    Uint16 mapIndex;
    // Two nibbles the field mode reads on arrival (gmFieldGetSceneAttributes).
    Sint16 attributes;
} SCENE;

// Every scene's row, by scene number (field/scenes.s).
extern const SCENE ScenesWork[];

// The sixteen headings an actor faces, in turns from east, clockwise on the screen: the eight
// between the main ones turn an actor a little past a main heading, which picks the side its sprite
// leans to.
typedef enum FACING
{
    FACING_EAST            = 0,
    FACING_EAST_SOUTHEAST  = TRIG_TURN / 16,
    FACING_SOUTHEAST       = DEGtoANG(45),
    FACING_SOUTH_SOUTHEAST = TRIG_TURN * 3 / 16,
    FACING_SOUTH           = DEGtoANG(90),
    FACING_SOUTH_SOUTHWEST = TRIG_TURN * 5 / 16,
    FACING_SOUTHWEST       = TRIG_TURN * 3 / 8,
    FACING_WEST_SOUTHWEST  = TRIG_TURN * 7 / 16,
    FACING_WEST            = DEGtoANG(180),
    FACING_WEST_NORTHWEST  = TRIG_TURN * 9 / 16,
    FACING_NORTHWEST       = TRIG_TURN * 5 / 8,
    FACING_NORTH_NORTHWEST = TRIG_TURN * 11 / 16,
    FACING_NORTH           = TRIG_TURN * 3 / 4,
    FACING_NORTH_NORTHEAST = TRIG_TURN * 13 / 16,
    FACING_NORTHEAST       = TRIG_TURN * 7 / 8,
    FACING_EAST_NORTHEAST  = TRIG_TURN * 15 / 16
} FACING;

// A sixteenth of a turn, half the step between two headings.
enum
{
    FACING_STEP = TRIG_TURN / 16
};

// A sliver of a turn off a heading: an actor turning round to face the opposite way turns by the
// side the nudge leans to.
enum
{
    FACING_NUDGE = 10
};

// The quarter of a turn a facing lies in, what is past it dropped: east, south, west or north.
#define FACING_QUARTER(facing)          ((facing) & 0xc000)
// The eighth of a turn a facing lies in, what is past it dropped.
#define FACING_EIGHTH(facing)           ((facing) & 0xe000)
// The eighth of a turn a facing lies in, as a number from 0 (east) to 7.
#define FACING_EIGHTH_INDEX(facing)     ((facing) >> 13)
// The sixteenth of a turn a facing lies in, what is past it dropped.
#define FACING_SIXTEENTH(facing)        ((facing) & 0xf000)
// The sixteenth of a turn a facing lies in, as a number from 0 (east) to 15, which indexes the
// scenes' sixteen-direction step tables.
#define FACING_INDEX(facing)            ((facing) >> 12)
// The facing at the start of the sixteenth of a turn numbered index.
#define FACING_FROM_INDEX(index)        ((index) << 12)

// The cardinal heading nearest to facing: the shop counters turn a customer this way.
#define FACING_NEAREST_CARDINAL(facing) ((Uint16)(((facing) + DEGtoANG(45)) & ~0x3fff))

// In the first column of a scene table, ends the table.
enum
{
    SCENE_TABLE_END = -1
};

// A table row's condition: CONDITION_ALWAYS, a flag number that must be clear, or IF_FLAG_SET of a
// flag that must be set (fdGameFlagIsConditionActive).
enum
{
    CONDITION_ALWAYS   = -1,
    CONDITION_FLAG_SET = 0x1000 // the flag must be set, not clear
};

// A condition that holds while flag is set.
#define IF_FLAG_SET(flag) ((flag) | CONDITION_FLAG_SET)

// Where the party arrives by one of a scene's entrances, which way the leader faces, and the
// camera's bounds there.
typedef struct SCENE_ENTRANCE
{
    Sint16 entrance;                // the entrance number; SCENE_TABLE_END ends the table
    Sint16 requiredFlag;            // a flag that must be set, or -1 for none
    Sint16 x;                       // pixels
    Sint16 y;                       // pixels
    Sint16 z;                       // pixels
    Uint16 facing;                  // 1/65536 turns (FACING)
    Sint16 unusedAfterFacing;       // no code names it
    Sint16 cameraLeft;              // pixels, -1 for none
    Sint16 cameraTop;               // pixels, -1 for none
    Sint16 cameraRight;             // pixels, -1 for none
    Sint16 cameraBottom;            // pixels, -1 for none
    Sint16 unusedAfterCameraBottom; // no code names it
} SCENE_ENTRANCE;

// An exit table's header word: the scene whose exits follow, named by its name
// (`SCENE_EXITS(Alhafra)`). The table is read by gmTakeExit as the scene ends, with the exit kind
// it ended on.
#define SCENE_EXITS(scene) ((Uint32)&SceneId##scene)
// One exit: exit kind `exit` leads to `entrance` of the scene `scene`. EXIT_ID_ANY matches any
// exit; kind 0 or the next header sends the party to the first exit's scene. The fields are added
// rather than ORed so the scene can be its SceneId symbol, which the linker fills in; they never
// overlap. bits: 27..20 exit kind | 19..12 entrance | 11..0 scene
#define SCENE_EXIT(exit, scene, entrance)                                                          \
    (((Uint32)(exit) << 20) + ((Uint32)(entrance) << 12) + (Uint32)&SceneId##scene)
// An exit word's scene.
#define EXIT_SCENE_MASK       0xfff
// An exit word's entrance.
#define ENTRANCE(word)        (((word) & 0xff000) >> 12)
// An exit word's exit kind.
#define EXIT_ID(word)         (((word) & 0x0ff00000) >> 20)
// An exit word's bit 28: a flag word follows it.
#define EXIT_HAS_FLAG         0x10000000
// The exit kind that matches whatever exit the scene ended on.
#define EXIT_ID_ANY           255
// The exit kind that leaves for the scene and entrance already chosen (a battle, Retreat, a
// script's destination): gmTakeExit keeps them.
#define EXIT_KEEP_DESTINATION 999
// An exit with a flag word after it: the exit is taken unless `flag` is set, and when it is set the
// search goes on to the next exit.
#define SCENE_EXIT_UNLESS(exit, scene, entrance, flag)                                             \
    (SCENE_EXIT(exit, scene, entrance) + EXIT_HAS_FLAG), (Uint32)(flag)
// An exit's flag word of -1 stands for no flag: gmTakeExit takes the exit whatever the flags hold,
// so a chain of exits always ends somewhere.
#define EXIT_FLAG_NONE  0xffffffff
// The last header word: it ends the last scene's exits and the table.
#define SCENE_EXITS_END 511

// An actor a scene places: its sprite, the condition for placing it, how it behaves, where it
// stands and which way it faces, and how it turns to talk.
typedef struct ACTOR_PLACE
{
    Sint16 sprite;    // the actor's sprite; SCENE_TABLE_END ends the table
    Sint16 condition; // CONDITION_ALWAYS, or a flag condition
    // A movement type (MOVEMENT_TYPE) or an actor script, for fdActorSetMovementType.
    Sint32 behavior;
    FIXED  x;          // 16.16 fixed-point pixels
    FIXED  y;          // 16.16 fixed-point pixels
    FIXED  z;          // 16.16 fixed-point pixels
    Uint16 facing;     // 1/65536 turns (FACING)
    Uint8  talkFacing; // ACTOR_TALK_FACING
    Uint8  flags;
} ACTOR_PLACE;

// ACTOR_PLACE.flags: the actor takes the previous actor's sprite resource instead of loading its
// own, when both are single sprites.
#define ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE 0x01
// ACTOR_PLACE.flags: x is the number of a scene point to stand on, not a position
// (fdFindActorPlacement).
#define ACTOR_PLACEMENT_AT_SCENE_POINT        0x02

// One row of a scene's actor placements, its spot in plain pixels.
#define ACTOR_PLACEMENT(sprite, condition, movement, x, y, z, facing, talkFacing, flags)           \
    {sprite, condition, movement, toFIXED(x), toFIXED(y), toFIXED(z), facing, talkFacing, flags}

// The movement types an actor placement's behavior can name, which fdActorSetMovementType turns
// into scripts.
typedef enum MOVEMENT_TYPE
{
    MOVEMENT_TYPE_STAND              = 1, // its script only ends
    MOVEMENT_TYPE_WANDER             = 2,
    MOVEMENT_TYPE_WANDER_NEAR        = 3,
    MOVEMENT_TYPE_WANDER_CLOSE       = 4,
    MOVEMENT_TYPE_FOLLOW_WATCHED     = 5, // moves toward the watched actor
    MOVEMENT_TYPE_FACE_LEADER        = 6, // watches the leader and turns toward it
    // MovementTypeClearCollisionScript: clears the actor's child and collision flags
    // (fdActorClearSpriteAndCollisionFlags).
    MOVEMENT_TYPE_CLEAR_COLLISION    = 7,
    // MovementTypeSquashScript: squashes and stretches for ever.
    MOVEMENT_TYPE_SQUASH             = 8,
    // MovementTypeTakeLeaderLayerScript
    MOVEMENT_TYPE_TAKE_LEADER_LAYER  = 9,
    MOVEMENT_TYPE_IGNORE_RISES       = 10, // MovementTypeIgnoreRisesScript
    // MovementTypeClearSpriteFlagsScript
    MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS = 11
} MOVEMENT_TYPE;

// How a placed actor turns when the leader talks to it (fdTalkToActor). With all but
// TALK_FACING_KEEP the leader stops and each watches the other.
typedef enum ACTOR_TALK_FACING
{
    TALK_FACING_PARTY            = 0,
    // Faces the party, then turns back to its old heading.
    TALK_FACING_PARTY_AND_BACK   = 1,
    TALK_FACING_KEEP             = 2,
    // Faces the party, then keeps turning to face the leader once the talk is over
    // (ActorFaceWatchedScript).
    TALK_FACING_PARTY_THEN_WATCH = 3
} ACTOR_TALK_FACING;

// A row of a scene's cue table.
typedef struct CUE
{
    // The kind and its detail; CUE_TABLE_END ends the table. Item cues read it as bits: 15..8 item
    // | 4 on an actor | 3..0 kind (4)
    Uint32 control;
    // What the cue answers: an actor id or a tile code. Item cues read it as bits: 15..12 facing |
    // 11 needs facing | 7..0 target
    Sint16 subject;
    Sint16 condition; // CONDITION_ALWAYS, or a flag condition
    Uint32 value;     // a message (FIND_MESSAGE) or a script (SCRIPT_REF)
} CUE;

// CUE.subject bit 10: a Psynergy cue runs in place of its field

// Psynergy, as Mind Read on someone who notices it does; an item cue's script shows its own message
// in place of the "uses" message (MsgActorUsesItem); an examine cue shows no checking message.
#define CUE_SUBJECT_OWN_EFFECT   0x400
// The examine code the search finds aboard the ship facing open sea (World Map collision 3), for
// the World Map's examine cue on it (scWorldMapGazeIntoPond).
#define SHIP_SEA_EXAMINE_CODE    99
// CUE.subject bit 9 on a Psynergy cue: the cast neither flashes the caster nor announces the
// ability (FIELD_EFFECT_STATE.flashCaster and .announceName).
#define CUE_SUBJECT_QUIET        0x200
// An item cue's subject: the heading it needs in bits 15..12, whether it needs one, and its target,
// an actor or a metatile behavior.
#define CUE_SUBJECT_FACING       0xf000
#define CUE_SUBJECT_NEEDS_FACING 0x0800
#define CUE_SUBJECT_TARGET       0xff
// A cue's subject that answers only while the leader faces `facing`.
#define CUE_FACING_SUBJECT(facing, target)                                                         \
    ((Sint16)((facing) | CUE_SUBJECT_NEEDS_FACING | (target)))
// A cue's value below this is a message, from it a script.
#define CUE_RESULT_MESSAGE_END 65536

// What a search finds, an examine cue's value: an item, coins or a message.
#define FIND_ITEM(item)        (0x00100000 | (item))
#define FIND_COINS(coins)      (0x00200000 | (coins))
#define FIND_MESSAGE(message)  (0x00400000 | (message))
// A search result's type, its top twelve bits (FIND_ITEM to FIND_MESSAGE).
#define FIND_TYPE_MASK         0xfff00000
// Bits 27..24 of a search result, set only in a function's address: the result is then a function
// to call.
#define FIND_FUNCTION_BITS     0x0f000000
#define SCRIPT_REF(event)      ((Uint32)(event))
#define CUE_TABLE_END          0xffffffff

// A cue as the field reads it while the scene runs: the target is one byte.
typedef struct CUE_VIEW
{
    Sint32 flags;            // the low 4 bits are the kind (CUE_KIND_MASK)
    Uint8  value;            // the target (an actor id or a metatile behavior)
    Uint8  unusedAfterValue; // no code names it
    Sint16 condition;        // as for fdGameFlagIsConditionActive
    Sint32 result;
} CUE_VIEW;

// A cue's control bits 3..0, its kind.
#define CUE_KIND_MASK           0xf
// A cue's control bits 15..12: the heading CUE_FLAG_FACING compares, in sixteenths of a turn.
#define CUE_FACING_MASK         0xf000
// A step cue that needs the leader to have pushed against it for more than FIELD_PUSH_TICKS, or 2
// ticks with STEP_CUE_FLAG_QUICK.
#define STEP_CUE_FLAG_WALKING   0x200
// With STEP_CUE_FLAG_WALKING, 2 ticks of pushing are enough.
#define STEP_CUE_FLAG_QUICK     0x800
// A script that needs the leader to face within an eighth of a turn of the heading in control
// bits 15..12.
#define CUE_FLAG_FACING         0x400
#define STEP_CUE_WALKING        (CUE_STEP | STEP_CUE_FLAG_WALKING)
// facing is an FACING; only its top four bits are compared.
#define STEP_CUE_FACING(facing) (CUE_STEP | CUE_FLAG_FACING | (facing))
#define STEP_CUE_WALKING_INTO(facing)                                                              \
    (CUE_STEP | STEP_CUE_FLAG_WALKING | CUE_FLAG_FACING | (facing))
#define EXAMINE_CUE(target)        (CUE_EXAMINE | (target) << 4)
// An examine cue searched while facing the given way, as a counter is searched across.
#define EXAMINE_CUE_FACING(facing) (CUE_EXAMINE | CUE_FLAG_FACING | (facing))
// A talk cue that answers only while the leader faces the given way, so one actor can answer
// differently from each side.
#define TALK_CUE_FACING(facing)    (CUE_TALK | CUE_FLAG_FACING | (facing))

// An exit cue's control bits 6..4: how the leader leaves (fdRunStepCueOrExit).
typedef enum EXIT_STYLE
{
    EXIT_STYLE_NORMAL       = 0x00, // with SE_MAP_EXIT
    EXIT_STYLE_SILENT       = 0x10,
    // With SE_STAIRS_UP and ANIM_LEADER_LEAVE; the next scene plays ANIM_LEADER_ARRIVE
    // (FLAG_ARRIVAL_STAIRS_UP).
    EXIT_STYLE_STAIRS_UP    = 0x20,
    // With SE_STAIRS_DOWN and ANIM_LEADER_LEAVE_ALTERNATE; the next scene plays
    // ANIM_LEADER_ARRIVE_ALTERNATE (FLAG_ARRIVAL_STAIRS_DOWN).
    EXIT_STYLE_STAIRS_DOWN  = 0x30,
    // With SE_MAP_EXIT and a hidden layer on the leader's sprite (fdExitAddHiddenLeaderLayer).
    EXIT_STYLE_HIDDEN_LAYER = 0x50,
    EXIT_STYLE_MASK         = 0x70
} EXIT_STYLE;

// What an examine cue is on, EXAMINE_CUE()'s control bits 8..4. Target n shows message
// MsgTileChecked + n, "<leader> checked the ...", as the leader checks it, unless the cue has
// CUE_SUBJECT_OWN_EFFECT; target 0 shows nothing.
typedef enum EXAMINE
{
    EXAMINE_SILENT = 0,
    EXAMINE_CHEST  = 1,
    EXAMINE_JAR,
    EXAMINE_BARREL,
    EXAMINE_WALL,
    EXAMINE_GROUND,
    EXAMINE_ROCK,
    EXAMINE_HOLE,
    EXAMINE_GRAVE,
    EXAMINE_TREE,
    EXAMINE_UNDERBRUSH,
    EXAMINE_DOOR,
    EXAMINE_CHIMNEY,
    EXAMINE_WOODEN_BOX,
    EXAMINE_BED,
    EXAMINE_BOOKCASE,
    EXAMINE_STONE_COFFIN,
    EXAMINE_FIREPLACE,
    EXAMINE_WATER,
    EXAMINE_STONE_PILLAR,
    EXAMINE_STALACTITE,
    EXAMINE_BOARDS,
    EXAMINE_FOUNTAIN,
    EXAMINE_OVEN,
    EXAMINE_TABLE,
    EXAMINE_STONE_STATUE,
    EXAMINE_STONE_TABLET,
    EXAMINE_SHELF,
    EXAMINE_WARDROBE,
    EXAMINE_FIREWOOD,
    EXAMINE_BOOKS,
    EXAMINE_WELL,
    EXAMINE_POND
} EXAMINE;

#include "ability_ids.h"

// Actor ids: 0 to 7 the party's characters, the placed actors from 8. A party member's actor takes
// its character's number as its actor id.
typedef enum ACTOR_ID
{
    ACTOR_ISAAC,
    ACTOR_GARET,
    ACTOR_IVAN,
    ACTOR_MIA,
    ACTOR_FELIX,
    ACTOR_JENNA,
    ACTOR_SHEBA,
    ACTOR_PIERS,
    ACTOR_FIRST_PLACED
} ACTOR_ID;

// CUE.kind's low four bits: what the cue answers.
typedef enum CUE_KIND
{
    CUE_TALK        = 0, // the A button on an actor; the subject is its id
    // Stepping onto a metatile whose metatile behavior is the subject, when no step cue answers it;
    // value is the exit kind.
    CUE_EXIT        = 1,
    CUE_STEP        = 2, // stepping onto or pushing against a metatile behavior
    CUE_EXAMINE     = 3, // the A button on what is ahead, when it is no actor
    CUE_ITEM        = 4, // an item used on a metatile or an actor
    CUE_PSYNERGY    = 5,
    CUE_RAISED      = 6, // raised by code
    CUE_TALK_HALTED = 7, // talking to an actor Halt has stopped
    // Pushing an actor ahead, for any actor: it runs, with the keys and the actor's id, before the
    // leader pushes (fdRunPushMetatile).
    CUE_PUSH        = 8,
    CUE_PUSHED      = 9 // as CUE_PUSH, once the leader has pushed
} CUE_KIND;

// When a Psynergy cue runs around its field Psynergy, control bits 31..28.
typedef enum PSYNERGY_PHASE
{
    // After the field Psynergy, PSYNERGY_ON_ACTOR's phase.
    PSYNERGY_PHASE_AFTER_CAST  = 0,
    PSYNERGY_PHASE_BEFORE_CAST = 1, // once the PP is paid, before the cast
                                    // After a Psynergy that runs out by itself has ended.
    PSYNERGY_PHASE_AFTER_END   = 2,
    PSYNERGY_PHASE_BEFORE_END  = 3, // as PSYNERGY_PHASE_AFTER_END, before it
    PSYNERGY_PHASE_FADES       = 4, // as Reveal's colours come back
    PSYNERGY_PHASE_ACTS        = 5, // run by the field Psynergy as it acts
    PSYNERGY_PHASE_HITS        = 6, // as Blaze's fire reaches what it is aimed at
                             // Before every other phase, as soon as the Psynergy is chosen and
                             // Retreat has asked its question.
    PSYNERGY_PHASE_FIRST       = 8,
    PSYNERGY_PHASE_ANY         = 15 // fdFindPsynergyCue matches every phase
} PSYNERGY_PHASE;

// The phase a field Psynergy's state is left in once it has finished, which the cast waits for; its
// acting scripts get it as their stage the last time they run.
#define FIELD_PSYNERGY_FINISHED    999
// The stage Pound's acting scripts get: 0 until the fist lands, then POUND_STAGE_LANDED.
#define POUND_STAGE_LANDED         1
// The stages Scoop's acting scripts get on a tick of the dig, 0 on the rest: SCOOP_STAGE_DUG on the
// tick the ground is dug (SCOOP_DUG_TICK), and SCOOP_STAGE_DUG_UP on tick 50, as what was buried
// comes up.
#define SCOOP_STAGE_DUG            1
#define SCOOP_STAGE_DUG_UP         2

// A Psynergy cue's phase bits, 31..28.
#define PSYNERGY_PHASE_SHIFT       28
#define PSYNERGY_PHASE_MASK        (15u << PSYNERGY_PHASE_SHIFT)
// The control word of a Psynergy cue (CUE_PSYNERGY) in the given phase, as fdFindPsynergyCue
// compares it (bits 31..28 phase | 3..0 kind).
#define PSYNERGY_CUE(phase)        ((Uint32)(phase) << PSYNERGY_PHASE_SHIFT | CUE_PSYNERGY)
#define PSYNERGY_CUE_ANY           PSYNERGY_CUE(PSYNERGY_PHASE_ANY)

// A Psynergy cue on the actor the leader faces.
#define CUE_PSYNERGY_ON_ACTOR      0x10
#define PSYNERGY_ON_ACTOR(ability) (CUE_PSYNERGY | CUE_PSYNERGY_ON_ACTOR | (ability) << 8)
// A Psynergy cue on an actor at one phase of the cast, as PSYNERGY_ANYWHERE is for any target.
#define PSYNERGY_PHASE_ON_ACTOR(ability, phase)                                                    \
    ((Uint32)(phase) << PSYNERGY_PHASE_SHIFT | PSYNERGY_ON_ACTOR(ability))

// What fdSelectPsynergyTarget chooses: a metatile behavior as it is, an actor's id with
// PSYNERGY_TARGET_ACTOR, or PSYNERGY_TARGET_ANYWHERE.
typedef enum PSYNERGY_TARGET
{
    PSYNERGY_TARGET_ACTOR    = 0x100,
    PSYNERGY_TARGET_ANYWHERE = 0x200,
    PSYNERGY_TARGET_KIND     = 0xff00, // which of the three
    PSYNERGY_TARGET_ID       = 0xff    // the metatile behavior or actor
} PSYNERGY_TARGET;

// A Psynergy cue on the metatile behavior ahead, at one phase of the cast.
#define PSYNERGY_PHASE_ON_METATILE(ability, phase)                                                 \
    ((Uint32)(phase) << PSYNERGY_PHASE_SHIFT | (ability) << 8 | CUE_PSYNERGY)

// Using an item on a metatile behavior, or on an actor; the item is an ITEM_ID (item_ids.h) or an
// ITEM_FILTER_ value.
#define CUE_ITEM_ON_ACTOR       0x10
#define USE_ITEM(item)          (CUE_ITEM | (item) << 8)
#define USE_ITEM_ON_ACTOR(item) (CUE_ITEM | CUE_ITEM_ON_ACTOR | (item) << 8)

// A Psynergy cue for any target, not a particular actor or metatile.
#define CUE_PSYNERGY_ANYWHERE   0x80
#define PSYNERGY_ANYWHERE(ability, phase)                                                          \
    ((Uint32)(phase) << PSYNERGY_PHASE_SHIFT | (ability) << 8 | CUE_PSYNERGY_ANYWHERE              \
     | CUE_PSYNERGY)

// A scene overlay's own entrances, actor placements and cues, which its getters return.
extern SCENE_ENTRANCE SceneEntrances[];
extern ACTOR_PLACE    ActorPlacements[];
extern CUE            SceneCues[];

// A scene's exit region: the box that, while its condition holds, takes the party leader out
// through the exit whose number is its value (fdCheckExitRegions).
typedef struct SCENE_REGION
{
    // Its lower corner in pixels, inclusive; a minX of SCENE_TABLE_END ends the table.
    Sint16 minX, minY, minZ;
    Sint16 maxX, maxY, maxZ; // its upper corner in pixels, exclusive
    // CONDITION_ALWAYS, a flag | CONDITION_FLAG_SET set, else the flag clear
    Sint16 condition;
    Sint16 value; // the exit kind, stored in FIELD_STATE's exit
} SCENE_REGION;

// The table a scene overlay starts with (OverlayArea): a stub word and an entry point for each of
// its tables.
typedef struct SCENE_ENTRY_TABLE
{
    Uint32                initializeStub;
    Sint32                (*initialize)(void);
    Uint32                entrancesStub;
    const SCENE_ENTRANCE *(*entrances)(void);
    Uint32                exitsStub;
    const Uint32         *(*exits)(void);
    Uint32                placementsStub;
    const ACTOR_PLACE    *(*placements)(void);
    Uint32                cuesStub;
    const CUE            *(*cues)(void);
    Uint32                regionsStub;
    const SCENE_REGION   *(*regions)(void);
    Uint32                prepareMapStub;
    // Run once the scene's actors are placed, on a map other than the World Map: the scene's own
    // edits to its map before it is shown.
    Sint32                (*prepareMap)(void);
} SCENE_ENTRY_TABLE;

// The bit that marks a song in EncounterBattleMusic; encounter keys have it clear.
#define ENCOUNTER_BATTLE_MUSIC_MARKER 0x8000
// A song's row in EncounterBattleMusic, before the encounters that take it: the song (a MUS_ id)
// with ENCOUNTER_BATTLE_MUSIC_MARKER set.
#define ENCOUNTER_BATTLE_MUSIC(song)  (Sint16)(ENCOUNTER_BATTLE_MUSIC_MARKER | (song))
// An encounter's row in EncounterBattleMusic: slot of encounter zone.
#define ENCOUNTER_KEY(zone, slot)     (((zone) << 4) + (slot))

#endif

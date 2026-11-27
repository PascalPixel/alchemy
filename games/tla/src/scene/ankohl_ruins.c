// The Ankohl Ruins, where Felix, once Gaia Rock has given him Sand, climbs through the old ruins to
// the Left Prong of the Trident of Ankohl lying on their top. Past the forest at the doorway, the
// halls of the stone faces come first: Whirlwind clears their weeds, some of which hide the way on,
// and Move pushes each of the seven stone faces in the north wall off its ledge to open the doorway
// behind it. In the Thanatos Mace's rooms Move pushes the left half of a stone face against its
// right half and sand pours down into a slope. Move also pushes pillars aside, one by the Nut's
// chest, one in the great face's rooms, three in the face halves' rooms and one in the Crystal
// Powder's rooms. In each of the two sand rooms a switch makes the great stone face in the north
// wall pour sand until the room is full, and a button on the far ledge, reached on the sand, opens
// the way on. In the face halves' rooms flowing sand carries Felix down a slide. In the great
// face's rooms Move pushes the right half of the great face into place, its eyes glow, and stepping
// into its corridor sets it sliding down at Felix, who hides from it under the Sand. In the Crystal
// Powder's rooms Reveal shows a gap that Move pushes a half face into; it lands in the face halves'
// rooms, where Move pushes it against the other half and sand pours into a slope. From the Crystal
// Powder's rooms a way leads out onto the top of the ruins.
#include "../field/actor_set_sprite_priority.c"
#include "field/actor_script.h"
#include "field/script.h"
#include "macros.h"

#include "../field/actor_create_silhouette.c"
#include "game/game.h"
#include "field/field_mode.h"
#include "scene/scene.h"
#include "edition.h"
#include "field/script_inline.h"
#include "field/call.h"

// The seven stone faces Move pushes off their ledge in the halls of the stone faces, one flag a
// face; the macro takes the face's actor.
#define FLAG_STONE_FACE_A_MOVED            0x9d2
#define FLAG_STONE_FACE_MOVED(face)        (FLAG_STONE_FACE_A_MOVED - ACTOR_STONE_FACE_A + (face))
// The ruins' weeds Whirlwind scatters, one flag a clump: the fourteen of the halls of the stone
// faces from FLAG_STONE_FACES_WEEDS_A_SCATTERED, the Thanatos Mace's rooms' three, then the face
// halves' rooms' seven; the macros take the clump's actor.
#define FLAG_STONE_FACES_WEEDS_A_SCATTERED 0x990
#define FLAG_THANATOS_WEEDS_A_SCATTERED    0x99e
#define FLAG_FACE_HALVES_WEEDS_A_SCATTERED 0x9a1
#define FLAG_STONE_FACES_WEEDS_SCATTERED(weeds)                                                    \
    (FLAG_STONE_FACES_WEEDS_A_SCATTERED - ACTOR_STONE_FACES_WEEDS_A + (weeds))
#define FLAG_THANATOS_WEEDS_SCATTERED(weeds)                                                       \
    (FLAG_THANATOS_WEEDS_A_SCATTERED - ACTOR_THANATOS_WEEDS_A + (weeds))
#define FLAG_FACE_HALVES_WEEDS_SCATTERED(weeds)                                                    \
    (FLAG_FACE_HALVES_WEEDS_A_SCATTERED - ACTOR_FACE_HALVES_WEEDS_A + (weeds))
// The pillars Move has pushed to the end of their tracks: the one by the Nut's chest, the great
// face's rooms' one, the face halves' rooms' three and the Crystal Powder's rooms' one.
#define FLAG_NUT_CHEST_PILLAR_MOVED          0x9c6
#define FLAG_GREAT_FACE_ROOMS_PILLAR_MOVED   0x9c7
#define FLAG_FACE_HALVES_PILLAR_MOVED        0x9c8
#define FLAG_FACE_HALVES_SECOND_PILLAR_MOVED 0x9c9
#define FLAG_FACE_HALVES_THIRD_PILLAR_MOVED  0x9ca
#define FLAG_CRYSTAL_POWDER_PILLAR_MOVED     0x9cb
// The halves of stone faces pushed into place: the face halves' rooms' two, the half pushed into
// the Crystal Powder's rooms' gap, the Thanatos Mace's rooms' half, and the great face's right
// half.
#define FLAG_LEFT_FACE_HALF_PLACED           0x9d9
#define FLAG_RIGHT_FACE_HALF_PLACED          0x9da
#define FLAG_FACE_HALF_PUSHED_INTO_GAP       0x9db
#define FLAG_THANATOS_FACE_HALF_PLACED       0x9dc
#define FLAG_GREAT_FACE_HALF_PLACED          0x9dd
// The doors the buttons on the sand rooms' far ledges open, the first room's and the second's.
#define FLAG_FIRST_SAND_DOOR_OPENED          0x9e2
#define FLAG_SECOND_SAND_DOOR_OPENED         0x9e3
// The sand rooms' switches pressed, for this visit: the sand is rising or has risen in each.
#define FLAG_FIRST_SAND_SWITCH_PRESSED       FLAG_SCENE_TEMP_02
#define FLAG_SECOND_SAND_SWITCH_PRESSED      FLAG_SCENE_TEMP_03
// Set while Reveal is up: the sand rooms stop trimming the sand's edge, and in every edition but
// the Japanese the Crystal Powder's rooms' gap lies open to a pushed half face.
#define FLAG_ANKOHL_RUINS_REVEAL_SHOWN       FLAG_SCENE_TEMP_05
// Set once the great face has slid to the foot of its corridor, for this visit: the corridor stays
// closed behind it.
#define FLAG_GREAT_FACE_AT_REST              FLAG_SCENE_TEMP_01
// Set once a sand room's sand has risen to the top, for this visit.
#define FLAG_SAND_ROOM_FULL                  FLAG_SCENE_TEMP_04

// The metatile behaviours the rooms' cue tables answer: the foot of the great face's corridor and
// the metatile right in front of the face at its top; the sand rooms' switches and their far
// ledges' buttons; and the pit in the Crystal Powder's rooms Felix jumps down into the face halves'
// rooms.
enum
{
    METATILE_BEHAVIOR_GREAT_FACE_CORRIDOR_FOOT = 40,
    METATILE_BEHAVIOR_BEFORE_GREAT_FACE        = 41,
    METATILE_BEHAVIOR_FIRST_SAND_SWITCH        = 40,
    METATILE_BEHAVIOR_FIRST_SAND_DOOR_BUTTON   = 41,
    METATILE_BEHAVIOR_SECOND_SAND_SWITCH       = 42,
    METATILE_BEHAVIOR_SECOND_SAND_DOOR_BUTTON  = 43,
    METATILE_BEHAVIOR_PIT_TO_FACE_HALVES       = 13
};

// The cues the rooms' code raises: the flowing sand carrying Felix off a ledge, and each sand
// room's next rise.
enum
{
    CUE_CARRIED_OFF_LEDGE = 66,
    CUE_RAISE_FIRST_SAND  = 68,
    CUE_RAISE_SECOND_SAND = 69
};

// The attribute bit that marks the metatiles whose sand flows south here
// (scAnkohlRuinsFlowWithSand); the field engine reads it elsewhere as MAP_METATILE_SECOND_ZONE.
#define MAP_METATILE_FLOWING_SAND MAP_METATILE_SECOND_ZONE

// The metatile column each pillar Move pushes stands in at its track's end, where its flag is set
// and the set-up places it again.
enum
{
    NUT_CHEST_PILLAR_END_COLUMN          = 4,
    GREAT_FACE_ROOMS_PILLAR_END_COLUMN   = 32,
    FACE_HALVES_PILLAR_END_COLUMN        = 2,
    FACE_HALVES_SECOND_PILLAR_END_COLUMN = 39,
    FACE_HALVES_THIRD_PILLAR_END_COLUMN  = 38,
    CRYSTAL_POWDER_PILLAR_END_COLUMN     = 17
};

// The row below the stone faces' ledge: a face pushed into it falls.
#define STONE_FACE_DROP_ROW 31

// The metatile of the Crystal Powder's rooms' gap the half face drops into.
#define GAP_COLUMN          26
#define GAP_ROW             35

// The Thanatos Mace's rooms' entrances up the stairs from their south hall, east and west, into the
// hall of the face half.
enum
{
    THANATOS_FACE_HALL_FROM_EAST_STAIRS = 15,
    THANATOS_FACE_HALL_FROM_WEST_STAIRS = 17
};

// The face halves' rooms' first entrance into the hall of the halves, where the sand flows; the
// hall's entrances are numbered on from it.
#define FACE_HALVES_HALL_FIRST_ENTRANCE                                                            \
    ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST
// The hall's last entrance where the placed halves sound as the party arrives: in every edition but
// the Japanese the way down from the Crystal Powder chest's pit too.
#if FACE_HALVES_CHIME_ON_PIT_ARRIVAL
#    define FACE_HALVES_HALL_SOUND_LAST_ENTRANCE                                                   \
        ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTHWEST
#else
#    define FACE_HALVES_HALL_SOUND_LAST_ENTRANCE                                                   \
        ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_B
#endif

// The moves a rise of the sand takes, and the sand rows' end marker.
#define SAND_RISE_MOVES 16
#define SAND_ROWS_END   99

// The sprite the sand's edge masks the screen with: 16 by 16 pixels, four tiles of colour 1.
#define SAND_MASK_BYTES (4 * TILE_SIZE_4BPP)

// How each of a sand room's masks moves as the sand rises (scAnkohlRuinsDrawSand): out west or east
// along a ledge's edge, up and out of the way once the sand is 16 pixels deep, or straight up until
// then.
typedef enum ANKOHL_RUINS_SAND_MASK
{
    SAND_MASK_WEST_SHALLOW,
    SAND_MASK_WEST,
    SAND_MASK_EAST,
    SAND_MASK_EAST_SHALLOW,
    SAND_MASK_UP_SHALLOW
} ANKOHL_RUINS_SAND_MASK;

// The halls of the stone faces (AnkohlRuinsStoneFacesPlacements).
typedef enum ANKOHL_RUINS_STONE_FACES_ACTOR_ID
{
    // The pillar beside the Nut's chest, in the room behind the westmost face.
    ACTOR_NUT_CHEST_PILLAR = ACTOR_FIRST_PLACED,
    // The seven stone faces in the north wall, west to east.
    ACTOR_STONE_FACE_A     = ACTOR_FIRST_PLACED + 2,
    ACTOR_STONE_FACE_B,
    ACTOR_STONE_FACE_C,
    // Unused: placed at (0, 0), off the room; at its place in the row the wall's own face stands
    // split open on a doorway.
    ACTOR_STONE_FACE_D,
    ACTOR_STONE_FACE_E,
    ACTOR_STONE_FACE_F,
    ACTOR_STONE_FACE_G,
    // The fourteen clumps of weeds.
    ACTOR_STONE_FACES_WEEDS_A,
    ACTOR_STONE_FACES_WEEDS_B,
    ACTOR_STONE_FACES_WEEDS_C,
    ACTOR_STONE_FACES_WEEDS_D,
    ACTOR_STONE_FACES_WEEDS_E,
    ACTOR_STONE_FACES_WEEDS_F,
    ACTOR_STONE_FACES_WEEDS_G,
    ACTOR_STONE_FACES_WEEDS_H,
    ACTOR_STONE_FACES_WEEDS_I,
    ACTOR_STONE_FACES_WEEDS_J,
    ACTOR_STONE_FACES_WEEDS_K,
    ACTOR_STONE_FACES_WEEDS_L,
    ACTOR_STONE_FACES_WEEDS_M,
    ACTOR_STONE_FACES_WEEDS_N,
} ANKOHL_RUINS_STONE_FACES_ACTOR_ID;

// The Thanatos Mace's rooms (AnkohlRuinsThanatosMaceChestPlacements).
typedef enum ANKOHL_RUINS_THANATOS_MACE_CHEST_ACTOR_ID
{
    // The left half of a stone face, pushed into place.
    ACTOR_THANATOS_FACE_HALF = ACTOR_FIRST_PLACED,
    // Its three clumps of weeds.
    ACTOR_THANATOS_WEEDS_A,
    ACTOR_THANATOS_WEEDS_B,
    ACTOR_THANATOS_WEEDS_C
} ANKOHL_RUINS_THANATOS_MACE_CHEST_ACTOR_ID;

// The great face's rooms (AnkohlRuinsGreatFacePlacements).
typedef enum ANKOHL_RUINS_GREAT_FACE_ACTOR_ID
{
    ACTOR_GREAT_FACE_ROOMS_PILLAR = ACTOR_FIRST_PLACED,
    // The great stone face, which slides down its corridor once its right half is in place.
    ACTOR_GREAT_FACE,
    // Its right half, which Move pushes against it.
    ACTOR_GREAT_FACE_HALF
} ANKOHL_RUINS_GREAT_FACE_ACTOR_ID;

// The face halves' rooms (AnkohlRuinsFaceHalvesPlacements).
typedef enum ANKOHL_RUINS_FACE_HALVES_ACTOR_ID
{
    ACTOR_FACE_HALVES_PILLAR = ACTOR_FIRST_PLACED,
    // The two halves of a stone face: the left one stands in the room, the right one falls in
    // through the Crystal Powder's rooms' gap.
    ACTOR_LEFT_FACE_HALF,
    ACTOR_RIGHT_FACE_HALF,
    // Its seven clumps of weeds.
    ACTOR_FACE_HALVES_WEEDS_A,
    ACTOR_FACE_HALVES_WEEDS_B,
    ACTOR_FACE_HALVES_WEEDS_C,
    ACTOR_FACE_HALVES_WEEDS_D,
    ACTOR_FACE_HALVES_WEEDS_E,
    ACTOR_FACE_HALVES_WEEDS_F,
    ACTOR_FACE_HALVES_WEEDS_G,
    ACTOR_FACE_HALVES_SECOND_PILLAR,
    ACTOR_FACE_HALVES_THIRD_PILLAR
} ANKOHL_RUINS_FACE_HALVES_ACTOR_ID;

// The Crystal Powder's rooms (AnkohlRuinsCrystalPowderChestPlacements).
typedef enum ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_ACTOR_ID
{
    ACTOR_CRYSTAL_POWDER_PILLAR = ACTOR_FIRST_PLACED,
    // The half face pushed into the gap Reveal shows.
    ACTOR_FACE_HALF_FOR_THE_GAP
} ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_ACTOR_ID;

// The Left Prong, lying on the top of the ruins.
#define ACTOR_LEFT_PRONG_CHEST 64

void                 scAnkohlRuinsRaiseFirstSand(void);
void                 scAnkohlRuinsRaiseSecondSand(void);
void                 scAnkohlRuinsSlideGreatFace(void);

// Whether the game flag is set.
static inline Sint32 scAnkohlRuinsFlagCheck(Sint32 flag)
{
    return syGameFlagGet(flag);
}

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE   fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

ANKOHL_RUINS_SPR_STATE AnkohlRuinsSand BSS_DATA ALIGNED(16);

const SCENE_ENTRANCE                           *scGetEntrances(void)
{
    return SceneEntrances;
}

extern Sint16       AnkohlRuinsSandStepDelay;
extern Sint16       AnkohlRuinsSandTimer;
extern Sint16       AnkohlRuinsSandRiseDelay;
extern Sint32       AnkohlRuinsSandStep;
extern Sint32       AnkohlRuinsSandRoom;

extern SCENE_REGION AnkohlRuinsCrystalPowderChestExitRegions[];
extern SCENE_REGION AnkohlRuinsLeftProngExitRegions[];

const SCENE_REGION *scGetExitRegions(void)
{
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST)
        return AnkohlRuinsCrystalPowderChestExitRegions;
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_LEFT_PRONG)
        return AnkohlRuinsLeftProngExitRegions;
    return 0;
}

const Uint32 *scGetExits(void)
{
    return SceneExits;
}

// Jumps actor `id` down a ledge (fdActorJumpDownLedge).
static __inline__ void scAnkohlRuinsJumpActorDownLedge(Sint32 id, Sint32 pose, Sint32 param)
{
    fdActorJumpDownLedge(id, pose, param);
}

// Felix walks south into the pit in the middle of a small room of the Crystal Powder's rooms and
// jumps down it; the screen fades and he drops into the face halves' rooms beside the left half of
// the stone face, landing in a ring of dust (scAnkohlRuinsLandFromFall).
void scAnkohlRuinsJumpDownLedge(void)
{
    scFieldActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 2);
    scAnkohlRuinsJumpActorDownLedge(
        MainRoutineStateWork.leaderActorId, ANIM_LEADER_SPRING, LEDGE_JUMP_NO_LANDING_WAIT);
    fdScriptPlaySound(SE_FALL);
    fdScriptSetFlag(FLAG_ARRIVE_FALLING);
    syFieldScriptSetExitRequest(
        ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_SOUTHWEST);
}

// Includes the shared fdEffectAdvanceMotion in this scene. Neither it nor its caller
// scAnkohlRuinsLandFromFall exists in the Japanese scene.
#if PIT_FALL_DUST_LANDING
#    include "../field/field_effect_advance_motion.c"
#endif

#if PIT_FALL_DUST_LANDING

// Felix drops into the face halves' rooms from the pit above, lands sitting in a ring of dust and
// shakes the ground, then stands up. In every edition but the Japanese.
void scAnkohlRuinsLandFromFall(void)
{
    ACTOR         *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
    EFFECT_OPTIONS options;
    Uint32         i;
    Sint32         angle;
    FIXED          velocity[3];

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    fdMapGridCenterOnFollowedPoint();
    syWait(1);
    leader->y.fixed            = toFIXED(130);
    leader->gravity            = toFIXED(1) / 2;
    leader->bounce             = 0;
    leader->motionFlags        = 0;
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptPlaySound(SE_DUST_PUFF);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    syFieldScriptWait(24);
    options.palette     = SPRITE_COLOR_WHITE;
    options.update      = fdEffectAdvanceMotion;
    options.startScaleX = toFIXED(16) / 25;
    options.startScaleY = toFIXED(16) / 25;
    i                   = 0;
    do
    {
        angle        = i * (TRIG_TURN / DUST_RING_PUFFS);
        velocity[0]  = syCos(angle);
        velocity[1]  = 0;
        velocity[2]  = syMathSin(angle);
        velocity[0] += velocity[0] >> 1;
        velocity[0] += syRandomBelow(toFIXED(3) / 4) - (toFIXED(3) / 8);
        velocity[2] += syRandomBelow(toFIXED(3) / 8) - (toFIXED(3) / 16);
        scFieldEffectSpawn(leader->x.fixed,
                           leader->y.fixed,
                           leader->z.fixed,
                           velocity[0],
                           velocity[1],
                           velocity[2],
                           EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE
                               | EFFECT_SCRIPT_MEDIUM,
                           &options);
        i++;
    } while (i <= DUST_RING_PUFFS);
    fdScriptPlaySound(SE_THUMP);
    fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
                             ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_LEADER_SIT);
    fdMapGridSetShake(5, 5, 1);
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    syFieldMapGridWaitForShakeToSettle();
    fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
                             ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
    leader->gravity = toFIXED(1);
    leader->bounce  = toFIXED(1) / 4;
    syFieldScriptWait(10);
    fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
    FieldStateWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    fdScriptEnd();
}

#endif

// The call AnkohlRuinsWeedsScript makes: clears the clump's sprite flags, draws it at priority 2
// and stands it four pixels lower.
Sint32 scAnkohlRuinsSetUpWeeds(ACTOR *actor)
{
    fdActorSetSpriteFlags(actor, 0);
    ((FIELD_SPR *)actor->sprite)->priority  = 2;
    actor->priorityFlags                   &= ~ACTOR_PRIORITY_AUTOMATIC;
    actor->motionFlags                      = 0;
    actor->y.fixed                         -= toFIXED(4);
    return 0;
}

extern ACTOR_PLACE AnkohlRuinsPlacements[];
extern ACTOR_PLACE AnkohlRuinsStoneFacesPlacements[];
extern ACTOR_PLACE AnkohlRuinsThanatosMaceChestPlacements[];
extern ACTOR_PLACE AnkohlRuinsGreatFacePlacements[];
extern ACTOR_PLACE AnkohlRuinsFaceHalvesPlacements[];
extern ACTOR_PLACE AnkohlRuinsCrystalPowderChestPlacements[];
extern ACTOR_PLACE AnkohlRuinsSandDoorsPlacements[];
extern ACTOR_PLACE AnkohlRuinsLeftProngPlacements[];

const ACTOR_PLACE *scGetActorPlacements(void)
{
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_STONE_FACES)
        return AnkohlRuinsStoneFacesPlacements;
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_THANATOS_MACE_CHEST)
        return AnkohlRuinsThanatosMaceChestPlacements;
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_GREAT_FACE)
        return AnkohlRuinsGreatFacePlacements;
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_FACE_HALVES)
        return AnkohlRuinsFaceHalvesPlacements;
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST)
        return AnkohlRuinsCrystalPowderChestPlacements;
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_SAND_DOORS)
        return AnkohlRuinsSandDoorsPlacements;
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_LEFT_PRONG)
        return AnkohlRuinsLeftProngPlacements;
    return AnkohlRuinsPlacements;
}

// Felix sinks into the ground with Sand and moves on the sand layer, under the ruins' blocks and
// pillars.
void scAnkohlRuinsSinkIntoSand(void)
{
    fdActorGet(MainRoutineStateWork.leaderActorId)->layer = MAPGRID_LAYER_OVERHEAD;
}

// Felix comes up out of the Sand and walks on the ground again.
void scAnkohlRuinsRiseFromSand(void)
{
    fdActorGet(MainRoutineStateWork.leaderActorId)->layer = MAPGRID_LAYER_GROUND;
}

// Nothing visible happens as Felix starts to cast Move on one of the ruins' pillars; the game
// rewrites the floor metatiles around the pillar for the push. It rewrites the metatiles beside the
// pillar by the Nut's chest, the great face's rooms' pillar, the face halves' rooms' three and the
// Crystal Powder's rooms' one. For the half face beside the Crystal Powder's rooms' gap it copies
// in the gap's metatiles, in every edition but the Japanese only while Reveal shows the gap.
void scAnkohlRuinsStartMovingPillar(Sint32 unused, Sint32 id)
{
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_STONE_FACES)
    {
        if (id == ACTOR_NUT_CHEST_PILLAR)
        {
            fdMapGridCopyMetatileAttributes(4, 68, 1, 1, 4, 69);
            fdMapGridCopyMetatileAttributes(3, 68, 1, 1, 3, 70);
        }
    }
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_FACE_HALVES)
    {
        if (id == ACTOR_FACE_HALVES_THIRD_PILLAR)
        {
            fdMapGridCopyMetatileAttributes(35, 72, 1, 1, 38, 72);
            fdMapGridCopyMetatileAttributes(36, 72, 1, 1, 37, 73);
        }
        if (id == ACTOR_FACE_HALVES_PILLAR)
            fdMapGridCopyMetatileAttributes(2, 113, 1, 1, 3, 113);
        if (id == ACTOR_FACE_HALVES_SECOND_PILLAR)
            fdMapGridCopyMetatileAttributes(39, 93, 1, 1, 40, 93);
    }
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_GREAT_FACE)
    {
        if (id == ACTOR_GREAT_FACE_ROOMS_PILLAR)
        {
            fdMapGridCopyMetatileAttributes(32, 69, 1, 1, 32, 70);
            fdMapGridCopyMetatileAttributes(33, 69, 1, 1, 33, 71);
        }
    }
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST)
    {
        if (id == ACTOR_CRYSTAL_POWDER_PILLAR)
        {
            fdMapGridCopyMetatileAttributes(17, 85, 1, 1, 17, 83);
            fdMapGridCopyMetatileAttributes(19, 84, 1, 1, 18, 82);
            fdMapGridCopyMetatileAttributes(19, 84, 1, 1, 19, 83);
        }
#if !ANKOHL_GAP_OPENS_FOR_MOVE_ONLY_WHILE_REVEALED
        if (id == ACTOR_FACE_HALF_FOR_THE_GAP)
#else
        if (id == ACTOR_FACE_HALF_FOR_THE_GAP && fdScriptGetFlag(FLAG_ANKOHL_RUINS_REVEAL_SHOWN))
#endif
            fdMapGridCopyMetatiles(35, 98, 3, 5, 25, 98);
    }
}

// Felix pushes one of the ruins' pillars a metatile along with Move, and pushed to the end of its
// short track it stays there for good: the one beside the Nut's chest slides east into its groove.
// The game fixes the metatiles the pillar has left and entered, and at the track's end sets the
// pillar's flag, so that the set-up puts it there again.
void scAnkohlRuinsFinishMovingPillar(Sint32 unused, Sint32 id)
{
    ACTOR *actor = fdActorGet(id);

    if (id == ACTOR_NUT_CHEST_PILLAR && MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_STONE_FACES)
    {
        if (FIELD_METATILE(actor->x.fixed) == NUT_CHEST_PILLAR_END_COLUMN)
        {
            fdMapGridCopyMetatileAttributes(4, 68, 1, 1, 2, 69);
            fdScriptSetFlag(FLAG_NUT_CHEST_PILLAR_MOVED);
        }
        fdMapGridCopyMetatileAttributes(3, 68, 1, 1, 4, 69);
        fdMapGridCopyMetatileAttributes(3, 69, 1, 1, 3, 70);
    }
    if (id == ACTOR_GREAT_FACE_ROOMS_PILLAR
        && MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_GREAT_FACE)
    {
        fdMapGridCopyMetatileAttributes(32, 71, 1, 1, 33, 71);
        fdMapGridCopyMetatileAttributes(31, 70, 1, 1, 32, 70);
        if (FIELD_METATILE(actor->x.fixed) == GREAT_FACE_ROOMS_PILLAR_END_COLUMN)
        {
            fdMapGridCopyMetatileAttributes(32, 69, 1, 1, 33, 69);
            fdScriptSetFlag(FLAG_GREAT_FACE_ROOMS_PILLAR_MOVED);
        }
    }
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_FACE_HALVES)
    {
        if (id == ACTOR_FACE_HALVES_PILLAR)
        {
            fdMapGridCopyMetatileAttributes(3, 114, 1, 1, 3, 113);
            if (FIELD_METATILE(actor->x.fixed) == FACE_HALVES_PILLAR_END_COLUMN)
                fdScriptSetFlag(FLAG_FACE_HALVES_PILLAR_MOVED);
        }
        if (id == ACTOR_FACE_HALVES_SECOND_PILLAR)
        {
            fdMapGridCopyMetatileAttributes(40, 94, 1, 1, 40, 93);
            if (FIELD_METATILE(actor->x.fixed) == FACE_HALVES_SECOND_PILLAR_END_COLUMN)
                fdScriptSetFlag(FLAG_FACE_HALVES_SECOND_PILLAR_MOVED);
        }
        if (id == ACTOR_FACE_HALVES_THIRD_PILLAR)
        {
            fdMapGridCopyMetatileAttributes(36, 72, 1, 1, 38, 72);
            fdMapGridCopyMetatileAttributes(37, 74, 1, 1, 37, 73);
            if (FIELD_METATILE(actor->x.fixed) == FACE_HALVES_THIRD_PILLAR_END_COLUMN)
            {
                fdMapGridCopyMetatileAttributes(37, 70, 1, 1, 37, 71);
                fdScriptSetFlag(FLAG_FACE_HALVES_THIRD_PILLAR_MOVED);
            }
        }
    }
    if (id == ACTOR_CRYSTAL_POWDER_PILLAR
        && MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST)
    {
        fdMapGridCopyMetatileAttributes(16, 83, 1, 1, 17, 83);
        fdMapGridCopyMetatileAttributes(19, 82, 1, 1, 18, 82);
        fdMapGridCopyMetatileAttributes(19, 82, 1, 1, 19, 83);
        if (FIELD_METATILE(actor->x.fixed) == CRYSTAL_POWDER_PILLAR_END_COLUMN)
        {
            fdMapGridCopyMetatileAttributes(16, 84, 1, 1, 18, 84);
            fdScriptSetFlag(FLAG_CRYSTAL_POWDER_PILLAR_MOVED);
        }
    }
}

// Felix pushes one of the seven stone faces in the north wall of the halls of the stone faces south
// off its ledge with Move; it falls with a puff of dust and is gone for good, leaving open the
// doorway it stood in front of.
void scAnkohlRuinsPushStoneFaceOffLedge(Sint32 unused, Sint32 face)
{
    Sint32 id    = face;
    ACTOR *actor = fdActorGet(id);
    Sint16 ticks = 60;

    if (FIELD_METATILE(actor->z.fixed) == STONE_FACE_DROP_ROW)
    {
        actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        fdActorSetSpritePriorityDirect(actor, 3);
        fdScriptPlaySound(SE_DUST_PUFF);
        while (ticks != 0)
        {
            syWait(1);
            if (actor->velocityY == 0)
                break;
            ticks--;
        }
        fdActorSetPos(id, 0, 0);
        if (id == ACTOR_STONE_FACE_A)
            fdScriptSetFlag(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_A));
        if (id == ACTOR_STONE_FACE_B)
            fdScriptSetFlag(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_B));
        if (id == ACTOR_STONE_FACE_C)
            fdScriptSetFlag(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_C));
        if (id == ACTOR_STONE_FACE_D)
            fdScriptSetFlag(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_D));
        if (id == ACTOR_STONE_FACE_E)
            fdScriptSetFlag(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_E));
        if (id == ACTOR_STONE_FACE_F)
            fdScriptSetFlag(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_F));
        if (id == ACTOR_STONE_FACE_G)
            fdScriptSetFlag(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_G));
    }
}

#include "shared/actor_wait_to_land_with_snd.h"

// Sand pours down from above: `rows` rows of `count` grains a metatile apart, each row two pixels
// further south, `ticks` apart.
void scAnkohlRuinsPourSand(
    Sint32 rows, Sint32 ticks, Sint32 x, Sint32 y, Sint32 startZ, Sint32 count)
{
    EFFECT_OPTIONS params;
    Sint32         row;
    Sint32         left;
    Sint32         dx;
    Sint32         dy;
    Sint32         z;

    params.palette     = SPRITE_COLOR_GRB;
    params.startScaleX = toFIXED(3) / 4;
    params.startScaleY = toFIXED(3) / 4;
    row                = 0;
    if (row < rows)
    {
        z = startZ;
        do
        {
            for (left = 0; left < count; left++)
            {
                {
                    Sint32 r  = syRandom();
                    dx        = 15;
                    dx       &= r;
                }
                dy = syRandom() & 31;
                scFieldEffectSpawn(toFIXED(x + left * MAPGRID_METATILE_PIXELS) + INTtoFIXED(dx - 8),
                                   toFIXED(y + dy),
                                   toFIXED(z),
                                   0,
                                   0,
                                   toFIXED(1) / 256,
                                   EFFECT_USE_START_SCALE | EFFECT_SCRIPT_MEDIUM,
                                   &params);
            }
            syFieldScriptWait(ticks);
            z += 2;
            row++;
        } while (row < rows);
    }
}

// Felix pushes a half of the stone face in the face halves' rooms with Move, and once it reaches
// its place, the left half at metatile (12, 45) and the right half beside it at (14, 45), it
// settles with a boom. When both halves are in place the ground shakes, the camera looks at the
// face, and sand pours down below it, row by row, into a slope. The slope is the face's metatiles
// copied in from beside the room, a row at a time, as in scAnkohlRuinsPlaceThanatosFaceHalf.
void scAnkohlRuinsPlaceFaceHalves(Sint32 unused, Sint32 id)
{
    ACTOR *actor  = fdActorGetById(id);
    Sint32 column = FIELD_METATILE(actor->x.fixed);
    Sint32 row    = FIELD_METATILE(actor->z.fixed);
    Sint32 toColumn;
    Sint32 lowRow;
    Sint32 highRow;
    Sint32 height;
    Sint32 count;

    if (id == ACTOR_LEFT_FACE_HALF && column == 12 && row == 45)
    {
        fdScriptSetFlag(FLAG_LEFT_FACE_HALF_PLACED);
        scActorWaitToLandWithSound(ACTOR_LEFT_FACE_HALF, SE_BOOM);
    }
    if (id == ACTOR_RIGHT_FACE_HALF && column == 14 && row == 45)
    {
        fdScriptSetFlag(FLAG_RIGHT_FACE_HALF_PLACED);
        scActorWaitToLandWithSound(ACTOR_RIGHT_FACE_HALF, SE_BOOM);
    }
    if (!syGameFlagGet(FLAG_RIGHT_FACE_HALF_PLACED) || !syGameFlagGet(FLAG_LEFT_FACE_HALF_PLACED))
        return;
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdMapGridSetShake(2, 2, 1);
    fdCameraMoveTo(216, CAMERA_KEEP, 770, 1);
    syFieldScriptWait(15);
    fdScriptPlaySound(SE_FLAME_FLARE);
    toColumn = 12;
    highRow  = 111;
    fdMapGridCopyMetatilesLayered(12, 91, 3, 1, toColumn, highRow);
    syFieldScriptWait(60);
    fdScriptPlaySound(SE_ANKOHL_RUINS_PIECE);
    lowRow = 47;
    fdMapGridCopyMetatiles(16, 27, 3, 2, toColumn, lowRow);
    fdMapGridCopyMetatiles(16, 91, 3, 2, toColumn, highRow);
    height = -40;
    scAnkohlRuinsPourSand(3, 15, 200, height, 744, count = 3);
    fdMapGridCopyMetatiles(16, 27, 3, 3, toColumn, lowRow);
    fdMapGridCopyMetatiles(16, 91, 3, 3, toColumn, highRow);
    scAnkohlRuinsPourSand(3, 10, 200, height, 760, count);
    fdMapGridCopyMetatiles(16, 27, 3, 4, toColumn, lowRow);
    fdMapGridCopyMetatiles(16, 91, 3, 4, toColumn, highRow);
    scAnkohlRuinsPourSand(3, 10, 200, height, 776, count);
    fdMapGridCopyMetatiles(16, 27, 3, 5, toColumn, lowRow);
    fdMapGridCopyMetatiles(16, 91, 3, 5, toColumn, highRow);
    scAnkohlRuinsPourSand(3, 10, 200, height, 792, count);
    fdMapGridCopyMetatiles(16, 27, 3, 6, toColumn, lowRow);
    fdMapGridCopyMetatiles(16, 91, 3, 6, toColumn, highRow);
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    scAnkohlRuinsPourSand(3, 10, 200, height, 808, count);
    fdMapGridCopyMetatiles(16, 27, 3, 13, toColumn, lowRow);
    fdMapGridCopyMetatiles(16, 91, 3, 13, toColumn, highRow);
    fdScriptEnd();
}

// Felix pushes the left half of the stone face in the back room of the Thanatos Mace's rooms east
// with Move, and when it meets the right half in the wall, at metatile (12, 5), it settles with a
// boom: the ground shakes, the camera looks at the face, and sand pours down from it, row by row,
// and fills the hall below it.
void scAnkohlRuinsPlaceThanatosFaceHalf(Sint32 unused, Sint32 id)
{
    ACTOR *pillar = fdActorGetById(id);
    Sint32 column = FIELD_METATILE(pillar->x.fixed);
    Sint32 row    = FIELD_METATILE(pillar->z.fixed);

    if (id != ACTOR_THANATOS_FACE_HALF || column != 12 || row != 5)
        return;
    fdScriptSetFlag(FLAG_THANATOS_FACE_HALF_PLACED);
    scActorWaitToLandWithSound(ACTOR_THANATOS_FACE_HALF, SE_BOOM);
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    syFieldScriptWait(15);
    fdScriptPlaySound(SE_FLAME_FLARE);
    fdMapGridCopyMetatiles(29, 70, 3, 1, 12, 71);
    fdCameraMoveTo(216, CAMERA_KEEP, 120, 1);
    fdMapGridSetShake(2, 2, 1);
    syFieldScriptWait(60);
    fdScriptPlaySound(SE_ANKOHL_RUINS_PIECE);
    fdMapGridCopyMetatiles(35, 6, 7, 2, 10, 7);
    fdMapGridCopyMetatiles(35, 70, 7, 2, 10, 71);
    scAnkohlRuinsPourSand(6, 10, 200, -40, 124, 3);
    fdMapGridCopyMetatiles(35, 6, 7, 3, 10, 7);
    fdMapGridCopyMetatiles(35, 70, 7, 3, 10, 71);
    scAnkohlRuinsPourSand(3, 10, 184, -40, 140, 5);
    fdMapGridCopyMetatiles(35, 6, 7, 4, 10, 7);
    fdMapGridCopyMetatiles(35, 70, 7, 4, 10, 71);
    scAnkohlRuinsPourSand(3, 10, 184, -40, 156, 5);
    fdMapGridCopyMetatiles(35, 6, 7, 5, 10, 7);
    fdMapGridCopyMetatiles(35, 70, 7, 5, 10, 71);
    scAnkohlRuinsPourSand(3, 10, 184, -40, 172, 5);
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    fdMapGridCopyMetatiles(35, 6, 7, 6, 10, 7);
    fdMapGridCopyMetatiles(35, 70, 7, 6, 10, 71);
    syFieldScriptWait(15);
    fdMapGridCopyMetatiles(35, 6, 7, 16, 10, 7);
    fdMapGridCopyMetatiles(35, 70, 7, 16, 10, 71);
    fdScriptEnd();
}

// Felix pushes the right half of the great stone face west with Move, and when it meets the face,
// at metatile (15, 24), it sinks into place with a thump, the face is whole, and its eyes glow
// green. Now stepping into the face's corridor sets it sliding
// (scAnkohlRuinsStartGreatFaceSliding).
void scAnkohlRuinsPlaceGreatFaceHalf(Sint32 unused, Sint32 id)
{
    ACTOR *actor = fdActorGetById(id);
    Sint32 x     = FIELD_METATILE(actor->x.fixed);
    Sint32 z     = FIELD_METATILE(actor->z.fixed);

    if (id == ACTOR_GREAT_FACE_HALF && x == 15 && z == 24)
    {
        fdActorSetSpriteFlags(fdActorGetById(ACTOR_GREAT_FACE_HALF), 0);
        actor->y.fixed -= toFIXED(2);
        syFieldScriptWait(1);
        actor->y.fixed -= toFIXED(2);
        syFieldScriptWait(1);
        actor->y.fixed -= toFIXED(4);
        syFieldScriptWait(1);
        actor->y.fixed -= toFIXED(8);
        syFieldScriptWait(1);
        fdActorSetAnimation(ACTOR_GREAT_FACE, 3);
        fdActorSetPos(ACTOR_GREAT_FACE_HALF, 0, 0);
        fdScriptPlaySound(SE_THUMP);
        syFieldScriptWait(45);
        fdActorSetAnimation(ACTOR_GREAT_FACE, 4);
        fdScriptPlaySound(SE_EYES_GLOW);
        fdScriptSetFlag(FLAG_GREAT_FACE_HALF_PLACED);
    }
}

extern MAP_METATILE MapGridMetatileBuffer[];

// Felix, standing on the sand that flows down the face halves' rooms' slide or the Crystal Powder's
// rooms' sand, drifts south with it, and where it runs over a drop it carries him off
// (scAnkohlRuinsCarriedOffLedge). It runs every tick from the room's set-up, and moves him and the
// camera half a pixel south on metatiles marked MAP_METATILE_FLOWING_SAND, unless he is under the
// Sand.
void                scAnkohlRuinsFlowWithSand(void)
{
    ACTOR        *leader    = fdActorGet(MainRoutineStateWork.leaderActorId);
    FIELD_STATE  *state     = (FIELD_STATE *)RamHeapSlots->fieldState;
    MAP_STATE    *map       = (MAP_STATE *)RamHeapSlots->mapGrid;
    ACTOR        *companion = state->camera;
    MAP_METATILE *ground;
    MAP_METATILE *upper;
    Sint32        index;
    FIXED         pos[3];

    if (fdScriptGetFlag(FLAG_FIELD_HANDLING_REQUESTS))
        return;
    if (map == NULL)
    {
        upper  = MapGridMetatileBuffer;
        ground = MapGridMetatileBuffer;
    }
    else
    {
        ground = map->layers[MAPGRID_LAYER_GROUND].metatiles;
        upper  = map->layers[MAPGRID_LAYER_OVERHEAD].metatiles;
    }
    index   = FIELD_METATILE(leader->x.fixed) + (FIELD_METATILE(leader->z.fixed) << 7);
    ground += index;
    upper  += index;
    if (((ground->attributeB & MAP_METATILE_FLOWING_SAND)
         | (upper->collisionCode & METATILE_BEHAVIOR_SAND))
        != (MAP_METATILE_FLOWING_SAND | METATILE_BEHAVIOR_SAND))
        return;
    if (MainRoutineStateWork.movementMode == LEADER_MOVEMENT_SAND)
        return;
    pos[0] = leader->x.fixed;
    pos[1] = leader->y.fixed;
    pos[2] = leader->z.fixed + toFIXED(8);
    if (fdMapGridCheckCollision(leader, pos) < 0)
        state->stepCue = CUE_CARRIED_OFF_LEDGE;
    leader->z.fixed    += toFIXED(1) / 2;
    companion->z.fixed += toFIXED(1) / 2;
}

// Felix, carried by the flowing sand to the edge of a drop in the Crystal Powder's rooms, slides
// over it, starts with a "!" and falls to the floor below with a thump.
void scAnkohlRuinsCarriedOffLedge(void)
{
    ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STILL);
    fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,
                             ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    actor->motionFlags   = 0;
    actor->steeringFlags = 0;
    fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 0, 8);
    scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
    fdActorShowEmoteById(MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    syFieldScriptWait(10);
    fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    fdScriptPlaySound(SE_DUST_PUFF);
    scActorWaitToLandWithSound(MainRoutineStateWork.leaderActorId, SE_THUMP);
    actor->steeringFlags = ACTOR_FACES_MOTION;
    fdScriptEnd();
}

// Felix pushes a face half or a pillar in the face halves' rooms by walking into it: once he has
// leant on it a moment where the floor lets him push, the field engine shoves it a metatile ahead
// (fdRunPushMetatile) and then this runs, the halves going on as scAnkohlRuinsPlaceFaceHalves and
// the pillars as scAnkohlRuinsFinishMovingPillar, as after Move. A push by Move does not come here:
// its own rows run those two directly.
void scAnkohlRuinsFinishPushing(Sint32 a, Sint32 id)
{
    if (id == ACTOR_LEFT_FACE_HALF || id == ACTOR_RIGHT_FACE_HALF)
        scAnkohlRuinsPlaceFaceHalves(a, id);
    else
        scAnkohlRuinsFinishMovingPillar(a, id);
}

// Felix casts Whirlwind ("Conjure wind to scatter weeds.") on a clump of weeds in the halls of the
// stone faces, the Thanatos Mace's rooms or the face halves' rooms: the leaves blow away and the
// clump is gone for good. Most hide nothing; a few hide the way on, such as the one in the middle
// of the entrance hall and the one at the upper left of the halls, over a stairway. Whirlwind
// itself blows the clump away; this only sets its flag, so that the set-up leaves it out next time.
void scAnkohlRuinsScatterWeeds(Sint32 a, Sint32 id)
{
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_STONE_FACES)
        fdScriptSetFlag(FLAG_STONE_FACES_WEEDS_SCATTERED(id));
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_THANATOS_MACE_CHEST)
        fdScriptSetFlag(FLAG_THANATOS_WEEDS_SCATTERED(id));
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_FACE_HALVES)
        fdScriptSetFlag(FLAG_FACE_HALVES_WEEDS_SCATTERED(id));
}

// The rising sand of the sand rooms, drawn each tick: the sand's picture is raised to its level,
// and its edges against the ledges are trimmed. Until the sand is 32 pixels deep, and while Reveal
// is not up, eight sprites drawn into the object window mask its edge, each moved by the level as
// its kind says; map layer 1, the sand, is scrolled by the level over its base.
void scAnkohlRuinsDrawSand(void)
{
    MAP_STATE              *map    = RamHeapSlots->mapGrid;
    Sint32                  viewX  = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
    Sint32                  viewZ  = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
    ANKOHL_RUINS_SPR_STATE *state  = &AnkohlRuinsSand;
    // DEAD: viewY, subtracted from both y and z, cancels in screenY; the ROM keeps its load and its
    // store to the stack.
    Sint32                  viewY  = map->origin[1];
    Sint32                  tile   = VramCacheEntries[state->tile].offset >> 5;
    ANKOHL_RUINS_SPR       *sprite = state->sprites;
    Sint32                  i;

    if (fdScriptGetFlag(FLAG_ANKOHL_RUINS_REVEAL_SHOWN) == 0 && state->level < toFIXED(32))
    {
        for (i = 0; i <= 7; i++)
        {
            Uint32 kind = sprite->kind;
            FIXED  x    = sprite->x;
            Sint32 y    = sprite->y;
            Sint32 z    = sprite->z;
            Sint32 screenX, screenY;

            switch (kind)
            {
                case SAND_MASK_WEST_SHALLOW:
                    x -= state->level;
                    y += state->level;
                    if (state->level > toFIXED(16))
                        x = -toFIXED(16);
                    break;
                case SAND_MASK_WEST:
                    x -= state->level;
                    y += state->level;
                    break;
                case SAND_MASK_EAST:
                    x += state->level;
                    y += state->level;
                    break;
                case SAND_MASK_EAST_SHALLOW:
                    x += state->level;
                    y += state->level;
                    if (state->level > toFIXED(16))
                        x = -toFIXED(16);
                    break;
                case SAND_MASK_UP_SHALLOW:
                    if (state->level < toFIXED(16))
                        y += state->level;
                    else
                        x = -toFIXED(16);
                    break;
            }
            x       -= viewX;
            y       -= viewY;
            z        = z - viewZ - viewY;
            screenY  = z - y;
            screenX  = FIXEDtoINT(x) - 8;
            screenY  = FIXEDtoINT(screenY) - 8;
            if (screenX >= -15 && screenX <= 319 && screenY >= -15 && screenY <= 239)
            {
                screenX        &= 511;
                screenY        &= 255;
                sprite->oam[0]  = 0;
                sprite->oam[1] =
                    screenY | OBJ_X(screenX) | OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_WINDOW);
                sprite->oam[2] = tile | OBJ_PRIORITY(1);
                syOAMAddSpriteAtDepthClamped(sprite->oam, OAM_DEPTH_FRONT);
            }
            sprite++;
        }
    }
    {
        MAP_LAYER *layer = &map->layers[MAPGRID_LAYER_MIDDLE];

        layer->offsetY   = state->baseOffsetY + state->level;
    }
}

// Sets up the sand rooms' rising sand on entering one: the tile of the masks that trim its edge,
// the window they draw into, the event that draws it, and the masks for the room the entrance leads
// into.
void scAnkohlRuinsInitSand(Sint32 unused)
{
    ANKOHL_RUINS_SPR_STATE *state = &AnkohlRuinsSand;
    Uint32                 *tiles = syHeapAllocate(SAND_MASK_BYTES);
    Uint32                 *pixel = tiles;
    MAP_LAYER        *layer  = &((MAP_STATE *)RamHeapSlots->mapGrid)->layers[MAPGRID_LAYER_MIDDLE];
    ANKOHL_RUINS_SPR *sprite = state->sprites;
    Sint32            i;

    if (fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
        syWordsClearEntry(state, sizeof(*state));
    for (i = 0; i < SAND_MASK_BYTES / 4; i++)
    {
        *pixel++ = PIXELS_4BPP(1);
    }
    *(Sint16 *)&state->tile = syVRAMCacheFindFreeEntry();
    mnVRAMLoad(state->tile, SAND_MASK_BYTES, tiles);
    syHeapRelease(tiles);
    state->baseOffsetX = layer->offsetX;
    state->baseOffsetY = layer->offsetY;
    syIOWrite16(WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3
                    | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_BG0 | WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG3
                    | WINOUT_WINOBJ_OBJ,
                &REG_WINOUT);
    REG_DISPCNT |= DISPCNT_OBJWIN_ON;
    sySetTickEvent(scAnkohlRuinsDrawSand, EVENT_PASS_TICK);
    if (MainRoutineStateWork.entrance == ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_GREAT_FACE
        || MainRoutineStateWork.entrance
               == ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST)
    {
        sprite->x    = toFIXED(40);
        sprite->kind = SAND_MASK_WEST;
        sprite->y    = 0;
        sprite->z    = toFIXED(120);
        sprite++;
        sprite->x    = toFIXED(56);
        sprite->kind = SAND_MASK_WEST;
        sprite->y    = 0;
        sprite->z    = toFIXED(120);
        sprite++;
        sprite->x    = toFIXED(216);
        sprite->y    = 0;
        sprite->z    = toFIXED(120);
        sprite->kind = SAND_MASK_EAST;
        sprite++;
        sprite->x    = toFIXED(232);
        sprite->y    = 0;
        sprite->z    = toFIXED(120);
        sprite->kind = SAND_MASK_EAST;
        sprite++;
        sprite->x    = toFIXED(184);
        sprite->z    = toFIXED(88);
        sprite->y    = 0;
        sprite->kind = SAND_MASK_UP_SHALLOW;
        sprite++;
    }
    if (*(Sint16 *)&MainRoutineStateWork.entrance
            == ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_FACE_HALVES_SOUTH
        || *(Sint16 *)&MainRoutineStateWork.entrance
               == ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_FACE_HALVES_NORTH)
    {
        sprite->x    = toFIXED(40);
        sprite->z    = toFIXED(440);
        sprite->y    = 0;
        sprite->kind = SAND_MASK_WEST;
        sprite++;
        sprite->x    = toFIXED(56);
        sprite->z    = toFIXED(440);
        sprite->y    = 0;
        sprite->kind = SAND_MASK_WEST;
        sprite++;
        sprite->x    = toFIXED(216);
        sprite->y    = 0;
        sprite->z    = toFIXED(408);
        sprite->kind = SAND_MASK_WEST_SHALLOW;
        sprite++;
        sprite->x    = toFIXED(232);
        sprite->y    = 0;
        sprite->z    = toFIXED(408);
        sprite->kind = SAND_MASK_WEST;
        sprite++;
        sprite->x    = toFIXED(72);
        sprite->z    = toFIXED(376);
        sprite->y    = 0;
        sprite->kind = SAND_MASK_UP_SHALLOW;
    }
}

// Counts the time to the sand's next rise after the switch; when it runs out, the room's sand rises
// again (CUE_RAISE_FIRST_SAND or CUE_RAISE_SECOND_SAND).
void scAnkohlRuinsCountSandTime(void)
{
    FIELD_STATE *state = (FIELD_STATE *)RamHeapSlots->fieldState;

    if (fdScriptGetFlag(FLAG_FIELD_HANDLING_REQUESTS) == 0)
    {
        if (++AnkohlRuinsSandTimer >= AnkohlRuinsSandRiseDelay)
        {
            syFieldCloseEvent(scAnkohlRuinsCountSandTime);
            if (AnkohlRuinsSandRoom == 1)
                state->raisedCue = CUE_RAISE_FIRST_SAND;
            else
                state->raisedCue = CUE_RAISE_SECOND_SAND;
        }
    }
}

extern ANKOHL_RUINS_SAND_ROW AnkohlRuinsFirstSandRows[];

// The sand in the first sand room rises a step as the ground shakes and the great stone face in the
// north wall pours, lifting Felix if he stands on it, until the room is full and the door button on
// the far ledge can be reached. A step takes SAND_RISE_MOVES moves; after it the room's metatiles,
// read from AnkohlRuinsFirstSandRows, follow the new level, and while the sand is below 32 pixels
// the next rise is timed (scAnkohlRuinsCountSandTime).
void                         scAnkohlRuinsRaiseFirstSand(void)
{
    ACTOR                  *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
    MAP_STATE              *map    = (MAP_STATE *)RamHeapSlots->mapGrid;
    ANKOHL_RUINS_SPR_STATE *state  = &AnkohlRuinsSand;
    Sint16                  i;
    Sint16                  j;
    Sint16                  m;
    Sint32                  row;
    Sint16                  n;

    fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STILL);
    m                   = METATILE_HEIGHT_STEPS(state->level);
    leader->motionFlags = 0;
    fdMapGridSetShake(4, 4, 1);
    syFieldScriptWait(30);
    fdScriptPlaySound(SE_RISE_WITH_SPARKS_HIGH);
    fdMapGridCopyMetatiles(22, 66, 3, 1, 7, 66);
    fdMapGridCopyMetatiles(22, 1, 3, 5, 7, 1);
    for (i = 0; i < SAND_RISE_MOVES; i++)
    {
        MAP_METATILE *metatiles;

        for (j = 0; j < AnkohlRuinsSandStepDelay; j++)
            syFieldScriptWait(1);
        state->level += AnkohlRuinsSandStep;
        metatiles     = map->layers[MAPGRID_LAYER_OVERHEAD].metatiles;
        metatiles    += leader->x.fixed / toFIXED(16) + ((leader->z.fixed / toFIXED(16)) << 7);
        if (metatiles->collisionCode == METATILE_BEHAVIOR_SAND)
        {
            leader->y.fixed      += AnkohlRuinsSandStep;
            leader->groundHeight  = leader->y.fixed;
        }
        syFieldScriptWait(1);
        if (m == 2 && i == 0)
        {
            fdMapGridCopyMetatiles(11, 70, 1, 1, 11, 69);
            fdMapGridCopyMetatilesLayered(10, 68, 1, 1, 11, 68);
            fdMapGridCopyMetatileAttributes(10, 5, 1, 1, 11, 5);
        }
        if (m == 1 && i == SAND_RISE_MOVES - 1)
            fdMapGridCopyMetatilesLayered(18, 73, 1, 1, 11, 68);
    }
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    syFieldScriptWait(5);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    if (state->level < toFIXED(32))
    {
        AnkohlRuinsSandTimer = 0;
        sySetTickEvent(scAnkohlRuinsCountSandTime, EVENT_PASS_TICK);
    }
    else
    {
        fdMapGridCopyMetatilesLayered(69, 6, 1, 2, 68, 5);
        fdScriptSetFlag(FLAG_SAND_ROOM_FULL);
        fdScriptClearFlag(FLAG_NO_PLACE_TO_SAVE);
    }
    fdMapGridCopyMetatiles(18, 66, 3, 1, 7, 66);
    fdMapGridCopyMetatiles(18, 1, 3, 5, 7, 1);
    i = METATILE_HEIGHT_STEPS(state->level);
    for (row = 0; AnkohlRuinsFirstSandRows[row].level != SAND_ROWS_END; row++)
    {
        if (i >= AnkohlRuinsFirstSandRows[row].level)
        {
            for (m = 0; m < AnkohlRuinsFirstSandRows[row].height; m++)
            {
                for (n = 0; n < AnkohlRuinsFirstSandRows[row].width; n++)
                {
                    fdMapGridCopyMetatileAttributes(21,
                                                    14 - i,
                                                    1,
                                                    1,
                                                    AnkohlRuinsFirstSandRows[row].x + n,
                                                    AnkohlRuinsFirstSandRows[row].z + m);
                    fdMapGridCopyMetatileAttributes(25,
                                                    14 - i,
                                                    1,
                                                    1,
                                                    AnkohlRuinsFirstSandRows[row].x + n,
                                                    AnkohlRuinsFirstSandRows[row].z + m + 64);
                }
            }
        }
    }
    fdMapGridCopyMetatileAttributes(19, 14 - i, 2, 1, 2, 7);
    fdMapGridCopyMetatileAttributes(22, 14 - i, 2, 1, 13, 7);
    fdMapGridCopyMetatileAttributes(18, 14 - i, 1, 1, 7, 5);
    fdMapGridCopyMetatileAttributes(24, 14 - i, 1, 1, 9, 5);
    fdMapGridCopyMetatileAttributes(19, 14 - i, 2, 1, 2, 71);
    fdMapGridCopyMetatileAttributes(22, 14 - i, 2, 1, 13, 71);
    fdMapGridCopyMetatileAttributes(26, 14 - i, 1, 1, 7, 69);
    fdMapGridCopyMetatileAttributes(27, 14 - i, 1, 1, 9, 69);
    if (i == 2)
        fdMapGridCopyMetatileAttributes(18, 8, 1, 1, 11, 5);
}

extern ANKOHL_RUINS_SAND_ROW AnkohlRuinsSecondSandRows[];

// The sand in the second sand room rises a step, as in the first (scAnkohlRuinsRaiseFirstSand), the
// shaking harder.
void                         scAnkohlRuinsRaiseSecondSand(void)
{
    ACTOR                  *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
    MAP_STATE              *map    = (MAP_STATE *)RamHeapSlots->mapGrid;
    ANKOHL_RUINS_SPR_STATE *state  = &AnkohlRuinsSand;
    Sint32                  level;
    Sint16                  i;
    Sint16                  j;
    Sint32                  row;
    Sint16                  m;
    Sint16                  n;

    fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STILL);
    level               = METATILE_HEIGHT_STEPS(state->level);
    leader->motionFlags = 0;
    fdMapGridSetShake(5, 5, 1);
    syFieldScriptWait(30);
    fdScriptPlaySound(SE_RISE_WITH_SPARKS_HIGH);
    fdMapGridCopyMetatiles(22, 66, 3, 1, 7, 84);
    fdMapGridCopyMetatiles(22, 1, 3, 5, 7, 19);
    for (i = 0; i < SAND_RISE_MOVES; i++)
    {
        MAP_METATILE *metatiles;

        for (j = 0; j < AnkohlRuinsSandStepDelay; j++)
            syFieldScriptWait(1);
        state->level += AnkohlRuinsSandStep;
        metatiles     = map->layers[MAPGRID_LAYER_OVERHEAD].metatiles;
        metatiles    += leader->x.fixed / toFIXED(16) + ((leader->z.fixed / toFIXED(16)) << 7);
        if (metatiles->collisionCode == METATILE_BEHAVIOR_SAND)
        {
            leader->y.fixed      += AnkohlRuinsSandStep;
            leader->groundHeight  = leader->y.fixed;
        }
        syFieldScriptWait(1);
        if (level == 2 && i == 0)
        {
            fdMapGridCopyMetatiles(11, 70, 1, 1, 11, 69);
            fdMapGridCopyMetatilesLayered(10, 68, 1, 1, 4, 86);
            fdMapGridCopyMetatileAttributes(4, 24, 1, 1, 4, 23);
        }
        if (level == 1 && i == SAND_RISE_MOVES - 1)
            fdMapGridCopyMetatilesLayered(18, 73, 1, 1, 4, 86);
    }
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    syFieldScriptWait(5);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    if (state->level < toFIXED(32))
    {
        AnkohlRuinsSandTimer = 0;
        sySetTickEvent(scAnkohlRuinsCountSandTime, EVENT_PASS_TICK);
    }
    else
    {
        fdMapGridCopyMetatilesLayered(69, 24, 1, 4, 68, 23);
        fdScriptSetFlag(FLAG_SAND_ROOM_FULL);
        fdScriptClearFlag(FLAG_NO_PLACE_TO_SAVE);
    }
    fdMapGridCopyMetatiles(18, 66, 3, 1, 7, 84);
    fdMapGridCopyMetatiles(18, 1, 3, 5, 7, 19);
    i = METATILE_HEIGHT_STEPS(state->level);
    for (row = 0; AnkohlRuinsSecondSandRows[row].level != SAND_ROWS_END; row++)
    {
        if (i >= AnkohlRuinsSecondSandRows[row].level)
        {
            for (m = 0; m < AnkohlRuinsSecondSandRows[row].height; m++)
            {
                for (n = 0; n < AnkohlRuinsSecondSandRows[row].width; n++)
                {
                    fdMapGridCopyMetatileAttributes(21,
                                                    14 - i,
                                                    1,
                                                    1,
                                                    AnkohlRuinsSecondSandRows[row].x + n,
                                                    AnkohlRuinsSecondSandRows[row].z + m);
                    fdMapGridCopyMetatileAttributes(25,
                                                    14 - i,
                                                    1,
                                                    1,
                                                    AnkohlRuinsSecondSandRows[row].x + n,
                                                    AnkohlRuinsSecondSandRows[row].z + m + 64);
                }
            }
        }
    }
    fdMapGridCopyMetatileAttributes(19, 14 - i, 2, 1, 2, 27);
    fdMapGridCopyMetatileAttributes(19, 14 - i, 2, 1, 13, 25);
    fdMapGridCopyMetatileAttributes(18, 14 - i, 1, 1, 7, 23);
    fdMapGridCopyMetatileAttributes(24, 14 - i, 1, 1, 9, 23);
    fdMapGridCopyMetatileAttributes(19, 14 - i, 2, 1, 2, 91);
    fdMapGridCopyMetatileAttributes(19, 14 - i, 2, 1, 13, 89);
    fdMapGridCopyMetatileAttributes(26, 14 - i, 1, 1, 7, 87);
    fdMapGridCopyMetatileAttributes(27, 14 - i, 1, 1, 9, 87);
    if (i == 2)
        fdMapGridCopyMetatileAttributes(18, 7, 1, 1, 4, 23);
}

// Felix steps onto the grey switch on the west ledge of the first sand room (metatile (1, 7)): the
// great stone face in the north wall opens its mouth and pours sand, which rises a step, then
// slowly another and another until the room is full. Saving is barred while it rises.
void scAnkohlRuinsPressFirstSandSwitch(void)
{
    Sint32 done = scAnkohlRuinsFlagCheck(FLAG_FIRST_SAND_SWITCH_PRESSED);

    if (done == 0)
    {
        syFieldScriptEndLastingPsynergy(0);
        fdScriptPlaySound(SE_BUTTON_PRESS);
        syFieldScriptWait(15);
        fdScriptPlaySound(SE_FLAME_FLARE);
        fdMapGridCopyMetatiles(18, 66, 3, 1, 7, 66);
        fdMapGridSetShake(2, 2, 1);
        syFieldScriptWait(15);
        AnkohlRuinsSandStepDelay = 7;
        AnkohlRuinsSandTimer     = done;
        AnkohlRuinsSandRiseDelay = 200;
        AnkohlRuinsSandStep      = toFIXED(1) / 2;
        AnkohlRuinsSandRoom      = 1;
        fdScriptSetFlag(FLAG_NO_PLACE_TO_SAVE);
        scAnkohlRuinsRaiseFirstSand();
        fdScriptSetFlag(FLAG_FIRST_SAND_SWITCH_PRESSED);
    }
}

// Defines a sand room's door button: the first time it is stepped on, under flag n1, it clicks and
// the door at metatile (n2, n3) slides open with a boom, its metatile taking (n4, n5)'s attributes.
#define DEFINE_ANKOHL_RUINS_OPEN_ONCE(name, n1, n2, n3, n4, n5)                                    \
    void name(void)                                                                                \
    {                                                                                              \
        if (scAnkohlRuinsFlagCheck(n1) == 0)                                                       \
        {                                                                                          \
            fdScriptSetFlag(n1);                                                                   \
            fdScriptPlaySound(SE_BUTTON_PRESS);                                                    \
            fdScriptStart();                                                                       \
            syFieldScriptEndLastingPsynergy(0);                                                    \
            fdMapGridCopyMetatiles(26, 70, 1, 3, n2, n3);                                          \
            syFieldScriptWait(20);                                                                 \
            fdScriptPlaySound(SE_BOOM);                                                            \
            fdMapGridCopyMetatiles(26, 67, 1, 3, n2, n3);                                          \
            fdMapGridCopyMetatileAttributes(n4, n5, 1, 1, n2, n5);                                 \
            fdScriptEnd();                                                                         \
        }                                                                                          \
    }

// Felix steps on the first sand room's door button and the door in its north-east wall, metatile
// (14, 5), slides open. The button is the block 16 pixels high at metatile (11, 5), east of the
// great stone face: once the sand has risen two steps, to 16 pixels, scAnkohlRuinsRaiseFirstSand
// gives that metatile the button's behaviour from metatile (18, 8), which the map keeps east of the
// rooms where no camera shows it, and the next rise takes it away again.
DEFINE_ANKOHL_RUINS_OPEN_ONCE(
    scAnkohlRuinsOpenFirstSandDoor, FLAG_FIRST_SAND_DOOR_OPENED, 14, 65, 15, 5)

// Felix steps onto the grey switch on the east ledge of the second sand room (metatile (12, 25)):
// the great stone face in the north wall pours sand, which rises as in the first sand room but
// faster.
void scAnkohlRuinsPressSecondSandSwitch(void)
{
    Sint32 done = scAnkohlRuinsFlagCheck(FLAG_SECOND_SAND_SWITCH_PRESSED);

    if (done == 0)
    {
        syFieldScriptEndLastingPsynergy(0);
        fdScriptPlaySound(SE_BUTTON_PRESS);
        syFieldScriptWait(15);
        fdScriptPlaySound(SE_FLAME_FLARE);
        fdMapGridCopyMetatiles(18, 66, 3, 1, 7, 84);
        fdMapGridSetShake(2, 2, 1);
        syFieldScriptWait(15);
        AnkohlRuinsSandStepDelay = 3;
        AnkohlRuinsSandTimer     = done;
        AnkohlRuinsSandRiseDelay = 100;
        AnkohlRuinsSandStep      = toFIXED(1) / 2;
        AnkohlRuinsSandRoom      = 2;
        fdScriptSetFlag(FLAG_NO_PLACE_TO_SAVE);
        scAnkohlRuinsRaiseSecondSand();
        fdScriptSetFlag(FLAG_SECOND_SAND_SWITCH_PRESSED);
    }
}

// Felix steps on the second sand room's door button and the door in its north-west wall, metatile
// (2, 23), slides open. The button is the block 16 pixels high at metatile (4, 23), two metatiles
// east of the door: once the sand has risen two steps, to 16 pixels, scAnkohlRuinsRaiseSecondSand
// gives that metatile the button's behaviour from metatile (18, 7), which the map keeps east of the
// rooms where no camera shows it, and the next rise takes it away again.
DEFINE_ANKOHL_RUINS_OPEN_ONCE(
    scAnkohlRuinsOpenSecondSandDoor, FLAG_SECOND_SAND_DOOR_OPENED, 2, 83, 1, 23)

// The great stone face slides south down its corridor, shoving Felix ahead of it if he stands in
// its way and carrying him along if he stands right behind it, unless he is under the Sand; it
// stops at the corridor's foot. It moves four fifths of a pixel a tick. While it passes the sand
// patch midway, rows 31 to 36, Sand cannot end (FLAG_SAND_CANNOT_END). Past row 41 it stops, the
// shaking dies away with a sound, and the six metatiles at the corridor's foot where it comes to
// rest, (13, 41) to (15, 42), where the corridor opens onto the hall below, become wall: Felix
// cannot walk through the face there, and the metatiles that set the slide off are gone with them.
// FLAG_GREAT_FACE_AT_REST keeps them so for this visit.
void scAnkohlRuinsSlideGreatFace(void)
{
    ACTOR       *camera = FieldStateWork->camera;
    ACTOR       *face   = fdActorGetById(ACTOR_GREAT_FACE);
    ACTOR       *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    FIELD_STATE *state  = (FIELD_STATE *)RamHeapSlots->fieldState;
    Sint32       column;
    Sint32       row;

    if (syGameFlagGet(FLAG_FIELD_HANDLING_REQUESTS)
        && state->stepCue != METATILE_BEHAVIOR_GREAT_FACE_CORRIDOR_FOOT)
        return;
    face->z.fixed += toFIXED(4) / 5;
    column         = FIELD_METATILE(leader->x.fixed);
    if (MainRoutineStateWork.movementMode != LEADER_MOVEMENT_SAND && column > 12 && column <= 15)
    {
        if (face->z.fixed + toFIXED(24) >= leader->z.fixed && leader->z.fixed >= face->z.fixed)
        {
            leader->z.fixed = face->z.fixed + toFIXED(24);
            camera->z.fixed = face->z.fixed + toFIXED(24);
        }
        if (face->z.fixed >= leader->z.fixed && leader->z.fixed >= face->z.fixed - toFIXED(16))
        {
            leader->z.fixed = face->z.fixed - toFIXED(16);
            camera->z.fixed = face->z.fixed - toFIXED(16);
        }
    }
    row = FIELD_METATILE(face->z.fixed);
    if ((Uint16)(row - 31) <= 4 && !scAnkohlRuinsFlagCheck(FLAG_SAND_CANNOT_END))
    {
        fdMapGridCopyMetatileAttributes(35, 83, 5, 4, 12, 96);
        fdScriptSetFlag(FLAG_SAND_CANNOT_END);
    }
    if (row == 37 && scAnkohlRuinsFlagCheck(FLAG_SAND_CANNOT_END))
    {
        fdMapGridCopyMetatileAttributes(40, 83, 5, 4, 12, 96);
        fdScriptClearFlag(FLAG_SAND_CANNOT_END);
    }
    if (row > 41)
    {
        syFieldCloseEvent(scAnkohlRuinsSlideGreatFace);
        fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
        fdMapGridCopyMetatileAttributes(41, 48, 3, 2, 13, 41);
        fdScriptSetFlag(FLAG_GREAT_FACE_AT_REST);
        fdScriptClearFlag(FLAG_NO_PLACE_TO_SAVE);
        fdScriptClearFlag(FLAG_NO_ENCOUNTERS_IN_AREA);
        fdScriptPlaySound(SE_BANE_ESCAPE);
    }
}

// Once the great face is whole, Felix stepping into the foot of its corridor (metatiles (13, 42) to
// (15, 42)) sets it moving: there is a rush, the ground shakes, the camera looks up the corridor at
// the face as it starts to slide down towards him, and then follows Felix again. Saving and battles
// are barred while it slides.
void scAnkohlRuinsStartGreatFaceSliding(void)
{
    Sint32 z = FIELD_METATILE(fdActorGet(ACTOR_GREAT_FACE)->z.fixed);

    if (fdScriptGetFlag(FLAG_GREAT_FACE_HALF_PLACED) != 0 && z == 24)
    {
        Sint32 found = syFindEvent(scAnkohlRuinsSlideGreatFace);

        if (found == -1)
        {
            fdScriptStart();
            syFieldScriptEndLastingPsynergy(0);
            fdScriptPlaySound(SE_RUSH);
            fdMapGridSetShake(2, 2, 1);
            syFieldScriptWait(10);
            fdCameraMoveTo(232, CAMERA_KEEP, 392, 1);
            fdCameraWaitForArrival();
            sySetTickEvent(scAnkohlRuinsSlideGreatFace, EVENT_PASS_TICK);
            syFieldScriptWait(40);
            syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
            fdCameraWaitForArrival();
            fdMapGridCopyMetatileAttributes(13, 30, 3, 2, 13, z);
            fdMapGridCopyMetatileAttributes(14, 87, 1, 1, 14, 88);
            fdScriptSetFlag(FLAG_NO_PLACE_TO_SAVE);
            fdScriptSetFlag(FLAG_NO_ENCOUNTERS_IN_AREA);
            fdScriptEnd();
        }
    }
}

// Once the great face is whole, Felix stepping right in front of it at the top of its corridor
// (metatile (13, 26)) sets it sliding at once, the ground shaking, with no look up the corridor.
void scAnkohlRuinsStartGreatFaceSlidingUpClose(void)
{
    Sint32 z = FIELD_METATILE(fdActorGet(ACTOR_GREAT_FACE)->z.fixed);

    if (fdScriptGetFlag(FLAG_GREAT_FACE_HALF_PLACED) != 0 && z == 24)
    {
        void (*callback)(void) = scAnkohlRuinsSlideGreatFace;

        if (syFindEvent(callback) == -1)
        {
            fdMapGridSetShake(2, 2, 1);
            sySetTickEvent(callback, EVENT_PASS_TICK);
            fdMapGridCopyMetatileAttributes(13, 30, 3, 2, 13, z);
            fdMapGridCopyMetatileAttributes(14, 87, 1, 1, 14, 88);
            fdScriptSetFlag(FLAG_NO_PLACE_TO_SAVE);
            fdScriptSetFlag(FLAG_NO_ENCOUNTERS_IN_AREA);
            fdScriptEnd();
        }
    }
}

extern CUE AnkohlRuinsCues[];
extern CUE AnkohlRuinsStoneFacesCues[];
extern CUE AnkohlRuinsThanatosMaceChestCues[];
extern CUE AnkohlRuinsGreatFaceCues[];
extern CUE AnkohlRuinsFaceHalvesCues[];
extern CUE AnkohlRuinsCrystalPowderChestCues[];
extern CUE AnkohlRuinsSandDoorsCues[];
extern CUE AnkohlRuinsLeftProngCues[];

// Felix pushes the half face in the room of brown and red blocks of the Crystal Powder's rooms into
// the gap Reveal shows (metatile (26, 35)): it falls in, and the screen fades to the face halves'
// rooms below, where it drops in from above with a thump, then back to Felix.
void       scAnkohlRuinsPushFaceHalfIntoGap(void)
{
    ACTOR *actor = fdActorGet(ACTOR_FACE_HALF_FOR_THE_GAP);

    if (FIELD_METATILE(actor->x.fixed) == GAP_COLUMN && FIELD_METATILE(actor->z.fixed) == GAP_ROW)
    {
        fdScriptSetFlag(FLAG_FACE_HALF_PUSHED_INTO_GAP);
        fdScriptPlaySound(SE_DUST_PUFF);
        scActorWaitToLandWithSound(ACTOR_FACE_HALF_FOR_THE_GAP, 0);
        fdActorSetPos(ACTOR_FACE_HALF_FOR_THE_GAP, 0, 0);
        fdActorTableSnapshot();
        syFieldScriptSetExitRequest(
            ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH_C);
    }
// The Japanese edition copies metatiles here.
#if !ANKOHL_GAP_OPENS_FOR_MOVE_ONLY_WHILE_REVEALED
    fdMapGridCopyMetatiles(40, 98, 3, 5, 25, 98);
#endif
}

// Reveal in the Crystal Powder's rooms shows a gap in the floor of the room of brown and red
// blocks, beside the half face (metatiles (25, 34) to (27, 38)).
void scAnkohlRuinsRevealGap(void)
{
// The Japanese edition leaves the flag alone.
#if ANKOHL_GAP_OPENS_FOR_MOVE_ONLY_WHILE_REVEALED
    fdScriptSetFlag(FLAG_ANKOHL_RUINS_REVEAL_SHOWN);
#endif
    fdMapGridCopyMetatiles(35, 34, 3, 5, 25, 34);
    fdMapGridCopyMetatiles(35, 98, 3, 3, 25, 98);
}

// As Reveal fades, the gap in the room of brown and red blocks is hidden again.
void scAnkohlRuinsHideGap(void)
{
#if ANKOHL_GAP_OPENS_FOR_MOVE_ONLY_WHILE_REVEALED
    fdScriptClearFlag(FLAG_ANKOHL_RUINS_REVEAL_SHOWN);
#endif
    fdMapGridCopyMetatiles(40, 34, 3, 5, 25, 34);
    fdMapGridCopyMetatiles(40, 98, 3, 3, 25, 98);
}

// Nothing visible happens to the sand when Felix casts Reveal in the sand rooms; the game stops
// trimming the sand's edge while Reveal is up.
void scAnkohlRuinsStartReveal(void)
{
    syGameFlagSet(FLAG_ANKOHL_RUINS_REVEAL_SHOWN);
}

// As Reveal fades in the sand rooms the sand's edge is trimmed again.
void scAnkohlRuinsEndReveal(void)
{
    syGameFlagClear(FLAG_ANKOHL_RUINS_REVEAL_SHOWN);
}

const CUE *scGetCues(void)
{
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_STONE_FACES)
        return AnkohlRuinsStoneFacesCues;
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_THANATOS_MACE_CHEST)
        return AnkohlRuinsThanatosMaceChestCues;
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_GREAT_FACE)
        return AnkohlRuinsGreatFaceCues;
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_FACE_HALVES)
        return AnkohlRuinsFaceHalvesCues;
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST)
        return AnkohlRuinsCrystalPowderChestCues;
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_SAND_DOORS)
        return AnkohlRuinsSandDoorsCues;
    if (MainRoutineStateWork.next == SCENE_ANKOHL_RUINS_LEFT_PRONG)
        return AnkohlRuinsLeftProngCues;
    return AnkohlRuinsCues;
}

#include "shared/prong_sparkle.h"

void          scAnkohlRuinsRiseFromSand(void);
void          scAnkohlRuinsCarriedOffLedge(void);
void          scAnkohlRuinsPressFirstSandSwitch(void);
void          scAnkohlRuinsOpenFirstSandDoor(void);
void          scAnkohlRuinsPressSecondSandSwitch(void);
void          scAnkohlRuinsOpenSecondSandDoor(void);
void          scAnkohlRuinsStartGreatFaceSliding(void);
void          scAnkohlRuinsStartGreatFaceSlidingUpClose(void);
void          scAnkohlRuinsPushFaceHalfIntoGap(void);
void          scAnkohlRuinsRevealGap(void);
void          scAnkohlRuinsStartReveal(void);
void          scAnkohlRuinsEndReveal(void);

extern Uint32 FieldEffectShortScript[];
extern Uint32 FieldEffectMediumScript[];
extern Uint32 FieldEffectLongScript[];
extern Sint32 AnkohlRuinsWeedsScript[];
extern Uint32 AnkohlRuinsFaceHalfScript[];
extern Uint32 AnkohlRuinsGreatFaceScript[];

struct ANKOHL_RUINS_SPR_STATE;

void   scAnkohlRuinsFlowWithSand(void);
void   scAnkohlRuinsLandFromFall(void);

Sint32 scInit(Sint32 unused)
{
    MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;
    Sint32              scene = state->next;
    ACTOR              *actor;

    if (scene == SCENE_ANKOHL_RUINS_STONE_FACES)
    {
        fdEffectStartBlankOverlay(0);
        if (syGameFlagGet(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_A)))
            fdActorSetPos(ACTOR_STONE_FACE_A, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_B)))
            fdActorSetPos(ACTOR_STONE_FACE_B, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_C)))
            fdActorSetPos(ACTOR_STONE_FACE_C, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_D)))
            fdActorSetPos(ACTOR_STONE_FACE_D, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_E)))
            fdActorSetPos(ACTOR_STONE_FACE_E, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_F)))
            fdActorSetPos(ACTOR_STONE_FACE_F, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_G)))
            fdActorSetPos(ACTOR_STONE_FACE_G, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_A)))
            fdActorSetPos(ACTOR_STONE_FACES_WEEDS_A, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_B)))
            fdActorSetPos(ACTOR_STONE_FACES_WEEDS_B, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_C)))
            fdActorSetPos(ACTOR_STONE_FACES_WEEDS_C, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_D)))
            fdActorSetPos(ACTOR_STONE_FACES_WEEDS_D, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_E)))
            fdActorSetPos(ACTOR_STONE_FACES_WEEDS_E, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_F)))
            fdActorSetPos(ACTOR_STONE_FACES_WEEDS_F, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_G)))
            fdActorSetPos(ACTOR_STONE_FACES_WEEDS_G, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_H)))
            fdActorSetPos(ACTOR_STONE_FACES_WEEDS_H, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_I)))
            fdActorSetPos(ACTOR_STONE_FACES_WEEDS_I, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_J)))
            fdActorSetPos(ACTOR_STONE_FACES_WEEDS_J, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_K)))
            fdActorSetPos(ACTOR_STONE_FACES_WEEDS_K, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_L)))
            fdActorSetPos(ACTOR_STONE_FACES_WEEDS_L, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_M)))
            fdActorSetPos(ACTOR_STONE_FACES_WEEDS_M, 0, 0);
        if (syGameFlagGet(FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_N)))
            fdActorSetPos(ACTOR_STONE_FACES_WEEDS_N, 0, 0);
        if (syGameFlagGet(FLAG_NUT_CHEST_PILLAR_MOVED))
        {
            fdActorSetPos(ACTOR_NUT_CHEST_PILLAR, 72, 88);
            fdMapGridCopyMetatileAttributes(4, 68, 1, 1, 2, 69);
        }
    }
    else if (scene == SCENE_ANKOHL_RUINS_THANATOS_MACE_CHEST)
    {
        if (syGameFlagGet(FLAG_THANATOS_WEEDS_SCATTERED(ACTOR_THANATOS_WEEDS_A)))
            fdActorSetPos(ACTOR_THANATOS_WEEDS_A, 0, 0);
        if (syGameFlagGet(FLAG_THANATOS_WEEDS_SCATTERED(ACTOR_THANATOS_WEEDS_B)))
            fdActorSetPos(ACTOR_THANATOS_WEEDS_B, 0, 0);
        if (syGameFlagGet(FLAG_THANATOS_WEEDS_SCATTERED(ACTOR_THANATOS_WEEDS_C)))
            fdActorSetPos(ACTOR_THANATOS_WEEDS_C, 0, 0);
        if (syGameFlagGet(FLAG_THANATOS_FACE_HALF_PLACED))
        {
            Sint32 width;

            actor              = fdActorGetById(ACTOR_THANATOS_FACE_HALF);
            actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
            fdActorSetPos(ACTOR_THANATOS_FACE_HALF, 200, 88);
            width = 10;
            fdMapGridCopyMetatiles(35, 6, 7, 16, width, 7);
            fdMapGridCopyMetatiles(35, 70, 7, 16, width, 71);
            if (state->entrance
                    == ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_GREAT_FACE_EAST
                || state->entrance == THANATOS_FACE_HALL_FROM_WEST_STAIRS
                || state->entrance == THANATOS_FACE_HALL_FROM_EAST_STAIRS)
                fdScriptPlaySound(SE_ANKOHL_RUINS_PIECE);
        }
    }
    else if (scene == SCENE_ANKOHL_RUINS_GREAT_FACE)
    {
        if (syGameFlagGet(FLAG_GREAT_FACE_ROOMS_PILLAR_MOVED))
        {
            fdActorSetPos(ACTOR_GREAT_FACE_ROOMS_PILLAR, 520, 104);
            fdMapGridCopyMetatileAttributes(32, 69, 1, 1, 33, 69);
        }
        if (syGameFlagGet(FLAG_GREAT_FACE_HALF_PLACED))
        {
            fdActorSetPos(ACTOR_GREAT_FACE_HALF, 0, 0);
            fdActorSetAnimation(ACTOR_GREAT_FACE, 4);
        }
        if (syGameFlagGet(FLAG_GREAT_FACE_AT_REST))
        {
            Sint32 width = 13;

            fdMapGridCopyMetatileAttributes(13, 30, 3, 2, width, 24);
            fdMapGridCopyMetatileAttributes(14, 87, 1, 1, 14, 88);
            fdMapGridCopyMetatileAttributes(41, 48, 3, 2, width, 41);
        }
    }
    else if (scene == SCENE_ANKOHL_RUINS_FACE_HALVES)
    {
        if (syGameFlagGet(FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_A)))
            fdActorSetPos(ACTOR_FACE_HALVES_WEEDS_A, 0, 0);
        if (syGameFlagGet(FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_B)))
            fdActorSetPos(ACTOR_FACE_HALVES_WEEDS_B, 0, 0);
        if (syGameFlagGet(FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_C)))
            fdActorSetPos(ACTOR_FACE_HALVES_WEEDS_C, 0, 0);
        if (syGameFlagGet(FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_D)))
            fdActorSetPos(ACTOR_FACE_HALVES_WEEDS_D, 0, 0);
        if (syGameFlagGet(FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_E)))
            fdActorSetPos(ACTOR_FACE_HALVES_WEEDS_E, 0, 0);
        if (syGameFlagGet(FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_F)))
            fdActorSetPos(ACTOR_FACE_HALVES_WEEDS_F, 0, 0);
        if (syGameFlagGet(FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_G)))
            fdActorSetPos(ACTOR_FACE_HALVES_WEEDS_G, 0, 0);
        if (syGameFlagGet(FLAG_FACE_HALVES_PILLAR_MOVED))
            fdActorSetPos(ACTOR_FACE_HALVES_PILLAR, 40, 776);
        if (syGameFlagGet(FLAG_FACE_HALVES_SECOND_PILLAR_MOVED))
            fdActorSetPos(ACTOR_FACE_HALVES_SECOND_PILLAR, 632, 456);
        if (syGameFlagGet(FLAG_FACE_HALVES_THIRD_PILLAR_MOVED))
        {
            fdActorSetPos(ACTOR_FACE_HALVES_THIRD_PILLAR, 616, 136);
            fdMapGridCopyMetatiles(37, 70, 1, 1, 37, 71);
        }
        if (syGameFlagGet(FLAG_LEFT_FACE_HALF_PLACED))
        {
            actor              = fdActorGetById(ACTOR_LEFT_FACE_HALF);
            actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
            fdActorSetPos(ACTOR_LEFT_FACE_HALF, 200, 728);
        }
        if (syGameFlagGet(FLAG_RIGHT_FACE_HALF_PLACED))
        {
            actor = fdActorGetById(ACTOR_RIGHT_FACE_HALF);
            fdActorSetPos(ACTOR_RIGHT_FACE_HALF, 232, 728);
            actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        }
        else if (syGameFlagGet(FLAG_FACE_HALF_PUSHED_INTO_GAP)
                 && !syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
        {
            fdActorSetPos(ACTOR_RIGHT_FACE_HALF, 328, 760);
        }
        if (syGameFlagGet(FLAG_RIGHT_FACE_HALF_PLACED) && syGameFlagGet(FLAG_LEFT_FACE_HALF_PLACED))
        {
            Sint32 width;

            if (MainRoutineStateWork.entrance >= FACE_HALVES_HALL_FIRST_ENTRANCE
                && MainRoutineStateWork.entrance <= FACE_HALVES_HALL_SOUND_LAST_ENTRANCE)
                fdScriptPlaySound(SE_ANKOHL_RUINS_PIECE);
            width = 12;
            fdMapGridCopyMetatiles(16, 27, 3, 13, width, 47);
            fdMapGridCopyMetatiles(16, 91, 3, 13, width, 111);
        }
#if PIT_FALL_DUST_LANDING
        if (!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)
            && MainRoutineStateWork.entrance
                   == ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTHWEST)
        {
            scAnkohlRuinsLandFromFall();
#    if ANKOHL_PIT_FALL_DOES_NOT_REPEAT
            fdScriptClearFlag(FLAG_ARRIVE_FALLING);
#    endif
        }
#endif
        {
            MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;

            if (state->entrance
                == ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_C)
            {
                Sint32 ticks;

                fdScriptStart();
                syFieldScriptEndLastingPsynergy(0);
                fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
                fdActorSetPos(state->leaderActorId, 0, 0);
                fdActorSetPos(ACTOR_RIGHT_FACE_HALF, 0, 0);
                fdScriptStartScreenTransition();
                syFieldScriptWait(15);
                actor = fdActorGetById(ACTOR_RIGHT_FACE_HALF);
                fdActorSetPos(ACTOR_RIGHT_FACE_HALF, 328, 760);
                actor->y.fixed     += toFIXED(128);
                ticks               = 60;
                actor->motionFlags  = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
                for (; ticks != 0; ticks--)
                {
                    syWait(1);
                    if (actor->velocityY > -1)
                        break;
                }
                fdScriptPlaySound(SE_THUMP);
                syFieldScriptWait(60);
                fdScriptSetFlag(FLAG_KEEP_PARTY_POSITION);
                syFieldScriptSetExitRequest(
                    ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_C);
            }
        }
        if (MainRoutineStateWork.entrance >= FACE_HALVES_HALL_FIRST_ENTRANCE
            && MainRoutineStateWork.entrance
                   <= ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_B)
            sySetTickEvent(scAnkohlRuinsFlowWithSand, EVENT_PASS_TICK);
    }
    else if (scene == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST)
    {
        if (syGameFlagGet(FLAG_FACE_HALF_PUSHED_INTO_GAP))
            fdActorSetPos(ACTOR_FACE_HALF_FOR_THE_GAP, 0, 0);
        if (syGameFlagGet(FLAG_CRYSTAL_POWDER_PILLAR_MOVED))
        {
            fdActorSetPos(ACTOR_CRYSTAL_POWDER_PILLAR, 280, 312);
            fdMapGridCopyMetatileAttributes(16, 84, 1, 1, 18, 84);
        }
        if (state->entrance == 1
            || state->entrance == ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_LEFT_PRONG)
        {
            fdScriptPlaySound(SE_ANKOHL_RUINS_PIECE);
            sySetTickEvent(scAnkohlRuinsFlowWithSand, EVENT_PASS_TICK);
        }
    }
    else if (scene == SCENE_ANKOHL_RUINS_SAND_DOORS)
    {
        // Coming into either sand room from outside.
        if (state->entrance <= ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_FACE_HALVES_NORTH)
            scAnkohlRuinsInitSand(unused);
        if (syGameFlagGet(FLAG_FIRST_SAND_DOOR_OPENED))
        {
            Sint32 x = 14;

            fdMapGridCopyMetatiles(26, 67, 1, 3, x, 65);
            fdMapGridCopyMetatileAttributes(15, 5, 1, 1, x, 5);
        }
        if (syGameFlagGet(FLAG_SECOND_SAND_DOOR_OPENED))
        {
            Sint32 x = 2;

            fdMapGridCopyMetatiles(26, 67, 1, 3, x, 83);
            fdMapGridCopyMetatileAttributes(1, 23, 1, 1, x, 23);
        }
        if (syGameFlagGet(FLAG_KEEP_PARTY_POSITION) && syGameFlagGet(FLAG_SAND_ROOM_FULL))
        {
            FIXED level           = toFIXED(32);

            AnkohlRuinsSand.level = level;
            // The first sand room's two entrances.
            if (state->entrance >= ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_GREAT_FACE
                && state->entrance
                       <= ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST)
            {
                Sint32 z;
                Sint32 x;

                x = 4;
                z = 5;
                fdMapGridCopyMetatileAttributes(25, 22, 9, 2, x, z);
                fdMapGridCopyMetatileAttributes(25, 86, 9, 2, x, 69);
                {
                    Sint32 x = 1;

                    fdMapGridCopyMetatileAttributes(22, 24, 15, 7, x, 7);
                    fdMapGridCopyMetatileAttributes(22, 88, 15, 7, x, 71);
                }
                fdMapGridCopyMetatilesLayered(69, 6, 1, 2, 68, z);
            }
            // The second sand room's two entrances.
            if (state->entrance >= ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_FACE_HALVES_SOUTH
                && state->entrance <= ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_FACE_HALVES_NORTH)
            {
                Sint32 z;
                Sint32 x;

                x = 4;
                z = 23;
                fdMapGridCopyMetatileAttributes(25, 22, 9, 2, x, z);
                fdMapGridCopyMetatileAttributes(25, 86, 9, 2, x, 87);
                {
                    Sint32 x = 1;

                    fdMapGridCopyMetatileAttributes(22, 24, 15, 7, x, 25);
                    fdMapGridCopyMetatileAttributes(22, 88, 15, 7, x, 89);
                }
                fdMapGridCopyMetatilesLayered(69, 24, 1, 4, 68, z);
            }
            actor               = fdActorGetById(state->leaderActorId);
            actor->groundHeight = level;
            actor->y.fixed      = level;
            syFieldScriptWait(1);
            fdMapGridCenterOnFollowedPoint();
        }
    }
    else if (scene == SCENE_ANKOHL_RUINS_LEFT_PRONG)
    {
        actor = fdActorGetById(ACTOR_LEFT_PRONG_CHEST);
        if (actor != NULL)
        {
            actor->motionFlags  = 0;
            actor->groundHeight = toFIXED(72);
            actor->y.fixed      = toFIXED(72);
            actor->update       = (OBJECT_UPDATE)scProngSparkle;
        }
    }
    return 0;
}

Sint32 scPrepareMap(void)
{
    return 0;
}

void scAnkohlRuinsJumpDownLedge(void);
void scAnkohlRuinsSinkIntoSand(void);
void scAnkohlRuinsRaiseFirstSand(void);
void scAnkohlRuinsRaiseSecondSand(void);
void scAnkohlRuinsHideGap(void);

// clang-format off
#include "shared/field_effect_duration_scripts.c"

// The first sand room's metatile blocks, each with the sand level that covers it, ended by SAND_ROWS_END.
ANKOHL_RUINS_SAND_ROW AnkohlRuinsFirstSandRows[] = {
    {0, 15, 4, 1, 8},  {0, 3, 3, 4, 5},   {0, 6, 2, 7, 6},
    {0, 1, 1, 10, 5},  {0, 1, 1, 12, 5},  {2, 1, 1, 11, 5},
    {2, 15, 1, 1, 12}, {4, 15, 1, 1, 13}, {SAND_ROWS_END},
};

// The second sand room's metatile blocks, each with the sand level that covers it, ended by SAND_ROWS_END.
ANKOHL_RUINS_SAND_ROW AnkohlRuinsSecondSandRows[] = {
    {0, 15, 2, 1, 28}, {0, 12, 2, 4, 26}, {0, 8, 2, 4, 24},
    {0, 2, 1, 5, 23},  {0, 2, 1, 10, 23}, {0, 1, 1, 15, 25},
    {2, 1, 1, 4, 23},  {2, 15, 1, 1, 30}, {4, 15, 1, 1, 31},
    {SAND_ROWS_END},
};

const Sint32 *FieldEffectDurationScripts[] = {
    FieldEffectShortScript,
    FieldEffectMediumScript,
    FieldEffectLongScript,
};

SCENE_ENTRANCE SceneEntrances[] = {
    {0, CONDITION_ALWAYS, 136, 0, 120, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// The Crystal Powder's rooms' way out onto the top of the ruins.
SCENE_REGION AnkohlRuinsCrystalPowderChestExitRegions[] = {
    {819, 238, 852, 827, 246, 860, CONDITION_ALWAYS,
     ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_LEFT_PRONG},
    {SCENE_TABLE_END},
};

// The way back down from the top of the ruins to the Crystal Powder's rooms.
SCENE_REGION AnkohlRuinsLeftProngExitRegions[] = {
    {180, -26, 164, 188, -18, 172, CONDITION_ALWAYS,
     ANKOHL_RUINS_LEFT_PRONG_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST},
    {SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
    SCENE_EXITS(AnkohlRuinsEntrance),
    SCENE_EXIT(ANKOHL_RUINS_ENTRANCE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_ANKOHL_RUINS_ENTRANCE),
    SCENE_EXIT(ANKOHL_RUINS_ENTRANCE_TO_ANKOHL_RUINS_STONE_FACES, AnkohlRuinsStoneFaces, ANKOHL_RUINS_STONE_FACES_FROM_ANKOHL_RUINS_ENTRANCE),
    SCENE_EXITS(AnkohlRuinsStoneFaces),
    SCENE_EXIT(ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_ENTRANCE, AnkohlRuinsEntrance, ANKOHL_RUINS_ENTRANCE_FROM_ANKOHL_RUINS_STONE_FACES),
    SCENE_EXIT(2, AnkohlRuinsStoneFaces, 11),
    SCENE_EXIT(3, AnkohlRuinsStoneFaces, 5),
    SCENE_EXIT(4, AnkohlRuinsStoneFaces, 6),
    SCENE_EXIT(5, AnkohlRuinsStoneFaces, 3),
    SCENE_EXIT(6, AnkohlRuinsStoneFaces, 4),
    SCENE_EXIT(ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_SOUTHWEST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_STONE_FACES_SOUTHWEST),
    SCENE_EXIT(8, AnkohlRuinsStoneFaces, 12),
    SCENE_EXIT(9, AnkohlRuinsStoneFaces, 10),
    SCENE_EXIT(10, AnkohlRuinsStoneFaces, 9),
    SCENE_EXIT(11, AnkohlRuinsStoneFaces, 2),
    SCENE_EXIT(12, AnkohlRuinsStoneFaces, 8),
    SCENE_EXIT(13, AnkohlRuinsStoneFaces, 20),
    SCENE_EXIT(14, AnkohlRuinsStoneFaces, 21),
    SCENE_EXIT(15, AnkohlRuinsStoneFaces, 22),
    SCENE_EXIT(ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_SAND_DOORS, AnkohlRuinsSandDoors, ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_STONE_FACES),
    SCENE_EXIT(17, AnkohlRuinsStoneFaces, 24),
    SCENE_EXIT(18, AnkohlRuinsStoneFaces, 25),
    SCENE_EXIT(ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_STONE_FACES),
    SCENE_EXIT(20, AnkohlRuinsStoneFaces, 13),
    SCENE_EXIT(21, AnkohlRuinsStoneFaces, 14),
    SCENE_EXIT(22, AnkohlRuinsStoneFaces, 15),
    SCENE_EXIT(24, AnkohlRuinsStoneFaces, 17),
    SCENE_EXIT(25, AnkohlRuinsStoneFaces, 18),
    SCENE_EXIT(27, AnkohlRuinsStoneFaces, 30),
    SCENE_EXIT(29, AnkohlRuinsStoneFaces, 31),
    SCENE_EXIT(30, AnkohlRuinsStoneFaces, 27),
    SCENE_EXIT(31, AnkohlRuinsStoneFaces, 29),
    SCENE_EXIT(ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_EAST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_STONE_FACES_EAST),
    SCENE_EXIT(ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_STONE_FACES_WEST),
    SCENE_EXITS(AnkohlRuinsThanatosMaceChest),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_STONE_FACES_SOUTHWEST, AnkohlRuinsStoneFaces, ANKOHL_RUINS_STONE_FACES_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_SOUTHWEST),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_STONE_FACES_WEST, AnkohlRuinsStoneFaces, ANKOHL_RUINS_STONE_FACES_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_STONE_FACES_EAST, AnkohlRuinsStoneFaces, ANKOHL_RUINS_STONE_FACES_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_EAST),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_SAND_DOORS, AnkohlRuinsSandDoors, ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_GREAT_FACE_EAST, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_EAST),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_GREAT_FACE_SOUTHWEST, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_SOUTHWEST),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_GREAT_FACE_WEST, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST),
    SCENE_EXIT(11, AnkohlRuinsThanatosMaceChest, 12),
    SCENE_EXIT(12, AnkohlRuinsThanatosMaceChest, 11),
    SCENE_EXIT(14, AnkohlRuinsThanatosMaceChest, 15),
    SCENE_EXIT(15, AnkohlRuinsThanatosMaceChest, 14),
    SCENE_EXIT(16, AnkohlRuinsThanatosMaceChest, 17),
    SCENE_EXIT(17, AnkohlRuinsThanatosMaceChest, 16),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_WEST, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_EAST, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_EAST),
    SCENE_EXITS(AnkohlRuinsGreatFace),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_FACE_HALVES_NORTH, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_GREAT_FACE_NORTH),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_GREAT_FACE_SOUTH),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_GREAT_FACE_WEST),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_SOUTHWEST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_GREAT_FACE_SOUTHWEST),
    SCENE_EXIT(5, AnkohlRuinsGreatFace, 8),
    SCENE_EXIT(6, AnkohlRuinsGreatFace, 10),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_SAND_DOORS, AnkohlRuinsSandDoors, ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_GREAT_FACE),
    SCENE_EXIT(8, AnkohlRuinsGreatFace, 5),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_EAST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_GREAT_FACE_EAST),
    SCENE_EXIT(10, AnkohlRuinsGreatFace, 6),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_FACE_HALVES_WEST, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_GREAT_FACE_WEST),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_FACE_HALVES_EAST, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_GREAT_FACE_EAST),
    SCENE_EXITS(AnkohlRuinsFaceHalves),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_GREAT_FACE_NORTH, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_FACE_HALVES_NORTH),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_NORTH_A, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_FACE_HALVES_NORTH_A),
    SCENE_EXIT(3, AnkohlRuinsFaceHalves, 14),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_NORTH_B, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_FACE_HALVES_NORTH_B),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_GREAT_FACE_WEST, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_FACE_HALVES_WEST),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_GREAT_FACE_EAST, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_FACE_HALVES_EAST),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_SAND_DOORS_NORTH, AnkohlRuinsSandDoors, ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_FACE_HALVES_NORTH),
    SCENE_EXIT(8, AnkohlRuinsFaceHalves, 11),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_GREAT_FACE_SOUTH, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_FACE_HALVES_SOUTH),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_EAST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_FACE_HALVES_EAST),
    SCENE_EXIT(11, AnkohlRuinsFaceHalves, 8),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_SAND_DOORS_SOUTH, AnkohlRuinsSandDoors, ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_FACE_HALVES_SOUTH),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_FACE_HALVES_WEST),
    SCENE_EXIT(14, AnkohlRuinsFaceHalves, 3),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_A, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_FACE_HALVES_SOUTH_A),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_B, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_FACE_HALVES_SOUTH_B),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_C, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_FACE_HALVES_SOUTH_A),
    SCENE_EXITS(AnkohlRuinsCrystalPowderChest),
    SCENE_EXIT(1, AnkohlRuinsCrystalPowderChest, 2),
    SCENE_EXIT(2, AnkohlRuinsCrystalPowderChest, 1),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH_B, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_B),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH_A, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_A),
    SCENE_EXIT(5, AnkohlRuinsCrystalPowderChest, 6),
    SCENE_EXIT(6, AnkohlRuinsCrystalPowderChest, 5),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_NORTH_B, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_NORTH_B),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_NORTH_A, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_NORTH_A),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_SAND_DOORS, AnkohlRuinsSandDoors, ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST),
    SCENE_EXIT(10, AnkohlRuinsCrystalPowderChest, 11),
    SCENE_EXIT(11, AnkohlRuinsCrystalPowderChest, 10),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_LEFT_PRONG, AnkohlRuinsLeftProng, ANKOHL_RUINS_LEFT_PRONG_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_SOUTHWEST, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTHWEST),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH_C, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_C),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_STONE_FACES, AnkohlRuinsStoneFaces, ANKOHL_RUINS_STONE_FACES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST),
    SCENE_EXITS(AnkohlRuinsSandDoors),
    SCENE_EXIT(ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_GREAT_FACE, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_SAND_DOORS),
    SCENE_EXIT(ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_SAND_DOORS),
    SCENE_EXIT(ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_SAND_DOORS_SOUTH),
    SCENE_EXIT(ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_FACE_HALVES_NORTH, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_SAND_DOORS_NORTH),
    SCENE_EXIT(ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_STONE_FACES, AnkohlRuinsStoneFaces, ANKOHL_RUINS_STONE_FACES_FROM_ANKOHL_RUINS_SAND_DOORS),
    SCENE_EXIT(ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_SAND_DOORS),
    SCENE_EXITS(AnkohlRuinsLeftProng),
    SCENE_EXIT(ANKOHL_RUINS_LEFT_PRONG_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_LEFT_PRONG),
    SCENE_EXITS_END,
};

// The weeds' script: each clump is drawn half as tall again, then set up by scAnkohlRuinsSetUpWeeds.
Sint32 AnkohlRuinsWeedsScript[] = {
    SC_SET(SW_SCALE_Y_DUPLICATE, toFIXED(3) / 2),
    SC_CALL(scAnkohlRuinsSetUpWeeds),
    SC_END,
};

// The face halves' script: each stands still until pushed.
Uint32 AnkohlRuinsFaceHalfScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

// The great face's script: it does not collide.
Uint32 AnkohlRuinsGreatFaceScript[] = {
    SC_SET(SW_COLLISION, 0),
    SC_END,
};

// The forest at the ruins' doorway: no actors.
ACTOR_PLACE AnkohlRuinsPlacements[] = {
    {SCENE_TABLE_END},
};

// The halls of the stone faces' actors, in ANKOHL_RUINS_STONE_FACES_ACTOR_ID's order.
ACTOR_PLACE AnkohlRuinsStoneFacesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 456, 0, 840, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 696, 0, 840, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 536, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 568, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 600, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 648, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 584, 0, 56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 168, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 264, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 296, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 184, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 840, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 936, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 856, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// The Thanatos Mace's rooms' actors, in its enum's order.
ACTOR_PLACE AnkohlRuinsThanatosMaceChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR_HALF_A, CONDITION_ALWAYS, (Sint32)AnkohlRuinsFaceHalfScript, 120, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 472, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 520, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 600, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// The great face's rooms' actors, in ANKOHL_RUINS_GREAT_FACE_ACTOR_ID's order.
ACTOR_PLACE AnkohlRuinsGreatFacePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE, CONDITION_ALWAYS, (Sint32)AnkohlRuinsGreatFaceScript, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR_HALF_B, CONDITION_ALWAYS, (Sint32)AnkohlRuinsFaceHalfScript, 296, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// The face halves' rooms' actors, in ANKOHL_RUINS_FACE_HALVES_ACTOR_ID's order.
ACTOR_PLACE AnkohlRuinsFaceHalvesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR_HALF_A, CONDITION_ALWAYS, (Sint32)AnkohlRuinsFaceHalfScript, 104, 0, 760, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR_HALF_B, CONDITION_ALWAYS, (Sint32)AnkohlRuinsFaceHalfScript, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 424, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 504, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 712, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 792, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 648, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 728, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (Sint32)AnkohlRuinsWeedsScript, 808, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// The Crystal Powder's rooms' actors, in ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_ACTOR_ID's order.
ACTOR_PLACE AnkohlRuinsCrystalPowderChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR_HALF_B, CONDITION_ALWAYS, (Sint32)AnkohlRuinsFaceHalfScript, 440, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// The sand rooms' actors: none.
ACTOR_PLACE AnkohlRuinsSandDoorsPlacements[] = {
    {SCENE_TABLE_END},
};

// The top of the ruins' actors: none.
ACTOR_PLACE AnkohlRuinsLeftProngPlacements[] = {
    {SCENE_TABLE_END},
};

// Ticks between the sand's moves as it rises.
Sint16 AnkohlRuinsSandStepDelay = 10;

// Ticks since the sand last rose.
Sint16 AnkohlRuinsSandTimer = 0;

// Ticks between the sand's rises.
Sint16 AnkohlRuinsSandRiseDelay = 0;

// How far the sand rises each move, in 16.16 pixels.
Sint32 AnkohlRuinsSandStep = toFIXED(1) / 2;

// Which sand room's switch was pressed: 1 or 2.
Sint32 AnkohlRuinsSandRoom = 0;

// The forest at the ruins' doorway: its two exits.
CUE AnkohlRuinsCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, ANKOHL_RUINS_ENTRANCE_TO_WORLD_MAP},
    {CUE_EXIT, 2, CONDITION_ALWAYS, ANKOHL_RUINS_ENTRANCE_TO_ANKOHL_RUINS_STONE_FACES},
    {CUE_TABLE_END},
};

// The halls of the stone faces' exits and stairs, Sand, Move on the stone faces and the pillar, and Whirlwind on the weeds.
CUE AnkohlRuinsStoneFacesCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_ENTRANCE},
    {CUE_EXIT, 2, CONDITION_ALWAYS, 2},
    {CUE_EXIT, 3, CONDITION_ALWAYS, 3},
    {CUE_EXIT, 4, CONDITION_ALWAYS, 4},
    {CUE_EXIT, 5, CONDITION_ALWAYS, 5},
    {CUE_EXIT, 6, CONDITION_ALWAYS, 6},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_SOUTHWEST},
    {CUE_EXIT, 8, CONDITION_ALWAYS, 8},
    {CUE_EXIT, 9, CONDITION_ALWAYS, 9},
    {CUE_EXIT, 10, CONDITION_ALWAYS, 10},
    {CUE_EXIT, 11, CONDITION_ALWAYS, 11},
    {CUE_EXIT, 12, CONDITION_ALWAYS, 12},
    {CUE_EXIT, 13, CONDITION_ALWAYS, 13},
    {CUE_EXIT, 14, CONDITION_ALWAYS, 14},
    {CUE_EXIT, 15, CONDITION_ALWAYS, 15},
    {CUE_EXIT, 16, CONDITION_ALWAYS, ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_SAND_DOORS},
    {CUE_EXIT, 17, CONDITION_ALWAYS, 17},
    {CUE_EXIT, 18, CONDITION_ALWAYS, 18},
    {CUE_EXIT, 19, CONDITION_ALWAYS, ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST},
    {CUE_EXIT, 20, CONDITION_ALWAYS, 20},
    {CUE_EXIT, 21, CONDITION_ALWAYS, 21},
    {CUE_EXIT, 22, CONDITION_ALWAYS, 22},
    {CUE_EXIT, 23, CONDITION_ALWAYS, 23},
    {CUE_EXIT, 24, CONDITION_ALWAYS, 24},
    {CUE_EXIT, 25, CONDITION_ALWAYS, 25},
    {CUE_EXIT, 26, CONDITION_ALWAYS, 26},
    {CUE_EXIT, 27, CONDITION_ALWAYS, 27},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 28, CONDITION_ALWAYS, 28},
    {CUE_EXIT, 29, CONDITION_ALWAYS, 29},
    {CUE_EXIT, 30, CONDITION_ALWAYS, 30},
    {CUE_EXIT, 31, CONDITION_ALWAYS, 31},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 32, CONDITION_ALWAYS, ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_EAST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 33, CONDITION_ALWAYS, ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_NUT_CHEST_PILLAR, FLAG_NUT_CHEST_PILLAR_MOVED, SCRIPT_REF(scAnkohlRuinsFinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_NUT_CHEST_PILLAR, FLAG_NUT_CHEST_PILLAR_MOVED, SCRIPT_REF(scAnkohlRuinsStartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_A, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_A), SCRIPT_REF(scAnkohlRuinsPushStoneFaceOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_B, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_B), SCRIPT_REF(scAnkohlRuinsPushStoneFaceOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_C, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_C), SCRIPT_REF(scAnkohlRuinsPushStoneFaceOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_D, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_D), SCRIPT_REF(scAnkohlRuinsPushStoneFaceOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_E, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_E), SCRIPT_REF(scAnkohlRuinsPushStoneFaceOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_F, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_F), SCRIPT_REF(scAnkohlRuinsPushStoneFaceOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_G, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_G), SCRIPT_REF(scAnkohlRuinsPushStoneFaceOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_A, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_B, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_C, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_D, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_E, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_F, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_G, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_H, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_I, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_J, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_K, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_L, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_M, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_N, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsSinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsRiseFromSand)},
    {CUE_TABLE_END},
};

// The Thanatos Mace's rooms' exits and stairs, Sand, Move on the face half and Whirlwind on the weeds.
CUE AnkohlRuinsThanatosMaceChestCues[] = {
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_STONE_FACES_SOUTHWEST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_STONE_FACES_WEST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_STONE_FACES_EAST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_SAND_DOORS},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_GREAT_FACE_EAST},
    {CUE_EXIT, 7, CONDITION_ALWAYS, ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_GREAT_FACE_SOUTHWEST},
    {CUE_EXIT, 10, CONDITION_ALWAYS, ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_GREAT_FACE_WEST},
    {CUE_EXIT, 11, CONDITION_ALWAYS, 11},
    {CUE_EXIT, 12, CONDITION_ALWAYS, 12},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 13, CONDITION_ALWAYS, 13},
    {CUE_EXIT, 14, CONDITION_ALWAYS, 14},
    {CUE_EXIT, 15, CONDITION_ALWAYS, 15},
    {CUE_EXIT, 16, CONDITION_ALWAYS, 16},
    {CUE_EXIT, 17, CONDITION_ALWAYS, 17},
    {CUE_EXIT, 18, CONDITION_ALWAYS, ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_WEST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 19, CONDITION_ALWAYS, ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_EAST},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THANATOS_FACE_HALF, FLAG_THANATOS_FACE_HALF_PLACED, SCRIPT_REF(scAnkohlRuinsPlaceThanatosFaceHalf)},
    {CUE_PUSHED, ACTOR_THANATOS_FACE_HALF, FLAG_THANATOS_FACE_HALF_PLACED, SCRIPT_REF(scAnkohlRuinsPlaceThanatosFaceHalf)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_THANATOS_WEEDS_A, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_THANATOS_WEEDS_B, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_THANATOS_WEEDS_C, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsSinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsRiseFromSand)},
    {CUE_TABLE_END},
};

// The great face's rooms' exits and stairs, Sand, Move on the pillar and the great face's right half, and the steps that set the face sliding.
CUE AnkohlRuinsGreatFaceCues[] = {
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_FACE_HALVES_NORTH},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH},
    {CUE_EXIT, 3, CONDITION_ALWAYS, ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST},
    {CUE_EXIT, 4, CONDITION_ALWAYS, ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_SOUTHWEST},
    {CUE_EXIT, 5, CONDITION_ALWAYS, 5},
    {CUE_EXIT, 6, CONDITION_ALWAYS, 6},
    {CUE_EXIT, 7, CONDITION_ALWAYS, ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_SAND_DOORS},
    {CUE_EXIT, 8, CONDITION_ALWAYS, 8},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_EAST},
    {CUE_EXIT, 10, CONDITION_ALWAYS, 10},
    {CUE_EXIT, 11, CONDITION_ALWAYS, ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_FACE_HALVES_WEST},
    {CUE_EXIT, 12, CONDITION_ALWAYS, ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_FACE_HALVES_EAST},
    {CUE_STEP, METATILE_BEHAVIOR_GREAT_FACE_CORRIDOR_FOOT, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsStartGreatFaceSliding)},
    {CUE_STEP, METATILE_BEHAVIOR_BEFORE_GREAT_FACE, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsStartGreatFaceSlidingUpClose)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_GREAT_FACE_ROOMS_PILLAR, FLAG_GREAT_FACE_ROOMS_PILLAR_MOVED, SCRIPT_REF(scAnkohlRuinsStartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GREAT_FACE_ROOMS_PILLAR, FLAG_GREAT_FACE_ROOMS_PILLAR_MOVED, SCRIPT_REF(scAnkohlRuinsFinishMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GREAT_FACE_HALF, FLAG_GREAT_FACE_HALF_PLACED, SCRIPT_REF(scAnkohlRuinsPlaceGreatFaceHalf)},
    {CUE_PUSHED, ACTOR_GREAT_FACE_HALF, FLAG_GREAT_FACE_HALF_PLACED, SCRIPT_REF(scAnkohlRuinsPlaceGreatFaceHalf)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsSinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsRiseFromSand)},
    {CUE_TABLE_END},
};

// The face halves' rooms' exits and stairs, Sand, Move on the face halves and the pillars, and Whirlwind on the weeds.
CUE AnkohlRuinsFaceHalvesCues[] = {
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_GREAT_FACE_NORTH},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_NORTH_A},
    {CUE_EXIT, 3, CONDITION_ALWAYS, 3},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_NORTH_B},
    {CUE_EXIT, 5, CONDITION_ALWAYS, ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_GREAT_FACE_WEST},
    {CUE_EXIT, 6, CONDITION_ALWAYS, ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_GREAT_FACE_EAST},
    {CUE_EXIT, 7, CONDITION_ALWAYS, ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_SAND_DOORS_NORTH},
    {CUE_EXIT, 8, CONDITION_ALWAYS, 8},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_GREAT_FACE_SOUTH},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS, ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_EAST},
    {CUE_EXIT, 11, CONDITION_ALWAYS, 11},
    {CUE_EXIT, 12, CONDITION_ALWAYS, ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_SAND_DOORS_SOUTH},
    {CUE_EXIT, 13, CONDITION_ALWAYS, ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST},
    {CUE_EXIT, 14, CONDITION_ALWAYS, 14},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 15, CONDITION_ALWAYS, ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_A},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 16, CONDITION_ALWAYS, ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_B},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_HALVES_WEEDS_A, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_HALVES_WEEDS_B, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_HALVES_WEEDS_C, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_HALVES_WEEDS_D, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_HALVES_WEEDS_E, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_HALVES_WEEDS_F, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_HALVES_WEEDS_G, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsScatterWeeds)},
    {CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsFinishPushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_FACE_HALF, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsPlaceFaceHalves)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_FACE_HALF, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsPlaceFaceHalves)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_HALVES_PILLAR, FLAG_FACE_HALVES_PILLAR_MOVED, SCRIPT_REF(scAnkohlRuinsStartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_HALVES_PILLAR, FLAG_FACE_HALVES_PILLAR_MOVED, SCRIPT_REF(scAnkohlRuinsFinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_HALVES_SECOND_PILLAR, FLAG_FACE_HALVES_SECOND_PILLAR_MOVED, SCRIPT_REF(scAnkohlRuinsStartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_HALVES_SECOND_PILLAR, FLAG_FACE_HALVES_SECOND_PILLAR_MOVED, SCRIPT_REF(scAnkohlRuinsFinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_HALVES_THIRD_PILLAR, FLAG_FACE_HALVES_THIRD_PILLAR_MOVED, SCRIPT_REF(scAnkohlRuinsStartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_HALVES_THIRD_PILLAR, FLAG_FACE_HALVES_THIRD_PILLAR_MOVED, SCRIPT_REF(scAnkohlRuinsFinishMovingPillar)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsSinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsRiseFromSand)},
    {CUE_TABLE_END},
};

// The Crystal Powder's rooms' exits and stairs, the pit, the flowing sand's drop, Sand, Move on the pillar and the half face, and Reveal on the gap.
CUE AnkohlRuinsCrystalPowderChestCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, 1},
    {CUE_EXIT, 2, CONDITION_ALWAYS, 2},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH_B},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH_A},
    {CUE_EXIT, 5, CONDITION_ALWAYS, 5},
    {CUE_EXIT, 6, CONDITION_ALWAYS, 6},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_NORTH_B},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_NORTH_A},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_SAND_DOORS},
    {CUE_EXIT, 10, CONDITION_ALWAYS, 10},
    {CUE_EXIT, 11, CONDITION_ALWAYS, 11},
    {STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_PIT_TO_FACE_HALVES, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsJumpDownLedge)},
    {CUE_EXIT, 26, CONDITION_ALWAYS, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_STONE_FACES},
    {CUE_STEP, CUE_CARRIED_OFF_LEDGE, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsCarriedOffLedge)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CRYSTAL_POWDER_PILLAR, FLAG_CRYSTAL_POWDER_PILLAR_MOVED, SCRIPT_REF(scAnkohlRuinsStartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRYSTAL_POWDER_PILLAR, FLAG_CRYSTAL_POWDER_PILLAR_MOVED, SCRIPT_REF(scAnkohlRuinsFinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_HALF_FOR_THE_GAP, FLAG_FACE_HALF_PUSHED_INTO_GAP, SCRIPT_REF(scAnkohlRuinsStartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_HALF_FOR_THE_GAP, FLAG_FACE_HALF_PUSHED_INTO_GAP, SCRIPT_REF(scAnkohlRuinsPushFaceHalfIntoGap)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsRevealGap)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsHideGap)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsSinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsRiseFromSand)},
    {CUE_TABLE_END},
};

// The sand rooms' exits and stairs, Sand, Reveal, the switches, the door buttons and the rising sand.
CUE AnkohlRuinsSandDoorsCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_GREAT_FACE},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST},
    {CUE_EXIT, 3, CONDITION_ALWAYS, ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH},
    {CUE_EXIT, 4, CONDITION_ALWAYS, ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_FACE_HALVES_NORTH},
    {CUE_EXIT, 23, CONDITION_ALWAYS, ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_STONE_FACES},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, 28, CONDITION_ALWAYS, ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST},
    {CUE_STEP, METATILE_BEHAVIOR_FIRST_SAND_SWITCH, FLAG_FIRST_SAND_SWITCH_PRESSED, SCRIPT_REF(scAnkohlRuinsPressFirstSandSwitch)},
    {CUE_STEP, METATILE_BEHAVIOR_FIRST_SAND_DOOR_BUTTON, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsOpenFirstSandDoor)},
    {CUE_STEP, METATILE_BEHAVIOR_SECOND_SAND_SWITCH, FLAG_SECOND_SAND_SWITCH_PRESSED, SCRIPT_REF(scAnkohlRuinsPressSecondSandSwitch)},
    {CUE_STEP, METATILE_BEHAVIOR_SECOND_SAND_DOOR_BUTTON, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsOpenSecondSandDoor)},
    {CUE_RAISED, CUE_RAISE_FIRST_SAND, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsRaiseFirstSand)},
    {CUE_RAISED, CUE_RAISE_SECOND_SAND, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsRaiseSecondSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsSinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsRiseFromSand)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsStartReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scAnkohlRuinsEndReveal)},
    {CUE_TABLE_END},
};

// The top of the ruins' cues: none.
CUE AnkohlRuinsLeftProngCues[] = {
    {CUE_TABLE_END},
};
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY
#undef DEFINE_ANKOHL_RUINS_OPEN_ONCE

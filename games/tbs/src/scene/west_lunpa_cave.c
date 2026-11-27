// West Lunpa Cave (SceneIdWestLunpaCave, area name West Lunpa Cave), the cave between the World Map
// and Lunpa, with a gate across it, where the party meets Bunza on the way out of Lunpa with
// Hammet.
//
// From the World Map the party comes in at the south end (metatile (10, 40)) into a round chamber,
// and goes north up a passage split by a column of rock. Further on, a green metal gate stands half
// raised across the way, with a puddle under it. Beyond the gate the cave runs on to Lunpa, whose
// passage leaves from the east (metatile (34, 21)).
//
// Frost on the puddle under the gate, as the walkthrough says, freezes it into a tall ice pillar
// that pushes the gate up and opens the way. Two switches on the cave wall move the gate too. The
// one east of the gate, on the World Map side (metatile (24, 15)), drops it to the floor over the
// puddle; the one north of it, on the Lunpa side (metatile (21, 8)), raises it so the party can
// walk under. Each does nothing while the gate already stands where it would put it, and neither
// does anything once the ice pillar holds the gate up. Two more puddles freeze into ice pillars:
// one by the rocks on the first chamber's west side, and one in the passage north of the gate.
//
// WEST_LUNPA_CAVE_FROM_LUNPA_UNUSED, which no exit names, repeats WEST_LUNPA_CAVE_FROM_LUNPA with a
// shorter camera.
//
// The gate and the pillars are kept in scene flags, which scInit redraws:
//
// - FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN: the gate's puddle is frozen,
//   propping the gate open.
// - FLAG_WEST_LUNPA_CAVE_GATE_LOWERED: the lowering switch has lowered the
//   gate.
// - FLAG_WEST_LUNPA_CAVE_GATE_RAISED: the gate is raised.
// - FLAG_WEST_LUNPA_CAVE_NORTH_PUDDLE_FROZEN: the north puddle is frozen.
// - FLAG_WEST_LUNPA_CAVE_SOUTH_PUDDLE_FROZEN: the puddle in the first
//   chamber is frozen into a pillar.
//
// The reunion with Bunza, a Kalay merchant, plays here once (FLAG_LUNPA_CAVE_REUNION_SEEN), after
// the party has brought Hammet out of Lunpa Fortress (FLAG_DODONPA_LOCKED_UP). On the way back to
// the World Map, as the party comes down the passage into the first chamber (metatile (11, 29)),
// Bunza steps out further up the passage behind them and Hammet knows him
// (scWestLunpaCaveHammetFindsBunza). They all talk in the first chamber
// (scWestLunpaCaveHoldReunion), and Isaac's answers decide whether the party rides home to Kalay in
// Bunza's wagon or stays behind (scWestLunpaCaveAskAboutWagon).
//
// Riding leaves by WEST_LUNPA_CAVE_BY_WAGON to WORLD_MAP_FROM_WEST_LUNPA_CAVE, where world_map.c
// plays the departure. If the party stays, Hammet and Bunza leave alone. Either way
// FLAG_BUNZAS_WAGON_LEFT is set, on the World Map or here.
//
// Unused: a fourth puddle is placed at the map's top-left corner, metatile (0, 0), where no part of
// the cave the screen shows reaches, so Frost can never be cast on it
// (scWestLunpaCaveUnusedFreezePuddle).

#include "ability_ids.h"
#include "flags.h"
#include "sounds.h"
#include "scene/walk_into_leader.h"
#include "text/msg_ids.h"

extern const SCENE_ENTRANCE WestLunpaCaveEntrances[];
extern const Uint32         WestLunpaCaveExits[];
extern const ACTOR_PLACE    WestLunpaCaveNoActorPlacements[];
extern const ACTOR_PLACE    WestLunpaCaveActorPlacements[];
extern const CUE            WestLunpaCaveCues[];

// The metatiles the cave's cues answer: the two ways out, the two switches and the spot where Bunza
// is seen.
typedef enum WEST_LUNPA_CAVE_METATILE_BEHAVIOR
{
    METATILE_BEHAVIOR_WEST_LUNPA_CAVE_WORLD_MAP_PASSAGE  = 1,
    METATILE_BEHAVIOR_WEST_LUNPA_CAVE_LUNPA_PASSAGE      = 2,
    METATILE_BEHAVIOR_WEST_LUNPA_CAVE_LOWERING_SWITCH    = 3,
    METATILE_BEHAVIOR_WEST_LUNPA_CAVE_RAISING_SWITCH     = 4,
    METATILE_BEHAVIOR_WEST_LUNPA_CAVE_BUNZA_HIDING_PLACE = 10
} WEST_LUNPA_CAVE_METATILE_BEHAVIOR;

// The cave's actors, numbered in the order of WestLunpaCaveActorPlacements.
typedef enum WEST_LUNPA_CAVE_ACTOR_ID
{
    // Placed at the map's top-left corner, off every part of the cave the screen shows, so the
    // player never sees it (scWestLunpaCaveUnusedFreezePuddle).
    ACTOR_UNUSED_PUDDLE = ACTOR_FIRST_PLACED,
    // By the rocks on the first chamber's west side.
    ACTOR_SOUTH_PUDDLE,
    // Under the gate.
    ACTOR_GATE_PUDDLE,
    // In the passage north of the gate.
    ACTOR_NORTH_PUDDLE,
    ACTOR_BUNZA,
    ACTOR_HAMMET
} WEST_LUNPA_CAVE_ACTOR_ID;

// The sprite priority the gate's puddle takes once the gate has come down.
enum
{
    GATE_PILLAR_PRIORITY = 3
};

// The party's lines as Bunza steps out behind them (scWestLunpaCaveHammetFindsBunza), in order.
typedef enum SIGHTING_MESSAGE
{
    SIGHTING_GARET_SAW_SOMEONE,
    SIGHTING_MIA_SAW_SOMETHING,
    SIGHTING_IVAN_ASKS_IF_FOUND,
    SIGHTING_GARET_WILL_FIGHT,
    SIGHTING_GARET_ASKS_WHAT_ELSE
} SIGHTING_MESSAGE;

// Hammet and Bunza recognizing each other (scWestLunpaCaveHammetFindsBunza), in order.
typedef enum RECOGNITION_MESSAGE
{
    RECOGNITION_HAMMET_CALLS_OUT,
    RECOGNITION_HAMMET_NAMES_BUNZA,
    RECOGNITION_BUNZA_KNOWS_VOICE,
    RECOGNITION_BUNZA_NAMES_HAMMET
} RECOGNITION_MESSAGE;

// The reunion's talk in the first chamber (scWestLunpaCaveHoldReunion), in order.
typedef enum REUNION_MESSAGE
{
    REUNION_BUNZA_ASKS_ABOUT_RELEASE,
    REUNION_HAMMET_CREDITS_IVAN,
    REUNION_IVAN_CREDITS_LEADER,
    REUNION_GARET_QUESTIONS_IVAN,
    REUNION_IVAN_CREDITS_EVERYONE,
    REUNION_HAMMET_THANKS_PARTY,
    REUNION_HAMMET_THANKS_IVAN,
    REUNION_MIA_PRAISES_BUNZA,
    REUNION_HAMMET_PRAISES_BUNZA,
    REUNION_BUNZA_CALLS_IT_CHANCE,
    REUNION_BUNZA_AVOIDED_LUNPA,
    REUNION_BUNZA_KNEW_OF_PRISON,
    REUNION_GARET_ASKS_WHY_BUNZA_CAME,
    REUNION_BUNZA_RECALLS_ADVICE,
    REUNION_MIA_ASKS_ABOUT_TRADE,
    REUNION_BUNZA_MEANS_WISDOM,
    REUNION_HAMMET_ON_APPEARANCES,
    REUNION_BUNZA_ON_UNPLEASANT_PLACES,
    REUNION_HAMMET_ON_SELLING,
    REUNION_IVAN_ASKS_ABOUT_SERVING,
    REUNION_IVAN_ON_FATE,
    REUNION_GARET_ASKS_ABOUT_ENTRY,
    REUNION_BUNZA_WAS_REFUSED,
    REUNION_MIA_ASKS_WHY_BUNZA_STILL_CAME,
    REUNION_BUNZA_MENTIONS_COMMOTION,
    REUNION_IVAN_EXPLAINS_COMMOTION,
    REUNION_BUNZA_LINKS_COMMOTION,
    REUNION_BUNZA_HAD_TO_KNOW,
    REUNION_GARET_ASKS_ABOUT_CAVE,
    REUNION_MIA_ON_GATE,
    REUNION_BUNZA_HID,
    REUNION_HAMMET_STARTLED_BUNZA,
    REUNION_BUNZA_IS_GLAD,
    REUNION_IVAN_ASKS_WHY,
    REUNION_BUNZA_WARNS_OF_SEARCH,
    REUNION_BUNZA_FEARS_CAPTURE,
    REUNION_MIA_URGES_ESCAPE,
    REUNION_IVAN_WANTS_STEALTH,
    REUNION_GARET_ASKS_TO_FIGHT,
    REUNION_BUNZA_DISCOURAGES_FIGHT,
    REUNION_BUNZA_AGREES_NOT_TO_FIGHT
} REUNION_MESSAGE;

// Hammet asking Bunza's plan, and Bunza's wagon to Kalay, in order.
typedef enum PLAN_MESSAGE
{
    PLAN_HAMMET_ASKS_PLAN,
    PLAN_BUNZA_HAS_WAGON,
    PLAN_MIA_ASKS_ABOUT_WAGON,
    PLAN_BUNZA_OFFERS_RIDE,
    PLAN_IVAN_DOUBTS_WAGON,
    PLAN_BUNZA_REASSURES,
    PLAN_GARET_AGREES,
    PLAN_BUNZA_IS_UNSUSPECTED
} PLAN_MESSAGE;

// The lines before the wagon question (scWestLunpaCaveAskAboutWagon).
typedef enum WAGON_MESSAGE
{
    WAGON_BUNZA_LEADS_THE_WAY,
    WAGON_GARET_ASKS_TO_RIDE
} WAGON_MESSAGE;

// Ivan, confused by Isaac's answers, and Garet asking the wagon question again.
typedef enum CONFUSION_MESSAGE
{
    CONFUSION_IVAN_IS_CONFUSED,
    CONFUSION_GARET_ASKS_AGAIN
} CONFUSION_MESSAGE;

// The lines of scWestLunpaCaveBunzaCannotWait.
typedef enum WARNING_MESSAGE
{
    WARNING_IVAN_ASKS_IF_STAYING,
    WARNING_BUNZA_CANNOT_WAIT,
    WARNING_MIA_ASKS_ABOUT_BUSINESS
} WARNING_MESSAGE;

// The lines as the party stays behind (scWestLunpaCaveStayBehind).
typedef enum FAREWELL_MESSAGE
{
    FAREWELL_GARET_STAYS,
    FAREWELL_MIA_STAYS,
    FAREWELL_IVAN_STAYS,
    FAREWELL_HAMMET_LETS_IVAN_GO,
    FAREWELL_BUNZA_SAYS_GOODBYE,
    FAREWELL_GARET_SIGHS,
    FAREWELL_MIA_HOPES_FOR_SAFETY,
    FAREWELL_IVAN_REASSURES,
    FAREWELL_GARET_MOVES_ON
} FAREWELL_MESSAGE;

// The lines as the party rides the wagon (scWestLunpaCaveRideWagon).
typedef enum DEPARTURE_MESSAGE
{
    DEPARTURE_GARET_HEADS_FOR_KALAY,
    DEPARTURE_IVAN_THINKS_OF_LAYANA,
    DEPARTURE_HAMMET_LONGS_FOR_LAYANA,
    DEPARTURE_BUNZA_SETS_OFF
} DEPARTURE_MESSAGE;

// The gate's puddle is frozen into a pillar, propping the gate open.
#define FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN  FLAG_SCENE_TEMP_00
// The lowering switch has lowered the gate.
#define FLAG_WEST_LUNPA_CAVE_GATE_LOWERED        FLAG_SCENE_TEMP_01
// The gate is raised, by the raising switch or the ice pillar under it.
#define FLAG_WEST_LUNPA_CAVE_GATE_RAISED         FLAG_SCENE_TEMP_02
// The north puddle is frozen into a pillar.
#define FLAG_WEST_LUNPA_CAVE_NORTH_PUDDLE_FROZEN FLAG_SCENE_TEMP_03
// The puddle in the first chamber is frozen into a pillar.
#define FLAG_WEST_LUNPA_CAVE_SOUTH_PUDDLE_FROZEN FLAG_SCENE_TEMP_04

static void           scWestLunpaCaveHoldReunion(void);
static void           scWestLunpaCaveAskAboutWagon(void);
static Uint8          scWestLunpaCaveLeaderAnswersYes(void);
static Uint8          scWestLunpaCaveGaretAsksIfNotRiding(void);
static Uint8          scWestLunpaCaveGaretAsksAboutUnfinishedBusiness(void);
static Uint8          scWestLunpaCaveConfirmStaying(void);
static Uint8          scWestLunpaCaveBunzaAsksAboutUnfinishedBusiness(void);
static Uint8          scWestLunpaCaveGaretChecksNothingLeftToDo(void);
static Uint8          scWestLunpaCaveMiaAsksIfRidingAfterAll(void);
static Uint8          scWestLunpaCaveBunzaCannotWait(void);
static Uint8          scWestLunpaCaveGaretAsksAboutThingsToDo(void);
static void           scWestLunpaCaveStayBehind(void);
static void           scWestLunpaCaveRideWagon(void);

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return WestLunpaCaveEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

// The ways out: to the World Map, to Lunpa, and away in Bunza's wagon.
const Uint32 *scGetExits(void)
{
    return WestLunpaCaveExits;
}

// The puddles, and Bunza, Hammet and the party for the reunion, kept off the stage until it plays.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    if (MainRoutineStateWork.next == SCENE_WEST_LUNPA_CAVE)
    {
        return WestLunpaCaveActorPlacements;
    }
    return WestLunpaCaveNoActorPlacements;
}

// The cave's scripts: its ways out, the two switches, Frost on each puddle and the spot where Bunza
// is seen.
const CUE *scGetCues(void)
{
    return WestLunpaCaveCues;
}

// Unused: Frost on the fourth puddle, at the map's top-left corner (metatile (0, 0)) where the
// screen never goes, would freeze it, and no flag would remember it. The player can never cast it
// there.
static void scWestLunpaCaveUnusedFreezePuddle(void)
{
    ACTOR *puddle;

    puddle = fdActorGet(ACTOR_UNUSED_PUDDLE);
    if (puddle != NULL)
    {
        fdActorSetSpriteFlags(puddle, 0);
    }
}

// Frost on the puddle by the rocks on the first chamber's west side (metatile (8, 32)) freezes it
// into a tall ice pillar against the wall. It only stands in the way: walking into it from the
// east, Isaac stops beside it and does not climb onto it.
static void scWestLunpaCaveFreezeSouthPuddle(void)
{
    ACTOR *puddle;

    puddle = fdActorGet(ACTOR_SOUTH_PUDDLE);
    if (puddle != NULL)
    {
        puddle->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
        puddle->motionFlags   = 0;
    }
    // The pillar's metatile takes the height of the metatiles along the west wall, (7, 30..34), 64
    // pixels up: the top of the wall's rock, which no floor leads onto.
    fdMapGridCopyMetatileAttributes(7, 32, 1, 1, 8, 32);
    gmFlagSet(FLAG_WEST_LUNPA_CAVE_SOUTH_PUDDLE_FROZEN);
}

// The gate rises onto the ice pillar under it in steps of four ticks and stays up, and the way
// under it opens.
static void scWestLunpaCaveGateDrawPropped(void)
{
    ACTOR *pillar;

    pillar = fdActorGet(ACTOR_GATE_PUDDLE);
    fdActorSetAnimation(ACTOR_GATE_PUDDLE, PUDDLE_ANIMATION_PILLAR);
    if (pillar != NULL)
    {
        fdActorSetSpriteFlags(pillar, 0);
        pillar->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    }
    fdMapGridCopyMetatiles(41, 87, 2, 5, 21, 59);
    sySynch(4);
    fdMapGridCopyMetatiles(3, 93, 1, 1, 24, 62);
    fdMapGridCopyMetatiles(1, 94, 1, 1, 21, 55);
    fdMapGridCopyMetatiles(43, 87, 2, 5, 21, 58);
    sySynch(4);
    fdMapGridCopyMetatiles(41, 87, 2, 5, 21, 58);
    sySynch(4);
    sySynch(4);
    fdMapGridCopyMetatileAttributes(21, 11, 2, 2, 21, 13);
    fdMapGridCopyMetatileAttributes(21, 11, 1, 1, 22, 15);
    fdMapGridCopyMetatileAttributes(19, 17, 1, 1, 21, 14);
}

// Frost on the puddle under the half-raised gate (metatile (21, 15)) freezes it into a tall ice
// pillar. Unless the gate is already raised, the pillar pushes it up and the puzzle-solved tune
// plays: the way on is open.
static void scWestLunpaCaveFreezeGatePuddle(void)
{
    ACTOR *pillar;

    pillar = fdActorGet(ACTOR_GATE_PUDDLE);
    gmFlagSet(FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN);
    if (pillar != NULL)
    {
        fdActorSetSpriteFlags(pillar, 0);
        pillar->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    }
    if (fdScriptGetFlag(FLAG_WEST_LUNPA_CAVE_GATE_RAISED) == 0)
    {
        sySoundPlay(SE_GATE_MOVE);
        scWestLunpaCaveGateDrawPropped();
        sySoundPlay(MUS_PUZZLE_SOLVED);
        gmFlagSet(FLAG_WEST_LUNPA_CAVE_GATE_RAISED);
    }
}

// Frost on the puddle in the passage north of the gate, east of the raising switch (metatile (26,
// 8)), freezes it into a tall ice pillar, which is still there when the party comes back.
static void scWestLunpaCaveFreezeNorthPuddle(void)
{
    gmFlagSet(FLAG_WEST_LUNPA_CAVE_NORTH_PUDDLE_FROZEN);
}

// The gate drops to the floor in steps of four ticks, over the puddle, and the way under it is
// shut.
static void scWestLunpaCaveGateLower(void)
{
    if (gmFlagGet(FLAG_WEST_LUNPA_CAVE_GATE_RAISED) != 0)
    {
        fdMapGridCopyMetatiles(41, 86, 2, 6, 21, 57);
        sySynch(4);
        fdMapGridCopyMetatiles(43, 86, 2, 6, 21, 57);
        sySynch(4);
        fdMapGridCopyMetatiles(41, 86, 2, 6, 21, 58);
        sySynch(4);
        fdMapGridCopyMetatiles(43, 86, 2, 6, 21, 58);
        sySynch(4);
    }
    fdMapGridCopyMetatiles(2, 93, 1, 1, 24, 62);
    fdMapGridCopyMetatiles(2, 94, 1, 1, 21, 55);
    fdMapGridCopyMetatiles(41, 86, 2, 6, 21, 59);
    sySynch(4);
    fdMapGridCopyMetatiles(1, 93, 1, 1, 24, 62);
    fdMapGridCopyMetatiles(3, 94, 1, 1, 21, 55);
    fdMapGridCopyMetatiles(43, 86, 2, 6, 21, 59);
    sySynch(4);
    fdActorSetSpritePriority(ACTOR_GATE_PUDDLE, GATE_PILLAR_PRIORITY);
    fdMapGridCopyMetatileAttributes(19, 17, 1, 1, 22, 15);
}

// Isaac flips the switch on the wall east of the gate, on the World Map side (metatile (24, 15)):
// "Isaac flipped the switch." The gate drops to the floor, covering the puddle. Nothing happens
// while the gate is already down or the ice pillar holds it up.
static void scWestLunpaCaveFlipLoweringSwitch(void)
{
    if (fdScriptGetFlag(FLAG_WEST_LUNPA_CAVE_GATE_LOWERED) == 0)
    {
        if (gmFlagGet(FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN) == 0)
        {
            mnFieldMessageShow(MsgFieldFlippedSwitch, 1);
            sySoundPlay(SE_GATE_MOVE);
            scWestLunpaCaveGateLower();
            gmFlagSet(FLAG_WEST_LUNPA_CAVE_GATE_LOWERED);
            gmFlagClear(FLAG_WEST_LUNPA_CAVE_GATE_RAISED);
        }
    }
}

// The gate rises clear in steps of four ticks, and the way under it opens.
static void scWestLunpaCaveGateRaise(void)
{
    fdMapGridCopyMetatiles(41, 87, 2, 5, 21, 59);
    sySynch(4);
    fdMapGridCopyMetatiles(2, 93, 1, 1, 24, 62);
    fdMapGridCopyMetatiles(2, 94, 1, 1, 21, 55);
    fdMapGridCopyMetatiles(43, 87, 2, 5, 21, 58);
    sySynch(4);
    fdMapGridCopyMetatiles(3, 93, 1, 1, 24, 62);
    fdMapGridCopyMetatiles(1, 94, 1, 1, 21, 55);
    fdMapGridCopyMetatiles(41, 87, 2, 5, 21, 58);
    fdMapGridCopyMetatileAttributes(21, 11, 2, 2, 21, 13);
    fdMapGridCopyMetatileAttributes(19, 17, 1, 1, 21, 14);
}

// Isaac flips the switch on the wall north of the gate, on the Lunpa side (metatile (21, 8)):
// "Isaac flipped the switch." The gate rises so the party can walk under it. Nothing happens while
// the gate is already raised or the ice pillar holds it up.
static void scWestLunpaCaveFlipRaisingSwitch(void)
{
    if (gmFlagGet(FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN) == 0)
    {
        if (fdScriptGetFlag(FLAG_WEST_LUNPA_CAVE_GATE_RAISED) == 0)
        {
            mnFieldMessageShow(MsgFieldFlippedSwitch, 1);
            sySoundPlay(SE_GATE_MOVE);
            scWestLunpaCaveGateRaise();
            gmFlagSet(FLAG_WEST_LUNPA_CAVE_GATE_RAISED);
            gmFlagClear(FLAG_WEST_LUNPA_CAVE_GATE_LOWERED);
        }
    }
}

// On the way out of Lunpa with Hammet, as the party comes down the passage into the first chamber
// (metatile (11, 29)), Bunza steps out further up the passage behind them. Garet and Mia saw
// someone, and Ivan asks whether Dodonpa's henchmen have found them. Whatever Isaac answers, Hammet
// calls out, "Is that you, Bunza?", and Bunza knows his voice: "It's Hammet!" The screen closes for
// the talk that follows.
static void scWestLunpaCaveHammetFindsBunza(void)
{
    Sint32 sighting;
    Sint32 recognition;

    if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0)
    {
        gmFlagSet(FLAG_LUNPA_CAVE_REUNION_SEEN);
        fdScriptStart();
        fdActorSetPos(ACTOR_BUNZA, 144, 400);
        fdActorSetSpeed(ACTOR_BUNZA, 1.5);
        fdActorWalkTo(ACTOR_BUNZA, 184, 400);
        fdActorWaitForMoveById(ACTOR_BUNZA);
        fdActorSetAnimation(ACTOR_BUNZA, ANIM_STAND);
        fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHEAST, 0);
        fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        fdScriptWait(30);
        fdCameraSetSpeed(0.5);
        fdCameraMoveTo(192, CAMERA_KEEP, 432, 1);
        scPlaceAtLeader(ACTOR_HAMMET);
        fdActorSetSpeed(ACTOR_HAMMET, 1.3);
        fdActorWalkTo(ACTOR_HAMMET, 168, 464);
        fdActorWaitForMoveById(ACTOR_HAMMET);
        fdActorFaceDirection(ACTOR_HAMMET, FACING_NORTH, 0);
        scPlaceAtLeader(ACTOR_IVAN);
        fdActorSetSpeed(ACTOR_IVAN, 1.3);
        fdActorWalkTo(ACTOR_IVAN, 152, 488);
        fdActorWaitForMoveById(ACTOR_IVAN);
        fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        scPlaceAtLeader(ACTOR_MIA);
        fdActorSetSpeed(ACTOR_MIA, 1.3);
        fdActorWalkTo(ACTOR_MIA, 168, 488);
        fdActorWaitForMoveById(ACTOR_MIA);
        fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        scPlaceAtLeader(ACTOR_GARET);
        fdActorSetSpeed(ACTOR_GARET, 1.3);
        fdActorWalkTo(ACTOR_GARET, 184, 488);
        fdActorWaitForMoveById(ACTOR_GARET);
        fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
        fdScriptWait(30);

        // The party spots someone ahead, before Bunza recognizes Hammet's voice.
        fdActorRunRepeatedMotion(ACTOR_GARET, 1);
        sighting = MsgLunpaThinkSawSomeone;
        fdScriptSetMessageId(sighting + SIGHTING_GARET_SAW_SOMEONE);
        fdScriptShowMessage(ACTOR_GARET, 0);
        fdScriptWait(30);
        fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
        fdScriptWait(10);
        fdScriptSetMessageId(sighting + SIGHTING_MIA_SAW_SOMETHING);
        fdScriptShowMessage(ACTOR_MIA, 0);
        fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
        fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
        fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
        fdScriptSetMessageId(sighting + SIGHTING_IVAN_ASKS_IF_FOUND);
        fdScriptOpenMessage(ACTOR_IVAN, 0);
        fdActorFaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
        fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
        fdScriptWait(30);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
        {
            fdScriptSetMessageId(sighting + SIGHTING_GARET_WILL_FIGHT);
            fdScriptShowMessage(ACTOR_GARET, 0);
        }
        else
        {
            fdScriptSetMessageId(sighting + SIGHTING_GARET_ASKS_WHAT_ELSE);
            fdScriptShowMessage(ACTOR_GARET, 0);
        }

        fdActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
        recognition = MsgLunpaMoment;
        fdScriptSetMessageId(recognition + RECOGNITION_HAMMET_CALLS_OUT);
        fdScriptShowMessage(ACTOR_HAMMET, 0);
        fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        fdActorStartRepeatedMotion(ACTOR_GARET, 2);
        fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
        fdActorStartRepeatedMotion(ACTOR_MIA, 2);
        fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
        fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
        fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        fdActorSetAnimation(ACTOR_HAMMET, ANIM_WALK);
        fdActorWalkBy(ACTOR_HAMMET, 0, -16);
        fdActorWaitForMoveById(ACTOR_HAMMET);
        fdActorSetAnimation(ACTOR_HAMMET, ANIM_STAND);
        fdScriptSetMessageId(recognition + RECOGNITION_HAMMET_NAMES_BUNZA);
        fdScriptShowMessage(ACTOR_HAMMET, 0);
        fdActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 65);
        fdScriptSetMessageId(recognition + RECOGNITION_BUNZA_KNOWS_VOICE);
        fdScriptShowMessage(ACTOR_BUNZA, 0);
        fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
        fdScriptWait(80);
        fdActorSetSpeed(ACTOR_BUNZA, 0.4);
        fdActorWalkBy(ACTOR_BUNZA, -13, 0);
        fdActorWaitForMoveById(ACTOR_BUNZA);
        fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH, 0);
        fdActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
        fdScriptSetMessageId(recognition + RECOGNITION_BUNZA_NAMES_HAMMET);
        fdScriptShowMessage(ACTOR_BUNZA, 0);
        fdActorWalkTo(ACTOR_BUNZA, 168, 432);
        fdScriptWait(40);
        fdScriptFinishScreenTransition();
        fdScriptWaitTransitionTicks();
        fdScriptWait(20);
        fdScriptEnd();
        scWestLunpaCaveHoldReunion();
    }
}

// The screen opens on everyone gathered in the first chamber. Bunza learns that Dodonpa never meant
// to free Hammet and that Ivan and his friends came to the rescue, tells how he came to be hiding
// in the cave, and offers them all a ride to Kalay in his wagon.
static void scWestLunpaCaveHoldReunion(void)
{
    Sint32 reunion;
    Sint32 msgBase;

    // Arrange the reunion while the screen is closed, then reveal the group.
    fdScriptStart();
    fdActorSetPos(ACTOR_GARET, 200, 544);
    fdActorSetPos(ACTOR_PARTY_LEADER, 184, 544);
    fdActorSetPos(ACTOR_MIA, 168, 544);
    fdActorSetPos(ACTOR_IVAN, 212, 528);
    fdActorSetPos(ACTOR_HAMMET, 200, 512);
    fdActorSetPos(ACTOR_BUNZA, 168, 512);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_HAMMET, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_BUNZA, FACING_EAST, 0);
    fdActorSetAnimation(ACTOR_BUNZA, ANIM_STAND);
    fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
    sySynch(1);
    fdMapGridCenterOnFollowedPoint();
    sySynch(1);
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptWait(20);

    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
    reunion = MsgLunpaDodonpaNeverIntention;
    fdScriptSetMessageId(reunion + REUNION_BUNZA_ASKS_ABOUT_RELEASE);
    fdScriptShowMessage(ACTOR_BUNZA, 0);
    fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    fdScriptSetMessageId(reunion + REUNION_HAMMET_CREDITS_IVAN);
    fdScriptShowMessage(ACTOR_HAMMET, 0);
    fdActorFaceActor(ACTOR_IVAN, ACTOR_HAMMET, 0);
    fdScriptWait(30);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
    fdScriptWait(40);
    fdScriptSetMessageId(reunion + REUNION_IVAN_CREDITS_LEADER);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 80);
    fdScriptSetMessageId(reunion + REUNION_GARET_QUESTIONS_IVAN);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
    fdActorFaceActor(ACTOR_IVAN, ACTOR_GARET, 0);
    fdScriptSetMessageId(reunion + REUNION_IVAN_CREDITS_EVERYONE);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    fdActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(60);
    fdActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptWait(60);
    fdScriptSetMessageId(reunion + REUNION_HAMMET_THANKS_PARTY);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    fdScriptWait(30);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(120);
    fdActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_SMILE, 80);
    fdScriptSetMessageId(reunion + REUNION_HAMMET_THANKS_IVAN);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    fdActorFaceActor(ACTOR_IVAN, ACTOR_HAMMET, 0);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);

    // Bunza explains how he reached the cave despite being refused entry.
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    fdActorFaceActor(ACTOR_MIA, ACTOR_BUNZA, 0);
    fdScriptSetMessageId(reunion + REUNION_MIA_PRAISES_BUNZA);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    fdActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    fdActorFaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
    fdScriptSetMessageId(reunion + REUNION_HAMMET_PRAISES_BUNZA);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    fdActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
    fdScriptSetMessageId(reunion + REUNION_BUNZA_CALLS_IT_CHANCE);
    fdScriptShowMessage(ACTOR_BUNZA, 0);
    fdActorRunRepeatedMotion(ACTOR_BUNZA, 1);
    fdScriptWait(20);
    fdScriptSetMessageId(reunion + REUNION_BUNZA_AVOIDED_LUNPA);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    fdActorTurnToFaceEachOther(ACTOR_HAMMET, ACTOR_IVAN, 0);
    fdScriptWait(60);
    fdActorFaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
    fdActorFaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
    fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    fdScriptWait(30);
    fdScriptSetMessageId(reunion + REUNION_BUNZA_KNEW_OF_PRISON);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    fdActorRunRepeatedMotion(ACTOR_GARET, 1);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_GARET, ACTOR_BUNZA, 0);
    fdScriptSetMessageId(reunion + REUNION_GARET_ASKS_WHY_BUNZA_CAME);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
    fdActorFaceActor(ACTOR_BUNZA, ACTOR_GARET, 0);
    fdScriptWait(20);
    fdScriptSetMessageId(reunion + REUNION_BUNZA_RECALLS_ADVICE);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    fdActorFaceActor(ACTOR_MIA, ACTOR_BUNZA, 0);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 70);
    fdScriptSetMessageId(reunion + REUNION_MIA_ASKS_ABOUT_TRADE);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    fdScriptWait(40);
    fdScriptSetMessageId(reunion + REUNION_BUNZA_MEANS_WISDOM);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    fdActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
    fdScriptSetMessageId(reunion + REUNION_HAMMET_ON_APPEARANCES);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    fdActorFaceActor(ACTOR_BUNZA, ACTOR_HAMMET, 0);
    fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    fdScriptWait(30);
    fdScriptSetMessageId(reunion + REUNION_BUNZA_ON_UNPLEASANT_PLACES);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    fdScriptWait(30);
    fdScriptSetMessageId(reunion + REUNION_HAMMET_ON_SELLING);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    fdActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_SMILE, 70);
    fdActorStartRepeatedMotion(ACTOR_IVAN, 1);
    fdScriptWait(30);
    fdScriptSetMessageId(reunion + REUNION_IVAN_ASKS_ABOUT_SERVING);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    fdActorFaceActor(ACTOR_BUNZA, ACTOR_IVAN, 0);
    fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    fdScriptWait(30);
    fdScriptSetMessageId(reunion + REUNION_IVAN_ON_FATE);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    fdActorRunRepeatedMotion(ACTOR_GARET, 1);
    fdScriptSetMessageId(reunion + REUNION_GARET_ASKS_ABOUT_ENTRY);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
    fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHEAST, 0);
    fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    fdScriptSetMessageId(reunion + REUNION_BUNZA_WAS_REFUSED);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    fdActorRunRepeatedMotion(ACTOR_MIA, 1);
    fdScriptSetMessageId(reunion + REUNION_MIA_ASKS_WHY_BUNZA_STILL_CAME);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    fdActorFaceActor(ACTOR_BUNZA, ACTOR_MIA, 0);
    fdActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    fdScriptSetMessageId(reunion + REUNION_BUNZA_MENTIONS_COMMOTION);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    fdActorTurnToFaceEachOther(ACTOR_IVAN, ACTOR_HAMMET, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    fdScriptSetMessageId(reunion + REUNION_IVAN_EXPLAINS_COMMOTION);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    fdActorFaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
    fdActorFaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
    fdScriptWait(20);
    fdScriptSetMessageId(reunion + REUNION_BUNZA_LINKS_COMMOTION);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    fdActorSetAttachedEffect(ACTOR_BUNZA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptSetMessageId(reunion + REUNION_BUNZA_HAD_TO_KNOW);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_BUNZA, 0);
    fdScriptSetMessageId(reunion + REUNION_GARET_ASKS_ABOUT_CAVE);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
    fdActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    fdScriptSetMessageId(reunion + REUNION_MIA_ON_GATE);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    fdActorRunRepeatedMotion(ACTOR_BUNZA, 1);
    fdScriptSetMessageId(reunion + REUNION_BUNZA_HID);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    fdActorRunRepeatedMotion(ACTOR_BUNZA, 1);
    fdScriptSetMessageId(reunion + REUNION_HAMMET_STARTLED_BUNZA);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    fdScriptWait(30);
    fdActorFaceActor(ACTOR_BUNZA, ACTOR_HAMMET, 0);
    fdScriptWait(60);
    fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    fdScriptSetMessageId(reunion + REUNION_BUNZA_IS_GLAD);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    fdScriptWait(30);

    // Bunza checks the passage, worries about pursuit and proposes his wagon.
    fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH, 0);
    fdActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    fdActorSetSpeed(ACTOR_BUNZA, 1.5);
    fdActorWalkTo(ACTOR_BUNZA, 144, 528);
    fdActorWaitForMoveById(ACTOR_BUNZA);
    fdActorWalkTo(ACTOR_BUNZA, 168, 560);
    fdActorWaitForMoveById(ACTOR_BUNZA);
    fdScriptWait(60);
    fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(40);
    fdActorWalkTo(ACTOR_BUNZA, 144, 528);
    fdActorWaitForMoveById(ACTOR_BUNZA);
    fdActorWalkTo(ACTOR_BUNZA, 168, 488);
    fdScriptSetMessageId(reunion + REUNION_IVAN_ASKS_WHY);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    fdActorWaitForMoveById(ACTOR_BUNZA);
    fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHEAST, 0);
    fdActorSetAnimation(ACTOR_BUNZA, ANIM_STAND);
    fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    fdScriptSetMessageId(reunion + REUNION_BUNZA_WARNS_OF_SEARCH);
    fdScriptShowMessage(ACTOR_BUNZA, 0);
    fdScriptWait(20);
    fdActorTurnToFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(60);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    fdScriptSetMessageId(reunion + REUNION_BUNZA_FEARS_CAPTURE);
    fdScriptShowMessage(ACTOR_BUNZA, 0);
    fdActorRunRepeatedMotion(ACTOR_MIA, 1);
    fdScriptSetMessageId(reunion + REUNION_MIA_URGES_ESCAPE);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    fdScriptSetMessageId(reunion + REUNION_IVAN_WANTS_STEALTH);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    fdScriptSetMessageId(reunion + REUNION_GARET_ASKS_TO_FIGHT);
    fdScriptOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptSetMessageId(reunion + REUNION_BUNZA_DISCOURAGES_FIGHT);
        fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    }
    else
    {
        fdScriptSetMessageId(reunion + REUNION_BUNZA_AGREES_NOT_TO_FIGHT);
        fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    }

    fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHEAST, 0);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_BUNZA, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_BUNZA, 0);
    fdActorFaceActor(ACTOR_MIA, ACTOR_BUNZA, 0);
    fdActorFaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
    fdActorFaceActor(ACTOR_HAMMET, ACTOR_BUNZA, 0);
    fdActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    msgBase = MsgLunpaBunza;
    fdScriptSetMessageId(msgBase + PLAN_HAMMET_ASKS_PLAN);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HAMMET, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_BUNZA, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
    fdScriptSetMessageId(msgBase + PLAN_BUNZA_HAS_WAGON);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    fdScriptSetMessageId(msgBase + PLAN_MIA_ASKS_ABOUT_WAGON);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0);
    fdActorFaceActor(ACTOR_BUNZA, ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptSetMessageId(msgBase + PLAN_BUNZA_OFFERS_RIDE);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    fdActorFaceActor(ACTOR_IVAN, ACTOR_BUNZA, 0);
    fdScriptSetMessageId(msgBase + PLAN_IVAN_DOUBTS_WAGON);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    fdScriptSetMessageId(msgBase + PLAN_BUNZA_REASSURES);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    fdActorTurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    fdScriptWait(60);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_BUNZA, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_BUNZA, 0);
    fdScriptSetMessageId(msgBase + PLAN_GARET_AGREES);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
    fdActorFaceActor(ACTOR_BUNZA, ACTOR_GARET, 0);
    fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    fdScriptWait(20);
    fdScriptSetMessageId(msgBase + PLAN_BUNZA_IS_UNSUSPECTED);
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_BUNZA, 0);
    fdActorTurnToFaceEachOther(ACTOR_HAMMET, ACTOR_IVAN, 0);
    fdScriptWait(60);
    fdActorSetAnimation(ACTOR_HAMMET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(60);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    fdActorFaceActor(ACTOR_BUNZA, ACTOR_PARTY_LEADER, 0);
    fdActorSetAnimation(ACTOR_BUNZA, ANIM_NOD);
    fdScriptWait(60);
    scWestLunpaCaveAskAboutWagon();
}

// Garet asks whether they should ride in the wagon too, and the questions go on until Isaac's
// answers settle it: the party rides to Kalay, or stays behind to finish its business in Lunpa (the
// walkthrough's No, Yes, Yes).
static void scWestLunpaCaveAskAboutWagon(void)
{
    Sint32 wagon;
    Sint32 insisted;
    Sint32 confusion;

    wagon = MsgLunpaLetsTakeWagon;
    fdScriptSetMessageId(wagon + WAGON_BUNZA_LEADS_THE_WAY);
    fdScriptShowMessage(ACTOR_BUNZA, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdScriptSetMessageId(wagon + WAGON_GARET_ASKS_TO_RIDE);
    fdScriptOpenMessage(ACTOR_GARET, 0);
    fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    fdActorFaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
    fdActorFaceActor(ACTOR_HAMMET, ACTOR_PARTY_LEADER, 0);
    fdActorFaceActor(ACTOR_BUNZA, ACTOR_PARTY_LEADER, 0);

ask_to_ride:
    if (scWestLunpaCaveLeaderAnswersYes())
    {
    ask_about_business:
        if (!scWestLunpaCaveBunzaAsksAboutUnfinishedBusiness())
        {
            goto ride;
        }
        insisted = FALSE;
        if (!scWestLunpaCaveGaretAsksAboutThingsToDo())
        {
        insist_nothing_left:
            insisted = TRUE;
        check_nothing_left:
            scWestLunpaCaveGaretChecksNothingLeftToDo();
            if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
            {
                goto ride;
            }
        }
        if (scWestLunpaCaveBunzaCannotWait() || !insisted)
        {
            goto stay;
        }
        goto check_nothing_left;
    }
    else
    {
        if (scWestLunpaCaveGaretAsksIfNotRiding())
        {
            if (scWestLunpaCaveGaretAsksAboutUnfinishedBusiness())
            {
                goto stay;
            }
            goto insist_nothing_left;
        }
        if (scWestLunpaCaveMiaAsksIfRidingAfterAll())
        {
            goto ask_about_business;
        }
        confusion = MsgLunpaTotallyConfusedChanged;
        fdScriptSetMessageId(confusion + CONFUSION_IVAN_IS_CONFUSED);
        fdScriptShowMessage(ACTOR_IVAN, 0);
        fdScriptSetMessageId(confusion + CONFUSION_GARET_ASKS_AGAIN);
        fdScriptOpenMessage(ACTOR_GARET, 0);
        goto ask_to_ride;
    ride:
        scWestLunpaCaveRideWagon();
        return;
    }
stay:
    scWestLunpaCaveConfirmStaying();
    scWestLunpaCaveStayBehind();
}

// The yes-or-no menu answers 0 for Yes; this turns that into true or false.
static Uint8 scWestLunpaCaveLeaderAnswersYes(void)
{
    return fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0;
}

// Garet asks, "Huh? You don't want to ride in Bunza's wagon?"
static Uint8 scWestLunpaCaveGaretAsksIfNotRiding(void)
{
    fdScriptSetMessageId(MsgLunpaGaretAsksIfNotRiding);
    // "Huh? You don't want to ride in Bunza's wagon?"
    fdScriptOpenMessage(ACTOR_GARET, 0);
    return fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0;
}

// Garet asks, "Do you have some unfinished business in Lunpa?"
static Uint8 scWestLunpaCaveGaretAsksAboutUnfinishedBusiness(void)
{
    fdScriptSetMessageId(MsgLunpaGaretAsksAboutUnfinishedBusiness);
    // "Do you have some unfinished business in Lunpa?"
    fdScriptOpenMessage(ACTOR_GARET, 0);
    return fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0;
}

// A step before the party stays that shows nothing and always agrees.
static Uint8 scWestLunpaCaveConfirmStaying(void)
{
    return TRUE;
}

// Bunza asks, "Do you have any unfinished business?"
static Uint8 scWestLunpaCaveBunzaAsksAboutUnfinishedBusiness(void)
{
    fdScriptSetMessageId(MsgLunpaBunzaAsksAboutUnfinishedBusiness);
    // "Do you have any unfinished business?"
    fdScriptOpenMessage(ACTOR_BUNZA, 0);
    return fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0;
}

// Garet asks, "Don't startle me! You don't have anything left to do, do you?"
// scWestLunpaCaveAskAboutWagon takes the answer.
static Uint8 scWestLunpaCaveGaretChecksNothingLeftToDo(void)
{
    fdScriptSetMessageId(MsgLunpaGaretChecksNothingLeft);
    // "Don't startle me! You don't have anything left to do, do you?"
    fdScriptOpenMessage(ACTOR_GARET, 0);
    return TRUE;
}

// Mia asks, "So, what? You've changed your mind? You do want to go with them?"
static Uint8 scWestLunpaCaveMiaAsksIfRidingAfterAll(void)
{
    fdScriptSetMessageId(MsgLunpaMiaAsksIfRidingAfter);
    // "So, what? You've changed your mind? You do want to go with them?"
    fdScriptOpenMessage(ACTOR_MIA, 0);
    return fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0;
}

// Ivan winces: so they won't ride in the wagon? Bunza shakes his head, he cannot safely wait any
// longer, and Mia asks whether they mean to stay and finish their business.
static Uint8 scWestLunpaCaveBunzaCannotWait(void)
{
    Sint32 warning;

    fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    warning = MsgLunpaMeaningWontRide;
    fdScriptSetMessageId(warning + WARNING_IVAN_ASKS_IF_STAYING);
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    fdScriptSetMessageId(warning + WARNING_BUNZA_CANNOT_WAIT);
    fdScriptShowMessage(ACTOR_BUNZA, 0);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    fdScriptSetMessageId(warning + WARNING_MIA_ASKS_ABOUT_BUSINESS);
    fdScriptOpenMessage(ACTOR_MIA, 0);
    return fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0;
}

// Garet asks, "Are you saying you still have some things to do here in Lunpa?"
static Uint8 scWestLunpaCaveGaretAsksAboutThingsToDo(void)
{
    fdScriptSetMessageId(MsgLunpaGaretAsksAboutThingsTo);
    // "Are you saying you still have some things to do here in Lunpa?"
    fdScriptOpenMessage(ACTOR_GARET, 0);
    return fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0;
}

// The party stays: Garet, Mia and Ivan each say they'll stay with Isaac, Hammet lets Ivan go, and
// Hammet and Bunza walk off south out of the cave. The others sigh after them, hope they reach
// Kalay safely, and fall in behind Isaac again.
static void scWestLunpaCaveStayBehind(void)
{
    Sint32 farewell;

    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    farewell = MsgLunpaInsistStick;
    fdScriptSetMessageId(farewell + FAREWELL_GARET_STAYS);
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdActorRunRepeatedMotion(ACTOR_MIA, 1);
    fdScriptSetMessageId(farewell + FAREWELL_MIA_STAYS);
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdActorFaceActor(ACTOR_IVAN, ACTOR_HAMMET, 0);
    fdScriptWait(60);
    fdScriptSetMessageId(farewell + FAREWELL_IVAN_STAYS);
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdActorFaceActor(ACTOR_HAMMET, ACTOR_IVAN, 0);
    fdActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
    fdScriptSetMessageId(farewell + FAREWELL_HAMMET_LETS_IVAN_GO);
    fdScriptShowMessage(ACTOR_HAMMET, 0);
    fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    fdScriptSetMessageId(farewell + FAREWELL_BUNZA_SAYS_GOODBYE);
    fdScriptShowMessage(ACTOR_BUNZA, 0);
    fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(60);
    fdActorSetDestinationOffset(ACTOR_HAMMET, -16, 0);
    fdActorWaitForMoveById(ACTOR_HAMMET);
    fdActorSetAnimation(ACTOR_HAMMET, ANIM_STAND);
    fdScriptWait(40);
    fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    fdActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptWait(30);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    fdActorWalkTo(ACTOR_BUNZA, 156, 528);
    fdScriptWait(20);
    fdActorWalkTo(ACTOR_HAMMET, 164, 528);
    fdActorWaitForMoveById(ACTOR_BUNZA);
    fdActorWalkTo(ACTOR_BUNZA, 168, 640);
    fdActorWaitForMoveById(ACTOR_HAMMET);
    fdActorWalkTo(ACTOR_HAMMET, 168, 640);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    fdScriptWait(60);
    fdActorSetPos(ACTOR_HAMMET, 0, 0);
    fdActorSetPos(ACTOR_BUNZA, 0, 0);
    fdScriptWait(110);

    fdScriptSetMessageId(farewell + FAREWELL_GARET_SIGHS);
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(30);
    fdScriptSetMessageId(farewell + FAREWELL_MIA_HOPES_FOR_SAFETY);
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptSetMessageId(farewell + FAREWELL_IVAN_REASSURES);
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(140);
    fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdScriptSetMessageId(farewell + FAREWELL_GARET_MOVES_ON);
    fdScriptShowMessage(ACTOR_GARET, 0);

    scWalkIntoLeader(ACTOR_GARET);
    scWalkIntoLeader(ACTOR_IVAN);
    scWalkIntoLeader(ACTOR_MIA);
    fdScriptWait(30);
    fdCameraMoveToActorById(ACTOR_PARTY_LEADER, 1);
    fdCameraWaitForArrival();
    fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
    gmFlagSet(FLAG_BUNZAS_WAGON_LEFT);
}

// The party rides: Garet says on to Kalay, Ivan and Hammet look forward to seeing Lady Layana, and
// everyone walks out of the cave to the south, where the World Map plays the wagon's departure.
static void scWestLunpaCaveRideWagon(void)
{
    Sint32 departure;

    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
    departure = MsgLunpaDontUnfinishedBusiness;
    fdScriptSetMessageId(departure + DEPARTURE_GARET_HEADS_FOR_KALAY);
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptSetMessageId(departure + DEPARTURE_IVAN_THINKS_OF_LAYANA);
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdActorFaceActor(ACTOR_HAMMET, ACTOR_IVAN, 0);
    fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    fdScriptWait(20);
    fdScriptSetMessageId(departure + DEPARTURE_HAMMET_LONGS_FOR_LAYANA);
    fdScriptShowMessage(ACTOR_HAMMET, 0);
    fdActorFaceDirection(ACTOR_BUNZA, FACING_SOUTH_SOUTHEAST, 0);
    fdActorSetAnimationAndWait(ACTOR_BUNZA, ANIM_NOD);
    fdScriptWait(30);
    fdScriptSetMessageId(departure + DEPARTURE_BUNZA_SETS_OFF);
    fdScriptShowMessage(ACTOR_BUNZA, 0);
    fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    fdScriptWait(20);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(80);
    fdActorSetDestinationOffset(ACTOR_HAMMET, -16, 0);
    fdActorWaitForMoveById(ACTOR_HAMMET);
    fdActorSetAnimation(ACTOR_HAMMET, ANIM_STAND);
    fdScriptWait(40);
    fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    fdActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptWait(30);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    fdActorWalkTo(ACTOR_BUNZA, 152, 528);
    fdScriptWait(20);
    fdActorWalkTo(ACTOR_HAMMET, 160, 528);
    fdActorWaitForMoveById(ACTOR_BUNZA);
    fdActorWalkTo(ACTOR_BUNZA, 168, 640);
    fdActorWaitForMoveById(ACTOR_HAMMET);
    fdActorWalkTo(ACTOR_HAMMET, 168, 640);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    fdScriptWait(200);
    fdActorSetPos(ACTOR_HAMMET, 0, 0);
    fdActorSetPos(ACTOR_BUNZA, 0, 0);

    scWalkIntoLeader(ACTOR_GARET);
    scWalkIntoLeader(ACTOR_IVAN);
    scWalkIntoLeader(ACTOR_MIA);
    fdScriptWait(30);
    fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, -16, 0);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdCameraMoveToActorById(ACTOR_PARTY_LEADER, 1);
    fdActorWalkTo(ACTOR_PARTY_LEADER, 168, 640);
    fdScriptWait(60);
    fdScriptFinishScreenTransition();
    syImportScriptSetExitRequest(WEST_LUNPA_CAVE_BY_WAGON);
}

// Draws the gate and the ice pillars as the party left them.
Sint32 scInit(void)
{
    FieldStateWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    if (MainRoutineStateWork.next == SCENE_WEST_LUNPA_CAVE)
    {
        fdActorSetSpriteFlags(fdActorGet(ACTOR_UNUSED_PUDDLE), 0);
        fdActorSetSpriteFlags(fdActorGet(ACTOR_SOUTH_PUDDLE), 0);
        fdActorSetSpriteFlags(fdActorGet(ACTOR_GATE_PUDDLE), 0);
        fdActorSetSpriteFlags(fdActorGet(ACTOR_NORTH_PUDDLE), 0);
        fdActorGet(ACTOR_NORTH_PUDDLE)->scaleY = toFIXED(19) / 20;
        if (gmFlagGet(FLAG_WEST_LUNPA_CAVE_GATE_LOWERED) != 0)
        {
            scWestLunpaCaveGateLower();
        }
        if (gmFlagGet(FLAG_WEST_LUNPA_CAVE_GATE_RAISED) != 0)
        {
            scWestLunpaCaveGateRaise();
        }
        if (gmFlagGet(FLAG_WEST_LUNPA_CAVE_GATE_PUDDLE_FROZEN) != 0)
        {
            scWestLunpaCaveGateDrawPropped();
        }
        if (gmFlagGet(FLAG_WEST_LUNPA_CAVE_NORTH_PUDDLE_FROZEN) != 0)
        {
            fdActorSetAnimation(ACTOR_NORTH_PUDDLE, PUDDLE_ANIMATION_PILLAR);
        }
        if (gmFlagGet(FLAG_WEST_LUNPA_CAVE_SOUTH_PUDDLE_FROZEN) != 0)
        {
            fdActorSetAnimation(ACTOR_SOUTH_PUDDLE, PUDDLE_ANIMATION_PILLAR);
        }
    }
    return 0;
}

// clang-format off
const SCENE_ENTRANCE WestLunpaCaveEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { WEST_LUNPA_CAVE_FROM_WORLD_MAP, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, 88, 56, 648, 648, 0 },
    { WEST_LUNPA_CAVE_FROM_LUNPA, CONDITION_ALWAYS, 552, 0, 328, FACING_NORTH, 0, 88, 56, 648, 648, 0 },
    { WEST_LUNPA_CAVE_FROM_LUNPA_UNUSED, CONDITION_ALWAYS, 552, 0, 328, FACING_NORTH, 0, 88, 56, 648, 488, 0 },
    { SCENE_TABLE_END },
};

const Uint32 WestLunpaCaveExits[] = {
    SCENE_EXITS(WestLunpaCave),
    SCENE_EXIT(WEST_LUNPA_CAVE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_WEST_LUNPA_CAVE),
    SCENE_EXIT(WEST_LUNPA_CAVE_TO_LUNPA, Lunpa, LUNPA_FROM_WEST_LUNPA_CAVE),
    SCENE_EXIT(WEST_LUNPA_CAVE_BY_WAGON, WorldMap, WORLD_MAP_FROM_WEST_LUNPA_CAVE),
    SCENE_EXITS_END,
};

const ACTOR_PLACE WestLunpaCaveNoActorPlacements[] = {
    { SCENE_TABLE_END },
};

const ACTOR_PLACE WestLunpaCaveActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      136, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      344, 0, 248, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      424, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BUNZA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

// Unused: an empty cue table nothing returns.
const CUE WestLunpaCaveNoCues[] = {
    { CUE_TABLE_END },
};

const CUE WestLunpaCaveCues[] = {
    { CUE_EXIT, METATILE_BEHAVIOR_WEST_LUNPA_CAVE_WORLD_MAP_PASSAGE, CONDITION_ALWAYS, WEST_LUNPA_CAVE_TO_WORLD_MAP },
    { CUE_EXIT, METATILE_BEHAVIOR_WEST_LUNPA_CAVE_LUNPA_PASSAGE, CONDITION_ALWAYS, WEST_LUNPA_CAVE_TO_LUNPA },
    { CUE_STEP, METATILE_BEHAVIOR_WEST_LUNPA_CAVE_BUNZA_HIDING_PLACE, FLAG_LUNPA_CAVE_REUNION_SEEN,
      SCRIPT_REF(scWestLunpaCaveHammetFindsBunza) },
    { EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_WEST_LUNPA_CAVE_LOWERING_SWITCH, CONDITION_ALWAYS,
      SCRIPT_REF(scWestLunpaCaveFlipLoweringSwitch) },
    { EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_WEST_LUNPA_CAVE_RAISING_SWITCH, CONDITION_ALWAYS,
      SCRIPT_REF(scWestLunpaCaveFlipRaisingSwitch) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_UNUSED_PUDDLE, CONDITION_ALWAYS,
      SCRIPT_REF(scWestLunpaCaveUnusedFreezePuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SOUTH_PUDDLE, CONDITION_ALWAYS,
      SCRIPT_REF(scWestLunpaCaveFreezeSouthPuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_GATE_PUDDLE, CONDITION_ALWAYS,
      SCRIPT_REF(scWestLunpaCaveFreezeGatePuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_NORTH_PUDDLE, CONDITION_ALWAYS,
      SCRIPT_REF(scWestLunpaCaveFreezeNorthPuddle) },
    { CUE_TABLE_END },
};
// clang-format on

// Kolima Forest's pool above the floodgate (SceneIdKolimaForestFloodgate), the fourth of the
// forest's five scenes. The party comes in from the cliffs to the west (SceneIdKolimaForestCliff)
// and goes on by the path north to Tret (SceneIdKolimaForestTree).
//
// A river pool fills most of the scene, with five logs floating on it. At the bottom of the scene,
// by the floodgate and a sign that reads "Do not touch Floodgate Switch!", the Floodgate Switch
// drains the pool and fills it again. Drained, the pool is a grassy riverbed with the logs lying on
// it, and walking into a log rolls it ahead until it hits something. Filled, the logs float where
// they were left, and Isaac hops across them from log to log; from the north bank he can jump out
// onto one. Rolled into the right places, they make a way across to the chest with a Fur Coat on
// the north-east bank. A lone upright log on the grass west of the pool slides two metatiles west,
// or back east, when walked into. The forest keeps its cursed colours until Tret lifts the curse
// (FLAG_KOLIMA_CURSE_LIFTED).
//
// The water level, the bank log's place and where each log lies are kept in SceneLocalState
// (KolimaForestFloodgateWaterHigh, KolimaForestFloodgateBankLogWest and KolimaForestFloodgateLogs),
// so they survive a battle.
//
// The metatiles under a log are METATILE_BEHAVIOR_JUMP_FORWARD while it floats, which is what makes
// the leader hop across it, and METATILE_BEHAVIOR_WALL while it lies on the riverbed
// (scKolimaForestFloodgateMarkLogMetatiles), which scKolimaForestFloodgateCheckLogFits also treats
// as taken.
//
// Flipping the switch while the pool is full lifts the floodgate: water churns out through it
// (SPLASH_ANIMATION_CHURN, SE_WATER_DRAIN) and the pool turns to riverbed a line at a time, at
// random. Flipping it back drops the gate in a burst of spray (SPLASH_ANIMATION_BURST) and the
// water comes back the same way.

#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "debug/debug.h"
#include "field/script.h"
#include "field/map_grid_scroll.h"
#include "field/speaker.h"
#include "graphics/graphics.h"
#include "scene/kolima_palette.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

// Unused: eight bytes before the logs' records that nothing reads.
Uint8 KolimaForestFloodgateUnusedBeforeRecords[8] BSS_DATA;
// The bend's logs in SceneLocalState from byte 4, so they survive a battle.
struct FLOODGATE_LOG *KolimaForestFloodgateLogs BSS_DATA;
// The bank log's place in SceneLocalState's first halfword: 1 west, 0 east.
Uint16 *KolimaForestFloodgateBankLogWest        BSS_DATA;
// The bend's water level in SceneLocalState's second halfword: 1 high (the logs float), 0 low; the
// floodgate switch flips it.
Sint16 *KolimaForestFloodgateWaterHigh          BSS_DATA;
// Unused: four bytes nothing reads or writes.
Uint8                                           KolimaForestFloodgateUnusedAfterMap[4] BSS_DATA;
// The healthy colours, kept by scKolimaPaletteSaveHealthy: the palette fade's 448 background and
// sprite colours, in the array's first half.
Uint8                                           KolimaPaletteHealthy[1792] BSS_DATA;
// The cursed colours, kept by scKolimaPaletteSaveCursed.
Uint8                                           KolimaPaletteCursed[896] BSS_DATA;
// Unused: 896 bytes nothing reads or writes.
Uint8                                       KolimaForestFloodgateUnusedAfterCursed[896] BSS_DATA;
// BG1 to BG3's scroll, captured each tick and moved down to where the river's new level is drawn
// (NEW_LEVEL_OFFSET).
Uint32                                      KolimaForestFloodgateNewLevelScroll[3] BSS_DATA;
// The percent chance that a line of the river shows its new level: 0 when the rumble starts
// (SE_WATER_LEVEL), then a point higher each tick up to 101
// (scKolimaForestFloodgateFlipFloodgateSwitch).
Uint32 KolimaForestFloodgateNewLevelPercent BSS_DATA;

struct ACTOR;

// A log's saved record: its sprite, first map metatile and orientation, then its actor once
// created.
typedef struct FLOODGATE_LOG
{
    Sint16 sprite;   // sprite number; -1 ends the list
    Sint16 x;        // map metatiles
    Sint16 z;        // map metatiles
    Sint16 vertical; // nonzero runs north to south; zero west to east
    ACTOR *actor;    // created for this row; NULL in the defaults
} FLOODGATE_LOG;

STATIC_ASSERT(sizeof(FLOODGATE_LOG) == 12);
STATIC_ASSERT(offsetof(FLOODGATE_LOG, x) == 2);
STATIC_ASSERT(offsetof(FLOODGATE_LOG, vertical) == 6);
STATIC_ASSERT(offsetof(FLOODGATE_LOG, actor) == 8);

// UB: the scene calls this before its definition with no prototype, so as a function returning int.
#if FIX_FUNCTION_SIGNATURES
void scKolimaForestFloodgateMarkLogMetatiles(const FLOODGATE_LOG *records, Sint32 value);
#endif
extern FLOODGATE_LOG  KolimaForestFloodgateDefaultLogs[];
extern Uint8          KolimaForestFloodgatePushAnimations[];
extern Sint8          KolimaForestFloodgatePushStepX[];
extern Sint8          KolimaForestFloodgatePushStepZ[];
extern SCENE_ENTRANCE KolimaForestFloodgateEntrances[];
extern Uint32         KolimaForestFloodgateExits[];
extern ACTOR_PLACE    KolimaForestFloodgateActorPlacements[];
extern CUE            KolimaForestFloodgateCues[];

// The actors, numbering KolimaForestFloodgateActorPlacements' rows from ACTOR_FIRST_PLACED.
enum
{
    // The upright log on the grass west of the pool, which slides between metatile columns 4 and 6.
    ACTOR_BANK_LOG = ACTOR_FIRST_PLACED,
    // The water at the floodgate, which churns out as the gate lifts and bursts as it drops shut.
    ACTOR_FLOODGATE_SPRAY,
    ACTOR_SIGN
};

// The most metatiles one shove rolls a log.
#define LOG_ROLL_MAX_METATILES 11

// The background metatile row the river is drawn at, and the row its new level is drawn at first
// while the river's lines change to it.
#define RIVER_PANEL_ROW        19
#define NEW_LEVEL_PANEL_ROW    30
// How far below the river its new level is drawn, in pixels.
#define NEW_LEVEL_OFFSET       ((NEW_LEVEL_PANEL_ROW - RIVER_PANEL_ROW) * MAPGRID_METATILE_PIXELS)
// The last screen line of the river, the lines that change to its new level.
#define RIVER_LAST_LINE        53

// The metatiles this screen gives a meaning of its own.
typedef enum KOLIMA_FOREST_FLOODGATE_METATILE_BEHAVIOR
{
    // The Floodgate Switch's metatile, which a search flips.
    METATILE_BEHAVIOR_KOLIMA_FOREST_FLOODGATE_SWITCH = 9,
    // The north bank's edge at metatile (10, 7), where the leader jumps out onto a floating log;
    // the full pool's metatiles alone have it.
    METATILE_BEHAVIOR_KOLIMA_FOREST_NORTH_BANK_JUMP  = 10,
    // The drained pool's riverbed, where walking into a log rolls it.
    METATILE_BEHAVIOR_KOLIMA_FOREST_RIVERBED         = 11,
    // West and east of the bank log, metatiles (3, 9) to (3, 12) and (7, 9) to (7, 12), where
    // walking into it shoves it.
    METATILE_BEHAVIOR_KOLIMA_FOREST_WEST_OF_BANK_LOG = 12,
    METATILE_BEHAVIOR_KOLIMA_FOREST_EAST_OF_BANK_LOG = 13
} KOLIMA_FOREST_FLOODGATE_METATILE_BEHAVIOR;

static void           scKolimaForestFloodgateMarkBankLogMetatiles(void);
static void           scKolimaForestFloodgateCreateLogs(FLOODGATE_LOG *log);
static void           scKolimaForestFloodgateRollLog(FLOODGATE_LOG *logs);

// The party comes in from the cliffs to the west or down the path from Tret to the north.
const SCENE_ENTRANCE *scGetEntrances(void)
{
    return KolimaForestFloodgateEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return 0;
}

// The ways back west to the cliffs (SceneIdKolimaForestCliff) and north to Tret
// (SceneIdKolimaForestTree).
const Uint32 *scGetExits(void)
{
    return KolimaForestFloodgateExits;
}

// The bank log, the floodgate's spray and the sign.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    return KolimaForestFloodgateActorPlacements;
}

// The logs, the switch, the sign, the jump and the chest.
const CUE *scGetCues(void)
{
    return KolimaForestFloodgateCues;
}

// With the pool above Kolima Forest's floodgate drained, walking into a log on the riverbed rolls
// it ahead until it meets the bank or another log, with the view following it, and it stays where
// it stops. Walking anywhere on the riverbed runs this, but only a log right ahead of Isaac moves;
// the riverbed is then given back its own metatiles and the logs' metatiles made solid where they
// now lie.
static void scKolimaForestFloodgatePushLog(void)
{
    scKolimaForestFloodgateRollLog(KolimaForestFloodgateLogs);
    fdMapGridCopyMetatileAttributes(0, 64, 32, 32, 0, 0);
    scKolimaForestFloodgateMarkLogMetatiles(KolimaForestFloodgateLogs, METATILE_BEHAVIOR_WALL);
    scKolimaForestFloodgateMarkBankLogMetatiles();
}

// With the pool above Kolima Forest's floodgate full, walking south off the north bank at metatile
// (10, 7) jumps Isaac two metatiles out onto a floating log, when one lies there; over open water
// he does not jump.
static void scKolimaForestFloodgateJumpSouth(void)
{
    fdTryJumpForward();
}

#include "shared/map_grid_panel.c"

// Makes the bank log's four metatiles solid: column 4 at its west place, column 6 at its east, rows
// 9 to 12.
static void scKolimaForestFloodgateMarkBankLogMetatiles(void)
{
    if ((Sint16)*KolimaForestFloodgateBankLogWest == 1)
    {
        fdMapGridCopyMetatileAttributes(0, 0, 1, 4, 4, 9);
    }
    else
    {
        fdMapGridCopyMetatileAttributes(0, 0, 1, 4, 6, 9);
    }
}

// Draws the pool full or drained and each log floating on it or lying on the riverbed, and gives
// the pool the metatiles of that level.
static void scKolimaForestFloodgateDrawWaterLevel(void)
{
    ACTOR         *log;
    FLOODGATE_LOG *record = KolimaForestFloodgateLogs;

    // The floodgate, shut over the full pool or lifted over the drained one.
    if (*KolimaForestFloodgateWaterHigh != 0)
    {
        fdMapGridCopyMetatiles(65, 53, 2, 1, 79, 29);
        fdMapGridCopyMetatiles(65, 40, 2, 4, 15, 28);
    }
    else
    {
        fdMapGridCopyMetatiles(65, 50, 2, 5, 79, 25);
    }

    // The pool's water or its riverbed, on both layers, and its metatiles.
    if (*KolimaForestFloodgateWaterHigh != 0)
    {
        fdMapGridCopyMetatiles(0, 32, 32, 32, 32, 0);
        fdMapGridCopyMetatiles(32, 32, 32, 32, 64, 0);
        fdMapGridCopyMetatileAttributes(0, 32, 32, 32, 0, 0);
    }
    else
    {
        fdMapGridCopyMetatiles(0, 64, 32, 32, 32, 0);
        fdMapGridCopyMetatiles(32, 64, 32, 32, 64, 0);
        fdMapGridCopyMetatileAttributes(0, 64, 32, 32, 0, 0);
    }

    // Each log floats or rests, and on high water its four metatiles are drawn.
    for (; record->sprite != -1; record++)
    {
        log = record->actor;

        if (*KolimaForestFloodgateWaterHigh == 1)
        {
            fdActorSelectAnimation(log, LOG_ANIMATION_FLOAT);
            log->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_UNDERFOOT;
            log->motionFlags   = 0;
            log->y.fixed       = toFIXED(26);

            if (record->vertical != 0)
            {
                fdMapGridCopyMetatiles(68, 40, 1, 4, record->x + 32, record->z);
            }
            else
            {
                fdMapGridCopyMetatiles(70, 40, 4, 1, record->x + 32, record->z);
            }
        }
        else
        {
            fdActorSelectAnimation(log, LOG_ANIMATION_AT_REST);
            log->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
            log->motionFlags   = ACTOR_FALLS;
            log->y.fixed       = 0;
        }
    }

    // The full pool's own metatile (10, 50), which it copies in as metatile (10, 18), takes
    // metatile (70, 42): a wall metatile (render probe scene KolimaForestFloodgate --metatiles
    // 8,48,12,52 reads 255 there), so the water under the west end of the last log of
    // KolimaForestFloodgateDefaultLogs stays a wall once the log has been shoved away. Its picture
    // shows from the next time the pool fills. XXX: what that picture is; a map photo of the full
    // pool with that log shoved off (10, 18) would settle it.
    fdMapGridCopyMetatiles(70, 42, 1, 1, 10, 50);

    if (*KolimaForestFloodgateWaterHigh == 1)
    {
        fdMapGridCopyMetatileAttributes(0, 32, 32, 32, 0, 0);
        scKolimaForestFloodgateMarkLogMetatiles(KolimaForestFloodgateLogs,
                                                METATILE_BEHAVIOR_JUMP_FORWARD);
    }
    else
    {
        fdMapGridCopyMetatileAttributes(0, 64, 32, 32, 0, 0);
        scKolimaForestFloodgateMarkLogMetatiles(KolimaForestFloodgateLogs, METATILE_BEHAVIOR_WALL);
    }

    scKolimaForestFloodgateMarkBankLogMetatiles();
}

// HBlank callback: gives each line of the river, with a chance of
// KolimaForestFloodgateNewLevelPercent in 100, the scroll that shows its new level. The HBlank
// after line n sets line n + 1's scroll.
static void scKolimaForestFloodgateChangeRiverLines(void)
{
    Uint16           line        = REG_VCOUNT;
    Uint32          *src         = (Uint32 *)&BgScrollWork[1];
    volatile Uint32 *destination = (volatile Uint32 *)&REG_BG1HOFS;

    if (line == DISPLAY_TOTAL_LINES - 1 || line < RIVER_LAST_LINE)
    {
        if (syRandomBelow(100) < KolimaForestFloodgateNewLevelPercent)
        {
            src = KolimaForestFloodgateNewLevelScroll;
        }
    }

    *destination   = *src++;
    destination    = (volatile Uint32 *)&REG_BG2HOFS;
    *destination++ = *src++;
    *destination   = *src;
}

// Captures BG1 to BG3's scroll and moves it down to the river's new level.
static void scKolimaForestFloodgateCaptureNewLevelScroll(void)
{
    const Uint32 *src  = (const Uint32 *)&BgScrollWork[1];
    Uint32       *dest = KolimaForestFloodgateNewLevelScroll;
    BG_SCROLL    *newLevel;

    // BG1 to BG3's offsets, each pair one word.
    *dest++        = *src++;
    *dest++        = *src++;
    *dest          = *src;
    newLevel       = (BG_SCROLL *)KolimaForestFloodgateNewLevelScroll;
    newLevel[0].y += NEW_LEVEL_OFFSET;
    newLevel[1].y += NEW_LEVEL_OFFSET;
    newLevel[2].y += NEW_LEVEL_OFFSET;
}

// Searching the Floodgate Switch, on the bank by Kolima Forest's floodgate west of the sign
// (metatile (19, 28)), flips it: "Isaac flipped the switch." The view moves to the gate. If the
// pool is full, the gate lifts with water churning out below it and the pool drains away a line at
// a time; if it is drained, the gate drops shut in a burst of spray and the pool fills again the
// same way.
static void scKolimaForestFloodgateFlipFloodgateSwitch(void)
{
    Sint32 nextPercent;

    fdScriptStart();
    fdCameraSetSpeed(1);
    fdCameraMoveTo(264, CAMERA_KEEP, 448, 1);
    fdCameraWaitForArrival();
    mnFieldMessageShow(MsgFieldFlippedSwitch, 1);
    sySoundPlay(SE_CRUNCH);
    // The switch's lever turns, then the gate drops shut, or lifts, a picture at a time.
    if (*KolimaForestFloodgateWaterHigh == 0)
    {
        fdActorSetPos(ACTOR_FLOODGATE_SPRAY, 256, 462);
        fdMapGridCopyMetatiles(77, 34, 1, 2, 83, 25);
        sySynch(3);
        fdMapGridCopyMetatiles(78, 34, 1, 2, 83, 25);
        sySynch(3);
        fdMapGridCopyMetatiles(79, 34, 1, 2, 83, 25);
        sySynch(30);
        fdMapGridCopyMetatiles(67, 34, 2, 5, 79, 25);
        sySynch(6);
        fdMapGridCopyMetatiles(69, 34, 2, 5, 79, 25);
        fdActorSetAnimation(ACTOR_FLOODGATE_SPRAY, SPLASH_ANIMATION_BURST);
        sySoundPlay(SE_LOG_FALL);
        sySynch(6);
        fdMapGridCopyMetatiles(71, 34, 2, 5, 79, 25);
        sySynch(6);
        fdMapGridCopyMetatiles(73, 34, 2, 5, 79, 25);
        fdMapGridCopyMetatiles(75, 38, 2, 1, 79, 29);
        sySynch(4);
        fdMapGridCopyMetatiles(77, 38, 2, 1, 79, 29);
        sySynch(6);
        fdMapGridCopyMetatiles(79, 38, 2, 1, 79, 29);
        sySynch(8);
        fdMapGridCopyMetatiles(65, 53, 2, 1, 79, 29);
        fdMapGridCopyMetatiles(65, 40, 2, 4, 15, 28);
    }
    else
    {
        fdActorSetPos(ACTOR_FLOODGATE_SPRAY, 256, 480);
        fdMapGridCopyMetatiles(78, 34, 1, 2, 83, 25);
        sySynch(3);
        fdMapGridCopyMetatiles(77, 34, 1, 2, 83, 25);
        sySynch(3);
        fdMapGridCopyMetatiles(76, 34, 1, 2, 83, 25);
        sySynch(30);
        fdMapGridCopyMetatiles(65, 45, 2, 4, 15, 28);
        fdMapGridCopyMetatiles(71, 50, 2, 5, 79, 25);
        fdActorSetAnimation(ACTOR_FLOODGATE_SPRAY, SPLASH_ANIMATION_CHURN);
        sySoundPlay(SE_WATER_DRAIN);
        sySynch(6);
        fdMapGridCopyMetatiles(69, 50, 2, 5, 79, 25);
        sySynch(6);
        fdMapGridCopyMetatiles(67, 50, 2, 5, 79, 25);
        sySynch(6);
        fdMapGridCopyMetatiles(65, 50, 2, 5, 79, 25);
        sySynch(30);
    }
    if (*KolimaForestFloodgateWaterHigh == 0)
    {
        scMapGridDrawMetatilePanel(
            9, 19, 16, 5, *KolimaForestFloodgateWaterHigh, 9, NEW_LEVEL_PANEL_ROW);
        scMapGridDrawMetatilePanel(9, 51, 16, 5, 1, 9, NEW_LEVEL_PANEL_ROW);
        scMapGridDrawMetatilePanel(41, 51, 16, 5, 2, 9, NEW_LEVEL_PANEL_ROW);
    }
    else
    {
        scMapGridDrawMetatilePanel(9, 19, 16, 5, 0, 9, NEW_LEVEL_PANEL_ROW);
        scMapGridDrawMetatilePanel(9, 83, 16, 5, 1, 9, NEW_LEVEL_PANEL_ROW);
        scMapGridDrawMetatilePanel(41, 83, 16, 5, 2, 9, NEW_LEVEL_PANEL_ROW);
    }

    // Each tick captures the scroll, and the HBlank handler shows the river's lines at their new
    // level, more often as the chance climbs to 100%.
    KolimaForestFloodgateNewLevelPercent = 0;
    sySetTickEvent(scKolimaForestFloodgateCaptureNewLevelScroll, EVENT_PASS_TICK);
    sySynch(1);
    syIntFunction(IRQ_HBLANK, 0, scKolimaForestFloodgateChangeRiverLines);
    sySoundPlay(SE_WATER_LEVEL);
    KolimaForestFloodgateNewLevelPercent = 0;
    do
    {
        sySynch(1);
        nextPercent                           = KolimaForestFloodgateNewLevelPercent + 1;
        KolimaForestFloodgateNewLevelPercent += 1;
    } while (nextPercent <= 100);
    sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
    if (*KolimaForestFloodgateWaterHigh == 0)
    {
        scMapGridDrawMetatilePanel(
            9, 19, 16, 5, *KolimaForestFloodgateWaterHigh, 9, RIVER_PANEL_ROW);
        scMapGridDrawMetatilePanel(9, 51, 16, 5, 1, 9, RIVER_PANEL_ROW);
        scMapGridDrawMetatilePanel(41, 51, 16, 5, 2, 9, RIVER_PANEL_ROW);
    }
    else
    {
        scMapGridDrawMetatilePanel(9, 19, 16, 5, 0, 9, RIVER_PANEL_ROW);
        scMapGridDrawMetatilePanel(9, 83, 16, 5, 1, 9, RIVER_PANEL_ROW);
        scMapGridDrawMetatilePanel(41, 83, 16, 5, 2, 9, RIVER_PANEL_ROW);
    }
    sySynch(1);
    syIntFunction(IRQ_HBLANK, 0, 0);
    sySynch(1);
    syCloseEvent(scKolimaForestFloodgateCaptureNewLevelScroll);
    *KolimaForestFloodgateWaterHigh ^= 1;
    scKolimaForestFloodgateDrawWaterLevel();
    fdMapGridCenterOnFollowedPoint();
    fdScriptEnd();
}

// Walking west into the upright log on the grass west of Kolima Forest's pool, from its east side
// (metatiles (7, 9) to (7, 12)), shoves it two metatiles west, to column 4, with Isaac following it
// a step.
static void scKolimaForestFloodgatePushBankLogWest(void)
{
    fdScriptStart();
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
    fdScriptWait(6);
    sySoundPlay(SE_HEAVY_MOVE);
    fdActorSetSpeedAndAcceleration(ACTOR_BANK_LOG, 0.5, 0.2);
    fdActorSetAnimation(ACTOR_BANK_LOG, LOG_ANIMATION_ROLL_EAST);
    fdActorSetDestination(ACTOR_BANK_LOG, 72, 176);
    fdScriptWait(6);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 0.3, 0.2);
    fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, -8, 0);
    fdScriptWait(24);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    fdActorWaitForMoveById(ACTOR_BANK_LOG);
    fdActorSetAnimation(ACTOR_BANK_LOG, LOG_ANIMATION_AT_REST);
    sySoundPlay(SE_STOP_ACTION_SE);
    sySoundPlay(SE_HEAVY_STOP);
    fdMapGridCopyMetatileAttributes(5, 9, 1, 4, 6, 9);
    fdMapGridCopyMetatileAttributes(0, 0, 1, 4, 4, 9);
    *KolimaForestFloodgateBankLogWest = 1;
    fdScriptEnd();
}

// Walking east into the upright log on the grass west of Kolima Forest's pool, from its west side
// (metatiles (3, 9) to (3, 12)), shoves it two metatiles east, back to column 6, with Isaac
// following it a step.
static void scKolimaForestFloodgatePushBankLogEast(void)
{
    fdScriptStart();
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
    fdScriptWait(6);
    sySoundPlay(SE_HEAVY_MOVE);
    fdActorSetSpeedAndAcceleration(ACTOR_BANK_LOG, 0.5, 0.2);
    fdActorSetAnimation(ACTOR_BANK_LOG, LOG_ANIMATION_ROLL_EAST);
    fdActorSetDestination(ACTOR_BANK_LOG, 104, 176);
    fdScriptWait(6);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 0.3, 0.2);
    fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 8, 0);
    fdScriptWait(24);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    fdActorWaitForMoveById(ACTOR_BANK_LOG);
    fdActorSetAnimation(ACTOR_BANK_LOG, LOG_ANIMATION_AT_REST);
    sySoundPlay(SE_STOP_ACTION_SE);
    sySoundPlay(SE_HEAVY_STOP);
    fdMapGridCopyMetatileAttributes(5, 9, 1, 4, 4, 9);
    fdMapGridCopyMetatileAttributes(0, 0, 1, 4, 6, 9);
    *KolimaForestFloodgateBankLogWest = 0;
    fdScriptEnd();
}

// How far the curse tints the floodgate until it is lifted: green and blue lose 1 /
// FLOODGATE_CURSE_DIVISOR of themselves (scKolimaPaletteCurse).
#define FLOODGATE_CURSE_DIVISOR 4

// Sets up the pool on entry: full, with the logs where they start, on a fresh visit, and as the
// party left it after a battle.
Sint32 scInit(void)
{
    ACTOR *actor;

    KolimaForestFloodgateBankLogWest = (Uint16 *)SceneLocalState;
    KolimaForestFloodgateWaterHigh   = (Sint16 *)SceneLocalState + 1;
    // The bank log's place and the water level take the first two halfwords.
    KolimaForestFloodgateLogs        = (FLOODGATE_LOG *)(SceneLocalState + 2 * sizeof(Uint16));
    fdMapGridCopyMetatiles(32, 0, 64, 32, 0, 64);
    fdMapGridCopyMetatileAttributes(0, 0, 32, 32, 0, 64);
    fdMapGridCopyMetatileAttributes(32, 0, 32, 32, 0, 32);
    if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
    {
        // The five logs and the end of the list, six records of three words.
        syDMAXCopy(KolimaForestFloodgateDefaultLogs,
                   KolimaForestFloodgateLogs,
                   DMA_COPY_WORDS(18),
                   REG_DMA3);
        *KolimaForestFloodgateBankLogWest = 0;
        *KolimaForestFloodgateWaterHigh   = 1;
    }
    scKolimaForestFloodgateCreateLogs(KolimaForestFloodgateLogs);
    scKolimaForestFloodgateMarkLogMetatiles(KolimaForestFloodgateDefaultLogs,
                                            METATILE_BEHAVIOR_WALL);
    scKolimaForestFloodgateDrawWaterLevel();
    fdActorSetAnimation(ACTOR_FLOODGATE_SPRAY, 0);
    fdActorGet(ACTOR_FLOODGATE_SPRAY)->motionFlags = 0;
    actor                                          = fdActorGet(ACTOR_SIGN);
    actor->radius                                  = 8;
    actor->scaleX                                  = toFIXED(3) / 4;
    actor->scaleY                                  = toFIXED(3) / 4;
    FieldStateWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    if (gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0)
        scKolimaPaletteCurse(FLOODGATE_CURSE_DIVISOR);
    return 0;
}

// Makes each log at the middle of its four metatiles, standing on the ground.
static void scKolimaForestFloodgateCreateLogs(FLOODGATE_LOG *entry)
{
    ACTOR *actor;
    FIXED  height;
    Sint32 x;
    Sint32 z;

    for (; entry->sprite != -1; entry++)
    {
        if (entry->vertical == 0)
        {
            x = FIELD_METATILE_ORIGIN(entry->x) + toFIXED(32);
            z = FIELD_METATILE_CENTER(entry->z);
        }
        else
        {
            x = FIELD_METATILE_CENTER(entry->x);
            z = FIELD_METATILE_ORIGIN(entry->z) + toFIXED(32);
        }
        actor = syFieldActorCreate(entry->sprite, x, 0, z);
        if (actor == NULL)
            return;
        entry->actor = actor;
        fdActorSelectAnimation(actor, LOG_ANIMATION_AT_REST);
        fdActorSetSpriteFlags(actor, 0);
        actor->collisionFlags = 0;
        actor->radius         = 32;
        height                = toFIXED(
            fdMapGridGetTerrainHeight(0, actor->x.part[0].integer, actor->z.part[0].integer));
        actor->y.fixed      += height;
        actor->groundHeight  = height;
    }
}

// Writes a metatile behavior into the four metatiles of each log.
void scKolimaForestFloodgateMarkLogMetatiles(const FLOODGATE_LOG *records, Sint32 value)
{
    const FLOODGATE_LOG *record = records;
    Sint32               column;
    Sint32               row;
    Sint32               i;

    if (record->sprite == -1)
        return;
    for (; record->sprite != -1; record++)
    {
        column = record->x;
        row    = record->z;
        for (i = 0; i < 4; i++)
        {
            ((MAP_GRID_METATILE *)MapGridMetatileBuffer)[column + (row << MAPGRID_ROW_SHIFT)]
                .behavior = value;
            if (record->vertical == 0)
                column++;
            else
                row++;
        }
    }
}

// The log lying across the metatile, or none.
static FLOODGATE_LOG *scKolimaForestFloodgateFindLogAt(FLOODGATE_LOG *log, Sint32 x, Sint32 y)
{
    for (; log->sprite != -1; log++)
    {
        Sint32 x0 = log->x;
        Sint32 x1 = x0;
        Sint32 y0 = log->z;
        Sint32 y1 = y0;

        if (log->vertical == 0)
            x1 += 3;
        else
            y1 += 3;

        if (x >= x0 && x <= x1 && y >= y0 && y <= y1)
            return log;
    }
    return 0;
}

// Whether four metatiles from the one given, across or down, are free for a log: none is a wall or
// has a shape other than flat ground.
static Sint32 scKolimaForestFloodgateCheckLogFits(Sint32 x, Sint32 z, Sint32 vertical)
{
    Sint32 i;

    for (i = 0; i <= 3; i++)
    {
        MAP_GRID_METATILE *metatile =
            &((MAP_GRID_METATILE *)MapGridMetatileBuffer)[x + z * MAPGRID_SIZE];

        if (metatile->behavior == METATILE_BEHAVIOR_WALL
            || *(metatile->shape * 4 + MapGridShapes) != 0)
        {
            return -1;
        }
        if (vertical == 0)
        {
            x++;
        }
        else
        {
            z++;
        }
    }
    return 0;
}

// Finds the log ahead of the leader and rolls it on, a metatile at a time, up to
// LOG_ROLL_MAX_METATILES metatiles or until a metatile is not free. The leader follows its shove.
static void scKolimaForestFloodgateRollLog(FLOODGATE_LOG *logs)
{
    ACTOR         *actor;
    FIELD_STATE   *state;
    FLOODGATE_LOG *log;
    Sint32         point[3];
    Sint32         facing;
    Sint32         moved;
    Sint32         i;
    FIXED          x;
    FIXED          z;
    Sint32         quarter;

    moved  = 0;
    actor  = fdActorGet(ACTOR_PARTY_LEADER);
    facing = FACING_NEAREST_CARDINAL(actor->facing);
    SET_VECTOR(point,
               (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8),
               actor->y.fixed,
               (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(8));
    syVectorAddPolarOffset(toFIXED(16), facing, point);
    log = scKolimaForestFloodgateFindLogAt(logs, point[0] / toFIXED(16), point[2] / toFIXED(16));
    if (log == NULL)
        return;
    for (i = 0; i < LOG_ROLL_MAX_METATILES; i++)
    {
        point[0] = FIELD_METATILE_ORIGIN(log->x);
        point[2] = FIELD_METATILE_ORIGIN(log->z);
        syVectorAddPolarOffset(toFIXED(16), facing, point);
        if (scKolimaForestFloodgateCheckLogFits(
                point[0] / toFIXED(16), point[2] / toFIXED(16), log->vertical)
            != 0)
            break;
        moved = 1;
        if (log->vertical == 0)
        {
            x = point[0] + toFIXED(32);
            z = point[2] + toFIXED(8);
        }
        else
        {
            x = point[0] + toFIXED(8);
            z = point[2] + toFIXED(32);
        }
        log->x = point[0] / toFIXED(16);
        log->z = point[2] / toFIXED(16);
    }
    if (moved == 0)
        return;
    SET_VECTOR(point,
               (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8),
               actor->y.fixed,
               (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(8));
    syVectorAddPolarOffset(toFIXED(8), facing, point);
    actor   = log->actor;
    quarter = facing / (DEGtoANG(90));
    fdScriptStart();
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
    fdScriptWait(6);
    actor->speedLimit   = toFIXED(1) / 2;
    actor->acceleration = toFIXED(1) / 5;
    sySoundPlay(SE_HEAVY_MOVE);
    fdActorSelectAnimation(actor, KolimaForestFloodgatePushAnimations[quarter]);
    syFieldActorSetMoveTargetFX(actor, x, 0, z);
    fdScriptWait(6);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    state = syHeapSlotAllocateEWRAM(HEAP_SLOT_FIELD, sizeof(*state));
    fdActorStartCamera(state->camera, (Sint32)actor);
    fdActorSetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 0.3, 0.2);
    fdActorSetDestinationOffset(ACTOR_PARTY_LEADER,
                                KolimaForestFloodgatePushStepX[quarter],
                                KolimaForestFloodgatePushStepZ[quarter]);
    fdScriptWait(24);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    fdActorWaitForMove(actor);
    fdActorSelectAnimation(actor, LOG_ANIMATION_AT_REST);
    sySoundPlay(SE_STOP_ACTION_SE);
    sySoundPlay(SE_HEAVY_STOP);
    fdScriptWait(15);
    fdScriptEnd();
}

#include "shared/kolima_palette.c"

// The logs on a first visit, as FLOODGATE_LOG, each in the sprite of the way it runs: one sprite
// for north to south, another for west to east.
// clang-format off
FLOODGATE_LOG KolimaForestFloodgateDefaultLogs[] = {
    {SPRITE_LOG_NORTH_SOUTH, 11, 9, 1, NULL},
    {SPRITE_LOG_WEST_EAST, 12, 12, 0, NULL},
    {SPRITE_LOG_WEST_EAST, 17, 12, 0, NULL},
    {SPRITE_LOG_NORTH_SOUTH, 13, 13, 1, NULL},
    {SPRITE_LOG_WEST_EAST, 10, 18, 0, NULL},
    {-1},
};

// For each quarter turn of the way a log is pushed: its rolling animation, and the leader's step after it in pixels.
Uint8 KolimaForestFloodgatePushAnimations[] = {
    LOG_ANIMATION_ROLL_EAST,
    LOG_ANIMATION_ROLL_EAST,
    LOG_ANIMATION_ROLL_WEST,
    LOG_ANIMATION_ROLL_WEST,
};
Sint8 KolimaForestFloodgatePushStepX[] = {8, 0, -8, 0};
Sint8 KolimaForestFloodgatePushStepZ[] = {0, 8, 0, -8};

SCENE_ENTRANCE KolimaForestFloodgateEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 407, 0, 276, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_CLIFF, CONDITION_ALWAYS, 50, 32, 369, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_TREE, CONDITION_ALWAYS, 162, 32, 84, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

Uint32 KolimaForestFloodgateExits[] = {
    SCENE_EXITS(KolimaForestFloodgate),
    SCENE_EXIT(KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_CLIFF, KolimaForestCliff, KOLIMA_FOREST_CLIFF_FROM_KOLIMA_FOREST_FLOODGATE),
    SCENE_EXIT(KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_TREE, KolimaForestTree, KOLIMA_FOREST_TREE_FROM_KOLIMA_FOREST_FLOODGATE),
    SCENE_EXITS_END,
};

ACTOR_PLACE KolimaForestFloodgateActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        104, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        256, 0, 464, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        348, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};
// clang-format on

CUE KolimaForestFloodgateCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_CLIFF},
    {CUE_EXIT, 2, CONDITION_ALWAYS, KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_TREE},
    {STEP_CUE_WALKING_INTO(FACING_SOUTH),
     METATILE_BEHAVIOR_KOLIMA_FOREST_NORTH_BANK_JUMP, CONDITION_ALWAYS,
     SCRIPT_REF(scKolimaForestFloodgateJumpSouth)},
    {CUE_STEP | STEP_CUE_FLAG_WALKING,
     METATILE_BEHAVIOR_KOLIMA_FOREST_RIVERBED, CONDITION_ALWAYS,
     SCRIPT_REF(scKolimaForestFloodgatePushLog)},
    {STEP_CUE_WALKING_INTO(FACING_WEST),
     METATILE_BEHAVIOR_KOLIMA_FOREST_EAST_OF_BANK_LOG, CONDITION_ALWAYS,
     SCRIPT_REF(scKolimaForestFloodgatePushBankLogWest)},
    {STEP_CUE_WALKING_INTO(FACING_EAST),
     METATILE_BEHAVIOR_KOLIMA_FOREST_WEST_OF_BANK_LOG, CONDITION_ALWAYS,
     SCRIPT_REF(scKolimaForestFloodgatePushBankLogEast)},
    {CUE_TALK, ACTOR_SIGN, CONDITION_ALWAYS, MSGNO(MsgKolimaDoNotTouchFloodgate)},
    {EXAMINE_CUE(EXAMINE_SILENT),
     METATILE_BEHAVIOR_KOLIMA_FOREST_FLOODGATE_SWITCH, CONDITION_ALWAYS,
     SCRIPT_REF(scKolimaForestFloodgateFlipFloodgateSwitch)},
    {EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_KOLIMA_FOREST_FUR_COAT, FIND_ITEM(ITEM_FUR_COAT)},
    {CUE_TABLE_END},
};

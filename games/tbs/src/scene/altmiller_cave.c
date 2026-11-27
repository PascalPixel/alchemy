// Altmiller Cave, north of Tolbi, where the party finds Babi while all of Tolbi searches for him
// during Colosso (babi_palace.c). Its six scenes share this overlay and the area name Altmiller
// Cave. In the order the party goes through them:
//
// - The winding passage in from the cave mouth (`AltmillerCaveEntrance`). It
//   grows darker in steps as the party walks in, until only a circle of
// light around them is bright. At its end a hole leads into a small chamber, where a torch burns
// beside the doorway down.
// - The big dark room below (`AltmillerCaveBabi`). Near its middle an old man
//   lies on the floor, seen only as a white outline: the invisible Babi
//   (scAltmillerCaveMeetInvisibleBabi). He asks the party to fetch his
// draught from the bottom of the cave and tells them the order of the five rocks' colors. The chest
// on the room's east side is a Mimic.
// - Two dark rooms with a chest each: the Vial (`AltmillerCaveVialChest`)
//   and, further down, the Cookie (`AltmillerCaveCookieChest`).
// - Between them, a lit hall with a brick floor (`AltmillerCaveSquall`):
//   three fallen columns to push, a puddle Frost turns into an ice pillar
// to stop them, Squall, a Jupiter Djinni, in the north-west corner, and a stone pillar on a ledge
// to push into the pit before the way on.
// - The lit rooms at the bottom (`AltmillerCaveDraught`): a hall of three more
//   fallen columns, the room of the five rocks (scAltmillerCaveTurnRock),
// and behind the wall the rocks open, the Mystic Draught's chest on a stone
//       table.
//
// Back in Babi's room with the draught, the party watches him drink it and appear, and two men from
// Tolbi come to take him home (scAltmillerCaveRestoreBabiAndInviteParty). Saving him
// (FLAG_BABI_SAVED) is the story flag the rest of Tolbi waits on. Everything else the cave
// remembers is kept in scene temp flags and SceneLocalState, so the puzzles start over on every
// ordinary arrival.
//
// The rooms share one entrance table and one exit table, and the other getters choose by
// MainRoutineStateWork.next. scInit sends an entry from outside the cave (entrance 0) to the room's
// first entrance, darkens the dark rooms and puts back what the party has done.
//
// The darkness is BG1 laid over the rest (ALTMILLER_CAVE_DARKNESS_BLEND), and the light around the
// party is a display window, an ellipse around the leader inside which the layers are drawn
// differently (scAltmillerCaveInitDarkness). In the dark rooms BG1 covers everything outside the
// light, so the room is black there but for the glow of the torches on its walls, and inside it BG1
// only dims the ground.
//
// The passage in darkens by steps instead (FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS). Six kinds of
// metatile along it each set a darkness level, kept in the cave's state: at the mouth the passage
// is fully lit, and five lines further in each make everything outside the light a little dimmer
// (scAltmillerCaveApplyFadeBlend). Where one level meets the next a metatile of each lies side by
// side, so the level is always that of the last metatile stepped on, whichever way the party walks.
//
// Reveal lights the passage in while it lasts and shows the rocks' colors
// (scAltmillerCaveStartReveal, scAltmillerCaveEndReveal).
//
// The rock puzzle keeps the rounds matched, its state and the light at the top in the cave's state.
// Turning the leftmost rock and then the rightmost brings out a ring of five lights above the
// rocks. The ring spins (scAltmillerCaveSpinLights, an event) and stops with one light at the top.
// Turning that light's rock spins it on to a different light. A wrong rock puts the lights away.
// The third match bursts the wall behind the rocks open (scAltmillerCaveReleaseWallBurst). Coming
// up to the rocks from the hall of fallen columns (ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE)
// starts the puzzle over.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "field/field_psynergy_state.h"
#include "graphics/display_transition.h"
#include "graphics/graphics.h"
#include "scene/walk_into_leader.h"
#include "text/msg_ids.h"

struct LIGHT_WHEEL;

extern struct LIGHT_WHEEL *AltmillerCaveLightWheel;
extern SCENE_ENTRANCE      AltmillerCaveEntrances[];
extern Uint32              AltmillerCaveExits[];
extern ACTOR_PLACE         AltmillerCaveActorPlacementsOther[];
extern ACTOR_PLACE         AltmillerCaveBabiActorPlacements[];
extern ACTOR_PLACE         AltmillerCaveSquallActorPlacements[];
extern ACTOR_PLACE         AltmillerCaveDraughtActorPlacements[];
extern Uint32              AltmillerCavePulseScales[];
extern Sint32              AltmillerCaveGuardsArrivePath[];
extern Sint32              AltmillerCaveSecondGuardLeavePath[];
extern Sint32              AltmillerCaveFirstGuardLeavePath[];
extern Sint32              AltmillerCaveBabiLeavePath[];
extern CUE                 AltmillerCaveCuesOther[];
extern CUE                 AltmillerCaveBabiCues[];
extern CUE                 AltmillerCaveVialChestCues[];
extern CUE                 AltmillerCaveSquallCues[];
extern CUE                 AltmillerCaveCookieChestCues[];
extern CUE                 AltmillerCaveDraughtCues[];

// The actors of Babi's room: Babi while invisible and once seen, and the two men from Tolbi who
// come looking for him. Garet, Ivan and Mia are placed first, in their own party slots.
enum
{
    // Babi while invisible (SPRITE_INVISIBLE_BABI), seen only as a white outline lying on the floor
    // near the middle of his room.
    ACTOR_INVISIBLE_BABI = ACTOR_FIRST_PLACED,
    // SPRITE_BABI, the same old man once he has drunk the draught.
    ACTOR_BABI,
    // Their sprite is a helmeted soldier carrying a torch (SPRITE_SOLDIER_WITH_TORCH).
    ACTOR_FIRST_SOLDIER,
    ACTOR_SECOND_SOLDIER
};

// Squall's hall and the hall at the bottom place first the three fallen columns the party pushes.
// Squall's hall then places its puddle, the stone pillar on the ledge and Squall, a Jupiter Djinni.
enum
{
    ACTOR_FIRST_FALLEN_COLUMN = ACTOR_FIRST_PLACED,
    ACTOR_SQUALL_ROOM_PUDDLE  = ACTOR_FIRST_FALLEN_COLUMN + 3,
    ACTOR_SQUALL_ROOM_PILLAR,
    ACTOR_SQUALL
};

// The rock room, after the hall's three fallen columns: the five colored lights and the five rocks,
// each in Babi's order, the rocks left to right.
enum
{
    ACTOR_FIRST_LIGHT = ACTOR_FIRST_FALLEN_COLUMN + 3,
    ACTOR_BLUE_LIGHT  = ACTOR_FIRST_LIGHT,
    ACTOR_GREEN_LIGHT,
    ACTOR_WHITE_LIGHT,
    ACTOR_YELLOW_LIGHT,
    ACTOR_RED_LIGHT,
    ACTOR_FIRST_ROCK,
    ACTOR_BLUE_ROCK = ACTOR_FIRST_ROCK,
    ACTOR_GREEN_ROCK,
    ACTOR_WHITE_ROCK,
    ACTOR_YELLOW_ROCK,
    ACTOR_RED_ROCK
};

// The lights, one of each colour.
enum
{
    LIGHT_COUNT = 5
};

// The colour mode that draws a light or a rock in each of Babi's five colors, a shuffle or tint of
// the sprite's own colours, and greyscale for a rock while Reveal is off.
enum
{
    COLOR_BLUE   = SPRITE_COLOR_GRB,
    COLOR_GREEN  = SPRITE_COLOR_GBR,
    COLOR_WHITE  = SPRITE_COLOR_GREEN_TINT,
    COLOR_YELLOW = SPRITE_COLOR_RBG,
    COLOR_RED    = SPRITE_COLOR_BGR,
    COLOR_NONE   = SPRITE_COLOR_GREY
};

// The scene temp flags the cave keeps.
enum
{
    // The room is dark. It is set on entry everywhere but Squall's hall and the lit rooms at the
    // bottom.
    FLAG_ALTMILLER_CAVE_DARK             = FLAG_SCENE_TEMP_00,
    // The passage in from the cave mouth darkens by steps as the party walks in.
    FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS = FLAG_SCENE_TEMP_01,
    // Frost has turned the puddle in Squall's hall into an ice pillar.
    FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN    = FLAG_SCENE_TEMP_11,
    // The stone pillar in Squall's hall has dropped into the pit before the way on. Only the first
    // patch on keeps it: the pit's metatile is then filled again when the party comes back, though
    // the pillar is drawn back on its ledge either way.
    FLAG_ALTMILLER_CAVE_PILLAR_IN_PLACE  = FLAG_SCENE_TEMP_12
};

// The metatile behaviors the rooms' cue tables answer, besides their exits, whose metatiles carry
// the exit's own number. Each is a metatile of the map named here.
typedef enum ALTMILLER_CAVE_CUE_METATILE
{
    // The passage in: the first bend north of the cave mouth, on the mouth's side, metatiles (27,
    // 19) to (27, 21). The passage is fully lit.
    METATILE_BEHAVIOR_ALTMILLER_CAVE_FULL_LIGHT           = 60,
    // Each further line across the passage, on its far side: the first bend (metatiles (28, 19) and
    // (28, 20)) and the way north (metatile (26, 10)).
    METATILE_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_1           = 61,
    // North up the passage (metatile (26, 9)), and where it turns west along the north wall
    // (metatiles (18, 4) and (18, 5)).
    METATILE_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_2           = 62,
    // Along the north wall (metatiles (17, 4) and (17, 5)), and where the passage turns south in
    // the north-west (metatiles (7, 8) and (7, 9)).
    METATILE_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_3           = 63,
    // The north-west turn (metatiles (6, 8) and (6, 9)), and down the west side (metatiles (7, 20)
    // to (9, 20)).
    METATILE_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_4           = 64,
    // The last stretch before the hole into the torch-lit chamber, metatiles (8, 21) to (10, 21).
    METATILE_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_5           = 65,
    // Babi's room: the floor just west of where he lies, on the way to the stairs down, metatiles
    // (11, 9) to (13, 9).
    METATILE_BEHAVIOR_ALTMILLER_CAVE_BESIDE_BABI          = 19,
    // The floor of the fallen columns: in Squall's hall metatiles (19, 11) to (26, 19), and in the
    // hall at the bottom metatiles (23, 24) to (27, 32).
    METATILE_BEHAVIOR_ALTMILLER_CAVE_DRAUGHT_COLUMN_FLOOR = 70,
    METATILE_BEHAVIOR_ALTMILLER_CAVE_SQUALL_COLUMN_FLOOR  = 71,
    // Squall's hall: the four metatiles around the puddle at (23, 17), north, east, south and west
    // of it.
    METATILE_BEHAVIOR_ALTMILLER_CAVE_NORTH_OF_PUDDLE      = 72,
    METATILE_BEHAVIOR_ALTMILLER_CAVE_EAST_OF_PUDDLE       = 73,
    METATILE_BEHAVIOR_ALTMILLER_CAVE_SOUTH_OF_PUDDLE      = 74,
    METATILE_BEHAVIOR_ALTMILLER_CAVE_WEST_OF_PUDDLE       = 75,
    // Squall's hall: the ledge just west of the stone pillar, metatile (28, 20).
    METATILE_BEHAVIOR_ALTMILLER_CAVE_WEST_OF_PILLAR       = 77,
    // The chest in each room that has one: the Mimic in Babi's room (metatile (28, 36)), the Vial
    // (metatile (25, 13)), the Cookie (metatile (12, 8)) and the Mystic Draught on its table
    // (metatile (11, 7)).
    METATILE_BEHAVIOR_ALTMILLER_CAVE_CHEST                = 100
} ALTMILLER_CAVE_CUE_METATILE;

// The darkness: BG1 blended over every layer, at the weights

// BLEND_ALPHA gives each level.
#define ALTMILLER_CAVE_DARKNESS_BLEND (BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)
// A dark room's weights, which the passage's levels do not use.
#define ALTMILLER_CAVE_DARK_ALPHA     BLEND_ALPHA(4, 12)

// The light around the party is a window, an ellipse a little taller than wide around the leader
// (WINDOW_TRANSITION_LEADER_LOW); the transition's progress sets its size, 5 pixels to each side of
// the leader for each step. The light's progress: 25 pixels to each side of the leader.
#define ALTMILLER_CAVE_LIGHT_STEPS    5
// The light's progress once Babi is found: wide enough to fill the screen.
#define ALTMILLER_CAVE_LIGHT_FULL     32
// Inside the light where the passage darkens by steps: every layer but BG1, with no blend, so the
// party's surroundings show at full brightness.
#define ALTMILLER_CAVE_STEPS_LIGHT_WININ                                                           \
    (WININ_WIN0_BG0 | WININ_WIN0_BG2 | WININ_WIN0_BG3 | WININ_WIN0_OBJ | WININ_WIN1_BG0            \
     | WININ_WIN1_BG2 | WININ_WIN1_BG3 | WININ_WIN1_OBJ)
// Outside that light: everything, blended to the step's darkness.
#define ALTMILLER_CAVE_STEPS_LIGHT_WINOUT WINOUT_WIN01_ALL
// Inside the light of an evenly dark room: everything, blended at ALTMILLER_CAVE_DARK_ALPHA.
#define ALTMILLER_CAVE_DARK_LIGHT_WININ   (WININ_WIN0_ALL | WININ_WIN1_ALL)
// Outside it: every layer with no blend, so BG1 covers the room.
#define ALTMILLER_CAVE_DARK_LIGHT_WINOUT                                                           \
    (WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ)
// Outside the light once Babi is found: everything, blended, so the whole room brightens as BG1
// fades.
#define ALTMILLER_CAVE_BRIGHTENING_WINOUT (WINOUT_WIN01_ALL | WINOUT_WINOBJ_ALL)

static void scAltmillerCaveReleaseWallBurst(void);

// The ring of five lights above the rocks as it spins to its next light. Angles are 16-bit turns,
// and the light at the top is the chosen one.
typedef struct LIGHT_WHEEL
{
    Sint16 phase;  // WHEEL_SPEEDING_UP to WHEEL_STOPPED
    Sint16 ticks;  // ticks into the phase
    Sint16 unused; // nothing reads or writes it
    Uint16 angle;
    Uint16 speed;
    // How far the ring has turned since its last click.
    Uint16 travel;
} LIGHT_WHEEL;

// The ring speeds up, coasts half a second, slows down, then stops with the chosen light at the
// top.
enum
{
    WHEEL_SPEEDING_UP,
    WHEEL_COASTING,
    WHEEL_SLOWING,
    WHEEL_STOPPING,
    WHEEL_STOPPED = 99
};

// The ring's speeds, in angle a tick. It speeds up by WHEEL_SPEED_STEP a tick to WHEEL_TOP_SPEED
// and slows by half a step a tick to WHEEL_LOW_SPEED, then stops on the chosen light once it is
// within a tick's turn of the top.
enum
{
    WHEEL_SPEED_STEP = TRIG_TURN / 4096,
    WHEEL_TOP_SPEED  = TRIG_TURN * 3 / 64,
    WHEEL_LOW_SPEED  = TRIG_TURN * 3 / 256
};

// The pieces the wall bursts into (scAltmillerCaveReleaseWallBurst).
enum
{
    WALL_BURST_PIECES = 24
};

// The cave's own state, bytes of SceneLocalState: the rock puzzle and the passage's darkness.
enum
{
    CAVE_ROUNDS_MATCHED = 0, // the rounds of lights matched, up to three
    CAVE_PUZZLE         = 1, // the rock puzzle's state
    // the light the ring stops with at the top, 0 to 4
    CAVE_CHOSEN_LIGHT   = 2,
    CAVE_DARKNESS       = 4 // the passage's darkness level, 0 to 5
};

// A byte of the cave's state; the chosen light and the darkness are signed.
#define CAVE_STATE(byte)        (((Uint8 *)SceneLocalState)[byte])
#define CAVE_SIGNED_STATE(byte) (((Sint8 *)SceneLocalState)[byte])

// The rock puzzle's two bytes, from CAVE_PUZZLE.
typedef struct ROCK_PUZZLE_STATE
{
    Uint8 state;
    Sint8 chosenLight;
} ROCK_PUZZLE_STATE;

// The rock puzzle's bytes in the cave's state.
#define ROCK_PUZZLE ((ROCK_PUZZLE_STATE *)&CAVE_STATE(CAVE_PUZZLE))

// The puzzle's states. The leftmost rock is turned, then the rightmost, which shows the lights.
// After three lights matched the wall opens.
enum
{
    PUZZLE_START,
    PUZZLE_LEFT_ROCK_TURNED,
    PUZZLE_LIGHTS_SHOWN,
    PUZZLE_SOLVED = 99
};

// The ring of lights.
LIGHT_WHEEL AltmillerCaveLightWheelState __attribute__((nocommon)) ALIGNED(4);

#include "shared/pushed_actor.c"

// Talking to Squall, the Jupiter Djinni in the north-west corner of the hall of fallen columns,
// starts a battle with it.
static void scAltmillerCaveStartDjinnBattle(void)
{
    fdRequestDjinnBattle(ACTOR_SQUALL, DJINNI_SQUALL);
}

// The six rooms share one entrance table.
const SCENE_ENTRANCE *scGetEntrances(void)
{
    return AltmillerCaveEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

const Uint32 *scGetExits(void)
{
    return AltmillerCaveExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
    Sint16 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_ALTMILLER_CAVE_BABI)
    {
        return AltmillerCaveBabiActorPlacements;
    }
    if (sceneId == SCENE_ALTMILLER_CAVE_SQUALL)
    {
        return AltmillerCaveSquallActorPlacements;
    }
    if (sceneId == SCENE_ALTMILLER_CAVE_DRAUGHT)
    {
        return AltmillerCaveDraughtActorPlacements;
    }
    return AltmillerCaveActorPlacementsOther;
}

// Darkens the passage in to its current level: everything outside the light around the party is
// drawn at its full brightness at level 0, and two sixteenths dimmer at each level after, down to
// six sixteenths at level 5.
static void scAltmillerCaveApplyFadeBlend(void)
{
    Sint8 level = CAVE_SIGNED_STATE(CAVE_DARKNESS);

    IO_WRITE16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
    if (level == 0)
    {
        IO_WRITE16(BLEND_ALPHA(0, 16), &REG_BLDALPHA);
    }
    else if (level == 1)
    {
        IO_WRITE16(BLEND_ALPHA(0, 14), &REG_BLDALPHA);
    }
    else if (level == 2)
    {
        IO_WRITE16(BLEND_ALPHA(0, 12), &REG_BLDALPHA);
    }
    else if (level == 3)
    {
        IO_WRITE16(BLEND_ALPHA(0, 10), &REG_BLDALPHA);
    }
    else if (level == 4)
    {
        IO_WRITE16(BLEND_ALPHA(0, 8), &REG_BLDALPHA);
    }
    else
    {
        IO_WRITE16(BLEND_ALPHA(0, 6), &REG_BLDALPHA);
    }
}

// Keeps the passage's darkness level, and shows it unless Reveal is lighting the passage.
static void scAltmillerCaveSetFadeLevel(Sint32 level)
{
    FIELD_STATE *state        = FieldStateWork;

    CAVE_STATE(CAVE_DARKNESS) = level;
    if (state->revealActive == 0)
    {
        scAltmillerCaveApplyFadeBlend();
    }
}

// Walking back toward the cave mouth past the first bend north of it (metatiles (27, 19) to (27,
// 21)) lifts the darkness: the passage is as bright as at the mouth again.
static void scAltmillerCaveDarkenToLevel0(void)
{
    scAltmillerCaveSetFadeLevel(0);
}

// Everything outside the light around the party grows a little darker as Isaac walks on east from
// the first bend north of the cave mouth (metatiles (28, 19) and (28, 20)), or comes back down the
// passage from the north (metatile (26, 10)).
static void scAltmillerCaveDarkenToLevel1(void)
{
    scAltmillerCaveSetFadeLevel(1);
}

// The passage grows darker again as Isaac walks on north up it (metatile (26, 9)), or comes back
// east along the north wall (metatiles (18, 4) and (18, 5)).
static void scAltmillerCaveDarkenToLevel2(void)
{
    scAltmillerCaveSetFadeLevel(2);
}

// The passage grows darker again as Isaac walks on west along the cave's north wall (metatiles (17,
// 4) and (17, 5)), or comes back from the north-west turn (metatiles (7, 8) and (7, 9)).
static void scAltmillerCaveDarkenToLevel3(void)
{
    scAltmillerCaveSetFadeLevel(3);
}

// The passage grows darker again as Isaac turns south at its north-west end (metatiles (6, 8) and
// (6, 9)), or comes back up the west side (metatiles (7, 20) to (9, 20)).
static void scAltmillerCaveDarkenToLevel4(void)
{
    scAltmillerCaveSetFadeLevel(4);
}

// The passage is at its darkest, only the light around the party bright, as Isaac walks down the
// west side into the last stretch before the small torch-lit chamber (metatiles (8, 21) to (10,
// 21)).
static void scAltmillerCaveDarkenToLevel5(void)
{
    scAltmillerCaveSetFadeLevel(5);
}

// Darkens the room on entry, with the light around the party. The passage in darkens by its levels,
// the other dark rooms evenly, black outside the light.
static void scAltmillerCaveInitDarkness(void)
{
    FIELD_STATE             *fieldState = FieldStateWork;
    SCREEN_TRANSITION_STATE *state;

    fieldState->transition      = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    fieldState->transitionFrame = 24;
    sySynch(1);
    fdScreenTransitionStartHeldWindow(WINDOW_TRANSITION_LEADER_LOW);
    state        = ScreenTransitionStateWork;
    state->value = ALTMILLER_CAVE_LIGHT_STEPS;
    if (gmFlagGet(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS) != 0)
    {
        state->insideLayers  = ALTMILLER_CAVE_STEPS_LIGHT_WININ;
        state->outsideLayers = ALTMILLER_CAVE_STEPS_LIGHT_WINOUT;
        scAltmillerCaveApplyFadeBlend();
        return;
    }
    state->insideLayers  = ALTMILLER_CAVE_DARK_LIGHT_WININ;
    state->outsideLayers = ALTMILLER_CAVE_DARK_LIGHT_WINOUT;
    IO_WRITE16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
    IO_WRITE16(ALTMILLER_CAVE_DARK_ALPHA, &REG_BLDALPHA);
}

// Casting Reveal in the passage in lights the whole passage while Reveal lasts, and in the rock
// room at the bottom of the cave shows each of the five rocks in its color. The rocks are blue,
// green, white, yellow and red from the left, the order Babi gives.
static void scAltmillerCaveStartReveal(void)
{
    if (MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_ENTRANCE)
    {
        IO_WRITE16(BLEND_ALPHA(0, 16), &REG_BLDALPHA);
    }
    if (MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_DRAUGHT)
    {
        fdActorSetSpriteColorById(ACTOR_BLUE_ROCK, COLOR_BLUE);
        fdActorSetSpriteColorById(ACTOR_GREEN_ROCK, COLOR_GREEN);
        fdActorSetSpriteColorById(ACTOR_WHITE_ROCK, COLOR_WHITE);
        fdActorSetSpriteColorById(ACTOR_YELLOW_ROCK, COLOR_YELLOW);
        fdActorSetSpriteColorById(ACTOR_RED_ROCK, COLOR_RED);
    }
}

// When Reveal wears off, a dark room goes dark again around the light, and the five rocks in the
// rock room turn back to plain grey.
static void scAltmillerCaveEndReveal(void)
{
    FIELD_EFFECT_STATE *effectState = (FIELD_EFFECT_STATE *)FieldEffectStateWork;

    if (gmFlagGet(FLAG_ALTMILLER_CAVE_DARK) != 0)
    {
        scAltmillerCaveInitDarkness();
        // Reveal then leaves the light around the party open as it ends.
        effectState->sceneVariant = 1;
    }
    if (MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_DRAUGHT)
    {
        fdActorSetSpriteColorById(ACTOR_BLUE_ROCK, COLOR_NONE);
        fdActorSetSpriteColorById(ACTOR_GREEN_ROCK, COLOR_NONE);
        fdActorSetSpriteColorById(ACTOR_WHITE_ROCK, COLOR_NONE);
        fdActorSetSpriteColorById(ACTOR_YELLOW_ROCK, COLOR_NONE);
        fdActorSetSpriteColorById(ACTOR_RED_ROCK, COLOR_NONE);
    }
}

// Casting Retreat in the dark rooms of the cave first turns Isaac to face the screen, then lets the
// spell carry the party out. Retreat plays on a copy of him made after the turn, and the leader
// himself stops updating.
static void scAltmillerCaveStartRetreat(void)
{
    FIELD_EFFECT_STATE *effectState;
    ACTOR              *original;
    ACTOR              *replacement;

    effectState = (FIELD_EFFECT_STATE *)FieldEffectStateWork;
    original    = effectState->caster;
    fdActorFaceDirection(effectState->casterId, FACING_SOUTH, 0);
    fdActorSetSpriteColorMode(original, SPRITE_COLOR_NORMAL);
    fdScriptWait(20);
    replacement = syFieldActorCreate(0, original->x.fixed, original->y.fixed, original->z.fixed);
    if (replacement != NULL)
    {
        syDMAXCopy(original, replacement, DMA_COPY_WORDS(sizeof(ACTOR) / 4), REG_DMA3);
        original->update            = 0;
        effectState->caster         = replacement;
        original->spriteStorageKind = 0;
    }
}

// Once the stone pillar on the ledge in Squall's hall has been pushed east over the edge, with Move
// or by hand, it drops into the pit below (metatile (30, 20)), and its top fills the gap so the
// party can cross to the way on. Pushed anywhere else, nothing more happens.
static void scAltmillerCaveDropPillarIntoPit(void)
{
    ACTOR *actor = fdActorGet(ACTOR_SQUALL_ROOM_PILLAR);

    if (FIELD_METATILE(actor->x.fixed) == 30 && FIELD_METATILE(actor->z.fixed) == 20)
    {
        actor->motionFlags   = ACTOR_FALLS;
        actor->groundHeight  = 0;
        actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        fdMapGridCopyMetatileAttributes(30, 20, 1, 1, 32, 20);
#if ALTMILLER_PILLAR_BRIDGE_KEPT_AFTER_BATTLE
        gmFlagSet(FLAG_ALTMILLER_CAVE_PILLAR_IN_PLACE);
#endif
    }
}

// Walking east into the stone pillar on the ledge in Squall's hall, from metatile (28, 20), pushes
// it a metatile and over the edge into the pit.
static void scAltmillerCavePushPillar(void)
{
    scPushedActorAdvancePair();
    scAltmillerCaveDropPillarIntoPit();
}

// Before Frost is cast on the puddle in Squall's hall, the game checks whether anything stands on
// it. If so, nothing freezes.
static void scAltmillerCaveCheckPuddleCovered(void)
{
    FIELD_EFFECT_STATE *effectState = (FIELD_EFFECT_STATE *)FieldEffectStateWork;
    ACTOR              *target      = fdActorGet(ACTOR_SQUALL_ROOM_PUDDLE);
    FIELD_POS           pos;

    pos.x = target->x.fixed;
    pos.y = target->y.fixed;
    pos.z = target->z.fixed;

    if (syFieldActorCheckMovementCollision(target, &pos) > 0)
    {
        effectState->leaveTarget = 1;
    }
}

// Frost on the puddle in Squall's hall (metatile (23, 17)) turns it into a pillar of ice. A fallen
// column pushed toward it stops against it, which is how the party wedges the columns out of the
// way to reach Squall. The puddle's metatile becomes a wall, copied from a spare metatile of the
// map.
static void scAltmillerCaveFreezePuddle(void)
{
    FIELD_EFFECT_STATE *effectState = (FIELD_EFFECT_STATE *)FieldEffectStateWork;
    ACTOR              *puddle      = fdActorGet(ACTOR_SQUALL_ROOM_PUDDLE);
    Sint32              blocked;

    blocked = effectState->leaveTarget;
    if (blocked == 0)
    {
        fdMapGridCopyMetatileAttributes(76, 16, 1, 1, 73, 17);
        if (puddle != NULL)
        {
            Sint32 motionFlags    = ACTOR_FALLS;
            puddle->motionFlags   = motionFlags;
            // blocked is 0 here: the pillar is drawn as anything standing is.
            puddle->priorityFlags = blocked;
        }
        gmFlagSet(FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN);
    }
}

// Pushing against the puddle in Squall's hall from any side (metatiles (23, 16), (24, 17), (23, 18)
// and (22, 17)) makes Isaac hop over it, if the ground two metatiles on is clear. Once the puddle
// is frozen he can no longer hop it.
static void scAltmillerCaveHopOverPuddle(void)
{
    ACTOR *actor       = fdActorGet(MainRoutineStateWork.leaderActorId);
    Sint32 motionFlags = actor->motionFlags;

    union
    {
        FIELD_POS        pos;
        Sint32           coordinates[3];
        FIELD_COORDINATE components[3];
    } dest;

    FIELD_COORDINATE *components = dest.components;

    components[0].fixed          = actor->x.fixed;
    components[1].fixed          = actor->y.fixed;
    components[2].fixed          = actor->z.fixed;
    {
        Sint32 angle = FACING_SIXTEENTH(actor->facing);

        syVectorAddPolarOffset(toFIXED(32), angle, dest.coordinates);
    }
    if (syFieldActorCheckMovementCollision(actor, &dest.pos) == 0)
    {
        fdScriptStart();
        fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
        sySynch(6);
        sySoundPlay(SE_JUMP);
        fdActorSelectAnimation(actor, ANIM_LEADER_LEAP);
        actor->speedLimit    = toFIXED(3);
        actor->acceleration  = toFIXED(2);
        actor->velocityY     = toFIXED(4);
        actor->motionFlags  &= ACTOR_JUMP_KEEPS_MOTION;
        fdActorSetSpriteFlags(actor, 0);
        fdActorMoveToAndWait(
            ACTOR_PARTY_LEADER, components[0].part[0].integer, components[2].part[0].integer);
        fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
        fdActorSetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
        actor->motionFlags = motionFlags;
        fdScriptEnd();
    }
}

#include "shared/pushed_actor_push.c"

// Update function of the light at the top of the stopped ring. It pulses through four sizes.
static void scAltmillerCavePulseLight(ACTOR *light)
{
    Sint32 scale  = AltmillerCavePulseScales[(light->timer >> 2) & 3];

    light->scaleX = scale;
    light->scaleY = scale;
    light->timer++;
    light->timer &= 15;
}

// Puts a light on the ring, radius pixels from its center above the rocks, at the given angle.
static void scAltmillerCavePlaceLight(Sint32 actorId, Sint32 radius, Sint32 angle)
{
    FIXED  centreX      = toFIXED(504);
    FIXED  centreHeight = toFIXED(24);
    FIXED  depth        = toFIXED(144);
    ACTOR *light        = fdActorGet(actorId);
    Sint32 coordinates[3];

    coordinates[0] = centreX;
    coordinates[2] = centreHeight;
    syVectorAddPolarOffset(radius, angle, coordinates);
    light->x.fixed = coordinates[0];
    light->y.fixed = coordinates[2];
    light->z.fixed = depth;
}

// Spaces the five lights evenly around the ring, the first at the given angle.
static void scAltmillerCavePlaceLights(Sint32 angle)
{
    Sint32 i;

    for (i = 0; i < LIGHT_COUNT; i++)
    {
        scAltmillerCavePlaceLight(ACTOR_FIRST_LIGHT + i, toFIXED(24), angle);
        angle -= DEGtoANG(72);
    }
}

// Event that spins the ring of lights.
static void scAltmillerCaveSpinLights(void)
{
    LIGHT_WHEEL *wheel  = AltmillerCaveLightWheel;
    Sint32       moving = 1;
    Sint32       phase  = wheel->phase;

    if (phase == WHEEL_SPEEDING_UP)
    {
        wheel->speed += WHEEL_SPEED_STEP;
        if (wheel->speed > WHEEL_TOP_SPEED - 1)
        {
            wheel->phase++;
            wheel->ticks = 0;
        }
    }
    else if (phase == WHEEL_COASTING)
    {
        if (wheel->ticks == 30)
        {
            wheel->phase++;
        }
    }
    else if (phase == WHEEL_SLOWING)
    {
        wheel->speed -= WHEEL_SPEED_STEP / 2;
        if (wheel->speed <= WHEEL_LOW_SPEED - 1)
        {
            wheel->phase++;
        }
    }
    else if (phase == WHEEL_STOPPING)
    {
        Sint32 chosenLight = CAVE_SIGNED_STATE(CAVE_CHOSEN_LIGHT);
        Sint32 chosenAngle = syMathDivide(chosenLight * TRIG_TURN, LIGHT_COUNT);
        Sint16 fromTop     = wheel->angle - chosenAngle - DEGtoANG(90);

        if (fromTop > -WHEEL_LOW_SPEED && fromTop < WHEEL_LOW_SPEED)
        {
            wheel->angle                                        = chosenAngle + DEGtoANG(90);
            wheel->phase                                        = WHEEL_STOPPED;
            wheel->speed                                        = 0;
            fdActorGet(ACTOR_FIRST_LIGHT + chosenLight)->update = scAltmillerCavePulseLight;
        }
    }
    else if (phase == WHEEL_STOPPED)
    {
        moving = 0;
    }
    if (moving != 0)
    {
        AltmillerCaveLightWheel->angle += AltmillerCaveLightWheel->speed;
        scAltmillerCavePlaceLights(AltmillerCaveLightWheel->angle);
        AltmillerCaveLightWheel->travel += AltmillerCaveLightWheel->speed;
        if (AltmillerCaveLightWheel->travel > TRIG_TURN * 3 / 16)
        {
            AltmillerCaveLightWheel->travel = 0;
            sySoundPlay(SE_CLICK);
        }
    }
    AltmillerCaveLightWheel->ticks++;
}

// Pressing A at one of the five rocks in the rock room at the bottom of the cave turns it: "Isaac
// rotated the rock." A chime says it was the right rock and an error sound the wrong one. The
// leftmost and then the rightmost bring out the ring of five lights on the wall above the middle
// rock; after that, the rock of the light at the top spins the ring on, and the third such match
// makes the ground shake, the ring spiral into nothing and the wall behind the rocks burst open.
static void scAltmillerCaveTurnRock(Sint32 rock)
{
    Uint8       *puzzle;
    LIGHT_WHEEL *wheel;
    Sint32       correctRock = 0;
    Sint32       i;
    ACTOR       *actor;
    Sint32       scale;
    Sint8        state;
    Sint32       chosenLight;
    Uint8        prevLight;
    FIXED        radius;
    Uint16       angle;

    fdScriptStart();
    fdEndBusyAndReveal();
    fdScriptSetMessageId(MsgAltmillerRotatedRock);
    // "Isaac rotated the rock."
    fdScriptShowMessage(ACTOR_FIRST_ROCK, 0);
    fdScriptEnd();
    for (i = 0; i < LIGHT_COUNT; i++)
    {
        actor         = fdActorGet(ACTOR_FIRST_LIGHT + i);
        actor->update = NULL;
        actor->scaleX = toFIXED(1);
        actor->scaleY = toFIXED(1);
    }
    state        = ROCK_PUZZLE->state;
    prevLight    = ROCK_PUZZLE->chosenLight;
    wheel        = AltmillerCaveLightWheel;
    chosenLight  = ROCK_PUZZLE->chosenLight;
    wheel->angle = (chosenLight * TRIG_TURN) / LIGHT_COUNT + DEGtoANG(90);
    // The leftmost rock, then the rightmost, before the rounds of lights.
    if (state == PUZZLE_START)
    {
        if (rock == ACTOR_BLUE_ROCK)
        {
            state = PUZZLE_LEFT_ROCK_TURNED;
            sySoundPlay(SE_CHIME);
        }
        else
        {
            sySoundPlay(SE_MENU_ERROR);
        }
        CAVE_STATE(CAVE_ROUNDS_MATCHED) = 0;
    }
    else if (state == PUZZLE_LEFT_ROCK_TURNED)
    {
        if (rock == ACTOR_BLUE_ROCK)
        {
            sySoundPlay(SE_CHIME);
        }
        else if (rock == ACTOR_RED_ROCK)
        {
            // The lights grow in one by one, each with SE_LIGHT_APPEAR.
            state = PUZZLE_LIGHTS_SHOWN;
            sySoundPlay(SE_CHIME);
            sySynch(30);
            angle = AltmillerCaveLightWheel->angle;
            for (i = 0; i < LIGHT_COUNT; i++)
            {
                scAltmillerCavePlaceLight(ACTOR_FIRST_LIGHT + i, toFIXED(24), angle);
                sySoundPlay(SE_LIGHT_APPEAR);
                actor         = fdActorGet(ACTOR_FIRST_LIGHT + i);
                actor->scaleX = 0;
                scale         = toFIXED(2) / 5;
                do
                {
                    actor->scaleY = scale;
                    actor->scaleX = scale;
                    sySynch(1);
                    scale += toFIXED(3) / 64;
                } while (actor->scaleX < toFIXED(1));
                angle -= DEGtoANG(72);
            }
            sySynch(30);
            correctRock = 1;
        }
        else
        {
            sySoundPlay(SE_MENU_ERROR);
            state = PUZZLE_START;
        }
    }
    else if (state == PUZZLE_LIGHTS_SHOWN)
    {
        if (rock != ACTOR_FIRST_ROCK + chosenLight)
        {
            // The wrong rock: the lights shrink away.
            state = PUZZLE_START;
            sySoundPlay(SE_MENU_ERROR);
            sySynch(30);
            for (i = 0; i < LIGHT_COUNT; i++)
            {
                actor = fdActorGet(ACTOR_FIRST_LIGHT + i);
                sySoundPlay(SE_LIGHT_APPEAR);
                for (scale  = actor->scaleX; actor->scaleX > toFIXED(2) / 5;
                     scale -= toFIXED(3) / 64)
                {
                    actor->scaleY = scale;
                    actor->scaleX = scale;
                    sySynch(1);
                }
                fdActorSetPos(ACTOR_FIRST_LIGHT + i, 0, 0);
            }
        }
        else
        {
            sySoundPlay(SE_CHIME);
            correctRock = 1;
            sySynch(30);
        }
    }
    puzzle  = &CAVE_STATE(CAVE_PUZZLE);
    *puzzle = state;
    // A light matched: choose a different light and spin the ring to it.
    if (correctRock)
    {
        Uint8 rounds = ++puzzle[CAVE_ROUNDS_MATCHED - CAVE_PUZZLE];

        // One of the four others: 1 to 4 lights on from the last.
        puzzle[CAVE_CHOSEN_LIGHT - CAVE_PUZZLE] =
            ((Sint8)((syRandomBelow(4)) + prevLight + 1) + 5) % 5;
        AltmillerCaveLightWheel->phase  = WHEEL_SPEEDING_UP;
        AltmillerCaveLightWheel->ticks  = 0;
        AltmillerCaveLightWheel->speed  = TRIG_TURN / 128;
        AltmillerCaveLightWheel->travel = TRIG_TURN * 3 / 16;
        sySetTickEvent(scAltmillerCaveSpinLights, EVENT_PASS_TICK);
        if (rounds <= 2)
        {
            while (AltmillerCaveLightWheel->phase != WHEEL_STOPPED)
            {
                sySynch(1);
            }
            sySynch(10);
            sySoundPlay(SE_CHIME);
        }
        else
        {
            // The third light matched: the ground shakes, the ring spirals in to nothing, and the
            // wall bursts.
            *puzzle = PUZZLE_SOLVED;
            while (AltmillerCaveLightWheel->phase != WHEEL_SLOWING)
            {
                sySynch(1);
            }
            AltmillerCaveLightWheel->phase = WHEEL_SLOWING;
            AltmillerCaveLightWheel->ticks = 0;
            fdMapGridSetShake(2, 2, 1);
            fdScriptWait(20);
            fdMapGridSetShake(4, 4, 1);
            AltmillerCaveLightWheel->phase = WHEEL_STOPPED;
            sySoundPlay(SE_WARP);
            radius = toFIXED(24);
            angle  = AltmillerCaveLightWheel->angle;
            do
            {
                for (i = 0; i < LIGHT_COUNT; i++)
                {
                    actor          = fdActorGet(ACTOR_FIRST_LIGHT + i);
                    actor->scaleX -= toFIXED(1) / 4096;
                    actor->scaleY -= toFIXED(1) / 4096;
                    scAltmillerCavePlaceLight(ACTOR_FIRST_LIGHT + i, radius, angle);
                    angle -= DEGtoANG(72);
                }
                radius -= toFIXED(1) / 5;
                angle  += WHEEL_TOP_SPEED;
                sySynch(1);
            } while (radius > 0);
            for (i = 0; i < LIGHT_COUNT; i++)
            {
                fdActorSetPos(ACTOR_FIRST_LIGHT + i, 0, 0);
            }
            scAltmillerCaveReleaseWallBurst();
            sySoundPlay(MUS_PUZZLE_SOLVED);
        }
        syCloseEvent(scAltmillerCaveSpinLights);
    }
}

// The party turns north, toward Babi.
static inline void scAltmillerCaveFacePartyNorth(void)
{
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
}

// Talking to the invisible Babi in his room, or reading his mind, once the party has the Mystic
// Draught: he drinks it and appears. Ivan reads his mind and learns who he is, Lord Babi of Tolbi.
// Two men from Tolbi arrive with torches to take him home, and he asks the party to enter Colosso's
// finals. They decide whether to.
static void scAltmillerCaveRestoreBabiAndInviteParty(void)
{
    SCREEN_TRANSITION_STATE *state;
    ACTOR                   *actor;
    Sint32                   i;
    Sint32                   eva;
    Sint32                   evb;

    gmFlagSet(FLAG_BABI_SAVED);
    gmPartyInventoryDiscard(ITEM_MYSTIC_DRAUGHT);
    fdScriptStart();
    fdEndBusyAndReveal();
    fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
    fdScriptWait(20);
    fdScriptSetMessageId(MsgAltmillerCameBackPleaseDraughtQuickly);
    // "You... You came back... Please, the draught- quickly."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
    fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 232, 160);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdScriptWait(50);
    fdScriptWait(10);
    fdCameraMoveTo(264, CAMERA_KEEP, 200, 1);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
    fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 264, 208);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    syFieldActorLaunchFromLeader(ACTOR_GARET, -16, 16, FACING_NORTH);
    syFieldActorLaunchFromLeader(ACTOR_MIA, 0, 16, FACING_NORTH);
    syFieldActorLaunchFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
    fdActorWaitForMoveById(ACTOR_GARET);
    fdScriptWait(20);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
    // "Oh, thank you! You've saved my life. Squeak... Glug glug glug..."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdScriptWait(20);
    actor = fdActorGet(ACTOR_INVISIBLE_BABI);
    fdScriptWait(30);
    fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
    fdScriptWait(30);
    fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
    fdScriptWait(30);
    fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
    fdScriptWait(60);
    sySoundPlay(SOUND_FADE_OUT_BGM);
    actor->sprite->rotation = 0;
    fdActorJump(ACTOR_INVISIBLE_BABI, 10, 70);
    for (i = 0; i < 30; i++)
    {
        actor->sprite->image.priority = 0;
        fdScriptWait(2);
        actor->sprite->image.priority = 2;
        fdScriptWait(2);
    }
    fdScriptWait(40);
    {
        Sint32 x;
        Sint32 z;

        x = fdActorGet(ACTOR_INVISIBLE_BABI)->x.part[0].integer;
        z = fdActorGet(ACTOR_INVISIBLE_BABI)->z.part[0].integer;
        fdActorSetPos(ACTOR_INVISIBLE_BABI, 0, 0);
        fdActorSetPos(ACTOR_BABI, x, z);
    }
    actor->sprite->flags = 0;
    sySoundPlay(MUS_BABIS_THEME);
    fdActorSetSpeed(ACTOR_BABI, 0.8);
    fdActorWalkByAndWait(ACTOR_BABI, 32, 32);
    fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
    fdScriptWait(20);
    fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    fdActorGet(ACTOR_GARET)->steeringFlags        &= ~ACTOR_FACES_MOTION;
    fdActorGet(ACTOR_MIA)->steeringFlags          &= ~ACTOR_FACES_MOTION;
    fdActorGet(ACTOR_IVAN)->steeringFlags         &= ~ACTOR_FACES_MOTION;
    fdActorWalkBy(ACTOR_PARTY_LEADER, 0, 16);
    fdActorWalkBy(ACTOR_GARET, 0, 16);
    fdActorWalkBy(ACTOR_MIA, 0, 16);
    fdActorWalkByAndWait(ACTOR_IVAN, 0, 16);
    fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    fdActorGet(ACTOR_GARET)->steeringFlags        |= ACTOR_FACES_MOTION;
    fdActorGet(ACTOR_MIA)->steeringFlags          |= ACTOR_FACES_MOTION;
    fdActorGet(ACTOR_IVAN)->steeringFlags         |= ACTOR_FACES_MOTION;
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
    fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
    sySynch(1);
    scAltmillerCaveFacePartyNorth();
    fdScriptWait(30);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdActorStartRepeatedMotion(ACTOR_GARET, 2);
    fdActorStartRepeatedMotion(ACTOR_MIA, 2);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
    fdScriptWait(20);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    // "Thank you for your help. I'll be okay now."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    fdActorJump(ACTOR_GARET, 4, 13);
    fdActorJump(ACTOR_GARET, 4, 30);
    // "Look! It's a man!"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdActorFaceActor(ACTOR_IVAN, ACTOR_GARET, 30);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    // "What are you talking about? What did you expect?"
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_IVAN, 30);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "Well, I mean, we couldn't see who was talking..."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
    fdScriptWait(30);
    // "My sudden appearance seems to have startled you."
    fdScriptOpenMessage(ACTOR_BABI, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(10);
        fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
        fdScriptWait(30);
        fdScriptSetMessageId(MsgAltmillerWouldHaveRevealedMyselfSooner);
        // "I would have revealed myself sooner, but I could not even..."
        fdScriptShowMessage(ACTOR_BABI, 0);
    }
    else
    {
        fdScriptSetMessageId(MsgAltmillerMustTraveledIndeedIfDidnt);
        // "You must be well traveled indeed if that didn't shock you."
        fdScriptShowMessage(ACTOR_BABI, 0);
    }
    fdScriptSetMessageId(MsgAltmillerHowDidBecomeParalyzedWhile);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    fdScriptWait(20);
    // "How did you become paralyzed while you were invisible?"
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
    fdScriptWait(20);
    // "And that ability to conceal yourself... Are you an Adept?"
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    // "An Adept? What are you talking about?"
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    scAltmillerCaveFacePartyNorth();
    fdScriptWait(30);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(30);
    // "You used Psynergy to conceal yourself, didn't you? Only an Adept..."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    fdScriptWait(30);
    // "Oh, um... Yes, I was using Psynergy to conceal myself..."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    // "Something in his tale doesn't sound right..."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    // "I shall peer into his heart."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(20);
    fdActorSetSpeed(ACTOR_IVAN, 1);
    fdActorWalkByAndWait(ACTOR_IVAN, 0, -48);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    fdScriptWait(40);
    // Ivan reads Babi's mind: he is Babi, the ruler of Tolbi, and hid by the power of an ancient
    // civilization.
    fdPsynergyLoadResources(ABILITY_MIND_READ, 1);
    fdPsynergySetupActorPair(ACTOR_IVAN, ACTOR_BABI);
    fdPsynergyStartCast();
    fdPsynergyRunHandler(1);
    fdScriptWait(150);
    fdPsynergyRunHandler(2);
    fdPsynergyStopCasterUpdate();
    fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
    fdScriptWait(40);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    // "What's the matter, Ivan?"
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "This is Babi, the ruler of Tolbi!"
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    fdScriptWait(30);
    // "And that wasn't Psynergy he was using to conceal himself... It was..."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorJump(ACTOR_BABI, 4, 13);
    fdActorJump(ACTOR_BABI, 4, 30);
    fdActorFaceDirection(ACTOR_BABI, FACING_EAST, 0);
    fdScriptWait(30);
    // "I said nothing of the kind! How can you know that?"
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
    fdScriptWait(30);
    // "So, I guess that would make you Adepts..."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    // "Does that mean... Have I finally found true Lemurians!?"
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(30);
    // "That's what I've been saying! We're Adepts. ...But what's Lemuria?"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    // "Yes, what is this 'Lemuria'?"
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    // "What? You're Adepts... but not Lemurians?"
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(50);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_BABI, 2);
    fdScriptWait(20);
    fdScriptWait(20);
    fdActorSetSpeed(ACTOR_IVAN, 1);
    fdActorWalkByAndWait(ACTOR_IVAN, 0, 48);
    fdScriptWait(10);
    fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    scAltmillerCaveFacePartyNorth();
    fdScriptWait(50);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
    fdScriptWait(30);
    // "So I was wrong..."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "Are you searching for these Lemurians, Babi?"
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_BABI, 2);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    fdScriptWait(30);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "Babi, you're the ruler of Tolbi... You can have anything you want...."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
    fdScriptWait(30);
    // "Even the ruler of Tolbi has his limitations."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_BABI, FACING_NORTH, 0);
    fdScriptWait(50);
    // "Every year, I grow older and more frail... This body's had it."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    // "And you regret growing older?"
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_BABI, 2);
    fdScriptWait(20);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    // "I understand your desire to remain young, but there's nothing anyone..."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_BABI, 2);
    fdScriptWait(20);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    // "Wait a sec, Isaac! Do you remember when we were in Sol Sanctum?"
    fdScriptOpenMessage(ACTOR_GARET, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptSetMessageId(MsgAltmillerThatsKradenSaidAlchemyCould);
        // "That's right! Kraden said Alchemy could thwart death itself!"
        fdScriptShowMessage(ACTOR_GARET, 0);
    }
    else
    {
        fdScriptSetMessageId(MsgAltmillerForgotKradenSaidAlchemyCould);
        // "You forgot? Kraden said Alchemy could be used to thwart death."
        fdScriptShowMessage(ACTOR_GARET, 0);
    }
    fdScriptSetMessageId(MsgAltmillerWhatWasThat);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
    fdScriptWait(10);
    // "What was that?"
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    fdScriptWait(10);
    fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdScriptWait(30);
    fdScriptWait(10);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(30);
    // "Did you say you know Kraden?"
    fdScriptOpenMessage(ACTOR_BABI, 0);
    fdScriptSetMessageId(MsgAltmillerBabi);
    fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
    // The room brightens as two men from Tolbi arrive: "Babi!" The light widens to fill the screen
    // and the darkness (BG1) fades out.
    state        = ScreenTransitionStateWork;
    state->value = ALTMILLER_CAVE_LIGHT_FULL;
    for (eva = 0, evb = BLEND_WEIGHT_MAX; eva <= BLEND_WEIGHT_MAX; eva++, evb--)
    {
        IO_WRITE16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
        REG_BLDALPHA = BLEND_ALPHA(evb, eva);
        sySynch(7);
    }
    {
        Uint16 *slot  = (Uint16 *)&state->outsideLayers;
        Sint32  value = ALTMILLER_CAVE_BRIGHTENING_WINOUT;

        *slot         = value;
    }
    fdActorSetPos(ACTOR_FIRST_SOLDIER, 280, 304);
    fdActorSetPos(ACTOR_SECOND_SOLDIER, 296, 304);
    // "Babi!"
    fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    fdScriptWait(10);
    fdScriptWait(10);
    fdCameraFollowActor(ACTOR_FIRST_SOLDIER, 1);
    fdCameraWaitForMove();
    fdScriptWait(70);
    fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "We've been searching everywhere for you, Babi!"
    fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
    fdActorWatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_FIRST_SOLDIER);
    fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_FIRST_SOLDIER);
    fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_FIRST_SOLDIER);
    fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_FIRST_SOLDIER);
    // The two men greet Babi; he invites the party to the finals.
    fdActorSetSpeed(ACTOR_FIRST_SOLDIER, 1.4);
    fdActorSetSpeed(ACTOR_SECOND_SOLDIER, 1.4);
    fdActorSetScript(ACTOR_FIRST_SOLDIER, AltmillerCaveGuardsArrivePath);
    fdScriptWait(3);
    fdActorWalkByAndWait(ACTOR_SECOND_SOLDIER, -16, 0);
    fdActorSetScript(ACTOR_SECOND_SOLDIER, AltmillerCaveGuardsArrivePath);
    fdActorWaitForScriptEndById(ACTOR_FIRST_SOLDIER);
    fdActorWalkByAndWait(ACTOR_FIRST_SOLDIER, 0, -16);
    fdActorWaitForScriptEndById(ACTOR_SECOND_SOLDIER);
    fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
    fdActorStop(ACTOR_PARTY_LEADER);
    fdActorStop(ACTOR_GARET);
    fdActorStop(ACTOR_MIA);
    fdActorStop(ACTOR_IVAN);
    scAltmillerCaveFacePartyNorth();
    fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
    fdCameraMoveTo(248, CAMERA_KEEP, 216, 1);
    fdCameraWaitForMove();
    fdScriptWait(20);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
    fdScriptWait(20);
    // "Iodem said we would find you in Altmiller Cave!"
    fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_BABI, FACING_SOUTHEAST, 0);
    fdScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_BABI, 2);
    fdScriptWait(50);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    // "You're...not glad we found you?"
    fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_BABI, 2);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
    fdScriptWait(10);
    // "No, it's not that."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "What are you doing in a cave such as this, my lord?"
    fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "I have my reasons..."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdActorWalkByAndWait(ACTOR_SECOND_SOLDIER, 16, 0);
    fdScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
    fdScriptWait(20);
    // "Everyone in Tolbi has been in a panic over your absence."
    fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_BABI, 2);
    fdScriptWait(20);
    // "I did not mean to cause anyone any undue concern."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdActorWalkByAndWait(ACTOR_FIRST_SOLDIER, 16, 0);
    fdScriptWait(20);
    // "We're just happy we found you before the finals began."
    fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    // "The trials have already ended?"
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorJump(ACTOR_SECOND_SOLDIER, 4, 13);
    fdActorJump(ACTOR_SECOND_SOLDIER, 4, 30);
    // "Why have you been in this cave for so long?"
    fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "I had an important errand, but it has been taken care of."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
    fdScriptWait(30);
    // "Splendid! Then you can attend the finals."
    fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_BABI, FACING_SOUTHEAST, 0);
    fdScriptWait(30);
    // "I must speak more with these people."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_SOUTHEAST, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_SOUTHEAST, 0);
    fdScriptWait(70);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
    fdScriptWait(30);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    // "Who are they?"
    fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_BABI, FACING_WEST, 0);
    fdScriptWait(30);
    // "I owe my life to them..."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorFaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 70);
    fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_SOUTHEAST, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
    fdScriptWait(30);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
    fdScriptWait(20);
    // "Pardon our rudeness. You have our gratitude for saving Babi's life."
    fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
    fdScriptWait(10);
    fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    scAltmillerCaveFacePartyNorth();
    fdScriptWait(30);
    fdScriptWait(10);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
    // "We must have Iodem grant them a reward."
    fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
    fdScriptWait(30);
    fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
    fdScriptWait(20);
    // "You can speak with them again after the finals are finished."
    fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "Oh. I see... If that's what we have to do, then..."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    fdScriptWait(40);
    fdActorWatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_BABI);
    fdActorWatchAndSetScript(ACTOR_GARET, ACTOR_BABI);
    fdActorWatchAndSetScript(ACTOR_MIA, ACTOR_BABI);
    fdActorWatchAndSetScript(ACTOR_IVAN, ACTOR_BABI);
    fdActorSetSpeed(ACTOR_BABI, 1);
    fdActorWalkByAndWait(ACTOR_BABI, -16, 0);
    fdActorWalkBy(ACTOR_FIRST_SOLDIER, 0, 48);
    fdActorWalkBy(ACTOR_SECOND_SOLDIER, 0, 48);
    fdActorWalkByAndWait(ACTOR_BABI, -16, 0);
    fdActorWalkByAndWait(ACTOR_BABI, 0, 32);
    fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_STAND);
    fdActorSetAnimation(ACTOR_SECOND_SOLDIER, ANIM_STAND);
    fdActorStop(ACTOR_PARTY_LEADER);
    fdActorStop(ACTOR_GARET);
    fdActorStop(ACTOR_MIA);
    fdActorStop(ACTOR_IVAN);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    fdActorFaceDirection(ACTOR_BABI, FACING_EAST, 0);
    fdScriptWait(20);
    // "I have not heard your names yet."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_NORTH, 0);
    fdScriptWait(30);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    // "I'm Garet."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_GARET, ACTOR_BABI, 30);
    // "This is Isaac."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    // "I am Ivan."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(30);
    // "My name is Mia."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_BABI, 2);
    fdScriptWait(20);
    // "Come see me at my palace once the finals have ended."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
    fdScriptWait(70);
    fdActorFaceDirection(ACTOR_BABI, FACING_EAST, 0);
    fdScriptWait(20);
    // "I would like to speak with you further."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    fdScriptWait(30);
    // Babi and the two men walk back toward the cave entrance.
    fdActorSetScript(ACTOR_SECOND_SOLDIER, AltmillerCaveSecondGuardLeavePath);
    fdActorSetScript(ACTOR_FIRST_SOLDIER, AltmillerCaveFirstGuardLeavePath);
    fdActorSetScript(ACTOR_BABI, AltmillerCaveBabiLeavePath);
    fdScriptWait(10);
    fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    fdActorWaitForScriptEndById(ACTOR_BABI);
    // "Say, I have an idea... Why don't you enter the finals?"
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    fdScriptWait(20);
    fdCameraMoveTo(248, CAMERA_KEEP, 248, 1);
    fdCameraWaitForMove();
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_BABI, FACING_NORTH, 0);
    fdScriptWait(20);
    // "You have some interesting powers."
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_WEST, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_WEST, 0);
    fdScriptWait(20);
    // "But, Lord Babi, they have not cleared the trials."
    fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
    fdScriptWait(30);
    // "Only warriors who clear the trials can appear in the finals."
    fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
    fdActorFaceDirection(ACTOR_BABI, FACING_EAST, 0);
    fdScriptWait(20);
    // "And you don't think that saving my life qualifies as a suitable trial?"
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    fdScriptWait(30);
    fdActorFaceActor(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
    fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_WEST, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_BABI, FACING_NORTH, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    fdScriptWait(30);
    // "I look forward to seeing you battle!"
    fdScriptShowMessage(ACTOR_BABI, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    fdScriptWait(30);
    fdActorFaceDirection(ACTOR_BABI, FACING_EAST, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    fdScriptWait(20);
    fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
    fdScriptWait(40);
    fdActorWalkBy(ACTOR_BABI, 32, 0);
    fdActorWalkBy(ACTOR_SECOND_SOLDIER, 0, 64);
    fdActorWalkByAndWait(ACTOR_FIRST_SOLDIER, 16, 0);
    fdActorWalkBy(ACTOR_FIRST_SOLDIER, 0, 64);
    fdActorWaitForMoveById(ACTOR_BABI);
    fdActorWalkByAndWait(ACTOR_BABI, 0, 64);
    fdActorSetPos(ACTOR_BABI, 0, 0);
    fdActorSetPos(ACTOR_FIRST_SOLDIER, 0, 0);
    fdActorSetPos(ACTOR_SECOND_SOLDIER, 0, 0);
    fdScriptWait(20);
    // With Babi gone the darkness returns: BG1 fades back in, then the light shrinks around the
    // party again.
    for (eva = 0, evb = BLEND_WEIGHT_MAX; eva <= BLEND_WEIGHT_MAX; eva++, evb--)
    {
        IO_WRITE16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
        REG_BLDALPHA = BLEND_ALPHA(eva, evb);
        sySynch(7);
    }
    state->value = ALTMILLER_CAVE_LIGHT_STEPS;
    {
        Uint16 *slot  = (Uint16 *)&state->outsideLayers;
        Sint32  value = ALTMILLER_CAVE_DARK_LIGHT_WINOUT;

        *slot         = value;
    }
    sySynch(1);
    IO_WRITE16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
    IO_WRITE16(ALTMILLER_CAVE_DARK_ALPHA, &REG_BLDALPHA);
    fdCameraMoveToActorById(ACTOR_PARTY_LEADER, 1);
    fdCameraWaitForMove();
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    // "He's asking us to appear in Colosso, isn't he?"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    // "Is he serious?"
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "He wants to see us fight using our Psynergy... That's what it is."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(20);
    // "The finals... Should we do it?"
    fdScriptOpenMessage(ACTOR_GARET, 0);
    fdActorFaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
    fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(10);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        fdScriptWait(20);
        fdScriptSetMessageId(MsgAltmillerSoWellDoIt);
        // "So we'll do it?"
        fdScriptShowMessage(ACTOR_GARET, 0);
    }
    else
    {
        fdScriptWait(10);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        fdScriptWait(20);
        fdScriptSetMessageId(MsgAltmillerDontWantEitherDo);
        // "You don't want to either, do you?"
        fdScriptShowMessage(ACTOR_GARET, 0);
    }
    fdScriptSetMessageId(MsgAltmillerEitherWayWereStuckHere);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    fdScriptWait(30);
    // "Either way, we're stuck here in Tolbi until Colosso is over."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(30);
    // "Babi said that he wanted to talk to us, didn't he?"
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "He seemed to know Kraden."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    // "What could be the connection between Lemuria and Adepts?"
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    fdScriptWait(20);
    // "I suspect he knows quite a bit about what's going on with us."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    // "I guess all we can do is wait around until Colosso is..."
    fdScriptShowMessage(ACTOR_GARET, 0);
    // "Come on, let's enter Colosso! Can we do it?"
    fdScriptOpenMessage(ACTOR_GARET, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(10);
        fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
        fdScriptWait(30);
        fdScriptSetMessageId(MsgAltmillerGreatItsDecidedJustHave);
        // "Great! It's decided. We'll just have to do our best!"
        fdScriptShowMessage(ACTOR_GARET, 0);
        fdScriptWait(10);
        fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
        fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        fdScriptWait(30);
        // "If you want to do it, Isaac, we'll go along with you."
        fdScriptShowMessage(ACTOR_IVAN, 0);
        fdScriptWait(20);
        fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
        fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
        fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
        fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
        fdScriptWait(30);
    }
    else
    {
        fdScriptWait(10);
        fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
        fdScriptWait(30);
        fdScriptSetMessageId(MsgAltmillerDontLetMeDownLike);
        // "What? Don't let me down like this!"
        fdScriptShowMessage(ACTOR_GARET, 0);
        fdScriptWait(10);
        fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
        fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        fdScriptWait(30);
        // "I guess we'll just wait until we reach Tolbi and let Isaac..."
        fdScriptShowMessage(ACTOR_IVAN, 0);
        fdScriptWait(10);
        fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
        fdScriptWait(30);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
    }
    sySoundPlay(SOUND_FADE_OUT_BGM);
    fdActorSetSpeed(ACTOR_GARET, 1.2);
    fdActorSetSpeed(ACTOR_IVAN, 1.2);
    fdActorSetSpeed(ACTOR_MIA, 1.2);
    scWalkIntoLeader(ACTOR_GARET);
    scWalkIntoLeader(ACTOR_IVAN);
    scWalkIntoLeader(ACTOR_MIA);
    gmPlayMusic();
    fdScriptEnd();
}

// Talking to the white outline lying on the floor near the middle of Babi's room, or reading its
// mind with Mind Read, the first time: the invisible Babi, too weak to move, asks the party to
// bring his draught from deep in the cave and tells them the order of the rocks' colors.
static void scAltmillerCaveMeetInvisibleBabi(void)
{
    ACTOR *leader;
    Sint32 line;

    gmFlagSet(FLAG_MET_INVISIBLE_BABI);
    sySoundPlay(MUS_CAVE_THEME);
    fdScriptStart();
    fdEndBusyAndReveal();
    fdScriptSetMessageId(MsgAltmillerSee);
    fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
    fdScriptWait(20);
    // "You... You can see me?"
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdCameraMoveTo(248, CAMERA_KEEP, 184, 1);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
    fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 248, 192);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    syFieldActorLaunchFromLeader(ACTOR_GARET, -16, 16, FACING_NORTH);
    syFieldActorLaunchFromLeader(ACTOR_MIA, 0, 16, FACING_NORTH);
    syFieldActorLaunchFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
    fdActorWaitForMoveById(ACTOR_GARET);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(40);
    // "Someone's there."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    fdScriptWait(20);
    // "Wrapped in Psynergetic light... Who could it be?"
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    // "You know about Psynergy? And you can see me?"
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorFaceEachOther(ACTOR_GARET, ACTOR_IVAN, 50);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdScriptWait(30);
    // "Come on, mister! Don't hide-show yourself!"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorSetAttachedEffect(ACTOR_INVISIBLE_BABI, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
    fdScriptWait(30);
    // "Even if I wanted to show myself... As you can see, I cannot..."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(20);
    fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdScriptWait(30);
    // "What do you mean, 'as you can see'? You're invisible!"
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    // "Oh... That's right."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "You say you can't move... What happened?"
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    // "Hmmm, yes. I ran out of my draught and collapsed here."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    // "Draught?"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdScriptWait(20);
    // "Exactly what kind of draught is it?"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "You couldn't even begin to comprehend if I tried to tell..."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "You seem to be in a lot of pain. Your life depends on that..."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
    // "It does. Could you do me a favor?"
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdScriptWait(60);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
    fdScriptWait(20);
    // "A favor? What is it?"
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(20);
    // "I'd like you to get my draught for me."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "Get it? Where is it?"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
    fdScriptWait(20);
    // "Deep in this cave..."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    fdScriptWait(20);
    // "What, is there a pharmacy down there?"
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "Witty, aren't we? I'll have to tell you how to find it. Can..."
    fdScriptOpenMessage(ACTOR_INVISIBLE_BABI, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptSetMessageId(MsgAltmillerKiddingHaventActually);
        fdScriptWait(20);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
        fdActorSetSpeed(ACTOR_GARET, 2);
        fdActorWalkByAndWait(ACTOR_GARET, 0, -16);
        fdScriptWait(10);
        fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
        // "Are you kidding, Isaac? We haven't even actually seen this guy!"
        fdScriptShowMessage(ACTOR_GARET, 0);
    }
    else
    {
        fdScriptSetMessageId(MsgAltmillerDontTrustAnyone);
        fdScriptWait(10);
        fdActorSetSpeed(ACTOR_GARET, 1);
        fdActorWalkByAndWait(ACTOR_GARET, 0, -16);
        fdScriptWait(10);
        fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
        fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
        fdScriptWait(20);
        // "I'm with you, Isaac. I don't trust anyone who's afraid to..."
        fdScriptShowMessage(ACTOR_GARET, 0);
    }

    // Ivan and Mia speak up for helping him, and Garet gives in.
    line = MsgAltmillerSaidCouldntMove;
    fdScriptSetMessageId(line);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    fdActorSetSpeed(ACTOR_IVAN, 2);
    fdActorWalkByAndWait(ACTOR_IVAN, 0, -16);
    fdScriptWait(10);
    fdActorFaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
    // "He said he couldn't move even if he wanted to, right?..."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_MIA, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_MIA, 0);
    fdActorFaceActor(ACTOR_IVAN, ACTOR_MIA, 0);
    fdScriptWait(20);
    // "I agree with Ivan- whoever he is, he needs our help."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(20);
    fdActorSetSpeed(ACTOR_GARET, 1);
    fdActorWalkByAndWait(ACTOR_GARET, 0, 16);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    fdScriptWait(30);
    fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(20);
    // "Fine! Okay! We should help people in need. Is that better!?"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(30);
    fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 30);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(30);
    fdActorSetSpeed(ACTOR_IVAN, 1);
    fdActorWalkByAndWait(ACTOR_IVAN, 0, 16);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    line += 7;
    fdScriptSetMessageId(line);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    // "Will you get it for me?!"
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    // "What do we have to do to get your draught?"
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
    // "First, you'll have to go deep into the cave."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    // "You'll find five rocks sticking up out of the ground."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(30);
    // "Rotate the rocks on the left and right sides."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdScriptWait(30);
    // "When you turn the rocks in the right order, five colored lights will..."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    // "Colored lights!? This sounds weird."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(30);
    // "That's right. Lights will appear on the wall."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdScriptWait(30);
    // "There will be five colors: blue, green, white, yellow, and red."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdScriptWait(30);
    // "Turn the five rocks in accordance with those colors."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    // "Riiight... Spin the rocks in accordance with the color of the light..."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(30);
    // "The blue light corresponds to the rock farthest to the left. In..."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
    fdScriptWait(20);
    // "So, in order, left to right, the colors are blue, green, white,..."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
    // "Correct. If you do this correctly, a hidden door will open."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    // "A door? Somewhere deep in a cave? Who put it there?"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "An ancient civilization, long since vanished. Ohhh! Please hurry!"
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
    fdScriptWait(20);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "He seems to be in a lot of pain. We'd better find that cave, Isaac."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_MIA, 40);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(30);
    fdActorFaceActor(ACTOR_GARET, ACTOR_MIA, 0);
    fdActorFaceActor(ACTOR_IVAN, ACTOR_MIA, 0);
    fdScriptWait(20);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    sySoundPlay(SOUND_FADE_OUT_BGM);
    fdActorSetSpeed(ACTOR_GARET, 1.2);
    fdActorSetSpeed(ACTOR_IVAN, 1.2);
    fdActorSetSpeed(ACTOR_MIA, 1.2);
    fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if (leader != NULL)
    {
        fdActorSetDestination(ACTOR_GARET, leader->x.part[0].integer, leader->z.part[0].integer);
    }
    fdActorWaitForMoveById(ACTOR_GARET);
    fdActorSetPos(ACTOR_GARET, 0, 0);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if (leader != NULL)
    {
        fdActorSetDestination(ACTOR_IVAN, leader->x.part[0].integer, leader->z.part[0].integer);
    }
    fdActorWaitForMoveById(ACTOR_IVAN);
    fdActorSetPos(ACTOR_IVAN, 0, 0);
    fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if (leader != NULL)
    {
        fdActorSetDestination(ACTOR_MIA, leader->x.part[0].integer, leader->z.part[0].integer);
    }
    fdActorWaitForMoveById(ACTOR_MIA);
    fdActorSetPos(ACTOR_MIA, 0, 0);
    gmPlayMusic();
    fdScriptEnd();
}

// Talking to the invisible Babi again before the draught is found: he asks whether the party forgot
// the order of the rock colors, and tells it again.
static void scAltmillerCaveRepeatRockOrder(void)
{
    Sint32 question;

    fdScriptStart();
    question = MsgAltmillerForgetOrderRock;
    fdScriptSetMessageId(question);
    // "Did you forget the order of rock colors?"
    fdScriptOpenMessage(ACTOR_INVISIBLE_BABI, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(20);
        fdScriptSetMessageId(question + 1);
        // "From the left, it's blue, green, white, yellow, and red...."
        fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    }
    else
    {
        fdScriptWait(20);
        fdScriptSetMessageId(question + 2);
        // "Deep in the cave are five rocks. Turn the ones on the ends..."
        fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    }
    fdScriptEnd();
}

// As the party walks away from the invisible Babi toward the stairs down in the north-west of his
// room (metatiles (11, 9) to (13, 9)), he calls them back once: "Wait..." Isaac turns, and Babi
// tells where the draught is: "The draught is... on a table behind the door."
static void scAltmillerCaveBabiCallsPartyBack(void)
{
    if (gmFlagGet(FLAG_MET_INVISIBLE_BABI) == 0)
        return;
    if (gmFlagGet(FLAG_BABI_SAVED) != 0)
        return;

    gmFlagSet(FLAG_BABI_CALLED_PARTY_BACK);
    fdScriptStart();
    fdScriptSetMessageId(MsgAltmillerWait);
    // "Wait..."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdScriptWait(30);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_INVISIBLE_BABI, 0);
    fdScriptWait(30);
    // "The draught is... on a table behind the door."
    fdScriptShowMessage(ACTOR_INVISIBLE_BABI, 0);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(20);
    fdScriptEnd();
}

const CUE *scGetCues(void)
{
    Sint16 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_ALTMILLER_CAVE_BABI)
    {
        return AltmillerCaveBabiCues;
    }
    if (sceneId == SCENE_ALTMILLER_CAVE_VIAL_CHEST)
    {
        return AltmillerCaveVialChestCues;
    }
    if (sceneId == SCENE_ALTMILLER_CAVE_SQUALL)
    {
        return AltmillerCaveSquallCues;
    }
    if (sceneId == SCENE_ALTMILLER_CAVE_COOKIE_CHEST)
    {
        return AltmillerCaveCookieChestCues;
    }
    if (sceneId == SCENE_ALTMILLER_CAVE_DRAUGHT)
    {
        return AltmillerCaveDraughtCues;
    }
    return AltmillerCaveCuesOther;
}

// Update function of the fallen columns in the hall at the bottom of the cave: each is drawn at the
// leader's own sprite priority.
static void scAltmillerCaveMatchLeaderPriority(ACTOR *actor)
{
    if (actor != NULL)
    {
        actor->priorityFlags          = 0;
        actor->sprite->image.priority = fdActorGet(ACTOR_PARTY_LEADER)->sprite->image.priority;
    }
}

// Walls off the metatiles the three fallen columns stand on.
static inline void scAltmillerCaveBlockFallenColumnMetatiles(void)
{
    scPushedActorBlockFootprint(ACTOR_FIRST_FALLEN_COLUMN);
    scPushedActorBlockFootprint(ACTOR_FIRST_FALLEN_COLUMN + 1);
    scPushedActorBlockFootprint(ACTOR_FIRST_FALLEN_COLUMN + 2);
}

// Sets up the room being entered: darkness, the ice pillar in Squall's hall as the party left it,
// Babi still lying there or gone, and in the rock room the rocks and lights as far as the puzzle
// got. Returns 0.
Sint32 scInit(void)
{
    ACTOR *actor;
    Sint32 i;

    if (MainRoutineStateWork.entrance == 0)
    {
        if (MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_BABI)
        {
            MainRoutineStateWork.entrance = ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_ENTRANCE;
        }
        if (MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_VIAL_CHEST)
        {
            MainRoutineStateWork.entrance = ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_BABI;
        }
        if (MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_SQUALL)
        {
            MainRoutineStateWork.entrance = ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_VIAL_CHEST;
        }
        if (MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_COOKIE_CHEST)
        {
            MainRoutineStateWork.entrance = ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_SQUALL;
        }
        if (MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_DRAUGHT)
        {
            MainRoutineStateWork.entrance = ALTMILLER_CAVE_DRAUGHT_FROM_ALTMILLER_CAVE_COOKIE_CHEST;
        }
    }
    gmFlagSet(FLAG_ALTMILLER_CAVE_DARK);
    fdScriptClearFlag(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS);
    if (MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_ENTRANCE)
    {
        if (MainRoutineStateWork.entrance == ALTMILLER_CAVE_ENTRANCE_FROM_WORLD_MAP)
        {
            if (!gmFlagGet(FLAG_KEEP_PARTY_POSITION))
            {
                CAVE_STATE(CAVE_DARKNESS) = 0;
            }
            gmFlagSet(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS);
        }
        if (MainRoutineStateWork.entrance == ALTMILLER_CAVE_ENTRANCE_FROM_TORCH_CHAMBER)
        {
            if (!gmFlagGet(FLAG_KEEP_PARTY_POSITION))
            {
                CAVE_STATE(CAVE_DARKNESS) = 5;
            }
            gmFlagSet(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS);
        }
    }
    if (MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_BABI)
    {
        if (gmFlagGet(FLAG_BABI_SAVED))
        {
            fdActorSetPos(ACTOR_INVISIBLE_BABI, 0, 0);
        }
        else
        {
            actor                         = fdActorGet(ACTOR_INVISIBLE_BABI);
            actor->sprite->image.priority = 1;
            actor->sprite->flags          = SPRITE_FLAG_OUTLINED;
            actor->sprite->rotation       = DEGtoANG(90);
        }
    }
    if (MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_SQUALL)
    {
        gmFlagClear(FLAG_ALTMILLER_CAVE_DARK);
        scAltmillerCaveBlockFallenColumnMetatiles();
        if (gmFlagGet(FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN))
        {
            fdActorSetAnimation(ACTOR_SQUALL_ROOM_PUDDLE, PUDDLE_ANIMATION_PILLAR);
            fdMapGridCopyMetatileAttributes(76, 16, 1, 1, 73, 17);
        }
        else
        {
            fdActorGet(ACTOR_SQUALL_ROOM_PUDDLE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        }
        fdActorSetSpriteFlags(fdActorGet(ACTOR_SQUALL_ROOM_PUDDLE), 0);
#if ALTMILLER_PILLAR_BRIDGE_KEPT_AFTER_BATTLE
        // The metatiles scAltmillerCaveDropPillarIntoPit copies as it sets the flag, redrawn on
        // arrival.
        if (gmFlagGet(FLAG_ALTMILLER_CAVE_PILLAR_IN_PLACE))
        {
            fdMapGridCopyMetatileAttributes(30, 20, 1, 1, 32, 20);
        }
#endif
    }
    if (MainRoutineStateWork.next == SCENE_ALTMILLER_CAVE_DRAUGHT)
    {
        gmFlagClear(FLAG_ALTMILLER_CAVE_DARK);
        scAltmillerCaveBlockFallenColumnMetatiles();
        fdActorGet(ACTOR_FIRST_FALLEN_COLUMN)->update     = scAltmillerCaveMatchLeaderPriority;
        fdActorGet(ACTOR_FIRST_FALLEN_COLUMN + 1)->update = scAltmillerCaveMatchLeaderPriority;
        // UB: the last is stored through a word view of the function pointer.
#if FIX_TYPE_PUNNING
        fdActorGet(ACTOR_FIRST_FALLEN_COLUMN + 2)->update = scAltmillerCaveMatchLeaderPriority;
#else
        *(Sint32 *)&fdActorGet(ACTOR_FIRST_FALLEN_COLUMN + 2)->update =
            (Sint32)scAltmillerCaveMatchLeaderPriority;
#endif
        // Coming up to the rocks (ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE) starts the puzzle
        // over, with the red light first.
        if (MainRoutineStateWork.entrance == ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE)
        {
            DMA_FILL32(0, AltmillerCaveLightWheel, sizeof(LIGHT_WHEEL));
            if (!gmFlagGet(FLAG_KEEP_PARTY_POSITION))
            {
                CAVE_STATE(CAVE_ROUNDS_MATCHED) = 0;
                CAVE_STATE(CAVE_PUZZLE)         = PUZZLE_START;
                CAVE_STATE(CAVE_CHOSEN_LIGHT)   = ACTOR_RED_LIGHT - ACTOR_FIRST_LIGHT;
            }
        }
        if (CAVE_SIGNED_STATE(CAVE_PUZZLE) == PUZZLE_SOLVED)
        {
            fdMapGridCopyMetatilesLayered(41, 55, 3, 2, 30, 55);
            fdMapGridCopyMetatileAttributes(42, 8, 1, 1, 31, 8);
        }
        if (CAVE_SIGNED_STATE(CAVE_PUZZLE) == PUZZLE_LIGHTS_SHOWN)
        {
            scAltmillerCavePlaceLights(
                (CAVE_SIGNED_STATE(CAVE_CHOSEN_LIGHT) * TRIG_TURN) / LIGHT_COUNT + DEGtoANG(90));
        }
        for (i = 0; i < LIGHT_COUNT; i++)
        {
            actor                 = fdActorGet(ACTOR_FIRST_LIGHT + i);
            actor->motionFlags    = 0;
            actor->collisionFlags = 0;
            actor->scaleX         = toFIXED(1);
            actor->scaleY         = toFIXED(1);
            fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_LIGHT + i), 0);
            // The lights glimmer alike, each starting six ticks after the one before, so that they
            // do not glimmer in step.
            fdActorSetAnimation(ACTOR_FIRST_LIGHT + i, i + 1);
        }
        fdActorSetSpriteColorById(ACTOR_BLUE_LIGHT, COLOR_BLUE);
        fdActorSetSpriteColorById(ACTOR_GREEN_LIGHT, COLOR_GREEN);
        fdActorSetSpriteColorById(ACTOR_WHITE_LIGHT, COLOR_WHITE);
        fdActorSetSpriteColorById(ACTOR_YELLOW_LIGHT, COLOR_YELLOW);
        fdActorSetSpriteColorById(ACTOR_RED_LIGHT, COLOR_RED);
        fdActorSetSpriteColorById(ACTOR_BLUE_ROCK, COLOR_NONE);
        fdActorSetSpriteColorById(ACTOR_GREEN_ROCK, COLOR_NONE);
        fdActorSetSpriteColorById(ACTOR_WHITE_ROCK, COLOR_NONE);
        fdActorSetSpriteColorById(ACTOR_YELLOW_ROCK, COLOR_NONE);
        fdActorSetSpriteColorById(ACTOR_RED_ROCK, COLOR_NONE);
        fdActorGet(ACTOR_BLUE_ROCK)->sprite->image.priority = 3;
        fdActorGet(ACTOR_RED_ROCK)->sprite->image.priority  = 3;
        fdActorGet(ACTOR_BLUE_ROCK)->priorityFlags          = ACTOR_PRIORITY_UNDERFOOT;
        fdActorGet(ACTOR_RED_ROCK)->priorityFlags           = ACTOR_PRIORITY_UNDERFOOT;
        fdActorSetSpriteFlags(fdActorGet(ACTOR_BLUE_ROCK), 0);
        fdActorSetSpriteFlags(fdActorGet(ACTOR_RED_ROCK), 0);
    }
    if (gmFlagGet(FLAG_ALTMILLER_CAVE_DARK))
    {
        scAltmillerCaveInitDarkness();
    }
    else
    {
        FieldStateWork->transition =
            SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
        FieldStateWork->transitionFrame = 24;
    }
    return 0;
}

// Update function of a piece of the burst wall. It flies from the wall's center to a random point
// 120 pixels away, then starts again from the center.
static void scAltmillerCaveUpdateWallFragment(FIELD_PARTICLE *fragment)
{
    FIXED pos[3];

    if (fragment->state == 0)
    {
        FIXED  startZ = fragment->originZ;
        Sint32 startX = fragment->originX;

        fragment->z   = startZ;
        pos[2]        = startZ;
        fragment->x   = startX;
        pos[0]        = startX;
        syVectorAddPolarOffset(toFIXED(120), syRandomNext(), pos);
        fragment->targetX         = pos[0];
        fragment->targetZ         = pos[2];
        fragment->acceleration    = toFIXED(5);
        fragment->speedMax        = toFIXED(5);
        fragment->turnStepLimited = 0;
        fragment->state++;
        if ((FrameCount & 3) == 0)
            sySoundPlay(SE_HEAVY_IMPACT);
    }
    else if (fragment->state == 1)
    {
        if (fdParticleHasPendingTarget(fragment) == 0)
        {
            fragment->state--;
        }
    }
    else if (fragment->state == 2)
    {
        // Unused: no step leads here.
        if (fdParticleHasPendingTarget(fragment) == 0)
            fdEffectClearOwnedSlot(fragment);
    }
}

// The wall above the middle rock bursts open. Pieces of rock fly out from where the ring was, with
// heavy thuds, the wall becomes an opening into the Mystic Draught's room, and the shaking calms.
// While the WALL_BURST_PIECES pieces fly, the field's colors are tinted
// (mnShopSaveAndTintFieldPalette).
static void scAltmillerCaveReleaseWallBurst(void)
{
    FIELD_EFFECT_STATE *effectState;
    VECTOR3             pos;
    FIELD_PARTICLE     *fragment;
    Sint32              fragmentIndex;

    fdEffectInitSlots();
    effectState = (FIELD_EFFECT_STATE *)FieldEffectStateWork;
    mnShopSaveAndTintFieldPalette(PALETTE_TINT_CUSTOM | RGB(8, 8, 8));
    pos.x = toFIXED(504);
    pos.y = toFIXED(24);
    pos.z = toFIXED(144);
    fdCameraWorldToScreen(&pos);
    fragment = effectState->particles;
    for (fragmentIndex = 0; fragmentIndex < WALL_BURST_PIECES; fragmentIndex++)
    {
        FIXED scale;

        fdParticleInit(fragment, SPRITE_PARTICLE, pos.x, pos.z);
        fdParticleSetCallback(fragment, scAltmillerCaveUpdateWallFragment);
        fdParticleSetAnimation(fragment, PARTICLE_ANIMATION_FLICKER);
        fdSpriteSetAnimationColorModes(fragment->sprite, syRandomBelow(7));
        scale            = syMathDivideUnsigned(syRandomNext(), 3) + toFIXED(3) / 2;
        fragment->scaleY = scale;
        fragment->scaleX = scale;
        sySynch(1);
        fragment++;
    }
    sySynch(80);
    fdMapGridCopyMetatilesLayered(41, 55, 3, 2, 30, 55);
    fdMapGridCopyMetatileAttributes(42, 8, 1, 1, 31, 8);
    sySynch(50);
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    sySynch(30);
    for (fragmentIndex = 0; fragmentIndex < (Sint32)ARRAY_COUNT(effectState->particles);
         fragmentIndex++)
    {
        if (effectState->particles[fragmentIndex].active)
            effectState->particles[fragmentIndex].state = 2;
    }
    fdMapGridWaitForShakeToSettle();
    mnShopRestoreFieldPalette();
    fdEffectReleaseSlots();
}

#include "shared/pushed_actor_tables.c"

// The ring of lights, as the cave's code reaches it.
LIGHT_WHEEL   *AltmillerCaveLightWheel  = &AltmillerCaveLightWheelState;

SCENE_ENTRANCE AltmillerCaveEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTMILLER_CAVE_ENTRANCE_FROM_WORLD_MAP,
     CONDITION_ALWAYS, 456,
     0, 504,
     FACING_NORTH, 0,
     0, 0,
     544, 528,
     0},
    {ALTMILLER_CAVE_ENTRANCE_FROM_TORCH_CHAMBER,
     CONDITION_ALWAYS, 280,
     0, 360,
     FACING_SOUTH, 0,
     0, 0,
     544, 528,
     0},
    {ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_FROM_PASSAGE,
     CONDITION_ALWAYS, 120,
     0, 696,
     FACING_NORTH, 0,
     0, 552,
     240, 720,
     0},
    {ALTMILLER_CAVE_ENTRANCE_FROM_ALTMILLER_CAVE_BABI,
     CONDITION_ALWAYS, 120,
     0, 600,
     FACING_SOUTH, 0,
     0, 552,
     240, 720,
     0},
    {ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_ENTRANCE,
     CONDITION_ALWAYS, 392,
     0, 504,
     FACING_SOUTH, 0,
     48, 16,
     496, 672,
     0},
    {ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_VIAL_CHEST,
     CONDITION_ALWAYS, 136,
     0, 104,
     FACING_SOUTH, 0,
     48, 16,
     496, 672,
     0},
    {ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_BABI,
     CONDITION_ALWAYS, 232,
     0, 216,
     FACING_SOUTH, 0,
     64, 48,
     592, 576,
     0},
    {ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_SQUALL,
     CONDITION_ALWAYS, 328,
     0, 264,
     FACING_SOUTH, 0,
     64, 48,
     592, 576,
     0},
    {ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_VIAL_CHEST,
     CONDITION_ALWAYS, 200,
     0, 424,
     FACING_SOUTH, 0,
     48, 80,
     576, 480,
     0},
    {ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_COOKIE_CHEST,
     CONDITION_ALWAYS, 520,
     0, 328,
     FACING_SOUTH, 0,
     48, 80,
     576, 480,
     0},
    {ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_SQUALL,
     CONDITION_ALWAYS, 680,
     0, 152,
     FACING_SOUTH, 0,
     0, 0,
     768, 512,
     0},
    {ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_DRAUGHT,
     CONDITION_ALWAYS, 40,
     0, 248,
     FACING_SOUTH, 0,
     0, 0,
     768, 512,
     0},
    {ALTMILLER_CAVE_DRAUGHT_FROM_ALTMILLER_CAVE_COOKIE_CHEST,
     CONDITION_ALWAYS, 296,
     0, 392,
     FACING_SOUTH, 0,
     160, 304,
     576, 704,
     0},
    {ALTMILLER_CAVE_DRAUGHT_FROM_ROCKS,
     CONDITION_ALWAYS, 424,
     0, 392,
     FACING_SOUTH, 0,
     160, 304,
     576, 704,
     0},
    {ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE,
     CONDITION_ALWAYS, 504,
     0, 232,
     FACING_NORTH, 0,
     384, 64,
     624, 240,
     0},
    {ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_CHEST,
     CONDITION_ALWAYS, 504,
     0, 152,
     FACING_SOUTH, 0,
     384, 64,
     624, 240,
     0},
    {ALTMILLER_CAVE_DRAUGHT_CHEST_FROM_ROCKS,
     CONDITION_ALWAYS, 184,
     0, 216,
     FACING_NORTH, 0,
     64, 64,
     304, 240,
     0},
    {SCENE_TABLE_END},
};

Uint32 AltmillerCaveExits[] = {
    SCENE_EXITS(AltmillerCaveEntrance),
    SCENE_EXIT(
        ALTMILLER_CAVE_ENTRANCE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_ALTMILLER_CAVE_ENTRANCE),
    SCENE_EXIT(ALTMILLER_CAVE_ENTRANCE_TO_TORCH_CHAMBER,
               AltmillerCaveEntrance,
               ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_FROM_PASSAGE),
    SCENE_EXIT(ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_TO_PASSAGE,
               AltmillerCaveEntrance,
               ALTMILLER_CAVE_ENTRANCE_FROM_TORCH_CHAMBER),
    SCENE_EXIT(ALTMILLER_CAVE_ENTRANCE_TO_ALTMILLER_CAVE_BABI,
               AltmillerCaveBabi,
               ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_ENTRANCE),
    SCENE_EXITS(AltmillerCaveBabi),
    SCENE_EXIT(ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_ENTRANCE,
               AltmillerCaveEntrance,
               ALTMILLER_CAVE_ENTRANCE_FROM_ALTMILLER_CAVE_BABI),
    SCENE_EXIT(ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_VIAL_CHEST,
               AltmillerCaveVialChest,
               ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_BABI),
    SCENE_EXITS(AltmillerCaveVialChest),
    SCENE_EXIT(ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_BABI,
               AltmillerCaveBabi,
               ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_VIAL_CHEST),
    SCENE_EXIT(ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_SQUALL,
               AltmillerCaveSquall,
               ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_VIAL_CHEST),
    SCENE_EXITS(AltmillerCaveSquall),
    SCENE_EXIT(ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_VIAL_CHEST,
               AltmillerCaveVialChest,
               ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_SQUALL),
    SCENE_EXIT(ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_COOKIE_CHEST,
               AltmillerCaveCookieChest,
               ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_SQUALL),
    SCENE_EXITS(AltmillerCaveCookieChest),
    SCENE_EXIT(ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_SQUALL,
               AltmillerCaveSquall,
               ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_COOKIE_CHEST),
    SCENE_EXIT(ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_DRAUGHT,
               AltmillerCaveDraught,
               ALTMILLER_CAVE_DRAUGHT_FROM_ALTMILLER_CAVE_COOKIE_CHEST),
    SCENE_EXITS(AltmillerCaveDraught),
    SCENE_EXIT(ALTMILLER_CAVE_DRAUGHT_TO_ALTMILLER_CAVE_COOKIE_CHEST,
               AltmillerCaveCookieChest,
               ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_DRAUGHT),
    SCENE_EXIT(ALTMILLER_CAVE_DRAUGHT_TO_ROCKS,
               AltmillerCaveDraught,
               ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE),
    SCENE_EXIT(ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_PILLAR_CAVE,
               AltmillerCaveDraught,
               ALTMILLER_CAVE_DRAUGHT_FROM_ROCKS),
    SCENE_EXIT(ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_CHEST,
               AltmillerCaveDraught,
               ALTMILLER_CAVE_DRAUGHT_CHEST_FROM_ROCKS),
    SCENE_EXIT(ALTMILLER_CAVE_DRAUGHT_CHEST_TO_ROCKS,
               AltmillerCaveDraught,
               ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_CHEST),
    SCENE_EXITS_END,
};

ACTOR_PLACE AltmillerCaveActorPlacementsOther[] = {
    {SCENE_TABLE_END},
};

ACTOR_PLACE AltmillerCaveBabiActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    0,
                    0,
                    0,
                    FACING_EAST,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(ACTOR_IVAN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    0,
                    0,
                    0,
                    FACING_EAST,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(ACTOR_MIA,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    0,
                    0,
                    0,
                    FACING_EAST,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_INVISIBLE_BABI,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    232,
                    0,
                    160,
                    FACING_SOUTH,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_BABI,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    -64,
                    0,
                    -64,
                    FACING_SOUTH,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_WITH_TORCH,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    -64,
                    0,
                    -64,
                    FACING_NORTH,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_WITH_TORCH,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    -64,
                    0,
                    -64,
                    FACING_NORTH,
                    TALK_FACING_KEEP,
                    0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE AltmillerCaveSquallActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_RESET_ACTION,
                    328,
                    0,
                    256,
                    FACING_SOUTH,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_RESET_ACTION,
                    352,
                    0,
                    296,
                    FACING_SOUTH,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_RESET_ACTION,
                    368,
                    0,
                    200,
                    FACING_SOUTH,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    376,
                    0,
                    280,
                    FACING_SOUTH,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    472,
                    0,
                    328,
                    FACING_SOUTH,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI,
                    FLAG_DJINNI(DJINNI_SQUALL),
                    MOVEMENT_TYPE_STAND,
                    264,
                    0,
                    200,
                    FACING_SOUTHWEST,
                    TALK_FACING_PARTY,
                    0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE AltmillerCaveDraughtActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_RESET_ACTION,
                    392,
                    0,
                    416,
                    FACING_SOUTH,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_RESET_ACTION,
                    416,
                    0,
                    456,
                    FACING_SOUTH,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_RESET_ACTION,
                    456,
                    0,
                    480,
                    FACING_SOUTH,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_LIGHT,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    488,
                    8,
                    1752,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_LIGHT,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    472,
                    40,
                    1752,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_LIGHT,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    504,
                    56,
                    1752,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_LIGHT,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    536,
                    40,
                    1752,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_LIGHT,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    520,
                    8,
                    1752,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    440,
                    0,
                    190,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    472,
                    0,
                    174,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    504,
                    0,
                    174,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    536,
                    0,
                    174,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    568,
                    0,
                    190,
                    FACING_SOUTH,
                    TALK_FACING_PARTY,
                    0),
    {SCENE_TABLE_END},
};

// The sizes the light at the top pulses through.
Uint32 AltmillerCavePulseScales[] = {
    toFIXED(1),
    toFIXED(11) / 10,
    toFIXED(6) / 5,
    toFIXED(11) / 10,
};

// The two men from Tolbi walk in to Babi from the south.
Sint32 AltmillerCaveGuardsArrivePath[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(0, 0, -32),
    SC_SYNC,
    SC_MOVE_BY(-64, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -64),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

// Babi and the two men walk out of the room, each by his own path.
Sint32 AltmillerCaveSecondGuardLeavePath[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(16, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, 32),
    SC_SYNC,
    SC_MOVE_BY(48, 0, 0),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

Sint32 AltmillerCaveFirstGuardLeavePath[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(0, 0, 16),
    SC_SYNC,
    SC_MOVE_BY(16, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, 32),
    SC_SYNC,
    SC_MOVE_BY(32, 0, 0),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

Sint32 AltmillerCaveBabiLeavePath[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(0, 0, 32),
    SC_SYNC,
    SC_MOVE_BY(16, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, 32),
    SC_SYNC,
    SC_MOVE_BY(16, 0, 0),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

// clang-format off
CUE AltmillerCaveCuesOther[] = {
    {CUE_EXIT, ALTMILLER_CAVE_ENTRANCE_TO_WORLD_MAP, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TO_WORLD_MAP},
    {CUE_EXIT, ALTMILLER_CAVE_ENTRANCE_TO_TORCH_CHAMBER, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TO_TORCH_CHAMBER},
    {CUE_EXIT, ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_TO_PASSAGE, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_TO_PASSAGE},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, ALTMILLER_CAVE_ENTRANCE_TO_ALTMILLER_CAVE_BABI, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TO_ALTMILLER_CAVE_BABI},
    {CUE_STEP, METATILE_BEHAVIOR_ALTMILLER_CAVE_FULL_LIGHT, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveDarkenToLevel0)},
    {CUE_STEP, METATILE_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_1, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveDarkenToLevel1)},
    {CUE_STEP, METATILE_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_2, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveDarkenToLevel2)},
    {CUE_STEP, METATILE_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_3, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveDarkenToLevel3)},
    {CUE_STEP, METATILE_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_4, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveDarkenToLevel4)},
    {CUE_STEP, METATILE_BEHAVIOR_ALTMILLER_CAVE_DARKNESS_5, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveDarkenToLevel5)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS,
        SCRIPT_REF(scAltmillerCaveStartReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        SCRIPT_REF(scAltmillerCaveEndReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveStartRetreat)},
    {CUE_TABLE_END},
};

CUE AltmillerCaveBabiCues[] = {
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_ENTRANCE, CONDITION_ALWAYS, ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_ENTRANCE},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_VIAL_CHEST, CONDITION_ALWAYS, ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_VIAL_CHEST},
    {CUE_TALK, ACTOR_INVISIBLE_BABI, FLAG_MET_INVISIBLE_BABI, SCRIPT_REF(scAltmillerCaveMeetInvisibleBabi)},
    {CUE_TALK, ACTOR_INVISIBLE_BABI, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT, SCRIPT_REF(scAltmillerCaveRepeatRockOrder)},
    {CUE_TALK, ACTOR_INVISIBLE_BABI, FLAG_BABI_SAVED, SCRIPT_REF(scAltmillerCaveRestoreBabiAndInviteParty)},
    {CUE_STEP, METATILE_BEHAVIOR_ALTMILLER_CAVE_BESIDE_BABI, FLAG_BABI_CALLED_PARTY_BACK, SCRIPT_REF(scAltmillerCaveBabiCallsPartyBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_INVISIBLE_BABI, FLAG_MET_INVISIBLE_BABI,
        SCRIPT_REF(scAltmillerCaveMeetInvisibleBabi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INVISIBLE_BABI, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT, MSGNO(MsgAltmillerNeedDraught)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_INVISIBLE_BABI, FLAG_BABI_SAVED,
        SCRIPT_REF(scAltmillerCaveRestoreBabiAndInviteParty)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        SCRIPT_REF(scAltmillerCaveEndReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveStartRetreat)},
    {EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_ALTMILLER_CAVE_CHEST, FLAG_ALTMILLER_CAVE_MIMIC_BEATEN, FIND_MIMIC | 5},
    {CUE_TABLE_END},
};

CUE AltmillerCaveVialChestCues[] = {
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_BABI, CONDITION_ALWAYS, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_BABI},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_SQUALL, CONDITION_ALWAYS, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_SQUALL},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        SCRIPT_REF(scAltmillerCaveEndReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveStartRetreat)},
    {EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_ALTMILLER_CAVE_CHEST, FLAG_ALTMILLER_CAVE_VIAL, FIND_ITEM(ITEM_VIAL)},
    {CUE_TABLE_END},
};

CUE AltmillerCaveSquallCues[] = {
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_VIAL_CHEST, CONDITION_ALWAYS, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_VIAL_CHEST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_COOKIE_CHEST, CONDITION_ALWAYS, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_COOKIE_CHEST},
    {STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_ALTMILLER_CAVE_NORTH_OF_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, SCRIPT_REF(scAltmillerCaveHopOverPuddle)},
    {STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_ALTMILLER_CAVE_EAST_OF_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, SCRIPT_REF(scAltmillerCaveHopOverPuddle)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_ALTMILLER_CAVE_SOUTH_OF_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, SCRIPT_REF(scAltmillerCaveHopOverPuddle)},
    {STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_ALTMILLER_CAVE_WEST_OF_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, SCRIPT_REF(scAltmillerCaveHopOverPuddle)},
    {CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_ALTMILLER_CAVE_SQUALL_COLUMN_FLOOR, CONDITION_ALWAYS, SCRIPT_REF(scPushedActorPushIfClear)},
    {STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_ALTMILLER_CAVE_WEST_OF_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCavePushPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SQUALL_ROOM_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveDropPillarIntoPit)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SQUALL_ROOM_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, SCRIPT_REF(scAltmillerCaveCheckPuddleCovered)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SQUALL_ROOM_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, SCRIPT_REF(scAltmillerCaveFreezePuddle)},
    {CUE_TALK, ACTOR_SQUALL, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveStartDjinnBattle)},
    {CUE_TABLE_END},
};

CUE AltmillerCaveCookieChestCues[] = {
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_SQUALL, CONDITION_ALWAYS, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_SQUALL},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_DRAUGHT, CONDITION_ALWAYS, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_DRAUGHT},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        SCRIPT_REF(scAltmillerCaveEndReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveStartRetreat)},
    {EXAMINE_CUE(EXAMINE_CHEST), METATILE_BEHAVIOR_ALTMILLER_CAVE_CHEST, FLAG_ALTMILLER_CAVE_COOKIE, FIND_ITEM(ITEM_COOKIE)},
    {CUE_TABLE_END},
};
// clang-format on

CUE AltmillerCaveDraughtCues[] = {
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP,
     ALTMILLER_CAVE_DRAUGHT_TO_ALTMILLER_CAVE_COOKIE_CHEST, CONDITION_ALWAYS,
     ALTMILLER_CAVE_DRAUGHT_TO_ALTMILLER_CAVE_COOKIE_CHEST},
    {CUE_EXIT, ALTMILLER_CAVE_DRAUGHT_TO_ROCKS, CONDITION_ALWAYS, ALTMILLER_CAVE_DRAUGHT_TO_ROCKS},
    {CUE_EXIT,
     ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_PILLAR_CAVE, CONDITION_ALWAYS,
     ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_PILLAR_CAVE},
    {CUE_EXIT,
     ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_CHEST, CONDITION_ALWAYS,
     ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_CHEST},
    {CUE_EXIT,
     ALTMILLER_CAVE_DRAUGHT_CHEST_TO_ROCKS, CONDITION_ALWAYS,
     ALTMILLER_CAVE_DRAUGHT_CHEST_TO_ROCKS},
    {CUE_STEP | STEP_CUE_FLAG_WALKING,
     METATILE_BEHAVIOR_ALTMILLER_CAVE_DRAUGHT_COLUMN_FLOOR, CONDITION_ALWAYS,
     SCRIPT_REF(scPushedActorPushIfClear)},
    {CUE_TALK, ACTOR_BLUE_ROCK, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveTurnRock)},
    {CUE_TALK, ACTOR_GREEN_ROCK, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveTurnRock)},
    {CUE_TALK, ACTOR_WHITE_ROCK, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveTurnRock)},
    {CUE_TALK, ACTOR_YELLOW_ROCK, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveTurnRock)},
    {CUE_TALK, ACTOR_RED_ROCK, CONDITION_ALWAYS, SCRIPT_REF(scAltmillerCaveTurnRock)},
    {EXAMINE_CUE(EXAMINE_CHEST),
     METATILE_BEHAVIOR_ALTMILLER_CAVE_CHEST, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT,
     FIND_ITEM(ITEM_MYSTIC_DRAUGHT)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scAltmillerCaveStartReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scAltmillerCaveEndReveal)},
    {CUE_TABLE_END},
};

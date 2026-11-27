// The Mars Lighthouse's central hall, the four ledges in the rain that lead from it to the element
// towers, and the beacon ledge at the top of each tower, all sharing this overlay.
//
// Felix reaches the hall up the stairs from the lighthouse's entrance. The great dragon statue at
// its head speaks as he steps in: without the Mars Star it tells him to begone; with it, once the
// lighthouse has thawed, it bids him set the four spirits aflame. Four inscriptions hang beside the
// statue, two on the floor by its chin and two on the gallery by its head, naming the peoples who
// ruled earth, water, fire and wind. Each tower comes out on a beacon ledge in the rain, where
// Blaze at the torch lights that element's flame. Each flame lit burns on one of the hall's four
// robed statues and makes its element's inscription glow. With all four lit the statue says the
// heavens await, a fireball from its mouth breaks the floor before it to show a round carving, and
// Teleport there carries the party up to the aerie. Once the lighthouse has thawed, the gallery's
// arched doorways light purple as Felix walks up to them.

#include "graphics/palette_buffers.h"
#include "item_ids.h"
#include "scene/flame_throwers.h"
#include "field/field_mode.h"
#include "field/field_mode_script.h"
#include "scene/scene.h"
#include "field/script_inline.h"
#include "field/call.h"
#include "edition.h"
#include "shared/script_check_spheres_overlap.h"
#include "window/window.h"
#include "field/field_sprite.h"
#include "macros.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

// All four flames lit: the floor before the statue has broken open.
#define FLAG_WAY_TO_AERIE_OPEN    0xa31
#define FLAG_VENUS_FLAME_LIT      0xa32
#define FLAG_MERCURY_FLAME_LIT    0xa33
#define FLAG_MARS_FLAME_LIT       0xa34
#define FLAG_JUPITER_FLAME_LIT    0xa35
// "If you wish to scale the heavens, set the four spirits aflame!"
#define FLAG_TOLD_TO_LIGHT_FLAMES 0xa2e
// "If you do not hold the star, you shall not ascend to the heavens... Begone!"
#define FLAG_TOLD_TO_BEGONE       0xa2f

// The hall's own entrances, through its four doorways: arriving from the passage toward each
// element's tower, the party comes in under that doorway's arch with its light shown.
enum
{
    ENTRANCE_THROUGH_VENUS_DOORWAY   = 8,
    ENTRANCE_THROUGH_MERCURY_DOORWAY = 10,
    ENTRANCE_THROUGH_MARS_DOORWAY    = 12,
    ENTRANCE_THROUGH_JUPITER_DOORWAY = 14
};

// The step metatiles before the hall's doorways, two to each in the doorways' order: the metatile
// under the arch, which fades its light in, then the metatiles beside it, which fade it out.
enum
{
    METATILE_BEHAVIOR_VENUS_DOORWAY   = 20,
    METATILE_BEHAVIOR_MERCURY_DOORWAY = 22,
    METATILE_BEHAVIOR_MARS_DOORWAY    = 24,
    METATILE_BEHAVIOR_JUPITER_DOORWAY = 26
};

// The hall's doorways, in the order of their step metatiles (MarsLighthouseHallDoorway).
enum
{
    DOORWAY_VENUS,
    DOORWAY_MERCURY,
    DOORWAY_MARS,
    DOORWAY_JUPITER
};

// The hall's other metatiles: the carving Teleport is cast on, the four inscriptions, and the steps
// up from the stairs where the dragon statue speaks.
enum
{
    METATILE_AERIE_CARVING     = 1,
    METATILE_EARTH_INSCRIPTION = 30,
    METATILE_WATER_INSCRIPTION,
    METATILE_FIRE_INSCRIPTION,
    METATILE_WIND_INSCRIPTION,
    METATILE_BEFORE_DRAGON_STATUE
};

// The beacon ledge's doorway back into its tower.
#define METATILE_LEDGE_DOORWAY      1

// The inscriptions' colours: BG palettes 8 and 9 hold six for each element, the hall's four
// inscriptions glowing in them once that element's flame is lit, and the ledge's inscription takes
// its tower's six into palette 13.
#define INSCRIPTION_FIRST_PALETTE   8
#define LEDGE_INSCRIPTION_PALETTE   13
// The hall's palette whose last six colours are an unlit inscription's.
#define UNLIT_INSCRIPTION_PALETTE   2
// An inscription's glow: a run of INSCRIPTION_GLOW_COLORS colours that turns round, one colour on
// every INSCRIPTION_GLOW_STEP_TICKS ticks.
#define INSCRIPTION_GLOW_COLORS     6
#define INSCRIPTION_GLOW_STEP_TICKS 8

// The first of each element's six colours in palettes 8 and 9, and of the ledge inscription's in
// palette 13.
enum
{
    JUPITER_GLOW = 4,
    VENUS_GLOW   = 10,
    MERCURY_GLOW = PALETTE_COLORS + 4,
    MARS_GLOW    = PALETTE_COLORS + 10,
    LEDGE_GLOW   = 10
};

// Each element's flame among the flame sprite's animations.
enum
{
    FLAME_ANIMATION_MARS    = 2,
    FLAME_ANIMATION_JUPITER = 3,
    FLAME_ANIMATION_VENUS   = 4,
    FLAME_ANIMATION_MERCURY = 5
};

// The beacon ledge: the torch Blaze is cast at, tinted to its tower's element, its flame, the point
// above it Blaze reaches, and two dragon statues.
enum
{
    ACTOR_LEDGE_TORCH        = 8,
    ACTOR_LEDGE_FLAME        = 9,
    ACTOR_LEDGE_BLAZE_TARGET = 10
};

// The hall: four robed statues (actors 8 to 11), the four elements' flames on them, shown once lit,
// and the dragon statue's mouth, which lets fall the fireball that breaks the floor.
enum
{
    ACTOR_VENUS_FLAME   = 12,
    ACTOR_MERCURY_FLAME = 13,
    ACTOR_MARS_FLAME    = 14,
    ACTOR_JUPITER_FLAME = 15,
    ACTOR_STATUE_MOUTH  = 16,
    ACTOR_FIREBALL      = 18
};

struct ACTOR;
struct VECTOR3;

Sint32 fdLeaderKnockBack(void);

Sint32 scFireballCarryLeader(void);
void   fdActorLaunchAhead(ACTOR *self);

void   scTeleportStartShimmer(void);

void   scMarsLighthouseHallFadeDoorway(void);

void   scMarsLighthouseHallBreakFloorTile(
      Sint32 x, Sint32 z, Sint32 srcX, Sint32 srcY, Sint32 metatileX, Sint32 metatileY);

void          scMarsLighthouseHallCycleLedgeInscription(void);

void          scMarsLighthouseHallCycleHallInscriptions(void);

void          scMarsLighthouseHallStartDoorways(Sint32 entrance);

void          scFlameThrowersCreate(void);

void          scMarsLighthouseHallFlashDoorways(void);

void          fdActorBlink(ACTOR *actor);

extern Uint32 CrumbsScript[];
extern Uint32 IceChipsScript[];

extern Sint32 MarsLighthouseHallTorchScript[];

void          fdTeleportPlaySparks(void);

// The colours of BG palettes 8 and 9.
typedef struct HALL_COL
{
    Uint16 colors[2 * PALETTE_COLORS];
} HALL_COL;

// The teleport circle's shimmer, a palette cycle that fdPaletteCycleFade steps.
Uint16 PaletteCycleBright         BSS_DATA;
Uint16 *PaletteCycleSlots         BSS_DATA;
Uint16 PaletteCycleTimer          BSS_DATA;
Uint16 PaletteCycleDelay BSS_DATA ALIGNED(4);
Uint16 *PaletteSlots              BSS_DATA;
Uint16 *PaletteColors             BSS_DATA;
Uint16 *PaletteCycleColors        BSS_DATA;
Uint16 PaletteCycleLength         BSS_DATA;
Uint16 PaletteCycleIndex BSS_DATA ALIGNED(4);
// The beacon ledge inscription's colours: the element palettes as loaded, the palette the glow is
// built in, and the inscription's own palette as loaded.
Uint16 MarsLighthouseHallLedgeElementColors[2 * PALETTE_COLORS] BSS_DATA ALIGNED(4);
Uint16 MarsLighthouseHallLedgePalette[2 * PALETTE_COLORS] BSS_DATA;
Uint16 MarsLighthouseHallLedgeSavedPalette[PALETTE_COLORS] BSS_DATA;
HALL_COL MarsLighthouseHallHallElementColors BSS_DATA;
HALL_COL MarsLighthouseHallHallPalette       BSS_DATA;
// BG palette 2's first ten colours; it is saved whole, with the unlit inscription colours after
// them.
Uint16                                       MarsLighthouseHallHallSavedPalette[10] BSS_DATA;
Uint16                                       MarsLighthouseHallUnlitInscriptionColors[6] BSS_DATA;
Sint16 MarsLighthouseHallDoorwayBlend        BSS_DATA;
Sint16 MarsLighthouseHallDoorwayBlendTarget  BSS_DATA;
Sint16 MarsLighthouseHallDoorway             BSS_DATA;
Sint16 MarsLighthouseHallDoorwayStep         BSS_DATA;

const SCENE_ENTRANCE                        *scGetEntrances(void)
{
    return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
    return 0;
}

const Uint32 *scGetExits(void)
{
    return SceneExits;
}

void   scTeleportStopShimmer(void);

// A script call of the beacon ledge's torch and of Blaze's mark above it: they stand without a
// shadow.
Sint32 scMarsLighthouseHallSetUpTorch(ACTOR *actor)
{
    fdActorSetSpriteFlags(actor, 0);
    return 0;
}

// Shows a flame on its torch, 14 pixels up, in its element's animation.
Sint32 scMarsLighthouseHallShowFlame(Sint32 id, Sint32 animation)
{
    ACTOR *actor       = fdActorGet(id);

    actor->motionFlags = 0;
    actor->y.fixed     = toFIXED(14);
    fdActorSetSpriteFlags(actor, 0);
    fdActorSelectAnimation(actor, animation);
#if FIX_FUNCTION_SIGNATURES
    return 0;
#endif
}

void               scFlameUpdateForward(ACTOR *actor);
void               scActorSetupScriptedPart(ACTOR *actor, Sint32 layer);
void               scFlameUpdateSideways(ACTOR *actor);

extern Sint32      ScriptedPartScript[];
extern ACTOR_PLACE MarsLighthouseHallPlacements[];
extern ACTOR_PLACE MarsLighthouseHallFlamePlacements[];
extern ACTOR_PLACE MarsLighthouseHallInscriptionsPlacements[];

extern CUE         MarsLighthouseHallCues[];
extern CUE         MarsLighthouseHallFlameCues[];
extern CUE         MarsLighthouseHallInscriptionsCues[];
extern CUE         MarsLighthouseHallLedgesCues[];
extern Sint32      MarsLighthouseHallFlamePuffSwellScript[];
extern Sint32      MarsLighthouseHallFlamePuffThrownScript[];

Sint32             syListCountEntries(Uint16 *list);
void               fdPaletteCycleFade(void);
void               scTeleportStopShimmer(void);
void               scTeleportStartFastShimmer(void);
Bool               scActorIsAbove(ACTOR *a, ACTOR *b);
void               scFlameThrowerUpdateSpark(ACTOR *actor);
void               scActorSetupScripted(ACTOR *actor);
void               scMarsLighthouseHallSpawnFlamePuff(Sint32 x, Sint32 z, Sint32 kind);
void               scActorHoldDuringScript(ACTOR *actor);
void               fdTeleportPlayWhirl(void);

const ACTOR_PLACE *scGetActorPlacements(void)
{
    Sint16 scene = MainRoutineStateWork.next;

    if (scene == SCENE_MARS_LIGHTHOUSE_BEACON_LEDGE)
        return MarsLighthouseHallFlamePlacements;
    if (scene == SCENE_MARS_LIGHTHOUSE_HALL)
        return MarsLighthouseHallInscriptionsPlacements;
    return MarsLighthouseHallPlacements;
}

extern Sint32 MarsLighthouseHallLedgeCycleTick;

// Each tick on the beacon ledge: once its flame is lit, the inscription on the wall behind it glows
// in its element's colours, turning round.
void          scMarsLighthouseHallCycleLedgeInscription(void)
{
    Sint32       entrance = MainRoutineStateWork.entrance;
    FIELD_STATE *state    = RamHeapSlots->fieldState;
    Sint32       base;
    Sint32       i;
    Sint8        busy;

    if (state->psynergy != 0)
        return;
    busy = state->revealActive;
    if (busy != 0)
        return;
    if (MarsLighthouseHallLedgeCycleTick == -1)
    {
        syWordsCopyEntry(MarsLighthouseHallLedgeSavedPalette,
                         (void *)&BG_PLTT_COLOR(LEDGE_INSCRIPTION_PALETTE, 0),
                         sizeof(MarsLighthouseHallLedgeSavedPalette));
        syWordsCopyEntry(MarsLighthouseHallLedgeElementColors,
                         (void *)&BG_PLTT_COLOR(INSCRIPTION_FIRST_PALETTE, 0),
                         sizeof(MarsLighthouseHallLedgeElementColors));
        syWordsCopyEntry(MarsLighthouseHallLedgePalette,
                         MarsLighthouseHallLedgeElementColors,
                         sizeof(MarsLighthouseHallLedgeElementColors));
        MarsLighthouseHallLedgeCycleTick = 0;
    }
    base = MarsLighthouseHallLedgeCycleTick / INSCRIPTION_GLOW_STEP_TICKS % INSCRIPTION_GLOW_COLORS;
    syWordsCopyEntry(MarsLighthouseHallLedgePalette,
                     MarsLighthouseHallLedgeSavedPalette,
                     sizeof(MarsLighthouseHallLedgeSavedPalette));
    if (entrance == MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_VENUS_TOWER
        && fdScriptGetFlag(FLAG_VENUS_FLAME_LIT))
    {
        for (i = 0; i < INSCRIPTION_GLOW_COLORS; i++)
            MarsLighthouseHallLedgePalette[LEDGE_GLOW + i] =
                MarsLighthouseHallLedgeElementColors[VENUS_GLOW
                                                     + (base + i) % INSCRIPTION_GLOW_COLORS];
    }
    if (entrance == MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_MERCURY_TOWER
        && fdScriptGetFlag(FLAG_MERCURY_FLAME_LIT))
    {
        for (i = 0; i < INSCRIPTION_GLOW_COLORS; i++)
            MarsLighthouseHallLedgePalette[LEDGE_GLOW + i] =
                MarsLighthouseHallLedgeElementColors[MERCURY_GLOW
                                                     + (base + i) % INSCRIPTION_GLOW_COLORS];
    }
    if (entrance == MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_MARS_TOWER
        && fdScriptGetFlag(FLAG_MARS_FLAME_LIT))
    {
        for (i = 0; i < INSCRIPTION_GLOW_COLORS; i++)
            MarsLighthouseHallLedgePalette[LEDGE_GLOW + i] =
                MarsLighthouseHallLedgeElementColors[MARS_GLOW
                                                     + (base + i) % INSCRIPTION_GLOW_COLORS];
    }
    if (entrance == MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_JUPITER_TOWER
        && fdScriptGetFlag(FLAG_JUPITER_FLAME_LIT))
    {
        for (i = 0; i < INSCRIPTION_GLOW_COLORS; i++)
            MarsLighthouseHallLedgePalette[LEDGE_GLOW + i] =
                MarsLighthouseHallLedgeElementColors[JUPITER_GLOW
                                                     + (base + i) % INSCRIPTION_GLOW_COLORS];
    }
    IO_WRITE_QUEUE_PUSH((Uint32)&BG_PLTT_COLOR(LEDGE_INSCRIPTION_PALETTE, 0),
                        (Uint32)MarsLighthouseHallLedgePalette,
                        DMA_COPY_WORDS(8));
    MarsLighthouseHallLedgeCycleTick++;
}

// Walking back into the doorway at the top of the beacon ledge's steps (metatile (31, 17)) takes
// Felix back into the tower he came up through.
void scMarsLighthouseHallLeaveLedge(void)
{
    fdScriptPlaySound(SE_MAP_EXIT);
    syFieldScriptSetExitRequest(MainRoutineStateWork.entrance);
}

// Nothing visible happens yet when Felix casts Blaze at the torch on the beacon ledge; the game
// only marks this cast's jet of flame as the long one, so that it carries past the torch up to the
// beacon flame.
void scMarsLighthouseHallStartBlazeOnTorch(void)
{
    FIELD_STATE *event    = RamHeapSlots->fieldState;

    event->blazeLongSpray = 1;
}

// As Felix's Blaze on the beacon ledge's torch flies on past it, the camera follows the fire up to
// the unlit beacon.
void scMarsLighthouseHallWatchBlazeReachFlame(void)
{
    fdCameraMoveToActorById(ACTOR_LEDGE_BLAZE_TARGET, 1);
    fdCameraWaitForArrival();
}

// Blaze reaches the beacon above the ledge's torch: the screen flashes white with a chime, and the
// beacon of the tower Felix came up through is lit.
void scMarsLighthouseHallLightFlame(void)
{
    FIELD_STATE *state;

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
    fdPaletteFadeInterpolate(30);
    syFieldScriptWait(30);
    fdScriptPlaySound(SE_WARP);
    switch (MainRoutineStateWork.entrance)
    {
        case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_VENUS_TOWER:
            fdScriptSetFlag(FLAG_VENUS_FLAME_LIT);
            break;
        case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_MERCURY_TOWER:
            fdScriptSetFlag(FLAG_MERCURY_FLAME_LIT);
            break;
        case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_MARS_TOWER:
            fdScriptSetFlag(FLAG_MARS_FLAME_LIT);
            break;
        case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_JUPITER_TOWER:
            fdScriptSetFlag(FLAG_JUPITER_FLAME_LIT);
            break;
    }
    state           = RamHeapSlots->fieldState;
    state->psynergy = 0;
    syFieldScriptWait(30);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    fdPaletteFadeInterpolate(30);
    syFieldScriptWait(30);
    syFieldScriptWait(10);
    fdScriptEnd();
}

extern Sint32 MarsLighthouseHallHallCycleTick;

// Each tick in the hall: the inscription of each element whose flame is lit glows in its colours,
// turning round, and the others show unlit.
void          scMarsLighthouseHallCycleHallInscriptions(void)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;
    Sint32       base;
    Sint32       i;
    Sint8        busy;

    if (state->psynergy != 0)
        return;
    busy = state->revealActive;
    if (busy != 0)
        return;
    if (MarsLighthouseHallHallCycleTick == -1)
    {
        syWordsCopyEntry(MarsLighthouseHallHallSavedPalette,
                         (void *)&BG_PLTT_COLOR(UNLIT_INSCRIPTION_PALETTE, 0),
                         PALETTE_COLORS * 2);
        syWordsCopyEntry(MarsLighthouseHallHallElementColors.colors,
                         (void *)&BG_PLTT_COLOR(INSCRIPTION_FIRST_PALETTE, 0),
                         sizeof(MarsLighthouseHallHallElementColors));
        syWordsCopyEntry(MarsLighthouseHallHallPalette.colors,
                         MarsLighthouseHallHallElementColors.colors,
                         sizeof(MarsLighthouseHallHallElementColors));
        MarsLighthouseHallHallCycleTick = 0;
    }
    base = MarsLighthouseHallHallCycleTick / INSCRIPTION_GLOW_STEP_TICKS % INSCRIPTION_GLOW_COLORS;
    for (i = 0; i < INSCRIPTION_GLOW_COLORS; i++)
    {
        MarsLighthouseHallHallPalette.colors[JUPITER_GLOW + i] =
            MarsLighthouseHallHallElementColors
                .colors[JUPITER_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
        MarsLighthouseHallHallPalette.colors[VENUS_GLOW + i] =
            MarsLighthouseHallHallElementColors
                .colors[VENUS_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
        MarsLighthouseHallHallPalette.colors[MERCURY_GLOW + i] =
            MarsLighthouseHallHallElementColors
                .colors[MERCURY_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
        MarsLighthouseHallHallPalette.colors[MARS_GLOW + i] =
            MarsLighthouseHallHallElementColors
                .colors[MARS_GLOW + (base + i) % INSCRIPTION_GLOW_COLORS];
    }
    if (!fdScriptGetFlag(FLAG_VENUS_FLAME_LIT))
        syWordsCopyEntry(&MarsLighthouseHallHallPalette.colors[VENUS_GLOW],
                         MarsLighthouseHallUnlitInscriptionColors,
                         sizeof(MarsLighthouseHallUnlitInscriptionColors));
    if (!fdScriptGetFlag(FLAG_MERCURY_FLAME_LIT))
        syWordsCopyEntry(&MarsLighthouseHallHallPalette.colors[MERCURY_GLOW],
                         MarsLighthouseHallUnlitInscriptionColors,
                         sizeof(MarsLighthouseHallUnlitInscriptionColors));
    if (!fdScriptGetFlag(FLAG_MARS_FLAME_LIT))
        syWordsCopyEntry(&MarsLighthouseHallHallPalette.colors[MARS_GLOW],
                         MarsLighthouseHallUnlitInscriptionColors,
                         sizeof(MarsLighthouseHallUnlitInscriptionColors));
    if (!fdScriptGetFlag(FLAG_JUPITER_FLAME_LIT))
        syWordsCopyEntry(&MarsLighthouseHallHallPalette.colors[JUPITER_GLOW],
                         MarsLighthouseHallUnlitInscriptionColors,
                         sizeof(MarsLighthouseHallUnlitInscriptionColors));
    IO_WRITE_QUEUE_PUSH((Uint32)&BG_PLTT_COLOR(INSCRIPTION_FIRST_PALETTE, 0),
                        (Uint32)MarsLighthouseHallHallPalette.colors,
                        DMA_COPY_WORDS(16));
    MarsLighthouseHallHallCycleTick++;
}

// Each tick, the purple light in the arched doorway Felix last stepped up to fades one step in or
// out. Fully lit, the doorway opens a way through the wall (the metatiles under its arch are copied
// from the open template); faded out, the wall closes again.
void scMarsLighthouseHallFadeDoorway(void)
{
    Sint32 step;

    if (MarsLighthouseHallDoorwayBlend == MarsLighthouseHallDoorwayBlendTarget)
        return;
    step = MarsLighthouseHallDoorwayBlendTarget - MarsLighthouseHallDoorwayBlend;
    if (step < 0)
        step = -1;
    else
        step = 1;
    MarsLighthouseHallDoorwayBlend += step;
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA,
                        BLEND_ALPHA(MarsLighthouseHallDoorwayBlend,
                                    BLEND_WEIGHT_MAX - MarsLighthouseHallDoorwayBlend),
                        IO_WRITE_HALFWORD);
    if (MarsLighthouseHallDoorwayBlend == 0)
    {
        if (MarsLighthouseHallDoorway == DOORWAY_VENUS)
            fdMapGridCopyMetatileAttributes(34, 8, 3, 3, 17, 8);
        if (MarsLighthouseHallDoorway == DOORWAY_MERCURY)
            fdMapGridCopyMetatileAttributes(30, 8, 3, 3, 8, 8);
        if (MarsLighthouseHallDoorway == DOORWAY_MARS)
            fdMapGridCopyMetatileAttributes(34, 8, 3, 3, 17, 8);
        if (MarsLighthouseHallDoorway == DOORWAY_JUPITER)
            fdMapGridCopyMetatileAttributes(30, 8, 3, 3, 8, 8);
    }
    if (MarsLighthouseHallDoorwayBlend == BLEND_WEIGHT_MAX)
    {
        if (MarsLighthouseHallDoorway == DOORWAY_VENUS)
            fdMapGridCopyMetatileAttributes(34, 0, 3, 3, 17, 8);
        if (MarsLighthouseHallDoorway == DOORWAY_MERCURY)
            fdMapGridCopyMetatileAttributes(30, 4, 3, 3, 8, 8);
        if (MarsLighthouseHallDoorway == DOORWAY_MARS)
            fdMapGridCopyMetatileAttributes(34, 4, 3, 3, 17, 8);
        if (MarsLighthouseHallDoorway == DOORWAY_JUPITER)
            fdMapGridCopyMetatileAttributes(30, 0, 3, 3, 8, 8);
    }
}

// Once the lighthouse has thawed, stepping under one of the hall's four arched doorways, two each
// side of the dragon statue, lights its opening purple with a hum, and stepping to the metatiles
// beside it fades it out again. The arch under Venus's doorway is metatile (18, 10), Mercury's (9,
// 11), Mars's (18, 11) and Jupiter's (9, 10); the last doorway lit is put away first.
void scMarsLighthouseHallStepTowardDoorway(Sint32 state)
{
    Sint32  next;
    Sint32  odd;
    Uint16 *flash;

    if (state == MarsLighthouseHallDoorwayStep)
        return;
    if ((state ^ MarsLighthouseHallDoorwayStep) & 1)
        fdScriptPlaySound(SE_MARS_LIGHTHOUSE_DOORWAY);
    MarsLighthouseHallDoorwayStep = state;
    odd                           = state & 1;
    flash                         = (Uint16 *)&MarsLighthouseHallDoorwayBlendTarget;
    next                          = state - METATILE_BEHAVIOR_VENUS_DOORWAY;
    if (odd)
        *flash = 0;
    else
        *flash = BLEND_WEIGHT_MAX;
    if (MarsLighthouseHallDoorway == DOORWAY_VENUS)
        fdMapGridCopyMetatilesLayered(0, 122, 3, 3, 17, 67);
    if (MarsLighthouseHallDoorway == DOORWAY_MERCURY)
        fdMapGridCopyMetatilesLayered(0, 122, 3, 3, 8, 72);
    if (MarsLighthouseHallDoorway == DOORWAY_MARS)
        fdMapGridCopyMetatilesLayered(0, 122, 3, 3, 17, 72);
    if (MarsLighthouseHallDoorway == DOORWAY_JUPITER)
        fdMapGridCopyMetatilesLayered(0, 122, 3, 3, 8, 67);
    MarsLighthouseHallDoorway = next / 2;
    if (MarsLighthouseHallDoorway == DOORWAY_VENUS)
        fdMapGridCopyMetatilesLayered(0, 125, 3, 3, 17, 67);
    if (MarsLighthouseHallDoorway == DOORWAY_MERCURY)
        fdMapGridCopyMetatilesLayered(0, 125, 3, 3, 8, 72);
    if (MarsLighthouseHallDoorway == DOORWAY_MARS)
        fdMapGridCopyMetatilesLayered(0, 125, 3, 3, 17, 72);
    if (MarsLighthouseHallDoorway == DOORWAY_JUPITER)
        fdMapGridCopyMetatilesLayered(0, 125, 3, 3, 8, 67);
}

// As the dragon statue speaks, the four arched doorways light up purple together, flicker and fade
// away.
void scMarsLighthouseHallFlashDoorways(void)
{
    fdMapGridCopyMetatilesLayered(0, 125, 3, 3, 17, 67);
    fdMapGridCopyMetatilesLayered(0, 125, 3, 3, 8, 72);
    fdMapGridCopyMetatilesLayered(0, 125, 3, 3, 17, 72);
    fdMapGridCopyMetatilesLayered(0, 125, 3, 3, 8, 67);
    syWait(1);
    MarsLighthouseHallDoorwayBlendTarget = BLEND_WEIGHT_MAX * 3 / 4;
    syWait(12);
    MarsLighthouseHallDoorwayBlendTarget = BLEND_WEIGHT_MAX / 2;
    syWait(4);
    MarsLighthouseHallDoorwayBlendTarget = BLEND_WEIGHT_MAX * 3 / 4;
    syWait(4);
    MarsLighthouseHallDoorwayBlendTarget = BLEND_WEIGHT_MAX / 2;
    syWait(4);
    MarsLighthouseHallDoorwayBlendTarget = BLEND_WEIGHT_MAX * 3 / 4;
    syWait(4);
    MarsLighthouseHallDoorwayBlendTarget = 0;
    syWait(12);
    fdMapGridCopyMetatilesLayered(0, 122, 3, 3, 17, 67);
    fdMapGridCopyMetatilesLayered(0, 122, 3, 3, 8, 72);
    fdMapGridCopyMetatilesLayered(0, 122, 3, 3, 17, 72);
    fdMapGridCopyMetatilesLayered(0, 122, 3, 3, 8, 67);
    syWait(1);
}

// On arriving in the hall through one of its arched doorways, that doorway is shown lit purple
// behind Felix (one step short of full, so the next fade opens its way); arriving any other way,
// all four are dark.
void scMarsLighthouseHallStartDoorways(Sint32 entrance)
{
    Sint32  metatile = -1;
    Sint16 *blend;
    fdBlendScriptSuspend();
    blend = &MarsLighthouseHallDoorwayBlend;
    if (entrance == ENTRANCE_THROUGH_VENUS_DOORWAY)
        metatile = METATILE_BEHAVIOR_VENUS_DOORWAY;
    if (entrance == ENTRANCE_THROUGH_MERCURY_DOORWAY)
        metatile = METATILE_BEHAVIOR_MERCURY_DOORWAY;
    if (entrance == ENTRANCE_THROUGH_MARS_DOORWAY)
        metatile = METATILE_BEHAVIOR_MARS_DOORWAY;
    if (entrance == ENTRANCE_THROUGH_JUPITER_DOORWAY)
        metatile = METATILE_BEHAVIOR_JUPITER_DOORWAY;
    if (metatile == -1)
    {
        *blend                               = 0;
        MarsLighthouseHallDoorwayBlendTarget = 0;
        MarsLighthouseHallDoorway            = metatile;
        MarsLighthouseHallDoorwayStep        = metatile;
    }
    else
    {
        Sint32 step;
        *blend                               = BLEND_WEIGHT_MAX - 1;
        MarsLighthouseHallDoorwayBlendTarget = BLEND_WEIGHT_MAX;
        step                                 = (metatile - METATILE_BEHAVIOR_VENUS_DOORWAY) / 2;
        MarsLighthouseHallDoorway            = step;
        MarsLighthouseHallDoorwayStep        = metatile;
        if (step == DOORWAY_VENUS)
            fdMapGridCopyMetatilesLayered(0, 125, 3, 3, 17, 67);
        if (MarsLighthouseHallDoorway == DOORWAY_MERCURY)
            fdMapGridCopyMetatilesLayered(0, 125, 3, 3, 8, 72);
        if (MarsLighthouseHallDoorway == DOORWAY_MARS)
            fdMapGridCopyMetatilesLayered(0, 125, 3, 3, 17, 72);
        if (MarsLighthouseHallDoorway == DOORWAY_JUPITER)
            fdMapGridCopyMetatilesLayered(0, 125, 3, 3, 8, 67);
    }
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA,
                        BLEND_ALPHA(MarsLighthouseHallDoorwayBlend,
                                    BLEND_WEIGHT_MAX - MarsLighthouseHallDoorwayBlend),
                        IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDCNT,
                        BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
                        IO_WRITE_HALFWORD);
    sySetTickEvent(scMarsLighthouseHallFadeDoorway, EVENT_PASS_TICK);
}

// Felix casts Teleport on the round carving the dragon statue's fireball uncovered in the floor
// before it (metatiles (12, 12) to (15, 14)): the screen turns red, sparks swirl round the party,
// and they arrive on the carved octagon of the Mars Lighthouse Aerie in the storm.
void scMarsLighthouseHallTeleportToAerie(void)
{
    scTeleportStartShimmer();
    fdTeleportPlaySparks();
    MainRoutineStateWork.teleportState = TELEPORT_ARRIVING;
    syFieldScriptSetExitRequest(MARS_LIGHTHOUSE_HALL_TO_MARS_LIGHTHOUSE_AERIE);
    syFieldScriptWait(40);
    scTeleportStopShimmer();
    syFieldCloseEvent(scMarsLighthouseHallFadeDoorway);
}

const CUE *scGetCues(void)
{
    Sint16 scene = MainRoutineStateWork.next;

    if (scene == SCENE_MARS_LIGHTHOUSE_BEACON_LEDGE)
        return MarsLighthouseHallFlameCues;
    if (scene == SCENE_MARS_LIGHTHOUSE_HALL)
        return MarsLighthouseHallInscriptionsCues;
    if (scene == SCENE_MARS_LIGHTHOUSE_MERCURY_LEDGE || scene == SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE
        || scene == SCENE_MARS_LIGHTHOUSE_MARS_LEDGE || scene == SCENE_MARS_LIGHTHOUSE_VENUS_LEDGE)
        return MarsLighthouseHallLedgesCues;
    return MarsLighthouseHallCues;
}

void          scPaletteSetColorsFade(Uint16 *slots, Uint16 *colours, Uint16 *a, Uint16 *b);

extern Uint16 TeleportCirclePaletteSlots[];
extern Uint16 TeleportCircleColors[];
extern Uint16 TeleportCircleShimmerColors[];
extern Uint16 TeleportCircleShimmerSlots[];

Sint32        scInit(void)
{
    Sint32 entrance;
    Sint32 scene;

    scene                      = MainRoutineStateWork.next;
    entrance                   = MainRoutineStateWork.entrance;

    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    fdScriptSetFlag(FLAG_IN_MARS_LIGHTHOUSE);
    scPaletteSetColorsFade((Sint32)TeleportCirclePaletteSlots,
                           (Sint32)TeleportCircleColors,
                           (Sint32)TeleportCircleShimmerColors,
                           (Sint32)TeleportCircleShimmerSlots);
    if (scene != SCENE_MARS_LIGHTHOUSE_BEACON_LEDGE && scene == SCENE_MARS_LIGHTHOUSE_HALL)
    {
        Call1((void (*)())fdEffectStartBlankOverlay, 0);
        FieldStateWork->transition =
            SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
        if (entrance == MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_AERIE
            || entrance == MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE
            || entrance == ENTRANCE_THROUGH_VENUS_DOORWAY
            || entrance == ENTRANCE_THROUGH_MERCURY_DOORWAY
            || entrance == ENTRANCE_THROUGH_MARS_DOORWAY
            || entrance == ENTRANCE_THROUGH_JUPITER_DOORWAY)
        {
            Sint32 unset = fdScriptGetFlag(FLAG_WAY_TO_AERIE_OPEN);

            if (unset == 0 && fdScriptGetFlag(FLAG_VENUS_FLAME_LIT)
                && fdScriptGetFlag(FLAG_MERCURY_FLAME_LIT) && fdScriptGetFlag(FLAG_MARS_FLAME_LIT)
                && fdScriptGetFlag(FLAG_JUPITER_FLAME_LIT))
            {
                ACTOR  *actor;
                Uint16 *color = (Uint16 *)&CENTERED_TEXT_COLOR;

                fdScriptStart();
                syFieldScriptEndLastingPsynergy(0);
                fdScriptStartScreenTransition();
                fdScriptWaitTransitionTicks();
                fdScriptPlaySound(SOUND_FADE_OUT_BGM);
                fdCameraSetSpeed(2);
                fdCameraMoveTo(224, 64, 216, 1);
                fdCameraWaitForArrival();
                syFieldScriptWait(20);
                fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
                fdPaletteFadeApplyTarget(PALETTE_TINT_WARM, PALETTES_ALL);
                fdPaletteFadeInterpolate(20);
                syWait(20);
                *color = RGB_WHITE;
                syWait(1);
                syScriptMessageShowCentered(
                    (Sint32)MsgMarsLighthouseHallYouHaveProvenYourWorth, 0, 4);
                fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
                fdPaletteFadeInterpolate(20);
                syWait(20);
                fdCameraSetSpeed(1);
                fdCameraMoveTo(224, 16, 216, 1);
                fdCameraWaitForArrival();
                syFieldScriptWait(40);
                // unset is zero here: the mouth stands still and watches nothing.
                actor                  = fdActorGet(ACTOR_STATUE_MOUTH);
                actor->motionFlags     = unset;
                actor->y.fixed         = toFIXED(16);
                actor->groundHeight    = toFIXED(16);
                actor->launchedActorId = ACTOR_FIREBALL;
                actor->userValue       = FACING_SOUTH;
                actor->watchedActor    = (union FIELD_OBJECT *)unset;
                fdActorLaunchAhead(actor);
                syWait(40);
                fdMapGridSetShake(2, 2, 1);
                scMarsLighthouseHallBreakFloorTile(216, 216, 65, 65, 13, 13);
                scMarsLighthouseHallBreakFloorTile(232, 216, 66, 65, 14, 13);
                scMarsLighthouseHallBreakFloorTile(232, 232, 66, 66, 14, 14);
                scMarsLighthouseHallBreakFloorTile(216, 200, 65, 64, 13, 12);
                scMarsLighthouseHallBreakFloorTile(248, 200, 67, 64, 15, 12);
                scMarsLighthouseHallBreakFloorTile(248, 232, 67, 66, 15, 14);
                scMarsLighthouseHallBreakFloorTile(216, 232, 65, 66, 13, 14);
                scMarsLighthouseHallBreakFloorTile(200, 216, 64, 65, 12, 13);
                scMarsLighthouseHallBreakFloorTile(232, 200, 66, 64, 14, 12);
                scMarsLighthouseHallBreakFloorTile(248, 216, 67, 65, 15, 13);
                scMarsLighthouseHallBreakFloorTile(200, 232, 64, 66, 12, 14);
                scMarsLighthouseHallBreakFloorTile(200, 200, 64, 64, 12, 12);
                fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
                fdMapGridCopyMetatileAttributes(30, 12, 4, 3, 12, 12);
                syFieldScriptWait(40);
                fdScriptSetFlag(FLAG_WAY_TO_AERIE_OPEN);
                fdScriptPlaySound(MUS_PUZZLE_SOLVED);
                sySoundWaitForBGMRestore();
                gmPlayMusic();
                fdScriptEnd();
            }
        }
    }
    return 0;
}

// The line scroll's wave across and down that the rain ledges start with the rain.
static __inline__ void scMarsLighthouseHallStartRainWave(Sint32 mode,
                                                         FIXED  frequencyX,
                                                         Sint32 stepX,
                                                         FIXED  ampX,
                                                         FIXED  frequencyY,
                                                         Sint32 stepY,
                                                         FIXED  ampY)
{
    fdLineScrollInitWave(mode, frequencyX, stepX, ampX, frequencyY, stepY, ampY);
}

Sint32 scPrepareMap(void)
{
    Sint16 scene    = MainRoutineStateWork.next;
    Sint32 entrance = MainRoutineStateWork.entrance;
    ACTOR *actor;

    if (scene == SCENE_MARS_LIGHTHOUSE_BEACON_LEDGE || scene == SCENE_MARS_LIGHTHOUSE_MERCURY_LEDGE
        || scene == SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE || scene == SCENE_MARS_LIGHTHOUSE_MARS_LEDGE
        || scene == SCENE_MARS_LIGHTHOUSE_VENUS_LEDGE)
    {
        fdWeatherStartRain(1);
        scMarsLighthouseHallStartRainWave(
            1, toFIXED(1), toFIXED(4), toFIXED(2), toFIXED(1), toFIXED(1) / 2, toFIXED(1) / 4);
    }
    if (scene == SCENE_MARS_LIGHTHOUSE_BEACON_LEDGE)
    {
        switch (entrance)
        {
            case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_VENUS_TOWER:
                scFieldActorSetSpriteColor(fdActorGet(ACTOR_LEDGE_TORCH), SPRITE_COLOR_GRB);
                scMarsLighthouseHallShowFlame(ACTOR_LEDGE_FLAME, FLAME_ANIMATION_VENUS);
                fdTilesetAnimationResume(4);
                fdTilesetAnimationResume(5);
                break;
            case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_MERCURY_TOWER:
                scFieldActorSetSpriteColor(fdActorGet(ACTOR_LEDGE_TORCH), SPRITE_COLOR_BGR);
                scMarsLighthouseHallShowFlame(ACTOR_LEDGE_FLAME, FLAME_ANIMATION_MERCURY);
                fdTilesetAnimationResume(6);
                fdTilesetAnimationResume(7);
                break;
            case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_MARS_TOWER:
                scFieldActorSetSpriteColor(fdActorGet(ACTOR_LEDGE_TORCH), SPRITE_COLOR_NORMAL);
                scMarsLighthouseHallShowFlame(ACTOR_LEDGE_FLAME, FLAME_ANIMATION_MARS);
                fdTilesetAnimationResume(0);
                fdTilesetAnimationResume(1);
                break;
            case MARS_LIGHTHOUSE_BEACON_LEDGE_FROM_MARS_LIGHTHOUSE_JUPITER_TOWER:
                scFieldActorSetSpriteColor(fdActorGet(ACTOR_LEDGE_TORCH), SPRITE_COLOR_GBR);
                scMarsLighthouseHallShowFlame(ACTOR_LEDGE_FLAME, FLAME_ANIMATION_JUPITER);
                fdTilesetAnimationResume(2);
                fdTilesetAnimationResume(3);
                break;
        }
        actor              = fdActorGet(ACTOR_LEDGE_BLAZE_TARGET);
        actor->motionFlags = 0;
        actor->z.fixed     = toFIXED(104);
        syFieldSetTickEvent(scMarsLighthouseHallCycleLedgeInscription, EVENT_PASS_TICK);
    }
    else if (scene == SCENE_MARS_LIGHTHOUSE_HALL)
    {
        if (entrance == MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_AERIE
            || entrance == MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_ENTRANCE_FUGUE
            || entrance == ENTRANCE_THROUGH_VENUS_DOORWAY
            || entrance == ENTRANCE_THROUGH_MERCURY_DOORWAY
            || entrance == ENTRANCE_THROUGH_MARS_DOORWAY
            || entrance == ENTRANCE_THROUGH_JUPITER_DOORWAY)
        {
            scFlameThrowersCreate();
            if (fdScriptGetFlag(FLAG_WAY_TO_AERIE_OPEN))
            {
                fdMapGridCopyMetatileAttributes(30, 12, 4, 3, 12, 12);
                fdMapGridCopyMetatilesLayered(64, 64, 4, 3, 76, 12);
            }
            scMarsLighthouseHallStartDoorways(entrance);
            {
                Sint32 priority = EVENT_PASS_TICK;

                syFieldSetTickEvent(scMarsLighthouseHallCycleHallInscriptions, priority);
            }
            scMarsLighthouseHallShowFlame(ACTOR_VENUS_FLAME, FLAME_ANIMATION_VENUS);
            scMarsLighthouseHallShowFlame(ACTOR_MERCURY_FLAME, FLAME_ANIMATION_MERCURY);
            scMarsLighthouseHallShowFlame(ACTOR_MARS_FLAME, FLAME_ANIMATION_MARS);
            scMarsLighthouseHallShowFlame(ACTOR_JUPITER_FLAME, FLAME_ANIMATION_JUPITER);
            if (!fdScriptGetFlag(FLAG_VENUS_FLAME_LIT))
                fdActorSetPos(ACTOR_VENUS_FLAME, 0, 0);
            if (!fdScriptGetFlag(FLAG_MERCURY_FLAME_LIT))
                fdActorSetPos(ACTOR_MERCURY_FLAME, 0, 0);
            if (!fdScriptGetFlag(FLAG_MARS_FLAME_LIT))
                fdActorSetPos(ACTOR_MARS_FLAME, 0, 0);
            if (!fdScriptGetFlag(FLAG_JUPITER_FLAME_LIT))
                fdActorSetPos(ACTOR_JUPITER_FLAME, 0, 0);
        }
    }
    if (fdScriptGetFlag(FLAG_MARS_LIGHTHOUSE_THAWED))
    {
        fdMapCopyPalette(MainRoutineStateWork.mapId + MARS_LIGHTHOUSE_THAWED_MAPS, (Sint32)BG_PLTT);
    }
    return 0;
}

// Felix reads the inscription on the hall's gallery east of the dragon statue's head: the screen
// turns yellow-green and "Mankind... With the power of wisdom, they ruled the earth." appears
// across it.
void scMarsLighthouseHallReadEarthInscription(void)
{
    Uint16 *color = (Uint16 *)&CENTERED_TEXT_COLOR;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 0);
    fdPaletteFadeStartInterpolation(20);
    sySynch(20);
    *color = RGB_WHITE;
    sySynch(1);
    syScriptMessageShowCentered((Sint32)MsgMarsLighthouseHallMankindWithPowerOfWisdom, 0, 0);
    fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
    fdPaletteFadeStartInterpolation(20);
    sySynch(20);
    fdScriptEnd();
}

// Felix reads the inscription on the hall's floor west of the dragon statue's chin: the screen
// turns yellow-green and "Fish... With cold courage, they ruled the water." appears across it.
void scMarsLighthouseHallReadWaterInscription(void)
{
    Uint16 *color = (Uint16 *)&CENTERED_TEXT_COLOR;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 0);
    fdPaletteFadeStartInterpolation(20);
    sySynch(20);
    *color = RGB_WHITE;
    sySynch(1);
    syScriptMessageShowCentered((Sint32)MsgMarsLighthouseHallFishWithColdCourageThey, 0, 0);
    fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
    fdPaletteFadeStartInterpolation(20);
    sySynch(20);
    fdScriptEnd();
}

// Felix reads the inscription on the hall's floor east of the dragon statue's chin: the screen
// turns yellow-green and "Dragons... Burning with might, they ruled the fires." appears across it.
void scMarsLighthouseHallReadFireInscription(void)
{
    Uint16 *color = (Uint16 *)&CENTERED_TEXT_COLOR;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 0);
    fdPaletteFadeStartInterpolation(20);
    sySynch(20);
    *color = RGB_WHITE;
    sySynch(1);
    syScriptMessageShowCentered((Sint32)MsgMarsLighthouseHallDragonsBurningWithMightThey, 0, 0);
    fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
    fdPaletteFadeStartInterpolation(20);
    sySynch(20);
    fdScriptEnd();
}

// Felix reads the inscription on the hall's gallery west of the dragon statue's head: the screen
// turns yellow-green and "Bird... On the wings of truth, they ruled the winds." appears across it.
void scMarsLighthouseHallReadWindInscription(void)
{
    Uint16 *color = (Uint16 *)&CENTERED_TEXT_COLOR;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 0);
    fdPaletteFadeStartInterpolation(20);
    sySynch(20);
    *color = RGB_WHITE;
    sySynch(1);
    syScriptMessageShowCentered((Sint32)MsgMarsLighthouseHallBirdOnWingsOfTruth, 0, 0);
    fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
    fdPaletteFadeStartInterpolation(20);
    sySynch(20);
    fdScriptEnd();
}

// A puff of flame where the fireball breaks the floor: one that swells as it rises and shrinks
// back, or one thrown off at random.
void scMarsLighthouseHallSpawnFlamePuff(Sint32 x, Sint32 z, Sint32 kind)
{
    ACTOR *actor;

    actor = scFieldActorCreate(SPRITE_FLAME_PUFF, toFIXED(x), 0, toFIXED(z));
    if (actor != 0)
    {
        fdActorSetSpriteFlags(actor, 0);
        fdActorSelectAnimation(actor, 3);
        if (kind != 0)
        {
            ((PLACED_SPR *)actor->sprite)->priority = 2;
            fdActorStartScript(actor, MarsLighthouseHallFlamePuffSwellScript);
        }
        else
        {
            ((PLACED_SPR *)actor->sprite)->priority = 1;
            actor->velocityX                        = toFIXED(syRandomBelow(8) - 4);
            actor->velocityY                        = toFIXED(4);
            fdActorStartScript(actor, MarsLighthouseHallFlamePuffThrownScript);
        }
    }
}

// One floor tile before the statue breaks in a burst of flame with a clatter, showing its part of
// the round carving beneath: the metatile (metatileX, metatileY) takes the picture at (srcX, srcY).
void scMarsLighthouseHallBreakFloorTile(
    Sint32 x, Sint32 z, Sint32 srcX, Sint32 srcY, Sint32 metatileX, Sint32 metatileY)
{
    Uint32 i;
    Sint32 px;
    Sint32 pz;

    fdScriptPlaySound(SE_CLATTER);
    scMarsLighthouseHallSpawnFlamePuff(x, z + 12, 1);
    for (i = 0; i < 3; i++)
    {
        px = x + (syRandomBelow(12)) - 6;
        pz = z + (syRandomBelow(12)) - 6;
        scMarsLighthouseHallSpawnFlamePuff(px, pz, 0);
    }
    sySynch(4);
    fdMapGridCopyMetatilesTo(srcX, srcY, metatileX + 64, metatileY, 1, 1);
}

// The great dragon statue at the head of the hall speaks as Felix steps in from the stairs
// (metatiles (11, 16) to (16, 16)): the camera rises to it and the screen turns yellow-green.
// Without the Mars Star it says "If you do not hold the star, you shall not ascend to the
// heavens... Begone!", once. With the star it bids the party set the four spirits aflame and the
// gallery's four doorways flash, after which it speaks no more; in every edition but the Japanese
// it waits for the lighthouse to have thawed as well.
void scMarsLighthouseHallHearDragonStatue(void)
{
    if (gmPartyInventoryFindMember(ITEM_MYTHRIL_BAG_MARS_STAR) == -1)
    {
        if (!fdScriptGetFlag(FLAG_TOLD_TO_BEGONE))
        {
            Uint16 *color = (Uint16 *)&CENTERED_TEXT_COLOR;

            fdScriptStart();
            fdScriptEndLastingPsynergy(0);
            fdCameraSetSpeed(2);
            fdCameraMoveTo(224, 64, 216, 1);
            fdCameraWaitForArrival();
            syFieldScriptWait(20);
            fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
            fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 0);
            fdPaletteFadeStartInterpolation(20);
            sySynch(20);
            *color = RGB_WHITE;
            sySynch(1);
            syScriptMessageShowCentered((Sint32)MsgMarsLighthouseHallIfYouDoNotHold, 0, 4);
            fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
            fdPaletteFadeStartInterpolation(20);
            sySynch(20);
            fdScriptSetFlag(FLAG_TOLD_TO_BEGONE);
            fdScriptEnd();
        }
    }
#if !DRAGON_STATUE_WAITS_FOR_THAW
    else
    {
#else
    else if (fdScriptGetFlag(FLAG_MARS_LIGHTHOUSE_THAWED))
    {
#endif
        Uint16 *color = (Uint16 *)&CENTERED_TEXT_COLOR;

        fdScriptStart();
        fdScriptEndLastingPsynergy(0);
        fdCameraSetSpeed(2);
        fdCameraMoveTo(224, 64, 216, 1);
        fdCameraWaitForArrival();
        syFieldScriptWait(20);
        fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
        fdPaletteFadeSetTarget(PALETTE_TINT_WARM, 0);
        fdPaletteFadeStartInterpolation(20);
        sySynch(20);
        *color = RGB_WHITE;
        sySynch(1);
        syScriptMessageShowCentered((Sint32)MsgMarsLighthouseHallYouWhoHoldStarOpen, 0, 4);
        sySynch(10);
        syScriptMessageShowCentered((Sint32)MsgMarsLighthouseHallYouWhoHoldStarOpen + 1, 0, 3);
        fdCameraMoveTo(224, 88, 216, 1);
        fdCameraWaitForArrival();
        syFieldScriptWait(20);
        fdScriptPlaySound(SE_MARS_LIGHTHOUSE_DOORWAY);
        scMarsLighthouseHallFlashDoorways();
        syFieldScriptWait(20);
        fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
        fdPaletteFadeStartInterpolation(20);
        sySynch(40);
        fdScriptSetFlag(FLAG_TOLD_TO_LIGHT_FLAMES);
        fdScriptEnd();
    }
}

void   scTeleportArriveWhirling(void);

Sint32 fdLeaderKnockBack(void);
Sint32 scFireballCarryLeader(void);
void   fdActorLaunchAhead(ACTOR *self);

void   fdTeleportPlaySparks(void);
void   scMarsLighthouseHallFadeDoorway(void);

#include "shared/teleport_shimmer.c"

#include "shared/actor_is_touching.h"

#include "shared/actor_is_beside.h"

Bool fdActorIsTouching(ACTOR *a, ACTOR *b);
Bool fdActorIsBeside(ACTOR *a, ACTOR *b);

#include "shared/flame_update_forward.h"

#include "shared/actor_reset_scripted_for.h"

// A flame thrower's flame (FLAME_FORWARD), sent south from `pos` at `speed` (scFlameUpdateForward);
// `offScreen` keeps its sprite's picture loaded out of sight.
void scFlameThrowerSendForward(VECTOR3 *pos, Sint32 speed, Sint32 offScreen)
{
    ACTOR *actor = scFieldActorCreate(SPRITE_BLAZE_FLAME, pos->x, pos->y, pos->z);

    if (actor != 0)
    {
        actor->bounce    = 0;
        actor->gravity   = 0;
        actor->sceneWord = speed;
        scActorResetScriptedFor(actor, 2);
        actor->update = scFlameUpdateForward;
        if (offScreen != 0)
            actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
        fdActorSelectAnimation(actor, 2);
    }
}

Bool fdActorIsTouching(ACTOR *a, ACTOR *b);
Bool fdActorIsBeside(ACTOR *a, ACTOR *b);

#include "shared/flame_update_sideways.h"

#include "shared/actor_setup_scripted_part.h"

// A flame thrower's flame (FLAME_BACKWARD), sent north from `pos` at `speed`
// (scFlameUpdateForward); `offScreen` keeps its sprite's picture loaded out of sight.
void scFlameThrowerSendBackward(VECTOR3 *pos, Sint32 speed, Sint32 offScreen)
{
    ACTOR *actor = scFieldActorCreate(SPRITE_BLAZE_FLAME, pos->x, pos->y, pos->z);

    if (actor != 0)
    {
        actor->bounce    = 0;
        actor->gravity   = 0;
        actor->sceneWord = -speed;
        scActorSetupScriptedPart(actor, 3);
        actor->update = scFlameUpdateForward;
        if (offScreen != 0)
            actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
        fdActorSelectAnimation(actor, 2);
    }
}

// A flame thrower's flame (FLAME_EAST), sent east from `pos` at `speed` (scFlameUpdateSideways);
// `offScreen` keeps its sprite's picture loaded out of sight.
void scFlameThrowerSendEast(VECTOR3 *pos, Sint32 speed, Sint32 offScreen)
{
    ACTOR *actor = scFieldActorCreate(SPRITE_BLAZE_FLAME, pos->x, pos->y, pos->z);

    if (actor != 0)
    {
        actor->bounce    = 0;
        actor->gravity   = 0;
        actor->sceneWord = speed;
        scActorSetupScriptedPart(actor, 3);
        actor->update = scFlameUpdateSideways;
        if (offScreen != 0)
            actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
        fdActorSelectAnimation(actor, 2);
    }
}

// A flame thrower's flame (FLAME_WEST), sent west from `pos` at `speed` (scFlameUpdateSideways);
// `offScreen` keeps its sprite's picture loaded out of sight.
void scFlameThrowerSendWest(VECTOR3 *pos, Sint32 speed, Sint32 offScreen)
{
    ACTOR *actor = scFieldActorCreate(SPRITE_BLAZE_FLAME, pos->x, pos->y, pos->z);

    if (actor != 0)
    {
        actor->bounce    = 0;
        actor->gravity   = 0;
        actor->sceneWord = -speed;
        scActorSetupScriptedPart(actor, 3);
        actor->update = scFlameUpdateSideways;
        if (offScreen != 0)
            actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
        fdActorSelectAnimation(actor, 2);
    }
}

#include "shared/actor_is_above.h"

#include "shared/flame_thrower_update_spark.h"

#include "shared/actor_setup_scripted.h"

// A spark puffed from a flame thrower, drifting off in a random direction.
void scFlameThrowerSendSpark(VECTOR3 *pos, Sint32 speed, Sint32 offScreen)
{
    ACTOR *actor = scFieldActorCreate(SPRITE_DUST_CLOUD, pos->x, pos->y, pos->z - toFIXED(2));
    Sint32 spin;

    if (actor != 0)
    {
        actor->bounce    = syCos(syRandomBelow(TRIG_TURN * 3 / 16) + TRIG_TURN * 3 / 16);
        actor->gravity   = (Uint16)syRandom();
        actor->sceneWord = (Uint16)(syRandom() * 2) + speed;
        spin             = (Uint16)syRandom() - DEGtoANG(180);
        actor->timer     = spin;
        ((void (*)())scActorSetupScripted)(actor, 2);
        actor->update = scFlameThrowerUpdateSpark;
        fdActorSetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
    }
}

#include "shared/flame_throwers_puff.c"

#include "shared/flame_throwers_create.c"

#include "shared/flame_throwers_release_flame.h"

// Sets whether the flame throwers keep puffing while Psynergy is used.
void scFlameThrowersSetMode(Sint16 value)
{
    FLAME_THROWERS_STATE->runsDuringPsynergy = value;
}

#include "shared/beam_start.h"

#include "shared/flame_thrower_is_active.c"

#include "shared/flame_thrower_set_active.c"

// The flame throwers' counter, which scFlameThrowersAdvanceCounter steps.
Sint32 scFlameThrowersGetCounter(void)
{
    return FLAME_THROWERS_STATE->count;
}

#include "shared/flame_throwers_advance_counter.h"

#include "shared/flame_thrower_set.h"

#include "shared/flame_thrower_set_puffs_off_screen.c"

#include "shared/flame_thrower_get_flags.c"

#include "shared/flame_throwers_set_stop.h"

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect_advance_spin.c"

#include "shared/actor_blink.h"

#include "shared/leader_knock_back.c"

// Hit by the flames, the leader is knocked back.
void scMarsLighthouseHallKnockBackIfHit(void)
{
    if (FLAME_THROWERS_STATE->active)
        fdLeaderKnockBack();
}

// Hit by a rolling fireball: the leader is hurt, blinks, and is carried along with it until it
// stops.
Sint32 scFireballCarryLeader(void)
{
    FIELD_STATE     *event = RamHeapSlots->fieldState;
    FLAME_HIT_STATE *ride;
    ACTOR           *leader;
    // LEFTOVER(frame): an unused local the ROM's frame reserves and never
    // * touches.
    UNUSED Sint32    unused[3];

    if (event->transitionActive == 0)
    {
        fdScreenTransitionStart(event->transition, event->transitionFrame);
        event->transitionActive = 1;
    }
    fdScriptClearFlag(FLAG_MENU_OPEN);
    event->psynergy  = 0;
    event->fieldMenu = 0;
    event->pauseMenu = 0;
    event->aButton   = 0;
    event->encounter = 0;
#if FIX_FUNCTION_SIGNATURES
    gmPartyTakeFallDamage();
#else
    // UB: the fall damage takes no arguments and returns nothing; it is passed the arguments it
    // gives fdPartyAddHp.
    ((Sint32 (*)())gmPartyTakeFallDamage)(-FALL_DAMAGE, 0);
#endif
    fdCameraSetSpeed(7);
    leader = fdActorGet(MainRoutineStateWork.leaderActorId);
    ride   = FLAME_THROWERS_STATE;
    fdScriptSetFlag(FLAG_MENU_OPEN);
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdScriptClearFlag(FLAG_MENU_OPEN);
    leader->update = (OBJECT_UPDATE)fdActorBlink;
    leader->facing = FACING_SOUTH;
    fdActorSelectAnimation(leader, ANIM_LEADER_SIT);
    while (ride->car->timer == 0)
    {
        Sint32 kind = fdMapGridGetTerrainKindOrBumpAt(0, leader->x.fixed, leader->z.fixed);

        if (kind != TERRAIN_ROUNDED_SQUARE)
        {
            leader->x.fixed += ride->car->stepX;
            leader->z.fixed += ride->car->stepZ;
        }
        else
        {
            if (ride->car->stepX != 0)
                leader->x.fixed = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
            if (ride->car->stepZ != 0)
                leader->z.fixed = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
        }
        ride->active = 0;
        syWait(1);
    }
    syFieldScriptWait(30);
    leader->update = NULL;
    fdActorSetSpriteColorMode(leader, 0);
    ride->active = 0;
#if FIX_FUNCTION_SIGNATURES
    return 0;
#endif
}

void fdTeleportPlaySparks(void);

#include "shared/fireball_carry_leader_if_hit.h"

// Whether a flame or fireball has hit the leader.
Sint32 scFlameThrowersHasHitLeader(void)
{
    return FLAME_THROWERS_STATE->active;
}

#include "shared/actor_scatter_crumbs.c"

#include "shared/ice_chips_spawn.h"

#include "shared/actor_hold_during_script.c"

#include "shared/actor_roll_and_crumble.c"

#include "shared/actor_attach.c"

#include "shared/actor_launch_ahead.h"

Sint32 fdMapGetTerrainHeight(Sint32 layer, Sint32 x, Sint32 z);

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

#include "shared/actor_park.h"

Sint32 MarsLighthouseHallTorchScript[] = {
    SC_CALL(scMarsLighthouseHallSetUpTorch),
    SC_END,
};

// clang-format off
#include "shared/mars_lighthouse_part_scripts.c"

#define TELEPORT_CIRCLE_FIRST_ENTRY 201
#include "shared/teleport_circle_palette.c"

SCENE_ENTRANCE SceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
    SCENE_EXITS(MarsLighthouseBeaconLedge),
    SCENE_EXIT(MARS_LIGHTHOUSE_BEACON_LEDGE_TO_MARS_LIGHTHOUSE_VENUS_TOWER, MarsLighthouseVenusTower, MARS_LIGHTHOUSE_VENUS_TOWER_FROM_MARS_LIGHTHOUSE_BEACON_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_BEACON_LEDGE_TO_MARS_LIGHTHOUSE_MERCURY_TOWER, MarsLighthouseMercuryTower, MARS_LIGHTHOUSE_MERCURY_TOWER_FROM_MARS_LIGHTHOUSE_BEACON_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_BEACON_LEDGE_TO_MARS_LIGHTHOUSE_MARS_TOWER, MarsLighthouseMarsTower, MARS_LIGHTHOUSE_MARS_TOWER_FROM_MARS_LIGHTHOUSE_BEACON_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_BEACON_LEDGE_TO_MARS_LIGHTHOUSE_JUPITER_TOWER, MarsLighthouseJupiterTower, MARS_LIGHTHOUSE_JUPITER_TOWER_FROM_MARS_LIGHTHOUSE_BEACON_LEDGE),
    SCENE_EXITS(MarsLighthouseHall),
    SCENE_EXIT(MARS_LIGHTHOUSE_HALL_TO_MARS_LIGHTHOUSE_AERIE, MarsLighthouseAerie, MARS_LIGHTHOUSE_AERIE_FROM_MARS_LIGHTHOUSE_HALL),
    SCENE_EXIT(MARS_LIGHTHOUSE_HALL_TO_MARS_LIGHTHOUSE_ENTRANCE_FUGUE, MarsLighthouseEntranceFugue, MARS_LIGHTHOUSE_ENTRANCE_FUGUE_FROM_MARS_LIGHTHOUSE_HALL),
    SCENE_EXIT(MARS_LIGHTHOUSE_HALL_TO_MARS_LIGHTHOUSE_VENUS_LEDGE, MarsLighthouseVenusLedge, MARS_LIGHTHOUSE_VENUS_LEDGE_FROM_MARS_LIGHTHOUSE_HALL),
    SCENE_EXIT(MARS_LIGHTHOUSE_HALL_TO_MARS_LIGHTHOUSE_MERCURY_LEDGE, MarsLighthouseMercuryLedge, MARS_LIGHTHOUSE_MERCURY_LEDGE_FROM_MARS_LIGHTHOUSE_HALL),
    SCENE_EXIT(MARS_LIGHTHOUSE_HALL_TO_MARS_LIGHTHOUSE_MARS_LEDGE, MarsLighthouseMarsLedge, MARS_LIGHTHOUSE_MARS_LEDGE_FROM_MARS_LIGHTHOUSE_HALL),
    SCENE_EXIT(MARS_LIGHTHOUSE_HALL_TO_MARS_LIGHTHOUSE_JUPITER_LEDGE, MarsLighthouseJupiterLedge, MARS_LIGHTHOUSE_JUPITER_LEDGE_FROM_MARS_LIGHTHOUSE_HALL),
    SCENE_EXIT(7, MarsLighthouseHall, 8),
    SCENE_EXIT(8, MarsLighthouseHall, 7),
    SCENE_EXIT(9, MarsLighthouseHall, 10),
    SCENE_EXIT(10, MarsLighthouseHall, 9),
    SCENE_EXIT(11, MarsLighthouseHall, 12),
    SCENE_EXIT(12, MarsLighthouseHall, 11),
    SCENE_EXIT(13, MarsLighthouseHall, 14),
    SCENE_EXIT(14, MarsLighthouseHall, 13),
    SCENE_EXITS(MarsLighthouseMercuryLedge),
    SCENE_EXIT(MARS_LIGHTHOUSE_MERCURY_LEDGE_TO_MARS_LIGHTHOUSE_HALL, MarsLighthouseHall, MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_MERCURY_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_MERCURY_LEDGE_TO_MARS_LIGHTHOUSE_MERCURY_TOWER, MarsLighthouseMercuryTower, MARS_LIGHTHOUSE_MERCURY_TOWER_FROM_MARS_LIGHTHOUSE_MERCURY_LEDGE),
    SCENE_EXITS(MarsLighthouseJupiterLedge),
    SCENE_EXIT(MARS_LIGHTHOUSE_JUPITER_LEDGE_TO_MARS_LIGHTHOUSE_HALL, MarsLighthouseHall, MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_JUPITER_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_JUPITER_LEDGE_TO_MARS_LIGHTHOUSE_JUPITER_TOWER, MarsLighthouseJupiterTower, MARS_LIGHTHOUSE_JUPITER_TOWER_FROM_MARS_LIGHTHOUSE_JUPITER_LEDGE),
    SCENE_EXITS(MarsLighthouseMarsLedge),
    SCENE_EXIT(MARS_LIGHTHOUSE_MARS_LEDGE_TO_MARS_LIGHTHOUSE_HALL, MarsLighthouseHall, MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_MARS_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_MARS_LEDGE_TO_MARS_LIGHTHOUSE_MARS_TOWER, MarsLighthouseMarsTower, MARS_LIGHTHOUSE_MARS_TOWER_FROM_MARS_LIGHTHOUSE_MARS_LEDGE),
    SCENE_EXITS(MarsLighthouseVenusLedge),
    SCENE_EXIT(MARS_LIGHTHOUSE_VENUS_LEDGE_TO_MARS_LIGHTHOUSE_HALL, MarsLighthouseHall, MARS_LIGHTHOUSE_HALL_FROM_MARS_LIGHTHOUSE_VENUS_LEDGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_VENUS_LEDGE_TO_MARS_LIGHTHOUSE_VENUS_TOWER, MarsLighthouseVenusTower, MARS_LIGHTHOUSE_VENUS_TOWER_FROM_MARS_LIGHTHOUSE_VENUS_LEDGE),
    SCENE_EXITS_END,
};

ACTOR_PLACE MarsLighthouseHallPlacements[] = {
    {SCENE_TABLE_END},
};

ACTOR_PLACE MarsLighthouseHallFlamePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (Sint32)MarsLighthouseHallTorchScript, 504, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 504, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, (Sint32)MarsLighthouseHallTorchScript, 504, 0, 112, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE MarsLighthouseHallInscriptionsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ROBED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 224, 0, 144, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

CUE MarsLighthouseHallCues[] = {
    {CUE_TABLE_END},
};

// The beacon ledge inscription's glow, in ticks; -1 before it starts.
Sint32 MarsLighthouseHallLedgeCycleTick = -1;

CUE MarsLighthouseHallFlameCues[] = {
    {CUE_STEP, METATILE_LEDGE_DOORWAY, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseHallLeaveLedge)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LEDGE_TORCH, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseHallStartBlazeOnTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), ACTOR_LEDGE_BLAZE_TARGET, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseHallWatchBlazeReachFlame)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), ACTOR_LEDGE_BLAZE_TARGET, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseHallLightFlame)},
    {CUE_TABLE_END},
};

// The hall inscriptions' glow, in ticks; -1 before it starts.
Sint32 MarsLighthouseHallHallCycleTick = -1;

CUE MarsLighthouseHallInscriptionsCues[] = {
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {CUE_EXIT, 3, CONDITION_ALWAYS, 3},
    {CUE_EXIT, 4, CONDITION_ALWAYS, 4},
    {CUE_EXIT, 5, CONDITION_ALWAYS, 5},
    {CUE_EXIT, 6, CONDITION_ALWAYS, 6},
    {CUE_EXIT, 7, CONDITION_ALWAYS, 7},
    {CUE_EXIT, 8, CONDITION_ALWAYS, 8},
    {CUE_EXIT, 9, CONDITION_ALWAYS, 9},
    {CUE_EXIT, 10, CONDITION_ALWAYS, 10},
    {CUE_EXIT, 11, CONDITION_ALWAYS, 11},
    {CUE_EXIT, 12, CONDITION_ALWAYS, 12},
    {CUE_EXIT, 13, CONDITION_ALWAYS, 13},
    {CUE_EXIT, 14, CONDITION_ALWAYS, 14},
    {CUE_STEP, METATILE_BEHAVIOR_VENUS_DOORWAY, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), SCRIPT_REF(scMarsLighthouseHallStepTowardDoorway)},
    {CUE_STEP, METATILE_BEHAVIOR_VENUS_DOORWAY + 1, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), SCRIPT_REF(scMarsLighthouseHallStepTowardDoorway)},
    {CUE_STEP, METATILE_BEHAVIOR_MERCURY_DOORWAY, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), SCRIPT_REF(scMarsLighthouseHallStepTowardDoorway)},
    {CUE_STEP, METATILE_BEHAVIOR_MERCURY_DOORWAY + 1, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), SCRIPT_REF(scMarsLighthouseHallStepTowardDoorway)},
    {CUE_STEP, METATILE_BEHAVIOR_MARS_DOORWAY, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), SCRIPT_REF(scMarsLighthouseHallStepTowardDoorway)},
    {CUE_STEP, METATILE_BEHAVIOR_MARS_DOORWAY + 1, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), SCRIPT_REF(scMarsLighthouseHallStepTowardDoorway)},
    {CUE_STEP, METATILE_BEHAVIOR_JUPITER_DOORWAY, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), SCRIPT_REF(scMarsLighthouseHallStepTowardDoorway)},
    {CUE_STEP, METATILE_BEHAVIOR_JUPITER_DOORWAY + 1, IF_FLAG_SET(FLAG_MARS_LIGHTHOUSE_THAWED), SCRIPT_REF(scMarsLighthouseHallStepTowardDoorway)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), METATILE_AERIE_CARVING, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseHallTeleportToAerie)},
    {CUE_EXAMINE, METATILE_EARTH_INSCRIPTION, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseHallReadEarthInscription)},
    {CUE_EXAMINE, METATILE_WATER_INSCRIPTION, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseHallReadWaterInscription)},
    {CUE_EXAMINE, METATILE_FIRE_INSCRIPTION, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseHallReadFireInscription)},
    {CUE_EXAMINE, METATILE_WIND_INSCRIPTION, CONDITION_ALWAYS, SCRIPT_REF(scMarsLighthouseHallReadWindInscription)},
    {CUE_STEP, METATILE_BEFORE_DRAGON_STATUE, FLAG_TOLD_TO_LIGHT_FLAMES, SCRIPT_REF(scMarsLighthouseHallHearDragonStatue)},
    {CUE_TABLE_END},
};

CUE MarsLighthouseHallLedgesCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, 1},
    {CUE_EXIT, 2, CONDITION_ALWAYS, 2},
    {CUE_TABLE_END},
};

Sint32 MarsLighthouseHallFlamePuffSwellScript[] = {
    SC_ADD(SW_SCALE_X, toFIXED(3) / 16),
    SC_ADD(SW_SCALE_Y, toFIXED(3) / 16),
    SC_ADD(SW_Y, -toFIXED(3) / 32),
    SC_WAIT(1),
    SC_LOOP(8, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -toFIXED(3) / 16),
    SC_ADD(SW_SCALE_Y, -toFIXED(3) / 16),
    SC_ADD(SW_Y, toFIXED(3) / 32),
    SC_WAIT(1),
    SC_LOOP(12, SC_TO(1)),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

Sint32 MarsLighthouseHallFlamePuffThrownScript[] = {
    SC_WAIT(20),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};
// clang-format on

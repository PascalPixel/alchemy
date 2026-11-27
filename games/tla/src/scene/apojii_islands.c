// The Apojii Islands, at the edge of the world in the Eastern Sea, in three rooms of this overlay:
// the village round the spring where the Aqua Stone stands, which Douse wakes
// (scApojiiIslandsDouseAquaStone) to carry the party across the sea to Aqua Rock; its houses, with
// the inn and the general store, and beside them the Sea of Time Islet's house; and the islet among
// the falls where the Jupiter Djinni Haze waits (scApojiiIslandsHazeJoinsParty). The village's sea
// and the islet's falls cycle their colours.
#include "scene/script_layer.h"
#include "flags.h"
#include "field/field_mode.h"
#include "field/actor_script.h"
#include "scene/dust.h"
#include "shop/shop.h"

// Set once Douse has woken the Aqua Stone.
#define FLAG_APOJII_AQUA_STONE_DOUSED          0x95f
// Set by Douse on the Aqua Stone, which leaves for the World Map: back in the village, the father
// who wished on the stone sees it answer (scApojiiIslandsFatherSeesTheStone), and that clears it.
#define FLAG_FATHER_SEES_THE_STONE             0x95e
// Set once the treasures buried under the weeds are found (SceneItems): the village's Mint and Herb
// and the islet's Bramble Seed.
#define FLAG_APOJII_ISLANDS_MINT_TAKEN         0xf9a
#define FLAG_APOJII_ISLANDS_HERB_TAKEN         0xf9b
#define FLAG_APOJII_ISLANDS_BRAMBLE_SEED_TAKEN 0xf9e
// Haze's islet: set once Lash has tied the rope between its posts.
#define FLAG_HAZE_ROPE_TIED                    FLAG_SCENE_TEMP_00

// The metatile behaviours the village's and the islet's cue tables answer: the bank of the stream
// north of the hut by the Aqua Stone's pool, which Felix jumps across walking north; the weeds
// Cyclone scatters; the weeds over a buried treasure, the village's Herb and Mint and the islet's
// Bramble Seed; and the edge of Haze's ledge, which Felix jumps down walking south.
enum
{
    METATILE_BEHAVIOR_STREAM_BANK             = 33,
    METATILE_BEHAVIOR_HAZE_LEDGE_EDGE         = 40,
    METATILE_BEHAVIOR_WEEDS                   = 21,
    METATILE_BEHAVIOR_WEEDS_OVER_HERB         = 22,
    METATILE_BEHAVIOR_WEEDS_OVER_MINT         = 23,
    METATILE_BEHAVIOR_WEEDS_OVER_BRAMBLE_SEED = 22
};

#include "macros.h"

// The village by the Aqua Stone (ApojiiIslandsVillagePlacements).
typedef enum APOJII_ISLANDS_VILLAGE_ACTOR_ID
{
    // "This is Apojii, the easternmost island in the Eastern Sea!"
    ACTOR_WELCOMING_YOUNG_MAN = ACTOR_FIRST_PLACED,
    // "Whatever you do, don't sail out to the eastern edge of the sea..."
    ACTOR_WOMAN_WARNING_OF_THE_EDGE,
    // "The huge waterfall at the edge of the world is known as Gaia Falls."
    ACTOR_MAN_ABOUT_GAIA_FALLS,
    // "On Aqua Island, you can see the guardian moai."
    ACTOR_WOMAN_ABOUT_THE_MOAI,
    // "Have you any idea what it's like living on the edge of the world?"
    ACTOR_OLD_MAN_AT_THE_EDGE_OF_THE_WORLD,
    // "I don't know what brings you to Apojii, but you'd better not be planning anything stupid."
    ACTOR_SUSPICIOUS_OLD_WOMAN,
    // "When they come of age, all the men of Apojii go to Aqua Island. Will I, too?"
    ACTOR_BOY_COMING_OF_AGE,
    // "Do you think if you fall off of Gaia Falls that you keep on falling forever and ever?"
    ACTOR_GIRL_ABOUT_GAIA_FALLS,
    // The father who wished on the rock: "What... What just happened? That rock couldn't have heard
    // my wishes..."
    ACTOR_FATHER,
    // The blue tear-shaped Aqua Stone Douse wakes.
    ACTOR_AQUA_STONE
} APOJII_ISLANDS_VILLAGE_ACTOR_ID;

// The houses (ApojiiIslandsShopsPlacements).
typedef enum APOJII_ISLANDS_HOUSES_ACTOR_ID
{
    // "There's this guy in town who spends his time at the spring, staring at the Aqua Stone."
    ACTOR_WOMAN_ABOUT_THE_STARING_MAN = ACTOR_FIRST_PLACED,
    // His son (scApojiiIslandsTalkToSon).
    ACTOR_SON,
    // "I absolutely refuse to let my son swim in the ocean until he is an adult."
    ACTOR_STRICT_FATHER,
    // "The currents around Apojii are just too strong to let children swim on their own."
    ACTOR_CAUTIOUS_MOTHER,
    // "I'm not allowed to go into the ocean."
    ACTOR_BOY_KEPT_FROM_THE_OCEAN,
    // "Hey, mister! Did you know that all the kids are calling you their hero?"
    ACTOR_GIRL_CALLING_FELIX_A_HERO,
    // "That old Aqua Stone is supposed to be the key to getting onto Aqua Island."
    ACTOR_OLD_MAN_ABOUT_THE_AQUA_STONE,
    // "He's too old to try now, but my husband used to dream of scaling Aqua Rock."
    ACTOR_OLD_WOMAN_ABOUT_HER_HUSBANDS_DREAM,
    // "It'd be nice if we could import some items that our visitors would like."
    ACTOR_GENERAL_STOREKEEPER,
    // "Wow! You look just like the guy who showed up in the fancy boat."
    ACTOR_INNKEEPER,
    // "You guys aren't the warriors who came in that boat, are you?"
    ACTOR_OLD_WOMAN_AT_THE_INN,
    // In the Sea of Time Islet's house: "The temperature of the ocean has been rising."
    ACTOR_OLD_MAN_ABOUT_THE_WARMING_SEA
} APOJII_ISLANDS_HOUSES_ACTOR_ID;

// Haze's islet (ApojiiIslandsHazePlacements).
typedef enum APOJII_ISLANDS_HAZE_ACTOR_ID
{
    // The rope's posts: Lash at the one on the rock among the falls ties the rope up to the one on
    // the cliff.
    ACTOR_FALLS_ROPE_POST = ACTOR_FIRST_PLACED,
    ACTOR_CLIFF_ROPE_POST,
    ACTOR_HAZE
} APOJII_ISLANDS_HAZE_ACTOR_ID;

#include "field/script_inline.h"
#include "text/msg_ids.h"

void   scApojiiIslandsCycleFallsColors(void);
void   scApojiiIslandsCycleSeaColors(void);
void   scApojiiIslandsCycleAquaStoneColors(void);
void   scApojiiIslandsStopFlashing(ACTOR *actor);
void   scActorFlashWhite(ACTOR *actor);
void   scApojiiIslandsFatherSeesTheStone(void);
void   scScrollHBlank(void);
void   scLayerTrackScroll(void);
void   scScrollStartHBlank(void);
void   scApojiiIslandsInitHaze(void);
void   scApojiiIslandsEnterSeaOfTimeIsletHouse(void);
void   scActorSpawnWadingSplash(ACTOR *actor);
Sint32 scActorEmitMist(ACTOR *actor);
void   scLeaderRiseFromWater(void);
void   scApojiiIslandsDrawMetatileMarkers(void);
void   scApojiiIslandsStartMetatileMarkers(void);
void   scApojiiIslandsAddMetatileMarkers(Uint32 base, Sint32 level, Sint32 restLevel);
void   fdGroundParticlesDraw(void);
void   scGroundParticlesSetAnchor(Sint32 id);

// The markers the woken Aqua Stone lays over the spring: at most this many.
#define METATILE_MARKERS      128
// A marker's kinds, by its metatile's collision code from the first: kind 2 shows kind 3's picture
// mirrored.
#define METATILE_MARKER_KINDS 4

// A marker drawn over a metatile of the spring: its place, the height it rests at, its kind (the
// metatile's collision code less METATILE_BEHAVIOR_SPRING), its state and its sprite's attributes.
typedef struct METATILE_MARKER
{
    Sint32 x;
    Sint32 y;
    Sint32 z;
    Sint32 rest;
    Uint16 kind;
    Uint16 state;
    Uint32 oam[3];
} METATILE_MARKER;

// scApojiiIslandsDrawMetatileMarkers's table: the markers' VRAM cache entry and the markers.
typedef struct METATILE_MARKER_SET
{
    Uint16          block;
    Uint16          count;
    METATILE_MARKER markers[METATILE_MARKERS];
} METATILE_MARKER_SET;

// The falls' palette as loaded, the source of each cycle.
Uint16 ApojiiIslandsFallsColors[PALETTE_COLORS] BSS_DATA ALIGNED(4);
// The falls' palette as this tick's cycle leaves it.
Uint16                                           ApojiiIslandsFallsCycled[PALETTE_COLORS] BSS_DATA;
// The spray's palette as loaded.
Uint16                                           ApojiiIslandsSprayColors[PALETTE_COLORS] BSS_DATA;
// The spray's palette as this tick's cycle leaves it.
Uint16                                           ApojiiIslandsSprayCycled[PALETTE_COLORS] BSS_DATA;
// The sea's palette as loaded.
Uint16                                           ApojiiIslandsSeaColors[PALETTE_COLORS] BSS_DATA;
// The sea's palette as this tick's cycle leaves it.
Uint16                                           ApojiiIslandsSeaCycled[PALETTE_COLORS] BSS_DATA;
// The shore's palette as loaded.
Uint16                                           ApojiiIslandsShoreColors[PALETTE_COLORS] BSS_DATA;
// The shore's palette as this tick's cycle leaves it.
Uint16                                           ApojiiIslandsShoreCycled[PALETTE_COLORS] BSS_DATA;
// The woken Aqua Stone's colours as loaded.
Uint16                                           ApojiiIslandsStoneColors[PALETTE_COLORS] BSS_DATA;
// The woken Aqua Stone's colours as this tick's cycle leaves them.
Uint16                                           ApojiiIslandsStoneCycled[PALETTE_COLORS] BSS_DATA;
// The islet's screen line below which BG1 drifts and is blended (scScrollHBlank), and BG1's scroll
// above and below it.
Sint32 ApojiiIslandsSplitLine                    BSS_DATA;
Uint16 ApojiiIslandsUpperScrollX                 BSS_DATA;
Uint16 ApojiiIslandsLowerScrollX                 BSS_DATA;
// The markers drawn over the spring once the Aqua Stone is woken.
METATILE_MARKER_SET ApojiiIslandsMetatileMarkers BSS_DATA;
extern Uint32                                    WadingSplashScript[];
extern Uint32                                    RippleScript[];

#include "field/call.h"
#include "field/field_script.h"
#include "scene/scene.h"
#include "scene/ground_effects.h"
#include "field/map.h"
#include "graphics/palette_buffers.h"
#include "edition.h"

// The cautious mother's collision as she paces her house: in every edition but the Japanese the
// leader can no longer push her.
#define PACER_COLLISION                                                                            \
    WHEN(DAILA_APOJII_TUNDARIA_PACERS_NOT_PUSHABLE,                                                \
         (ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),                                                   \
         (ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID))

// The palettes the colour cycles read and write: background palettes but for the last.
enum
{
    VILLAGE_SEA_PALETTE  = 3,  // the village's sea
    SHORE_PALETTE        = 13, // the village's shore
    FALLS_PALETTE        = 3,  // the islet's falls
    SPRAY_PALETTE        = 4,  // the islet's spray
    // The village's palette the woken Aqua Stone's colours are loaded from.
    STONE_PALETTE        = 4,
    // The sprite palette they turn in, which the spring's markers take.
    STONE_SPRITE_PALETTE = 14
};

// The sea's, shore's, falls' and spray's cycles turn their last four colours, a step every eighth
// tick.
enum
{
    CYCLE_FIRST_COLOR = 12,
    CYCLE_COLORS      = 4,
    CYCLE_STEP_TICKS  = 8
};

// The woken Aqua Stone's cycle turns every colour but the transparent first.
#define STONE_CYCLE_COLORS       (PALETTE_COLORS - 1)

// The collision code of the spring round the Aqua Stone, the first of the four kinds of its
// metatiles (scApojiiIslandsAddMetatileMarkers).
#define METATILE_BEHAVIOR_SPRING 33

// The treasures buried under the weeds, by their place in SceneItems: the village's Mint and Herb,
// and the islet's Bramble Seed.
enum
{
    ACTOR_MINT = FIELD_TREASURE_ACTOR,
    ACTOR_HERB,
    ACTOR_BRAMBLE_SEED = FIELD_TREASURE_ACTOR
};

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "shared/scene_create_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE   fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

extern ACTOR_PLACE ApojiiIslandsPlacements[];
extern ACTOR_PLACE ApojiiIslandsVillagePlacements[];
extern ACTOR_PLACE ApojiiIslandsShopsPlacements[];
extern ACTOR_PLACE ApojiiIslandsHazePlacements[];
extern Sint32      ApojiiIslandsStoneCycleTick[];

extern CUE         ApojiiIslandsCues[];
extern CUE         ApojiiIslandsVillageCues[];
extern CUE         ApojiiIslandsShopsCues[];
extern CUE         ApojiiIslandsHazeCues[];
void               scApojiiIslandsSinkIntoSand(void);
void               scApojiiIslandsRiseFromSand(void);
void               scApojiiIslandsResumeSeaColors(void);
void               scApojiiIslandsScatterHazeWeeds(void);
void               scApojiiIslandsScatterWeedsOverBrambleSeed(void);
void               scApojiiIslandsEnterDoor(void);
void               scApojiiIslandsJumpDownLedge(void);
void               scApojiiIslandsJumpForward(void);
extern Uint32      FieldEffectShortScript[];
extern Uint32      FieldEffectMediumScript[];
extern Uint32      FieldEffectLongScript[];
extern Sint32      ApojiiIslandsPaceScript[];

// Haze, the Jupiter Djinni on the islet, joins the party.
void               scApojiiIslandsHazeJoinsParty(void)
{
    fdDjinnJoinParty(ACTOR_HAZE, DJINNI_HAZE / DJINN_PER_ELEMENT, DJINNI_INDEX(DJINNI_HAZE));
}

const SCENE_ENTRANCE *scGetEntrances(void)
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

const ACTOR_PLACE *scGetActorPlacements(void)
{
    Sint16 scene = MainRoutineStateWork.next;

    if (scene == SCENE_APOJII_ISLANDS)
        return ApojiiIslandsVillagePlacements;
    if (scene == SCENE_APOJII_ISLANDS_SHOPS)
        return ApojiiIslandsShopsPlacements;
    if (scene == SCENE_APOJII_ISLANDS_HAZE)
        return ApojiiIslandsHazePlacements;
    return ApojiiIslandsPlacements;
}

// Sand takes the leader into the sand (and scApojiiIslandsRiseFromSand back up).
void scApojiiIslandsSinkIntoSand(void)
{
    fdActorGet(MainRoutineStateWork.leaderActorId)->layer = MAPGRID_LAYER_OVERHEAD;
}

// Sand ends: the leader is back on the ground layer.
void scApojiiIslandsRiseFromSand(void)
{
    fdActorGet(MainRoutineStateWork.leaderActorId)->layer = MAPGRID_LAYER_GROUND;
}

extern Sint32 ApojiiIslandsFallsCycleTick;

// The islet's falls and their spray cycle their last colours, held while Psynergy or Reveal runs.
void          scApojiiIslandsCycleFallsColors(void)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;
    Sint32       phase, k, offset;
    Uint16      *firstCopy, *secondCopy;

    if (state->psynergy != 0)
        return;
    if (state->revealActive != 0)
        return;
    if (ApojiiIslandsFallsCycleTick == -1)
    {
        syWordsCopy(ApojiiIslandsFallsColors,
                    (void *)&BG_PLTT_COLOR(FALLS_PALETTE, 0),
                    sizeof(ApojiiIslandsFallsColors));
        syWordsCopyEntry(
            ApojiiIslandsFallsCycled, ApojiiIslandsFallsColors, sizeof(ApojiiIslandsFallsCycled));
        syWordsCopyEntry(ApojiiIslandsSprayColors,
                         (void *)&BG_PLTT_COLOR(SPRAY_PALETTE, 0),
                         sizeof(ApojiiIslandsSprayColors));
        syWordsCopyEntry(
            ApojiiIslandsSprayCycled, ApojiiIslandsSprayColors, sizeof(ApojiiIslandsSprayCycled));
        ApojiiIslandsFallsCycleTick = 0;
    }
    phase      = ApojiiIslandsFallsCycleTick / CYCLE_STEP_TICKS % CYCLE_COLORS;
    k          = 0;
    firstCopy  = ApojiiIslandsFallsCycled;
    secondCopy = ApojiiIslandsSprayCycled;
    for (; k < CYCLE_COLORS; k++)
    {
        offset = ((phase + k) % CYCLE_COLORS + CYCLE_FIRST_COLOR) * 2;
        *(Uint16 *)((Uint8 *)firstCopy + offset)  = ApojiiIslandsFallsColors[CYCLE_FIRST_COLOR + k];
        *(Uint16 *)((Uint8 *)secondCopy + offset) = ApojiiIslandsSprayColors[CYCLE_FIRST_COLOR + k];
    }
    IO_WRITE_QUEUE_PUSH(
        (Uint32)&BG_PLTT_COLOR(FALLS_PALETTE, 0), (Uint32)firstCopy, DMA_COPY_WORDS(8));
    IO_WRITE_QUEUE_PUSH(
        (Uint32)&BG_PLTT_COLOR(SPRAY_PALETTE, 0), (Uint32)secondCopy, DMA_COPY_WORDS(8));
    ApojiiIslandsFallsCycleTick++;
}

extern Sint32 ApojiiIslandsSeaCycleTick;

// The village's sea and shore cycle their last colours, held while Psynergy or Reveal runs.
void          scApojiiIslandsCycleSeaColors(void)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;
    Sint32       busy, phase, k, j;

    if (state->psynergy != 0)
        return;
    busy = state->revealActive;
    if (busy != 0)
        return;
    if (ApojiiIslandsSeaCycleTick == -1)
    {
        syWordsCopy(ApojiiIslandsSeaColors,
                    (void *)&BG_PLTT_COLOR(VILLAGE_SEA_PALETTE, 0),
                    sizeof(ApojiiIslandsSeaColors));
        syWordsCopyEntry(
            ApojiiIslandsSeaCycled, ApojiiIslandsSeaColors, sizeof(ApojiiIslandsSeaCycled));
        syWordsCopyEntry(ApojiiIslandsShoreColors,
                         (void *)&BG_PLTT_COLOR(SHORE_PALETTE, 0),
                         sizeof(ApojiiIslandsShoreColors));
        syWordsCopyEntry(
            ApojiiIslandsShoreCycled, ApojiiIslandsShoreColors, sizeof(ApojiiIslandsShoreCycled));
        ApojiiIslandsSeaCycleTick = busy;
    }
    phase = ApojiiIslandsSeaCycleTick / CYCLE_STEP_TICKS % CYCLE_COLORS;
    for (k = 0; k < CYCLE_COLORS; k++)
    {
        j = (phase + k) % CYCLE_COLORS;
        ApojiiIslandsSeaCycled[CYCLE_FIRST_COLOR + j] =
            ApojiiIslandsSeaColors[CYCLE_FIRST_COLOR + k];
        ApojiiIslandsShoreCycled[CYCLE_FIRST_COLOR + j] =
            ApojiiIslandsShoreColors[CYCLE_FIRST_COLOR + k];
    }
    IO_WRITE_QUEUE_PUSH((Uint32)&BG_PLTT_COLOR(VILLAGE_SEA_PALETTE, 0),
                        (Uint32)ApojiiIslandsSeaCycled,
                        DMA_COPY_WORDS(8));
    IO_WRITE_QUEUE_PUSH((Uint32)&BG_PLTT_COLOR(SHORE_PALETTE, 0),
                        (Uint32)ApojiiIslandsShoreCycled,
                        DMA_COPY_WORDS(8));
    ApojiiIslandsSeaCycleTick++;
}

// Reveal holds the sea's colour cycle as it takes effect.
void scApojiiIslandsPauseSeaColors(void)
{
    sySuspendEvent(scApojiiIslandsCycleSeaColors);
    syWait(3);
}

// The sea's colour cycle runs again as Reveal fades.
void scApojiiIslandsResumeSeaColors(void)
{
    syResumeEvent(scApojiiIslandsCycleSeaColors);
    syWait(3);
}

// The woken Aqua Stone's colours turn a step every other tick, in the sprite palette the spring's
// markers take.
void scApojiiIslandsCycleAquaStoneColors(void)
{
    Sint32 base;
    Sint32 i;

    if (ApojiiIslandsStoneCycleTick[0] == -1)
    {
        syWordsCopyEntry(ApojiiIslandsStoneColors,
                         (void *)&BG_PLTT_COLOR(STONE_PALETTE, 0),
                         sizeof(ApojiiIslandsStoneColors));
        ApojiiIslandsStoneCycleTick[0] = 0;
    }
    base = (ApojiiIslandsStoneCycleTick[0] / 2) % STONE_CYCLE_COLORS;
    for (i = 0; i < STONE_CYCLE_COLORS; i++)
        ApojiiIslandsStoneCycled[(base + i) % STONE_CYCLE_COLORS + 1] =
            ApojiiIslandsStoneColors[i + 1];
    IO_WRITE_QUEUE_PUSH((Uint32)&OBJ_PLTT_COLOR(STONE_SPRITE_PALETTE, 0),
                        (Uint32)ApojiiIslandsStoneCycled,
                        DMA_COPY_WORDS(8));
    ApojiiIslandsStoneCycleTick[0]++;
}

// A flashing actor's last update: its sprite's colour back to normal.
void scApojiiIslandsStopFlashing(ACTOR *actor)
{
    fdActorSetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
    actor->update = 0;
}

#include "shared/actor_flash_white.h"

// Douse on the Aqua Stone: it flickers white as Aqua Rock's music starts, sparkles rise round it
// and the screen flashes four times in random colours; then the sea stops cycling, markers in the
// stone's turning colours blend in over the spring, the father starts, and the camera pans
// southeast before the party leaves for the World Map, where sparkles carry it across the sea to
// Aqua Rock (scWorldMapCarryPartyToAquaRock).
void scApojiiIslandsDouseAquaStone(Sint32 unused, Sint32 id)
{
    ACTOR *actor = fdActorGetById(id);
    Sint32 i;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    actor->update = scActorFlashWhite;
    sySoundPlay(SOUND_FADE_OUT_BGM);
    sySynch(30);
    sySoundPlay(MUS_AQUA_ROCK);
    fdGroundParticlesStart();
    scGroundParticlesSetAnchor(ACTOR_AQUA_STONE);
    fdQueueMusic(SE_AMBIENT_WATER);
    for (i = 0; i < 4; i++)
    {
        fdPaletteFadeSetTarget(syRandom() & 0x7fff, 0);
        fdPaletteFadeStartInterpolation(15);
        sySynch(10);
        Value2((Sint32 (*)())fdPaletteFadeSetTarget, PALETTE_TINT_NONE, 0);
        fdPaletteFadeStartInterpolation(10);
        sySynch(10);
    }
    fdActorTableSnapshot();
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDCNT,
                        BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3,
                        IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA, BLEND_ALPHA(0, BLEND_WEIGHT_MAX), IO_WRITE_HALFWORD);
    sySynch(1);
    syCloseEvent(scApojiiIslandsCycleSeaColors);
    sySetEvent(scApojiiIslandsCycleAquaStoneColors, EVENT_PASS_TICK);
    scApojiiIslandsStartMetatileMarkers();
    scApojiiIslandsAddMetatileMarkers(METATILE_BEHAVIOR_SPRING, 0, 0);
    for (i = 1; i <= 6; i++)
    {
        IO_WRITE_QUEUE_PUSH(
            (Uint32)&REG_BLDALPHA, BLEND_ALPHA(i, BLEND_WEIGHT_MAX), IO_WRITE_HALFWORD);
        sySynch(4);
    }
    syGameFlagSet(FLAG_APOJII_AQUA_STONE_DOUSED);
    syGameFlagSet(FLAG_FATHER_SEES_THE_STONE);
    fdCameraSetSpeed(1);
    fdCameraMoveTo(360, CAMERA_KEEP, 328, 1);
    fdActorShowEmoteById(ACTOR_FATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    fdCameraWaitForArrival();
    fdCameraMoveTo(440, CAMERA_KEEP, 376, 1);
    fdCameraWaitForArrival();
    fdCameraMoveTo(472, CAMERA_KEEP, 408, 1);
    syImportScriptSetExitRequest(APOJII_ISLANDS_TO_WORLD_MAP_AQUA_ROCK);
}

// Back in the village after Douse, the father who wished on the Aqua Stone looks about him and
// wonders whether it heard his wishes.
void scApojiiIslandsFatherSeesTheStone(void)
{
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdActorGet(ACTOR_AQUA_STONE)->update = (OBJECT_UPDATE)scApojiiIslandsStopFlashing;
    syFieldScriptSetMessageId((Sint32)&MsgWhatWhatJust);
    fdActorFaceActor(MainRoutineStateWork.leaderActorId, ACTOR_FATHER, 0);
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    fdActorFaceDirection(ACTOR_FATHER, FACING_EAST, 0);
    fdActorSetAnimationAndWait(ACTOR_FATHER, 3);
    fdActorFaceDirection(ACTOR_FATHER, FACING_WEST, 0);
    fdActorSetAnimationAndWait(ACTOR_FATHER, 3);
    fdActorFaceDirection(ACTOR_FATHER, FACING_NORTHEAST, 0);
    fdActorSetAnimationAndWait(ACTOR_FATHER, 3);
    scFieldActorShowEmote(ACTOR_FATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
    // "What... What just happened?"
    fdScriptShowMessage(ACTOR_FATHER, 0);
    scFieldActorJump(ACTOR_FATHER, 2, 25);
    // "That rock couldn't have heard my wishes... That's impossible!"
    fdScriptShowMessage(ACTOR_FATHER, 0);
    fdActorRunRepeatedMotion(ACTOR_FATHER, 1);
    // "It's got to be."
    fdScriptShowMessage(ACTOR_FATHER, 0);
    fdActorSetAnimationAndWait(ACTOR_FATHER, 3);
    // "Well, OK then... Maybe we should try putting some more of our wishes..."
    fdScriptShowMessage(ACTOR_FATHER, 0);
    fdScriptEnd();
    fdScriptClearFlag(FLAG_FATHER_SEES_THE_STONE);
}

// Cyclone scatters the weeds it touches: their metatiles take those of the scattered weeds, kept
// below the room in the map from row 38.
#define SCATTER_WEEDS(column, row, width, height)                                                  \
    syFieldTileCopiesCopyTouchedMetatiles(column, 38, width, height, column, row, 1)

// Cyclone on the village's weeds scatters them.
void scApojiiIslandsScatterWeeds(void)
{
    SCATTER_WEEDS(68, 4, 39, 24);
}

// Defines Cyclone on the weeds over a buried treasure: they scatter, and until the treasure is
// found it shows at (spotX, spotZ), the leader hopping a metatile east when he stands on it.
#define DEFINE_SCATTER_WEEDS_OVER_TREASURE(                                                        \
    name, column, row, width, height, flag, treasure, spotX, spotZ)                                \
    void name(void)                                                                                \
    {                                                                                              \
        ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);                            \
                                                                                                   \
        SCATTER_WEEDS(column, row, width, height);                                                 \
        if (fdScriptGetFlag(flag) == 0)                                                            \
        {                                                                                          \
            fdActorSetPos(treasure, spotX, spotZ);                                                 \
            if (FIELD_METATILE(leader->x.fixed) == FIELD_METATILE(toFIXED(spotX))                  \
                && FIELD_METATILE(leader->z.fixed) == FIELD_METATILE(toFIXED(spotZ)))              \
            {                                                                                      \
                fdScriptStart();                                                                   \
                syFieldScriptEndLastingPsynergy(0);                                                \
                fdActorSetAttachedEffect(MainRoutineStateWork.leaderActorId,                       \
                                         ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);         \
                fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 16, 0);            \
                leader->velocityY = toFIXED(6);                                                    \
                scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);                   \
                fdScriptEnd();                                                                     \
            }                                                                                      \
        }                                                                                          \
    }

// Cyclone on the clump of weeds south of the Aqua Stone, at metatile (23, 23), blows it away and
// uncovers the Herb buried there.
DEFINE_SCATTER_WEEDS_OVER_TREASURE(ApojiiIslandsScatterWeedsOverHerb,
                                   68,
                                   4,
                                   38,
                                   21,
                                   FLAG_APOJII_ISLANDS_HERB_TAKEN,
                                   ACTOR_HERB,
                                   376,
                                   376)

// Cyclone on the clump of weeds in the village's south-east corner, at metatile (37, 25), blows it
// away and uncovers the Mint buried there.
DEFINE_SCATTER_WEEDS_OVER_TREASURE(ApojiiIslandsScatterWeedsOverMint,
                                   68,
                                   4,
                                   38,
                                   21,
                                   FLAG_APOJII_ISLANDS_MINT_TAKEN,
                                   ACTOR_MINT,
                                   600,
                                   408)

// Cyclone on Haze's islet's weeds scatters them.
void scApojiiIslandsScatterHazeWeeds(void)
{
    SCATTER_WEEDS(67, 8, 11, 6);
}

// Cyclone on the clump of weeds on Haze's islet at metatile (12, 9) blows it away and uncovers the
// Bramble Seed buried there.
DEFINE_SCATTER_WEEDS_OVER_TREASURE(scApojiiIslandsScatterWeedsOverBrambleSeed,
                                   67,
                                   8,
                                   11,
                                   6,
                                   FLAG_APOJII_ISLANDS_BRAMBLE_SEED_TAKEN,
                                   ACTOR_BRAMBLE_SEED,
                                   200,
                                   152)

// Felix walks in at the door of one of the village's huts, and the screen fades to its room: the
// house of the man who stares at the Aqua Stone (its door at metatile (29, 18)), the strict
// father's family's (8, 17), the old couple's in the north-east (35, 7), the inn (7, 6) or the
// general store (12, 5). Each door's metatile behaviour is the number of the exit it leaves by,
// which the field engine keeps as `stepCue`.
void scApojiiIslandsEnterDoor(void)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
    scFieldActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 2);
    fdActorGet(MainRoutineStateWork.leaderActorId)->motionFlags = 0;
    fdScriptPlaySound(SE_MAP_EXIT);
    fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
    fdActorCenterAndWalk(MainRoutineStateWork.leaderActorId, 2, -6);
    syFieldScriptWait(10);
    syFieldScriptSetExitRequest(state->stepCue);
    fdScriptFinishScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptEnd();
}

// The staring man's son turns to the leader and asks why his father stares at the rock, unless the
// leader comes at him from the west, facing east.
Sint32 scApojiiIslandsTalkToSon(void)
{
    Uint16 facing;

    facing = FACING_NEAREST_CARDINAL(fdActorGet(MainRoutineStateWork.leaderActorId)->facing);
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    if (facing != FACING_EAST)
    {
        fdActorFaceActor(ACTOR_SON, MainRoutineStateWork.leaderActorId, 0);
        syFieldScriptSetMessageId((Sint32)&MsgWhyDoesMy);
        // "Why does my father keep staring at that rock every day?"
        fdScriptShowMessage(ACTOR_SON, 0);
        fdActorFaceDirection(ACTOR_SON, FACING_SOUTHEAST, 0);
    }
    fdScriptEnd();
    return 0;
}

// Mind Read on the staring man's son: his thought, unless the leader reads it from the west, facing
// east.
Sint32 scApojiiIslandsMindReadSon(void)
{
    Uint16 facing;

    facing = FACING_NEAREST_CARDINAL(fdActorGet(MainRoutineStateWork.leaderActorId)->facing);
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    if (facing != FACING_EAST)
    {
        syFieldScriptSetMessageId((Sint32)&MsgICanTEver);
        // "I can't ever tell what kind of mood Mom and Dad are in. At least..."
        fdScriptShowMessage(ACTOR_SON, 0);
    }
    fdScriptEnd();
    return 0;
}

// Walking south off Haze's ledge jumps the leader down.
void scApojiiIslandsJumpDownLedge(void)
{
    fdActorJumpDownLedge(MainRoutineStateWork.leaderActorId, ANIM_STAND, 5);
}

const CUE *scGetCues(void)
{
    Sint16 scene = MainRoutineStateWork.next;

    if (scene == SCENE_APOJII_ISLANDS)
        return ApojiiIslandsVillageCues;
    if (scene == SCENE_APOJII_ISLANDS_SHOPS)
        return ApojiiIslandsShopsCues;
    if (scene == SCENE_APOJII_ISLANDS_HAZE)
        return ApojiiIslandsHazeCues;
    return ApojiiIslandsCues;
}

// The Apojii Islands' general store, as mnShopRun numbers the shops.
enum
{
    SHOP_APOJII = 23
};

// The Apojii Islands' inn, as mnInnCheckIn numbers the inns.
enum
{
    APOJII_INN = 8
};

// The innkeeper: across the counter, the inn; from elsewhere, his words about the fancy boat.
void scApojiiIslandsTalkToInnkeeper(Sint32 actor)
{
    if (FACING_NEAREST_CARDINAL(fdActorGet(MainRoutineStateWork.leaderActorId)->facing)
        == FACING_NORTH)
    {
        mnInnCheckIn(APOJII_INN, actor);
        return;
    }
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    syFieldScriptSetMessageId((Sint32)&MsgWowYouLook);
    // "Wow! You look just like the guy who showed up in the fancy boat. He..."
    fdScriptShowMessage(actor, 0);
    fdScriptEnd();
}

// The inn worker asks whether the party are the warriors who came in the boat, and answers the yes
// or the no.
void scApojiiIslandsAskAboutWarriors(Sint32 actor)
{
    Sint32 msg;

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    msg = (Sint32)&MsgYouGuysAren;
    syFieldScriptSetMessageId(msg);
    // "You guys aren't the warriors who came in that boat, are you?"
    fdScriptOpenMessage(actor, 0);
    if (syFieldScriptAskYesNoWithActorAnimation(syFieldPartyGetLeaderActor(), 0) == 0)
    {
        syFieldScriptWait(10);
        // Yes: "Hmm... A while ago, some others came in a boat much like yours..."
        syFieldScriptSetMessageId(msg + 1);
    }
    else
    {
        syFieldScriptWait(20);
        // No: "It's not nice to tease an old lady! If that's not your boat, then how did you get
        // here?"
        syFieldScriptSetMessageId(msg + 2);
    }
    fdScriptShowMessage(actor, 0);
    fdScriptEnd();
}

// The general storekeeper: across the counter, the store; from elsewhere, her wish for imported
// goods.
void scApojiiIslandsTalkToGeneralStorekeeper(Sint32 actor)
{
    if (FACING_NEAREST_CARDINAL(fdActorGet(MainRoutineStateWork.leaderActorId)->facing)
        == FACING_NORTH)
    {
        mnShopRun(SHOP_APOJII, actor);
        return;
    }
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    syFieldScriptSetMessageId((Sint32)&MsgItDBe);
    // "It'd be nice if we could import some items that our visitors would like."
    fdScriptShowMessage(actor, 0);
    fdScriptEnd();
}

// The islet's HBlank split: above ApojiiIslandsSplitLine BG1 scrolls with the map and blending is
// off; below it, BG1 drifts and is blended.
void scScrollHBlank(void)
{
    Uint32 value;

    if (REG_VCOUNT < ApojiiIslandsSplitLine)
    {
        value       = ApojiiIslandsUpperScrollX;
        REG_BG1HOFS = value;
        REG_BLDCNT  = 0;
    }
    else
    {
        value = ApojiiIslandsLowerScrollX;
        syIOWrite16(value, &REG_BG1HOFS);
        REG_BLDALPHA = BLEND_ALPHA(12, BLEND_WEIGHT_MAX);
        REG_BLDCNT   = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    }
}

// Each tick, follows the overhead layer's scroll for the split, the water below it drifting west a
// pixel every fourth tick.
void scLayerTrackScroll(void)
{
    MAP_LAYER *layer       = &((MAP_STATE *)RamHeapSlots->mapGrid)->layers[MAPGRID_LAYER_OVERHEAD];
    Sint16     y           = FIXEDtoINT(layer->y);

    ApojiiIslandsSplitLine = 1448 - y;
    ApojiiIslandsUpperScrollX = FIXEDtoINT(layer->x);
    ApojiiIslandsLowerScrollX = (Sint16)FIXEDtoINT(layer->x) - (TickCount >> 2);
}

#include "shared/scroll_start_hblank.h"

// Sets the rooms' wading and their window transition, the houses fading from the backdrop instead,
// then each room's own setup: the islet's, and in the village the sea's cycle and the father's
// surprise once Douse has woken the Aqua Stone.
Sint32 scInit(void)
{
    Sint16              scene;
    MAIN_ROUTINE_STATE *party;

    fdMapSetUpperLevelWading(RamHeapSlots->mapGrid);
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
    party                      = &MainRoutineStateWork;
    scene                      = party->next;
    if (scene == SCENE_APOJII_ISLANDS_HAZE)
    {
        scApojiiIslandsInitHaze();
    }
    else if (scene == SCENE_APOJII_ISLANDS)
    {
        syFieldSetTickEvent(scApojiiIslandsCycleSeaColors, EVENT_PASS_TICK);
        if (fdScriptGetFlag(FLAG_FATHER_SEES_THE_STONE))
            scApojiiIslandsFatherSeesTheStone();
    }
    else if (scene == SCENE_APOJII_ISLANDS_SHOPS)
    {
        FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        if (party->entrance == APOJII_ISLANDS_SHOPS_FROM_SEA_OF_TIME_ISLET)
            scApojiiIslandsEnterSeaOfTimeIsletHouse();
    }
    return 0;
}

Sint32 scPrepareMap(void)
{
    return 0;
}

// Haze's islet on entry: the falls' colours cycle, the rope is placed, tied or not, the water
// sounds, and the leader rises from the water when he comes up from the cave.
void scApojiiIslandsInitHaze(void)
{
    sySetTickEvent(scApojiiIslandsCycleFallsColors, EVENT_PASS_TICK);
    fdRopeInit();
    fdRopePlace(0, FLAG_HAZE_ROPE_TIED, ACTOR_FALLS_ROPE_POST, ACTOR_CLIFF_ROPE_POST);
    fdQueueMusic(SE_AMBIENT_WATER);
    scScrollStartHBlank();
    if (MainRoutineStateWork.entrance == APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_CAVE_SOUTHWEST)
        scLeaderRiseFromWater();
    if (MainRoutineStateWork.entrance == APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_CAVE_SOUTHEAST)
        scLeaderRiseFromWater();
    if (MainRoutineStateWork.entrance == APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_CAVE_NORTH)
        scLeaderRiseFromWater();
}

// The Sea of Time Islet's house, drawn beside the houses, entered from that islet: the window opens
// round the leader and the Apojii Islands' name is not shown.
void scApojiiIslandsEnterSeaOfTimeIsletHouse(void)
{
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    fdScriptClearFlag(FLAG_SHOW_AREA_NAME);
}

// Felix jumps across the stream north of the hut by the Aqua Stone's pool when he walks north into
// its bank (metatiles (29, 12) and (30, 12)), landing on the grass beyond. The field engine's
// `fdTryJumpForward` makes the jump. The second metatile is one of the weeds' metatiles, and
// walking north into weeds anywhere in the village tries the same jump; away from the stream it
// starts nothing: walking north into the weeds by the hut, metatile (26, 19), or at (11, 16), Felix
// only walks on, with no jump.
void scApojiiIslandsJumpForward(void)
{
    fdTryJumpForward();
}

#include "shared/actor_spawn_wading_splash.h"

#include "shared/actor_emit_ripples.h"

// Unused here: Aqua Rock's stream carries the leader south, splashing, and off a ledge he drops
// with the water (AquaRockFlowWithStream).
UNUSED void scApojiiIslandsFlowWithStream(void)
{
    FIELD_STATE *state  = RamHeapSlots->fieldState;
    Sint32       owner  = MainRoutineStateWork.leaderActorId;
    ACTOR       *anchor = state->camera;
    ACTOR       *leader = fdActorGet(owner);
    FIXED        pos[3];
    Sint32       result;

    if (MainRoutineStateWork.next != SCENE_AQUA_ROCK_MOAI || leader->groundHeight != 0)
    {
        pos[0] = leader->x.fixed;
        pos[1] = leader->y.fixed;
        pos[2] = leader->z.fixed + toFIXED(2);
        result = fdMapGridCheckCollision(leader, pos);
        if ((TickCount & 4) == 0)
            scActorSpawnWadingSplash(leader);
        if (result < 0)
        {
            fdActorSetAttachedEffect(owner, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
            scFieldActorSetMoveTargetFX(
                leader, leader->x.fixed, leader->y.fixed, leader->z.fixed + toFIXED(8));
            fdActorSelectAnimation(leader, ANIM_LEADER_SIT);
            scFieldActorWaitForMove(leader);
            do
            {
                syWait(1);
            } while (leader->y.fixed != leader->groundHeight);
            scActorSpawnWadingSplash(leader);
            fdActorSelectAnimation(leader, ANIM_LEADER_SIT);
            syWait(3);
        }
        else
        {
            pos[0] = leader->x.fixed;
            pos[1] = leader->y.fixed;
            pos[2] = leader->z.fixed + toFIXED(8);
            result = fdMapGridCheckCollision(leader, pos);
            if (result <= 0)
            {
                pos[0] = leader->x.fixed + (toFIXED(57) / 10);
                pos[1] = leader->y.fixed;
                pos[2] = leader->z.fixed + (toFIXED(57) / 10);
                result = fdMapGridCheckCollision(leader, pos);
                if (result <= 0)
                {
                    pos[0] = leader->x.fixed - (toFIXED(57) / 10);
                    pos[1] = leader->y.fixed;
                    pos[2] = leader->z.fixed + (toFIXED(57) / 10);
                    result = fdMapGridCheckCollision(leader, pos);
                    if (result <= 0)
                    {
                        anchor->z.fixed += toFIXED(2);
                        leader->z.fixed += toFIXED(2);
                        leader->facing   = FACING_SOUTH;
                    }
                }
            }
        }
    }
}

#include "shared/moai_add_water.h"

// The Aqua Rock room's actor the copy below splashes at, as aqua_rock_inside.c names it.
enum
{
    ACTOR_VIAL_CHEST_PUDDLE_C = ACTOR_FIRST_PLACED + 7
};

#include "shared/leader_spin_away.h"

#include "shared/actor_emit_mist.h"

#include "shared/leader_sink_away.h"

#include "shared/leader_rise_from_water.h"

// The markers' pictures, three of the map's animated ones: from tile 1408, counted from background
// character block 1 as the map's tile animation counts, each two tiles wide over two rows of 32
// tiles.
#define MARKER_SOURCE_TILE   1408
// The tiles of a 16 by 16 marker picture in sprite VRAM: its upper half's two, then its lower
// half's.
#define MARKER_PICTURE_TILES 4

// Copies two tiles of the map's pictures to the markers' tiles in sprite VRAM.
#define COPY_TILES(fromTile, toTile)                                                               \
    syDMAXCopy((void *)(BG_CHAR_BLOCK(1) + (fromTile) * TILE_SIZE_4BPP),                           \
               (void *)(offset + OBJ_VRAM0 + (toTile) * TILE_SIZE_4BPP),                           \
               DMA_COPY_WORDS(2 * TILE_SIZE_4BPP / 4),                                             \
               REG_DMA3)
// Copies the map's picture `from` to marker picture `to`, upper half first.
#define COPY_PICTURE(from, to)                                                                     \
    (COPY_TILES(MARKER_SOURCE_TILE + (from) * 2, (to) * MARKER_PICTURE_TILES),                     \
     COPY_TILES(MARKER_SOURCE_TILE + 32 + (from) * 2, (to) * MARKER_PICTURE_TILES + 2))

// Each tick: copies the markers' pictures, which the map's tile animation redraws, to sprite VRAM,
// and draws a sprite over each live marker in view, semi-transparent in the woken Aqua Stone's
// turning colours, at the lowest priority while it rests.
void scApojiiIslandsDrawMetatileMarkers(void)
{
    MAP_STATE       *map    = RamHeapSlots->mapGrid;
    Sint32           viewX  = syMapStateView(map)[0] & FIELD_PIXEL_MASK;
    Sint32           viewZ  = syMapStateView(map)[1] & FIELD_PIXEL_MASK;
    // DEAD: viewY, subtracted from both height and z, cancels in dy; the ROM keeps its load and its
    // store to the stack.
    Sint32           viewY  = map->origin[1];
    Uint32           tile   = VramCacheEntries[ApojiiIslandsMetatileMarkers.block].offset >> 5;
    Uint32           offset = VramCacheEntries[ApojiiIslandsMetatileMarkers.block].offset;
    METATILE_MARKER *record;
    Sint32           i;

    // Kinds 2 and 3 show one picture, kind 2 mirrored.
    COPY_PICTURE(0, 0);
    COPY_PICTURE(1, 1);
    COPY_PICTURE(2, 2);
    COPY_PICTURE(2, 3);
    record = ApojiiIslandsMetatileMarkers.markers;
    for (i = METATILE_MARKERS - 1; i >= 0; i--, record++)
    {
        if (record->state != 0)
        {
            Sint32  dx      = record->x - viewX;
            Sint32  height  = record->y - viewY;
            Sint32  z       = record->z - viewZ - viewY;
            Sint32  dy      = z - height;
            Sint32  palette = record->y == record->rest && record->state != 2
                                ? OBJ_PALETTE(STONE_SPRITE_PALETTE) | OBJ_PRIORITY(3)
                                : OBJ_PALETTE(STONE_SPRITE_PALETTE) | OBJ_PRIORITY(2);
            Sint32  y       = FIXEDtoINT(dy);
            Uint32  link    = 0;
            Sint32  x       = FIXEDtoINT(dx);
            Sint32  left    = x - 8;
            Sint32  top     = y - 8;
            Sint32  flip, kind, picture;
            Uint32 *attribute;

            if ((Uint32)(x + 7) > 334)
                continue;
            if ((Uint32)(y + 39) > 286)
                continue;
            left                  &= OBJ_X_MASK;
            // FAKEMATCH(dead): a self-update of RamHeapSlots->mapGrid after
            // * left's mask.
            RamHeapSlots->mapGrid += 0;
            top                   &= OBJ_Y_MASK;
            attribute              = record->oam;
            kind                   = record->kind;
            picture                = kind * MARKER_PICTURE_TILES;
            flip                   = 0;
            if (kind == 2)
                flip = OBJ_HFLIP;
            *attribute++ = link;
            *attribute++ =
                top | OBJ_X(left) | flip | OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
            *attribute = palette | (tile + picture);
            syOAMAddSpriteAtDepth(record->oam, 0);
        }
    }
}

// Clears the markers, takes a free VRAM cache entry for their four pictures, left empty, and starts
// drawing them each tick.
void scApojiiIslandsStartMetatileMarkers(void)
{
    syWordsClearEntry(&ApojiiIslandsMetatileMarkers, sizeof(METATILE_MARKER_SET));
    ApojiiIslandsMetatileMarkers.block = syVRAMCacheFindFreeEntry();
    mnVRAMLoad(ApojiiIslandsMetatileMarkers.block,
               METATILE_MARKER_KINDS * MARKER_PICTURE_TILES * TILE_SIZE_4BPP,
               0);
    syFieldSetTickEvent(scApojiiIslandsDrawMetatileMarkers, EVENT_PASS_TICK);
}

// Adds a marker over each metatile of layer 1 whose collision code is one of the
// METATILE_MARKER_KINDS from `base`, at `level` and resting at `restLevel`, each in steps of 8
// pixels, and gives those metatiles MAP_SHAPE_MARKED, its entry set to `level`.
void scApojiiIslandsAddMetatileMarkers(Uint32 base, Sint32 level, Sint32 restLevel)
{
    MAP_STATE           *map       = RamHeapSlots->mapGrid;
    MAP_METATILE        *metatiles = fdMapGridGetLayer(MAPGRID_LAYER_MIDDLE)->metatiles;
    Sint32               left      = fdMapGridGetLayer(MAPGRID_LAYER_GROUND)->offsetX + map->minX;
    Sint32               top       = fdMapGridGetLayer(MAPGRID_LAYER_GROUND)->offsetY + map->minZ;
    // FAKEMATCH(temporary): markers, a pointer standing in for ApojiiIslandsMetatileMarkers's
    // address; as the table by name.
    METATILE_MARKER_SET *markers   = &ApojiiIslandsMetatileMarkers;
    Sint32               width     = FIELD_METATILE(map->maxX - map->minX);
    Sint32               height    = FIELD_METATILE(map->maxZ - map->minZ);
    METATILE_MARKER     *entry     = &markers->markers[markers->count];
    // FAKEMATCH(temporary): slot, a pointer standing in for the marked shape's
    // * entry; as the entry written through its cast address.
    Sint32              *slot      = (Sint32 *)(MapGridShapes + MAP_SHAPE_MARKED * 4);
    Sint32               origin    = (FIELD_METATILE(map->minZ) << 7) + FIELD_METATILE(map->minX);
    MAP_METATILE        *metatile;
    Uint16               x, z;
    Uint32               value;

    *slot      = level << 8;
    metatiles += origin;
    for (z = 0; z < height; z++)
    {
        metatile = metatiles + (z << 7);
        for (x = 0; x < width; x++)
        {
            value = metatile->collisionCode;
            if (value != 0 && value >= base && value < base + METATILE_MARKER_KINDS)
            {
                entry->x     = FIELD_METATILE_ORIGIN(x) + left + toFIXED(8);
                entry->z     = FIELD_METATILE_ORIGIN(z) + top + toFIXED(8);
                entry->rest  = restLevel * toFIXED(8);
                entry->y     = level * toFIXED(8);
                entry->kind  = value - base;
                entry->state = 1;
                entry++;
                markers->count++;
                {
                    Uint8 *terrain        = map->layers[0].terrainIds + origin;

                    terrain[x + (z << 7)] = MAP_SHAPE_MARKED;
                }
            }
            metatile++;
        }
    }
}

extern Sint16 GroundParticleFrames[];

#include "shared/ground_particles_draw.h"

#include "shared/ground_particles_start.h"

#include "shared/ground_particles_set_anchor.h"

// clang-format off
#include "shared/field_effect_duration_scripts.c"

// A splash's script: it shows for 20 ticks, then is released.
Uint32 WadingSplashScript[] = {
    SC_WAIT(20),
    SC_RELEASE,
};

#include "shared/ripple_script.c"

#include "shared/ground_particle_frames.c"

const Sint32 *FieldEffectDurationScripts[] = {
    FieldEffectShortScript,
    FieldEffectMediumScript,
    FieldEffectLongScript,
};

// The cautious mother paces her house: a step east and up to the north wall, a pause, then back to her place, a pause.
Sint32 ApojiiIslandsPaceScript[] = {
    SC_SET(SW_COLLISION, PACER_COLLISION),
    SC_ACCELERATION(0.2),
    SC_SPEED(0.6),
    SC_LABEL(1),
    SC_ANIMATION(2),
    SC_MOVE(296, 0, 136),
    SC_SYNC,
    SC_MOVE(296, 0, 104),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(60),
    SC_WAIT(10),
    SC_WAIT(60),
    SC_ANIMATION(2),
    SC_MOVE(296, 0, 136),
    SC_SYNC,
    SC_MOVE(280, 0, 136),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(60),
    SC_WAIT(10),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
    SCENE_EXITS(ApojiiIslands),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_WORLD_MAP_A, WorldMap, WORLD_MAP_FROM_APOJII_ISLANDS, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_APOJII_ISLANDS_HAZE_NORTH, ApojiiIslandsHaze, APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_APOJII_ISLANDS_HAZE_SOUTH, ApojiiIslandsHaze, APOJII_ISLANDS_HAZE_FROM_APOJII_ISLANDS_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_SOUTHEAST, ApojiiIslandsShops, APOJII_ISLANDS_SHOPS_FROM_APOJII_ISLANDS_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_SOUTHWEST, ApojiiIslandsShops, APOJII_ISLANDS_SHOPS_FROM_APOJII_ISLANDS_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_EAST, ApojiiIslandsShops, APOJII_ISLANDS_SHOPS_FROM_APOJII_ISLANDS_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_WEST, ApojiiIslandsShops, APOJII_ISLANDS_SHOPS_FROM_APOJII_ISLANDS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_NORTHWEST, ApojiiIslandsShops, APOJII_ISLANDS_SHOPS_FROM_APOJII_ISLANDS_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_TO_MIKASALLA_ROOMS_SANCTUM, MikasallaRoomsSanctum, MIKASALLA_ROOMS_SANCTUM_FROM_APOJII_ISLANDS, EXIT_FLAG_NONE),
    SCENE_EXIT(APOJII_ISLANDS_TO_WORLD_MAP_AQUA_ROCK, WorldMap, WORLD_MAP_CARRIED_TO_AQUA_ROCK),
    SCENE_EXITS(ApojiiIslandsShops),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_SOUTHEAST, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_SHOPS_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_SOUTHWEST, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_SHOPS_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_EAST, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_SHOPS_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_WEST, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_SHOPS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_NORTHWEST, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_SHOPS_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_SHOPS_TO_SEA_OF_TIME_ISLET, SeaOfTimeIslet, SEA_OF_TIME_ISLET_FROM_APOJII_ISLANDS_SHOPS, EXIT_FLAG_NONE),
    SCENE_EXITS(ApojiiIslandsHaze),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_NORTH, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_HAZE_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_SOUTH, ApojiiIslands, APOJII_ISLANDS_FROM_APOJII_ISLANDS_HAZE_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_CAVE_SOUTHWEST, ApojiiIslandsCave, APOJII_ISLANDS_CAVE_FROM_APOJII_ISLANDS_HAZE_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_CAVE_SOUTHEAST, ApojiiIslandsCave, APOJII_ISLANDS_CAVE_FROM_APOJII_ISLANDS_HAZE_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_CAVE_NORTH, ApojiiIslandsCave, APOJII_ISLANDS_CAVE_FROM_APOJII_ISLANDS_HAZE_NORTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

// The scene's other rooms' actors: none.
ACTOR_PLACE ApojiiIslandsPlacements[] = {
    {SCENE_TABLE_END},
};

// The village's actors, in APOJII_ISLANDS_VILLAGE_ACTOR_ID's order.
ACTOR_PLACE ApojiiIslandsVillagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 168, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 88, FACING_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 312, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 504, 0, 200, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 264, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 184, 0, 152, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 216, 0, 376, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 104, 0, 344, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 280, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WATER_DROP_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// The houses' actors, in APOJII_ISLANDS_HOUSES_ACTOR_ID's order.
ACTOR_PLACE ApojiiIslandsShopsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 152, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 104, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 104, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, (Sint32)ApojiiIslandsPaceScript, 280, 0, 136, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 344, 0, 120, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 592, 0, 120, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_TEAL_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416, 0, 360, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 360, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 376, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 128, 0, 620, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

// Haze's islet's actors, in APOJII_ISLANDS_HAZE_ACTOR_ID's order.
ACTOR_PLACE ApojiiIslandsHazePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_HAZE), MOVEMENT_TYPE_STAND, 376, 0, 200, FACING_WEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

// The scene's other rooms' cues: none.
CUE ApojiiIslandsCues[] = {
    {CUE_TABLE_END},
};

// Ticks the falls' cycle has run; -1 until it loads the palettes.
Sint32 ApojiiIslandsFallsCycleTick = -1;

// Ticks the sea's cycle has run; -1 until it loads the palettes.
Sint32 ApojiiIslandsSeaCycleTick = -1;

// Ticks the Aqua Stone's cycle has run; -1 until it loads the palette.
Sint32 ApojiiIslandsStoneCycleTick[] = {
    -1,
};

// The village's exits, doors, talks and Psynergy.
CUE ApojiiIslandsVillageCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, APOJII_ISLANDS_TO_WORLD_MAP_A},
    {CUE_EXIT, 2, CONDITION_ALWAYS, APOJII_ISLANDS_TO_APOJII_ISLANDS_HAZE_NORTH},
    {CUE_EXIT, 3, CONDITION_ALWAYS, APOJII_ISLANDS_TO_APOJII_ISLANDS_HAZE_SOUTH},
    {CUE_STEP, APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_SOUTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsEnterDoor)},
    {CUE_STEP, APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_SOUTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsEnterDoor)},
    {CUE_STEP, APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_EAST, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsEnterDoor)},
    {CUE_STEP, APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_WEST, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsEnterDoor)},
    {CUE_STEP, APOJII_ISLANDS_TO_APOJII_ISLANDS_SHOPS_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsEnterDoor)},
    {CUE_EXIT, 9, CONDITION_ALWAYS, APOJII_ISLANDS_TO_MIKASALLA_ROOMS_SANCTUM},
    {STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_STREAM_BANK, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsJumpForward)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_WEEDS, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsJumpForward)},
    {CUE_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgThisIsApojii)},
    {CUE_TALK, ACTOR_WOMAN_WARNING_OF_THE_EDGE, CONDITION_ALWAYS, MSGNO(MsgWhateverYouDo)},
    {CUE_TALK, ACTOR_MAN_ABOUT_GAIA_FALLS, CONDITION_ALWAYS, MSGNO(MsgTheHugeWaterfall)},
    {CUE_TALK, ACTOR_WOMAN_ABOUT_THE_MOAI, CONDITION_ALWAYS, MSGNO(MsgOnAquaIsland)},
    {CUE_TALK, ACTOR_OLD_MAN_AT_THE_EDGE_OF_THE_WORLD, CONDITION_ALWAYS, MSGNO(MsgHaveYouAny)},
    {CUE_TALK, ACTOR_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgIDonTKnowWhat)},
    {CUE_TALK, ACTOR_BOY_COMING_OF_AGE, CONDITION_ALWAYS, MSGNO(MsgWhenTheyCome)},
    {CUE_TALK, ACTOR_GIRL_ABOUT_GAIA_FALLS, CONDITION_ALWAYS, MSGNO(MsgDoYouThink)},
    {CUE_TALK, ACTOR_FATHER, CONDITION_ALWAYS, MSGNO(MsgIfThereS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgLookingOverGaia)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARNING_OF_THE_EDGE, CONDITION_ALWAYS, MSGNO(MsgTheCurrentsNear)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ABOUT_GAIA_FALLS, CONDITION_ALWAYS, MSGNO(MsgAllOfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ABOUT_THE_MOAI, CONDITION_ALWAYS, MSGNO(MsgWhyWouldAnyone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_AT_THE_EDGE_OF_THE_WORLD, CONDITION_ALWAYS, MSGNO(MsgWellAtLeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgApojiiHasSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_COMING_OF_AGE, CONDITION_ALWAYS, MSGNO(MsgAquaIslandIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ABOUT_GAIA_FALLS, CONDITION_ALWAYS, MSGNO(MsgIWonderWhere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHER, CONDITION_ALWAYS, MSGNO(MsgAquaRockMust)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_AQUA_STONE, FLAG_APOJII_AQUA_STONE_DOUSED, SCRIPT_REF(scApojiiIslandsDouseAquaStone)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsSinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsRiseFromSand)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsPauseSeaColors)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsResumeSeaColors)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_WEEDS, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsScatterWeeds)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_WEEDS_OVER_HERB, CONDITION_ALWAYS, SCRIPT_REF(ApojiiIslandsScatterWeedsOverHerb)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_WEEDS_OVER_MINT, CONDITION_ALWAYS, SCRIPT_REF(ApojiiIslandsScatterWeedsOverMint)},
    {CUE_TABLE_END},
};

// The houses' exits, doors, talks and Psynergy.
CUE ApojiiIslandsShopsCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_SOUTHEAST},
    {CUE_EXIT, 2, CONDITION_ALWAYS, APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_SOUTHWEST},
    {CUE_EXIT, 3, CONDITION_ALWAYS, APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_EAST},
    {CUE_EXIT, 4, CONDITION_ALWAYS, APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_WEST},
    {CUE_EXIT, 5, CONDITION_ALWAYS, APOJII_ISLANDS_SHOPS_TO_APOJII_ISLANDS_NORTHWEST},
    {CUE_EXIT, 6, CONDITION_ALWAYS, APOJII_ISLANDS_SHOPS_TO_SEA_OF_TIME_ISLET},
    {CUE_TALK, ACTOR_WOMAN_ABOUT_THE_STARING_MAN, CONDITION_ALWAYS, MSGNO(MsgThereSThis)},
    {CUE_TALK, ACTOR_SON, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsTalkToSon)},
    {CUE_TALK, ACTOR_STRICT_FATHER, CONDITION_ALWAYS, MSGNO(MsgIAbsolutelyRefuse)},
    {CUE_TALK, ACTOR_CAUTIOUS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgTheCurrentsAround)},
    {CUE_TALK, ACTOR_BOY_KEPT_FROM_THE_OCEAN, CONDITION_ALWAYS, MSGNO(MsgIMNotAllowed)},
    {CUE_TALK, ACTOR_GIRL_CALLING_FELIX_A_HERO, CONDITION_ALWAYS, MSGNO(MsgHeyMisterDid)},
    {CUE_TALK, ACTOR_OLD_MAN_ABOUT_THE_AQUA_STONE, CONDITION_ALWAYS, MSGNO(MsgThatOldAqua)},
    {CUE_TALK, ACTOR_OLD_WOMAN_ABOUT_HER_HUSBANDS_DREAM, CONDITION_ALWAYS, MSGNO(MsgHeSToo)},
    {CUE_TALK, ACTOR_GENERAL_STOREKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsTalkToGeneralStorekeeper)},
    {CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsTalkToInnkeeper)},
    {CUE_TALK, ACTOR_OLD_WOMAN_AT_THE_INN, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsAskAboutWarriors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ABOUT_THE_STARING_MAN, CONDITION_ALWAYS, MSGNO(MsgIJustWish)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SON, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsMindReadSon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRICT_FATHER, CONDITION_ALWAYS, MSGNO(MsgISinkLike)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CAUTIOUS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgPoorBoyHis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_KEPT_FROM_THE_OCEAN, CONDITION_ALWAYS, MSGNO(MsgItSStupid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_CALLING_FELIX_A_HERO, CONDITION_ALWAYS, MSGNO(MsgWowHeSwam)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ABOUT_THE_AQUA_STONE, CONDITION_ALWAYS, MSGNO(MsgYouUnlockThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ABOUT_HER_HUSBANDS_DREAM, CONDITION_ALWAYS, MSGNO(MsgHeAlwaysWanted)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GENERAL_STOREKEEPER, CONDITION_ALWAYS, MSGNO(MsgNoOneShips)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgIMSure)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgThoseGuysOn)},
    {CUE_TALK, ACTOR_OLD_MAN_ABOUT_THE_WARMING_SEA, CONDITION_ALWAYS, MSGNO(MsgTheTemperatureOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ABOUT_THE_WARMING_SEA, CONDITION_ALWAYS, MSGNO(MsgLongAgoPoseidon)},
    {CUE_TABLE_END},
};

// Haze's islet's ways into the water, its ledge, Haze, Lash on the rope and Cyclone on the weeds. Each way down into the water to the cave is the metatile behaviour of the exit it leaves by, which scLeaderSinkAway takes.
CUE ApojiiIslandsHazeCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_NORTH},
    {CUE_EXIT, 2, CONDITION_ALWAYS, APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_SOUTH},
    {CUE_STEP, APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_CAVE_SOUTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scLeaderSinkAway)},
    {CUE_STEP, APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_CAVE_SOUTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scLeaderSinkAway)},
    {CUE_STEP, APOJII_ISLANDS_HAZE_TO_APOJII_ISLANDS_CAVE_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scLeaderSinkAway)},
    {STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_HAZE_LEDGE_EDGE, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsJumpDownLedge)},
    {CUE_TALK, ACTOR_HAZE, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsHazeJoinsParty)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_FALLS_ROPE_POST, FLAG_HAZE_ROPE_TIED, 0},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_WEEDS, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsScatterHazeWeeds)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_WEEDS_OVER_BRAMBLE_SEED, CONDITION_ALWAYS, SCRIPT_REF(scApojiiIslandsScatterWeedsOverBrambleSeed)},
    {CUE_TABLE_END},
};
#undef COPY_PICTURE
#undef COPY_TILES
#undef PACER_COLLISION
#undef DEFINE_SCATTER_WEEDS_OVER_TREASURE
#undef SCATTER_WEEDS
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY

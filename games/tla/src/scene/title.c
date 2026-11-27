// The title: the publisher's and Camelot's logos, the intro, and the starts it leads to, the
// prologue with Jenna or the game with Felix. TITLE_STORY_CRAWL plays the story crawl a new game
// opens with, over Vale in the rain, and TITLE_TO_BE_CONTINUED, which nothing reaches, keeps The
// Broken Seal's Golden Sun logo with "To Be Continued" revealed under it.

// This unit declares its own form of mnTitleShowCamelotLogo, below.
#define OWN_mnTitleShowCamelotLogo

#include "field/script_inline.h"
#include "field/field.h"
#include "field/field_mode_script.h"
#include "game/game.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "scene/title.h"

extern Sint16 TitleVramBlock;

static void   scTitleRevealScreen(Sint32 unused);
static void   scTitleLoadBackground(void);

// One sprite of the logo's lettering, as syOAMAddSpriteAtDepthClamped queues it.
// scTitleUpdateSpriteReveal rewrites every entry each tick.
typedef struct TITLE_SPR
{
    // [0] the queue link the depth queue writes; [1] OAM attribute 0 (y, mode and shape) in the low
    // half and attribute 1 (x) in the high half; [2] attribute 2 (the tile, and the palette).
    Uint32 attribute[3];
} TITLE_SPR;

// Ticks since the lettering's reveal began; a column appears every two.
extern Sint16         TitleRevealTick;
// The lettering's 18 sprites, in the overlay's BSS (scene/title_bss.s).
extern TITLE_SPR      TitleSprites[];

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
    return ActorPlacements;
}

const CUE *scGetCues(void)
{
    return SceneCues;
}

// The ROM's caller here is compiled against this form, which takes an argument and returns nothing
// where the definition takes none and returns an Sint32.
void   mnTitleShowCamelotLogo(Sint32 unused);

// The overlay's initialize entry, which plays what the entrance asks for.
Sint32 scInit(void)
{
    Sint32 waitTicks;

    if (MainRoutineStateWork.entrance == TITLE_TO_BE_CONTINUED)
    {
        fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlags = 0;
        fdScriptPlaySound(MUS_TITLE_AFTER_PROLOGUE);
        scTitleRevealScreen(0);
        // After 120 ticks, the logo waits up to 3600 ticks (a minute) for a key press.
        sySynch(120);
        for (waitTicks = 0; waitTicks < 3600; waitTicks++)
        {
            sySynch(1);
            if (PerWork.push != 0)
                break;
        }
        fdScriptSetDestination(SCENE_TITLE, TITLE_PROLOGUE);
        return 0;
    }
    if (MainRoutineStateWork.entrance == TITLE_STORY_CRAWL)
    {
        fdScriptPlaySound(MUS_THE_BROKEN_SEAL);
        // "Ages ago, or so the stories / tell, the power of Alchemy..."
        mnCreditsRun(CREDITS_MODE_STORY_CRAWL);
        fdScriptPlaySound(SOUND_FADE_OUT_BGM);
        syScreenFadeToBlack(60);
        syScreenFadeWait();
        fdScriptWait(240);
        fdScriptPlaySound(MUS_SILENCE);
        fdScriptSetDestination(SCENE_TITLE, TITLE_PROLOGUE);
        return 0;
    }
    syResourceReservedNoOperation(RESOURCE_TITLE_STUBS);
    if (MainRoutineStateWork.entrance == TITLE_START)
    {
        for (;;)
        {
            fdScriptPlaySound(MUS_SILENCE);
            mnTitleShowPublisherLogo(0);
            mnTitleShowCamelotLogo(0);
            // With no save file the intro is skipped. Left alone, the title fades out and the logos
            // play again after two seconds, or a key press.
            if (mnSaveScanFileSummaries() > 0)
            {
                fdScriptPlaySound(MUS_MAIN_THEME_TITLE);
                mnTitleIntroRun(1);
                if (mnTitleShowTitleScreen(1) == 0)
                {
                    fdScriptPlaySound(SOUND_FADE_OUT_BGM);
                    syScreenFadeToBlack(30);
                    syScreenFadeWait();
                    for (waitTicks = 0; waitTicks <= 119; waitTicks++)
                    {
                        sySynch(1);
                        if (PerWork.push != 0)
                            break;
                    }
                    continue;
                }
            }
            break;
        }
        fdScriptSetDestination(SCENE_CLEAR, CLEAR_FROM_TITLE);
    }
    else
    {
        if (MainRoutineStateWork.entrance == TITLE_PROLOGUE)
        {
            fdScriptPlaySound(MUS_MAIN_THEME_TITLE);
            mnTitleIntroRun(0);
            syFieldWindowColorApply(MainRoutineStateWork.windowHue,
                                    MainRoutineStateWork.windowBrightness);
            gmPartySetLeaderJenna();
            fdScriptSetDestination(SCENE_VENUS_LIGHTHOUSE, VENUS_LIGHTHOUSE_FROM_TITLE);
        }
        if (MainRoutineStateWork.entrance == TITLE_FROM_IDEJIMA)
        {
            fdScriptPlaySound(MUS_TITLE_AFTER_PROLOGUE);
            mnTitleShowTitleScreen(0);
            syFieldWindowColorApply(MainRoutineStateWork.windowHue,
                                    MainRoutineStateWork.windowBrightness);
            gmPartySetLeaderFelix();
            fdScriptSetDestination(SCENE_IDEJIMA, IDEJIMA_RIDE_THE_TIDAL_WAVE);
        }
    }
    fdScriptPlaySound(SOUND_FADE_OUT_BGM);
    syScreenFadeToBlack(30);
    syScreenFadeWait();
    fdScriptWait(60);
    fdScriptPlaySound(MUS_SILENCE);
    return 0;
}

Sint32 scPrepareMap(void)
{
    return 0;
}

// The sprite palette of the "To Be Continued" lettering.
enum
{
    TO_BE_CONTINUED_PALETTE = 15
};

// Loads the logo's lettering sprites and their palette.
void scTitleLoadSprites(Sint32 unused)
{
    Uint8           *decodedSprites;
    volatile Sint32 *dma3;

    // The decoded data opens with its 16-colour palette, then 40 4-bit tiles of lettering.
    decodedSprites = syHeapAllocateEWRAM(PALETTE_COLORS * 2 + 40 * TILE_SIZE_4BPP);
    if (TitleVramBlock == -1)
        TitleVramBlock = syVRAMCacheFindFreeEntry();
    syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_INTRO_TILES_B), decodedSprites);
    syDMAXCopy(decodedSprites,
               (void *)&OBJ_PLTT_COLOR(TO_BE_CONTINUED_PALETTE, 0),
               DMA_COPY_WORDS(PALETTE_COLORS / 2),
               REG_DMA3);
    syVRAMCacheLoad(TitleVramBlock, 40 * TILE_SIZE_4BPP, decodedSprites + PALETTE_COLORS * 2);
    dma3 = (volatile Sint32 *)REG_DMA3;
    while (dma3[2] & DMA_BUSY)
        ;
    syHeapRelease(decodedSprites);
}

// The event that queues the logo's lettering sprites each tick, revealing them from the left: those
// the reveal has reached are queued, the two newest only on even ticks.
void scTitleUpdateSpriteReveal(void)
{
    Uint32    *attributes;
    TITLE_SPR *sprite;
    Sint32     tileIndex;
    Sint32     column;
    Sint32     revealAge;
    Sint32     screenX;

    // One row of 18 semi-transparent sprites of 8 by 16 pixels at y 136 and x 88 to 224.
    sprite     = TitleSprites;
    attributes = TitleSprites[0].attribute;
    tileIndex  = VramCacheEntries[TitleVramBlock].offset >> 5;
    for (column = 0; column <= 17; column++)
    {
        Sint32 screenY = 136;

        screenX        = 232 - (18 - column) * 8;
#if FIX_POINTER_OUTSIDE_OBJECT
        // Stay inside each sprite's three-word array instead of walking into the next struct.
        attributes = TitleSprites[column].attribute;
#endif
        *attributes++ = 0;
        *attributes++ =
            OBJ_X(screenX) | screenY | OBJ_SIZE_8X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
        *attributes++ = OBJ_PALETTE(TO_BE_CONTINUED_PALETTE) | tileIndex;
        revealAge     = TitleRevealTick / 2 - column;
        if (revealAge < 0)
            revealAge = 0;
        if (revealAge <= 2 && (TickCount & 1))
            revealAge = 0;
        if (revealAge != 0)
            syOAMAddSpriteAtDepthClamped(sprite++, OAM_DEPTH_FRONT);
        tileIndex += 2;
    }
    TitleRevealTick++;
}

// Shows the Golden Sun logo for TITLE_TO_BE_CONTINUED: its "To Be Continued" lettering is revealed
// from the left while the logo brightens out of black.
static void scTitleRevealScreen(Sint32 unused)
{
    Sint32 brightnessStep;

    scTitleLoadBackground();
    fdScriptWait(30);
    TitleRevealTick = 0;
    scTitleLoadSprites(0);
    sySetEvent(scTitleUpdateSpriteReveal, EVENT_PASS_TICK);
    // Show BG0, BG2 and the sprites. BG1 to BG3 start fully darkened, so the logo on BG2 is black;
    // the semi-transparent lettering is blended onto the layers under it at full weight.
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
                        DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP,
                        IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDCNT,
                        BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 | BLDCNT_EFFECT_DARKEN
                            | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3
                            | BLDCNT_TGT2_BD,
                        IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDY, BLEND_FADE_MAX, IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH(
        (Uint32)&REG_BLDALPHA, BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX), IO_WRITE_HALFWORD);
    fdScriptWait(120);
    for (brightnessStep = 0; brightnessStep <= BLEND_FADE_MAX; brightnessStep++)
    {
        IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDY, BLEND_FADE_MAX - brightnessStep, IO_WRITE_HALFWORD);
        sySynch(3);
    }
    FieldStateWork->transition      = 0;
    FieldStateWork->transitionFrame = 1;
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    FieldStateWork->transitionFrame = 60;
}

// The title logo: its palette, its tiles decoded and copied to VRAM, a 30 by 20 tilemap of
// consecutive tiles with two blank metatiles closing each row, and the four scroll pairs cleared.
static void scTitleLoadBackground(void)
{
    Uint8     *encodedGraphics;
    Sint32     resourceId;
    Uint16    *tilemap;
    Uint32     screenX;
    Uint32     screenY;
    Uint16     tileIndex;
    BG_SCROLL *bgScroll;
    Sint32     blankTile;

    resourceId = RESOURCE_LOGO_ON_BLACK;
    syScreenFadeToBlack(0);
    REG_BG2CNT        = LOGO_BGCNT;
    BgScrollWork[2].y = 0;
    blankTile         = LOGO_BLANK_TILE;
    encodedGraphics   = syResourceGetTableEntry(resourceId);
    syDMAXCopy(encodedGraphics, (void *)BG_PLTT, DMA_COPY_WORDS(LOGO_PALETTE_COLORS / 2), REG_DMA3);
    encodedGraphics += LOGO_PALETTE_COLORS * 2;
    syFieldDecompressTaggedLZ(encodedGraphics, RamMapMetatileBuffer);
    syDMAXCopy((void *)RamMapMetatileBuffer, LOGO_TILES, DMA_COPY_WORDS(9600), REG_DMA3);
    tilemap   = LOGO_MAP;
    tileIndex = LOGO_FIRST_TILE;
    for (screenY = 0; screenY <= 19; screenY++)
    {
        for (screenX = 0; screenX <= 29; screenX++)
            *tilemap++ = tileIndex++;
        *tilemap++ = blankTile;
        *tilemap++ = blankTile;
    }
    bgScroll = BgScrollWork;
    for (screenY = 0; screenY <= 3; screenY++)
    {
        bgScroll->y = 0;
        bgScroll->x = 0;
        bgScroll++;
    }
    syDMAXCopy(BgScrollWork, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
    ((MAP_STATE *)RamHeapSlots->mapGrid)->dispcnt = DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
}

#include "shared/scene_entrances_mapless.c"

Uint32 SceneExits[] = {
    SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
    {SCENE_TABLE_END},
};

CUE SceneCues[] = {
    {CUE_TABLE_END},
};

// The VRAM cache entry the logo's lettering takes; -1 for none yet.
Sint16 TitleVramBlock = -1;

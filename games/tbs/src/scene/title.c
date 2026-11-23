/// @file
/// The title overlay holds the start screen, the Golden Sun logo shown after
/// the ending, and the screens that lead from the prologue and the last event
/// on to Vale and the File Screen.
///
/// Scene_Initialize chooses by entrance.

#include "scene/title.h"
#include "resource_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/map_grid_scroll.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "menu/title_intro.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/heap_slots.h"
#include "system/input.h"
#include "system/io_write_queue.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "system/save.h"
#include "system/system.h"
#include "system/vram_cache.h"
#include "window/screens.h"
#include "window/window.h"

extern struct SceneEntrance gScreen_Entrances[];
extern u32 gTitleExits[];
extern struct ActorPlacement gTitleActorPlacements[];
extern struct Trigger gTitleTriggers[];
extern s16 gTitleVramBlock;

static void Title_RevealScreen(s32 unused);
static void Title_LoadBackground(void);

/// The title overlay's scene tables, which FieldSequence_Run reads through
/// entry/title.s.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gScreen_Entrances;
}

/// The exit regions entry, which is empty because the title has no exit
/// boxes.
const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gTitleExits; }

/// The title overlay has no actors.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gTitleActorPlacements;
}

/// The title overlay has no triggers.
const struct Trigger *Scene_GetTriggers(void) { return gTitleTriggers; }

/// The overlay's initialize entry, which plays what the entrance asks for.
s32 Scene_Initialize(void) {
  s32 waitFrames;

  // After 120 frames, the logo waits up to 3600 frames (a minute) for a key
  // press.
  if (gMainRoutineWork.entrance == TITLE_FROM_WORLD_MAP) {
    Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags = 0;
    Sound_Play(MUS_TO_BE_CONTINUED);
    Title_RevealScreen(0);
    wait(120);
    for (waitFrames = 0; waitFrames < 3600 && gKeysPressed == 0; waitFrames++)
      wait(1);
    Event_SetDestination(SCENE_TITLE, TITLE_AFTER_RUN);
    return 0;
  }
  if (gMainRoutineWork.entrance == TITLE_FROM_LEMURIAN_SHIP) {
    Sound_Play(MUS_THE_BROKEN_SEAL_ENDING);
    // The credits: Credits_Run, through the scroll far-call
    // table.
    Import_CreditsRun(0);
    Sound_Play(SOUND_FADE_OUT_BGM);
    ScreenFade_ToBlack(60);
    ScreenFade_Wait();
    Event_Wait(240);
    Sound_Play(MUS_SILENCE);
    Event_SetDestination(SCENE_FILE_SCREEN, FILE_SCREEN_FROM_TITLE);
    return 0;
  }
  // The stub does nothing with its argument (system/system_start_up.c), the
  // far-call bank the logo and intro calls below go through.
  // XXX (no evidence): what the call once did; the stub is empty in every
  // edition, so only an earlier build of the game would show.
  Stub_NoOpHook(RESOURCE_TITLE_FAR_CALLS);
  if (gMainRoutineWork.entrance == TITLE_AFTER_RUN) {
    for (;;) {
      Sound_Play(MUS_SILENCE);
      // The Nintendo logo, then the Camelot logo (Title_ShowCamelotLogo).
      Title_ShowNintendoLogo(0);
      Import_TitleShowCamelotLogo(0);
      // With no save file (or no backup memory) there is no title intro.
      if (Save_ScanFileSummaries() > 0) {
        Sound_Play(MUS_MAIN_THEME_TITLE);
        // Title_ShowIntro with its prompt returns 0 when a minute passes
        // without A or Start: fade out and start again from the logos.
        if (Title_ShowIntro(1) == 0) {
          Sound_Play(SOUND_FADE_OUT_BGM);
          ScreenFade_ToBlack(30);
          ScreenFade_Wait();
          waitFrames = 0;
          if (gKeysHeld == 0) {
            do {
              wait(1);
              if (++waitFrames > 119)
                break;
            } while (gKeysHeld == 0);
          }
          continue;
        }
      }
      break;
    }
    Event_SetDestination(SCENE_FILE_SCREEN, FILE_SCREEN_FROM_SANCTUM);
  } else {
    // The intro without its prompt holds the title five seconds.
    Sound_Play(MUS_THE_PASSAGE_OF_TIME);
    Title_ShowIntro(0);
    Party_EndChildhood();
    Event_SetDestination(SCENE_VALE, VALE_FROM_TITLE);
    Sound_Play(SOUND_FADE_OUT_BGM);
  }
  Sound_Play(SOUND_FADE_OUT_BGM);
  ScreenFade_ToBlack(30);
  ScreenFade_Wait();
  Event_Wait(60);
  Sound_Play(MUS_SILENCE);
  return 0;
}

/// The lettering's sprite palette.
enum { LETTERING_PALETTE = 15 };
/// In the German, Spanish, French and Italian pictures, the lettering's rows at
/// the bottom of the screen.
enum { LETTERING_ROWS = 32 };

/// Loads the logo's lettering sprites and their palette.
static void Title_LoadSprites(s32 unused) {
  u8 *decodedSprites;
  volatile u32 *dma3;
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
  u8 *encodedGraphics;

  decodedSprites = Heap_AllocEwram(DISPLAY_WIDTH * DISPLAY_HEIGHT);
  encodedGraphics = Resource_GetTableEntry(RESOURCE_INTRO_TILES_B);
  if (gTitleVramBlock == -1)
    gTitleVramBlock = VramCache_FindFreeEntry();
  // The resource opens with its colours, before its compressed data.
  Dma_Set(encodedGraphics, (void *)OBJ_PLTT,
          DMA_COPY_WORDS(LOGO_PALETTE_COLORS / 2), REG_DMA3);
  encodedGraphics += LOGO_PALETTE_COLORS * 2;
  FieldDecompress_TaggedLz(encodedGraphics, decodedSprites);
  // The picture is a whole screen in 256 colours (to_be_continued_de.png and
  // the others). The lettering is its bottom LETTERING_ROWS rows: 120 sprites
  // of one 64-byte tile. The tiles of one colour above it are not loaded.
  Vram_Load(gTitleVramBlock, DISPLAY_WIDTH * LETTERING_ROWS,
            decodedSprites + DISPLAY_WIDTH * (DISPLAY_HEIGHT - LETTERING_ROWS));
#else
  // The decoded data opens with its 16-colour palette, then 40 4-bit tiles of
  // lettering.
  decodedSprites = Heap_AllocEwram(PALETTE_COLORS * 2 + 40 * TILE_SIZE_4BPP);
  if (gTitleVramBlock == -1)
    gTitleVramBlock = VramCache_FindFreeEntry();
  FieldDecompress_TaggedLz(Resource_GetTableEntry(RESOURCE_INTRO_TILES_B),
                           decodedSprites);
  Dma_Set(decodedSprites, (void *)&OBJ_PLTT_COLOR(LETTERING_PALETTE, 0),
          DMA_COPY_WORDS(PALETTE_COLORS / 2), REG_DMA3);
  Vram_Load(gTitleVramBlock, 40 * TILE_SIZE_4BPP,
            decodedSprites + PALETTE_COLORS * 2);
#endif
  dma3 = REG_DMA3;
  while (dma3[2] & DMA_BUSY)
    ;
  Heap_Release(decodedSprites);
}

/// The task that queues the logo's lettering sprites each frame,
/// revealing them from the left.
static void Title_UpdateSpriteReveal(void) {
  u32 *attributes;
  struct TitleSprite *sprite;
  s32 tileIndex;
  s32 column;
  s32 revealAge;
  s32 screenY;
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
  s32 screenX;
#endif
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
  s32 row;

  // Four rows of 30 8-by-8 sprites across the screen at y 128 to 152, in 256
  // colours and semi-transparent. A 256-colour tile is two of the 32-byte tile
  // numbers the cache offset counts in.
  sprite = gTitleSprites;
  attributes = gTitleSprites[0].attr;
  tileIndex = gVramCacheEntries[gTitleVramBlock].offset >> 5;
  row = 0;
  screenY = 128;
  for (; row <= 3; row++) {
    for (column = 0; column <= 29; column++) {
#ifdef UBFIX
      // Stay inside each sprite's three-word array instead of walking into the
      // next struct.
      attributes = sprite->attr;
#endif
      *attributes++ = 0;
      *attributes++ = OBJ_X(column * 8) | screenY | OBJ_256_COLORS |
                      OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
      *attributes++ = tileIndex;
      revealAge = gTitleRevealFrame / 2 - column;
      if (revealAge < 0)
        revealAge = 0;
      if (revealAge <= 2 && (gFrameCount & 1))
        revealAge = 0;
      if (revealAge != 0)
        Oam_AddSpriteAtDepth(sprite, OAM_DEPTH_FRONT);
      sprite++;
      tileIndex += 2;
    }
    screenY += 8;
  }
#else

  // One row of 18 semi-transparent sprites of 8 by 16 pixels at y 136 and x 88
  // to 224.
  sprite = gTitleSprites;
  attributes = gTitleSprites[0].attr;
  tileIndex = gVramCacheEntries[gTitleVramBlock].offset >> 5;
  column = 0;
  screenY = 136;
  for (; column <= 17; column++) {
    screenX = 232 - (18 - column) * 8;
#ifdef UBFIX
    attributes = gTitleSprites[column].attr;
#endif
    *attributes++ = 0;
    *attributes++ = OBJ_X(screenX) | screenY | OBJ_SIZE_8X16 |
                    OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
    *attributes++ = OBJ_PALETTE(LETTERING_PALETTE) | tileIndex;
    revealAge = gTitleRevealFrame / 2 - column;
    if (revealAge < 0)
      revealAge = 0;
    if (revealAge <= 2 && (gFrameCount & 1))
      revealAge = 0;
    if (revealAge != 0)
      Oam_AddSpriteAtDepth(sprite++, OAM_DEPTH_FRONT);
    tileIndex += 2;
  }
#endif
  gTitleRevealFrame++;
}

/// Shows the Golden Sun logo for Scene_Initialize's entrance 10.
static void Title_RevealScreen(s32 unused) {
  s32 brightnessStep;

  Title_LoadBackground();
  Event_Wait(30);
  gTitleRevealFrame = 0;
  Title_LoadSprites(0);
  Task_AddFrame(Title_UpdateSpriteReveal, TASK_PASS_FRAME);
  // Show BG0, BG2 and the sprites. BG1 to BG3 start fully darkened, so the
  // logo on BG2 is black; the semi-transparent lettering is blended onto the
  // layers under it at full weight.
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON |
                          DISPCNT_OBJ_ON,
                      IO_WRITE_HALFWORD);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                          BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_BG0 |
                          BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 |
                          BLDCNT_TGT2_BD,
                      IO_WRITE_HALFWORD);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, BLEND_FADE_MAX, IO_WRITE_HALFWORD);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                      BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX),
                      IO_WRITE_HALFWORD);
  Event_Wait(120);
  for (brightnessStep = 0; brightnessStep <= BLEND_FADE_MAX; brightnessStep++) {
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, BLEND_FADE_MAX - brightnessStep,
                        IO_WRITE_HALFWORD);
    wait(3);
  }
  gFieldWork->transition = 0;
  gFieldWork->transitionFrames = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  gFieldWork->transitionFrames = 60;
}

/// Loads the Golden Sun logo onto BG2 (title.h's LOGO_ map) with the
/// screen faded to black.
static void Title_LoadBackground(void) {
  u8 *encodedGraphics;
  s32 resourceId;
  u16 *tilemap;
  u32 screenX;
  u32 screenY;
  u16 tileIndex;
  struct ScrollPair *bgScroll;
  s32 offscreenTile = OFFSCREEN_TILE;

  resourceId = RESOURCE_GOLDEN_SUN_LOGO;
  ScreenFade_ToBlack(0);
  REG_BG2CNT = LOGO_BGCNT;
  gBgScroll[2].y = 0;
  encodedGraphics = Resource_GetTableEntry(resourceId);
  Dma_Set(encodedGraphics, (void *)BG_PLTT,
          DMA_COPY_WORDS(LOGO_PALETTE_COLORS / 2), REG_DMA3);
  encodedGraphics += LOGO_PALETTE_COLORS * 2;
  FieldDecompress_TaggedLz(encodedGraphics, Ram_MapGridCellBuffer);
  Dma_Set((void *)Ram_MapGridCellBuffer, LOGO_TILES, DMA_COPY_WORDS(9600),
          REG_DMA3);
  tilemap = LOGO_MAP;
  tileIndex = LOGO_FIRST_TILE;
  for (screenY = 0; screenY <= 19; screenY++) {
    for (screenX = 0; screenX <= 29; screenX++)
      *tilemap++ = tileIndex++;
    *tilemap++ = offscreenTile;
    *tilemap++ = offscreenTile;
  }
  bgScroll = (struct ScrollPair *)gBgScroll;
  for (screenY = 0; screenY <= 3; screenY++) {
    bgScroll->y = 0;
    bgScroll->x = 0;
    bgScroll++;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  ((struct MapGridScrollWork *)gMapGridWork[0])->flags =
      DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
}

#include "shared/screen_entrances.c"

u32 gTitleExits[] = {
    SCENE_EXITS_END,
};

struct ActorPlacement gTitleActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct Trigger gTitleTriggers[] = {
    {TRIGGER_TABLE_END},
};

/// The VRAM cache entry holding the lettering's tiles, which is -1 until
/// Title_LoadSprites takes one.
s16 gTitleVramBlock = -1;

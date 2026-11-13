/// @file
/// The title overlay holds the start screen, the Golden Sun logo shown after
/// the ending, and the screens that lead from the prologue and the last event
/// on to Vale and the File Screen.
///
/// Title_Run chooses by entrance.

#include "system/main_routine_work.h"
#include "system/vram_cache.h"
#include "types.h"
#include "field/scene_tables.h"

const struct SceneEntrance *Title_GetEntrances(void);
const u32 *Title_GetExits(void);
const struct ActorPlacement *Title_GetActorPlacements(void);
const struct Trigger *Title_GetTriggers(void);
s32 Title_Run(void);
extern struct SceneEntrance gTitleEntrances[];
extern u32 gTitleExits[];
extern struct ActorPlacement gTitleActorPlacements[];
extern struct Trigger gTitleTriggers[];
extern s16 gTitleVramBlock;

#include "field/event_commands.h"
#include "game/game.h"
#include "menu/title_intro.h"
#include "window/window.h"
#include "window/screens.h"
#include "system/system.h"
#include "battle/battle.h"
#include "system/input.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/heap_slots.h"
#include "system/save.h"
#include "field/event_inline.h"
#include "resource_ids.h"
#include "scene/title.h"
#include "system/io_write_queue.h"
#include "system/ram_buffer.h"
#include "field/map_grid_scroll.h"
#include "sound/sound_driver_symbols.h"

static void Title_RevealScreen(s32 unused);
static void Title_LoadBackground(void);

/// The title overlay's scene tables, which FieldSequence_Run reads through
/// entry/title.s.
const struct SceneEntrance *Title_GetEntrances(void) { return gTitleEntrances; }

/// The exit regions entry, which is empty because the title has no exit
/// boxes.
const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Title_GetExits(void) { return gTitleExits; }

/// The title overlay has no actors.
const struct ActorPlacement *Title_GetActorPlacements(void) {
  return gTitleActorPlacements;
}

/// The title overlay has no triggers.
const struct Trigger *Title_GetTriggers(void) { return gTitleTriggers; }

/// The overlay's initialize entry, which plays what the entrance asks for.
s32 Title_Run(void) {
  s32 waitFrames;

  // After 120 frames, the logo waits up to 3600 frames (a minute) for a key
  // press.
  if (gMainRoutineWork.entrance == TITLE_FROM_WORLD_MAP) {
    Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags = 0;
    Sound_Play(MUS_ENDING_LOGO);
    Title_RevealScreen(0);
    wait(120);
    waitFrames = 0;
    if (gKeysPressed == 0) {
      do {
        wait(1);
        if (++waitFrames > 3599)
          break;
      } while (gKeysPressed == 0);
    }
    Event_SetDestination(SCENE_TITLE, TITLE_AFTER_RUN);
    return 0;
  }
  if (gMainRoutineWork.entrance == TITLE_FROM_LEMURIAN_SHIP) {
    Sound_Play(MUS_ENDING);
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
  // The stub does nothing with its argument (system/system/start_up.c), the
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
        Sound_Play(MUS_TITLE);
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
    Sound_Play(MUS_TITLE_AFTER_PROLOGUE);
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

/// Loads the logo's lettering sprites and their palette.
static void Title_LoadSprites(s32 unused) {
  u8 *decodedSprites;
  volatile u32 *dma3;
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
  u8 *encodedGraphics;

  decodedSprites = Heap_AllocEwram(240 * 160);
  encodedGraphics = Resource_GetTableEntry(RESOURCE_INTRO_TILES_B);
  if (gTitleVramBlock == -1)
    gTitleVramBlock = VramCache_FindFreeEntry();
  // The resource opens with 224 colours of palette (112 words)
  // before its compressed data.
  Dma_Set(encodedGraphics, (void *)OBJ_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  encodedGraphics += 224 * 2;
  FieldDecompress_TaggedLz(encodedGraphics, decodedSprites);
  // The picture is a whole 240 x 160 screen (to_be_continued_de.png and the
  // others). The lettering is its bottom 32 rows, the last 240 * 32 bytes: 120
  // sprites of one 64-byte tile. The 240 * 128 bytes before it are the 480
  // tiles of one colour above, which nothing loads.
  Vram_Load(gTitleVramBlock, 240 * 32, decodedSprites + 240 * 128);
#else
  // The decoded data opens with its 16-colour palette (8 words), then 40
  // 4-bit tiles of lettering.
  decodedSprites = Heap_AllocEwram(32 + 40 * TILE_SIZE_4BPP);
  if (gTitleVramBlock == -1)
    gTitleVramBlock = VramCache_FindFreeEntry();
  FieldDecompress_TaggedLz(Resource_GetTableEntry(RESOURCE_INTRO_TILES_B),
                           decodedSprites);
  Dma_Set(decodedSprites, (void *)&OBJ_PLTT_COLOR(15, 0),
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  Vram_Load(gTitleVramBlock, 40 * TILE_SIZE_4BPP, decodedSprites + 32);
#endif
  dma3 = REG_DMA3;
  while (dma3[2] & (DMA_ENABLE << 16))
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
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
  s32 screenX;
#endif
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
  s32 row;

  // Four rows of 30 8-by-8 sprites across the screen at y 128 to 152:
  // 0x2400 is 256 colours and semi-transparent. A 256-colour tile is two of
  // the 32-byte tile numbers the cache offset counts in.
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
      *attributes++ = (column * 8 << 16) | screenY | 0x2400;
      *attributes++ = tileIndex;
      revealAge = gTitleRevealFrame / 2 - column;
      if (revealAge < 0)
        revealAge = 0;
      if (revealAge <= 2 && (gFrameCount & 1))
        revealAge = 0;
      if (revealAge != 0)
        Oam_AddSpriteAtDepth(sprite, 255);
      sprite++;
      tileIndex += 2;
    }
    screenY += 8;
  }
#else

  // One row of 18 sprites of 8 by 16 pixels at y 136 and x 88 to 224: 0x8400
  // is the tall shape and semi-transparent, 0xf000 palette 15.
  sprite = gTitleSprites;
  attributes = gTitleSprites[0].attr;
  tileIndex = gVramCacheEntries[gTitleVramBlock].offset >> 5;
  column = 0;
  screenY = 136;
  do {
    screenX = 232 - (18 - column) * 8;
#ifdef UBFIX
    attributes = gTitleSprites[column].attr;
#endif
    *attributes++ = 0;
    *attributes++ = (screenX << 16) | screenY | 0x8400;
    *attributes++ = 0xf000 | tileIndex;
    revealAge = gTitleRevealFrame / 2 - column;
    if (revealAge < 0)
      revealAge = 0;
    if (revealAge <= 2 && (gFrameCount & 1))
      revealAge = 0;
    if (revealAge != 0)
      Oam_AddSpriteAtDepth(sprite++, 255);
    tileIndex += 2;
  } while (++column <= 17);
#endif
  gTitleRevealFrame++;
}

/// Shows the Golden Sun logo for Title_Run's entrance 10.
static void Title_RevealScreen(s32 unused) {
  s32 brightnessStep;

  Title_LoadBackground();
  Event_Wait(30);
  gTitleRevealFrame = 0;
  Title_LoadSprites(0);
  Task_AddFrame(Title_UpdateSpriteReveal, TASK_PASS_FRAME);
  // Show BG0, BG2 and the sprites. BG1 to BG3 start fully darkened (BLDY
  // 16), so the logo on BG2 is black; the semi-transparent lettering is
  // blended onto the layers under it at full weight (BLEND_ALPHA(16, 16)).
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON |
                          DISPCNT_OBJ_ON,
                      IO_WRITE_16);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                          BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_BG0 |
                          BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 |
                          BLDCNT_TGT2_BD,
                      IO_WRITE_16);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 16, IO_WRITE_16);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(16, 16), IO_WRITE_16);
  Event_Wait(120);
  for (brightnessStep = 0; brightnessStep <= 16; brightnessStep++) {
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 16 - brightnessStep, IO_WRITE_16);
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
  s32 blankTile = LOGO_BLANK_TILE;

  resourceId = RESOURCE_GOLDEN_SUN_LOGO;
  ScreenFade_ToBlack(0);
  REG_BG2CNT = LOGO_BGCNT;
  gBgScroll[2].y = 0;
  encodedGraphics = Resource_GetTableEntry(resourceId);
  Dma_Set(encodedGraphics, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  encodedGraphics += 224 * 2;
  FieldDecompress_TaggedLz(encodedGraphics, Ram_MapGridCellBuffer);
  Dma_Set((void *)Ram_MapGridCellBuffer, LOGO_TILES, DMA_COPY_WORDS(9600),
          REG_DMA3);
  tilemap = LOGO_MAP;
  tileIndex = LOGO_FIRST_TILE;
  for (screenY = 0; screenY <= 19; screenY++) {
    for (screenX = 0; screenX <= 29; screenX++)
      *tilemap++ = tileIndex++;
    *tilemap++ = blankTile;
    *tilemap++ = blankTile;
  }
  bgScroll = (struct ScrollPair *)gBgScroll;
  for (screenY = 0; screenY <= 3; screenY++) {
    bgScroll->y = 0;
    bgScroll->x = 0;
    bgScroll++;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, (DMA_ENABLE | DMA_32BIT) << 16 | 4, REG_DMA3);
  ((struct MapGridScrollWork *)gMapGridWork[0])->flags =
      DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
}

struct SceneEntrance gTitleEntrances[] = {
    {0, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

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

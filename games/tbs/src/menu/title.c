/// @file
/// The title overlay: the start screen, the Golden Sun logo shown after the
/// ending, and the screens that lead from the prologue and the last scene on
/// to Vale and the file select screen. Title_Run chooses by warp destination.

#include "game_state.h"
#include "vram_tab.h"
#include "unit/menu/title.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/graphics/title/intro.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "global_cells.h"
#include "types.h"
#include "save_state.h"
#include "mem.h"
#include "script_layer.h"
#include "field_service.h"
#include "map_ids.h"
#include "resource_ids.h"
#include "title.h"
#include "dma.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "vram_block.h"
#include "ram_buffer.h"
#include "map_tables.h"
#include "map_grid_scroll.h"
#include "songs.h"
#include "audio_engine_symbols.h"

static void Title_RevealScreen(s32 unused);
static void Title_LoadBackground(void);

/// The title overlay's map tables, which Field_RunMap reads through
/// entry/title.s: one warp destination row, the warp, and no object event
/// templates or map events.
u8 *Title_GetWarpDests(void) { return (u8 *)gTitleWarpDests; }

/// The warp regions entry: the title has no warp boxes.
s32 MapData_ReturnZero(void) { return 0; }

u8 *Title_GetWarps(void) { return (u8 *)gTitleWarps; }

/// The title overlay has no object events.
u8 *Title_GetObjectEventTemplates(void) {
  return (u8 *)gTitleObjectEventTemplates;
}

/// The title overlay has no map events.
u8 *Title_GetEvents(void) { return (u8 *)gTitleEvents; }

/// The overlay's initialize entry: plays what the warp destination asks for,
/// sets the next destination and returns 0.
/// - Warp destination 10, after Jenna and Kraden's closing scene on the world
///   map (WorldMap_ShowEndingScene): the Golden Sun logo for two seconds, then
///   until a key is pressed or a minute passes, then warp destination 2.
/// - Warp destination 9, after the ship sails (lemurian_ship.c's SetSail): the
///   credits, then the file select screen's warp destination 2, which offers to
///   save the cleared game.
/// - Warp destination 2, at power-on (Game_Run) and after the logo: the
///   Nintendo and Camelot logos and, when a save file exists, the title intro
///   with its "Press Start" prompt; then the file select screen's warp
///   destination 1.
/// - Any other warp destination is the title shown after the prologue: the
///   intro without its prompt, then Vale's warp destination 16
///   (Party_EndChildhood).
s32 Title_Run(void) {
  s32 waitFrames;

  // After 120 frames, the logo waits up to 3600 frames (a minute) for a key
  // press.
  if (gGameState.warpDest == 10) {
    ObjectEvent_GetById(gGameState.selectedLocalId)->motionFlags = 0;
    Engine_AudioPlayCue(MUS_ENDING_LOGO);
    Title_RevealScreen(0);
    Engine_TaskWait(120);
    waitFrames = 0;
    if (gKeysPressed == 0) {
      do {
        Engine_TaskWait(1);
        if (++waitFrames > 3599)
          break;
      } while (gKeysPressed == 0);
    }
    FieldScript_SetDestination((s32)&MapId_Title, TITLE_AFTER_RUN);
    return 0;
  }
  if (gGameState.warpDest == 9) {
    Engine_AudioPlayCue(MUS_ENDING);
    // The credits: DisplayScroll_RunSlideshow, through the scroll far-call
    // table.
    ScrollFar_Entry00(0);
    Engine_AudioPlayCue(AUDIO_CUE_FADE_BGM);
    Fade_ToBlack(60);
    Fade_Wait();
    Engine_ScriptWait(240);
    Engine_AudioPlayCue(MUS_SILENCE);
    FieldScript_SetDestination((s32)&MapId_Clear, CLEAR_FROM_TITLE);
    return 0;
  }
  // The stub does nothing with its argument (runtime/runtime/system.c).
  // XXX: likely left from loading the palette far-call bank, because the
  // argument names that bank and the logo and intro calls below go through
  // it.
  RuntimeDispatch_NoOpHook((s32)&ResourceId_PaletteFarCalls);
  if (gGameState.warpDest == 2) {
    for (;;) {
      Engine_AudioPlayCue(MUS_SILENCE);
      // The Nintendo logo, then the Camelot logo (Title_ShowAnimatedSplash).
      Title_ShowSplashScreen(0);
      PaletteFar_Entry20(0);
      // With no save file (or no backup memory) there is no title intro.
      if (SaveState_ScanRecordFlags() > 0) {
        Engine_AudioPlayCue(MUS_TITLE);
        // Title_ShowIntro with its prompt returns 0 when a minute passes
        // without A or Start: fade out and start again from the logos.
        if (PaletteFar_Entry00(1) == 0) {
          Engine_AudioPlayCue(AUDIO_CUE_FADE_BGM);
          Fade_ToBlack(30);
          Fade_Wait();
          waitFrames = 0;
          if (gKeysHeld == 0) {
            do {
              Engine_TaskWait(1);
              if (++waitFrames > 119)
                break;
            } while (gKeysHeld == 0);
          }
          continue;
        }
      }
      break;
    }
    FieldScript_SetDestination((s32)&MapId_Clear, CLEAR_FROM_SANCTUM);
  } else {
    // The intro without its prompt holds the title five seconds.
    Engine_AudioPlayCue(MUS_TITLE_AFTER_PROLOGUE);
    PaletteFar_Entry00(0);
    Party_EndChildhood();
    FieldScript_SetDestination((s32)&MapId_Vale, VALE_FROM_TITLE);
    Engine_AudioPlayCue(AUDIO_CUE_FADE_BGM);
  }
  Engine_AudioPlayCue(AUDIO_CUE_FADE_BGM);
  Fade_ToBlack(30);
  Fade_Wait();
  Engine_ScriptWait(60);
  Engine_AudioPlayCue(MUS_SILENCE);
  return 0;
}

/// Loads the logo's lettering sprites and their palette. The tiles go into
/// the VRAM cache entry gTitleVramBlock, taken on the first call; the decode
/// buffer comes from the alternate pool and is freed before returning.
/// English and Japanese letter the logo with 4-bit tiles on palette 15; the
/// other editions with 256-colour tiles and a 224-colour palette.
static void Title_LoadSprites(s32 unused) {
  u8 *decodedSprites;
  volatile u32 *dma3;
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
  u8 *encodedGraphics;

  decodedSprites = Runtime_BumpAllocateAlternatePool(0x9600);
  encodedGraphics = Resource_GetTableEntry((s32)&ResourceId_IntroTilesB);
  if (gTitleVramBlock == -1)
    gTitleVramBlock = Resource_FindFreeEntry();
  // The resource opens with 0x1c0 bytes of palette (0x70 words, 224 colours)
  // before its compressed data.
  Dma_Set(encodedGraphics, (void *)OBJ_PLTT,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x70, REG_DMA3);
  encodedGraphics += 0x1c0;
  FieldResource_DecodeType01(encodedGraphics, decodedSprites);
  // The picture is a whole 240 x 160 screen (to_be_continued_de.png and the
  // others). The lettering is its bottom 32 rows, the last 0x1e00 bytes: 120
  // sprites of one 64-byte tile. The 0x7800 bytes before it are the 480 tiles
  // of one colour above, which nothing loads.
  Vram_Load(gTitleVramBlock, 0x1e00, decodedSprites + 0x7800);
#else
  // The decoded data opens with its 16-colour palette (8 words), then 0x500
  // bytes (40 4-bit tiles) of lettering.
  decodedSprites = Runtime_BumpAllocateAlternatePool(0x520);
  if (gTitleVramBlock == -1)
    gTitleVramBlock = Resource_FindFreeEntry();
  FieldResource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_IntroTilesB), decodedSprites);
  Dma_Set(decodedSprites, (void *)&OBJ_PLTT_COLOR(15, 0),
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  Vram_Load(gTitleVramBlock, 0x500, decodedSprites + 32);
#endif
  // Wait for the final DMA transfer before releasing the decode buffer.
  dma3 = REG_DMA3;
  while (dma3[2] & (DMA_ENABLE << 16))
    ;
  MemFree(decodedSprites);
}

/// The scheduler callback that queues the logo's lettering sprites each frame,
/// revealing them from the left. Column n appears once gTitleRevealFrame / 2
/// passes n, and is hidden on odd frames for its first four frames. Advances
/// gTitleRevealFrame. Title_RevealScreen schedules it, and it is not removed
/// in this overlay.
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
  tileIndex = gVramBlockCache[gTitleVramBlock].offset >> 5;
  row = 0;
  screenY = 128;
  for (; row <= 3; row++) {
    for (column = 0; column <= 29; column++) {
#ifdef UBFIX
      // Stay inside each sprite's three-word array instead of walking into
      // the next struct. The English indexed form adds four native bytes.
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
        Runtime_PushSlotEntry(sprite, 255);
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
  tileIndex = gVramBlockCache[gTitleVramBlock].offset >> 5;
  column = 0;
  screenY = 0x88;
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
      Runtime_PushSlotEntry(sprite++, 255);
    tileIndex += 2;
  } while (++column <= 17);
#endif
  gTitleRevealFrame++;
}

/// Shows the Golden Sun logo for Title_Run's warp destination 10: loads it
/// while the screen is black, starts the lettering's reveal
/// (Title_UpdateSpriteReveal, left running), brings the logo up from black over
/// 51 frames and opens the screen. Leaves the event work's transitions at 60
/// frames.
static void Title_RevealScreen(s32 unused) {
  s32 brightnessStep;

  // Load while dark, then let the sprite callback reveal the lettering.
  Title_LoadBackground();
  Engine_ScriptWait(30);
  gTitleRevealFrame = 0;
  Title_LoadSprites(0);
  FieldScheduler_AddCallback(Title_UpdateSpriteReveal, 0xc80);
  // Show BG0, BG2 and the sprites. BG1 to BG3 start fully darkened (BLDY
  // 16), so the logo on BG2 is black; the semi-transparent lettering is
  // blended onto the layers under it at full weight (BLEND_ALPHA(16, 16)).
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON |
                          DISPCNT_OBJ_ON,
                      0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                          BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_BG0 |
                          BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 |
                          BLDCNT_TGT2_BD,
                      0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 16, 0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(16, 16), 0x20000);
  Engine_ScriptWait(120);
  // Lift the darkening in 17 steps of three frames, down to BLDY 0.
  for (brightnessStep = 0; brightnessStep <= 16; brightnessStep++) {
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 16 - brightnessStep, 0x20000);
    Engine_TaskWait(3);
  }
  // Open the screen in one frame, then set 60-frame transitions for what
  // follows.
  gEventWork->startTransition = 0;
  gEventWork->transitionFrames = 1;
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  gEventWork->transitionFrames = 60;
}

/// Loads the Golden Sun logo onto BG2 (title.h's LOGO_ layout) with the
/// screen faded to black: its 224-colour palette, its 600 256-colour tiles,
/// decoded through Ram_MapGridCellBuffer, and a map that numbers them row by
/// row. Zeroes the four backgrounds' scroll and leaves the map work showing
/// only BG2 and the sprites. Clear_LoadBackground (clear.c) is its twin.
static void Title_LoadBackground(void) {
  u8 *encodedGraphics;
  s32 resourceId;
  u16 *tilemap;
  u32 screenX;
  u32 screenY;
  u16 tileIndex;
  struct ScrollPair *bgScroll;
  s32 blankTile;

  resourceId = (s32)&ResourceId_GoldenSunLogo;
  Fade_ToBlack(0);
  REG_BG2CNT = LOGO_BGCNT;
  gBgScroll[2].y = 0;
  blankTile = LOGO_BLANK_TILE;
  encodedGraphics = Resource_GetTableEntry(resourceId);
  Dma_Set(encodedGraphics, (void *)BG_PLTT,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x70, REG_DMA3);
  encodedGraphics += 0x1c0;
  FieldResource_DecodeType01(encodedGraphics, Ram_MapGridCellBuffer);
  Dma_Set((void *)Ram_MapGridCellBuffer, LOGO_TILES,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x2580, REG_DMA3);
  // Fill the visible 30 columns and pad each 32-entry tilemap row.
  tilemap = LOGO_MAP;
  tileIndex = LOGO_FIRST_TILE;
  for (screenY = 0; screenY <= 19; screenY++) {
    for (screenX = 0; screenX <= 29; screenX++)
      *tilemap++ = tileIndex++;
    *tilemap++ = blankTile;
    *tilemap++ = blankTile;
  }
  // Reset all four background scroll pairs before enabling BG2 and sprites.
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

struct MapWarpDest gTitleWarpDests[] = {
    {0, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

u32 gTitleWarps[] = {
    MAP_WARPS_END,
};

struct ObjectEventTemplate gTitleObjectEventTemplates[] = {
    {MAP_TABLE_END},
};

struct MapEvent gTitleEvents[] = {
    {MAP_EVENTS_END},
};

/// The VRAM cache entry holding the lettering's tiles; -1 until
/// Title_LoadSprites takes one.
s16 gTitleVramBlock = -1;

/// @file
/// The title overlay: the start screen, the Golden Sun logo shown after the
/// ending, and the screens that lead from the prologue and the last event on
/// to Vale and the File Screen. Title_Run chooses by entrance.

#include "common.h"
#include "vram_cache.h"
#include "unit/menu/title.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/game/game.h"
#include "unit/graphics/title/intro.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
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
#include "task.h"
#include "resource.h"
#include "heap_slots.h"
#include "types.h"
#include "save.h"
#include "mem.h"
#include "event_layer.h"
#include "event_inline.h"
#include "scene_ids.h"
#include "resource_ids.h"
#include "title.h"
#include "dma.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "map_grid_scroll.h"
#include "songs.h"
#include "audio_engine_symbols.h"

static void Title_RevealScreen(s32 unused);
static void Title_LoadBackground(void);

/// The title overlay's scene tables, which FieldLoop_Run reads through
/// entry/title.s: one entrance row, the exit, and no actor
/// templates or triggers.
u8 *Title_GetEntrances(void) { return (u8 *)gTitleEntrances; }

/// The exit regions entry: the title has no exit boxes.
s32 Scene_GetExitRegions(void) { return 0; }

u8 *Title_GetExits(void) { return (u8 *)gTitleExits; }

/// The title overlay has no actors.
u8 *Title_GetActorTemplates(void) { return (u8 *)gTitleActorTemplates; }

/// The title overlay has no triggers.
u8 *Title_GetTriggers(void) { return (u8 *)gTitleTriggers; }

/// The overlay's initialize entry: plays what the entrance asks for,
/// sets the after-battle destination and returns 0.
/// - Entrance 10, after Jenna and Kraden's closing event on the world
///   map (WorldMap_ShowEndingEvent): the Golden Sun logo for two seconds, then
///   until a key is pressed or a minute passes, then entrance 2.
/// - Entrance 9, after the ship sails (lemurian_ship.c's SetSail): the
///   credits, then the File Screen's entrance 2, which offers to
///   save the cleared game.
/// - Entrance 2, at power-on (Monitor_Run) and after the logo:
/// the
///   Nintendo and Camelot logos and, when a save file exists, the title intro
///   with its "Press Start" prompt; then the File Screen's
///   entrance 1.
/// - Any other entrance is the title shown after the prologue: the
///   intro without its prompt, then Vale's entrance 16
///   (Party_EndChildhood).
s32 Title_Run(void) {
  s32 waitFrames;

  // After 120 frames, the logo waits up to 3600 frames (a minute) for a key
  // press.
  if (gCommon.entrance == TITLE_FROM_WORLD_MAP) {
    Actor_GetById(gCommon.leaderActorId)->motionFlags = 0;
    Sound_PlaySong(MUS_ENDING_LOGO);
    Title_RevealScreen(0);
    WaitFrames(120);
    waitFrames = 0;
    if (gKeysPressed == 0) {
      do {
        WaitFrames(1);
        if (++waitFrames > 3599)
          break;
      } while (gKeysPressed == 0);
    }
    Event_SetDestination((s32)&SceneId_Title, TITLE_AFTER_RUN);
    return 0;
  }
  if (gCommon.entrance == TITLE_FROM_LEMURIAN_SHIP) {
    Sound_PlaySong(MUS_ENDING);
    // The credits: Credits_Run, through the scroll far-call
    // table.
    Import_CreditsRun(0);
    Sound_PlaySong(SONG_FADE_OUT_BGM);
    ScreenFade_ToBlack(60);
    ScreenFade_Wait();
    Event_WaitFrames(240);
    Sound_PlaySong(MUS_SILENCE);
    Event_SetDestination((s32)&SceneId_FileScreen, FILE_SCREEN_FROM_TITLE);
    return 0;
  }
  // The stub does nothing with its argument (runtime/runtime/system.c), the
  // far-call bank the logo and intro calls below go through. XXX (no
  // evidence): what the call once did; the stub is empty in every edition, so
  // only an earlier build of the game would show.
  Stub_NoOpHook((s32)&ResourceId_TitleFarCalls);
  if (gCommon.entrance == TITLE_AFTER_RUN) {
    for (;;) {
      Sound_PlaySong(MUS_SILENCE);
      // The Nintendo logo, then the Camelot logo (Title_ShowCamelotLogo).
      Title_ShowNintendoLogo(0);
      Import_TitleShowCamelotLogo(0);
      // With no save file (or no backup memory) there is no title intro.
      if (Save_ScanRecordFlags() > 0) {
        Sound_PlaySong(MUS_TITLE);
        // Title_ShowIntro with its prompt returns 0 when a minute passes
        // without A or Start: fade out and start again from the logos.
        if (Title_ShowIntro(1) == 0) {
          Sound_PlaySong(SONG_FADE_OUT_BGM);
          ScreenFade_ToBlack(30);
          ScreenFade_Wait();
          waitFrames = 0;
          if (gKeysHeld == 0) {
            do {
              WaitFrames(1);
              if (++waitFrames > 119)
                break;
            } while (gKeysHeld == 0);
          }
          continue;
        }
      }
      break;
    }
    Event_SetDestination((s32)&SceneId_FileScreen, FILE_SCREEN_FROM_SANCTUM);
  } else {
    // The intro without its prompt holds the title five seconds.
    Sound_PlaySong(MUS_TITLE_AFTER_PROLOGUE);
    Title_ShowIntro(0);
    Party_EndChildhood();
    Event_SetDestination((s32)&SceneId_Vale, VALE_FROM_TITLE);
    Sound_PlaySong(SONG_FADE_OUT_BGM);
  }
  Sound_PlaySong(SONG_FADE_OUT_BGM);
  ScreenFade_ToBlack(30);
  ScreenFade_Wait();
  Event_WaitFrames(60);
  Sound_PlaySong(MUS_SILENCE);
  return 0;
}

/// Loads the logo's lettering sprites and their palette. The tiles go into
/// the VRAM cache entry gTitleVramBlock, taken on the first call; the decode
/// buffer comes from the EWRAM pool and is freed before returning.
/// English and Japanese letter the logo with 4-bit tiles on palette 15; the
/// other editions with 256-colour tiles and a 224-colour palette.
static void Title_LoadSprites(s32 unused) {
  u8 *decodedSprites;
  volatile u32 *dma3;
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
  u8 *encodedGraphics;

  decodedSprites = AllocEwram(0x9600);
  encodedGraphics = Resource_GetTableEntry((s32)&ResourceId_IntroTilesB);
  if (gTitleVramBlock == -1)
    gTitleVramBlock = VramCache_FindFreeEntry();
  // The resource opens with 0x1c0 bytes of palette (0x70 words, 224 colours)
  // before its compressed data.
  Dma_Set(encodedGraphics, (void *)OBJ_PLTT,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x70, REG_DMA3);
  encodedGraphics += 0x1c0;
  FieldDecompress_TaggedLz(encodedGraphics, decodedSprites);
  // The picture is a whole 240 x 160 screen (to_be_continued_de.png and the
  // others). The lettering is its bottom 32 rows, the last 0x1e00 bytes: 120
  // sprites of one 64-byte tile. The 0x7800 bytes before it are the 480 tiles
  // of one colour above, which nothing loads.
  Vram_Load(gTitleVramBlock, 0x1e00, decodedSprites + 0x7800);
#else
  // The decoded data opens with its 16-colour palette (8 words), then 0x500
  // bytes (40 4-bit tiles) of lettering.
  decodedSprites = AllocEwram(0x520);
  if (gTitleVramBlock == -1)
    gTitleVramBlock = VramCache_FindFreeEntry();
  FieldDecompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_IntroTilesB),
                           decodedSprites);
  Dma_Set(decodedSprites, (void *)&OBJ_PLTT_COLOR(15, 0),
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  Vram_Load(gTitleVramBlock, 0x500, decodedSprites + 32);
#endif
  // Wait for the final DMA transfer before releasing the decode buffer.
  dma3 = REG_DMA3;
  while (dma3[2] & (DMA_ENABLE << 16))
    ;
  Free(decodedSprites);
}

/// The task that queues the logo's lettering sprites each frame,
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
  tileIndex = gVramCacheEntries[gTitleVramBlock].offset >> 5;
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
      Oam_AddSpriteAtDepth(sprite++, 255);
    tileIndex += 2;
  } while (++column <= 17);
#endif
  gTitleRevealFrame++;
}

/// Shows the Golden Sun logo for Title_Run's entrance 10: loads it
/// while the screen is black, starts the lettering's reveal
/// (Title_UpdateSpriteReveal, left running), brings the logo up from black over
/// 51 frames and opens the screen. Leaves the field work's transitions at 60
/// frames.
static void Title_RevealScreen(s32 unused) {
  s32 brightnessStep;

  // Load while dark, then let the sprite task reveal the lettering.
  Title_LoadBackground();
  Event_WaitFrames(30);
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
  Event_WaitFrames(120);
  // Lift the darkening in 17 steps of three frames, down to BLDY 0.
  for (brightnessStep = 0; brightnessStep <= 16; brightnessStep++) {
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 16 - brightnessStep, IO_WRITE_16);
    WaitFrames(3);
  }
  // Open the screen in one frame, then set 60-frame transitions for what
  // follows.
  gFieldWork->transition = 0;
  gFieldWork->transitionFrames = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  gFieldWork->transitionFrames = 60;
}

/// Loads the Golden Sun logo onto BG2 (title.h's LOGO_ map) with the
/// screen faded to black: its 224-colour palette, its 600 256-colour tiles,
/// decoded through Ram_MapGridCellBuffer, and a tile map that numbers them row
/// by row. Zeroes the four backgrounds' scroll and leaves the map work showing
/// only BG2 and the sprites. FileScreen_LoadBackground (file_screen.c) is its
/// twin.
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
  ScreenFade_ToBlack(0);
  REG_BG2CNT = LOGO_BGCNT;
  gBgScroll[2].y = 0;
  blankTile = LOGO_BLANK_TILE;
  encodedGraphics = Resource_GetTableEntry(resourceId);
  Dma_Set(encodedGraphics, (void *)BG_PLTT,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x70, REG_DMA3);
  encodedGraphics += 0x1c0;
  FieldDecompress_TaggedLz(encodedGraphics, Ram_MapGridCellBuffer);
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

struct SceneEntrance gTitleEntrances[] = {
    {0, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gTitleExits[] = {
    SCENE_EXITS_END,
};

struct ActorTemplate gTitleActorTemplates[] = {
    {SCENE_TABLE_END},
};

struct Trigger gTitleTriggers[] = {
    {TRIGGERS_END},
};

/// The VRAM cache entry holding the lettering's tiles; -1 until
/// Title_LoadSprites takes one.
s16 gTitleVramBlock = -1;

// The title overlay: the Golden Sun logo and its sprites revealed over the
// title screen.

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
#include "field_event.h"
#include "field_service.h"
#include "scene_ids.h"
#include "resource_ids.h"
#include "title.h"
#include "dma.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "vram_block.h"
#include "ram_buffer.h"
#include "field_scene.h"
#include "map_scroll.h"
#include "songs.h"

static void Title_RevealScreen(s32 unused);
static void Title_LoadBackground(void);

// Expose the title overlay entrance table to the scene loader.
u8 *Title_GetEntrances(void) { return (u8 *)gTitleEntrances; }

// The title overlay needs no additional scene data.
s32 SceneData_ReturnZero(void) { return 0; }

// Expose the title overlay exit table to the scene loader.
u8 *Title_GetExits(void) { return (u8 *)gTitleExits; }

// The title overlay has no placed field actors.
u8 *Title_GetPlacements(void) { return (u8 *)gTitlePlacements; }

// The title overlay has no field events.
u8 *Title_GetEvents(void) { return (u8 *)gTitleEvents; }

// Route the logo, completion presentation and new-game title entrances.
s32 Title_Run(void) {
  s32 waitFrames;

  // Play the logo entrance until input or the one-minute timeout.
  if (gGameState.entrance == 10) {
    Object_GetById(gGameState.selectedActor)->motionFlags = 0;
    Engine_AudioPlayCue(MUS_075);
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
    Event_SetDestination((s32)&SceneId_Title, 2);
    return 0;
  }
  // Finish the completion presentation before entering the clear overlay.
  if (gGameState.entrance == 9) {
    Engine_AudioPlayCue(MUS_067);
    ScrollFar_Entry00(0);
    Engine_AudioPlayCue(MUS_017);
    Fade_ToBlack(60);
    Fade_Wait();
    Engine_EventWait(240);
    Engine_AudioPlayCue(MUS_019);
    Event_SetDestination((s32)&SceneId_Clear, 2);
    return 0;
  }
  RuntimeDispatch_NoOpHook((s32)&ResourceId_PaletteFarCalls);
  // Returning from the logo may repeat the splash and save-selection flow.
  if (gGameState.entrance == 2) {
    for (;;) {
      Engine_AudioPlayCue(MUS_019);
      Title_ShowSplashScreen(0);
      PaletteFar_Entry20(0);
      if (SaveState_ScanRecordFlags() > 0) {
        Engine_AudioPlayCue(MUS_070);
        if (PaletteFar_Entry00(1) == 0) {
          Engine_AudioPlayCue(MUS_017);
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
    Event_SetDestination((s32)&SceneId_Clear, 1);
  } else {
    Engine_AudioPlayCue(MUS_064);
    PaletteFar_Entry00(0);
    Party_EndChildhood();
    Event_SetDestination((s32)&SceneId_Vale, 16);
    Engine_AudioPlayCue(MUS_017);
  }
  Engine_AudioPlayCue(MUS_017);
  Fade_ToBlack(30);
  Fade_Wait();
  Engine_EventWait(60);
  Engine_AudioPlayCue(MUS_019);
  return 0;
}

// Decode the language-specific title sprites and upload their palette and
// tiles.
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
  Dma_Set(encodedGraphics, (void *)OBJ_PLTT,
          (DMA_ENABLE | DMA_32BIT) << 16 | 0x70, REG_DMA3);
  encodedGraphics += 0x1c0;
  FieldResource_DecodeType01(encodedGraphics, decodedSprites);
  Vram_Load(gTitleVramBlock, 0x1e00, decodedSprites + 0x7800);
#else
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

// Reveal successive sprite columns, flickering each newly reached column.
static void Title_UpdateSpriteReveal(void) {
  u32 *attributes;
  struct TitleSprite *sprite;
  s32 tileIndex;
  s32 column;
  s32 revealAge;
  s32 screenY;
  s32 screenX;
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
  s32 row;

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

// Prepare the logo, reveal its sprites and then open the screen transition.
static void Title_RevealScreen(s32 unused) {
  s32 brightnessStep;

  // Load while dark, then let the sprite callback reveal the lettering.
  Title_LoadBackground();
  Engine_EventWait(30);
  gTitleRevealFrame = 0;
  Title_LoadSprites(0);
  FieldScheduler_AddCallback(Title_UpdateSpriteReveal, 0xc80);
  // Show BG0, BG2 and the sprites, with BG1 to BG3 darkened to black over
  // the other layers and the backdrop.
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
  Engine_EventWait(120);
  // Remove the darkening in three-frame steps.
  for (brightnessStep = 0; brightnessStep <= 16; brightnessStep++) {
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 16 - brightnessStep, 0x20000);
    Engine_TaskWait(3);
  }
  // Open immediately here and restore the normal transition duration.
  gEventWork->startTransition = 0;
  gEventWork->transitionFrames = 1;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  gEventWork->transitionFrames = 60;
}

// Load the logo palette and tiles, build its tilemap and reset scrolling.
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
  FieldResource_DecodeType01(encodedGraphics, Ram_MapCellBuffer);
  Dma_Set((void *)Ram_MapCellBuffer, LOGO_TILES,
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
  ((struct MapScrollWork *)gMapWork[0])->flags =
      DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
}

struct SceneEntrance gTitleEntrances[] = {
    {0, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gTitleExits[] = {
    SCENE_EXITS(511),
};

struct ScenePlacement gTitlePlacements[] = {
    {SCENE_TABLE_END},
};

struct SceneEvent gTitleEvents[] = {
    {SCENE_EVENTS_END},
};

s16 gTitleVramBlock = -1;

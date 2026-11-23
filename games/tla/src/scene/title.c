/// @file
/// The title: the Nintendo and Camelot logos, the intro, and the starts it
/// leads to, the prologue with Jenna or the game with Felix; entrance 8 plays
/// the story crawl a new game opens with, over Vale in the rain, and entrance
/// 10, which nothing reaches, keeps The Broken Seal's Golden Sun logo with "To
/// Be Continued" revealed under it.
#include "resource_ids.h"
#include "scene_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence_event.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "graphics/resource.h"
#include "graphics/vram_tab.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "scene/title.h"
#include "sound/sound.h"
#include "system/bg_scroll.h"
#include "system/dma.h"
#include "system/frame.h"
#include "system/heap_release.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/io_write_queue.h"
#include "system/main_routine_work.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/system_call.h"
#include "system/task.h"

extern s16 gTitleVramBlock;

s32 Credits_Run(s32 mode);

s32 Title_ShowIntro(s32 prompt);

void TitleIntro_Run(s32 unused);

static void Title_RevealScreen(s32 unused);
static void Title_LoadBackground(void);

/// One sprite of the logo's lettering, as Oam_AddSpriteAtDepthClamped queues
/// it. Title_UpdateSpriteReveal rewrites every entry each frame.
struct TitleSprite {
  /// [0] the queue link the depth queue writes; [1] OAM attribute 0 (y, mode
  /// and shape) in the low half and attribute 1 (x) in the high half; [2]
  /// attribute 2 (the tile, and the palette).
  u32 attr[3];
};

/// Frames since the lettering's reveal began; a column appears every two.
extern s16 gTitleRevealFrame;
/// The lettering's 18 sprites, in the overlay's BSS (scene/title_bss.s).
extern struct TitleSprite gTitleSprites[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

s32 Save_ScanFileSummaries(void);
s32 Title_ShowNintendoLogo(s32 mode);
void Title_ShowCamelotLogo(s32 unused);

/// The overlay's initialize entry, which plays what the entrance asks for.
s32 Scene_Initialize(void) {
  s32 waitFrames;

  if (gMainRoutineWork.entrance == TITLE_TO_BE_CONTINUED) {
    Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = 0;
    Event_PlaySound(MUS_TITLE_AFTER_PROLOGUE);
    Title_RevealScreen(0);
    // After 120 frames, the logo waits up to 3600 frames (a minute) for a key
    // press.
    wait(120);
    for (waitFrames = 0; waitFrames < 3600; waitFrames++) {
      wait(1);
      if (gInput.pressed != 0)
        break;
    }
    Event_SetDestination(SCENE_TITLE, TITLE_PROLOGUE);
    return 0;
  }
  if (gMainRoutineWork.entrance == TITLE_STORY_CRAWL) {
    Event_PlaySound(MUS_THE_BROKEN_SEAL);
    // "Ages ago, or so the stories / tell, the power of Alchemy..."
    Credits_Run(1);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    ScreenFade_ToBlack(60);
    ScreenFade_Wait();
    Event_Wait(240);
    Event_PlaySound(MUS_SILENCE);
    Event_SetDestination(SCENE_TITLE, TITLE_PROLOGUE);
    return 0;
  }
  Resource_ReservedNoOp(RESOURCE_TITLE_FAR_CALLS);
  if (gMainRoutineWork.entrance == TITLE_START) {
    for (;;) {
      Event_PlaySound(MUS_SILENCE);
      Title_ShowNintendoLogo(0);
      Title_ShowCamelotLogo(0);
      if (Save_ScanFileSummaries() > 0) {
        Event_PlaySound(MUS_MAIN_THEME_TITLE);
        TitleIntro_Run(1);
        if (Title_ShowIntro(1) == 0) {
          Event_PlaySound(SOUND_FADE_OUT_BGM);
          ScreenFade_ToBlack(30);
          ScreenFade_Wait();
          for (waitFrames = 0; waitFrames <= 119; waitFrames++) {
            wait(1);
            if (gInput.pressed != 0)
              break;
          }
          continue;
        }
      }
      break;
    }
    Event_SetDestination(SCENE_CLEAR, 1);
  } else {
    if (gMainRoutineWork.entrance == TITLE_PROLOGUE) {
      Event_PlaySound(MUS_MAIN_THEME_TITLE);
      TitleIntro_Run(0);
      FieldWindowColor_Apply(gMainRoutineWork.windowHue,
                             gMainRoutineWork.windowBrightness);
      Party_SetLeaderJenna();
      // Venus Lighthouse's ENTRANCE_OPENING.
      Event_SetDestination(SCENE_VENUS_LIGHTHOUSE, 99);
    }
    if (gMainRoutineWork.entrance == TITLE_FROM_IDEJIMA) {
      Event_PlaySound(MUS_TITLE_AFTER_PROLOGUE);
      Title_ShowIntro(0);
      FieldWindowColor_Apply(gMainRoutineWork.windowHue,
                             gMainRoutineWork.windowBrightness);
      Party_SetLeaderFelix();
      // Idejima's ENTRANCE_RIDE_THE_TIDAL_WAVE.
      Event_SetDestination(SCENE_IDEJIMA, 13);
    }
  }
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  ScreenFade_ToBlack(30);
  ScreenFade_Wait();
  Event_Wait(60);
  Event_PlaySound(MUS_SILENCE);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The sprite palette of the "To Be Continued" lettering.
enum { TO_BE_CONTINUED_PALETTE = 15 };

/// Loads the logo's lettering sprites and their palette.
void Title_LoadSprites(s32 unused) {
  u8 *decodedSprites;
  volatile s32 *dma3;

  // The decoded data opens with its 16-colour palette, then 40 4-bit tiles of
  // lettering.
  decodedSprites = Heap_AllocEwram(PALETTE_COLORS * 2 + 40 * TILE_SIZE_4BPP);
  if (gTitleVramBlock == -1)
    gTitleVramBlock = VramCache_FindFreeEntry();
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_INTRO_TILES_B),
                      decodedSprites);
  Dma_Set(decodedSprites, (void *)&OBJ_PLTT_COLOR(TO_BE_CONTINUED_PALETTE, 0),
          DMA_COPY_WORDS(PALETTE_COLORS / 2), REG_DMA3);
  VramCache_Load(gTitleVramBlock, 40 * TILE_SIZE_4BPP,
                 decodedSprites + PALETTE_COLORS * 2);
  dma3 = (volatile s32 *)REG_DMA3;
  while (dma3[2] & DMA_BUSY)
    ;
  Heap_Release(decodedSprites);
}

/// The task that queues the logo's lettering sprites each frame, revealing
/// them from the left: those the reveal has reached are queued, the two newest
/// only on even frames.
void Title_UpdateSpriteReveal(void) {
  u32 *attributes;
  struct TitleSprite *sprite;
  s32 tileIndex;
  s32 column;
  s32 revealAge;
  s32 screenX;

  // One row of 18 semi-transparent sprites of 8 by 16 pixels at y 136 and x 88
  // to 224.
  sprite = gTitleSprites;
  attributes = gTitleSprites[0].attr;
  tileIndex = gVramCacheEntries[gTitleVramBlock].offset >> 5;
  for (column = 0; column <= 17; column++) {
    s32 screenY = 136;

    screenX = 232 - (18 - column) * 8;
#ifdef UBFIX
    // Stay inside each sprite's three-word array instead of walking into the
    // next struct.
    attributes = gTitleSprites[column].attr;
#endif
    *attributes++ = 0;
    *attributes++ = OBJ_X(screenX) | screenY | OBJ_SIZE_8X16 |
                    OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
    *attributes++ = OBJ_PALETTE(TO_BE_CONTINUED_PALETTE) | tileIndex;
    revealAge = gTitleRevealFrame / 2 - column;
    if (revealAge < 0)
      revealAge = 0;
    if (revealAge <= 2 && (gFrameCount & 1))
      revealAge = 0;
    if (revealAge != 0)
      Oam_AddSpriteAtDepthClamped(sprite++, OAM_DEPTH_FRONT);
    tileIndex += 2;
  }
  gTitleRevealFrame++;
}

/// Shows the Golden Sun logo for entrance 10: its "To Be Continued" lettering
/// is revealed from the left while the logo brightens out of black.
static void Title_RevealScreen(s32 unused) {
  s32 brightnessStep;

  Title_LoadBackground();
  Event_Wait(30);
  gTitleRevealFrame = 0;
  Title_LoadSprites(0);
  Task_Add(Title_UpdateSpriteReveal, TASK_PASS_FRAME);
  // Show BG0, BG2 and the sprites. BG1 to BG3 start fully darkened, so the
  // logo on BG2 is black; the semi-transparent lettering is blended onto the
  // layers under it at full weight.
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP,
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

/// The title logo: its palette, its tiles decoded and copied to VRAM, a 30 by
/// 20 tilemap of consecutive tiles with two blank cells closing each row, and
/// the four scroll pairs cleared.
static void Title_LoadBackground(void) {
  u8 *encodedGraphics;
  s32 resourceId;
  u16 *tilemap;
  u32 screenX;
  u32 screenY;
  u16 tileIndex;
  struct BgScroll *bgScroll;
  s32 blankTile;

  resourceId = RESOURCE_LOGO_ON_BLACK;
  ScreenFade_ToBlack(0);
  REG_BG2CNT = LOGO_BGCNT;
  gBgScroll[2].y = 0;
  blankTile = LOGO_BLANK_TILE;
  encodedGraphics = Resource_GetTableEntry(resourceId);
  Dma_Set(encodedGraphics, (void *)BG_PLTT,
          DMA_COPY_WORDS(LOGO_PALETTE_COLORS / 2), REG_DMA3);
  encodedGraphics += LOGO_PALETTE_COLORS * 2;
  FieldDecompress_TaggedLz(encodedGraphics, Ram_MapCellBuffer);
  Dma_Set((void *)Ram_MapCellBuffer, LOGO_TILES, DMA_COPY_WORDS(9600),
          REG_DMA3);
  tilemap = LOGO_MAP;
  tileIndex = LOGO_FIRST_TILE;
  for (screenY = 0; screenY <= 19; screenY++) {
    for (screenX = 0; screenX <= 29; screenX++)
      *tilemap++ = tileIndex++;
    *tilemap++ = blankTile;
    *tilemap++ = blankTile;
  }
  bgScroll = gBgScroll;
  for (screenY = 0; screenY <= 3; screenY++) {
    bgScroll->y = 0;
    bgScroll->x = 0;
    bgScroll++;
  }
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  ((struct MapWork *)Ram_HeapSlots->mapGrid)->dispcnt =
      DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
}

#include "shared/scene_entrances_mapless.c"

u32 gSceneExits[] = {
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_TABLE_END},
};

s16 gTitleVramBlock = -1;

/// @file
/// The title: the Nintendo and Camelot logos, the intro, the title screen with
/// its row of sprites revealed under the logo, and the starts it leads to, the
/// prologue with Jenna or the game with Felix; entrance 8 plays the credits.

#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "graphics/resource.h"
#include "system/dma.h"
#include "system/heap_release.h"
#include "system/io_reg.h"
#include "graphics/vram_tab.h"
#include "system/input.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "system/task.h"
#include "system/io_write_queue.h"
#include "system/ram_buffer.h"
#include "system/bg_scroll.h"
#include "system/frame.h"
#include "field/field_sequence_event.h"
#include "game/game_bank.h"
#include "scene/scene.h"
#include "system/system_bank.h"
#include "resource_ids.h"
#include "system/system_call.h"

extern s16 gTitleVramBlock;

void Credits_Run(s32 value);

s32 Title_ShowIntro(s32 value);

void TitleIntro_Run(s32 value);

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
/// The lettering's 18 sprites, in the overlay's BSS (scene/bss/title.s).
extern struct TitleSprite gTitleSprites[];

// The logo the title shows: an 8-bit background on BG2, its map in screen
// block 6 and its tiles from tile 416 of character block 0, as The Broken
// Seal's.
#define LOGO_BGCNT (BGCNT_PRIORITY(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6))
#define LOGO_MAP ((u16 *)BG_SCREEN_BLOCK(6))
#define LOGO_FIRST_TILE 416
#define LOGO_TILES (BG_CHAR_BLOCK(0) + LOGO_FIRST_TILE * 64)
/// The map entry after each row's 30 visible tiles.
#define LOGO_BLANK_TILE 511

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

s32 Save_ScanFileSummaries(void);
void Title_ShowNintendoLogo(s32 value);
void Title_ShowCamelotLogo(s32 value);

/// The title's entrances, named by what each plays.
enum {
  /// The Nintendo and Camelot logos and the intro, then the clear-data screen
  /// (power-on, and after saving the clear data).
  TITLE_START = 1,
  /// The intro without its prompt, then the prologue with Jenna.
  TITLE_PROLOGUE = 2,
  /// The intro once more, then the game with Felix at Idejima.
  TITLE_AFTER_PROLOGUE = 3,
  /// Credits_Run, then TITLE_PROLOGUE.
  TITLE_CREDITS = 8,
  /// The logo revealed, held up to a minute, then TITLE_PROLOGUE.
  TITLE_LOGO = 10,
};

/// The overlay's initialize entry, which plays what the entrance asks for.
s32 Scene_Initialize(void) {
  s32 frames;

  if (gMainRoutineWork.entrance == TITLE_LOGO) {
    Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = 0;
    Event_PlaySound(MUS_TITLE_AFTER_PROLOGUE);
    Title_RevealScreen(0);
    // After 120 frames, the logo waits up to 3600 frames (a minute) for a key
    // press.
    wait(120);
    for (frames = 0; frames < 3600; frames++) {
      wait(1);
      if (gInput.pressed != 0)
        break;
    }
    Event_SetDestination(SCENE_TITLE, TITLE_PROLOGUE);
    return 0;
  }
  if (gMainRoutineWork.entrance == TITLE_CREDITS) {
    Event_PlaySound(MUS_ENDING);
    Credits_Run(1);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    ScreenFade_ToBlack(60);
    ScreenFade_Wait();
    Event_Wait(240);
    Event_PlaySound(MUS_SILENCE);
    Event_SetDestination(SCENE_TITLE, TITLE_PROLOGUE);
    return 0;
  }
  Resource_ReservedNoOp((s32)&ResourceId_TitleFarCalls);
  if (gMainRoutineWork.entrance == TITLE_START) {
    for (;;) {
      Event_PlaySound(MUS_SILENCE);
      Title_ShowNintendoLogo(0);
      Title_ShowCamelotLogo(0);
      if (Save_ScanFileSummaries() > 0) {
        Event_PlaySound(MUS_TITLE);
        TitleIntro_Run(1);
        if (Title_ShowIntro(1) == 0) {
          Event_PlaySound(SOUND_FADE_OUT_BGM);
          ScreenFade_ToBlack(30);
          ScreenFade_Wait();
          for (frames = 0; frames <= 119; frames++) {
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
      Event_PlaySound(MUS_TITLE);
      TitleIntro_Run(0);
      FieldWindowColor_Apply(gMainRoutineWork.windowHue,
                             gMainRoutineWork.windowBrightness);
      Party_SetLeaderJenna();
      // Venus Lighthouse's ENTRANCE_OPENING.
      Event_SetDestination(SCENE_VENUS_LIGHTHOUSE, 99);
    }
    if (gMainRoutineWork.entrance == TITLE_AFTER_PROLOGUE) {
      Event_PlaySound(MUS_TITLE_AFTER_PROLOGUE);
      Title_ShowIntro(0);
      FieldWindowColor_Apply(gMainRoutineWork.windowHue,
                             gMainRoutineWork.windowBrightness);
      Party_SetLeaderFelix();
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

void Title_LoadSprites(s32 unused) {
  u8 *buffer;
  volatile s32 *dma;

  // The decoded data opens with its 16-colour palette (8 words), then 40
  // 4-bit tiles of lettering.
  buffer = Heap_AllocEwram(32 + 40 * TILE_SIZE_4BPP);
  if (gTitleVramBlock == -1)
    gTitleVramBlock = VramCache_FindFreeEntry();
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_IntroTilesB),
                      buffer);
  Dma_Set(buffer, (void *)&OBJ_PLTT_COLOR(15, 0), DMA_COPY_WORDS(8), REG_DMA3);
  VramCache_Load(gTitleVramBlock, 40 * TILE_SIZE_4BPP, buffer + 32);
  dma = (volatile s32 *)REG_DMA3;
  while (dma[2] < 0)
    ;
  Heap_Release(buffer);
}

/// Rebuilds the row of 18 sprites under the logo and pushes those the reveal
/// has reached, hiding the two newest on odd frames, then counts the reveal
/// on.
// ☀️'s Title_UpdateSpriteReveal.
void Title_UpdateSpriteReveal(void) {
  u32 *attributes;
  struct TitleSprite *sprite;
  s32 tileIndex;
  s32 column;
  s32 revealAge;
  s32 screenX;

  // One row of 18 sprites of 8 by 16 pixels at y 136 and x 88 to 224: 0x8400
  // is the tall shape and semi-transparent, 0xf000 palette 15.
  sprite = gTitleSprites;
  attributes = gTitleSprites[0].attr;
  tileIndex = gVramCacheEntries[gTitleVramBlock].offset >> 5;
  column = 0;
  do {
    s32 screenY = 136;

    screenX = 232 - (18 - column) * 8;
    *attributes++ = 0;
    *attributes++ = (screenX << 16) | screenY | 0x8400;
    *attributes++ = 0xf000 | tileIndex;
    revealAge = gTitleRevealFrame / 2 - column;
    if (revealAge < 0)
      revealAge = 0;
    if (revealAge <= 2 && (gFrameCount & 1))
      revealAge = 0;
    if (revealAge != 0)
      Oam_AddSpriteAtDepthClamped(sprite++, 255);
    tileIndex += 2;
  } while (++column <= 17);
  gTitleRevealFrame++;
}

/// The title screen appears: the logo and its sprite row are loaded, the
/// blend fades the logo in over 17 steps and the screen transition opens.
static void Title_RevealScreen(s32 unused) {
  s32 i;

  Title_LoadBackground();
  Event_Wait(30);
  gTitleRevealFrame = 0;
  Title_LoadSprites(0);
  Task_Add(Title_UpdateSpriteReveal, TASK_PASS_VBLANK);
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
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 16, IO_WRITE_HALFWORD);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(16, 16),
                      IO_WRITE_HALFWORD);
  Event_Wait(120);
  i = 0;
fade:
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 16 - i, IO_WRITE_HALFWORD);
  wait(3);
  if (++i <= 16)
    goto fade;
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

  resourceId = (s32)&ResourceId_LogoOnBlack;
  ScreenFade_ToBlack(0);
  REG_BG2CNT = LOGO_BGCNT;
  gBgScroll[2].y = 0;
  blankTile = LOGO_BLANK_TILE;
  encodedGraphics = Resource_GetTableEntry(resourceId);
  Dma_Set(encodedGraphics, (void *)BG_PLTT, DMA_COPY_WORDS(112), REG_DMA3);
  encodedGraphics += 224 * 2;
  FieldDecompress_TaggedLz(encodedGraphics, Ram_MapCellBuffer);
  Dma_Set((void *)Ram_MapCellBuffer, LOGO_TILES, DMA_COPY_WORDS(9600),
          REG_DMA3);

  // Fill the visible 30 columns and pad each 32-entry tilemap row.
  tilemap = LOGO_MAP;
  tileIndex = LOGO_FIRST_TILE;
  screenY = 0;
row:
  screenX = 0;
column:
  *tilemap++ = tileIndex++;
  screenX++;
  if (screenX <= 29)
    goto column;
  *tilemap++ = blankTile;
  *tilemap++ = blankTile;
  screenY++;
  if (screenY <= 19)
    goto row;
  // Reset all four background scroll pairs.
  bgScroll = gBgScroll;
  screenY = 0;
scroll_pair:
  bgScroll->y = 0;
  bgScroll->x = 0;
  bgScroll++;
  screenY++;
  if (screenY <= 3)
    goto scroll_pair;
  Dma_Set(gBgScroll, REG_BG0HOFS, DMA_COPY_WORDS(4), REG_DMA3);
  ((struct TitleWork *)Ram_HeapSlots->mapGrid)->mode =
      DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

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

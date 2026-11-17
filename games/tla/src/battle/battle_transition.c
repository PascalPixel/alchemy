/// @file
/// The battle's screen transitions: the mosaic fade to black, and the rows of
/// tiles that close in, with the HBlank scroll they run on.
// Part of battle.c.

#include "battle_internal.h"
#include "gamepak/far_call/window.h"
#include "battle/battle.h"
#include "battle/presentation.h"
#include "system/system.h"

/// The curtain's first tile in BG character block 1.
#define TRANSITION_CURTAIN_FIRST_TILE 128
/// The curtain's map entries: its tile n, WINDOW_PALETTE.
#define TRANSITION_CURTAIN_ENTRY(n)                                            \
  (WINDOW_TILE(TRANSITION_CURTAIN_FIRST_TILE) + (n))
/// The screen block of the curtain's map, and its middle row, from which it
/// opens a row up and a row down every eight frames for TRANSITION_OPEN_FRAMES.
#define TRANSITION_CURTAIN_SCREEN 12
#define TRANSITION_CURTAIN_MIDDLE_ROW 13
#define TRANSITION_CURTAIN_LAST_ROW 20
#define TRANSITION_OPEN_FRAMES 80
/// The curtain's tile n in BG character block 1.
#define CURTAIN_TILE(n)                                                        \
  (BG_CHAR_BLOCK(1) + (TRANSITION_CURTAIN_FIRST_TILE + (n)) * TILE_SIZE_4BPP)
/// The curtain's solid tile takes one of eight variants, a new one every four
/// frames (BattleTransition_UploadTileVariant).
#define CURTAIN_VARIANTS 8
#define CURTAIN_VARIANT_FRAMES 4

/// The battle background's picture: 32 tiles to a row and 15 rows, its first
/// 256 tiles in character block 2 and the rest in block 3.
#define BACKGROUND_PICTURE_ROWS 15
#define BACKGROUND_PICTURE_TILES (BACKGROUND_PICTURE_ROWS * BG_SCREEN_WIDTH)
#define BACKGROUND_UPPER_TILES 256
/// BG1's flat map of the picture (BATTLE_BG1CNT): the picture's first tile in
/// its numbering, character block 2, and the blank tile it reads from the last
/// row of its own screen block, which BattleBackground_Load leaves clear.
#define BACKGROUND_FIRST_TILE 512
#define BACKGROUND_CLEAR_TILE 1023
/// A row of BG1's flat map: 32 entries, in bytes and in words.
#define TILEMAP_ROW_BYTES (BG_SCREEN_WIDTH * 2)
#define TILEMAP_ROW_WORDS (BG_SCREEN_WIDTH / 2)

/// Fades the screen to black over sixteen frames as every background's
/// mosaic grows, then shows no layer. Sets REG_BLDCNT to darken the
/// backgrounds, leaves REG_MOSAIC at its largest and REG_DISPCNT in mode 1
/// with every layer off; the four BGxCNT are put back as they were. It draws
/// four random numbers a frame, which only advances the random number
/// generator. Returns 0.
s32 BattleTransition_RunMosaicFadeOut(void) {
  u16 bg0cnt;
  u16 bg1cnt;
  u16 bg2cnt;
  u16 bg3cnt;
  s32 mosaicSize;
  volatile u16 *registerCursor;

  Io_Read16(&REG_BG0CNT, &bg0cnt);
  REG_BG0CNT = bg0cnt | BGCNT_MOSAIC;
  registerCursor = &REG_BG0CNT;
  Io_Read16(++registerCursor, &bg1cnt);
  *registerCursor = bg1cnt | BGCNT_MOSAIC;
  Io_Read16(++registerCursor, &bg2cnt);
  *registerCursor = bg2cnt | BGCNT_MOSAIC;
  Io_Read16(++registerCursor, &bg3cnt);
  *registerCursor = bg3cnt | BGCNT_MOSAIC;
  registerCursor += &REG_BLDCNT - &REG_BG3CNT;
  *registerCursor = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                    BLDCNT_TGT1_BD | BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_BG1 |
                    BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_OBJ |
                    BLDCNT_TGT2_BD;

  ScreenFade_ToBlack(16);
  for (mosaicSize = 0; mosaicSize <= 15; mosaicSize++) {
    Random16();
    Random16();
    Random16();
    Random16();
    REG_MOSAIC = (mosaicSize << 8) | mosaicSize;
    wait(1);
  }

  REG_DISPCNT = DISPCNT_MODE_1;
  wait(4);
  REG_BG0CNT = bg0cnt;
  REG_BG1CNT = bg1cnt;
  REG_BG2CNT = bg2cnt;
  REG_BG3CNT = bg3cnt;
  return 0;
}

/// Builds the zoomed background's 32 by 32 affine map, in which the background
/// picture's two halves come from character blocks 2 and 3: the upper half's
/// tiles numbered from 0, the lower half's from 0 again, then tile 255 for the
/// rest.
void Graphics_BuildSequentialTileTable(void *tilemap) {
  u32 *words = tilemap;
  u32 tiles;
  u32 i;

  // Each word is four consecutive tile numbers, one a byte.
  tiles = 0x03020100;
  for (i = 0; i < BACKGROUND_UPPER_TILES / 4; i++) {
    *words++ = tiles;
    tiles += 0x04040404;
  }
  tiles = 0x03020100;
  for (i = 0; i < (BACKGROUND_PICTURE_TILES - BACKGROUND_UPPER_TILES) / 4;
       i++) {
    *words++ = tiles;
    tiles += 0x04040404;
  }
  Iwram_FillWords(
      words, BG_SCREEN_WIDTH * BG_SCREEN_WIDTH - BACKGROUND_PICTURE_TILES, -1);
}

/// Builds BG1's flat map of the same picture, in screen block 31: four rows of
/// blank entries, two of BACKGROUND_CLEAR_TILE, the picture's tiles in order on
/// rows 6 to 20, then BACKGROUND_CLEAR_TILE down to the last row, which
/// BattleBackground_Load clears.
void BattleTransition_BuildTilemap(s32 *destination) {
  s32 entry;
  u32 index;

  Iwram_FillWords(destination, 4 * TILEMAP_ROW_BYTES, -1);
  destination += 4 * TILEMAP_ROW_WORDS;
  Iwram_FillWords(destination, 2 * TILEMAP_ROW_BYTES,
                  WINDOW_TILE_PAIR(BACKGROUND_CLEAR_TILE));
  entry = (BACKGROUND_FIRST_TILE + 1) << 16 | BACKGROUND_FIRST_TILE;
  destination += 2 * TILEMAP_ROW_WORDS;
  index = 0;
  do {
    index++;
    *destination++ = entry;
    entry += WINDOW_TILE_PAIR(2);
  } while (index < BACKGROUND_PICTURE_ROWS * TILEMAP_ROW_WORDS);
  Iwram_FillWords(destination, 10 * TILEMAP_ROW_BYTES,
                  WINDOW_TILE_PAIR(BACKGROUND_CLEAR_TILE));
}

/// Copies this frame's BG2CNT word and starts the HBlank DMA of the BG2 lines
/// and the affine block, while the background view is in mode 2.
void BattleBackground_ShowZoomedPage(void) {
  struct BattleAffineHdma *records;
  u16 *record;

  if ((Ram_HeapSlots->battleBackground)->mode == BATTLE_BACKGROUND_ZOOMED) {
    records = Ram_HeapSlots->battleScanlines;
    record = records->lines[records->page];
    Io_Write16(record[0], &REG_BG2CNT);
    Dma_Set(record + 1, (void *)&REG_BG2CNT, DMA_HBLANK_HALFWORD_PER_LINE,
            &REG_DMA0SAD);
    Dma_Set(&records->affine, (void *)&REG_BG2PA, DMA_COPY_WORDS(4), REG_DMA3);
  }
}

/// Unused: steps the curtain's solid tile through its variants
/// (gBattleTransition_CurtainVariants) while the transition timer runs from 1.
void BattleTransition_UploadTileVariant(void) {
  u32 variant = *(s32 *)Ram_HeapSlots->battleEntryTimer - 1;

  if (variant < CURTAIN_VARIANTS * CURTAIN_VARIANT_FRAMES)
    Dma_Set(gBattleTransition_CurtainVariants +
                variant / CURTAIN_VARIANT_FRAMES * TILE_SIZE_4BPP,
            CURTAIN_TILE(0), DMA_COPY_WORDS(TILE_SIZE_4BPP / 4), REG_DMA3);
}

/// Advances the curtain opening by a frame: BG0 slides into place as the camera
/// turns round to the party and zooms out.
void BattleTransition_AdvanceTimer(void) {
  s32 scroll;
  struct BattleCamera *camera;
  u32 *timer;
  u32 frame;
  u32 next;

  timer = (u32 *)Ram_HeapSlots->battleEntryTimer;
  frame = *timer;
  camera = (struct BattleCamera *)Ram_HeapSlots->cameraWork;
  scroll = 52 - frame;
  if (scroll > 32)
    scroll = 32;
  if (scroll < 0)
    scroll = 0;
  gBgScroll[0].y = scroll;
  if (frame <= TRANSITION_OPEN_FRAMES)
    camera->yaw = frame * 360 + 44928;
  next = (*timer = *timer + 1);
  if (next <= TRANSITION_OPEN_FRAMES) {
    BattleCamera_SetView(0, 0, 0,
                         BATTLE_ZOOM_NORMAL + TRANSITION_OPEN_FRAMES - next);
    return;
  }
  BattleCamera_SetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
}

/// Draws the curtain opening from the middle of the screen, one row up and one
/// down every eight frames.
void BattleTransition_DrawRows(void) {
  u32 column;
  s32 transitionFrame;
  s32 tileEntry;
  u32 tileRow;
  u16 *tilemap;

  transitionFrame = *(s32 *)Ram_HeapSlots->battleEntryTimer;
  if (transitionFrame >= 0 && transitionFrame < TRANSITION_OPEN_FRAMES) {
    tileEntry = (7 & transitionFrame) + TRANSITION_CURTAIN_ENTRY(1);
    tileRow = TRANSITION_CURTAIN_MIDDLE_ROW - transitionFrame / 8;
    tilemap = (u16 *)BG_SCREEN_BLOCK(TRANSITION_CURTAIN_SCREEN) +
              tileRow * BG_SCREEN_WIDTH;
    for (column = 0; column != BG_SCREEN_WIDTH; column++)
      tilemap[column] = tileEntry;

    // The lower half: the same tile flipped vertically.
    tileEntry |= TILE_VFLIP;
    tileRow = transitionFrame / 8 + TRANSITION_CURTAIN_MIDDLE_ROW;
    if (tileRow <= TRANSITION_CURTAIN_LAST_ROW) {
      tilemap = (u16 *)BG_SCREEN_BLOCK(TRANSITION_CURTAIN_SCREEN) +
                tileRow * BG_SCREEN_WIDTH;
      for (column = 0; column != BG_SCREEN_WIDTH; column++)
        tilemap[column] = tileEntry;
    }
  }
}

/// Clears BG0's scroll from the line the VCount interrupt fires on.
void BattleTransition_ResetBg0Scroll(void) {
  u32 zero = 0;

  REG_BG0VOFS = zero;
}

/// Opens the battle against formation kind with enemyCount enemies: the curtain
/// opening, then each enemy announced (formations 74 to 76 and 638 wait a
/// second instead); for formations 78 to 80 the shattering opening over the
/// Serpent's stars.
void BattleTransition_Run(s32 enemyCount, s32 kind) {
  struct BattlePresentationWork work;
  u16 owners[UNIT_MAX];
  u16 shown[UNIT_MAX];
  s32 *timer;
  struct BattleWork *battle;
  struct BattleBackgroundWork *view;
  struct HeapSlots *heap;
  s32 ownerCount;
  s32 count;
  u32 i;

  heap = Ram_HeapSlots;
  view = heap->battleBackground;
  timer = HeapSlot_Alloc(HEAP_SLOT_BATTLE_ENTRY_TIMER, 4);
  if ((u32)(kind - FORMATION_DOOM_DRAGON_A) >
      FORMATION_DOOM_DRAGON_C - FORMATION_DOOM_DRAGON_A) {
    s32 y;
    s32 zero;

    Dma_Set(gBattleTransition_CurtainTiles, CURTAIN_TILE(1), DMA_COPY_WORDS(8),
            REG_DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 1 * TILE_SIZE_4BPP,
            CURTAIN_TILE(2), DMA_COPY_WORDS(8), REG_DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 2 * TILE_SIZE_4BPP,
            CURTAIN_TILE(3), DMA_COPY_WORDS(8), REG_DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 3 * TILE_SIZE_4BPP,
            CURTAIN_TILE(4), DMA_COPY_WORDS(8), REG_DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 4 * TILE_SIZE_4BPP,
            CURTAIN_TILE(5), DMA_COPY_WORDS(8), REG_DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 5 * TILE_SIZE_4BPP,
            CURTAIN_TILE(6), DMA_COPY_WORDS(8), REG_DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 6 * TILE_SIZE_4BPP,
            CURTAIN_TILE(7), DMA_COPY_WORDS(8), REG_DMA3);
    view->secondMode = BATTLE_BACKGROUND_FIXED;
    view->busy = 0;
    view->mode = BATTLE_BACKGROUND_FIXED;
    REG_DISPCNT = DISPCNT_MODE_1;
    IWRAM_FILL_WORDS(CURTAIN_TILE(0), TILE_SIZE_4BPP, PIXELS_4BPP(3));
    IWRAM_FILL_WORDS(CURTAIN_TILE(8), TILE_SIZE_4BPP, 0);
    REG_BG1CNT =
        BGCNT_CHAR_BASE(1) | BGCNT_SCREEN_BASE(TRANSITION_CURTAIN_SCREEN);
    REG_BG0CNT |= BGCNT_PRIORITY(2);
    view->mode = BATTLE_BACKGROUND_ZOOMED;
    // A closed curtain: solid rows 0 to TRANSITION_CURTAIN_LAST_ROW, clear
    // below.
    {
      u16 *map = (u16 *)BG_SCREEN_BLOCK(TRANSITION_CURTAIN_SCREEN);

      for (i = 0; i < BG_SCREEN_WIDTH; i++) {
        u32 tile;
        u32 column;

        if (i <= TRANSITION_CURTAIN_LAST_ROW)
          tile = TRANSITION_CURTAIN_ENTRY(0);
        else
          tile = TRANSITION_CURTAIN_ENTRY(8);
        for (column = 0; column < BG_SCREEN_WIDTH; column++)
          *map++ = tile;
      }
    }
    y = 32;
    gBgScroll[1].x = 8;
    gBgScroll[0].y = y;
    gBgScroll[1].y = y;
    zero = 0;
    wait(1);
    REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
    REG_WIN0V = WIN_RANGE(0, 136);
    REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
    REG_WIN1V = WIN_RANGE(0, 136);
    REG_WININ = WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 |
                WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG0 |
                WININ_WIN1_BG2 | WININ_WIN1_OBJ | WININ_WIN1_CLR;
    REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL;
    IoWriteQueue_Store16((u32)&REG_DISPCNT,
                         DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                             DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON |
                             DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
    BattleCamera_SetView(0, 0, 0, BATTLE_ZOOM_NORMAL + TRANSITION_OPEN_FRAMES);
    *timer = zero;
    Task_Add(BattleTransition_AdvanceTimer, TASK_PASS_FRAME);
    Task_Add(BattleTransition_DrawRows, TASK_PASS_VBLANK);
    Irq_SetHandler(IRQ_VCOUNT, 32, BattleTransition_ResetBg0Scroll);
    gBgScroll[0].y = y;
    wait(1);
    PartyStatus_CreateWindowFar(Ram_HeapSlots->battleWork->partyStatus);
    wait(20);
    IoWriteQueue_ClearBits16((u32)&REG_BG0CNT, 2);
    IoWriteQueue_SetBits16((u32)&REG_BG0CNT, 0);
    // Karst and Agatio's battles open with no announcement.
    if ((u32)(kind - FORMATION_KARST_AND_AGATIO_A) <=
            FORMATION_KARST_AND_AGATIO_C - FORMATION_KARST_AND_AGATIO_A ||
        kind == FORMATION_KARST_AND_AGATIO_VS_ISAAC)
      wait(60);
    else
      BattleIntro_AnnounceEncounter(enemyCount);
    Task_Remove(BattleTransition_AdvanceTimer);
    Task_Remove(BattleTransition_DrawRows);
    gBgScroll[0].y = 0;
    Irq_SetHandler(IRQ_VCOUNT, 0, 0);
  } else {
    s32 zero;

    battle = heap->battleWork;
    view->secondMode = BATTLE_BACKGROUND_FIXED;
    view->busy = 0;
    ownerCount = BattleParty_PrepareMembers(owners);
    ownerCount += BattleParty_PrepareReserveOwners(owners + ownerCount);
    for (i = 0; i != ownerCount; i++) {
      s32 id = owners[i];
      struct BattleObject *battleObject = BattleObject_Get(id);
      u16 enemyId = Unit_Get(id)->enemyId;

      if (enemyId != ENEMY_FUSION_DRAGON && enemyId != ENEMY_DOOM_DRAGON_A &&
          enemyId != ENEMY_DOOM_DRAGON_E && enemyId != ENEMY_DOOM_DRAGON_H)
        battleObject->scale = FIXED_ONE * 7 / 10;
    }
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                        DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_WIN0_ON |
                            DISPCNT_WIN1_ON,
                        IO_WRITE_HALFWORD);
    wait(1);
    {
      s32 background = RESOURCE_SERPENT_STARS_BACKDROP;
      u16 *slot = &battle->background;

      zero = 0;
      *slot = background;
    }
    work.count =
        BattleUnit_ListIds(BATTLE_SIDE_MASK_ENEMIES, (u16 *)work.actors);
    work.actors[work.count] = BATTLE_UNIT_LIST_END;
    BattleActor_SpawnObjectsForList(work.actors, 0);
    BattleEffect_RunShatteringOpeningFar(&work);
    BattleCamera_SetView(0, 0, 0, BATTLE_ZOOM_NORMAL);
    *timer = zero;
    Irq_SetHandler(IRQ_VCOUNT, 32, BattleTransition_ResetBg0Scroll);
    wait(1);
    wait(20);
    PartyStatus_CreateWindowFar(Ram_HeapSlots->battleWork->partyStatus);
    IoWriteQueue_ClearBits16((u32)&REG_BG0CNT, 2);
    IoWriteQueue_SetBits16((u32)&REG_BG0CNT, 0);
    REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
    count = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, shown);
    shown[count] = BATTLE_UNIT_LIST_END;
    BattleActor_SpawnObjectsForList((s16 *)shown, 0);
    count = BattleUnit_ListIds(BATTLE_SIDE_MASK_PARTY, shown);
    for (i = 0; i != count; i++)
      BattleHighlight_SetSpriteMode(shown[i], SPRITE_SEMI_TRANSPARENT);
    for (i = 0; i != BLEND_WEIGHT_MAX; i++) {
      REG_BLDALPHA = BLEND_ALPHA(i, BLEND_WEIGHT_MAX);
      wait(1);
    }
    for (i = 0; i != count; i++)
      BattleHighlight_SetSpriteMode(shown[i], SPRITE_OPAQUE);
    BattleIntro_AnnounceEncounter(enemyCount);
    gBgScroll[0].y = 0;
    wait(1);
    Irq_SetHandler(IRQ_VCOUNT, 0, 0);
  }
  Irq_SetHandler(IRQ_VCOUNT, 0, 0);
  REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  wait(1);
  REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  REG_BG0CNT &= ~BGCNT_PRIORITY(2);
  gBgScroll[1].x = 8;
  REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  HeapSlot_Release(HEAP_SLOT_BATTLE_ENTRY_TIMER);
}

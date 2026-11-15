/// @file
/// The battle's screen transitions: the mosaic fade to black, and the rows of
/// tiles that close in, with the HBlank scroll they run on.
// Part of battle.c.

#include "battle_internal.h"
#include "gamepak/far_call/window.h"
#include "battle/battle.h"
#include "battle/presentation.h"
#include "system/system_bank.h"

/// The curtain's map entries: tile 128 + n, WINDOW_PALETTE.
#define TRANSITION_CURTAIN_ENTRY(n) (0xf080 + (n))
/// The screen block of the curtain's map, and its middle row, from which it
/// opens a row up and a row down every eight frames for TRANSITION_OPEN_FRAMES.
#define TRANSITION_CURTAIN_SCREEN 12
#define TRANSITION_CURTAIN_MIDDLE_ROW 13
#define TRANSITION_CURTAIN_LAST_ROW 20
#define TRANSITION_OPEN_FRAMES 80

/// Fades the screen to black over sixteen frames as every background's
/// mosaic grows, then shows no layer. Sets REG_BLDCNT to darken the
/// backgrounds, leaves REG_MOSAIC at its largest and REG_DISPCNT in mode 1
/// with every layer off; the four BGxCNT are put back as they were. It draws
/// four random numbers a frame, which only advances the random number
/// generator. Returns 0.
s32 BattleTransition_RunMosaicFadeOut(void) {
  /* FAKEMATCH(volatile): the four saved BGxCNT values kept on the stack, each
   * stored and loaded again. */
  volatile u16 bg0cnt;
  volatile u16 bg1cnt;
  volatile u16 bg2cnt;
  volatile u16 bg3cnt;
  s32 mosaicSize;
  volatile u16 *registerCursor;

  bg0cnt = REG_BG0CNT;
  REG_BG0CNT = bg0cnt | BGCNT_MOSAIC;
  registerCursor = &REG_BG0CNT;
  bg1cnt = *++registerCursor;
  *registerCursor = bg1cnt | BGCNT_MOSAIC;
  bg2cnt = *++registerCursor;
  *registerCursor = bg2cnt | BGCNT_MOSAIC;
  bg3cnt = *++registerCursor;
  *registerCursor = bg3cnt | BGCNT_MOSAIC;
  // 33 halfwords on from REG_BG3CNT is REG_BLDCNT.
  registerCursor += 33;
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

// Tiles: build a map of consecutive byte values.

/// Writes consecutive bytes 0-255 followed by 0-223, then fills the remaining
/// 544 bytes with 0xff (1024 bytes in all).
void Graphics_BuildSequentialTileTable(void *tilemap) {
  u32 *words = tilemap;
  u32 tiles;
  u32 i;

  // Each word is four consecutive tile numbers, one a byte.
  tiles = 0x03020100;
  for (i = 0; i < 64; i++) {
    *words++ = tiles;
    tiles += 0x04040404;
  }
  tiles = 0x03020100;
  for (i = 0; i < 56; i++) {
    *words++ = tiles;
    tiles += 0x04040404;
  }
  Iwram_FillWords(words, 544, -1);
}

/// Builds a flat tilemap: rows 0 to 3 0xffff, rows 4 and 5 tile 1023, rows 6
/// to 20 tiles 512 to 991 in order, then tile 1023. Called with screen block
/// 31 by BattleBackground_SetMode and BattleBackground_Load.
void BattleTransition_BuildTilemap(s32 *destination) {
  s32 entry;
  u32 index;

  Iwram_FillWords(destination, 0x100, -1);
  destination += 0x40;
  Iwram_FillWords(destination, 0x80, 0x03ff03ff);
  entry = (0x0201 << 16) | 0x0200;
  destination += 0x20;
  index = 0;
  do {
    index++;
    *destination++ = entry;
    entry += 0x00020002;
  } while (index <= 239);
  Iwram_FillWords(destination, 0x280, 0x03ff03ff);
}

/// Copies this frame's BG2CNT word and starts the HBlank DMA of the BG2 lines
/// and the affine block, while the background view is in mode 2.
void BattleBackground_ShowZoomedPage(void) {
  struct BattleAffineHdma *records;
  u16 *record;

  if (((struct BattleBackgroundView *)Ram_HeapSlots->battleBackground)->mode ==
      2) {
    records = Ram_HeapSlots->battleScanlines;
    record = records->lines[records->page];
    Io_Write16(record[0], &REG_BG2CNT);
    Dma_Set(record + 1, (void *)&REG_BG2CNT, DMA_HBLANK_HALFWORD_PER_LINE,
            &REG_DMA0SAD);
    Dma_Set(&records->affine, (void *)&REG_BG2PA, DMA_COPY_WORDS(4), REG_DMA3);
  }
}

/// Copies one of eight tiles from gBattleTransition_CurtainVariants into
/// 0x06005000, a new one every four frames while the transition timer
/// (unk84[9], as BattleTransition_AdvanceTimer counts it) runs from 1 to 32.
void BattleTransition_UploadTileVariant(void) {
  u32 variant = *(s32 *)Ram_HeapSlots->battleEntryTimer - 1;

  if (variant <= 31)
    Dma_Set(gBattleTransition_CurtainVariants + (variant >> 2) * 32,
            (void *)BG_SCREEN_BLOCK(10), DMA_COPY_WORDS(8), REG_DMA3);
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

/// An HBlank handler: clears BG0's vertical scroll from the line it fires on.
void BattleTransition_ResetBg0Scroll(void) {
  u32 zero = 0;

  REG_BG0VOFS = zero;
}

#define DMA3 ((volatile u32 *)0x040000d4)

/// Opens the battle against formation kind with arg enemies: the curtain
/// opening, then each enemy announced (formations 74 to 76 and 638 wait a
/// second instead); for formations 78 to 80 the shattering opening over the
/// Serpent's stars.
void BattleTransition_Run(s32 arg, s32 kind) {
  struct BattlePresentationWork work;
  u16 owners[UNIT_MAX];
  u16 shown[UNIT_MAX];
  s32 *timer;
  struct BattleWork *battle;
  struct BattleBackgroundView *view;
  struct HeapSlots *heap;
  s32 ownerCount;
  s32 count;
  u32 i;

  heap = Ram_HeapSlots;
  view = (struct BattleBackgroundView *)heap->battleBackground;
  timer = HeapSlot_Alloc(HEAP_SLOT_BATTLE_ENTRY_TIMER, 4);
  if ((u32)(kind - 78) > 2) {
    s32 y;
    s32 zero;

    Dma_Set(gBattleTransition_CurtainTiles, (void *)0x06005020,
            DMA_COPY_WORDS(8), DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 32, (void *)0x06005040,
            DMA_COPY_WORDS(8), DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 64, (void *)0x06005060,
            DMA_COPY_WORDS(8), DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 96, (void *)0x06005080,
            DMA_COPY_WORDS(8), DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 128, (void *)0x060050a0,
            DMA_COPY_WORDS(8), DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 160, (void *)0x060050c0,
            DMA_COPY_WORDS(8), DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 192, (void *)0x060050e0,
            DMA_COPY_WORDS(8), DMA3);
    view->secondMode = 1;
    view->busy = 0;
    view->mode = 1;
    REG_DISPCNT = DISPCNT_MODE_1;
    IWRAM_FILL_WORDS((void *)BG_SCREEN_BLOCK(10), 32, 0x33333333);
    IWRAM_FILL_WORDS((void *)0x06005100, 32, 0);
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
    REG_WIN0H = 0xf0;
    REG_WIN0V = 0x88;
    REG_WIN1H = 0xf0;
    REG_WIN1V = 0x88;
    REG_WININ = WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 |
                WININ_WIN0_OBJ | WININ_WIN0_CLR | WININ_WIN1_BG0 |
                WININ_WIN1_BG2 | WININ_WIN1_OBJ | WININ_WIN1_CLR;
    REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL;
    IoWriteQueue_Store16((u32)&REG_DISPCNT, 0x7741);
    BattleCamera_SetView(0, 0, 0, 180);
    *timer = zero;
    Task_Add(BattleTransition_AdvanceTimer, TASK_PASS_VBLANK);
    Task_Add(BattleTransition_DrawRows, TASK_PASS_FRAME);
    Irq_SetHandler(2, 32, BattleTransition_ResetBg0Scroll);
    gBgScroll[0].y = y;
    wait(1);
    PartyStatus_CreateWindowFar(Ram_HeapSlots->battleWork->partyStatus);
    wait(20);
    IoWriteQueue_ClearBits16((u32)&REG_BG0CNT, 2);
    IoWriteQueue_SetBits16((u32)&REG_BG0CNT, 0);
    if ((u32)(kind - 74) <= 2 || kind == 638)
      wait(60);
    else
      BattleIntro_AnnounceEncounter(arg);
    Task_Remove(BattleTransition_AdvanceTimer);
    Task_Remove(BattleTransition_DrawRows);
    gBgScroll[0].y = 0;
    Irq_SetHandler(2, 0, 0);
  } else {
    s32 zero;

    battle = heap->battleWork;
    view->secondMode = 1;
    view->busy = 0;
    ownerCount = BattleParty_PrepareMembers(owners);
    ownerCount += BattleParty_PrepareReserveOwners(owners + ownerCount);
    for (i = 0; i != ownerCount; i++) {
      s32 id = owners[i];
      struct BattleObject *battleObject = BattleObject_Get(id);
      u16 enemyId = Unit_Get(id)->enemyId;

      if (enemyId != ENEMY_FUSION_DRAGON && enemyId != ENEMY_DOOM_DRAGON_A &&
          enemyId != ENEMY_DOOM_DRAGON_E && enemyId != ENEMY_DOOM_DRAGON_H)
        battleObject->scale = Q16_ONE * 7 / 10;
    }
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x6041, 0x20000);
    wait(1);
    {
      s32 background = (s32)&ResourceId_SerpentStarsBackdrop;
      u16 *slot = &battle->background;

      zero = 0;
      *slot = background;
    }
    work.count = BattleUnit_ListIds(2, (u16 *)work.actors);
    work.actors[work.count] = 255;
    BattleActor_SpawnObjectsForList(work.actors, 0);
    BattleEffect_RunShatteringOpeningFar(&work);
    BattleCamera_SetView(0, 0, 0, 100);
    *timer = zero;
    Irq_SetHandler(2, 32, BattleTransition_ResetBg0Scroll);
    wait(1);
    wait(20);
    PartyStatus_CreateWindowFar(Ram_HeapSlots->battleWork->partyStatus);
    IoWriteQueue_ClearBits16((u32)&REG_BG0CNT, 2);
    IoWriteQueue_SetBits16((u32)&REG_BG0CNT, 0);
    REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
    count = BattleUnit_ListIds(3, shown);
    shown[count] = 255;
    BattleActor_SpawnObjectsForList((s16 *)shown, 0);
    count = BattleUnit_ListIds(1, shown);
    for (i = 0; i != count; i++)
      BattleHighlight_SetSpriteMode(shown[i], 1);
    for (i = 0; i != 16; i++) {
      REG_BLDALPHA = i | 0x1000;
      wait(1);
    }
    for (i = 0; i != count; i++)
      BattleHighlight_SetSpriteMode(shown[i], 0);
    BattleIntro_AnnounceEncounter(arg);
    gBgScroll[0].y = 0;
    wait(1);
    Irq_SetHandler(2, 0, 0);
  }
  Irq_SetHandler(2, 0, 0);
  REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  wait(1);
  REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(31);
  REG_BG0CNT &= 0xfffd;
  gBgScroll[1].x = 8;
  REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  HeapSlot_Release(HEAP_SLOT_BATTLE_ENTRY_TIMER);
}

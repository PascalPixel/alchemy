/// @file
/// The opening transition: the curtain, or the Venus Lighthouse storm.
// Part of battle.c.

#include "battle_internal.h"

/// The formation the Fusion Dragon leads, fought on the Venus Lighthouse aerie:
/// it opens with the storm instead of the curtain.
#define TRANSITION_STORM_FORMATION 347

/// The curtain's map entries: tile 128 + n of CURTAIN_TILE, WINDOW_PALETTE.
#define TRANSITION_CURTAIN_ENTRY(n) (0xf080 + (n))

/// Fades the screen to black over sixteen frames as every background's mosaic
/// grows, then shows no layer.
s32 BattleTransition_RunMosaicFadeOut(void) {
  /* FAKEMATCH(volatile): the four saved BGxCNT values kept on the stack, each
   * stored and loaded again; as plain u16 locals, 61 of 90 instructions
   * differ in tbs-en. */
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

/// Builds the zoomed background's 32 by 32 affine map, in which the background
/// picture's two halves come from character blocks 2 and 3.
void Graphics_BuildSequentialTileTable(void *tilemap) {
  u32 *words = tilemap;
  u32 tiles;
  u32 i;

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

/// Builds BG1's flat map of the same picture, rows 6 to 20 of the screen.
void BattleTransition_BuildTilemap(void *tilemap) {
  u32 *pairs = tilemap;
  u32 pair;
  u32 i;

  Iwram_FillWords(pairs, 4 * 64, -1);
  pairs += 4 * 16;
  Iwram_FillWords(pairs, 2 * 64, 0x03ff03ff);
  pairs += 2 * 16;
  pair = 513 << 16 | 512;
  for (i = 0; i < 15 * 16; i++) {
    *pairs++ = pair;
    pair += 1 << 17 | 2;
  }
  Iwram_FillWords(pairs, 640, 0x03ff03ff);
}

/// Shows the zoomed background's finished page each VBlank.
void BattleBackground_ShowZoomedPage(void) {
  struct BattleBackgroundWork *view =
      HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  struct BattleAffineHdma *scanlines;
  u16 *line;

  if (view->mode != BATTLE_BACKGROUND_ZOOMED)
    return;
  scanlines = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SCANLINES);
  line = scanlines->lines[scanlines->page];
  Io_Write16(line[0], &REG_BG2CNT);
  Dma_Set(line + 1, (void *)&REG_BG2CNT, DMA_HBLANK_HALFWORD_PER_LINE,
          &REG_DMA0SAD);
  Dma_Set(&scanlines->affine, (void *)&REG_BG2PA, DMA_COPY_WORDS(4), REG_DMA3);
}

/// Unused: steps the curtain's solid tile through eight variants, a new one
/// every four frames.
// XXX (no evidence): which transition used it; nothing refers to it.
void BattleTransition_UploadTileVariant(void) {
  u32 variant = *gBattleEntryTimer - 1;
  if (variant <= 31)
    Dma_Set(gBattleTransition_CurtainVariants + (variant >> 2) * 32,
            CURTAIN_TILE(0), DMA_COPY_WORDS(8), REG_DMA3);
}

/// Advances the curtain opening by a frame: BG0 slides into place as the camera
/// turns round to the party and zooms out.
void BattleTransition_AdvanceTimer(void) {
  void **slots = &gHeapSlots[HEAP_SLOT_BATTLE_ENTRY_TIMER];
  u32 *timer = slots[0];
  u32 frame = *timer;
  struct BattleCamera *camera =
      HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_ENTRY_TIMER, HEAP_SLOT_CAMERA);
  s32 scroll = 52 - frame;

  if (scroll > 32)
    scroll = 32;
  if (scroll < 0)
    scroll = 0;
  gBgScroll[0].y = scroll;
  if (frame <= 80)
    camera->yaw = frame * 360 + 44928;
  frame = ++*timer;
  if (frame <= 80)
    BattleCamera_SetView(0, 0, 0, 180 - frame);
  else
    BattleCamera_SetView(0, 0, 0, 100);
}

/// Draws the curtain opening from the middle of the screen, one row up and one
/// down every eight frames.
void BattleTransition_DrawRows(void) {
  u32 column;
  s32 transitionFrame;
  s32 alignedFrame;
  s32 tileEntry;
  u32 tileRow;
  u16 *tilemap;

  transitionFrame = *gBattleEntryTimer;
  if (transitionFrame >= 0 && transitionFrame <= 79) {
    tileEntry = (7 & transitionFrame) + TRANSITION_CURTAIN_ENTRY(1);
    if (transitionFrame >= 0) {
      alignedFrame = transitionFrame;
    } else {
      alignedFrame = transitionFrame + 7;
    }
    tileRow = 13 - (alignedFrame >> 3);
    column = 0;
    tilemap = (u16 *)(BG_SCREEN_BLOCK(12) + (tileRow << 6));
    do {
      column++;
      *tilemap = tileEntry;
      tilemap++;
    } while (column != 32);

    // The lower half: the same tile flipped vertically.
    tileEntry |= TILE_VFLIP;
    alignedFrame = transitionFrame;
    if (transitionFrame < 0) {
      alignedFrame += 7;
    }
    tileRow = (alignedFrame >> 3) + 13;
    if (tileRow <= 20) {
      column = 0;
      tilemap = (u16 *)(BG_SCREEN_BLOCK(12) + (tileRow << 6));
      do {
        column++;
        *tilemap = tileEntry;
        tilemap++;
      } while (column != 32);
    }
  }
}

/// Clears BG0's scroll from the line the interrupt fires on.
void BattleTransition_ClearBg0Vofs(void) {
  u32 top = 0;

  REG_BG0VOFS = top;
}

/// Opens a battle: the curtain opening, or for the Fusion Dragon the storm on
/// the Venus Lighthouse aerie; then announces the enemies.
void BattleTransition_Run(s32 enemyCount, s32 encounterId) {
  volatile u32 fill;
  struct BattleStagingWork work;
  u16 unitIds[UNIT_MAX];
  u16 visibleUnitIds[UNIT_MAX];
  struct BattleBackgroundWork *view;
  s32 *timer;
  u16 *tilemap;
  s32 tile;
  u32 index;
  u32 column;
  s32 count;
  s32 id;
  struct BattleObject *battleObject;
  struct BattleWork *battle;
  u16 *visibleUnits;
  s32 visibleCount;

  view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
  timer = HeapSlot_Alloc(HEAP_SLOT_BATTLE_ENTRY_TIMER, 4);
  if (encounterId != TRANSITION_STORM_FORMATION) {
    // The curtain's seven edge tiles.
    Dma_Set(gBattleTransition_CurtainTiles, CURTAIN_TILE(1), DMA_COPY_WORDS(8),
            REG_DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 32, CURTAIN_TILE(2),
            DMA_COPY_WORDS(8), REG_DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 64, CURTAIN_TILE(3),
            DMA_COPY_WORDS(8), REG_DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 96, CURTAIN_TILE(4),
            DMA_COPY_WORDS(8), REG_DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 128, CURTAIN_TILE(5),
            DMA_COPY_WORDS(8), REG_DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 160, CURTAIN_TILE(6),
            DMA_COPY_WORDS(8), REG_DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 192, CURTAIN_TILE(7),
            DMA_COPY_WORDS(8), REG_DMA3);
    view->secondMode = BATTLE_BACKGROUND_FIXED;
    view->mode = BATTLE_BACKGROUND_FIXED;
    view->busy = 0;
    // The curtain on BG1, behind BG0.
    REG_DISPCNT = DISPCNT_MODE_1;
    Dma_Fill32Cell(fill, 0x33333333, CURTAIN_TILE(0), TILE_SIZE_4BPP);
    Dma_Fill32Cell(fill, 0, CURTAIN_TILE(8), TILE_SIZE_4BPP);
    REG_BG1CNT = BGCNT_CHAR_BASE(1) | BGCNT_SCREEN_BASE(12);
    REG_BG0CNT |= BGCNT_PRIORITY(2);
    view->mode = BATTLE_BACKGROUND_ZOOMED;
    // A closed curtain: solid rows 0 to 20, clear below.
    tilemap = (u16 *)BG_SCREEN_BLOCK(12);
    for (index = 0; index <= 31; index++) {
      if (index <= 20)
        tile = TRANSITION_CURTAIN_ENTRY(0);
      else
        tile = TRANSITION_CURTAIN_ENTRY(8);
      for (column = 0; column <= 31; column++)
        *tilemap++ = tile;
    }
    {
      s32 start = 0;

      gBgScroll[0].y = 32;
      gBgScroll[1].y = 32;
      gBgScroll[1].x = 8;
      wait(1);
      REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
      REG_WIN0V = WIN_RANGE(0, 136);
      REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
      REG_WIN1V = WIN_RANGE(0, 136);
      REG_WININ = BATTLE_WININ;
      REG_WINOUT = BATTLE_WINOUT;
      IoWriteQueue_Store16((u32)&REG_DISPCNT,
                           DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP |
                               DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                               DISPCNT_BG2_ON | DISPCNT_OBJ_ON |
                               DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
      // The opening runs from the tasks while the status window opens
      // and the encounter is announced.
      BattleCamera_SetView(0, 0, 0, 180);
      *timer = start;
      Task_Add(BattleTransition_AdvanceTimer, TASK_PASS_FRAME);
      Task_Add(BattleTransition_DrawRows, TASK_PASS_VBLANK);
      Irq_SetHandler(IRQ_VCOUNT, 32, BattleTransition_ClearBg0Vofs);
      gBgScroll[0].y = 32;
      wait(1);
      PartyStatus_CreateWindowFar(gBattleWork->partyStatus);
      wait(20);
      IoWriteQueue_ClearBits16((u32)&REG_BG0CNT, 2);
      IoWriteQueue_SetBits16((u32)&REG_BG0CNT, 0);
      BattleIntro_AnnounceEncounter(enemyCount);
      Task_Remove(BattleTransition_AdvanceTimer);
      Task_Remove(BattleTransition_DrawRows);
      gBgScroll[0].y = start;
      Irq_SetHandler(IRQ_VCOUNT, 0, 0);
    }
  } else {
    // The storm: everyone but the Fusion Dragon drawn smaller.
    battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
    view->secondMode = BATTLE_BACKGROUND_FIXED;
    view->busy = 0;
    count = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, (u16 *)unitIds);
    for (index = 0; index != count; index++) {
      // BUG: unitIds is never read; the index is taken as a unit id, and a gap
      // among the party's ids makes the scale store go near address 0.
      id = index + 120;
      if ((s32)index <= ALLY_MAX)
        id = index;
      battleObject = BattleObject_Get(id);
      if (Unit_GetFar(id)->id != ENEMY_FUSION_DRAGON)
        battleObject->scale = Q16_ONE * 7 / 10;
    }
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                        DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_WIN0_ON |
                            DISPCNT_WIN1_ON,
                        IO_WRITE_16);
    wait(1);
    {
      s32 start = 0;

      // The enemies alone take part in the shattering opening.
      battle->background = (u32)RESOURCE_VENUS_LIGHTHOUSE_AERIE_STORM_BACKDROP;
      work.effect.targetCount =
          BattleUnit_ListIds(BATTLE_SIDE_MASK_ENEMIES, work.effect.targets);
      work.effect.targets[work.effect.targetCount] = BATTLE_UNIT_LIST_END;
      BattleObject_SpawnObjectsForList(work.effect.targets, 0);
      BattleEffect_RunShatteringOpeningFar(&work.effect);
      BattleCamera_SetView(0, 0, 0, 100);
      *timer = start;
    }
    Irq_SetHandler(IRQ_VCOUNT, 32, BattleTransition_ClearBg0Vofs);
    wait(1);
    wait(20);
    PartyStatus_CreateWindowFar(gBattleWork->partyStatus);
    IoWriteQueue_ClearBits16((u32)&REG_BG0CNT, 2);
    IoWriteQueue_SetBits16((u32)&REG_BG0CNT, 0);
    // Everyone appears, the party fading in.
    REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
    visibleUnits = visibleUnitIds;
    visibleCount =
        BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, (u16 *)visibleUnits);
    visibleUnitIds[visibleCount] = BATTLE_UNIT_LIST_END;
    BattleObject_SpawnObjectsForList(visibleUnits, 0);
    visibleCount =
        BattleUnit_ListIds(BATTLE_SIDE_MASK_PARTY, (u16 *)visibleUnits);
    for (index = 0; index != visibleCount; index++)
      BattleHighlight_SetSpriteMode(visibleUnits[index],
                                    SPRITE_SEMI_TRANSPARENT);
    for (index = 0; index != 16; index++) {
      REG_BLDALPHA = BLEND_ALPHA(index, 16);
      wait(1);
    }
    for (index = 0; index != visibleCount; index++)
      BattleHighlight_SetSpriteMode(visibleUnits[index], SPRITE_OPAQUE);
    BattleIntro_AnnounceEncounter(enemyCount);
    gBgScroll[0].y = 0;
    wait(1);
    Irq_SetHandler(IRQ_VCOUNT, 0, 0);
  }
  Irq_SetHandler(IRQ_VCOUNT, 0, 0);
  REG_BG1CNT = BATTLE_BG1CNT;
  wait(1);
  REG_BG1CNT = BATTLE_BG1CNT;
  REG_BG0CNT &= ~BGCNT_PRIORITY(2);
  gBgScroll[1].x = 8;
  REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  HeapSlot_Release(HEAP_SLOT_BATTLE_ENTRY_TIMER);
}

#undef TRANSITION_CURTAIN_ENTRY
#undef TRANSITION_STORM_FORMATION

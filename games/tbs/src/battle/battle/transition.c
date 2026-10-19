/// @file
/// The opening transition: the curtain, or the Venus Lighthouse storm.
/// Part of battle.c, the battle object, compiled only through it.

#include "../battle_internal.h"

/// Fades to black while increasing mosaic size over sixteen frames; preserves
/// the four background controls and consumes four random draws each frame.
s32 BattlePres_RunMosaicFadeOut(void) {
  /* FAKEMATCH(volatile): the four saved BGxCNT values kept on the stack, each
   * stored and loaded again as in the ROM; without volatile, 28 of 90
   * instructions differ in tbs-en. */
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
  registerCursor += 0x21;
  *registerCursor = 0x3eee;

  Fade_ToBlack(16);
  for (mosaicSize = 0; mosaicSize <= 15; mosaicSize++) {
    Random16();
    Random16();
    Random16();
    Random16();
    REG_MOSAIC = (mosaicSize << 8) | mosaicSize;
    WaitFrames(1);
  }

  REG_DISPCNT = DISPCNT_MODE_1;
  WaitFrames(4);
  registerCursor = &REG_BG0CNT;
  *registerCursor++ = bg0cnt;
  *registerCursor++ = bg1cnt;
  *registerCursor++ = bg2cnt;
  *registerCursor = bg3cnt;
  return 0;
}

/// Tiles: builds a map of sequential byte indices.
/// Writes byte sequences 0..255 and 0..223, then fills the remaining 544 bytes
/// with 0xff, for 1024 bytes total.
void Graphics_BuildSequentialTileTable(s32 *destination) {
  u32 packedIndices;
  s32 wordIndex;

  // Advance four indices per iteration because each store writes four bytes.
  packedIndices = 0x03020100;
  wordIndex = 0;
  do {
    wordIndex += 1;
    *destination++ = packedIndices;
    packedIndices += 0x04040404U;
  } while (wordIndex >= 0 && wordIndex <= 63);

  packedIndices = 0x03020100;
  wordIndex = 0;
  do {
    wordIndex += 1;
    *destination++ = packedIndices;
    packedIndices += 0x04040404U;
  } while (wordIndex >= 0 && wordIndex <= 55);

  Iwram_FillWords(destination, 544, -1);
}

/// Builds the background's padded tilemap around sequential tile pairs
/// 0x200..0x3df.
void BattlePresentation_BuildTilemap(s32 *destination) {
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

/// Installs the active scanline page and affine registers while the battle
/// background uses mode 2.
void BattlePres_UpdateHBlankScroll(void) {
  struct BattleAffineHdma *scanlines;
  u16 *line;

  if (((struct BattleBackgroundView *)WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND))
          ->mode == 2) {
    scanlines = WORK_SLOT(10);
    line = scanlines->lines[scanlines->page];
    Io_Write16(line[0], &REG_BG2CNT);
    Dma_Set(line + 1, (void *)&REG_BG2CNT, DMA_HBLANK_HALFWORD_PER_LINE,
            &REG_DMA0SAD);
    Dma_Set(&scanlines->affine, (void *)&REG_BG2PA, DMA_COPY_WORDS(4),
            REG_DMA3);
  }
}

/// Uploads one of eight 32-byte curtain tile frames while the transition timer
/// is 1..32.
void BattlePresentation_UploadTileVariant(void) {
  u32 variant = *gDisp - 1;
  if (variant <= 31)
    Dma_Set(gBattlePres_TileVariants + (variant >> 2) * 32, CURTAIN_TILE(0),
            DMA_COPY_WORDS(8), REG_DMA3);
}

/// Advances the entry camera turn and curtain scroll, then settles at the
/// normal zoom.
void BattlePres_AdvanceTransitionTimer(void) {
  s32 scroll;
  struct BattleCamera *camera;
  u32 *timer;
  struct BgScroll *offset;
  u32 frame;
  u32 next;

  timer = *(u32 **)Ram_Disp;
  frame = *timer;
  camera = *(struct BattleCamera **)Ram_CameraWork;
  scroll = 0x34 - frame;
  if (scroll > 0x20)
    scroll = 0x20;
  offset = (struct BgScroll *)Ram_BgScroll;
  if (scroll < 0)
    scroll = 0;
  offset->y = scroll;
  if (frame <= 0x50U)
    camera->yaw = 45 * frame * 8 + 0xaf80;
  next = (*timer = *timer + 1);
  if (next <= 0x50U) {
    BattlePres_SetupTransitionScene(0, 0, 0, 0xb4 - next);
    return;
  }
  BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
}

/// Draws the mirrored curtain rows as the eighty-frame entry transition opens.
void BattlePres_DrawTransitionRows(void) {
  u32 column;
  s32 transitionFrame;
  s32 alignedFrame;
  s32 tileEntry;
  u32 tileRow;
  u16 *tilemap;

  transitionFrame = *gDisp;
  if (transitionFrame >= 0 && transitionFrame <= 79) {
    tileEntry = (7 & transitionFrame) + 0xf081;
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

    tileEntry |= 0x800;
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

/// Clears BG0 vertical scroll from the entry transition's interrupt handler.
void Graphics_ClearBg0Vofs(void) {
  u32 zero = 0;

  REG_BG0VOFS = zero;
}

/// The display control and green swap registers, REG_DISPCNT onward.
struct DisplayRegisters {
  u16 control;
  u16 greenSwap;
};

/// Runs the curtain entrance or the Venus Aerie storm entrance, announces the
/// enemies and restores the normal battle display and interrupt handlers.
void BattlePres_RunEntryTransition(s32 enemyCount, s32 encounterId) {
  volatile u32 fill;
  struct BattlePresentationWork presentation;
  u16 unitIds[14];
  u16 visibleUnitIds[14];
  struct BattleBackgroundView *background;
  s32 *timer;
  u16 *tilemap;
  s32 tile;
  u32 index;
  u32 column;
  s32 count;
  s32 id;
  struct BattleObj *object;
  struct BattleWork *battle;
  u16 *visibleUnits;
  s32 visibleCount;

  background = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
  timer = Runtime_AllocateHeapBlock(42, 4);
  if (encounterId != 0x15b) {
    {

      volatile u32 *dma;

      Dma_Set(gBattlePres_CurtainTiles, CURTAIN_TILE(1), DMA_COPY_WORDS(8),
              REG_DMA3);
      Dma_Set(gBattlePres_CurtainTiles + 32, CURTAIN_TILE(2), DMA_COPY_WORDS(8),
              REG_DMA3);
      Dma_Set(gBattlePres_CurtainTiles + 64, CURTAIN_TILE(3), DMA_COPY_WORDS(8),
              REG_DMA3);
      Dma_Set(gBattlePres_CurtainTiles + 96, CURTAIN_TILE(4), DMA_COPY_WORDS(8),
              REG_DMA3);
      Dma_Set(gBattlePres_CurtainTiles + 128, CURTAIN_TILE(5),
              DMA_COPY_WORDS(8), REG_DMA3);
      Dma_Set(gBattlePres_CurtainTiles + 160, CURTAIN_TILE(6),
              DMA_COPY_WORDS(8), REG_DMA3);
      Dma_Set(gBattlePres_CurtainTiles + 192, CURTAIN_TILE(7),
              DMA_COPY_WORDS(8), REG_DMA3);
      dma = REG_DMA3;
      /* FAKEMATCH(barrier): REG_DISPCNT reached from DMA3's address through an
       * empty asm, as the ROM's subs r3, #212; without the barrier, 12 of 414
       * instructions differ in tbs-en. */
      asm volatile("" : "+l"(dma));
      ((struct DisplayRegisters *)((u8 *)dma - 212))->control = 1;
    }
    background->secondMode = 1;
    background->mode = 1;
    background->busy = 0;
    Dma_Fill32Cell(fill, 0x33333333, CURTAIN_TILE(0), 0x20);
    Dma_Fill32Cell(fill, 0, CURTAIN_TILE(8), 0x20);
    REG_BG1CNT = BGCNT_CHAR_BASE(1) | BGCNT_SCREEN_BASE(12);
    REG_BG0CNT |= BGCNT_PRIORITY(2);
    background->mode = 2;
    tilemap = (u16 *)BG_SCREEN_BLOCK(12);
    for (index = 0; index <= 31; index++) {
      if (index <= 20)
        tile = 0xf080;
      else
        tile = 0xf088;
      for (column = 0; column <= 31; column++)
        *tilemap++ = tile;
    }
    {
      s32 start = 0;

      gBgScroll[0].y = 32;
      gBgScroll[1].y = 32;
      gBgScroll[1].x = 8;
      WaitFrames(1);
      REG_WIN0H = WIN_RANGE(0, 240);
      REG_WIN0V = WIN_RANGE(0, 136);
      REG_WIN1H = WIN_RANGE(0, 240);
      REG_WIN1V = WIN_RANGE(0, 136);
      REG_WININ = BATTLE_WININ;
      REG_WINOUT = BATTLE_WINOUT;
      QueueIoWriteDelay2((u32)&REG_DISPCNT,
                         DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                             DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON |
                             DISPCNT_WIN0_ON | DISPCNT_WIN1_ON);
      BattlePres_SetupTransitionScene(0, 0, 0, 180);
      *timer = start;
      Scheduler_AddOrUpdateCallback(BattlePres_AdvanceTransitionTimer, 3200);
      Scheduler_AddOrUpdateCallback(BattlePres_DrawTransitionRows, 0x480);
      Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
      gBgScroll[0].y = 32;
      WaitFrames(1);
      UiWindow_CreateWithLayoutBoundsFar(gBattleWork->partyStatus);
      WaitFrames(20);
      QueueIoWriteDelay10((u32)&REG_BG0CNT, 2);
      QueueIoWriteDelay6((u32)&REG_BG0CNT, 0);
      BattleIntro_AnnounceEncounter(enemyCount);
      Scheduler_RemoveCallback(BattlePres_AdvanceTransitionTimer);
      Scheduler_RemoveCallback(BattlePres_DrawTransitionRows);
      gBgScroll[0].y = start;
      Runtime_SetIrqHandler(2, 0, 0);
    }
  } else {
    battle = WORK_SLOT(HEAP_SLOT_BATTLE);
    background->secondMode = 1;
    background->busy = 0;
    count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, (u16 *)unitIds);
    for (index = 0; index != count; index++) {
      id = index + 120;
      if ((s32)index <= 7)
        id = index;
      object = BattleObjGet(id);
      if (UnitGetFar(id)->classId != 148)
        object->scale = 0xb333;
    }
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                        DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_WIN0_ON |
                            DISPCNT_WIN1_ON,
                        0x20000);
    WaitFrames(1);
    {
      s32 start = 0;

      battle->background = (u32)&ResourceId_VenusLighthouseAerieStormBackdrop;
      presentation.effect.count = BattleParty_ListActorIds(
          BATTLE_SIDE_ENEMIES, presentation.effect.actors);
      presentation.effect.actors[presentation.effect.count] =
          BATTLE_UNIT_LIST_END;
      BattleActor_SpawnObjectsForList(presentation.effect.actors, 0);
      BattleFx_RunShatteringOpeningFar(&presentation.effect);
      BattlePres_SetupTransitionScene(0, 0, 0, 100);
      *timer = start;
    }
    Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
    WaitFrames(1);
    WaitFrames(20);
    UiWindow_CreateWithLayoutBoundsFar(gBattleWork->partyStatus);
    QueueIoWriteDelay10((u32)&REG_BG0CNT, 2);
    QueueIoWriteDelay6((u32)&REG_BG0CNT, 0);
    REG_BLDCNT = BLEND_SPRITES_OVER_ALL;
    visibleUnits = visibleUnitIds;
    visibleCount =
        BattleParty_ListActorIds(BATTLE_SIDE_BOTH, (u16 *)visibleUnits);
    visibleUnitIds[visibleCount] = BATTLE_UNIT_LIST_END;
    BattleActor_SpawnObjectsForList(visibleUnits, 0);
    visibleCount =
        BattleParty_ListActorIds(BATTLE_SIDE_PARTY, (u16 *)visibleUnits);
    for (index = 0; index != visibleCount; index++)
      BattlePres_SetActorRecordMode(visibleUnits[index], 1);
    for (index = 0; index != 16; index++) {
      REG_BLDALPHA = BLEND_ALPHA(index, 16);
      WaitFrames(1);
    }
    for (index = 0; index != visibleCount; index++)
      BattlePres_SetActorRecordMode(visibleUnits[index], 0);
    BattleIntro_AnnounceEncounter(enemyCount);
    gBgScroll[0].y = 0;
    WaitFrames(1);
    Runtime_SetIrqHandler(2, 0, 0);
  }
  Runtime_SetIrqHandler(2, 0, 0);
  REG_BG1CNT = BATTLE_BG1CNT;
  WaitFrames(1);
  REG_BG1CNT = BATTLE_BG1CNT;
  REG_BG0CNT &= ~BGCNT_PRIORITY(2);
  gBgScroll[1].x = 8;
  REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
                DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
  Runtime_ReleaseHeapBlock(42);
}

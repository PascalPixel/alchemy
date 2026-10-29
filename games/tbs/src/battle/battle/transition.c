/// @file
/// The opening transition: the curtain, or the Venus Lighthouse storm.
// Part of battle.c.

#include "../battle_internal.h"

/// The formation the Fusion Dragon leads, fought on the Venus Lighthouse aerie:
/// it opens with the storm instead of the curtain.
#define TRANSITION_STORM_FORMATION 347

/// The curtain's map entries: tile 128 + n of CURTAIN_TILE, WINDOW_PALETTE.
#define TRANSITION_CURTAIN_ENTRY(n) (0xf080 + (n))

/// Fades the screen to black over sixteen frames as every background's
/// mosaic grows, then shows no layer.
/// Reached only through its entry in the battle bank's far-call table
/// (FarCall_BattleTable), which no code in TBS names. Sets REG_BLDCNT to
/// darken the backgrounds, leaves REG_MOSAIC at its largest and REG_DISPCNT
/// in DISPCNT_MODE_1 with every layer off; the four BGxCNT are put back as they
/// were. It draws four random numbers a frame, which only advances the
/// random number generator. Returns 0.
s32 BattleTransition_RunMosaicFadeOut(void) {
  /* FAKEMATCH(temporary): the four saved BGxCNT values written and read
   * through pointers to them, which keeps each on the stack as in the ROM; as
   * plain locals, 34 of 90 instructions differ in tbs-en. */
  u16 bg0cnt;
  u16 bg1cnt;
  u16 bg2cnt;
  u16 bg3cnt;
  u16 *saved0;
  u16 *saved1;
  u16 *saved2;
  u16 *saved3;
  s32 mosaicSize;
  volatile u16 *registerCursor;

  saved0 = &bg0cnt;
  *saved0 = REG_BG0CNT;
  REG_BG0CNT = *saved0 | BGCNT_MOSAIC;
  registerCursor = &REG_BG0CNT;
  saved1 = &bg1cnt;
  *saved1 = *++registerCursor;
  *registerCursor = *saved1 | BGCNT_MOSAIC;
  saved2 = &bg2cnt;
  *saved2 = *++registerCursor;
  *registerCursor = *saved2 | BGCNT_MOSAIC;
  saved3 = &bg3cnt;
  *saved3 = *++registerCursor;
  *registerCursor = *saved3 | BGCNT_MOSAIC;
  // 0x21 halfwords on from REG_BG3CNT is REG_BLDCNT.
  registerCursor += 0x21;
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
  registerCursor = &REG_BG0CNT;
  *registerCursor++ = *saved0;
  *registerCursor++ = *saved1;
  *registerCursor++ = *saved2;
  *registerCursor = *saved3;
  return 0;
}

/// Builds the zoomed background's 32 by 32 affine map: tiles 0 to 255 in its
/// first eight rows, 0 to 223 in the next seven, and 0xff in the rest.
/// The two runs show the background picture's two halves, which the
/// scanline BG2CNT values (BattleCamera_SetRange) read from character blocks 2
/// and 3. Called with screen block 7 by BattleBackground_Load and
/// BattleBackground_SetMode.
void Graphics_BuildSequentialTileTable(void *tilemap) {
  s32 *destination = tilemap;
  u32 packedIndices;
  s32 wordIndex;

  // Four tile numbers to a word.
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

/// Builds BG1's flat map of the same picture: rows 0 to 3 0xffff, rows 4 and
/// 5 tile 0x3ff, rows 6 to 20 tiles 0x200 to 0x3df in order, then tile 0x3ff.
/// In 256 colours from character block 0, tile 0x200 is the start of block 2,
/// where BattleBackground_Load decodes the picture. Called with screen block
/// 31 (BATTLE_BG1CNT) by BattleBackground_Load and
/// BattleBackground_SetMode.
void BattleTransition_BuildTilemap(void *tilemap) {
  s32 *destination = tilemap;
  s32 entry;
  u32 index;

  // Iwram_FillWords counts bytes; the pointer steps in words.
  Iwram_FillWords(destination, 256, -1);
  destination += 64;
  Iwram_FillWords(destination, 128, 0x03ff03ff);
  entry = (0x0201 << 16) | 0x0200;
  destination += 32;
  index = 0;
  do {
    index++;
    *destination++ = entry;
    entry += 0x00020002;
  } while (index <= 239);
  Iwram_FillWords(destination, 640, 0x03ff03ff);
}

/// Shows the zoomed background's finished page each VBlank while the
/// background is BATTLE_BACKGROUND_ZOOMED: its first line's BG2CNT now, the
/// rest by HBlank DMA 0, and its affine parameters into BG2PA to BG2Y. Runs
/// each VBlank as a task, in the VBlank pass at its highest
/// priority (TASK_PASS_VBLANK + 127), added by BattleBackground_Load and
/// BattleBackground_SetMode; BattleCamera_SetRange builds the
/// pages.
void BattleBackground_ShowZoomedPage(void) {
  struct BattleAffineHdma *scanlines;
  u16 *line;

  if (((struct BattleBackgroundWork *)HEAP_SLOT_BLOCK(
           HEAP_SLOT_BATTLE_BACKGROUND))
          ->mode == BATTLE_BACKGROUND_ZOOMED) {
    scanlines = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SCANLINES);
    line = scanlines->lines[scanlines->page];
    Io_Write16(line[0], &REG_BG2CNT);
    Dma_Set(line + 1, (void *)&REG_BG2CNT, DMA_HBLANK_HALFWORD_PER_LINE,
            &REG_DMA0SAD);
    Dma_Set(&scanlines->affine, (void *)&REG_BG2PA, DMA_COPY_WORDS(4),
            REG_DMA3);
  }
}

// Nothing in this game calls it, adds it as a task or holds its address.
// XXX (no evidence): which transition used it; with no reference left in the
// ROM, no transition the player sees runs it.
/// Copies one of eight curtain tiles from gBattleTransition_CurtainVariants
/// into CURTAIN_TILE(0), a new one every four frames while the transition timer
/// (gBattleEntryTimer) runs from 1 to 32.
void BattleTransition_UploadTileVariant(void) {
  u32 variant = *gBattleEntryTimer - 1;
  if (variant <= 31)
    Dma_Set(gBattleTransition_CurtainVariants + (variant >> 2) * 32,
            CURTAIN_TILE(0), DMA_COPY_WORDS(8), REG_DMA3);
}

/// Advances the curtain opening by a frame: scrolls BG0 from 32 pixels to 0
/// over frames 20 to 52, and over frames 0 to 80 turns the camera round to
/// YAW_PARTY_ACTION and zooms it from 180 to 100 percent.
/// Runs each frame as a task added by
/// BattleTransition_Run, which removes it once the encounter
/// is announced. Advances the timer in gBattleEntryTimer's slot
/// (HEAP_SLOT_BATTLE_ENTRY_TIMER), and writes gBgScroll[0].y and the camera's
/// yaw.
void BattleTransition_AdvanceTimer(void) {
  s32 scroll;
  struct BattleCamera *camera;
  u32 *timer;
  struct BgScroll *offset;
  u32 frame;
  u32 next;

  timer = *(u32 **)Ram_BattleEntryTimer;
  frame = *timer;
  camera = *(struct BattleCamera **)Ram_CameraWork;
  scroll = 52 - frame;
  if (scroll > 32)
    scroll = 32;
  offset = (struct BgScroll *)Ram_BgScroll;
  if (scroll < 0)
    scroll = 0;
  offset->y = scroll;
  // 360 a frame from 0xaf80 reaches 0x12000, YAW_PARTY_ACTION, at frame 80.
  if (frame <= 80U)
    camera->yaw = 45 * frame * 8 + 0xaf80;
  next = (*timer = *timer + 1);
  if (next <= 80U) {
    BattleCamera_SetView(0, 0, 0, 180 - next);
    return;
  }
  BattleCamera_SetView(0, 0, 0, 100);
}

/// Draws the curtain opening on BG1 as it stands at the current frame: over 80
/// frames a row moves up from row 13 to row 4 and its vertically flipped twin
/// down to row 20, the row's tile stepping through CURTAIN_TILE(1) to (8), the
/// last one clear. Runs each VBlank as a task in the VBlank pass, added by
/// BattleTransition_Run, reading the frame from gBattleEntryTimer's timer,
/// which BattleTransition_AdvanceTimer counts in the frame pass; writes BG1's
/// map in screen block 12.
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
    // transitionFrame / 8 as signed division rounds it; the frame is never
    // negative here.
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

/// Clears REG_BG0VOFS, so that BG0's scroll applies only above the line the
/// interrupt fires on.
/// Installed by BattleTransition_Run as the VCount interrupt's
/// handler at line 32, while gBgScroll[0].y holds BG0 scrolled.
void BattleTransition_ClearBg0Vofs(void) {
  u32 zero = 0;

  REG_BG0VOFS = zero;
}

/// Opens a battle: the curtain opening, or for the Fusion Dragon the storm
/// on the Venus Lighthouse aerie; then announces the enemies and leaves the
/// battle's display set.
/// Called once by Battle_Run with the enemy count and formation.
/// Uses HEAP_SLOT_BATTLE_ENTRY_TIMER for the opening's frame timer and frees
/// it; leaves BG1 on BATTLE_BG1CNT, BG0's priority lowered by 2 again,
/// REG_DISPCNT showing BG0, BG2 and the sprites, and no VCount handler. The
/// storm also sets BattleWork.background to the aerie's storm battle
/// background.
void BattleTransition_Run(s32 enemyCount, s32 encounterId) {
  volatile u32 fill;
  struct BattleStagingWork work;
  u16 unitIds[14];
  u16 visibleUnitIds[14];
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
    // The display off while BG1 is set up: tile 128 solid colour 3 and tile
    // 136 clear; BG1 shows the curtain from screen block 12 behind BG0, whose
    // priority is raised by 2.
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

      // Display on, with windows 0 and 1 over the field above the status
      // window (lines 0 to 136).
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
      // BG0's priority bit 1 cleared again at the next VBlank, undoing
      // BGCNT_PRIORITY(2) above; setting no bits queues a write that changes
      // nothing.
      IoWriteQueue_ClearBits16((u32)&REG_BG0CNT, 2);
      IoWriteQueue_SetBits16((u32)&REG_BG0CNT, 0);
      BattleIntro_AnnounceEncounter(enemyCount);
      Task_Remove(BattleTransition_AdvanceTimer);
      Task_Remove(BattleTransition_DrawRows);
      gBgScroll[0].y = start;
      Irq_SetHandler(IRQ_VCOUNT, 0, 0);
    }
  } else {
    // The storm: every unit but the Fusion Dragon drawn at 0.7
    // scale (16.16), the display blank but for the windows.
    battle = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE);
    view->secondMode = BATTLE_BACKGROUND_FIXED;
    view->busy = 0;
    count = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, (u16 *)unitIds);
    for (index = 0; index != count; index++) {
      // The list's index taken as a unit id: 0 to 7, then 128 onward.
      // BUG: unitIds is never read. A gap among party ids can make
      // BattleObject_Get return NULL before the loop reaches the enemy ids,
      // and the scale store then goes near address 0.
      id = index + 120;
      if ((s32)index <= ALLY_MAX)
        id = index;
      battleObject = BattleObject_Get(id);
      if (Unit_GetFar(id)->id != ENEMY_FUSION_DRAGON)
        battleObject->scale = 0xb333;
    }
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                        DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_WIN0_ON |
                            DISPCNT_WIN1_ON,
                        IO_WRITE_16);
    wait(1);
    {
      s32 start = 0;

      // The enemies alone take part in the shattering opening.
      battle->background = (u32)&ResourceId_VenusLighthouseAerieStormBackdrop;
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
    // Every unit's sprites appear; the party's fade in over 16 frames as
    // semi-transparent sprites.
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
  // Both: the battle's own BG1 and display.
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

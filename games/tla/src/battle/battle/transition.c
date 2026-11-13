/// @file
/// The battle's screen transitions: the mosaic fade to black, and the rows of
/// tiles that close in, with the HBlank scroll they run on.
// Part of battle.c.

#include "../battle_internal.h"
#include "gamepak/far_call/window.h"
#include "module/battle/battle.h"
#include "module/battle/presentation.h"
#include "module/system/system.h"

/// Fades the screen to black over sixteen frames as every background's
/// mosaic grows, then shows no layer. Sets REG_BLDCNT to darken the
/// backgrounds, leaves REG_MOSAIC at its largest and REG_DISPCNT in mode 1
/// with every layer off; the four BGxCNT are put back as they were. It draws
/// four random numbers a frame, which only advances the random number
/// generator. Returns 0.
s32 BattleTransition_RunMosaicFadeOut(void) {
  // volatile: the four saved BGxCNT values kept on the stack, each stored and
  // loaded again as in the ROM; without volatile, 65 of 94 instructions differ
  // in tla-en.
  volatile u16 bg0cnt;
  volatile u16 bg1cnt;
  volatile u16 bg2cnt;
  volatile u16 bg3cnt;
  s32 i;
  volatile u16 *reg;
  u32 saved;

  bg0cnt = REG_BG0CNT;
  REG_BG0CNT = bg0cnt | BGCNT_MOSAIC;
  reg = &REG_BG0CNT;
  bg1cnt = *++reg;
  *reg = bg1cnt | BGCNT_MOSAIC;
  bg2cnt = *++reg;
  *reg = bg2cnt | BGCNT_MOSAIC;
  bg3cnt = *++reg;
  *reg = bg3cnt | BGCNT_MOSAIC;
  reg += 0x21;
  *reg = 0x3eee;

  ScreenFade_ToBlack(16);
  for (i = 0; i <= 15; i++) {
    Random16();
    Random16();
    Random16();
    Random16();
    REG_MOSAIC = (i << 8) | i;
    wait(1);
  }

  REG_DISPCNT = 1;
  wait(4);
  saved = bg0cnt;
  reg = &REG_BG0CNT;
  *reg++ = saved;
  *reg++ = bg1cnt;
  *reg++ = bg2cnt;
  *reg = bg3cnt;
  return 0;
}

// Tiles: build a map of consecutive byte values.

/// Writes consecutive bytes 0-255 followed by 0-223, then fills the remaining
/// 544 bytes with 0xff (1024 bytes in all).
void Graphics_BuildSequentialTileTable(s32 *dst) {
  s32 v;
  s32 n;
  s32 (*fill)(void *, s32, u32);

  // Advance by four entries because each store writes four bytes.
  v = 0x03020100;
  n = 0;
  do {
    n += 1;
    *dst++ = v;
    v += 0x04040404;
  } while (n >= 0 && n <= 63);

  v = 0x03020100;
  n = 0;
  do {
    n += 1;
    *dst++ = v;
    v += 0x04040404;
  } while (n >= 0 && n <= 55);

  // The fill's entry held in a local loads its address before the size's
  // shift; called directly, 2 of 25 instructions differ in tla-en.
  fill = Iwram_FillWordsEntry;
  fill(dst, 544, -1);
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

  if (((struct BattleBackgroundView *)Ram_HeapSlots->transitionWork)->mode ==
      2) {
    records = Ram_HeapSlots->unk28[0];
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
  u32 variant = *(s32 *)Ram_HeapSlots->unk84[9] - 1;

  if (variant <= 31)
    Dma_Set(gBattleTransition_CurtainVariants + (variant >> 2) * 32,
            (void *)0x06005000, DMA_COPY_WORDS(8), REG_DMA3);
}

void BattleTransition_AdvanceTimer(void) {
  s32 scroll;
  struct BattleCamera *camera;
  u32 *timer;
  u32 frame;
  u32 next;

  timer = (u32 *)Ram_HeapSlots->unk84[9];
  frame = *timer;
  camera = (struct BattleCamera *)Ram_HeapSlots->cameraWork;
  scroll = 0x34 - frame;
  if (scroll > 0x20)
    scroll = 0x20;
  if (scroll < 0)
    scroll = 0;
  gBgScroll[0].y = scroll;
  if (frame <= 0x50U)
    camera->yaw = 45 * frame * 8 + 0xaf80;
  next = (*timer = *timer + 1);
  if (next <= 0x50U) {
    BattleCamera_SetView(0, 0, 0, 0xb4 - next);
    return;
  }
  BattleCamera_SetView(0, 0, 0, 0x64);
}

/// Draws the transition's two rows of tiles, closing in from row 13 as the
/// timer runs; the lower row takes the vertically flipped tiles.
void BattleTransition_DrawRows(void) {
  u32 i;
  s32 frame;
  s32 aligned;
  s32 tile;
  u32 row;
  u16 *tiles;

  frame = *(s32 *)Ram_HeapSlots->unk84[9];
  if (frame >= 0 && frame <= 79) {
    tile = (7 & frame) + 0xf081;
    if (frame >= 0)
      aligned = frame;
    else
      aligned = frame + 7;
    row = 13 - (aligned >> 3);
    i = 0;
    tiles = (u16 *)((row << 6) + 0x06006000);
    do {
      i++;
      *tiles = tile;
      tiles++;
    } while (i != 32);

    tile |= 0x800;
    aligned = frame;
    if (frame < 0)
      aligned += 7;
    row = (aligned >> 3) + 13;
    if (row <= 20) {
      i = 0;
      tiles = (u16 *)((row << 6) + 0x06006000);
      do {
        i++;
        *tiles = tile;
        tiles++;
      } while (i != 32);
    }
  }
}

void BattleTransition_ResetBg0Scroll(void) {
  u32 zero = 0;

  REG_BG0VOFS = zero;
}

#define DMA3 ((volatile u32 *)0x040000d4)

void BattleTransition_Run(s32 arg, s32 kind) {
  struct BattlePresentationWork work;
  u16 owners[14];
  u16 shown[14];
  s32 *timer;
  struct BattleWork *battle;
  struct BattleBackgroundView *view;
  struct HeapSlots *heap;
  s32 ownerCount;
  s32 count;
  u32 i;

  heap = Ram_HeapSlots;
  view = (struct BattleBackgroundView *)heap->transitionWork;
  timer = HeapSlot_Alloc(168, 4);
  if ((u32)(kind - 78) > 2) {
    s32 y;
    s32 zero;

    Dma_Set(gBattleTransition_CurtainTiles, (void *)0x06005020, 0x84000008,
            DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 32, (void *)0x06005040, 0x84000008,
            DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 64, (void *)0x06005060, 0x84000008,
            DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 96, (void *)0x06005080, 0x84000008,
            DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 128, (void *)0x060050a0,
            0x84000008, DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 160, (void *)0x060050c0,
            0x84000008, DMA3);
    Dma_Set(gBattleTransition_CurtainTiles + 192, (void *)0x060050e0,
            0x84000008, DMA3);
    view->secondMode = 1;
    view->busy = 0;
    view->mode = 1;
    REG_DISPCNT = 1;
    {
      u32 v = 0x33333333;
      /* FAKEMATCH(barrier): an empty asm on v. */
      __asm__("" : "=r"(v) : "0"(v));
      Iwram_FillWordsEntry((void *)0x06005000, 32, v);
    }
    IWRAM_FILL_WORDS((void *)0x06005100, 32, 0);
    REG_BG1CNT = 0xc04;
    REG_BG0CNT |= 2;
    view->mode = 2;
    {
      u16 *map = (u16 *)0x06006000;

      for (i = 0; i < 32; i++) {
        u32 tile;
        u32 column;

        if (i <= 20)
          tile = 0xf080;
        else
          tile = 0xf088;
        for (column = 0; column < 32; column++)
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
    REG_WININ = 0x3537;
    REG_WINOUT = 0x3f21;
    IoWriteQueue_Store16(0x04000000, 0x7741);
    BattleCamera_SetView(0, 0, 0, 180);
    *timer = zero;
    Task_Add(BattleTransition_AdvanceTimer, TASK_PASS_VBLANK);
    Task_Add(BattleTransition_DrawRows, TASK_PASS_FRAME);
    Irq_SetHandler(2, 32, BattleTransition_ResetBg0Scroll);
    gBgScroll[0].y = y;
    wait(1);
    PartyStatus_CreateWindowFar(Ram_HeapSlots->battleWork->partyStatus);
    wait(20);
    IoWriteQueue_ClearBits16(0x04000008, 2);
    IoWriteQueue_SetBits16(0x04000008, 0);
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
      struct BattleObj *battleObject = BattleObject_Get(id);
      u16 enemyId = ((struct BattleUnit *)Unit_Get__f11f66c(id))->enemyId;

      if (enemyId != 357 && enemyId != 369 && enemyId != 373 && enemyId != 376)
        battleObject->scale = 0xb333;
    }
    IO_WRITE_QUEUE_PUSH(0x04000000, 0x6041, 0x20000);
    wait(1);
    {
      s32 background = (s32)&ResourceId_SerpentStarsBackdrop;
      u16 *slot = (u16 *)battle->unk670;

      zero = 0;
      *slot = background;
    }
    work.count = BattleParty_ListActorIds(2, (u16 *)work.actors);
    work.actors[work.count] = 255;
    BattleActor_SpawnObjectsForList(work.actors, 0);
    BattleEffect_RunShatteringOpeningFar(&work);
    BattleCamera_SetView(0, 0, 0, 100);
    *timer = zero;
    Irq_SetHandler(2, 32, BattleTransition_ResetBg0Scroll);
    wait(1);
    wait(20);
    PartyStatus_CreateWindowFar(Ram_HeapSlots->battleWork->partyStatus);
    IoWriteQueue_ClearBits16(0x04000008, 2);
    IoWriteQueue_SetBits16(0x04000008, 0);
    REG_BLDCNT = 0x3f40;
    count = BattleParty_ListActorIds(3, shown);
    shown[count] = 255;
    BattleActor_SpawnObjectsForList((s16 *)shown, 0);
    count = BattleParty_ListActorIds(1, shown);
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
  REG_BG1CNT = 0x1f83;
  wait(1);
  REG_BG1CNT = 0x1f83;
  REG_BG0CNT &= 0xfffd;
  gBgScroll[1].x = 8;
  REG_DISPCNT = 0x1541;
  HeapSlot_Release(168);
}

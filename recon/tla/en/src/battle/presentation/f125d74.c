#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_work.h"
#include "battle_display.h"
#include "battle_side.h"
#include "motobj.h"
#include "bg_scroll.h"
#include "task.h"
#include "dma.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "irq.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"

#define DMA3 ((volatile u32 *)0x040000d4)

extern u8 Data_0812cd74[];

struct BattleUnit *Unit_Get(s32 unitId);
void PartyStatus_CreateWindowFar(s32 flags);
void Func_08138028(struct BattlePresentationWork *work);
void Func_08118bcc(s32 value);
void BattleTransition_AdvanceTimer(void);
void BattleTransition_DrawRows(void);
void Func_08125d68(void);

extern char ResourceId_SerpentStarsBackdrop;

void Func_08125d74(s32 arg, s32 kind) {
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

    Dma_Set(Data_0812cd74, (void *)0x06005020, SPLIT_CONSTANT(0x84000008), DMA3);
    Dma_Set(Data_0812cd74 + 32, (void *)0x06005040, SPLIT_CONSTANT(0x84000008), DMA3);
    Dma_Set(Data_0812cd74 + 64, (void *)0x06005060, SPLIT_CONSTANT(0x84000008), DMA3);
    Dma_Set(Data_0812cd74 + 96, (void *)0x06005080, SPLIT_CONSTANT(0x84000008), DMA3);
    Dma_Set(Data_0812cd74 + 128, (void *)0x060050a0, SPLIT_CONSTANT(0x84000008), DMA3);
    Dma_Set(Data_0812cd74 + 160, (void *)0x060050c0, SPLIT_CONSTANT(0x84000008), DMA3);
    Dma_Set(Data_0812cd74 + 192, (void *)0x060050e0, SPLIT_CONSTANT(0x84000008), DMA3);
    view->secondMode = 1;
    view->busy = 0;
    view->mode = 1;
    REG_DISPCNT = 1;
    {
      u32 v = 0x33333333;
      /* FAKEMATCH(barrier): an empty asm through v, which hides its value from
       * the compiler's constant and copy folding; as plain statements, 2 of 490
       * instructions differ in tla-en. */
      __asm__("" : "=r"(v) : "0"(v));
      Iwram_FillWords((void *)0x06005000, 32, v);
    }
    /* FAKEMATCH(one-pass): a once-only block from `Iwram_FillWords((void
     * *)0x06005100, 32, 0);`, which ends the scheduler's region there; as plain
     * statements, 6 of 490 instructions differ in tla-en. */
    do {
      Iwram_FillWords((void *)0x06005100, 32, 0);
    } while (0);
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
    QueueIoWriteDelay2(0x04000000, SPLIT_CONSTANT(0x7741));
    BattleCamera_SetView(0, 0, 0, 180);
    *timer = zero;
    Task_Add(BattleTransition_AdvanceTimer, TASK_PASS_VBLANK);
    Task_Add(BattleTransition_DrawRows, TASK_PASS_FRAME);
    Irq_SetHandler(2, 32, Func_08125d68);
    gBgScroll[0].y = y;
    wait(1);
    PartyStatus_CreateWindowFar(Ram_HeapSlots->battleWork->partyStatus);
    wait(20);
    QueueIoWriteDelay10(0x04000008, 2);
    QueueIoWriteDelay6(0x04000008, 0);
    if ((u32)(kind - 74) <= 2 || kind == 638)
      wait(60);
    else
      Func_08118bcc(arg);
    Task_Remove(BattleTransition_AdvanceTimer);
    Task_Remove(BattleTransition_DrawRows);
    gBgScroll[0].y = 0;
    Irq_SetHandler(2, 0, 0);
  } else {
    s32 zero;

    battle = heap->battleWork;
    view->secondMode = 1;
    view->busy = 0;
    ownerCount = BattleParty_PrepareActiveOwners(owners);
    ownerCount += BattleParty_PrepareReserveOwners(owners + ownerCount);
    for (i = 0; i != ownerCount; i++) {
      s32 id = owners[i];
      struct BattleObj *battleObject = BattleObject_Get(id);
      u16 enemyId = Unit_Get(id)->enemyId;

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
    Func_08138028(&work);
    BattleCamera_SetView(0, 0, 0, 100);
    *timer = zero;
    Irq_SetHandler(2, 32, Func_08125d68);
    wait(1);
    wait(20);
    PartyStatus_CreateWindowFar(Ram_HeapSlots->battleWork->partyStatus);
    QueueIoWriteDelay10(0x04000008, 2);
    QueueIoWriteDelay6(0x04000008, 0);
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
    Func_08118bcc(arg);
    gBgScroll[0].y = 0;
    wait(1);
    Irq_SetHandler(2, 0, 0);
  }
  Irq_SetHandler(2, 0, 0);
  REG_BG1CNT = 0x1f83;
  wait(1);
  REG_BG1CNT = 0x1f83;
  REG_BG0CNT &= SPLIT_CONSTANT(0xfffd);
  gBgScroll[1].x = 8;
  REG_DISPCNT = 0x1541;
  HeapSlot_Release(168);
}

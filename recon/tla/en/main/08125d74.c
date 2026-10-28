// First draft of Func_08125d74, from the listing (no ☀️ twin found): a battle
// screen change with a timer block in heap slot 168. For kinds 78 to 80 the
// party's active and reserve members are listed (every one not of enemy 357,
// 369, 373 or 376 drawn at scale 0xb333), the display switches through the
// I/O write queue, the party is spawned, Func_08138028 plays and the field
// fades back in; for any other kind seven tile rows are copied from
// Data_0812cd74 by DMA, BG1 shows a two-colour band, and
// BattlePres_AdvanceTransitionTimer and BattlePres_DrawTransitionRows run
// (with Func_08125d68 at VCount 32) for 60 frames (kinds 74 to 76 and 638) or
// until Func_08118bcc returns. Both restore BG1 and the display at the end.
// Reached from Func_081197f0.
// Differ 231 of 490 (2026-10-24): the order of the two paths and their calls
// is the native's; left are register choice (the tile loop's counters, the
// owners pointer, the zero kept in r8), a constant the native builds a second
// time (0xf088) where alchemy-gcc adds 8 to 0xf080, and instruction order.
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_work.h"
#include "battle_display.h"
#include "battle_party.h"
#include "motobj.h"
#include "bg_scroll.h"
#include "task.h"
#include "dma.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "irq.h"
#include "iwram_call.h"
#include "mem.h"
#include "ram_buffer.h"

#define DMA3 ((volatile u32 *)0x040000d4)

extern u8 Data_0812cd74[];

struct BattleUnit *Unit_Get(s32 unitId);
void PartyStatus_CreateWindowFar(s32 flags);
void Func_08138028(struct BattlePresentationWork *work);
void Func_08118bcc(s32 value);
void BattlePres_AdvanceTransitionTimer(void);
void BattlePres_DrawTransitionRows(void);
void Func_08125d68(void);

void Func_08125d74(s32 arg, s32 kind) {
  struct BattlePresentationWork work;
  u16 owners[14];
  u16 shown[14];
  s32 *timer;
  struct BattleWork *battle;
  struct BattlePresentationTransition *view;
  s32 count;
  s32 i;

  view = Ram_HeapSlots->transitionWork;
  timer = Runtime_AllocateHeapBlock(168, 4);
  if ((u32)(kind - 78) > 2) {
    u32 row;

    Dma_Set(Data_0812cd74, (void *)0x06005020, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 32, (void *)0x06005040, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 64, (void *)0x06005060, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 96, (void *)0x06005080, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 128, (void *)0x060050a0, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 160, (void *)0x060050c0, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 192, (void *)0x060050e0, 0x84000008, DMA3);
    view->active = 0;
    ((s32 *)view)[3] = 1;
    ((s32 *)view)[2] = 1;
    REG_DISPCNT = 1;
    Iwram_FillWords((void *)0x06005000, 32, 0x33333333);
    Iwram_FillWords((void *)0x06005100, 32, 0);
    REG_BG1CNT = 0xc04;
    REG_BG0CNT |= 2;
    ((s32 *)view)[2] = 2;
    {
      u16 *map = (u16 *)0x06006000;

      for (row = 0; row < 32; row++) {
        u32 tile = 0xf080;
        u32 column;

        if (row > 20)
          tile = 0xf088;
        for (column = 0; column < 32; column++)
          *map++ = tile;
      }
    }
    gBgScroll[1].x = 8;
    gBgScroll[0].y = 32;
    gBgScroll[1].y = 32;
    WaitFrames(1);
    REG_WIN0H = 0xf0;
    REG_WIN0V = 0x88;
    REG_WIN1H = 0xf0;
    REG_WIN1V = 0x88;
    REG_WININ = 0x3537;
    REG_WINOUT = 0x3f21;
    QueueIoWriteDelay2(0x04000000, 0x7741);
    BattlePres_SetupTransitionScene(0, 0, 0, 180);
    *timer = 0;
    Task_Add(BattlePres_AdvanceTransitionTimer, TASK_PASS_VBLANK);
    Task_Add(BattlePres_DrawTransitionRows, TASK_PASS_FRAME);
    Runtime_SetIrqHandler(2, 32, Func_08125d68);
    gBgScroll[0].y = 32;
    WaitFrames(1);
    PartyStatus_CreateWindowFar(Ram_HeapSlots->battleWork->partyStatus);
    WaitFrames(20);
    QueueIoWriteDelay10(0x04000008, 2);
    QueueIoWriteDelay6(0x04000008, 0);
    if ((u32)(kind - 74) <= 2 || kind == 638)
      WaitFrames(60);
    else
      Func_08118bcc(arg);
    Task_Remove(BattlePres_AdvanceTransitionTimer);
    Task_Remove(BattlePres_DrawTransitionRows);
    gBgScroll[0].y = 0;
    Runtime_SetIrqHandler(2, 0, 0);
  } else {
    u16 *owner;

    battle = Ram_HeapSlots->battleWork;
    ((s32 *)view)[3] = 1;
    view->active = 0;
    count = BattleParty_PrepareActiveOwners(owners);
    count += BattleParty_PrepareReserveOwners(owners + count);
    for (i = 0, owner = owners; i != count; i++) {
      u16 id = *owner++;
      struct BattleObj *battleObject = BattleObject_Get(id);
      u16 enemyId = Unit_Get(id)->enemyId;

      if (enemyId != 357 && enemyId != 369 && enemyId != 373 && enemyId != 376)
        battleObject->scale = 0xb333;
    }
    IO_WRITE_QUEUE_PUSH(0x04000000, 0x6041, 0x20000);
    WaitFrames(1);
    *(u16 *)((u8 *)battle + 0x670) = 100;
    work.count = BattleParty_ListActorIds(2, (u16 *)work.actors);
    work.actors[work.count] = 255;
    BattleActor_SpawnObjectsForList(work.actors, 0);
    Func_08138028(&work);
    BattlePres_SetupTransitionScene(0, 0, 0, 100);
    *timer = 0;
    Runtime_SetIrqHandler(2, 32, Func_08125d68);
    WaitFrames(1);
    WaitFrames(20);
    PartyStatus_CreateWindowFar(Ram_HeapSlots->battleWork->partyStatus);
    QueueIoWriteDelay10(0x04000008, 2);
    QueueIoWriteDelay6(0x04000008, 0);
    REG_BLDCNT = 0x3f40;
    count = BattleParty_ListActorIds(3, shown);
    shown[count] = 255;
    BattleActor_SpawnObjectsForList((s16 *)shown, 0);
    count = BattleParty_ListActorIds(1, shown);
    for (i = 0; i != count; i++)
      BattlePres_SetActorRecordMode(shown[i], 1);
    for (i = 0; i != 16; i++) {
      REG_BLDALPHA = i | 0x1000;
      WaitFrames(1);
    }
    for (i = 0; i != count; i++)
      BattlePres_SetActorRecordMode(shown[i], 0);
    Func_08118bcc(arg);
    gBgScroll[0].y = 0;
    WaitFrames(1);
    Runtime_SetIrqHandler(2, 0, 0);
  }
  Runtime_SetIrqHandler(2, 0, 0);
  REG_BG1CNT = 0x1f83;
  WaitFrames(1);
  REG_BG1CNT = 0x1f83;
  REG_BG0CNT &= 0xfffd;
  gBgScroll[1].x = 8;
  REG_DISPCNT = 0x1541;
  Runtime_ReleaseHeapBlock(168);
}

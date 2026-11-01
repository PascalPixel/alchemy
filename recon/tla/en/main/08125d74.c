// Draft, differ 18 of 490, skeleton gap 9 (tla-en, 2026-11-01), with no device.
// A battle screen change with a timer block in heap slot 168. For kinds 78 to
// 80 the party's active and reserve members are listed (every one not of enemy
// 357, 369, 373 or 376 drawn at scale 0xb333), the display switches through
// the I/O write queue, the party is spawned, Func_08138028 plays and the field
// fades back in; for any other kind seven tile rows are copied from
// Data_0812cd74 by DMA, BG1 shows a two-colour band, and
// BattleTransition_AdvanceTimer and BattleTransition_DrawRows run
// (with Func_08125d68 at VCount 32) for 60 frames (kinds 74 to 76 and 638) or
// until Func_08118bcc returns. Both restore BG1 and the display at the end.
// Reached from Func_081197f0. No twin in The Broken Seal was found.
//
// Every register now agrees; what is left is instruction order. Closed in
// plain C: i is one u32 shared by both paths, each path has its own zero,
// counts split, owners[i] as a giv, if/else for the tile (one set falling
// through lets reload_cse_move2add turn 0xf088 into adds #8), the view's
// statements in path two's order, and y = 32 set before the x store.
// The 100 in the pool is read as a resource id, as other pool-loaded small ids
// are: ResourceId_Data100 is a placeholder, not yet checked against the id
// table, and it reaches battle->unk670 (the battle background) through a local
// set before the address, with slot, so the value's register is live across
// the address reload. That puts r0 in reload's spill set (it held only r1 to
// r3 before), which fixes every scratch register in the function.
//
// Left, as a reordered instruction each:
// - The prologue: the listing has sub sp before movs r5, #192 and the argument
//   store before lsls. sched2 ranks the movs chain 37 and sub sp 36, because
//   the spilled arg's store costs 2 before the miss load of transitionWork.
//   The listing needs that cost to be 3; no source form has another path, as
//   the block holds only these instructions. This is the load-after-spill-store
//   model of -mthumb-slow-loads, so differ 0 needs a compiler decision, out of
//   scope here. The prologue accounts for 4 of the 18.
// - REG_DISPCNT = 1 is issued before the two view stores: its strh has one
//   more dependent than they do. The first fill's ldr r2 and ldr r0, the
//   second fill's ldr r0, the BG1 stores against the following address chains
//   and the tail's adds r6, #10 are the rest.
//
// Devices were measured and taken out, since they cannot reach 0 past the
// prologue. Together they give differ 4, the prologue alone: do { } while (0)
// around REG_BG1CNT = 0xc04, around the second tail REG_BG1CNT = 0x1f83 (with
// a volatile u16 *bg1cnt local set before the tail's Irq_SetHandler) and around
// the second Iwram_FillWords, plus the first fill through a static inline
// (tile, value) calling Iwram_FillWords(tile, 32, value). If the model changes,
// try this draft bare first, then those. Tried without gain: one or size or
// pattern locals, a fill pointer local, REG_DISPCNT and BG0CNT pointer locals.
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

extern char ResourceId_Data100;

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

    Dma_Set(Data_0812cd74, (void *)0x06005020, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 32, (void *)0x06005040, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 64, (void *)0x06005060, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 96, (void *)0x06005080, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 128, (void *)0x060050a0, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 160, (void *)0x060050c0, 0x84000008, DMA3);
    Dma_Set(Data_0812cd74 + 192, (void *)0x060050e0, 0x84000008, DMA3);
    view->secondMode = 1;
    view->busy = 0;
    view->mode = 1;
    REG_DISPCNT = 1;
    Iwram_FillWords((void *)0x06005000, 32, 0x33333333);
    Iwram_FillWords((void *)0x06005100, 32, 0);
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
    QueueIoWriteDelay2(0x04000000, 0x7741);
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
      s32 background = (s32)&ResourceId_Data100;
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
  REG_BG0CNT &= 0xfffd;
  gBgScroll[1].x = 8;
  REG_DISPCNT = 0x1541;
  HeapSlot_Release(168);
}

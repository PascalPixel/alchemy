/* Draft, not exact (2026-09-25): 1120 of 1116 bytes, 451 differing halfwords.
   Hand-written from the assembly: the battle-entry transition. The normal
   entry copies seven 32-byte palette rows, fills two tile blocks, draws the
   curtain map and runs the timer and row callbacks around the encounter
   announcement; kind 0x15b (the linked battle) hides unready party objects,
   spawns both sides and fades BLDY in over 16 frames. Both paths restore
   the BG and display registers.
   Found: fill must be a volatile local (declared first) to take the top
   stack slot; HImode constant stores through cast pointers pool-load, so
   DISPCNT's 1 and the scroll 32 are held in variables; the scroll block is
   a non-volatile struct pointer; 0x03001e74 is spelled from the 0x03001f00
   cell. Residual: the reference spills enemy_count to sp+0 and keeps the
   zero in r9 from before the first WaitFrames (here it lands in r7 late),
   DISPCNT's address is not derived from the DMA channel register (r3-212),
   the SetupTransitionScene zero arguments are ordered r2,r0,r1. */
#include "TYPES.H"
#include "DMA.H"
#include "IO_WRITE_QUEUE.H"

struct TransitionWork {
    u8 pad0[8];
    s32 phase;
    s32 visible;
    s32 done;
};

struct ActorList {
    u8 pad0[20];
    s32 count;
    u8 pad1[12];
    u16 ids[24];
};

struct BattleObjectSlot;

struct BgScroll {
    u16 x;
    u16 y;
};

extern volatile u16 Data_04000208;
extern u8 *Data_03001f00[];
extern const u8 Data_080c5b30[];

void *Runtime_AllocateHeapBlock(s32 slot, u32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void WaitFrames(s32 frames);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_SetIrqHandler(s32 irq, s32 mask, void (*handler)(void));
void BattlePres_SetupTransitionScene(s32 a, s32 b, s32 c, s32 frames);
void BattlePres_AdvanceTransitionTimer(void);
void BattlePres_DrawTransitionRows(void);
void Graphics_ClearBg0Vofs(void);
void Func_08015128(s32 terrain);
void BattleIntro_AnnounceEncounter(s32 enemy_count);
s32 BattleParty_ListActorIds(s32 side, u16 *ids);
u8 *GetBattleObjectSlot(s32 object_id);
u8 *Owner_GetStateFar(s32 id);
void BattleActor_SpawnObjectsForList(u16 *ids, s32 mode);
void Func_080c9028(struct ActorList *list);
void BattlePres_SetActorRecordMode(s32 id, s32 mode);

void Func_080c02a4(s32 enemy_count, s32 kind)
{
    struct TransitionWork *work;
    s32 *timer;
    u16 *map;
    u16 tile;
    u32 i;
    u32 j;
    s32 count;
    s32 id;
    u8 *object;
    u8 *link;
    u16 *ids;
    volatile u32 fill;
    struct ActorList list;
    u16 party[14];
    u16 actors[14];
    s32 zero;
    s32 mode;
    struct BgScroll *scroll;
    struct IoWriteQueue *q;
    volatile u16 *ime;
    u32 saved;

    work = (struct TransitionWork *)Data_03001f00[0];
    timer = Runtime_AllocateHeapBlock(42, 4);
    if (kind != 0x15b) {
        Dma_Set(Data_080c5b30, (void *)0x06005020, 0x84000008, (volatile u32 *)0x040000d4);
        Dma_Set(Data_080c5b30 + 32, (void *)0x06005040, 0x84000008, (volatile u32 *)0x040000d4);
        Dma_Set(Data_080c5b30 + 64, (void *)0x06005060, 0x84000008, (volatile u32 *)0x040000d4);
        Dma_Set(Data_080c5b30 + 96, (void *)0x06005080, 0x84000008, (volatile u32 *)0x040000d4);
        Dma_Set(Data_080c5b30 + 128, (void *)0x060050a0, 0x84000008, (volatile u32 *)0x040000d4);
        Dma_Set(Data_080c5b30 + 160, (void *)0x060050c0, 0x84000008, (volatile u32 *)0x040000d4);
        Dma_Set(Data_080c5b30 + 192, (void *)0x060050e0, 0x84000008, (volatile u32 *)0x040000d4);
        mode = 1;
        *(u16 *)((u8 *)0x040000d4 - 212) = mode;
        work->visible = mode;
        work->phase = mode;
        work->done = 0;
        fill = 0x33333333;
        Dma_Set((void *)&fill, (void *)0x06005000, 0x85000008, (volatile u32 *)0x040000d4);
        fill = 0;
        Dma_Set((void *)&fill, (void *)0x06005100, 0x85000008, (volatile u32 *)0x040000d4);
        *(volatile u16 *)0x0400000a = 0xc04;
        *(volatile u16 *)0x04000008 |= 2;
        work->phase = 2;
        map = (u16 *)0x06006000;
        for (i = 0; i < 32; i++) {
            if (i <= 20) {
                tile = 0xf080;
            } else {
                tile = 0xf088;
            }
            for (j = 0; j < 32; j++) {
                *map++ = tile;
            }
        }
        scroll = (struct BgScroll *)0x03001ad0;
        zero = 0;
        scroll[0].y = 32;
        scroll[1].y = 32;
        scroll[1].x = 8;
        WaitFrames(1);
        *(volatile u16 *)0x04000040 = 0xf0;
        *(volatile u16 *)0x04000044 = 0x88;
        *(volatile u16 *)0x04000042 = 0xf0;
        *(volatile u16 *)0x04000046 = 0x88;
        *(volatile u16 *)0x04000048 = 0x3537;
        *(volatile u16 *)0x0400004a = 0x3f21;
        QueueIoWriteDelay2(0x04000000, 0x7741);
        BattlePres_SetupTransitionScene(0, 0, 0, 180);
        *timer = zero;
        Scheduler_AddOrUpdateCallback(BattlePres_AdvanceTransitionTimer, 3200);
        Scheduler_AddOrUpdateCallback(BattlePres_DrawTransitionRows, 0x480);
        Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
        scroll[0].y = 32;
        WaitFrames(1);
        Func_08015128((*(u8 **)0x03001e74)[65]);
        WaitFrames(20);
        QueueIoWriteDelay10(0x04000008, 2);
        QueueIoWriteDelay6(0x04000008, 0);
        BattleIntro_AnnounceEncounter(enemy_count);
        Scheduler_RemoveCallback(BattlePres_AdvanceTransitionTimer);
        Scheduler_RemoveCallback(BattlePres_DrawTransitionRows);
        scroll[0].y = zero;
        Runtime_SetIrqHandler(2, 0, 0);
    } else {
        link = *(u8 **)((u8 *)Data_03001f00 - 140); /* FAKEMATCH: 0x03001e74 spelled from the work cell so both share one pool entry */
        work->visible = 1;
        work->done = 0;
        count = BattleParty_ListActorIds(3, party);
        for (i = 0; i != count; i++) {
            id = i + 120;
            if ((s32)i <= 7) {
                id = i;
            }
            object = GetBattleObjectSlot(id);
            if (Owner_GetStateFar(id)[0x128] != 148) {
                *(s32 *)(object + 24) = 0xb333;
            }
        }

        q = &gIoWriteQueue;
        do {
            ime = &Data_04000208;
            saved = *ime;
        } while (0);
        *ime = (u16)ime;
        count = q->count;
        if (count <= 31) {
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);
            *(u16 *)&q->count = count + 1;
            *destination++ = 0x6041;
            *destination++ = 0x04000000;
            *destination = 0x20000;
        }
        *ime = saved;
        WaitFrames(1);

        zero = 0;
        *(u16 *)(link + 0x648) = 0x21;
        list.count = BattleParty_ListActorIds(2, list.ids);
        list.ids[list.count] = 0xff;
        BattleActor_SpawnObjectsForList(list.ids, 0);
        Func_080c9028(&list);
        BattlePres_SetupTransitionScene(0, 0, 0, 100);
        *timer = zero;
        Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
        WaitFrames(1);
        WaitFrames(20);
        Func_08015128((*(u8 **)0x03001e74)[65]);
        QueueIoWriteDelay10(0x04000008, 2);
        QueueIoWriteDelay6(0x04000008, 0);
        *(volatile u16 *)0x04000050 = 0x3f40;
        ids = actors;
        count = BattleParty_ListActorIds(3, ids);
        ids[count] = 0xff;
        BattleActor_SpawnObjectsForList(ids, 0);
        count = BattleParty_ListActorIds(1, ids);
        for (i = 0; i != count; i++) {
            BattlePres_SetActorRecordMode(ids[i], 1);
        }
        for (i = 0; i != 16; i++) {
            *(volatile u16 *)0x04000052 = i | 0x1000;
            WaitFrames(1);
        }
        for (i = 0; i != count; i++) {
            BattlePres_SetActorRecordMode(ids[i], 0);
        }
        BattleIntro_AnnounceEncounter(enemy_count);
        ((volatile u16 *)0x03001ad0)[1] = 0;
        WaitFrames(1);
        Runtime_SetIrqHandler(2, 0, 0);
    }

    Runtime_SetIrqHandler(2, 0, 0);
    *(volatile u16 *)0x0400000a = 0x1f83;
    WaitFrames(1);
    *(volatile u16 *)0x0400000a = 0x1f83;
    *(volatile u16 *)0x04000008 &= 0xfffd;
    ((volatile u16 *)0x03001ad0)[2] = 8;
    *(volatile u16 *)0x04000000 = 0x1541;
    Runtime_ReleaseHeapBlock(42);
}

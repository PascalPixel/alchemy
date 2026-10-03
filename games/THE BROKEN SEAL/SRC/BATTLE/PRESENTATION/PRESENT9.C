#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"
#include "MAP_SCROLL.H"
#include "BATTLE_WORK.H"
#include "SYSTEM.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_PRESENTATION.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_RUNTIME.H"

/* The first two display registers. */
struct DisplayRegisters {
    u16 control;
    u16 green_swap;
};

/* The effect argument block as its callers allocate it: 84 bytes, with room
   for 24 actors. */
struct TransitionActorList {
    u8 unknown_00[20];
    s32 count;
    u8 unknown_18[12];
    u16 ids[24];
};

extern u8 gWorkSlot[];
/* Seven 4bpp tiles of the curtain's rows. */
extern const u8 BattlePres_CurtainTiles[];


void Runtime_SetIrqHandler(s32 irq, s32 mask, void (*handler)(void));
void BattlePres_SetupTransitionScene(s32 a, s32 b, s32 c, s32 frames);
void BattlePres_AdvanceTransitionTimer(void);
void BattlePres_DrawTransitionRows(void);
void Graphics_ClearBg0Vofs(void);
void UiWindow_CreateWithLayoutBoundsFar(s32 terrain);
void BattleIntro_AnnounceEncounter(s32 enemy_count);
s32 BattleParty_ListActorIds(s32 side, u16 *ids);
void BattleActor_SpawnObjectsForList(u16 *ids, s32 mode);
void BattleEffect_RunTileAndPaletteAnimationFar(struct TransitionActorList *list);
void BattlePres_SetActorRecordMode(s32 id, s32 mode);

#define REG_BG0CNT (*(volatile u16 *)0x04000008)
#define REG_BG1CNT (*(volatile u16 *)0x0400000a)

/*
 * The battle-entry transition. The normal entry copies the seven curtain
 * tiles, fills two tile blocks, draws the curtain map and runs the timer and
 * row callbacks around the encounter announcement. Kind 0x15b (the linked
 * battle) hides the party objects that are not ready, shows the storm
 * backdrop, spawns both sides and fades the blend in over 16 frames. Both
 * paths restore the BG and display registers.
 */
void Func_080c02a4(s32 enemy_count, s32 kind)
{
    volatile u32 fill;
    struct TransitionActorList list;
    u16 party[14];
    u16 actors[14];
    struct BattleBackgroundView *work;
    s32 *timer;
    u16 *map;
    s32 tile;
    u32 i;
    u32 j;
    s32 count;
    s32 id;
    struct BattleObjectSlot *object;
    struct BattleSession *session;
    u16 *ids;
    s32 n;

    work = *(struct BattleBackgroundView **)(gWorkSlot + 44 * 4);
    timer = Runtime_AllocateHeapBlock(42, 4);
    if (kind != 0x15b) {
        {
            /* The reference reaches the display control from the DMA
               channel address still in r3 after the last copy. */
            /* FAKEMATCH: the channel address is pinned to r3 so the display control can be addressed from it. */
            register volatile u32 *dma __asm__("r3") = (volatile u32 *)0x040000d4;

            Dma_Set(BattlePres_CurtainTiles, (void *)0x06005020, 0x84000008, dma);
            Dma_Set(BattlePres_CurtainTiles + 32, (void *)0x06005040, 0x84000008, dma);
            Dma_Set(BattlePres_CurtainTiles + 64, (void *)0x06005060, 0x84000008, dma);
            Dma_Set(BattlePres_CurtainTiles + 96, (void *)0x06005080, 0x84000008, dma);
            Dma_Set(BattlePres_CurtainTiles + 128, (void *)0x060050a0, 0x84000008, dma);
            Dma_Set(BattlePres_CurtainTiles + 160, (void *)0x060050c0, 0x84000008, dma);
            Dma_Set(BattlePres_CurtainTiles + 192, (void *)0x060050e0, 0x84000008, dma);
            ((struct DisplayRegisters *)((u8 *)dma - 212))->control = 1;
        }
        work->second_mode = 1;
        work->mode = 1;
        work->busy = 0;
        fill = 0x33333333;
        Dma_Set((void *)&fill, (void *)0x06005000, 0x85000008, (volatile u32 *)0x040000d4);
        fill = 0;
        Dma_Set((void *)&fill, (void *)0x06005100, 0x85000008, (volatile u32 *)0x040000d4);
        REG_BG1CNT = 0xc04;
        REG_BG0CNT |= 2;
        work->mode = 2;
        map = (u16 *)0x06006000;
        for (i = 0; i <= 31; i++) {
            if (i <= 20)
                tile = 0xf080;
            else
                tile = 0xf088;
            for (j = 0; j <= 31; j++)
                *map++ = tile;
        }
        {
        /* The timer's first value, kept across the calls for the scroll
           reset as well. */
        s32 start = 0;

        gBgScroll[0].y = 32;
        gBgScroll[1].y = 32;
        gBgScroll[1].x = 8;
        WaitFrames(1);
        REG_WIN0H = 0xf0;
        REG_WIN0V = 0x88;
        REG_WIN1H = 0xf0;
        REG_WIN1V = 0x88;
        REG_WININ = 0x3537;
        REG_WINOUT = 0x3f21;
        QueueIoWriteDelay2(0x04000000, 0x7741);
        BattlePres_SetupTransitionScene(0, 0, 0, 180);
        *timer = start;
        Scheduler_AddOrUpdateCallback((s32)BattlePres_AdvanceTransitionTimer, 3200);
        Scheduler_AddOrUpdateCallback((s32)BattlePres_DrawTransitionRows, 0x480);
        Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
        gBgScroll[0].y = 32;
        WaitFrames(1);
        UiWindow_CreateWithLayoutBoundsFar(gBattleWork->party_status_mode);
        WaitFrames(20);
        QueueIoWriteDelay10(0x04000008, 2);
        QueueIoWriteDelay6(0x04000008, 0);
        BattleIntro_AnnounceEncounter(enemy_count);
        Scheduler_RemoveCallback((u32)BattlePres_AdvanceTransitionTimer);
        Scheduler_RemoveCallback((u32)BattlePres_DrawTransitionRows);
        gBgScroll[0].y = start;
        Runtime_SetIrqHandler(2, 0, 0);
        }
    } else {
        session = *(struct BattleSession **)(gWorkSlot + 9 * 4);
        work->second_mode = 1;
        work->busy = 0;
        count = BattleParty_ListActorIds(3, party);
        for (i = 0; i != count; i++) {
            id = i + 120;
            if ((s32)i <= 7)
                id = i;
            object = GetBattleObjectSlot(id);
            if (Owner_GetStateFar(id)->class_id != 148)
                object->scale = 0xb333;
        }
        {
            volatile u16 *ime;
            struct IoWriteQueue *q;
            u32 saved;
            s32 used;

            q = &gIoWriteQueue;
            /* FAKEMATCH: the one-pass block keeps the queue load first and the saved copy ahead of the IME store, as in the IO write queue. */
            do {
                ime = &REG_IME;
                saved = *ime;
            } while (0);
            *ime = (u16)ime;
            used = q->count;
            if (used <= 31) {
                u32 *destination = q->entries[used];
                /* FAKEMATCH: the count is stored through a u16 pointer, as in the IO write queue, which places the store after the entry address. */
                *(u16 *)&q->count = used + 1;
                *destination++ = 0x6041;
                *destination++ = 0x04000000;
                *destination = 0x20000;
            }
            *ime = saved;
        }
        WaitFrames(1);
        {
        s32 start = 0;

        session->background = (u16)(u32)&ResourceId_VinasuChojoStormBackdrop;
        list.count = BattleParty_ListActorIds(2, list.ids);
        list.ids[list.count] = 0xff;
        BattleActor_SpawnObjectsForList(list.ids, 0);
        BattleEffect_RunTileAndPaletteAnimationFar(&list);
        BattlePres_SetupTransitionScene(0, 0, 0, 100);
        *timer = start;
        }
        Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
        WaitFrames(1);
        WaitFrames(20);
        UiWindow_CreateWithLayoutBoundsFar(gBattleWork->party_status_mode);
        QueueIoWriteDelay10(0x04000008, 2);
        QueueIoWriteDelay6(0x04000008, 0);
        REG_BLDCNT = 0x3f40;
        ids = actors;
        n = BattleParty_ListActorIds(3, ids);
        actors[n] = 0xff;
        BattleActor_SpawnObjectsForList(ids, 0);
        n = BattleParty_ListActorIds(1, ids);
        for (i = 0; i != n; i++)
            BattlePres_SetActorRecordMode(ids[i], 1);
        for (i = 0; i != 16; i++) {
            REG_BLDALPHA = i | 0x1000;
            WaitFrames(1);
        }
        for (i = 0; i != n; i++)
            BattlePres_SetActorRecordMode(ids[i], 0);
        BattleIntro_AnnounceEncounter(enemy_count);
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
    Runtime_ReleaseHeapBlock(42);
}

/* BattlePresentation_SetPaletteLevel: with IME off, copy the battle
   palette to BG palette 6 (level 0) or darken it by level/60 into the
   same place, recording the scale. */

s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count);

extern u8 gTransitionWork[];

struct Half {
    u16 v;
};

void BattlePres_UpdateHBlankScroll(void);
void Graphics_BuildSequentialTileTable(void *);
void BattlePresentation_BuildTilemap(void *);

void BattlePresentation_SetPaletteLevel(s32 unused, s32 level)
{
    /* FAKEMATCH: the IME save sits in a
     * one-pass loop around a pointer to its stack slot (taken before the IME
     * register address), and the scale is assigned inside the call's argument
     * list; the u32 restore temporary loads the saved word before the IME
     * address. */

    struct BattleSession *screen = gBattleWork;
    u16 *palette = screen->palette;
    volatile u32 ime;

    /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
    do {
        volatile u32 *slot = &ime;
        u16 *ime_reg = (u16 *)0x04000208;

        /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
        do {
            *slot = *ime_reg;
            *ime_reg = (u32)ime_reg;
        } while (0);
    } while (0);

    if (level == 0) {
        Dma_Set(palette, (void *)0x050000c0, 0x80000080, (volatile u32 *)0x040000d4);
    } else {
        Graphics_ScaleRgb555Clamped(palette, (u16 *)0x050000c0, screen->brightness = 0x10000 - level * 1092, 128);
    }
    {
        u32 saved = ime;

        *(volatile u16 *)0x04000208 = saved;
    }
}

/* BattlePresentation_ConfigurePaletteFade: start the H-blank scroll
   callback on first use and record the mode; mode 1 also queues a BG2
   control write. Copy the backdrop palette, then either copy the battle
   palette to BG palette 6 or darken each channel by fade into it, and
   rebuild the tile table and tilemap. */
void BattlePresentation_ConfigurePaletteFade(s32 mode, u16 value, s32 fade)
{
    /* FAKEMATCH: the IME save sits in one-pass loops, as in the IO write
     * queue, and the green and blue mask is a one-halfword struct, which keeps
     * it a pool constant held across the fade loop as in the ROM. */

    struct BattleBackgroundView *transition = *(struct BattleBackgroundView **)gTransitionWork;

    if (transition->mode == 0) {
        Scheduler_AddOrUpdateCallback((s32)(BattlePres_UpdateHBlankScroll), 0x4ff);
    }
    transition->mode = mode;

    if (mode == 1) {
        volatile u16 *ime;
        struct IoWriteQueue *q;
        u32 saved;
        s32 count;

        q = &gIoWriteQueue;
        {
            /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
            do {
                ime = (volatile u16 *)0x04000208;
                saved = *ime;
            } while (0);
            *ime = (u16)ime;
            count = q->count;
            if (count <= 31) {
                u32 *destination = q->entries[count];
                *(u16 *)&q->count = count + 1;
                *destination++ = 0x1f83;
                *destination++ = 0x0400000a;
                *destination = 0x20000;
            }
            *ime = saved;
        }
    }

    Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, (volatile u32 *)0x040000d4);
    *(u16 *)0x050000bc = *(u16 *)0x050001e8;

    if (fade == 0x80) {
        Dma_Set(gBattleWork->palette, (void *)0x050000c0, 0x80000080, (volatile u32 *)0x040000d4);
    } else if (fade != 0) {
        u16 *source = gBattleWork->palette;
        u16 *destination = (u16 *)0x050000c0;
        s32 i;
        struct Half mask;

        mask.v = 0x1f;
        for (i = 0; i != 128; i++) {
            s32 red = source[i] & 31;
            s32 green = (source[i] >> 5) & mask.v;
            s32 blue = (source[i] >> 10) & mask.v;

            if (red > fade) {
                red -= fade;
            } else {
                red = 0;
            }
            if (green > fade) {
                green -= fade;
            } else {
                green = 0;
            }
            if (blue > fade) {
                blue -= fade;
            } else {
                blue = 0;
            }
            destination[i] = (blue << 10) | (green << 5) | red;
        }
    }

    Graphics_BuildSequentialTileTable((void *)0x06003800);
    BattlePresentation_BuildTilemap((void *)0x0600f800);
}

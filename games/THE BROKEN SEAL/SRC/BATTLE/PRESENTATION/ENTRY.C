#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "BTLPTY.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"
#include "MAP_SCROLL.H"
#include "BTLWORK.H"
#include "SYSTEM.H"
#include "RESOURCE_IDS.H"
#include "BTLDISP.H"
#include "MOTOBJ.H"
#include "BTLSYS.H"
#include "HEAP_STATE.H"
#include "IRQ.H"
#include "BATTLE_INTRO.H"

#define BattleActor_SpawnObjectsForList ((void (*)(u16*,s32))BattleActor_SpawnObjectsForList)
#define Graphics_BuildSequentialTileTable ((void (*)(void*))Graphics_BuildSequentialTileTable)
#define BattlePresentation_BuildTilemap ((void (*)(void*))BattlePresentation_BuildTilemap)

#define gTransitionWork ((u8 (*))&gTransitionWork)

struct DisplayRegisters {
    u16 control;
    u16 green_swap;
};

struct TransitionActorList {
    u8 unknown_00[20];
    s32 count;
    u8 unknown_18[12];
    u16 ids[24];
};



void BattlePres_RunEntryTransition(s32 enemy_count, s32 kind)
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
    struct BtlObj *object;
    struct BtlWork *session;
    u16 *ids;
    s32 n;

    work = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
    timer = Runtime_AllocateHeapBlock(42, 4);
    if (kind != 0x15b) {
        {

            /* FAKEMATCH: the channel address is pinned to r3 so the display control can be addressed from it. */
            register volatile u32 *dma __asm__("r3") = REG_DMA3;

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
        Dma_Set((void *)&fill, (void *)0x06005000, 0x85000008, REG_DMA3);
        fill = 0;
        Dma_Set((void *)&fill, (void *)0x06005100, 0x85000008, REG_DMA3);
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
        Scheduler_AddOrUpdateCallback(BattlePres_AdvanceTransitionTimer, 3200);
        Scheduler_AddOrUpdateCallback(BattlePres_DrawTransitionRows, 0x480);
        Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
        gBgScroll[0].y = 32;
        WaitFrames(1);
        UiWindow_CreateWithLayoutBoundsFar(BtlWk->pty_st);
        WaitFrames(20);
        QueueIoWriteDelay10(0x04000008, 2);
        QueueIoWriteDelay6(0x04000008, 0);
        BattleIntro_AnnounceEncounter(enemy_count);
        Scheduler_RemoveCallback(BattlePres_AdvanceTransitionTimer);
        Scheduler_RemoveCallback(BattlePres_DrawTransitionRows);
        gBgScroll[0].y = start;
        Runtime_SetIrqHandler(2, 0, 0);
        }
    } else {
        session = *(gWorkSlot + HEAP_SLOT_BATTLE);
        work->second_mode = 1;
        work->busy = 0;
        count = BattleParty_ListActorIds(3, (u16 *)party);
        for (i = 0; i != count; i++) {
            id = i + 120;
            if ((s32)i <= 7)
                id = i;
            object = BtlObjGet(id);
            if (UnitGetFar(id)->class_id != 148)
                object->scale = 0xb333;
        }
        IO_WRITE_QUEUE_PUSH(0x04000000, 0x6041, 0x20000);
        WaitFrames(1);
        {
        s32 start = 0;

        session->background = (u32)&ResourceId_VinasuChojoStormBackdrop;
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
        UiWindow_CreateWithLayoutBoundsFar(BtlWk->pty_st);
        QueueIoWriteDelay10(0x04000008, 2);
        QueueIoWriteDelay6(0x04000008, 0);
        REG_BLDCNT = 0x3f40;
        ids = actors;
        n = BattleParty_ListActorIds(3, (u16 *)ids);
        actors[n] = 0xff;
        BattleActor_SpawnObjectsForList(ids, 0);
        n = BattleParty_ListActorIds(1, (u16 *)ids);
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



struct Half {
    u16 v;
};


void BattlePresentation_SetPaletteLevel(s32 unused, s32 level)
{
    struct BtlWork *screen = BtlWk;
    u16 *palette = screen->palette;
    u32 ime;
    u32 *slot = &ime;

    /* FAKEMATCH: needed to match: the one-pass block and the loop on a zero local keep the IME save through slot's register */
    do {
        u16 *ime_reg = (u16 *)0x04000208;
        s32 once = 0;

        do {
            *slot = *ime_reg;
            *ime_reg = (u32)ime_reg;
        } while (once);
    } while (0);

    if (level == 0) {
        Dma_Set(palette, (void *)0x050000c0, 0x80000080, REG_DMA3);
    } else {
        Graphics_ScaleRgb555Clamped(palette, (u16 *)0x050000c0, screen->brightness = 0x10000 - level * 1092, 128);
    }
    {
        u32 saved = *slot;

        REG_IME = saved;
    }
}

void BattlePresentation_ConfigurePaletteFade(s32 mode, u16 value, s32 fade)
{
    /* FAKEMATCH: needed to match: one-pass IME save, as in the IO write queue, and a one-halfword struct mask held across the fade loop */

    struct BattleBackgroundView *transition = *(struct BattleBackgroundView **)gTransitionWork;

    if (transition->mode == 0) {
        Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0x4ff);
    }
    transition->mode = mode;

    if (mode == 1) {
        volatile u16 *ime;
        struct IoWriteQueue *q;
        u32 saved;
        s32 count;

        q = &gIoWriteQueue;
        {
            /* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling */
            do {
                ime = &REG_IME;
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

    Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
    *(u16 *)0x050000bc = *(u16 *)0x050001e8;

    if (fade == 0x80) {
        Dma_Set(BtlWk->palette, (void *)0x050000c0, 0x80000080, REG_DMA3);
    } else if (fade != 0) {
        u16 *source = BtlWk->palette;
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

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
#include "RESOURCE.H"
#include "DMA.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"
#include "MAP_SCROLL.H"
#include "HEAP_STATE.H"
#include "RESOURCE_IDS.H"


typedef s32 (*PackedDecoder)(u8 *source, u32 destination, u32 fill);

extern u32 gFrameTick;

// スタッフロールで順に映す戦闘背景（0 で消す）
const u32 DisplayScroll_SlideResources[] = {
	(u32)&ResourceId_HaidiaArashiBackdrop,
	(u32)&ResourceId_SoruShindenBackdrop,
	(u32)&ResourceId_GrasslandBackdrop,
	(u32)&ResourceId_InteriorBackdrop,
	(u32)&ResourceId_ForestBackdrop,
	(u32)&ResourceId_GomaDouBackdrop,
	(u32)&ResourceId_KorimaMoriBackdrop,
	(u32)&ResourceId_ToretoHeyaBackdrop,
	(u32)&ResourceId_ToretoEdaBackdrop,
	(u32)&ResourceId_SnowfieldBackdrop,
	(u32)&ResourceId_MakyuriHeyaBackdrop,
	(u32)&ResourceId_MakyuriChojoBackdrop,
	(u32)&ResourceId_ImiruFuchinBackdrop,
	(u32)&ResourceId_MogoruMoriBackdrop,
	(u32)&ResourceId_ArutinYamaBackdrop,
	(u32)&ResourceId_RamakanSabakuBackdrop,
	(u32)&ResourceId_FuneKanpanBackdrop,
	(u32)&ResourceId_FuneHeyaBackdrop,
	(u32)&ResourceId_BeachBackdrop,
	(u32)&ResourceId_KorosseoBackdrop,
	(u32)&ResourceId_ArutamiraDouBackdrop,
	(u32)&ResourceId_SuharaSabakuBackdrop,
	(u32)&ResourceId_TakaraShimaCaveBackdrop,
	(u32)&ResourceId_TakaraShimaBackdrop,
	(u32)&ResourceId_SuharaGateBackdrop,
	(u32)&ResourceId_BabiChikaBackdrop,
	(u32)&ResourceId_BabiIriguchiBackdrop,
	(u32)&ResourceId_VinasuHeyaBackdrop,
	(u32)&ResourceId_VinasuChojoBackdrop,
	(u32)&ResourceId_VinasuChojoStormBackdrop,
	(u32)&ResourceId_DuskCloudsBackdrop,
	(u32)&ResourceId_VioletSkyBackdrop,
	0				// 消す
};

void Graphics_LoadCharacterBlockAndPalette(u32 resource, s32 alternate)
{
    /* FAKEMATCH: needed to match: the inline queue write keeps the one-pass IME read and u16 count-store scheduling. */
    u8 *data;
    u32 palette;
    u32 fill;
    u32 vram;
    void *decoder;
    u32 size;

    if (resource == 0) {
        Graphics_ClearCharacterBlockAndPalette(alternate);
        return;
    }
    data = Resource_GetTableEntry(resource);
    if (alternate == 0) {
        fill = 0;
        vram = 0x06000000;
        palette = 0x05000000;
    } else {
        fill = 0x80808080;
        vram = 0x06008000;
        palette = 0x05000100;
    }
    size = (u32)SentouKouka_TenkaiCodeSize;
    decoder = Runtime_AllocateHeapBlock(49, size);
    Dma_Set((void *)SentouKouka_Tenkai, decoder, 0x84000000 | (size >> 2), REG_DMA3);
    ((PackedDecoder)((union HeapState *)gWorkSlot)->slots[49])(data + 256, vram, fill);
    Runtime_ReleaseHeapBlock(49);

    {
        volatile u16 *ime;
        struct IoWriteQueue *q;
        u32 saved;
        s32 count;

        q = &gIoWriteQueue;
        /* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling. */
        do {
            ime = &REG_IME;
            saved = *ime;
        } while (0);
        *ime = (u16)ime;
        count = q->count;
        if (count <= 31) {
            u32 *destination = q->entries[count];
            *(u16 *)&q->count = count + 1;
            *destination++ = (u32)data;
            *destination++ = palette;
            *destination = 0x84000040;
        }
        *ime = saved;
    }
}

void DisplayScroll_BuildHblankWordTable(u32 *arg0)
{
    s32 count;
    u32 value = 0x01FF01FF;
    u32 step = 0x10000;

    count = 31;
    do {
        count--;
        *arg0++ = value;
    } while (count >= 0);
    count = 239;
    do {
        count--;
        *arg0++ = step;
        step += 0x20002;
    } while (count >= 0);
    count = 47;
    do {
        count--;
        *arg0++ = value;
    } while (count >= 0);
    step = 0;
    count = 191;
    do {
        count--;
        *arg0++ = step;
    } while (count >= 0);
}

void DisplayScroll_StepPositionEveryFourFrames(void)
{
    if ((gFrameTick & 3) == 0) {
        u32 decrement = 0xffff;

        gBgScroll[2].x += decrement;
        gBgScroll[3].x += decrement;
    }
}

extern u8 gOamCopyEnabled;
extern u8 gCpuLoadDisplayEnabled;
extern u8 gOptionMirror;

void WaitFrames(s32 frames);

/* FAKEMATCH: needed to match: the int temporary and one-pass write keep word-value construction, register order and a single literal pool. */
#define Io_Write16(reg, value) \
    do { \
        /* FAKEMATCH: needed to match: the one-pass register-write boundary preserves measured instruction scheduling. */ \
        /* FAKEMATCH: needed to match: the register-write word temporary preserves measured value allocation. */ \
        s32 value_ = (value); \
        (reg) = value_; \
    } while (0)

s32 DisplayScroll_RunSlideshow(void)
{
    u32 slide;
    s32 alpha;

    gOamCopyEnabled = 0;
    gCpuLoadDisplayEnabled = 0;
    gDebugTextEnabled = 0;
    gOptionMirror = 0;
    Scheduler_ResetTaskTable();
    Scheduler_AddOrUpdateCallback(DisplayScroll_StepPositionEveryFourFrames, 0x480);
    Io_Write16(REG_DISPCNT, 0x40);
    DisplayScroll_BuildHblankWordTable((u32 *)0x06007800);
    DisplayScroll_BuildHblankWordTable((u32 *)0x0600f800);
    Graphics_ClearCharacterBlockAndPalette(0);
    Graphics_ClearCharacterBlockAndPalette(1);
    Io_Write16(REG_BG2CNT, 0x1f8a);
    Io_Write16(REG_BG3CNT, 0x0f83);
    Io_Write16(REG_DISPCNT, 0x1c40);
    Io_Write16(REG_BLDCNT, 0x2844);
    DisplayScroll_InitObjectTable();
    WaitFrames(300);
    for (slide = 0; slide <= 32; slide++) {
        Graphics_LoadCharacterBlockAndPalette(DisplayScroll_SlideResources[slide], (slide & 1) ^ 1);
        for (alpha = 1; alpha <= 16; alpha++) {
            if (slide & 1)
                Io_Write16(REG_BLDALPHA, (alpha << 8) | (16 - alpha));
            else
                Io_Write16(REG_BLDALPHA, ((16 - alpha) << 8) | alpha);
            WaitFrames(4);
        }
        WaitFrames(267);
    }
    Io_Write16(REG_BLDCNT, 0);
    Io_Write16(REG_DISPCNT, 0x1040);
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    gOamCopyEnabled = 1;
    return 0;
}

#undef Io_Write16

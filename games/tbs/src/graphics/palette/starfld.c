#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "types.h"
#include "callback_scheduler.h"
#include "system.h"

#include "iwram_call.h"
#include "battle_efx.h"
#include "dma.h"
#include "resource.h"
#include "resource_ids.h"
#include "heap_state.h"
#include "fixed_math.h"
#include "io_write_queue.h"
#include "effect_step.h"
#include "btlwork.h"
#include "io_reg.h"
s32 BattleEffect_LoadWorkFar(s32 slot, s32 width_shift, s32 height_shift, s32 flags, u32 mode);

#define BattleEffect_LoadWorkFar ((s32 (*)(s32,s32,s32,s32,s32))BattleEffect_LoadWorkFar)
#define BattleFx_RunIceFleckBurstFar ((void (*)(struct EffectLaunch*))BattleFx_RunIceFleckBurstFar)

#define gTransitionWork (*(s32 * *)&gTransitionWork)

struct Spark {
    s32 pos[6];
    s32 life;
};

struct SparkWork {
    u8 sheet[0x11c0];
    struct Spark sparks[16];
    struct Ring rings[3];
    s32 frames;
    s32 ready;
    s32 origin_x;
    s32 origin_y;
    s32 origin_pending;
};


s32 BattleFx_InitializeStarField(s32 mode)
{
    struct SparkWork *work;
    struct Spark *spark;
    struct Ring *ring;
    s32 i;
    u8 *data;
    s32 scale;
    s32 zero;
    /* FAKEMATCH: needed to match: an unused Spark, the 28 bytes the native frame reserves. */
    struct Spark unused;

    gTransitionWork[2] = 1;
    work = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
    Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
    /* FAKEMATCH: needed to match: the empty loop ends a scheduling region, keeping the multiply routine's address load ahead of the spark cursor. */
    do { } while (0);
    for (i = 0; i < 16; i++) {
        s32 angle = Random16();
        u32 radius = Random16() + 0x10000;
        u32 half = radius >> 1;

        spark = &work->sparks[i];
        spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), half);
        spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), half);
        if (spark->pos[0] & 1)
            spark->pos[0] = -spark->pos[0];
        if (spark->pos[1] & 1)
            spark->pos[1] = -spark->pos[1];
        spark->pos[2] = (Random16() + 0x8000) >> 2;
        spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
        spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
        spark->pos[5] = 0;
        spark->life = (radius >> 13) + 1;
    }
    for (i = 0; i < 3; i++) {
        s32 radius = 0x1000;
        s32 angle;

        ring = &work->rings[i];
        angle = i * 0x5555;
        ring->x = Iwram_MulQ16(Trig_Cos(angle), radius);
        ring->y = Iwram_MulQ16(Trig_Sin(angle), radius);
        ring->vel_x = Iwram_MulQ16(Trig_Cos(angle), 0x200);
        ring->vel_y = Iwram_MulQ16(Trig_Sin(angle), 0x200);
        ring->age = 0;
    }
    work->frames = 0;
    work->ready = 0;
    work->origin_pending = 0;
    Iwram_ClearWords(gWorkSlot[HEAP_SLOT_BATTLE_CANVAS], 0x4000);
    data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
    Iwram_CopyWords(BG_PLTT, data, 128);
    data += 128;
    Resource_DecodeType01(data, work);
    switch (mode) {
    case 0:
        data = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteB);
        break;
    case 1:
        data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
        break;
    case 2:
        data = Resource_GetTableEntry((s32)&ResourceId_RedPaletteC);
        break;
    default:
        data = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteD);
        break;
    }
    Dma_Set(data, BG_PLTT, DMA_ENABLE32 | DMA_32BIT | 32, REG_DMA3);
    /* FAKEMATCH: needed to match: the two one-pass blocks keep the BG2 origin and matrix writes in source order, the zero loaded first. */
    do {
        zero = 0;
        REG_BG2X = zero;
        REG_BG2Y = zero;
    } while (0);
    do {
        scale = 0x100;
        REG_BG2PA = scale;
        REG_BG2PB = zero;
        REG_BG2PC = zero;
        REG_BG2PD = scale;
    } while (0);
    BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
    BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
    Scheduler_AddOrUpdateCallback((SchedulerCallback)BattleFx_UpdateStarField, 0xc80);
    return Scheduler_AddOrUpdateCallback((SchedulerCallback)Graphics_UploadVramBlock, 0xc80);
}

s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused)
{
    Iwram_ClearWords((void *)0x06004000, 0x4000);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
    REG_DISPCNT = 0x1341;
    Scheduler_RemoveCallback((SchedulerCallback)Graphics_UploadVramBlock);
    return Scheduler_RemoveCallback((SchedulerCallback)BattleFx_UpdateStarField);
}

s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count)
{
    s32 remaining;
    u32 mask_red;
    u32 mask_green;
    u32 mask_blue;
    u32 pixel;
    u32 red;
    u32 green;
    u32 blue;

    if (scale > 0x10000)
        scale = 0x10000;
    if (count > 0) {
        mask_red = 0x1f;
        mask_green = 0x3e0;
        mask_blue = 0x7c00;
        remaining = count;
        do {
            pixel = *source;
            red = pixel & mask_red;
            green = pixel & mask_green;
            blue = mask_blue & pixel;
            red *= scale;
            green *= scale;
            blue *= scale;
            pixel = ((red >> 16) & mask_red) | ((green >> 16) & mask_green);
            pixel |= (blue >> 16) & mask_blue;
            *destination = pixel;
            source++;
            destination++;
            remaining--;
        } while (remaining != 0);
    }
    return 0;
}

struct Bg2OriginWork {
    u8 unknown_0000[0x13c4];
    s32 origin_x;
    s32 origin_y;
    s32 origin_pending;
};

struct EffectLaunch {
    s32 kind;
    s32 side;
    s32 actor;
    s32 target;
    s32 unknown_0010;
    s32 count;
    s32 variant;
    s32 prelude;
    s32 unknown_0020;
    s16 actors[24];
};

extern void *gWorkSlot[];


/* FAKEMATCH: needed to match: the write goes through its own int, which loads each value as a word, and the do/while (0) ends a scheduling region after it, as in the slideshow's register writes. */
#define Io_Write16(reg, value) \
    do { \
        /* FAKEMATCH: needed to match: the one-pass register-write boundary preserves measured instruction scheduling. */ \
        /* FAKEMATCH: needed to match: the register-write word temporary preserves measured value allocation. */ \
        s32 value_ = (value); \
        (reg) = value_; \
    } while (0)

#define BG2_QUEUE_ORIGIN(ime, find_ime, origin) \
    { \
        struct IoWriteQueue *q; \
        u32 saved; \
        s32 count; \
 \
        q = &gIoWriteQueue; \
        /* FAKEMATCH: needed to match: the one-pass block keeps the saved copy ahead of the IME store, as in the IO write queue. */ \
        do { \
            find_ime; \
            saved = *ime; \
        } while (0); \
        *ime = (u16)(u32)ime; \
        count = q->count; \
        if (count <= 31) { \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4); \
            *(u16 *)&q->count = count + 1; \
            *destination++ = (u32)(origin); \
            *destination++ = 0x04000028; \
            *destination = 0x84000002; \
        } \
        /* FAKEMATCH: needed to match: the one-pass block keeps the IME restore ahead of the next statement. */ \
        do { \
            *ime = saved; \
        } while (0); \
    }

void BattleFx_PlayUnitElementEffect(s32 unit, s32 kind, s32 mode, s32 variant)
{
    /* FAKEMATCH: needed to match: an unread ten-word launch tail, the native frame's 40 extra bytes. */
    s32 spare[10];
    struct EffectPosition position;
    struct EffectPosition position2;
    struct EffectLaunch launch;
    struct EffectLaunch launch2;
    /* FAKEMATCH: needed to match: one variable, assigned twice, holds slot 12's address and later the IME address, as the bytes do; the session is slot 9, three words below. */
    volatile u16 *ime = (volatile u16 *)&gWorkSlot[12];
    struct BtlWork *session = *(struct BtlWork **)((u8 *)ime - 12);
    struct Bg2OriginWork *work;
    s32 frame;
    s32 *brightness;
    struct EffectPosition *pos;

    WaitFrames(1);
    BattlePresentation_ConfigurePaletteFade(1, session->background, 0);
    Iwram_ClearWords((void *)0x06004000, 0x4000);
    QueueIoWriteDelay2(0x04000000, 0x3741);
    QueueIoWriteDelay2(0x0400000c, 0x784);
    QueueIoWriteDelay2(0x04000050, 0x3f44);
    WaitFrames(1);
    Io_Write16(REG_WIN0H, 240);
    Io_Write16(REG_WIN0V, 0x1088);
    Io_Write16(REG_WININ, 63);
    Io_Write16(REG_WINOUT, 17);
    if (mode == 0) {
        QueueIoWriteDelay2(0x04000052, 0x100e);
        BattleFx_InitializeStarField(kind);
        for (frame = 0, pos = &position; frame <= 44; frame++) {
            brightness = &session->brightness;
            ime = &REG_IME;
            work = gWorkSlot[39];
            if (frame <= 24)
                Graphics_ScaleRgb555Clamped(session->palette, (u16 *)0x050000c0, *brightness = 0x10000 - frame * 1092, 128);
            BattleMotion_ProjectScaledPosition(unit, (s32 *)pos);
            work->origin_x = (64 - pos->x) << 8;
            work->origin_y = (64 - pos->y) << 8;
            BG2_QUEUE_ORIGIN(ime, (void)0, &work->origin_x);
            work->origin_pending = 1;
            WaitFrames(1);
        }
        Graphics_ResetVramBlockAndReleaseHeapBlocks(kind);
    } else if (mode == 1) {
        BattlePresentation_PrepareSceneFar(kind);
        for (frame = 0; frame < 40; frame++) {
            work = gWorkSlot[39];
            BattleMotion_ProjectScaledPosition(unit, (s32 *)&position2);
            work->origin_x = (64 - position2.x) << 8;
            work->origin_y = (64 - position2.y) << 8;
            {
                volatile u16 *reg;

                BG2_QUEUE_ORIGIN(reg, reg = &REG_IME, &work->origin_x);
            }
            work->origin_pending = 1;
            WaitFrames(1);
        }
        BattleFx_ScheduleCallbacksAndReleaseBlocksFar();
    } else if (mode == 2) {
        launch.prelude = 0;
        launch.kind = kind;
        launch.variant = variant;
        launch.actor = unit;
        launch.actors[0] = unit;
        launch.target = unit;
        launch.count = 1;
        launch.unknown_0010 = 1;
        BattleFx_RunIceFleckBurstFar(&launch);
    } else {
        launch2.kind = kind;
        launch2.prelude = 0;
        launch2.variant = 0;
        launch2.actor = unit;
        launch2.actors[0] = unit;
        launch2.target = unit;
        launch2.count = 1;
        launch2.unknown_0010 = 1;
        BattleFx_RunGatheringBurstFar(&launch2);
    }
}

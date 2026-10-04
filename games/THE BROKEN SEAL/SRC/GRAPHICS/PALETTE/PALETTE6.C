#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"

#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "DMA.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "HEAP_STATE.H"
#include "FIXED_MATH.H"

struct Spark {
    s32 pos[6];
    s32 life;
};

struct Ring {
    s32 x;
    s32 y;
    s32 vel_x;
    s32 vel_y;
    s32 age;
};

struct SparkWork {
    u8 padding0000[0x11c0];
    struct Spark sparks[16];
    struct Ring rings[3];
    s32 frames;
    s32 ready;
    u8 padding13c4[8];
    s32 unknown_13cc;
};

s32 BattleFx_UpdateStarField(void);
u32 Graphics_UploadVramBlock(void);
extern s32 *gTransitionWork;
s32 FarCall_EffectTable(s32 kind, s32 width, s32 height, s32 count, s32 mode);

s32 BattleFx_InitializeStarField(s32 mode)
{
    struct SparkWork *work;
    struct Spark *spark;
    struct Ring *ring;
    s32 i;
    u8 *data;
    s32 scale;
    s32 zero;
    /* FAKEMATCH: an unused Spark, the 28 bytes the native frame reserves. */
    struct Spark unused;

    gTransitionWork[2] = 1;
    work = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
    Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
    /* FAKEMATCH: the empty statement keeps the multiply routine's address load ahead of the spark cursor. */
    __asm__("");
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
    work->unknown_13cc = 0;
    Iwram_ClearWords(gWorkSlot[HEAP_SLOT_BATTLE_CANVAS], 0x4000);
    data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
    Iwram_CopyWords((void *)0x05000000, data, 128);
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
    Dma_Set(data, (void *)0x05000000, 0x84000020, (volatile u32 *)0x040000d4);
    /* FAKEMATCH: the two one-pass blocks keep the BG2 origin and matrix writes in source order, the zero loaded first. */
    do {
        zero = 0;
        *(s32 *)0x04000028 = zero;
        *(s32 *)0x0400002c = zero;
    } while (0);
    do {
        scale = 0x100;
        *(u16 *)0x04000020 = scale;
        *(u16 *)0x04000022 = zero;
        *(u16 *)0x04000024 = zero;
        *(u16 *)0x04000026 = scale;
    } while (0);
    FarCall_EffectTable(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
    FarCall_EffectTable(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
    Scheduler_AddOrUpdateCallback((s32)BattleFx_UpdateStarField, 0xc80);
    return Scheduler_AddOrUpdateCallback((s32)Graphics_UploadVramBlock, 0xc80);
}

u32 Graphics_UploadVramBlock(void);
s32 BattleFx_UpdateStarField(void);

/* The presentation passes its kind, but cleanup does not use it. The
   resident clear entry consumes destination/size and sets its fill to zero. */
s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused)
{
    Iwram_ClearWords((void *)0x06004000, 0x4000);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
    *(u16 *)0x04000000 = 0x1341;
    Scheduler_RemoveCallback((u32)&Graphics_UploadVramBlock);
    return Scheduler_RemoveCallback((u32)&BattleFx_UpdateStarField);
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

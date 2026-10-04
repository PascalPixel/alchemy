/* 2026-10-04 frame audit: BattleFx_InitializeStarField, 608 bytes, pool included.
 * Uncredited under S1/S2/C1; the owned English ROM supplies the raw listing.
 * Entries JA/EN/DE/ES/FR/IT: 080b8470, 080c1470, 080c3a88, 080c5488, 080c5488, 080c1488.
 * Native frame: 36 bytes. The only uses are the mode spill at sp+4 and
 * the fifth FarCall_EffectTable argument at sp+0. The sparks are written
 * directly into the heap work block; no local Spark address is formed.
 * The former unused Spark supplied 28 bytes solely for size, outside S2.
 * Removing it gives score 2: only frame adjustments, 36 to 8 bytes.
 * A consumed local Spark initialized before copying into work->sparks[i]
 * was measured at 3050 (27 register, 5 stack, 10 operand, 8 reordered,
 * 17 inserted, 5 deleted); its 40-byte frame and real copy operations
 * do not occur in the native function. Keep this ordinary near match.
 * Independent native-stack/consumer review agrees the explicit reserve
 * has no consumed owner. Adjacent functions remain in their source module.
 */
#include "RUNTIME_MEM.H"
#include "BATTLE_PARTY.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"

extern u8 Data_03001e74[];

struct State080c1084 {
    u8 padding_000[0x64e];
    u16 index;
    u16 field_650;
};

extern s8 Data_080c5c10[];

void Graphics_AdvancePaletteCycle(void);
void BattlePres_ClearAllActorRecordModes(void);
void QueueIoWriteDelay2(u32, u32);
void BattlePres_SetActorRecordMode(s32, s32);
extern u8 *gBattleWork;

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

typedef void (*DrawFunc)(void *dest, u8 *src, s32 x, s32 y, s32 width, s32 height);

struct WorkSlots {
    u8 padding00[0x9c];
    struct SparkWork *work;
    void *dest;
    u8 padding0a4[0xb8 - 0xa4];
    DrawFunc draw_spark;
    DrawFunc draw_ring;
};

extern struct WorkSlots Data_03001e50;
extern s32 Data_080c3604[];
extern u8 Data_080c3620[];
extern s32 Data_080c3628[];
s32 FixedSqrt(s32 value);
u32 Random16(void);
s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);

u32 ColorBuffer_BackupAndScaleThreeQuarters(void *, void *, u32);

/* graphics/vram/Display_UploadBlock.c */
struct State {
    u8 unknown[156];
    u32 context;
    u32 source;
};

extern struct State gWorkSlot;

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

    gTransitionWork[2] = 1;
    work = Runtime_AllocateBlock(39, 0x13d0);
    Runtime_AllocateHeapBlock(40, 0x4000);
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
    Iwram_ClearWords((void *)gWorkSlot.source, 0x4000);
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
    FarCall_EffectTable(46, 7, 7, 3, 3);
    FarCall_EffectTable(47, 7, 7, 3, 2);
    Scheduler_AddOrUpdateCallback((s32)BattleFx_UpdateStarField, 0xc80);
    return Scheduler_AddOrUpdateCallback((s32)Graphics_UploadVramBlock, 0xc80);
}

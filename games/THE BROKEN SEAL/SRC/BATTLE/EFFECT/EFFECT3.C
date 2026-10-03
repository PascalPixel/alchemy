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

void Graphics_AdvancePaletteCycle(void)
{
    s32 _c0 = ((u32)&Data_03001e74);
    s8 *table;
    u16 index;
    s32 next;
    struct State080c1084 *state;

    state = *(struct State080c1084 **)_c0;
    if ((state != NULL) && (state->field_650 != 0)) {
        FIELD_AT_OFFSET((void *)0x04000050, s16 *, 0) = 0x3F90;
        FIELD_AT_OFFSET((void *)0x04000050, s16 *, 2) = 0x10;
        table = Data_080c5c10;
        *(s16 *)0x04000054 = table[state->index];
        index = state->index;
        next = (index + 1) & 0xF;
        if ((u32)index > 0xEU) {
            next |= 0x10;
        }
        state->index = next;
    }
}

void BattlePres_SetActorModes(u16 *actors, s32 mode)
{
    s16 active_actors[14];
    u8 *battle = gBattleWork;
    u32 count;
    u32 i;
    volatile u16 *blend_y;

    if (mode == 0) {
        Scheduler_RemoveCallback((u32)((s32)Graphics_AdvancePaletteCycle));
        *(volatile u16 *)0x04000054 = mode;
        BattlePres_ClearAllActorRecordModes();
        WaitFrames(1);
        QueueIoWriteDelay2(0x04000050, 0);
    }
    if (battle != 0 && mode != 0) {
        u32 zero = 0;
        u32 sixteen;

        *(u16 *)(battle + 0x650) = mode;
        *(u16 *)(battle + 0x64e) = zero;
        blend_y = (volatile u16 *)0x04000054;
        *blend_y = zero;
        sixteen = 16;
        /* Preserve the volatile register-store scheduling used by agbcc. */
        /* FAKEMATCH: removing this one-pass block changes instruction scheduling. */
        do {
            blend_y[-1] = sixteen;
        } while (0);

        count = BattleParty_ListActorIds(3, (u16 *)active_actors);
        for (i = 0; i < count; i++)
            BattlePres_SetActorRecordMode(active_actors[i], mode & 1);

        if (actors != 0) {
            u32 actor = *actors;
            i = 0;
            actors++;
            if (actor != 0xff) {
                do {
                    BattlePres_SetActorRecordMode(actor, (mode & 1) ^ 1);
                    i++;
                    if (i > 13)
                        break;
                    actor = *actors++;
                } while (actor != 0xff);
            }
        }
        WaitFrames(1);
        QueueIoWriteDelay2(0x04000050, 0);
        Scheduler_AddOrUpdateCallback((s32)Graphics_AdvancePaletteCycle, 0x480);
    }
}

/* FAKEMATCH: the scheduler ignores the result; a non-void signature keeps
   the reference's return-address pop into r1. */
s32 BattleFx_UpdateStarField(void)
{
    s32 j;
    s32 frame;
    u32 size;
    s32 ring_x;
    s32 ring_y;
    s32 phase;
    s32 age;
    void *dest;
    s32 i;
    struct SparkWork *work;
    DrawFunc draw;
    struct Spark *spark;
    struct Ring *ring;
    s32 *pos;
    s32 k;
    s32 dist;
    s32 scale;
    s32 value;
    s32 step;
    u32 radius;
    s32 angle;
    u32 half;
    s32 x;
    s32 y;
    s32 life;

    dest = Data_03001e50.dest;
    work = Data_03001e50.work;
    work->ready = 0;
    draw = Data_03001e50.draw_spark;
    for (i = 0; i < 16; i++) {
        spark = &work->sparks[i];
        life = spark->life;
        if (life != 0) {
            dist = FixedSqrt((spark->pos[0] >> 8) * (spark->pos[0] >> 8)
                             + (spark->pos[1] >> 8) * (spark->pos[1] >> 8)
                             + (spark->pos[2] >> 8) * (spark->pos[2] >> 8));
            if (dist <= 0xfff) {
                spark->life = 0;
            } else {
                scale = Iwram_RatioMulQ14(dist, 0x10000);
                spark->life--;
                pos = spark->pos;
                for (k = 2; k >= 0; k--) {
                    value = *pos;
                    step = Iwram_MulQ16(Iwram_MulQ16(-value >> 8, scale), 0x13000);
                    pos[3] = pos[3] - (pos[3] >> 7) + step;
                    *pos = value + pos[3];
                    pos++;
                }
            }
            life = spark->life;
            if (life != 0)
                goto draw_spark;
        }
        if (work->frames <= 24) {
            angle = Random16();
            radius = Random16() + 0x10000;
            half = radius >> 1;
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
            life = spark->life = (radius >> 13) + 1;
        }
        if (life != 0) {
        draw_spark:
            x = (spark->pos[0] >> 10) + 64;
            y = (spark->pos[1] >> 10) + 64;
            frame = life;
            if (frame < 0)
                frame = 0;
            else if (frame > 6)
                frame = 6;
            size = Data_080c3620[frame];
            draw(dest, (u8 *)work + Data_080c3604[frame], x - (size >> 1), y - (size >> 1), size, size);
        }
    }
    draw = Data_03001e50.draw_ring;
    for (ring = work->rings, j = 2; j >= 0; j--, ring++) {
        ring->x += ring->vel_x;
        ring->y += ring->vel_y;
        ring_x = ring->x >> 10;
        ring_y = ring->y >> 10;
        phase = 3 - ring->age / 8;
        if (phase >= 0) {
            ring->age++;
            draw(dest, (u8 *)work + Data_080c3628[phase], ring_x + 48, ring_y + 48, 32, 32);
        }
    }
    work->frames++;
    work->ready = 1;
}

u32 Graphics_UploadVramBlock(void)
{
    void *source = (void *)gWorkSlot.source;
    u8 *context = (u8 *)gWorkSlot.context;

    if (source != 0) {
        u32 *active = (u32 *)(context + 0x13C0);

        if (*active != 0) {
            *active = 0;
            return ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
        }
    }
    return (u32)source;
}

extern s32 *gTransitionWork;
s32 FarCall_EffectTable(s32 kind, s32 width, s32 height, s32 count, s32 mode);

/* Battle effect: set up the star field. Sixteen sparks start on a random
   ring around the centre and three rings a third of a turn apart; the sheet
   resource gives the backdrop colours and the spark cells, the mode picks
   the palette, BG2 is reset to an unscaled view, and the two blitters and
   the update and upload callbacks are installed. */
s32 BattleFx_InitializeStarField(s32 mode)
{
    /* FAKEMATCH: the reference frame holds one spark it never uses. */
    struct Spark unused;
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

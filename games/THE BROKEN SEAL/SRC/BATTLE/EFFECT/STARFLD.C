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
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "IO_REG.H"
#include "BTLDISP.H"
#include "FIXED_MATH.H"

void QueueIoWriteDelay2(u32, u32);
#include "BTLWORK.H"

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

typedef void (*DrawFunc)(void *dest, u8 *src, s32 x, s32 y, s32 width, s32 height);



#include "HEAP_STATE.H"

#define ColorBuffer_BackupAndScaleThreeQuarters ((u32 (*)(void*,void*,u32))ColorBuffer_BackupAndScaleThreeQuarters)

void Graphics_AdvancePaletteCycle(void)
{
    struct BtlWork *battle = BtlWk;
    s8 *table;
    u16 step;
    s32 next;

    if (battle != NULL && battle->highlight_mode != 0) {
        REG_BLDCNT = 0x3f90;
        REG_BLDALPHA = 0x10;
        table = BattleFx_HighlightBlendRamp;
        REG_BLDY = table[battle->highlight_step];
        step = battle->highlight_step;
        next = (step + 1) & 0xf;
        if (step > 14)
            next |= 0x10;
        battle->highlight_step = next;
    }
}

void BattlePres_SetActorModes(u16 *actors, s32 mode)
{
    s16 active_actors[14];
    struct BtlWork *battle = BtlWk;
    u32 count;
    u32 i;
    volatile u16 *blend_y;

    if (mode == 0) {
        Scheduler_RemoveCallback(Graphics_AdvancePaletteCycle);
        REG_BLDY = mode;
        BattlePres_ClearAllActorRecordModes();
        WaitFrames(1);
        QueueIoWriteDelay2(0x04000050, 0);
    }
    if (battle != 0 && mode != 0) {
        u32 zero = 0;
        u32 sixteen;

        battle->highlight_mode = mode;
        battle->highlight_step = zero;
        blend_y = &REG_BLDY;
        *blend_y = zero;
        sixteen = 16;

        /* FAKEMATCH: needed to match: removing this one-pass block changes instruction scheduling */
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
        Scheduler_AddOrUpdateCallback(Graphics_AdvancePaletteCycle, 0x480);
    }
}

s32 BattleFx_UpdateStarField(void)
{
    /* FAKEMATCH: needed to match: the ignored non-void result keeps the return-address pop in r1 */
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

    dest = gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
    work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
    work->ready = 0;
    draw = (DrawFunc)gWorkSlot[HEAP_SLOT_BLITTER];
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
            size = BattleFx_StarSizes[frame];
            draw(dest, (u8 *)work + BattleFx_StarWorkOffsets[frame], x - (size >> 1), y - (size >> 1), size, size);
        }
    }
    draw = (DrawFunc)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
    for (ring = work->rings, j = 2; j >= 0; j--, ring++) {
        ring->x += ring->vel_x;
        ring->y += ring->vel_y;
        ring_x = ring->x >> 10;
        ring_y = ring->y >> 10;
        phase = 3 - ring->age / 8;
        if (phase >= 0) {
            ring->age++;
            draw(dest, (u8 *)work + BattleFx_RingFrameOffsets[phase], ring_x + 48, ring_y + 48, 32, 32);
        }
    }
    work->frames++;
    work->ready = 1;
}

u32 Graphics_UploadVramBlock(void)
{
    void *source = gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
    u8 *context = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];

    if (source != 0) {
        u32 *active = (u32 *)(context + 0x13C0);

        if (*active != 0) {
            *active = 0;
            return ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
        }
    }
    return (u32)source;
}

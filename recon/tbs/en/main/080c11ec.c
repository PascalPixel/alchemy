/* DRAFT: 588 of 588 bytes, 5 differing halfwords, same control flow and
 * pool. Remaining: before the spark loop the ROM sets the counter 15 in r2
 * and loads the IWRAM multiply routine through r3 (which waits on the draw
 * callback just moved out of r3); this compile gives the routine r2 and the
 * counter r3, so sched2 issues the routine load first. The -da dumps show
 * the loop pass keeps the source order; the swap comes from reload. The
 * sprite tables are not const: the ROM stores the ring age before loading
 * the ring table entry. */
#include "TYPES.H"
#include "IWRAM_CALL.H"

struct Spark {
    s32 pos[3];
    s32 vel_x;
    s32 vel_y;
    s32 unused;
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

#define Iwram_DivQ16 ((s32 (*)(s32, s32))0x0300013c)

s32 FixedSqrt(s32 value);
u32 Random16(void);
s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);

/* Returns nothing; the ROM pops the return address into r1 as for a
   value-returning function. */
s32 Func_080c11ec(void)
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
    for (i = 15, spark = work->sparks; i >= 0; spark++, i--) {
        life = spark->life;
        if (life != 0) {
            dist = FixedSqrt((spark->pos[0] >> 8) * (spark->pos[0] >> 8)
                             + (spark->pos[1] >> 8) * (spark->pos[1] >> 8)
                             + (spark->pos[2] >> 8) * (spark->pos[2] >> 8));
            if (dist <= 0xfff) {
                spark->life = 0;
            } else {
                scale = Iwram_DivQ16(dist, 0x10000);
                spark->life--;
                pos = spark->pos;
                for (k = 2; k >= 0; k--) {
                    value = *pos;
                    step = Iwram_MulQ16(Iwram_MulQ16(-value >> 8, scale), 0x13000);
                    pos[3] = pos[3] - (pos[3] >> 7) + step;
                    *pos++ = value + pos[3];
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
            spark->vel_x = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
            spark->vel_y = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
            spark->unused = 0;
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

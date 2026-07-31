#include "types.h"

/*
 * The per-frame step of the title backdrop that 0x080c1470 seeds: integrate
 * the sixteen particles, respawn the expired ones while the frame counter is
 * still low, draw each live particle with the sprite its remaining life
 * selects, then advance and draw the three rings.  The scene block, the draw
 * surface and the two blit entry points all come out of the table at
 * 0x03001e50, at +156, +160, +184 and +188.
 *
 * A particle is retired when the length of its position vector, measured by
 * Func_080045d4 over the three components shifted down by 8, has fallen to
 * 0x00000fff or less.  Otherwise 0x0300013c returns 0x10000 over that length
 * - the ratio helper gives its SECOND argument over its first - and each of
 * the three components is pulled toward the origin by that ratio while its
 * velocity decays by 1/128 per frame.
 *
 * The bl Func_080072f0 at 0x080c126c is the runtime _call_via_r3 thunk inside
 * 0x080072e4..0x08007320 - index 3 - an indirect call through the pooled
 * 0x0300013c.  The bl Func_08007310 at 0x080c1380 and 0x080c13e6 is
 * _call_via_fp, index 11, an indirect call through the blit pointer.  The
 * mov ip,pc ; bx r6 / bx r9 sequences at 0x080c1288, 0x080c1290, 0x080c12dc
 * and 0x080c12ec are ordinary indirect calls to the pooled 0x03000118, the
 * IWRAM-relocated ARM fixed-point multiply, as recorded for 0x0800ebec; the
 * movs r0,r0 at 0x080c12da and 0x080c12ea is inside the instruction stream,
 * aligning the return address mov ip,pc captures so it lands past the bx.
 *
 * if (v < 0) v += 7; v >>= 3 at 0x080c13ba is the truncating divide by eight
 * that turns a ring's frame counter into its sprite index.
 *
 * The epilogue at 0x080c1406 ends pop {r1}; bx r1 with N != 0, so r0 is the
 * result, but no path sets it deliberately - the last write is either the
 * movs r0,#32 of a ring blit or the 3 - frame/8 of the ring test.  That is
 * written as an uninitialised result local.
 *
 * Published as a callback at 0x080c16c8 (by 0x080c1470's Func_080041d8 call)
 * and at 0x080c171c, with no direct call site.
 */

struct Particle_080c11ec {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 life;
};

struct Ring_080c11ec {
    s32 x;
    s32 y;
    s32 velocity_x;
    s32 velocity_y;
    s32 frame;
};

struct Scene_080c11ec {
    u8 unknown_0000[4544];
    struct Particle_080c11ec particles[16];
    struct Ring_080c11ec rings[3];
    s32 frames;
    s32 ready;
};

typedef void (*Blit_080c11ec)(void *surface, const void *sprite, s32 x, s32 y,
                              s32 width, s32 height);

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080c11ec)(s32 a, s32 b);
/* 0x0300013c, the ratio helper: second argument over first. */
typedef s32 (*Ratio_080c11ec)(s32 first, s32 second);

extern u8 Data_03001e50[];

s32 Func_080045d4(s32 square);
s32 Func_08004458(void);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);

s32 Func_080c11ec(void)
{
    Multiply_080c11ec multiply = (Multiply_080c11ec)0x03000118;
    struct Scene_080c11ec *scene =
        *(struct Scene_080c11ec **)(Data_03001e50 + 156);
    void *surface = *(void **)(Data_03001e50 + 160);
    Blit_080c11ec blit = *(Blit_080c11ec *)(Data_03001e50 + 184);
    const u8 *sizes = (const u8 *)0x080c3620;
    const u32 *sprites = (const u32 *)0x080c3604;
    const u32 *ring_sprites = (const u32 *)0x080c3628;
    struct Particle_080c11ec *particle = scene->particles;
    struct Ring_080c11ec *ring;
    s32 result;
    s32 count;
    s32 life;

    scene->ready = 0;

    for (count = 15; count >= 0; count--) {
        life = particle->life;
        if (life != 0) {
            s32 length = Func_080045d4((particle->x >> 8) * (particle->x >> 8) +
                                       (particle->y >> 8) * (particle->y >> 8) +
                                       (particle->z >> 8) * (particle->z >> 8));

            if (length <= 0x00000fff) {
                particle->life = 0;
            } else {
                s32 ratio = ((Ratio_080c11ec)0x0300013c)(length, 0x10000);
                s32 *component = &particle->x;
                s32 axis;

                particle->life = life - 1;
                for (axis = 2; axis >= 0; axis--) {
                    s32 value = component[0];
                    s32 pull = multiply(multiply((-value) >> 8, ratio),
                                        0x13000);
                    s32 velocity = component[3];

                    velocity = velocity - (velocity >> 7) + pull;
                    component[3] = velocity;
                    component[0] = value + velocity;
                    component++;
                }
            }
            life = particle->life;
        }

        if (life == 0) {
            if (scene->frames <= 24) {
                s32 heading = Func_08004458();
                u32 span = 0x10000 + (u32)Func_08004458();
                s32 scale = (s32)(span >> 1);

                particle->x = multiply(Func_0800231c(heading), scale);
                particle->y = multiply(Func_08002322(heading), scale);
                if ((particle->x & 1) != 0)
                    particle->x = -particle->x;
                if ((particle->y & 1) != 0)
                    particle->y = -particle->y;

                particle->z = (s32)(((u32)Func_08004458() + 0x8000) >> 2);
                particle->velocity_y =
                    ((-particle->y) >> 7) + ((-particle->x) >> 8);
                particle->velocity_z = 0;
                particle->velocity_x =
                    ((-particle->x) >> 7) + (particle->y >> 8);
                particle->life = (s32)(span >> 13) + 1;
                life = particle->life;
            }
        }

        if (life != 0) {
            s32 x = (particle->x >> 10) + 64;
            s32 y = (particle->y >> 10) + 64;
            s32 index = life;
            s32 size;
            s32 half;

            if (index < 0)
                index = 0;
            else if (index > 6)
                index = 6;

            size = sizes[index];
            half = (s32)((u32)size >> 1);
            blit(surface, (const u8 *)scene + sprites[index],
                 x - half, y - half, size, size);
        }

        particle++;
    }

    blit = *(Blit_080c11ec *)(Data_03001e50 + 188);
    ring = scene->rings;
    for (count = 2; count >= 0; count--) {
        s32 x = ring->x + ring->velocity_x;
        s32 y = ring->y + ring->velocity_y;
        s32 frame;
        s32 index;

        ring->x = x;
        ring->y = y;
        frame = ring->frame;
        index = 3 - ((frame < 0 ? frame + 7 : frame) >> 3);
        if (index >= 0) {
            ring->frame = frame + 1;
            blit(surface, (const u8 *)scene + ring_sprites[index],
                 (x >> 10) + 48, (y >> 10) + 48, 32, 32);
        }
        ring++;
    }

    scene->frames++;
    scene->ready = 1;
    return result;
}

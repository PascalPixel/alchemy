#include "types.h"

struct Particle_080dc1ec {
    s32 x;
    s32 y;
    s32 z;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 timer;
};

struct Position_080dc1ec {
    s32 x;
    s32 y;
    s32 z;
};

struct Runtime_080dc1ec {
    u8 pad_0000[0x7780];
    s32 frame_ready;
    s32 phase;
    u8 pad_7788[0x9c];
    s32 complete;
    void *argument;
};

struct Globals_080dc1ec {
    struct Runtime_080dc1ec *runtime;
    s32 render_context;
};

extern void *Data_03001e80;
extern const u16 Data_080ede48[];

void Func_080cd594(s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080cef64(s32, void *);
s32 Func_08004458(void);
void Func_080041d8(s32, s32);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_08004cb4(void *);
void Func_080049e8(void);
void Func_08004c1c(s32);
void Func_08004bd4(s32);
void Func_08004c6c(s32);
void Func_080e3944(struct Particle_080dc1ec *, struct Position_080dc1ec *);
void Func_08004a5c(void);
/*
 * __call_via_rN veneer site, resolved per-site against the ROM.
 *
 * The `bl Func_080072f4` at 0x080dc390 is `__call_via_r4` -- an indirect call
 * through r4, loaded at 0x080dc38e from `[sp, #20]`.
 *
 * That stack slot is not opaque: `add r1, sp, #20` at 0x080dc226 hands it to
 * Func_080cef64 as an OUT-PARAMETER two hundred instructions earlier, and
 * Func_080cef64 is byte-exact in src/, so its body can simply be read. It
 * writes output[0] = *(u32 *)(Data_03001e50 + 184) and
 * output[1] = *(u32 *)(Data_03001e50 + 188).
 *
 * 184 = 46 * 4 and 188 = 47 * 4, so those are Func_080048b0's allocation
 * slots for ids 46 and 47 -- the SAME slot table as ids 0x31 and 0x32 read at
 * 0x08012388, 0x08021be0, 0x080f02b0 and 0x080196c4. The two calls to
 * Func_080ed408(46, ...) and Func_080ed408(47, ...) just above each write are
 * what fill them. So the whole "renderer globals at 0x03001f08 / 0x03001f0c"
 * family is one mechanism with the heap-kernel family: relocated routines
 * living in numbered allocator slots.
 *
 * The callee here is state[0], i.e. slot 46.
 *
 * ARITY: six. r4 is OUTSIDE the r0-r3 argument registers, so the callee never
 * occupied an argument slot and every argument the draft passed is real --
 * four in r0-r3 plus two pushed at [sp, #0] and [sp, #4] (0x080dc388,
 * 0x080dc38a). Nothing to strip here, which is the opposite failure mode from
 * the r0-r3 sites and the reason the domain limit has to be stated.
 */
typedef void (*ParticleRenderer)(s32 context, const void *sprite, s32 x, s32 y,
                                 s32 size, s32 doubledSize);
void Func_080e38b8(struct Particle_080dc1ec *, s32, s32);
void Func_080030f8(s32);
void Func_08004278(s32);
void Func_08002dd8(s32);
void Func_080cdbc0(void);

void Func_080dc1ec(void *argument)
{
    struct Globals_080dc1ec *globals =
        (struct Globals_080dc1ec *)0x03001eec;
    struct Runtime_080dc1ec *runtime = globals->runtime;
    struct Particle_080dc1ec *particles =
        (struct Particle_080dc1ec *)0x02010000;
    s32 render_context = globals->render_context;
    u32 state[2];
    struct Position_080dc1ec camera;
    s32 frame;
    s32 index;

    runtime->argument = argument;
    Func_080cd594(0);
    Func_080e0524(0x8c, runtime, 1, 1);
    Func_080cef64(((s32 *)argument)[1] ^ 1, state);

    for (index = 0; index < 256; index++) {
        particles[index].x = ((Func_08004458() & 0xff) - 127) << 16;
        particles[index].y = ((Func_08004458() & 0xff) - 127) << 16;
        particles[index].z = ((Func_08004458() & 0xff) - 127) << 16;
        particles[index].dx = 0;
        particles[index].dy = 0;
        particles[index].dz = 0;
        particles[index].timer = 0;
    }

    runtime->frame_ready = 1;
    runtime->phase = 0;
    Func_080041d8(0x080cd261, 0x480);

    camera.x = 0;
    camera.y = 0x500000;
    camera.z = 0;

    for (frame = 0; frame < 160; frame++) {
        void *object = Data_03001e80;
        s32 positive_step = frame << 8;
        s32 negative_step = -frame << 8;

        Func_080049ac();
        Func_080051d8(object, (u8 *)object + 12);
        Func_08004cb4(&camera);

        for (index = 0; index < 64; index++) {
            struct Particle_080dc1ec *particle = &particles[index];
            s32 quarter = index < 0 ? (index + 3) >> 2 : index >> 2;

            if (frame > quarter && particle->timer == 0) {
                struct Position_080dc1ec position;
                s32 size;
                s32 doubled_size;
                s32 phase = index & 3;

                Func_080049e8();
                switch (phase) {
                case 0:
                    Func_08004c1c(positive_step);
                    break;
                case 1:
                    Func_08004bd4(negative_step);
                    break;
                case 2:
                    Func_08004c6c(negative_step);
                    break;
                case 3:
                    Func_08004bd4(negative_step);
                    Func_08004c6c(negative_step);
                    break;
                }

                Func_080e3944(particle, &position);
                position.x >>= 1;
                Func_08004a5c();
                if (position.z < 250)
                    position.z = 250;
                if (position.z > 0x27a)
                    position.z = 0x27a;

                size = 9 - ((position.z - 250 +
                    ((position.z - 250) < 0 ? 63 : 0)) >> 6);
                doubled_size = size << 1;
                ((ParticleRenderer)state[0])(
                    render_context,
                    (const u8 *)runtime + phase * 0x604 +
                        Data_080ede48[doubled_size - 2],
                    position.x - ((size + ((u32)size >> 31)) >> 1),
                    position.y - size,
                    size,
                    doubled_size);
                Func_080e38b8(particle, 60, 0);

                if (frame > quarter + 30) {
                    particle->dx += -particle->x >> 8;
                    particle->dy += -particle->y >> 8;
                    particle->dz += -particle->z >> 8;
                }
            }

            positive_step += frame << 3;
            negative_step += -frame << 3;
        }

        runtime->complete = 1;
        Func_080030f8(1);
    }

    Func_08004278(0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}

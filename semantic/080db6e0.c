#include "types.h"

/*
 * __call_via_rN veneer sites, resolved per-site against the ROM.
 *
 * Three `bl` sites in this function land inside the 0x080072e4 veneer bank
 * (fifteen four-byte `bx rN; nop` entries, r0..lr). None of them is a call to
 * a function at the branch target; each is an indirect call through the named
 * register, and the instruction that last wrote that register is the callee
 * load.
 *
 * 0x080db754 -- `bl 0x080072f0` = __call_via_r3. r3 is loaded at 0x080db74e
 * from this function's own pool word 0x080dba44, which holds 0x03001388: the
 * relocated word-copy helper, established from the EXACT src/080d40ec.c as
 * (destination, source, size). r0 = 0x05000000 (built as 0xa0 << 19 at
 * 0x080db74c/0x080db750), r1 = the return of Func_08002f40(0x96), r2 = 0x80.
 * r3 is inside the r0-r3 argument registers, so this is the case where the
 * draft's fourth argument WAS the callee address; the call takes three.
 *
 * 0x080db930 and 0x080db9bc -- `bl 0x080072f4` = __call_via_r4. Both load r4
 * from [sp, #28] in the instruction immediately before the branch
 * (0x080db92e, 0x080db9ba). That slot is filled once, at 0x080db828, from
 * `ldr r3, [r3, #0]` at 0x080db826 where r3 = pool word 0x080dba50
 * (0x03001e50) plus 184. 0x03001e50 is Func_080048b0's slot table and
 * 184 = 46 * 4, so the word is allocator slot 46 at 0x03001f08 -- exactly the
 * pointer that `Func_080ed408(46, ...)` at 0x080db81e publishes, per the
 * exact src/080cef64.c. The load sits four instructions after that publish.
 *
 * ARITY of the renderer: six. r0..r3 are set at 0x080db92c / 0x080db910 /
 * 0x080db922 / 0x080db928 and two more words go out at [sp, #0] and [sp, #4]
 * (0x080db926, 0x080db92a). r4 is ABOVE the argument registers, so the
 * register-index regularity does not apply and no argument slot is holding
 * the callee -- every one of the six arguments the draft passed is real. The
 * shape matches the six-argument renderer ABI already recorded for slot 46 in
 * the sibling semantic/main/080da2ac.c.
 *
 * UNCERTAINTY, deliberately left standing: what slot 46 CONTAINS is not
 * settled here. The slot table unifies the ADDRESSING, never the contents --
 * other slots are known to take different ROM payloads from different
 * callers. This file proves which pointer is called, not which routine it is.
 */
typedef void *(*Transfer_080db6e0)(void *destination, const void *source,
                                   s32 size);
typedef void (*Renderer_080db6e0)(
    s32 target, const void *source, s32 x, s32 y, s32 width, s32 height);

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

struct Particle_080db6e0 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    u32 age;
};

void Func_080cd594(s32);
void Func_080e0524(s32, void *, s32, s32);
void *Func_08002f40(s32);
u32 Func_08004458(void);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080041d8(const void *, s32);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_080f9010(s32);
void Func_080b50e8(s32);
void Func_080e3944(void *, s32 *);
void Func_080e38b8(void *, s32, s32);
s32 Func_080022ec(s32, s32);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080b5088(s16, s32);
void Func_080e155c(s32, s32);
void Func_080cd52c(void);
void Func_080030f8(s32);
void Func_08004278(const void *);
void Func_08002dd8(s32);
s32 Func_080cdbc0(void);

s32 Func_080db6e0(void *argument, s32 monochrome)
{
    u8 *runtime = *(u8 **)0x03001eec;
    s32 graphics = *(s32 *)0x03001ef0;
    struct Particle_080db6e0 *emitters =
        (struct Particle_080db6e0 *)(runtime + 0x7080);
    struct Particle_080db6e0 *particles =
        (struct Particle_080db6e0 *)0x02010000;
    s32 variant;
    s32 table_index;
    s32 frame;
    u32 i;
    Renderer_080db6e0 renderer;

    PTR_AT(runtime, 0x7828) = argument;
    Func_080cd594(1);
    Func_080e0524(0xc0, runtime, 1, 0);

    if (monochrome == 1) {
        u16 *palette = (u16 *)0x05000000;
        for (i = 0; i < 0x40; i++) {
            s32 intensity = i / 2;
            palette[i] =
                (intensity << 10) | (intensity << 5) | intensity;
        }
        variant = 1;
    } else {
        ((Transfer_080db6e0)0x03001388)(
            (void *)0x05000000, Func_08002f40(0x96), 0x80);
        variant = S32_AT(argument, 0x18);
    }

    for (i = 0; i < 0x20; i++) {
        struct Particle_080db6e0 *emitter = &emitters[i];
        emitter->x =
            S32_AT(argument, 4) == 1 ? 0x320000 : (s32)0xffce0000;
        emitter->y = 0;
        emitter->z = 0;
        emitter->velocity_x =
            ((Func_08004458() & 0x3f) - 0x20) << 13;
        emitter->velocity_y =
            ((Func_08004458() & 0x3f) + 0x10) << 12;
        emitter->velocity_z =
            ((Func_08004458() & 0x3f) - 0x20) << 13;
        emitter->age = 0;
    }

    for (i = 0; i < 0x400; i++) {
        struct Particle_080db6e0 *particle = &particles[i];
        particle->x =
            S32_AT(argument, 4) == 1 ? 0x320000 : (s32)0xffce0000;
        particle->y = 0;
        particle->z = 0;
        particle->velocity_x =
            ((Func_08004458() & 0x3f) - 0x20) << 13;
        particle->velocity_y =
            ((Func_08004458() & 0x1f) + 8) << 13;
        particle->velocity_z =
            ((Func_08004458() & 0x3f) - 0x20) << 13;
        particle->age = 0;
    }

    Func_080ed408(0x2e, 7, 7, 3, 2);
    /* Allocator slot 46, read once at 0x080db828 and held in [sp, #28]. */
    renderer = *(Renderer_080db6e0 *)0x03001f08;
    S32_AT(runtime, 0x7780) = 2;
    S32_AT(runtime, 0x7784) = 0x4b;
    Func_080041d8((const void *)0x080cd261, 0x480);

    table_index = variant * 3;
    for (frame = 0;
         frame < U8_AT((void *)0x080eeae2, table_index + 2);
         frame++) {
        u8 *camera = *(u8 **)0x03001e80;

        Func_080049ac();
        Func_080051d8(camera, camera + 0x0c);
        if (frame == 2)
            Func_080f9010(0x90);
        if (frame ==
            U8_AT((void *)0x080eeae2, table_index + 2) - 0x30)
            Func_080b50e8(0x85);

        for (i = 0;
             i < U8_AT((void *)0x080eeae2, table_index);
             i++) {
            struct Particle_080db6e0 *particle = &particles[i];

            if (particle->y >= 0) {
                s32 projected[3];
                s32 depth;
                s32 size;
                s32 height;
                const void *source;

                Func_080e3944(particle, projected);
                projected[0] =
                    projected[0] / 2 +
                    (S32_AT(argument, 4) << 5) - 0x10;
                if (projected[2] < 0xa0)
                    projected[2] = 0xa0;
                if (projected[2] > 0x31f)
                    projected[2] = 0x31f;
                depth = projected[2] - 0xa0;
                size = 9 - depth / 0x40;
                height = size * 2;
                source =
                    runtime + 0x3200 + (i & 1) * 0x302 +
                    U16_AT((void *)0x080ede48, height - 2);
                renderer(
                    graphics, source,
                    projected[0] -
                        ((size + (size >> 31)) >> 1),
                    projected[1] - size,
                    size, height);
                Func_080e38b8(particle, 0x40, -0x2000);
            }
        }

        if (frame > 2) {
            for (i = 0;
                 i < U8_AT((void *)0x080eeae2, table_index + 1);
                 i++) {
                struct Particle_080db6e0 *emitter = &emitters[i];

                if (i < frame && emitter->y >= 0) {
                    s32 projected[3];
                    Func_080e3944(emitter, projected);
                    projected[0] =
                        projected[0] / 2 +
                        (S32_AT(argument, 4) << 5) - 0x10;

                    if (emitter->age <= 0x14) {
                        s32 animation =
                            Func_080022ec(emitter->age, 3) * 2;
                        u16 size =
                            U16_AT((void *)0x080eeafa, animation);
                        const void *source =
                            runtime +
                            U16_AT((void *)0x080eeaec, animation);
                        renderer(
                            graphics, source,
                            projected[0] - (size >> 1),
                            projected[1] - (size >> 1),
                            size, size);
                    }
                    if (emitter->age <= 0x14)
                        emitter->age++;
                    Func_080e38b8(emitter, 0x40, -0x2000);
                }
            }
        }

        if (monochrome == 0) {
            for (i = 0; i < S32_AT(argument, 0x14); i++) {
                if (frame == i + 6) {
                    s16 character =
                        S16_AT(argument, 0x24 + i * 2);
                    Func_080d6888(character, 7, 5, i, 0x0a);
                    Func_080b5088(character, 2);
                }
            }
        } else {
            for (i = 0; i < S32_AT(argument, 0x14); i++) {
                if (frame == i + 6) {
                    Func_080d6888(
                        S16_AT(argument, 0x24 + i * 2),
                        7, 5, i, 0x0a);
                }
            }
        }

        if (frame == 2)
            S32_AT(runtime, 0x77a8) = 6;
        Func_080e155c(0x10, 0x10);
        Func_080cd52c();
        S32_AT(runtime, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(0x2e);
    return Func_080cdbc0();
}

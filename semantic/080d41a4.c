#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

/*
 * __call_via_rN veneer sites, resolved per-site against the ROM. Five `bl`
 * sites land inside the 0x080072e4 bank; each is an indirect call through the
 * named register.
 *
 * 0x080d4248 -- __call_via_r3, r3 from pool 0x080d44ec = 0x03001388, the
 * relocated word copy (destination, source, size), from the EXACT
 * src/080d40ec.c. The draft's fourth argument WAS the callee.
 * 0x080d431e -- __call_via_r3, r3 from pool 0x080d4508 = 0x03000168, the fill
 * (destination, size, value). r0 = [sp, #36] (graphics), r1 = 0x4000,
 * r2 = pool 0x080d450c = 0x10101010, all set deliberately. Same correction.
 *
 * THE TWO-ENTRY RENDERER TABLE, reached here through the runtime header
 * rather than the slot table directly. r5 = 0x03001eec; [r5, #28] is read at
 * 0x080d41e4 into [sp, #44] and [r5, #32] at 0x080d41f8 into [sp, #48], each
 * right after its own `Func_080ed408(46/47, ...)` publish. 0x03001eec + 28 =
 * 0x03001f08 and + 32 = 0x03001f0c -- allocator slots 46 and 47, which is why
 * they read as struct fields here and as globals elsewhere (the same
 * observation 080d0ee0's header records). The base address sp + 44 is parked
 * in [sp, #12] at 0x080d41fe.
 *
 * 0x080d4366 and 0x080d43a4 -- r4 = `ldr r4, [sp, #44]`: entry 0, slot 46.
 * 0x080d4576 -- r4 = `ldr r4, [r4, r0]` at 0x080d4572 with r0 = [sp, #12]
 * (the base) and r4 = (r8 & 1) << 2, r8 being the particle index of the final
 * loop. So renderers[i & 1].
 *
 * ARITY: six at every renderer site. r0..r3 are set and two more words go out
 * at [sp, #0] and [sp, #4]. r4 is above the argument registers, so no
 * argument slot holds the callee.
 *
 * UNCERTAINTY, left standing: what slots 46 and 47 CONTAIN is not settled
 * here. The slot table unifies the addressing, never the contents.
 */
typedef void *(*WordCopy_080d41a4)(void *destination, const void *source,
                                   s32 size);
typedef void (*ArmFill_080d41a4)(void *destination, u32 size, u32 value);
typedef void (*Renderer_080d41a4)(
    s32 target, const void *source, s32 x, s32 y, s32 width, s32 height);

struct Particle_080d41a4 {
    s32 x;
    s32 y;
    s32 unknown_08;
    s32 velocity_x;
    s32 velocity_y;
    s32 unknown_14;
    s32 timer;
};

void Func_080cd594(s32);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void *Func_08002f40(s32);
void Func_080041d8(const void *, s32);
void Func_080b50e8(s32);
void Func_080f9010(s32);
u32 Func_08004458(void);
s32 Func_08002322(s32);
s32 Func_0800231c(s32);
void Func_080b5088(s16, s32);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e3908(void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080cd52c(void);
void Func_080030f8(s32);
void Func_08004278(const void *);
void Func_08002dd8(s32);
void Func_080cdbc0(void);

void Func_080d41a4(void *argument)
{
    u8 *runtime = *(u8 **)0x03001eec;
    s32 graphics = *(s32 *)0x03001ef0;
    u8 *effect_tiles = *(u8 **)0x03001ef4;
    struct Particle_080d41a4 *particles =
        (struct Particle_080d41a4 *)0x02010000;
    s32 variant;
    s32 effect_count;
    s32 duration;
    s32 frame;
    s32 i;
    Renderer_080d41a4 renderers[2];

    PTR_AT(runtime, 0x7828) = argument;
    Func_080cd594(1);
    *(volatile u16 *)0x04000052 = 0x1010;
    Func_080ed408(0x2e, 7, 7, 3, 3);
    renderers[0] = *(Renderer_080d41a4 *)0x03001f08;
    Func_080ed408(0x2f, 7, 7, 7, 2);
    renderers[1] = *(Renderer_080d41a4 *)0x03001f0c;
    Func_080e0524(0xd1, runtime, 1, 1);
    Func_080e0524(0x73, effect_tiles, 0, 0);

    variant = S32_AT(argument, 0x18);
    if (variant != 2) {
        ((WordCopy_080d41a4)0x03001388)(
            (void *)0x05000000, Func_08002f40(0x60), 0x80);
    }

    for (i = 0; i < 0x400; i++)
        particles[i].timer = 0;

    S32_AT(runtime, 0x7780) = 2;
    S32_AT(runtime, 0x7784) = 0x32;
    Func_080041d8((const void *)0x080cd261, 0x480);

    effect_count = U8_AT((void *)0x080ee25e, variant);
    duration = effect_count * 7 + 0x30;
    for (frame = 0; frame < duration; frame++) {
        if (variant == 2 && frame <= 0x3f) {
            u8 *object = *(u8 **)0x03001e80;
            s32 shift = frame <= 0x37 ? 0x100 : 0x2c0 - frame * 8;
            if (S32_AT(argument, 4) == 1)
                U16_AT(object, 0x36) -= shift;
            else
                U16_AT(object, 0x36) += shift;
        }

        if (frame == 0x20)
            Func_080b50e8(0x86);

        for (i = 0; i < effect_count; i++) {
            s32 start = i * 8;
            s32 trigger = start + 4;

            if (frame == start) {
                Func_080f9010(0x86);
                ((ArmFill_080d41a4)0x03000168)(
                    (void *)graphics, 0x4000, 0x10101010);
            }

            if (frame >= start && frame < start + 9) {
                s32 x =
                    U8_AT((void *)0x080ee250,
                          i + S32_AT(argument, 4) * 7) -
                    0x18;

                if (frame == start + 1) {
                    renderers[0](
                        graphics, runtime, x, 0, 0x30, 0x70);
                }
                if (frame >= start + 2 && frame < start + 4) {
                    renderers[0](
                        graphics, runtime + 0x1500,
                        x, 0, 0x30, 0x70);
                }

                if (frame == start + 2) {
                    s32 required =
                        U16_AT((void *)0x080ee244, variant * 4);
                    s32 created = 0;
                    s32 slot;

                    for (slot = 0; slot < 0x400; slot++) {
                        struct Particle_080d41a4 *particle =
                            &particles[slot];
                        s32 radius;
                        s32 angle;

                        if (particle->timer != 0)
                            continue;
                        particle->x =
                            U8_AT((void *)0x080ee250,
                                  i + S32_AT(argument, 4) * 7)
                            << 16;
                        particle->y = 0x680000;
                        radius = (Func_08004458() & 0x3ff) + 0x20;
                        angle = (Func_08004458() & 0x7fff) - 0x4000;
                        particle->velocity_x =
                            (s32)(radius * Func_08002322(angle)) >> 7;
                        particle->velocity_y =
                            -(s32)(radius * Func_0800231c(angle) * 2) >>
                            7;
                        particle->timer =
                            (Func_08004458() & 7) + 0x20;
                        created++;
                        if (created == required)
                            break;
                    }
                    S32_AT(runtime, 0x77a8) =
                        U16_AT((void *)0x080ee244, variant * 4 + 2);
                }
            }

            if (frame == trigger) {
                s32 member;
                for (member = 0;
                     member < S32_AT(argument, 0x14);
                     member++) {
                    s16 character =
                        S16_AT(argument, 0x24 + member * 2);
                    Func_080b5088(character, 1);
                    Func_080d6888(character, 7, 5, member, 8);
                }
            }
        }

        for (i = 0; i < 0x400; i++) {
            struct Particle_080d41a4 *particle = &particles[i];

            if (particle->timer <= 0)
                continue;
            particle->timer--;
            Func_080e3908(particle, 0x3c, 0x1000);

            if (particle->y > 0x680000) {
                s32 upward = -particle->velocity_y;
                particle->velocity_y =
                    (upward + (upward >> 31)) >> 1;
            } else if (particle->x >= 0 &&
                       (particle->x >> 16) <= 0x77 &&
                       particle->y >= 0) {
                s32 size = particle->timer / 8 + 1;
                s32 height = size * 2;
                const void *source =
                    effect_tiles +
                    U16_AT((void *)0x080ede48, height - 2);

                renderers[i & 1](
                    graphics, source,
                    (particle->x >> 16) -
                        ((size + (size >> 31)) >> 1),
                    (particle->y >> 16) - size,
                    size, height);
            }
        }

        Func_080e155c(8, 0x10);
        Func_080cd52c();
        S32_AT(runtime, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    Func_080cdbc0();
}

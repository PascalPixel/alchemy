#include "types.h"

/*
 * __call_via_rN veneer sites, resolved per-site against the ROM. Four `bl`
 * sites land inside the 0x080072e4 bank; each is an indirect call through the
 * named register, not a call to a function at the branch target.
 *
 * 0x080db3f0 -- `bl 0x080072f0` = __call_via_r3, and r3 comes from this
 * function's pool word 0x080db6a0 = 0x03000168, the relocated IWRAM fill
 * (destination, size, value), established in semantic/main/080e15e8.c. r3 is
 * an argument register, so the draft's fourth argument WAS the callee; the
 * call takes three.
 *
 * A TWO-ENTRY RENDERER TABLE LIVES IN THIS FRAME. At 0x080db2c6..0x080db2ce
 * r5 = pool 0x080db2f4 = 0x03001e50 (Func_080048b0's slot table); +184 is
 * slot 46 and its contents go to [sp, #36]. At 0x080db2e0..0x080db300, +188
 * is slot 47 and its contents go to [sp, #40] via `str r3, [r2, #4]`, where
 * r2 = sp + 36. The address sp + 36 is itself parked in [sp, #12]. So the two
 * pointers published by the `Func_080ed408(46, ...)` and
 * `Func_080ed408(47, ...)` calls immediately above them (src/080cef64.c) are
 * a two-element local array, and [sp, #12] is its base.
 *
 * 0x080db474 and 0x080db5c6 -- `bl 0x080072f4` = __call_via_r4, r4 loaded
 * from [sp, #36] in the instruction before each branch: entry 0, slot 46.
 *
 * 0x080db4b8 -- `bl 0x080072f4` = __call_via_r4, and r4 is
 * `ldr r4, [r1, r3]` at 0x080db4ae: r3 = [sp, #12] (the table base) and
 * r1 = ([sp, #20] & 1) << 2. [sp, #20] is the wave counter, zeroed at
 * 0x080db3fa and used at 0x080db40a as the multiplier of the per-frame angle.
 * So this site alternates between renderer 46 and renderer 47 on wave parity.
 *
 * STRUCTURE CORRECTED. The draft carried `if (variant == 0) { A } else { A }`
 * with two byte-identical bodies and no stated reason for the split. The
 * branch is real -- `cmp r3, #0; bne 0x080db47a` at 0x080db43c on
 * argument+0x18 -- and the difference is the callee, which the draft could
 * not see: the taken arm calls entry 0 unconditionally, the other indexes the
 * table by wave parity. A wrong callee had flattened a genuine distinction
 * into a duplicated body.
 *
 * ARITY of the renderer: six, at all three sites. r0..r3 are set and two more
 * words go out at [sp, #0] and [sp, #4]. r4 is above the argument registers,
 * so no argument slot holds the callee and every argument the draft passed is
 * real.
 *
 * UNCERTAINTY, left standing: what slots 46 and 47 CONTAIN is not settled
 * here. The slot table unifies the addressing, never the contents.
 */
typedef void (*ArmFill_080db264)(void *destination, u32 size, u32 value);
typedef void (*Renderer_080db264)(
    s32 target, const void *source, s32 x, s32 y, s32 width, s32 height);

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

struct Particle_080db264 {
    s32 x;
    s32 y;
    s32 unknown_08;
    s32 velocity_x;
    s32 velocity_y;
    s32 unknown_14;
    s32 timer;
};

void Func_080cdb24(s32);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
u32 Func_08004458(void);
s32 Func_08002322(s32);
s32 Func_0800231c(s32);
void Func_080041d8(const void *, s32);
void Func_080f9010(s32);
void Func_080b50e8(s32);
s32 Func_080022fc(s32, s32);
void Func_080e3908(void *, s32, s32);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e155c(s32, s32);
void Func_080cd52c(void);
void Func_080030f8(s32);
void Func_08004278(const void *);
void Func_08002dd8(s32);
void Func_080cdbc0(void);

void Func_080db264(void *argument)
{
    u8 *runtime = *(u8 **)0x03001eec;
    s32 graphics = *(s32 *)0x03001ef0;
    u8 *effect_tiles = *(u8 **)0x03001ef4;
    struct Particle_080db264 *particles =
        (struct Particle_080db264 *)0x02010000;
    s32 variant;
    s32 frame;
    s32 i;
    Renderer_080db264 renderers[2];

    PTR_AT(runtime, 0x7828) = argument;
    Func_080cdb24(1);
    variant = S32_AT(argument, 0x18);
    *(volatile u16 *)0x04000020 = variant == 2 ? 0x80 : 0x100;
    Func_080ed408(0x2e, 7, 7, 3, 2);
    renderers[0] = *(Renderer_080db264 *)0x03001f08;
    Func_080ed408(0x2f, 7, 7, 7, 3);
    renderers[1] = *(Renderer_080db264 *)0x03001f0c;
    Func_080e0524(0xc4, runtime + 0x60e, 1, 1);
    Func_080e0524(0x73, effect_tiles, 0, 0);

    if (variant == 2) {
        *(volatile s32 *)0x04000028 =
            S32_AT(argument, 4) == 1 ? -0x1000 : 0x1000;
    } else if (S32_AT(argument, 4) == 1) {
        *(volatile s32 *)0x04000028 = -0x8000;
    }

    for (i = 0; i < 0x400; i++) {
        struct Particle_080db264 *particle = &particles[i];
        s32 radius = (Func_08004458() & 0x3ff) + 0x100;
        s32 angle = (Func_08004458() & 0x7fff) - 0x4000;

        particle->x = 0x4000;
        particle->y = 0x7000;
        particle->velocity_x =
            (s32)(radius * Func_08002322(angle)) >> 16;
        particle->unknown_14 =
            -(s32)(radius * Func_0800231c(angle) * 2) >> 16;
        particle->timer = 0;
    }

    S32_AT(runtime, 0x7780) = 2;
    S32_AT(runtime, 0x7784) = 0x4b;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080f9010(0x8a);

    for (frame = 0; frame < 0x40; frame++) {
        if (frame == 0x14)
            Func_080b50e8(0x85);

        if (frame <= 0x0f) {
            s32 wave;

            if (Func_080022fc(frame, 5) == 2)
                ((ArmFill_080db264)0x03000168)(
                    (void *)graphics, 0x4000, 0x10101010);

            for (wave = 0; wave < 4; wave++) {
                s32 angle = wave * ((frame << 11) + 0x4000);
                s32 center_x =
                    ((s32)((0x20 - frame) * Func_08002322(angle)) >> 16) +
                    0x40;
                s32 center_y =
                    -((s32)(Func_0800231c(angle) * 8) >> 16) - 8;
                s32 created = 0;
                s32 slot;

                if (variant == 0) {
                    const void *source =
                        runtime + 0x60e +
                        (Func_08004458() & 3) * 0xb40;
                    renderers[0](
                        graphics, source,
                        center_x + (Func_08004458() & 7) - 0x10,
                        center_y, 0x18, 0x78);
                } else {
                    const void *source =
                        runtime + 0x60e +
                        (Func_08004458() & 3) * 0xb40;
                    renderers[wave & 1](
                        graphics, source,
                        center_x + (Func_08004458() & 7) - 0x10,
                        center_y, 0x18, 0x78);
                }

                for (slot = 0; slot < 0x400; slot++) {
                    struct Particle_080db264 *particle = &particles[slot];
                    s32 radius;
                    s32 particle_angle;

                    if (particle->timer != 0)
                        continue;
                    particle->y = (center_y + 0x70) << 16;
                    particle->x = center_x << 16;
                    radius = (Func_08004458() & 0x1ff) + 0x80;
                    particle_angle =
                        (Func_08004458() & 0x7fff) - 0x4000;
                    particle->velocity_x =
                        (s32)(radius * Func_08002322(particle_angle)) >> 9;
                    particle->velocity_y =
                        -(s32)(radius * Func_0800231c(particle_angle) * 2) >>
                        7;
                    particle->timer = (Func_08004458() & 7) + 0x20;
                    created++;
                    if (created ==
                        U8_AT((void *)0x080eeadc, variant * 2 + 1))
                        break;
                }
            }
            S32_AT(runtime, 0x77a8) = 1;
        }

        for (i = 0; i < 0x400; i++) {
            struct Particle_080db264 *particle = &particles[i];

            if (particle->timer <= 0)
                continue;
            particle->timer--;
            Func_080e3908(particle, 0x3c, -0x800);

            if (particle->y > 0x780000) {
                s32 upward = -particle->velocity_y;
                particle->velocity_y =
                    (upward + (upward >> 31)) >> 1;
            } else if ((u32)particle->x <= 0x7effff &&
                       particle->y >= 0) {
                s32 size = particle->timer / 8 + 1;
                s32 height = size * 2;
                const void *source =
                    effect_tiles +
                    U16_AT((void *)0x080ede48, height - 2);

                renderers[0](
                    graphics, source,
                    (particle->x >> 16) -
                        ((size + (size >> 31)) >> 1),
                    (particle->y >> 16) - size,
                    size, height);
            }
        }

        if ((u32)(frame - 4) <= 0x5b) {
            for (i = 0; i < S32_AT(argument, 0x14); i++) {
                if (frame == i * 4 + 4) {
                    Func_080d6888(
                        S16_AT(argument, 0x24 + i * 2),
                        7, 5, i, 0x0a);
                }
            }
        }

        Func_080e155c(2, 4);
        Func_080cd52c();
        S32_AT(runtime, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    Func_080cdbc0();
}

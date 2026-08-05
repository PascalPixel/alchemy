#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

struct Spark_080d3854 {
    s32 x;
    s32 y;
    s32 unknown_08;
    s32 unknown_0c;
    s32 unknown_10;
    s32 unknown_14;
    s32 age;
};

/*
 * __call_via_rN veneer sites, resolved per-site against the ROM. Six `bl`
 * sites land in the 0x080072e4 bank: two __call_via_r3, one __call_via_r6 and
 * three __call_via_r4 -- plus one more r4 site inside the spark loop, six in
 * total.
 *
 * 0x080d3894 -- __call_via_r3, pool 0x080d38d0 = 0x03001388, the IWRAM word
 * copy. r3 is an ARGUMENT register, so the draft's fourth argument WAS the
 * callee; the call takes three.
 *
 * 0x080d39a4 -- __call_via_r6, and the resolver REFUSES it: r6 is written at
 * 0x080d3992 and 0x080d3998 is a branch target, so a backward walk cannot
 * prove the write is on every path. The one-register test settles it without
 * changing the tool. r6 is written at 0x080d3862, 0x080d3992, 0x080d3a82,
 * 0x080d3ac2, 0x080d3ae4, 0x080d3b5a and 0x080d3ba8, but only 0x080d3992 lies
 * between the function's entry and this site, and the ONLY branch targeting
 * 0x080d3998 is `bne.n 0x080d3998` at 0x080d39b2 -- the back edge of the
 * seven-iteration loop that begins at 0x080d3998, downstream of the write and
 * unreachable without passing through it. Nothing inside that loop touches
 * r6. So the write dominates the site and the value is the same on every
 * iteration: pool 0x080d39cc = 0x03000168, the IWRAM ARM fill. r6 is above
 * r0-r3, so all three arguments the draft passes are real and there is no
 * callee hiding among them -- the draft already had the arity right.
 *
 * THE OTHER FOUR ARE A FRAME-LOCAL TWO-ENTRY RENDERER TABLE, and this file is
 * the plain case: both slots are published ONCE and released once, at the end.
 * 0x080d38ae publishes id 46 and 0x080d38b2 reads `[0x03001eec + 28]` =
 * 0x03001f08 into [sp, #44]; 0x080d38da publishes id 47 and 0x080d38de reads
 * `[0x03001eec + 32]` = 0x03001f0c into [sp, #48] via `str r3, [r0, #4]` with
 * r0 = sp + 44. The address sp + 44 is parked in [sp, #24] at 0x080d38e4.
 * There is no other Func_080ed408 in the function and the two releases are at
 * 0x080d3c2e and 0x080d3c34, so nothing republishes between the reads and the
 * uses and the cache is the ROM's own.
 *
 * 0x080d3a98, 0x080d3aba, 0x080d3ad6 -- entry 0, slot 46, read from [sp, #44]
 * immediately before each branch.
 *
 * 0x080d3b2c -- `ldr r4, [r5, r0]` at 0x080d3b26 with r0 = [sp, #24] (the
 * table base) and r5 = (r8 & 1) << 2, where r8 is the four-iteration spark
 * counter incremented at 0x080d3b56 and compared at 0x080d3b5c. So this site
 * alternates between renderer 46 and renderer 47 on spark parity, and the
 * draft called one renderer for both.
 *
 * PINNING: the three entry-0 sites differ only in their source offsets --
 * 0x0c56, 0x2a56 and 0x1156, the pool words at 0x080d3c58, 0x080d3c5c and
 * 0x080d3c60 -- and all three take the SAME callee, so no assignment between
 * them can change the answer. Nothing was invented to separate them.
 *
 * ARITY: six at the renderer sites, four in registers and two pushed; r4 is
 * above the argument registers.
 *
 * UNCERTAINTY, left standing: what slots 46 and 47 CONTAIN is not settled
 * here. The slot table unifies the addressing, never the contents.
 */
typedef void *(*WordCopy_080d3854)(void *destination, const void *source,
                                   s32 size);
typedef void (*ArmFill_080d3854)(void *destination, u32 size, u32 value);
typedef void (*Renderer_080d3854)(
    s32 target, const void *source, s32 x, s32 y, s32 width, s32 height);

void Func_080cd594(s32);
void *Func_08002f40(s32);
void Func_08005340(const void *, void *);
void Func_080ed408(s32, s32, s32, s32, s32);
u32 Func_08004458(void);
void Func_080041d8(const void *, s32);
void Func_080030f8(s32);
void Func_080f9010(s32);
s32 Func_08002322(s32);
void Func_080b50e8(s32);
s32 Func_0800231c(s32);
s32 Func_080022fc(s32, s32);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080b5088(s16, s32);
void Func_080e155c(s32, s32);
void Func_080cd52c(void);
void Func_08002dd8(s32);
void Func_08004278(const void *);
void Func_080cdbc0(void);

void Func_080d3854(void *argument)
{
    u8 *runtime = *(u8 **)0x03001eec;
    s32 graphics = *(s32 *)0x03001ef0;
    struct Spark_080d3854 *sparks =
        (struct Spark_080d3854 *)(runtime + 0x7080);
    void *palette;
    Renderer_080d3854 renderers[2];
    s32 variant;
    s32 frame;
    s32 i;

    PTR_AT(runtime, 0x7828) = argument;
    Func_080cd594(1);
    *(volatile u16 *)0x04000052 = 0x1010;
    palette = Func_08002f40(0xce);
    ((WordCopy_080d3854)0x03001388)(
        (void *)0x05000000, palette, 0x80);
    Func_08005340((u8 *)palette + 0x80, runtime);
    Func_080ed408(0x2e, 7, 7, 3, 2);
    renderers[0] = *(Renderer_080d3854 *)0x03001f08;
    Func_080ed408(0x2f, 7, 7, 7, 2);
    renderers[1] = *(Renderer_080d3854 *)0x03001f0c;

    for (i = 0; i < 16; i++) {
        sparks[i].x = Func_08004458() & 0x1f;
        sparks[i].y = (Func_08004458() & 0x3f) + 0x10;
        sparks[i].age = -(s32)(Func_08004458() & 0x0f);
    }

    S32_AT(runtime, 0x7780) = 2;
    S32_AT(runtime, 0x7784) = 0x32;
    Func_080041d8((const void *)0x080cd261, 0x480);
    *(volatile u16 *)0x04000052 = 0x1000;
    Func_080030f8(1);
    Func_080f9010(0x8d);

    variant = S32_AT(argument, 0x18);
    for (frame = 0; frame < 0x50; frame++) {
        s32 horizontal = Func_08002322(frame << 10) * 0x10;
        s32 group_count;
        s32 group;

        if (frame == 0x20)
            Func_080b50e8(0x85);
        for (i = 0; i < 7; i++) {
            if (frame == i * 8 + 0x10)
                ((ArmFill_080d3854)0x03000168)(
                    (void *)graphics, 0x4000, 0x08080808);
        }

        horizontal +=
            S32_AT(argument, 4) == 1 ? 0x200000 : (s32)0xffe00000;
        if (frame <= 0x10)
            *(volatile u16 *)0x04000052 = frame | 0x1000;
        if (frame > 0x3f)
            *(volatile u16 *)0x04000052 =
                (0x4f - frame) | 0x1000;

        group_count = U8_AT((void *)0x080ee1ca, variant * 3);
        for (group = 0; group < group_count; group++) {
            s32 angle = frame << 11;
            s32 center_x =
                ((s32)(U8_AT((void *)0x080ee1ca, variant * 3 + 1) *
                       Func_08002322(angle)) +
                 horizontal) >>
                    16;
            s32 center_y =
                ((s32)(Func_0800231c(angle) * 2) >> 16);
            s32 animation = Func_080022fc(frame / 2, 3);
            struct Spark_080d3854 *spark = &sparks[group * 4];

            center_x += 0x28;
            center_y += 0x10;
            renderers[0](
                graphics, runtime + animation * 0xa00 + 0xc56,
                center_x, center_y, 0x28, 0x20);
            renderers[0](
                graphics, runtime + animation * 0x500 + 0x2a56,
                center_x, center_y + 0x20, 0x28, 0x20);
            renderers[0](
                graphics, runtime + animation * 0xa00 + 0x1156,
                center_x, center_y + 0x40, 0x28, 0x20);

            for (i = 0; i < 4; i++, spark++) {
                if (spark->age >= 0) {
                    s32 image =
                        (spark->age + ((u32)spark->age >> 31)) / 2 +
                        (i / 2) * 3;
                    renderers[i & 1](
                        graphics,
                        runtime +
                            U16_AT((void *)0x080edebe, image * 2),
                        spark->x + center_x,
                        spark->y + center_y - 0x10,
                        U8_AT((void *)0x080edeca, image),
                        U8_AT((void *)0x080eded0, image));
                }
                spark->age++;
                if (spark->age == 6) {
                    spark->x = Func_08004458() & 0x1f;
                    spark->y = (Func_08004458() & 0x3f) + 0x10;
                    spark->age = 0;
                }
            }
        }

        for (i = 0; i < S32_AT(argument, 0x14); i++) {
            s32 pulse;
            for (pulse = 0; pulse < 7; pulse++) {
                if (frame == i * 3 + pulse * 8 + 0x10) {
                    s16 character =
                        S16_AT(argument, 0x24 + i * 2);
                    Func_080d6888(character, 7, 5, i, 4);
                    Func_080b5088(character, 6);
                }
            }
        }

        S32_AT(runtime, 0x77a8) = 1;
        {
            u8 layers =
                U8_AT((void *)0x080ee1ca, variant * 3 + 2);
            Func_080e155c(layers, layers * 2);
        }
        Func_080cd52c();
        S32_AT(runtime, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    Func_08004278((const void *)0x080cd261);
    Func_080cdbc0();
}

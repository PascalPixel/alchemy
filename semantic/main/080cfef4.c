#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

/*
 * __call_via_rN veneer sites, resolved per-site against the ROM.
 *
 * Ten `bl` sites land inside the 0x080072e4 bank. Two dispatch through r3 and
 * are fixed pooled constants: 0x080cff40 -> 0x03001388, the relocated word
 * copy (destination, source, size) from the EXACT src/080d40ec.c; and
 * 0x080d025e -> 0x03000168, the fill (destination, size, value). r3 is an
 * argument register at both, so the draft's fourth argument WAS the callee and
 * each call takes three.
 *
 * The other eight dispatch through r4. r6 = pool 0x080cff68 = 0x03001eec;
 * `[r6, #28]` is read at 0x080cff98 into [sp, #44] and `[r6, #32]` at
 * 0x080cffaa into [sp, #48] via `str r3, [r4, #4]` with r4 = sp + 44, and the
 * base sp + 44 is parked in [sp, #16] at 0x080cffb4. 0x03001eec + 28 =
 * 0x03001f08 and + 32 = 0x03001f0c: allocator slots 46 and 47, which the draft
 * already read correctly into its `local_state[2]` without ever connecting
 * them to the branches.
 *
 * Every one of the eight is `ldr r4, [r0, rBase]` with rBase = [sp, #16] and
 * r0 shifted left by two. Each is pinned to its C statement by argument
 * agreement -- the width/height constants 0x39/0x62, 0x63/0x45, 0x80/0x5b,
 * 0x80/0x3b, 0x7a/0x1d, 0x4c/0x19 and the pooled sources 0x00003081,
 * 0x02010000, 0x02012d80, 0x02014b00 are distinct at every position. The two
 * statements that share 0x39/0x62 are separated by control flow instead: the
 * `bgt 0x080d01cc` at 0x080d0198 skips the first, so it is the `frame <= 0x57`
 * arm.
 *
 * THE INDEX IS THE RAW FIELD, AND THAT IS THE FINDING. At 0x080d01a0 (and the
 * matching load at each site) the index is `ldr r0, [r3, #4]` -- scene field
 * +4 -- used raw, `lsls r0, r0, #2`, with NO normalisation. The function does
 * normalise the same field elsewhere: at 0x080d0180-0x080d0190 it computes
 * `r7 = (scene->field_4 != 0)` and r7 is what feeds the `* 7` byte tables. So
 * the draft's single `variant` name covered two different quantities, and only
 * one of them is a boolean.
 *
 * DOCUMENTED UNCERTAINTY -- the evidence stops here and a name would be
 * invention. What is PROVEN: the dispatch is `renderers[scene->field_4]` off
 * the two-word table this function fills at sp+44 and sp+48. What is NOT
 * proven: that `scene->field_4` is confined to {0, 1}. The normalisation at
 * 0x080d0180 is affirmative evidence that this function does NOT treat the
 * field as already boolean -- otherwise the `cmp`/`bne` would be dead -- yet
 * the renderer index uses it unnormalised. Three readings survive: the caller
 * constrains the field; the table is longer than the two words filled here and
 * the rest of the frame is deliberate; or the field is boolean in practice and
 * the normalisation is only the compiler materialising a `!= 0` for the
 * multiply. NOTHING IN THIS FUNCTION SEPARATES THEM. The code below reproduces
 * the raw index because that is what the ROM does; a reader must not infer
 * from it that the field is bounded.
 *
 * ARITY: six at all eight r4 sites. r0..r3 are set and two more words go out
 * at [sp, #0] and [sp, #4]. r4 is above the argument registers, so no argument
 * slot holds the callee.
 *
 * UNCERTAINTY, left standing: what slots 46 and 47 CONTAIN is not settled
 * here. The slot table unifies the addressing, never the contents. The memory
 * sites still open in this audit read a callee out of a heap record whose
 * contents depend on what ran before, and those SHOULD end as written
 * uncertainties rather than names. A page of bounded uncertainties is this
 * job going right, not a reconstruction giving up.
 */
typedef void (*Renderer_080cfef4)(
    s32 target, void *source, s32 x, s32 y, s32 width, s32 height);
typedef void *(*WordCopy_080cfef4)(void *destination, const void *source,
                                   s32 size);
typedef void (*ArmFill_080cfef4)(void *destination, u32 size, u32 value);

void Func_080cdb24(s32);
s32 Func_08002f40(s32);
void Func_08005340();
void Func_080ed408();
void Func_080041d8();
s32 Func_08002322(s32);
s32 Func_0800231c(s32);
void Func_080f9010(s32);
void Func_080e396c(s32, s32 *);
void **Func_080b5098(s32);
void Func_08009150();
void Func_080d6888(s16, s32, s32, s32);
void Func_080030f8(s32);
void Func_08004278(const void *);
void Func_080b5040(s32, u16, s32);
void Func_08002dd8(s32);
void Func_080cdbc0(void);

void Func_080cfef4(void *scene)
{
    u8 *runtime = *(u8 **)0x03001eec;
    s32 graphics_source = *(s32 *)0x03001ef0;
    s32 resource_2e;
    s32 side;
    s32 center_x;
    s32 center_y;
    s32 draw_enabled;
    s32 frame;
    Renderer_080cfef4 renderers[2];

    PTR_AT(runtime, 0x7828) = scene;
    Func_080cdb24(0);
    *(volatile u16 *)0x04000020 = 0x0100;
    *(volatile u16 *)0x04000052 = 0x1010;

    resource_2e = Func_08002f40(0xab);
    ((WordCopy_080cfef4)0x03001388)(
        (void *)0x05000000, (const void *)resource_2e, 0x80);
    Func_08005340(resource_2e + 0x80, runtime);
    Func_08005340(Func_08002f40(0xac) + 0x80, (void *)0x02010000);
    Func_080ed408(0x2e, 7, 7, 3, 1);
    renderers[0] = *(Renderer_080cfef4 *)0x03001f08;
    Func_080ed408(0x2f, 7, 7, 7, 1);
    renderers[1] = *(Renderer_080cfef4 *)0x03001f0c;

    Func_080041d8((void *)0x080dbb9d, 0x480);
    S32_AT(runtime, 0x7780) = 1;
    S32_AT(runtime, 0x7784) = 0;
    Func_080041d8((void *)0x080cd261, 0x480);

    draw_enabled = 1;
    if (S32_AT(scene, 4) == 1)
        center_x = -0x500000;
    else
        center_x = 0x700000;
    center_y = -0x200000;

    for (frame = 0; frame != 0x84; frame++) {
        s32 angle = frame << 9;
        s32 x = (center_x >> 16) +
                ((Func_08002322(angle) * 16) >> 16);
        s32 y = (center_y >> 16) +
                ((Func_0800231c(angle) * 4) >> 16) + 16;
        s32 spread = 0;
        s32 i;
        s32 wave_angle;
        s32 *wave;

        if (frame == 0x58)
            Func_080f9010(0x86);

        if (frame == 0x20) {
            center_x = S32_AT(scene, 4) == 1 ? -0x200000 : 0x480000;
            center_y = 0x180000;
            draw_enabled = 0;
        }
        if (frame == 0x21) {
            *(volatile u16 *)0x04000052 = 0x1010;
            draw_enabled = 1;
        }
        if (frame == 0x40) {
            s32 destination;
            Func_080e396c(S16_AT(scene, 0x24), &destination);
            center_x =
                (destination - (S32_AT(scene, 4) == 1 ? 0x80 : 0x40))
                << 16;
            center_y = 0;
            draw_enabled = 0;
        }
        if (frame == 0x41) {
            *(volatile u16 *)0x04000052 = 0x1010;
            draw_enabled = 1;
        }

        if (frame <= 0x1f) {
            if (frame > 0x0f) {
                spread = frame * 2 - 0x20;
                *(volatile u16 *)0x04000052 =
                    (u16)((0x1f - frame) | 0x1000);
            }
        } else if (frame <= 0x3f && frame > 0x2f) {
            *(volatile u16 *)0x04000052 =
                (u16)((0x3f - frame) | 0x1000);
            spread = frame * 2 - 0x60;
        }
        if (spread < 0)
            spread = 0;

        wave = (s32 *)(runtime + 0x6980);
        wave_angle = frame << 11;
        for (i = 0; i != 0xa0; i++) {
            *wave++ = ((6 - (x + 0x30)) << 8) -
                      ((spread * Func_08002322(wave_angle)) >> 10);
            wave_angle += 0x800;
        }

        if (draw_enabled) {
            s32 variant = S32_AT(scene, 4) != 0;
            s32 table_index;

            if (frame <= 0x57) {
                renderers[S32_AT(scene, 4)](
                    graphics_source, runtime,
                    U8_AT((void *)0x080ee10c, variant * 7),
                    U8_AT((void *)0x080ee11a, 0) + y, 0x39, 0x62);
            } else {
                table_index = variant * 7;
                if (frame <= 0x5b) {
                    renderers[S32_AT(scene, 4)](
                        graphics_source, runtime,
                        U8_AT((void *)0x080ee10c, table_index),
                        U8_AT((void *)0x080ee11a, 0) + y, 0x39, 0x62);
                }

                renderers[S32_AT(scene, 4)](
                    graphics_source, runtime + 0x15d2,
                    U8_AT((void *)0x080ee10c, table_index + 1),
                    U8_AT((void *)0x080ee11a, 1) + y, 0x63, 0x45);

                if ((u32)(frame - 0x58) <= 1)
                    ((ArmFill_080cfef4)0x03000168)(
                        (void *)graphics_source, 0x4000, 0x3f3f3f3f);
                if ((u32)(frame - 0x5a) <= 1)
                    renderers[S32_AT(scene, 4)](
                        graphics_source, runtime + 0x3081,
                        U8_AT((void *)0x080ee10c, table_index + 2),
                        U8_AT((void *)0x080ee11a, 2) + y, 0x80, 0x5b);
                if ((u32)(frame - 0x5c) <= 1)
                    renderers[S32_AT(scene, 4)](
                        graphics_source, (void *)0x02010000,
                        U8_AT((void *)0x080ee10c, table_index + 3),
                        U8_AT((void *)0x080ee11a, 3) + y, 0x80, 0x5b);
                if ((u32)(frame - 0x5e) <= 1)
                    renderers[S32_AT(scene, 4)](
                        graphics_source, (void *)0x02012d80,
                        U8_AT((void *)0x080ee10c, table_index + 4),
                        U8_AT((void *)0x080ee11a, 4) + y, 0x80, 0x3b);
                if ((u32)(frame - 0x60) <= 1)
                    renderers[S32_AT(scene, 4)](
                        graphics_source, (void *)0x02014b00,
                        U8_AT((void *)0x080ee10c, table_index + 5),
                        U8_AT((void *)0x080ee11a, 5) + y, 0x7a, 0x1d);
                if ((u32)(frame - 0x62) <= 1)
                    renderers[S32_AT(scene, 4)](
                        graphics_source, (void *)0x020158d2,
                        U8_AT((void *)0x080ee10c, table_index + 6),
                        U8_AT((void *)0x080ee11a, 6) + y, 0x4c, 0x19);
            }
        }

        if (frame == 0x58) {
            void *object = *Func_080b5098(S16_AT(scene, 0x24));
            S32_AT(object, 0x28) = 0x10000;
            S32_AT(object, 0x34) = 0x20000;
            S32_AT(object, 0x30) = 0x20000;
            S32_AT(object, 0x48) = 0;
            S8_AT(object, 0x5a) = 0;
            S8_AT(object, 0x58) = 0;
            Func_08009150(
                object, S32_AT(object, 8) * 2, 0, S32_AT(object, 0x10));
            Func_080d6888(S16_AT(scene, 0x24), -1, 5, 0);
        }
        if (frame == 0x78)
            S32_AT(*Func_080b5098(S16_AT(scene, 0x24)), 0x48) = 0xab85;

        S32_AT(runtime, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08004278((const void *)0x080dbb9d);
    Func_080b5040(1, U16_AT(*(void **)0x03001e74, 0x648), 0x18);
    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    Func_080cdbc0();
}

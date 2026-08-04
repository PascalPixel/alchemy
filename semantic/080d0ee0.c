#include "types.h"

/*
 * Correctness fix, veneer audit (2026-08-01).
 *
 * Neither `Func_080072f0` nor `Func_080072f4` is a function.  0x080072e4
 * begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte `bx rN; nop`
 * entries, r0..lr, ending at 0x08007320 -- so 0x080072f0 is `__call_via_r3`
 * and 0x080072f4 is `__call_via_r4`.  A `bl` to either calls whatever the
 * named register holds.
 *
 * Site 0x080d0f34, via r3.  r3 is loaded at 0x080d0f2e from the pool word at
 * 0x080d1278: 0x03001388, the relocated IWRAM word copy.  This file had
 * already found that value -- it declares `transfer` and casts 0x03001388 to
 * a transfer shape -- and then passed it as a fourth ARGUMENT.  It is the
 * call target.  Three arguments remain (r0 = 0x05000000, r1 = the resource
 * pointer, r2 = 0x80); the signature is the one the EXACT src/080d40ec.c
 * gives for this address.
 *
 * Site 0x080d11dc, via r4.  r4 comes from `[sp, #60]`, stored at 0x080d0f7e
 * from `ldr r6, [r6, #28]` at 0x080d0f72 with r6 = 0x03001eec (pool word at
 * 0x080d126c).  0x03001eec + 28 = 0x03001f08, which is not a "renderer
 * global" but allocator slot 46: Func_080048b0's table base is 0x03001e50 and
 * slot n lives at +n*4, and 46*4 = 184.  The slot holds a routine DMA'd out
 * of ROM, entered through the veneer.  The draft named the address correctly
 * as `runtime_header[7]` and was wrong only about what it is for.
 *
 * ARGUMENT COUNT AT THAT SITE IS SIX, not the drafted eight, and this is the
 * exception to the usual r4-and-above rule that the callee never appears in
 * the argument list.  Registers live at the branch:
 *
 *   r0       [sp, #72]          render context
 *   r1       graphics + tile
 *   r2       r0 - [sp, #28]     x - half
 *   r3       ip - r8            y - path_count
 *   [sp, #0] r8                 path_count
 *   [sp, #4] r9                 path_count * 2
 *   r4       [sp, #60]          THE CALLEE
 *
 * The draft's seventh argument is that callee, and its eighth is `[sp, #72]`
 * a second time -- the same value already passed as the first argument.  Both
 * trailing arguments are artefacts of an eight-parameter phantom prototype
 * being filled from the last two loads before the branch.
 *
 * Pool constants SETTLED against the ROM (2026-08-01, exact reconstruction).  The
 * four previously disputed values were read straight from the built reference
 * and the earlier reading was correct on all four; the draft has been corrected:
 *   0x080d1274 = 0x79        (was drafted 0x9d)
 *   0x080d127c = 0x73        (was drafted 0x9e)
 *   0x080d1294 = 0x080ee140  (was drafted 0x080ee200)
 *   0x080d129c = 0x080ede48  (was drafted 0x080ee280)
 * Why they looked unreachable: all four addresses lie OUTSIDE this owner's
 * span, which ends at 0x080d126c.  They are pool words of the NEXT owner,
 * 0x080d126c (span 60, ending 0x080d12a8), reached by this function's
 * pc-relative loads -- the shared-literal-pool pattern.  Verified by
 * indexing out/asm/080d126c.bin directly, not by reading the disassembly.
 * The constants that already matched (0x080cd261, 0x7828, 0x7780, 0x7784,
 * 134) live in this owner's own pool, which is why they were never in doubt.
 */

/* 0x03001388, the relocated IWRAM word copy; src/080d40ec.c gives the shape. */
typedef void *(*WordCopy_080d0ee0)(void *destination, const void *source,
                                   s32 size);

/*
 * Allocator slot 46 at 0x03001f08, read through 0x03001eec + 28.  Six
 * arguments, four in registers and two on the stack, read off the branch.
 */
typedef void (*SlotRoutine_080d0ee0)(void *render_context, const void *tile,
                                     s32 x, s32 y, s32 width, s32 span);

struct Scene_080d0ee0 {
    u8 unknown_00[8];
    s32 focus_id;
    u8 unknown_0c[24];
    s16 secondary_id;
};

struct Object_080d0ee0 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[20];
    s32 effect_40;
    u8 unknown_44[4];
    s32 effect_48;
    s32 effect_52;
    u8 unknown_56[16];
    void *callback;
};

struct ObjectContext_080d0ee0 {
    struct Object_080d0ee0 *object;
};

struct Motion_080d0ee0 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 unused;
};

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
void Func_080049ac(void);
void Func_080049e8(void);
void Func_08004a5c(void);
void Func_08004c1c(s32);
void Func_08004c6c(s32);
void Func_08004cb4(void *);
void Func_08004cf0(s32 *);
void Func_080051d8(void *, void *);
void Func_08005340(const void *, void *);
void Func_08009080(void *, s32);
void Func_08009088(void *, s32);
void Func_08009140(struct Object_080d0ee0 *);
void Func_08009150(struct Object_080d0ee0 *, s32, s32, s32);
struct ObjectContext_080d0ee0 *Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e155c(s32, s32);
s32 Func_080e3944(const s32 *, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080d0ee0(struct Scene_080d0ee0 *scene)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    void *graphics = (void *)runtime_header[2];
    void *effect_context = *(void **)0x03001e80;
    WordCopy_080d0ee0 word_copy = (WordCopy_080d0ee0)0x03001388;
    SlotRoutine_080d0ee0 slot_routine =
        (SlotRoutine_080d0ee0)runtime_header[7]; /* 0x03001f08, slot 46 */
    struct Object_080d0ee0 *focus;
    struct Object_080d0ee0 *target;
    struct Motion_080d0ee0 *motions =
        (struct Motion_080d0ee0 *)(runtime + 0x7080);
    s32 transformed[3];
    s32 frame;
    s32 phase;
    s32 i;

    *(struct Scene_080d0ee0 **)(runtime + 0x7828) = scene;
    target = Func_080b5098(scene->focus_id)->object;
    Func_080cd594(1);

    /* 0x080d0f34: bl __call_via_r3, r3 = 0x03001388. */
    word_copy((void *)0x05000000, Func_08002f40(0x79), 0x80);
    Func_08005340(Func_08002f40(0x73), graphics);
    Func_08009080(target, 2);
    Func_08009088(target, 48);
    Func_080ed408(46, 7, 7, 3, 2);

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    focus = Func_080b5098(scene->focus_id)->object;
    target = Func_080b5098(scene->secondary_id)->object;
    for (i = 0; i < 3; i++) {
        struct Motion_080d0ee0 *motion = &motions[i];

        motion->x = focus->x;
        motion->y = focus->y + 0x280000;
        motion->z = focus->z;
        motion->velocity_x =
            Func_080022ec(target->x * (i == 0 ? 1 : 2) - motion->x, 12);
        motion->velocity_y =
            Func_080022ec(target->y - motion->y + 0x280000, 12);
        motion->velocity_z =
            Func_080022ec(target->z - motion->z, 12);
        motion->unused = 0;
    }

    for (frame = 0; frame < 60; frame++) {
        volatile u16 *angle = (volatile u16 *)((u8 *)effect_context + 54);

        if (frame <= 47) {
            s32 turn = frame <= 39 ? 0x80 : 0x300 - frame * 16;

            if (*(s32 *)(runtime + 0x7824) == 0)
                *angle -= turn;
            else
                *angle += turn;
        }

        for (phase = 0; phase < 3; phase++) {
            s32 phase_frame = frame - phase * 12;
            s32 path_count;
            s32 maximum = 0;

            if (phase_frame < 0)
                continue;

            path_count = (phase_frame + (phase_frame < 0 ? 3 : 0)) / 4 + 2;
            if (path_count > 10)
                path_count = 10;

            Func_080049ac();
            Func_080051d8(effect_context, (u8 *)effect_context + 12);
            Func_08004cb4(&motions[phase]);

            for (i = 0; i < 10; i++) {
                s32 scale[3];
                s32 amount = phase_frame << 12;
                s32 stride = phase * 7 * 4;
                s32 value;

                Func_080049e8();
                Func_08004c6c(phase_frame << 10);
                Func_08004c1c(0x4000);
                scale[0] = amount + 0x1000;
                if (scale[0] > 0x10000)
                    scale[0] = 0x10000;
                scale[1] = scale[0];
                scale[2] = scale[0];
                Func_08004cf0(scale);
                Func_08004c6c(i * 0x1c72);

                value = Func_080e3944(
                    (const s32 *)(0x080ee140 + (i & 1) * 12 + stride),
                    transformed);
                if (maximum < value)
                    maximum = value;

                transformed[0] >>= 1;
                motions[i].velocity_x = transformed[0];
                motions[i].velocity_y = transformed[1];
                Func_08004a5c();
            }

            if (maximum <= 0x9fff) {
                s32 half = (path_count + (path_count < 0)) / 2;

                for (i = 0; i < 10; i++) {
                    s32 next = Func_080022fc(i + 1, 10) + phase;
                    struct Motion_080d0ee0 *from = &motions[i + phase];
                    struct Motion_080d0ee0 *to = &motions[next];
                    s32 step;

                    for (step = 1; step <= 16; step++) {
                        s32 x = from->velocity_x +
                            ((to->velocity_x - from->velocity_x) * step) / 16;
                        s32 y = from->velocity_y +
                            ((to->velocity_y - from->velocity_y) * step) / 16;
                        u16 tile = *(const u16 *)(0x080ede48 +
                            (path_count * 2 - 2));

                        /* 0x080d11dc: bl __call_via_r4, r4 = slot 46. */
                        slot_routine(
                            render_context,
                            (const u8 *)graphics + tile,
                            x - half,
                            y - path_count,
                            path_count,
                            path_count * 2);
                    }
                }
            }

            motions[phase].x += motions[phase].velocity_x;
            motions[phase].y += motions[phase].velocity_y;
            motions[phase].z += motions[phase].velocity_z;

            if (frame == phase * 12 + 10) {
                target->effect_52 = 0x10000;
                target->effect_48 = 0x80000;
                target->effect_40 = 0x1400000;
                target->callback = (void *)0x080d1341;
                *((u8 *)target + 90) = 0;
                Func_08009140(target);
                if (target->x < 0)
                    Func_08009150(
                        target, target->x + 0x280000, 0, target->z);
                else
                    Func_08009150(
                        target, target->x + 0x500000, 0, target->z);

                if (phase == 2)
                    Func_080b50e8(134);
                else {
                    Func_080f9010(134);
                    Func_080d6888(scene->secondary_id, 7, 5, 0, 8);
                    *(s32 *)(runtime + 0x77a8) = 4;
                }
            }
        }

        Func_080e155c(8, 8);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}

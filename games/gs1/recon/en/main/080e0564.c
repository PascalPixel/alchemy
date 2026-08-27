#include "types.h"

/*
 * Draft for the battle-presentation sub-effect at 0x080e0564.
 *
 * Family-matched to games/gs1/src/battle/effects/member_orbit/run.c
 * (owner 080ce85c, template-main-080ce85c) but this owner's size (860 bytes
 * vs the template's 724) shows it is a genuinely different sub-effect from
 * the same 0x03001eec "battle work" subsystem already partly recovered in
 * games/gs1/recon/en/main/080e7404.c, 080d59b0.c, 080d82b0.c, 080dc1ec.c and
 * 080e01e4.c.  This owner combines the template's direct/inline
 * Func_080ed408(46,...)/Func_080ed408(47,...) + heap_cache[7]/heap_cache[8]
 * "rectangle" readback (rather than 080e01e4's Func_080cef64 helper) with
 * 080e01e4's 96-pass outer loop / 512-slot shared particle pool shape.
 *
 * Per games/gs1/recon/en/main/080e01e4.json's fully-derived evidence,
 * "Func_080072f4" and "Func_08007300" are NOT real callees: 0x080072f4 and
 * 0x08007300 are the r4 and r7 slots of the `_call_via_rN` trampoline at
 * games/gs1/asm/080072e4.s (base 0x080072e4 + 4*4 and + 4*7).  Every `bl` to
 * either address in the retained assembly is an indirect call through
 * whatever DrawRectangleFn pointer the compiler most recently loaded into
 * that register -- here always one of the two "rectangle" pointers read
 * back from heap_cache[7]/heap_cache[8] right after the Func_080ed408 calls,
 * matching the immediate `ldr r4,[sp,#28]` / `ldr r7,[sp,#24]` right before
 * each such `bl` in games/gs1/asm/080e0564.s.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, u32 w, s32 h);

void Func_080cd594(s32 mode);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080e0524(s32 effect_id, void *target, s32 flag_a, s32 flag_b);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);
u32 Func_08004458(void);
void Func_080f9010(s32 id);
void Func_080b50e8(s32 id);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080b5088(s32 member_id, s32 b);
void Func_080e3908(void *particle, s32 count, s32 flags);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);

extern const u16 Data_080ede48[];
extern u8 Value_00000073;
extern u8 Value_00000094;
extern u8 Value_0000006f;

void Func_080e0564(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *extra_target;
    void *rectangle_a;
    void *rectangle_b;
    u8 *ring;
    s32 i;
    s32 accumulator;
    s32 frame;

    heap_cache = (void **) 0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    extra_target = heap_cache[2];
    M2C_FIELD(work, s32 *, 0x7828) = (s32) object;
    Func_080cd594(0);
    M2C_FIELD((void *)0x04000052, s16 *, 0) = 0x1010;

    Func_080ed408(46, 7, 7, 11, 2);
    Func_080ed408(47, 7, 7, 3, 3);
    rectangle_a = heap_cache[7];
    rectangle_b = heap_cache[8];

    Func_080e0524((s32) &Value_00000073, extra_target, 0, 0);
    Func_080e0524((s32) &Value_00000094, work, 1, 1);
    Func_080e0524((s32) &Value_0000006f, (u8 *) work + 0x2F8, 1, 0);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Func_080041d8((void *) 0x080CD261, 0x480);

    ring = (u8 *) work + 0x7080;
    for (i = 0; i != 32; i++) {
        M2C_FIELD(ring, s32 *, 0) = (s32) (Func_08004458() & 63);
        M2C_FIELD(ring, s32 *, 4) = 104;
        ring += 28;
    }

    {
        s32 *cursor2;

        cursor2 = (s32 *) 0x02010018;
        i = 0;
        do {
            i++;
            *cursor2 = -1;
            cursor2 += 7;
        } while (i != 512);
    }

    Func_080f9010(141);
    frame = 0;
    accumulator = 0x8000;
    do {
        if (frame <= 79) {
            s32 sin_val;
            s32 cos_val;
            s32 scale;
            s32 x;
            s32 y;

            sin_val = Func_08002322(accumulator);
            cos_val = Func_0800231c(accumulator);
            scale = 64 - frame * 2;
            x = ((sin_val * 24) >> 16) + 22;
            y = ((scale * cos_val) >> 16) + 29;
            ((DrawRectangleFn) rectangle_b)(
                draw_destination, work, x, y, 20, 38);
        }

        if (frame == 56) {
            Func_080b50e8(133);
        }

        {
            s32 slot;
            s32 threshold;
            s32 particle_base;
            u8 *slot_ring;

            slot_ring = (u8 *) work + 0x7080;
            threshold = 16;
            particle_base = 0;
            slot = 0;
            do {
                if (frame >= threshold) {
                    ((DrawRectangleFn) rectangle_a)(
                        draw_destination, (u8 *) work + 0x9E0,
                        M2C_FIELD(slot_ring, s32 *, 0) - 17,
                        M2C_FIELD(slot_ring, s32 *, 4) - 32, 34, 65);

                    if (frame == threshold) {
                        u8 *particle;

                        particle = (u8 *) 0x02010000 + particle_base;
                        i = 0;
                        do {
                            u32 kind;
                            s32 scale;

                            kind = (Func_08004458() & 0x7FFF) + 0x4000;
                            scale = (s32) (Func_08004458() & 0x1FF) + 0x100;
                            M2C_FIELD(particle, s32 *, 0) =
                                M2C_FIELD(slot_ring, s32 *, 0) << 16;
                            M2C_FIELD(particle, s32 *, 4) =
                                (M2C_FIELD(slot_ring, s32 *, 4) + 16) << 16;
                            M2C_FIELD(particle, s32 *, 12) =
                                (scale * Func_08002322((s32) kind)) >> 7;
                            M2C_FIELD(particle, s32 *, 16) =
                                (scale * Func_0800231c((s32) kind)) >> 6;
                            M2C_FIELD(particle, s32 *, 24) =
                                (s32) ((Func_08004458() & 15) + 32);
                            particle += 28;
                            i++;
                        } while (i != 16);

                        if (slot & 1) {
                            Func_080f9010(133);
                        }

                        M2C_FIELD(work, s32 *, 0x77A8) = 4;
                        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                                s32 *, 20) != 0) {
                            s32 member_id_offset;
                            s32 idx;

                            member_id_offset = 36;
                            idx = 0;
                            do {
                                Func_080d6888(
                                    M2C_FIELD(M2C_FIELD(work, void **,
                                        0x7828), s16 *, member_id_offset),
                                    7, 5, idx, 6);
                                Func_080b5088(
                                    M2C_FIELD(M2C_FIELD(work, void **,
                                        0x7828), s16 *, member_id_offset),
                                    6);
                                idx++;
                                member_id_offset += 2;
                            } while (idx != M2C_FIELD(M2C_FIELD(work,
                                    void **, 0x7828), s32 *, 20));
                        }
                    }

                    M2C_FIELD(slot_ring, s32 *, 4) =
                        M2C_FIELD(slot_ring, s32 *, 4) - 12;
                }

                particle_base += 0x380;
                threshold += 4;
                slot_ring += 28;
                slot++;
            } while (slot != 10);
        }

        {
            u8 *particle;

            particle = (u8 *) 0x02010000;
            for (i = 0; i != 512; i++) {
                if (M2C_FIELD(particle, s32 *, 24) != -1) {
                    s32 lifetime;
                    s32 idx;
                    s32 half;
                    s32 y;
                    s32 h;

                    lifetime = M2C_FIELD(particle, s32 *, 24);
                    idx = ((lifetime >= 0 ? lifetime : lifetime + 15) >> 4)
                        + 2;
                    half = (idx + ((u32) idx >> 31)) >> 1;
                    y = M2C_FIELD(particle, s16 *, 2) - half;
                    h = M2C_FIELD(particle, s16 *, 6) - idx;
                    ((DrawRectangleFn) rectangle_b)(
                        draw_destination,
                        (u8 *) extra_target + Data_080ede48[idx - 1],
                        y, h, idx, idx << 1);
                    Func_080e3908(particle, 62, 0x2000);
                    M2C_FIELD(particle, s32 *, 24) =
                        M2C_FIELD(particle, s32 *, 24) - 1;
                }
                particle += 28;
            }
        }

        Func_080e155c(4, 4);
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);

        accumulator -= 2048;
        frame++;
    } while (frame != 96);

    Func_08004278((void *) 0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}

#include "types.h"

/*
 * Draft for the battle-presentation sub-effect at 0x080e08c0.
 *
 * Family-matched to games/gs1/src/battle/effects/member_orbit/run.c
 * (template-main-080ce85c, score 7947/10000) but the owner's real callee
 * set and field/constant layout match the 0x03001eec "battle work"
 * subsystem cluster already recovered at games/gs1/recon/en/main/
 * 080e7404.c, 080d59b0.c, 080d82b0.c, 080dc1ec.c and 080e01e4.c, not the
 * assigned template's Func_080cd594/Func_08002f40/Func_080ed408 shape.
 * See those files and their dossiers for the evidence behind the choices
 * below (heap_cache/cursor prologue, Value_XXXXXXXX effect-id idiom, the
 * Func_080072f4/Func_080072f0 trampoline calls, the shared
 * Data_080ede48/080ede9f/080edea5/080edeab/080edeb2 sprite-cell tables
 * already adopted in games/gs1/src/battle/effects/puff_arc/run.c).
 *
 * Func_080072f4 and Func_080072f0 are not real callees: they are the r4
 * and r3 entries of the _call_via_rN trampoline bundle at
 * games/gs1/asm/080072e4.s.  Every Func_080072f4 site is an indirect call
 * through the draw-rectangle callback pair Func_080cef64 populates;
 * Func_080072f0 is a direct call to the routine at 0x03001388 with that
 * pointer passed as the trailing WordCopyFn argument.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*WordCopyFn)(void *dest, void *src, s32 size);
typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, u32 w, s32 h);

void Func_080cd594(s32 mode);
void Func_080cef64(s32 flag, DrawRectangleFn *out_pair);
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
void *Func_08002f40(s32 id);
void Func_080072f0(void *dest, void *src, s32 size, WordCopyFn copier);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080b5088(s32 member_id, s32 unk);
void Func_080e3908(void *particle, s32 count, s32 flags);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);

extern const u16 Data_080ede48[];
extern u8 Data_080ede9f[];
extern u8 Data_080edea5[];
extern u8 Data_080edeab[];
extern u16 Data_080edeb2[];
extern u8 Value_00000073;
extern u8 Value_0000008e;
extern u8 Value_000000b7;
extern u8 Value_000000b4;

s32 Func_080e08c0(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *extra_target;
    DrawRectangleFn callbacks[2];
    DrawRectangleFn *callback_ptr;
    void *ring;
    void *particle;
    void *target;
    s32 burst_offset;
    s32 member;
    s32 i;
    s32 frame;
    s32 curtain_h;
    s32 member_id_offset;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    extra_target = heap_cache[2];
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(0);

    callback_ptr = callbacks;
    Func_080cef64(0, callback_ptr);

    Func_080e0524((s32) &Value_00000073, extra_target, 0, 0);
    Func_080e0524((s32) &Value_0000008e, work, 1, 0);
    Func_080e0524((s32) &Value_000000b7, (u8 *) work + 0x320, 1, 1);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 0x4B;
    Func_080041d8((void *) 0x080CD261, 0x480);

    burst_offset = 0;
    member = 0;
    ring = (u8 *) work + 0x7080;
    do {
        s32 angle;

        angle = member << 11;
        M2C_FIELD(ring, s32 *, 0) = (Func_08002322(angle) * 0x18) >> 16;
        M2C_FIELD(ring, s32 *, 4) =
            ((Func_0800231c(angle) * 4) >> 16) + 0x34;
        if (member & 1) {
            M2C_FIELD(ring, s32 *, 0) = 0x20 - M2C_FIELD(ring, s32 *, 0);
        } else {
            M2C_FIELD(ring, s32 *, 0) = M2C_FIELD(ring, s32 *, 0) + 0x20;
        }
        M2C_FIELD(ring, s32 *, 0x18) = 0 - (member * 2);

        i = 0;
        particle = (u8 *) 0x02010000 + burst_offset;
        {
            s32 mask;

            mask = 0x02010000;
            do {
                M2C_FIELD(particle, s32 *, 0) =
                    (((Func_08004458() & 0xF) + M2C_FIELD(ring, s32 *, 0))
                        - 8)
                    << 16;
                M2C_FIELD(particle, s32 *, 4) =
                    ((Func_08004458() & 7) + 0x60) << 16;
                M2C_FIELD(particle, s32 *, 0xC) =
                    ((Func_08004458() & 0x7F) - 0x40) << 11;
                M2C_FIELD(particle, s32 *, 0x10) =
                    ((Func_08004458() & 0x7F) - 0x40) << 10;
                M2C_FIELD(particle, s32 *, 8) = Func_08004458() & mask;
                i++;
                M2C_FIELD(particle, s32 *, 0x14) = Func_08004458() & mask;
                particle = (u8 *) particle + 0x1C;
            } while (i != 16);
        }

        member++;
        ring = (u8 *) ring + 0x1C;
        burst_offset += 0x1C0;
    } while (member != 9);

    Func_080f9010(0x88);
    frame = 0;
    curtain_h = -0xAC;
    do {
        if (frame == 0x38) {
            Func_080b50e8(0x85);
        }

        if (frame <= 0x17) {
            callbacks[0](
                draw_destination,
                (u8 *) work + 0x320 + (frame / 4) * 0x640,
                0x28, 0x14, 0x28, 0x28);
        }

        if (frame == 0x14) {
            Func_080072f0(
                (void *) 0x05000000, Func_08002f40((s32) &Value_0000008e),
                0x80, (WordCopyFn) 0x03001388);
        }

        if ((u32) (frame - 0x14) <= 0xBU) {
            if (frame > 0x17) {
                callback_ptr[1](
                    draw_destination, work, 0x92 - (frame * 4), curtain_h,
                    0x14, 0x28);
            } else {
                callback_ptr[1](
                    draw_destination, work, 0x32, 0x14, 0x14, 0x28);
            }
        }

        if (frame == 0x20) {
            Func_080f9010(0x91);
            M2C_FIELD(work, s32 *, 0x77A8) = 8;
            Func_080e0524((s32) &Value_000000b4, work, 1, 1);
        }

        if (frame > 0x1F) {
            ring = (u8 *) work + 0x7080;
            member = 0;
            do {
                if (M2C_FIELD(ring, s32 *, 0x18) >= 0
                        && M2C_FIELD(ring, s32 *, 0x18) <= 0x2F) {
                    s32 cell;
                    u32 width;

                    cell = M2C_FIELD(ring, s32 *, 0x18) / 8;
                    callbacks[0](
                        draw_destination,
                        (u8 *) work + Data_080edeb2[cell],
                        M2C_FIELD(ring, s32 *, 0)
                            - ((width = Data_080ede9f[cell]) >> 1),
                        M2C_FIELD(ring, s32 *, 4) + Data_080edeab[cell],
                        width, Data_080edea5[cell]);
                }
                member++;
                M2C_FIELD(ring, s32 *, 0x18) =
                    M2C_FIELD(ring, s32 *, 0x18) + 1;
                ring = (u8 *) ring + 0x1C;
            } while (member != 9);
        }

        particle = (void *) 0x02010000;
        member = 0;
        do {
            if (frame >= (member / 16) * 2 + 0x28) {
                s32 size;
                s32 x;
                s32 old_angle;
                s32 new_angle;

                x = M2C_FIELD(particle, s16 *, 2)
                    + ((Func_08002322(M2C_FIELD(particle, s32 *, 8)) * 4)
                        >> 16);
                callback_ptr[1](
                    draw_destination,
                    (u8 *) extra_target
                        + Data_080ede48[(size = (member & 1) + 3) - 1],
                    x - ((u32) size >> 1),
                    M2C_FIELD(particle, s16 *, 6) - size, size, size * 2);
                Func_080e3908(particle, 0x40, -0x2000);
                old_angle = M2C_FIELD(particle, s32 *, 8);
                new_angle = old_angle + 0x800;
                M2C_FIELD(particle, s32 *, 8) = new_angle;
                if (new_angle > 0xFFFF) {
                    M2C_FIELD(particle, s32 *, 8) = old_angle + 0xFFFF0801;
                }
            }
            member++;
            particle = (u8 *) particle + 0x1C;
        } while (member != 0x90);

        if (frame == 0x26) {
            member = 0;
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20)
                    != 0) {
                member_id_offset = 36;
                do {
                    target = M2C_FIELD(work, void **, 0x7828);
                    Func_080d6888(
                        M2C_FIELD(target, s16 *, member_id_offset), 7, 5,
                        member, 16);
                    target = M2C_FIELD(work, void **, 0x7828);
                    Func_080b5088(
                        M2C_FIELD(target, s16 *, member_id_offset), 6);
                    target = M2C_FIELD(work, void **, 0x7828);
                    member++;
                    member_id_offset += 2;
                } while (member != M2C_FIELD(target, s32 *, 20));
            }
        }

        Func_080e155c(8, 8);
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);

        frame++;
        curtain_h += 8;
    } while (frame != 0x70);

    Func_08004278((void *) 0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    return Func_080cdbc0();
}

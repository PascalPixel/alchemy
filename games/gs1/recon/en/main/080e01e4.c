#include "types.h"

/*
 * Draft for the battle-presentation sub-effect at 0x080e01e4.
 *
 * Assigned from the orbiting_particles/run.c compiler-family cluster
 * (template-main-08099160); like its siblings 080d59b0/080d82b0/080dc1ec
 * this owner's real callee set and constants match the 0x03001eec "battle
 * work" subsystem already partly recovered at games/gs1/recon/en/main/
 * 080e7404.c, 080d59b0.c, 080d82b0.c and 080dc1ec.c, not the assigned
 * template.  See those files for the evidence behind the field/signature
 * choices below.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, u32 w, s32 h);

void Func_080cd594(s32 mode);
void Func_080cef64(s32 flag, DrawRectangleFn *out_callbacks);
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
void Func_080072f4(
    s32 dest, s32 src, s32 x, s32 y, DrawRectangleFn callback, s32 w,
    s32 h);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080b5088(s32 member_id);
void Func_080e3908(void *particle, s32 count, s32 flags);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);

extern const u16 Data_080ede48[];

s32 Func_080e01e4(void *object)
{
    void *work;
    void *draw_destination;
    void *extra_target;
    void *target;
    void **target_slot;
    DrawRectangleFn callbacks[2];
    u8 *ring;
    u8 *particle;
    s32 i;
    s32 fp;
    s32 accumulator;
    s32 member;
    s32 member_id_offset;
    s32 member_offset;

    work = *(void **)0x03001EEC;
    draw_destination = *(void **)0x03001EF0;
    extra_target = *(void **)0x03001EF4;
    M2C_FIELD(work, s32 *, 0x7828) = (s32) object;
    Func_080cd594(0);
    M2C_FIELD((void *)0x04000052, s16 *, 0) = 0x1010;
    Func_080cef64(0, callbacks);
    Func_080e0524(0x73, extra_target, 0, 0);
    Func_080e0524(0x90, work, 1, 1);
    Func_080e0524(0x89, (u8 *) work + 0x320, 1, 0);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Func_080041d8((void *) 0x080CD261, 0x480);

    ring = (u8 *) work + 0x7080;
    for (i = 0; i != 32; i++) {
        M2C_FIELD(ring, s32 *, 0) = (s32) ((Func_08004458() & 63) + 64);
        M2C_FIELD(ring, s32 *, 4) = (s32) ((Func_08004458() & 63) - 80);
        ring += 28;
    }

    {
        s32 *cursor;
        s32 n;

        cursor = (s32 *) 0x02010018;
        n = 0;
        do {
            n++;
            *cursor = -1;
            cursor += 7;
        } while (n != 512);
    }

    Func_080f9010(171);
    accumulator = 0x8000;
    fp = 0;
    do {
        if (fp == 56) {
            Func_080b50e8(133);
        }

        if (fp <= 95) {
            s32 sin_val;
            s32 cos_val;
            s32 scale;
            s32 x;
            s32 y;

            sin_val = Func_08002322(accumulator);
            scale = 64 - fp * 2;
            x = ((scale * sin_val) >> 17) + 86;
            cos_val = Func_0800231c(accumulator);
            y = ((scale * cos_val) >> 16) + 28;
            Func_080072f4(
                (s32) draw_destination, (s32) work, x, y, callbacks[0],
                20, 40);
        }

        ring = (u8 *) work + 0x7080;
        member_offset = 0;
        member = 0;
        do {
            if (fp >= member * 4 + 8 && M2C_FIELD(ring, s32 *, 4) <= 95) {
                s32 rx;
                s32 ry;

                rx = M2C_FIELD(ring, s32 *, 0) - 20;
                ry = M2C_FIELD(ring, s32 *, 4) - 32;
                Func_080072f4(
                    (s32) draw_destination, (s32) ((u8 *) work + 800), rx,
                    ry, callbacks[1], 40, 64);

                M2C_FIELD(ring, s32 *, 0) =
                    M2C_FIELD(ring, s32 *, 0) - 6;
                M2C_FIELD(ring, s32 *, 4) =
                    M2C_FIELD(ring, s32 *, 4) + 12;

                if (M2C_FIELD(ring, s32 *, 4) > 95) {
                    particle = (u8 *) 0x02010000 + member_offset;
                    for (i = 0; i != 32; i++) {
                        u32 kind;
                        s32 angle;

                        kind = Func_08004458() & 0xFFFF;
                        angle = (s32) (Func_08004458() & 0x1FF) + 256;
                        M2C_FIELD(particle, s32 *, 0) =
                            M2C_FIELD(ring, s32 *, 0) << 16;
                        M2C_FIELD(particle, s32 *, 4) =
                            M2C_FIELD(ring, s32 *, 4) << 16;
                        M2C_FIELD(particle, s32 *, 12) =
                            (angle * Func_08002322((s32) kind)) >> 7;
                        M2C_FIELD(particle, s32 *, 16) =
                            (angle * Func_0800231c((s32) kind)) >> 6;
                        M2C_FIELD(particle, s32 *, 24) =
                            (s32) ((Func_08004458() & 15) + 32);
                        particle += 28;
                    }
                    Func_080f9010(133);
                }
            }

            ring += 28;
            member_offset += 0x380;
            member++;
        } while (member != 8);

        M2C_FIELD(work, s32 *, 0x77A8) = 4;
        target = M2C_FIELD(work, void **, 0x7828);
        if (M2C_FIELD(target, s32 *, 20) != 0) {
            s32 count;
            s32 idx;

            target_slot = (void **) ((u8 *) work + 0x7828);
            member_id_offset = 36;
            idx = 0;
            do {
                s32 member_id;

                target = *target_slot;
                member_id = M2C_FIELD(target, s16 *, member_id_offset);
                Func_080d6888(member_id, 7, 5, idx, 6);
                target = *target_slot;
                member_id = M2C_FIELD(target, s16 *, member_id_offset);
                Func_080b5088(member_id);
                target = *target_slot;
                count = M2C_FIELD(target, s32 *, 20);
                idx++;
                member_id_offset += 2;
            } while (idx != count);
        }

        particle = (u8 *) 0x02010000;
        for (i = 0; i != 512; i++) {
            if (M2C_FIELD(particle, s32 *, 24) != -1) {
                s32 lifetime;
                s32 idx;
                s32 half;
                s32 y;
                s32 h;

                lifetime = M2C_FIELD(particle, s32 *, 24);
                idx = ((lifetime >= 0 ? lifetime : lifetime + 15) >> 4) + 1;
                half = (idx + ((u32) idx >> 31)) >> 1;
                y = M2C_FIELD(particle, s16 *, 2) - half;
                h = M2C_FIELD(particle, s16 *, 6) - idx;
                Func_080072f4(
                    (s32) draw_destination,
                    (s32) ((u8 *) extra_target + Data_080ede48[idx - 1]),
                    y, h, callbacks[1], idx, idx << 1);
                Func_080e3908(particle, 62, 0x2000);
                M2C_FIELD(particle, s32 *, 24) =
                    M2C_FIELD(particle, s32 *, 24) - 1;
            }
            particle += 28;
        }

        Func_080e155c(4, 4);
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);

        accumulator -= 2048;
        fp++;
    } while (fp != 96);

    Func_08004278((void *) 0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    return Func_080cdbc0();
}

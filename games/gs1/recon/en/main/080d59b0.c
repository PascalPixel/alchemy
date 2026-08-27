#include "types.h"

/*
 * Draft for the battle-presentation sub-effect at 0x080d59b0.
 *
 * This owner was assigned from the orbiting_particles/run.c compiler-family
 * cluster (template-main-08099160), but its own callee set and constants
 * (Func_080cd594, Func_080e0524, Func_080041d8/Func_08004278 with the shared
 * 0x080CD261 callback and 0x480 interval, Func_08002dd8, Func_080cdbc0,
 * Func_080ed408, work-offsets 0x7780/0x7784/0x7824/0x7828) match the
 * already-drafted 0x080e7404 "battle work" subsystem instead -- see that
 * owner's dossier and games/gs1/recon/en/main/080e7404.c, which is the
 * evidence source for the field/signature choices below.  Byte-offset
 * accesses are kept generic where no evidence-backed structure exists yet.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, u32 w, s32 h);

void Func_080cd594(s32 mode);
void Func_080e0524(s32 effect_id, void *work, s32 flag_a, s32 flag_b);
void Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);
u32 Func_08004458(void);
void Func_080030f8(s32 frames);
void Func_080f9010(s32 id);
void Func_080b50e8(s32 id);
void **Func_080b5098(s32 member_id);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void Func_08004cb4(void *record);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);

s32 Func_080d59b0(void *object)
{
    void *work;
    void *draw_destination;
    void *target;
    void **heap_cache;
    void **cursor;
    DrawRectangleFn callback_a;
    DrawRectangleFn callback_b;
    u8 *star;
    s32 i;
    s32 j;
    s32 frame;
    s32 facing;
    s32 record[3];

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(0);
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x0100;
    M2C_FIELD((void *)0x04000050, s16 *, 0) = 0;
    Func_080e0524(0xA8, work, 1, 1);
    Func_080ed408(46, 7, 7, 3, 1);
    callback_a = (DrawRectangleFn) heap_cache[7];
    Func_080ed408(47, 7, 7, 15, 1);
    callback_b = (DrawRectangleFn) heap_cache[8];

    star = (u8 *)0x02010000;
    for (i = 0; i != 32; i++) {
        M2C_FIELD(star, s32 *, 0) = (s32) (((Func_08004458() & 0x3F) + 32) << 16);
        M2C_FIELD(star, s32 *, 4) = (s32) 0xFFE00000;
        Func_08004458();
        M2C_FIELD(star, s32 *, 16) = 0;
        M2C_FIELD(star, s32 *, 8) = (s32) (Func_08004458() & 3);
        M2C_FIELD(star, s32 *, 24) = (s32) (Func_08004458() & 0xFF);
        star += 28;
    }

    if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 1) {
        *(u32 *)0x04000028 = 0xFFFF9000;
    }
    M2C_FIELD(work, s32 *, 0x7780) = 1;
    M2C_FIELD(work, s32 *, 0x7784) = 0;
    Func_080041d8((void *)0x080CD261, 0x480);
    Func_080f9010(142);

    for (frame = 0; frame != 148; frame++) {
        facing = *(s32 *)0x03001E80;
        if (frame == 80) {
            Func_080b50e8(0);
        }

        target = M2C_FIELD(work, void **, 0x7828);
        j = 0;
        if (M2C_FIELD(target, s32 *, 20) != 0) {
            void **target_slot;
            void *member;
            s32 member_id_offset;

            target_slot = (void **)((u8 *)work + 0x7828);
            member_id_offset = 36;
            do {
                target = *target_slot;
                member = *Func_080b5098(
                    M2C_FIELD(target, s16 *, member_id_offset));
                Func_080049ac();
                Func_080051d8(facing, facing + 12);
                record[0] = M2C_FIELD(member, s32 *, 8);
                record[1] = (s32) (160 << 14);
                record[2] = M2C_FIELD(member, s32 *, 16);
                Func_08004cb4(record);

                if (frame == j * 16 + 64) {
                    Func_080d6888(
                        M2C_FIELD(target, s16 *, member_id_offset),
                        0, 5, -1, 0);
                }

                member_id_offset += 2;
                j++;
                target = *target_slot;
            } while (j != M2C_FIELD(target, s32 *, 20));
        }

        star = (u8 *)0x02010000;
        for (i = 0; i != 12; i++) {
            if (frame > i * 4) {
                s32 velocity;
                s32 phase;
                s32 kind;

                velocity = M2C_FIELD(star, s32 *, 4);
                if (velocity <= 0x7FFFFF) {
                    phase = M2C_FIELD(star, s32 *, 24);
                    kind = ((phase >= 0 ? phase : phase + 15) >> 4) & 7;
                    if (kind <= 3) {
                        callback_a(
                            draw_destination,
                            (void *) ((kind << 10) + (s32) work),
                            M2C_FIELD(star, s16 *, 2) - 16,
                            (velocity >> 16) - 16,
                            32, 32);
                    } else {
                        callback_b(
                            draw_destination,
                            (void *) ((kind << 10) + (s32) work - 0x1000),
                            M2C_FIELD(star, s16 *, 2) - 16,
                            (velocity >> 16) - 16,
                            32, 32);
                    }

                    {
                        s32 accumulator;
                        s32 new_velocity;
                        s32 new_phase;

                        accumulator = M2C_FIELD(star, s32 *, 16) + 0x2000;
                        new_velocity = velocity + M2C_FIELD(star, s32 *, 16);
                        new_phase = phase + M2C_FIELD(star, s32 *, 8);
                        M2C_FIELD(star, s32 *, 4) = new_velocity;
                        M2C_FIELD(star, s32 *, 16) = accumulator;
                        M2C_FIELD(star, s32 *, 24) = new_phase;
                        if (new_velocity > 0x5C00000 && accumulator == 0) {
                            s32 bounced;

                            bounced = -(M2C_FIELD(star, s32 *, 16) + 0x2001);
                            M2C_FIELD(star, s32 *, 8) =
                                M2C_FIELD(star, s32 *, 8) + 4;
                            bounced = (bounced + (s32) ((u32) bounced >> 31)) >> 1;
                            M2C_FIELD(star, s32 *, 4) = 0x5C00000;
                            M2C_FIELD(star, s32 *, 16) = bounced;
                        }
                    }
                }
            }
            star += 28;
        }

        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    return Func_080cdbc0();
}

#include "types.h"

/*
 * Draft for the battle-presentation sub-effect at 0x080dc1ec.
 *
 * Assigned from the orbiting_particles/run.c compiler-family cluster
 * (template-main-08099160); like its siblings 080d59b0/080d82b0/080e01e4
 * this owner's real callee set and constants match the 0x03001eec "battle
 * work" subsystem already partly recovered at games/gs1/recon/en/main/
 * 080e7404.c, 080d59b0.c and 080d82b0.c, not the assigned template.  See
 * those files for the evidence behind the field/signature choices below.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, u32 w, s32 h);

void Func_080cd594(s32 mode);
void Func_080e0524(s32 effect_id, void *work, s32 flag_a, s32 flag_b);
void Func_080cef64(s32 flag, DrawRectangleFn *out_pair);
u32 Func_08004458(void);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void Func_08004cb4(void *record);
void Func_080049e8(void);
void Func_08004c1c(s32 a);
void Func_08004bd4(s32 a);
void Func_08004c6c(s32 a);
void Func_080e3944(void *src, void *dest);
void Func_08004a5c(void);
void Func_080072f4(
    s32 dest, s32 src, s32 x, s32 y, DrawRectangleFn callback, s32 w,
    s32 h);
void Func_080e38b8(void *particle, s32 a, s32 b);
void Func_080030f8(s32 frames);

extern const u16 Data_080ede48[];

s32 Func_080dc1ec(void *object)
{
    void **cursor;
    void *work;
    void *draw_destination;
    DrawRectangleFn callback_pair[2];
    u8 *star;
    s32 i;
    s32 outer;
    s32 j;
    s32 facing;
    s32 record[3];
    s32 result[3];
    s32 velocity_a;
    s32 velocity_b;

    cursor = (void **)0x03001EEC;
    work = *cursor++;
    draw_destination = *cursor;
    M2C_FIELD(work, s32 *, 0x7828) = (s32) object;
    Func_080cd594(0);
    Func_080e0524(140, work, 1, 1);
    Func_080cef64(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) ^ 1,
        callback_pair);

    star = (u8 *)0x02010000;
    for (i = 0; i != 256; i++) {
        M2C_FIELD(star, s32 *, 0) =
            (s32) (((Func_08004458() & 0xFF) - 127) << 16);
        M2C_FIELD(star, s32 *, 4) =
            (s32) (((Func_08004458() & 0xFF) - 127) << 16);
        M2C_FIELD(star, s32 *, 8) =
            (s32) (((Func_08004458() & 0xFF) - 127) << 16);
        M2C_FIELD(star, s32 *, 12) = 0;
        M2C_FIELD(star, s32 *, 16) = 0;
        M2C_FIELD(star, s32 *, 20) = 0;
        M2C_FIELD(star, s32 *, 24) = 0;
        star += 28;
    }

    M2C_FIELD(work, s32 *, 0x7780) = 1;
    M2C_FIELD(work, s32 *, 0x7784) = 0;
    Func_080041d8((void *)0x080CD261, 0x480);

    record[0] = 0;
    record[1] = (s32) (160 << 15);
    record[2] = 0;

    outer = 0;
    do {
        facing = *(s32 *)0x03001E80;
        Func_080049ac();
        Func_080051d8(facing, facing + 12);
        Func_08004cb4(record);

        velocity_a = outer << 8;
        star = (u8 *)0x02010000;
        for (j = 0; j != 64; j++) {
            s32 quarter;

            velocity_b = -velocity_a;

            quarter = (j >= 0 ? j : j + 3) >> 2;
            if (outer > quarter && M2C_FIELD(star, s32 *, 24) == 0) {
                Func_080049e8();
                switch (j & 3) {
                case 0:
                    Func_08004c1c(velocity_a);
                    break;
                case 1:
                    Func_08004bd4(velocity_b);
                    break;
                case 2:
                    Func_08004c6c(velocity_b);
                    break;
                case 3:
                    Func_08004bd4(velocity_b);
                    Func_08004c6c(velocity_b);
                    break;
                }

                Func_080e3944(star, result);
                result[0] = result[0] >> 1;
                Func_08004a5c();
                if (result[2] <= 249) {
                    result[2] = 250;
                }
                if (result[2] > 0x27A) {
                    result[2] = 0x27A;
                }

                {
                    s32 bucket;
                    s32 raw;
                    s32 src_off;
                    s32 half;
                    s32 y;
                    s32 h;

                    bucket = result[2] - 250;
                    if (bucket < 0) {
                        bucket += 63;
                    }
                    bucket >>= 6;
                    raw = 9 - bucket;
                    src_off = (j & 3) * 770 + Data_080ede48[raw - 1];
                    half = (raw + ((u32) raw >> 31)) >> 1;
                    y = result[0] - half;
                    h = result[1] - raw;
                    Func_080072f4(
                        (s32) draw_destination,
                        src_off + (s32) work, y, h, callback_pair[0],
                        raw, raw << 1);
                }

                Func_080e38b8(star, 60, 0);
            }

            quarter = (j >= 0 ? j : j + 3) >> 2;
            quarter += 30;
            if (outer > quarter) {
                M2C_FIELD(star, s32 *, 12) =
                    M2C_FIELD(star, s32 *, 12)
                    + (-M2C_FIELD(star, s32 *, 0) >> 8);
                M2C_FIELD(star, s32 *, 16) =
                    M2C_FIELD(star, s32 *, 16)
                    + (-M2C_FIELD(star, s32 *, 4) >> 8);
                M2C_FIELD(star, s32 *, 20) =
                    M2C_FIELD(star, s32 *, 20)
                    + (-M2C_FIELD(star, s32 *, 8) >> 8);
            }

            velocity_a += outer << 3;
            star += 28;
        }

        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        outer++;
    } while (outer != 160);

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    return Func_080cdbc0();
}

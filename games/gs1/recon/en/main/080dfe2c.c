#include "types.h"

/*
 * Draft for the battle-presentation sub-effect at 0x080dfe2c.
 *
 * Assigned from the member_orbit/run.c compiler-family cluster
 * (template-main-080ce85c); like the other measured drafts in the same
 * 0x03001eec "battle work" subsystem family (games/gs1/recon/en/main/
 * 080e7404.c, 080d59b0.c, 080d82b0.c, 080dc1ec.c, 080e01e4.c, 080dc454.c,
 * 080dc6bc.c, 080ccc38.c, 080cb4ec.c, 080d5c48.c), this owner's real callee
 * set, field offsets, and control flow were recovered from its own retained
 * assembly, not transcribed from the assigned template. In particular the
 * heap_cache/cursor prologue, the M2C_FIELD field-offset idiom, the
 * DrawRectangleFn typedef, and the Func_080cef64/Func_080e0524/
 * Func_080e3908/Func_080e155c/Func_080cd52c/Func_080030f8 calling shapes are
 * shared with 080e01e4.c and documented there; the 512-slot sentinel-init
 * loop at 0x02010018, the final 256-slot particle-scan draw (idx = (lifetime
 * >>4)+2, half = (idx+(idx>>31))>>1, src = extra_target+Data_080ede48[idx-1])
 * and the two-callback-pair via a retained callback_ptr are byte-identical
 * in shape to 080e01e4's equivalents, just with this owner's own constants.
 *
 * New to this owner (not present in 080e01e4 or the template): a 5-of-8
 * capacity work+0x7080 "orbit ring" record array (28-byte stride, fields at
 * +0 x, +4 y, +12 field12, +24 state) that is drawn and aged every frame
 * while active (state != -1) and, five frames after activation (state==5),
 * spawns a fresh 32-particle burst into the shared work+0x02010000 particle
 * pool at record_index*0x380; and an activation search driven by an 8-bit
 * table (Data_080eec5a) of trigger frame numbers, where slot 0 has its own
 * specialized activation formula (compiled separately from slots 1..4, which
 * share a small search loop) and slot-index*7-plus-6 additionally cues
 * Func_080d6888/Func_080b5088 for every real party member.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, u32 w, s32 h);

void Func_080cd594(s32 mode);
void Func_080cef64(s32 flag, DrawRectangleFn *out_callbacks);
void Func_080e0524(s32 effect_id, void *target, s32 flag_a, s32 flag_b);
void Func_080dfddc(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
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
extern const u8 Data_080eec5a[];
extern u8 Value_00000073;
extern u8 Value_00000092;
extern u8 Value_0000006f;

void Func_080dfe2c(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *extra_target;
    DrawRectangleFn callbacks[2];
    DrawRectangleFn *callback_ptr;
    s32 *slot;
    s32 n;
    s32 record_index;
    u8 *record;
    s32 x_base;
    s32 y_base;
    s32 particle_offset;
    u8 *particle;
    s32 frame;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    extra_target = heap_cache[2];
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(0);
    callback_ptr = callbacks;
    Func_080cef64(0, callback_ptr);
    Func_080e0524((s32)&Value_00000073, extra_target, 0, 0);
    Func_080e0524((s32)&Value_00000092, work, 1, 0);
    Func_080e0524((s32)&Value_0000006f, (void *)0x02010000, 1, 1);
    Func_080dfddc(0x02010000, (s32)((u8 *)work + 0x2A8), 17, 104);
    Func_080dfddc(0x020106E8, (s32)((u8 *)work + 0x990), 34, 65);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Func_080041d8((void *)0x080CD261, 0x480);

    slot = (s32 *)((u8 *)work + 0x7098);
    n = 0;
    do {
        n++;
        *slot = -1;
        slot += 7;
    } while (n != 8);

    slot = (s32 *)0x02010018;
    n = 0;
    do {
        n++;
        *slot = -1;
        slot += 7;
    } while (n != 512);

    Func_080f9010(162);
    frame = 0;
    do {
        if (frame == 56) {
            Func_080b50e8(133);
        }

        record = (u8 *)work + 0x7080;
        record_index = 0;
        particle_offset = 0;
        do {
            if (M2C_FIELD(record, s32 *, 24) != -1) {
                callback_ptr[0](
                    draw_destination, (u8 *)work + 0x990,
                    M2C_FIELD(record, s32 *, 0) - 16,
                    M2C_FIELD(record, s32 *, 4) - 17, 65, 34);
                M2C_FIELD(record, s32 *, 0) =
                    M2C_FIELD(record, s32 *, 0) - 12;
                M2C_FIELD(record, s32 *, 24) =
                    M2C_FIELD(record, s32 *, 24) + 1;
                if (M2C_FIELD(record, s32 *, 24) == 5) {
                    Func_080f9010(133);
                    M2C_FIELD(work, s32 *, 0x77A8) = 4;
                    particle = (u8 *)0x02010000 + particle_offset;
                    n = 0;
                    do {
                        u32 kind;
                        s32 angle;

                        kind = Func_08004458() & 0xFFFF;
                        angle = (s32)(Func_08004458() & 0x1FF) + 256;
                        M2C_FIELD(particle, s32 *, 0) =
                            M2C_FIELD(record, s32 *, 0) << 16;
                        M2C_FIELD(particle, s32 *, 4) =
                            M2C_FIELD(record, s32 *, 4) << 16;
                        M2C_FIELD(particle, s32 *, 12) =
                            (angle * Func_08002322((s32)kind)) >> 8;
                        M2C_FIELD(particle, s32 *, 16) =
                            (angle * Func_0800231c((s32)kind)) >> 7;
                        M2C_FIELD(particle, s32 *, 24) =
                            (s32)((Func_08004458() & 15) + 32);
                        particle += 28;
                        n++;
                    } while (n != 32);
                }
            }
            record_index++;
            record += 28;
            particle_offset += 0x380;
        } while (record_index != 5);

        if (frame <= 95) {
            s32 angle;
            s32 scale;
            s32 sin_val;
            s32 cos_val;
            s32 dx;
            s32 dy;

            angle = frame << 11;
            sin_val = Func_08002322(angle);
            scale = 64 - frame * 2;
            dx = (sin_val * scale) >> 17;
            x_base = dx + 96;
            cos_val = Func_0800231c(angle);
            dy = (cos_val * scale) >> 16;
            y_base = dy + 60;
            callback_ptr[1](
                draw_destination, work, dx + 86, dy + 43, 20, 34);

            record = (u8 *)work + 0x7080;
            if (frame == Data_080eec5a[0] &&
                    M2C_FIELD(record, s32 *, 24) == -1) {
                M2C_FIELD(record, s32 *, 0) = dx + 88;
                M2C_FIELD(record, s32 *, 4) = y_base;
                M2C_FIELD(record, s32 *, 12) = x_base;
                M2C_FIELD(record, s32 *, 24) = 0;
            } else {
                s32 index;

                index = 0;
                for (;;) {
                    if (frame == Data_080eec5a[index] + 6) {
                        void **target_slot;
                        void *target;
                        s32 member_id_offset;
                        s32 idx2;
                        s32 count;

                        target_slot = (void **)((u8 *)work + 0x7828);
                        target = *target_slot;
                        count = M2C_FIELD(target, s32 *, 20);
                        if (count != 0) {
                            member_id_offset = 36;
                            idx2 = 0;
                            do {
                                s32 member_id;

                                target = *target_slot;
                                member_id = M2C_FIELD(
                                    target, s16 *, member_id_offset);
                                Func_080d6888(member_id, 7, 5, idx2, 6);
                                target = *target_slot;
                                member_id = M2C_FIELD(
                                    target, s16 *, member_id_offset);
                                Func_080b5088(member_id, 6);
                                target = *target_slot;
                                count = M2C_FIELD(target, s32 *, 20);
                                idx2++;
                                member_id_offset += 2;
                            } while (idx2 != count);
                        }
                    }
                    index++;
                    if (index == 5) {
                        break;
                    }
                    if (frame == Data_080eec5a[index]) {
                        record = (u8 *)work + (index * 7 * 4) + 0x7080;
                        if (M2C_FIELD(record, s32 *, 24) == -1) {
                            M2C_FIELD(record, s32 *, 0) = x_base - 8;
                            M2C_FIELD(record, s32 *, 12) = x_base;
                            M2C_FIELD(record, s32 *, 4) = y_base;
                            M2C_FIELD(record, s32 *, 24) = 0;
                            break;
                        }
                    }
                }
            }
        }

        particle = (u8 *)0x02010000;
        n = 0;
        do {
            if (M2C_FIELD(particle, s32 *, 24) != -1) {
                s32 lifetime;
                s32 idx;
                s32 h;
                s32 half;
                void *src;
                s32 x;
                s32 y;

                lifetime = M2C_FIELD(particle, s32 *, 24);
                idx = ((lifetime >= 0 ? lifetime : lifetime + 15) >> 4) + 2;
                h = idx << 1;
                src = (u8 *)extra_target +
                    *(const u16 *)((u8 *)Data_080ede48 + h - 2);
                half = (idx + ((u32)idx >> 31)) >> 1;
                x = M2C_FIELD(particle, s16 *, 2) - half;
                y = M2C_FIELD(particle, s16 *, 6) - idx;
                callback_ptr[1](draw_destination, src, x, y, idx, h);
                Func_080e3908(particle, 62, 0x2000);
                M2C_FIELD(particle, s32 *, 24) =
                    M2C_FIELD(particle, s32 *, 24) - 1;
            }
            n++;
            particle += 28;
        } while (n != 256);

        Func_080e155c(4, 4);
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);

        frame++;
    } while (frame != 96);

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}

#include "types.h"

/*
 * Draft for the battle-presentation sub-effect at 0x080d82b0.
 *
 * Assigned from the orbiting_particles/run.c compiler-family cluster
 * (template-main-08099160); like its siblings 080d59b0/080dc1ec/080e01e4
 * this owner's real callee set and constants match the 0x03001eec "battle
 * work" subsystem already partly recovered at games/gs1/recon/en/main/
 * 080e7404.c and 080d59b0.c, not the assigned template.  See those files
 * for the evidence behind the field/signature choices below.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*WordCopyFn)(void *dest, void *src, s32 size);
typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, u32 w, s32 h);

void Func_080cd594(s32 mode);
void Func_080e0524(s32 effect_id, void *target, s32 flag_a, s32 flag_b);
void *Func_08002f40(s32 id);
void Func_080072f0(void *dest, void *src, s32 size, WordCopyFn copier);
void Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void **Func_080b5098(s32 member_id);
s32 Func_080b5070(s32 member_id);
void Func_080e3944(void *src, void *dest);
u32 Func_08004458(void);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);
void Func_080b50e8(s32 id);
void Func_080f9010(s32 id);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
s32 Func_080022fc(s32 a, s32 b);
void Func_080072f4(
    s32 dest, s32 src, s32 x, s32 y, DrawRectangleFn callback, s32 w,
    s32 h);
void Func_080e3908(void *particle, s32 count, s32 flags);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);

extern const u16 Data_080ede48[];
extern const s32 Data_080ee9f8[];

s32 Func_080d82b0(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *extra_target;
    DrawRectangleFn draw_rectangle_fn;
    s32 facing;
    void *target;
    s32 *pool_cursor;
    s32 pool_index;
    s32 outer;
    s32 member;
    s32 member_offset;
    s32 member_id_offset;
    s32 particle_base;
    s32 result0;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    extra_target = heap_cache[2];
    facing = *(s32 *)((u8 *)heap_cache - 108);
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(1);
    Func_080e0524(0x73, extra_target, 0, 0);
    Func_080072f0(
        (void *)(160 << 19), Func_08002f40(0xB9), 128,
        (WordCopyFn)0x03001388);
    Func_080ed408(46, 7, 7, 3, 2);
    draw_rectangle_fn = *(DrawRectangleFn *)((u8 *)heap_cache + 28);

    pool_cursor = (s32 *)0x02010018;
    pool_index = 0;
    do {
        pool_index++;
        *pool_cursor = -1;
        pool_cursor += 7;
    } while (pool_index != 1024);

    Func_080049ac();
    Func_080051d8(facing, facing + 12);

    target = M2C_FIELD(work, void **, 0x7828);
    member = 0;
    if (M2C_FIELD(target, s32 *, 20) != 0) {
        s32 sp44[3];
        s32 sp32[2];
        s32 *sp44_ptr;
        s32 *sp32_ptr;

        sp44_ptr = sp44;
        sp32_ptr = sp32;
        member_id_offset = 36;
        member_offset = 0;
        do {
            void *member_ptr;
            s32 member_id;
            s32 i;
            s32 *particle;

            target = M2C_FIELD(work, void **, 0x7828);
            member_id = M2C_FIELD(target, s16 *, member_id_offset);
            member_ptr = *Func_080b5098(member_id);
            target = M2C_FIELD(work, void **, 0x7828);
            member_id = M2C_FIELD(target, s16 *, member_id_offset);
            result0 = Func_080b5070(member_id);
            result0 = (result0 + ((u32) result0 >> 31)) >> 1;
            sp44_ptr[0] = M2C_FIELD(member_ptr, s32 *, 8);
            sp44_ptr[1] = result0;
            sp44_ptr[2] = M2C_FIELD(member_ptr, s32 *, 16);
            Func_080e3944(sp44_ptr, sp32_ptr);
            sp32_ptr[0] = sp32_ptr[0] >> 1;

            particle_base = member_offset;
            particle = (s32 *)((u8 *)0x02010000 + particle_base);
            for (i = 0; i != 128; i++) {
                u32 kind;
                u32 angle;
                s32 sin_val;
                s32 cos_val;

                kind = Func_08004458() & 0xFF;
                angle = Func_08004458() & 0xFFFF;
                sin_val = Func_08002322((s32) angle);
                *(s32 *)particle =
                    (s32) (((s32) (kind * sin_val) >> 7)
                        + (sp32_ptr[0] << 16));
                cos_val = Func_0800231c((s32) angle);
                *(s32 *)((u8 *)particle + 4) =
                    (s32) (((s32) (kind * cos_val) >> 3)
                        + (sp32_ptr[1] << 16));
                *(s32 *)((u8 *)particle + 12) =
                    (s32) ((128 - (s32) (Func_08004458() & 0xFF)) << 9);
                *(s32 *)((u8 *)particle + 24) = 0;
                *(s32 *)((u8 *)particle + 16) =
                    (s32) ((-(s32) (Func_08004458() & 0xFF) - 128) << 10);
                particle = (s32 *)((u8 *)particle + 28);
            }

            member_id_offset += 2;
            member_offset += 0xE00;
            target = M2C_FIELD(work, void **, 0x7828);
            member++;
        } while (member != M2C_FIELD(target, s32 *, 20));
    }

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 50;
    Func_080041d8((void *)0x080CD261, 0x480);

    target = M2C_FIELD(work, void **, 0x7828);
    if (M2C_FIELD(target, s32 *, 20) * 20 != -56) {
        outer = 0;
        do {
            s32 pass_count;

            if (outer == 32) {
                Func_080b50e8(0);
            }
            target = M2C_FIELD(work, void **, 0x7828);
            pass_count = M2C_FIELD(target, s32 *, 20);
            member = 0;
            if (pass_count != 0) {
                s32 stagger;
                s32 offset;

                stagger = 0;
                offset = 0;
                do {
                    if (outer == stagger) {
                        s32 member_id;

                        Func_080f9010(143);
                        target = M2C_FIELD(work, void **, 0x7828);
                        member_id = M2C_FIELD(target, s16 *, 36 + member * 2);
                        Func_080d6888(member_id, 7, -1, member, 20);
                    }
                    if (outer > stagger) {
                        s32 k;
                        s32 *particle;

                        particle = (s32 *)((u8 *)0x02010000 + offset);
                        for (k = 0; k != 128; k++) {
                            if (*(s32 *)((u8 *)particle + 24) >= 0) {
                                s32 raw;
                                s32 idx;
                                s32 half;
                                s32 y;
                                s32 h;

                                raw = Func_080022fc(k, 3);
                                idx = raw + 1;
                                half = (idx + ((u32) idx >> 31)) >> 1;
                                y = *(s16 *)((u8 *)particle + 2) - half;
                                h = *(s16 *)((u8 *)particle + 6) - idx;
                                Func_080072f4(
                                    (s32) draw_destination,
                                    (s32) ((u8 *) extra_target
                                        + Data_080ede48[raw]),
                                    y, h, draw_rectangle_fn, idx, idx * 2);
                                Func_080e3908(
                                    particle, 62,
                                    Data_080ee9f8[k & 3]);
                                *(s32 *)((u8 *)particle + 24) += 1;
                                if (*(s32 *)((u8 *)particle + 16) > 0
                                    && *(s16 *)((u8 *)particle + 6) > 112) {
                                    *(s32 *)((u8 *)particle + 24) = -1;
                                }
                            }
                            particle = (s32 *)((u8 *)particle + 28);
                        }
                    }
                    member++;
                    stagger += 20;
                    offset += 0xE00;
                    target = M2C_FIELD(work, void **, 0x7828);
                } while (member != M2C_FIELD(target, s32 *, 20));
            }

            Func_080cd52c();
            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);

            target = M2C_FIELD(work, void **, 0x7828);
            outer++;
        } while (outer != M2C_FIELD(target, s32 *, 20) * 20 + 56);
    }

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(46);
    return Func_080cdbc0();
}

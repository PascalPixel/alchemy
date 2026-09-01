#include "types.h"

/*
 * Battle-presentation sub-effect at 0x080d85d0, from the 0x03001eec
 * "battle work" subsystem compiler family (template-main-080ce85c /
 * template-main-08099160), already partly recovered at
 * games/gs1/recon/en/main/080e7404.c, 080d59b0.c, 080d82b0.c, 080dc1ec.c and
 * 080e01e4.c.  This owner does not match its assigned template's shape
 * (888 bytes here vs. 724 there): it adds a member-scaled particle-cloud
 * pool at 0x02010000 that is both initialised once (128 particles per
 * party member, positioned at that member's rest coordinates with a random
 * outward kick) and then, once per displayed frame, drawn and pulled toward
 * a single currently-tracked member (state offset 8) with a staggered
 * per-member release gate.  Field offsets, the Value_/Data_03001e50
 * absolute-symbol conventions, and the 0x02010018/0x02010000 particle-pool
 * reset idiom are the same evidence already recorded for the sibling owners
 * above; see those files for the supporting citations.
 *
 * Per games/gs1/recon/en/dossiers.json#main:080d82b0's evidence, the work+0x7828
 * state pointer must never be materialized into its own named local (even
 * reused verbatim at every site): this compiler's CSE hoists the address
 * across the whole enclosing block and produces a spurious spill/reload the
 * reference does not have.  The STATE macro below re-expands the field
 * access textually at every use instead.
 *
 * The (dx >> 8) and (dz >> 8) range test against [-0xFFF, 0xFFF] is GCC's
 * standard unsigned-bias fold of two signed range comparisons into one
 * `(unsigned)(v + 0xFFF) > 0x1FFE`; it is spelled here as the ordinary
 * bounded comparison and left for the compiler to fold, not hand-written as
 * the bias arithmetic.  The loop-count guards that compare a scaled member
 * count against a negative bias (`* 20 != -72`) rather than testing the
 * unscaled count are kept in that literal, unsimplified form to match
 * 080d82b0.c's confirmed reference shape for the same idiom.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))
#define STATE M2C_FIELD(work, void **, 0x7828)

typedef void (*WordCopyFn)(void *dest, void *src, s32 size);
typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, s32 width, s32 height);

void Func_080cd594(s32 mode);
void Func_080e0524(s32 effect_id, void *target, s32 flag_a, s32 flag_b);
void *Func_08002f40(s32 id);
void Func_080072f0(void *dest, void *src, s32 size, WordCopyFn copier);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void **Func_080b5098(s32 member_id);
s32 Func_080b5070(s32 member_id);
u32 Func_08004458(void);
void Func_080e3944(void *src, void *dest);
void Func_080e38b8(void *particle, s32 a, s32 b);
void Func_080f9010(s32 id);
void Func_080b50e8(s32 id);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080030f8(s32 frames);
void Func_080cd52c(void);

extern u8 Value_00000073;
extern u8 Value_000000b9;
extern u8 Value_000000c0;
extern void *Data_03001e50[];
extern const u16 Data_080ede48[];

s32 Func_080d85d0(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *extra_target;
    s32 facing;
    s32 variant;
    void *palette;
    DrawRectangleFn draw_rectangle_fn;
    s32 *pool_cursor;
    s32 pool_index;
    s32 member;
    s32 member_id_offset;
    s32 particle_offset;
    s32 fp;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    extra_target = heap_cache[2];
    facing = *(s32 *)((u8 *)heap_cache - 108);
    variant = M2C_FIELD(object, s32 *, 0x18) != 0;
    STATE = object;

    Func_080cd594(1);
    Func_080e0524((s32) &Value_00000073, extra_target, 0, 0);
    if (variant) {
        palette = Func_08002f40((s32) &Value_000000c0);
    } else {
        palette = Func_08002f40((s32) &Value_000000b9);
    }
    Func_080072f0((void *)(160 << 19), palette, 128, (WordCopyFn)0x03001388);

    pool_cursor = (s32 *)0x02010018;
    pool_index = 0;
    do {
        pool_index++;
        *pool_cursor = -1;
        pool_cursor += 7;
    } while (pool_index != 1024);

    if (M2C_FIELD(STATE, s32 *, 20) != 0) {
        s32 *particle;
        void *member_ptr;
        s32 member_id;
        s32 half;
        s32 i;

        member_id_offset = 36;
        particle_offset = 0;
        member = 0;
        do {
            member_id = M2C_FIELD(STATE, s16 *, member_id_offset);
            member_ptr = *Func_080b5098(member_id);
            member_id = M2C_FIELD(STATE, s16 *, member_id_offset);
            half = Func_080b5070(member_id);
            half = half / 2;

            particle = (s32 *)((u8 *)0x02010000 + particle_offset);
            for (i = 0; i != 128; i++) {
                particle[0] = M2C_FIELD(member_ptr, s32 *, 8);
                particle[1] = M2C_FIELD(member_ptr, s32 *, 12) + half;
                particle[2] = M2C_FIELD(member_ptr, s32 *, 16);
                particle[3] = (s32) (((Func_08004458() & 0xFF) - 128) << 10);
                particle[4] = (s32) (((Func_08004458() & 0xFF) - 128) << 10);
                particle[5] = (s32) (((Func_08004458() & 0xFF) - 128) << 10);
                particle[6] = 0;
                particle = (s32 *)((u8 *)particle + 28);
            }

            member_id_offset += 2;
            particle_offset += 0xE00;
            member++;
        } while (member != M2C_FIELD(STATE, s32 *, 20));
    }

    Func_080ed408(46, 7, 7, 3, 2);
    draw_rectangle_fn = (DrawRectangleFn) Data_03001e50[46];
    M2C_FIELD(work, s32 *, 0x7780) = 3;
    M2C_FIELD(work, s32 *, 0x7784) = 0x04040404;
    Func_080041d8((void *)0x080CD261, 0x480);
    Func_080f9010(142);

    if (M2C_FIELD(STATE, s32 *, 20) * 20 != -72) {
        fp = 0;
        do {
            void *member_ptr;
            s32 member_id;
            s32 half;

            member_id = M2C_FIELD(STATE, s32 *, 8);
            member_ptr = *Func_080b5098(member_id);
            member_id = M2C_FIELD(STATE, s32 *, 8);
            half = Func_080b5070(member_id);
            half = half / 2;

            if (fp == 64) {
                Func_080b50e8(133);
            }
            Func_080049ac();
            Func_080051d8(facing, facing + 12);
            if (fp == 40) {
                Func_080d6888(member_id, 7, -1, -1, 0);
            }
            if (fp == M2C_FIELD(STATE, s32 *, 20) * 20 + 52) {
                Func_080d6888(member_id, 0, -1, -1, 0);
            }

            if (M2C_FIELD(STATE, s32 *, 20) != 0) {
                s32 stagger;
                s32 draw_offset;

                member = 0;
                stagger = 0;
                draw_offset = 0;
                do {
                    if (fp == stagger) {
                        s32 trigger_id;

                        trigger_id = M2C_FIELD(STATE, s16 *, 36 + member * 2);
                        Func_080d6888(trigger_id, 7, 5, member, 42);
                    }
                    if (fp > stagger) {
                        s32 *particle;
                        s32 k;

                        particle = (s32 *)((u8 *)0x02010000 + draw_offset);
                        for (k = 0; k != 32; k++) {
                            if (particle[6] >= 0) {
                                s32 sp52[2];

                                Func_080e3944(particle, sp52);
                                sp52[0] = sp52[0] >> 1;
                                draw_rectangle_fn(
                                    draw_destination,
                                    (u8 *) extra_target + Data_080ede48[5],
                                    sp52[0] - 3, sp52[1] - 6, 6, 12);
                                Func_080e38b8(particle, 62, 0);
                                if (fp > stagger + k + 10) {
                                    s32 dx;
                                    s32 dy;
                                    s32 dz;
                                    s32 dxs;
                                    s32 dzs;

                                    dx = M2C_FIELD(member_ptr, s32 *, 8)
                                        - particle[0];
                                    dy = (M2C_FIELD(member_ptr, s32 *, 12)
                                            + half)
                                        - particle[1];
                                    dz = M2C_FIELD(member_ptr, s32 *, 16)
                                        - particle[2];
                                    dxs = dx >> 8;
                                    dzs = dz >> 8;
                                    particle[3] += dxs;
                                    particle[4] += dy >> 8;
                                    particle[5] += dzs;
                                    if (dxs >= -0xFFF && dxs <= 0xFFF) {
                                        if (dzs >= -0xFFF && dzs <= 0xFFF) {
                                            particle[6] = -1;
                                        }
                                    }
                                }
                            }
                            particle = (s32 *)((u8 *)particle + 28);
                        }
                    }
                    member++;
                    stagger += 20;
                    draw_offset += 0xE00;
                } while (member != M2C_FIELD(STATE, s32 *, 20));
            }

            Func_080cd52c();
            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);

            fp++;
        } while (fp != M2C_FIELD(STATE, s32 *, 20) * 20 + 72);
    }

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(46);
    return Func_080cdbc0();
}

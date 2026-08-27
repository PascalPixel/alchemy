#include "types.h"

/*
 * Battle-presentation sub-effect at 0x080ca1fc, part of the 0x03001eec
 * "battle work" subsystem already partly recovered at games/gs1/recon/en/
 * main/080e7404.c, 080d82b0.c and games/gs1/src/battle/effects/
 * member_orbit/run.c.  Unlike member_orbit (single object argument), this
 * owner takes a second explicit `mode` argument that both selects the
 * Func_08002f40 palette resource (via the established Value_ literal-pool
 * trick) and gates several halving/branch decisions throughout the frame
 * loop.
 *
 * Semantic summary: opens display kinds 46 and 47 (draw-rectangle blit
 * routines cached in Data_03001e50[]), copies a palette through the
 * generic word-copy helper (Func_080072f0 taking the 0x03001388 word-copy
 * routine as a trailing callback argument -- Func_080072f0 is the r3 slot
 * of the _call_via_rN trampoline at games/gs1/asm/080072e4.s, modeled as a
 * direct call with the real callee as a trailing argument per that
 * trampoline's established convention), seeds a 256-slot particle pool at
 * 0x02010000 from one party member's position, then runs 128 frames.
 * Each frame redraws the first 128 particles whose staggered reveal
 * window is open and whose phase field is non-negative, projecting each
 * through Func_080e3944 and blitting through the kind-46 routine (also a
 * genuinely traced function pointer, the r4 slot of the same trampoline).
 * Mode 1 additionally nudges each drawn particle vertically by +-8192 and
 * drives Func_080d6888 portrait callouts differently than mode 0.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*WordCopyFn)(void *dest, void *src, s32 size);
typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

extern void *Data_03001e50[];
extern const u16 Data_080ede48[];
extern u8 Value_00000073;
extern u8 Value_0000007b;
extern u8 Value_0000007c;

void Func_080cd594(s32 mode);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080e0524(s32 effect_id, void *target, s32 flag_a, s32 flag_b);
void *Func_08002f40(s32 id);
void Func_080072f0(void *dest, void *src, s32 size, WordCopyFn copier);
void Func_080041d8(void *callback, s32 interval);
void **Func_080b5098(s32 member_id);
u32 Func_08004458(void);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void Func_080e3944(const void *source, void *screen);
void Func_080e38b8(void *particle, s32 a, s32 b);
void Func_080b50e8(s32 id);
void Func_080f9010(s32 id);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

void Func_080ca1fc(void *object, s32 mode)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *extra_target;
    void * volatile second_blit_kind;
    void *member_object;
    DrawRectangleFn draw_rectangle_fn;
    s32 facing;
    s32 look_target;
    s32 status;
    s32 outer;
    s32 i;
    s32 j;
    s32 member;
    s32 member_id_offset;
    void *particle;
    s32 position[3];
    s32 screen[3];

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    facing = *(s32 *)((u8 *)heap_cache - 108);
    extra_target = heap_cache[2];
    M2C_FIELD(work, void **, 0x7828) = object;

    if (mode == 0) {
        Func_080cd594(0);
    } else {
        Func_080cd594(1);
    }

    status = Func_080ed408(46, 7, 7, 3, 2);
    draw_rectangle_fn = (DrawRectangleFn)Data_03001e50[46];
    status = Func_080ed408(47, 7, 7, 11, 2);
    second_blit_kind = Data_03001e50[47];

    Func_080e0524((s32)&Value_00000073, extra_target, 0, 0);

    Func_080072f0((void *)(160 << 19),
        Func_08002f40(mode == 0 ? (s32)&Value_0000007c : (s32)&Value_0000007b),
        128, (WordCopyFn)0x03001388);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Func_080041d8((void *)0x080CD261, 0x480);

    member_object = *Func_080b5098(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 8));

    particle = (void *)0x02010000;
    i = 0;
    do {
        u32 rand1;
        u32 angle;
        s32 radius;
        s32 sin_val;
        s32 cos_val;

        rand1 = Func_08004458() & 0x3FF;
        angle = Func_08004458() & 0xFFFF;
        M2C_FIELD(particle, s32 *, 0) = M2C_FIELD(member_object, s32 *, 8);
        M2C_FIELD(particle, s32 *, 4) =
            M2C_FIELD(member_object, s32 *, 12) + 0x50000;
        M2C_FIELD(particle, s32 *, 8) = M2C_FIELD(member_object, s32 *, 16);
        sin_val = Func_08002322((s32)angle);
        radius = rand1 + 32;
        M2C_FIELD(particle, s32 *, 0xC) = (sin_val * radius) >> 8;
        M2C_FIELD(particle, s32 *, 0x10) =
            ((s32)(Func_08004458() & 0xFF) - 32) << 9;
        cos_val = Func_0800231c((s32)angle);
        M2C_FIELD(particle, s32 *, 0x14) = -(cos_val * radius * 2) >> 8;
        M2C_FIELD(particle, s32 *, 0x18) =
            (s32)(Func_08004458() & 0x1F) + 48;
        if (mode == 0) {
            M2C_FIELD(particle, s32 *, 0xC) =
                M2C_FIELD(particle, s32 *, 0xC) / 2;
            M2C_FIELD(particle, s32 *, 0x14) =
                M2C_FIELD(particle, s32 *, 0x14) / 2;
        }
        i++;
        particle = (u8 *)particle + 28;
    } while (i != 256);

    look_target = facing + 12;
    outer = 0;
    do {
        Func_080049ac();
        Func_080051d8(facing, look_target);

        particle = (void *)0x02010000;
        j = 0;
        do {
            s32 threshold;

            threshold = (j / 32) * 8;
            if (outer >= threshold) {
                s32 phase;

                phase = M2C_FIELD(particle, s32 *, 0x18);
                if (phase >= 0) {
                    s32 z;
                    s32 offset;
                    s32 idx;
                    s32 doubled;
                    s32 half;
                    void *src_addr;
                    s32 x;
                    s32 y;

                    position[0] = M2C_FIELD(particle, s32 *, 0)
                        + (Func_08002322(((j * 4) + phase) << 10) << 4);
                    position[1] = M2C_FIELD(particle, s32 *, 4);
                    position[2] = M2C_FIELD(particle, s32 *, 8);
                    Func_080e3944(position, screen);

                    screen[0] = screen[0] >> 1;
                    z = screen[2];
                    if (z <= 313) {
                        z = 314;
                        screen[2] = 314;
                    }
                    if (z > 634) {
                        screen[2] = 634;
                        z = 634;
                    }

                    offset = z - 314;
                    if (offset < 0) {
                        offset = z - 251;
                    }
                    idx = 6 - (offset >> 6);
                    doubled = idx * 2;
                    src_addr = (u8 *)extra_target + Data_080ede48[idx - 1];
                    half = idx / 2;
                    x = screen[0] - half;
                    y = screen[1] - idx;
                    draw_rectangle_fn(draw_destination, src_addr, x, y,
                        idx, doubled);

                    Func_080e38b8(particle, 62, 1024);

                    if (mode == 1) {
                        if (M2C_FIELD(member_object, s32 *, 8) < 0) {
                            M2C_FIELD(particle, s32 *, 0xC) =
                                M2C_FIELD(particle, s32 *, 0xC) + 8192;
                        } else {
                            M2C_FIELD(particle, s32 *, 0xC) =
                                M2C_FIELD(particle, s32 *, 0xC) - 8192;
                        }
                    }

                    M2C_FIELD(particle, s32 *, 0x18) = phase - 1;
                }
            }
            j++;
            particle = (u8 *)particle + 28;
        } while (j != 128);

        if (mode == 1) {
            member = 0;
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20)
                    != 0) {
                s32 stagger;

                member_id_offset = 36;
                stagger = 48;
                do {
                    if (outer == stagger) {
                        s32 member_id;

                        Func_080b50e8(-1);
                        member_id = M2C_FIELD(
                            M2C_FIELD(work, void **, 0x7828), s16 *,
                            member_id_offset);
                        Func_080d6888(member_id, 7, 5, member, 8);
                    }
                    member++;
                    member_id_offset += 2;
                    stagger += 8;
                } while (member
                    != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *,
                        20));
            }
        } else {
            member = 0;
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20)
                    != 0) {
                s32 stagger;

                member_id_offset = 36;
                stagger = 48;
                do {
                    if (outer == stagger) {
                        s32 member_id;

                        Func_080f9010(126);
                        Func_080b50e8(-1);
                        member_id = M2C_FIELD(
                            M2C_FIELD(work, void **, 0x7828), s16 *,
                            member_id_offset);
                        Func_080d6888(member_id, 7, -1, member, 8);
                    }
                    member++;
                    member_id_offset += 2;
                    stagger += 8;
                } while (member
                    != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *,
                        20));
            }
        }

        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);

        outer++;
    } while (outer != 128);

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}

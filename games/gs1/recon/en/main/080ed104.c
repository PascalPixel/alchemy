#include "types.h"

/*
 * Draft for the battle-presentation sub-effect at 0x080ed104.
 *
 * The family matcher assigned games/gs1/src/battle/effects/member_orbit/run.c
 * (owner 0x080ce85c, already adopted) as the closest structural template, but
 * this owner's real callee set and constants instead match the already-drafted
 * 0x03001eec "battle work" subsystem documented in games/gs1/recon/en/main/
 * 080e7404.c, 080d59b0.c, 080d82b0.c, 080dc1ec.c and 080e01e4.c: identical
 * Func_080cd594(mode)/Func_080e0524(id,work,f,f)/Func_080041d8(cb,interval)/
 * Func_08004278(cb)/Func_08002dd8(id)/Func_080cdbc0() call shapes and the same
 * work-struct field offsets (0x7780/0x7784/0x77A8/0x7824/0x7828) relative to
 * the shared *(void **)0x03001EEC base. Unlike its siblings this owner does
 * not walk the active member list at all (field 0x14/20 is never read) and
 * instead runs a fixed 54-frame loop that fills a horizontal progress-bar
 * region, using only the fixed first member slot (object+0x24) twice.
 *
 * Two call sites in the retained assembly are `bl` targets that land inside
 * games/gs1/asm/080072e4.s, the fourteen-slot `call_via_rN` trampoline bundle
 * (`bx rN; mov r8,r8` at 0x080072e4+4*N for N=r0..sp): 0x08007314 is exactly
 * the ip (r12) slot (N=12) and 0x080072f4 is exactly the r4 slot (N=4). Both
 * are therefore indirect calls through a genuinely traced function-pointer
 * value, not calls to real functions named "Func_08007314"/"Func_080072f4" --
 * see games/gs1/recon/en/dossiers.json#main:080dc1ec's score.note for the full
 * cross-family derivation (arm_function_arg proof, r4-value provenance
 * trace, and the already-adopted member_orbit/run.c's identical
 * `((DrawRectangleFn)rectangle_slot[slot & 1])(...)` idiom). The ip-slot call
 * loads its target from *(void **)0x03001F08, which is exactly
 * heap_cache[7] (0x03001EEC + 7*4); the r4-slot call loads its target from
 * the two-word out_callbacks array most recently filled by Func_080cef64,
 * matching 080dc1ec.c's `callback_pair[0]` idiom.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, u32 w, s32 h);

void Func_080cd594(s32 mode);
void Func_080e0524(s32 effect_id, void *target, s32 flag_a, s32 flag_b);
void Func_080cef64(s32 flag, DrawRectangleFn *out_callbacks);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_080e3980(s32 value, s32 *out);
u32 Func_08004458(void);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080b50e8(s32 id);
void Func_080b5088(s32 member_id, s32 flag);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
s32 Func_080022ec(s32 numerator, s32 denominator);
void Func_080e3908(void *particle, s32 count, s32 flags);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

extern u8 Value_00000073;
extern u8 Value_00000051;
extern u8 Value_000000c0;
extern const u16 Data_080eef88[];
extern const u16 Data_080eef96[];

void Func_080ed104(void *object)
{
    void *work;
    void *draw_destination;
    void *extra_target;
    void **heap_cache;
    void **cursor;
    void **object_slot;
    DrawRectangleFn rect_fns[2];
    u8 *star;
    s32 flag;
    s32 screen[3];
    s32 star_index;
    s32 frame;
    s32 bar_progress;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    M2C_FIELD(work, void **, 0x7828) = object;
    extra_target = heap_cache[2];
    Func_080cd594(0);
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x100;
    Func_080e0524((s32) &Value_00000073, extra_target, 0, 0);
    Func_080e0524((s32) &Value_00000051, work, 1, 1);
    Func_080e0524((s32) &Value_000000c0, (u8 *)work + 0x460, 1, 0);

    flag = M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4);
    Func_080cef64(flag, rect_fns);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Func_080041d8((void *)0x080CD261, 0x480);

    Func_080e3980(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 0x24),
        screen);
    *(s32 *)0x04000028 = (0x40 - screen[0]) << 8;

    star = (u8 *)work + 0x7080;
    for (star_index = 0; star_index != 16; star_index++) {
        s32 seed_a;
        s32 seed_b;
        s32 magnitude;

        seed_a = 0x1FF & Func_08004458();
        seed_b = 0xFFFF & Func_08004458();
        M2C_FIELD(star, s32 *, 0) = 0x400000;
        M2C_FIELD(star, s32 *, 4) = 0x700000;
        magnitude = seed_a + 0x80;
        M2C_FIELD(star, s32 *, 0xC) = (magnitude * Func_08002322(seed_b)) >> 8;
        M2C_FIELD(star, s32 *, 0x10) = (magnitude * Func_0800231c(seed_b)) >> 9;
        M2C_FIELD(star, s32 *, 0x18) = 7 & Func_08004458();
        star += 0x1C;
    }

    M2C_FIELD(work, s32 *, 0x77A8) = 8;
    object_slot = (void **)((u8 *)work + 0x7828);
    bar_progress = 0;

    for (frame = 0; frame != 54; frame++) {
        if (frame == 10) {
            M2C_FIELD(work, s32 *, 0x77A8) = 8;
            Func_080b50e8(212);
            Func_080b5088(M2C_FIELD(*object_slot, s16 *, 0x24), 0);
            Func_080d6888(M2C_FIELD(*object_slot, s16 *, 0x24), 7, 5, 0, 8);
        }

        if (frame > 7) {
            s32 bar_height;

            if (frame <= 31) {
                bar_height = frame * 12 - 96;
            } else {
                bar_height = 272 - bar_progress;
            }

            if (bar_height > 0) {
                s32 bar_style;
                s32 bar_x;
                s32 seg;

                if (bar_height > 80) {
                    bar_height = 80;
                    bar_style = 2;
                } else {
                    bar_style = 3;
                }

                bar_x = 50;
                for (seg = 0; seg != 2; seg++) {
                    if (seg == 0) {
                        Func_080ed408(46, 7, 7, 3, bar_style);
                    } else {
                        Func_080ed408(46, 7, 7, 7, bar_style);
                    }
                    ((DrawRectangleFn) heap_cache[7])(
                        draw_destination, work,
                        bar_x, 112 - bar_height, 14, bar_height);
                    Func_08002dd8(46);
                    bar_x += 14;
                }
            }
        }

        flag = M2C_FIELD(*object_slot, s32 *, 4);
        Func_080cef64(flag, rect_fns);

        star = (u8 *)work + 0x7080;
        for (star_index = 0; star_index != 16; star_index++) {
            if (frame >= star_index / 2 + 8) {
                s32 lifetime;

                lifetime = M2C_FIELD(star, s32 *, 0x18);
                if (lifetime <= 28) {
                    s32 bucket;
                    u32 raw;
                    s32 half;
                    s32 sy;
                    s32 sh;
                    void *src;

                    bucket = Func_080022ec(lifetime, 3);
                    sy = M2C_FIELD(star, s16 *, 2);
                    sh = M2C_FIELD(star, s16 *, 6);
                    if (bucket > 6) {
                        bucket = 6;
                    }
                    src = (u8 *)work + Data_080eef88[bucket] + 0x460;
                    raw = Data_080eef96[bucket];
                    half = raw >> 1;

                    ((DrawRectangleFn) rect_fns[0])(
                        draw_destination, src,
                        sy - half, sh - half,
                        raw, raw);

                    M2C_FIELD(star, s32 *, 0x18) =
                        M2C_FIELD(star, s32 *, 0x18) + 1;
                    Func_080e3908(star, 62, 0xFFFFE000);
                }
            }
            star += 0x1C;
        }

        Func_08002dd8(47);
        Func_08002dd8(46);
        if (frame <= 7) {
            Func_080e155c(2, 2);
        } else {
            Func_080e155c(16, 16);
        }
        Func_080cd52c();

        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);

        bar_progress += 6;
    }

    Func_08004278((void *)0x080CD261);
    Func_080cdbc0();
}

#include "types.h"

#define BattleEffect_RunSparkDescent Func_080de2f8

/*
 * Battle-presentation sub-effect at 0x080de2f8 (1660 bytes, one function).
 *
 * Confirmed member of the 0x03001eec "battle work" subsystem family already
 * recovered at games/gs1/recon/en/main/080ddde0.c (the immediately preceding
 * owner), 080dea70.c, 080ce4e8.c and 080d85d0.c: the same
 * heap_cache = (void **)0x03001EEC / cursor / work / draw_destination
 * prologue, the same M2C_FIELD(work, void **, 0x7828) = object republish,
 * the same Func_080cd594(0) / Func_080041d8(0x080CD261, 0x480) /
 * Func_08004278(0x080CD261) / Func_08002dd8(46) bracket, and the same
 * Data_03001e50[46] / Data_03001e50[47] blit-callback slots that
 * Func_080ed408(id, ...) installs and Func_08002dd8(id) releases.
 *
 * This owner is the callee 080dea70.c already declares as
 * `void Func_080de2f8(void *object, s32 a, s32 b, s32 c, s32 *out_x,
 * s32 *out_y)`; the two pointer parameters are read from the reference
 * frame at sp+140 / sp+144, which is exactly the fifth/sixth stack argument
 * of that declaration.
 *
 * Behaviour: a `kind`-parameterised descent effect.
 *   - `kind` selects one of four palette resources and, above 3, a longer
 *     84-frame run instead of 64 (kind is reduced by 4 in that case, so the
 *     palette choice is shared between the short and long variants).
 *   - 64 spark records are laid out at the shared 0x02010000 pool (stride
 *     28: x, y, z, vx, vy, vz, hit-flag) with a random heading, a random
 *     speed and a random start height; only the first 32 are ever stepped.
 *   - `anchor_kind` (the fourth parameter) picks the descent target point
 *     from the caster record, the target record or the origin; the run
 *     interpolates `base` towards it over frames 6..45 in forty steps.
 *   - Frames 54..69 draw the impact sprite and publish its position through
 *     *out_x / *out_y; frame 64 seeds a second 64-slot burst pool at
 *     work+0x7080 (stride 28: x.16 at +0, y.16 at +4, vx, vy, life) from
 *     that published position, which frames 64.. then draw and decay.
 *   - Frames 76.. fade the effect out through BLDALPHA (0x04000052).
 *
 * Every `Func_080072f0` / `Func_080072f4` / `Func_080072f8` call site is an
 * indirect call through the value the reference loads into r3/r4/r5
 * immediately before the `bl`, not a real callee: those addresses are
 * consecutive slots of the `_call_via_rN` trampoline bank at
 * games/gs1/asm/080072e4.s (r3 at +0x0c, r4 at +0x10, r5 at +0x14). They
 * are modelled here as ordinary calls through the cached draw callbacks,
 * through the IWRAM word copier at 0x03001388 and through the IWRAM fill
 * routine at 0x03000164 -- the same treatment 080ce4e8.c, 080cd594.c and
 * 080dea70.c already use.
 *
 * Small constants that the reference loads from its literal pool where a
 * `movs` immediate would have fit (0x94, 0x92, 0x8e, 0x90, 0x73, 0xa8) are
 * spelled `(s32)&Value_XXXXXXXX` per the established family precedent; that
 * is what makes GCC emit a pool word instead of an immediate.
 *
 * Uncertain / not established here:
 *   - the meaning of the work-relative words 0x77b4 / 0x77b8 / 0x7780 /
 *     0x7784 / 0x7824 is read off their use sites only.
 *   - Data_080ede48 is the shared per-step sprite-size halfword table
 *     already used by 080d82b0.c and 080ddde0.c; its contents were not
 *     decoded here.
 *   - the `switch (anchor_kind)` default arm leaves `target` unwritten in
 *     the reference too (the default label only materialises the frame
 *     address), so it is left uninitialised here rather than invented.
 *
 * Measured state (alchemy score --owner 080de2f8): candidate 1656 bytes vs
 * reference 1660, 573 differing halfwords, 510 wrong instructions,
 * topology=equal, matched_prefix=30.  Of the 273 substituted instructions in
 * the aligned diff, every single one carries the reference mnemonic and
 * differs only in register numbering; the remaining 50 inserted / 52 deleted
 * rows are scheduling reorderings of the same instructions.
 * Every reference branch, loop, switch arm, call and store is
 * present at the reference multiplicity; the four-byte shortfall is one
 * shared literal-pool word (the reference re-emits duplicates such as
 * 0x02010000 / 0x0000ffff / 0x080CD261 that this candidate's pool folds),
 * not dropped code.
 *
 * The residual is whole-function register allocation, not structure.  Five
 * source hypotheses were tried and measured against it:
 *   - reversing the declaration order of the five vec3 locals so `pos`
 *     lands at sp+48 and `base` at sp+96 (588 -> 583, kept);
 *   - reordering the scalar declarations so the spill slots come out in the
 *     reference order target_actor/total/extra_target/draw_cb/flash_cb at
 *     sp+16..sp+32 (583 -> 578, kept);
 *   - bracketing the two spark-despawn range tests so `fold_range_test`
 *     merges each pair into one biased unsigned compare, as the reference
 *     does for both (578 -> 573, topology different -> equal, kept);
 *   - hoisting the BLDALPHA value into an s32 local so the 0x1000 operand
 *     is loaded as a word rather than a halfword (578 -> 648, reverted);
 *   - swapping the operand order of the four `speed * sin/cos` products to
 *     move `speed` into the `muls` destination as the reference does
 *     (no change at all, reverted to the readable spelling).
 * What remains is a systematic register-numbering divergence that starts
 * one instruction after the matched prefix: the reference parks the spark
 * cursor in r7, the heading in r6 and the speed in r5, while this candidate
 * uses r6/r5/r7 and therefore needs one extra copy to hold `speed` across
 * the two trig calls.  Forcing that assignment from source would be
 * contrivance; this stays an honest draft.
 */

/* Only the m2c spellings this draft actually uses. */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))
#define STATE M2C_FIELD(work, void **, 0x7828)

typedef void (*WordCopyFn)(void *dest, const void *src, s32 words);
typedef void (*FillFn)(void *dest, s32 size);
typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

void Func_080cd594(s32 mode);
void *Func_08002f40(s32 resource_id);
u32 Func_08005340(const void *source, void *destination);
void Func_080e0524(s32 resource_id, void *destination, s32 a, s32 b);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void **Func_080b5098(s32 member_id);
u32 Func_08004458(void);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
s32 Func_080022ec(s32 numerator, s32 denominator);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
void Func_080f9010(s32 id);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void Func_08004cb4(s32 *vector);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080049e8(void);
void Func_08004c1c(s32 angle);
void Func_08004bd4(s32 angle);
void Func_08004c6c(s32 angle);
void Func_080e3944(void *source, s32 *out_vector);
void Func_08004a5c(void);
void Func_080e38b8(void *particle, s32 a, s32 b);
void Func_080e3908(void *particle, s32 a, s32 b);
void Func_080030f8(s32 frames);

extern u8 Value_0000008e;
extern u8 Value_00000090;
extern u8 Value_00000092;
extern u8 Value_00000094;
extern u8 Value_00000073;
extern u8 Value_000000a8;

extern void *Data_03001e50[];
extern const u16 Data_080ede48[]; /* shared per-step sprite-size table */

void BattleEffect_RunSparkDescent(
    void *object, s32 kind, s32 mode, s32 anchor_kind,
    s32 *out_x, s32 *out_y)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    DrawRectangleFn flash_cb;
    DrawRectangleFn draw_cb;
    void *extra_target;
    s32 total;
    void *target_actor;
    void *palette;
    void *caster;
    s32 palette_id;
    s32 facing;
    s32 frame;
    s32 i;
    s32 *p;
    s32 angle;
    s32 speed;
    s32 size;
    s32 dx;
    s32 dz;
    s32 base[3];
    s32 target[3];
    s32 step[3];
    s32 tmp[3];
    s32 pos[3];

    heap_cache = (void **) 0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    extra_target = heap_cache[2];
    STATE = object;
    Func_080cd594(0);

    M2C_FIELD(work, s32 *, 0x77b4) = 24;
    M2C_FIELD(work, s32 *, 0x77b8) = 0;

    if (kind > 3) {
        total = 84;
        kind -= 4;
    } else {
        total = 64;
    }

    switch (kind) {
    case 0:
        palette_id = (s32) &Value_00000094;
        break;
    case 1:
        palette_id = (s32) &Value_00000092;
        break;
    case 2:
        palette_id = (s32) &Value_0000008e;
        break;
    default:
        palette_id = (s32) &Value_00000090;
        break;
    }
    palette = Func_08002f40(palette_id);
    ((WordCopyFn) 0x03001388)((void *)(160 << 19), palette, 128);
    Func_08005340((s8 *)palette + 128, work);
    Func_080e0524((s32) &Value_00000073, extra_target, 0, 0);

    if (mode == 1) {
        Func_080ed408(46, 7, 7, 7, 3);
        Func_080ed408(47, 7, 7, 7, 2);
    } else {
        Func_080ed408(46, 7, 7, 3, 3);
        Func_080ed408(47, 7, 7, 3, 2);
    }
    flash_cb = (DrawRectangleFn) Data_03001e50[47];
    draw_cb = (DrawRectangleFn) Data_03001e50[46];

    caster = *Func_080b5098(M2C_FIELD(STATE, s32 *, 8));
    target_actor = *Func_080b5098(M2C_FIELD(STATE, s16 *, 36));

    /* ---- seed the shared 0x02010000 spark pool ---- */
    p = (s32 *) 0x02010000;
    i = 0;
    do {
        angle = (s32)(Func_08004458() & 0xffff);
        speed = (s32)(Func_08004458() & 0xff) + 128;
        p[0] = 0;
        p[1] = ((s32)(Func_08004458() & 31) + 20) << 16;
        p[2] = 0;
        p[3] = speed * Func_08002322(angle) >> 5;
        p[4] = 0;
        p[5] = speed * Func_0800231c(angle) >> 5;
        p[6] = 0;
        i++;
        p += 7;
    } while (i != 64);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Func_080041d8((void *) 0x080CD261, 144 << 3);

    base[0] = M2C_FIELD(caster, s32 *, 8);
    base[1] = 0;
    base[2] = M2C_FIELD(caster, s32 *, 16);

    switch (anchor_kind) {
    case 0:
        target[0] = M2C_FIELD(target_actor, s32 *, 8);
        target[1] = 240 << 14;
        target[2] = M2C_FIELD(target_actor, s32 *, 16);
        break;
    case 1:
        target[0] = M2C_FIELD(target_actor, s32 *, 8);
        target[1] = 240 << 14;
        target[2] = 0;
        break;
    case 2:
        target[0] = M2C_FIELD(caster, s32 *, 8);
        target[1] = 240 << 14;
        target[2] = M2C_FIELD(caster, s32 *, 16);
        break;
    case 3:
        target[0] = M2C_FIELD(caster, s32 *, 8);
        target[1] = 240 << 14;
        target[2] = 0;
        break;
    case 4:
        target[0] = 0;
        target[1] = 240 << 14;
        target[2] = 0;
        break;
    }

    step[0] = Func_080022ec(target[0] - base[0], 40);
    step[1] = Func_080022ec(target[1] - base[1], 40);
    step[2] = Func_080022ec(target[2] - base[2], 40);

    frame = 0;
    if (total != 0) {
        do {
            facing = *(s32 *) 0x03001E80;

            if (frame > 75) {
                *(u16 *) 0x04000052 =
                    ((s32) &Value_000000a8 - frame * 2) | 0x1000;
            }
            if (frame == 8) {
                Func_080f9010(212);
            }

            Func_080049ac();
            Func_080051d8(facing, facing + 12);

            if (frame >= 6 && frame <= 45) {
                base[0] += step[0];
                base[1] += step[1];
                base[2] += step[2];
            }
            Func_08004cb4(base);

            if (frame == 0) {
                Func_080d6888(M2C_FIELD(STATE, s32 *, 8), 7, -1, -1, 0);
            }
            if (frame == 24) {
                Func_080d6888(M2C_FIELD(STATE, s32 *, 8), 0, -1, -1, 0);
            }

            /* ---- step and draw the first 32 sparks ---- */
            p = (s32 *) 0x02010000;
            i = 0;
            do {
                if (frame >= i / 8 && p[6] == 0) {
                    Func_080049e8();
                    switch (i & 3) {
                    case 0:
                        Func_08004c1c(frame * (i * 32 + 256));
                        break;
                    case 1:
                        Func_08004bd4((-frame) * (i * 32 + 256));
                        break;
                    case 2:
                        Func_08004c6c((-frame) * (i * 32 + 256));
                        break;
                    case 3:
                        Func_08004bd4((-frame) * (i * 32 + 256));
                        Func_08004c6c((-frame) * (i * 32 + 256));
                        break;
                    }

                    Func_080e3944(p, pos);
                    pos[0] = pos[0] >> 1;
                    Func_08004a5c();
                    if (pos[2] <= 249) {
                        pos[2] = 250;
                    }
                    if (pos[2] > 634) {
                        pos[2] = 634;
                    }
                    size = 8 - (pos[2] - 250) / 64;
                    flash_cb(draw_destination,
                        (s8 *)extra_target + Data_080ede48[size - 1],
                        pos[0] - size / 2, pos[1] - size, size, size * 2);
                    Func_080e38b8(p, 60, 0);

                    if (frame >= i / 8 + 24) {
                        dx = (-p[0]) >> 7;
                        dz = (-p[2]) >> 7;
                        p[3] = p[3] + dx;
                        p[4] = p[4] + ((-p[1]) >> 7);
                        p[5] = p[5] + dz;
                        p[3] = p[3] * 62 / 64;
                        p[4] = p[4] * 62 / 64;
                        p[5] = p[5] * 62 / 64;
                        if ((dx > -2048 && dx < 2048)
                                && (dz > -2048 && dz < 2048)) {
                            p[6] = -1;
                        }
                    }
                }
                i++;
                p += 7;
            } while (i != 32);

            /* ---- the impact sprite, and its published anchor ---- */
            if (frame >= 54 && frame <= 69) {
                tmp[0] = Func_08002322(frame << 10) << 2;
                tmp[1] = 0;
                tmp[2] = 0;
                Func_080e3944(tmp, pos);
                *out_x = pos[0];
                *out_y = pos[1];
                pos[0] = pos[0] >> 1;
                draw_cb(draw_destination, work,
                    pos[0] - 10, pos[1] - 20, 20, 40);
            }

            /* ---- seed the burst pool once, at the impact frame ---- */
            if (frame == 64) {
                p = (s32 *)((s8 *)work + (225 << 7));
                i = 0;
                do {
                    angle = (s32)(Func_08004458() & 0xffff);
                    speed = (s32)(Func_08004458() & 0xff) + 128;
                    p[0] = *out_x << 15;
                    p[1] = *out_y << 16;
                    p[3] = speed * Func_08002322(angle) >> 6;
                    p[4] = speed * Func_0800231c(angle) >> 5;
                    p[6] = (s32)(Func_08004458() & 15) + 8;
                    i++;
                    p += 7;
                } while (i != 64);
            }

            /* ---- draw and decay the burst ---- */
            if (frame > 63) {
                p = (s32 *)((s8 *)work + (225 << 7));
                i = 0;
                do {
                    if (p[6] >= 0) {
                        size = (p[6] >> 3) + 2;
                        draw_cb(draw_destination,
                            (s8 *)extra_target + Data_080ede48[size - 1],
                            M2C_FIELD(p, s16 *, 2) - size / 2,
                            M2C_FIELD(p, s16 *, 6) - size,
                            size, size * 2);
                        Func_080e3908(p, 60, 0);
                        p[6] = p[6] - 1;
                    }
                    i++;
                    p += 7;
                } while (i != 64);
            }

            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);
            frame++;
        } while (frame != total);
    }

    Func_08004278((void *) 0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_08004278((void *) 0x080CD4B5);
    ((FillFn) 0x03000164)((void *) 0x06004000, 0x4000);
    ((FillFn) 0x03000164)(draw_destination, 0x4000);
    *(u16 *) 0x04000052 = 0x1010;
}

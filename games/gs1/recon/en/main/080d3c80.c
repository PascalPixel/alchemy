#include "types.h"

/*
 * Draft for the battle-presentation sub-effect at 0x080d3c80.
 *
 * The family matcher assigned games/gs1/src/battle/effects/member_orbit/
 * run.c (owner 080ce85c) as the closest structural template, but this owner
 * shares only the 0x03001eec "battle work" prologue and callback-teardown
 * shape with it -- there is no palette/tile upload, no Func_080ed408, and no
 * sine/cosine orbit math at all.  The real callee set and constants instead
 * match the same subsystem already recovered across games/gs1/recon/en/
 * main/080e7404.c, 080d59b0.c, 080d82b0.c, 080dc1ec.c and 080e01e4.c: a
 * single Func_080041d8(0x080CD261,0x480) callback, Func_08004458() as the
 * RNG, and (per games/gs1/recon/en/main/080e01e4.json's already-resolved
 * derivation) Func_080072f4 is not a real callee -- it is the r4 slot of
 * the _call_via_rN trampoline at games/gs1/asm/080072e4.s, so every call
 * through it below is modeled as a genuine indirect call through a
 * DrawRectangleFn value read out of the `callbacks` pair Func_080cef64
 * fills in.
 *
 * This owner grows an up-to-8-slot ring of particles at work + 0x7080
 * (matching 080e01e4's `ring`), one slot per listed table entry.  Each
 * slot accumulates until its threshold (record + 0x18) is reached, is
 * promoted (record + 8 set to 1), and from then on drives its own private
 * 16-entry sub-burst array at 0x02010000 + slot * 0x1C0, animated from a
 * handful of small byte/word tables at 0x080ee1d3/0x080ee1f5/0x080ee1fb/
 * 0x080ee207/0x080ee214 whose contents are not independently recoverable
 * from this owner alone.  The per-member_count slot count and total frame
 * count are direct table lookups (Data_080ee1f5[member_count*2] and
 * [member_count*2+1]), unlike the member_orbit template's `member_count *
 * 16 + 48` formula.
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
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);
u32 Func_08004458(void);
void Func_080f9010(s32 id);
void Func_080b50e8(s32 id);
s32 Func_080022fc(s32 a, s32 b);
s32 Func_080022ec(s32 a, s32 b);
void Func_080e3908(void *particle, s32 count, s32 flags);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);

extern u8 Value_000000cf;
extern const u8 Data_080ee1f5[];
extern const u8 Data_080ee1d3[];
extern const u8 Data_080ee1fb[];
extern const u8 Data_080ee207[];
extern const s32 Data_080ee214[];

void Func_080d3c80(void *object)
{
    void **cursor;
    void *work;
    void *draw_destination;
    DrawRectangleFn callbacks[2];
    DrawRectangleFn *callback_ptr;
    u8 *record;
    s32 i;
    s32 x_base;
    s32 x_rand;
    s32 slot;
    void **object_slot;
    void *screen_ptr;
    s32 amount;
    s32 frame;

    cursor = (void **)0x03001EEC;
    work = *cursor++;
    draw_destination = *cursor;
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(0);
    M2C_FIELD((void *)0x04000052, s16 *, 0) = 0x1010;
    Func_080e0524((s32) &Value_000000cf, work, 1, 1);
    callback_ptr = callbacks;
    Func_080cef64(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4), callback_ptr);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 50;
    Func_080041d8((void *) 0x080CD261, 0x480);

    if (Data_080ee1f5[
            M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x18) * 2]
            != 0) {
        record = (u8 *) work + 0x7080;
        i = 0;
        do {
            Func_08004458();
            M2C_FIELD(record, s32 *, 4) = (s32) 0xFFC00000;
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4)
                    == 1) {
                x_base = ((Func_08004458() & 0x1F) + 0x50) << 16;
                x_rand = 0x3F & Func_08004458();
            } else {
                x_base = ((Func_08004458() & 0x1F) + 8) << 16;
                x_rand = -(0x3F & Func_08004458());
            }
            M2C_FIELD(record, s32 *, 0xC) = x_rand << 12;
            M2C_FIELD(record, s32 *, 0) =
                x_base - (M2C_FIELD(record, s32 *, 0xC) * 18);
            M2C_FIELD(record, s32 *, 0x10) = 0;
            M2C_FIELD(record, s32 *, 8) = 0;
            M2C_FIELD(record, s32 *, 0x18) = i * 8;
            i++;
            record += 28;
        } while (i != Data_080ee1f5[
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)
                    * 2]);
    }

    frame = 0;
    if (Data_080ee1f5[
            M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x18) * 2
                + 1] != 0) {
        object_slot = (void **) ((u8 *) work + 0x7828);
        do {
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)
                        == 2
                    && frame <= 103) {
                screen_ptr = *(void **) 0x03001E80;
                amount = 0xC0;
                if (frame > 95) {
                    amount = 0x9C0 - frame * 24;
                }
                if (M2C_FIELD(*object_slot, s32 *, 4) == 0) {
                    M2C_FIELD(screen_ptr, u16 *, 54) =
                        (u16) (M2C_FIELD(screen_ptr, u16 *, 54) - amount);
                } else {
                    M2C_FIELD(screen_ptr, u16 *, 54) =
                        (u16) (M2C_FIELD(screen_ptr, u16 *, 54) + amount);
                }
            }
            if (frame == Data_080ee1f5[
                    M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *,
                        0x18) * 2 + 1] - 80) {
                Func_080b50e8(0x86);
            }
            if (frame == Data_080ee1f5[
                    M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *,
                        0x18) * 2 + 1] - 8) {
                M2C_FIELD(work, s32 *, 0x7780) = 3;
                M2C_FIELD(work, s32 *, 0x7784) = (s32) 0x06060606;
            }
            if (frame <= Data_080ee1f5[
                    M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *,
                        0x18) * 2 + 1] - 8) {
                if (Data_080ee1f5[
                        M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                            s32 *, 0x18) * 2] != 0) {
                    slot = 0;
                    record = (u8 *) work + 0x7080;
                    do {
                        if (M2C_FIELD(record, s32 *, 8) == 1) {
                            u8 *sub;
                            s32 j;

                            sub = (u8 *) 0x02010000 + slot * 0x1C0;
                            j = 0;
                            do {
                                s32 idx;
                                s32 w;
                                s32 h;
                                s32 x;
                                s32 y;
                                s32 delta;

                                idx = Func_080022fc(j, 5) * 3
                                    + Func_080022fc(
                                        Func_080022ec(
                                            M2C_FIELD(sub, s32 *, 0x18),
                                            0x60),
                                        3);
                                w = Data_080ee1fb[idx];
                                h = Data_080ee207[idx];
                                x = M2C_FIELD(sub, s16 *, 2) - (w >> 1);
                                y = M2C_FIELD(sub, s16 *, 6) - (h >> 1);
                                callback_ptr[(j <= 2) ? 1 : 0](
                                    draw_destination,
                                    (u8 *) work + Data_080ee214[idx]
                                        + 0x800,
                                    x, y, w, h);
                                Func_080e3908(sub, 64, 0x2000);
                                delta = M2C_FIELD(sub, s32 *, 8);
                                M2C_FIELD(sub, s32 *, 0x18) =
                                    M2C_FIELD(sub, s32 *, 0x18) + delta;
                                if (delta > 1 && (frame & 1)) {
                                    M2C_FIELD(sub, s32 *, 8) = delta - 1;
                                }
                                j++;
                                sub += 28;
                            } while (j != 16);
                        } else if (frame
                                >= M2C_FIELD(record, s32 *, 0x18)) {
                            s32 odd;

                            odd = slot & 1;
                            callback_ptr[odd](
                                draw_destination, work,
                                M2C_FIELD(record, s16 *, 2) - 16,
                                M2C_FIELD(record, s16 *, 6), 32, 64);
                            Func_080e3908(record, 64, 0x10000);
                            if (M2C_FIELD(record, s32 *, 4) > 0x380000) {
                                const u8 *tbl;
                                u8 *sub;
                                s32 k;

                                M2C_FIELD(record, s32 *, 8) = 1;
                                M2C_FIELD(record, s32 *, 4) = 0x380000;
                                tbl = Data_080ee1d3;
                                sub = (u8 *) 0x02010000 + slot * 0x1C0;
                                k = 0;
                                do {
                                    M2C_FIELD(sub, s32 *, 0) =
                                        (s32) ((tbl[0] - 40) << 16)
                                        + M2C_FIELD(record, s32 *, 0);
                                    M2C_FIELD(sub, s32 *, 4) =
                                        (s32) (tbl[1] << 16);
                                    M2C_FIELD(sub, s32 *, 0xC) =
                                        ((Func_08004458() & 0x7F) - 64)
                                            << 11;
                                    M2C_FIELD(sub, s32 *, 0x10) =
                                        -(s32) (Func_08004458() & 0x7F)
                                            << 11;
                                    if (odd) {
                                        M2C_FIELD(sub, s32 *, 0xC) =
                                            M2C_FIELD(sub, s32 *, 0xC)
                                                * 2;
                                        M2C_FIELD(sub, s32 *, 0x10) =
                                            -(s32) (Func_08004458()
                                                & 0x7F)
                                                << 12;
                                    }
                                    M2C_FIELD(sub, s32 *, 8) = 32;
                                    M2C_FIELD(sub, s32 *, 0x18) = 0;
                                    k++;
                                    tbl += 2;
                                    sub += 28;
                                } while (k != 16);
                                M2C_FIELD(work, s32 *, 0x77A8) = 8;
                                Func_080f9010(144);
                                if (M2C_FIELD(
                                        M2C_FIELD(work, void **, 0x7828),
                                        s32 *, 0x14) != 0) {
                                    s32 m;

                                    m = 0;
                                    do {
                                        Func_080d6888(
                                            M2C_FIELD(
                                                M2C_FIELD(work,
                                                    void **, 0x7828),
                                                s16 *, 36 + m * 2),
                                            7, 5, m, 4);
                                        m++;
                                    } while (m != M2C_FIELD(
                                            M2C_FIELD(work, void **,
                                                0x7828),
                                            s32 *, 0x14));
                                }
                            }
                        }
                        slot++;
                        record += 28;
                    } while (slot != Data_080ee1f5[
                            M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                                s32 *, 0x18) * 2]);
                }
            }

            Func_080e155c(
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)
                        * 2
                    + 4,
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)
                        * 4
                    + 8);
            Func_080cd52c();
            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);

            frame++;
        } while (frame != Data_080ee1f5[
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)
                    * 2 + 1]);
    }

    Func_08004278((void *) 0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}

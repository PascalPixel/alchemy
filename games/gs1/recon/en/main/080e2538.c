#include "types.h"

/*
 * Battle-presentation sub-effect at 0x080e2538.  Single argument is the
 * effect state pointer, stored at work + 0x7828 like every other member of
 * the 0x03001eec "battle work" subsystem family -- see
 * games/gs1/recon/en/main/080e7404.c, 080e01e4.c and
 * games/gs1/src/battle/effects/member_orbit/run.c for the shared field and
 * callee evidence this owner reuses.  The family-matcher's assigned template
 * (main:080ce85c, games/gs1/src/battle/effects/member_orbit/run.c) shares no
 * callees and a materially different control-flow shape (22 branch labels
 * against the template's half a dozen); only its prologue idiom (heap_cache
 * cursor pair, work+0x7828 state pointer) transfers.
 *
 * Sets BG2/BG3 affine scale, opens effect registration 0x8A against work,
 * creates a single heap-kind-46 blit routine (kept live in heap_cache[7] for
 * the whole function), then pans BG2's X reference point halfway from the
 * state's first party member's angle field to its last member's.  Looks up a
 * per-"variant" row count from Data_080eecfc (indexed by the state's own s32
 * field at offset 0x18), seeds a small per-row "vine" ring at work + 0x7080
 * and a much larger 21-items-per-row burst table at the fixed heap address
 * 0x02010000, then runs one pass per animation frame (bounded by
 * Data_080eecf7[row_count-1] + 0x50).  Each row becomes active once the
 * running frame counter crosses its own Data_080eecf7[row] + 0x12 threshold:
 * from then on every pass draws (and steps) all 21 of that row's burst
 * items; before that threshold, every pass instead draws (once past
 * Data_080eecf7[row]) and grows the row's own vine entry.  Exactly on the
 * frame a row's threshold is first crossed, the owner also cues
 * Func_080d6888 for every real party member on the state.
 *
 * `Func_080072f4` is not a real function symbol: it is the r4 entry of the
 * `_call_via_rN` trampoline bundle at games/gs1/asm/080072e4.s
 * (0x080072f4 == 0x080072e4 + 4*4), i.e. an indirect call through whatever
 * function pointer the compiler kept in r4 -- here the single blit routine
 * populated by `Func_080ed408(46, ...)` into heap_cache[7], already
 * established as `DrawRectangleFn` by games/gs1/recon/en/main/080e01e4.c and
 * games/gs1/src/battle/effects/member_orbit/run.c.
 * games/gs1/recon/en/main/080dc1ec.json's score.note has the full
 * derivation of the trampoline read.
 *
 * The one-time BG2X pan blend near the top is a genuine signed halving: the
 * reference's `lsrs`+`adds`+`asrs` sequence is GCC 2.96's compiled form of
 * ordinary `x / 2` (round-towards-zero via the sign-bit correction before an
 * arithmetic shift), not a bug to avoid -- writing plain `/ 2` reproduces it
 * byte-for-byte.
 *
 * The `fp >= threshold+0x12` item-loop guard and its `else` vine-growth path
 * share a merge point that the reference's own control flow reaches two
 * different ways (falling out of the item loop when a fresh re-read of the
 * threshold table no longer satisfies the guard, or entering the `else`
 * directly) -- kept here as the label/goto the disassembly actually shows
 * rather than smoothed into two independent ifs, since collapsing it changes
 * the emitted branch shape.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, u32 w, s32 h);

extern u8 Value_0000008a;
extern const u8 Data_080eecb2[];
extern const s8 Data_080eecf2[];
extern const u8 Data_080eecf7[];
extern const u8 Data_080eecfc[];
extern const u8 Data_080eecff[];
extern const u8 Data_080eed0e[];
extern const u16 Data_080eed1e[];

void Func_080cd594(s32 mode);
void Func_080e0524(s32 id, void *target, s32 flag_a, s32 flag_b);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080e396c(s32 value, s32 *out);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
u32 Func_08004458(void);
s32 Func_08002304(s32 numerator, s32 denominator);
s32 Func_080022fc(s32 a, s32 b);
s32 Func_080022ec(s32 numerator, s32 denominator);
void Func_080b50e8(s32 id);
void Func_080f9010(s32 id);
void Func_080e3908(void *particle, s32 count, s32 flags);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

void Func_080e2538(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_target;
    DrawRectangleFn blit;
    s32 row_count;
    s32 last_row;
    s32 row;
    s32 fp;
    s32 pan;
    s32 target_pan;
    u8 *vine;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_target = *cursor;
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(1);
    M2C_FIELD((void *)0x04000020, s16 *, 0) = 0x100;
    M2C_FIELD((void *)0x04000020, s16 *, 0x30) = 0;
    Func_080e0524((s32)&Value_0000008a, work, 1, 1);
    Func_080ed408(46, 7, 7, 3, 1);
    blit = (DrawRectangleFn)heap_cache[7];

    Func_080e396c(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 0x24), &pan);
    Func_080e396c(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *,
            (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x14) * 2)
                + 0x22),
        &target_pan);
    pan += (target_pan - pan) / 2;
    *(s32 *)0x04000028 = (0x40 - pan) << 8;

    M2C_FIELD(work, s32 *, 0x7780) = 1;
    M2C_FIELD(work, s32 *, 0x7784) = 0;
    Func_080041d8((void *)0x080CD261, 0x480);

    row_count = Data_080eecfc[M2C_FIELD(
        M2C_FIELD(work, void **, 0x7828), s32 *, 0x18)];
    if (row_count != 0) {
        u8 *ring;

        ring = (u8 *)work + 0x7080;
        row = 0;
        do {
            M2C_FIELD(ring, s32 *, 4) = (s32)0xFFC00000;
            M2C_FIELD(ring, s32 *, 0x10) = 0;
            row++;
            ring += 0x1C;
        } while (row != row_count);

        row = 0;
        do {
            u8 *seed;
            u8 *slot;
            s32 item;

            seed = (u8 *)Data_080eecb2;
            slot = (u8 *)0x02010000 + row * 0x24C;
            item = 0;
            do {
                M2C_FIELD(slot, s32 *, 0) =
                    (s32)((seed[0] + Data_080eecf2[row]) << 16);
                M2C_FIELD(slot, s32 *, 4) = (s32)(seed[1] << 16);
                M2C_FIELD(slot, s32 *, 0xC) =
                    (s32)((Func_08002304(Func_08004458(), 0x60) - 0x30) << 10);
                M2C_FIELD(slot, s32 *, 0x10) =
                    (s32)((0 - ((Func_08004458() & 0x7F) + 0x20)) << 11);
                M2C_FIELD(slot, s32 *, 8) = 0x20;
                M2C_FIELD(slot, s32 *, 0x18) = 0;
                item++;
                seed += 2;
                slot += 0x1C;
            } while (item != 21);
            row++;
        } while (row != row_count);
    }

    last_row = row_count - 1;
    fp = 0;
    if (Data_080eecf7[last_row] != -0x50) {
        do {
            if (fp == Data_080eecf7[last_row] + 0x30) {
                Func_080b50e8(0x84);
            }

            if (row_count != 0) {
                vine = (u8 *)work + 0x7080;
                row = 0;
                do {
                    if (fp == Data_080eecf7[row] + 0x12) {
                        Func_080f9010(0x86);
                        M2C_FIELD(work, s32 *, 0x77A8) = 4;
                    }

                    if (fp >= Data_080eecf7[row] + 0x12) {
                        s32 item;

                        item = 0;
                        do {
                            u8 *it;
                            s32 shape;
                            s32 speed;
                            u8 w;
                            u8 h;

                            it = (u8 *)0x02010000 + row * 0x24C + item * 0x1C;
                            shape = Func_080022fc(item, 5) * 3
                                + Func_080022fc(
                                    Func_080022ec(
                                        M2C_FIELD(it, s32 *, 0x18), 0x60),
                                    3);
                            w = Data_080eecff[shape];
                            h = Data_080eed0e[shape];
                            blit(draw_target,
                                (u8 *)work + Data_080eed1e[shape] + 0x83C,
                                M2C_FIELD(it, s16 *, 2) - (w >> 1),
                                M2C_FIELD(it, s16 *, 6) - (h >> 1), w, h);
                            Func_080e3908(it, 0x40, 0x4000);
                            speed = M2C_FIELD(it, s32 *, 8);
                            M2C_FIELD(it, s32 *, 0x18) += speed;
                            if (speed > 1 && (fp & 1)) {
                                M2C_FIELD(it, s32 *, 8) = speed - 1;
                            }
                            item++;
                        } while (item != 21);

                        if (fp < Data_080eecf7[row] + 0x12) {
                            goto block_28;
                        }
                    } else {
                    block_28:
                        if (fp >= Data_080eecf7[row]) {
                            blit(draw_target, work, Data_080eecf2[row] + 0x2F,
                                M2C_FIELD(vine, s16 *, 6), 0x22, 0x3E);
                        }
                        {
                            s32 growth;

                            growth = M2C_FIELD(vine, s32 *, 0x10);
                            M2C_FIELD(vine, s32 *, 4) += growth;
                            if (fp > Data_080eecf7[row]) {
                                M2C_FIELD(vine, s32 *, 0x10) =
                                    growth + 0x10000;
                            }
                            if (M2C_FIELD(vine, s32 *, 4) > 0x320000) {
                                M2C_FIELD(vine, s32 *, 4) = 0x320000;
                            }
                        }
                    }

                    if (fp == Data_080eecf7[row] + 0x12) {
                        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828),
                                s32 *, 0x14)
                            != 0) {
                            s32 member;
                            s32 member_id_offset;

                            member = 0;
                            member_id_offset = 0x24;
                            do {
                                Func_080d6888(
                                    M2C_FIELD(
                                        M2C_FIELD(work, void **, 0x7828),
                                        s16 *, member_id_offset),
                                    7, 5, member, 8);
                                member++;
                                member_id_offset += 2;
                            } while (member
                                != M2C_FIELD(
                                    M2C_FIELD(work, void **, 0x7828), s32 *,
                                    0x14));
                        }
                    }

                    row++;
                    vine += 0x1C;
                } while (row != row_count);
            }

            Func_080e155c(2, 4);
            Func_080cd52c();
            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);

            fp++;
        } while (fp != Data_080eecf7[last_row] + 0x50);
    }

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(46);
    Func_080cdbc0();
}

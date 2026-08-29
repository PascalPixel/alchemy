#include "types.h"

/*
 * Split function: head 080dd9c0, continuation 080dda3c, terminal region
 * (call-graph-reached, contains the real epilogue) 080ddb34. Reconstructed
 * as one C function per CONTRIBUTING.md "Split functions". Span 0x080dd9c0
 * .. 0x080ddde0 (1056 bytes).
 *
 * Family: the 0x03001eec "battle work" / heap_cache subsystem documented in
 * games/gs1/recon/en/main/080d82b0.c, 080d85d0.c and
 * games/gs1/src/battle/effects/member_orbit/run.c (080ce85c). This owner is
 * a fuller sibling of 080d82b0.c: it adds a BG2 affine/blend setup in its
 * head region, allocates TWO heap-cached rectangle-blit routines (kinds 46
 * and 47, not just 46), gates both the per-member setup loop and the whole
 * per-frame body on a byte read from ROM tables at 0x080eebae/0x080eebb6
 * indexed by the effect object's offset-0x24 field, and its per-frame body
 * draws two rectangles per active particle (one per heap kind) selected by
 * bit 0 of a per-slot ROM flag byte, rather than 080d82b0.c's single
 * draw call per particle.
 *
 * Literal-pool constants were resolved by hand from the retained assembly's
 * own pc-relative immediates against the LiteralPool_* labels already
 * present in games/gs1/asm/080dd9c0.s, 080dda3c.s (pool bases 0x080dda18
 * and 0x080ddb0c match the given labels exactly), and cross-checked with a
 * standalone `arm-none-eabi-as -mthumb` + `objdump -d --adjust-vma`
 * reassembly of each region file. The reassembly widened the two `.set`
 * absolute-symbol branches in 080dd9c0.s (bne/b to sub_080dda3c) to 4-byte
 * `bne.w`/`b.w` forms, which shifts the tool-computed pool addresses for the
 * final BG2X pair by 4 bytes versus the ROM's real narrow encoding; the pool
 * label position given in the .s file (LiteralPool_080dda18 at the address
 * consistent with narrow branches) was used as ground truth there instead of
 * the widened reassembly's own address arithmetic.
 */

#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, u32 w, s32 h);

#define REG_BG2PA (*(volatile u16 *)0x04000020)
#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)
#define REG_BG2X (*(volatile u32 *)0x04000028)

void Func_080cd594(s32 mode);
void Func_080e0524(s32 resource_id, void *destination, s32 destination_offset, s32 copy_palette);
extern u8 Value_0000007e;
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);
void Func_080b50e8(s32 id);
void Func_080f9010(s32 id);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_08004458_stub(void); /* placeholder removed below */
u32 Func_08004458(void);
s32 Func_080022fc(s32 a, s32 b);
void Func_080e3908(void *particle, s32 count, s32 flags);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_080e155c(s32 a, s32 b);

extern const u8 Data_080eeba6[];  /* per-release-index draw-shape selector byte */
extern const u8 Data_080eebae[];  /* per-release-slot flag byte (region3 gate) */
extern const u8 Data_080eebb6[];  /* frame-bound multiplier / release-gate-count table */
extern const u8 Data_080eebc0[];  /* interpolation table A: signed per-step amplitude */
extern const u16 Data_080eebc8[]; /* interpolation table B: per-step src offset (u16) */
extern const u8 Data_080eebb9[];  /* interpolation table C: signed per-step amplitude 2 */

/*
 * Func_080072fc/Func_080072f4 are not real callees: they are the r6 and
 * r4 entries of the _call_via_rN trampoline bundle at
 * games/gs1/asm/080072e4.s (see games/gs1/recon/en/main/080e08c0.c's
 * header and its cited dossiers for the established shape). Every site
 * that the disassembly shows branching through one of those trampolines
 * is really an indirect call through the currently-selected
 * draw_cb_46/draw_cb_47 callback -- ground truth (games/gs1/asm/
 * 080ddb34.s) loads the pointer from the same stack slots the
 * continuation region (games/gs1/asm/080dda3c.s) spills them to right
 * after the Func_080ed408(46, ...) / Func_080ed408(47, ...) calls:
 * [sp,#36] is draw_cb_46, [sp,#40] is draw_cb_47. Each call site below is
 * rewritten as an indirect call through whichever of the two the ground
 * truth loads at that point.
 */

void Func_080dd9c0(void *table_param)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *table;
    DrawRectangleFn draw_cb_46;
    DrawRectangleFn draw_cb_47;
    s32 status46;
    s32 status47;
    s32 frame_bound;
    s32 fade_start;
    s32 end_wait;
    s32 frame;
    s32 member_bound;
    s32 member_index;
    s32 gate_count;
    s32 release_count;
    s32 release_threshold;
    s32 release_window;
    s32 announce_frame;
    const u8 *release_flag_ptr;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    M2C_FIELD(work, void **, 0x7828) = table_param;

    Func_080cd594(1);

    REG_BG2PA = 0x0100;
    REG_BLDCNT = 0x0000;
    REG_BLDALPHA = 0x1010;

    Func_080e0524((s32) &Value_0000007e, work, 1, 1);

    table = M2C_FIELD(work, void **, 0x7828);
    if (M2C_FIELD(table, s32 *, 4) == 1) {
        REG_BG2X = 0xffff9000;
    }

    /* ---- Continuation_080dda3c ---- */

    status46 = Func_080ed408(46, 7, 7, 3, 1);
    draw_cb_46 = *(DrawRectangleFn *)((u8 *)heap_cache + 28);
    status47 = Func_080ed408(47, 7, 7, 3, 1);
    draw_cb_47 = *(DrawRectangleFn *)((u8 *)heap_cache + 32);
    (void)status46;
    (void)status47;

    M2C_FIELD(work, s32 *, 0x7780) = 1;
    M2C_FIELD(work, s32 *, 0x7784) = 0;

    Func_080041d8((void *)0x080cd261, 144 << 3);

    table = M2C_FIELD(work, void **, 0x7828);
    frame_bound = Data_080eebb6[M2C_FIELD(table, u8 *, 24)] * 8 + 56;

    {
        s32 *pool_cursor;
        s32 pool_index;

        pool_cursor = (s32 *)0x02010018;
        pool_index = 0;
        do {
            pool_index++;
            *pool_cursor = -1;
            pool_cursor += 7;
        } while (pool_index != 1024);
    }

    frame = 0;
    if (frame_bound != 0) {
        fade_start = frame_bound - 64;
        end_wait = frame_bound - 16;

        for (frame = 0; frame != frame_bound; frame++) {
            if (frame == fade_start) {
                Func_080b50e8(132);
            }
            if (frame >= end_wait) {
                REG_BLDCNT = 0x3f44;
                REG_BLDALPHA = ((frame_bound - frame - 1) | 0x1000);
            }

            table = M2C_FIELD(work, void **, 0x7828);
            gate_count = Data_080eebb6[M2C_FIELD(table, u8 *, 24)];

            if (gate_count != 0) {
                /*
                 * ---- Region_080ddb34, release/retire portion ----
                 * Runs the release-scan + announce/retire scan repeatedly
                 * (the retire scan always executes at least once per frame,
                 * whether or not a release happened this pass) until
                 * release_count catches up with gate_count.
                 */
                release_window = frame - 8;
                release_flag_ptr = Data_080eebae;
                announce_frame = 12;
                release_threshold = 8;
                release_count = 0;

                for (;;) {
                    if (frame > release_threshold) {
                        s32 gate = Data_080eeba6[release_count];

                        if (gate <= 1) {
                            s32 h0, w0, x, y;

                            h0 = release_window * 6;
                            if (h0 > 30) h0 = 30;
                            w0 = release_window * 16;
                            if (w0 > 80) w0 = 80;
                            y = 108 - w0;

                            if (gate & 1) {
                                x = *release_flag_ptr - h0;
                                draw_cb_47(draw_destination, work, x, y, 48, w0);
                            } else {
                                x = *release_flag_ptr + h0;
                                draw_cb_46(draw_destination, work, x, y, 48, w0);
                            }
                        } else {
                            s32 h0, w0, x, y;
                            void *src;

                            w0 = release_window * 8;
                            if (w0 > 64) w0 = 64;
                            h0 = release_window;
                            if (h0 > 8) h0 = 8;
                            y = 108 - w0;

                            if (gate & 1) {
                                x = *release_flag_ptr - h0;
                                src = (u8 *)work + 3840;
                                draw_cb_47(draw_destination, src, x, y, 32, w0);
                            } else {
                                x = *release_flag_ptr + h0;
                                src = (u8 *)work + 1728;
                                draw_cb_46(draw_destination, src, x, y, 32, w0);
                            }
                        }

                        if (frame == release_threshold + 1) {
                            M2C_FIELD(work, s32 *, 0x77a8) = 3;
                        }

                        if (frame < release_threshold + 3) {
                            /* find a free particle slot and spawn one */
                            s32 life;
                            s32 *particle;
                            s32 i;

                            life = (Func_08004458() & 31) + 72;

                            particle = (s32 *)0x02010000;
                            for (i = 0; i != 64; i++, particle += 7) {
                                if (particle[6] == -1) {
                                    break;
                                }
                            }
                            if (i != 64) {
                                s32 v;

                                v = *release_flag_ptr + (s32)(Func_08004458() & 31) + 32;
                                if (v > 96) v = 96;
                                particle[0] = v;
                                particle[1] = life;
                                particle[6] = 0;
                            }
                        }
                    }

                    /* ---- retire / announce scan (always runs) ---- */
                    table = M2C_FIELD(work, void **, 0x7828);
                    member_bound = M2C_FIELD(table, s32 *, 20);
                    if (member_bound != 0) {
                        for (member_index = 0; member_index != member_bound; member_index++) {
                            if (frame == announce_frame) {
                                Func_080f9010(132);
                                table = M2C_FIELD(work, void **, 0x7828);
                                Func_080d6888(
                                    M2C_FIELD(table, s16 *, 36 + member_index * 2),
                                    7, 5, member_index, 3);
                            }
                        }
                    }

                    release_flag_ptr += 1;
                    announce_frame += 8;
                    release_threshold += 8;
                    release_window -= 8;
                    table = M2C_FIELD(work, void **, 0x7828);
                    gate_count = Data_080eebb6[M2C_FIELD(table, u8 *, 24)];
                    release_count += 1;
                    if (release_count == gate_count) {
                        break;
                    }
                }
            }

            /* ---- Region_080ddb34, interpolation portion (always runs) --- */
            {
                s32 *particle;
                s32 i;

                particle = (s32 *)0x02010000;
                for (i = 0; i != 64; i++, particle += 7) {
                    s32 state = particle[6];

                    if (state >= 0) {
                        s32 half = state / 2;
                        u16 src_off = Data_080eebc8[half];
                        s32 rawA = (s8)Data_080eebc0[half];
                        s32 halfA = rawA / 2;
                        s32 valC = (s8)Data_080eebb9[half];
                        s32 x1, y;

                        x1 = particle[0] - valC;
                        y = particle[1] - halfA;
                        draw_cb_46(draw_destination, (u8 *)work + src_off, x1, y, valC, rawA);
                        draw_cb_47(draw_destination, (u8 *)work + src_off, particle[0], y, valC, rawA);

                        state += 1;
                        particle[6] = state;
                        if (state == 14) {
                            particle[6] = -1;
                        }
                    }
                }
            }

            Func_080e155c(8, 8);
            Func_080cd52c();
            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);
        }
    }

    Func_08004278((void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}

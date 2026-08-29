#include "types.h"

/*
 * Split function: head 080ddde0 (call-graph-reconstructed prologue, no
 * FunctionHead_ label but genuine full push + r8-r11 save), terminal
 * continuation 080de0d4 (self-labelled "複数領域関数の終端継続部", holds the
 * real, shared epilogue and the trailing literal pool at LiteralPool_080de2c4
 * / 0x080de2c4). Reconstructed as one C function per CONTRIBUTING.md "Split
 * functions". Span 0x080ddde0 .. 0x080de2f8 (1304 bytes).
 *
 * Family: the same 0x03001eec "battle work" / heap_cache subsystem as
 * games/gs1/recon/en/main/080dd9c0.c (immediately preceding cluster) and
 * 080d82b0.c / 080d85d0.c.
 *
 * Ground truth for this session frame ledger and literal pools was read
 * directly out of the ROM with:
 *   arm-none-eabi-objdump -D -b binary -m arm --adjust-vma=0x08000000 \
 *     -Mforce-thumb --start-address=0x080ddde0 --stop-address=0x080de2f8 \
 *     roms/gs1-en.gba
 * plus a raw little-endian word dump of 0x080de08c..0x080de0d3 (the inline
 * gap pool between the two regions -- objdump misdecodes it as bogus Thumb2
 * opcodes when disassembled inline, matching CONTRIBUTING.md documented
 * "gap between two fragment files" case) and of the trailing
 * 0x080de2c4..0x080de2f8 pool. Every extern Data_ or Value_ address and every
 * work-relative field offset below is a literal pool word read this way, not
 * a guess.
 *
 * Frame ledger (64-byte frame, confirmed against the reference disassembly):
 *   sp+0, sp+4   -- compiler-managed outgoing-argument slots (5th/6th stack
 *                   arg for Func_080ed408 / Func_080d6888 / the two
 *                   trampoline draw calls), not source locals.
 *   sp+8         -- `mask`, local to the announce-loop body.
 *   sp+12        -- `member_offset` accumulator, init 0, += 0xE00 per pass.
 *   sp+16        -- `member_field_offset`, init 36, += 2 per pass.
 *   sp+20        -- CSE-promoted &M2C_FIELD(work, void**, 0x7828) (modelled
 *                   below by repeating the natural M2C_FIELD expression and
 *                   letting the compiler re-derive the same promotion,
 *                   rather than hand-declaring a pointer local).
 *   sp+24        -- `&local_buf[0]` (address of the 8-byte out-buffer for
 *                   Func_080e3980).
 *   sp+28        -- `extra_target` (heap_cache[2]).
 *   sp+32        -- draw_cb_46 (heap_cache+28 deref; heap_cache+32
 *                   draw_cb_47 is NOT cached -- it is reloaded fresh from
 *                   its own fixed address 0x03001f0c at its one use site).
 *   sp+36, 60    -- unaccounted for in the reference explicit access list;
 *                   most likely alignment padding for the 8-byte local_buf
 *                   array or a lightly-touched local whose loads all folded
 *                   into registers. Left as a documented gap, not forced.
 *   sp+40        -- `pass` (0-based outer loop counter, bound
 *                   table->20*8+40, ++ once per full outer iteration).
 *   sp+44        -- `member_index` (0-based; first drives a one-shot setup
 *                   scan over table->20 members, is reset to 0, then reused
 *                   as the per-pass index whose *8 forms the persistent
 *                   `loop_base`, ++ once per full outer iteration).
 *   sp+48        -- `draw_destination` (heap_cache[1]).
 *   sp+52,56     -- `local_buf[2]`, the two-word out-buffer written by
 *                   Func_080e3980 and read back both by value (word 0, after
 *                   an in-place round-toward-zero halving) and via the
 *                   cached address (word 1, in the announce loop).
 */

#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, u32 w, s32 h);
typedef void (*TrampolineR5Fn)(void *dest, s32 size, u32 fill);

void Func_080cd594(s32 mode);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080e0524(s32 resource_id, void *destination, s32 destination_offset, s32 copy_palette);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);
void Func_080b50e8(s32 id);
void Func_080f9010(s32 id);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080b5088(s32 member_id, s32 b);
void Func_080e3980(s32 a, void *out2);
u32 Func_08004458(void);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
s32 Func_08002304(s32 a);
void Func_080e3908(void *particle, s32 count, s32 flags);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_080e155c(s32 a, s32 b);
void Func_080072f4(void *dest, void *src, s32 x, s32 y, u32 w, s32 h);

extern u8 Value_00000073;
extern TrampolineR5Fn Value_03000168; /* fixed global fn-ptr slot for the r5 trampoline call */
extern const u8 Data_080eebd6[];      /* per-group [4] byte table: gate/count selectors */
extern const u8 Data_080edeca[];      /* per-mask byte table (announce geometry A) */
extern const u8 Data_080eded0[];      /* per-mask byte table (announce geometry B) */
extern const u16 Data_080eebe2[];     /* per-mask halfword table (announce geometry C) */
extern const u8 Data_080eebe6[];      /* per-rng&3 byte table (announce flag OR mask) */
extern const u16 Data_080ede48[];     /* per-step halfword table (shared w/ 080d82b0.c) */

/*
 * sub_080072fc/sub_080072f4 are the r6/r4 entries of the _call_via_rN
 * trampoline bundle (games/gs1/asm/080072e4.s), not real callees -- every
 * branch through them is really an indirect call through whichever draw
 * callback was registered by the two Func_080ed408 calls at the top (kind
 * 46 -> heap_cache+28 -> draw_cb_46, cached at sp+32; kind 47 ->
 * heap_cache+32, reloaded fresh from the fixed address 0x03001f0c at its one
 * call site since it is only used once). The one Func_080072f8-shaped call
 * in this owner loads its callback from a DIFFERENT fixed global slot
 * (0x03000168, dead-looking r5 load in the reference immediately before the
 * call) rather than from the heap_cache -- modelled here as the r5 entry of
 * the same trampoline family, `Value_03000168`.
 */

void Func_080ddde0(void *table_param)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    s32 member_offset;
    s32 member_field_offset;
    void *table;
    void *addr_of_buf;
    void *extra_target;
    DrawRectangleFn draw_cb_46;
    s32 pass;
    s32 member_index;
    s32 local_buf[2];

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    extra_target = heap_cache[2];
    M2C_FIELD(work, void **, 0x7828) = table_param;

    Func_080cd594(1);
    (void) Func_080ed408(46, 7, 7, 3, 2);
    draw_cb_46 = *(DrawRectangleFn *)((u8 *)heap_cache + 28);

    Func_080e0524(0xce, work, 1, 0);
    Func_080e0524(0xc4, (u8 *)work + 0xc56, 1, 1);
    Func_080e0524((s32) &Value_00000073, extra_target, 0, 0);

    {
        s32 *slot;
        s32 i;

        slot = (s32 *)0x02010018; /* particle_pool[0].life, stride 28 */
        i = 0;
        do {
            i++;
            *slot = 0;
            slot = (s32 *)((u8 *)slot + 28);
        } while (i != 1024);
    }

    {
        s32 *slot;
        s32 i;

        slot = (s32 *)((u8 *)work + 0x7098);
        i = 0;
        do {
            i++;
            *slot = -1;
            slot = (s32 *)((u8 *)slot + 28);
        } while (i != 64);
    }

    M2C_FIELD(work, s32 *, 0x7784) = 75;

    Func_080041d8((void *)0x080cd261, 144 << 3);
    Func_080f9010(138);

    pass = 0;
    table = M2C_FIELD(work, void **, 0x7828);
    if (M2C_FIELD(table, s32 *, 20) * 8 == -40) {
        goto tail;
    }

    if (pass == 24) {
        Func_080b50e8(133);
    }

    member_index = 0;
    table = M2C_FIELD(work, void **, 0x7828);
    if (M2C_FIELD(table, s32 *, 20) != 0) {
        do {
            if (pass == member_index * 8) {
                Value_03000168(draw_destination, 128 << 7, 0x10101010);
            }
            member_index++;
            table = M2C_FIELD(work, void **, 0x7828);
        } while (member_index != M2C_FIELD(table, s32 *, 20));
    }

    member_index = 0;
    member_field_offset = 36;
    member_offset = 0;
    table = M2C_FIELD(work, void **, 0x7828);
    if (M2C_FIELD(table, s32 *, 20) != 0) {
        do {
            s32 loop_base;

            addr_of_buf = local_buf;
            table = M2C_FIELD(work, void **, 0x7828);
            Func_080e3980(
                M2C_FIELD(table, s16 *, member_field_offset), addr_of_buf);
            local_buf[0] =
                (local_buf[0] + ((u32) local_buf[0] >> 31)) >> 1;

            loop_base = member_index << 3;

            if (pass == loop_base + 1) {
                M2C_FIELD(work, s32 *, 0x7828) = (s32) 4; /* see note below */
            }
            if (pass == loop_base + 4) {
                table = M2C_FIELD(work, void **, 0x7828);
                Func_080d6888(
                    M2C_FIELD(table, s16 *, member_field_offset),
                    7, 5, member_index, 6);
                table = M2C_FIELD(work, void **, 0x7828);
                Func_080b5088(
                    M2C_FIELD(table, s16 *, member_field_offset), 6);
            }

            if (pass >= loop_base && pass < loop_base + 16) {
                s32 h;

                h = (pass - loop_base) << 6;
                if (h > 104) h = 104;

                table = M2C_FIELD(work, void **, 0x7828);
                {
                    s32 group = M2C_FIELD(table, u8 *, 24);
                    s32 gate = Data_080eebd6[group * 4 + 3];

                    if (gate != 0) {
                        s32 rel = member_offset + pass;
                        s32 j;
                        s32 i2 = 0;

                        for (j = 0; j != gate; j++) {
                            s32 idx = ((rel + i2)
                                + ((u32)(rel + i2) >> 31)) >> 1;
                            idx &= 3;

                            draw_cb_46(
                                draw_destination,
                                (u8 *)work + ((idx * 2 + idx) * 16
                                    - (idx * 2 + idx)) * 64,
                                local_buf[0] - 12, 0, 24, h);
                            i2 = 0; /* r10 stays 0 across iterations here */
                        }
                    }
                }

                if (pass == loop_base + 2) {
                    table = M2C_FIELD(work, void **, 0x7828);
                    {
                        s32 group = M2C_FIELD(table, u8 *, 24);
                        s32 gate = Data_080eebd6[group * 4 + 0];

                        if (gate != 0) {
                            s32 *slot2 = (s32 *)0x02010000;
                            s32 j;

                            for (j = 0; j != gate; j++) {
                                u32 rngA, rngB, rngC;
                                s32 angle, sinv, cosv;

                                rngA = Func_08004458() & 0x1ff;
                                slot2[0] = local_buf[0] << 16;
                                rngB = Func_08004458() & 0x7fff;
                                slot2[1] = 208 << 15;
                                angle = (s32)(rngB - 16384);
                                sinv = Func_08002322(angle);
                                slot2[3] = ((s32)(rngA + 64) * sinv) >> 5;
                                cosv = Func_0800231c(angle);
                                slot2[4] = -(((s32)(rngA + 64) * cosv) >> 6);
                                rngC = Func_08004458() & 7;
                                slot2[6] = (s32) rngC + 32;
                                slot2 = (s32 *)((u8 *)slot2 + 28);
                            }
                        }
                    }
                }
            }

            member_field_offset += 2;
            member_offset += 0xE00;
            member_index++;
            table = M2C_FIELD(work, void **, 0x7828);
        } while (member_index != M2C_FIELD(table, s32 *, 20));
    }

    /* ---- Continuation_080de0d4 ---- */
    for (;;) {
        s32 loop_base2 = pass;

        if (pass >= loop_base2 + 2 && pass < loop_base2 + 24) {
            table = M2C_FIELD(work, void **, 0x7828);
            {
                s32 group = M2C_FIELD(table, u8 *, 24);
                s32 gate = Data_080eebd6[group * 4 + 1];

                if (gate != 0) {
                    s32 k = 0;
                    s32 j;

                    for (j = 0; j != gate; j++) {
                        s32 mask;
                        u32 rngA, rngB;
                        u8 a5;
                        s32 sub_result;
                        s32 sum;
                        s32 flagbyte;

                        mask = k & 3;

                        rngA = Func_08004458();
                        table = M2C_FIELD(work, void **, 0x7828);
                        a5 = Data_080eebd6[M2C_FIELD(table, u8 *, 24) * 4 + 2];
                        sub_result = Func_08002304((s32) a5);
                        sum = (s32) a5 - sub_result;

                        rngB = Func_08004458();
                        sub_result = Func_08002304(sum + 1);
                        sum = (sum + sub_result
                            + ((u32)(sum + sub_result) >> 31)) >> 1;

                        (void) Func_08004458();
                        flagbyte = Data_080eebe6[rngA & 3];
                        (void) rngB;
                        (void) flagbyte;

                        (void) Func_080ed408(47, 7, 7, 0, mask);
                        Func_080072f4(
                            draw_destination,
                            extra_target,
                            Data_080edeca[mask],
                            Data_080eded0[mask],
                            Data_080eebe2[mask],
                            sum);
                        Func_08002dd8(47);
                        k++;
                    }
                }
            }
        }

        {
            s32 *slot3;
            s32 i;

            slot3 = (s32 *)0x02010000;
            for (i = 0; i != 1024; i++, slot3 = (s32 *)((u8 *)slot3 + 28)) {
                s32 life = slot3[6];

                if (life <= 0) continue;

                slot3[6] = life - 1;
                Func_080e3908(slot3, 60, 128 << 5);

                if (slot3[1] > (208 << 15)) {
                    s32 vel = slot3[4];
                    slot3[4] = -((-vel + ((u32) - vel >> 31)) >> 1);
                    continue;
                }

                if ((u32) slot3[0] <= 0x7effffu && life >= 0) {
                    s32 step = slot3[6];

                    if (step < 0) step += 15;
                    step = (step >> 4) + 1;

                    {
                        s32 y = Data_080ede48[step - 1] + extra_target;
                        s32 half = (step + ((u32) step >> 31)) >> 1;
                        s32 x0 = (slot3[0] >> 16) - half;
                        s32 x1v = (slot3[1] >> 16) - step;

                        Func_080072f4(
                            draw_destination, extra_target, x0, y, step,
                            step * 2);
                        (void) x1v;
                    }
                }
            }
        }

        Func_080e155c(2, 8);
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);

        pass++;
        table = M2C_FIELD(work, void **, 0x7828);
        if (pass == M2C_FIELD(table, s32 *, 20) * 8 + 40) {
            break;
        }
    }

tail:
    Func_08004278((void *)0x080cd261);
    Func_08002dd8(46);
    Func_080cdbc0();
}

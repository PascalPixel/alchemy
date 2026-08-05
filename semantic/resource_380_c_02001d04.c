#include "types.h"

/*
 * resource_380 owner at 0x02001d04, 1400 bytes: a scene beat centered
 * on actor 14 (a spawned guest -- the drafted Func_020043bc burst is
 * invoked to bring it in) and actor 1.  The head wires actor 14's
 * record (Func_080091e0 registration, Func_0808a158 modes, a
 * three-text exchange 0x10b6/0x10bb with Func_08015210(0x10ba, 1, 10)
 * between), runs a 30-tick +0x10000 rise on the record's +12 field
 * behind Func_080f9010(0xdc) with the +0x55 phase byte 0 -> 5, then a
 * dialogue burst against id 0x01dd0000.  The middle is a re-prompt
 * loop like 27f8's: gated on Func_0808a070(1, 0) -- note the (1, 0)
 * arguments, unlike the (0, 0) skip-beat gate everywhere else -- with
 * first prompt 0x10c3 and re-prompt 0x10c6.  The close hands actor 14
 * off through Func_08009150, plays three Func_080f9010(0x99) text
 * blocks on actor 1's record (the 178c idiom, ids 342/312/278), and
 * raises flags 220, 221, and 223 via Func_080772e8 -- the very code
 * values the 0x0200178c owner later counts in its roster scan.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + r8 spill at
 * 0x02001d04-0x02001d08 through the epilogue at 0x02002254-0x0200225c,
 * alignment halfword, tail pool to 0x0200227b; next owner
 * (resource_380_c_0200227c.c, already drafted) exactly at 0x0200227c.
 * One mid-function pool cluster 0x02002016-0x02002046 hopped by
 * `b.n`.  The queue's ~670-byte estimate was short by 730.
 *
 * Callee names from bun tools/overlay_call_targets.ts resource_380
 * 1d04 227c (the +2 rule): 144 sites, 13 to drafted locals
 * (Func_02004248, Func_020043bc).
 *
 * Uncertainty: the record fields (+12 risen by the loop, +0x55 phase,
 * +90 flag byte, +0x28/+0x30/+0x34 text-block reloads) are transcribed
 * by offset; Func_08009150's four arguments are recorded as compiled
 * (record pointer, two fixed-point constants, zero) with roles open;
 * what distinguishes Func_0808a070(1, 0) from the (0, 0) form is not
 * established here.
 */

extern void Func_02004248(s32 arg0, s32 arg1);
extern void Func_020043bc(void);

extern void Func_080000c0(s32 arg0);
extern void Func_08009128(void);
extern void Func_08009150(void *record, s32 arg1, s32 arg2, s32 arg3);
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_08015210(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080772e8(s32 flag_id);
extern void Func_0808a010(s32 frames);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0e8(s32 id);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a150(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a170(s32 textId);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a180(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a200(s32 arg0, s32 arg1);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_080f9010(s32 arg0);

void Func_02001d04(void)
{
    u8 *record;
    u8 *phase;
    u8 *flag;
    u8 *entry;
    s32 tick;
    s32 promptId;

    Func_0808a138(1, 3);
    Func_0808a010(10);
    Func_0808a1b8(1, 0x3000, 0);
    Func_080091e0(Func_0808a080(14), 0);
    Func_0808a158(14, 15);
    Func_0808a0f0(14, 0x1880000, 0x1c60000);
    Func_020043bc();
    Func_0808a1b8(1, 0xd000, 10);
    Func_0808a130(1, 2);
    Func_0808a1e8(1, 256, 40);
    Func_0808a1b8(14, 0x5000, 10);
    Func_0808a138(14, 2);
    Func_0808a010(20);
    Func_0808a170(0x10b6);
    Func_0808a180(14, 0);
    Func_0808a0f0(10, 0x01d50000, 0x15c0000);
    Func_0808a010(20);
    Func_02004248(0x200a, 10);
    Func_02004248(0x200a, 40);
    Func_0808a0f0(10, 0x01fb0000, 0x15c0000);
    Func_0808a138(1, 2);
    Func_0808a010(40);
    Func_0808a110(1, 3);
    Func_0808a010(40);
    Func_0808a090(1, 0x8000, 0x4000);
    Func_0808a0d0(1, 389, 468);
    Func_0808a1b8(1, 0xd000, 60);
    Func_02004248(1, 20);
    Func_08015210(0x10ba, 1, 10);
    Func_0808a090(1, 0x8000, 0x4000);
    flag = Func_0808a080(1) + 90;
    *flag &= 0xfe;
    Func_0808a0d0(1, 376, 470);
    Func_0808a010(30);
    *flag |= 1;
    Func_0808a110(14, 4);
    Func_0808a010(10);
    Func_0808a170(0x10bb);
    Func_02004248(14, 20);
    Func_0808a1e8(1, 0x101, 60);
    Func_0808a110(14, 3);
    Func_02004248(14, 20);
    Func_0808a1e8(1, 258, 60);
    Func_0808a110(14, 3);
    Func_0808a010(20);
    Func_0808a1b8(14, 0xc000, 20);
    Func_0808a158(14, 256);
    Func_080091e0(Func_0808a080(14), 0);
    record = Func_0808a080(14);
    phase = record + 0x55;
    Func_080f9010(0xdc);
    for (tick = 0; tick != 30; tick++) {
        *(s32 *)(record + 12) += 0x10000;
        Func_0808a010(1);
    }
    *phase = 0;
    *phase = 5;
    Func_0808a130(1, 2);
    Func_02004248(1, 10);
    Func_0808a1e8(14, 0x101, 60);
    Func_0808a1b8(14, 0x5000, 10);
    Func_02004248(1, 20);
    Func_0808a138(14, 1);
    Func_02004248(14, 20);
    Func_0808a1e8(1, 0x103, 20);
    Func_02004248(1, 30);
    Func_0808a1e8(14, 0x105, 80);
    Func_0808a1b8(14, 0xd000, 40);
    Func_0808a1b8(10, 0x5000, 10);
    Func_0808a368();
    Func_0808a370();
    Func_0808a210(0x01dd0000, -1, 0x14e0000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);
    Func_0808a110(10, 4);
    Func_02004248(10, 10);
    Func_0808a178(11, 0);
    Func_0808a208(0x66666, 0xcccc);
    Func_0808a210(0x1760000, -1, 0x1d60000, 1);
    Func_0808a218();
    Func_0808a1b8(14, 0x5000, 0);
    Func_0808a1b8(1, 0xe000, 10);
    Func_0808a138(1, 2);
    if (Func_0808a070(1, 0) != 0) {
        Func_0808a010(10);
        Func_0808a110(14, 4);
        promptId = 0x10c3;
        goto presentPrompt;

repeatPrompt:
        Func_0808a010(20);
        Func_0808a110(14, 4);
        Func_0808a010(10);
        promptId = 0x10c6;

presentPrompt:
        Func_0808a170(promptId);
        Func_0808a178(14, 0);
        if (Func_0808a070(1, 0) == 0) {
            goto repeatPrompt;
        }
    }
    Func_0808a010(30);
    Func_0808a110(14, 3);
    Func_0808a010(20);
    Func_0808a170(0x10c4);
    Func_02004248(14, 30);
    Func_0808a110(14, 3);
    Func_0808a010(10);
    Func_02004248(14, 30);
    Func_0808a090(14, 0x26666, 0x13333);
    Func_08009150(record, 0x1cc0000, 0, 0x1680000);
    *(s32 *)(record + 0x30) = 0x30000;
    *phase = 0;
    Func_0808a0e8(14);
    Func_0808a158(14, 0);
    Func_080091e0(Func_0808a080(14), 1);
    Func_0808a010(30);
    Func_0808a200(1, 1);
    Func_0808a218();
    Func_0808a010(40);
    Func_0808a1e8(1, 0x103, 40);
    Func_0808a138(1, 3);
    Func_0808a010(20);
    record = Func_0808a080(1);
    record[90] |= 1;
    *(s32 *)(record + 0x34) = 0x20000;
    Func_080f9010(0x99);
    *(s32 *)(record + 0x28) = 0x60000;
    Func_0808a100(1, 7);
    Func_0808a0c0(1, 342, 470);
    Func_0808a100(1, 1);
    Func_0808a010(30);
    Func_080f9010(0x99);
    *(s32 *)(record + 0x28) = 0x60000;
    Func_0808a100(1, 7);
    Func_0808a0c0(1, 312, 470);
    Func_0808a100(1, 1);
    Func_0808a010(30);
    Func_080f9010(0x99);
    *(s32 *)(record + 0x28) = 0x60000;
    Func_0808a100(1, 7);
    Func_0808a0c0(1, 278, 480);
    Func_0808a100(1, 1);
    Func_0808a010(30);
    Func_0808a208(0x8000, 0x1000);
    Func_0808a200(0, 1);
    Func_0808a090(1, 0x19999, 0xcccc);
    Func_0808a150(0, 1, 0);
    Func_0808a010(30);
    Func_0808a110(1, 3);
    Func_0808a110(0, 4);
    Func_0808a138(1, 2);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a100(1, 2);
    entry = Func_0808a080(0);
    if (entry != 0) {
        Func_0808a0b8(1, *(s16 *)(entry + 10), *(s16 *)(entry + 18));
    }
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_080772e8(220);
    Func_080772e8(221);
    Func_080772e8(223);
}

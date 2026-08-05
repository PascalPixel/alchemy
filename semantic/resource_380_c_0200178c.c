#include "types.h"

/*
 * resource_380 owner at 0x0200178c, 1400 bytes: a scripted scene beat
 * over actors 5/9/10/11/12/13 with a real decision in the middle.  The
 * long linear head (~70 calls) choreographs placements, mode/anim runs,
 * and hold waits, culminating in a dialogue burst against id 0x01050000
 * bracketed by Func_0808a368/370 and a Func_08009128 + Func_080000c0(1)
 * refresh.  Then the function counts party/roster records: 15 u16
 * fields at Func_08077008(1)+0xd8 stepped by 2, tallying entries whose
 * low nine bits are 220, 221, or 223.  A two-level branch on
 * Func_0808a070(0, 0) (the skip-beat/choice gate the 3c9 drafts named)
 * crossed with count<=2 picks one of four text arms (ids
 * 0x10b0/0x10b4/0x10b2/0x10b5, two of them followed by
 * Func_08015210(id+1, 1, 0)), and all four meet at a shared closing
 * beat: camera/fade calls, the +90 record-flag clear/set idiom, three
 * repeated text blocks (ids 312/342/376, all against 470) each behind
 * Func_080f9010(0x99) with the record's +0x28 field reloaded with
 * 0x60000, +0x30/+0x34 seeded 0x30000/0x20000 once before the first.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x0200178c through
 * `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02001cdc-0x02001ce0,
 * alignment halfword, then the eight-word literal pool
 * 0x02001ce4-0x02001d03; next owner's prologue exactly at 0x02001d04.
 * The queue's ~1090-byte estimate was short by 310 bytes -- span
 * measured against the next owner's prologue, per the 1450 lesson.
 *
 * Callee names come from bun tools/overlay_call_targets.ts
 * resource_380 178c 1d04 (the +2 rule): 146 sites, 32 distinct
 * targets, 127 through the overlay's import-veneer table into the
 * familiar main-ROM scene vocabulary (Func_0808a0xx), 19 to the local
 * wrapper Func_02004248.  NOTE: the ten earlier resource_380 drafts
 * predate the +2 rule and carry naive-decode callee names
 * (Func_02005xxx/02006xxx) that do not exist -- correction flagged to
 * the team; this file uses the resolved names only.
 *
 * Uncertainty: which record family Func_08077008(1) returns (the
 * +0xd8 u16 array read 15 deep, and the 220/221/223 code values) is
 * unresolved; the +90 flag byte and +0x28/+0x30/+0x34 fields of the
 * Func_0808a080 record are transcribed by offset, roles open; text
 * and dialogue ids are literal.
 */

extern void Func_02004248(s32 arg0, s32 arg1);

extern void Func_080000c0(s32 arg0);
extern void Func_08009128(void);
extern void Func_08015210(s32 arg0, s32 arg1, s32 arg2);
extern u8 *Func_08077008(s32 arg0);
extern void Func_0808a010(s32 frames);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0e8(s32 id);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a128(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a170(s32 textId);
extern void Func_0808a178(s32 id, s32 arg1);
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

void Func_0200178c(void)
{
    u8 *entry;
    u8 *record;
    u16 *field;
    s32 remaining;
    s32 matches;
    s32 code;

    Func_080f9010(0xa1);
    Func_0808a138(12, 3);
    Func_0808a010(40);
    entry = Func_0808a080(12);
    if (entry != 0) {
        Func_0808a0f0(13, *(s32 *)(entry + 8), *(s32 *)(entry + 16));
    }
    Func_0808a0f0(12, 0, 0);
    Func_0808a010(20);
    Func_0808a1b8(13, 0x3000, 40);
    Func_0808a138(5, 3);
    Func_0808a138(5, 3);
    Func_0808a010(40);
    Func_0808a138(5, 2);
    Func_02004248(5, 20);
    Func_0808a110(13, 3);
    Func_0808a010(10);
    Func_0808a130(5, 3);
    Func_0808a1b8(9, 0x8000, 10);
    Func_0808a138(9, 2);
    Func_02004248(9, 40);
    Func_0808a110(5, 3);
    Func_0808a010(40);
    Func_0808a1b8(9, 0xb000, 40);
    Func_0808a138(13, 2);
    Func_02004248(13, 20);
    Func_0808a138(5, 1);
    Func_0808a010(40);
    Func_0808a110(13, 3);
    Func_0808a010(10);
    Func_02004248(13, 40);
    Func_0808a130(10, 1);
    Func_0808a110(10, 3);
    Func_0808a010(10);
    Func_02004248(10, 10);
    Func_0808a138(11, 1);
    Func_0808a100(11, 3);
    Func_02004248(11, 80);
    Func_0808a138(13, 2);
    Func_02004248(13, 40);
    Func_0808a138(5, 2);
    Func_0808a010(10);
    Func_02004248(5, 10);
    Func_0808a130(13, 2);
    Func_0808a138(9, 2);
    Func_0808a010(80);
    Func_0808a110(5, 4);
    Func_0808a010(20);
    Func_02004248(5, 80);
    Func_0808a110(13, 4);
    Func_02004248(13, 80);
    Func_0808a138(5, 2);
    Func_0808a010(4);
    Func_02004248(5, 20);
    Func_0808a138(10, 1);
    Func_0808a100(10, 3);
    Func_02004248(10, 10);
    Func_0808a138(11, 1);
    Func_02004248(11, 10);
    Func_0808a138(10, 1);
    Func_02004248(10, 10);
    Func_0808a1b8(9, 0x3000, 80);
    Func_0808a1e8(9, 0x105, 80);
    Func_0808a138(11, 1);
    Func_0808a1b8(11, 0x5000, 40);
    Func_0808a130(11, 2);
    Func_02004248(11, 20);
    Func_0808a368();
    Func_0808a370();
    Func_0808a210(0x01050000, -1, 0x1d20000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);
    Func_0808a1b8(0, 0x3000, 0);
    Func_0808a1b8(1, 0xb000, 20);
    Func_0808a130(0, 2);
    Func_0808a138(1, 2);
    Func_0808a090(0, 0x8000, 0x4000);
    Func_0808a090(1, 0x8000, 0x4000);
    Func_0808a0c8(0, 244, 478);
    Func_0808a0d0(1, 260, 490);
    Func_0808a0e8(0);
    Func_0808a100(0, 1);
    Func_0808a1b8(0, 0x3000, 0);
    Func_0808a1b8(1, 0xb000, 20);
    Func_0808a138(0, 2);
    Func_0808a110(1, 4);
    Func_0808a010(20);

    /* Count roster records coded 220/221/223 in the low nine bits of
     * the 15 u16 fields at +0xd8.  Uncertainty: record family and the
     * meaning of the three code values are unresolved. */
    field = (u16 *)(Func_08077008(1) + 0xd8);
    matches = 0;
    for (remaining = 14; remaining >= 0; remaining--) {
        code = *field & 0x1ff;
        if ((u32)(code - 220) <= 1 || code == 223) {
            matches++;
        }
        field++;
    }

    Func_0808a178(1, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a170(0x10b0);
        Func_0808a110(1, 3);
        Func_0808a010(10);
        if (matches <= 2) {
            Func_02004248(1, 30);
            Func_0808a0d0(1, 252, 486);
            Func_0808a138(1, 2);
            Func_0808a010(10);
            Func_08015210(0x10b1, 1, 0);
        } else {
            Func_0808a170(0x10b4);
            Func_02004248(1, 30);
        }
    } else if (matches <= 2) {
        Func_0808a170(0x10b2);
        Func_0808a138(1, 3);
        Func_0808a110(1, 4);
        Func_02004248(1, 10);
        Func_0808a110(1, 4);
        Func_0808a138(1, 1);
        Func_0808a090(1, 0x20000, 0x10000);
        record = Func_0808a080(0);
        record[90] &= 0xfe;
        Func_0808a0d0(1, 244, 478);
        Func_0808a090(0, 0x18000, 0xc000);
        Func_0808a128(0, 6, 0);
        Func_0808a0b8(0, 218, 471);
        Func_0808a0e8(0);
        Func_08015210(0x10b3, 1, 0);
        Func_0808a138(0, 2);
        Func_0808a1b8(0, 0, 30);
        record[90] |= 1;
    } else {
        Func_0808a170(0x10b5);
        Func_0808a138(1, 3);
        Func_0808a110(1, 4);
        Func_02004248(1, 10);
        Func_0808a110(1, 4);
        Func_0808a1b8(0, 0xe000, 30);
    }

    Func_0808a208(0x8000, 0x1000);
    Func_0808a200(1, 1);
    Func_0808a218();
    Func_0808a1b8(1, 0x8000, 30);
    *(s32 *)(record + 0x30) = 0x30000;
    Func_0808a138(1, 2);
    Func_0808a090(1, 0x8000, 0x4000);
    record = Func_0808a080(1);
    record[90] &= 0xfe;
    Func_0808a0d0(1, 264, 482);
    record[90] |= 1;
    Func_0808a0d0(1, 278, 480);
    *(s32 *)(record + 0x34) = 0x20000;
    Func_080f9010(0x99);
    Func_0808a100(1, 7);
    Func_0808a0c0(1, 312, 470);
    Func_0808a100(1, 1);
    Func_0808a010(30);
    *(s32 *)(record + 0x28) = 0x60000;
    Func_080f9010(0x99);
    Func_0808a100(1, 7);
    Func_0808a0c0(1, 342, 470);
    Func_0808a100(1, 1);
    *(s32 *)(record + 0x28) = 0x60000;
    Func_0808a010(30);
    Func_080f9010(0x99);
    Func_0808a100(1, 7);
    Func_0808a0c0(1, 376, 470);
    Func_0808a100(1, 1);
    *(s32 *)(record + 0x28) = 0x60000;
}

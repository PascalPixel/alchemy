#include "types.h"

/*
 * Resource 3c2 cutscene script at 0x02000240 -- the overlay's largest owner,
 * 2,068 bytes and 252 call sites.
 *
 * Owner is complete: `push {lr}` at 0x02000240 and the single interworking
 * epilogue `pop {r0} / bx r0` at 0x02000a40, so the owner is void and takes no
 * argument (r0 is written before its first use).  Two literal pools sit inside
 * the span: 0x02000644-0x02000657 and 0x02000a44-0x02000a53.  The FIRST ENDS
 * MID-ROW between an argument setup and its own call -- r0/r1 are loaded at
 * 0x0200063e/0x02000640, the `b.n 0x02000658` at 0x02000642 jumps the pool,
 * and the `bl Func_0808a180` that consumes them is the first instruction after
 * it.  That is why the argument pair for that one site is recovered by hand
 * below rather than from the straight-line register trace.
 *
 * Link base 0x02008000 (established from the export veneer table at file
 * offset 0; see resource_3c2_c_02000a78.c).  This owner references no in-image
 * pool word: every pool word here is either a plain constant (0x98a, 0x9b0,
 * 0x25eb, 0x13333, 0x9999, 0x107, 0x101) or the IWRAM workspace pointer cell
 * 0x03001ebc.
 *
 * Control flow: one early-out on story flag 0x98a, one two-armed test on
 * Func_0808a070(0, 0) near the end, one null check on the scene record, and
 * otherwise straight-line script.
 *
 * The two arms of the Func_0808a070 test are BEHAVIOURALLY IDENTICAL and are
 * deliberately kept separate.  Each waits, repaints actor 19 and bumps the
 * u16 skip-beat counter at workspace + 472 (`movs r3,#236 / lsls r3,#1` off
 * the 0x03001ebc pointer cell -- the documented cross-overlay idiom); they
 * differ only in the wait length (20 against 10) and in where the bump sits
 * relative to the Func_0808a180 call.  Merging them would deflate the
 * per-target multiset by two Func_0808a010 and two Func_0808a180 sites.
 *
 * Completeness proof, PER TARGET rather than by total.  Every site was taken
 * from `bun tools/overlay_call_targets.ts resource_3c2 0240 --json` (site ->
 * target), never from the summary histogram, and the C below was then counted
 * per callee.  Both sides agree on all 29 targets:
 *   Func_0808a010 104, Func_0808a180 38, Func_0808a138 22, Func_0808a1b8 17,
 *   Func_0808a1e8 15, Func_0808a110 12, Func_0808a148 8, Func_0808a580 5,
 *   Func_0808a090 4, Func_0808a150 3, Func_0808a0d0 2, Func_0808a0e8 2,
 *   Func_0808a100 2, Func_0808a130 2, Func_080f9010 2, and one each of
 *   Func_080770c0, Func_080770c8, Func_0808a018, Func_0808a020, Func_0808a070,
 *   Func_0808a080, Func_0808a0b8, Func_0808a0f0, Func_0808a170, Func_0808a178,
 *   Func_0808a210, Func_0808a218, Func_0808a4f0, Func_0808a570.
 *   Total 252 = 252 sites.
 *
 * Uncertainties.  The imports' interfaces are not established beyond the
 * argument registers each call site sets, so their arities are taken per
 * target from the register writes in the window before each `bl` and are
 * uniform across this owner: Func_0808a010/0170/0080/0e8/080f9010/080770c0/
 * 080770c8 take one, Func_0808a018/0020/0218/04f0 take none, Func_0808a210 and
 * Func_0808a570 take four, and the rest take two or three as written.
 * Func_0808a010's single argument is a frame count (10/14/20/28/30 recur), and
 * 19/20/21 recur as actor or slot ids, but neither reading is proven here.
 * The large literals 0x13333 and 0x9999 are 16.16 fixed-point (1.2 and 0.6).
 */

/* Overlay imports (through the veneer table at file offset 0x0b34).  Old-style
 * declarations: overlay imports vary their argument count between call sites,
 * and two names can be one import. */
s32 Func_080770c0();
void Func_080770c8();
void Func_080f9010();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a210();
void Func_0808a218();
void Func_0808a4f0();
void Func_0808a570();
void Func_0808a580();

/* Pointer CELL, not the workspace itself: the original does
 * `ldr r3,[pc] / ldr r2,[r3]`. */
extern u8 *Data_03001ebc;

void Func_02000240(void)
{
    u16 *counter;
    u8 *record;

    Func_080770c8(0x9b0);

    if (Func_080770c0(0x98a) != 0) {
        return;
    }

    Func_080f9010(30);
    Func_0808a018();
    Func_0808a210(0x1700000, -1, 0x680000, 1);
    Func_0808a0d0(0, 368, 160);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a570(19, 0, -16, 0xc000);
    Func_0808a0e8(19);
    Func_0808a218();
    Func_0808a170(0x25eb);
    Func_0808a010(10);
    Func_0808a138(20, 2);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a138(19, 2);
    Func_0808a010(20);
    Func_0808a090(19, 0x13333, 0x9999);
    Func_0808a580(19, 0, -16);
    Func_0808a1b8(19, 0, 0);
    Func_0808a010(30);
    Func_0808a1b8(19, 0xe000, 0);
    Func_0808a010(30);
    Func_0808a1b8(19, 0, 0);
    Func_0808a010(30);
    Func_0808a1e8(19, 256, 40);
    Func_0808a180(19, 0);
    Func_0808a010(10);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a090(19, 0x13333, 0x9999);
    Func_0808a580(19, 0, -24);
    Func_0808a580(19, 48, 0);
    Func_0808a1b8(19, 0xc000, 0);
    Func_0808a010(30);
    Func_0808a180(19, 0);
    Func_0808a010(20);
    Func_0808a1e8(19, 256, 40);
    Func_0808a180(19, 0);
    Func_0808a010(20);
    Func_0808a1e8(20, 258, 40);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a1b8(19, 0x2000, 0);
    Func_0808a010(20);
    Func_0808a180(19, 0);
    Func_0808a010(10);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a110(21, 4);
    Func_0808a010(20);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a1b8(19, 0xc000, 0);
    Func_0808a010(30);
    Func_0808a1e8(19, 263, 40);
    Func_0808a180(19, 0);
    Func_0808a010(10);
    Func_0808a130(20, 2);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a090(0, 0x13333, 0x9999);
    Func_0808a0d0(0, 368, 104);
    Func_0808a580(0, 16, 0);
    Func_0808a1b8(0, 0, 0);
    Func_0808a010(20);
    Func_0808a010(10);
    Func_0808a110(19, 4);
    Func_0808a010(20);
    Func_0808a180(19, 0);
    Func_0808a010(20);
    Func_0808a1e8(19, 258, 50);
    Func_0808a180(19, 0);
    Func_0808a010(10);
    Func_0808a138(20, 2);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a148(0, 20, 0);
    Func_0808a1b8(19, 0x2000, 0);
    Func_0808a010(20);
    Func_0808a180(19, 0);
    Func_0808a010(10);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a148(0, 21, 0);
    Func_0808a1b8(19, 0xc000, 0);
    Func_0808a010(40);
    Func_0808a110(19, 3);
    Func_0808a010(30);
    Func_0808a180(19, 0);
    Func_0808a010(10);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a110(21, 4);
    Func_0808a010(20);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a150(19, 0, 30);
    Func_0808a180(19, 0);
    Func_0808a010(10);
    Func_0808a138(20, 2);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a148(0, 20, 0);
    Func_0808a1b8(19, 0x2000, 0);
    Func_0808a010(40);
    Func_0808a010(10);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a148(0, 21, 0);
    Func_0808a1b8(19, 0xc000, 0);
    Func_0808a010(40);
    Func_0808a100(0, 3);
    Func_0808a110(19, 3);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a110(20, 4);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a1e8(0, 256, 0);
    Func_0808a1e8(19, 256, 40);
    Func_0808a010(10);
    Func_0808a148(0, 20, 0);
    Func_0808a1b8(19, 0x2000, 0);
    Func_0808a010(40);
    Func_0808a010(10);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a1e8(0, 256, 50);
    Func_0808a148(0, 21, 0);
    Func_0808a010(30);
    Func_0808a138(21, 2);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a1b8(19, 0xc000, 0);
    Func_0808a010(30);
    Func_0808a180(19, 0);
    Func_0808a010(10);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a110(21, 3);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a1e8(19, 258, 40);
    Func_0808a180(19, 0);
    Func_0808a010(10);
    Func_0808a110(20, 4);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a1e8(19, 257, 50);
    Func_0808a010(10);
    Func_0808a148(0, 20, 0);
    Func_0808a1b8(19, 0x2000, 0);
    Func_0808a010(20);
    Func_0808a010(10);
    Func_0808a138(20, 2);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a1e8(19, 256, 40);
    Func_0808a180(19, 0);
    Func_0808a010(10);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a180(21, 0);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a010(10);
    Func_0808a150(19, 0, 0);
    Func_0808a010(30);
    Func_0808a180(19, 0);
    Func_0808a010(10);
    Func_0808a110(20, 4);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a1e8(0, 258, 0);
    Func_0808a1e8(19, 258, 80);
    Func_0808a1e8(21, 258, 50);
    Func_0808a180(21, 0);
    Func_0808a010(10);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a180(21, 0);
    Func_0808a138(21, 3);
    Func_0808a010(20);
    Func_0808a010(10);
    Func_0808a1b8(19, 0xc000, 0);
    Func_0808a010(30);
    Func_0808a138(19, 2);
    Func_0808a010(10);
    Func_0808a180(19, 0);
    Func_0808a010(10);
    Func_0808a138(20, 2);
    Func_0808a010(20);
    Func_0808a180(20, 0);
    Func_0808a010(10);
    Func_0808a148(0, 20, 0);
    Func_0808a1b8(19, 0x2000, 0);
    Func_0808a010(30);
    Func_0808a110(19, 3);
    Func_0808a010(30);
    Func_0808a180(19, 0);
    Func_0808a010(10);
    Func_0808a130(20, 2);
    Func_0808a138(21, 2);
    Func_0808a010(30);
    Func_0808a010(10);
    Func_0808a150(19, 0, 20);
    Func_0808a580(19, -12, 0);
    Func_0808a010(20);
    Func_0808a178(19, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_0808a180(19, 0);
        counter = (u16 *)(Data_03001ebc + 472);
        *counter = (u16)(*counter + 1);
    } else {
        Func_0808a010(10);
        counter = (u16 *)(Data_03001ebc + 472);
        *counter = (u16)(*counter + 1);
        Func_0808a180(19, 0);
    }

    Func_0808a010(10);
    Func_0808a138(19, 2);
    Func_0808a010(20);
    Func_0808a180(19, 0);
    Func_0808a010(10);
    Func_0808a1e8(19, 258, 50);
    Func_0808a180(19, 0);
    Func_0808a010(10);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a110(19, 3);
    Func_0808a010(30);
    Func_080f9010(30);
    Func_0808a090(19, 0x13333, 0x9999);
    Func_0808a100(19, 2);

    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0b8(19, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }

    Func_0808a0e8(19);
    Func_0808a0f0(19, 0, 0);
    Func_0808a010(10);
    Func_0808a4f0();
    Func_0808a020();
}

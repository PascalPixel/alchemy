#include "types.h"

/*
 * resource_380 owner at 0x02000a98, 708 bytes: a three-panel scene
 * build.  A six-pass warm-up loop (with one extra centering call on
 * pass 1), then three near-identical panel blocks -- each one places a
 * layer, loads a strip from the resident table 0x0200d088 (rows 65/31,
 * 79/9, 91/10), configures it with the same 87/42 parameter shape at
 * increasing offsets (10/33, 24/9-11, 36/12), waits 40, and fades in.
 * After the third block it writes the Data_03001ebc counter idiom
 * (word at +448 = 514), runs a settle sequence, spawns object 223 at a
 * fixed coordinate triple through the in-overlay Func_02004260, and
 * hands it plus id 0x1077 to the closer.
 *
 * Complete owner: `push {r5,r6,lr}` + r8/sl spill at 0x02000a98
 * through `pop {r0} / bx r0` at 0x02000d22, then the literal pool
 * 0x02000d24-0x02000d5b; next owner's prologue at 0x02000d5c (this
 * overlay's own resource_380_c_02000d5c.c, drafted alongside).
 *
 * CORRECTION (name sweep): this file's callee names came from a naive
 * pc-relative decode and were wrong; they are resolved here through
 * the overlay's import-veneer table under the +2 rule
 * (tools/lib/overlay_call_targets.ts) to their main-ROM identities.  The
 * earlier note that three callees were "called at two arities inside
 * this one owner" was an artifact of that decode -- each of those
 * phantom names covered two genuinely different functions, which is
 * why the arities disagreed.  Nothing here is called at two arities or
 * declared old-style.
 *
 * Uncertainty: callee roles beyond call shape remain open.  The
 * 0x004049d2 loop constant and the fixed-point pool values are
 * recorded as passed.
 */

extern s32 Func_02004260(s32 template_id, s32 x, s32 y, s32 z);
extern void Func_080000c0(s32 arg0);
extern void Func_08009128(void);
extern void Func_08009178(s32 table, s32 arg1, s32 arg2);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080091f8(void);
extern void Func_08015040(s32 arg0, s32 arg1);
extern void Func_08015120(s32 arg0, s32 arg1);
extern void Func_0808a010(s32 arg0);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_0808a330(s32 arg0, s32 arg1);
extern void Func_0808a348(s32 arg0);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_080f9010(s32 arg0);
extern void Func_08009180(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void Func_02000a98(void)
{
    s32 table = 0x0200d088;
    s32 object;
    u8 *record;
    s32 pass;

    Func_080f9010(141);
    for (pass = 0; pass != 6; pass++) {
        Func_0808a330(0x4049d2, 1);
        Func_0808a348(8);
        Func_0808a010(8);
        Func_0808a330(0x10000, 1);
        Func_0808a348(8);
        Func_0808a010(8);
        if (pass == 1)
            Func_080091f0(0x10000, 0x10000, 0x10000);
    }
    Func_080091f0(0x20000, 0x10000, 0x10000);
    Func_0808a010(30);
    Func_0808a208(0x26666, 0x4ccc);
    Func_0808a210(0xa70000, -1, 0x02110000, 1);
    Func_0808a218();
    Func_080091f0(0x10000, 0x20000, 0x10000);
    Func_0808a010(20);

    Func_080f9010(144);
    Func_08009178(table, 65, 31);
    Func_080091c0(0, 0, 1, 1, 10, 31);
    Func_08009180(87, 42, 10, 33, 1, 2);
    Func_0808a010(40);
    Func_080091f0(0, 0, 0);
    Func_0808a208(0x66666, 0xcccc);
    Func_0808a210(0x01870000, -1, 0xb10000, 1);
    Func_0808a218();
    Func_080091f0(0x10000, 0x20000, 0x10000);
    Func_0808a010(20);

    Func_080f9010(144);
    Func_08009178(table, 79, 9);
    Func_080091c0(0, 0, 1, 1, 24, 9);
    Func_08009180(87, 42, 24, 11, 1, 2);
    Func_0808a010(40);
    Func_080091f0(0, 0, 0);
    Func_0808a208(0x26666, 0x4ccc);
    Func_0808a210(0x02470000, -1, 0xc10000, 1);
    Func_0808a218();
    Func_080091f0(0x10000, 0x20000, 0x10000);
    Func_0808a010(20);

    Func_080f9010(144);
    Func_08009178(table, 91, 10);
    Func_080091c0(0, 0, 1, 1, 36, 10);
    Func_08009180(87, 42, 36, 12, 1, 2);
    Func_0808a010(40);

    record = *(u8 **)0x03001ebc;
    *(s32 *)(record + 448) = 514;
    Func_0808a368();
    Func_0808a370();
    Func_0808a210(0xe80000, -1, 0x01dd0000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_080091f0(0x20000, 0x10000, 0x10000);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(40);
    Func_080f9010(0x121);
    Func_080091f0(-1, -1, 0xe666);
    Func_080091f8();
    Func_0808a010(20);
    Func_08009180(0, 40, 13, 66, 3, 3);
    Func_0808a010(20);
    object = Func_02004260(223, 0xe80000, 0x100000, 0x1d00000);
    Func_0808a010(40);
    Func_08015120(object, 1);
    Func_08015040(0x1077, 1);
}

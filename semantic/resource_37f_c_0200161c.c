#include "types.h"

/*
 * Resource 37f, owner at 0x0200161c (376 bytes of code, `push {lr}` at
 * 0x0200161c through the interworking return `pop {r0} / bx r0` at
 * 0x02001790, plus an eleven-word literal pool at 0x02001794-0x020017bf).
 * r0 is the popped return address, so the owner is `void`.
 *
 * All 41 call sites were resolved with `tools/lib/overlay_call_targets.ts`; all 41
 * are import veneers.  The inventory's `calls=39` is the documented undercount
 * of that field.  Control flow is straight-line apart from two identical null
 * guards on the Func_0808a080(0) record, so every call below appears once.
 *
 * Every pool word here is a plain constant, including 0x00004ccc, which
 * decodes as `ldr r4, [pc, #816]` and is the documented pool-word-as-
 * instruction trap; the control-flow walk stops at the `bx r0`, so the whole
 * 0x02001794..0x020017bf range is data.
 *
 * `Data_03001ebc` is the overlay workspace pointer (proven by the byte-exact
 * `assets/code/resource_37f_c_0200079c.c`).  The store is
 * `*(s32 *)(ws + 448) = 516`: r2 is first the displacement 448 and then, after
 * `adds r2, #68`, the stored value 516 — the same register-reuse shape as
 * 0x0200092c, spelled explicitly here.
 *
 * Record fields: +8 and +16 are the 16.16 X and Z words, +10 and +18 their
 * s16 integer parts, which is why the two guarded calls read the same record
 * two different ways.
 */

extern u8 *Data_03001ebc;

void Func_020031cc();
void Func_020032d6();
void Func_020032ea();
void Func_020031e0();
s32 Func_0200320e();
void Func_0200324c();
void Func_02003232();
void Func_0200326a();
void Func_0200325e();
void Func_0200326c();
void Func_02003284();
void Func_0200322a();
void Func_020032ee();
void Func_020032fa();
void Func_02003322();
void Func_0200333c();
void Func_02003348();
void Func_02003266();
void Func_02003346();
void Func_02003360();
void Func_0200336c();
void Func_0200328a();
void Func_0200336a();
void Func_02003382();
void Func_0200338e();
void Func_020032ac();
void Func_02003334();
void Func_02003376();
void Func_02003354();
void Func_020033a2();
void Func_02003362();
void Func_020033a6();
void Func_020033a0();
s32 Func_0200332e();
void Func_02003350();
void Func_0200336e();
void Func_02003380();
void Func_0200331e();
void Func_02003342();

                     

                     

                     

                     

                     

                     

                     

void Func_0200161c(void)
{
    u8 *workspace;
    s32 *record;

    Func_020031cc();

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 516;

    Func_020032d6();
    Func_020032ea();
    Func_020031e0(20);

    record = (s32 *)Func_0200320e(0);
    if (record != 0) {
        Func_0200324c(8, record[2], record[4]);
    }

    Func_02003232(8, 0x10000, 0x8000);
    Func_0200326a(8, 2);
    Func_0200325e(8, 24, -10);
    Func_0200326c(8);
    Func_02003284(8, 1);
    Func_0200322a(6);

    Func_020032ee(8, 0xb000, 0);
    Func_020032fa(0, 0xc000, 40);

    Func_02003322(0x26666, 0x4ccc);
    Func_0200333c(0x6880000, -1, 0x20c0000, 1);
    Func_02003348();
    Func_02003266(20);

    Func_02003346(0x19999, 0x3333);
    Func_02003360(0x7580000, -1, 0x20c0000, 1);
    Func_0200336c();
    Func_0200328a(20);

    Func_0200336a(0x33333, 0x6666);
    Func_02003382(0x6e90000, -1, 0x2240000, 1);
    Func_0200338e();
    Func_020032ac(20);

    Func_02003334(8, 2);
    Func_02003376(8, 0, 30);
    Func_02003354(0x103a);
    Func_02003376(0x4008, 0, 10);
    Func_020033a2(8, 256, 40);

    Func_02003362(8, 1);
    Func_020033a6(8, 0x5000, 20);
    Func_020033a0(0x4008, 0, 10);
    Func_02003360(8, 2);

    record = (s32 *)Func_0200332e(0);
    if (record != 0) {
        Func_02003350(8, *(s16 *)((u8 *)record + 10), *(s16 *)((u8 *)record + 18));
    }

    Func_0200336e(8);
    Func_02003380(8, 0, 0);

    Func_0200331e(0x825);
    Func_02003342();
}

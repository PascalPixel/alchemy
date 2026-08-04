#include "types.h"

/*
 * Resource 37f, owner at 0x0200161c (376 bytes of code, `push {lr}` at
 * 0x0200161c through the interworking return `pop {r0} / bx r0` at
 * 0x02001790, plus an eleven-word literal pool at 0x02001794-0x020017bf).
 * r0 is the popped return address, so the owner is `void`.
 *
 * All 41 call sites were resolved with `tools/overlay_call_targets.ts`; all 41
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

s32 Func_0808a080();
void Func_0808a018();
void Func_0808a360();
void Func_0808a370();
void Func_0808a010();
void Func_0808a0f0();
void Func_0808a090();
void Func_0808a100();
void Func_0808a0e0();
void Func_0808a0e8();
void Func_0808a1b8();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a138();
void Func_0808a170();
void Func_0808a188();
void Func_0808a1e8();
void Func_0808a0b8();
void Func_080770c8();
void Func_0808a020();

void Func_0200161c(void)
{
    u8 *workspace;
    s32 *record;

    Func_0808a018();

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 516;

    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);

    record = (s32 *)Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(8, record[2], record[4]);
    }

    Func_0808a090(8, 0x10000, 0x8000);
    Func_0808a100(8, 2);
    Func_0808a0e0(8, 24, -10);
    Func_0808a0e8(8);
    Func_0808a100(8, 1);
    Func_0808a010(6);

    Func_0808a1b8(8, 0xb000, 0);
    Func_0808a1b8(0, 0xc000, 40);

    Func_0808a208(0x26666, 0x4ccc);
    Func_0808a210(0x6880000, -1, 0x20c0000, 1);
    Func_0808a218();
    Func_0808a010(20);

    Func_0808a208(0x19999, 0x3333);
    Func_0808a210(0x7580000, -1, 0x20c0000, 1);
    Func_0808a218();
    Func_0808a010(20);

    Func_0808a208(0x33333, 0x6666);
    Func_0808a210(0x6e90000, -1, 0x2240000, 1);
    Func_0808a218();
    Func_0808a010(20);

    Func_0808a138(8, 2);
    Func_0808a1b8(8, 0, 30);
    Func_0808a170(0x103a);
    Func_0808a188(0x4008, 0, 10);
    Func_0808a1e8(8, 256, 40);

    Func_0808a138(8, 1);
    Func_0808a1b8(8, 0x5000, 20);
    Func_0808a188(0x4008, 0, 10);
    Func_0808a100(8, 2);

    record = (s32 *)Func_0808a080(0);
    if (record != 0) {
        Func_0808a0b8(8, *(s16 *)((u8 *)record + 10), *(s16 *)((u8 *)record + 18));
    }

    Func_0808a0e8(8);
    Func_0808a0f0(8, 0, 0);

    Func_080770c8(0x825);
    Func_0808a020();
}

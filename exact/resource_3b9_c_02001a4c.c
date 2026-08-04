#include "types.h"
extern u8 Data_02000240[];

/*
 * resource_3b9 owner at 0x02001a4c, 508 bytes: the parked 66-entry
 * message/event dispatcher.  Reads the s16 selector at 0x02000240+450
 * (0x020003c2), subtracts 5, and dispatches through the `mov pc, r3`
 * jump table at 0x02001a70 (link base 0x02009a70, the table's own pool
 * word being the free link-base witness per HANDOVER).  The 66 entries
 * collapse to 12 real arms plus the shared exit at 0x02001c2e:
 * selectors 5, 7, 12, 21, 31, 64, 65, 66, 67, 68, 69, 70 act;
 * everything else falls straight through.
 *
 * Complete owner: `push {lr}` at 0x02001a4c through `pop {r0} / bx r0`
 * at 0x02001c2e-0x02001c30, alignment halfword, then the literal pool
 * 0x02001c34-0x02001c47 (0x02000240, 0x02009a70, 0x109, 0x90e, 0x90f);
 * the next owner's prologue is at 0x02001c48.
 *
 * The arm bodies at 0x02001b78-0x02001c2d were left as raw `.4byte`
 * data by the reconstructed disassembly (reachable only through the
 * `mov pc` table, so the control-flow walk never entered them); they
 * were recovered here by re-disassembling those 182 bytes as Thumb at
 * their runtime address, which is where the twelve arms and all their
 * call targets below come from.
 *
 * CASE ORDER IS LOAD-BEARING, and it is not the selector order.  The arms
 * are written 5, 69, 7, 70, 64, 65, 66, 12, 21, 67, 68, 31 because that is
 * the order the reference lays their bodies out, and the jump table stores
 * their absolute addresses: written in ascending selector order the row is
 * 164 bytes wrong, and every one of those bytes is layout, not content.
 * The callee names are keyed to the reference's instruction addresses (the
 * `name = insn + 2 + target_offset` identity), so putting each arm back at
 * its reference address is what makes the names emit the right bl bytes --
 * the ordering and the naming are one fix, not two.
 *
 * Uncertainty: none of the callees are identified beyond call shape.
 * Func_020047ec is called with two arguments by selector 69 and with
 * one by selector 31, so it is declared old-style per the
 * resource_3b9_c_02002668.c precedent.  The 0x109 status id gates both
 * selector 12 and selector 69 (call first, act only when the check
 * returns zero); 0x90e/0x90f are passed ids from the pool.
 */

extern void Func_020045de(s32 arg0);
extern void Func_020047da(s32 arg0, s32 arg1);
extern void Func_020047e2(s32 arg0, s32 arg1);
extern void Func_020047ec();
extern void Func_020047f4(s32 arg0, s32 arg1);
extern s32 Func_0200475a(s32 arg0);
extern void Func_020031b2(void);
extern void Func_02002e44(void);
extern void Func_0200323a(void);
extern void Func_0200388c(void);
extern void Func_020047a0(void);
extern void Func_02003fa2(void);
extern void Func_020041b8(void);
extern void Func_02004796(s32 arg0);
extern void Func_020046f2(void);
extern s32 Func_02004798(s32 arg0);
extern void Func_020040bc(void);
extern void Func_020047c0(s32 arg0);
extern void Func_020047c6(s32 arg0);
extern void Func_020047cc(s32 arg0);
extern void Func_020047c2(s32 arg0);
extern void Func_0200426a(void);
extern void Func_02004428(void);
extern void Func_02004512(void);
extern void Func_020047ea(s32 arg0);
extern void Func_020047f0(s32 arg0);
extern void Func_020047f6(s32 arg0);
extern void Func_02004590(void);

void Func_02001a4c(void)
{
    s32 selector;

    Func_020045de(1);
    {
        s32 off = 450;

        selector = *(s16 *)(Data_02000240 + off);
    }
    switch (selector) {
    case 5:
        Func_020047da(8, 2);
        Func_020047e2(9, 2);
        break;
    case 69:
        Func_020047ec(8, 2);
        Func_020047f4(9, 2);
        if (Func_0200475a(0x109) != 0)
            break;
        Func_020031b2();
        break;
    case 7:
        Func_02002e44();
        break;
    case 70:
        Func_0200323a();
        break;
    case 64:
        Func_0200388c();
        Func_020047a0();
        break;
    case 65:
        Func_02003fa2();
        break;
    case 66:
        Func_020041b8();
        break;
    case 12:
        Func_02004796(324);
        Func_020046f2();
        if (Func_02004798(0x109) != 0)
            break;
        Func_020040bc();
        break;
    case 21:
        Func_020047c0(1);
        Func_020047c6(2);
        Func_020047cc(3);
        Func_020047c2(0x90e);
        Func_0200426a();
        break;
    case 67:
        Func_02004428();
        break;
    case 68:
        Func_02004512();
        break;
    case 31:
        Func_020047ea(1);
        Func_020047f0(2);
        Func_020047f6(3);
        Func_020047ec(0x90f);
        Func_02004590();
        break;
    default:
        break;
    }
}

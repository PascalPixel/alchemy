#include "types.h"

/*
 * resource_377 owner at 0x0200133c, 120 bytes.  Complete owner: 'push {r5, lr}'
 * prologue and the interworking return 'pop {r5} ; pop {r0} ; bx r0'.  r0 holds
 * the popped return address, so the owner returns nothing.  Nothing reads r0 on
 * entry -- the first call is the argument-less scene bracket -- so the owner
 * takes no argument.
 *
 * One scene beat with two variants selected by progress flag 0x203, bracketed by
 * the established Func_0808a018 / Func_0808a020 pair.
 *
 * Literal pool: owner offsets 0x68..0x77, preceded by an alignment 'movs r0,r0'
 * at 0x020013a2.  The control-flow walk from the prologue ends at the epilogue
 * (0x0200139c), so the tail is entirely pool:
 *   0x00000203  progress-flag id
 *   0x02009e6c  even in-image address -> file offset 0x1e6c under this family's
 *               proven 0x02008000 link base, i.e. a data/script block, matching
 *               the parity rule and the usual role of the last Func_0808a168
 *               argument
 *   0x00001c77, 0x00001c79  cue ids; the second is kept live in r5 and bumped to
 *               0x1c7a for the final call, which is why it is loaded early
 *
 * All twelve calls resolved with
 * 'bun tools/lib/overlay_call_targets.ts resource_377 133c --json' (12 sites,
 * 10 distinct veneer targets; Func_0808a010 and Func_0808a170 twice each, which
 * is exactly the multiset written below):
 *   0x133e -> 0x190c -> Func_0808a018()
 *   0x1344 -> 0x18ec -> Func_080770c0(0x203)          result tested
 *   0x1354 -> 0x19bc -> Func_0808a168(8, 0x10000, 0x02009e6c)
 *   0x135a -> 0x1904 -> Func_0808a010(20)
 *   0x1360 -> 0x19c4 -> Func_0808a170(0x1c77)
 *   0x1368 -> 0x19d4 -> Func_0808a180(8, 0)
 *   0x1372 -> 0x199c -> Func_0808a138(8, 2)
 *   0x1378 -> 0x1904 -> Func_0808a010(40)
 *   0x1380 -> 0x19c4 -> Func_0808a170(0x1c79)
 *   0x138c -> 0x19dc -> Func_0808a188(8, 0, 40)
 *   0x1394 -> 0x18d4 -> Func_08015040(0x1c7a, 1)
 *   0x1398 -> 0x1914 -> Func_0808a020()
 * The disassembler's own 'bl' annotations are wrong, as on every overlay.
 *
 * 'movs r1,#128 ; lsls r1,r1,#9' builds 0x10000, i.e. 1.0 in the family's 16.16
 * fixed-point convention.
 */












extern void Func_02002c4c();
extern s32 Func_02002c32();
extern void Func_02002d12();
extern void Func_02002c60();
extern void Func_02002d26();
extern void Func_02002d3e();
extern void Func_02002d10();
extern void Func_02002c7e();
extern void Func_02002d46();
extern void Func_02002d6a();
extern void Func_02002c6a();
extern void Func_02002cae();
void Func_0200133c(void)
{
    Func_02002c4c();

    if (Func_02002c32(0x203) != 0) {
        Func_02002d12(8, 0x10000, 0x02009E6C);
        Func_02002c60(20);
        Func_02002d26(0x1C77);
        Func_02002d3e(8, 0);
    } else {
        s32 cue;

        Func_02002d10(8, 2);
        Func_02002c7e(40);
        cue = 0x1C79;
        Func_02002d46(cue);
        Func_02002d6a(8, 0, 40);
        Func_02002c6a(cue + 1, 1);
    }

    Func_02002cae();
}

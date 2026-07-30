typedef signed short s16;
typedef signed int s32;

/*
 * Resource 37f, owner at 0x020008f4 (42 bytes of code + a three-word literal
 * pool at 0x02000920-0x0200092b).  `push {lr}` at 0x020008f4, interworking
 * return `pop {r1} / bx r1` at 0x0200091a: the popped register is r1, so r0
 * survives and IS the result — and r0 is set to 0 at the join, so this owner
 * always returns 0.
 *
 * Both call targets were resolved with `tools/overlay_call_targets.ts`; they
 * are the overlay's own prologues at 0x0200092c and 0x02000a24, not veneers.
 *
 * `Data_02000240` is the same scenario-state array the byte-exact sibling
 * `assets/code/resource_37f_c_02000030.c` reads: it selects on element 224
 * against the same two ids, 0x13 and 0x10.  The disassembly forms the address
 * as base + 448 with an `ldrsh` at index 0, i.e. the s16 at [224].
 */

extern s16 Data_02000240[];

void Func_0200092c();
void Func_02000a24();

s32 Func_020008f4(void)
{
    s16 scenario = Data_02000240[224];

    if (scenario == 0x13) {
        Func_0200092c();
    } else if (scenario == 0x10) {
        Func_02000a24();
    }

    return 0;
}

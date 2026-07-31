typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

/*
 * resource_3bc owner at 0x020038dc, 30 bytes: reset the same four scene
 * record fields resource_3bc_c_0200381c.c resets (+36, +44 to 0, +56, +64
 * to 0x80000000) after an unestablished no-arg helper call.
 *
 * Complete owner: `push {r5, lr}` at 0x020038dc through the matching
 * interworking return `pop {r5} / pop {r0} / bx r0` at 0x20038f6-0x20038fa.
 * No literal pool. One argument (r0), void.
 *
 * Not found by the structural inventory walk (unindexed); one incoming call
 * site per overlay_unindexed.ts. This owner makes two outgoing calls, both
 * resolved with the `+2` rule against the raw image (this owner starts
 * outside a recorded row, so overlay_call_targets.ts's own table does not
 * cover it -- resolved with `bun tools/overlay_show.ts resource_3bc 38dc |
 * bun tools/overlay_call_targets.ts resource_3bc 38dc 38fa --annotate`):
 *   0x20038de -> veneer -> Func_020082f8(selector)   scene-record accessor, established
 *   0x20038e4 -> veneer -> Func_08009140()           unestablished, reached with the leftover record pointer in r0 but no evidence it is read
 */

u8 *Func_020082f8();           /* scene-record accessor, established (veneer to Func_0808a080) */
void Func_020081e6();          /* unestablished */

void Func_020038dc(s32 selector)
{
    u8 *record;

    record = Func_020082f8(selector);
    Func_020081e6();

    *(u32 *)(record + 36) = 0;
    *(u32 *)(record + 44) = 0;
    *(u32 *)(record + 56) = 0x80000000;
    *(u32 *)(record + 64) = 0x80000000;
}

#include "types.h"

/*
 * resource_3bc owner at 0x02003ddc, 276 bytes: on first call (a story flag
 * gates it), spawn the current scene record at a position computed from
 * this overlay's other workspace, attach an animation script, wait for the
 * spawn to settle, then release one of the two caller-supplied handles
 * (chosen by the same flag) and refresh two more handles. Returns the
 * gating flag's value.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/sl saves at
 * 0x02003ddc, through the matching interworking return `pop {r3, r5} /
 * mov r8,r3 / mov sl,r5 / pop {r5, r6, r7} / pop {r1} / bx r1` at
 * 0x2003ec6-0x2003ed0. The popped return address goes to r1, so r0 (the
 * gating flag, never overwritten after it is first read) survives -- this
 * owner returns s32. Two arguments (r0, r1).
 *
 * Not found by the structural inventory walk (unindexed); one incoming call
 * site per overlay_unindexed.ts. Thirteen outgoing calls over nine
 * distinct targets, all resolved with the `+2` rule against the raw image
 * (this owner starts outside a recorded row, so overlay_call_targets.ts's
 * own table does not cover it -- resolved with `bun tools-rs/overlay-show
 * resource_3bc 3ddc | bun tools/lib/overlay_call_targets.ts resource_3bc 3ddc
 * 3ed0 --annotate`):
 *   0x2003dee -> veneer -> Func_080770c0(0x211)        story-flag query, established
 *   0x2003dfe -> veneer -> Func_0808a080(selector)      scene-record accessor, established
 *   0x2003e58 -> veneer -> Func_08009150(record, x, y, z)   established, move a record to a position
 *   0x2003e5e -> veneer -> Func_080770c8(0x211)         set the same story flag just queried, established
 *   0x2003e66 -> veneer -> Func_08009098(record, script)   established, "attach an animation script" (resource_3a7_c_02000b8c.c, resource_39a_c_02001c18.c, resource_3c8_c_02000118.c)
 *   0x2003e74 -> veneer -> Func_080000c0(1)              established, advance the task scheduler
 *   0x2003e88, 0x2003e9a -> raw prologue -> Func_02002e18(0, handle)   sibling item-28 owner drafted earlier in the reconstruction (resource_3bc_c_02002e18.c)
 *   0x2003e90, 0x2003ea2, 0x2003eb2 -> veneer -> Func_08015120(handle, mode)   established, release/select by handle
 *   0x2003eba -> veneer -> Func_08015040(0x96a, 3)       established shape, id + mode (resource_37f/38b/383)
 *   0x2003ec0 -> veneer -> Func_08009148(record)         unestablished, single argument
 *
 * 0x03001f3c is the second overlay workspace pointer this reconstruction already
 * saw in resource_3bc_c_0200457c.c; this owner reads its +226/+228 (u16),
 * +232 and +236 (s32) fields.  Data_02000240+500 is the established
 * subject-selector idiom, read here twice (fresh each time, matching the
 * "no aliasing assumed across an intervening call" caution other units in
 * this queue already note).
 */

extern u8 Data_02000240[];

s32 Func_080770c0();           /* story-flag query, established */
void Func_080770c8();          /* set a story flag, established */
u8 *Func_0808a080();           /* scene-record accessor, established */
void Func_08009150();          /* established (record, x, y, z) */
void Func_08009098();          /* established (record, script) */
void Func_080000c0();          /* established, advance the task scheduler */
void Func_02002e18();          /* sibling item-28 owner, same overlay */
void Func_08015120();          /* established (handle, mode) */
void Func_08015040();          /* established (id, mode) */
void Func_08009148();          /* unestablished, single argument */

s32 Func_02003ddc(s32 handleA, s32 handleB)
{
    u8 *workspace;
    u8 *record;
    s32 flag;
    s32 selector;
    s32 xVal;
    s32 yVal;
    u16 cue;

    s32 permuted_3;
    s32 permuted_4;
    workspace = *(u8 **)0x03001f3c;

    flag = Func_080770c0(0x211);

    record = Func_0808a080(selector);
    selector = *(s32 *)&Data_02000240[500];

    if (*(s32 *)(workspace + 232) < *(s32 *)(record + 8)) {
        xVal = *(s32 *)(workspace + 232) + 0xc0000;
    } else {
        xVal = *(s32 *)(workspace + 232) - 0xc0000;
    }

    if (flag != 0) {
        yVal = *(s32 *)(workspace + 236) + 0x100000;
        cue = *(u16 *)(workspace + 228);
    } else {
        permuted_3 = *(s32 *)(workspace + 236) - 0x100000;
        permuted_4 = *(u16 *)(workspace + 226);
        yVal  = permuted_3;
        cue  = permuted_4;
    }

    *(s32 *)(record + 52) = 0x4000;
    *(s16 *)(record + 100) = (s16)cue;
    *(s32 *)(record + 48) = 0x10000;

    Func_08009150(record, xVal, 0, yVal);
    Func_080770c8(0x211);
    Func_08009098(record, 0x0200db24);

    while (*(s16 *)(record + 100) != 0) {
        Func_080000c0(1);
    }

    if (flag == 0) {
        Func_02002e18(0, handleA);
        Func_08015120(handleA, 2);
    } else {
        Func_02002e18(0, handleB);
        Func_08015120(handleB, 2);
    }

    Func_08015120(selector, 1);
    selector = *(s32 *)&Data_02000240[500];
    Func_08015040(0x96a, 3);
    Func_08009148(record);

    return flag;
}

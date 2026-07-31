typedef unsigned short u16;

/*
 * resource_3a4 owner at 0x02003a44, 44 bytes: clear two RAM globals and
 * install a per-frame task at the standard 200-frame rate.
 *
 * Complete owner: `push {lr}` at 0x02003a44 through the matching
 * interworking return `pop {r0} / bx r0` at 0x02003a6c-0x02003a6e,
 * followed by the four-word literal pool 0x02003a5c-0x02003a6b that closes
 * the 44-byte span. No arguments, void.
 *
 * Not found by the structural inventory walk (unindexed); three incoming
 * call sites per overlay_unindexed.ts. This owner makes one outgoing
 * call, resolved with the `+2` rule against the raw image (this owner
 * starts outside a recorded row, so overlay_call_targets.ts's own table
 * does not cover it -- resolved with `bun tools/overlay_show.ts
 * resource_3a4 3a44 | bun tools/overlay_call_targets.ts resource_3a4 3a44
 * 3a6e --annotate`):
 *   0x2003a56 -> veneer -> Func_080000d0(0x0200b9c9, 0xc80)   install a per-frame task, established, the standard 200-frame rate
 *
 * Data_0200d260 and Data_0200d25c are not otherwise cross-referenced from
 * this owner alone.
 */

typedef signed int s32;
extern unsigned char Value_00000000;
extern u16 Data_0200d260;
extern u16 Data_0200d25c;

s32 Func_020074d8();          /* install a per-frame task, established (veneer to Func_080000d0) */

void Func_02003a44(void)
{
    s32 zero = (s32)&Value_00000000;

    Data_0200d260 = zero;
    Data_0200d25c = zero;

    Func_020074d8(0x0200b9c9, 0xc80);
}

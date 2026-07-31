typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_3a4 owner at 0x02003724, 20 bytes: stamp a fixed value into the
 * caller's record at +102, then play sound cue 288.
 *
 * Complete owner: `push {lr}` at 0x02003724 through the matching
 * interworking return `pop {r0} / bx r0` at 0x02003734-0x02003736. No
 * literal pool. One argument (r0), void.
 *
 * Not found by the structural inventory walk (unindexed); one incoming
 * call site per overlay_unindexed.ts. This owner makes one outgoing call,
 * resolved with the `+2` rule against the raw image (this owner starts
 * outside a recorded row, so overlay_call_targets.ts's own table does not
 * cover it -- resolved with `bun tools/overlay_show.ts resource_3a4 3724 |
 * bun tools/overlay_call_targets.ts resource_3a4 3724 3736 --annotate`):
 *   0x2003730 -> veneer -> Func_080f9010(288)   established, play a sound cue (288 = 144 << 1)
 */

void Func_02007452();          /* play a sound cue, established (veneer to Func_080f9010) */

void Func_02003724(u16 *record)
{
    record = (u16 *)((char *)record + 102);
    {
        s32 value = 0x21;

        *record = value;
    }
    Func_02007452(288);
}

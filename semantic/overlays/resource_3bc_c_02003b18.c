typedef signed short s16;
typedef signed int s32;

/*
 * resource_3bc owner at 0x02003b18, 40 bytes: tear down task 0x0200391c --
 * the same task both resource_3bc_c_02003abc.c and resource_3bc_c_
 * 02003a58.c install -- release whatever Data_0200dace's current handle
 * refers to, then reset that handle to the empty sentinel.
 *
 * Complete owner: `push {r5, lr}` at 0x02003b18 through the matching
 * interworking return `pop {r5} / pop {r0} / bx r0` at 0x02003b2e-0x02003b32,
 * followed by the three-word literal pool 0x02003b34-0x02003b3f that closes
 * the 40-byte span; the next owner's prologue is at 0x02003b40.  No
 * arguments (r0 is overwritten by the first pool load before any incoming
 * value could be read), void.
 *
 * Not found by the structural inventory walk (unindexed; four incoming call
 * sites per `overlay_unindexed.ts`, the queue's ranking count). This owner
 * itself makes two outgoing calls, both resolved with the `+2` rule against
 * the raw image (this owner starts outside a recorded row, so
 * overlay_call_targets.ts's own table does not cover it -- resolved with
 * `bun tools/overlay_show.ts resource_3bc 3b18 | bun
 * tools/overlay_call_targets.ts resource_3bc 3b18 3b34 --annotate`):
 *   0x2003b1c -> veneer -> Func_080000d8(0x0200b91d)   established (resource_395/372/3bb/38f/3b4), "remove a per-frame task (callback)"; same 0x0200b91d = Func_0200391c task pointer as the two seeders in this overlay
 *   0x2003b26 -> veneer -> Func_080001b8(value)         unestablished; single-argument, argument is the s16 read from Data_0200dace before it is reset
 *
 * Uncertainty: Data_0200dace's role (a handle of some kind, read then
 * discarded into Func_080001b8 and unconditionally overwritten) is not
 * otherwise cross-referenced from this owner alone.
 */

extern s16 Data_0200dace;

void Func_080000d8();          /* remove a per-frame task, established */
void Func_080001b8();          /* unestablished, single argument */

void Func_02003b18(void)
{
    s16 handle;

    Func_080000d8(0x0200b91d);

    handle = Data_0200dace;
    Func_080001b8(handle);

    Data_0200dace = -1;
}

typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * resource_3bb owner at 0x02003ae4, 84 bytes (0x02003ae4-0x02003b37):
 * 62 bytes of code, two alignment bytes at 0x02003b22, and the five-word
 * literal pool at 0x02003b24.
 *
 * Prologue `push {r5, r6, lr}`, epilogue `pop {r5, r6} / pop {r0} / bx r0`:
 * r0 holds the popped return address, so the owner is void.
 *
 * All four `bl` sites are placed and reach four distinct import veneers,
 * matching the inventory row's calls=4.  Targets come from
 * tools/overlay_call_targets.ts (target offset = stored displacement + 2),
 * never from the disassembler's annotations: site 0x3aec -> veneer 0x3e30 ->
 * Func_08000290, 0x3af4 -> 0x3df8 -> Func_080001a8, 0x3afa -> 0x3ee8 ->
 * Func_080770c0, 0x3b18 -> 0x3da8 -> Func_080000d0.
 *
 * The pool word 0x0200b459 is `Func_02003458 + 1`, this overlay's own
 * prologue at file offset 0x3458 carrying the Thumb bit, so the final call
 * installs that routine as a task under key 0xc85.  It is a second witness
 * for the 0x02008000 link base (the first being 0x02008715 in the byte-exact
 * sibling assets/code/resource_3bb_c_02000950.c).
 *
 * 0x03001f3c is the usual IWRAM workspace pointer; 0x02001000 is below the
 * link base, so it is a RAM record and not in-image data.  Func_02003458
 * loads the very same two addresses, which is what ties the record to the
 * installed task.
 *
 * The first call takes no set argument: r0-r3 still hold entry values at
 * 0x02003ae4 and none is written before the branch.  Its result is carried
 * straight into Func_080001a8's first argument, with the workspace field at
 * +240 as the second.
 *
 * Uncertainty: only the five halfword fields written below are asserted for
 * the 0x02001000 record; nothing else about its layout is claimed.  The
 * guarded block runs when Func_080770c0(0x109) reports zero, and the zeroes
 * stored there are that same returned r0, reproduced literally.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
s32 Func_08000290();
void Func_080001a8();
s32 Func_080770c0();
void Func_080000d0();

/* This overlay's own routine at file offset 0x3458, installed as a task. */
void Func_02003458();

void Func_02003ae4(void)
{
    u8 *workspace;
    u16 *record;
    s32 handle;

    workspace = *(u8 **)0x03001f3c;
    record = (u16 *)0x02001000;

    handle = Func_08000290();
    Func_080001a8(handle, workspace + 240);

    if (Func_080770c0(0x109) == 0) {
        record[0] = 1;
        record[1] = 1;
        record[4] = 0;
        record[2] = *(u16 *)(workspace + 224);
        record[3] = 0;
    }

    /* The task is published as its entry address plus the Thumb bit. */
    Func_080000d0((s32)Func_02003458 + 1, 0xc85);
}

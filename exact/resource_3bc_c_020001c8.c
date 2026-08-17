#include "types.h"

/*
 * resource_3bc owner at 0x020001c8, 60 bytes.
 *
 * A frame-yield wait: ask for ten frames, then poll once per frame until the
 * two state words read 3 and 1, giving up after 119 polls.
 *
 * `push {r5, lr}` through `pop {r5} ; pop {r0} ; bx r0`; r0 holds the popped
 * return address, so the owner returns nothing.  r5 is the poll counter, live
 * across both calls, which is what it is saved for.
 *
 * TWO CHANGES WERE NEEDED TOGETHER, and each looks wrong on its own -- which is
 * why the row sat at 21 differing halfwords with both of them missing.
 *
 * The wait is an ordinary loop reading the two globals in its condition, not the
 * goto cascade with a cached `cond` that was here before.  On its own that reads
 * 9 differing halfwords instead of 21, but 64 bytes against 60.
 *
 * The frame count is the immediate 10, not `(s32) &Value_0000000a`.  On its own
 * THAT reads worse than the goto form, 25 halfwords against 21, because the goto
 * shape was absorbing the difference elsewhere.  The four bytes the loop form was
 * over are exactly the pool word `&Value_0000000a` needs, and every pool
 * displacement in the row was shifted by four because of it.
 *
 * So the pool word and the loop had to go in the same edit.  Neither reading is
 * visible from the other's baseline: one costs four bytes, the other costs
 * halfwords, and only together do they reach 0.
 *
 * Also ruled out while getting here: `volatile` on either global or both changes
 * nothing.  The reference dereferences a pointer once before the loop where the
 * old goto form dropped that read, which looks exactly like the guide's volatile
 * tell; it is the loop condition's own first evaluation.
 */

extern s32 Data_0200d480;
extern s32 Data_0200d484;

void Func_02004a0e(s32 frames);
void Func_02004a1e(s32 frames);

void Func_020001c8(void)
{
    s32 polls;

    Func_02004a0e(10);

    polls = 0;
    while (Data_0200d480 != 3 || Data_0200d484 != 1) {
        Func_02004a1e(1);
        polls++;
        if (polls > 119) {
            return;
        }
    }
}

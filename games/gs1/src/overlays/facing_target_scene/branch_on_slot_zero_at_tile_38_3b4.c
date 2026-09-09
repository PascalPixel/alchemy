/*
 * Facing target scene for resource_3b4.
 *
 * A Func_ name in the import veneer band 0x02002468-0x0200261f names the
 * main-image address held in the veneer's trailing word, not a runtime
 * address the call reaches directly.  Declarations are old-style because
 * those imports are reached with differing argument counts from different
 * call sites.
 */
#include "types.h"

/* Slot record lookup, then the notification and step imports. */
s32 *Func_020040a6();
s32 *Func_020040ae();
void Func_02004174();
void Func_0200417a();
void Func_02003598();
void Func_02001c78();
void Func_02003680();

/*
 * Compare the X tiles of slots 0 and 8, each the word at +8 divided by
 * 0x100000.  The 100-byte owner includes its one pool word, 0x000fffff,
 * which that truncating signed division reads.  The special cases apply only
 * when slot 0 sits on tile 38 and slot 8 does not; then slot 0's halfword at
 * +6 selects one of two notifications.  Every other case, an unrecognised
 * halfword included, runs the three-step ordinary path.
 */
void SceneActor_BranchOnSlotZeroAtTile38(void)
{
    s32 *slot0 = Func_020040a6(0);
    s32 *slot8 = Func_020040ae(8);
    s32 x0 = slot0[2] / 0x100000;
    s32 x8 = slot8[2] / 0x100000;

    if (x0 == 38 && x8 != 38) {
        u16 facing = ((u16 *)slot0)[3];

        /*
         * The facing value is still in r0 at both branches, but whether
         * either callee reads it is unverified, so no argument is passed.
         */
        if (facing == 0xc000) {
            Func_02004174();
            return;
        }
        if (facing == 0x4000) {
            Func_0200417a();
            return;
        }
    }

    Func_02003598();
    Func_02001c78();
    Func_02003680();
}

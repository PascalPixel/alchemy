#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/staged_nav/step_down_until_clamp.h"

/*
 * Steps a record down by a tenth of a unit per frame until it reaches the
 * clamp at 0x1999 -- resource_3b3.  The eighty-byte owner includes its
 * alignment halfword and four pool words: the clamp 0x1999, -0x1999,
 * -0xcccc and the loop bound 0x1998, which is one less than the clamp.
 * None of them is an address, and each is reached only through a
 * pc-relative load.
 */

u8 *Actor_Run();           /* Record fetch, returns the record. */
void Actor_Run2();          /* Dispatch stub table entry 0. */

/*
 * Each Func_ name labels the call word of one call site rather than a
 * runtime address.  The first call is made before r0 is disturbed, so the
 * index is passed straight through instead of being materialised again.  The
 * two exits differ: the thirty-two frame cap returns without pinning, while
 * the clamp path pins the record to exactly 0x1999.
 */
void StagedActor_StepDownUntilClamp(s32 index)
{
    u8 *obj = Actor_Run(index);
    u32 cnt;

    obj[0x55] = 0;

    cnt = 0;
    for (;;) {
        if (cnt > 31) return;
        Actor_Run2(1);
        *(s32 *)(obj + 28) += -0x1999;
        *(s32 *)(obj + 12) += -0xcccc;
        cnt++;
        if (*(s32 *)(obj + 28) <= 0x1998) {
            *(s32 *)(obj + 28) = 0x1999;
            return;
        }
    }
}

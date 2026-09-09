#include "types.h"

/* Per-site veneers: both reach the same main-image import, but each names its
 * own loader-relocated call word rather than a runtime address. */
extern u8 Value_0000000a;
void Func_02004716();
void Func_02004724();

/* In-image status words. The 64-byte owner includes its alignment bytes and
 * the two-word literal pool that holds these two addresses. */
extern volatile s32 Data_0200c834;
extern volatile s32 Data_0200c838;

/*
 * Spin until the first status word reaches zero with the second equal to 75,
 * giving up after 600 polls. Both words are re-read on every pass, so they are
 * volatile and their reads must not be hoisted or merged across the poll call.
 * The plain while loop is the spelling that reproduces the reference. What the
 * two words mean is not established here -- only that another task publishes
 * them while this owner spins.
 */
void SceneState_WaitForStatusWords(void)
{
    s32 cnt;

    /* The frame count is a literal ten. */
    Func_02004716(10);

    cnt = 0;
    while (Data_0200c834 != 0 || Data_0200c838 != 75) {
        Func_02004724(1);
        cnt++;
        if (cnt >= 600) {
            return;
        }
    }
}

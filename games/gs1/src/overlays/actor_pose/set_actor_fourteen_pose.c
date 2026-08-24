#include "types.h"

/* The import's main-image target is Func_0808a1f0. Tracked scene callers
 * identify its second argument (258) as a pose id. */
#define SetActorPose Func_020012de
#define SetActorFourteenPose Func_02000030

void SetActorPose(s32 actor, s32 pose);

s32 SetActorFourteenPose(void)
{
    do {
        SetActorPose(14, 258);
        return 0;
    } while (0);
}

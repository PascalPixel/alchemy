#include "types.h"

/* The import's main-image target is Func_0808a1f0. Tracked scene callers
 * identify its second argument (258) as a pose id. */
#define SetActorPose Func_020012de
#define SetActorFourteenPose Func_02000030

void SetActorPose(s32 actor, s32 pose);

static __inline__ void ConfigureActorPose(s32 actor, s32 pose)
{
    SetActorPose(actor, pose);
}

s32 SetActorFourteenPose(void)
{
    ConfigureActorPose(14, 258);
    return 0;
}

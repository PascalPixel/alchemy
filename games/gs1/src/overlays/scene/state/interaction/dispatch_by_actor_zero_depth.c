#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/state/interaction/dispatch_by_actor_zero_depth.h"

struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
};

extern struct Actor *State_Run(s32);

void State_DispatchByActorZeroDepth(void)
{
    struct Actor *p = State_Run(0);

    if (p->f0c >= 0x100000) {
        State_Run2();
    } else {
        State_Run3();
    }
}

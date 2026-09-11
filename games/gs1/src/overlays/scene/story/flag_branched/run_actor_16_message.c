/* Contiguous unnamed leaf-owner run for resource_38d. */

#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/flag_branched/run_actor_16_message.h"

    Story_Do(0x1750);
    if (Story_Check(0x302) != 0) {
        Story_Do2(0x1768);
    }
    Story_Apply(15, 0);
    Story_Do3(0x302);
    Story_Run();
}

#include "types.h"

void Dialogue_RunActor16Message1769(void)
{
    Story_Run2();
    Story_Do4(0x1769);
    Story_Apply2(16, 0);
    Story_Run3();
}

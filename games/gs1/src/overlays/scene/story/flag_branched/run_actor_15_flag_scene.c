#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/flag_branched/run_actor_15_flag_scene.h"

extern u8 gWork[];

void Dialogue_RunActor15Flag303Scene(void)
{
    u32 i;
    s32 record;

    Story_Run();
    Story_Do(0x1756);
    if (Story_Check(0x303) != 0) {
        Story_Do2(0x176c);
    }
    Story_Run2(15, 0);
    Story_Do3(0x303);
    Story_Run3();
}

#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/scenario_dispatch/dispatch_by_scenario_id.h"

extern s16 gCell[];
extern u8 Value_00000013;
extern u8 Value_00000010;

s32 Scene_DispatchByScenarioId(void)
{
    s32 scenario = gCell[224];

    if (scenario == (s32)&Value_00000013) {
        Story_Run();
    } else if (scenario == (s32)&Value_00000010) {
        Story_Run2();
    }
    return 0;
}

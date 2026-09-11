#include "scene.h"
#include "abi/overlays/scene/state/interaction/select_secondary_by_runtime_selector.h"
#define PrimaryRuntimeSelector Value_00000060
#define SecondaryRuntimeSelector Value_00000061
#define TertiaryRuntimeSelector Value_00000062

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectSecondaryByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

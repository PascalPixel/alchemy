#include "scene.h"
#include "abi/overlays/scene/actor/waypoint/select_data_by_runtime_selector.h"
#define PrimaryRuntimeSelector Value_00000044
#define SecondaryRuntimeSelector Value_00000045
#define TertiaryRuntimeSelector Value_00000046

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

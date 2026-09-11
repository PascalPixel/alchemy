#include "scene.h"
#include "abi/overlays/scene/actor/staged_step/select_data_by_runtime_selector.h"
#define PrimaryRuntimeSelector Value_00000075
#define SecondaryRuntimeSelector Value_00000076
#define TertiaryRuntimeSelector Value_00000078

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

#include "scene.h"
#include "abi/overlays/scene/actor/push_puzzle/select_data_by_runtime_selector.h"
#define PrimaryRuntimeSelector Value_0000001e
#define SecondaryRuntimeSelector Value_00000023
#define TertiaryRuntimeSelector Value_00000020

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

/* The stage's scene control: decodes the given resource into the Colosso
 * work, starts the control in the scene's saved words unless flag 0x109 is
 * set, and schedules the path rival's update. Its setter twin is
 * COMMON/KOROSSEO/SCENE_STATE.C. */
#include "LOG_ROLLING.H"

u8 *Resource_GetTableEntry(s32 resource);
void Korosseo_UpdatePathRival(void);

void ColossoLogRollingStage_InitializeSceneControl(s32 resource)
{
    u8 *work = gKorosseoWork;
    SceneControl *control = (SceneControl *)gSceneState;

    Resource_DecodeType01(Resource_GetTableEntry(resource), work + 240);
    if (GameFlag_IsSet(0x109) == 0) {
        control->enabled = 1;
        control->active = 1;
        control->scene_variant = *(u16 *)(work + 224);
        control->timer = 0;
        control->phase = 0;
    }
    Task_AddCallback(Korosseo_UpdatePathRival, 0xc85);
}

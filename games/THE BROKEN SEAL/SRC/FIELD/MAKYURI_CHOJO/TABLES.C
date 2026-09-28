#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "OVERLAY_OBJECT.H"
#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "AERIE.H"

void *SceneData_GetTableB938(void)
{
    return MakyuriChojo_ScriptTable;
}

s32 get_runtime_default_result(void)
{
    return 0;
}

void *SceneData_GetTableb9c8(void)
{
    return MakyuriChojo_MessageTable;
}

void *SceneData_GetTableB9d4AfterStateCheck(void)
{
    if (gGameState.entrance != 1) {
        GameFlag_Set(0x253);
    }
    return MakyuriChojo_ActorTable;
}

void *SceneData_GetTablebbe4(void)
{
    return MakyuriChojo_EventTable;
}

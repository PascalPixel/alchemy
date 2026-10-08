

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SCENE.H"
extern struct Resource37deSceneRuntime *gEventWork;

#include "RESOURCE_37DE_SCENE.H"

s32 Engine_GameFlagIsSet(s32);

void Engine_WorkSetValuesIfNonNegative(s32, s32, s32);

u8 *SceneData_GetScriptTable(void)
{
    return SoruRoka_SceneTable0;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return SoruRoka_SceneTable1;
}

u8 *SceneData_GetActorTable(void)
{
    return SoruRoka_SceneTable2;
}

u8 *SceneData_GetEffectTable(void)
{
    return SoruRoka_SceneTable3;
}

s32 Scene_InitSceneRequestAndCameraZoom(void)
{
    struct Resource37deSceneRuntime *runtime;

    s32 zoom_x = 0x10000;
    s32 zoom_y = 0x10000;
    s32 zoom_z = 0x10000;

    runtime = gEventWork;
    runtime->scene_request_1c0 = 0x204;
    runtime->scene_setup_word_1c8 = 0x10;
    if (Engine_GameFlagIsSet(0x814) != 0) {
        QueueSceneSound(0x8D);
        Engine_WorkSetValuesIfNonNegative(zoom_x, zoom_y, zoom_z);
        InitializeSceneRecordBuffer();
    }
    return 0;
}

#include "STATUE_HALL.H"

u8 *SceneEventRuntime_GetScriptData(void)
{
    return SceneEventRuntime_ScriptData;
}

s32 SceneEventRuntime_ReturnZero(void)
{
    return 0;
}

u8 *SceneEventRuntime_GetMessageData(void)
{
    return SceneEventRuntime_MessageData;
}

u8 *SceneEventRuntime_GetActorData(void)
{
    return SceneEventRuntime_ActorData;
}

u8 *SceneEventRuntime_GetEffectData(void)
{
    return SceneEventRuntime_EffectData;
}

s32 SceneEventRuntime_SelectInitialSceneByFlags(void)
{
    s32 no;

    if (GameFlag_IsSet(0x818) != 0) {
        if (GameFlag_IsSet(FLAG_STATUE_TRAP_SPRUNG) == 0) {
            no = 3;
            goto apply;
        }
        goto fail;
    }
    if (GameFlag_IsSet(0x812) == 0) {
        no = 4;
apply:
        Event_RequestExit(no);
        return 1;
    }
fail:
    return -1;
}

void Scene_OpenTheHole(void)
{
    s32 i;

    { s32 k5 = 2, k6 = 1; Map_CopyCellsTo(0, 28, 17, 8, k5, k6); }
    Audio_PlayCue(200);
    for (i = 0; i != 22; i++) {
        Map_CopyCellsTo(10, 61, 17, 40, 2, 1);
        Event_Wait(4);
        Map_CopyCellsTo(8, 61, 17, 40, 2, 1);
        Event_Wait(4);
    }
    { s32 k5 = 4, k6 = 3;
      Map_CopyCellsTo(0, 59, 15, 38, k5, k6);
      Map_CopyCellsTo(4, 59, 17, 38, k5, k6); }
    Map_CopyCellsTo(8, 60, 17, 39, 2, 2);
    { s32 k5 = 17, k6 = 8; Map_CopyCellAttributes(0, 0, 2, 1, k5, k6); }
    GameFlag_Set(0x207);
    SoruSekizo_RunStatueDropScene();
}

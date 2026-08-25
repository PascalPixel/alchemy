#include "types.h"

#define SceneEventRuntime_GetScriptData Func_02000030
#define SceneEventRuntime_ReturnZero Func_02000038
#define SceneEventRuntime_GetMessageData Func_0200003c
#define SceneEventRuntime_GetActorData Func_02000044
#define SceneEventRuntime_GetEffectData Func_0200004c

extern u8 SceneEventRuntime_ScriptData[];
extern u8 SceneEventRuntime_MessageData[];
extern u8 SceneEventRuntime_ActorData[];
extern u8 SceneEventRuntime_EffectData[];

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

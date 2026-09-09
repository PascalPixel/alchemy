#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SceneEventRuntime_GetScriptData Func_02000030
#define SceneEventRuntime_ReturnZero Func_02000038
#define SceneEventRuntime_GetMessageData Func_0200003c
#define SceneEventRuntime_GetActorData Func_02000044
#define SceneEventRuntime_GetEffectData Func_0200004c
#define SceneEventRuntime_SelectInitialSceneByFlags Func_02000054

extern u8 SceneEventRuntime_ScriptData[];
extern u8 SceneEventRuntime_MessageData[];
extern u8 SceneEventRuntime_ActorData[];
extern u8 SceneEventRuntime_EffectData[];

s32 Func_0200248e(s32);
s32 Func_02002498(s32);
s32 Func_020024a6(s32);
void Func_020025a8(s32);

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

    if (Func_0200248e(0x818) != 0) {
        if (Func_02002498(0x813) == 0) {
            no = 3;
            goto apply;
        }
        goto fail;
    }
    if (Func_020024a6(0x812) == 0) {
        no = 4;
apply:
        Func_020025a8(no);
        return 1;
    }
fail:
    return -1;
}

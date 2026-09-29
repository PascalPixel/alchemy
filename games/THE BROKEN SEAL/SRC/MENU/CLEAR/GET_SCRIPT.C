#include "TYPES.H"
#include "SCENE.H"

extern u8 Clear_ScriptTable[];
extern u8 Clear_MessageTable[];
extern u8 Clear_ActorTable[];
extern u8 Clear_EffectTable[];

/* Each getter is eight bytes including the one pool word that holds the
 * table it returns. */
u8 *SceneData_GetScriptTable(void)
{
    return Clear_ScriptTable;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Clear_MessageTable;
}

u8 *SceneData_GetActorTable(void)
{
    return Clear_ActorTable;
}

u8 *SceneData_GetEffectTable(void)
{
    return Clear_EffectTable;
}

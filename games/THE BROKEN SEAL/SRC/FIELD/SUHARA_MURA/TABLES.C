#include "TYPES.H"
#include "FIELD_EVENT.H"

/*
 * Suhara village's scene tables: the script and message tables and the
 * actor table the event flag 0x96f selects.
 */

extern u8 SuharaMura_Scripts[];
extern u8 SuharaMura_Messages[];
extern u8 SuharaMura_Actors[];
extern u8 SuharaMura_ActorsFlag96f[];

u8 *SceneData_GetScriptTable(void)
{
    return SuharaMura_Scripts;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return SuharaMura_Messages;
}

s32 SceneData_SelectActorTableByFlag96f(void)
{
    if (GameFlag_IsSet(0x96f) != 0) {
        return (s32)SuharaMura_ActorsFlag96f;
    }
    return (s32)SuharaMura_Actors;
}

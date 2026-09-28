/* The scene hooks the entry veneers export: the stage's tables. */
#include "LOG_ROLLING.H"

u8 *ColossoLogRollingStage_GetScriptData(void)
{
    return (u8 *)gKorosseoMarutaEntrances;
}

s32 ColossoLogRollingStage_GetMessageData(void)
{
    return 0;
}

u8 *ColossoLogRollingStage_GetActorData(void)
{
    return (u8 *)gKorosseoMarutaExits;
}

u8 *ColossoLogRollingStage_GetEffectData(void)
{
    return (u8 *)gKorosseoMarutaPlacements;
}

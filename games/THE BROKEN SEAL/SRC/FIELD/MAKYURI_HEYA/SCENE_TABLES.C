#include "PROBE.H"

extern u8 MakyuriHeya_SceneTable[];

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTableE614(void)
{
    return MakyuriHeya_SceneTable;
}

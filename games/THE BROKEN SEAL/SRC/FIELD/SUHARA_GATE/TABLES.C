#include "GATE.H"

extern u8 SuharaGate_SceneTable[];

s32 SceneData_ReturnZero(void)
{
    extern s16 gCell[];

    return 0;
}

u8 *SceneData_GetSceneTable(void)
{
    return SuharaGate_SceneTable;
}

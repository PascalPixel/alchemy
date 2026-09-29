#include "IMIRU.H"

/* The whole four-byte owner. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes its one pool word. */
u8 *SceneData_GetTableA990(void)
{
    return (u8 *)ImiruMura_SceneTable;
}

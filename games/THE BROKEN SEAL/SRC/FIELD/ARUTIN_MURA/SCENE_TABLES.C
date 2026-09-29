#include "TYPES.H"

/* The table accessors between the scene-id selectors at the head of the
   overlay. */

/* The scene's message table, laid out after the code. */
extern u8 Placement_Messages[];

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

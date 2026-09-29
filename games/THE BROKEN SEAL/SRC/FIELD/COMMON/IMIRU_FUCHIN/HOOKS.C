/* Two of the scene hooks the entry veneers export: no follow-up, and the
 * cave's exits. */
#include "IMIRU_FUCHIN.H"

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 *ImiruFuchin_GetExits(void)
{
    return gImiruFuchinExits;
}

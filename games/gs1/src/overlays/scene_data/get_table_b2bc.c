#include "types.h"

/*
 * resource_3b9 owner at 0x02000074: a leaf that loads its literal pool word
 * and returns it. The eight-byte owner includes that one pool word at
 * 0x02000078, holding the address 0x0200b2bc -- image offset 0x32bc --
 * which is returned without being dereferenced.
 */

extern u8 Data_0200b2bc[];   /* image offset 0x32bc */

u8 *SceneData_GetTableB2bc(void)
{
    return Data_0200b2bc;
}

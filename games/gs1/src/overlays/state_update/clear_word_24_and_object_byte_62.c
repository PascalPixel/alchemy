#include "types.h"

/*
 * Scene state reset for overlay resource_3b3. The callee name refers to its
 * own call word rather than to a shared runtime address.
 */

extern u8 *Data_03001ee0;

u8 *Func_02004bbe();

/*
 * Clears Data_03001ee0[+24] and one flag byte on the object returned by
 * Func_02004bbe. The 28-byte owner at 0x0200209c includes its one pool
 * word, the Data_03001ee0 pointer.
 */
void SceneState_ClearWord24AndObjectByte62(void)
{
    u8 *obj = Func_02004bbe(0);

    *(s32 *)(Data_03001ee0 + 24) = 0;
    obj[0x62] = 0;
}

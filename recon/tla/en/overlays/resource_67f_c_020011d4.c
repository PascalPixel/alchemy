/* Near miss, differ 6 of 29: the listing masks the old cell into r3 and builds 0xff00 in r2;
 * this draft uses the two registers the other way round. */
#include "../../../../games/tla/src/field/overlay_67f/ov67f.h"

void Func_020011d4(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov67FRoom1) {
        Func_02001518(Data_02001654);
    } else {
        Func_02000ee8(2, 50, 13, 0xff00 | (Func_02000eac(2, 50, 13) & 255));
        Func_02001518(Data_02001658);
    }
}

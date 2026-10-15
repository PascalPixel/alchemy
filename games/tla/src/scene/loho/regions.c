#include "types.h"
#include "party_state.h"

extern char SceneId_Ov6A6Room2;
extern const u8 gData_0200183c[];

const u8 *Func_02000050(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov6A6Room2)
        return gData_0200183c;
    return 0;
}

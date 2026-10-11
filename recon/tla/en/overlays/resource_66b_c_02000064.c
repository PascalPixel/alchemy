#include "types.h"
#include "field_scene.h"
#include "game.h"

extern const u8 Data_02001728[];
extern const u8 Data_02001698[];

const u8 *Func_02000064(void)
{
    if (gPartyState.scene == 0x67)
        return Data_02001728;
    return Data_02001698;
}

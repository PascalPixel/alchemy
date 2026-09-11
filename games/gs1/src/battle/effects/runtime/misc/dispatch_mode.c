#include "types.h"
#include "scene.h"
#include "abi/battle/effects/runtime/misc/dispatch_mode.h"
#include "global_cells.h"

typedef void (*Callback)(s32 *);

s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);

extern Callback BattleFx_CallbackTable[];

void BattleFx_DispatchMode(s32 *state)
{
    s32 index;
    s32 **destination;

    Runtime_AllocateHeapBlock(41, 0x302);
    Runtime_AllocateHeapBlock(39, 0x782c);
    Runtime_AllocateHeapBlock(40, 0x4000);

    destination = (s32 **)(*(s32 *)ADDR_03001EEC + 0x7828);
    index = state[0];
    *destination = state;
    if (index == 0)
        state[6] = 0;
    else
        BattleFx_CallbackTable[index - 1](state);

    Battle_Do(40);
    Battle_Do(39);
    Battle_Do(41);
}

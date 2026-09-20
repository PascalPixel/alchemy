#include "TYPES.H"
#include "GLOBAL_CELLS.H"

typedef void (*Callback)(s32 *);

s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);
void Func_08002dd8(s32);
#define Runtime_ReleaseHeapBlock Func_08002dd8
extern Callback Data_080ee2b4[];

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
        Data_080ee2b4[index - 1](state);

    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
    Runtime_ReleaseHeapBlock(41);
}

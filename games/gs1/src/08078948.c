#include "types.h"
#include "owner_state.h"
s32 Func_080788c4(s32, s32);
s32 Func_08078ad0(s32 arg0, s32 arg1);
void Func_0808a548(s32);

s32 Func_08078948(s32 owner, s32 index)
{
    s32 value =
        ((struct OwnerInventoryState *)Func_08077394(owner))->inventory[index];
    s32 result = Func_080788c4(owner, index);

    if (result != -1) {
        Func_0808a548(Func_08078ad0(value, 1));
    }
    return result;
}

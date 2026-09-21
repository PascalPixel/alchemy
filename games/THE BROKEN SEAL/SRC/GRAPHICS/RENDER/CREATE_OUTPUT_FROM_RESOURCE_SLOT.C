#include "RENDER_INPUT.H"
#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08004080();
#define find_free_slot Func_08004080
s32 Func_08021b30(s32 input, s32 arg1);

s32 Ui_CreateOutputFromResourceSlot(
    struct RenderInput *input,
    s32 arg1,
    s32 arg2,
    s32 resource)
{
    s32 slot;

    slot = find_free_slot();
    if (slot != 0x60) {
        Func_08021b30(resource, slot);
        Func_0801eadc(slot, 0x40000000, input, arg1, arg2);
    }
}

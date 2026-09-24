#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

/* object/table/ObjectTable_FindLastActiveId.c */
struct State_0808b824 {
    u8 padding[0x34];
    s32 values[58];
};

extern struct State_0808b824 *gWork;

s32 ObjectTable_FindLastActiveId(void)
{
    struct State_0808b824 *state = gWork;
    s32 result = 7;
    s32 index = 8;
    s32 *value = state->values;

    do {
        s32 current = *value++;

        if (current != 0) {
            result = index;
        }
        index++;
    } while (index <= 65);
    result++;
    if (result == 66) {
        result = -1;
    }
    return result;
}

/* object/table/ObjectTable_GetSlotAddress.c */
void *ObjectTable_GetSlotAddress(u32 index)
{
    return *(u8 **)ADDR_03001EBC + index * 4 + 20;
}

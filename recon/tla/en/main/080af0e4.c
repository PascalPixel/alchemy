#include "TYPES.H"

struct OwnerItems {
    u8 unknown_00[216];
    s16 items[15];
};

struct OwnerItems *Owner_GetState(s32 owner);
void Owner_RecalculateStats(s32 owner);

s32 Func_080af0e4(s32 owner, s32 index)
{
    struct OwnerItems *state = Owner_GetState(owner);
    s16 *items = state->items;
    s16 *src;
    s16 *dst;
    s32 count;
    s32 i;

    state->items[index] = 0;
    src = items;
    count = 0;
    dst = items;
    for (i = 0; i < 15; i++) {
        s16 item = *src++;

        if (item != 0) {
            *dst++ = item;
            count++;
        }
    }
    for (i = count; i < 15; i++)
        items[i] = 0;
    Owner_RecalculateStats(owner);
    return 2;
}

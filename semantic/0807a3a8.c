#include "types.h"

u8 *Func_08077330(s32 group);

s32 Func_0807a3a8(s32 owner, s32 first, s32 second)
{
    u8 *state;
    u8 *entries;
    u8 *firstCursor;
    u8 *secondCursor;
    s32 count;
    s32 index;
    s32 offset;
    s32 shiftOffset;
    s32 removed;
    s32 group;

    removed = 0;
    group = 0;
    if ((u32)owner > 7)
        group = 1;
    state = Func_08077330(group);
    entries = state + 8;
    count = *(s32 *)(state + 0x108);
    index = 0;
    secondCursor = state + 9;
    offset = 0;
    firstCursor = entries;
    if (count <= 0)
        goto done;

    shiftOffset = 0;
    if (first == state[8] && second == state[9]) {
        *(s32 *)(state + 0x108) = count - 1;
        removed = 1;
        goto shift;
    }

scan:
    count = *(s32 *)(state + 0x108);
    index++;
    secondCursor += 4;
    firstCursor += 4;
    offset += 4;
    if (index >= count)
        goto done;
    shiftOffset = offset;
    if (first != *firstCursor || second != *secondCursor)
        goto scan;
    *(s32 *)(state + 0x108) = count - 1;
    removed = 1;

shift:
    if (index >= *(s32 *)(entries + 0x100))
        goto done;
shift_next:
    *(u32 *)(entries + shiftOffset) =
        *(u32 *)(entries + shiftOffset + 4);
    index++;
    if (index < *(s32 *)(entries + 0x100)) {
        shiftOffset = index * 4;
        goto shift_next;
    }

done:
    return removed;
}

#include "types.h"

struct RuntimeSelectionState {
    u8 padding_00[16];
    u16 ids[6];
    u32 enabledChoices[6];
    s8 selectedChoices[6];
    u8 padding_3a[6];
    u8 count;
};

struct RuntimeObject {
    u8 name[14];
    u8 padding_0e[282];
    u8 selectionId;
    u8 inactive;
};

extern struct RuntimeSelectionState *Data_03001e74;

struct RuntimeObject *Func_08077008(s32 id);

/*
 * The return value is incidental and the known caller discards it, but these
 * expressions describe the value that the reference leaves in r0 on each
 * exit path.
 */
s32 Func_080c1ebc(s32 id)
{
    struct RuntimeSelectionState *state;
    struct RuntimeObject *object;
    s32 count;
    s32 index;
    s32 length;
    s32 choice;
    s32 selectionId;
    s32 result;

    state = Data_03001e74;
    count = state->count;
    object = Func_08077008(id);
    result = (s32)object;
    if (object->inactive != 0)
        return result;

    selectionId = object->selectionId;
    result = selectionId;
    index = 0;
    while (index < count && state->ids[index] != selectionId)
        index++;

    if (index == count || state->enabledChoices[index] == 0)
        return result;

    length = 0;
    while (length <= 13 && object->name[length] != 0)
        length++;

    choice = 32;
    if (length > 0)
        choice = object->name[length - 1] - '1';

    state->enabledChoices[index] &= ~(1 << choice);
    result = choice;
    return result;
}

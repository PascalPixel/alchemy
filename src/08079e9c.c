typedef unsigned char u8;
typedef signed int s32;

u8 *Func_080773d8(s32 index);
u8 *Func_08079ad8(s32 index);

s32 Func_08079e9c(u8 *state, s32 target)
{
    u8 *entries;
    u8 *field;
    s32 index;
    s32 offset = 0x129;
    s32 value;

    field = state + offset;
    if (*field == 0) {
        offset--;
        field = state + offset;
        entries = Func_080773d8(*field) + 0x48;
        index = 0;
first_loop:
        if (*entries != target) {
            index++;
            entries++;
            if (index > 2) {
                goto not_found;
            }
            goto first_loop;
        }
        goto found;
    }

    offset = 0x129;
    field = state + offset;
    entries = Func_08079ad8(*field) + 0x50;
    index = 0;
second_loop:
    value = *entries++;
    if (value == target) {
found:
        return 1;
    }
    index++;
    if (index > 2) {
not_found:
        return 0;
    }
    goto second_loop;
}

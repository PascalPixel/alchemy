typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct State_08078664 {
    u8 padding[216];
    u16 entries[15];
};

struct State_08078664 *Func_08077394(s32);

s32 Func_08078664(s32 owner, s32 value)
{
    struct State_08078664 *state = Func_08077394(owner);
    s32 index = 0;
    u16 *entry = state->entries;

    do {
        if (((*entry++) & 0x1ff) == value) {
            return index;
        }
        index++;
    } while (index <= 14);
    return -1;
}

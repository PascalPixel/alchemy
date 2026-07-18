typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct State_080784d8 {
    u8 padding[0xd8];
    u16 entries[15];
};

struct State_080784d8 *Func_08077394(s32);

s32 Func_080784d8(s32 owner)
{
    struct State_080784d8 *state = Func_08077394(owner);
    s32 count = 0;

    if (state->entries[count] != 0) {
        do {
            count++;
            if (count > 14)
                break;
        } while (state->entries[count] != 0);
    }
    return count;
}

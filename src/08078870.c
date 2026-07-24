typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct State_08078870 {
    u8 padding[216];
    u16 entries[15];
};

struct Item_08078870 {
    u8 padding[2];
    u8 type;
};

struct Item_08078870 *Func_08078414(s32);

s32 Func_08078870(struct State_08078870 *state, s32 type)
{
    s32 i;

    for (i = 0; i <= 14; i++) {
        if (state->entries[i] & 0x200) {
            struct Item_08078870 *item = Func_08078414(state->entries[i]);

            if (item->type == type) {
                return state->entries[i] & 0x1FF;
            }
        }
    }
    return 0;
}

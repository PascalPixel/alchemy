typedef unsigned char u8;
typedef signed int s32;

struct State {
    u8 unknown_000[276];
    void *items[64];
    u8 unknown_214[5];
    u8 count;
};

extern struct State *Data_03001f2c;

void Func_08009020(void *item, s32 enabled);

void Func_080ad69c(void)
{
    struct State *state = Data_03001f2c;
    s32 index;

    for (index = 0; index < state->count; ++index) {
        Func_08009020(state->items[index], 1);
    }
}

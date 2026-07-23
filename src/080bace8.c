typedef unsigned char u8;
typedef signed int s32;

u8 *Func_08077008(s32);
s32 *Func_080b7dd0(s32);
u8 *Func_080b7f70(s32, s32);
void Func_08009020(void *, s32);
void Func_080030f8(s32);
void Func_080bac6c(s32);
void Func_08009108(void **, s32);
void Func_080b7e60(s32);

void Func_080bace8(s32 id)
{
    void *items[4];
    u8 *state;
    u8 *item;
    u8 *child;
    s32 index;

    state = Func_08077008(id);
    index = 0;
    while ((item = Func_080b7f70(*Func_080b7dd0(id), index)) != 0) {
        if (state[0x12a] != 1)
            Func_08009020(item, 4);
        else
            Func_08009020(item, 5);
        index++;
    }

    if (state[0x12a] == 1) {
        index = 0;
        while ((item = Func_080b7f70(*Func_080b7dd0(id), index)) != 0) {
            child = *(u8 **)(item + 40);
            items[index] = item;
            child[5] = 6;
            child[22] = 0xff;
            index++;
        }
        Func_080030f8(4);
        Func_080bac6c(id);
        Func_08009108(items, index);
        Func_080b7e60(id);
    }
}

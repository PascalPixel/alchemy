typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)

struct Entity {
    u8 pad0[15];
    u8 marker;
    u8 pad1[8];
    unsigned short value : 10;
    unsigned short reserved : 6;
};

s32 Func_08004080(void);
void Func_080215e0(s32, s32);
void *Func_0801eadc(s32, u32, void *, s32, s32);

void *Func_08021620(s32 arg0, void *arg1, s32 arg2, s32 arg3)
{
    s32 index;
    struct Entity *first;
    struct Entity *second;

    index = Func_08004080();
    if (index > 95)
        return NULL;

    Func_080215e0(arg0, index);
    first = Func_0801eadc(index, 0x80004000, arg1, arg2, arg3);
    first->marker = 0xFD;
    second = Func_0801eadc(index, 0x80004000, arg1, arg2 + 32, arg3);
    second->marker = 0xFD;
    second->value += 8;
    return first;
}

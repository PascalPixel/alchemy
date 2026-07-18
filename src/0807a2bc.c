typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct State_0807a2bc {
    u8 padding[0x108];
    u32 flags[32];
};

struct State_0807a2bc *Func_08077394(s32);

u32 Func_0807a2bc(s32 owner, s32 index, s32 bit)
{
    s32 value = Func_08077394(owner)->flags[index] & (1 << bit);

    return (u32)(-value | value) >> 31;
}

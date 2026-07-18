typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct State_08078948 {
    u8 padding[216];
    u16 entries[15];
};

struct State_08078948 *Func_08077394(s32);
s32 Func_080788c4(s32, s32);
s32 Func_08078ad0(s32, s32);
void Func_0808a548(s32);

s32 Func_08078948(s32 owner, s32 index)
{
    s32 value = Func_08077394(owner)->entries[index];
    s32 result = Func_080788c4(owner, index);

    if (result != -1) {
        Func_0808a548(Func_08078ad0(value, 1));
    }
    return result;
}

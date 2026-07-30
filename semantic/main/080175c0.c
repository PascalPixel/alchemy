typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

s32 Func_08018038(s32, s32);
void *Func_080165d8(void *, s32, s32, s32, const u16 *, s32);

s32 Func_080175c0(void *object, s32 kind)
{
    u8 *base = *(u8 **)0x03001e8c;
    s32 index;
    void *value;
    s32 result;
    u16 zero;

    zero = 0;
    *(u16 *)(base + 4852) = zero;
    *(u16 *)(base + 4854) = zero;
    index = Func_08018038(kind, 1);
    result = 0;
    if (*(u16 *)(base + index * 2 + 3760) != 0 && object != 0) {
        value = Func_080165d8(object, index, 0, 0, 0, 1);
        if (value != 0) {
            result = (s32)value;
        }
    }
    return result;
}

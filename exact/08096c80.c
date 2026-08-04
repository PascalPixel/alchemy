typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

u8 *Func_080090c8(s32, s32, s32, s32);
void Func_080090d0(void);
void Func_080091e0(u8 *, s32);
void Func_08009080(u8 *, s32);

u8 *Func_08096c80(s32 kind, s32 x, s32 y, s32 z)
{
    u8 *base = *(u8 **)0x03001f30;
    u8 *object;
    u8 *child;
    u8 flag;

    object = Func_080090c8(kind, x, y, z);
    if (object != 0) {
        if (object[84] == 0) {
            Func_080090d0();
            return 0;
        }
        *(u32 *)(object + 20) = *(u32 *)(*(u8 **)(base + 16) + 20);
        flag = 4;
        object[85] = flag;
        object[35] = flag;
        child = *(u8 **)(object + 80);
        child[9] &= ~(flag + 8);
        Func_080091e0(object, 0);
        Func_08009080(object, 1);
    }
    return object;
}

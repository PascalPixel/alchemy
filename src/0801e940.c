typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

u16 *Func_08004970(s32);
void Func_08017c8c(u16 *, s32, s32, s32);
void Func_08002df0(u16 *);

void Func_0801e940(u8 *s, s32 arg1, u32 arg2, u32 arg3)
{
    u16 *buffer = Func_08004970(0x200);
    u16 *p = buffer;

    while (*s != 0) {
        *p = *s;
        s++;
        p++;
    }
    *p = 0;
    arg2 >>= 3;
    arg3 >>= 3;
    Func_08017c8c(buffer, arg1, arg2, arg3);
    Func_08002df0(buffer);
}

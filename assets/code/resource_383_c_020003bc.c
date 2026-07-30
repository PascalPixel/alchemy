typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

extern u8 *Data_03001ebc;

extern void Func_020049ea(s32, s32, s32);
extern s32 Func_020051ea(s32, s32);
extern s32 Func_0200514a(s32, s32);
extern void Func_02005216(s32, s32);

void Func_020003bc(s32 x)
{
    Func_020049ea(x, 0, 2);
    Func_020051ea(x, 0);
    if (Func_0200514a(0, 0) != 0) {
        *(u16 *)(Data_03001ebc + 472) += 1;
    }
    Func_02005216(x, 0);
}

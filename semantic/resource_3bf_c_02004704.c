#include "types.h"

void Func_02009e26(s32, s32, s32);
void Func_02009e22(s32, s32, s32);
void Func_02009d48(s32);
void Func_02009e34(s32, s32, s32);
void Func_02009e34_b(s32);
void Func_02009d5a(s32);
void Func_02009e46(s32, s32, s32);
void Func_02009d6c(s32);
void Func_02009e4c(s32, s32);
void Func_02009e04(s32, s32);
void Func_02009d8a(s32);
void Func_02009e76(s32, s32, s32);
void Func_02009d9e(s32);
void Func_02009e64(s32);
void Func_02009e7c(s32, s32);

void Func_02004704(void)
{
    extern unsigned char Value_00002411;
    s32 t;

    Func_02009e26(21, 0x101, 30);
    Func_02009e22(21, 0xd000, 0);
    Func_02009d48(50);
    Func_02009e34(21, 0xb000, 0);
    Func_02009d5a(50);
    Func_02009e46(21, 0x5000, 0);
    Func_02009d6c(50);
    t = (s32)&Value_00002411;
    Func_02009e34_b(t);
    Func_02009e4c(21, 0);
    Func_02009e04(21, 4);
    Func_02009d8a(60);
    Func_02009e76(21, 0xb000, 0);
    t += 1;
    Func_02009d9e(40);
    Func_02009e64(t);
    Func_02009e7c(21, 0);
}

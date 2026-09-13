#include "types.h"

void Func_02004a3a(void);
void Func_02004a28(s32);
void Func_02004aea(s32, s32, s32);
void Func_02004a48(s32);
void Func_02004b16(s32);
void Func_02004af6(s32, s32);
void Func_02004a5c(s32);
void Func_02004b58(s32, s32, s32);
void Func_02004b58_a(s32, s32);
void Func_02004a76(s32);
void Func_02004b7a(s32, s32, s32);
void Func_02004b7e(s32, s32, s32);
void Func_02004b36(s32, s32);
void Func_02004a9c(s32);
void Func_02004b86(s32, s32, s32);
void Func_02004ba0(s32, s32, s32);
void Func_02004b40(s32, s32);
void Func_02004abe(s32);
void Func_02004ba8(s32, s32, s32);
void Func_02004aa4(s32, s32, s32, s32, s32, s32);
void Func_02004af0(void);

static __inline__ void Scene_Call3(void (*func)(s32, s32, s32), s32 a0, s32 a1, s32 a2)
{
    func(a0, a1, a2);
}

static __inline__ void Scene_Call6(void (*func)(s32, s32, s32, s32, s32, s32),
                                  s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    func(a0, a1, a2, a3, a4, a5);
}

void Func_02000658(void)
{
    Func_02004a3a();
    Func_02004a28(2196);
    Func_02004aea(9, 0, 0);
    Func_02004a48(10);
    Func_02004b16(6071);
    Func_02004af6(9, 2);
    Func_02004a5c(20);
    Scene_Call3(Func_02004b58, 0, 32768, 20);
    Func_02004b58_a(9, 0);
    Func_02004a76(10);
    Scene_Call3(Func_02004b7a, 9, 256, 80);
    Scene_Call3(Func_02004b7e, 9, 53248, 20);
    Func_02004b36(9, 2);
    Func_02004a9c(20);
    Func_02004b86(9, 0, 20);
    Func_02004ba0(9, 0, 20);
    Func_02004b40(9, 3);
    Func_02004abe(20);
    Func_02004ba8(9, 0, 20);
    Scene_Call6(Func_02004aa4, 10, 26, 1, 1, 10, 24);
    Func_02004af0();
}

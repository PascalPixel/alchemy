#include "types.h"

extern u16 Data_0200c57e[];

extern void Func_020063e2(void);
extern void Func_020064f6(void);
extern void Func_0200650a(void);
extern void Func_020063e8(s32);
extern void Func_0200654e(s32);
extern void Func_02004880(s32);
extern void Func_02004e0c(s32, s32);
extern void Func_02006402(s32);
extern void Func_02006416(void);
extern void Func_0200656e(s32);
extern void Func_0200641a(void);
extern void Func_0200652e(void);
extern void Func_02006542(void);
extern void Func_0200642a(s32);
extern void Func_02006592(s32);
extern void Func_020048c4(s32);
extern void Func_02004e50(s32, s32);
extern void Func_02006446(s32);
extern s32 Func_020065ba(void);
extern void Func_020062be(s32);
extern void Func_020065bc(s32);
extern void Func_020048ee(s32);
extern void Func_02004e7a(s32, s32);
extern void Func_020065d0(s32);
extern void Func_02006476(s32);
extern void Func_02004e8e(s32, s32);
extern void Func_020065e4(s32);
extern void Func_0200648a(s32);
extern void Func_0200491c(s32);
extern void Func_02004ea8(s32, s32);
extern void Func_020065fe(s32);
extern void Func_020064a4(s32);
extern void Func_02004936(s32);
extern void Func_02004ec2(s32, s32);
extern void Func_02006618(s32);
extern void Func_020065fc(void);
extern void Func_020064d0(void);
extern void Func_0200648e(s32);

void Func_02002738(s32 a)
{
    if (a == 0) {
        Func_020063e2();
        Func_020064f6();
        Func_0200650a();
        Func_020063e8(30);
        Func_0200654e(89);
        Func_02004880(0);
        Func_02004e0c(1, 0);
        Func_02006402(120);
        Func_02006416();
    } else {
        Func_0200656e(247);
        Func_0200641a();
        Func_0200652e();
        Func_02006542();
        Data_0200c57e[15] = a * 60;
        Func_0200642a(30);
        Func_02006592(a + 90);
        Func_020048c4(a);
        Func_02004e50(1, 0);
        Func_02006446(120);
        while (Func_020065ba() != 0) {
            Func_020062be(1);
        }
        Func_020065bc(0x121);
        Func_020048ee(5);
        Func_02004e7a(2, 0);
        Func_020065d0(236);
        Func_02006476(60);
        Func_02004e8e(2, 1);
        Func_020065e4(236);
        Func_0200648a(60);
        Func_0200491c(6);
        Func_02004ea8(2, 0);
        Func_020065fe(236);
        Func_020064a4(60);
        Func_02004936(7);
        Func_02004ec2(4, 0);
        Func_02006618(237);
        Func_020065fc();
        Func_020064d0();
        Func_0200648e(0x123);
    }
}

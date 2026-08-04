#include "types.h"

extern u8 Value_00000966;
extern u8 Value_00000967;
extern u8 Value_00002241;
extern u8 Value_00002245;

extern void Func_0200464a(void);
extern void Func_02004776(void);
extern s32 Func_02004614(s32);
extern void Func_02004626(s32);
extern void Func_0200462c(s32);
extern void Func_02004740(s32, s32, s32);
extern void Func_020046d2(s32, s32, s32);
extern void Func_02004756(s32, s32, s32);
extern void Func_02004684(s32);
extern void Func_0200474c(s32);
extern s32 Func_0200475c(s32, s32);
extern s32 Func_020046bc(s32, s32);
extern void Func_020046a6(s32);
extern void Func_0200476c(s32);
extern void Func_02004774(s32);
extern void Func_0200478c(s32, s32);
extern void Func_020046c2(s32);
extern void Func_0200475a(s32, s32);
extern void Func_020046d0(s32);
extern void Func_0200470e(s32, s32, s32);
extern void Func_02004842(s32, s32, s32);
extern void Func_0200484c(s32, s32, s32);
extern void Func_020047bc(s32);
extern void Func_020047cc(s32, s32);
extern void Func_02004718(void);

void Func_02000264(s32 a)
{
    Func_0200464a();
    Func_02004776();
    if (Func_02004614(0x966) == 0) {
        s32 k;

        Func_02004626(0x966);
        Func_0200462c(0x967);
        Func_02004740(a, 0x4000, 0);
        Func_020046d2(0, 0x78, 0x60);
        Func_02004756(0, 0xc000, 0);
        Func_02004684(20);
        k = (s32)&Value_00002241;
        Func_0200474c(k);
        Func_0200475c(a, 0);
        if (Func_020046bc(0, 0) == 0) {
            Func_020046a6(10);
            Func_0200476c(k + 1);
        } else {
            Func_02004774(k + 2);
        }
        Func_0200478c(a, 0);
        Func_020046c2(10);
        Func_0200475a(a, 3);
        Func_020046d0(20);
        Func_0200470e(a, 0x10000, 0x8000);
        Func_02004842(a, -0x40, 0);
        Func_0200484c(a, 0, 0x30);
    } else {
        Func_020047bc((s32)&Value_00002245);
        Func_020047cc(a, 0);
    }
    Func_02004718();
}

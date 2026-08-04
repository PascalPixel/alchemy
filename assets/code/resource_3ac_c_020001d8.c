#include "resource_3ac.h"

#include "types.h"
extern u8 Value_00001be0;
extern s32 Func_02000676(s32);
extern void Func_02000696(void);
extern void Func_020006b4(s32);
extern void Func_020006c4(s32, s32);
extern void Func_020006b0(void);
extern void Func_020006ae(void);
extern void Func_020006cc(s32);
extern void Func_020006dc(s32, s32);
extern void Func_020006c8(void);
void Func_020001d8(void)
{
    if (Func_02000676(0x941) != 0) {
        Func_02000696();
        Func_020006b4(0x24fa);
        Func_020006c4(16, 0);
        Func_020006b0();
    } else {
        Func_020006ae();
        Func_020006cc((s32)&Value_00001be0);
        Func_020006dc(16, 0);
        Func_020006c8();
    }
}

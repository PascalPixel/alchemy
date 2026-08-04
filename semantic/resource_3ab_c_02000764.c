#include "types.h"
extern u8 *Data_03001ebc;
extern void Func_0808a018(void);
extern s32 Func_080770c0(s32);
extern void Func_0808a170(s32);
extern void Func_0808a178(s32, s32);
extern s32 Func_0808a070(s32, s32);
extern void Func_0808a180(s32, s32);
extern void Func_0808a020(void);
void Func_02000764(void)
{
    Func_0808a018();
    if (Func_080770c0(0x85a) == 0) {
        Func_0808a170(0x1be1);
        Func_0808a180(18, 0);
    } else {
        Func_0808a170(0x1b9f);
        Func_0808a178(18, 0);
        if (Func_0808a070(0, 0) == 0)
            (*(u16 *)(*(u8 **)Data_03001ebc + 472))++;
        Func_0808a178(18, 0);
        if (Func_0808a070(0, 0) == 1)
            (*(u16 *)(*(u8 **)Data_03001ebc + 472))++;
        Func_0808a180(18, 0);
    }
    Func_0808a020();
}

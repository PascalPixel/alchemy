#include "resource_38c.h"

#include "types.h"

extern u8 *Func_020005fa(s32);
extern void Func_020005e0(void);
extern void Func_02000662(s32, s32);
extern s32 Func_020005e2(s32);
extern void Func_0200063c(s32);
extern void Func_02000644(s32);
extern void Func_0200065c(s32, s32);
extern void Func_02000620(void);

void Func_020000bc(void)
{
    u32 place;

    place = *(u16 *) (Func_020005fa(0) + 6);
    Func_020005e0();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000662(9, 18);
    } else {
        if (Func_020005e2(0x845) == 0) {
            Func_0200063c(0x13E9);
        } else {
            Func_02000644(0x16F9);
        }
        Func_0200065c(18, 0);
    }

    Func_02000620();
}

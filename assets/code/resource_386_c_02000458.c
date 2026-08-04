#include "types.h"

extern u8 *Func_02000ac2(s32);
extern void Func_02000aa0(void);
extern void Func_02000b4a(s32, s32);
extern void Func_02000b12(s32);
extern void Func_02000b2a(s32, s32);
extern void Func_02000ace(void);

void Func_02000458(void)
{
    u32 place;

    place = *(u16 *) (Func_02000ac2(0) + 6);
    Func_02000aa0();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Func_02000b4a(1, 22);
    } else {
        Func_02000b12(0x1CEC);
        Func_02000b2a(22, 0);
    }

    Func_02000ace();
}

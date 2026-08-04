#include "resource_390.h"

#include "types.h"

struct Approach390Subject {
    u16 unknown_00[3];
    u16 position;                   /* 0x06, wrapped 16-bit */
};

extern struct Approach390Subject *Func_020003d2(s32);
extern void Func_020003c0(void);
extern void Func_0200040a(s32, s32);
extern void Func_02000402(s32);
extern void Func_02000412(s32, s32);
extern void Func_020003ee(void);

void Func_020000e4(void)
{
    /* The local is wider than the field: read into a u16 the compiler reloads
     * it signed and re-normalises across the call. */
    u32 position = Func_020003d2(0)->position;

    Func_020003c0();

    if (position + 0xFFFF5FFF <= 0x3FFE) {
        Func_0200040a(14, 17);
    } else {
        Func_02000402(0x16AF);
        Func_02000412(17, 0);
    }

    Func_020003ee();
}

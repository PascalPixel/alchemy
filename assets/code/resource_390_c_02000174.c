#include "resource_390.h"

typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

struct Approach390Subject {
    u16 unknown_00[3];
    u16 position;                   /* 0x06, wrapped 16-bit */
};

extern struct Approach390Subject *Func_02000462(s32);
extern void Func_02000450(void);
extern void Func_020004a2(s32, s32);
extern void Func_02000492(s32);
extern void Func_020004a2(s32, s32);
extern void Func_0200047e(void);

void Func_02000174(void)
{
    /* The local is wider than the field: read into a u16 the compiler reloads
     * it signed and re-normalises across the call. */
    u32 position = Func_02000462(0)->position;

    Func_02000450();

    if (position + 0xFFFF5FFF <= 0x3FFE) {
        Func_020004a2(3, 19);
    } else {
        Func_02000492(0x16B7);
        Func_020004a2(19, 0);
    }

    Func_0200047e();
}

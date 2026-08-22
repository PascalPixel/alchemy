#include "resource_390.h"

#include "types.h"

struct Approach390Subject {
    u16 unknown_00[3];
    u16 position;                   /* 0x06, wrapped 16-bit */
};

extern struct Approach390Subject *Func_0200038a(s32);
extern void Func_02000378(void);
extern void Func_020003c2(s32, s32);
extern void Func_020003ba(s32);
extern void Func_020003ca(s32, s32);
extern void Func_020003a6(void);

void Func_0200009c(void)
{
    /* The local is wider than the field: read into a u16 the compiler reloads
     * it signed and re-normalises across the call. */
    u32 position = Func_0200038a(0)->position;

    Func_02000378();

    if (position + 0xFFFF5FFF <= 0x3FFE) {
        Func_020003c2(13, 16);
    } else {
        Func_020003ba(0x16AD);
        Func_020003ca(16, 0);
    }

    Func_020003a6();
}

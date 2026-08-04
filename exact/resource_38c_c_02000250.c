#include "resource_38c.h"

#include "types.h"

extern void Func_0200076c(void);
/* Two different imports land on this one name: the message at +8 and the
 * two-argument reader at +0x28. The `_b` alias is the same address carrying
 * its own prototype; a K&R declaration would serve both call shapes but would
 * cost the argument-setter order at every site that used it. */
extern void Func_020007aa(s32);
extern s32 Func_020007aa_b(s32, s32);
extern void Func_020007ac(s32, s32, s32);
extern void Func_0200077a(s32);
extern s32 Func_020007ca(s32, s32);
extern void Func_020007f6(s32, s32);
extern void Func_0200081a(s32, s32, s32);
extern void Func_020007c6(void);

#define WORKSPACE (*(u8 **) 0x03001EBC)

void Func_02000250(void)
{
    Func_0200076c();
    Func_020007aa(0x13F6);
    Func_020007ac(27, 0, 0);
    Func_0200077a(10);
    Func_020007ca(27, 0);

    if (Func_020007aa_b(0, 0) != 0) {
        ++*(u16 *) (WORKSPACE + 472);
    }

    Func_020007f6(27, 0);
    Func_0200081a(27, 0x4000, 10);
    Func_020007c6();
}

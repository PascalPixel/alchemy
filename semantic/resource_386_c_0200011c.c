#include "types.h"

extern void Func_0200075c(void);
extern void Func_020007ba(s32);
extern void Func_020007bc(s32, s32, s32);
extern void Func_020007ac(s32, s32);
extern void Func_020007ee(s32, s32, s32);
extern void Func_020007c6(s32, s32);
extern void Func_02000784(s32);
extern void Func_02000806(s32, s32, s32);
extern void Func_02000822(s32, s32, s32);
extern void Func_0200081c(s32, s32, s32);
/* Returns a value: the reference sets r1 before r0 at this site, which is what
 * a value-returning callee does and a void one does not. */
extern s32 Func_02000814(s32, s32);
extern s32 Func_020007dc(s32, s32);
extern void Func_0200084a(s32, s32, s32);
extern void Func_020007c2(s32);
extern void Func_020007c8(s32);
extern void Func_020007f4(void);

#define WORKSPACE (*(u8 **) 0x03001EBC)

void Func_0200011c(void)
{
    Func_0200075c();
    Func_020007ba(0x1CD4);
    Func_020007bc(16, 0, 2);
    Func_020007ac(16, 1);
    Func_020007ee(16, 0, 20);
    Func_020007c6(16, 4);
    Func_02000784(20);
    Func_02000806(16, 0, 20);
    /* r1 is built as 129 << 1. */
    Func_02000822(16, 0x102, 60);
    Func_0200081c(16, 0, 30);
    Func_02000814(16, 0);

    /* Skip-beat counter: the u16 at workspace + 472, built as 236 << 1. */
    if (Func_020007dc(0, 0) != 0) {
        ++*(u16 *) (WORKSPACE + 472);
    }

    Func_0200084a(16, 0, 20);
    /* r0 is built as 192 << 2. */
    Func_020007c2(0x300);
    Func_020007c8(0x868);
    Func_020007f4();
}

#include "resource_3a9.h"

#include "types.h"

extern u8 *Func_0200062a(int);
extern void Func_0200069e(int);
extern void Func_0200062c(void);
extern void Func_0200067a(int);
extern void Func_0200068a(int, int);
extern void Func_02000646(void);

void Func_020001fc(void)
{
    u8 *player = Func_0200062a(0);

    /* Band guard: facing in 0xa001..0xdfff. The reference falls through to the
     * short arm and branches away to the scene, so the test is spelled as the
     * short arm's condition. */
    if ((u16) (*(u16 *) (player + 6) + 0x5FFF) <= 0x3FFE) {
        Func_0200069e(8);
    } else {
        Func_0200062c();
        Func_0200067a(0x1A8F);
        Func_0200068a(8, 0);
        Func_02000646();
    }
}

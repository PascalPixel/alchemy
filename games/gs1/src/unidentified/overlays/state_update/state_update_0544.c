#include "resource_3a9.h"

#include "types.h"

extern u8 *Func_020005ba(s32);
extern void Func_02000638(s32, s32);
extern void Func_020005be(void);
extern s32 Func_020005ac(s32);
extern void Func_02000616(s32);
/* 0x02000626 serves two imports in sibling arms: the two-argument gesture in
 * the first and the one-argument message in the second. */
extern void Func_02000626(s32, s32);
extern void Func_02000626_b(s32);
extern void Func_0200063e(s32, s32);
extern void Func_020005de(s32);
extern void Func_020005fa(void);

void Func_0200018c(void)
{
    u8 *player = Func_020005ba(0);

    /* Band guard: facing in 0x6001..0x9fff. The reference falls through to the
     * short arm and branches away to the scene, so the test is spelled as the
     * short arm's condition. */
    if ((u16) (*(u16 *) (player + 6) - 0x6001) <= 0x3FFE) {
        Func_02000638(7, 8);
    } else {
        Func_020005be();

        if (Func_020005ac(0x911) != 0) {
            Func_02000616(0x1AFB);
            Func_02000626(8, 0);
        } else {
            Func_02000626_b(0x1AD7);
            Func_0200063e(8, 0);
            Func_020005de(0x910);           /* 145 << 4 */
        }

        Func_020005fa();
    }
}

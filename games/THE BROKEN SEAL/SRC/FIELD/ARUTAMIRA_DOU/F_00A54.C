#include "TYPES.H"

/* Set the blend mode, then the blend weights for the current fade level. */
void ArutamiraDou_Func02000a54(void)
{
    s8 level = *(s8 *)0x02001004;

    /* FAKEMATCH: the do-while wrap keeps the level's sign extension after
     * the BLDCNT store. */
    do {
        s32 blend = 0x3f42;

        *(volatile u16 *)0x04000050 = blend;
    } while (0);
    if (level == 0) {
        s32 alpha = 0x1000;

        *(volatile u16 *)0x04000052 = alpha;
    } else if (level == 1) {
        s32 alpha = 0xe00;

        *(volatile u16 *)0x04000052 = alpha;
    } else if (level == 2) {
        s32 alpha = 0xc00;

        *(volatile u16 *)0x04000052 = alpha;
    } else if (level == 3) {
        s32 alpha = 0xa00;

        *(volatile u16 *)0x04000052 = alpha;
    } else if (level == 4) {
        s32 alpha = 0x800;

        *(volatile u16 *)0x04000052 = alpha;
    } else {
        s32 alpha = 0x600;

        *(volatile u16 *)0x04000052 = alpha;
    }
}

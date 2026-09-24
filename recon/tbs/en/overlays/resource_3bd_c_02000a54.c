#include "TYPES.H"

/* NONMATCHING: 104 of 104 bytes, 2 halfword edits (2026-09-24). The
 * BLDCNT store is scheduled after the level sign extension; the reference
 * stores it right after the loads. */
void Func_02000a54(void)
{
    s8 level = *(s8 *)0x02001004;

    {
        s32 blend = 0x3f42;

        *(volatile u16 *)0x04000050 = blend;
    }
    if (level == 0) {
        {
            s32 alpha = 0x1000;

            *(volatile u16 *)0x04000052 = alpha;
        }
    } else if (level == 1) {
        {
            s32 alpha = 0xe00;

            *(volatile u16 *)0x04000052 = alpha;
        }
    } else if (level == 2) {
        {
            s32 alpha = 0xc00;

            *(volatile u16 *)0x04000052 = alpha;
        }
    } else if (level == 3) {
        {
            s32 alpha = 0xa00;

            *(volatile u16 *)0x04000052 = alpha;
        }
    } else if (level == 4) {
        {
            s32 alpha = 0x800;

            *(volatile u16 *)0x04000052 = alpha;
        }
    } else {
        {
            s32 alpha = 0x600;

            *(volatile u16 *)0x04000052 = alpha;
        }
    }
}

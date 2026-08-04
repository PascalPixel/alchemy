#include "types.h"

extern s16 Data_02000240[];
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_080000c0(s32 frames);
extern void Func_0808a1e0(s32 actorId, s32 mode);
extern void Func_0808a0f0(s32 actorId, s32 x, s32 z);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);
extern void Func_020000a0(s32 x, s32 y, s32 z, s32 mode);
extern s32 Func_02000528(s32, s32, s32, s32, s32, s32);
extern s32 Func_02000ba4(s32 actorId);
extern void Func_02001770(void);

/* Reconcile resource-specific actors and map occupancy for scenes 94 and 95.
 * The first scene groups ten selectors into three choreography arms; the
 * second rebuilds three flag-selected placement rectangles. */
void Func_02001984(void)
{
    s32 selector = Data_02000240[225];

    if (Data_02000240[224] == 94) {
        switch (selector) {
        case 1:
        case 2:
        case 3:
        case 4:
            Func_0808a1e0(15, 3);
            Func_0808a1e0(13, 3);
            Func_020000a0(0x00780000, 0, 0x00e80000, 0xdf);
            break;

        case 5:
        case 6:
        case 7:
            if (Func_080770c0(0x70) != 0)
                break;
            if (Func_080770c0(0x302) == 0)
                break;
            Func_080770c8(0x200);
            if (Data_02000240[225] == 5)
                Func_080770c8(0x23f);
            Func_080000c0(1);
            if (Func_080770c0(0x109) != 0)
                break;
            Func_0808a0f0(8, 0x03180000, 0x01180000);
            *(u32 *)(Func_0808a080(8) + 108) = 0x02008cf9;
            break;

        case 8:
        case 9:
        case 10:
            Func_020000a0(0x02820000, 0, 0x02280000, 20);
            Func_080091c0(23, 34, 13, 3, 0, 34);
            Func_02001770();
            if (Func_080770c0(0x200) != 0)
                Func_080091c0(23, 41, 1, 1, 23, 39);
            if (Func_080770c0(0x201) != 0)
                Func_080091c0(31, 39, 2, 1, 27, 41);
            break;
        }
        return;
    }

    if (Data_02000240[224] != 95 || selector < 1 || selector > 3)
        return;

    if (Func_080770c0(0x240) != 0) {
        Func_02000528(0, 12, 16, 1, 4, 0);
        Func_02000528(0, 13, 16, 1, 4, 0);
    } else {
        Func_02000ba4(9);
    }

    if (Func_080770c0(0x203) != 0) {
        Func_02000528(2, 16, 16, 1, 4, 0);
        Func_02000528(0, 16, 16, 1, 4, 0);
    } else {
        Func_02000ba4(10);
    }

    if (Func_080770c0(0x205) != 0) {
        Func_02000528(0, 13, 19, 4, 2, 0);
    } else if (Func_080770c0(0x204) != 0) {
        Func_02000528(0, 13, 15, 4, 2, 0);
        Func_080091c0(14, 17, 2, 1, 14, 16);
        Func_080091c0(14, 13, 1, 1, 14, 15);
    } else {
        Func_02000ba4(11);
        Func_0808a1e0(11, 3);
    }
}

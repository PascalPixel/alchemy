#include "types.h"

extern s16 Data_02000240[];












/* Reconcile resource-specific actors and map occupancy for scenes 94 and 95.
 * The first scene groups ten selectors into three choreography arms; the
 * second rebuilds three flag-selected placement rectangles. */
extern void Func_02003702(s32 actorId, s32 mode);
extern void Func_0200370a(s32 actorId, s32 mode);
extern void Func_02001a9e(s32 x, s32 y, s32 z, s32 mode);
extern s32 Func_02003692(s32 flagId);
extern s32 Func_0200369e(s32 flagId);
extern void Func_020036b4(s32 flagId);
extern void Func_020036ca(s32 flagId);
extern void Func_02003658(s32 frames);
extern s32 Func_020036ce(s32 flagId);
extern void Func_0200374a(s32 actorId, s32 x, s32 z);
extern u8 * Func_02003718(s32 actorId);
extern void Func_02001b10(s32 x, s32 y, s32 z, s32 mode);
extern void Func_020036f0(s32, s32, s32, s32, s32, s32);
extern void Func_020031f8(void);
extern s32 Func_0200371c(s32 flagId);
extern void Func_02003714(s32, s32, s32, s32, s32, s32);
extern s32 Func_0200373a(s32 flagId);
extern void Func_02003734(s32, s32, s32, s32, s32, s32);
extern s32 Func_02003774(s32 flagId);
extern s32 Func_02002028(s32, s32, s32, s32, s32, s32);
extern s32 Func_02002038(s32, s32, s32, s32, s32, s32);
extern s32 Func_020026bc(s32 actorId);
extern s32 Func_020037aa(s32 flagId);
extern s32 Func_0200205e(s32, s32, s32, s32, s32, s32);
extern s32 Func_0200206e(s32, s32, s32, s32, s32, s32);
extern s32 Func_020026f2(s32 actorId);
extern s32 Func_020037e0(s32 flagId);
extern s32 Func_02002096(s32, s32, s32, s32, s32, s32);
extern s32 Func_02003804(s32 flagId);
extern s32 Func_020020b6(s32, s32, s32, s32, s32, s32);
extern void Func_0200380e(s32, s32, s32, s32, s32, s32);
extern void Func_02003820(s32, s32, s32, s32, s32, s32);
extern s32 Func_02002760(s32 actorId);
extern void Func_020038e0(s32 actorId, s32 mode);
void Func_02001984(void)
{
    s32 selector = Data_02000240[225];

    if (Data_02000240[224] == 94) {
        switch (selector) {
        case 1:
        case 2:
        case 3:
        case 4:
            Func_02003702(15, 3);
            Func_0200370a(13, 3);
            Func_02001a9e(0x00780000, 0, 0x00e80000, 0xdf);
            break;

        case 6:
                break;
                break;
        case 5:
            if (Func_02003692(0x70) != 0)
            if (Func_0200369e(0x302) == 0)
            Func_020036b4(0x200);
        case 7:
            if (Data_02000240[225] == 5)
                Func_020036ca(0x23f);
            Func_02003658(1);
                break;
            if (Func_020036ce(0x109) != 0)
            Func_0200374a(8, 0x03180000, 0x01180000);
            *(u32 *)(Func_02003718(8) + 108) = 0x02008cf9;
            break;

        case 8:
        case 9:
        case 10:
            Func_02001b10(0x02820000, 0, 0x02280000, 20);
            Func_020036f0(23, 34, 13, 3, 0, 34);
            Func_020031f8();
            if (Func_0200371c(0x200) != 0)
                Func_02003714(23, 41, 1, 1, 23, 39);
            if (Func_0200373a(0x201) != 0)
                Func_02003734(31, 39, 2, 1, 27, 41);
            break;
        }
        return;
    }

    if (Data_02000240[224] != 95 || selector < 1 || selector > 3)
        return;

    if (Func_02003774(0x240) != 0) {
        Func_02002028(0, 12, 16, 1, 4, 0);
        Func_02002038(0, 13, 16, 1, 4, 0);
    } else {
        Func_020026bc(9);
    }

    if (Func_020037aa(0x203) != 0) {
        Func_0200205e(2, 16, 16, 1, 4, 0);
        Func_0200206e(0, 16, 16, 1, 4, 0);
    } else {
        Func_020026f2(10);
    }

    if (Func_020037e0(0x205) != 0) {
        Func_02002096(0, 13, 19, 4, 2, 0);
    } else if (Func_02003804(0x204) != 0) {
        Func_020020b6(0, 13, 15, 4, 2, 0);
        Func_0200380e(14, 17, 2, 1, 14, 16);
        Func_02003820(14, 13, 1, 1, 14, 15);
    } else {
        Func_02002760(11);
        Func_020038e0(11, 3);
    }
}

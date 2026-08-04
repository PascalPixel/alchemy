/*
 * resource_3c4 @ 0x02001f70 (84 bytes: 72 code + alignment + two pool words).
 *
 * Publishes selector 0x974 for slot 17 and 0x975 for slot 18, choosing a
 * different publisher for each depending on whether that slot's +8 word sits
 * at 12.20 row 45 and 46 respectively.  `asrs #20` makes both tests signed.
 * Both pool words are selectors, not addresses.
 *
 * `pop {r0} ; bx r0` return: void.
 */
#include "types.h"

typedef struct Slot_02001f70 {
    s32 pad0;
    s32 pad4;
    s32 w8;                /* +8, 12.20 fixed point */
} Slot_02001f70;

void Func_02005068(void);
Slot_02001f70 *Func_02005086(s32 slot);
Slot_02001f70 *Func_020050a2(s32 slot);
void Func_02005064(s32 selector);
void Func_02005074(s32 selector);
void Func_02005080(s32 selector);
void Func_02005090(s32 selector);
void Func_020044dc(void);
void Func_020050b0(void);

void Func_02001f70(void)
{
    Func_02005068();

    if ((Func_02005086(17)->w8 >> 20) == 45) {
        Func_02005064(0x974);
    } else {
        Func_02005074(0x974);
    }

    if ((Func_020050a2(18)->w8 >> 20) == 46) {
        Func_02005080(0x975);
    } else {
        Func_02005090(0x975);
    }

    Func_020044dc();
    Func_020050b0();
}

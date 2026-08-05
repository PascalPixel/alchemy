#include "types.h"

/*
 * Complete slot-to-active-choice resolver.  Each slot selects a run of nine
 * story flags; the first set flag chooses the corresponding shared result.
 */
extern const s32 Data_0200e718[];


extern s32 Func_02007708(s32 flag);
s32 Func_02003448(s32 slot)
{
    s32 first_flag;
    u32 choice;

    switch (slot) {
    case 1:
        first_flag = 0x935;
        break;
    case 0:
        first_flag = 0x92c;
        break;
    case 2:
        first_flag = 0x917;
        break;
    case 3:
        first_flag = 0x990;
        break;
    default:
        first_flag = 0;
        break;
    }

    for (choice = 0; choice <= 8; choice += 1) {
        if (Func_02007708(first_flag + choice) != 0) {
            return Data_0200e718[choice];
        }
    }
    return 0;
}

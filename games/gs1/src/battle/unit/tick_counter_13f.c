#include "types.h"

#define BattleUnit_TickCounter13f Func_080bf54c

u8 *Runtime_GetObject(void);

s32 BattleUnit_TickCounter13f(void) {
    u8 *value = Runtime_GetObject() + 0x13F;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            return 1;
        }
    }
    return 0;
}

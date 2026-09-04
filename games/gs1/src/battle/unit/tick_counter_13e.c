#include "types.h"

#define BattleUnit_TickCounter13e Func_080bf524

u8 *Runtime_GetObject(void);

s32 BattleUnit_TickCounter13e(void) {
    u8 *value = Runtime_GetObject() + 0x13E;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            return 1;
        }
    }
    return 0;
}

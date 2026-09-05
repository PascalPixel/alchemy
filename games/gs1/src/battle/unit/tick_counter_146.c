#include "types.h"

u8 *Runtime_GetObject(void);

s32 BattleUnit_TickCounter146(void) {
    u8 *base = Runtime_GetObject();
    u8 *value = base + 0x146;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            base[0x147] = 0;
            return 1;
        }
    }
    return 0;
}

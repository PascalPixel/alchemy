#include "types.h"

s32 Party_RemapCharacterIdIfEnabled(s32 arg0, s32 arg1) {
    s32 result;

    result = arg0;
    if ((result <= 8) && (arg1 != 0)) {
        if (result == 0) {
            result = 0x12;
        }
        if (result == 1) {
            result = 0x13;
        }
    }
    return result;
}

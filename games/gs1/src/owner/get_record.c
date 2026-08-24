#include "types.h"
#include "runtime_interfaces.h"

extern const u8 Data_08080ec8[];

const u8 *Owner_GetRecord(s32 arg0) {
    u32 var_r0;

    var_r0 = arg0 - 8;
    if (var_r0 > 0xF9U) {
        var_r0 = 0;
    }
    return Data_08080ec8 + var_r0 * 0x54;
}

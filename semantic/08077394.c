#include "types.h"

extern u8 Data_02000500[];
extern u8 *Data_03001f28;

u8 *Func_08077394(s32 owner)
{
    u8 *result = Data_02000500;

    if ((u32)owner <= 7) {
        result += owner * 0x14c;
    } else {
        if ((u32)(owner - 0x80) <= 5) {
            result = Data_03001f28;
            if (result != 0) {
                result += (owner - 0x80) * 0x14c;
                return result;
            }
        }
        result = 0;
    }
    return result;
}

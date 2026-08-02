#include "types.h"

struct OwnerState_08077394 {
    u8 bytes[0x14c];
};

extern struct OwnerState_08077394 Data_02000500[];
extern struct OwnerState_08077394 *Data_03001f28;

struct OwnerState_08077394 *Func_08077394(s32 owner)
{
    struct OwnerState_08077394 *result = Data_02000500;

    if ((u32)owner <= 7) {
        result += owner;
    } else {
        if ((u32)(owner - 0x80) <= 5) {
            result = Data_03001f28;
            if (result != 0) {
                result += owner - 0x80;
                return result;
            }
        }
        result = 0;
    }
    return result;
}

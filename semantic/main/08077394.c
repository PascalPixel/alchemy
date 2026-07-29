#include "types.h"

struct OwnerState_08077394 {
    u8 bytes[0x14c];
};

extern struct OwnerState_08077394 Data_02000500[];
extern struct OwnerState_08077394 *Data_03001f28;

struct OwnerState_08077394 *Func_08077394(s32 owner)
{
    u32 result;
    u32 base = (u32)Data_02000500;

    if ((u32)owner <= 7) {
        result = owner * sizeof(struct OwnerState_08077394);
        result += base;
    } else if ((u32)(owner - 0x80) <= 5 &&
               (base = (u32)Data_03001f28) != 0) {
        result = owner * sizeof(struct OwnerState_08077394);
        result += base;
        result -= 0xa600;
    } else {
        result = 0;
    }
    return (struct OwnerState_08077394 *)result;
}

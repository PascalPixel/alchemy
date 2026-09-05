#include "types.h"

extern const u8 Data_08084b1c[];

s32 Owner_GetRecordStride84(s32 arg0) {
    return (s32)(Data_08084b1c + arg0 * 0x54);
}

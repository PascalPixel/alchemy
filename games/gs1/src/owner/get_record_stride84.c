#include "types.h"

#define Owner_GetRecordStride84 Func_08079ad8

extern const u8 Data_08084b1c[];

s32 Owner_GetRecordStride84(s32 arg0) {
    return (s32)(Data_08084b1c + arg0 * 0x54);
}

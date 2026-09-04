#include "types.h"

#define Owner_GetRecordStride180 Func_08078ed8

typedef struct {
    u8 bytes[0xB4];
} Data_080844ec_Record;

extern Data_080844ec_Record Data_080844ec[];

Data_080844ec_Record *Owner_GetRecordStride180(s32 index) {
    Data_080844ec_Record *base;

    base = Data_080844ec;
    return &base[index];
}

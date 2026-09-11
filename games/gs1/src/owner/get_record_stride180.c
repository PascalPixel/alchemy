#include "types.h"
#include "scene.h"
#include "abi/owner/get_record_stride180.h"

typedef struct {
    u8 bytes[0xB4];
} gRom;

extern gRom gRom2[];

gRom *Owner_GetRecordStride180(s32 index)
{
    gRom *base;

    base = gRom2;
    return &base[index];
}

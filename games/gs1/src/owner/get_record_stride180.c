#include "types.h"
#include "scene.h"

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

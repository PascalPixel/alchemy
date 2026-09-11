#include "types.h"
#include "scene.h"

extern s32 gRom[];

s32 Owner_LookupFourColumnTable(s32 row, s32 column)
{
    return gRom[(row * 4) + column];
}

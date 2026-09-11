#include "types.h"
#include "scene.h"
#include "abi/owner/get_record_stride84.h"

extern const u8 gRom[];

s32 Owner_GetRecordStride84(s32 arg0)
{
    return (s32)(gRom + arg0 * 0x54);
}

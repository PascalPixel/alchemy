#include "types.h"
#include "scene.h"
#include "abi/resource/copy_fixed_block_a.h"

s32 Resource_CopyData(s32, s32, s32);
extern const u8 gRom[];

void Resource_CopyFixedBlockA(s32 arg0)
{
    Resource_CopyData(arg0, 0x80, (s32)gRom);
}

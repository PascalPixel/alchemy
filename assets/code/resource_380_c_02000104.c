#include "types.h"

void Func_02000104(s32 obj)
{
    *(s32 *)(obj + 0x08) += *(s32 *)(obj + 0x44);
    *(s32 *)(obj + 0x0c) += *(s32 *)(obj + 0x48);
    *(s32 *)(obj + 0x10) += *(s32 *)(obj + 0x4c);
    *(s32 *)(obj + 0x18) += *(s32 *)(obj + 0x30);
    *(s32 *)(obj + 0x1c) += *(s32 *)(obj + 0x34);
    *(u16 *)(*(u8 **)(obj + 0x50) + 0x1e) += *(u16 *)(obj + 0x64);
}

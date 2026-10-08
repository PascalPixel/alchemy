#include "types.h"

struct DisplayBlock {
    u8 unknown_000[0x18a];
    s8 mode;
    s8 pending;
    u8 unknown_18c[0x40];
    u8 *object;
};

struct DisplayBlock *Runtime_AllocateBlock(s32 slot, s32 size);
void Func_08020270(void);

void Func_080cb8a4(void)
{
    struct DisplayBlock *block = Runtime_AllocateBlock(108, 0xd50);

    if (block->mode == 3 && block->pending != 0) {
        block->pending = 0;
        block->object[91] = 0;
        Func_08020270();
    }
}

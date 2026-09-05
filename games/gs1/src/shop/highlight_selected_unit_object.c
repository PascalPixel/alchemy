#include "types.h"

void Object_InitializeMode(void *, s32);
s32 Func_080b27b0(s32, s32);

extern u8 *Data_03001f2c;

void Shop_HighlightSelectedUnitObject(s32 enabled, s32 selected)
{
    u8 *state;
    u8 *half_base;
    s32 *item;
    s32 index;
    s32 offset;
    s32 variant;
    s16 id;

    state = Data_03001f2c;
    variant = *(s8 *)(state + 0x3aa);
    if (enabled != 0) {
        index = 0;
        if (index < *(s8 *)(state + 0x3a7)) {
            half_base = state + 2;
            offset = 0x36c;
            item = (s32 *)(state + 0x114);
            do {
                if (index == selected)
                    Object_InitializeMode((void *)*item, 30);
                else
                    Object_InitializeMode((void *)*item, 1);
                item[16] = 0x10000;
                id = *(s16 *)(half_base + offset);
                if (Func_080b27b0(id, variant) == 0)
                    item[16] = 0xb333;
                index++;
                offset += 2;
                item++;
            } while (index < *(s8 *)(state + 0x3a7));
        }
    }
}

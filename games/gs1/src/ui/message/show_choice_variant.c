#include "types.h"
#include "global_cells.h"

#define UiMessage_ShowChoiceVariant Func_080b0664

void Func_080a1038(void *);
s32 Func_080153b0(s32, s32, s32);

s32 UiMessage_ShowChoiceVariant(s32 arg0) {
    u8 **slot = (u8 **)(*(u8 **)ADDR_03001F2C + 0x380);
    u8 saved = (*slot)[5];
    Func_080a1038(*slot);
    arg0 = Func_080153b0(7, 7, arg0);
    (*slot)[5] = saved;
    return arg0;
}

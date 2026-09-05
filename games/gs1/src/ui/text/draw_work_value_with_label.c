#include "types.h"

struct SharedWork080a23c0 {
    u8 padding_00[0x10];
    s32 resource;
};

extern s32 Func_080150a8(s32, s32, s32, s32, s32);
extern void UiText_DrawAt(s32, s32, s32, s32);
extern struct SharedWork080a23c0 Data_02000240;

void UiText_DrawWorkValueWithLabel(s32 work) {
    Func_080150a8(Data_02000240.resource, 7, work, 8, 0);
    UiText_DrawAt(0xB0B, work, 0x40, 0);
}

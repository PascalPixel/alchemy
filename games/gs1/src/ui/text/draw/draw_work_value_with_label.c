#include "types.h"
#include "scene.h"
#include "abi/ui/text/draw/draw_work_value_with_label.h"

struct SharedWork080a23c0 {
    u8 padding_00[0x10];
    s32 resource;
};

extern void UiText_DrawAt(s32, s32, s32, s32);
extern struct SharedWork080a23c0 gCell;

void UiText_DrawWorkValueWithLabel(s32 work)
{
    Ui_SetRange(gCell.resource, 7, work, 8, 0);
    UiText_DrawAt(0xB0B, work, 0x40, 0);
}

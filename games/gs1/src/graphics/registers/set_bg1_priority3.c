#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "render_input.h"

/* graphics/registers/set_bg1_priority3.c */
/* graphics/registers/set_bg1_priority3.c */
#define REG_BG1CNT (*(volatile u16 *)0x0400000a)

void Graphics_SetBg1Priority3(void)
{
    u32 mask = 4;
    u32 value = REG_BG1CNT;

    mask = -mask;
    value &= mask;
    value |= 3;
    REG_BG1CNT = value;
}

/* graphics/registers/clear_bg1_control_bit2.c */
#define REG_BG1CNT (*(volatile u16 *)0x0400000a)

void Graphics_ClearBg1ControlBit2(void)
{
    u32 mask = 4;
    u32 value = REG_BG1CNT;

    mask = -mask;
    value &= mask;
    REG_BG1CNT = value;
}

/* ui/text_show_message_and_wait_complete.c */
/* ui/text/msg/show_message_and_wait_complete.c */
void WaitFrames(s32);
s32 UiWork_IsComplete(void);

s32 UiText_ShowMessageAndWaitComplete(s32 arg0, s32 arg1, s32 arg2)
{
    s32 result;

    result = Ui_SetMode(arg0, arg1, arg2, 1);
    goto check;
again:
    WaitFrames(1);
check:
    if (UiWork_IsComplete() == 0) {
        goto again;
    }
    return result;
}

/* ui/text/fmt/format_number_to_halfwords.c */
s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value)
{
    s16 *dst;
    s32 n;
    u8 *src;

    dst = out;
    Ui_Do(value);
    src = (u8 *)ADDR_03001F70;
    n = 0xD;
    do {
        n -= 1;
        *dst = (s16)*src;
        src += 1;
        dst += 1;
    } while (n >= 0);
}

/* ui/render/create_output_from_resource_slot.c */
s32 Resource_FindFreeSlot();

s32 Ui_CreateOutputFromResourceSlot(
    struct RenderInput *input,
    s32 arg1,
    s32 arg2,
    s32 resource)
{
    s32 slot;

    slot = Resource_FindFreeSlot();
    if (slot != 0x60) {
        Ui_Apply(resource, slot);
        Ui_Run(slot, 0x40000000, input, arg1, arg2);
    }
}

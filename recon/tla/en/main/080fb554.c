#include "TYPES.H"
#include "ITEM.H"

#define COMMAND_DISABLED (-1)
#define COMMAND_AVAILABLE 1

s32 Item_ClassifyUseMode(s32 owner, s32 item);
s32 BattleFx_HasTriggerFar(s32 item);

void ItemMenu_DrawCmd(void *command_states, s32 window)
{
    s32 disabled;
    s32 value;
    u32 message;

    UiWork_SetParamNibbleFar(0xf);
    value = FIELD(command_states, s8 *, 0);
    disabled = -1;
    if (value == disabled)
        UiWork_SetParamNibbleFar(0xe);

    message = (u32)&MsgItemCommandUse;
    UiText_DrawCharacterAtOffsetFar(message, window, 0, 0x18);
    UiWork_SetParamNibbleFar(0xf);
    if (FIELD(command_states, s8 *, 1) == disabled)
        UiWork_SetParamNibbleFar(0xe);

    UiText_DrawCharacterAtOffsetFar(message + 1, window, ITEM_TEXT_X, 0x18);
    UiWork_SetParamNibbleFar(0xf);
    if (FIELD(command_states, s8 *, 3) == disabled)
        UiWork_SetParamNibbleFar(0xe);

    UiText_DrawCharacterAtOffsetFar(message + 2, window, 0, 0x20);
    UiWork_SetParamNibbleFar(0xf);
    if (FIELD(command_states, s8 *, 5) == disabled)
        UiWork_SetParamNibbleFar(0xe);

    UiText_DrawCharacterAtOffsetFar(message + 3, window, 0x50, 0x20);
    UiWork_SetParamNibbleFar(0xf);
    if (FIELD(command_states, s8 *, 2) == disabled)
        UiWork_SetParamNibbleFar(0xe);

    UiText_DrawCharacterAtOffsetFar(message + 4, window, 0x50, 0x18);
    UiWork_SetParamNibbleFar(0xf);
    if (FIELD(command_states, s8 *, 4) == disabled)
        UiWork_SetParamNibbleFar(0xe);

    UiText_DrawCharacterAtOffsetFar(message + 5, window, ITEM_TEXT_X, 0x20);
    UiWork_SetParamNibbleFar(0xf);
}

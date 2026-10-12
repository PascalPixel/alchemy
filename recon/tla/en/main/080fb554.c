#include "types.h"

extern char MsgItemCommandUse[];

s32 UiWork_SetParamNibbleFar(s32 color);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

// コマンドの並び（command_states の添字）
enum {
    ITEM_COMMAND_USE,
    ITEM_COMMAND_EQUIP,
    ITEM_COMMAND_INSPECT,
    ITEM_COMMAND_GIVE,
    ITEM_COMMAND_REMOVE,
    ITEM_COMMAND_DROP
};

#define ITEM_TEXT_X 40

// どうぐのコマンド 6 つを描く。使えないものは色 14
void ItemMenu_DrawCmd(const s8 *command_states, s32 window)
{
    s32 disabled;
    s32 value;
    u32 message;

    UiWork_SetParamNibbleFar(0xf);
    value = command_states[ITEM_COMMAND_USE];
    disabled = -1;
    if (value == disabled)
        UiWork_SetParamNibbleFar(0xe);

    message = (u32)MsgItemCommandUse;
    UiText_DrawCharacterAtOffsetFar(message, window, 0, 0x18);
    UiWork_SetParamNibbleFar(0xf);
    if (command_states[ITEM_COMMAND_EQUIP] == disabled)
        UiWork_SetParamNibbleFar(0xe);

    UiText_DrawCharacterAtOffsetFar(message + 1, window, ITEM_TEXT_X, 0x18);
    UiWork_SetParamNibbleFar(0xf);
    if (command_states[ITEM_COMMAND_GIVE] == disabled)
        UiWork_SetParamNibbleFar(0xe);

    UiText_DrawCharacterAtOffsetFar(message + 2, window, 0, 0x20);
    UiWork_SetParamNibbleFar(0xf);
    if (command_states[ITEM_COMMAND_DROP] == disabled)
        UiWork_SetParamNibbleFar(0xe);

    UiText_DrawCharacterAtOffsetFar(message + 3, window, 0x50, 0x20);
    UiWork_SetParamNibbleFar(0xf);
    if (command_states[ITEM_COMMAND_INSPECT] == disabled)
        UiWork_SetParamNibbleFar(0xe);

    UiText_DrawCharacterAtOffsetFar(message + 4, window, 0x50, 0x18);
    UiWork_SetParamNibbleFar(0xf);
    if (command_states[ITEM_COMMAND_REMOVE] == disabled)
        UiWork_SetParamNibbleFar(0xe);

    UiText_DrawCharacterAtOffsetFar(message + 5, window, ITEM_TEXT_X, 0x20);
    UiWork_SetParamNibbleFar(0xf);
}

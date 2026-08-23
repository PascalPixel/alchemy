#include "inventory_menu.h"
#include "gs1_edition.h"

#if defined(GS1_EDITION_JA)
#define ITEM_TEXT_X 0x28
#else
#define ITEM_TEXT_X 0x20
#endif

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void UiText_DrawAt(s32, s32, s32, s32);
extern char Value_00000b33;
#define InventoryMenu_CommandUseMessage Value_00000b33

void InventoryMenu_DrawCommandLabels(void *command_states, s32 window)
{
    s32 disabled;
    s32 value;
    u32 message;

    UiPalette_SetColor(0xf);
    value = FIELD(command_states, s8 *, 0);
    disabled = -1;
    if (value == disabled)
        UiPalette_SetColor(0xe);

    message = (u32)&InventoryMenu_CommandUseMessage;
    UiText_DrawAt(message, window, 0, 0x18);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 1) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 1, window, ITEM_TEXT_X, 0x18);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 3) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 2, window, 0, 0x20);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 5) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 3, window, 0x50, 0x20);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 2) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 4, window, 0x50, 0x18);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 4) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 5, window, ITEM_TEXT_X, 0x20);
    UiPalette_SetColor(0xf);
}

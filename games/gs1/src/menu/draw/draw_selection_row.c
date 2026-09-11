#include "types.h"
#include "scene.h"
#include "abi/menu/draw/draw_selection_row.h"
#include "gs1_edition.h"

struct Work;

void RenderOutput_PrepareForRedraw(void *);
void UiText_DrawNumber(s32 n, s32 digits, s32 work, s32 x, s32 y);
void UiText_DrawString(u8 *str, s32 work, s32 x, s32 y);
void UiText_DrawResource(s32 no, s32 work, s32 x, s32 y);
extern u8 Value_0000099b[];
extern u8 Value_00000a07[];
extern u8 gRom[];
void Menu_DrawSelectionRow(struct Work *work, s16 first, const s16 *second)
{
    s16 selected = first;
    s32 label = Menu_Apply(selected, *second) + (s32)Value_0000099b;
    RenderOutput_PrepareForRedraw(work);
    UiText_DrawNumber(selected, 3, (s32)work, 0, 14);
    UiText_DrawNumber(*second, 3, (s32)work, MENU_LABEL_X, 14);
    UiText_DrawString(gRom, (s32)work, MENU_TEXT_X, 0);
    UiText_DrawResource(selected + (s32)Value_00000a07, (s32)work, 0, 0);
    UiText_DrawString(gRom, (s32)work, MENU_TEXT_X, 14);
    UiText_DrawResource(label, (s32)work, MENU_LABEL_X, 0);
}

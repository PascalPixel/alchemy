#include "TYPES.H"
#include "SCENE.H"
#include "SYSTEM.H"
s32 BattleFx_FindConditionResourceFar(s16, s16);
void Event_SetPairWork1c0Far(s16 primary, s16 secondary);
void UiTextResource_Release(s32 resource);

/* menu/draw/draw_selection_row.c */
struct Work;

void RenderOutput_PrepareForRedraw();
void UiText_DrawNumber(s32 n, s32 digits, s32 work, s32 x, s32 y);
void UiText_DrawString(u8 *str, s32 work, s32 x, s32 y);
void UiText_DrawResource(s32 no, s32 work, s32 x, s32 y);
extern u8 MsgDebugEntryName[];
extern u8 MsgDebugCategoryName[];
extern u8 Menu_ColonString[];

void Menu_DrawSelectionRow(struct Work *work, s16 first, const s16 *second)
{
    s16 selected = first;
    s32 label = BattleFx_FindConditionResourceFar(selected, *second) + (s32)MsgDebugEntryName;
    RenderOutput_PrepareForRedraw(work);
    UiText_DrawNumber(selected, 3, (s32)work, 0, 14);
    UiText_DrawNumber(*second, 3, (s32)work, MENU_LABEL_X, 14);
    UiText_DrawString(Menu_ColonString, (s32)work, MENU_TEXT_X, 0);
    UiText_DrawResource(selected + (s32)MsgDebugCategoryName, (s32)work, 0, 0);
    UiText_DrawString(Menu_ColonString, (s32)work, MENU_TEXT_X, 14);
    UiText_DrawResource(label, (s32)work, MENU_LABEL_X, 0);
}

#include "types.h"

#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
// スペイン語版とイタリア語版のビルドでは、この far call の名前が違う（shop.c と同じ）
s32 Func_080c8638(s32 scene, s32 entrance);
#else
s32 Scene_GetAreaNameIndexFar(s32 scene, s32 entrance);
#endif
void RenderOutput_PrepareForRedraw(void *window);
void UiText_DrawNumber(s32 value, s32 digits, void *window, s32 x, s32 y);
void UiText_DrawString(const u8 *text, void *window, s32 x, s32 y);
void UiText_DrawResource(s32 message, void *window, s32 x, s32 y);
extern const u8 Menu_ColonString[];
extern u8 MsgDebugEntryName[];
extern u8 MsgDebugCategoryName[];

#if defined(TLA_EDITION_JA)
#define SELROW_TEXT_X 100
#define SELROW_LABEL_X 110
#else
#define SELROW_TEXT_X 74
#define SELROW_LABEL_X 82
#endif

void Menu_DrawSelectionRow(void *window, s16 first, const s16 *second)
{
    s16 selected = first;
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
    s32 label = Func_080c8638(selected, *second) + (s32)MsgDebugEntryName;
#else
    s32 label = Scene_GetAreaNameIndexFar(selected, *second) + (s32)MsgDebugEntryName;
#endif

    RenderOutput_PrepareForRedraw(window);
    UiText_DrawNumber(selected, 3, window, 0, 14);
    UiText_DrawNumber(*second, 3, window, SELROW_LABEL_X, 14);
    UiText_DrawString(Menu_ColonString, window, SELROW_TEXT_X, 0);
    UiText_DrawResource(selected + (s32)MsgDebugCategoryName, window, 0, 0);
    UiText_DrawString(Menu_ColonString, window, SELROW_TEXT_X, 14);
    UiText_DrawResource(label, window, SELROW_LABEL_X, 0);
}

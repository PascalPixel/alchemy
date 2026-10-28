#include "types.h"
#include "edition.h"

s32 Scene_GetAreaNameIndexFar(s32 scene, s32 entrance);
void WindowSprite_PrepareForRedraw(void *window);
void Text_DrawNumber(s32 value, s32 digits, void *window, s32 x, s32 y);
void Text_DrawString(const u8 *text, void *window, s32 x, s32 y);
void Text_DrawMessage(s32 message, void *window, s32 x, s32 y);
extern const u8 gMenu_ColonString[];
extern u8 MsgDebugEntryName[];
extern u8 MsgDebugCategoryName[];

#if (LANGUAGE == LANGUAGE_JAPANESE)
#define SELROW_TEXT_X 100
#define SELROW_LABEL_X 110
#else
#define SELROW_TEXT_X 74
#define SELROW_LABEL_X 82
#endif

void Menu_DrawSelectionRow(void *window, s16 first, const s16 *second) {
  s16 selected = first;
  s32 label =
      Scene_GetAreaNameIndexFar(selected, *second) + (s32)MsgDebugEntryName;

  WindowSprite_PrepareForRedraw(window);
  Text_DrawNumber(selected, 3, window, 0, 14);
  Text_DrawNumber(*second, 3, window, SELROW_LABEL_X, 14);
  Text_DrawString(gMenu_ColonString, window, SELROW_TEXT_X, 0);
  Text_DrawMessage(selected + (s32)MsgDebugCategoryName, window, 0, 0);
  Text_DrawString(gMenu_ColonString, window, SELROW_TEXT_X, 14);
  Text_DrawMessage(label, window, SELROW_LABEL_X, 0);
}

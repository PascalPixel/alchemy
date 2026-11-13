/// @file
/// The debug menus' destination row.
// Part of menus.c.

#include "../menus_internal.h"
#include "gamepak/far_call/field_sequence.h"
#include "module/window/menus.h"

/* VIEW(selrow): routine WindowSprite_PrepareForRedraw as void
 * WindowSprite_PrepareForRedraw(void *window); resource_work has void
 * WindowSprite_PrepareForRedraw(struct Window *). */
extern void WindowSprite_PrepareForRedraw__selrow(void *window) __asm__(
    "WindowSprite_PrepareForRedraw");
void Text_DrawNumber(s32 value, s32 digits, void *window, s32 x, s32 y);
/* VIEW(selrow): routine Text_DrawString as void Text_DrawString(const u8 *text,
 * void *window, s32 x, s32 y); graphics.h has void Text_DrawString(u8 *text,
 * s32 window, s32 x, s32 y). */
extern void Text_DrawString__selrow(const u8 *text, void *window, s32 x,
                                    s32 y) __asm__("Text_DrawString");

#if (LANGUAGE == LANGUAGE_JAPANESE)
#define SELROW_TEXT_X 100
#define SELROW_LABEL_X 110
#else
#define SELROW_TEXT_X 74
#define SELROW_LABEL_X 82
#endif

/// Draws the debug scene and entrance numbers with their scene code and area
/// name. The first line has the scene's code (MsgDebugSceneCode + scene)
/// and, beside it, the area its entrance leads to (MsgDebugAreaName + area);
/// the numbers, three digits each, go under them.
void DebugSceneSelect_DrawRow(void *window, s16 first, const s16 *second) {
  s16 selected = first;
  s32 label = Area_GetNameIndexFar(selected, *second) + (s32)MsgDebugAreaName;

  WindowSprite_PrepareForRedraw__selrow(window);
  Text_DrawNumber(selected, 3, window, 0, 14);
  Text_DrawNumber(*second, 3, window, SELROW_LABEL_X, 14);
  Text_DrawString__selrow(gMenu_ColonString, window, SELROW_TEXT_X, 0);
  Text_DrawMessage__menus(selected + (s32)MsgDebugSceneCode, window, 0, 0);
  Text_DrawString__selrow(gMenu_ColonString, window, SELROW_TEXT_X, 14);
  Text_DrawMessage__menus(label, window, SELROW_LABEL_X, 0);
}

struct Work;

/* VIEW(selrow): routine MenuArrow_Create as void MenuArrow_Create(struct
 * TextObject *object, s32 *resource); menu.h has void MenuArrow_Create(struct
 * Actor_0801c0dc *obj, s32 *slot). */
extern void
UiTextResource_Initialize__selrow(struct TextObject *object,
                                  s32 *resource) __asm__("MenuArrow_Create");
/* VIEW(selrow): routine MenuArrow_SetPosition as void
 * MenuArrow_SetPosition(struct TextObject *object, s32 x, s32 y); menu.h has
 * void MenuArrow_SetPosition(struct Obj *obj, s32 arg1, s32 arg2). */
extern void
UiTextResource_SetPosition__selrow(struct TextObject *object, s32 x,
                                   s32 y) __asm__("MenuArrow_SetPosition");
/* VIEW(selrow): routine Window_Create as struct Work *Window_Create(s32 kind,
 * s32 x, s32 y, s32 width, s32 layer); resource_work has struct Window
 * *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 style). */
extern struct Work *Window_Create__selrow(s32 kind, s32 x, s32 y, s32 width,
                                          s32 layer) __asm__("Window_Create");
/* VIEW(selrow): routine Window_Close as void Window_Close(struct Work *work,
 * s32 release); uiflow.h has void Window_Close(struct Window *window, s32
 * release). */
extern void Window_Close__selrow(struct Work *work,
                                 s32 release) __asm__("Window_Close");
/* VIEW(selrow): routine DebugSceneSelect_HandleRowInput as s32
 * DebugSceneSelect_HandleRowInput(struct Work *work, s16 value, s16 *sub, s16
 * *mode); the definition has s32 DebugSceneSelect_HandleRowInput(s32 arg, s16
 * value, s16 *sub, s16 *mode); tried as the rest of the object declares it, it
 * does not compile in tla-ja (make modern):
 * games/tla/src/window/menus.c:129:74: note: expected 'struct TargetWindow *'
 * but argument is of type 's32' {aka 'int'}. */
extern s32 Menu_HandleSelectionRowInput__selrow(
    struct Work *work, s16 value, s16 *sub,
    s16 *mode) __asm__("DebugSceneSelect_HandleRowInput");

/// Picks a destination (primary and secondary) in a row menu and travels
/// there; returns -1 after travelling and -2 when cancelled.
s16 DebugSceneSelect_Run(void) {
  s32 resource;
  s16 mode;
  s16 secondary;
  struct TextObject object;
  s16 primary;
  struct Work *work;
  s16 result;

  work = 0;
  mode = 0;
  primary = gMainRoutineWork.primary;
  secondary = gMainRoutineWork.secondary;
  work = Window_Create__selrow(0, 7, 30, 5, 2);
  DebugSceneSelect_DrawRow(work, primary, &secondary);
  UiTextResource_Initialize__selrow(&object, &resource);

  while (gInput.held != 0)
    wait(1);

  for (;;) {
    result = (s16)Menu_HandleSelectionRowInput__selrow(work, primary,
                                                       &secondary, &mode);
    if (result == -1) {
      MenuArrow_Release(resource);
      Window_Close__selrow(work, 2);
      Event_SetDestinationFar(primary, secondary);
      return result;
    }
    if (result == -2) {
      MenuArrow_Release(resource);
      Window_Close__selrow(work, 2);
      return result;
    }

    UiTextResource_SetPosition__selrow(&object, MENU_TEXT_X,
                                       mode * 14 + MENU_TEXT_Y);
    primary = result;
    wait(1);
  }
}
#undef SELROW_TEXT_X
#undef SELROW_LABEL_X

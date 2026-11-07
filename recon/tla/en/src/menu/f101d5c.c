#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "task.h"
#include "window.h"

struct UiPanel {
  u8 unk00[0xc];
  s16 x;
  s16 y;
};

struct DjinnMenuWork {
  u8 unk000[0x10];
  struct UiPanel *statusWindow;
  u8 unk014[0x34 - 0x14];
  struct UiPanel *listWindow;
  u8 unk038[0xf0 - 0x38];
  struct UiPanel *helpWindow;
};

void WindowSprite_ClearListFar(void *window);
void WindowSprite_PrepareForRedrawFar(void *window);
void Text_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);
void Text_DrawMessageFar(s32 message, void *window, s32 x, s32 y);
void Func_08101c7c(s32 x, s32 y, s32 width, s32 height, s32 palette);
void UiPanel_SetRectPalette(struct UiPanel *panel, s32 x, s32 y, s32 width,
                            s32 height, s32 palette);
void UiPanel_SetRectHighlight(struct UiPanel *panel, s32 x, s32 y, s32 width,
                              s32 height, s32 palette);
void *Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 flags);
void Func_08038290(void);
struct UiChannelSlot *Func_080383d8(void *window, s32 message);
s32 Menu_GetModuloOfSum(s32 value, s32 count);
void Sound_PlayFar(s32 cue);
s32 Message_AllTypedFar(void);
void Func_080f8a44(s32 x, s32 y);
s32 Resource_ResetEntry(u32 entry);
void Window_CloseFar(void *window, s32 mode);
void Menu_UpdateEntryObjectTransforms(void);
void wait(s32 frames);
extern char MsgDjinnInfoPrompt[], MsgDjinnInfoTopic[], MsgDjinnInfoText[];

s32 Func_08101d5c(void) {
  struct UiPanel *helpWindow;
  struct UiPanel *statusWindow;
  struct DjinnMenuWork *menu;
  struct WindowWork *render;
  s32 result;
  s32 previous;
  s32 selection;
  struct UiPanel *list;
  struct UiChannelSlot *channel;
  s32 message;
  s32 helpIndex;
  s32 listMessage;

  menu = Ram_HeapSlots->menuRuntime;
  selection = 0;
  render = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  result = 0;
  previous = 0;
  WindowSprite_ClearListFar(menu->listWindow);
  wait(1);
  WindowSprite_PrepareForRedrawFar(menu->helpWindow);
  message = (s32)MsgDjinnInfoPrompt;
  Text_DrawCharacterAtOffsetFar(message, menu->helpWindow, 0, 0);
  message++;
  Text_DrawCharacterAtOffsetFar(message, menu->helpWindow, 0, 16);
  Func_08101c7c(1, 1, 11, 3, 6);
  UiPanel_SetRectPalette(menu->listWindow, 0, 0, 28, 10, 6);
  list = Window_CreateFar(0, 9, 8, 10, 6);
  helpWindow = Window_CreateFar(8, 12, 22, 7, 2);
  statusWindow = Window_CreateFar(8, 9, 22, 3, 2);
  Func_08038290();
  helpIndex = 0;
  listMessage = (s32)MsgDjinnInfoTopic;
  do {
    /* FAKEMATCH(temporary): the topic's row held in y before the call; as
       the argument helpIndex * 8, 2 of 289 instructions differ in tla-en. */
    s32 y = helpIndex * 8;

    Text_DrawCharacterAtOffsetFar(helpIndex + listMessage, list, 0, y);
    helpIndex++;
  } while (helpIndex <= 6);
  do {
    WindowSprite_PrepareForRedrawFar(statusWindow);
    Text_DrawMessageFar(selection + (s32)MsgDjinnInfoTopic, statusWindow, 0, 0);
    channel = Func_080383d8(helpWindow, selection + (s32)MsgDjinnInfoText);
    UiPanel_SetRectHighlight(list, 0, previous, 6, 1, 15);
    UiPanel_SetRectHighlight(list, 0, selection, 6, 1, 14);
    previous = selection;
    for (;;) {
      Func_080f8a44(-12, (list->y + selection) * 8 + 8);
      wait(1);
      if (gInput.repeat & 0x90) {
        selection++;
        selection = Menu_GetModuloOfSum(selection, 7);
        Sound_PlayFar(111);
        break;
      }
      if (gInput.repeat & 0x60) {
        selection--;
        selection = Menu_GetModuloOfSum(selection, 7);
        Sound_PlayFar(111);
        break;
      } else if (gInput.pressed & 8) {
        Sound_PlayFar(113);
        result = -2;
        break;
      } else if (gInput.pressed & 6) {
        Sound_PlayFar(113);
        result = -1;
        break;
      } else if (gInput.pressed & 1) {
        if (Message_AllTypedFar()) {
          selection++;
          selection = Menu_GetModuloOfSum(selection, 7);
          Sound_PlayFar(112);
          break;
        }
        Sound_PlayFar(111);
      }
    }
    if (render->glyphResource != 99) {
      Resource_ResetEntry(render->glyphResource);
      render->glyphResource = 99;
    }
    ((struct WindowWork *)Ram_HeapSlots->windowTiles)->unk1388 = 0;
    WindowSprite_PrepareForRedrawFar(helpWindow);
    {
      struct Window *window = channel->work;

      window->frame = 0;
      window->duration = 0;
      window->state = 0;
    }
    channel->work = 0;
  } while (result == 0);
  ((struct WindowWork *)Ram_HeapSlots->windowTiles)->menuBusy = 1;
  WindowSprite_ClearListFar(statusWindow);
  WindowSprite_ClearListFar(helpWindow);
  wait(1);
  Window_CloseFar(statusWindow, 1);
  Window_CloseFar(list, 1);
  Window_CloseFar(helpWindow, 1);
  Func_08038290();
  if (result == -2) {
    WindowSprite_PrepareForRedrawFar(menu->helpWindow);
    WindowSprite_PrepareForRedrawFar(menu->listWindow);
    WindowSprite_PrepareForRedrawFar(menu->statusWindow);
    ((struct WindowWork *)Ram_HeapSlots->windowTiles)->menuBusy = 0;
  }
  Task_Add(Menu_UpdateEntryObjectTransforms, TASK_PASS_VBLANK);
  return result;
}

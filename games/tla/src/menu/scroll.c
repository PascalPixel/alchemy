#include "types.h"
#include "io_reg.h"
#include "scene.h"
#include "system.h"
#include "static_assert.h"
#include "ram_buffer.h"
#include "input.h"

#define SELECTION_FIXED 999

#define NODE_STATUS_SCROLLING 33
#define NODE_STATUS_SHOWN 1

#define NODE_KIND_YES_NO 6

#define SELECTION_ROWS 4
#define ARROW_SCROLL_FRAMES 8

struct SelectionNode {
  u8 pad0[10];
  u16 kind;
};

struct SelectionScreen {
  u8 pad0[8];
  u16 upScroll;
  u16 upArrow;
  u8 pad1[0x3c - 0xc];
  u16 downScroll;
  u16 downArrow;
  u8 pad2[0x348 - 0x40];
  struct SelectionNode *node;
  u8 pad3[0x394 - 0x34c];
  u16 count;
  u8 pad4[0x39c - 0x396];
  u16 top;
  u16 cursor;
  u16 busy;
  u16 status;
};

STATIC_ASSERT(offsetof(struct SelectionScreen, downArrow) == 0x3e);
STATIC_ASSERT(offsetof(struct SelectionScreen, node) == 0x348);
STATIC_ASSERT(offsetof(struct SelectionScreen, status) == 0x3a2);

void Sound_PlayFar(u32);
void Menu_LoadSelectionNodeResource(struct SelectionScreen *screen, u32 index);
void Menu_StepRight(struct SelectionScreen *screen);
void Menu_StepLeft(struct SelectionScreen *screen);
s32 Menu_ConfirmSelection(struct SelectionScreen *screen);
void Menu_ReloadNodeResource(struct SelectionScreen *screen, u32 index);
void Menu_ScrollSelectionList(struct SelectionScreen *screen, u32 down);
void Menu_OpenSelectionWindow(u16 type, u32 value);

s32 Menu_SelectionLoop(s32 mode) {
  struct SelectionScreen *screen = Ram_HeapSlots->menuWork;

  Menu_LoadSelectionNodeResource(screen, 0);
  for (;;) {
    WaitFrames(1);
    if (screen->busy != 0) {
      continue;
    }
    if (mode != SELECTION_FIXED) {
      if (gInput.repeat & KEY_RIGHT) {
        Menu_StepRight(screen);
      } else if (gInput.repeat & KEY_LEFT) {
        Menu_StepLeft(screen);
      } else if (gInput.pressed & KEY_A) {
        return Menu_ConfirmSelection(screen);
      }
    }
    if (mode != 0 && (gInput.pressed & KEY_B)) {
      return -1;
    }
  }
}

#define SOUND_MENU_CURSOR_MOVE 111
#define SOUND_MENU_CONFIRM 112
#define SOUND_MENU_CANCEL 113

u32 Menu_WaitForSelectionInput(u32 mode) {
  struct SelectionScreen *screen = Ram_HeapSlots->menuWork;
  u32 result;

  for (;;) {
    WaitFrames(1);
    if (screen->busy != 0) {
      continue;
    }

    if (mode != SELECTION_FIXED) {
      if (gInput.repeat & KEY_RIGHT) {
        Sound_PlayFar(SOUND_MENU_CURSOR_MOVE);
        Menu_StepRight(screen);
      } else if (gInput.repeat & KEY_LEFT) {
        Sound_PlayFar(SOUND_MENU_CURSOR_MOVE);
        Menu_StepLeft(screen);
      }

      if (gInput.pressed & KEY_A) {
        result = screen->top + screen->cursor;
        if (screen->node->kind == NODE_KIND_YES_NO) {
          if (result == 0)
            Sound_PlayFar(SOUND_MENU_CONFIRM);
          else
            Sound_PlayFar(SOUND_MENU_CANCEL);
        } else {
          Sound_PlayFar(SOUND_MENU_CONFIRM);
        }
        return result;
      }
    }

    if (mode != 0 && (gInput.pressed & KEY_B)) {
      Sound_PlayFar(SOUND_MENU_CANCEL);
      return -1;
    }
  }
}

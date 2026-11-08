#include "types.h"
#include "static_assert.h"

#define NODE_STATUS_SCROLLING 33
#define NODE_STATUS_SHOWN 1

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

void Menu_LoadSelectionNodeResource(struct SelectionScreen *screen, u32 index);
void Menu_ReloadNodeResource(struct SelectionScreen *screen, u32 index);
void Menu_ScrollSelectionList(struct SelectionScreen *screen, u32 down);
void Menu_OpenSelectionWindow(u16 type, u32 value);

void Func_0803ea54(struct SelectionScreen *screen) {
  u32 end = screen->top + screen->cursor + 1;

  if (end == screen->count) {
    return;
  }
  Menu_ReloadNodeResource(screen, screen->cursor);
  screen->status = NODE_STATUS_SCROLLING;
  wait(1);
  screen->cursor++;
  if (screen->cursor == SELECTION_ROWS && end + 1 < screen->count) {
    screen->cursor--;
    screen->downScroll = ARROW_SCROLL_FRAMES;
    screen->top++;
    Menu_ScrollSelectionList(screen, 1);
    if (screen->top + screen->cursor + 2 == screen->count) {
      screen->downArrow = 0;
    }
    screen->upArrow = 1;
  }
  screen->status = NODE_STATUS_SHOWN;
  Menu_LoadSelectionNodeResource(screen, screen->cursor);
  wait(1);
  Menu_OpenSelectionWindow(screen->node->kind, 0);
  wait(1);
}

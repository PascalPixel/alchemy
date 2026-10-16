// Near miss, differ 10 of 80 (2026-10-12): three scheduling differences, all
// loads the listing takes before the 0xffff constant it adds, and the up-arrow
// offset built after the store instead of before.
#include "types.h"
#include "io_reg.h"
#include "scene.h"
#include "system.h"
#include "layout_guard.h"

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

LAYOUT_OFFSET_GUARD(SelectionScreen_DownArrow, struct SelectionScreen,
                    downArrow, 0x3e);
LAYOUT_OFFSET_GUARD(SelectionScreen_Node, struct SelectionScreen, node, 0x348);
LAYOUT_OFFSET_GUARD(SelectionScreen_Status, struct SelectionScreen, status,
                    0x3a2);

extern struct SelectionScreen *gResQueueWork;
extern u32 gKeyState;
extern volatile u32 gKeysRepeat;
void Audio_PlayCue(u32);
void Menu_LoadSelectionNodeResource(struct SelectionScreen *screen, u32 index);
void Menu_StepRight(struct SelectionScreen *screen);
void Menu_StepLeft(struct SelectionScreen *screen);
s32 Menu_ConfirmSelection(struct SelectionScreen *screen);
void Menu_ReloadNodeResource(struct SelectionScreen *screen, u32 index);
void Menu_ScrollSelectionList(struct SelectionScreen *screen, u32 down);
void Menu_OpenSelectionWindow(u16 type, u32 value);

void Menu_MoveSelectionBackward(struct SelectionScreen *screen) {
  u32 cursor;

  /* FAKEMATCH: top and cursor are tested together as one word. */
  if (*(u32 *)&screen->top == 0) {
    return;
  }
  Menu_ReloadNodeResource(screen, screen->cursor);
  screen->status = NODE_STATUS_SCROLLING;
  WaitFrames(1);
  cursor = screen->cursor;
  if (cursor == 1 && screen->top != 0) {
    screen->upScroll = ARROW_SCROLL_FRAMES;
    screen->top--;
    Menu_ScrollSelectionList(screen, 0);
    if (screen->top == 0) {
      screen->upArrow = 0;
    }
    screen->downArrow = cursor;
  } else {
    screen->cursor--;
  }
  screen->status = NODE_STATUS_SHOWN;
  Menu_LoadSelectionNodeResource(screen, screen->cursor);
  WaitFrames(1);
  Menu_OpenSelectionWindow(screen->node->kind, 0);
  WaitFrames(1);
}

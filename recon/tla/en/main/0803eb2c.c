// Draft, differ 2 of 80 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
// Draft, differ 4 of 80 (2026-10-24): a one-pass block around the upScroll
// store and an empty asm after the status store place the upScroll offset and
// the reloads as the native has them. Left: in the else branch the native
// reads the cursor before it builds 0xffff, and it builds the cursor offset
// before the status store.
// Near miss, differ 10 of 80 (2026-10-12): three scheduling differences, all
// loads the listing takes before the 0xffff constant it adds, and the up-arrow
// offset built after the store instead of before.
#include "types.h"
#include "io_reg.h"
#include "scene.h"
#include "system.h"
#include "types.h"
#include "static_assert.h"

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
    do {
      screen->upScroll = ARROW_SCROLL_FRAMES;
    } while (0);
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
  __asm__ volatile("");
  Menu_LoadSelectionNodeResource(screen, screen->cursor);
  WaitFrames(1);
  Menu_OpenSelectionWindow(screen->node->kind, 0);
  WaitFrames(1);
}

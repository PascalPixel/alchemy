#include "TYPES.H"
#include "SCENE.H"
#include "SYSTEM.H"
#include "LAYOUT_GUARD.H"

#define KEY_A 0x0001
#define KEY_B 0x0002
#define KEY_RIGHT 0x0010
#define KEY_LEFT 0x0020

/* A selection that cannot be moved, only confirmed or cancelled. */
#define SELECTION_FIXED 999

/* The node window status while it scrolls, and once it has settled. */
#define NODE_STATUS_SCROLLING 33
#define NODE_STATUS_SHOWN 1

/* The node kind whose first entry confirms and whose others cancel. */
#define NODE_KIND_YES_NO 6

/* Rows visible in the selection list. */
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

LAYOUT_OFFSET_GUARD(SelectionScreen_DownArrow, struct SelectionScreen, downArrow, 0x3e);
LAYOUT_OFFSET_GUARD(SelectionScreen_Node, struct SelectionScreen, node, 0x348);
LAYOUT_OFFSET_GUARD(SelectionScreen_Status, struct SelectionScreen, status, 0x3a2);

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

/* Runs the selection until it is confirmed, or cancelled when mode allows. */

u32 Menu_WaitForSelectionInput(u32 mode)
{
    struct SelectionScreen *screen = gResQueueWork;
    u32 result;

    for (;;) {
        WaitFrames(1);
        if (screen->busy != 0) {
            continue;
        }

        if (mode != SELECTION_FIXED) {
            if (gKeysRepeat & KEY_RIGHT) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                Menu_StepRight(screen);
            } else if (gKeysRepeat & KEY_LEFT) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                Menu_StepLeft(screen);
            }

            if (gKeyState & KEY_A) {
                result = screen->top + screen->cursor;
                if (screen->node->kind == NODE_KIND_YES_NO) {
                    if (result == 0)
                        Audio_PlayCue(SOUND_MENU_CONFIRM);
                    else
                        Audio_PlayCue(SOUND_MENU_CANCEL);
                } else {
                    Audio_PlayCue(SOUND_MENU_CONFIRM);
                }
                return result;
            }
        }

        if (mode != 0 && (gKeyState & KEY_B)) {
            Audio_PlayCue(SOUND_MENU_CANCEL);
            return -1;
        }
    }
}

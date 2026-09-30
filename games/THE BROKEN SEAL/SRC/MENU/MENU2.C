#include "NODE_CHAIN.H"
#include "TYPES.H"
#include "SCENE.H"
#include "SOUND_IDS.H"
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
s32 Menu_ConfirmSelection(struct SelectionScreen *screen);
void Menu_OpenSelectionWindow(u16 type, u32 value);

struct StepNode {
    u8 unk_00[4];
    struct StepNode *next;
    u8 unk_08[2];
    u16 id;
    u8 unk_0c[4];
    s16 y;
    u8 unk_12[2];
    u16 speed;
    u8 unk_16[2];
    s16 target_y;
    u16 target_z;
};

struct StepMenu {
    u8 unk_000[8];
    u16 scroll_up;
    u16 more_above;
    u8 unk_00c[0x30];
    u16 scroll_down;
    u16 more_below;
    u8 unk_040[0x308];
    struct StepNode *nodes;
    u8 unk_34c[8];
    u16 entry_ids[16];
    u16 entry_kinds[16];
    u16 count;
    u16 base_y; /* read through MENU_BASE_Y */
    u16 base_z; /* read through MENU_BASE_Z */
    u8 unk_39a[2];
    u16 top;
    u16 cursor;
    u8 unk_3a0[2];
    u16 status;
};

/* FAKEMATCH: the row targets are read as plain halfwords, not struct
   members, so GCC reloads them after every node store as the reference
   does. */
#define MENU_BASE_Y(state) (*(u16 *)((u8 *)(state) + 0x396))
#define MENU_BASE_Z(state) (*(u16 *)((u8 *)(state) + 0x398))
void WaitFrames(s32 frames);
void Menu_ScrollSelectionList(struct StepMenu *state, u32 mode);
void MenuSelection_SetupEntry(u32 id, u32 kind, struct StepNode *node, u32 flag);

struct MenuResourceNode {
    u8 unknown0[4];
    struct MenuResourceNode *next;
    u8 unknown8[2];
    u16 type;
    u16 value;
    u8 unknown14[18];
    u16 base;
};

struct MenuResourceList {
    u8 unknown0[0x348];
    struct MenuResourceNode *head;
};

extern u8 MsgCommandName;
void Ui_BuildPairedPatternsToSlot(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
void Menu_LoadSelectedResource(void);
void BattlePres_SetActorModesFar(u16 *, s32);
void Menu_ReloadNodeResource(struct MenuResourceList *state, u32 index);
void Menu_LoadSelectionNodeResource(struct MenuResourceList *state, u32 index);
void Menu_StepRight(struct StepMenu *state);
void Menu_StepLeft(struct StepMenu *state);

struct NodeChainNode *NodeChain_GetNodeAtCount(struct NodeChainState *state)
{
    struct NodeChainNode *node = state->node;
    s32 index;

    for (index = 0; index != state->count; ++index) {
        node = node->next;
    }
    return node;
}

/* Runs the selection until it is confirmed, or cancelled when mode allows. */
s32 Menu_SelectionLoop(s32 mode)
{
    struct SelectionScreen *screen = gResQueueWork;

    Menu_LoadSelectionNodeResource(screen, 0);
    for (;;) {
        WaitFrames(1);
        if (screen->busy != 0) {
            continue;
        }
        if (mode != SELECTION_FIXED) {
            if (gKeysRepeat & KEY_RIGHT) {
                Menu_StepRight(screen);
            } else if (gKeysRepeat & KEY_LEFT) {
                Menu_StepLeft(screen);
            } else if (gKeyState & KEY_A) {
                return Menu_ConfirmSelection(screen);
            }
        }
        if (mode != 0 && (gKeyState & KEY_B)) {
            return -1;
        }
    }
}

/* As Menu_SelectionLoop, with sound cues, returning the chosen row. */
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

/* Moves the cursor down one row, scrolling the list at the bottom row. */
void Menu_MoveSelectionForward(struct SelectionScreen *screen)
{
    u32 end = screen->top + screen->cursor + 1;

    if (end == screen->count) {
        return;
    }
    Menu_ReloadNodeResource(screen, screen->cursor);
    screen->status = NODE_STATUS_SCROLLING;
    WaitFrames(1);
    screen->cursor++;
    if (screen->cursor == SELECTION_ROWS && end + 1 < screen->count) {
        screen->cursor--;
        screen->downScroll = ARROW_SCROLL_FRAMES;
        screen->top++;
        Menu_ScrollSelectionList(screen, TRUE);
        if (screen->top + screen->cursor + 2 == screen->count) {
            screen->downArrow = FALSE;
        }
        screen->upArrow = TRUE;
    }
    screen->status = NODE_STATUS_SHOWN;
    Menu_LoadSelectionNodeResource(screen, screen->cursor);
    WaitFrames(1);
    Menu_OpenSelectionWindow(screen->node->kind, 0);
    WaitFrames(1);
}

/* Moves the cursor up one row, scrolling the list at the top row. */
void Menu_MoveSelectionBackward(struct SelectionScreen *screen)
{
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
        Menu_ScrollSelectionList(screen, FALSE);
        if (screen->top == 0) {
            screen->upArrow = FALSE;
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

/* Step the selection cursor right; at the end of a long list, slide every
   entry back to the top and restart, otherwise scroll one row when the
   cursor reaches the fourth slot. */
void Menu_StepRight(struct StepMenu *state)
{
    struct StepNode *node;
    u16 *ids;
    s32 y;
    u32 end;

    Menu_ReloadNodeResource(state, state->cursor);
    state->status = 33;
    WaitFrames(1);
    state->cursor++;
    if (state->count > 5) {
        end = state->top + state->cursor;
        if (end == state->count) {
            node = state->nodes;
            state->more_above = 0;
            while (node->next != NULL) {
                node->target_y = MENU_BASE_Y(state);
                node->target_z = MENU_BASE_Z(state);
                node->speed = 0xfff4;
                node = node->next;
            }
            node->target_y = MENU_BASE_Y(state);
            node->target_z = MENU_BASE_Z(state);
            node->speed = 0xfff4;
            while (node->target_y != node->y)
                WaitFrames(1);
            node = state->nodes;
            if (node != NULL) {
                ids = state->entry_ids;
                do {
                    MenuSelection_SetupEntry(ids[0], ids[16], node, 1);
                    node = node->next;
                    ids++;
                } while (node != NULL);
            }
            state->cursor = 0;
            state->top = 0;
            node = state->nodes;
            y = node->y + 16;
            node = node->next;
            while (node != NULL) {
                node->target_y = y;
                node->speed = 12;
                node = node->next;
                y += 16;
            }
            state->more_below = 1;
        } else if (state->cursor == 4 && end + 1 < state->count) {
            state->cursor--;
            state->scroll_down = 8;
            state->top++;
            Menu_ScrollSelectionList(state, 1);
            if (state->top + state->cursor + 2 == state->count)
                state->more_below = 0;
            state->more_above = 1;
        }
    } else if (state->cursor == state->count) {
        state->cursor = 0;
    }
    state->status = 1;
    Menu_LoadSelectionNodeResource(state, state->cursor);
    WaitFrames(1);
    Menu_OpenSelectionWindow(state->nodes->id, 0);
    WaitFrames(1);
}

/* Selection lists: move the cursor up one entry, scrolling or wrapping the list to its end. */

/* main:0801b810 Menu_StepLeft - hand-written draft, 174 of 204 halfwords
   differ, almost all from one register choice: the ROM keeps the top row
   in r1 and ORs it with the cursor into a scratch r3, then stores 0 from a
   fresh r0; this C ORs into r1 and reuses that known zero, which shifts
   every later instruction by four bytes and swaps r1/r2 in the row loops.
   Baseline is 404 bytes / 34 aligned halfword edits (2026-09-26).
   Explicit u32 cursor/top snapshots give 400 bytes / 40 edits, dropping
   another required input copy. An inline OR-result boundary gives 404
   bytes / 42 edits, reverses the input loads and moves the cursor pointer
   to r5. Neither recovers the r1-to-r3 copy or the independent wrap zero;
   keep the original control flow until their source ancestry is known.
   Writing the test as two != 0 tests merges them into one word load,
   which the ROM does not do. Otherwise the code is the ROM's, including
   the count - 5 loop.
   2026-09-29 (alchemy permute scorer): the draft scored 280 (16
   register-only, 2 deleted). Storing the wrap's zero through the count
   variable (more_below = i = 0) scores 65, 13 register-only rows: the OR
   test, its r1-to-r3 copy and the fresh wrap zero now match. Remaining:
   the zero takes r1 (i's register) where the ROM uses r0, and in both row
   loops y and the hoisted 12/0xfff4 take r2/r1 where the ROM has r1/r2;
   plus one commutative add order (state + offset). A separate zero
   variable is constant-propagated back to the old code. Two 400-second
   searches (256,000 candidates) found nothing below 65.

   The mirror of Menu_StepRight: step the cursor left, wrap a long list to
   its last page, or scroll one row up at the first slot. */
void Menu_StepLeft(struct StepMenu *state)
{
    struct StepNode *node;
    u16 *ids;
    register s32 y asm("r1"); /* FAKEMATCH: keeps the row offset in r1 */
    s32 i;

    Menu_ReloadNodeResource(state, state->cursor);
    state->status = 33;
    WaitFrames(1);
    if (state->count > 5) {
        if ((state->cursor | state->top) != 0) {
            if (state->cursor == 1 && state->top != 0) {
                state->scroll_up = 8;
                state->top--;
                Menu_ScrollSelectionList(state, 0);
                if (state->top == 0)
                    state->more_above = 0;
                state->more_below = 1;
            } else {
                state->cursor--;
            }
        } else {
            node = state->nodes;
            y = 64;
            { register s32 z asm("r0") = 0; /* FAKEMATCH: the zero goes through r0 */
            state->more_below = i = z; }
            while (node->next != NULL) {
                node->target_y = node->y + y;
                node->speed = 12;
                node = node->next;
                y -= 16;
            }
            node = state->nodes;
            while (node->y != node->target_y)
                WaitFrames(1);
            i = 0;
            while (i != state->count - 5)
                i++;
            node = state->nodes;
            state->top = i;
            state->cursor = 4;
            if (node != NULL) {
                /* FAKEMATCH: the index is scaled and added to the base before the field offset */
                ids = (u16 *)((i * 2 + (s32)state) + (s32)((struct StepMenu *)0)->entry_ids);
                do {
                    MenuSelection_SetupEntry(ids[0], ids[16], node, 1);
                    node = node->next;
                    ids++;
                } while (node != NULL);
            }
            node = state->nodes;
            y = state->base_y;
            while (node->next != NULL) {
                node->target_y = y;
                node->speed = 0xfff4;
                node = node->next;
                y += 16;
            }
            state->more_above = 1;
        }
    } else if (state->cursor != 0) {
        state->cursor--;
    } else {
        state->cursor = state->count - 1;
    }
    state->status = 1;
    Menu_LoadSelectionNodeResource(state, state->cursor);
    WaitFrames(1);
    Menu_OpenSelectionWindow(state->nodes->id, 0);
    WaitFrames(1);
}

void Menu_ReloadNodeResource(struct MenuResourceList *state, u32 index)
{
    struct MenuResourceNode *node = state->head;
    u32 output;
    u32 value;

    while (index != 0) {
        index--;
        node = node->next;
    }
    if (node->type == 1 || node->type == 6) {
        u32 first = node->base - (u32)&MsgCommandName;

        value = node->value;
        Ui_BuildPairedPatternsToSlot(first, 0, &value, &output, 1);
    }
}

void Menu_LoadSelectionNodeResource(struct MenuResourceList *state, u32 index)
{
    struct MenuResourceNode *node = state->head;
    u32 res;
    u32 value;

    while (index != 0) {
        index--;
        node = node->next;
    }
    if (node->type == 1 || node->type == 6) {
        u32 id = node->base - (u32)&MsgCommandName;

        value = node->value;
        Ui_BuildPairedPatternsToSlot(id, 0, &value, &res, 1);
        Menu_LoadSelectedResource();
    }
}

void Menu_SendNodeCountList(u8 *arg0)
{
    u16 data[6];
    u8 *node = *(u8 **)(arg0 + 0x348);
    s32 count = 0;

    while (node != 0) {
        node = *(u8 **)(node + 4);
        count++;
    }
    data[count] = 0xff;
    BattlePres_SetActorModesFar(data, 0);
}

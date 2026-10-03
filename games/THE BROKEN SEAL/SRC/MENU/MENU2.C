#include "SELECT.H"
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

extern struct SelectionScreen *gResQueueWork;
extern u32 gKeyState;
extern volatile u32 gKeysRepeat;
void Audio_PlayCue(u32);
u32 Menu_ConfirmSelection(struct SelectionScreen *screen);
void Menu_OpenSelectionWindow(u16 type, u32 value);

#define MENU_BASE_X(state) (*(u16 *)((u8 *)(state) + 0x396))
#define MENU_BASE_Y(state) (*(u16 *)((u8 *)(state) + 0x398))
void WaitFrames(s32 frames);
void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 forward);
void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node, s32 reuse);

extern u8 MsgCommandName;
void Ui_BuildPairedPatternsToSlot(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
void Menu_LoadSelectedResource(void);
void BattlePres_SetActorModesFar(u16 *, s32);
void Menu_ReloadNodeResource(struct SelectionScreen *state, u32 index);
void Menu_LoadSelectionNodeResource(struct SelectionScreen *state, u32 index);
void Menu_StepRight(struct SelectionScreen *state);
void Menu_StepLeft(struct SelectionScreen *state);

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
    /* FAKEMATCH: the volatile repeat-key cell preserves the separate RAM
     * loads in the right and left tests. */
    struct SelectionScreen *screen = gResQueueWork;

    Menu_LoadSelectionNodeResource(screen, 0);
    for (;;) {
        WaitFrames(1);
        if (screen->vertical_scroll != 0) {
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
    /* FAKEMATCH: the volatile repeat-key cell preserves the separate RAM
     * loads in the right and left tests. */
    struct SelectionScreen *screen = gResQueueWork;
    u32 result;

    for (;;) {
        WaitFrames(1);
        if (screen->vertical_scroll != 0) {
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
                result = screen->top + screen->cursor_index;
                if (screen->head->kind == NODE_KIND_YES_NO) {
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
    u32 end = screen->top + screen->cursor_index + 1;

    if (end == screen->count) {
        return;
    }
    Menu_ReloadNodeResource(screen, screen->cursor_index);
    screen->status = NODE_STATUS_SCROLLING;
    WaitFrames(1);
    screen->cursor_index++;
    if (screen->cursor_index == SELECTION_ROWS && end + 1 < screen->count) {
        screen->cursor_index--;
        screen->records[1].base = ARROW_SCROLL_FRAMES;
        screen->top++;
        Menu_ScrollSelectionList(screen, TRUE);
        if (screen->top + screen->cursor_index + 2 == screen->count) {
            screen->records[1].kind = FALSE;
        }
        screen->records[0].kind = TRUE;
    }
    screen->status = NODE_STATUS_SHOWN;
    Menu_LoadSelectionNodeResource(screen, screen->cursor_index);
    WaitFrames(1);
    Menu_OpenSelectionWindow(screen->head->kind, 0);
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
    Menu_ReloadNodeResource(screen, screen->cursor_index);
    screen->status = NODE_STATUS_SCROLLING;
    WaitFrames(1);
    cursor = screen->cursor_index;
    if (cursor == 1 && screen->top != 0) {
        screen->records[0].base = ARROW_SCROLL_FRAMES;
        screen->top--;
        Menu_ScrollSelectionList(screen, FALSE);
        if (screen->top == 0) {
            screen->records[0].kind = FALSE;
        }
        screen->records[1].kind = cursor;
    } else {
        screen->cursor_index--;
    }
    screen->status = NODE_STATUS_SHOWN;
    Menu_LoadSelectionNodeResource(screen, screen->cursor_index);
    WaitFrames(1);
    Menu_OpenSelectionWindow(screen->head->kind, 0);
    WaitFrames(1);
}

/* Step the selection cursor right; at the end of a long list, slide every
   entry back to the top and restart, otherwise scroll one row when the
   cursor reaches the fourth slot. */
void Menu_StepRight(struct SelectionScreen *state)
{
    /* FAKEMATCH: the row targets are read as plain halfwords, not struct
       members, so GCC reloads them after every node store as the reference
       does. */
    struct SelectionNode *node;
    u16 *ids;
    s32 y;
    u32 end;

    Menu_ReloadNodeResource(state, state->cursor_index);
    state->status = 33;
    WaitFrames(1);
    state->cursor_index++;
    if (state->count > 5) {
        end = state->top + state->cursor_index;
        if (end == state->count) {
            node = state->head;
            state->records[0].kind = 0;
            while (node->next != NULL) {
                node->target_x = MENU_BASE_X(state);
                node->target_y = MENU_BASE_Y(state);
                node->dx = 0xfff4;
                node = node->next;
            }
            node->target_x = MENU_BASE_X(state);
            node->target_y = MENU_BASE_Y(state);
            node->dx = 0xfff4;
            while (node->target_x != node->x)
                WaitFrames(1);
            node = state->head;
            if (node != NULL) {
                ids = state->kinds;
                do {
                    MenuSelection_SetupEntry(ids[0], ids[16], node, 1);
                    node = node->next;
                    ids++;
                } while (node != NULL);
            }
            state->cursor_index = 0;
            state->top = 0;
            node = state->head;
            y = node->x + 16;
            node = node->next;
            while (node != NULL) {
                node->target_x = y;
                node->dx = 12;
                node = node->next;
                y += 16;
            }
            state->records[1].kind = 1;
        } else if (state->cursor_index == 4 && end + 1 < state->count) {
            state->cursor_index--;
            state->records[1].base = 8;
            state->top++;
            Menu_ScrollSelectionList(state, 1);
            if (state->top + state->cursor_index + 2 == state->count)
                state->records[1].kind = 0;
            state->records[0].kind = 1;
        }
    } else if (state->cursor_index == state->count) {
        state->cursor_index = 0;
    }
    state->status = 1;
    Menu_LoadSelectionNodeResource(state, state->cursor_index);
    WaitFrames(1);
    Menu_OpenSelectionWindow(state->head->kind, 0);
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
void Menu_StepLeft(struct SelectionScreen *state)
{
    struct SelectionNode *node;
    u16 *ids;
    register s32 y asm("r1"); /* FAKEMATCH: keeps the row offset in r1 */
    s32 i;

    Menu_ReloadNodeResource(state, state->cursor_index);
    state->status = 33;
    WaitFrames(1);
    if (state->count > 5) {
        if ((state->cursor_index | state->top) != 0) {
            if (state->cursor_index == 1 && state->top != 0) {
                state->records[0].base = 8;
                state->top--;
                Menu_ScrollSelectionList(state, 0);
                if (state->top == 0)
                    state->records[0].kind = 0;
                state->records[1].kind = 1;
            } else {
                state->cursor_index--;
            }
        } else {
            node = state->head;
            y = 64;
            { register s32 z asm("r0") = 0; /* FAKEMATCH: the zero goes through r0 */
            state->records[1].kind = i = z; }
            while (node->next != NULL) {
                node->target_x = node->x + y;
                node->dx = 12;
                node = node->next;
                y -= 16;
            }
            node = state->head;
            while (node->x != node->target_x)
                WaitFrames(1);
            i = 0;
            while (i != state->count - 5)
                i++;
            node = state->head;
            state->top = i;
            state->cursor_index = 4;
            if (node != NULL) {
                /* FAKEMATCH: the index is scaled and added to the base before the field offset */
                ids = (u16 *)((i * 2 + (s32)state) + (s32)((struct SelectionScreen *)0)->kinds);
                do {
                    MenuSelection_SetupEntry(ids[0], ids[16], node, 1);
                    node = node->next;
                    ids++;
                } while (node != NULL);
            }
            node = state->head;
            y = state->base_x;
            while (node->next != NULL) {
                node->target_x = y;
                node->dx = 0xfff4;
                node = node->next;
                y += 16;
            }
            state->records[0].kind = 1;
        }
    } else if (state->cursor_index != 0) {
        state->cursor_index--;
    } else {
        state->cursor_index = state->count - 1;
    }
    state->status = 1;
    Menu_LoadSelectionNodeResource(state, state->cursor_index);
    WaitFrames(1);
    Menu_OpenSelectionWindow(state->head->kind, 0);
    WaitFrames(1);
}

void Menu_ReloadNodeResource(struct SelectionScreen *state, u32 index)
{
    struct SelectionNode *node = state->head;
    u32 output;
    u32 value;

    while (index != 0) {
        index--;
        node = node->next;
    }
    if (node->kind == 1 || node->kind == 6) {
        u32 first = (u16)node->message - (u32)&MsgCommandName;

        value = node->slot;
        Ui_BuildPairedPatternsToSlot(first, 0, &value, &output, 1);
    }
}

void Menu_LoadSelectionNodeResource(struct SelectionScreen *state, u32 index)
{
    struct SelectionNode *node = state->head;
    u32 res;
    u32 value;

    while (index != 0) {
        index--;
        node = node->next;
    }
    if (node->kind == 1 || node->kind == 6) {
        u32 id = (u16)node->message - (u32)&MsgCommandName;

        value = node->slot;
        Ui_BuildPairedPatternsToSlot(id, 0, &value, &res, 1);
        Menu_LoadSelectedResource();
    }
}

void Menu_SendNodeCountList(struct SelectionScreen *screen)
{
    u16 data[6];
    struct SelectionNode *node = screen->head;
    s32 count = 0;

    while (node != 0) {
        node = node->next;
        count++;
    }
    data[count] = 0xff;
    BattlePres_SetActorModesFar(data, 0);
}

struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind);
void Resource_ResetEntry(s32 id);

/* Scrolls the visible list one row. A new entry grows in at the far end
   while every entry slides 16 lines, two a frame, and the entry that
   leaves shrinks away; then the entry that left is released. */
void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 forward)
{
    struct SelectionNode *p;
    struct SelectionNode *last;
    struct SelectionNode *first;
    s32 base;
    s32 kind;
    u32 index;
    s32 y;
    s32 z;

    if (forward != 0) {
        index = screen->top + SELECTION_ROWS;
        base = screen->bases[index];
        kind = screen->kinds[index];
        p = Resource_FindFreeTransferEntry(0);
        if (p == NULL)
            return;
        MenuSelection_SetupEntry(kind, base, p, 0);
        y = screen->base_x;
        p->x = y + 80;
        z = screen->base_y;
        p->target_x = y + 64;
        p->y = z;
        p->target_y = z;
        p->scale_step = 32;
        p->scale = 32;
        p->scale_end = 256;
        p->dx = -2;
        last = p;
        p = screen->head;
        p->scale_step = -32;
        p->scale_end = 0;
        for (;;) {
            p->target_x = p->x - 16;
            p->dx = -2;
            if (p->next == NULL)
                break;
            p = p->next;
        }
        p->next = last;
        last->next = NULL;
        last->prev = p;
        p = screen->head;
        do {
            WaitFrames(1);
        } while (p->scale != 0);
        screen->head = p->next;
        Resource_ResetEntry(p->slot);
        p->kind = 0;
        p = p->next;
        p->prev = NULL;
    } else {
        index = screen->top;
        base = screen->bases[index];
        kind = screen->kinds[index];
        p = Resource_FindFreeTransferEntry(0);
        if (p == NULL)
            return;
        MenuSelection_SetupEntry(kind, base, p, 0);
        p->x = screen->base_x - 16;
        p->y = screen->base_y;
        p->target_y = p->y;
        p->dx = 2;
        p->scale = 32;
        p->scale_step = 32;
        p->target_x = p->x + 16;
        p->scale_end = 256;
        first = p;
        p = screen->head;
        p->prev = first;
        first->next = p;
        first->prev = NULL;
        screen->head = first;
        p = first;
        for (;;) {
            p->target_x = p->x + 16;
            p->dx = 2;
            if (p->next == NULL)
                break;
            p = p->next;
        }
        p->scale_end = 0;
        p->scale_step = -32;
        p = screen->head;
        do {
            WaitFrames(1);
        } while (p->scale != 256);
        while (p->next != NULL)
            p = p->next;
        Resource_ResetEntry(p->slot);
        p->kind = 0;
        p->prev->next = NULL;
    }
}

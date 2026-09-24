#include "TYPES.H"

/* main:0801b810 Menu_StepLeft - hand-written draft, 174 of 204 halfwords
   differ, almost all from one register choice: the ROM keeps the top row
   in r1 and ORs it with the cursor into a scratch r3, then stores 0 from a
   fresh r0; this C ORs into r1 and reuses that known zero, which shifts
   every later instruction by four bytes and swaps r1/r2 in the row loops.
   Writing the test as two != 0 tests merges them into one word load,
   which the ROM does not do. Otherwise the code is the ROM's, including
   the count - 5 loop.

   The mirror of Menu_StepRight: step the cursor left, wrap a long list to
   its last page, or scroll one row up at the first slot. */

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
    u16 base_y;
    u16 base_z;
    u8 unk_39a[2];
    u16 top;
    u16 cursor;
    u8 unk_3a0[2];
    u16 status;
};

void WaitFrames(s32 frames);
void Menu_ReloadNodeResource(struct StepMenu *state, u32 index);
void Menu_LoadSelectionNodeResource(struct StepMenu *state, u32 index);
void Menu_ScrollSelectionList(struct StepMenu *state, u32 mode);
void MenuSelection_SetupEntry(u32 id, u32 kind, struct StepNode *node, u32 flag);
void Menu_OpenSelectionWindow(u16 type, u32 value);

void Menu_StepLeft(struct StepMenu *state)
{
    struct StepNode *node;
    u16 *ids;
    s32 y;
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
            state->more_below = 0;
            y = 64;
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
                ids = &state->entry_ids[i];
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

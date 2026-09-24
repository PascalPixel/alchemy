#include "TYPES.H"

/* Step the selection cursor right; at the end of a long list, slide every
   entry back to the top and restart, otherwise scroll one row when the
   cursor reaches the fourth slot. */

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

/* The row targets are read as plain halfwords, not struct members: GCC then
   reloads them after every node store, as the reference does. */
#define MENU_BASE_Y(state) (*(u16 *)((u8 *)(state) + 0x396))
#define MENU_BASE_Z(state) (*(u16 *)((u8 *)(state) + 0x398))

void WaitFrames(s32 frames);
void Menu_ReloadNodeResource(struct StepMenu *state, u32 index);
void Menu_LoadSelectionNodeResource(struct StepMenu *state, u32 index);
void Menu_ScrollSelectionList(struct StepMenu *state, u32 mode);
void MenuSelection_SetupEntry(u32 id, u32 kind, struct StepNode *node, u32 flag);
void Menu_OpenSelectionWindow(u16 type, u32 value);

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

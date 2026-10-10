/* Pin-free Menu_StepLeft trial: all six editions 184 native/built,
 * 182 common, differ 4.
 * A literal zero merges with the outer condition and differs 28 in 182
 * built instructions. The value-preserving u16 shift prevents that merge
 * and restores the native extent, but uses r3 instead of r0 for zero and
 * places its move after the head load. u8/u16/u32 zero locals, register
 * qualification and top/count as the zero source all retain 4. Moving the
 * zero declaration before the head load differs 18; adding it to i differs
 * 19; adding it to the node pointer differs 12 or 17. A two-minute seed-389
 * search compiled 2752 candidates without a zero. Its lower weighted score
 * differs 19, so this faithful differ-4 trial is retained. The linked
 * binding remains until all six editions match. */
/* Sharing the zero assignment with i and resetting i before the walk
 * retains four differences. A zero from cursor_index | top leaves 28;
 * OR-ing the known zero into node leaves 12. Replacing the known-zero
 * shift with a literal zero and generic output plus head-address input
 * leaves 46 (182 built); a kind-address input leaves 58 (184 built). */
#define OWN_AudioCommand_PlayFar
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "select.h"
#include "node_chain.h"
#include "types.h"
#include "io_reg.h"
#include "scene.h"
#include "sound_ids.h"
#include "system.h"
#include "layout_guard.h"
#include "text/msg_ids.h"

#define gKeyState (*(u32 *)&gKeyState)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)

#define Menu_OpenSelectionWindow ((void (*)(u16,u32))Menu_OpenSelectionWindow)

#define SELECTION_FIXED 999

#define NODE_STATUS_SCROLLING 33
#define NODE_STATUS_SHOWN 1

#define NODE_KIND_YES_NO 6

#define SELECTION_ROWS 4
#define ARROW_SCROLL_FRAMES 8

void AudioCommand_PlayFar(u32);

void Menu_StepLeft(struct SelectionScreen *state)
{
    struct SelectionNode *node;
    u16 *ids;
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
            i = 64;
            { /* FAKEMATCH: the known-zero shift prevents the literal-zero/condition merge, restoring 184 instructions; zero still takes r3 instead of native r0. */
            s32 z = (u32)(u16)state->cursor_index >> 16;
            state->records[1].kind = z; }
            while (node->next != NULL) {
                node->target_x = node->x + i;
                node->dx = 12;
                node = node->next;
                i -= 16;
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
            i = state->base_x;
            while (node->next != NULL) {
                node->target_x = i;
                node->dx = 0xfff4;
                node = node->next;
                i += 16;
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

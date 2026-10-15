/* Draft, differ 58 of 641: The Broken Seal's MenuSelection_DrawFrame with this game's
 * three-word projected position (frame 24). Every remaining row is instruction order under
 * the ARM9TDMI scheduling: the halves of split constants and the loads beside them come in
 * another order (first at cnt = 0 against the cursor entry's address, then the bitfield
 * masks -4 and -63 in each loop). Assigning cursor and transfer after the head load, and an
 * empty one-pass block after cnt = 0, measured 66 and 62. */
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "select.h"

struct EffectPosition
{
	s32 x;
	s32 y;
	s32 z;
};

extern u32 gFrameTick;
extern s8  Menu_CursorBobOffsets[];
extern u8  Data_0805c9c4[];

s32	 BattleMotion_ProjectScaledPositionFar(s32 id, struct EffectPosition *projected);
s32	 GameFlag_Test(s32 flag);
void Runtime_PushSlotEntry(s32 *entry, s32 slot);
struct SelectionNode *NodeChain_GetNodeAtCount(struct SelectionScreen *screen);
s32	 AffineMatrix_BuildForEffect(void *matrix);
s32	 VramBlock_LoadCached(s32 slot, s32 size, void *source);
void Func_0803e488(struct SelectionScreen *state, s32 index);

void MenuSelection_DrawFrame(void)
{
    struct SelectionScreen *state = Ram_HeapSlots->menuWork;
    struct SelectionSprite *cursor_entry = &state->records[14].sprite;
    struct SelectionNode *cursor = &state->records[14];
    struct SelectionNode *transfer = &state->records[15];
    struct SelectionNode *node;
    struct SelectionSprite *e;
    struct SelectionSprite *te;
    s32 cnt = 0;
    s32 slot;
    s32 tmp;
    struct EffectPosition pos;

    node = state->head;
    while (node != 0) {
        e = &node->sprite;
        e->affine = 0;
        e->param = 0;
        e->x = node->x;
        e->y = node->y;
        slot = 240;
        if (state->verticalScroll != 0) {
            e->y += state->verticalScroll;
        } else if (node->x != node->targetX) {
            if (node->dx > 0) {
                if (node->x + node->dx > node->targetX) {
                    node->x = node->targetX;
                } else {
                    node->x = node->x + node->dx;
                }
            } else {
                if (node->x + node->dx < node->targetX) {
                    node->x = node->targetX;
                } else {
                    node->x = node->x + node->dx;
                }
            }
            e->x = node->x;
        } else if (cnt == state->cursorIndex) {
            slot = 241;
            if (cursor->kind != 0) {
                if (BattleMotion_ProjectScaledPositionFar(node->base, &pos) != -1) {
                    cursor->targetX = pos.x;
                    cursor->targetY = pos.y;
                    if (cursor->scale == 0) {
                        cursor->x = pos.x;
                        cursor->y = pos.y;
                        cursor->scale = 1;
                    }
                }
            }
        }
        if (node->scale != 0) {
            if (GameFlag_Test(0x103) != 0) {
                if (state->records[14].kind == 1) {
                    e->mode = 1;
                } else {
                    e->mode = 0;
                }
                if (node->kind == 1) {
                    e->mode = 1;
                }
            }
            Runtime_PushSlotEntry((s32 *)e, slot);
        }
        node = node->next;
        cnt++;
    }
    if (transfer->kind != 0) {
        node = NodeChain_GetNodeAtCount(state);
        te = &transfer->sprite;
        te->mode = 0;
        te->affine = 0;
        te->param = 0;
        te->mosaic = 0;
        te->colors = 1;
        te->size = 2;
        te->shape = 0;
        te->priority = 0;
        te->tile = transfer->tile;
        te->x = node->x - 4;
        te->y = node->y + (Menu_CursorBobOffsets[(gFrameTick >> 1) & 15] >> 1) - 4;
        if (transfer->scale != transfer->scaleEnd) {
            state->matrix[0] = transfer->scale;
            state->matrix[1] = transfer->scale;
            state->matrix[2] = 0;
            te->param = AffineMatrix_BuildForEffect(state->matrix);
            te->affine = 3;
            te->x = te->x + 0xfff0;
            te->y = te->y + 0xf0;
            transfer->scale = transfer->scale + transfer->scaleStep;
        }
        if (GameFlag_Test(0x103) != 0) {
            te->mode = 1;
        }
        Runtime_PushSlotEntry((s32 *)te, 248);
    }
    Func_0803e488(state, 0);
    Func_0803e488(state, 1);
    node = state->path;
    while (node != 0) {
        e = &node->sprite;
        if (node->x != node->targetX) {
            node->x = node->x + node->dx;
        }
        if (node->y != node->targetY) {
            node->y = node->y + node->dy;
        }
        e->x = node->x;
        e->y = node->y;
        if (node->scale != node->scaleEnd) {
            node->scale = node->scale + node->scaleStep;
            state->matrix[0] = node->scale;
            state->matrix[1] = node->scale;
            state->matrix[2] = 0;
            e->param = AffineMatrix_BuildForEffect(state->matrix);
            e->affine = 3;
            e->x = e->x + 0xfff8;
            e->y = e->y + 0xf8;
        } else {
            e->affine = 0;
            e->param = 0;
        }
        if (GameFlag_Test(0x103) != 0) {
            if (state->records[14].kind == 1) {
                e->mode = 1;
            } else {
                e->mode = 0;
            }
            if (node->kind == 1) {
                e->mode = 1;
            }
        }
        Runtime_PushSlotEntry((s32 *)e, 240);
        node = node->next;
    }
    if (cursor->kind != 0) {
        cnt = (s32)Data_0805c9c4;
        {
            void *src = (void *)(cnt + (((gFrameTick >> 2) & 15) << 8));

            cursor_entry->tile = VramBlock_LoadCached(cursor->slot, 0x100, src);
        }
        if (cursor->targetX != cursor->x) {
            tmp = (cursor->targetX - cursor->x) >> 1;
            if (tmp != 0) {
                cursor->x = cursor->x + tmp;
            } else {
                cursor->x = cursor->targetX;
            }
        }
        if (cursor->targetY != cursor->y) {
            if ((tmp = (cursor->targetY - cursor->y) >> 1) != 0) {
                cursor->y = cursor->y + tmp;
            } else {
                cursor->y = cursor->targetY;
            }
        }
        cursor_entry->y = cursor->y + Menu_CursorBobOffsets[(gFrameTick >> 2) & 15] - 32;
        cursor_entry->x = cursor->x - 4;
        if (GameFlag_Test(0x103) != 0) {
            if (state->records[14].kind == 1) {
                cursor_entry->mode = 1;
            } else {
                cursor_entry->mode = 0;
            }
        }
        Runtime_PushSlotEntry((s32 *)cursor_entry, 248);
    }
    state->status += 1;
}

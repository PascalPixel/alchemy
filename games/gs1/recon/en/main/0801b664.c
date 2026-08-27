#include "types.h"

struct ListNode {
    s32 unknown00;
    struct ListNode *next;
    s32 unknown08;
    s16 unknown0c;
    s16 unknown0e;
    s16 base;
    s16 unknown12;
    s16 status;
    s16 unknown16;
    s16 y;
    s16 x;
};

void Func_0801b9a8(u8 *state, u32 index);
void Func_0801b9ec(u8 *state, u32 index);
void Func_0801ba68(void *state, u32 mode);
void Func_0801b010(u16 type, u32 value);
void WaitFrames(u32 value);
void Func_0801bd98(u16 a, u16 b, struct ListNode *node, u32 flag);

void Func_0801b664(u8 *state)
{
    u16 *index = (u16 *)(state + 0x39e);
    u16 *count = (u16 *)(state + 0x394);
    u16 *selection = (u16 *)(state + 0x39c);
    struct ListNode **list = (struct ListNode **)(state + 0x348);
    u16 slot;

    Func_0801b9a8(state, *index);
    *(u16 *)(state + 0x3a2) = 33;
    WaitFrames(1);

    *index += 1;
    slot = *(u16 *)(state + 916);

    if (slot > 5) {
        u16 selval = *(u16 *)(state + 924);
        u16 total = selval + *index;
        if (total == slot) {
            struct ListNode *node = *list;
            *(u16 *)(state + 10) = 0;
            if (node->next != 0) {
                u16 base = *count;
                u16 y = *selection;
                do {
                    node->base = base;
                    node->status = y;
                    node->y = base;
                    node = node->next;
                } while (node->next != 0);
            }
            node->base = *count;
            node->y = *selection;
            node->status = 16;

            while (node->x != node->status) {
                WaitFrames(1);
            }

            {
                struct ListNode *n = *list;
                if (n != 0) {
                    u16 *w = (u16 *)(state + 852);
                    do {
                        Func_0801bd98(*w, w[16], n, 1);
                        n = n->next;
                        w++;
                    } while (n != 0);
                }
            }

            *count = 0;
            *selection = 0;

            {
                struct ListNode *n = *list;
                s16 basey = n->x;
                n = n->next;
                basey += 16;
                if (n != 0) {
                    do {
                        n->base = basey;
                        n->status = 12;
                        n = n->next;
                        basey += 16;
                    } while (n != 0);
                }
            }

            *(u16 *)(state + 62) = 1;
        } else {
            if (slot == 4 && total + 1 < *(u16 *)(state + 924)) {
                *index += 0xffff;
                *(u16 *)(state + 60) = 8;
                *selection += 1;
                Func_0801ba68(state, 1);
                if (*selection + *index + 2 == *(u16 *)(state + 924)) {
                    *(u16 *)(state + 62) = 0;
                }
                *(u16 *)(state + 10) = 1;
            }
        }
    } else {
        if (slot == *count) {
            *(u16 *)(state + 924) = 0;
        }
    }

    *(u16 *)(state + 0x3a2) = 1;
    Func_0801b9ec(state, *index);
    WaitFrames(1);
    Func_0801b010(*(u16 *)(*(u8 **)(state + 0x348) + 10), 0);
    WaitFrames(1);
}

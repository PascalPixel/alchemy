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

void Func_0801b810(u8 *state)
{
    u16 *index = (u16 *)(state + 0x39e);
    struct ListNode **list = (struct ListNode **)(state + 0x348);
    u16 slot;

    Func_0801b9a8(state, *index);
    *(u16 *)(state + 0x3a2) = 33;
    WaitFrames(1);

    slot = *(u16 *)(state + 916);

    if (slot > 5) {
        u16 selval = *(u16 *)(state + 924);
        u16 idx = *index;

        if ((selval | idx) != 0) {
            if (idx == 1 && selval != 0) {
                *(u16 *)(state + 8) = 8;
                *(u16 *)(state + 924) = selval + 0xffff;
                Func_0801ba68(state, 0);
                if (*(u16 *)(state + 924) == 0)
                    *(u16 *)(state + 10) = 0;
                *(u16 *)(state + 62) = idx;
            } else {
                *(u16 *)(state + 916) += 0xffff;
            }
        } else {
            struct ListNode *node = *list;
            s16 y = 64;
            *(u16 *)(state + 62) = 0;
            if (node->next != 0) {
                do {
                    node->base = node->x + y;
                    node->status = 12;
                    node = node->next;
                    y -= 16;
                } while (node->next != 0);
            }
            node = *list;
            while (node->y != node->base) {
                WaitFrames(1);
                node = *list;
            }

            {
                s16 count = *(u16 *)(state + 916);
                s16 offset = (count != 5) ? (s16)(count - 5) : 0;
                s16 i = 0;
                while (offset != 0 && i != offset)
                    i++;
                *(u16 *)(state + 916 + 84) = i;
                *(u16 *)(state + 916 + 0x100) = 4;

                node = *list;
                if (node != 0) {
                    u16 *w = (u16 *)(state + 852);
                    do {
                        Func_0801bd98(*w, w[16], node, 1);
                        node = node->next;
                        w++;
                    } while (node != 0);
                }

                node = *list;
                if (node->next != 0) {
                    do {
                        node->base = i;
                        node->status = i;
                        node = node->next;
                        i += 16;
                    } while (node->next != 0);
                }
            }
            *(u16 *)(state + 10) = 1;
        }
    } else {
        u16 idx = *index;
        if (idx != 0)
            *index = idx - 1;
        else
            *index = 0xffff;
    }

    *(u16 *)(state + 0x3a2) = 1;
    Func_0801b9ec(state, *index);
    WaitFrames(1);
    Func_0801b010(*(u16 *)(*(u8 **)(state + 0x348) + 10), 0);
    WaitFrames(1);
}

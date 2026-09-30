#include "TYPES.H"

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

struct SelectionNode {
    struct SelectionNode *prev;
    struct SelectionNode *next;
    s16 base;
    u16 kind;
    u8 unknown_0c[4];
    u16 x;
    u16 y;
    u16 unknown_14;
    u16 unknown_16;
    u16 draw_x;
    u16 draw_y;
    u8 unknown_1c[0x34 - 0x1c];
};

struct SelectionScreen {
    u8 unknown_000[0x68];
    struct SelectionNode nodes[7];
    struct SelectionNode others[5];
    u8 unknown_2d8[0x348 - 0x2d8];
    struct SelectionNode *head;
    u8 unknown_34c[0x354 - 0x34c];
    u16 kinds[16];
    u16 bases[16];
    u16 count;
    u16 x;
    u16 y;
    u16 unknown_39a;
    u16 first;
    u8 unknown_39e[0x3b8 - 0x39e];
    u16 f3b8;
};

extern struct SelectionScreen *gResQueueWork;

struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind);
void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node, s32 reuse);
void Menu_LoadSelectedResource(void);

/* Link up to five of the selection screen's options into its node list,
   then place the nodes around the screen's centred x. */

void MenuSelection_BuildEntries(void)
{
    struct SelectionScreen *screen = gResQueueWork;
    u32 count = screen->count;
    u32 index = screen->first;
    struct SelectionNode *prev = 0;
    struct SelectionNode *node;
    s32 cnt = 0;

    while (index < count) {
        s32 base = screen->bases[index];
        u32 kind = screen->kinds[index];

        node = Resource_FindFreeTransferEntry(0);
        if (node == 0)
            break;
        MenuSelection_SetupEntry(kind, base, node, 0);
        if (screen->head == 0) {
            screen->head = node;
            node->prev = 0;
        } else {
            prev->next = node;
            node->prev = prev;
        }
        node->next = 0;
        cnt++;
        prev = node;
        if (cnt == 5)
            break;
        index++;
    }

    screen->x = 100 - cnt * 8;
    screen->y = 140;
    for (prev = screen->head, cnt = 0; prev != 0; prev = prev->next) {
        s32 x = screen->x + cnt;
        s32 y;

        prev->x = x;
        y = screen->y;
        prev->y = y;
        prev->draw_x = x;
        prev->draw_y = y;
        if (prev->kind == 6 && screen->f3b8 == 0) {
            prev->y = 6;
            prev->draw_y = 6;
        }
        prev->unknown_14 = 0;
        prev->unknown_16 = 0;
        cnt += 16;
    }
    Menu_LoadSelectedResource();
}

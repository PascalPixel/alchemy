/* Draft, not exact: 22 differing halfwords, 278 of 280 bytes.
   Initialization now emits the immediate 140; x/y pointer setup and saved
   register copies still differ before the second loop. */
#include "TYPES.H"

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
};

struct SelectionScreen {
    u8 unknown_000[0x348];
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

/* FAKEMATCH: inline initialization keeps 140 an immediate. */
static __inline__ void SetPosition(struct SelectionScreen *screen, s32 cnt)
{
    screen->x = 100 - cnt * 8;
    screen->y = 140;
}

void MenuSelection_BuildEntries(void)
{
    struct SelectionScreen *screen = gResQueueWork;
    u32 count = screen->count;
    u32 index = screen->first;
    struct SelectionNode *prev = 0;
    struct SelectionNode *node;
    s32 cnt = 0;
    u16 *px;
    u16 *py;

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

    SetPosition(screen, cnt);
    px = &screen->x;
    py = &screen->y;
    cnt = 0;
    for (prev = screen->head; prev != 0; prev = prev->next) {
        s32 x = *px + cnt;
        s32 y;

        prev->x = x;
        y = *py;
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

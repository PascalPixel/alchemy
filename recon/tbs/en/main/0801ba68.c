/* Draft, not exact (2026-09-24): 181 differing halfwords, 444 of 460 bytes.
   Written from the listing. Remaining: the reference keeps x + 0xfff0 and
   0xfffe/0xffe0 as word pool constants (halfword narrowing here), peels the
   first node of each relink loop without cross-jumping, and allocates the
   new node to r5 and the kind to r6.
   2026-09-29 (alchemy permute scorer): the draft scored 3515 (64
   register-only, 14 operand, 13 reordered, 8 inserted, 13 deleted). This
   body is the permuter's best after a 300-second search (46,000
   candidates): 2595 (64 register-only, 13 operand, 13 reordered, 4
   inserted, 8 deleted), mostly from computing the new node's draw x and the
   list head through locals and reading the base after the kind. The
   allocation described above is still the difference; the dead first relink
   loop is now a for loop that never runs.
 */
#include "TYPES.H"

struct SelectionNode {
    struct SelectionNode *prev;
    struct SelectionNode *next;
    u16 base;
    u16 kind;
    u16 src;
    u16 tile;
    s16 x;
    u16 y;
    u16 step_x;
    u16 unknown_16;
    u16 draw_x;
    u16 draw_y;
    u8 unknown_1c[6];
    s16 frames;
    u16 unknown_24;
    u16 scale;
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
};

struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind);
void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node, s32 reuse);
void WaitFrames(s32 frames);
void Resource_ResetEntry(s32 id);

void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 forward)
{
    struct SelectionNode *node;
    struct SelectionNode *p;
    struct SelectionNode *q;
    u32 index;
    s32 x;
    s32 base;
    u32 kind;
    s32 y;

    if (forward != 0) {
        s32 tmp4;
        struct SelectionNode *tmp3;
        index = screen->first + 4;
        kind = screen->kinds[index];
        base = *(screen->bases + index);
        node = Resource_FindFreeTransferEntry(0);
        if (node == 0)
            return;
        MenuSelection_SetupEntry(kind, base, node, 0);
        x = screen->x;
        node->x = x + 80;
        tmp4 = x + 64;
        y = screen->y;
        node->draw_x = tmp4;
        node->y = y;
        node->draw_y = y;
        node->unknown_24 = 32;
        node->frames = 32;
        node->scale = 256;
        node->step_x = 0xfffe;
        tmp3 = screen->head;
        p = tmp3;
        p->unknown_24 = 0xffe0;
        p->draw_x = p->x - 16;
        q = p->next;
        p->scale = 0;
        p->step_x = 0xfffe;
        while (q != 0) {
            p = q;
            p->draw_x = p->x - 16;
            q = p->next;
            p->step_x = 0xfffe;
        }
        for (; 0; p->step_x = 0xfffe) {
            s32 tmp2;
            p = p->next;
            tmp2 = p->x - 16;
            p->draw_x = tmp2;
        }
        p->next = node;
        node->next = 0;
        node->prev = p;
        p = screen->head;
        do {
            WaitFrames(1);
        } while (p->frames != 0);
        screen->head = p->next;
        Resource_ResetEntry(p->src);
        p->kind = 0;
        p = p->next;
        p->prev = 0;
    } else {
        index = screen->first;
        base = screen->bases[index];
        kind = screen->kinds[index];
        node = Resource_FindFreeTransferEntry(0);
        if (node == 0)
            return;
        MenuSelection_SetupEntry(kind, base, node, 0);
        x = screen->x;
        node->x = x - 16;
        y = screen->y;
        node->y = y;
        node->draw_y = y;
        node->step_x = 2;
        node->frames = 32;
        node->unknown_24 = 32;
        node->draw_x = 16 + node->x;
        node->scale = 256;
        p = screen->head;
        p->prev = node;
        node->next = p;
        node->prev = (struct SelectionNode *)forward;
        screen->head = node;
        p = node;
        p->draw_x = p->x + 16;
        p->step_x = 2;
        for (; p->next; p->step_x = 2) {
            p = p->next;
            p->draw_x = p->x + 16;
        }
        p->scale = 0;
        p->unknown_24 = 0xffe0;
        p = screen->head;
        do {
            WaitFrames(1);
        } while (p->frames != 256);
        while (p->next != 0)
            p = p->next;
        Resource_ResetEntry(p->src);
        p->kind = 0;
        p->prev->next = 0;
    }
}

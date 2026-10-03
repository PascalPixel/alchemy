#include "RESOURCE.H"
#include "SYSTEM.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "SCENE.H"

struct UiWork {
    u8 pad0[8];
    u16 x;
};

struct Node {
    u8 pad0[32];
    u16 glyph;
};

struct Screen {
    u8 pad0[0x350];
    struct UiWork *window;
    u8 pad1[0x394 - 0x354];
    u16 f394;
    u8 pad2[0x3a0 - 0x396];
    u16 f3a0;
    u8 pad3[0x3b8 - 0x3a2];
    u16 f3b8;
};


extern u8 MsgItemNotHeld[];
extern u8 MsgAbilityNotKnown[];

struct UiWork *UiWindow_Create(s32, s32, s32, s32, s32);

struct Node *NodeChain_GetNodeAtCount(struct Screen *, u32);
void UiText_DrawCharacterAtOffset(s32, struct UiWork *, s32, s32);
void UiWork_Finalize(struct UiWork *, s32);
void RenderOutput_PrepareForRedraw(struct UiWork *);

void Menu_OpenSelectionWindow(s32 mode, u32 count)
{
    struct Screen *screen;
    struct Node *node;
    struct UiWork **slot;
    struct UiWork *window;

    screen = Ram_HeapSlots->menu_work;
    node = NodeChain_GetNodeAtCount(screen, count);
    slot = &screen->window;
    window = *slot;
    if (window == 0) {
        if (mode == 6) {
            if (screen->f3b8 != 0) {
                *slot = UiWindow_Create(17, 17, 5, 3, mode);
            } else {
                *slot = UiWindow_Create(17, 0, 5, 3, mode);
            }
            screen->f3a0 = 0;
            screen->f3b8 = 999;
        } else {
            *slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
        }
        RenderOutput_PrepareForRedraw(screen->window);
    } else {
        if (count != 0 && window->x != count + 2) {
            UiWork_Finalize(window, 2);
            *slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
        }
        RenderOutput_PrepareForRedraw(screen->window);
    }
    if (screen->f394 != 0) {
        UiText_DrawCharacterAtOffset(node->glyph, screen->window, 0, 0);
    } else {
        switch (mode) {
        case 4:
            UiText_DrawCharacterAtOffset((s32)MsgAbilityNotKnown, screen->window, 0, 0);
            break;
        case 2:
            UiText_DrawCharacterAtOffset((s32)MsgItemNotHeld, screen->window, 0, 0);
            break;
        }
    }
}

struct ResourceNode {
    u32 value0;
    struct ResourceNode *next;
    u16 value8;
    u16 active;
    u16 handle;
};

void Resource_ScheduleOwnerReset(void);
void Resource_ResetPendingTransfer(void);

/* ☀️'s, reaching the menu work through its heap slot, whose block ⚓️
   releases by the slot's offset. */
void Resource_ResetOwnerEntries(void)
{
    u8 *state = Ram_HeapSlots->menu_work;
    struct ResourceNode *node;

    Resource_ScheduleOwnerReset();
    UiWork_Finalize(*(struct UiWork **)(state + 0x350), 2);
    WaitFrames(1);
    node = *(struct ResourceNode **)(state + 0x348);
    while (node != 0) {
        if (node->active != 0) {
            Resource_ResetEntry(node->handle);
            node->active = 0;
        }
        node = node->next;
    }
    node = *(struct ResourceNode **)(state + 0x34c);
    while (node != 0) {
        if (node->active != 0) {
            Resource_ResetEntry(node->handle);
            node->active = 0;
        }
        node = node->next;
    }
    Resource_ResetPendingTransfer();
    if (*(s16 *)(state + 18) != 0) {
        Resource_ResetEntry(*(u16 *)(state + 12));
        if (*(s16 *)(state + 18) != 0) {
            Resource_ResetEntry(*(u16 *)(state + 64));
        }
    }
    Resource_ResetEntry(*(u16 *)(state + 0x2e4));
    Runtime_ReleaseHeapBlock(72);
}

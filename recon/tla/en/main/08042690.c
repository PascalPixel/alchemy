/*
 * Draft: UiWindow_CreateWithLayoutBounds matches as it stands; it follows
 * UiWindow_BuildLayoutBounds (08042630) into GRAPHICS/WINDOW/LAYOUT.C once
 * that one does.
 */
#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "RAM_BUFFER.H"

/* The party status window's place, which its heap slot keeps. */
struct UiWindowBounds {
    s32 handle;
    u16 left;
    u16 top;
    u16 right;
    u16 height;
    u16 flags;
};

s32 BattleParty_PrepareActiveOwnersFar(u16 *owners);
s32 Party_CountActiveOwnersFar(void);
s32 UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWindow_DrawPartyStatusContents(s32 flags);

void UiWindow_BuildLayoutBounds(s32 flags);

void UiWindow_CreateWithLayoutBounds(s32 flags)
{
    s32 zero;
    struct UiWindowBounds *window;
    u8 *tiles;

    window = Runtime_AllocateBlock(64, sizeof(struct UiWindowBounds));
    tiles = Ram_HeapSlots->window_tiles;
    zero = 0;
    tiles[6] = 1;
    UiWindow_BuildLayoutBounds(flags);
    window->handle = UiWindow_Create(
        window->left, window->top, window->right, window->height, 6);
    UiWindow_DrawPartyStatusContents(flags);
    tiles[6] = zero;
}

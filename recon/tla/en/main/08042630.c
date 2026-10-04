/*
 * Draft: UiWindow_BuildLayoutBounds, ported from its ☀️ twin; it goes after
 * UiWindow_FillScreenBlockRect in GRAPHICS/WINDOW/LAYOUT.C. Score 2: the
 * listing copies flags to r6 between loading the window work and reading its
 * mode byte; this C copies it after. Barriers and a flags temporary do not
 * place it. It matches with load scheduling (-mtune=arm9tdmi).
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

void UiWindow_BuildLayoutBounds(s32 flags)
{
    struct UiWindowBounds *state = Ram_HeapSlots->window_layout;
    u8 *base = Ram_HeapSlots->window_tiles;
    s32 height = 4;
    s32 n;
    s32 right;
    s32 left;

    if (base[5] != 0) {
        n = BattleParty_PrepareActiveOwnersFar(0);
        height = 3;
    } else {
        n = Party_CountActiveOwnersFar();
        if ((u32)n > 4)
            n = 4;
    }
    if (flags & 1)
        height++;
    else
        flags &= -3;

    n *= 6;
    right = n + 1;
    if (flags & 2)
        right = n + 6;

    left = 30;
    left -= right;
    state->left = left;
    state->top = 0;
    state->right = right;
    state->height = height;
    state->flags = flags;
}


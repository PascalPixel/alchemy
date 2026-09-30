#include "TYPES.H"
extern u8 Data_03001e90[];
extern u8 Data_03001e8c[];

void UiWindow_BuildLayoutBounds(s32 flags)
{
    void **slot = (void **)((u32)&Data_03001e90);
    struct UiWindowBounds *state = *slot;
    u8 *base = *(u8 **)(slot - 1);
    s32 height = 4;
    s32 n;
    s32 right;
    s32 left;

    if (base[RENDER_MENU_STATE_OFS] != 0) {
        n = BattleParty_PrepareActiveOwnersFar(0);
        height = 3;
    } else {
        n = Party_CountActiveOwnersFar();
    }
    if (flags & 1)
        height++;
    else
        flags &= -3;

    n *= 6;
    right = n + 1;
    if (flags & 2)
        right += 5;

    left = 30;
    left -= right;
    state->left = left;
    state->top = 0;
    state->right = right;
    state->height = height;
    state->flags = flags;
}

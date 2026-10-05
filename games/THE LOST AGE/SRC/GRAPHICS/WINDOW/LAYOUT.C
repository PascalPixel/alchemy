#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "RAM_BUFFER.H"

s32 BattleParty_PrepareActiveOwnersFar(u16 *owners);
s32 Party_CountActiveOwnersFar(void);
s32 UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWindow_DrawPartyStatusContents(s32 flags);

void UiWindow_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width, u32 height,
                   s32 value)
{
    u32 row = 0;
    s16 *dst = (s16 *)0x06002000;

    if (row < height) {
        do {
            u32 column = 0;
            if (column < width) {
                do {
                    column++;
                    *dst = value;
                    dst++;
                } while (column < width);
            }
            row++;
            dst += 32 - width;
        } while (row < height);
    }
}

/* The party status window's handle and place, which its heap slot keeps. */
struct PartyStatusWindow {
    s32 window;
    u16 x;
    u16 y;
    u16 width;
    u16 height;
    u16 flags;
};

/* Places the party status window at the screen's right edge: six tiles
   for each member (at most four, or the battle's active owners), one more
   row with flag 1 and a five-tile wider frame with flag 2. Byte 5 of the
   window tilemap's header marks battle. */
void UiWindow_BuildLayoutBounds(s32 flags)
{
    struct PartyStatusWindow *state = Ram_HeapSlots->window_layout;
    u8 *tiles = Ram_HeapSlots->window_tiles;
    s32 height = 4;
    s32 n;
    s32 right;
    s32 left;

    if (tiles[5] != 0) {
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
    state->x = left;
    state->y = 0;
    state->width = right;
    state->height = height;
    state->flags = flags;
}

/* Opens the party status window where UiWindow_BuildLayoutBounds places it
   and draws its contents, with the tilemap's busy byte set meanwhile. */
void UiWindow_CreateWithLayoutBounds(s32 flags)
{
    s32 zero;
    struct PartyStatusWindow *window;
    u8 *tiles;

    window = Runtime_AllocateBlock(64, sizeof(struct PartyStatusWindow));
    tiles = Ram_HeapSlots->window_tiles;
    zero = 0;
    tiles[6] = 1;
    UiWindow_BuildLayoutBounds(flags);
    window->window = UiWindow_Create(
        window->x, window->y, window->width, window->height, 6);
    UiWindow_DrawPartyStatusContents(flags);
    tiles[6] = zero;
}

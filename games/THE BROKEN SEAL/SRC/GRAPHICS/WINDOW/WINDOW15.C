#include "TYPES.H"
#include "DMA.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

extern const u8 Tile_BuildMetatiles[];
extern u8 Tile_BuildMetatilesCodeSize[];
extern void *Data_03001e8c;
void *Runtime_BumpAllocate(u32 size);
void Runtime_BumpFree(void *allocation);
typedef void (*RamRoutine)(void *dst, void *scene);

extern u8 Data_03001e90[];

struct UiWindowBounds {
    s32 handle;
    u16 left;
    u16 top;
    u16 right;
    u16 height;
    u16 flags;
};

extern s32 BattleParty_PrepareActiveOwnersFar(s32);
extern s32 Party_CountActiveOwnersFar(void);
void UiWindow_BuildLayoutBounds(s32 flags);
void UiWindow_DrawPartyStatusContents(s32);
void *Runtime_AllocateBlock(s32 flags, s32 arg1);
s32 UiWindow_Create(u16, u16, u16, u16, s32);

/* With no scene loaded, fills the 160-entry buffer with 0xe0e0; otherwise
   copies the metatile builder into RAM and runs it over the buffer and the
   scene record. */
void UiWindow_FillFromScene(void *dst)
{
    void *scene = Data_03001e8c;

    if (scene == 0) {
        volatile u16 fill = 0xe0e0;

        Dma_Set((void *)&fill, dst, 0x810000a0, (volatile u32 *)0x040000d4);
    } else {
        u32 size = (u32)Tile_BuildMetatilesCodeSize;
        RamRoutine code = (RamRoutine)Runtime_BumpAllocate(size);

        Dma_Set((void *)Tile_BuildMetatiles, code, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
        code(dst, scene);
        Runtime_BumpFree(code);
    }
}

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

void UiWindow_CreateWithLayoutBounds(s32 flags)
{
    s32 zero;
    struct UiWindowBounds *window;
    s8 *busy;

    window = Runtime_AllocateBlock(0x10, 0x10);
    busy = (s8 *)((u8 *)*(void **)((u32)&Data_03001e8c) + RENDER_MENU_BUSY_OFS);
    zero = 0;
    *busy = 1;
    UiWindow_BuildLayoutBounds(flags);
    window->handle = UiWindow_Create(
        window->left, window->top, window->right, window->height, 6);
    UiWindow_DrawPartyStatusContents(flags);
    *busy = zero;
}

#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "GLYPH.H"
#include "RESOURCE.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "SELECT.H"

extern u8 Tile_Decompress4bppCode[];
extern u8 Tile_ExpandMaskedCode[];
extern u8 Tile_ExpandOpaqueCode[];
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];

void Resource_ClearOwnerListAndCounters(void)
{
    struct SelectionScreen *screen;

    screen = Ram_HeapSlots->menu_work;
    screen->head = NULL;
    screen->unknown_39a = 0;
    if (screen->cursor_index & 0x80) {
        screen->top = 0;
        screen->cursor_index = 0;
    }
    screen->vertical_scroll = 0;
    screen->count = 0;
}

void Resource_PushPendingPair(u32 first, u32 second)
{
    struct SelectionScreen *state = Ram_HeapSlots->menu_work;
    u16 cnt = state->count;

    if (cnt != 16) {
        state->kinds[cnt] = first;
        state->bases[cnt] = second;
        state->count++;
    }
}

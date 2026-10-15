#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "glyph.h"
#include "resource.h"
#include "dma.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "select.h"

extern u8 Tile_Decompress4bppCode[];
extern u8 Tile_ExpandMaskedCode[];
extern u8 Tile_ExpandOpaqueCode[];
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];

void Resource_ClearOwnerListAndCounters(void)
{
    struct SelectionScreen *screen;

    screen = Ram_HeapSlots->menuWork;
    screen->head = NULL;
    screen->unk39a = 0;
    if (screen->cursorIndex & 0x80) {
        screen->top = 0;
        screen->cursorIndex = 0;
    }
    screen->verticalScroll = 0;
    screen->count = 0;
}

void Resource_PushPendingPair(u32 first, u32 second)
{
    struct SelectionScreen *state = Ram_HeapSlots->menuWork;
    u16 cnt = state->count;

    if (cnt != 16) {
        state->kinds[cnt] = first;
        state->bases[cnt] = second;
        state->count++;
    }
}

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "types.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "map_scroll.h"
#include "map.h"
#include "ram_buffer.h"
#include "io_reg.h"


void MapAnimation_Update(void)
{
    /* FAKEMATCH: needed to match: indexed channel expressions preserve ip/lr allocation and the sentinel/VRAM pool order. */
    struct MapState *state = gMapWork[0];
    u32 i;

    for (i = 0; i <= 15; i++) {
        const u16 *script;
        u32 op;
        u32 count;
        u32 dst;

        if ((state->anim + i)->start == NULL || (state->anim + i)->paused != 0)
            continue;
    next:
        if ((state->anim + i)->timer == 0) {
            script = (state->anim + i)->cursor;
            op = *script++;
            if (op == 0xffff) {
                (state->anim + i)->cursor = (state->anim + i)->start;
                goto next;
            }
            if ((op & 0xff00) == 0xfe00) {
                if ((op & 0xff) == 0xff)
                    continue;
                (state->anim + i)->cursor = (state->anim + i)->start + (op & 0xff) * 2;
                goto next;
            }
            count = *script++;
            dst = script[0];
            (state->anim + i)->timer = script[1];
            if (state->wide_tiles == 0) {
                if (op >= 0x600)
                    Dma_Set(Ram_DecodeBuffer + op * 32, BG_CHAR_BLOCK(1) + dst * 32, DMA_ENABLE32 | DMA_32BIT | count * 8, REG_DMA3);
                else
                    Dma_Set(BG_CHAR_BLOCK(1) + op * 32, BG_CHAR_BLOCK(1) + dst * 32, DMA_ENABLE32 | DMA_32BIT | count * 8, REG_DMA3);
            } else {
                if (op >= 0x200)
                    Dma_Set(Ram_MapBlocks + op * 64, BG_CHAR_BLOCK(2) + dst * 64, DMA_ENABLE32 | DMA_32BIT | count * 16, REG_DMA3);
                else
                    Dma_Set(BG_CHAR_BLOCK(2) + op * 64, BG_CHAR_BLOCK(2) + dst * 64, DMA_ENABLE32 | DMA_32BIT | count * 16, REG_DMA3);
            }
            (state->anim + i)->cursor += 4;
            goto next;
        } else {
            (state->anim + i)->timer--;
        }
    }
}

void Map_ClearLayerEntryFlag(u32 no)
{
    struct MapState *state = gMapWork[0];
    struct MapAnimation *anim = &state->anim[no];

    anim->paused = 0;
}

void Map_SetLayerEntryFlag(u32 no)
{
    struct MapState *state = gMapWork[0];
    struct MapAnimation *anim = &state->anim[no];

    anim->paused = 1;
}

void MapAnimation_StartChannels(const u16 *script)
{
    struct MapState *state;
    s32 count;
    u32 command;
    volatile u32 zero;

    state = gMapWork[0];
    count = 0;
    zero = 0;
    Dma_Set((const void *)&zero, state->anim, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x30, REG_DMA3);
    command = *script++;
    while (command != 0xffff) {
        if ((command & 0xff00) == 0xfd00) {
            struct MapAnimation *anim;
            u32 idx = command & 15;
            u32 flag = 0;
            if (command & 0x80)
                flag = 1;
            anim = &state->anim[idx];
            anim->start = script;
            anim->cursor = script;
            anim->timer = 0;
            anim->paused = flag;
            count++;
        }
        command = *script++;
    }
    if (count != 0)
        Scheduler_AddOrUpdateCallback(MapAnimation_Update, 0xc80);
}

void Map_EnableUpdateCallback(void)
{
    if (((struct MapScrollWork *)gMapWork[0])->animation_active == 0)
        Scheduler_EnableCallbacks(MapAnimation_Update);
}

void Map_DisableUpdateCallback(void)
{
    if (((struct MapScrollWork *)gMapWork[0])->animation_active == 0)
        Scheduler_DisableCallbacks(MapAnimation_Update);
}

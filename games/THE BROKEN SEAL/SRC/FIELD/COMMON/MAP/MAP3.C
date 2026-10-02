#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "MAP_RENDER_WORK.H"
#include "RAM_BUFFER.H"

void MapAnimation_Update(void);

/* One of the sixteen tile-animation channels in the map state. */
struct MapAnimation {
    const u16 *start;
    const u16 *cursor;
    u16 timer;
    u16 paused;
};

struct MapState {
    u8 unknown_00[22];
    /* 0: the map's characters are 4 bpp in the second character block;
       otherwise 8 bpp in the third. */
    u8 wide_tiles;
    u8 unknown_17;
    struct MapAnimation anim[16];
};


/* Steps the sixteen tile-animation channels. A channel whose timer has run
   out reads commands until one copies characters: 0xffff restarts the
   channel, 0xfeXX jumps to command XX (0xfeff stops it for this frame), and
   any other command copies COUNT characters from character OP to character
   DST and waits TIMER frames. Characters past VRAM's range come from EWRAM.
   Every channel access is spelled (state->anim + i)->field: the repeated
   address arithmetic keeps loop.c's first pass over its threshold, so the
   0xffff sentinel is hoisted only in the rerun, as in the game. */
void MapAnimation_Update(void)
{
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
                    Dma_Set(Ram_DecodeBuffer + op * 32, (void *)(dst * 32 + 0x06004000), (count * 8) | 0x84000000, (volatile u32 *)0x040000d4);
                else
                    Dma_Set((void *)(op * 32 + 0x06004000), (void *)(dst * 32 + 0x06004000), (count * 8) | 0x84000000, (volatile u32 *)0x040000d4);
            } else {
                if (op >= 0x200)
                    Dma_Set(Ram_MapBlocks + op * 64, (void *)(dst * 64 + 0x06008000), (count * 16) | 0x84000000, (volatile u32 *)0x040000d4);
                else
                    Dma_Set((void *)(op * 64 + 0x06008000), (void *)(dst * 64 + 0x06008000), (count * 16) | 0x84000000, (volatile u32 *)0x040000d4);
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
    u8 *base = ((void *volatile *)gMapWork)[0];
    u8 *entry = base + no * 12;
    u32 value = 0;
    *(u16 *)(entry + 0x22) = value;
}

void Map_SetLayerEntryFlag(u32 no)
{
    u8 *base = ((void *volatile *)gMapWork)[0];
    u8 *entry = base + no * 12;
    u32 value = 1;
    *(u16 *)(entry + 0x22) = value;
}

/* Clears the sixteen tile-animation channels, then reads a 0xffff-terminated
   command list: each 0xfdXX command starts channel XX & 15 on the commands
   that follow it, held paused when bit 7 is set. Schedules the animation update
   when any channel started. */
void MapAnimation_StartChannels(const u16 *script)
{
    struct MapState *state;
    s32 count;
    u32 command;
    volatile u32 zero;

    state = gMapWork[0];
    count = 0;
    zero = 0;
    Dma_Set((const void *)&zero, state->anim, 0x85000030, (volatile u32 *)0x040000d4);
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
        Scheduler_AddOrUpdateCallback((s32)(MapAnimation_Update), 0xc80);
}

void Map_EnableUpdateCallback(void)
{
    if (((struct MapRenderWork *)gMapWork[0])->active == 0)
        Scheduler_EnableCallbacks((u32)MapAnimation_Update);
}

void Map_DisableUpdateCallback(void)
{
    if (((struct MapRenderWork *)gMapWork[0])->active == 0)
        Scheduler_DisableCallbacks((u32)MapAnimation_Update);
}

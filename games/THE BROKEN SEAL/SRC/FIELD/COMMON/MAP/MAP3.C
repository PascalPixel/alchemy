#include "TYPES.H"
#include "DMA.H"
#include "MAP_RENDER_WORK.H"

s32 Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
void MapAnimation_Update(void);

/* One of the sixteen tile-animation channels in the map state. */
struct MapAnimation {
    const u16 *start;
    const u16 *cursor;
    u16 timer;
    u16 looping;
};

struct MapState {
    u8 unk_000[24];
    struct MapAnimation anim[16];
};

extern struct MapState *gCam;

s32 Scheduler_EnableCallbacks(u32 value);

s32 Scheduler_DisableCallbacks(u32 value);

void Map_ClearLayerEntryFlag(u32 no)
{
    u8 *base = (*(u8 *volatile *)&gMapWork);
    u8 *entry = base + no * 12;
    u32 value = 0;
    *(u16 *)(entry + 0x22) = value;
}

void Map_SetLayerEntryFlag(u32 no)
{
    u8 *base = (*(u8 *volatile *)&gMapWork);
    u8 *entry = base + no * 12;
    u32 value = 1;
    *(u16 *)(entry + 0x22) = value;
}

/* Clears the sixteen tile-animation channels, then reads a 0xffff-terminated
   command list: each 0xfdXX command starts channel XX & 15 on the commands
   that follow it, looping when bit 7 is set. Schedules the animation update
   when any channel started. */
void MapAnimation_StartChannels(const u16 *script)
{
    struct MapState *state;
    s32 count;
    u32 command;
    volatile u32 zero;

    state = gCam;
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
            anim->looping = flag;
            count++;
        }
        command = *script++;
    }
    if (count != 0)
        Scheduler_AddOrUpdateCallback(MapAnimation_Update, 0xc80);
}

void Map_EnableUpdateCallback(void)
{
    if (gMapWork->active == 0)
        Scheduler_EnableCallbacks((u32)MapAnimation_Update);
}

void Map_DisableUpdateCallback(void)
{
    if (gMapWork->active == 0)
        Scheduler_DisableCallbacks((u32)MapAnimation_Update);
}

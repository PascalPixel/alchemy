/* alchemy permute 0802cc9c: weighted 120, differ 4 (2 reordered); draft weighted 170, differ 14 (5 register, 2 reordered).
   Job 0, iteration 839; rewrites: 1x reorder independent statements (minimized from 1: 1x reorder independent statements). */
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "iwram_call.h"
#include "callback_scheduler.h"

void MapAnimation_Update(void);

void MapAnimation_StartChannels(const u16 *script)
{
    struct MapWork *state = Ram_HeapSlots->mapWork;
    s32 count;
    u32 command;

    count = 0;
    Iwram_ClearWords(state->anim, sizeof(state->anim));
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
    if (count != 0 && Scheduler_FindCallback(MapAnimation_Update) == -1)
        Scheduler_AddOrUpdateCallback(MapAnimation_Update, 0x480);
}

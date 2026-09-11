#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/map_init/wait_value_below_limit.h"

struct Track02001038 {
    u8 head[12];
    s32 value;
    u8 gap16[4];
    s32 limit;
    u8 gap24[16];
    s32 state;
    u8 gap44[16];
    s32 mark;
};

void Actor_WaitValueBelowLimit(struct Track02001038 *track)
{
    s32 cnt = 60;
    s32 limit;

    for (;;) {
        if (cnt != 0) {
            s32 value;

            Actor_Do(1);
            value = track->value;
            limit = track->limit;
            cnt--;
            if (value <= limit) {
                break;
            }
            continue;
        }
        limit = track->limit;
        break;
    }
    track->state = 0;
    track->value = limit;
    track->mark = (s32) 0x80000000;
}

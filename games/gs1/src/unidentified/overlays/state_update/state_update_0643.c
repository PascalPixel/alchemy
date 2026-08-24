#include "types.h"

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

extern void Func_0200406a(s32);

void Func_02001038(struct Track02001038 *track)
{
    s32 remaining = 60;
    s32 limit;

    for (;;) {
        if (remaining != 0) {
            s32 value;

            Func_0200406a(1);
            value = track->value;
            limit = track->limit;
            remaining--;
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

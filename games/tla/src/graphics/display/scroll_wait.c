#include "types.h"

extern s16 Data_02007520;
extern s32 Data_0200751c;

void WaitFrames(s32 frames);

// 値がそろうまで待つ (最長 300 フレーム)
void Func_081a17d0(void)
{
    s32 frames = 0;

    if (Data_0200751c != Data_02007520 << 16) {
        do {
            WaitFrames(1);
            frames++;
            if (frames > 299)
                break;
        } while (Data_0200751c != Data_02007520 << 16);
    }
}

struct BucketEntry {
    void *next;
    u8 unk04;
    u8 unk05[7];
};

extern struct BucketEntry *Data_0200752c;

void Func_080140d8(void *entry, s32 slot);

void Func_081a1810(void)
{
    struct BucketEntry *entry = Data_0200752c;
    s32 i;

    for (i = 0; i < 24; i++)
        Func_080140d8(entry++, 0);
}

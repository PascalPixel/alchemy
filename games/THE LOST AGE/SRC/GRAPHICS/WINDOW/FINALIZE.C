#include "TYPES.H"

struct UiWindowWork {
    s32 unknown00;
    s32 unknown04;
    u16 width;
    u16 height;
    u16 x;
    u16 y;
    u16 unknown10;
    u16 unknown12;
    u16 state;
    u16 flags;
    u16 frame;
    s16 duration;
    u16 previous_x;
    u16 previous_y;
    u16 previous_width;
    u16 previous_height;
};

s32 WaitFrames(s32 frames);

void UiWork_WaitUntilField1aClear(void *work)
{
    /* 値が0になるまで更新処理を進める。 */
    if (!(2 & ((struct UiWindowWork *)work)->flags) && (((struct UiWindowWork *)work)->duration != 0)) {
        do {
            WaitFrames(1);
        } while (((struct UiWindowWork *)work)->duration != 0);
    }
}

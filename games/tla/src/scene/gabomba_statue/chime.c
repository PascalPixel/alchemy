#include "types.h"
#include "iwram_call.h"

struct ClockWork {
    u8 unk00[132];
    s32 timer;
    u8 ringing;
    u8 mode;
};

extern u8 Data_020023c4[];
void Func_02005854(s32 sound);
s32 Math_Cosine(s32 angle);

// 残り時間 (フレーム) を "MM:SS:T" の数字にする
void Func_02004f84(s32 time, u8 *text)
{
    if (time < 0)
        time = 0;
    *text = time / 36000 + '0';
    time %= 36000;
    *++text = time / 3600 + '0';
    time %= 3600;
    *++text = ':';
    *++text = time / 600 + '0';
    time %= 600;
    *++text = time / 60 + '0';
    time %= 60;
    *++text = ':';
    *++text = time / 6 + '0';
    text[1] = 0;
}

s32 Func_02005020(s32 time)
{
    struct ClockWork *clock = (struct ClockWork *)(Data_020023c4 + 0x288);
    s32 second = time % 60;
    s32 volume = 512;
    u8 *ringing = &clock->ringing;

    if (second != 0) {
        *ringing = 1;
    } else if (*ringing) {
        if (clock->timer != 0)
            Func_02005854(236);
        *ringing = second;
    }
    if (second <= 19) {
        s32 cosine = Math_Cosine((second << 15) / 10);
        s32 (*mul)(s32 a, s32 b);
        s32 scale = 128;

        mul = Iwram_MulQ16;
        volume = mul(scale << 2, cosine);
    }
    if (volume <= 16)
        volume = 16;
    return volume;
}

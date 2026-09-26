/* Draft, not exact (2026-09-26): 264 of 256 bytes, 131 differing halfwords.
   Complete palette-transition fade owner, including its literal pool.
   Corrected the previous draft's red increment to the observed -0x800;
   green and blue decrease by 64 and 2 over sixteen frames. Remaining:
   palette-pointer retention, loop register allocation, and an early pool
   introduced by the volatile halfword white store. Direct symbolic palette
   cells and pool-symbol white also failed to reproduce the saved registers. */
#include "TYPES.H"

struct SceneColorWork {
    u8 unknown_000[414];
    s16 mode;
    u8 unknown_1a0[38];
    u16 step;
};
extern struct SceneColorWork *Data_03001ebc;
extern s16 Data_02000240[];
extern u16 Data_050001e6;
extern const u8 Value_00007fff;
extern const u8 Value_00000401;
extern const u8 Value_00000207;
extern const u8 Value_fffff800;
void Audio_PlayCue(s32);
void Func_080901c0(s32, s32);
void WaitFrames(s32);

void Func_080941e0(void)
{
    struct SceneColorWork *work;
    work = Data_03001ebc;
    Audio_PlayCue(Data_02000240[247]);
    Audio_PlayCue(288);
    Audio_PlayCue(147);
    if (work->mode == 3) {
        s32 red, green, blue, cnt;
        *(volatile u16 *)0x050001e6 = 0x7fff;
        Func_080901c0((s32)&Value_00000401, 16);
        work->step = 0;
        WaitFrames(16);
        red = 0x7800;
        green = 960;
        blue = 30;
        for (cnt = 0; cnt < 16; cnt++) {
            *(volatile u16 *)0x050001e6 = red | green | blue;
            WaitFrames(1);
            red -= 0x800;
            green -= 64;
            blue -= 2;
        }
    } else {
        s32 red, green, blue, cnt;
        *(volatile u16 *)0x05000000 = 0x7fff;
        Func_080901c0((s32)&Value_00000207, 16);
        work->step = 0;
        WaitFrames(16);
        red = 0x7800;
        green = 960;
        blue = 30;
        cnt = 15;
        do {
            *(volatile u16 *)0x05000000 = red | green | blue;
            WaitFrames(1);
            red -= 0x800;
            green -= 64;
            blue -= 2;
            cnt--;
        } while (cnt >= 0);
    }
}

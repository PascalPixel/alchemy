/* 2026-09-29 alchemy permute: score 2300 to 1270 on the permuter's scorer
   (0 is exact); remaining 25 register-only, 11 operand, 2 reordered, 7
   inserted, 1 deleted. Kept rewrites: 7x swap commutative operands, 6x
   reorder independent statements, 5x add a same-width cast, 4x introduce a
   temporary, 2x reorder local declarations, 2x remove a temporary, 2x
   split or join a compound assignment, 1x drop a same-width cast, 1x
   toggle register. FAKEMATCH: the permuter's temporaries, register hints
   and swapped operand orders below only steer allocation and scheduling;
   no programmer would write them, so they stay tagged until a natural
   spelling replaces them. */
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
        s32 green, cnt, blue, red;
        *(volatile u16 *)0x050001e6 = 0x7fff;
        Func_080901c0((s32)&Value_00000401, 16);
        work->step = 0;
        WaitFrames(16);
        blue = 30;
        green = 960;
        red = 0x7800;
        for (cnt = 0; cnt < 16; (u32)cnt++) {
            *(volatile u16 *)0x050001e6 = red | blue | green;
            WaitFrames(1);
            green -= 64;
            red -= 0x800;
            blue -= 2;
        }
    } else {
        s32 red, green, blue, cnt;
        u32 tmp2;
        tmp2 = (u32)0x7fff;
        *(volatile u16 *)0x05000000 = tmp2;
        red = 0x7800;
        Func_080901c0((s32)&Value_00000207, 16);
        work->step = 0;
        WaitFrames(16);
        green = 960;
        blue = 30;
        cnt = 15;
        do {
            s32 tmp;
            *(volatile u16 *)0x05000000 = red | (blue | green);
            WaitFrames(1);
            tmp = green - 64;
            green = tmp;
            red -= 0x800;
            (u32)(blue = blue - (s32)2);
            cnt--;
        } while (cnt >= 0);
    }
}

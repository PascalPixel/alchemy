/* 2026-09-29 alchemy permute: score 1270 to 925 on the permuter's scorer
   (0 is exact); remaining 29 register-only, 8 operand, 2 reordered, 4
   inserted, 1 deleted. Kept rewrites: 3x reorder independent statements,
   2x swap commutative operands, 2x introduce a temporary, 2x add a
   same-width cast, 2x split or join a compound assignment, 1x reorder
   local declarations, 1x remove a temporary, 1x drop a same-width cast, 1x
   change loop form, 1x toggle register. FAKEMATCH: the permuter's
   temporaries, register hints and swapped operand orders below only steer
   allocation and scheduling; no programmer would write them, so they stay
   tagged until a natural spelling replaces them. */
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
extern struct SceneColorWork *gEventWork;
extern s16 gGameState[];
extern u16 Data_050001e6;
void Audio_PlayCue(s32);
void DisplayTransition_Finish(s32, s32);
void WaitFrames(s32);

void Func_080941e0(void)
{
    register struct SceneColorWork *work;

    work = gEventWork;
    Audio_PlayCue(gGameState[247]);
    Audio_PlayCue(288);
    Audio_PlayCue(147);
    if (work->mode == 3) {
        s32 green, cnt, blue, red;
        *(volatile u16 *)0x050001e6 = 0x7fff;
        DisplayTransition_Finish(0x401, 16);
        work->step = 0;
        WaitFrames(16);
        blue = (u32)30;
        green = 960;
        red = 0x7800;
        cnt = 0;
        while (cnt < 16) {
            s32 tmp3;
            tmp3 = red | blue;
            *(volatile u16 *)0x050001e6 = tmp3 | green;
            WaitFrames(1);
            green -= 64;
            red = red - 0x800;
            blue -= 2;
            (u32)cnt++;
        }
    } else {
        u32 tmp2;
        s32 red, green, blue, cnt;
        tmp2 = (u32)0x7fff;
        *(volatile u16 *)0x05000000 = tmp2;
        DisplayTransition_Finish(0x207, 16);
        work->step = 0;
        WaitFrames(16);
        green = 960;
        red = 0x7800;
        blue = 30;
        cnt = 15;
        do {
            s32 tmp;
            tmp = green - 64;
            *(volatile u16 *)0x05000000 = red | green | blue;
            WaitFrames(1);
            red = red - 0x800;
            (u32)(blue = blue - (s32)2);
            green = tmp;
            cnt--;
        } while (cnt >= 0);
    }
}

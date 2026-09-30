/* 2026-09-29 alchemy permute: score 1170 to 480 on the permuter's scorer
   (0 is exact); remaining 21 register-only, 6 operand, 4 reordered. Kept
   rewrites: 11x reorder independent statements, 8x swap commutative
   operands, 5x reorder local declarations, 5x introduce a temporary, 3x
   change loop form, 2x pointer arithmetic or indexing, 2x split or join a
   compound assignment, 2x move an assignment into or out of a condition,
   2x toggle register, 1x remove a temporary, 1x add a same-width cast, 1x
   drop a same-width cast, 1x test truth or compare with zero. FAKEMATCH:
   the permuter's temporaries, register hints and swapped operand orders
   below only steer allocation and scheduling; no programmer would write
   them, so they stay tagged until a natural spelling replaces them. */
/* Draft, not exact (2026-09-28): 328 of 328 bytes, 83 differing lines
   (from 119). Crossfades the 33 slide images between BG2 and BG3, then
   restores the menu display. Hardware writes go through an inline taking an
   s32 value so constants are word-built as in the reference; the fade loops
   are ordinary counted loops, which reproduces the reference's outer-loop
   invariants (1 in fp, the BLDALPHA address in sl, the resource pointer in
   r9) and the inner loop's shape. Residuals: the reference keeps the loop's
   odd-slide test in its own register (a copy made after the load call) and
   gives base r5 and alpha r6; here the test shares the call argument's
   register and base/alpha take r7/r5. The first DISPCNT address is also
   scheduled above the scheduler call, the BG3CNT value/address registers are
   swapped, and BLDCNT is derived from BG3CNT + 66 instead of loaded fresh.
   Needs the label DisplayScroll_SlideResources at 0x080f0a5c and the
   0x080f0000 veneer to name this function. Separate odd flags, do/while
   and while inner loops and explicit nonzero tests did not move it. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"

extern u8 gOamCopyEnabled;
extern u8 Data_03001f58;
extern u8 Data_03001ac4;
extern u8 gOptionMirror;
extern const u32 DisplayScroll_SlideResources[];

void Scheduler_ResetTaskTable(void);
void DisplayScroll_StepPositionEveryFourFrames(void);
void DisplayScroll_BuildHblankWordTable(void *table);
void Graphics_ClearCharacterBlockAndPalette(s32 block);
void Graphics_LoadCharacterBlockAndPalette(u32 resource, s32 block);
void DisplayScroll_InitObjectTable(void);
void Ui_LoadWindowGraphics(void);
void Bg0_ClearTilemap(void);

static __inline__ void WriteHalf(volatile u16 *port, s32 value)
{
    *port = value;
}

s32 DisplayScroll_RunSlideshow(void)
{
    register u32 slide;
    const u32 *resource;
    s32 alpha;
    s32 tmp;
    register s32 base;
    s32 tmp2;
    void *tmp4;

    gOamCopyEnabled = 0;
    Data_03001f58 = 0;
    tmp4 = (void *)0x0600f800;
    Data_03001ac4 = 0;
    gOptionMirror = 0;
    Scheduler_ResetTaskTable();
    tmp2 = (s32)DisplayScroll_StepPositionEveryFourFrames;
    Scheduler_AddOrUpdateCallback(tmp2, 0x480);
    WriteHalf((volatile u16 *)0x04000000, 0x40);
    DisplayScroll_BuildHblankWordTable((void *)0x06007800);
    DisplayScroll_BuildHblankWordTable(tmp4);
    tmp = 0x2844;
    Graphics_ClearCharacterBlockAndPalette(0);
    Graphics_ClearCharacterBlockAndPalette(1);
    WriteHalf((volatile u16 *)0x0400000c, 0x1f8a);
    WriteHalf((volatile u16 *)0x0400000e, 0xf83);
    WriteHalf((volatile u16 *)0x04000000, 0x1c40);
    WriteHalf((volatile u16 *)0x04000050, tmp);
    DisplayScroll_InitObjectTable();
    WaitFrames(300);
    slide = 0;
    resource = DisplayScroll_SlideResources;
    if (slide <= 32) {
        do {
            Graphics_LoadCharacterBlockAndPalette(*resource, 1 ^ (slide & 1));
            alpha = 1;
            base = 0xf00;
            while (alpha <= 16) {
                s32 tmp3;
                tmp3 = 0 != (slide & 1);
                if (tmp3)
                    WriteHalf((volatile u16 *)0x04000052, (alpha << 8) | (16 - alpha));
                else
                    WriteHalf((volatile u16 *)0x04000052, base | alpha);
                WaitFrames(4);
                base -= 256;
                ++alpha;
            }
            WaitFrames(267);
            resource += 1;
            slide++;
        } while (slide <= 32);
    }
    WriteHalf((volatile u16 *)0x04000050, 0);
    WriteHalf((volatile u16 *)0x04000000, 0x1040);
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    gOamCopyEnabled = 1;
    return 0;
}

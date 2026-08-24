#include "types.h"

#define PlayWorkspaceCueAndClearPaletteZero Func_0200004c
#define Audio_PlayCue Func_020014c2
#define PALETTE ((volatile u16 *)0x05000000)

extern u8 *Data_03001ebc;
extern void Audio_PlayCue(s32 cue);

void PlayWorkspaceCueAndClearPaletteZero(void)
{
    Audio_PlayCue(*(s16 *)(Data_03001ebc + 364));
    do {
        u16 color = PALETTE == 0;
        register volatile u16 *palette = PALETTE;

        palette[color] = color;
    } while (0);
}

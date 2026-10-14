#include "types.h"

extern u8 Data_0200308c;

void WaitFrames(s32 frames);

static __inline__ void Blend_WriteAlpha(s32 value)
{
    /* The s32 parameter keeps each constant a word built with shifts; a u16 store loads it from the literal pool. */
    *(volatile u16 *)0x04000052 = value;
}

// A statement macro: every write of these fades matches through the do/while (0) shape.
#define BLEND_SET_ALPHA(value) do { Blend_WriteAlpha(value); } while (0)

// Fades the second target out and the first in over 16 steps of four frames.
void Func_02000510(void)
{
    s32 i;

    BLEND_SET_ALPHA(0x1000);
    Data_0200308c = 0;
    i = 0;
loop:
    BLEND_SET_ALPHA(((16 - i) << 8) | i);
    WaitFrames(4);
    i++;
    if (i < 16)
        goto loop;
    BLEND_SET_ALPHA(16);
    Data_0200308c = 1;
}

// The reverse fade, in steps of three frames.
void Func_02000550(void)
{
    s32 i;

    BLEND_SET_ALPHA(16);
    Data_0200308c = 0;
    i = 0;
loop:
    BLEND_SET_ALPHA((i << 8) | (16 - i));
    WaitFrames(3);
    i++;
    if (i < 16)
        goto loop;
    BLEND_SET_ALPHA(0x1000);
}

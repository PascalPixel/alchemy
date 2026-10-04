/* Trial 2026-10-04: this draft compiles to the listing exactly (no halfword
   differs outside relocated words) with the TLA options plus -mtune=arm9tdmi,
   which turns on GCC's Thumb load scheduling; with the approved options the
   scheduler places one load or its first use later. Evidence for a K3
   question, not a routing: the build never uses that option. */
#include "TYPES.H"
#include "FIXED_MATH.H"

extern u8 gBlendFramesLeft;
extern volatile u8 gBlendTargetLevel;
extern u8 gBlendStartLevel;
extern volatile u8 gBlendDuration;
extern u8 gBlendBrighten;
extern u16 gBlendLayers;

void Blend_SetBrightenTarget0(s32 duration)
{
    gBlendBrighten = 1;
    gBlendLayers = 0x3e;
    gBlendStartLevel = gBlendTargetLevel;
    gBlendTargetLevel = 0;
    gBlendDuration = duration;
    gBlendFramesLeft = gBlendDuration;
}

#include "TYPES.H"

void UiWork_ProcessRenderChannels(void);
s32 UiWork_ProcessDirectWork(void);
s32 UiWork_AnimateSpriteSlots(void);

void UiWork_ProcessAll(void)
{
    UiWork_ProcessDirectWork();
    UiWork_ProcessRenderChannels();
    UiWork_AnimateSpriteSlots();
}

#include "TYPES.H"

void UiWork_ProcessDirectWork(void);
void UiWork_ProcessRenderChannels(void);
void UiWork_AnimateSpriteSlots(void);

void UiWork_ProcessAll(void)
{
    UiWork_ProcessDirectWork();
    UiWork_ProcessRenderChannels();
    UiWork_AnimateSpriteSlots();
}

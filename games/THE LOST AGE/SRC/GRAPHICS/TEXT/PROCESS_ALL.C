#include "PROTO.H"
#include "TYPES.H"
#include "RAM_BUFFER.H"


void UiWork_ProcessAll(void)
{
    UiWork_ProcessDirectWork();
    UiWork_ProcessRenderChannels();
    UiWork_AnimateSpriteSlots();
}

void UiWork_ProcessAllOrChannels(void)
{
    if (Ram_HeapSlots->unknown_e4[0] != 0) {
        UiWork_ProcessDirectWork();
        UiWork_ProcessRenderChannels();
        UiWork_AnimateSpriteSlots();
    } else {
        UiWork_ProcessRenderChannels();
    }
}

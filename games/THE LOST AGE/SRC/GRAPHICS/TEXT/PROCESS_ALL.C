#include "TYPES.H"
#include "RAM_BUFFER.H"

void UiWork_ProcessDirectWork(void);
void UiWork_ProcessRenderChannels(void);
void UiWork_AnimateSpriteSlots(void);

void UiWork_ProcessAll(void)
{
    UiWork_ProcessDirectWork();
    UiWork_ProcessRenderChannels();
    UiWork_AnimateSpriteSlots();
}

/* Runs every UI work while the work in heap slot 57 is up, else only the
   text channels. */
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

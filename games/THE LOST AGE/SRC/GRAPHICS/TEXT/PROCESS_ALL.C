#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
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

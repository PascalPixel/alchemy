#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"


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

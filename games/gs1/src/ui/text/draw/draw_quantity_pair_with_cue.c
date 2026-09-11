#include "types.h"
#include "scene.h"
#include "abi/ui/text/draw/draw_quantity_pair_with_cue.h"

void Audio_PlayCue(s32);
void UiText_DrawQuantity(s32, s32);
void UiText_DrawMessage(void *, s32);
extern u8 Value_0000001e;

void UiText_DrawQuantityPairWithCue(s32 first, s32 second)
{
    Ui_Run();
    Audio_PlayCue(0x53);
    UiText_DrawQuantity(first, 1);
    UiText_DrawQuantity(second, 4);
    UiText_DrawMessage(&Value_0000001e, 3);
}

#include "types.h"

void Func_08077088(void);
void Audio_PlayCue(s32);
void UiText_DrawQuantity(s32, s32);
void UiText_DrawMessage(void *, s32);
extern u8 Value_0000001e;

void Func_08091d94(s32 first, s32 second)
{
    Func_08077088();
    Audio_PlayCue(0x53);
    UiText_DrawQuantity(first, 1);
    UiText_DrawQuantity(second, 4);
    UiText_DrawMessage(&Value_0000001e, 3);
}

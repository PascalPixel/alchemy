#include "TYPES.H"
#include "SCENE.H"

void Audio_PlayCue(s32);
void UiWork_PushValueSlotFar(s32, s32);
void UiText_ShowPositionedMessageAndWaitFar(void *, s32);
extern u8 Value_0000001e;

void UiText_DrawQuantityPairWithCue(s32 first, s32 second)
{
    OwnerAction_AddFar();
    Audio_PlayCue(0x53);
    UiWork_PushValueSlotFar(first, 1);
    UiWork_PushValueSlotFar(second, 4);
    UiText_ShowPositionedMessageAndWaitFar(&Value_0000001e, 3);
}

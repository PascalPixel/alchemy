#include "TYPES.H"
#include "SCENE.H"
void OwnerAction_AddFar(void);

void Audio_PlayCue(s32);
void UiText_DrawQuantity(s32, s32);
void UiText_ShowPositionedMessageAndWaitFar(void *, s32);
extern u8 MsgAbilityLearned;

void UiText_DrawQuantityPairWithCue(s32 first, s32 second)
{
    OwnerAction_AddFar();
    Audio_PlayCue(0x53);
    UiText_DrawQuantity(first, 1);
    UiText_DrawQuantity(second, 4);
    UiText_ShowPositionedMessageAndWaitFar(&MsgAbilityLearned, 3);
}

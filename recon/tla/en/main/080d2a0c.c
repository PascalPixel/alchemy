/*
 * Draft: UiText_DrawQuantityPairWithCue does not yet match; ported from its ☀️ twin.
 * Differing ranges: identical
 * Links as its recon/tla/raw listing.
 */
#include "TYPES.H"
#include "SCENE.H"
void OwnerAction_AddFar(void);

void Audio_PlayCue(s32);
void UiWork_PushValueSlotFar(s32, s32);
void UiText_ShowPositionedMessageAndWaitFar(void *, s32);
extern u8 MsgAbilityLearned;

void UiText_DrawQuantityPairWithCue(s32 first, s32 second)
{
    OwnerAction_AddFar();
    Audio_PlayCue(0x53);
    UiWork_PushValueSlotFar(first, 1);
    UiWork_PushValueSlotFar(second, 4);
    UiText_ShowPositionedMessageAndWaitFar(&MsgAbilityLearned, 3);
}

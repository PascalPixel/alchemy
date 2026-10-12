#include "types.h"
#include "party_state.h"

extern char MsgAbilityWoreOff;
extern char MsgItemWoreOff;

void Func_080cb82c(void);
void Func_080cb8a4(void);
void UiText_DrawQuantity(s32 value, s32 slot);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 flags);

void FieldEvent_ShowStatusMessage(void)
{
    gPartyState.avoid_distance = 0;
    Func_080cb82c();
    if (gPartyState.avoid_by_item == 0) {
        UiText_DrawQuantity(150, 4);
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgAbilityWoreOff, 1);
    } else {
        UiText_DrawQuantity(236, 2);
        UiText_ShowPositionedMessageAndWaitFar((s32)&MsgItemWoreOff, 1);
    }
    Func_080cb8a4();
}

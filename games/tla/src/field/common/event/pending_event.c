#include "types.h"
#include "event_work.h"
#include "text/msg_ids.h"

s32 GameFlag_Test(s32 flag);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 position);


s32 EventRuntime_RunPendingEvent(struct PendingEvent *event, s32 mode, s32 target)
{
    if (event != NULL) {
        if (event->action != 0) {
            if (event->action < 0x10000) {
                EventRuntime_Begin();
                EventRuntime_SetMessage(event->action);
                EventRuntime_ShowMessageAndWait(target, 0);
                EventRuntime_End();
            } else {
                ((void (*)(s32, s32))event->action)(mode, target);
            }
        }
        if (GameFlag_Test(0x142)) {
            EventRuntime_Begin();
            UiText_ShowPositionedMessageAndWaitFar((s32)MsgNothingHappens, 1);
            EventRuntime_End();
        }
    }
    return 0;
}

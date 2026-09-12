#include "types.h"
#include "scene.h"
#include "runtime_interfaces.h"
#include "gs1_edition.h"
#include "global_cells.h"

/* save/state/confirm_and_process_selected_slot.c */
/* save/state/state_confirm_and_process_selected_slot.c */
/* save/state/confirm_and_process_selected_slot.c */
extern u8 Value_00000014;
extern u8 Value_00000017;

s32 UiWork_IsComplete(void);
void WaitFrames(s32);

void UiWork_FinalizePendingCore(void);
void Audio_PlayCue(s32);

s32 SaveState_ConfirmAndProcessSelectedSlot(void)
{
    s32 result;

    State_SetMode((s32)&Value_00000014, 8, 12, 2);
    while (UiWork_IsComplete() == 0) {
        WaitFrames(1);
    }
    if (State_SetMode2(1, 0, 0, 1)) {
        UiWork_FinalizePendingCore();
    } else {
        UiWork_FinalizePendingCore();
        Audio_PlayCue(85);
        result = FunctionHead_0801faa8();
        if (result >= 0) {
            State_Apply((s32)&Value_00000017, 1);
        }
    }
    return result;
}

// near miss: differ 23 (listing 40 instructions, draft 39); the wait loop: the native loads gInput's address and the mask inside the loop; the draft hoists both
#include "../../../../games/tla/src/field/overlay_670/ov670.h"
#include "input.h"

extern const u8 Data_02005cd0[];

void Func_02005a54(struct Ov670Object *object, s32 x, s32 y, s32 z);
void Func_02005a5c(struct Ov670Object *object);
void Event_StartScreenTransition(void);
void Func_02003a08(s32 x, s32 z);

void Func_02003c14(void)
{
    u32 i;

    Func_02005a9c();
    Func_02005c3c(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Scheduler_AddOrUpdateCallback(Func_02002d64, 0x480);
    Battle_WaitMode0(20);
    for (i = 0; i <= 159; i++) {
        Battle_WaitMode0(1);
        do { } while (0);
        if (gInput.held & 11)
            break;
    }
    Scheduler_RemoveCallbackFar(Func_02002d64);
    ((struct Ov670EventWork *)Ram_HeapSlots->event_work)->field_1b4 = 32;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02005bc4(11);
}

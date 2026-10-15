#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "party_state.h"

void Event_NoOp(void)
{
}

#if !defined(TLA_EDITION_ES) && !defined(TLA_EDITION_IT)
void Audio_PlayCue(s32 cue);

void Func_080d2c9c(void)
{
    u8 *work = Ram_HeapSlots->event_work;

    if (work != 0) {
        s16 cue = *(s16 *)(work + 0xcb8);

        if (cue != -1)
            Audio_PlayCue(cue);
    }
}

void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);

void GameFlag_ClearBit(s32 flag);

s32 Party_FindOwnerWithActionFar(s32 action);

struct EventWork15a {
    u8 unknown_000[0x15a];
    u16 flag;
};

void Func_080d2cc4(s32 message)
{
    EventRuntime_Begin();
    EventRuntime_ResolvePendingActions(0);
    UiText_ShowPositionedMessageAndWaitFar(message, 1);
    GameFlag_ClearBit(0x142);
    EventRuntime_End();
    if (Party_FindOwnerWithActionFar(136) != -1)
        ((struct EventWork15a *)Ram_HeapSlots->event_work)->flag = 1;
}

struct CueObject {
    u8 unknown_00[8];
    s32 x;
    s32 base;
    s32 top;
};

struct EventWork1cc {
    u8 unknown_000[0x1cc];
    struct CueObject *focus;
};

#define ABS(x) ((x) >= 0 ? (x) : -(x))

void Func_080d2d08(struct CueObject *object, s32 cue)
{
    struct CueObject *focus = ((struct EventWork1cc *)Ram_HeapSlots->event_work)->focus;

    if (object == 0 || focus == 0)
        return;
    if (ABS(focus->x - object->x) > 0x980000)
        return;
    if (ABS((focus->top - focus->base) - (object->top - object->base)) > 0x700000)
        return;
    Audio_PlayCue(cue);
}

void Func_080d2d70(void)
{
    gPartyState.counter_274++;
}
#endif

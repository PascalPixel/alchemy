#include "OWNER_STATE.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"

/* The event work's screen effect and its delay: ☀️ keeps them at 0x1c0 and
   0x1c8. */
struct FieldEffectState {
    u8 padding0[0x1AC];
    s32 effect;
    u8 padding1B0[4];
    s32 delay;
};

struct FieldObject {
    u8 padding0[0x34];
    u16 saved_x;
    u16 saved_y;
    s16 x;
    u16 y;
};

s32 Party_ListActiveOwnersFar(s16 *);
void Party_AdjustSixDigitCounterAFar(s32);
void Owner_RecalculateRatiosFar(s32);
void Event_ClearStatus1c6Far(void);
void Event_WaitValue1c8FramesFar(void);
void Audio_PlayCue(s32);
void AudioCommand_WaitForStateByteClear(void);
void Event_SetStatus1c6Far(void);
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
void Func_080c8398(void);
void Func_080c83a0(void);
void Event_DelayEffectFramesFar(void);
#endif

/* ☀️'s: pay for the room, restore every active member and play the night's
   fade and tune. */
void Inn_PlaySleep(s32 room_price)
{
    s16 objects[8];
    s32 count;
    s32 index;
    struct FieldObject *object;
    struct FieldEffectState *state;

    count = Party_ListActiveOwnersFar(objects);
    Party_AdjustSixDigitCounterAFar(-room_price);

    for (index = 0; index < count; index++) {
        object = Owner_GetState(objects[index]);
        if (object->x != 0) {
            object->x = object->saved_x;
            object->y = object->saved_y;
            Owner_RecalculateRatiosFar(objects[index]);
        }
    }

    state = Ram_HeapSlots->event_work;
    state->effect = 0x209;
    state->delay = 60;
    WaitFrames(20);
    /* Spanish/Italian use the native table's set, clear and delay entries. */
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
    Func_080c83a0();
#else
    Event_ClearStatus1c6Far();
#endif
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
    Event_DelayEffectFramesFar();
#else
    Event_WaitValue1c8FramesFar();
#endif
    Audio_PlayCue(86);
    AudioCommand_WaitForStateByteClear();
    WaitFrames(10);
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
    Func_080c8398();
#else
    Event_SetStatus1c6Far();
#endif
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
    Event_DelayEffectFramesFar();
#else
    Event_WaitValue1c8FramesFar();
#endif
    WaitFrames(30);
    ((struct FieldEffectState *)Ram_HeapSlots->event_work)->delay = 16;
}

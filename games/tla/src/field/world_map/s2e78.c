#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "flag_call.h"
#include "text/msg_ids.h"

struct EventCounters {
    u8 unknown_000[0x1c4];
    u16 count;
};

struct ObjectRuntime *Object_GetById(s32 id);
void Battle_WaitMode0(s32 frames);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004c88(s32 actor, s32 x, s32 z);
void Func_02004cd8(s32 message);
void Func_02004ce8(s32 actor, s32 flags, s32 frames);
s32 Func_02004d08(s32 actor, s32 value, s32 frames);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004db8(s32 value);

static __inline__ void Func_02004d08_(s32 actor, s32 value, s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02004d08(actor, value, frames);
}

void Func_02002e78(void)
{
    s32 scale;
    s32 seen = Flag_IsSet(0x8de);

    if (seen == 0) {
        Func_02004c20();
        Func_02004db8(0);
        scale = Object_GetById(gPartyState.current_owner)->scale_x;
        Object_GetById(gPartyState.current_owner)->scale_x = 0;
        ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Func_02004da8();
        Func_02004cd8((s32)MsgTakeTheTiller);
        Battle_WaitMode0(40);
        Func_02004ce8(7, 0, 10);
        Func_02004d08_(8, 256, 30);
        Func_02004ce8(4, 0, 10);
        Func_02004ce8(7, 0, 10);
        Func_02004d08_(8, 257, 30);
        Func_02004ce8(6, 0, 10);
        Func_02004ce8(9, 0, 10);
        Func_02004d08_(8, 264, 30);
        Func_02004ce8(5, 0, 10);
        /* FAKEMATCH: called as returning a value, which the native's argument order shows (r1 before r0). */
        ((s32 (*)(s32, s32))UiText_OpenMessageAtObject)(7, 0);
        if (Inventory_PromptAndSetObjectMode(4, 0) == 0) {
            Func_02004d08_(8, 265, 30);
            Func_02004ce8(9, 0, 10);
            ((struct EventCounters *)Ram_HeapSlots->event_work)->count++;
        } else {
            Func_02004d08_(8, 265, 30);
            ((struct EventCounters *)Ram_HeapSlots->event_work)->count++;
            Func_02004ce8(9, 0, 10);
        }
        Func_02004d08_(8, 256, 30);
        Func_02004ce8(7, 0, 10);
        Func_02004ce8(9, 0, 10);
        Func_02004d08_(8, 257, 30);
        Func_02004ce8(6, 0, 10);
        Func_02004ce8(9, 0, 10);
        Func_02004d08_(8, 256, 30);
        Func_02004ce8(5, 0, 10);
        Func_02004ce8(9, 0, 10);
        Func_02004ce8(6, 0, 10);
        Func_02004d08_(8, 264, 30);
        Func_02004ce8(5, 0, 10);
        Func_02004d08_(8, 258, 30);
        Func_02004ce8(6, 0, 10);
        Func_02004ce8(5, 0, 10);
        Func_02004ce8(9, 0, 10);
        Func_02004d08_(8, 257, 30);
        Func_02004ce8(7, 0, 10);
        Func_02004d08_(8, 264, 30);
        Func_02004ce8(9, 0, 10);
        Func_02004ce8(6, 0, 10);
        Func_02004ce8(5, 0, 10);
        Func_02004ce8(7, 0, 10);
        Func_02004ce8(9, 0, 10);
        Func_02004c88(gPartyState.current_owner, 0, 0);
        gPartyState.render_mode = 7;
        gPartyState.owner_override = 1;
        Func_02004db0();
        Flag_Set(0x8de);
        Func_02004c88(7, 0, 0);
        Func_02004c88(6, 0, 0);
        Func_02004c88(9, 0, 0);
        Func_02004c88(5, 0, 0);
        Object_GetById(gPartyState.current_owner)->scale_x = scale;
        Func_02004c28();
    }
}

/* 2026-10-03 finite trial record (extent/differing bytes, relocations unlinked):
 * Four ordinary forms: baseline168/6, pointer-local168/4, early owner168/9, long-lived pointer168/130. Four scheduling forms: retained pointer handoff168/0; scratch r2/r1 and memory boundaries168/6.
 * No whole-edition linked proof is claimed by these object measurements. */
/* Event begin trial: ordinary C through maintained EventWork and PartyState.
 * Full linked edition proof remains pending. */
#include "RAM_BUFFER.H"
#include "EVENTWRK.H"
#include "PARTY_STATE.H"
#include "CALLBACK_SCHEDULER.H"
#include "GAME_FLAGS.H"

void Func_08038208(void);
void Func_080d2260(void);
void Func_080cdec8(void);
void Func_080d21f4(void);

void Func_080d22a8(void)
{
    struct EventWork *state = Ram_HeapSlots->event_work;
    Func_08038208();
    Func_080d2260();
    if (state->action_counter != 0)
        Func_080cdec8();
    state->unknown_cb0 = 0;
    state->unknown_cb2 = 0;
    state->unknown_cb4 = 0;
    state->delay = 16;
    state->wait_mode = 0;
    state->message_actor = 0xffff;
    state->choice = -1;
    state->message_state = -1;
    Scheduler_AddOrUpdateCallback((s32)Func_080d21f4, 0x480);
    GameFlag_ClearBit(0x132);
    { struct PartyState *party = &gPartyState;
    /* FAKEMATCH: four ordinary forms loaded the party pointer after constructing the destination offset; this used pointer handoff preserves the native 168-byte order. */
    __asm__ volatile("" : "+r"(party));
    state->current_owner = party->current_owner;
    state->object_status = 0;
    }
}

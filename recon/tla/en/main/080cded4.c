/* Pending-action dispatcher trial, 2026-10-02.
 * Plain typed C: EN score 0, complete 136-byte owner including literal pool.
 * Fresh six-edition objects match all 120 non-relocation bytes; gPartyState
 * pool identity matches all six. EN resolves all three BLs exactly.
 * Other five editions lack the executor physical name in the current main
 * namespace. This is native raw identity evidence, not localized link readiness.
 * Field widths come from the current owner; these private views do not define
 * storage or establish the complete work-block extent. No steering device. */
#include "TYPES.H"
#include "RAM_BUFFER.H"

struct PendingEventWork {
    u8 unknown_000[0xca2];
    s16 action_counter;
    s8 pending_first;
    u8 unknown_ca5[7];
    s8 pending_second;
};
struct PendingPartyState {
    u8 unknown_000[0x214];
    s32 current_owner;
    u8 unknown_218[0x54];
    s8 pending_third;
    u8 unknown_26d[9];
    s16 owner_override;
};
extern struct PendingPartyState gPartyState;
s32 Func_080ce574(u32 action);

void Func_080cded4(s32 skip)
{
    struct PendingEventWork *state = Ram_HeapSlots->event_work;

    state->action_counter = 0;
    if (state->pending_first != 0 && !(skip & 1))
        Func_080ce574(0x2090);
    if (state->pending_second != 0 && !(skip & 2))
        Func_080ce574(0x209b);
    if (gPartyState.pending_third != 0 && !(skip & 4))
        Func_080ce574(0x208b);
}

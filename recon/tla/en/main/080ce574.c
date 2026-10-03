/* Packed ability-effect executor trial, 2026-10-02.
 * Plain typed C: EN score 0, complete 168-byte owner (no literal pool).
 * Fresh six-edition objects match all 120 non-relocation bytes. EN resolves
 * all twelve BLs exactly; current other-edition symbolic bindings retain
 * 6/5/9/5/9 missing or wrong operands in JA/DE/ES/FR/IT.
 * Event records are 12 bytes; action-record byte 6 selects their kind.
 * Correcting the first packed event filter to 0x30000005 removed the initial
 * single-operand near miss. No steering device. Draft only; raw native
 * identity is distinct from recursive linked-source readiness and credit. */
#include "TYPES.H"

s32 EventRuntime_GetControlledOwner(void);

struct PendingAction {
    u8 unknown_00[6];
    u8 event_kind;
    u8 unknown_07[9];
};
struct PendingEventRecord {
    u32 key;
    u16 flags;
    u16 condition;
    s32 action;
};
const struct PendingAction *BattleAction_Get(s32 action);
s32 Func_080ce31c(s32 kind);
struct PendingEventRecord *Func_080ce458(u32 filter, s32 kind, s32 selector);
void Func_080dc410(s32 action, s32 flags);
void Func_080dc62c(s32 actor, s32 target);
s32 Func_080ceafc(struct PendingEventRecord *event, s32 mode, s32 target);
void FieldEvent_RunTypeHandler(void);
void Func_080dc7cc(void);
void Func_080dc7e8(void);

s32 EventRuntime_ExecutePackedAction(u32 packed)
{
    s32 action, mode, kind, selector, target;
    struct PendingEventRecord *first, *second;

    action = packed & 0x3ff;
    mode = (packed >> 10) & 15;
    kind = BattleAction_Get(action)->event_kind;
    selector = Func_080ce31c(kind);
    first = Func_080ce458(0x30000005, kind, selector);
    second = Func_080ce458(0x20000005, kind, selector);
    target = -1;
    if (first != NULL || second != NULL) {
        if (selector != -1 && (selector & 0x100))
            target = selector & 255;
    }
    Func_080dc410(action, 0);
    Func_080dc62c(EventRuntime_GetControlledOwner(), target);
    Func_080ceafc(first, mode, target);
    FieldEvent_RunTypeHandler();
    Func_080dc7cc();
    Func_080ceafc(second, mode, target);
    Func_080dc7e8();
    return 0;
}

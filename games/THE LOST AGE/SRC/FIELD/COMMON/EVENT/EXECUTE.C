#include "TYPES.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_UNIT.H"
#include "EVENTWRK.H"

s32 Func_080ce31c(s32 kind);
struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector);
void Func_080dc410(s32 action, s32 flags);
void Func_080dc62c(s32 actor, s32 target);
void FieldEvent_RunTypeHandler(void);
void Func_080dc7cc(void);
void Func_080dc7e8(void);

/* Runs a packed action (id in the low ten bits, mode above it) with the
   pending events of its kind before and after the type handler. */
s32 EventRuntime_ExecutePackedAction(u32 packed)
{
    s32 action, mode, kind, selector, target;
    struct PendingEvent *first, *second;

    action = packed & 0x3ff;
    mode = (packed >> 10) & 15;
    kind = BattleAction_Get(action)->unknown_04[2];
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
    EventRuntime_RunPendingEvent(first, mode, target);
    FieldEvent_RunTypeHandler();
    Func_080dc7cc();
    EventRuntime_RunPendingEvent(second, mode, target);
    Func_080dc7e8();
    return 0;
}

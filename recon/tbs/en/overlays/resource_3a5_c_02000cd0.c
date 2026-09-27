/* NONMATCHING: 304 of 304 bytes, 6 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: every instruction matches except the order
 * of the first three literal loads and their pool entries: the reference
 * loads row offset 0x232 before event-work and game-state addresses.
 * 2026-09-26: a link-symbol offset combined into indexed ldrsh and shrank to
 * 300 bytes; reading the numerator before event work retained 304 bytes but
 * left 10 aligned edits. The original six-halfword draft is retained with
 * verified import bindings.
 * 2026-09-27 H1: exact RAMAKAN_SABAKU/TRAVEL_DUST increments +0x232;
 * ENTER_AREA initializes +0x22c to 600, and exact Field_ProcessStep uses
 * these as step counter/limit, with mode and damage between them. This is
 * not an HP numerator despite the historical candidate-unit name.
 * Transferring that signed-halfword record, retaining the cached pointer,
 * is byte-identical to the old candidate: 304/304, six differing halfwords.
 * Whole extent [02000cd0,02000e00), return at 0dde, eight pool words at
 * 0de0..0dfc. Everything from 0cd8 through the return remains exact.
 * CSE insn 12 loads the explicit state pointer before event insns 15/17;
 * offset 562 enters only in insn 22. Next supported boundary is direct
 * typed-global access, as Field_ProcessStep uses, not a literal-order sweep.
 * H2 removed the cached state pointer and used the direct global expression
 * Engine_MathDivide(Data_02000240_t.steps * 100, Data_02000240_t.limit).
 * Result 304/304, nine differing halfwords / seven aligned edits: entry
 * loads event address, state address, then 562; reference wants 562 first.
 * Body 0cd8..0dde still exact, but all three initial loads/pool words rotate.
 * Rejected; H1 retained. STOP after one model and one evidence-led follow-up.
 * Future admission must emit the reference offset/event/state entry order
 * naturally; changing struct/array access alone has been falsified. No credit.
 * Transfer check: TITLE.C's inline argument boundary was applied as
 * ReadStepCount(s32 offset, struct TravelState *state), returning
 * *(s16 *)((u8 *)state + offset), called with (0x232, state).
 * Complete output shrinks to 300/304 bytes, 147 differing halfwords and
 * 42 aligned edits. The addition/zero-load pair folds into indexed ldrsh;
 * offset still follows event/state. It does not admit the required entry,
 * and the shifted pool is not a gain. Keep 304-byte body; stop this axis.
 * H3 (2026-09-27): form a typed pointer to steps before loading event work.
 * Prediction: the independent 0x232 address producer loads before the event
 * and state bases. Complete result 304/304, 21 differing halfwords / 19
 * aligned edits. The compiler instead adds state base and offset before
 * loading event work, and later event stores change scratch-register order.
 * The eight-word pool retains its extent. This pointer boundary fails;
 * the six-halfword body is restored below. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

struct TravelState {
    u8 unknown_000[0x22c];
    s16 limit;
    s16 mode;
    s16 damage;
    s16 steps;
};

extern struct TravelState Data_02000240_t;

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

void Local_02000cd0(void)
{
    struct EventWork *event;
    s32 percent;

    {
        struct TravelState *state = &Data_02000240_t;

        event = gEventWork;
        percent = Engine_MathDivide(state->steps * 100, state->limit);
    }
    if (Engine_GameFlagIsSet(0x201)) {
        return;
    }
    if (Value1(Engine_GameFlagIsSet, 0x302) && percent <= 74) {
        Call1(Engine_GameFlagClear, 0x302);
        Call1(Engine_GameFlagClear, 0x303);
        Call1(Engine_GameFlagClear, 0x304);
        Call1(Engine_GameFlagClear, 0x305);
    }
    if (Value1(Engine_GameFlagIsSet, 0x301) && percent <= 49) {
        Call1(Engine_GameFlagClear, 0x301);
        Call1(Engine_GameFlagClear, 0x303);
        Call1(Engine_GameFlagClear, 0x304);
        Call1(Engine_GameFlagClear, 0x305);
    }
    if (Value1(Engine_GameFlagIsSet, 0x300) && percent <= 24) {
        Call1(Engine_GameFlagClear, 0x300);
        Call1(Engine_GameFlagClear, 0x303);
        Call1(Engine_GameFlagClear, 0x304);
        Call1(Engine_GameFlagClear, 0x305);
    }
    if (!Value1(Engine_GameFlagIsSet, 0x300) && percent > 24) {
        Call1(Engine_GameFlagSet, 0x300);
        event->raised_trigger = 1;
    }
    if (!Value1(Engine_GameFlagIsSet, 0x301) && percent > 49) {
        Call1(Engine_GameFlagSet, 0x301);
        event->raised_trigger = 2;
    }
    if (!Value1(Engine_GameFlagIsSet, 0x302) && percent > 74) {
        Call1(Engine_GameFlagSet, 0x302);
        event->raised_trigger = 3;
    }
}

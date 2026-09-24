/* NONMATCHING: 304 of 304 bytes, 6 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: every instruction matches except the order of the first three literal loads: the reference loads the row offset 0x232 before the event-work and game-state addresses, which also reorders the pool (6 halfwords). The 0x232 load is a reload insn (luid after the base and event loads); the sched2 ready list ties on priority and dependents, so it is scheduled last. It needs to become an ordinary pseudo emitted before the base load. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;

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
        s16 *rows = Data_02000240_t.halves[0];

        event = gEventWork;
        percent = Engine_MathDivide(rows[281] * 100, rows[278]);
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

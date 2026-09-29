#include "TYPES.H"
#include "FIELD_EVENT.H"

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

void RamakanSabaku_RaiseQuarterTriggers(void)
{
    struct EventWork *event;
    s32 percent;

    { union GameStateRows *state = (union GameStateRows *)&gGameState; event = gEventWork; percent = Engine_MathDivide(100 * state->halves[281][0], state->halves[278][0]); }
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

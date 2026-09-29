#include "RAMAKAN.H"

/* The desert crossing in quarters: the percentage of the way the party has
   come clears the quarter flags it has fallen back behind and raises the
   trigger of each quarter it has newly passed. */
void RamakanSabaku_RaiseQuarterTriggers(void)
{
    struct EventWork *work = (struct EventWork *)gWork;
    u8 *state = (u8 *)&gGameState;
    s32 percent = *(s16 *)(state + 0x232) * 100 / *(s16 *)(state + 0x22c);

    if (GameFlag_IsSet(0x201) != 0)
        return;
    if (GameFlag_IsSet(0x302) != 0 && percent <= 74) {
        GameFlag_Clear(0x302);
        GameFlag_Clear(0x303);
        GameFlag_Clear(0x304);
        GameFlag_Clear(0x305);
    }
    if (GameFlag_IsSet(0x301) != 0 && percent <= 49) {
        GameFlag_Clear(0x301);
        GameFlag_Clear(0x303);
        GameFlag_Clear(0x304);
        GameFlag_Clear(0x305);
    }
    if (GameFlag_IsSet(0x300) != 0 && percent <= 24) {
        GameFlag_Clear(0x300);
        GameFlag_Clear(0x303);
        GameFlag_Clear(0x304);
        GameFlag_Clear(0x305);
    }
    if (GameFlag_IsSet(0x300) == 0 && percent > 24) {
        GameFlag_Set(0x300);
        work->raised_trigger = 1;
    }
    if (GameFlag_IsSet(0x301) == 0 && percent > 49) {
        GameFlag_Set(0x301);
        work->raised_trigger = 2;
    }
    if (GameFlag_IsSet(0x302) == 0 && percent > 74) {
        GameFlag_Set(0x302);
        work->raised_trigger = 3;
    }
}

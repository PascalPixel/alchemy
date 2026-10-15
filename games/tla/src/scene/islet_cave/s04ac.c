#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "event_work.h"

void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void Func_020000f4(void);
void Func_02000158(void);
void Func_02000178(void);
void Func_0200129c(s32 id, s32 x, s32 z);
void Func_020012bc(s32 id, s32 a);
void Func_020012ec(s32 a, s32 b);
void Func_020012fc(s32 a);
void Func_0200132c(void);
void Func_02001334(void);

void Func_020004ac(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;
    s32 effect = work->screen_effect;

    Func_02000178();
    Func_0200132c();
    WaitFrames(40);
    Func_02000158();
    Func_020012ec(0x10000, 1);
    Func_020012fc(16);
    WaitFrames(16);
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native loads the party state address before it builds the field offset (2 differ without it). */
    gPartyState.unknown_2c2 = 0;
    Func_020012bc(gPartyState.current_owner, 0);
    work->screen_effect = effect;
}

void Func_02000508(void)
{
    Func_020000f4();
    Func_02001334();
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native loads the party state address before it builds the field offset (one row late without it). */
    gPartyState.unknown_2c2 = 1;
    Battle_WaitMode0(40);
    Func_0200129c(gPartyState.current_owner, 232 << 16, 136 << 16);
    Func_020004ac();
}

void Func_02000544(void)
{
    Func_020000f4();
    Func_02001334();
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native loads the party state address before it builds the field offset (one row late without it). */
    gPartyState.unknown_2c2 = 1;
    Battle_WaitMode0(40);
    Func_0200129c(gPartyState.current_owner, 240 << 16, 480 << 16);
    Func_020004ac();
}

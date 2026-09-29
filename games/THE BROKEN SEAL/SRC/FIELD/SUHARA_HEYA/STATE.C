#include "SUHARA.H"

void State_ApplyCounter16cThenCall7b(void)
{
    u8 *state = (u8 *)gEventWork;
    s16 *cnt = (s16 *)(state + 0x16C);

    Engine_EventRequestExit(*cnt);
    Engine_AudioPlayCue(0x7B);
}

#include "SERIAL_RUNTIME.H"

#define BattleLink_ResetTransferState Func_0800651c

void BattleLink_ResetTransferState(void)
{
    struct SerialTransferState *state;
    volatile u16 *ime;
    u32 saved;
    u32 zero;

    /* FAKEMATCH: the do/while blocks and the second state assignment are
     * meaningless. The blocks keep the zero after the IME write; setting
     * state twice raises its allocation priority above ime so it gets r1. */
    do {
        state = &Data_02002220;
        ime = &Data_04000208;
    } while (0);
    do {
        saved = *ime;
        *ime = (u16)ime;
    } while (0);
    do {
        zero = 0;
        state = &Data_02002220;
    } while (0);
    state->status = 0x80;
    Data_02002080 = zero;
    Data_02002008 = zero;
    Data_020023ac = zero;
    state->peer_flags = zero;
    state->flags = zero;
    Data_02002238 = zero;
    *ime = saved;
}

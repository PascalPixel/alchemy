#include "SERIAL_RUNTIME.H"


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
        state = &gSerialTransfer;
        ime = &Data_04000208;
    } while (0);
    do {
        saved = *ime;
        *ime = (u16)ime;
    } while (0);
    do {
        zero = 0;
        state = &gSerialTransfer;
    } while (0);
    state->status = 0x80;
    gSerialSendSource = zero;
    gSerialSendSize = zero;
    gSerialReceiveDest = zero;
    state->peer_flags = zero;
    state->flags = zero;
    gSerialReceivedSize = zero;
    *ime = saved;
}

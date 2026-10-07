#include "TYPES.H"
#include "IO_REG.H"
#include "SERIAL_RUNTIME.H"

void BattleLink_ResetTransferState(void)
{
    struct SerialTransferState *state;
    volatile u16 *ime;
    u32 saved;
    u32 zero;

    state = &gSerialTransfer;
    ime = &REG_IME;
    /* FAKEMATCH: the one-pass block keeps the IME exchange before the zero and status constants. */
    do {
        saved = *ime;
        *ime = (u16)ime;
    } while (0);
    zero = 0;
    state = &gSerialTransfer;
    state->status = 0x80;
    * &state->peer_flags = zero;
    gSerialSendSource = zero;
    * &state->flags = zero;
    gSerialSendSize = zero;
    gSerialReceiveDest = zero;
    gSerialReceivedSize = zero;
    *ime = saved;
}

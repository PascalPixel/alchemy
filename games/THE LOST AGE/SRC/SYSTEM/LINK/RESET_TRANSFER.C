#include "TYPES.H"
#include "IO_REG.H"
#include "SERIAL_RUNTIME.H"

/* ⚓️ clears the two flag bytes between the send source and size, where The
   Broken Seal's inline SerialRuntime_ResetTransferState clears them last. */
void BattleLink_ResetTransferState(void)
{
    struct SerialTransferState *state;
    volatile u16 *ime;
    u32 saved;
    u32 zero;

    /* FAKEMATCH: the blocks keep the IME exchange ahead of the zero and
       status constants, as in The Broken Seal's copy in SERIAL_RUNTIME.H. */
    do {
        state = &gSerialTransfer;
        ime = &REG_IME;
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
    /* FAKEMATCH: each flag byte keeps its place among the volatile stores. */
    *(volatile u8 *)&state->peer_flags = zero;
    gSerialSendSource = zero;
    /* FAKEMATCH: each flag byte keeps its place among the volatile stores. */
    *(volatile u8 *)&state->flags = zero;
    gSerialSendSize = zero;
    gSerialReceiveDest = zero;
    gSerialReceivedSize = zero;
    *ime = saved;
}

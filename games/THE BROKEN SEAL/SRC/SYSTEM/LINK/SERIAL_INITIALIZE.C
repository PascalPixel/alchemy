#include "SERIAL_RUNTIME.H"
#include "DMA.H"

void Runtime_SetIrqHandler(s32 irq, s32 vcount, InterruptHandler handler);
void SerialRuntime_HandleTransferInterrupt(void);
void BattleLink_ResetTransferState(void);

extern volatile u16 gSerialExchangeActive;

/* Starts the serial runtime: installs the serial and timer interrupt
 * handlers, resets the SIO registers into multiplayer mode, clears the
 * runtime record and lays out its send, incoming, ready and pending buffers,
 * enables the serial interrupt and the VBlank exchange, and clears the
 * transfer state. */
void SerialRuntime_Initialize(void)
{
    u32 interrupt_enable;
    /* FAKEMATCH: keep the fill word's address before its store. */
    volatile u32 zero;
    s32 index;
    volatile u16 *ime_reg;
    struct SerialRuntime *state;

    interrupt_enable = REG_IME;
    /* FAKEMATCH: retain the reference's IME pointer lifetime. */
    do { ime_reg = &REG_IME; } while (0);
    *ime_reg = (u32)ime_reg;
    Runtime_SetIrqHandler(7, 0, SerialRuntime_HandleTransferInterrupt);
    Runtime_SetIrqHandler(6, 0, SerialRuntime_HandleTransferInterrupt);
    REG_IME = 0;
    REG_IE &= 0xff3f;
    if ((REG_IF & 0x80) != 0)
        REG_IF = 0x80;
    if ((REG_IF & 0x40) != 0)
        REG_IF = 0x40;
    REG_RCNT = 0x8000;
    REG_RCNT = 0;
    REG_SIOCNT = 0x1000;
    REG_RCNT = 0;
    REG_SIOCNT = 0x2000;
    REG_SIOCNT16 = REG_SIOCNT16 | 0x4003;
    state = &gSerialRuntime;
    REG_IME = 1;
    zero = 0;
    Dma_Set(&zero, state, 0x85000058, (volatile u32 *)0x040000d4);
    state->send_index = -1;
    state->send_buffer[0] = (u16 *)(state->storage + 0);
    state->send_buffer[1] = (u16 *)(state->storage + 32);
    for (index = 0; index <= 1; index++) {
        state->incoming_buffer[index] = (u16 *)(state->storage + 64 + index * 96);
        state->ready_buffer[index] = (u16 *)(state->storage + 96 + index * 96);
        state->pending_buffer[index] = (u16 *)(state->storage + 128 + index * 96);
    }
    REG_IME = 0;
    REG_IE |= 0x80;
    REG_IME = 1;
    gSerialExchangeActive = 1;
    gLinkExchangeState = 0;
    gSerialSendSource = 0;
    gSerialSendSize = 0;
    gSerialReceiveDest = 0;
    gSerialReceivedSize = 0;
    BattleLink_ResetTransferState();
    REG_IME = interrupt_enable;
}

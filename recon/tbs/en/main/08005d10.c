/* Draft, not exact (2026-09-27): 350/350 bytes, 8 differing halfwords,
 * 4 aligned edits. Named independent serial globals and one gSerialRuntime
 * pointer fix the folded addresses and buffer setup (94 -> 48 halfwords).
 * A volatile DMA fill local fixes the mov r0,sp / str [r0] sequence (44).
 * Literal 0x8000, not Value_00008000, gives the short-range pool before DMA
 * (7); grouping the two enable stores fixes their address-load order (8,
 * but 4 aligned edits rather than 5). Everything except the last zero load
 * and its pool ordering now agrees.
 * Residual: the byte-flag zero loads into r4 before the IE update, whereas
 * the reference loads it between the IME and handler-enable stores.
 * sched2 shows an independent HImode zero originating at the earlier IME
 * clear, retained for the byte store after CSE substitutes the word zero
 * into the clear itself. Wrapping the flag store, making it volatile,
 * sharing an explicit u32 clear local, and a typed IME member store all
 * reproduce 8 halfwords/4 edits. These axes are stopped; inspect the zero's
 * RTL ancestry before any further scheduling experiment. The obsolete
 * Value_ cast was also tested as u16 and did not change the 44-halfword pool.
 * The collector's s32 inline IME setter removes that HImode zero completely
 * (342 bytes/17 edits, flag clear reuses r0). An explicit u8 zero restores
 * the same early load (350/8/4); a linked Data_00000000 tail gives 350/15/10
 * and uses r3 after the handler store. The setter confirms the ancestry,
 * but neither follow-up satisfies the final zero's placement and register.
 */
#include "SERIAL_RUNTIME.H"
#include "DMA.H"

void Runtime_SetIrqHandler(s32 irq, s32 vcount, InterruptHandler handler);
void SerialRuntime_HandleTransferInterrupt(void);
void BattleLink_ResetTransferState(void);

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
    /* FAKEMATCH: retain the reference's enable-store ordering. */
    do {
        REG_IME = 1;
        *(volatile u16 *)ADDR_03001CB0 = 1;
    } while (0);
    *(u8 *)0x020023a0 = 0;
    gSerialSendSource = 0;
    gSerialSendSize = 0;
    gSerialReceiveDest = 0;
    gSerialReceivedSize = 0;
    BattleLink_ResetTransferState();
    REG_IME = interrupt_enable;
}

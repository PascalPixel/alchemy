/* The ordinary three-statement remover retains the old standalone result:
 * 42/44 bytes including the pool, score 100; only the final zero halfword
 * at +0x2a is absent. The empty blocks and temporary casts were redundant.
 * In an ordinary SIO_INTR translation unit followed by the exact status
 * wait, the compiler aligns that function to +44 and emits the missing
 * halfword naturally. Complete 96-byte pair and relocations match the raw
 * owners under all six edition routes; all six own-ROM spans also agree.
 * No explicit padding or steering is needed. Adoption is pending.
 */
#include "SERIAL_RUNTIME.H"

void SerialRuntime_RemoveIrqHandlers(void)
{
    gSerialExchangeActive = 0;
    Runtime_SetIrqHandler(7, 0, 0);
    Runtime_SetIrqHandler(6, 0, 0);
}

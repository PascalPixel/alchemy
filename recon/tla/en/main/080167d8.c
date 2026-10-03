/* Immutable refresh against the current English ELF: complete candidate and
 * reference are 52 bytes including both pool words. Score 0: exact.
 * Reference: recon/tla/raw/080167d8.s. The local volatile gLinkStatus
 * declaration remains pending its cross-owner shared declaration closure.
 */
#include "SERIAL_RUNTIME.H"

extern volatile u16 gLinkStatus;

u32 SerialRuntime_WaitForStatusMask(s32 mask)
{
    if ((mask & gLinkStatus) != mask) {
        do {
            WaitFrames(1);
        } while ((mask & gLinkStatus) != mask);
    }
    /* SIOCNT bits 4-5: this unit's multiplayer ID. */
    return (REG_SIOCNT << 0x1A) >> 0x1E;
}

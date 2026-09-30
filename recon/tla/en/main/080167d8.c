/*
 * Draft: SerialRuntime_WaitForStatusMask does not yet match; 2 halfwords differ from ☀️'s C, first at +0x2e (data).
 * Links as recon/tla/raw/080167d8.s.
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

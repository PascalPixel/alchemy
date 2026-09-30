/*
 * Draft: SerialRuntime_FindLatestPeer does not yet match; the reference keeps
 * the best stamp in r0, the result in r5 and the index in r4, and steps the
 * stamp pointer by 6 bytes in r1.
 * Links as its recon/tla/raw listing.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"

/* Of the five peer records (three bytes each, a stamp every six bytes from
   0x1e), the active one for the given id with the highest stamp; returns its
   byte offset, or 15 when none is. */
s32 SerialRuntime_FindLatestPeer(s32 id)
{
    u8 *work;
    u8 *peer;
    u16 *stamp;
    u32 best;
    s32 found;
    u32 i;

    work = Ram_HeapSlots->serial_work;
    stamp = (u16 *)(work + 0x1e);
    best = 0;
    found = 15;
    peer = work;
    for (i = 0; i <= 14; i += 3) {
        if (peer[0] != 0 && id == peer[15] && best < *stamp) {
            best = *stamp;
            found = i;
        }
        stamp += 3;
        peer += 3;
    }
    return found;
}

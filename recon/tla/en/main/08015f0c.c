#include "TYPES.H"
#include "RAM_BUFFER.H"

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

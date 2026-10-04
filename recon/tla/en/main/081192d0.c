/*
 * Draft: BattlePres_WaitSync, ☀️'s WAIT_SYNC.C body over ⚓️'s battle session.
 * Score 2: sub sp is issued after the mode byte load where the listing issues
 * it after the work load. The rest is load scheduling: the native issues
 * loads (and fills their delay) earlier than agscc at -mcpu=arm7tdmi; with
 * -mtune=arm9tdmi added it scores 0.
 */
#include "TYPES.H"
#include "SERIAL_RUNTIME.H"
#include "BATTLE_WORK.H"

extern u8 gLinkStatus[];
void WaitFrames(s32);

s32 BattlePres_WaitSync(void)
{
    struct BattleSession *work = Ram_HeapSlots->battle_work;
    u16 *peer;
    u16 *sync;
    s32 miss = 0;
    s32 i;

    if (work->two_sided != 0) {
        u32 other = work->link_side ^ 1;

        /* The serial state occupies the first four bytes of each payload. */
        peer = (u16 *)(gSerialPeerPayloads[other] + 4);
        sync = (u16 *)gSerialTransfer.payload;
        if (work->link_paused == 0) {
            sync[0] = 'E';
            sync[1] = 'X';
            sync[2] = 'E';
            sync[3] = 'C';

            for (i = 0; i <= 29; i++) {
                if ((*(u16 *)gLinkStatus & 3) != 3) {
                    miss++;
                    if (miss > 24) {
                        return -1;
                    }
                } else {
                    miss = 0;
                    if (sync[2] == peer[2] && sync[3] == peer[3]) {
                        return 0;
                    }
                }
                WaitFrames(1);
            }
        }
        return -1;
    }
    return 0;
}

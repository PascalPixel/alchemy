/* 2026-10-04 frame audit: BattlePres_SyncTurn, 560 bytes, pool included.
 * Uncredited under S1/S2/C1; the owned English ROM supplies the raw listing.
 * Entries JA/EN/DE/ES/FR/IT: 080ad148, 080b6148, 080b8760, 080ba160, 080ba160, 080b6160.
 * Native frame: 20 bytes. The function never addresses, reads or writes
 * local stack storage; its handshake records are in gSerialTransfer and
 * gLinkPeerSignatures. WaitFrames receives only a register argument.
 * The previous unused[10] supplied only frame size, outside S2. Removing
 * it gives score 220: two deleted stack adjustments and one shortened
 * branch. All other instructions/registers agree. The transfer's actual
 * shared state does not justify a second unused local handshake array.
 * Independent native-stack/consumer review agrees the explicit reserve
 * has no consumed owner. Adjacent functions remain in their source module.
 */
#include "TYPES.H"
#include "SERIAL_RUNTIME.H"
#include "BATTLE_WORK.H"

extern u8 gLinkPeerSignatures[];
extern u8 gLinkStatus[];
#define LINK_REC (u32)gLinkPeerSignatures
#define LINK_STAT (*(u16 *)gLinkStatus)
void WaitFrames(s32 frames);

/* The five-stage link handshake that opens a linked battle turn. Each side
   posts ex/TU, rn, EXEC, tu and RN, waiting for the peer to echo each tag.
   Twenty-five incomplete transfers in a row, or a peer that has already
   moved to a different tag, fail with -1. */
s32 BattlePres_SyncTurn(void)
{
    struct BattleSession *work = gBattleWork;
    u16 *peer;
    u16 *sync;
    s32 missed_frames = 0;

    if (work->two_sided != 0) {
        u32 side = work->link_side;
        u32 other = 1;

        other ^= side;
        side = other << 1;
        side += other;
        side <<= 3;
        peer = (u16 *)(LINK_REC + side);
        sync = (u16 *)gSerialTransfer.payload;
        if (work->link_paused != 0) {
            goto fail;
        }

        sync[0] = 'e';
        sync[1] = 'x';
        sync[4] = 'T';
        sync[5] = 'U';
        WaitFrames(1);
        goto check1;
    wait1:
        WaitFrames(1);
    check1:
        if ((LINK_STAT & 3) != 3) {
            if (++missed_frames > 24) {
                goto fail;
            }
            goto wait1;
        }
        missed_frames = 0;
        if (sync[2] != peer[2] || sync[3] != peer[3]) {
            goto fail;
        }
        if (sync[0] != peer[0] || sync[1] != peer[1] || sync[4] != peer[4] || sync[5] != peer[5]) {
            goto wait1;
        }

        sync[6] = 'r';
        sync[7] = 'n';
        goto check2;
    wait2:
        WaitFrames(1);
    check2:
        if ((LINK_STAT & 3) != 3) {
            if (++missed_frames > 24) {
                goto fail;
            }
            goto wait2;
        }
        missed_frames = 0;
        if (sync[4] != peer[4] || sync[5] != peer[5]) {
            goto fail;
        }
        if (sync[6] != peer[6] || sync[7] != peer[7]) {
            goto wait2;
        }

        sync[0] = 'E';
        sync[1] = 'X';
        sync[2] = 'E';
        sync[3] = 'C';
        goto check3;
    wait3:
        WaitFrames(1);
    check3:
        if ((LINK_STAT & 3) != 3) {
            if (++missed_frames > 24) {
                goto fail;
            }
            goto wait3;
        }
        missed_frames = 0;
        if (sync[6] != peer[6] || sync[7] != peer[7]) {
            goto fail;
        }
        if (sync[0] != peer[0] || sync[1] != peer[1] || sync[2] != peer[2] || sync[3] != peer[3]) {
            goto wait3;
        }

        sync[4] = 't';
        sync[5] = 'u';
        goto check4;
    wait4:
        WaitFrames(1);
    check4:
        if ((LINK_STAT & 3) != 3) {
            if (++missed_frames > 24) {
                goto fail;
            }
            goto wait4;
        }
        missed_frames = 0;
        if (sync[0] != peer[0] || sync[1] != peer[1] || sync[2] != peer[2] || sync[3] != peer[3]) {
            goto fail;
        }
        if (sync[4] != peer[4] || sync[5] != peer[5]) {
            goto wait4;
        }

        sync[6] = 'R';
        sync[7] = 'N';
        goto check5;
    wait5:
        WaitFrames(1);
    check5:
        if ((LINK_STAT & 3) != 3) {
            if (++missed_frames <= 24) {
                goto wait5;
            }
        fail:
            return -1;
        }
        missed_frames = 0;
        if (peer[6] == 'r' && peer[7] == 'n') {
            goto wait5;
        }
    }
    return 0;
}

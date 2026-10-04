/*
 * Draft: BattlePres_SyncTurn, ☀️'s WAIT_SYNC.C body over ⚓️'s battle session
 * (heap slot 9, link_side and link_paused now named in BATTLE_WORK.H). Score
 * 10: sub sp lands after the two_sided load instead of before it, and the 'e'
 * store's movs r0, #1 is hoisted. The rest is load scheduling: the native
 * issues loads (and fills their delay) earlier than agscc at -mcpu=arm7tdmi;
 * with -mtune=arm9tdmi added it scores 0. Goes with BattlePres_WaitSync
 * (081192d0) into a ⚓️ WAIT_SYNC.C, or COMMON with ☀️'s.
 */
#include "TYPES.H"
#include "SERIAL_RUNTIME.H"
#include "BATTLE_WORK.H"

extern u8 gLinkStatus[];
extern u16 gLinkPeerSignatures[][12];
void WaitFrames(s32);

/* The five-stage handshake that opens a linked battle turn. Each side posts
   ex/TU, rn, EXEC, tu and RN in turn, spelling EXECTURN, and waits for the
   peer to echo each tag. Twenty-five frames in a row without a complete
   transfer, or a peer whose earlier tag no longer matches, fail with -1. */
s32 BattlePres_SyncTurn(void)
{
    struct BattleSession *work = Ram_HeapSlots->battle_work;
    u16 *peer;
    u16 *sync;
    s32 miss = 0;
    /* FAKEMATCH: never used; the reference's frame keeps twenty bytes that no instruction touches. */
    u16 spare[10];

    if (work->two_sided != 0) {
        peer = gLinkPeerSignatures[work->link_side ^ 1];
        sync = (u16 *)gSerialTransfer.payload;
        if (work->link_paused != 0) {
            return -1;
        }

        sync[0] = 'e';
        sync[1] = 'x';
        sync[4] = 'T';
        sync[5] = 'U';
        WaitFrames(1);
        for (;;) {
            if ((*(u16 *)gLinkStatus & 3) != 3) {
                if (++miss > 24) {
                    return -1;
                }
            } else {
                miss = 0;
                if (sync[2] != peer[2] || sync[3] != peer[3]) {
                    return -1;
                }
                if (sync[0] == peer[0] && sync[1] == peer[1] && sync[4] == peer[4] && sync[5] == peer[5]) {
                    break;
                }
            }
            WaitFrames(1);
        }

        sync[6] = 'r';
        sync[7] = 'n';
        for (;;) {
            if ((*(u16 *)gLinkStatus & 3) != 3) {
                if (++miss > 24) {
                    return -1;
                }
            } else {
                miss = 0;
                if (sync[4] != peer[4] || sync[5] != peer[5]) {
                    return -1;
                }
                if (sync[6] == peer[6] && sync[7] == peer[7]) {
                    break;
                }
            }
            WaitFrames(1);
        }

        sync[0] = 'E';
        sync[1] = 'X';
        sync[2] = 'E';
        sync[3] = 'C';
        for (;;) {
            if ((*(u16 *)gLinkStatus & 3) != 3) {
                if (++miss > 24) {
                    return -1;
                }
            } else {
                miss = 0;
                if (sync[6] != peer[6] || sync[7] != peer[7]) {
                    return -1;
                }
                if (sync[0] == peer[0] && sync[1] == peer[1] && sync[2] == peer[2] && sync[3] == peer[3]) {
                    break;
                }
            }
            WaitFrames(1);
        }

        sync[4] = 't';
        sync[5] = 'u';
        for (;;) {
            if ((*(u16 *)gLinkStatus & 3) != 3) {
                if (++miss > 24) {
                    return -1;
                }
            } else {
                miss = 0;
                if (sync[0] != peer[0] || sync[1] != peer[1] || sync[2] != peer[2] || sync[3] != peer[3]) {
                    return -1;
                }
                if (sync[4] == peer[4] && sync[5] == peer[5]) {
                    break;
                }
            }
            WaitFrames(1);
        }

        /* The last tag needs no echo: wait only until the peer leaves rn. */
        sync[6] = 'R';
        sync[7] = 'N';
        for (;;) {
            if ((*(u16 *)gLinkStatus & 3) != 3) {
                if (++miss > 24) {
                    return -1;
                }
            } else {
                miss = 0;
                if (peer[6] != 'r' || peer[7] != 'n') {
                    break;
                }
            }
            WaitFrames(1);
        }
    }
    return 0;
}

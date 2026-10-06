#include "TYPES.H"
#include "EDITION.H"
#include "RAM_BUFFER.H"
#include "SERIAL_RUNTIME.H"
#include "BATTLE_WORK.H"

extern volatile u16 gLinkStatus;
extern u16 gLinkPeerSignatures[][12];

s32 BattlePres_WaitSync(void)
{
#if GAME_TLA
    struct BattleSession *work = Ram_HeapSlots->battle_work;
#else
    struct BattleSession *work = gBattleWork;
#endif
    u16 *peer;
    u16 *sync;
    s32 miss = 0;
    s32 i;

    if (work->two_sided != 0) {
        u32 other = work->link_side ^ 1;

        peer = (u16 *)(gSerialPeerPayloads[other] + 4);
        sync = (u16 *)gSerialTransfer.payload;
        if (work->link_paused == 0) {
            sync[0] = 'E';
            sync[1] = 'X';
            sync[2] = 'E';
            sync[3] = 'C';

            for (i = 0; i <= 29; i++) {
                if ((gLinkStatus & 3) != 3) {
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

s32 BattlePres_SyncTurn(void)
{
#if GAME_TLA
    struct BattleSession *work = Ram_HeapSlots->battle_work;
#else
    struct BattleSession *work = gBattleWork;
#endif
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
            if ((gLinkStatus & 3) != 3) {
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
            if ((gLinkStatus & 3) != 3) {
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
            if ((gLinkStatus & 3) != 3) {
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
            if ((gLinkStatus & 3) != 3) {
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

        sync[6] = 'R';
        sync[7] = 'N';
        for (;;) {
            if ((gLinkStatus & 3) != 3) {
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

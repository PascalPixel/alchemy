#include "TYPES.H"

/* battle/presentation/sync_turn.c */
/* The five-stage link handshake that opens a linked battle turn. Each side
   posts a two-letter tag in its own record ("ex" and "TU", then "rn", then
   "EXEC", "tu" and "RN") and waits until the peer's record echoes it. A
   frame without a complete transfer counts as a miss; 25 misses in a row,
   or a peer that has already moved on to a different tag, fail with -1. */
struct LinkWork {
    u8 pad0[0x44];
    u8 enabled;
    u8 pad1[0x0b];
    u8 side;
    u8 pad2;
    u8 paused;
};

#define LINK_WORK (*(struct LinkWork **)0x03001e74)
#define LINK_REC 0x02002024
#define LINK_LOCAL ((u16 *)0x02002224)
#define LINK_STAT (*(u16 *)0x03001f64)

void WaitFrames(s32 frames);

s32 BattlePres_SyncTurn(void)
{
    struct LinkWork *work = LINK_WORK;
    u16 *peer;
    u16 *sync;
    s32 miss = 0;
    u16 unused[10]; /* FAKEMATCH: the reference reserves a 20-byte frame it never uses */

    if (work->enabled != 0) {
        u32 side = work->side;
        u32 other = 1;

        other ^= side;
        side = other << 1;
        side += other;
        side <<= 3;
        peer = (u16 *)(LINK_REC + side);
        sync = LINK_LOCAL;
        if (work->paused != 0) {
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
            if (++miss > 24) {
                goto fail;
            }
            goto wait1;
        }
        miss = 0;
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
            if (++miss > 24) {
                goto fail;
            }
            goto wait2;
        }
        miss = 0;
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
            if (++miss > 24) {
                goto fail;
            }
            goto wait3;
        }
        miss = 0;
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
            if (++miss > 24) {
                goto fail;
            }
            goto wait4;
        }
        miss = 0;
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
            if (++miss <= 24) {
                goto wait5;
            }
        fail:
            return -1;
        }
        miss = 0;
        if (peer[6] == 'r' && peer[7] == 'n') {
            goto wait5;
        }
    }
    return 0;
}

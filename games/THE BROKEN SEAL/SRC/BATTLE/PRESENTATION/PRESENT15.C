#include "TYPES.H"
#include "SERIAL_RUNTIME.H"
#include "BATTLE_PARTY.H"

extern u8 gLinkPeerSignatures[];
extern u8 *gBattleWork;
extern u8 gLinkStatus[];

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

#define LINK_WORK ((struct LinkWork *)gBattleWork)
#define LINK_REC (u32)gLinkPeerSignatures
#define LINK_STAT (*(u16 *)gLinkStatus)
void WaitFrames(s32 frames);

extern u8 IwramClearWords[];

/*
 * _call_via_r3 names a `bx rN` slot: the call is indirect through the
 * register that slot selects, and the trailing argument is the callee
 * address at 0x03000164. That routine is reached with two arguments at
 * some sites and three at others, so its shape is not established.
 */
s16 _call_via_r3(s32, s32, s16, s32);

/* battle/presentation/sync_turn.c */
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
        sync = (u16 *)gSerialTransfer.reserved;
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

/*
 * Apply a value to the battle work record at 0x02002224.
 */
s32 BattleParty_AssignMemberSlots(void)
{
    u16 active_members[8];
    u8 *battle_state = gBattleWork;
    s32 party_size = BattleParty_PrepareActiveOwners(active_members);
    s32 member_slot;
    s32 unit_id;

    for (member_slot = 0; member_slot < party_size; member_slot++) {
        unit_id = active_members[member_slot];
        unit_id += 72;
        battle_state[unit_id] = (s8)(member_slot - 128);
    }
}

/*
 * The third argument reads val before val is written, so it carries
 * whatever the register already holds; it must not be respelled as a fresh
 * load. The two assignments that follow the call keep that order.
 */
char Battle_ApplyValueToWork2224(s16 arg2)
{
  s16 val;
  s16 val2;
  _call_via_r3((s32)gSerialTransfer.reserved, 0x10, val, (u32)IwramClearWords);
  val2 = arg2;
  val = val2;
}

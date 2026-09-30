#include "TYPES.H"
#include "SERIAL_RUNTIME.H"
#include "SCENE.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_RUNTIME.H"
extern u8 gLinkPeerSignatures[];

extern u8 gBattleWork[];
extern u8 gLinkStatus[];

/* battle/pres_wait_sync.c */
/* battle/presentation/misc/wait_sync.c */
struct LinkWork {
    u8 pad0[0x44];
    u8 enabled;
    u8 pad1[0x0b];
    u8 side;
    u8 pad2;
    u8 paused;
};

#define LINK_WORK (*(struct LinkWork **)gBattleWork)
#define LINK_REC (u32)gLinkPeerSignatures
#define LINK_STAT (*(u16 *)gLinkStatus)

s32 BattlePres_WaitSync(void)
{
    struct LinkWork *work = LINK_WORK;
    u16 *peer;
    u16 *sync;
    s32 miss = 0;
    s32 i;

    if (work->enabled != 0) {
        u32 side = work->side;
        u32 other = 1;

        other ^= side;
        side = other << 1;
        side += other;
        side <<= 3;
        peer = (u16 *)(LINK_REC + side);
        sync = (u16 *)gSerialTransfer.reserved;
        if (work->paused == 0) {
            sync[0] = 'E';
            sync[1] = 'X';
            sync[2] = 'E';
            sync[3] = 'C';

            for (i = 0; i <= 29; i++) {
                if ((LINK_STAT & 3) != 3) {
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

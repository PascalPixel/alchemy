#include "types.h"

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

#define BattlePresentation_WaitSync Func_080b60a0

s32 BattlePresentation_WaitSync(void)
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
        sync = LINK_LOCAL;
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

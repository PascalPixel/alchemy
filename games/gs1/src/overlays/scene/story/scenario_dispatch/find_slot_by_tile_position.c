/* Contiguous unnamed state-owner run for resource_37f. */

#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/story/scenario_dispatch/find_slot_by_tile_position.h"

extern s32 *Story_Run(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_Do(0x302);
    Story_Do2(0x303);
    if (dir == 93) {
        Story_Do3(0x303);
    } else if (dir == 95) {
        Story_Do4(0x302);
    }
}

#include "types.h"

extern s32 *Story_Run2(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_Do5(0x300);
    Story_Do6(0x301);
    if (dir == 115) {
        Story_Do7(0x300);
    } else if (dir == 113) {
        Story_Do8(0x301);
    }
}

#include "types.h"

extern s32 *Story_Run3(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_Do9(0x310);
    Story_Do10(0x311);
    if (dir == 99) {
        Story_Do11(0x311);
    } else if (dir == 101) {
        Story_Do12(0x310);
    }
    Story_Do13(0);
}

#include "types.h"

extern s32 *Story_Run4(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_Do14(0x312);
    Story_Do15(0x313);
    if (dir == 103) {
        Story_Do16(0x313);
    } else if (dir == 105) {
        Story_Do17(0x312);
    }
    Story_Do18(0);
}

#include "types.h"

extern s32 *Story_Run5(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_Do19(0x314);
    Story_Do20(0x315);
    if (dir == 107) {
        Story_Do21(0x315);
    } else if (dir == 109) {
        Story_Do22(0x314);
    }
    Story_Do23(0);
}

#include "types.h"

extern s32 *Story_Run6(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_Do24(0x316);
    Story_Do25(0x317);
    if (dir == 111) {
        Story_Do26(0x317);
    } else if (dir == 113) {
        Story_Do27(0x316);
    }
    Story_Do28(0);
}

#include "types.h"

extern s32 *Story_Run7(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_Do29(0x318);
    Story_Do30(0x319);
    if (dir == 115) {
        Story_Do31(0x319);
    } else if (dir == 117) {
        Story_Do32(0x318);
    }
    Story_Do33(0);
}

#include "types.h"

extern s32 *Story_Run8(s32);

    s32 dir;

    if (rec == 0) return;
    dir = rec[2] >> 20;
    Story_Do34(0x31a);
    Story_Do35(0x31b);
    if (dir == 119) {
        Story_Do36(0x31b);
    } else if (dir == 121) {
        Story_Do37(0x31a);
    }
    Story_Do38(0);
}

#include "types.h"

extern u8 *gWork;

s32 *Actor_FindSlotByTilePosition(s32 x, s32 z)
{
    s32 **slots = (s32 **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

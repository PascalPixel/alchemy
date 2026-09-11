#include "types.h"
#include "scene.h"

/* overlays/scene/story/scenario_dispatch/find_slot_by_tile_position.c */
/* Contiguous unnamed state-owner run for resource_37f. */


extern s32 *Story_Run(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_Do(0x302);
    Story_unk2_2(0x303);
    if (dir == 93) {
        Story_unk3_2(0x303);
    } else if (dir == 95) {
        Story_unk4_2(0x302);
    }
}


extern s32 *Story_unk2_4(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_unk5_2(0x300);
    Story_unk6_2(0x301);
    if (dir == 115) {
        Story_unk7_2(0x300);
    } else if (dir == 113) {
        Story_unk8_2(0x301);
    }
}


extern s32 *Story_unk3_4(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_unk9_2(0x310);
    Story_unk10_2(0x311);
    if (dir == 99) {
        Story_unk11_2(0x311);
    } else if (dir == 101) {
        Story_unk12_2(0x310);
    }
    Story_unk13_2(0);
}


extern s32 *Story_unk4_4(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_unk14_2(0x312);
    Story_unk15_2(0x313);
    if (dir == 103) {
        Story_unk16_2(0x313);
    } else if (dir == 105) {
        Story_unk17_2(0x312);
    }
    Story_unk18_2(0);
}


extern s32 *Story_unk5_4(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_unk19_2(0x314);
    Story_unk20_2(0x315);
    if (dir == 107) {
        Story_unk21_2(0x315);
    } else if (dir == 109) {
        Story_unk22_2(0x314);
    }
    Story_unk23_2(0);
}


extern s32 *Story_unk6_4(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_unk24_2(0x316);
    Story_unk25_2(0x317);
    if (dir == 111) {
        Story_unk26_2(0x317);
    } else if (dir == 113) {
        Story_unk27_2(0x316);
    }
    Story_unk28_2(0);
}


extern s32 *Story_unk7_4(s32);

    s32 dir;

    if (rec == 0) {
        return;
    }
    dir = rec[2] >> 20;
    Story_unk29_2(0x318);
    Story_unk30_2(0x319);
    if (dir == 115) {
        Story_unk31_2(0x319);
    } else if (dir == 117) {
        Story_unk32_2(0x318);
    }
    Story_unk33_2(0);
}


extern s32 *Story_unk8_4(s32);

    s32 dir;

    if (rec == 0) return;
    dir = rec[2] >> 20;
    Story_unk34_2(0x31a);
    Story_unk35_2(0x31b);
    if (dir == 119) {
        Story_unk36_2(0x31b);
    } else if (dir == 121) {
        Story_unk37_2(0x31a);
    }
    Story_unk38_2(0);
}


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

/* overlays/scene/story/scenario_dispatch/dispatch_by_scenario_id.c */
extern s16 gCell[];
extern u8 Value_00000013;
extern u8 Value_00000010;

s32 Scene_DispatchByScenarioId(void)
{
    s32 scenario = gCell[224];

    if (scenario == (s32)&Value_00000013) {
        Story_Run();
    } else if (scenario == (s32)&Value_00000010) {
        Story_unk2_4();
    }
    return 0;
}

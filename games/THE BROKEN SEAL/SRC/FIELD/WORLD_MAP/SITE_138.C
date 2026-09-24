#include "TYPES.H"

struct WorldMapSite {
    s32 kind;
    s16 id;
    u8 unknown_06[2];
    s32 handler;
};

struct WorldMapMark {
    s16 id;
    u8 unknown_02[6];
    s32 x;
    u8 unknown_0c[4];
    s32 z;
    u16 flags;
    u8 unknown_16[2];
};

extern struct WorldMapSite WorldMap_Sites[];
extern struct WorldMapMark WorldMap_Marks[];

/* Switch every kind-1 site with id 138 to kind 2 with the handler at 0x0200a858, then move mark 57 to (0x1794, 0xd48) with flags 0x3000. */
void WorldMap_ActivateSite138(void)
{
    s32 i;

    for (i = 0;; i++) {
        if (WorldMap_Sites[i].kind == 1 && WorldMap_Sites[i].id == 138) {
            WorldMap_Sites[i].kind = 2;
            WorldMap_Sites[i].handler = 0x0200a859;
        }
        /* FAKEMATCH: a goto past the loop, not a break, keeps the ROM's strength-reduced offsets. */
        if (WorldMap_Sites[i].kind == -1)
            goto marks;
    }
marks:
    for (i = 0;; i++) {
        if (WorldMap_Marks[i].id == 57) {
            WorldMap_Marks[i].x = 0x17940000;
            WorldMap_Marks[i].z = 0x0d480000;
            WorldMap_Marks[i].flags = 0x3000;
            return;
        }
    }
}

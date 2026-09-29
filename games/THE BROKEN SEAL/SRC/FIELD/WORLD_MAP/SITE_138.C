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

extern struct WorldMapSite gWorldMapEvents[];
extern struct WorldMapMark gWorldMapPlacements[];
void FieldScene_RunScene371_02002858(void);

/* Switch every kind-1 site with id 138 to kind 2 with the Robin dialogue as its handler, then move mark 57 to (0x1794, 0xd48) with flags 0x3000. */
void WorldMap_ActivateSite138(void)
{
    s32 i;

    for (i = 0;; i++) {
        if (gWorldMapEvents[i].kind == 1 && gWorldMapEvents[i].id == 138) {
            gWorldMapEvents[i].kind = 2;
            gWorldMapEvents[i].handler = (s32)FieldScene_RunScene371_02002858;
        }
        /* FAKEMATCH: a goto past the loop, not a break, keeps the ROM's strength-reduced offsets. */
        if (gWorldMapEvents[i].kind == -1)
            goto marks;
    }
marks:
    for (i = 0;; i++) {
        if (gWorldMapPlacements[i].id == 57) {
            gWorldMapPlacements[i].x = 0x17940000;
            gWorldMapPlacements[i].z = 0x0d480000;
            gWorldMapPlacements[i].flags = 0x3000;
            return;
        }
    }
}

#include "TYPES.H"

#define FieldScene_FindActorRegion Func_0200034c

extern u8 *Data_03001ebc;
extern s32 Data_0200df18[];
extern s32 Data_0200df30[];
extern s32 Data_0200ded8[];
extern u8 *Func_020059ba(s32);

u8 *FieldScene_FindActorRegion(s32 *direction, s32 *slot, s32 *footprint)
{
    u8 *work = Data_03001ebc;
    u8 *player = Func_020059ba(0);
    u8 **list;
    u32 i, j;

    *direction = *(u16 *)(player + 6) >> 12;
    list = (u8 **)(work + 52);
    for (i = 8; i <= 65; list++, i++) {
        u8 *actor = *list;
        s32 kind;
        j = 0;
        kind = (*(s16 **)(*(u8 **)(actor + 80) + 40))[j];
        for (j = 0; j < 6; j++) {
            if (kind == Data_0200df18[j]) {
                s32 x, z, left, top, right, bottom;
                *footprint = j;
                x = ((*(s32 *)(player + 8) >> 16) + (Data_0200ded8[*direction] >> 16)) >> 4;
                z = ((*(s32 *)(player + 16) >> 16) + (s16)Data_0200ded8[*direction]) >> 4;
                left = (*(s16 *)(actor + 10) + Data_0200df30[j * 4 + 0]) >> 4;
                top = (*(s16 *)(actor + 18) + Data_0200df30[j * 4 + 1]) >> 4;
                right = (*(s16 *)(actor + 10) + Data_0200df30[j * 4 + 2]) >> 4;
                bottom = (*(s16 *)(actor + 18) + Data_0200df30[j * 4 + 3]) >> 4;
                if (left <= x && x < right && top <= z && z < bottom) {
                    if (j & 1) {
                        if (left != (*(s32 *)(player + 8) >> 20)) {
                            *slot = i;
                            return actor;
                        }
                    } else if (top != (*(s32 *)(player + 16) >> 20)) {
                        *slot = i;
                        return actor;
                    }
                }
            }
        }
    }
    return NULL;
}

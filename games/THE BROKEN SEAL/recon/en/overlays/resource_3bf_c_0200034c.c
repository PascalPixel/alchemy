/* Draft C: complete owner reviewed, but linked bytes do not yet match. */
#include "TYPES.H"

#define FieldScene_FindActorRegion Func_0200034c

extern u8 *Data_03001ebc;
extern s32 Data_0200df18[];
extern s32 Data_0200df30[];
extern s32 Data_0200ded8[];
extern u8 *Func_020059ba(s32);

u8 *FieldScene_FindActorRegion(s32 *layer, s32 *slot, s32 *region)
{
    u8 *work;
    u8 *player;
    u8 **list;
    u8 *actor;
    s32 idx;
    s32 part;
    s32 kind;
    s32 *ids;
    s32 posy;
    s32 offset;
    s32 posx;
    s32 *bounds;
    s32 x;
    s32 y;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;

    work = Data_03001ebc;
    player = Func_020059ba(0);
    *layer = *(u16 *)(player + 6) >> 12;
    idx = 8;
    list = (u8 **)(work + 52);
    do {
        actor = *list;
        kind = **(s16 **)(*(u8 **)(actor + 80) + 40);
        ids = Data_0200df18;
        part = 0;
        bounds = Data_0200df30;
        do {
            if (kind == *ids++) {
                *region = part;
                offset = Data_0200ded8[*layer];
                posx = *(s32 *)(player + 8);
                posy = *(s32 *)(player + 16);
                x = ((posx >> 16) + (offset >> 16)) >> 4;
                y = ((posy >> 16) + (s16)offset) >> 4;
                left = (*(s16 *)(actor + 10) + bounds[0]) >> 4;
                top = (*(s16 *)(actor + 18) + bounds[1]) >> 4;
                right = (*(s16 *)(actor + 10) + bounds[2]) >> 4;
                bottom = (*(s16 *)(actor + 18) + bounds[3]) >> 4;
                if (left <= x && x < right && top <= y && y < bottom) {
                    if (part & 1) {
                        if (left != (posx >> 20)) {
                            *slot = idx;
                            return actor;
                        }
                    } else {
                        if (top != (posy >> 20)) {
                            *slot = idx;
                            return actor;
                        }
                    }
                }
            }
            part++;
            bounds += 4;
        } while ((u32)part <= 5);
        idx++;
        list++;
    } while ((u32)idx <= 65);
    return 0;
}

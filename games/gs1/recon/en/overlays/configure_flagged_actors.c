#include "types.h"

extern s16 Data_02000240_t[][1];
extern void Scene_PlaceSlots(void);
extern s32 Scene_CheckFlag(s32);
extern void Scene_SetFlag(s32);
extern void Scene_SetMode(s32, s32);
extern void Scene_SetPosition(s32, s32, s32);
extern void Scene_SetFacing(s32, s32, s32);
extern u8 *Scene_GetActor(s32);
extern void Scene_SetTiles(s32, s32, s32, s32, s32, s32);

static __inline__ s32 Check(s32 flag)
{
    return Scene_CheckFlag(flag);
}

static __inline__ void Position(s32 slot, s32 x, s32 z)
{
    Scene_SetPosition(slot, x, z);
}

static __inline__ void Facing(s32 slot, s32 angle, s32 mode)
{
    Scene_SetFacing(slot, angle, mode);
}

void FieldScene_ConfigureFlaggedActors(void)
{
    u8 *record;
    s32 none;

    Scene_PlaceSlots();
    if (Check(0x950) != 0)
        Scene_SetMode(12, 2);
    if (Data_02000240_t[225][0] == 3)
        Scene_SetFlag(0x12f);
    if (Data_02000240_t[225][0] == 1)
        Scene_SetFlag(0x8aa);
    if (Check(0x8aa) != 0) {
        Position(8, 0x1980000, 0x1280000);
        Facing(8, 0x8000, 0);
    }
    if (Check(0x8ab) != 0) {
        Position(13, 0x1180000, 0x1280000);
        Facing(13, 0xc000, 0);
        Position(16, 0x1200000, 0x1180000);
        Facing(16, 0xe000, 0);
        Position(10, 0xe80000, 0x1300000);
        Facing(10, 0x4000, 0);
        Position(11, 0xf00000, 0x1380000);
        Facing(11, 0xc000, 0);
        record = Scene_GetActor(10);
        record[89] = 0;
        record[35] = 2;
        (*(u8 **)(record + 80))[9] |= 12;
        (*(u8 **)(record + 80))[38] = 0;
        {
            s32 target = *(s32 *)(record + 80);
            s32 shown = 0xc000;
            *(u16 *)(target + 30) = shown;
        }
        record = Scene_GetActor(11);
        none = 0;
        record[35] = none;
        (*(u8 **)(record + 80))[9] |= 12;
        (*(u8 **)(record + 80))[21] |= 12;
    }
    if (Check(0x950) != 0) {
        s32 col = 14;
        s32 row = 18;
        Scene_SetTiles(18, 18, 1, 1, col, row);
        Scene_SetTiles(18, 18, 1, 1, 15, row);
    }
}

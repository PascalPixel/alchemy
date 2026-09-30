#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"

s32 Korosseo_ShowItemIcon(s32 slot, s32 item);

extern s32 TakaraAshiba_IconTimer;

/* Run a sixteen-frame cycle: on frame 12 return the actors whose flags are clear to their marks, and on the even frames before it show their item icons in turn. */
void TakaraAshiba_UpdateItemIcons(void)
{
    if (++TakaraAshiba_IconTimer > 16) {
        TakaraAshiba_IconTimer = 0;
    }
    switch (TakaraAshiba_IconTimer) {
    case 12:
        if (Engine_GameFlagIsSet(0xee7) == 0) {
            Call3(Engine_ActorSetPosition, 8, 0xe80000, 0x3680000);
        }
        if (Engine_GameFlagIsSet(0xee8) == 0) {
            Call3(Engine_ActorSetPosition, 9, 0x1280000, 0x3380000);
        }
        if (Engine_GameFlagIsSet(0xee9) == 0) {
            Call3(Engine_ActorSetPosition, 10, 0x1480000, 0x2f80000);
        }
        if (Engine_GameFlagIsSet(0xeea) == 0) {
            Call3(Engine_ActorSetPosition, 11, 0x1680000, 0x3680000);
        }
        break;
    case 10:
        Korosseo_ShowItemIcon(8, 0);
        break;
    case 8:
        Korosseo_ShowItemIcon(9, 0);
        break;
    case 6:
        Korosseo_ShowItemIcon(10, 0);
        break;
    case 4:
        Korosseo_ShowItemIcon(11, 0);
        break;
    case 2:
        Korosseo_ShowItemIcon(12, 1);
        break;
    }
}

#include "types.h"

/* The four declarations name this overlay's import veneers. */
u8 *Func_02001532();
s32 Func_0200078e();
u8 *Func_02001548();
void Func_020007ce();

#define Scene_GetPartnerActor Func_02001532
#define Actor_CheckProximity Func_0200078e
#define Scene_GetPlayerActor Func_02001548
#define Actor_UpdateProximity Func_020007ce
#define UpdateActorProximity Func_02000400

s32 UpdateActorProximity(u8 *actor)
{
    u8 **globals = (u8 **)0x03001e8c;
    u8 *scene = globals[0];
    u8 *work = globals[12];
    u16 *flags = (u16 *)(actor + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    if ((*flags & 1) != 0) {
        partner = Scene_GetPartnerActor(15);
    } else {
        partner = Scene_GetPartnerActor(14);
    }
    if (Actor_CheckProximity(actor, partner, 32, 0) != 0) {
        return 0;
    }

    player = Scene_GetPlayerActor(0);

    if (*(s16 *)(work + 376) != 0 || scene[0x0ea4] != 0) {
        range = 26;
        if ((*flags & 2) != 0) {
            force = 1;
        }
    }

    Actor_UpdateProximity(actor, player, range, force);
    return 0;
}

#include "types.h"

#define Scene_GetPartnerActor Func_02001532
#define Actor_CheckProximity Func_0200078e
#define Scene_GetPlayerActor Func_02001548
#define Actor_UpdateProximity Func_020007ce
#define UpdateActorProximity Func_02000400
#define PlaceActorAndSetSceneDelay Func_02000a80

extern u8 *Data_03001ebc;

/* The four declarations name this overlay's import veneers. */
u8 *Func_02001532();
s32 Func_0200078e();
u8 *Func_02001548();
void Func_020007ce();
u8 *Func_02001a82(int);
void Func_02001a9a(int, int, int);
void Func_02001a48(u8 *, int);
void Func_02001aca(int, int, int);
void Func_02001af4(int, int, int);
void Func_02001a34(int);
void Func_02001a84(u8 *, int);
void Func_02001ba0(s32, s32, s32);
void Func_02001bc2(s32, s32, s32);
void Func_02001c56(s32);

static __inline__ void SetScale(s32 actor, s32 horizontal, s32 vertical)
{
    Func_02001ba0(actor, horizontal, vertical);
}

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

void Func_02000970(int actor, int x, int z, int field40)
{
    u8 *record = Func_02001a82(actor); int frames;
    Func_02001a9a(actor, 0x30000, 0x18000); *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0; *(s32 *)(record + 40) = field40; Func_02001a48(record, 0);
    Func_02001aca(actor, x, z); Func_02001af4(actor, x << 16, z << 16);
    for (frames = 60; frames != 0; --frames) { Func_02001a34(1); if (*(s16 *)(record + 42) == 0) break; }
    Func_02001a84(record, 1); *(s32 *)(record + 72) = 0x10000;
}

void PlaceActorAndSetSceneDelay(s32 x, s32 y, s32 delay)
{
    s32 zero = 0;

    SetScale(zero, 0x8000, 0x4000);
    Func_02001bc2(zero, x, y);
    *(s32 *)(Data_03001ebc + 456) = 16;
    Func_02001c56(delay);
}

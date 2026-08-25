#include "types.h"

struct ActorFacing {
    u8 unknown_00[6];
    u16 angle;
};

struct ActorFacing *Func_020013e4(s32 actor_id);
s32 Func_020013d8(s32 flag_id);
void Func_02001482(s32 message_id);
void Func_02001492(s32 message_id);
void Func_0200149a(s32 value, s32 zero);
void Func_020014aa(s32 value, s32 zero);
void Func_020014e2(s32 message_id, s32 value);

void Func_020001c8(s32 value)
{
    struct ActorFacing *actor = Func_020013e4(0);
    s32 angle = (actor->angle + 0x2000) & 0xffffc000;

    if ((angle << 16) == 0xc0000000) {
        Func_020014e2(34, value);
        return;
    }

    if (Func_020013d8(0x9a7) != 0) {
        Func_02001482(0x28f4);
        Func_0200149a(value, 0);
    } else {
        Func_02001492(0x26e9);
        Func_020014aa(value, 0);
    }
}

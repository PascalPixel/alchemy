#include "types.h"

struct ActorFacing {
    u8 unknown_00[6];
    u16 angle;
};

struct ActorFacing *Func_02001378(s32 actor_id);
s32 Func_0200136c(s32 flag_id);
void Func_02001416(s32 message_id);
void Func_02001426(s32 message_id);
void Func_0200142e(s32 value, s32 zero);
void Func_0200143e(s32 value, s32 zero);
void Func_02001476(s32 message_id, s32 value);

void Func_0200015c(s32 value)
{
    struct ActorFacing *actor = Func_02001378(0);
    s32 angle = (actor->angle + 0x2000) & 0xffffc000;

    if ((angle << 16) == 0xc0000000) {
        Func_02001476(33, value);
        return;
    }

    if (Func_0200136c(0x9a7) != 0) {
        Func_02001416(0x28f2);
        Func_0200142e(value, 0);
    } else {
        Func_02001426(0x26e7);
        Func_0200143e(value, 0);
    }
}

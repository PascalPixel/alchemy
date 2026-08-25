#include "types.h"

struct ActorFacing { u8 unknown_00[6]; u16 angle; };
struct ActorFacing *Func_020014b8(s32);
s32 Func_020014ac(s32);
void Func_02001556(s32);
void Func_02001566(s32);
void Func_0200156e(s32, s32);
void Func_0200157e(s32, s32);
void Func_020015c6(s32, s32);

void Func_0200029c(s32 value)
{
    struct ActorFacing *actor = Func_020014b8(0);
    s32 angle = (actor->angle + 0x2000) & 0xffffc000;

    if ((angle << 16) == 0xc0000000) {
        Func_020015c6(11, value);
    } else if (Func_020014ac(0x9a7) != 0) {
        Func_02001556(0x28f6);
        Func_0200156e(value, 0);
    } else {
        Func_02001566(0x26eb);
        Func_0200157e(value, 0);
    }
}

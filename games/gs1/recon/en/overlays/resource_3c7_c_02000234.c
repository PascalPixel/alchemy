#include "types.h"

struct ActorFacing { u8 unknown_00[6]; u16 angle; };
struct ActorFacing *Func_02001450(s32);
s32 Func_0200143c(s32);
void Func_020014e6(s32);
void Func_020014fe(s32, s32);
void Func_02001504(s32);
void Func_0200151c(s32, s32);
void Func_02001554(s32);

void Func_02000234(s32 value)
{
    struct ActorFacing *actor = Func_02001450(0);
    s32 angle = (actor->angle + 0x2000) & 0xffffc000;

    if ((angle << 16) == 0xc0000000) {
        Func_02001554(value);
    } else if (Func_0200143c(0x9a7) != 0) {
        Func_020014e6(0x28fc);
        Func_020014fe(value, 0);
    } else {
        Func_02001504(0x26f6);
        Func_0200151c(value, 0);
    }
}

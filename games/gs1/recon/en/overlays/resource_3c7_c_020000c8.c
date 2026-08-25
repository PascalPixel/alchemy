#include "types.h"

struct ActorFacing { u8 unknown_00[6]; u16 angle; };
struct ActorFacing *Func_020012e4(s32);
s32 Func_020012d8(s32);
s32 Func_02001334(s32, s32);
void Func_02001326(s32);
void Func_02001382(s32);
void Func_02001394(s32);
void Func_0200139a(s32, s32);
void Func_020013a4(s32, s32);
void Func_020013b4(s32);
void Func_020013bc(s32);
void Func_020013d4(s32, s32);
void Func_020013e2(s32, s32);

void Func_020000c8(s32 value)
{
    struct ActorFacing *actor = Func_020012e4(0);
    s32 angle = (actor->angle + 0x2000) & 0xffffc000;

    if ((angle << 16) == 0xc0000000) {
        Func_020013e2(32, value);
    } else if (Func_020012d8(0x9a7) != 0) {
        Func_02001382(0x28f0);
        Func_0200139a(value, 0);
    } else {
        s32 message = 0x26e3;
        Func_02001394(message);
        Func_020013a4(value, 0);
        if (Func_02001334(0, 0) == 0) {
            Func_02001326(10);
            Func_020013b4(message + 1);
        } else {
            Func_020013bc(message + 2);
        }
        Func_020013d4(value, 0);
    }
}

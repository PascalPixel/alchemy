#include "types.h"

extern u32 Data_03001e40;

struct Actor {
    u8 reserved_00[80];
    u8 *presentation;
};

struct Actor *Func_0200854a(s32);

#define ActorPresentation_SelectActorTwentySevenState Func_02003e18
void ActorPresentation_SelectActorTwentySevenState(void)
{
    struct Actor *actor = Func_0200854a(27);
    u32 flags = Data_03001e40;
    u8 *presentation = actor->presentation;

    if (flags & 1) {
        u8 *state = presentation + 35;
        *state = 2;
    } else {
        u8 *state = presentation + 35;
        *state = 64;
    }
}

#include "types.h"

struct Presentation {
    u8 reserved_00[9];
    u8 flags;
};

struct SceneActor {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

void Func_02001f52(s32, s32, s32);
void Func_0200296c(const void *, s32, s32);
void Func_02002992(s32, s32, s32, s32, s32, s32);
struct SceneActor *Func_020029f8(s32);
void Func_02002aea(s32);

#define ActorPresentation_SetupActorZeroForSceneNineAt296_176 Func_02001010

void ActorPresentation_SetupActorZeroForSceneNineAt296_176(void)
{
    struct SceneActor *actor = Func_020029f8(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Func_02002aea(158);
    Func_0200296c((const void *)0x0200a880, 49, 10);
    {
        s32 cell = 18;
        s32 row = 10;

        Func_02002992(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Func_02001f52(296, 176, 9);
}

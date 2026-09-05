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

void Func_02001794(s32, s32, s32);
void Func_02001d6c(const void *, s32, s32);
void Func_02001d92(s32, s32, s32, s32, s32, s32);
struct SceneActor *Func_02001dd0(s32);
void Func_02001e8a(s32);

void ActorPresentation_SetupActorZeroForSceneTwelve(void)
{
    struct SceneActor *actor = Func_02001dd0(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Func_02001e8a(158);
    Func_02001d6c((const void *)0x020097c4, 35, 9);
    {
        s32 cell = 4;
        s32 row = 10;

        Func_02001d92(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Func_02001794(72, 160, 12);
}

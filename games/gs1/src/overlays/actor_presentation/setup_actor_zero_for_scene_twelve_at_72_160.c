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

void Func_0200207c(s32, s32, s32);
void Func_02002a98(const void *, s32, s32);
void Func_02002abe(s32, s32, s32, s32, s32, s32);
struct SceneActor *Func_02002b24(s32);
void Func_02002c16(s32);

#define ActorPresentation_SetupActorZeroForSceneTwelveAt72_160 Func_0200113c

void ActorPresentation_SetupActorZeroForSceneTwelveAt72_160(void)
{
    struct SceneActor *actor = Func_02002b24(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Func_02002c16(158);
    Func_02002a98((const void *)0x0200a8ac, 35, 9);
    {
        s32 cell = 4;
        s32 row = 10;

        Func_02002abe(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Func_0200207c(72, 160, 12);
}

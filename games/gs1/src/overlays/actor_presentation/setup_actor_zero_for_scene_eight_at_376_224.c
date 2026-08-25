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

void Func_02001ef6(s32, s32, s32);
void Func_02002910(const void *, s32, s32);
void Func_02002936(s32, s32, s32, s32, s32, s32);
struct SceneActor *Func_0200299c(s32);
void Func_02002a8e(s32);

#define ActorPresentation_SetupActorZeroForSceneEightAt376_224 Func_02000fb4

void ActorPresentation_SetupActorZeroForSceneEightAt376_224(void)
{
    struct SceneActor *actor = Func_0200299c(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Func_02002a8e(158);
    Func_02002910((const void *)0x0200a86a, 54, 13);
    {
        s32 cell = 23;
        s32 row = 12;

        Func_02002936(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Func_02001ef6(376, 224, 8);
}

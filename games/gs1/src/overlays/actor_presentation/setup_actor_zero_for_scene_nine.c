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

void Func_0200166a(s32, s32, s32);
void Func_02001c40(const void *, s32, s32);
void Func_02001c66(s32, s32, s32, s32, s32, s32);
struct SceneActor *Func_02001ca4(s32);
void Func_02001d5e(s32);

#define ActorPresentation_SetupActorZeroForSceneNine Func_02000b9c

void ActorPresentation_SetupActorZeroForSceneNine(void)
{
    struct SceneActor *actor = Func_02001ca4(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Func_02001d5e(158);
    Func_02001c40((const void *)0x02009798, 49, 10);
    {
        s32 cell = 18;
        s32 row = 10;

        Func_02001c66(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Func_0200166a(296, 176, 9);
}

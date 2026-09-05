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

void Func_0200160e(s32, s32, s32);
void Func_02001be4(const void *, s32, s32);
void Func_02001c0a(s32, s32, s32, s32, s32, s32);
struct SceneActor *Func_02001c48(s32);
void Func_02001d02(s32);

void ActorPresentation_SetupActorZeroForSceneEight(void)
{
    struct SceneActor *actor = Func_02001c48(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Func_02001d02(158);
    Func_02001be4((const void *)0x02009782, 54, 13);
    {
        s32 cell = 23;
        s32 row = 12;

        Func_02001c0a(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    Func_0200160e(376, 224, 8);
}

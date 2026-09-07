#include "types.h"

struct Presentation {
    u8 unk_00[9];
    u8 flags;
};

struct SceneActor {
    u8 unk_00[35];
    u8 state_23;
    u8 unk_24[44];
    struct Presentation *presentation;
};

void Func_02001794(s32, s32, s32);
void Func_02001d6c(const void *, s32, s32);
void Func_02001d92(s32, s32, s32, s32, s32, s32);
struct SceneActor *Func_02001dd0(s32);
void Func_02001e8a(s32);


#define ActorPresentation_SetupActorSceneCells Func_02000c1c
struct SceneActor *Func_02001d28(s32);
void Func_02001de4(s32);
void Func_02001cd6(s32, s32, s32, s32, s32, s32);
void Func_02001ce6(s32, s32, s32, s32, s32, s32);
void Func_02001d3c(s32);
void Func_02001cfc(s32, s32, s32, s32, s32, s32);
void Func_02001d0c(s32, s32, s32, s32, s32, s32);
void Func_02001d62(s32);
void Func_02001d30(s32, s32, s32, s32, s32, s32);
void Func_02001738(s32, s32, s32);

void ActorPresentation_SetupActorSceneCells(void)
{
    struct SceneActor *actor = Func_02001d28(0);
    struct Presentation *record = actor->presentation;
    u8 flags;

    Func_02001de4(188);
    Func_02001cd6(42, 33, 34, 16, 2, 2);
    Func_02001ce6(42, 35, 36, 16, 2, 2);
    Func_02001d3c(4);
    Func_02001cfc(40, 33, 34, 16, 2, 2);
    Func_02001d0c(40, 35, 36, 16, 2, 2);
    Func_02001d62(4);
    {
        s32 mode = 3;
        s32 value = 16;
        Func_02001d30(33, 21, 2, 2, mode, value);
    }
    actor->state_23 &= ~1;
    flags = record->flags;
    flags |= 12;
    record->flags = flags;
    Func_02001738(64, 272, 11);
}

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

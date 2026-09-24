#include "TYPES.H"
#include "FIELD_EVENT.H"

struct Workspace {
    u8 unknown_000[356];
    struct FieldActor actor;
};

extern struct Workspace *gWorkspace;
extern const u16 TorebiIzumi_AlphaSteps[];

/* Lifts the workspace actor sixteen pixels with a cue, then steps the blend alpha through its eight-entry table. */
void TorebiIzumi_RiseAndFadeIn(void)
{
    u8 *p = (u8 *)gWorkspace;
    s32 i;

    Engine_AudioPlayCue(216);
    p += 356;
    for (i = 0; i < 16; i++) {
        ((struct FieldActor *)p)->y.fixed -= 0x10000;
        Engine_TaskWait(4);
    }
    p = (u8 *)TorebiIzumi_AlphaSteps;
    for (i = 0; i < 8; i++) {
        {
            s32 mode = 0x3f42;

            *(volatile u16 *)0x04000050 = mode;
        }
        *(volatile u16 *)0x04000052 = *(u16 *)p;
        p += 2;
        /* FAKEMATCH: the empty do/while keeps the step pointer's advance ahead of the wait's argument. */
        do { } while (0);
        Engine_TaskWait(8);
    }
}

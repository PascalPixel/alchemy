/* NONMATCHING: 120 of 120 bytes, 3 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: 3 halfwords: the reference advances the alpha pointer between loading and storing the step, and sets r0 = 8 after the store */
#include "TYPES.H"
#include "FIELD_EVENT.H"

struct Workspace {
    u8 unknown_000[356];
    struct FieldActor actor;
};

extern struct Workspace *gWorkspace;
extern const u16 TorebiIzumi_AlphaSteps[];

void Local_02000880(void)
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
        Engine_TaskWait(8);
    }
}

#include "TYPES.H"

void Engine_EventBegin();
void Engine_AudioPlayCue();
u8 *Engine_ActorGet();
void Engine_ActorSetSpeed();
void Engine_ActorSetDestinationOffset();
void Engine_EventRequestExit();
void Engine_EventEnd();
void Engine_MapAnimateCells();
void FieldScene_ConfigureActor0ThenRun();

/* Call sites spelled through this wrapper pass their constants straight
 * into the argument registers. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Animate the map cells for the chosen switch, or leave the area. */
void KorimaMura_Func02000304(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    s32 x = 0;
    s32 y = 0;

    Engine_EventBegin();
    Engine_AudioPlayCue(158);
    switch (*(s16 *)(work + 0x16c)) {
    case 5:
        x = 71;
        y = 9;
        break;
    case 6:
        x = 73;
        y = 17;
        break;
    case 7:
        x = 80;
        y = 21;
        break;
    case 8:
        x = 84;
        y = 12;
        break;
    case 9:
        Engine_ActorGet(0)[85] = 0;
        Call3(Engine_ActorSetSpeed, 0, 0x8000, 0x4000);
        Engine_ActorSetDestinationOffset(0, 0, 8);
        *(s32 *)(*(u8 **)0x03001ebc + 0x1c8) = 16;
        Engine_EventRequestExit(9);
        Engine_EventEnd();
        return;
    }
    Engine_MapAnimateCells(0x200ae48, x, y);
    FieldScene_ConfigureActor0ThenRun(*(s16 *)(work + 0x16c));
    Engine_EventEnd();
}

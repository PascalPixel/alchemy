#include "TYPES.H"
#include "CALL.H"

extern u8 *gWork;

/* The map cell steps the switch plays, laid out after the code. */
extern u8 KorimaMura_SwitchCells[];

void Engine_EventBegin();
void Engine_AudioPlayCue();
u8 *Engine_ActorGet();
void Engine_ActorSetSpeed();
void Engine_ActorSetDestinationOffset();
void Engine_EventRequestExit();
void Engine_EventEnd();
void Engine_MapAnimateCells();
void FieldScene_ConfigureActor0ThenRun();

/* Animate the map cells for the chosen switch, or leave the area. */
void KorimaMura_AnimateSwitchOrExit(void)
{
    u8 *work = gWork;
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
        *(s32 *)(gWork + 0x1c8) = 16;
        Engine_EventRequestExit(9);
        Engine_EventEnd();
        return;
    }
    Engine_MapAnimateCells(KorimaMura_SwitchCells, x, y);
    FieldScene_ConfigureActor0ThenRun(*(s16 *)(work + 0x16c));
    Engine_EventEnd();
}

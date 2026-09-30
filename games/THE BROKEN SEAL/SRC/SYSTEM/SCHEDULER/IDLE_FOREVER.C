#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE.H"
#include "SYSTEM.H"
#include "IO_REG.H"

extern volatile u32 gKeyState;

void Runtime_IdleForever(void)
{
    for (;;) {
        (void)gKeyState;
        WaitFrames(1);
    }
}

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
/* The Spanish and Italian editions ask the cartridge which language it
   speaks: the last letter of the header's game code. German is 1, French 2,
   Spanish 3 and Italian 4; anything else is 0. */
s32 System_GetLanguage(void)
{
    s32 language;

    switch (ROM_GAME_CODE[3]) {
    case 'D':
        language = 1;
        break;
    case 'F':
        language = 2;
        break;
    case 'S':
        language = 3;
        break;
    case 'I':
        language = 4;
        break;
    default:
        language = 0;
        break;
    }
    return language;
}
#endif

void RuntimeDispatch_ReservedNoOpA(void)
{
}

void RuntimeDispatch_ReservedNoOpB(void)
{
}

void RuntimeDispatch_ReservedNoOpC(void)
{
}

void RuntimeDispatch_ReservedNoOpD(void)
{
}

void RuntimeDispatch_ReservedNoOpE(void)
{
}

s32 RuntimeDispatch_ReturnZero(void)
{
    return 0;
}

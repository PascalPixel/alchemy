#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE.H"
#include "SYSTEM.H"

extern volatile u32 gKeyState;

void Runtime_IdleForever(void)
{
    for (;;) {
        (void)gKeyState;
        WaitFrames(1);
    }
}

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)

s32 System_GetLanguage(void)
{
    s32 language;

    switch (Rom_LanguageCode) {
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

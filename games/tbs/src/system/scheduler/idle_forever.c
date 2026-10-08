#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene.h"
#include "resource.h"
#include "system.h"

void Runtime_IdleForever(void)
{
    for (;;) {
        (void)*(volatile u32 *)&gKeyState;
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

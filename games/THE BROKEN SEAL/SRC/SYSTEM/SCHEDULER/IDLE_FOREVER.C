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
/* The last letter of the cartridge header's four-letter game code, which
   names the release's language. */
extern const u8 Rom_LanguageCode;

/* The Spanish and Italian editions ask the cartridge which language it
   speaks: the last letter of the header's game code. German is 1, French 2,
   Spanish 3 and Italian 4; anything else is 0. */
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

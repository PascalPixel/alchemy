#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EDITION.H"
#include "TYPES.H"

#if !EDITION_INTERNATIONAL
#define PAIR_COLUMN 7
#elif defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define PAIR_COLUMN 10
#else
#define PAIR_COLUMN 9
#endif
s32 Menu_CenterResourceEntries(s32, s32, s32);

s32 Menu_SelectEntry20To21(s32 arg0)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(32);
    Menu_AppendResourceEntry(33);
    Menu_CenterResourceEntries(17, PAIR_COLUMN, 0);
    ret = Menu_RunResourceSelectionLoop(arg0);
    Menu_EndResourceSelection();
    return ret;
}

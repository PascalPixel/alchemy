#include "EDITION.H"
#include "TYPES.H"

/* The column that centres the pair: each edition's words set its width. */
#if !EDITION_INTERNATIONAL
#define PAIR_COLUMN 7
#elif defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT) || defined(TLA_EDITION_FR) || defined(TLA_EDITION_IT)
#define PAIR_COLUMN 10
#else
#define PAIR_COLUMN 9
#endif
void *AffineEffect_InitializeWork(void);
void Menu_EndResourceSelection(void);
s32 Menu_RunResourceSelectionLoop(s32);
void Menu_AppendResourceEntry(s32 arg0);
s32 Menu_CenterResourceEntries(s32, s32, s32);

s32 Menu_SelectEntry20To21(s32 arg0)
{
    s32 ret;

    AffineEffect_InitializeWork();
    Menu_AppendResourceEntry(0x20);
    Menu_AppendResourceEntry(0x21);
    Menu_CenterResourceEntries(0x11, PAIR_COLUMN, 0);
    ret = Menu_RunResourceSelectionLoop(arg0);
    Menu_EndResourceSelection();
    return ret;
}

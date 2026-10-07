#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "GLYPH.H"


s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2)
{
    GlyphTransfer *work;

    work = Runtime_AllocateHeapBlock(68, sizeof(GlyphTransfer));
    ItemIcon_Compose(arg0, arg1);
    VramBlock_LoadCached(arg2, 128, work->tiles);
    Runtime_ReleaseHeapBlock(68);
    return 1;
}

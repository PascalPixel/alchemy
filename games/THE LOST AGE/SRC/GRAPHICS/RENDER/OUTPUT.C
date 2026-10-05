#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RENDER_INPUT.H"
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "RESOURCE.H"

struct RenderOutput *RenderOutput_CreateWithTransform(
    s32 arg0,
    struct RenderInput *arg1,
    s32 arg2,
    s32 arg3)
{
    s32 count;
    s32 unused;
    struct RenderOutput *result;

    count = Resource_FindFreeEntry();
    if (count == 0x60) {
        return NULL;
    }
    Ui_BuildPairedPatternsToSlot(arg0, 1, &count, &unused, 1);
    result = RenderOutput_Create(count, 0x40000000, arg1, arg2, arg3);
    result->sentinel = 251;
    return result;
}


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


struct RenderOutput *RenderOutput_CreateFromResource(
    s32 arg0,
    s32 arg1,
    struct RenderInput *arg2,
    s32 arg3,
    s32 arg4)
{
    s32 no;

    no = Resource_LoadByMode(arg0, arg1);
    if (no < 0) {
        return NULL;
    }
    return RenderOutput_Create(no, 0x40000000, arg2, arg3, arg4);
}

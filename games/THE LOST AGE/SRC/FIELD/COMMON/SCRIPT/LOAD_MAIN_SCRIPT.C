#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SCRIPT.H"


s32 Script_LoadMainScript(struct ScriptInterpreter *interpreter)
{
    s32 result;

    interpreter->script = Script_MainScript;
    interpreter->cursor = (result = 0);
    return result;
}

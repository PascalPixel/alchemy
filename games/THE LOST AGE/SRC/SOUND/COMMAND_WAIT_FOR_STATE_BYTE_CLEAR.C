#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "SYSTEM.H"

void AudioCommand_WaitForStateByteClear(void)
{
    while (Audio_Check() != 0) {
        WaitFrames(1);
    }
}

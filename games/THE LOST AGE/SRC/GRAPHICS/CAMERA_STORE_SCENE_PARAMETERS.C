#include "BATTLE.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
void Camera_StoreSceneParameters(u32 value0, u32 value1, u32 value2)
{
    u32 *work = gCameraSceneParameters;
    work[0] = value0;
    work[1] = value1;
    work[2] = value2;
}

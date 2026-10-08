#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
void Camera_StoreSceneParameters(u32 value0, u32 value1, u32 value2)
{
    u32 *work = gCameraSceneParameters;
    work[0] = value0;
    work[1] = value1;
    work[2] = value2;
}

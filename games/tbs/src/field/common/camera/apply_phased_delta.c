#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "battle_effect_work.h"
#include "btldisp.h"
#include "types.h"
#include "scene.h"

#define gBattleFxWork (*(struct BattleEffectWork * *)&gBattleFxWork)


void Camera_ApplyPhasedDelta(void)
{
    struct BattleEffectWork *work = gBattleFxWork;
    struct BattleCamera *camera = *(struct BattleCamera **)((u8 *)&gBattleFxWork - 108);
    s32 *phase = &work->camera_phase;

    if (*phase == 1) {
        camera->yaw += work->camera_delta;
        *phase = 0;
    } else {
        camera->yaw += work->camera_delta / 2;
        if (*phase == 2)
            *phase = 0;
        else
            *phase = 2;
    }
}

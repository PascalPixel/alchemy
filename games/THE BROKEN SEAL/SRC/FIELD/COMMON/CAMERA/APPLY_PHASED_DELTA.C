#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "TYPES.H"
#include "SCENE.H"

extern struct BattleEffectWork *gBattleFxWork;

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

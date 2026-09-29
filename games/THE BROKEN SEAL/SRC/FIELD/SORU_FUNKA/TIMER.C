/* The random timer level. */
#include "FUNKA.H"

void SceneState_UpdateRandomTimerLevel(void)
{
    u32 v;

    if (gEmberLevelTimer != 0) {
        gEmberLevelTimer--;
        return;
    }
    if (gEmberLevel != 0) {
        gEmberLevel--;
    } else {
        gEmberLevel = (u32)(Random_Next() << 2) >> 16;
    }
    v = gEmberLevel;
    switch (v) {
    case 3:
        gEmberMask = v;
        gEmberLevelTimer = ((u32)(Random_Next() * 20) >> 16) + 40;
        break;
    case 2:
        gEmberMask = 15;
        gEmberLevelTimer = ((u32)(Random_Next() * 40) >> 16) + 80;
        break;
    case 1:
        gEmberMask = 63;
        gEmberLevelTimer = ((u32)(Random_Next() * 80) >> 16) + 160;
        break;
    default:
        gEmberMask = 127;
        gEmberLevelTimer = ((u32)(Random_Next() * 160) >> 16) + 320;
        break;
    }
}

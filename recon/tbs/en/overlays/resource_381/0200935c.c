/* Draft of resource_381 0x0200935c (SceneState_UpdateRandomTimerLevel and what follows it in this file),
 * from games/THE BROKEN SEAL/SRC/FIELD/SORU_FUNKA (FUNKA.H). Remaining
 * difference: it reads and writes the scene's variables that lie past the
 * overlay image (0x0200bac0 and on), which no source defines, so it cannot
 * link by name. The listing keeps these rows. */
/* The random timer level. */
#include "FUNKA.H"

void SceneState_UpdateRandomTimerLevel(void)
{
    u32 v;

    if (Data_0200bb70 != 0) {
        Data_0200bb70--;
        return;
    }
    if (Data_0200bb6c != 0) {
        Data_0200bb6c--;
    } else {
        Data_0200bb6c = (u32)(Random_Next() << 2) >> 16;
    }
    v = Data_0200bb6c;
    switch (v) {
    case 3:
        Data_0200bb68 = v;
        Data_0200bb70 = ((u32)(Random_Next() * 20) >> 16) + 40;
        break;
    case 2:
        Data_0200bb68 = 15;
        Data_0200bb70 = ((u32)(Random_Next() * 40) >> 16) + 80;
        break;
    case 1:
        Data_0200bb68 = 63;
        Data_0200bb70 = ((u32)(Random_Next() * 80) >> 16) + 160;
        break;
    default:
        Data_0200bb68 = 127;
        Data_0200bb70 = ((u32)(Random_Next() * 160) >> 16) + 320;
        break;
    }
}

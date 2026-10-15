#include "lemurian_ship.h"

void Func_020027e4(void)
{
    if (GameFlag_Test(0x983)) {
        LemurianShip_MapCopy(17, 125, 27, 63, 3, 3);
        LemurianShip_BlendStart(25, 0, 5, 3, 25, 4);
    }
}

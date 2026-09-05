#include "psynergy_menu.h"
#include "global_cells.h"

u8 *Ability_GetData(s32 action);
s32 Func_0808a488(u8 effect);

s32 PsynergyMenu_ClassifySelectedPsynergy(void)
{
    u8 *psynergy;
    s32 diff;
    s32 ret;

    psynergy = Ability_GetData(
        (s32)(0x3fff &
              (*(struct PsynergyMenuState **)ADDR_03001F2C)
                  ->selected_psynergy));
    if (Func_0808a488(psynergy[0x0c]) != 0) {
        return 0;
    }
    ret = 2;
    if (psynergy[8] != 0xff) {
        u8 kind = psynergy[0];
        diff = kind ^ 2;
        ret = (0 - diff) | diff;
        ret = (s32)((u32)ret >> 0x1f);
        ret = 1 - ret;
    }
    return ret;
}

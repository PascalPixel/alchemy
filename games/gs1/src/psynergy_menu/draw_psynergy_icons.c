#include "psynergy_menu.h"
#include "global_cells.h"

void Func_08015298(s32 style, u16 action, u8 target, s32 flags);

void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies)
{
    s32 remaining;
    struct PsynergyMenuIcon **icons;
    u16 *p;
    s32 psynergy_id;

    icons =
        (*(struct PsynergyMenuState **)ADDR_03001F2C)->entry_icons;
    p = psynergies;
    remaining = 31;
    do {
        psynergy_id = *p++;
        if (psynergy_id != 0) {
            Func_08015298(
                4, psynergy_id, (*icons)->render_target, 0);
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(psynergies);
}

#include "scene.h"
#include "abi/psynergy_menu/draw_psynergy_icons.h"
#include "psynergy_menu.h"
#include "global_cells.h"

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
            Sys_SetMode(
                4, psynergy_id, (*icons)->render_target, 0);
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(psynergies);
}

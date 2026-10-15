#include "types.h"
#include "ram_buffer.h"

struct PsynergyEntryIcon
{
    u8 unk00[14];
    u8 slot;
};

struct PsynergyEntryMenu
{
    u8                        unk000[0x4c];
    struct PsynergyEntryIcon *entryIcons[32];
};

void Func_08038288(s32 mode, s32 id, s32 slot, s32 flags);
void Menu_HideEmptyEntryIcons(const u16 *items);

void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies)
{
    s32                        remaining;
    struct PsynergyEntryIcon **icons;
    u16                       *p;
    s32                        psynergyId;

    icons = ((struct PsynergyEntryMenu *)Ram_HeapSlots->menuRuntime)->entryIcons;
    p = psynergies;
    remaining = 31;
    do
    {
        psynergyId = *p++;
        if (psynergyId != 0)
        {
            Func_08038288(4, psynergyId, (*icons)->slot, 0);
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(psynergies);
}
